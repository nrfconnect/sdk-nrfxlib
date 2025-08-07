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
 *   This file implements requests to the driver triggered by the MAC layer through SWI.
 *
 */

#include "nrf_802154_request.h"

#if NRF_802154_REQUEST_IMPL == NRF_802154_REQUEST_IMPL_SWI

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_config.h"
#include "nrf_802154_core.h"
#include "nrf_802154_critical_section.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_queue.h"
#include "nrf_802154_rx_buffer.h"
#include "nrf_802154_swi.h"
#include "nrf_802154_utils.h"
#include "mac_features/nrf_802154_csma_ca.h"
#include "hal/nrf_radio.h"
#include "hal/nrf_egu.h"
#include "mac_features/nrf_802154_delayed_trx.h"
#include "platform/nrf_802154_irq.h"

#include <nrfx.h>

/** Size of requests queue.
 *
 * Two is minimal queue size. It is not expected in current implementation to queue a few requests.
 */
#define REQ_QUEUE_SIZE 2

#define REQ_INT        NRFX_CONCAT_2(NRF_EGU_INT_TRIGGERED, NRF_802154_EGU_REQUEST_CHANNEL_NO)
#define REQ_TASK       NRFX_CONCAT_2(NRF_EGU_TASK_TRIGGER, NRF_802154_EGU_REQUEST_CHANNEL_NO)
#define REQ_EVENT      NRFX_CONCAT_2(NRF_EGU_EVENT_TRIGGERED, NRF_802154_EGU_REQUEST_CHANNEL_NO)

/// Type of requests in request queue.
typedef enum
{
    REQ_TYPE_SLEEP,
    REQ_TYPE_RECEIVE,
    REQ_TYPE_TRANSMIT,
    REQ_TYPE_ACK_TIMEOUT_HANDLE,
    REQ_TYPE_ENERGY_DETECTION,
    REQ_TYPE_CCA,
    REQ_TYPE_CONTINUOUS_CARRIER,
    REQ_TYPE_MODULATED_CARRIER,
    REQ_TYPE_BUFFER_FREE,
    REQ_TYPE_CHANNEL_UPDATE,
    REQ_TYPE_CCA_CFG_UPDATE,
    REQ_TYPE_RSSI_MEASURE,
    REQ_TYPE_RSSI_GET,
    REQ_TYPE_ANTENNA_UPDATE,
    REQ_TYPE_TRANSMIT_AT,
    REQ_TYPE_TRANSMIT_AT_CANCEL,
    REQ_TYPE_RECEIVE_AT,
    REQ_TYPE_RECEIVE_AT_CANCEL,
    REQ_TYPE_RECEIVE_AT_SCHEDULED_CANCEL,
    REQ_TYPE_CSMA_CA_START,
} nrf_802154_req_type_t;

/// Request data in request queue.
typedef struct
{
    nrf_802154_req_type_t type; ///< Type of the request.

    union
    {
        struct
        {
            nrf_802154_term_t term_lvl; ///< Request priority.
            bool            * p_result; ///< Sleep request result.
        } sleep;                        ///< Sleep request details.

        struct
        {
            nrf_802154_notification_func_t notif_func;  ///< Error notified in case of success.
            nrf_802154_term_t              term_lvl;    ///< Request priority.
            req_originator_t               req_orig;    ///< Request originator.
            bool                           notif_abort; ///< If function termination should be notified.
            uint32_t                       id;          ///< Identifier of a reception window.
            bool                         * p_result;    ///< Receive request result.
        } receive;                                      ///< Receive request details.

        struct
        {
            nrf_802154_notification_func_t notif_func; ///< Error notified in case of success.
            nrf_802154_term_t              term_lvl;   ///< Request priority.
            req_originator_t               req_orig;   ///< Request originator.
            uint8_t                      * p_data;     ///< Pointer to a buffer containing PHR and PSDU of the frame to transmit.
            nrf_802154_transmit_params_t * p_params;   ///< Pointer to transmission parameters.
            bool                         * p_result;   ///< Transmit request result.
        } transmit;                                    ///< Transmit request details.

        struct
        {
            const nrf_802154_ack_timeout_handle_params_t * p_param;
            bool                                         * p_result;
        } ack_timeout_handle;

        struct
        {
            nrf_802154_term_t term_lvl; ///< Request priority.
            bool            * p_result; ///< Energy detection request result.
            uint32_t          time_us;  ///< Requested time of energy detection procedure.
        } energy_detection;             ///< Energy detection request details.

        struct
        {
            nrf_802154_term_t term_lvl; ///< Request priority.
            bool            * p_result; ///< CCA request result.
        } cca;                          ///< CCA request details.

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        struct
        {
            nrf_802154_term_t term_lvl; ///< Request priority.
            bool            * p_result; ///< Continuous carrier request result.
        } continuous_carrier;           ///< Continuous carrier request details.

        struct
        {
            nrf_802154_term_t term_lvl; ///< Request priority.
            const uint8_t   * p_data;   ///< Pointer to a buffer to modulate the carrier wave with.
            bool            * p_result; ///< Modulated carrier request result.
        } modulated_carrier;            ///< Modulated carrier request details.
#endif  // NRF_802154_CARRIER_FUNCTIONS_ENABLED

        struct
        {
            uint8_t * p_data;   ///< Pointer to receive buffer to free.
            bool    * p_result; ///< Buffer free request result.
        } buffer_free;          ///< Buffer free request details.

        struct
        {
            req_originator_t req_orig; ///< Request originator.
            bool           * p_result; ///< Channel update request result.
        } channel_update;              ///< Channel update request details.

        struct
        {
            bool * p_result; ///< CCA config update request result.
        } cca_cfg_update;    ///< CCA config update request details.

        struct
        {
            bool * p_result; ///< RSSI measurement request result.
        } rssi_measure;      ///< RSSI measurement request details.

        struct
        {
            int8_t * p_rssi;   ///< RSSI measurement result.
            bool   * p_result; ///< RSSI measurement status.
        } rssi_get;            ///< Details of the getter that retrieves the RSSI measurement result.

        struct
        {
            bool * p_result; ///< Antenna update request result.
        } antenna_update;    ///< Antenna update request details.

#if NRF_802154_DELAYED_TRX_ENABLED
        struct
        {
            uint8_t                                 * p_data;
            uint64_t                                  tx_time;
            const nrf_802154_transmit_at_metadata_t * p_metadata;
            bool                                    * p_result;
        } transmit_at;

        struct
        {
            bool * p_result;
        } transmit_at_cancel;

        struct
        {
            uint64_t rx_time;
            uint32_t timeout;
            uint8_t  channel;
            uint32_t id;
            bool   * p_result;
        } receive_at;

        struct
        {
            uint32_t id;
            bool   * p_result;
        } receive_at_cancel;

#endif // NRF_802154_DELAYED_TRX_ENABLED

        struct
        {
            uint8_t                                      * p_data;
            const nrf_802154_transmit_csma_ca_metadata_t * p_metadata;
            bool                                         * p_result;
        } csma_ca_start; ///< Antenna update request details.

    } data;              ///< Request data depending on its type.
} nrf_802154_req_data_t;

/**@brief Instance of a requests queue */
static nrf_802154_queue_t m_requests_queue;

/**@brief Memory holding requests queue items */
static nrf_802154_req_data_t m_requests_queue_memory[REQ_QUEUE_SIZE];

/**@brief State of the MCU critical section */
static volatile nrf_802154_mcu_critical_state_t m_mcu_cs;

/**
 * Enter request block.
 *
 * This is a helper function used in all request functions to atomically
 * find an empty slot in request queue and allow atomic slot update.
 *
 * @return Pointer to an empty slot in the request queue.
 */
static nrf_802154_req_data_t * req_enter(void)
{
    nrf_802154_mcu_critical_enter(m_mcu_cs);

    NRF_802154_ASSERT(!nrf_802154_queue_is_full(&m_requests_queue));

    return (nrf_802154_req_data_t *)nrf_802154_queue_push_begin(&m_requests_queue);
}

/**
 * Exit request block.
 *
 * This is a helper function used in all request functions to end atomic slot update
 * and trigger SWI to process the request from the slot.
 */
static void req_exit(void)
{
    nrf_802154_queue_push_commit(&m_requests_queue);

    nrf_egu_task_trigger(NRF_802154_EGU_INSTANCE, REQ_TASK);

    nrf_802154_mcu_critical_exit(m_mcu_cs);
}

/** Assert if SWI interrupt is disabled. */
static inline void assert_interrupt_status(void)
{
    NRF_802154_ASSERT(nrf_802154_irq_is_enabled(nrfx_get_irq_number(NRF_802154_EGU_INSTANCE)));
}

#define REQUEST_FUNCTION(func_core, func_swi, ...) \
    bool result = false;                           \
                                                   \
    if (active_vector_priority_is_high())          \
    {                                              \
        result = func_core(__VA_ARGS__);           \
    }                                              \
    else                                           \
    {                                              \
        assert_interrupt_status();                 \
        func_swi(__VA_ARGS__, &result);            \
    }                                              \
                                                   \
    return result;

#define REQUEST_FUNCTION_NO_ARGS(func_core, func_swi) \
    bool result = false;                              \
                                                      \
    if (active_vector_priority_is_high())             \
    {                                                 \
        result = func_core();                         \
    }                                                 \
    else                                              \
    {                                                 \
        assert_interrupt_status();                    \
        func_swi(&result);                            \
    }                                                 \
                                                      \
    return result;

/** Check if active vector priority is high enough to call requests directly.
 *
 *  @retval  true   Active vector priority is greater or equal to SWI priority.
 *  @retval  false  Active vector priority is lower than SWI priority.
 */
static bool active_vector_priority_is_high(void)
{

    return nrf_802154_critical_section_active_vector_priority_get() <=
           nrf_802154_irq_priority_get(nrfx_get_irq_number(NRF_802154_EGU_INSTANCE));
}

/**
 * @brief Requests entering the @ref RADIO_STATE_SLEEP state from the SWI priority.
 *
 * @param[in]   term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[out]  p_result  Result of entering the sleep state.
 */
static void swi_sleep(nrf_802154_term_t term_lvl, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                = REQ_TYPE_SLEEP;
    p_slot->data.sleep.term_lvl = term_lvl;
    p_slot->data.sleep.p_result = p_result;

    req_exit();
}

/**
 * @brief Requests entering the @ref RADIO_STATE_RX state from the SWI priority.
 *
 * @param[in]   term_lvl         Termination level of this request. Selects procedures to abort.
 * @param[in]   req_orig         Module that originates this request.
 * @param[in]   notify_function  Function called to notify the status of the procedure. May be NULL.
 * @param[in]   notify_abort     If abort notification should be triggered automatically.
 * @param[in]   id               Identifier of the reception window.
 * @param[out]  p_result         Result of entering the receive state.
 */
static void swi_receive(nrf_802154_term_t              term_lvl,
                        req_originator_t               req_orig,
                        nrf_802154_notification_func_t notify_function,
                        bool                           notify_abort,
                        uint32_t                       id,
                        bool                         * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                     = REQ_TYPE_RECEIVE;
    p_slot->data.receive.term_lvl    = term_lvl;
    p_slot->data.receive.req_orig    = req_orig;
    p_slot->data.receive.notif_func  = notify_function;
    p_slot->data.receive.notif_abort = notify_abort;
    p_slot->data.receive.id          = id;
    p_slot->data.receive.p_result    = p_result;

    req_exit();
}

/**
 * @biref Requests entering the @ref RADIO_STATE_TX state from the SWI priority.
 *
 * @param[in]   term_lvl         Termination level of this request. Selects procedures to abort.
 * @param[in]   req_orig         Module that originates this request.
 * @param[in]   p_data           Pointer to a buffer that contains PHR and PSDU of the frame to be
 *                               transmitted.
 * @param[in]   p_params         Pointer to transmission parameters.
 * @param[in]   notify_function  Function called to notify the status of this procedure instead of
 *                               the default notification. If NULL, the default notification
 *                               is used.
 * @param[out]  p_result         Result of entering the transmit state.
 */
static void swi_transmit(nrf_802154_term_t              term_lvl,
                         req_originator_t               req_orig,
                         uint8_t                      * p_data,
                         nrf_802154_transmit_params_t * p_params,
                         nrf_802154_notification_func_t notify_function,
                         bool                         * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                     = REQ_TYPE_TRANSMIT;
    p_slot->data.transmit.term_lvl   = term_lvl;
    p_slot->data.transmit.req_orig   = req_orig;
    p_slot->data.transmit.p_data     = p_data;
    p_slot->data.transmit.p_params   = p_params;
    p_slot->data.transmit.notif_func = notify_function;
    p_slot->data.transmit.p_result   = p_result;

    req_exit();
}

static void swi_ack_timeout_handle(const nrf_802154_ack_timeout_handle_params_t * p_param,
                                   bool                                         * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                             = REQ_TYPE_ACK_TIMEOUT_HANDLE;
    p_slot->data.ack_timeout_handle.p_param  = p_param;
    p_slot->data.ack_timeout_handle.p_result = p_result;

    req_exit();
}

/**
 * @brief Requests entering the @ref RADIO_STATE_ED state from the SWI priority.
 *
 * @param[in]   term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[in]   time_us   Requested duration of the energy detection procedure.
 * @param[out]  p_result  Result of entering the energy detection state.
 */
static void swi_energy_detection(nrf_802154_term_t term_lvl,
                                 uint32_t          time_us,
                                 bool            * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                           = REQ_TYPE_ENERGY_DETECTION;
    p_slot->data.energy_detection.term_lvl = term_lvl;
    p_slot->data.energy_detection.time_us  = time_us;
    p_slot->data.energy_detection.p_result = p_result;

    req_exit();
}

/**
 * @brief Requests entering the @ref RADIO_STATE_CCA state from the SWI priority.
 *
 * @param[in]   term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[out]  p_result  Result of entering the CCA state.
 */
static void swi_cca(nrf_802154_term_t term_lvl, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type              = REQ_TYPE_CCA;
    p_slot->data.cca.term_lvl = term_lvl;
    p_slot->data.cca.p_result = p_result;

    req_exit();
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Requests entering the @ref RADIO_STATE_CONTINUOUS_CARRIER state from the SWI priority.
 *
 * @param[in]   term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[out]  p_result  Result of entering the continuous carrier state.
 */
static void swi_continuous_carrier(nrf_802154_term_t term_lvl, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                             = REQ_TYPE_CONTINUOUS_CARRIER;
    p_slot->data.continuous_carrier.term_lvl = term_lvl;
    p_slot->data.continuous_carrier.p_result = p_result;

    req_exit();
}

/**
 * @brief Requests entering the @ref RADIO_STATE_MODULATED_CARRIER state from the SWI priority.
 *
 * @param[in]   term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[in]   p_data    Pointer to the data buffer to modulate the carrier wave with.
 * @param[out]  p_result  Result of entering the modulated carrier state.
 */
static void swi_modulated_carrier(nrf_802154_term_t term_lvl,
                                  const uint8_t   * p_data,
                                  bool            * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                            = REQ_TYPE_MODULATED_CARRIER;
    p_slot->data.modulated_carrier.term_lvl = term_lvl;
    p_slot->data.modulated_carrier.p_data   = p_data;
    p_slot->data.modulated_carrier.p_result = p_result;

    req_exit();
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Notifies the core module that the given buffer is not used anymore and can be freed.
 *
 * @param[in]  p_data   Pointer to the buffer to be freed.
 * @param[out] p_result Pointer where the result to be returned by
 *                      nrf_802154_request_buffer_free should be written by the swi handler.
 */
static void swi_buffer_free(uint8_t * p_data, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                      = REQ_TYPE_BUFFER_FREE;
    p_slot->data.buffer_free.p_data   = p_data;
    p_slot->data.buffer_free.p_result = p_result;

    req_exit();
}

/**
 * @brief Notifies the core module that the next higher layer has requested an antenna change.
 *
 * @param[out] p_result Pointer where the result to be returned by
 *                      nrf_802154_request_antenna_update should be written by the swi handler.
 */
static void swi_antenna_update(bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                         = REQ_TYPE_ANTENNA_UPDATE;
    p_slot->data.antenna_update.p_result = p_result;

    req_exit();
}

/**
 * @brief Notifies the core module that the next higher layer has requested a channel change.
 *
 * @param[out] p_result Pointer where the result to be returned by
 *                      nrf_802154_request_channel_update should be written by the swi handler.
 */
static void swi_channel_update(req_originator_t req_orig, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                         = REQ_TYPE_CHANNEL_UPDATE;
    p_slot->data.channel_update.p_result = p_result;
    p_slot->data.channel_update.req_orig = req_orig;

    req_exit();
}

/**
 * @brief Notifies the core module that the next higher layer has requested a CCA configuration
 * change.
 * @param[out] p_result Pointer where the result to be returned by
 *                      nrf_802154_request_cca_cfg_update should be written by the swi handler.
 */
static void swi_cca_cfg_update(bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                         = REQ_TYPE_CCA_CFG_UPDATE;
    p_slot->data.cca_cfg_update.p_result = p_result;

    req_exit();
}

/**
 * @brief Notifies the core module that the next higher layer requested the RSSI measurement.
 *
 * @param[out] p_result Pointer where the result to be returned by
 *                      nrf_802154_request_rssi_measure should be written by the swi handler.
 */
static void swi_rssi_measure(bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                       = REQ_TYPE_RSSI_MEASURE;
    p_slot->data.rssi_measure.p_result = p_result;

    req_exit();
}

/**
 * @brief Gets the last RSSI measurement result from the core module.
 *
 * @param[out] p_rssi   Pointer where RSSI measurement value will be stored.
 * @param[out] p_result Pointer where the result to be returned by
 *                      nrf_802154_request_rssi_measurement_get should be written by the swi handler.
 */
static void swi_rssi_measurement_get(int8_t * p_rssi, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                   = REQ_TYPE_RSSI_GET;
    p_slot->data.rssi_get.p_rssi   = p_rssi;
    p_slot->data.rssi_get.p_result = p_result;

    req_exit();
}

#if NRF_802154_DELAYED_TRX_ENABLED
static void swi_transmit_at(uint8_t                                 * p_data,
                            uint64_t                                  tx_time,
                            const nrf_802154_transmit_at_metadata_t * p_metadata,
                            bool                                    * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                        = REQ_TYPE_TRANSMIT_AT;
    p_slot->data.transmit_at.p_data     = p_data;
    p_slot->data.transmit_at.tx_time    = tx_time;
    p_slot->data.transmit_at.p_metadata = p_metadata;
    p_slot->data.transmit_at.p_result   = p_result;

    req_exit();
}

static void swi_transmit_at_cancel(bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                             = REQ_TYPE_TRANSMIT_AT_CANCEL;
    p_slot->data.transmit_at_cancel.p_result = p_result;

    req_exit();
}

static void swi_receive_at(uint64_t rx_time,
                           uint32_t timeout,
                           uint8_t  channel,
                           uint32_t id,
                           bool   * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                     = REQ_TYPE_RECEIVE_AT;
    p_slot->data.receive_at.rx_time  = rx_time;
    p_slot->data.receive_at.timeout  = timeout;
    p_slot->data.receive_at.channel  = channel;
    p_slot->data.receive_at.id       = id;
    p_slot->data.receive_at.p_result = p_result;

    req_exit();
}

static void swi_receive_at_cancel(uint32_t id, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                            = REQ_TYPE_RECEIVE_AT_CANCEL;
    p_slot->data.receive_at_cancel.id       = id;
    p_slot->data.receive_at_cancel.p_result = p_result;

    req_exit();
}

static void swi_receive_at_scheduled_cancel(uint32_t id, bool * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                            = REQ_TYPE_RECEIVE_AT_SCHEDULED_CANCEL;
    p_slot->data.receive_at_cancel.id       = id;
    p_slot->data.receive_at_cancel.p_result = p_result;

    req_exit();
}

#endif // NRF_802154_DELAYED_TRX_ENABLED

static void swi_csma_ca_start(uint8_t                                      * p_data,
                              const nrf_802154_transmit_csma_ca_metadata_t * p_metadata,
                              bool                                         * p_result)
{
    nrf_802154_req_data_t * p_slot = req_enter();

    p_slot->type                          = REQ_TYPE_CSMA_CA_START;
    p_slot->data.csma_ca_start.p_data     = p_data;
    p_slot->data.csma_ca_start.p_metadata = p_metadata;
    p_slot->data.csma_ca_start.p_result   = p_result;
    req_exit();
}

void nrf_802154_request_init(void)
{
    nrf_802154_queue_init(&m_requests_queue,
                          m_requests_queue_memory,
                          sizeof(m_requests_queue_memory),
                          sizeof(m_requests_queue_memory[0]));

    nrf_egu_int_enable(NRF_802154_EGU_INSTANCE, REQ_INT);
}

bool nrf_802154_request_sleep(nrf_802154_term_t term_lvl)
{
    REQUEST_FUNCTION(nrf_802154_core_sleep, swi_sleep, term_lvl)
}

bool nrf_802154_request_receive(nrf_802154_term_t              term_lvl,
                                req_originator_t               req_orig,
                                nrf_802154_notification_func_t notify_function,
                                bool                           notify_abort,
                                uint32_t                       id)
{
    REQUEST_FUNCTION(nrf_802154_core_receive,
                     swi_receive,
                     term_lvl,
                     req_orig,
                     notify_function,
                     notify_abort,
                     id)
}

bool nrf_802154_request_transmit(nrf_802154_term_t              term_lvl,
                                 req_originator_t               req_orig,
                                 uint8_t                      * p_data,
                                 nrf_802154_transmit_params_t * p_params,
                                 nrf_802154_notification_func_t notify_function)
{
    REQUEST_FUNCTION(nrf_802154_core_transmit,
                     swi_transmit,
                     term_lvl,
                     req_orig,
                     p_data,
                     p_params,
                     notify_function)
}

bool nrf_802154_request_ack_timeout_handle(const nrf_802154_ack_timeout_handle_params_t * p_param)
{
    REQUEST_FUNCTION(nrf_802154_core_ack_timeout_handle, swi_ack_timeout_handle, p_param);
}

bool nrf_802154_request_energy_detection(nrf_802154_term_t term_lvl,
                                         uint32_t          time_us)
{
    REQUEST_FUNCTION(nrf_802154_core_energy_detection,
                     swi_energy_detection,
                     term_lvl,
                     time_us)
}

bool nrf_802154_request_cca(nrf_802154_term_t term_lvl)
{
    REQUEST_FUNCTION(nrf_802154_core_cca, swi_cca, term_lvl)
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
bool nrf_802154_request_continuous_carrier(nrf_802154_term_t term_lvl)
{
    REQUEST_FUNCTION(nrf_802154_core_continuous_carrier,
                     swi_continuous_carrier,
                     term_lvl)
}

bool nrf_802154_request_modulated_carrier(nrf_802154_term_t term_lvl,
                                          const uint8_t   * p_data)
{
    REQUEST_FUNCTION(nrf_802154_core_modulated_carrier,
                     swi_modulated_carrier,
                     term_lvl,
                     p_data)
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

bool nrf_802154_request_buffer_free(uint8_t * p_data)
{
    REQUEST_FUNCTION(nrf_802154_core_notify_buffer_free, swi_buffer_free, p_data)
}

bool nrf_802154_request_antenna_update(void)
{
    REQUEST_FUNCTION_NO_ARGS(nrf_802154_core_antenna_update, swi_antenna_update)
}

bool nrf_802154_request_channel_update(req_originator_t req_orig)
{
    REQUEST_FUNCTION(nrf_802154_core_channel_update, swi_channel_update, req_orig)
}

bool nrf_802154_request_cca_cfg_update(void)
{
    REQUEST_FUNCTION_NO_ARGS(nrf_802154_core_cca_cfg_update, swi_cca_cfg_update)
}

bool nrf_802154_request_rssi_measure(void)
{
    REQUEST_FUNCTION_NO_ARGS(nrf_802154_core_rssi_measure, swi_rssi_measure)
}

bool nrf_802154_request_rssi_measurement_get(int8_t * p_rssi)
{
    REQUEST_FUNCTION(nrf_802154_core_last_rssi_measurement_get,
                     swi_rssi_measurement_get,
                     p_rssi)
}

#if NRF_802154_DELAYED_TRX_ENABLED
bool nrf_802154_request_transmit_raw_at(uint8_t                                 * p_data,
                                        uint64_t                                  tx_time,
                                        const nrf_802154_transmit_at_metadata_t * p_metadata)
{
    REQUEST_FUNCTION(nrf_802154_delayed_trx_transmit, swi_transmit_at, p_data, tx_time, p_metadata);
}

bool nrf_802154_request_transmit_at_cancel(void)
{
    REQUEST_FUNCTION_NO_ARGS(nrf_802154_delayed_trx_transmit_cancel, swi_transmit_at_cancel);
}

bool nrf_802154_request_receive_at(uint64_t rx_time,
                                   uint32_t timeout,
                                   uint8_t  channel,
                                   uint32_t id)
{
    REQUEST_FUNCTION(nrf_802154_delayed_trx_receive, swi_receive_at, rx_time, timeout, channel, id);
}

bool nrf_802154_request_receive_at_cancel(uint32_t id)
{
    REQUEST_FUNCTION(nrf_802154_delayed_trx_receive_cancel, swi_receive_at_cancel, id);
}

bool nrf_802154_request_receive_at_scheduled_cancel(uint32_t id)
{
    REQUEST_FUNCTION(nrf_802154_delayed_trx_receive_scheduled_cancel,
                     swi_receive_at_scheduled_cancel,
                     id);
}

bool nrf_802154_request_csma_ca_start(uint8_t                                      * p_data,
                                      const nrf_802154_transmit_csma_ca_metadata_t * p_metadata)
{
    REQUEST_FUNCTION(nrf_802154_csma_ca_start,
                     swi_csma_ca_start,
                     p_data,
                     p_metadata);
}

#endif // NRF_802154_DELAYED_TRX_ENABLED

/**@brief Handles REQ_EVENT on NRF_802154_EGU_INSTANCE */
static void irq_handler_req_event(void)
{
    while (!nrf_802154_queue_is_empty(&m_requests_queue))
    {
        nrf_802154_req_data_t * p_slot =
            (nrf_802154_req_data_t *)nrf_802154_queue_pop_begin(&m_requests_queue);

        switch (p_slot->type)
        {
            case REQ_TYPE_SLEEP:
                *(p_slot->data.sleep.p_result) =
                    nrf_802154_core_sleep(p_slot->data.sleep.term_lvl);
                break;

            case REQ_TYPE_RECEIVE:
                *(p_slot->data.receive.p_result) =
                    nrf_802154_core_receive(p_slot->data.receive.term_lvl,
                                            p_slot->data.receive.req_orig,
                                            p_slot->data.receive.notif_func,
                                            p_slot->data.receive.notif_abort,
                                            p_slot->data.receive.id);
                break;

            case REQ_TYPE_TRANSMIT:
                *(p_slot->data.transmit.p_result) =
                    nrf_802154_core_transmit(p_slot->data.transmit.term_lvl,
                                             p_slot->data.transmit.req_orig,
                                             p_slot->data.transmit.p_data,
                                             p_slot->data.transmit.p_params,
                                             p_slot->data.transmit.notif_func);
                break;

            case REQ_TYPE_ACK_TIMEOUT_HANDLE:
                *(p_slot->data.ack_timeout_handle.p_result) =
                    nrf_802154_core_ack_timeout_handle(p_slot->data.ack_timeout_handle.p_param);
                break;

            case REQ_TYPE_ENERGY_DETECTION:
                *(p_slot->data.energy_detection.p_result) =
                    nrf_802154_core_energy_detection(
                        p_slot->data.energy_detection.term_lvl,
                        p_slot->data.energy_detection.time_us);
                break;

            case REQ_TYPE_CCA:
                *(p_slot->data.cca.p_result) = nrf_802154_core_cca(p_slot->data.cca.term_lvl);
                break;

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

            case REQ_TYPE_CONTINUOUS_CARRIER:
                *(p_slot->data.continuous_carrier.p_result) =
                    nrf_802154_core_continuous_carrier(
                        p_slot->data.continuous_carrier.term_lvl);
                break;

            case REQ_TYPE_MODULATED_CARRIER:
                *(p_slot->data.modulated_carrier.p_result) =
                    nrf_802154_core_modulated_carrier(p_slot->data.modulated_carrier.term_lvl,
                                                      p_slot->data.modulated_carrier.p_data);
                break;

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

            case REQ_TYPE_BUFFER_FREE:
                *(p_slot->data.buffer_free.p_result) =
                    nrf_802154_core_notify_buffer_free(p_slot->data.buffer_free.p_data);
                break;

            case REQ_TYPE_CHANNEL_UPDATE:
                *(p_slot->data.channel_update.p_result) =
                    nrf_802154_core_channel_update(p_slot->data.channel_update.req_orig);
                break;

            case REQ_TYPE_CCA_CFG_UPDATE:
                *(p_slot->data.cca_cfg_update.p_result) = nrf_802154_core_cca_cfg_update();
                break;

            case REQ_TYPE_RSSI_MEASURE:
                *(p_slot->data.rssi_measure.p_result) = nrf_802154_core_rssi_measure();
                break;

            case REQ_TYPE_RSSI_GET:
                *(p_slot->data.rssi_get.p_result) =
                    nrf_802154_core_last_rssi_measurement_get(p_slot->data.rssi_get.p_rssi);
                break;

            case REQ_TYPE_ANTENNA_UPDATE:
                *(p_slot->data.antenna_update.p_result) = nrf_802154_core_antenna_update();
                break;

#if NRF_802154_DELAYED_TRX_ENABLED
            case REQ_TYPE_TRANSMIT_AT:
                *(p_slot->data.transmit_at.p_result) =
                    nrf_802154_delayed_trx_transmit(p_slot->data.transmit_at.p_data,
                                                    p_slot->data.transmit_at.tx_time,
                                                    p_slot->data.transmit_at.p_metadata);
                break;

            case REQ_TYPE_TRANSMIT_AT_CANCEL:
                *(p_slot->data.transmit_at_cancel.p_result) =
                    nrf_802154_delayed_trx_transmit_cancel();
                break;

            case REQ_TYPE_RECEIVE_AT:
                *(p_slot->data.receive_at.p_result) =
                    nrf_802154_delayed_trx_receive(p_slot->data.receive_at.rx_time,
                                                   p_slot->data.receive_at.timeout,
                                                   p_slot->data.receive_at.channel,
                                                   p_slot->data.receive_at.id);
                break;

            case REQ_TYPE_RECEIVE_AT_CANCEL:
                *(p_slot->data.receive_at_cancel.p_result) =
                    nrf_802154_delayed_trx_receive_cancel(p_slot->data.receive_at_cancel.id);
                break;

            case REQ_TYPE_RECEIVE_AT_SCHEDULED_CANCEL:
                *(p_slot->data.receive_at_cancel.p_result) =
                    nrf_802154_delayed_trx_receive_scheduled_cancel(
                        p_slot->data.receive_at_cancel.id);
                break;

            case REQ_TYPE_CSMA_CA_START:
                *(p_slot->data.csma_ca_start.p_result) =
                    nrf_802154_csma_ca_start(p_slot->data.csma_ca_start.p_data,
                                             p_slot->data.csma_ca_start.p_metadata);
                break;
#endif // NRF_802154_DELAYED_TRX_ENABLED

            default:
                NRF_802154_ASSERT(false);
        }

        nrf_802154_queue_pop_commit(&m_requests_queue);
    }
}

void nrf_802154_request_swi_irq_handler(void)
{
    if (nrf_egu_event_check(NRF_802154_EGU_INSTANCE, REQ_EVENT))
    {
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, REQ_EVENT);

        irq_handler_req_event();
    }
}

#endif /* NRF_802154_REQUEST_IMPL == NRF_802154_REQUEST_IMPL_SWI */
