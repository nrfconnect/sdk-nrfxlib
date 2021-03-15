/*
 * Copyright (c) 2019 - 2021, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @file
 *   This file implements Long/Short Interframe Spacing handling procedure for the 802.15.4 driver.
 *
 */

#include "nrf_802154_ifs.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "nrf_802154_pib.h"
#include "nrf_802154_request.h"
#include "mac_features/nrf_802154_frame_parser.h"
#include "timer/nrf_802154_timer_sched.h"

#if NRF_802154_IFS_ENABLED
typedef struct
{
    uint8_t * p_data;
    bool      cca;
} ifs_operation_t;

static union
{
    uint8_t sh[SHORT_ADDRESS_SIZE];                   ///< Short address of the last frame transmitted.
    uint8_t ext[EXTENDED_ADDRESS_SIZE];               ///< Extended address of the last frame transmitted.
} m_last_address;

static bool               m_is_last_address_extended; ///< Whether the last transmitted frame had the extended address populated.
static uint32_t           m_last_frame_timestamp;     ///< Timestamp of the last transmitted frame (end of frame).
static uint8_t            m_last_frame_length;        ///< Length in bytes of the last transmitted frame.
static ifs_operation_t    m_context;                  ///< Context passed to the timer.
static nrf_802154_timer_t m_timer;                    ///< Interframe space timer.

static void ifs_tx_result_notify(bool result)
{
    if (!result)
    {
        nrf_802154_notify_transmit_failed(m_context.p_data, NRF_802154_TX_ERROR_TIMESLOT_DENIED);
    }
}

static void callback_fired(void * p_context)
{
    ifs_operation_t * p_ctx = (ifs_operation_t *)p_context;

    nrf_802154_request_transmit(NRF_802154_TERM_NONE,
                                REQ_ORIG_IFS,
                                p_ctx->p_data,
                                p_ctx->cca,
                                true,
                                ifs_tx_result_notify);
}

/**@brief Checks if the IFS is needed by comparing the addresses of the actual and the last frames. */
static bool is_ifs_needed_by_address(const uint8_t * p_frame)
{
    bool is_extended;

    const uint8_t * addr = nrf_802154_frame_parser_dst_addr_get(p_frame, &is_extended);

    if (!addr)
    {
        return true;
    }

    if (is_extended == m_is_last_address_extended)
    {
        uint8_t * last_addr = is_extended ? m_last_address.ext : m_last_address.sh;
        size_t    addr_len  = is_extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE;

        if (0 == memcmp(addr, last_addr, addr_len))
        {
            return true;
        }
    }

    return false;
}

/**@brief Checks if the IFS is needed by measuring time between the actual and the last frames.
 *        Returns the needed ifs, 0 if none.
 */
static uint16_t ifs_needed_by_time(uint32_t current_timestamp)
{
    if (!nrf_802154_timer_sched_time_is_in_future(m_last_frame_timestamp, 0, current_timestamp))
    {
        /* Explicitly allow case where the timstamps are equal, i.e. we are running very fast. */
        if (current_timestamp != m_last_frame_timestamp)
        {
            return 0;
        }
    }
    uint16_t ifs_period;
    uint32_t dt = current_timestamp - m_last_frame_timestamp;

    if (m_last_frame_length > MAX_SIFS_FRAME_SIZE)
    {
        ifs_period = nrf_802154_pib_ifs_min_lifs_period_get();
    }
    else
    {
        ifs_period = nrf_802154_pib_ifs_min_sifs_period_get();
    }

    if (dt > ifs_period)
    {
        return 0;
    }

    return ifs_period;
}

bool nrf_802154_ifs_pretransmission(const uint8_t * p_frame, bool cca)
{
    nrf_802154_ifs_mode_t mode = nrf_802154_pib_ifs_mode_get();

    if (mode == NRF_802154_IFS_MODE_DISABLED)
    {
        // Functionality is disabled - skip the routine.
        return true;
    }

    if (!m_last_frame_length)
    {
        // No frame was transmitted before - skip the routine.
        return true;
    }

    if ((mode == NRF_802154_IFS_MODE_MATCHING_ADDRESSES) && !is_ifs_needed_by_address(p_frame))
    {
        return true;
    }

    uint32_t current_timestamp = nrf_802154_timer_sched_time_get();
    uint32_t dt                = ifs_needed_by_time(current_timestamp);

    if (dt == 0)
    {
        return true;
    }

    m_context.p_data  = (uint8_t *)p_frame;
    m_context.cca     = cca;
    m_timer.t0        = m_last_frame_timestamp;
    m_timer.dt        = dt;
    m_timer.callback  = callback_fired;
    m_timer.p_context = &m_context;

    nrf_802154_timer_sched_add(&m_timer, true);

    return false;
}

void nrf_802154_ifs_transmitted_hook(const uint8_t * p_frame)
{
    assert(p_frame[0] != 0U);

    m_last_frame_timestamp = nrf_802154_timer_sched_time_get();

    const uint8_t * addr =
        nrf_802154_frame_parser_dst_addr_get(p_frame, &m_is_last_address_extended);

    if (!addr)
    {
        // If the transmitted frame has no address, we consider that enough time has passed so no IFS insertion will be needed.
        m_last_frame_length = 0;
        return;
    }

    if (m_is_last_address_extended)
    {
        memcpy(m_last_address.ext, addr, EXTENDED_ADDRESS_SIZE);
    }
    else
    {
        memcpy(m_last_address.sh, addr, SHORT_ADDRESS_SIZE);
    }

    m_last_frame_length = p_frame[0];
}

bool nrf_802154_ifs_abort(nrf_802154_term_t term_lvl, req_originator_t req_orig)
{
    bool result      = true;
    bool was_running = false;

    if (req_orig == REQ_ORIG_IFS)
    {
        // Ignore if self-request.
    }
    else
    {
        if (term_lvl >= NRF_802154_TERM_802154)
        {
            nrf_802154_timer_sched_remove(&m_timer, &was_running);
            if (was_running)
            {
                ifs_operation_t * p_op = (ifs_operation_t *)m_timer.p_context;

                nrf_802154_notify_transmit_failed(p_op->p_data, NRF_802154_TX_ERROR_ABORTED);
            }
        }
        else
        {
            result = !nrf_802154_timer_sched_is_running(&m_timer);
        }
    }

    return result;
}

#endif // NRF_802154_IFS_ENABLED
