/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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

#include "nrf_802154_sl_rsch.h"

#include "nrf_802154_assert.h"
#include <stddef.h>
#include <string.h>
#include <nrfx.h>

#include "rsch/nrf_802154_rsch.h"
#include "platform/nrf_802154_clock.h"

static rsch_prio_t m_prev_prio;
static bool        m_ready;

/**
 * @brief Notifies the core that the approved RSCH priority has changed.
 *
 * @note This function is called from the critical section context and does not preempt
 *       other critical sections.
 *
 * @param[in]  prio  Approved priority level.
 */
extern void nrf_802154_rsch_crit_sect_prio_changed(rsch_prio_t prio);

/***************************************************************************************************
 * Public API
 **************************************************************************************************/

void nrf_802154_rsch_init(void)
{
    m_ready     = false;
    m_prev_prio = RSCH_PRIO_IDLE;
}

void nrf_802154_rsch_uninit(void)
{
    // Intenionally empty
}

void nrf_802154_rsch_continuous_ended(void)
{
    // Intentionally empty
}

bool nrf_802154_rsch_timeslot_request(uint32_t length_us)
{
    (void)length_us;

    NRF_802154_ASSERT(m_ready);

    return true;
}

bool nrf_802154_rsch_timeslot_is_requested(void)
{
    return false;
}

bool nrf_802154_rsch_prec_is_approved(rsch_prec_t prec, rsch_prio_t prio)
{
    return prio == RSCH_PRIO_IDLE ? true : m_ready;
}

uint32_t nrf_802154_rsch_timeslot_us_left_get(void)
{
    return UINT32_MAX;
}

void nrf_802154_clock_hfclk_ready(void)
{
    m_ready = true;
    nrf_802154_rsch_crit_sect_prio_changed(RSCH_PRIO_MAX);
}

void nrf_802154_rsch_crit_sect_prio_request(rsch_prio_t prio)
{
    if (m_prev_prio != prio)
    {
        if (prio == RSCH_PRIO_IDLE)
        {
            nrf_802154_clock_hfclk_stop();

            m_ready = false;

            nrf_802154_rsch_crit_sect_prio_changed(RSCH_PRIO_IDLE);
        }
        else if (m_prev_prio == RSCH_PRIO_IDLE)
        {
            NRF_802154_ASSERT(!m_ready);

            nrf_802154_clock_hfclk_start();
        }
        else
        {
            // Intentionally empty
        }

        m_prev_prio = prio;
    }
}

void nrf_802154_rsch_crit_sect_init(void)
{
    // Intentionally empty
}

void nrf_802154_critical_section_rsch_enter(void)
{
    // Intentionally empty
}

void nrf_802154_critical_section_rsch_exit(void)
{
    // Intentionally empty
}

bool nrf_802154_critical_section_rsch_event_is_pending(void)
{
    return false;
}

void nrf_802154_critical_section_rsch_process_pending(void)
{
    // Intentionally empty
}

bool nrf_802154_rsch_delayed_timeslot_request(const rsch_dly_ts_param_t * p_dly_ts_param)
{
    (void)p_dly_ts_param;

    return false;
}

bool nrf_802154_rsch_delayed_timeslot_cancel(rsch_dly_ts_id_t dly_ts_id, bool handler)
{
    (void)dly_ts_id;
    (void)handler;

    return false;
}

bool nrf_802154_rsch_delayed_timeslot_priority_update(rsch_dly_ts_id_t dly_ts_id,
                                                      rsch_prio_t      dly_ts_prio)
{
    (void)dly_ts_id;
    (void)dly_ts_prio;

    return false;
}

bool nrf_802154_rsch_delayed_timeslot_ppi_update(uint32_t ppi_channel)
{
    (void)ppi_channel;

    return false;
}

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
uint32_t nrf_802154_rsch_delayed_timeslot_time_to_hw_trigger_get(void)
{
    return 0;
}

#endif

#ifdef NRF54L_SERIES

void nrf_802154_clock_hfclk_latency_set(uint32_t latency_us)
{
    /* Intentionally empty. */
    (void)latency_us;
}

#endif
