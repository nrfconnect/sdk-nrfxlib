/*
 * Copyright (c) 2019, Nordic Semiconductor ASA
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

#include "nrf_802154_assert.h"
#include <stdint.h>
#include <string.h>

#include "nrf_802154_pib.h"
#include "nrf_802154_request.h"
#include "mac_features/nrf_802154_frame_parser.h"
#include "nrf_802154_sl_timer.h"
#include "nrf_802154_sl_utils.h"
#include "nrf_802154_sl_atomics.h"

#if NRF_802154_IFS_ENABLED

/**
 * @brief States of IFS handling procedure.
 */
typedef enum
{
    IFS_STATE_STOPPED  = (1U << 0), ///< IFS procedure stopped.
    IFS_STATE_ARMED    = (1U << 1), ///< The @c m_timer is scheduled and waiting to fire.
    IFS_STATE_FIRED    = (1U << 2), ///< The @c m_timer fired and associated callback is in progress.
    IFS_STATE_ABORTING = (1U << 3)  ///< Abort request occurred. Resource release and notification are ongoing.
} ifs_state_t;

#define IFS_STATE_MASK ((ifs_state_t)(IFS_STATE_STOPPED | \
                                      IFS_STATE_ARMED |   \
                                      IFS_STATE_FIRED |   \
                                      IFS_STATE_ABORTING))

static ifs_state_t m_state;

typedef struct
{
    uint8_t                    * p_data;
    nrf_802154_transmit_params_t params;
} ifs_operation_t;

static union
{
    uint8_t sh[SHORT_ADDRESS_SIZE];                      ///< Short address of the last frame transmitted.
    uint8_t ext[EXTENDED_ADDRESS_SIZE];                  ///< Extended address of the last frame transmitted.
} m_last_address;

static bool                  m_is_last_address_extended; ///< Whether the last transmitted frame had the extended address populated.
static uint64_t              m_last_frame_timestamp;     ///< Timestamp of the last transmitted frame (end of frame).
static uint8_t               m_last_frame_length;        ///< Length in bytes of the last transmitted frame.
static ifs_operation_t       m_context;                  ///< Context passed to the timer.
static nrf_802154_sl_timer_t m_timer;                    ///< Interframe space timer.

/**
 * Set state of IFS procedure.
 *
 * @param[in]  expected_state  Expected IFS state prior state transition.
 * @param[in]  new_state       IFS procedure state to enter.
 *
 * @retval true   Successfully set the new state.
 * @retval false  Failed to set the new state.
 */
static bool ifs_state_set(ifs_state_t expected_state, ifs_state_t new_state)
{
    return nrf_802154_sl_atomic_cas_u8(&m_state, &expected_state, new_state);
}

static bool ifs_state_is(ifs_state_t expected_state_mask)
{
    return ((m_state & expected_state_mask) != 0);
}

static void ifs_tx_result_notify(bool result)
{
    if (!result)
    {
        nrf_802154_transmit_done_metadata_t metadata = {};

        metadata.frame_props = m_context.params.frame_props;
        nrf_802154_notify_transmit_failed(m_context.p_data,
                                          NRF_802154_TX_ERROR_TIMESLOT_DENIED,
                                          &metadata);
    }
}

static void callback_fired(nrf_802154_sl_timer_t * p_timer)
{
    ifs_operation_t * p_ctx = (ifs_operation_t *)p_timer->user_data.p_pointer;

    if (ifs_state_set(IFS_STATE_ARMED, IFS_STATE_FIRED))
    {
        nrf_802154_request_transmit(NRF_802154_TERM_NONE,
                                    REQ_ORIG_IFS,
                                    p_ctx->p_data,
                                    &p_ctx->params,
                                    ifs_tx_result_notify);

        ifs_state_set(IFS_STATE_FIRED, IFS_STATE_STOPPED);
    }
}

/**@brief Checks if the IFS is needed by comparing the addresses of the actual and the last frames. */
static bool is_ifs_needed_by_address(const uint8_t * p_frame)
{
    nrf_802154_frame_parser_data_t frame_data;
    const uint8_t                * addr;
    bool                           is_extended;

    bool result = nrf_802154_frame_parser_data_init(p_frame,
                                                    p_frame[PHR_OFFSET] + PHR_SIZE,
                                                    PARSE_LEVEL_ADDRESSING_END,
                                                    &frame_data);

    if (result)
    {
        addr        = nrf_802154_frame_parser_dst_addr_get(&frame_data);
        is_extended = nrf_802154_frame_parser_dst_addr_is_extended(&frame_data);
    }
    else
    {
        addr        = NULL;
        is_extended = false;
    }

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
static uint16_t ifs_needed_by_time(uint64_t current_timestamp)
{
    if (!nrf_802154_sl_time64_is_in_future(m_last_frame_timestamp, current_timestamp))
    {
        /* Explicitly allow case where the timstamps are equal, i.e. we are running very fast. */
        if (current_timestamp != m_last_frame_timestamp)
        {
            return 0;
        }
    }
    uint16_t ifs_period;
    uint64_t dt = current_timestamp - m_last_frame_timestamp;

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

void nrf_802154_ifs_init(void)
{
    m_state                    = IFS_STATE_STOPPED;
    m_is_last_address_extended = false;
    m_last_frame_timestamp     = 0;
    m_last_frame_length        = 0;
    m_context                  = (ifs_operation_t){ .p_data = NULL };

    nrf_802154_sl_timer_init(&m_timer);
}

void nrf_802154_ifs_deinit(void)
{
    nrf_802154_sl_timer_deinit(&m_timer);
}

bool nrf_802154_ifs_pretransmission(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function)
{
    (void)notify_function;

    nrf_802154_ifs_mode_t mode;

    if (p_params->immediate)
    {
        return true;
    }

    mode = nrf_802154_pib_ifs_mode_get();

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

    uint64_t current_timestamp = nrf_802154_sl_timer_current_time_get();
    uint32_t dt                = ifs_needed_by_time(current_timestamp);

    if (dt == 0)
    {
        return true;
    }

    if (!ifs_state_set(IFS_STATE_STOPPED, IFS_STATE_ARMED))
    {
        NRF_802154_ASSERT(false);
    }
    else
    {
        m_context.p_data                 = p_frame;
        m_context.params                 = *p_params;
        m_context.params.immediate       = true;
        m_timer.trigger_time             = m_last_frame_timestamp + dt;
        m_timer.action_type              = NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK;
        m_timer.action.callback.callback = callback_fired;
        m_timer.user_data.p_pointer      = &m_context;

        if (nrf_802154_sl_timer_add(&m_timer) != NRF_802154_SL_TIMER_RET_SUCCESS)
        {
            NRF_802154_ASSERT(false);
        }
    }

    return false;
}

void nrf_802154_ifs_transmitted_hook(const uint8_t * p_frame)
{
    NRF_802154_ASSERT(p_frame[0] != 0U);

    m_last_frame_timestamp = nrf_802154_sl_timer_current_time_get();

    nrf_802154_frame_parser_data_t frame_data;
    const uint8_t                * addr;

    bool result = nrf_802154_frame_parser_data_init(p_frame,
                                                    p_frame[PHR_OFFSET] + PHR_SIZE,
                                                    PARSE_LEVEL_ADDRESSING_END,
                                                    &frame_data);

    if (result)
    {
        addr                       = nrf_802154_frame_parser_dst_addr_get(&frame_data);
        m_is_last_address_extended = nrf_802154_frame_parser_dst_addr_is_extended(&frame_data);
    }
    else
    {
        addr                       = NULL;
        m_is_last_address_extended = false;
    }

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
    bool result = true;

    if (req_orig == REQ_ORIG_IFS)
    {
        // Ignore if self-request.
    }
    else
    {
        if (term_lvl >= NRF_802154_TERM_802154)
        {
            if (ifs_state_set(IFS_STATE_ARMED, IFS_STATE_ABORTING))
            {
                ifs_operation_t * p_op = (ifs_operation_t *)m_timer.user_data.p_pointer;

                // The IFS was still waiting, so the transmission didn't occur
                // at all. Notify with frame_props passed in nrf_802154_ifs_pretransmission hook
                nrf_802154_transmit_done_metadata_t metadata = {};

                metadata.frame_props = m_context.params.frame_props;
                nrf_802154_notify_transmit_failed(p_op->p_data,
                                                  NRF_802154_TX_ERROR_ABORTED,
                                                  &metadata);
            }

            ifs_state_set(IFS_STATE_MASK, IFS_STATE_STOPPED);
        }
        else
        {
            result = !ifs_state_is(IFS_STATE_ARMED | IFS_STATE_FIRED);
        }
    }

    return result;
}

#endif // NRF_802154_IFS_ENABLED
