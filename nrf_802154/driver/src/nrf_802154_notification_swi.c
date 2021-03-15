/*
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
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
 *   This file implements notifications triggered by the nrf 802.15.4 radio driver via SWI.
 *
 */

#include "nrf_802154_notification.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154.h"
#include "nrf_802154_config.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_queue.h"
#include "nrf_802154_swi.h"
#include "nrf_802154_utils.h"
#include "hal/nrf_egu.h"

/** Size of notification queue.
 *
 * One slot for each receive buffer, one for transmission, one for busy channel and one for energy
 * detection.
 *
 * One slot is lost due to simplified queue implementation.
 */
#define NTF_QUEUE_SIZE ((NRF_802154_RX_BUFFERS + 3) + 1)

#define NTF_INT        NRF_EGU_INT_TRIGGERED0   ///< Label of notification interrupt.
#define NTF_TASK       NRF_EGU_TASK_TRIGGER0    ///< Label of notification task.
#define NTF_EVENT      NRF_EGU_EVENT_TRIGGERED0 ///< Label of notification event.

/// Types of notifications in notification queue.
typedef enum
{
    NTF_TYPE_RECEIVED,                ///< Frame received
    NTF_TYPE_RECEIVE_FAILED,          ///< Frame reception failed
    NTF_TYPE_TRANSMITTED,             ///< Frame transmitted
    NTF_TYPE_TRANSMIT_FAILED,         ///< Frame transmission failure
    NTF_TYPE_ENERGY_DETECTED,         ///< Energy detection procedure ended
    NTF_TYPE_ENERGY_DETECTION_FAILED, ///< Energy detection procedure failed
    NTF_TYPE_CCA,                     ///< CCA procedure ended
    NTF_TYPE_CCA_FAILED,              ///< CCA procedure failed
} nrf_802154_ntf_type_t;

/// Notification data in the notification queue.
typedef struct
{
    nrf_802154_ntf_type_t type; ///< Notification type.

    union
    {
        struct
        {
            uint8_t * p_data; ///< Pointer to a buffer containing PHR and PSDU of the received frame.
            int8_t    power;  ///< RSSI of received frame.
            uint8_t   lqi;    ///< LQI of received frame.
        } received;           ///< Received frame details.

        struct
        {
            nrf_802154_rx_error_t error; ///< An error code that indicates reason of the failure.
        } receive_failed;

        struct
        {
            const uint8_t * p_frame; ///< Pointer to frame that was transmitted.
            uint8_t       * p_ack;   ///< Pointer to a buffer containing PHR and PSDU of the received ACK or NULL.
            int8_t          power;   ///< RSSI of received ACK or 0.
            uint8_t         lqi;     ///< LQI of received ACK or 0.
        } transmitted;               ///< Transmitted frame details.

        struct
        {
            const uint8_t       * p_frame; ///< Pointer to frame that was requested to be transmitted, but failed.
            nrf_802154_tx_error_t error;   ///< An error code that indicates reason of the failure.
        } transmit_failed;

        struct
        {
            int8_t result; ///< Energy detection result.
        } energy_detected; ///< Energy detection details.

        struct
        {
            nrf_802154_ed_error_t error; ///< An error code that indicates reason of the failure.
        } energy_detection_failed;       ///< Energy detection failure details.

        struct
        {
            bool result; ///< CCA result.
        } cca;           ///< CCA details.

        struct
        {
            nrf_802154_cca_error_t error; ///< An error code that indicates reason of the failure.
        } cca_failed;                     ///< CCA failure details.
    } data;                               ///< Notification data depending on it's type.
} nrf_802154_ntf_data_t;

static nrf_802154_queue_t    m_notifications_queue;
static nrf_802154_ntf_data_t m_notifications_queue_memory[NTF_QUEUE_SIZE];

static volatile nrf_802154_mcu_critical_state_t m_mcu_cs;

/**
 * Enter notify block.
 *
 * This is a helper function used in all notification functions to atomically
 * find an empty slot in the notification queue and allow atomic slot update.
 *
 * @return Pointer to an empty slot in the notification queue.
 */
static nrf_802154_ntf_data_t * ntf_enter(void)
{
    nrf_802154_mcu_critical_enter(m_mcu_cs);

    assert(!nrf_802154_queue_is_full(&m_notifications_queue));

    return nrf_802154_queue_push_begin(&m_notifications_queue);
}

/**
 * Exit notify block.
 *
 * This is a helper function used in all notification functions to end atomic slot update
 * and trigger SWI to process the notification from the slot.
 */
static void ntf_exit(void)
{
    nrf_802154_queue_push_commit(&m_notifications_queue);

    nrf_egu_task_trigger(NRF_802154_EGU_INSTANCE, NTF_TASK);

    nrf_802154_mcu_critical_exit(m_mcu_cs);
}

/**
 * @brief Notifies the next higher layer that a frame was received.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  p_data  Pointer to a buffer that contains PHR and PSDU of the received frame.
 * @param[in]  power   RSSI measured during the frame reception.
 * @param[in]  lqi     LQI that indicates the measured link quality during the frame reception.
 */
void swi_notify_received(uint8_t * p_data, int8_t power, uint8_t lqi)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                 = NTF_TYPE_RECEIVED;
    p_slot->data.received.p_data = p_data;
    p_slot->data.received.power  = power;
    p_slot->data.received.lqi    = lqi;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that the reception of a frame failed.
 *
 * @param[in]  error  Error code that indicates reason of the failure.
 */
void swi_notify_receive_failed(nrf_802154_rx_error_t error)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                      = NTF_TYPE_RECEIVE_FAILED;
    p_slot->data.receive_failed.error = error;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that a frame was transmitted
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the transmitted frame.
 * @param[in]  p_ack    Pointer to a buffer that contains PHR and PSDU of ACK frame. NULL if ACK was
 *                      not requested.
 * @param[in]  power    RSSI of the received frame, or 0 if ACK was not requested.
 * @param[in]  lqi      LQI of the received frame, or 0 if ACK was not requested.
 */
void swi_notify_transmitted(const uint8_t * p_frame,
                            uint8_t       * p_ack,
                            int8_t          power,
                            uint8_t         lqi)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                     = NTF_TYPE_TRANSMITTED;
    p_slot->data.transmitted.p_frame = p_frame;
    p_slot->data.transmitted.p_ack   = p_ack;
    p_slot->data.transmitted.power   = power;
    p_slot->data.transmitted.lqi     = lqi;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that a frame was not transmitted from the SWI priority
 * level.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the frame that failed
 *                      the transmission.
 * @param[in]  error    Reason of the transmission failure.
 */
void swi_notify_transmit_failed(const uint8_t * p_frame, nrf_802154_tx_error_t error)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                         = NTF_TYPE_TRANSMIT_FAILED;
    p_slot->data.transmit_failed.p_frame = p_frame;
    p_slot->data.transmit_failed.error   = error;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that the energy detection procedure ended from
 * the SWI priority level.
 *
 * @param[in]  result  Detected energy level.
 */
void swi_notify_energy_detected(uint8_t result)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                        = NTF_TYPE_ENERGY_DETECTED;
    p_slot->data.energy_detected.result = result;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that the energy detection procedure failed from
 * the SWI priority level.
 *
 * @param[in]  error  Reason of the energy detection failure.
 */
void swi_notify_energy_detection_failed(nrf_802154_ed_error_t error)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                               = NTF_TYPE_ENERGY_DETECTION_FAILED;
    p_slot->data.energy_detection_failed.error = error;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that the Clear Channel Assessment (CCA) procedure ended.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  channel_free  If a free channel was detected.
 */
void swi_notify_cca(bool channel_free)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type            = NTF_TYPE_CCA;
    p_slot->data.cca.result = channel_free;

    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that the Clear Channel Assessment (CCA) procedure failed.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  error  Reason of the CCA failure.
 */
void swi_notify_cca_failed(nrf_802154_cca_error_t error)
{
    nrf_802154_ntf_data_t * p_slot = ntf_enter();

    p_slot->type                  = NTF_TYPE_CCA_FAILED;
    p_slot->data.cca_failed.error = error;

    ntf_exit();
}

void nrf_802154_notification_init(void)
{
    nrf_802154_queue_init(&m_notifications_queue, m_notifications_queue_memory,
                          sizeof(m_notifications_queue_memory),
                          sizeof(m_notifications_queue_memory[0]));

    nrf_egu_int_enable(NRF_802154_EGU_INSTANCE, NTF_INT);

    nrf_802154_swi_init();
}

void nrf_802154_notify_received(uint8_t * p_data, int8_t power, uint8_t lqi)
{
    swi_notify_received(p_data, power, lqi);
}

void nrf_802154_notify_receive_failed(nrf_802154_rx_error_t error)
{
    swi_notify_receive_failed(error);
}

void nrf_802154_notify_transmitted(const uint8_t * p_frame,
                                   uint8_t       * p_ack,
                                   int8_t          power,
                                   uint8_t         lqi)
{
    swi_notify_transmitted(p_frame, p_ack, power, lqi);
}

void nrf_802154_notify_transmit_failed(const uint8_t * p_frame, nrf_802154_tx_error_t error)
{
    swi_notify_transmit_failed(p_frame, error);
}

void nrf_802154_notify_energy_detected(uint8_t result)
{
    swi_notify_energy_detected(result);
}

void nrf_802154_notify_energy_detection_failed(nrf_802154_ed_error_t error)
{
    swi_notify_energy_detection_failed(error);
}

void nrf_802154_notify_cca(bool is_free)
{
    swi_notify_cca(is_free);
}

void nrf_802154_notify_cca_failed(nrf_802154_cca_error_t error)
{
    swi_notify_cca_failed(error);
}

/**@brief Handles NTF_EVENT on NRF_802154_EGU_INSTANCE */
static void irq_handler_ntf_event(void)
{
    while (!nrf_802154_queue_is_empty(&m_notifications_queue))
    {
        nrf_802154_ntf_data_t * p_slot =
            (nrf_802154_ntf_data_t *)nrf_802154_queue_pop_begin(&m_notifications_queue);

        switch (p_slot->type)
        {
            case NTF_TYPE_RECEIVED:
#if NRF_802154_USE_RAW_API
                nrf_802154_received_raw(p_slot->data.received.p_data,
                                        p_slot->data.received.power,
                                        p_slot->data.received.lqi);
#else // NRF_802154_USE_RAW_API
                nrf_802154_received(p_slot->data.received.p_data + RAW_PAYLOAD_OFFSET,
                                    p_slot->data.received.p_data[RAW_LENGTH_OFFSET],
                                    p_slot->data.received.power,
                                    p_slot->data.received.lqi);
#endif
                break;

            case NTF_TYPE_RECEIVE_FAILED:
                nrf_802154_receive_failed(p_slot->data.receive_failed.error);
                break;

            case NTF_TYPE_TRANSMITTED:
            {
#if NRF_802154_USE_RAW_API
                nrf_802154_transmitted_raw(p_slot->data.transmitted.p_frame,
                                           p_slot->data.transmitted.p_ack,
                                           p_slot->data.transmitted.power,
                                           p_slot->data.transmitted.lqi);
#else // NRF_802154_USE_RAW_API
                uint8_t * p_ack  = NULL;
                uint8_t   length = 0;

                if (p_slot->data.transmitted.p_ack != NULL)
                {
                    p_ack  = p_slot->data.transmitted.p_ack + RAW_PAYLOAD_OFFSET;
                    length = p_slot->data.transmitted.p_ack[RAW_LENGTH_OFFSET];
                }
                nrf_802154_transmitted(p_slot->data.transmitted.p_frame + RAW_PAYLOAD_OFFSET,
                                       p_ack,
                                       length,
                                       p_slot->data.transmitted.power,
                                       p_slot->data.transmitted.lqi);
#endif
            }
            break;

            case NTF_TYPE_TRANSMIT_FAILED:
#if NRF_802154_USE_RAW_API
                nrf_802154_transmit_failed(p_slot->data.transmit_failed.p_frame,
                                           p_slot->data.transmit_failed.error);
#else // NRF_802154_USE_RAW_API
                nrf_802154_transmit_failed(
                    p_slot->data.transmit_failed.p_frame + RAW_PAYLOAD_OFFSET,
                    p_slot->data.transmit_failed.error);
#endif
                break;

            case NTF_TYPE_ENERGY_DETECTED:
                nrf_802154_energy_detected(p_slot->data.energy_detected.result);
                break;

            case NTF_TYPE_ENERGY_DETECTION_FAILED:
                nrf_802154_energy_detection_failed(
                    p_slot->data.energy_detection_failed.error);
                break;

            case NTF_TYPE_CCA:
                nrf_802154_cca_done(p_slot->data.cca.result);
                break;

            case NTF_TYPE_CCA_FAILED:
                nrf_802154_cca_failed(p_slot->data.cca_failed.error);
                break;

            default:
                assert(false);
        }

        nrf_802154_queue_pop_commit(&m_notifications_queue);
    }
}

void nrf_802154_notification_swi_irq_handler(void)
{
    if (nrf_egu_event_check(NRF_802154_EGU_INSTANCE, NTF_EVENT))
    {
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, NTF_EVENT);

        irq_handler_ntf_event();
    }
}
