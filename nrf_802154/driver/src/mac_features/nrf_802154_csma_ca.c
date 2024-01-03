/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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
 *   This file implements CSMA-CA procedure for the 802.15.4 driver.
 *
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_CSMACA

#include "nrf_802154_csma_ca.h"

#include "nrf_802154_config.h"
#if NRF_802154_CSMA_CA_ENABLED

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_const.h"
#include "nrf_802154_debug.h"
#include "nrf_802154_notification.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_request.h"
#include "nrf_802154_tx_power.h"
#include "nrf_802154_stats.h"
#include "platform/nrf_802154_random.h"
#include "rsch/nrf_802154_rsch.h"
#include "nrf_802154_sl_timer.h"
#include "nrf_802154_sl_atomics.h"

/**
 * @brief States of the CSMA-CA procedure.
 */
typedef enum
{
    CSMA_CA_STATE_IDLE,                                   ///< The CSMA-CA procedure is inactive.
    CSMA_CA_STATE_BACKOFF,                                ///< The CSMA-CA procedure is in backoff stage.
    CSMA_CA_STATE_ONGOING,                                ///< The frame is being sent.
    CSMA_CA_STATE_ABORTED                                 ///< The CSMA-CA procedure is being aborted.
} csma_ca_state_t;

static uint8_t m_nb;                                      ///< The number of times the CSMA-CA algorithm was required to back off while attempting the current transmission.
static uint8_t m_be;                                      ///< Backoff exponent, which is related to how many backoff periods a device shall wait before attempting to assess a channel.

static uint8_t                            * mp_data;      ///< Pointer to a buffer containing PHR and PSDU of the frame being transmitted.
static nrf_802154_transmitted_frame_props_t m_data_props; ///< Structure containing detailed properties of data in buffer.
static nrf_802154_fal_tx_power_split_t      m_tx_power;   ///< Power to be used when transmitting the frame split into components.
static uint8_t                              m_tx_channel; ///< Channel to be used to transmit the current frame.
static csma_ca_state_t                      m_state;      ///< The current state of the CSMA-CA procedure.

/**
 * @brief Perform appropriate actions for busy channel conditions.
 *
 * According to CSMA-CA description in 802.15.4 specification, when channel is busy NB and BE shall
 * be incremented and the device shall wait random delay before next CCA procedure. If NB reaches
 * macMaxCsmaBackoffs procedure fails.
 *
 * @retval true   Procedure failed and TX failure should be notified to the next higher layer.
 * @retval false  Procedure is still ongoing and TX failure should be handled internally.
 */
static bool channel_busy(void);

static bool csma_ca_state_set(csma_ca_state_t expected, csma_ca_state_t desired)
{
    nrf_802154_sl_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool result = nrf_802154_sl_atomic_cas_u8(&m_state, &expected, desired);

    if (result)
    {
        nrf_802154_sl_log_local_event(NRF_802154_LOG_VERBOSITY_LOW,
                                      NRF_802154_LOG_LOCAL_EVENT_ID_CSMACA__SET_STATE,
                                      (uint32_t)desired);
    }

    nrf_802154_sl_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);

    return result;
}

static void priority_leverage(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool first_transmit_attempt     = (0 == m_nb);
    bool coex_requires_boosted_prio = (nrf_802154_pib_coex_tx_request_mode_get() ==
                                       NRF_802154_COEX_TX_REQUEST_MODE_CCA_START);

    // Leverage priority only after the first backoff in the specified Coex TX request mode
    if (first_transmit_attempt && coex_requires_boosted_prio)
    {
        // It should always be possible to update this timeslot's priority here
        if (!nrf_802154_rsch_delayed_timeslot_priority_update(NRF_802154_RESERVED_CSMACA_ID,
                                                              RSCH_PRIO_TX))
        {
            NRF_802154_ASSERT(false);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

/**
 * @brief Notify MAC layer that CSMA-CA failed
 *
 * @param[in]  error  The error that caused the failure
 */
static void notify_failed(nrf_802154_tx_error_t error)
{
    // core rejected attempt, use my current frame_props
    nrf_802154_transmit_done_metadata_t metadata = {};

    metadata.frame_props = m_data_props;

    nrf_802154_notify_transmit_failed(mp_data, error, &metadata);
}

/**
 * @brief Notify MAC layer that channel is busy if tx request failed and there are no retries left.
 *
 * @param[in]  result  Result of TX request.
 */
static void notify_busy_channel(bool result)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_rsch_delayed_timeslot_cancel(NRF_802154_RESERVED_CSMACA_ID, true);

    // The 802.15.4 specification requires CSMA-CA to continue until m_nb is strictly greater
    // than nrf_802154_pib_csmaca_max_backoffs_get(), but at the moment this function is executed
    // the value of m_nb has not yet been incremented to reflect the latest attempt. Therefore
    // the comparison uses `greater or equal` instead of `greater than`.
    if (!result && (m_nb >= nrf_802154_pib_csmaca_max_backoffs_get()))
    {
        notify_failed(NRF_802154_TX_ERROR_BUSY_CHANNEL);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

/**
 * @brief Perform CCA procedure followed by frame transmission.
 *
 * If transmission is requested, CSMA-CA module waits for notification from the FSM module.
 * If transmission request fails, CSMA-CA module performs procedure for busy channel condition
 * @sa channel_busy().
 *
 * @param[in] dly_ts_id  Delayed timeslot identifier.
 */
static void frame_transmit(rsch_dly_ts_id_t dly_ts_id)
{
    NRF_802154_ASSERT(dly_ts_id == NRF_802154_RESERVED_CSMACA_ID);

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (csma_ca_state_set(CSMA_CA_STATE_BACKOFF, CSMA_CA_STATE_ONGOING))
    {
        priority_leverage();

        nrf_802154_transmit_params_t params =
        {
            .frame_props        = m_data_props,
            .tx_power           = m_tx_power,
            .channel            = m_tx_channel,
            .cca                = true,
            .immediate          = NRF_802154_CSMA_CA_WAIT_FOR_TIMESLOT ? false : true,
            .extra_cca_attempts = 0,
        };

        if (!nrf_802154_request_transmit(NRF_802154_TERM_NONE,
                                         REQ_ORIG_CSMA_CA,
                                         mp_data,
                                         &params,
                                         notify_busy_channel))
        {
            (void)channel_busy();
        }
    }
    else
    {
        nrf_802154_rsch_delayed_timeslot_cancel(dly_ts_id, true);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/**
 * @brief Calculates number of backoff periods as random value according to IEEE Std. 802.15.4.
 */
static uint8_t backoff_periods_calc_random(void)
{
    return nrf_802154_random_get() % (1U << m_be);
}

/**
 * @brief Calculates number of backoff periods to wait before the next CCA attempt of CSMA/CA
 *
 * @return Number of backoff periods
 */
static uint8_t backoff_periods_calc(void)
{
    uint8_t result;

#if NRF_802154_TEST_MODES_ENABLED

    switch (nrf_802154_pib_test_mode_csmaca_backoff_get())
    {
        case NRF_802154_TEST_MODE_CSMACA_BACKOFF_RANDOM:
            result = backoff_periods_calc_random();
            break;

        case NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MAX:
            result = (1U << m_be) - 1U;
            break;

        case NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MIN:
            result = 0U;
            break;

        default:
            result = backoff_periods_calc_random();
            NRF_802154_ASSERT(false);
            break;
    }
#else
    result = backoff_periods_calc_random();
#endif

    return result;
}

/**
 * @brief Delay CCA procedure for random (2^BE - 1) unit backoff periods.
 */
static void random_backoff_start(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    uint64_t backoff_us = backoff_periods_calc() * UNIT_BACKOFF_PERIOD;

    rsch_dly_ts_param_t backoff_ts_param =
    {
        .trigger_time     = nrf_802154_sl_timer_current_time_get() + backoff_us,
        .ppi_trigger_en   = false,
        .op               = RSCH_DLY_TS_OP_CSMACA,
        .type             = RSCH_DLY_TS_TYPE_RELAXED,
        .started_callback = frame_transmit,
        .id               = NRF_802154_RESERVED_CSMACA_ID,
    };

    switch (nrf_802154_pib_coex_tx_request_mode_get())
    {
        case NRF_802154_COEX_TX_REQUEST_MODE_FRAME_READY:
            // To request Coex precondition immediately, priority must be leveraged
            backoff_ts_param.prio = RSCH_PRIO_TX;
            break;

        case NRF_802154_COEX_TX_REQUEST_MODE_CCA_START:
            // Coex should be requested for all backoff periods but the first one
            backoff_ts_param.prio = (m_nb == 0) ? RSCH_PRIO_IDLE_LISTENING : RSCH_PRIO_TX;
            break;

        case NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE:
        case NRF_802154_COEX_TX_REQUEST_MODE_ON_CCA_TOGGLE:
            // Coex should not be requested during backoff periods
            backoff_ts_param.prio = RSCH_PRIO_IDLE_LISTENING;
            break;

        default:
            NRF_802154_ASSERT(false);
            break;
    }

    // Delayed timeslot with these parameters should always be scheduled
    if (!nrf_802154_rsch_delayed_timeslot_request(&backoff_ts_param))
    {
        NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static bool channel_busy(void)
{
    bool result = true;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (csma_ca_state_set(CSMA_CA_STATE_ONGOING, CSMA_CA_STATE_BACKOFF))
    {
        m_nb++;

        if (m_be < nrf_802154_pib_csmaca_max_be_get())
        {
            m_be++;
        }

        if (m_nb > nrf_802154_pib_csmaca_max_backoffs_get())
        {
            mp_data = NULL;
            bool ret = csma_ca_state_set(CSMA_CA_STATE_BACKOFF, CSMA_CA_STATE_IDLE);

            NRF_802154_ASSERT(ret);
            (void)ret;
        }
        else
        {
            random_backoff_start();
            result = false;
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
    return result;
}

bool nrf_802154_csma_ca_start(uint8_t                                      * p_data,
                              const nrf_802154_transmit_csma_ca_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    uint64_t ts = nrf_802154_sl_timer_current_time_get();

    nrf_802154_stat_timestamp_write(last_csmaca_start_timestamp, ts);
#endif

    bool result = csma_ca_state_set(CSMA_CA_STATE_IDLE, CSMA_CA_STATE_BACKOFF);

    NRF_802154_ASSERT(result);
    (void)result;

    uint8_t channel =
        p_metadata->tx_channel.use_metadata_value ? p_metadata->tx_channel.channel :
        nrf_802154_pib_channel_get();

    mp_data      = p_data;
    m_data_props = p_metadata->frame_props;
    m_nb         = 0;
    m_be         = nrf_802154_pib_csmaca_min_be_get();
    m_tx_channel = channel;
    (void)nrf_802154_tx_power_convert_metadata_to_tx_power_split(channel,
                                                                 p_metadata->tx_power,
                                                                 &m_tx_power);

    random_backoff_start();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return true;
}

bool nrf_802154_csma_ca_abort(nrf_802154_term_t term_lvl, req_originator_t req_orig)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = true;

    if (((req_orig != REQ_ORIG_CORE) && (req_orig != REQ_ORIG_HIGHER_LAYER)) ||
        (CSMA_CA_STATE_IDLE == nrf_802154_sl_atomic_load_u8(&m_state)))
    {
        // The request does not originate from core or the higher layer or the procedure
        // is stopped already. Ignore the abort request and return success, no matter
        // the termination level.
    }
    else if (term_lvl >= NRF_802154_TERM_802154)
    {
        // The procedure is active and the termination level allows the abort
        // request to be executed. Force aborted state. Don't clear the frame
        // pointer - it might be needed to notify failure.
        nrf_802154_sl_atomic_store_u8(&m_state, CSMA_CA_STATE_ABORTED);
        nrf_802154_rsch_delayed_timeslot_cancel(NRF_802154_RESERVED_CSMACA_ID, false);
    }
    else
    {
        // The procedure is active and the termination level does not allow
        // the abort request to be executed. Return failure
        result = false;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_csma_ca_tx_failed_hook(uint8_t * p_frame, nrf_802154_tx_error_t error)
{
    bool result = true;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (error)
    {
        // Below errors mean a failure occurred during the frame processing and the frame cannot be
        // transmitted unless a higher layer takes appropriate actions, hence the CSMA-CA procedure
        // shall be stopped.

        case NRF_802154_TX_ERROR_KEY_ID_INVALID:
        /* Fallthrough. */
        case NRF_802154_TX_ERROR_FRAME_COUNTER_ERROR:
            if (mp_data == p_frame)
            {
                mp_data = NULL;
                nrf_802154_sl_atomic_store_u8(&m_state, CSMA_CA_STATE_IDLE);
            }
            break;

        default:
            if (csma_ca_state_set(CSMA_CA_STATE_ABORTED, CSMA_CA_STATE_IDLE))
            {
                // The procedure was successfully aborted.

                if (p_frame != mp_data)
                {
                    // The procedure was aborted while another operation was holding
                    // frame pointer in the core - hence p_frame points to a different
                    // frame than mp_data. CSMA-CA failure must be notified directly.
                    notify_failed(error);
                }
            }
            else if (p_frame == mp_data)
            {
                // The procedure is active and transmission attempt failed. Try again
                result = channel_busy();
            }
            else
            {
                // Intentionally empty.
            }
            break;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_csma_ca_tx_started_hook(uint8_t * p_frame)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (mp_data == p_frame)
    {
        mp_data = NULL;
        nrf_802154_sl_atomic_store_u8(&m_state, CSMA_CA_STATE_IDLE);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
    return true;
}

#endif // NRF_802154_CSMA_CA_ENABLED
