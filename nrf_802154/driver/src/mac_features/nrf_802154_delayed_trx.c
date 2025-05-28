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
 *   This file implements delayed transmission and reception features.
 *
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_DELAYED_TRX

#include "nrf_802154_delayed_trx.h"

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>

#include <nrfx.h>
#include "../nrf_802154_debug.h"
#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_frame_parser.h"
#include "nrf_802154_notification.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_procedures_duration.h"
#include "nrf_802154_queue.h"
#include "nrf_802154_request.h"
#include "nrf_802154_utils.h"
#include "nrf_802154_tx_power.h"
#include "rsch/nrf_802154_rsch.h"
#include "nrf_802154_sl_timer.h"
#include "nrf_802154_sl_utils.h"
#include "nrf_802154_sl_atomics.h"

#ifdef NRF_802154_USE_INTERNAL_INCLUDES
#include "nrf_802154_delayed_trx_internal.h"
#endif

#if NRF_802154_DELAYED_TRX_ENABLED

#if defined(NRF52_SERIES)
#define TX_SETUP_TIME_MAX 270u ///< Maximum time needed to prepare TX procedure [us]. It does not include TX ramp-up time.
#define RX_SETUP_TIME_MAX 270u ///< Maximum time needed to prepare RX procedure [us]. It does not include RX ramp-up time.
#elif defined(NRF53_SERIES)
#define TX_SETUP_TIME_MAX 360u ///< Maximum time needed to prepare TX procedure [us]. It does not include TX ramp-up time.
#define RX_SETUP_TIME_MAX 290u ///< Maximum time needed to prepare RX procedure [us]. It does not include RX ramp-up time.
#elif defined(NRF54L_SERIES)
NRF_STATIC_ASSERT(NRF_CONFIG_CPU_FREQ_MHZ == 128,
                  "Currently nrf-802154 only works when frequency is 128MHz");
#define TX_SETUP_TIME_MAX 185u ///< Maximum time needed to prepare TX procedure [us]. It does not include TX ramp-up time.
#define RX_SETUP_TIME_MAX 150u ///< Maximum time needed to prepare RX procedure [us]. It does not include RX ramp-up time.
#elif defined(NRF54H_SERIES)
#ifndef TX_SETUP_TIME_MAX
#define TX_SETUP_TIME_MAX 400u ///< Maximum time needed to prepare TX procedure [us]. It does not include TX ramp-up time.
#endif
#ifndef RX_SETUP_TIME_MAX
#define RX_SETUP_TIME_MAX 400u ///< Maximum time needed to prepare RX procedure [us]. It does not include RX ramp-up time.
#endif
#endif

/**
 * @brief States of delayed operations.
 */
typedef enum
{
    DELAYED_TRX_OP_STATE_STOPPED     = (1 << 0),      ///< Delayed operation stopped.
    DELAYED_TRX_OP_STATE_PENDING     = (1 << 1),      ///< Delayed operation scheduled and waiting for timeslot.
    DELAYED_TRX_OP_STATE_ONGOING     = (1 << 2),      ///< Delayed operation ongoing (during timeslot).
    DELAYED_TRX_OP_STATE_ALLOWED_MSK = ((1 << 3) - 1) ///< Mask of allowed delayed operation states.
} delayed_trx_op_state_t;

/**
 * @brief RX delayed operation frame data.
 */
typedef struct
{
    uint64_t sof_timestamp; ///< Timestamp of last start of frame notification received in RX window.
    uint8_t  psdu_length;   ///< Length in bytes of the frame to be received in RX window.
    bool     ack_requested; ///< Flag indicating if Ack for the frame to be received in RX window is requested.
} delayed_rx_frame_data_t;

/**
 * @brief RX delayed operation data.
 */
typedef struct
{
    nrf_802154_sl_timer_t            timeout_timer;   ///< Timer for delayed RX timeout handling.
    uint32_t                         timeout_length;  ///< Requested length [us] of RX window plus RX_RAMP_UP_TIME.
    volatile delayed_rx_frame_data_t extension_frame; ///< Data of frame that caused extension of RX window.
    uint8_t                          channel;         ///< Channel number on which reception should be performed.
} dly_rx_data_t;

/**
 * @brief TX delayed operation data.
 */
typedef struct
{
    uint8_t                    * p_data; ///< Pointer to a buffer containing PHR and PSDU of the frame requested to be transmitted.
    nrf_802154_transmit_params_t params; ///< Transmission parameters.

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    uint64_t                     time;   ///< Target time of the first bit of the frame.

#endif
} dly_tx_data_t;

/**
 * @brief Delayed operation data.
 */
typedef struct
{
    delayed_trx_op_state_t state; ///< State of the delayed timeslot.
    rsch_dly_ts_id_t       id;    ///< Identifier of the delayed timeslot.
    rsch_dly_ts_op_t       op;    ///< Type of delayed operation to be performed.

    union
    {
        dly_tx_data_t tx; ///< Data specific for delayed transmission.
        dly_rx_data_t rx; ///< Data specific for delayed reception.
    };
} dly_op_data_t;

/**
 * @brief Array of slots for RX delayed operations.
 */
static dly_op_data_t m_dly_rx_data[NRF_802154_RSCH_DLY_TS_OP_DRX_SLOTS];

/**
 * @brief Array of slots for TX delayed operations.
 */
static dly_op_data_t m_dly_tx_data[NRF_802154_RSCH_DLY_TS_OP_DTX_SLOTS];

/**
 * @brief Queue of RX delayed operations IDs to be processed.
 */
static nrf_802154_queue_t m_dly_rx_id_q;

/**
 * @brief Storage for RX delayed operations ID queue.
 */
static dly_op_data_t * m_dly_rx_id_q_mem[NRF_802154_RSCH_DLY_TS_OP_DRX_SLOTS];

/**
 * @brief Search for a RX delayed operation with given ID.
 *
 * @param[in]  id   Identifier to search for.
 *
 * @return Pointer to matching slot.
 */
static dly_op_data_t * dly_rx_data_by_id_search(rsch_dly_ts_id_t id)
{
    dly_op_data_t * p_dly_op_data = NULL;

    for (uint32_t i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        if (m_dly_rx_data[i].id == id)
        {
            // Slot with a matching identifier found
            if (p_dly_op_data == NULL)
            {
                // It's the first matching slot found
                p_dly_op_data = &m_dly_rx_data[i];
            }
            else
            {
                // There's already been a matching slot. We expect all active slots to have unique
                // IDs and all inactive slots to have their IDs set to NRF_802154_RESERVED_INVALID_ID.
                // If we ended up here then either we're searching for invalid ID or the IDs assigned
                // to active slots aren't unique. Either way - assert.
                NRF_802154_ASSERT(false);
            }
        }
    }

    return p_dly_op_data;
}

/**
 * Set state of a delayed operation.
 *
 * @param[in]  p_dly_op_data       Data of the delayed operation.
 * @param[in]  expected_state      Expected delayed operation state prior to state transition.
 * @param[in]  new_state           Delayed operation state to enter.
 *
 * @retval true   Successfully set the new state.
 * @retval false  Failed to set the new state.
 */
static bool dly_op_state_set(dly_op_data_t        * p_dly_op_data,
                             delayed_trx_op_state_t expected_state,
                             delayed_trx_op_state_t new_state)
{
    bool result = false;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (p_dly_op_data->op)
    {
        case RSCH_DLY_TS_OP_DTX:
        case RSCH_DLY_TS_OP_DRX:
        {
            result = nrf_802154_sl_atomic_cas_u8(
                (uint8_t *)&p_dly_op_data->state,
                (uint8_t *)&expected_state,
                new_state);

            if (result)
            {
                nrf_802154_log_local_event(NRF_802154_LOG_VERBOSITY_LOW,
                                           NRF_802154_LOG_LOCAL_EVENT_ID_DELAYED_TRX__SET_STATE,
                                           (uint32_t)new_state);
            }
        }
        break;

        default:
        {
            NRF_802154_ASSERT(false);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

/**
 * @brief Search for a TX delayed operation with given ID.
 *
 * @param[in]  id   Identifier to search for.
 *
 * @return Pointer to matching slot.
 */
static dly_op_data_t * dly_tx_data_by_id_search(rsch_dly_ts_id_t id)
{
    // Note that this function only supports a single slot.
    if (id == m_dly_tx_data[0].id)
    {
        return &m_dly_tx_data[0];
    }

    return NULL;
}

/**
 * @brief Retrieve an available slot from a pool.
 *
 * @param[inout]  p_dly_op_data_pool  Pool of slots to allocate from.
 * @param[in]     pool_len            Number of elements in the pool.
 *
 * @return Pointer to an available slot.
 */
static dly_op_data_t * available_dly_ts_slot_from_pool_get(
    dly_op_data_t * p_dly_op_data_pool, uint32_t pool_len)
{
    for (uint32_t i = 0; i < pool_len; i++)
    {
        if (dly_op_state_set(&p_dly_op_data_pool[i],
                             DELAYED_TRX_OP_STATE_STOPPED,
                             DELAYED_TRX_OP_STATE_PENDING))
        {
            return &p_dly_op_data_pool[i];
        }
    }

    return NULL;
}

/**
 * @brief Get a slot for TX delayed operation.
 *
 * @return Pointer to a slot.
 */
static dly_op_data_t * available_dly_tx_slot_get(void)
{
    return available_dly_ts_slot_from_pool_get(
        m_dly_tx_data,
        sizeof(m_dly_tx_data) / sizeof(m_dly_tx_data[0]));
}

/**
 * @brief Get a slot for RX delayed operation.
 *
 * @return Pointer to a slot.
 */
static dly_op_data_t * available_dly_rx_slot_get(void)
{
    return available_dly_ts_slot_from_pool_get(
        m_dly_rx_data,
        sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]));
}

/**
 * @brief Get an ongoing RX delayed operation slot.
 *
 * @return Pointer to a slot or NULL if no ongoing RX delayed operations exist at the moment.
 */
static dly_op_data_t * ongoing_dly_rx_slot_get(void)
{
    dly_op_data_t * p_dly_op_data = NULL;

    for (uint32_t i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        if (m_dly_rx_data[i].state == DELAYED_TRX_OP_STATE_ONGOING)
        {
            p_dly_op_data = &m_dly_rx_data[i];
        }
    }

    return p_dly_op_data;
}

static bool dly_ts_slot_release(dly_op_data_t * p_dly_op_data, bool handler)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result;

    result = nrf_802154_rsch_delayed_timeslot_cancel(p_dly_op_data->id, handler);

    if (result)
    {
        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

/**
 * @brief Atomically push an ID to RX delayed operation ID queue.
 *
 * @param[in]  id  Identifier to be pushed to the queue.
 */
static void dly_rx_data_atomically_push(dly_op_data_t * p_dly_op_data)
{
    nrf_802154_mcu_critical_state_t mcu_cs;

    nrf_802154_mcu_critical_enter(mcu_cs);

    NRF_802154_ASSERT(!nrf_802154_queue_is_full(&m_dly_rx_id_q));

    dly_op_data_t ** pp_op = (dly_op_data_t **)nrf_802154_queue_push_begin(&m_dly_rx_id_q);

    *pp_op = p_dly_op_data;

    nrf_802154_queue_push_commit(&m_dly_rx_id_q);

    nrf_802154_mcu_critical_exit(mcu_cs);
}

/**
 * @brief Atomically pop an ID from RX delayed operation ID queue.
 *
 * @param[out]  p_id  Pointer to identifier popped from the queue.
 */
static dly_op_data_t * dly_rx_data_atomically_pop(void)
{
    nrf_802154_mcu_critical_state_t mcu_cs;

    nrf_802154_mcu_critical_enter(mcu_cs);

    dly_op_data_t ** pp_op = (dly_op_data_t **)nrf_802154_queue_pop_begin(&m_dly_rx_id_q);

    nrf_802154_queue_pop_commit(&m_dly_rx_id_q);

    nrf_802154_mcu_critical_exit(mcu_cs);

    return *pp_op;
}

/**
 * Start delayed operation.
 *
 * @param[in]     p_dly_ts_param  Parameters of the requested delayed timeslot.
 * @param[inout]  p_dly_op_data   Data of the delayed operation.
 */
static bool dly_op_request(const rsch_dly_ts_param_t * p_dly_ts_param,
                           dly_op_data_t             * p_dly_op_data)
{
    bool result = nrf_802154_rsch_delayed_timeslot_request(p_dly_ts_param);

    if (!result)
    {
        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;

        // Release the delayed operation slot immediately in case of failure.
        bool state_set = dly_op_state_set(p_dly_op_data,
                                          DELAYED_TRX_OP_STATE_PENDING,
                                          DELAYED_TRX_OP_STATE_STOPPED);

        NRF_802154_ASSERT(state_set);
        (void)state_set;
    }

    return result;
}

/**
 * Notify MAC layer that no frame was received before timeout.
 *
 * @param[in]  p_context  Not used.
 */
static void notify_rx_timeout(nrf_802154_sl_timer_t * p_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    dly_op_data_t * p_dly_op_data = (dly_op_data_t *)(p_timer->user_data.p_pointer);
    uint64_t        now           = nrf_802154_sl_timer_current_time_get();
    uint64_t        sof_timestamp = p_dly_op_data->rx.extension_frame.sof_timestamp;

    // Make sure that the timestamp has been latched safely. If frame reception preempts the code
    // after executing this line, the RX window will not be extended.
    __DMB();
    uint8_t  psdu_length   = p_dly_op_data->rx.extension_frame.psdu_length;
    bool     ack_requested = p_dly_op_data->rx.extension_frame.ack_requested;
    uint32_t frame_length  = nrf_802154_rx_duration_get(psdu_length, ack_requested);

    if (nrf_802154_sl_time64_is_in_future(now, sof_timestamp + frame_length))
    {
        // @TODO protect against infinite extensions - allow only one timer extension
        p_dly_op_data->rx.timeout_timer.trigger_time = sof_timestamp + frame_length;
        p_dly_op_data->rx.timeout_length             = frame_length;

        nrf_802154_sl_timer_ret_t ret;

        ret = nrf_802154_sl_timer_add(&p_dly_op_data->rx.timeout_timer);
        NRF_802154_ASSERT(ret == NRF_802154_SL_TIMER_RET_SUCCESS);
        (void)ret;
    }
    else
    {
        bool notified = nrf_802154_notify_receive_failed(
            NRF_802154_RX_ERROR_DELAYED_TIMEOUT,
            p_dly_op_data->id,
            false);

        // It should always be possible to notify DRX result
        NRF_802154_ASSERT(notified);
        (void)notified;

        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;

        bool result = dly_op_state_set(p_dly_op_data,
                                       DELAYED_TRX_OP_STATE_ONGOING,
                                       DELAYED_TRX_OP_STATE_STOPPED);

        NRF_802154_ASSERT(result);
        (void)result;

        if (!nrf_802154_pib_rx_on_when_idle_get())
        {
            (void)nrf_802154_request_sleep(NRF_802154_TERM_NONE);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/**
 * Transmit request result callback.
 *
 * @param[in]  result  Result of TX request.
 */
static void dly_tx_result_notify(bool result)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Currently there's only a single delayed transmission possible at a time
    dly_op_data_t * p_dly_op_data = dly_tx_data_by_id_search(NRF_802154_RESERVED_DTX_ID);

    NRF_802154_ASSERT(p_dly_op_data != NULL);

    if (!result)
    {
        // core rejected attempt, use my current frame_props
        nrf_802154_transmit_done_metadata_t metadata = {};

        metadata.frame_props = p_dly_op_data->tx.params.frame_props;
        nrf_802154_notify_transmit_failed(p_dly_op_data->tx.p_data,
                                          NRF_802154_TX_ERROR_TIMESLOT_DENIED,
                                          &metadata);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void dly_rx_all_ongoing_abort(void)
{
    nrf_802154_sl_timer_ret_t ret;
    dly_op_data_t           * p_dly_op_data;
    bool                      result;

    for (int i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        p_dly_op_data = &m_dly_rx_data[i];

        ret = nrf_802154_sl_timer_remove(&p_dly_op_data->rx.timeout_timer);

        if (ret != NRF_802154_SL_TIMER_RET_SUCCESS)
        {
            // If the timer is ongoing, the timer either fired or is about to fire.
            // Cleanup and notification will be performed by the timer callback.
            continue;
        }

        bool notified = nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_DELAYED_ABORTED,
                                                         p_dly_op_data->id,
                                                         false);

        // It should always be possible to notify DRX result
        NRF_802154_ASSERT(notified);
        (void)notified;

        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;

        result = dly_op_state_set(p_dly_op_data,
                                  DELAYED_TRX_OP_STATE_ONGOING,
                                  DELAYED_TRX_OP_STATE_STOPPED);
        NRF_802154_ASSERT(result);
        (void)result;
    }
}

/**
 * Receive request result callback.
 *
 * @param[in]  result  Result of RX request.
 */
static void dly_rx_result_notify(bool result)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    dly_op_data_t * p_dly_op_data = dly_rx_data_atomically_pop();

    if (p_dly_op_data == NULL)
    {
        NRF_802154_ASSERT(false);
        return;
    }

    if (result)
    {
        uint64_t now;

        dly_rx_all_ongoing_abort();

        now = nrf_802154_sl_timer_current_time_get();
        uint32_t tout_len = p_dly_op_data->rx.timeout_length;

        p_dly_op_data->rx.timeout_timer.trigger_time        = now + tout_len;
        p_dly_op_data->rx.timeout_timer.user_data.p_pointer = p_dly_op_data;
        p_dly_op_data->rx.timeout_timer.action_type         =
            NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK;

        p_dly_op_data->rx.extension_frame.sof_timestamp = now;
        p_dly_op_data->rx.extension_frame.psdu_length   = 0;
        p_dly_op_data->rx.extension_frame.ack_requested = false;

        nrf_802154_sl_timer_ret_t ret;

        ret = nrf_802154_sl_timer_add(&p_dly_op_data->rx.timeout_timer);
        NRF_802154_ASSERT(ret == NRF_802154_SL_TIMER_RET_SUCCESS);
        (void)ret;
    }
    else
    {
        bool notified = nrf_802154_notify_receive_failed(
            NRF_802154_RX_ERROR_DELAYED_TIMESLOT_DENIED,
            p_dly_op_data->id,
            false);

        // It should always be possible to notify DRX result
        NRF_802154_ASSERT(notified);
        (void)notified;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void transmit_attempt(dly_op_data_t * p_dly_op_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // No need to enqueue transmit attempts. Proceed to transmission immediately
    (void)nrf_802154_request_transmit(NRF_802154_TERM_802154,
                                      REQ_ORIG_DELAYED_TRX,
                                      p_dly_op_data->tx.p_data,
                                      &p_dly_op_data->tx.params,
                                      dly_tx_result_notify);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static bool receive_attempt(dly_op_data_t * p_dly_op_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool result                 = false;
    bool channel_update_success = true;

    // This function is expected to result in calling @ref dly_rx_result_notify.
    // In order for that function to differentiate between different delayed RX
    // windows, we atomically insert the ID of the current delayed RX into a FIFO queue.
    dly_rx_data_atomically_push(p_dly_op_data);

    if (nrf_802154_pib_channel_get() != p_dly_op_data->rx.channel)
    {
        nrf_802154_pib_channel_set(p_dly_op_data->rx.channel);
        channel_update_success = nrf_802154_request_channel_update(REQ_ORIG_DELAYED_TRX);
    }

    if (channel_update_success)
    {
        result = nrf_802154_request_receive(NRF_802154_TERM_802154,
                                            REQ_ORIG_DELAYED_TRX,
                                            dly_rx_result_notify,
                                            true,
                                            p_dly_op_data->id);
    }
    else
    {
        dly_rx_result_notify(result);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);

    return result;
}

/**
 * Notify that the previously requested delayed TX timeslot has started just now.
 *
 * @param[in]  dly_ts_id  ID of the started timeslot.
 */
static void tx_timeslot_started_callback(rsch_dly_ts_id_t dly_ts_id)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    dly_op_data_t * p_dly_op_data = dly_tx_data_by_id_search(dly_ts_id);

    NRF_802154_ASSERT(p_dly_op_data != NULL);

    bool result = dly_op_state_set(p_dly_op_data,
                                   DELAYED_TRX_OP_STATE_PENDING,
                                   DELAYED_TRX_OP_STATE_ONGOING);

    NRF_802154_ASSERT(result);

    transmit_attempt(p_dly_op_data);

    result = dly_ts_slot_release(p_dly_op_data, true);
    NRF_802154_ASSERT(result);

    result = dly_op_state_set(p_dly_op_data,
                              DELAYED_TRX_OP_STATE_ONGOING,
                              DELAYED_TRX_OP_STATE_STOPPED);
    NRF_802154_ASSERT(result);
    (void)result;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

/**
 * Notify that the previously requested delayed RX timeslot has started just now.
 *
 * @param[in]  dly_ts_id  ID of the started timeslot.
 */
static void rx_timeslot_started_callback(rsch_dly_ts_id_t dly_ts_id)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    dly_op_data_t * p_dly_op_data = dly_rx_data_by_id_search(dly_ts_id);

    if (p_dly_op_data == NULL)
    {
        /* The DRX was canceled. */
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
        return;
    }

    bool attempt_success = false;
    bool result          = dly_op_state_set(p_dly_op_data,
                                            DELAYED_TRX_OP_STATE_PENDING,
                                            DELAYED_TRX_OP_STATE_ONGOING);

    if (result)
    {
        attempt_success = receive_attempt(p_dly_op_data);
    }

    result = nrf_802154_rsch_delayed_timeslot_cancel(dly_ts_id, true);
    NRF_802154_ASSERT(result);

    if (!attempt_success)
    {
        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;

        result = dly_op_state_set(p_dly_op_data,
                                  DELAYED_TRX_OP_STATE_ONGOING,
                                  DELAYED_TRX_OP_STATE_STOPPED);
        NRF_802154_ASSERT(result);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#ifdef TEST
#include "string.h"
void nrf_802154_delayed_trx_module_reset(void)
{
    memset(m_dly_rx_data, 0, sizeof(m_dly_rx_data));
    memset(m_dly_tx_data, 0, sizeof(m_dly_tx_data));
    memset(&m_dly_rx_id_q, 0, sizeof(m_dly_rx_id_q));
    memset(m_dly_rx_id_q_mem, 0, sizeof(m_dly_rx_id_q_mem));
}

#endif // TEST

void nrf_802154_delayed_trx_init(void)
{
    nrf_802154_queue_init(&m_dly_rx_id_q,
                          m_dly_rx_id_q_mem,
                          sizeof(m_dly_rx_id_q_mem),
                          sizeof(m_dly_rx_id_q_mem[0]));

    for (uint32_t i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        m_dly_rx_data[i].state = DELAYED_TRX_OP_STATE_STOPPED;
        m_dly_rx_data[i].id    = NRF_802154_RESERVED_INVALID_ID;
        nrf_802154_sl_timer_init(&m_dly_rx_data[i].rx.timeout_timer);
    }

    for (uint32_t i = 0; i < sizeof(m_dly_tx_data) / sizeof(m_dly_tx_data[0]); i++)
    {
        m_dly_tx_data[i].state = DELAYED_TRX_OP_STATE_STOPPED;
        m_dly_tx_data[i].id    = NRF_802154_RESERVED_INVALID_ID;
    }
}

void nrf_802154_delayed_trx_deinit(void)
{
    for (uint32_t i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        nrf_802154_sl_timer_deinit(&m_dly_rx_data[i].rx.timeout_timer);
    }
}

bool nrf_802154_delayed_trx_transmit(uint8_t                                 * p_data,
                                     uint64_t                                  tx_time,
                                     const nrf_802154_transmit_at_metadata_t * p_metadata)
{
    dly_op_data_t * p_dly_tx_data = available_dly_tx_slot_get();
    bool            result        = false;

    if (p_dly_tx_data != NULL)
    {
        tx_time -= TX_SETUP_TIME_MAX;
        tx_time -= TX_RAMP_UP_TIME;

        if (p_metadata->cca)
        {
            tx_time -= nrf_802154_cca_before_tx_duration_get();
        }

        p_dly_tx_data->op = RSCH_DLY_TS_OP_DTX;

        p_dly_tx_data->tx.p_data             = p_data;
        p_dly_tx_data->tx.params.frame_props = p_metadata->frame_props;
        (void)nrf_802154_tx_power_convert_metadata_to_tx_power_split(
            p_metadata->channel,
            p_metadata->tx_power,
            &p_dly_tx_data->tx.params.tx_power);
        p_dly_tx_data->tx.params.cca                = p_metadata->cca;
        p_dly_tx_data->tx.params.immediate          = true;
        p_dly_tx_data->tx.params.extra_cca_attempts = p_metadata->extra_cca_attempts;
        p_dly_tx_data->tx.params.channel            = p_metadata->channel;
        p_dly_tx_data->id                           = NRF_802154_RESERVED_DTX_ID;

        rsch_dly_ts_param_t dly_ts_param =
        {
            .trigger_time     = tx_time,
            .ppi_trigger_en   = true,
            .ppi_trigger_dly  = TX_SETUP_TIME_MAX,
            .prio             = RSCH_PRIO_TX,
            .op               = RSCH_DLY_TS_OP_DTX,
            .type             = RSCH_DLY_TS_TYPE_PRECISE,
            .started_callback = tx_timeslot_started_callback,
            .id               = NRF_802154_RESERVED_DTX_ID,
        };

        result = dly_op_request(&dly_ts_param, p_dly_tx_data);
    }

    return result;
}

bool nrf_802154_delayed_trx_receive(uint64_t rx_time,
                                    uint32_t timeout,
                                    uint8_t  channel,
                                    uint32_t id)
{
    if (dly_rx_data_by_id_search(id) != NULL)
    {
        /* DRX with given id is already present. */
        return false;
    }

    dly_op_data_t * p_dly_rx_data = available_dly_rx_slot_get();
    bool            result        = false;

    if (p_dly_rx_data != NULL)
    {
        rx_time -= RX_SETUP_TIME_MAX;
        rx_time -= RX_RAMP_UP_TIME;

        p_dly_rx_data->op = RSCH_DLY_TS_OP_DRX;

        p_dly_rx_data->rx.timeout_length = timeout + RX_RAMP_UP_TIME +
                                           RX_SETUP_TIME_MAX;
        p_dly_rx_data->rx.timeout_timer.action.callback.callback = notify_rx_timeout;

        p_dly_rx_data->rx.channel = channel;
        p_dly_rx_data->id         = id;

        rsch_dly_ts_param_t dly_ts_param =
        {
            .trigger_time     = rx_time,
            .ppi_trigger_en   = true,
            .ppi_trigger_dly  = RX_SETUP_TIME_MAX,
            .prio             = RSCH_PRIO_IDLE_LISTENING,
            .op               = RSCH_DLY_TS_OP_DRX,
            .type             = RSCH_DLY_TS_TYPE_PRECISE,
            .started_callback = rx_timeslot_started_callback,
            .id               = id,
        };

        result = dly_op_request(&dly_ts_param, p_dly_rx_data);
    }

    return result;
}

bool nrf_802154_delayed_trx_transmit_cancel(void)
{
    // This function does not provide any ID because it assumes that only a single delayed
    // transmission can be scheduled at a time. Therefore use the first (and only) entry
    // of m_dly_tx_data
    dly_op_data_t * p_dly_op_data = &m_dly_tx_data[0];
    bool            result        = false;

    if (dly_ts_slot_release(p_dly_op_data, false))
    {
        result = dly_op_state_set(p_dly_op_data,
                                  DELAYED_TRX_OP_STATE_PENDING,
                                  DELAYED_TRX_OP_STATE_STOPPED);

        NRF_802154_ASSERT(result);
    }

    return result;
}

bool nrf_802154_delayed_trx_receive_cancel(uint32_t id)
{
    dly_op_data_t * p_dly_op_data = dly_rx_data_by_id_search(id);
    bool            stopped       = false;

    if (p_dly_op_data == NULL)
    {
        // Delayed receive window with provided ID could not be found.
        return false;
    }

    bool result      = nrf_802154_rsch_delayed_timeslot_cancel(id, false);
    bool was_running = false;

    nrf_802154_sl_timer_ret_t ret = nrf_802154_sl_timer_remove(&p_dly_op_data->rx.timeout_timer);

    was_running = (ret == NRF_802154_SL_TIMER_RET_SUCCESS);

    if (result || was_running)
    {
        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;
        stopped           = true;

        nrf_802154_sl_atomic_store_u8((uint8_t *)&p_dly_op_data->state,
                                      DELAYED_TRX_OP_STATE_STOPPED);
    }

    return stopped;
}

bool nrf_802154_delayed_trx_receive_scheduled_cancel(uint32_t id)
{
    dly_op_data_t * p_dly_op_data = dly_rx_data_by_id_search(id);

    if (p_dly_op_data == NULL)
    {
        // Delayed receive window with provided ID could not be found.
        return true;
    }

    bool result = nrf_802154_rsch_delayed_timeslot_cancel(id, false);

    if (!result)
    {
        result =
            nrf_802154_sl_atomic_load_u8((uint8_t *)&p_dly_op_data->state) ==
            DELAYED_TRX_OP_STATE_STOPPED;
    }

    if (result)
    {
        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;

        nrf_802154_sl_atomic_store_u8((uint8_t *)&p_dly_op_data->state,
                                      DELAYED_TRX_OP_STATE_STOPPED);
    }

    return result;
}

bool nrf_802154_delayed_trx_abort(nrf_802154_term_t term_lvl, req_originator_t req_orig)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    if (ongoing_dly_rx_slot_get() == NULL)
    {
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
        return true;
    }

    if (term_lvl < NRF_802154_TERM_802154)
    {
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
        return false;
    }

    dly_rx_all_ongoing_abort();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
    return true;
}

void nrf_802154_delayed_trx_rx_started_hook(const uint8_t * p_frame)
{
    dly_op_data_t                * p_dly_op_data = ongoing_dly_rx_slot_get();
    nrf_802154_frame_parser_data_t frame_data;

    bool result = nrf_802154_frame_parser_data_init(p_frame,
                                                    p_frame[PHR_OFFSET] + PHR_SIZE,
                                                    PARSE_LEVEL_FCF_OFFSETS,
                                                    &frame_data);

    if ((result) && (p_dly_op_data != NULL))
    {
        p_dly_op_data->rx.extension_frame.sof_timestamp = nrf_802154_sl_timer_current_time_get();
        p_dly_op_data->rx.extension_frame.psdu_length   = p_frame[PHR_OFFSET];
        p_dly_op_data->rx.extension_frame.ack_requested = nrf_802154_frame_parser_ar_bit_is_set(
            &frame_data);
    }
}

bool nrf_802154_delayed_trx_nearest_drx_time_to_midpoint_get(uint32_t * p_drx_time_to_midpoint)
{
    bool     result            = false;
    uint32_t min_time_to_start = 0xffffffff;
    uint64_t drx_time_to_start = UINT64_C(0xffffffff);
    uint32_t drx_time_to_midpoint;
    uint32_t drx_window_duration_time;

    for (int i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        if (m_dly_rx_data[i].state != DELAYED_TRX_OP_STATE_PENDING)
        {
            continue;
        }

        result = nrf_802154_rsch_delayed_timeslot_time_to_start_get(m_dly_rx_data[i].id,
                                                                    &drx_time_to_start);
        drx_time_to_start += RX_SETUP_TIME_MAX + RX_RAMP_UP_TIME;

        if (result)
        {
            min_time_to_start = drx_time_to_start < min_time_to_start ?
                                (uint32_t)drx_time_to_start : min_time_to_start;
            drx_window_duration_time = m_dly_rx_data[i].rx.timeout_length -
                                       (RX_SETUP_TIME_MAX + RX_RAMP_UP_TIME);
            drx_time_to_midpoint = min_time_to_start + drx_window_duration_time / 2;
        }
    }

    if (result)
    {
        *p_drx_time_to_midpoint = drx_time_to_midpoint;
    }

    return result;
}

#endif // NRF_802154_DELAYED_TRX_ENABLED
