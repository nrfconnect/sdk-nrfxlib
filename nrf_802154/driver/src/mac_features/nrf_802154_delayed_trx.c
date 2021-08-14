/*
 * Copyright (c) 2018 - 2021, Nordic Semiconductor ASA
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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

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
#include "rsch/nrf_802154_rsch.h"
#include "timer/nrf_802154_timer_sched.h"

#if NRF_802154_DELAYED_TRX_ENABLED

/* The following time is the sum of 70us RTC_IRQHandler processing time, 40us of time that elapses
 * from the moment a board starts transmission to the moment other boards (e.g. sniffer) are able
 * to detect that frame and in case of TX - 93us that accounts for a delay of yet unknown origin.
 */
#define TX_SETUP_TIME 203u ///< Time needed to prepare TX procedure [us]. It does not include TX ramp-up time.
#define RX_SETUP_TIME 110u ///< Time needed to prepare RX procedure [us]. It does not include RX ramp-up time.

/**
 * @brief States of delayed operations.
 */
typedef enum
{
    DELAYED_TRX_OP_STATE_STOPPED, ///< Delayed operation stopped.
    DELAYED_TRX_OP_STATE_PENDING, ///< Delayed operation scheduled and waiting for timeslot.
    DELAYED_TRX_OP_STATE_ONGOING, ///< Delayed operation ongoing (during timeslot).
    DELAYED_TRX_OP_STATE_NB       ///< Number of delayed operation states.
} delayed_trx_op_state_t;

/**
 * @brief RX delayed operation frame data.
 */
typedef struct
{
    uint32_t sof_timestamp; ///< Timestamp of last start of frame notification received in RX window.
    uint8_t  psdu_length;   ///< Length in bytes of the frame to be received in RX window.
    bool     ack_requested; ///< Flag indicating if Ack for the frame to be received in RX window is requested.
} delayed_rx_frame_data_t;

/**
 * @brief RX delayed operation data.
 */
typedef struct
{
    nrf_802154_timer_t               timeout_timer;   ///< Timer for delayed RX timeout handling.
    volatile delayed_rx_frame_data_t extension_frame; ///< Data of frame that caused extension of RX window.
    uint8_t                          channel;         ///< Channel number on which reception should be performed.
} dly_rx_data_t;

/**
 * @brief TX delayed operation data.
 */
typedef struct
{
    uint8_t                    * p_data;  ///< Pointer to a buffer containing PHR and PSDU of the frame requested to be transmitted.
    nrf_802154_transmit_params_t params;  ///< Transmission parameters.
    uint8_t                      channel; ///< Channel number on which transmission should be performed.
} dly_tx_data_t;

/**
 * @brief Delayed operation data.
 */
typedef struct
{
    volatile delayed_trx_op_state_t state; ///< State of the delayed timeslot.
    rsch_dly_ts_id_t                id;    ///< Identifier of the delayed timeslot.
    rsch_dly_ts_op_t                op;    ///< Type of delayed operation to be performed.

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
            if ((p_dly_op_data == NULL))
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
                assert(false);
            }
        }
    }

    return p_dly_op_data;
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
    assert(id == m_dly_tx_data[0].id);

    return &m_dly_tx_data[0];
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
        if (p_dly_op_data_pool[i].id == NRF_802154_RESERVED_INVALID_ID)
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

static void dly_ts_slot_release(dly_op_data_t * p_dly_op_data)
{
    (void)nrf_802154_rsch_delayed_timeslot_cancel(p_dly_op_data->id);

    p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;
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

    assert(!nrf_802154_queue_is_full(&m_dly_rx_id_q));

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
 * Set state of a delayed operation.
 *
 * @param[in]  p_dly_op_data  Data of the delayed operation.
 * @param[in]  expected_state Expected delayed operation state prior state transition.
 * @param[in]  new_state      Delayed operation state to enter.
 *
 * @retval true   Successfully set the new state.
 * @retval false  Failed to set the new state.
 */
static bool dly_op_state_set(dly_op_data_t        * p_dly_op_data,
                             delayed_trx_op_state_t expected_state,
                             delayed_trx_op_state_t new_state)
{
    assert(new_state < DELAYED_TRX_OP_STATE_NB);

    switch (p_dly_op_data->op)
    {
        case RSCH_DLY_TS_OP_DTX:
        case RSCH_DLY_TS_OP_DRX:
        {
            volatile delayed_trx_op_state_t current_state;

            do
            {
                current_state = (delayed_trx_op_state_t)__LDREXB((uint8_t *)&p_dly_op_data->state);

                if (current_state != expected_state)
                {
                    __CLREX();
                    return false;
                }

            }
            while (__STREXB((uint8_t)new_state, (uint8_t *)&p_dly_op_data->state));

            __DMB();

            return true;
        }

        default:
        {
            assert(false);
            return false;
        }
    }
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
    // Set PENDING state before timeslot request, in case timeslot starts
    // immediately and interrupts current function execution.
    bool state_set = dly_op_state_set(
        p_dly_op_data,
        DELAYED_TRX_OP_STATE_STOPPED,
        DELAYED_TRX_OP_STATE_PENDING);

    assert(state_set);

    bool result = nrf_802154_rsch_delayed_timeslot_request(p_dly_ts_param);

    if (!result)
    {
        state_set = dly_op_state_set(
            p_dly_op_data,
            DELAYED_TRX_OP_STATE_PENDING,
            DELAYED_TRX_OP_STATE_STOPPED);

        // In case of failure, the value of this parameter is undefined. Reset it
        p_dly_op_data->id = NRF_802154_RESERVED_INVALID_ID;
    }

    (void)state_set;

    return result;
}

/**
 * Notify MAC layer that no frame was received before timeout.
 *
 * @param[in]  p_context  Not used.
 */
static void notify_rx_timeout(void * p_context)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    dly_op_data_t * p_dly_op_data = (dly_op_data_t *)p_context;

    assert(p_dly_op_data->state != DELAYED_TRX_OP_STATE_PENDING);

    if (p_dly_op_data->state == DELAYED_TRX_OP_STATE_ONGOING)
    {
        uint32_t now           = nrf_802154_timer_sched_time_get();
        uint32_t sof_timestamp = p_dly_op_data->rx.extension_frame.sof_timestamp;

        // Make sure that the timestamp has been latched safely. If frame reception preempts the code
        // after executing this line, the RX window will not be extended.
        __DMB();
        uint8_t  psdu_length   = p_dly_op_data->rx.extension_frame.psdu_length;
        bool     ack_requested = p_dly_op_data->rx.extension_frame.ack_requested;
        uint32_t frame_length  = nrf_802154_rx_duration_get(psdu_length, ack_requested);

        if (nrf_802154_timer_sched_time_is_in_future(now, sof_timestamp, frame_length))
        {
            // @TODO protect against infinite extensions - allow only one timer extension
            p_dly_op_data->rx.timeout_timer.t0 = sof_timestamp;
            p_dly_op_data->rx.timeout_timer.dt = frame_length;

            nrf_802154_timer_sched_add(&p_dly_op_data->rx.timeout_timer, true);
        }
        else
        {
            if (dly_op_state_set(p_dly_op_data,
                                 DELAYED_TRX_OP_STATE_ONGOING,
                                 DELAYED_TRX_OP_STATE_STOPPED))
            {
                nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_DELAYED_TIMEOUT,
                                                 p_dly_op_data->id);
                dly_ts_slot_release(p_dly_op_data);
            }

            // even if the set operation failed, the delayed RX state
            // should be set to STOPPED from other context anyway
            assert(p_dly_op_data->state == DELAYED_TRX_OP_STATE_STOPPED);
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
    dly_op_data_t * p_dly_op_data = &m_dly_tx_data[0];

    // To avoid attaching to every possible transmit hook, in order to be able
    // to switch from ONGOING to STOPPED state, ONGOING state is not used at all
    // and state is changed to STOPPED right after transmit request.
    p_dly_op_data->state = DELAYED_TRX_OP_STATE_STOPPED;

    if (!result)
    {
        nrf_802154_notify_transmit_failed(p_dly_op_data->tx.p_data,
                                          NRF_802154_TX_ERROR_TIMESLOT_DENIED);
    }

    dly_ts_slot_release(p_dly_op_data);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
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
        assert(false);
        return;
    }

    if (result)
    {
        bool            state_set;
        uint32_t        now;
        dly_op_data_t * p_parallel_ongoing_dly_op_data = ongoing_dly_rx_slot_get();

        if (p_parallel_ongoing_dly_op_data != NULL)
        {
            // There's another ongoing delayed RX operation. Close it now
            state_set = dly_op_state_set(p_parallel_ongoing_dly_op_data,
                                         DELAYED_TRX_OP_STATE_ONGOING,
                                         DELAYED_TRX_OP_STATE_STOPPED);

            assert(state_set);
            (void)state_set;

            nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_DELAYED_ABORTED,
                                             p_parallel_ongoing_dly_op_data->id);
            dly_ts_slot_release(p_parallel_ongoing_dly_op_data);
        }

        state_set = dly_op_state_set(
            p_dly_op_data,
            DELAYED_TRX_OP_STATE_PENDING,
            DELAYED_TRX_OP_STATE_ONGOING);

        assert(state_set);
        (void)state_set;

        now = nrf_802154_timer_sched_time_get();

        p_dly_op_data->rx.timeout_timer.t0              = now;
        p_dly_op_data->rx.timeout_timer.p_context       = p_dly_op_data;
        p_dly_op_data->rx.extension_frame.sof_timestamp = now;
        p_dly_op_data->rx.extension_frame.psdu_length   = 0;
        p_dly_op_data->rx.extension_frame.ack_requested = false;

        nrf_802154_timer_sched_add(&p_dly_op_data->rx.timeout_timer, true);
    }
    else
    {
        bool state_set = dly_op_state_set(
            p_dly_op_data,
            DELAYED_TRX_OP_STATE_PENDING,
            DELAYED_TRX_OP_STATE_STOPPED);

        assert(state_set);
        (void)state_set;

        nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_DELAYED_TIMESLOT_DENIED,
                                         p_dly_op_data->id);
        dly_ts_slot_release(p_dly_op_data);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void transmit_attempt(dly_op_data_t * p_dly_op_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // No need to enqueue transmit attempts. Proceed to transmission immediately
    nrf_802154_pib_channel_set(p_dly_op_data->tx.channel);

    if (nrf_802154_request_channel_update(REQ_ORIG_DELAYED_TRX))
    {
        (void)nrf_802154_request_transmit(NRF_802154_TERM_802154,
                                          REQ_ORIG_DELAYED_TRX,
                                          p_dly_op_data->tx.p_data,
                                          &p_dly_op_data->tx.params,
                                          dly_tx_result_notify);
    }
    else
    {
        dly_tx_result_notify(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void receive_attempt(dly_op_data_t * p_dly_op_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // This function is expected to result in calling @ref dly_rx_result_notify.
    // In order for that function to differentiate between different delayed RX
    // windows, we atomically insert the ID of the current delayed RX into a FIFO queue.
    dly_rx_data_atomically_push(p_dly_op_data);

    nrf_802154_pib_channel_set(p_dly_op_data->rx.channel);

    if (nrf_802154_request_channel_update(REQ_ORIG_DELAYED_TRX))
    {
        (void)nrf_802154_request_receive(NRF_802154_TERM_802154,
                                         REQ_ORIG_DELAYED_TRX,
                                         dly_rx_result_notify,
                                         true,
                                         p_dly_op_data->id);
    }
    else
    {
        dly_rx_result_notify(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
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

    if (p_dly_op_data == NULL)
    {
        assert(false);
        return;
    }

    switch (p_dly_op_data->state)
    {
        case DELAYED_TRX_OP_STATE_PENDING:
            transmit_attempt(p_dly_op_data);
            break;

        case DELAYED_TRX_OP_STATE_STOPPED:
            // TODO: is it certain that p_dly_op_data->id has a valid value here?
            dly_ts_slot_release(p_dly_op_data);
            break;

        default:
            assert(false);
            break;
    }

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
        assert(false);
        return;
    }

    switch (p_dly_op_data->state)
    {
        case DELAYED_TRX_OP_STATE_PENDING:
            receive_attempt(p_dly_op_data);
            break;

        case DELAYED_TRX_OP_STATE_STOPPED:
            // TODO: is it certain that p_dly_op_data->id has a valid value here?
            dly_ts_slot_release(p_dly_op_data);
            break;

        default:
            assert(false);
            break;
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
        m_dly_rx_data[i].id = NRF_802154_RESERVED_INVALID_ID;
    }

    for (uint32_t i = 0; i < sizeof(m_dly_tx_data) / sizeof(m_dly_tx_data[0]); i++)
    {
        m_dly_tx_data[i].id = NRF_802154_RESERVED_INVALID_ID;
    }
}

bool nrf_802154_delayed_trx_transmit(uint8_t                                 * p_data,
                                     uint32_t                                  t0,
                                     uint32_t                                  dt,
                                     const nrf_802154_transmit_at_metadata_t * p_metadata)
{
    dly_op_data_t * p_dly_tx_data = available_dly_tx_slot_get();
    bool            result        = false;

    if (p_dly_tx_data != NULL)
    {
        dt -= TX_SETUP_TIME;
        dt -= TX_RAMP_UP_TIME;

        if (p_metadata->cca)
        {
            dt -= nrf_802154_cca_before_tx_duration_get();
        }

        p_dly_tx_data->op = RSCH_DLY_TS_OP_DTX;

        p_dly_tx_data->tx.p_data             = p_data;
        p_dly_tx_data->tx.params.frame_props = p_metadata->frame_props;
        p_dly_tx_data->tx.params.cca         = p_metadata->cca;
        p_dly_tx_data->tx.params.immediate   = true;
        p_dly_tx_data->tx.channel            = p_metadata->channel;
        p_dly_tx_data->id                    = NRF_802154_RESERVED_DTX_ID;

        rsch_dly_ts_param_t dly_ts_param =
        {
            .t0               = t0,
            .dt               = dt,
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

bool nrf_802154_delayed_trx_receive(uint32_t t0,
                                    uint32_t dt,
                                    uint32_t timeout,
                                    uint8_t  channel,
                                    uint32_t id)
{
    dly_op_data_t * p_dly_rx_data = available_dly_rx_slot_get();
    bool            result        = false;

    if (p_dly_rx_data != NULL)
    {
        dt -= RX_SETUP_TIME;
        dt -= RX_RAMP_UP_TIME;

        p_dly_rx_data->op = RSCH_DLY_TS_OP_DRX;

        p_dly_rx_data->rx.timeout_timer.dt       = timeout + RX_RAMP_UP_TIME;
        p_dly_rx_data->rx.timeout_timer.callback = notify_rx_timeout;

        p_dly_rx_data->rx.channel = channel;
        p_dly_rx_data->id         = id;

        // Remove timer in case it was left after abort operation
        nrf_802154_timer_sched_remove(&p_dly_rx_data->rx.timeout_timer, NULL);

        rsch_dly_ts_param_t dly_ts_param =
        {
            .t0               = t0,
            .dt               = dt,
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

    if (p_dly_op_data->id != NRF_802154_RESERVED_INVALID_ID)
    {
        result = nrf_802154_rsch_delayed_timeslot_cancel(p_dly_op_data->id);

        p_dly_op_data->state = DELAYED_TRX_OP_STATE_STOPPED;
        p_dly_op_data->id    = NRF_802154_RESERVED_INVALID_ID;
    }

    return result;
}

bool nrf_802154_delayed_trx_receive_cancel(uint32_t id)
{
    dly_op_data_t * p_dly_op_data = dly_rx_data_by_id_search(id);

    if (p_dly_op_data == NULL)
    {
        // Delayed receive window with provided ID could not be found.
        return false;
    }

    bool result      = nrf_802154_rsch_delayed_timeslot_cancel(id);
    bool was_running = false;

    nrf_802154_timer_sched_remove(&p_dly_op_data->rx.timeout_timer, &was_running);

    p_dly_op_data->state = DELAYED_TRX_OP_STATE_STOPPED;
    p_dly_op_data->id    = NRF_802154_RESERVED_INVALID_ID;

    result = result || was_running;

    return result;
}

bool nrf_802154_delayed_trx_abort(nrf_802154_term_t term_lvl, req_originator_t req_orig)
{
    bool            result        = true;
    dly_op_data_t * p_dly_op_data = ongoing_dly_rx_slot_get();

    if (p_dly_op_data != NULL)
    {
        if (term_lvl >= NRF_802154_TERM_802154)
        {
            if (dly_op_state_set(p_dly_op_data,
                                 DELAYED_TRX_OP_STATE_ONGOING,
                                 DELAYED_TRX_OP_STATE_STOPPED))
            {
                nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_DELAYED_ABORTED,
                                                 p_dly_op_data->id);
                dly_ts_slot_release(p_dly_op_data);
            }

            // Even if the set operation failed, the delayed RX state
            // should be set to STOPPED from other context anyway
            assert(p_dly_op_data->state == DELAYED_TRX_OP_STATE_STOPPED);
        }
        else
        {
            result = false;
        }
    }

    return result;
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
        p_dly_op_data->rx.extension_frame.sof_timestamp = nrf_802154_timer_sched_time_get();
        p_dly_op_data->rx.extension_frame.psdu_length   = p_frame[PHR_OFFSET];
        p_dly_op_data->rx.extension_frame.ack_requested = nrf_802154_frame_parser_ar_bit_is_set(
            &frame_data);
    }
}

bool nrf_802154_delayed_trx_nearest_drx_time_to_midpoint_get(uint32_t * p_drx_time_to_midpoint)
{
    bool     result            = false;
    uint32_t min_time_to_start = 0xffffffff;
    uint32_t drx_time_to_start;
    uint32_t drx_time_to_midpoint;

    for (int i = 0; i < sizeof(m_dly_rx_data) / sizeof(m_dly_rx_data[0]); i++)
    {
        if (m_dly_rx_data[i].state != DELAYED_TRX_OP_STATE_PENDING)
        {
            continue;
        }

        result = nrf_802154_rsch_delayed_timeslot_time_to_start_get(m_dly_rx_data[i].id,
                                                                    &drx_time_to_start);

        if (result)
        {
            min_time_to_start = drx_time_to_start <
                                min_time_to_start ? drx_time_to_start : min_time_to_start;
            drx_time_to_midpoint = min_time_to_start + m_dly_rx_data[i].rx.timeout_timer.dt / 2;
        }
    }

    if (result)
    {
        *p_drx_time_to_midpoint = drx_time_to_midpoint;
    }

    return result;
}

#endif // NRF_802154_DELAYED_TRX_ENABLED
