/*
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
 *   This file implements ACK timeout procedure for the 802.15.4 driver.
 *
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_ACK_TIMEOUT

#include "nrf_802154_ack_timeout.h"

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>

#include "../nrf_802154_debug.h"
#include "nrf_802154_notification.h"
#include "nrf_802154_procedures_duration.h"
#include "nrf_802154_request.h"
#include "nrf_802154_sl_timer.h"

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
#include "nrf_802154_bsim_utils.h"
#endif

#if NRF_802154_ACK_TIMEOUT_ENABLED

#define RETRY_DELAY     500     ///< Procedure is delayed by this time if it cannot be performed at the moment [us].
#define MAX_RETRY_DELAY 1000000 ///< Maximum allowed delay of procedure retry [us].

static void timeout_timer_retry(void);

static uint32_t              m_timeout = NRF_802154_PRECISE_ACK_TIMEOUT_DEFAULT_TIMEOUT; ///< ACK timeout in us.
static nrf_802154_sl_timer_t m_timer;                                                    ///< Timer used to notify when the ACK frame is not received for too long.
static uint32_t              m_dt;                                                       ///< Length [us] of the originally scheduled timeout plus any retry extensions used.
static volatile bool         m_procedure_is_active;
static uint8_t             * mp_frame;

static void timeout_timer_fired(nrf_802154_sl_timer_t * p_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    (void)p_timer;

    if (m_procedure_is_active)
    {
        nrf_802154_ack_timeout_handle_params_t param = {0};

        param.p_frame = mp_frame;

        if (nrf_802154_request_ack_timeout_handle(&param))
        {
            m_procedure_is_active = false;
        }
        else
        {
            timeout_timer_retry();
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void timeout_timer_retry(void)
{
    m_dt += RETRY_DELAY;
    NRF_802154_ASSERT(m_dt <= MAX_RETRY_DELAY);

    m_timer.trigger_time += RETRY_DELAY;

    nrf_802154_sl_timer_ret_t ret;

    ret = nrf_802154_sl_timer_add(&m_timer);
    NRF_802154_ASSERT(ret == NRF_802154_SL_TIMER_RET_SUCCESS);
    (void)ret;
}

static void timeout_timer_start(void)
{
    uint64_t                  now = nrf_802154_sl_timer_current_time_get();
    nrf_802154_sl_timer_ret_t ret;

    m_dt = m_timeout + IMM_ACK_DURATION + nrf_802154_frame_duration_get(mp_frame[0], false, true);
#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    /**
     * In simulation, this function is executed immediately after setting up Tx ramp-up instead of
     * handler of RADIO.ADDRESS event. The timeout must be increased with simulation-specific
     * adjustments calculated earlier.
     */
    nrf_802154_bsim_utils_core_hooks_adjustments_t adjustments;

    nrf_802154_bsim_utils_core_hooks_adjustments_get(&adjustments);

    m_dt += adjustments.tx_started.time_to_radio_address_us;
#endif

    m_timer.action_type              = NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK;
    m_timer.action.callback.callback = timeout_timer_fired;
    m_timer.trigger_time             = now + m_dt;

    m_procedure_is_active = true;

    ret = nrf_802154_sl_timer_add(&m_timer);
    NRF_802154_ASSERT(ret == NRF_802154_SL_TIMER_RET_SUCCESS);
    (void)ret;
}

static void timeout_timer_stop(void)
{
    m_procedure_is_active = false;

    // To make sure `timeout_timer_fired()` detects that procedure is being stopped if it preempts
    // this function.
    __DMB();

    (void)nrf_802154_sl_timer_remove(&m_timer);
}

void nrf_802154_ack_timeout_init(void)
{
    m_timeout             = NRF_802154_PRECISE_ACK_TIMEOUT_DEFAULT_TIMEOUT;
    m_dt                  = 0;
    m_procedure_is_active = false;
    mp_frame              = NULL;

    nrf_802154_sl_timer_init(&m_timer);
}

void nrf_802154_ack_timeout_deinit(void)
{
    nrf_802154_sl_timer_deinit(&m_timer);
}

void nrf_802154_ack_timeout_time_set(uint32_t time)
{
    m_timeout = time;
}

bool nrf_802154_ack_timeout_tx_started_hook(uint8_t * p_frame)
{
    mp_frame = p_frame;
    timeout_timer_start();

    return true;
}

bool nrf_802154_ack_timeout_abort(nrf_802154_term_t term_lvl, req_originator_t req_orig)
{
    bool result;

    if (!m_procedure_is_active || req_orig == REQ_ORIG_ACK_TIMEOUT)
    {
        // Ignore if procedure is not running or self-request.
        result = true;
    }
    else if (term_lvl >= NRF_802154_TERM_802154)
    {
        // Stop procedure only if termination level is high enough.
        timeout_timer_stop();

        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

void nrf_802154_ack_timeout_transmitted_hook(const uint8_t * p_frame)
{
    NRF_802154_ASSERT((p_frame == mp_frame) || (!m_procedure_is_active));

    timeout_timer_stop();
}

void nrf_802154_ack_timeout_rx_ack_started_hook(void)
{
    NRF_802154_ASSERT(m_procedure_is_active);

    timeout_timer_stop();
}

bool nrf_802154_ack_timeout_tx_failed_hook(uint8_t * p_frame, nrf_802154_tx_error_t error)
{
    (void)error;
    NRF_802154_ASSERT((p_frame == mp_frame) || (!m_procedure_is_active));

    timeout_timer_stop();

    return true;
}

#endif // NRF_802154_ACK_TIMEOUT_ENABLED
