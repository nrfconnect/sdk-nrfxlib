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
#include "nrf_802154_csma_ca_backoff.h"
#include "rsch/nrf_802154_rsch.h"
#include "nrf_802154_sl_timer.h"
#include "nrf_802154_sl_atomics.h"
#include "nrf_802154_frame_parser.h"

/**
 * @brief States of the CSMA-CA procedure.
 */
typedef enum
{
    CSMA_CA_STATE_IDLE,                                   ///< The CSMA-CA procedure is inactive.
    CSMA_CA_STATE_BACKOFF,                                ///< The CSMA-CA procedure is in backoff stage.
    CSMA_CA_STATE_ATTEMPTING_CCATX,                       ///< The frame transmit attempt is in progress.
    CSMA_CA_STATE_TRANSMITTING,                           ///< The frame has begun transmission.
} csma_ca_state_t;

static uint8_t m_nb;                                      ///< The number of times the CSMA-CA algorithm was required to back off while attempting the current transmission.
static uint8_t m_be;                                      ///< Backoff exponent, which is related to how many backoff periods a device shall wait before attempting to assess a channel.

static nrf_802154_frame_t                   m_frame;      ///< Frame to be transmitted.
static nrf_802154_transmitted_frame_props_t m_data_props; ///< Structure containing detailed properties of data in buffer.
static nrf_802154_fal_tx_power_split_t      m_tx_power;   ///< Power to be used when transmitting the frame split into components.
static uint8_t                              m_tx_channel; ///< Channel to be used to transmit the current frame.
#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
static bool m_tx_timestamp_encode;                        ///< Tx timestamp request flag for the current transmission attempt.
#endif
static csma_ca_state_t m_state;                           ///< The current state of the CSMA-CA procedure.

static bool csma_ca_can_abort(nrf_802154_term_t              term_lvl,
                              req_originator_t               req_orig,
                              const nrf_802154_tx_client_t * p_client);
static void csma_ca_failed(uint8_t                                   * p_frame,
                           nrf_802154_tx_error_t                       error,
                           const nrf_802154_transmit_done_metadata_t * p_metadata,
                           const nrf_802154_tx_client_t              * p_client);
static void csma_ca_tx_started(const nrf_802154_tx_client_t * p_client);

static void csma_ca_tx_done(uint8_t                                   * p_frame,
                            const nrf_802154_transmit_done_metadata_t * p_metadata,
                            const nrf_802154_tx_client_t              * p_client);

static const nrf_802154_tx_client_interface_t m_csma_ca_tx_client_iface = {
    .can_abort = csma_ca_can_abort,
    .failed    = csma_ca_failed,
    .started   = csma_ca_tx_started,
    .done      = csma_ca_tx_done,
};

static nrf_802154_tx_client_t m_csma_ca_tx_client = {
    .p_iface = &m_csma_ca_tx_client_iface,
};

static bool csma_ca_state_set(csma_ca_state_t expected, csma_ca_state_t desired)
{
    nrf_802154_sl_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool result = nrf_802154_sl_atomic_cas_u8((uint8_t *)&m_state, (uint8_t *)&expected, desired);

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
    // It should always be possible to update this timeslot's priority here
    if (first_transmit_attempt && coex_requires_boosted_prio &&
        !nrf_802154_rsch_delayed_timeslot_priority_update(NRF_802154_RESERVED_CSMACA_ID,
                                                          RSCH_PRIO_TX))
    {
        NRF_802154_ASSERT(false);
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

    nrf_802154_tx_error_t result = NRF_802154_TX_ERROR_NONE;

    if (csma_ca_state_set(CSMA_CA_STATE_BACKOFF, CSMA_CA_STATE_ATTEMPTING_CCATX))
    {
        priority_leverage();

        nrf_802154_transmit_params_t params =
        {
            .frame              = m_frame,
            .frame_props        = m_data_props,
            .tx_power           = m_tx_power,
            .channel            = m_tx_channel,
            .cca                = true,
            .immediate          = NRF_802154_CSMA_CA_WAIT_FOR_TIMESLOT ? false : true,
            .extra_cca_attempts = 0,
#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
            .tx_timestamp_encode = m_tx_timestamp_encode,
#else
            .tx_timestamp_encode = false,
#endif
            .p_client         = &m_csma_ca_tx_client,
            .rsch_timeslot_id = dly_ts_id,
        };

        result = nrf_802154_request_transmit(NRF_802154_TERM_NONE,
                                             REQ_ORIG_CSMA_CA,
                                             &params);
    }
    else
    {
        bool cancel_status = nrf_802154_rsch_delayed_timeslot_cancel(dly_ts_id, true);

        NRF_802154_ASSERT(cancel_status);
        (void)cancel_status;
    }

    if (result != NRF_802154_TX_ERROR_NONE)
    {
        nrf_802154_transmit_done_metadata_t metadata = {};

        metadata.frame_props = m_data_props;
        csma_ca_failed(m_frame.p_frame, result, &metadata, &m_csma_ca_tx_client);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/**
 * @brief Delay CCA procedure for random (2^BE - 1) unit backoff periods.
 */
static void random_backoff_start(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    uint64_t backoff_us = nrf_802154_csma_ca_backoff_periods_get(m_be) * UNIT_BACKOFF_PERIOD;

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
static bool channel_busy(void)
{
    bool result = true;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (csma_ca_state_set(CSMA_CA_STATE_ATTEMPTING_CCATX, CSMA_CA_STATE_BACKOFF))
    {
        m_nb++;

        if (m_be < nrf_802154_pib_csmaca_max_be_get())
        {
            m_be++;
        }

        if (m_nb > nrf_802154_pib_csmaca_max_backoffs_get())
        {
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

nrf_802154_tx_error_t nrf_802154_csma_ca_start(
    const nrf_802154_frame_t                     * p_frame,
    const nrf_802154_transmit_csma_ca_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    uint64_t ts = nrf_802154_sl_timer_current_time_get();

    nrf_802154_stat_timestamp_write_last_csmaca_start_timestamp(ts);
#endif

    nrf_802154_tx_error_t error = NRF_802154_TX_ERROR_TIMESLOT_DENIED;

    bool result = csma_ca_state_set(CSMA_CA_STATE_IDLE, CSMA_CA_STATE_BACKOFF);

    if (result)
    {
        uint8_t channel =
            p_metadata->tx_channel.use_metadata_value ? p_metadata->tx_channel.channel :
            nrf_802154_pib_channel_get();

        m_frame      = *p_frame;
        m_data_props = p_metadata->frame_props;
        m_nb         = 0;
        m_be         = nrf_802154_pib_csmaca_min_be_get();
        m_tx_channel = channel;
    #if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
        m_tx_timestamp_encode = p_metadata->tx_timestamp_encode;
    #endif
        (void)nrf_802154_tx_power_convert_metadata_to_tx_power_split(channel,
                                                                     p_metadata->tx_power,
                                                                     &m_tx_power);

        random_backoff_start();

        error = NRF_802154_TX_ERROR_NONE;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return error;
}

static bool csma_ca_can_abort(nrf_802154_term_t              term_lvl,
                              req_originator_t               req_orig,
                              const nrf_802154_tx_client_t * p_client)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    (void)p_client;

    bool result = true;

    if ((req_orig != REQ_ORIG_CORE) && (req_orig != REQ_ORIG_HIGHER_LAYER))
    {
        // The request does not originate from core or the higher layer or the procedure
        // is stopped already. Ignore the abort request and return success, no matter
        // the termination level.
    }
    else if (term_lvl >= NRF_802154_TERM_802154)
    {
        // The procedure is active and the termination level allows the abort
        // request to be executed.
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

static void csma_ca_finish_failed(uint8_t                                   * p_frame,
                                  nrf_802154_tx_error_t                       error,
                                  const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    nrf_802154_sl_atomic_store_u8((uint8_t *)&m_state, CSMA_CA_STATE_IDLE);
    nrf_802154_frame_parser_data_clear(&m_frame);
    nrf_802154_notify_transmit_failed(p_frame, error, p_metadata);
}

static void csma_ca_failed(uint8_t                                   * p_frame,
                           nrf_802154_tx_error_t                       error,
                           const nrf_802154_transmit_done_metadata_t * p_metadata,
                           const nrf_802154_tx_client_t              * p_client)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    (void)p_client;

    switch (error)
    {
        case NRF_802154_TX_ERROR_TIMESLOT_DENIED:
        case NRF_802154_TX_ERROR_BUSY_CHANNEL:
        case NRF_802154_TX_ERROR_TIMESLOT_ENDED:
            if (channel_busy())
            {
                csma_ca_finish_failed(p_frame, error, p_metadata);
            }
            break;

        default:
            csma_ca_finish_failed(p_frame, error, p_metadata);
            break;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void csma_ca_tx_started(const nrf_802154_tx_client_t * p_client)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    (void)p_client;

    bool result = csma_ca_state_set(CSMA_CA_STATE_ATTEMPTING_CCATX, CSMA_CA_STATE_TRANSMITTING);

    NRF_802154_ASSERT(result);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void csma_ca_tx_done(uint8_t                                   * p_frame,
                            const nrf_802154_transmit_done_metadata_t * p_metadata,
                            const nrf_802154_tx_client_t              * p_client)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    (void)p_client;

    bool result = csma_ca_state_set(CSMA_CA_STATE_TRANSMITTING, CSMA_CA_STATE_IDLE);

    NRF_802154_ASSERT(result);

    nrf_802154_frame_parser_data_clear(&m_frame);
    nrf_802154_notify_transmitted(p_frame, p_metadata);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#ifdef TEST
void nrf_802154_csma_ca_module_reset(void)
{
    m_state      = CSMA_CA_STATE_IDLE;
    m_nb         = 0;
    m_be         = 0;
    m_frame      = (nrf_802154_frame_t){0};
    m_data_props = (nrf_802154_transmitted_frame_props_t){0};
    m_tx_channel = 0;
#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
    m_tx_timestamp_encode = false;
#endif
    m_state = CSMA_CA_STATE_IDLE;
}

#endif // TEST

#endif // NRF_802154_CSMA_CA_ENABLED
