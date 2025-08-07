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
 *   This file implements notifications triggered by the nrf 802.15.4 radio driver via SWI.
 *
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_NOTIFICATION

#include "nrf_802154_notification.h"

#if NRF_802154_NOTIFICATION_IMPL == NRF_802154_NOTIFICATION_IMPL_SWI

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "nrf_802154.h"
#include "nrf_802154_co.h"
#include "nrf_802154_config.h"
#include "nrf_802154_debug.h"
#include "nrf_802154_queue.h"
#include "nrf_802154_swi.h"
#include "nrf_802154_tx_work_buffer.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_utils.h"
#include "hal/nrf_egu.h"
#include "rsch/nrf_802154_rsch.h"

#define RAW_PAYLOAD_OFFSET 1
#define RAW_LENGTH_OFFSET  0

/** @brief Size of pool of slots for notifications that must not be lost.
 *
 * The pool needs to contain slots for:
 *  - immediate operation result notification
 *      ~ transmission
 *      ~ energy detection
 *      ~ standalone CCA
 *  - CSMA-CA result notification
 *  - DTX result notification
 *  - DRX timeout notifications
 *  - frame received notifications for each receive buffer.
 */
#define NTF_PRIMARY_POOL_SIZE \
    (NRF_802154_RX_BUFFERS + NRF_802154_RSCH_DLY_TS_SLOTS + 1)

/**
 * The implementation uses 8-bit integers to address slots with the oldest bit
 * indicating pool. That leaves 7 bits for addressing slots within a fixed pool.
 * If the pool's size exceeds that width, throw an error.
 */
#if NTF_PRIMARY_POOL_SIZE > 0x7FU
#error NTF_PRIMARY_POOL_SIZE exceeds its bit width
#endif

/** @brief Size of pool of slots for notifications that can be ignored.
 */
#define NTF_SECONDARY_POOL_SIZE    NRF_802154_MAX_DISREGARDABLE_NOTIFICATIONS

/** @brief Bitmask that represents slot pool used.
 */
#define NTF_POOL_ID_MASK           (1U << 7)

/** @brief Bitmask that indicates a given slot comes from the primary pool.
 */
#define NTF_PRIMARY_POOL_ID_MASK   (1U << 7)

/** @brief Bitmask that indicates a given slot comes from the secondary pool.
 */
#define NTF_SECONDARY_POOL_ID_MASK 0U

/** @brief Identifier of an invalid slot.
 */
#define NTF_INVALID_SLOT_ID        UINT8_MAX

/** @brief Size of notification queue.
 *
 * One slot is lost due to simplified queue implementation.
 */
#define NTF_QUEUE_SIZE             (NTF_PRIMARY_POOL_SIZE + NTF_SECONDARY_POOL_SIZE + 1)

#define NTF_INT                    NRFX_CONCAT_2(NRF_EGU_INT_TRIGGERED, \
                                                 NRF_802154_EGU_NOTIFICATION_CHANNEL_NO)
#define NTF_TASK                   NRFX_CONCAT_2(NRF_EGU_TASK_TRIGGER, \
                                                 NRF_802154_EGU_NOTIFICATION_CHANNEL_NO)
#define NTF_EVENT                  NRFX_CONCAT_2(NRF_EGU_EVENT_TRIGGERED, \
                                                 NRF_802154_EGU_NOTIFICATION_CHANNEL_NO)

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
    volatile uint8_t      taken; ///< Indicates if the slot is available.
    nrf_802154_ntf_type_t type;  ///< Notification type.

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
            uint32_t              id;    ///< Identifier of reception window the error occured in.
        } receive_failed;

        struct
        {
            uint8_t                           * p_frame;  ///< Pointer to frame that was transmitted.
            nrf_802154_transmit_done_metadata_t metadata; ///< Metadata structure describing @ref p_frame.
        } transmitted;                                    ///< Transmitted frame details.

        struct
        {
            uint8_t                           * p_frame;  ///< Pointer to frame that was requested to be transmitted, but failed.
            nrf_802154_tx_error_t               error;    ///< An error code that indicates reason of the failure.
            nrf_802154_transmit_done_metadata_t metadata; ///< Metadata structure describing @ref p_frame.
        } transmit_failed;                                ///< Failed transmission details.

        struct
        {
            nrf_802154_energy_detected_t result; ///< Energy detection result.
        } energy_detected;                       ///< Energy detection details.

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

/// Entry in the notification queue
typedef struct
{
    uint8_t id; ///< Identifier of the pool and an entry within.
} nrf_802154_queue_entry_t;

static nrf_802154_ntf_data_t m_primary_ntf_pool[NTF_PRIMARY_POOL_SIZE];
static nrf_802154_ntf_data_t m_secondary_ntf_pool[NTF_SECONDARY_POOL_SIZE];

static nrf_802154_queue_t       m_notifications_queue;
static nrf_802154_queue_entry_t m_notifications_queue_memory[NTF_QUEUE_SIZE];

static volatile nrf_802154_mcu_critical_state_t m_mcu_cs;

#if (NRF_802154_MAX_PENDING_NOTIFICATIONS + 1) != (NTF_QUEUE_SIZE)
#error "Mismatching sizes of notification queue and maximum number of pending notifications"
#endif

/** @brief Allocate notification slot from the specified pool.
 *
 * @param[inout]  p_pool    Pointer to a pool of slots.
 * @param[in]     pool_len  Length of the pool.
 *
 * @return   Index of the allocated slot or NTF_INVALID_SLOT_ID in case of failure.
 */
static uint8_t ntf_slot_alloc(nrf_802154_ntf_data_t * p_pool, size_t pool_len)
{
    // Linear search for a free slot
    for (size_t i = 0; i < pool_len; i++)
    {
        bool slot_found = true;

        do
        {
            uint8_t taken = __LDREXB(&p_pool[i].taken);

            if (taken)
            {
                // The slot is taken. Proceed to the next slot
                __CLREX();
                slot_found = false;
                break;
            }
        }
        while (__STREXB(true, &p_pool[i].taken));

        __DMB();

        if (slot_found)
        {
            // Free slot was found and it was successfully marked as taken
            return i;
        }
    }

    return NTF_INVALID_SLOT_ID;
}

/** @brief Release a slot.
 *
 * @param[inout]  p_slot  Pointer to a slot to free.
 */
static void ntf_slot_free(nrf_802154_ntf_data_t * p_slot)
{
    __DMB();
    p_slot->taken = false;
}

/**
 * Enter notify block.
 *
 * This is a helper function used in all notification functions to atomically
 * find an empty slot in the notification queue and allow atomic slot update.
 *
 * @return Pointer to an empty slot in the notification queue.
 */
static nrf_802154_queue_entry_t * ntf_enter(void)
{
    nrf_802154_mcu_critical_enter(m_mcu_cs);

    NRF_802154_ASSERT(!nrf_802154_queue_is_full(&m_notifications_queue));

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

/** @brief Push notification to the queue.
 *
 * @param[in]  slot_id  Identifier of the pool and a slot within.
 */
static void ntf_push(uint8_t slot_id)
{
    nrf_802154_queue_entry_t * p_entry = ntf_enter();

    p_entry->id = slot_id;
    ntf_exit();
}

/**
 * @brief Notifies the next higher layer that a frame was received.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  p_data  Pointer to a buffer that contains PHR and PSDU of the received frame.
 * @param[in]  power   RSSI measured during the frame reception.
 * @param[in]  lqi     LQI that indicates the measured link quality during the frame reception.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_received(uint8_t * p_data, int8_t power, uint8_t lqi)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type                 = NTF_TYPE_RECEIVED;
    p_slot->data.received.p_data = p_data;
    p_slot->data.received.power  = power;
    p_slot->data.received.lqi    = lqi;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

/**
 * @brief Notifies the next higher layer that the reception of a frame failed.
 *
 * @param[in]  error  Error code that indicates reason of the failure.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_receive_failed(nrf_802154_rx_error_t error, uint32_t id, bool allow_drop)
{
    nrf_802154_ntf_data_t * p_pool;
    size_t                  pool_len;
    uint32_t                pool_id_bitmask;

    if (!allow_drop)
    {
        // Choose the primary pool for DRX-related errors
        p_pool          = m_primary_ntf_pool;
        pool_len        = NTF_PRIMARY_POOL_SIZE;
        pool_id_bitmask = NTF_PRIMARY_POOL_ID_MASK;
    }
    else
    {
        // Choose the secondary pool for spurious reception errors
        p_pool          = m_secondary_ntf_pool;
        pool_len        = NTF_SECONDARY_POOL_SIZE;
        pool_id_bitmask = NTF_SECONDARY_POOL_ID_MASK;
    }

    uint8_t slot_id = ntf_slot_alloc(p_pool, pool_len);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &p_pool[slot_id];

    p_slot->type                      = NTF_TYPE_RECEIVE_FAILED;
    p_slot->data.receive_failed.error = error;
    p_slot->data.receive_failed.id    = id;

    ntf_push(slot_id | pool_id_bitmask);

    return true;
}

/**
 * @brief Notifies the next higher layer that a frame was transmitted
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  p_frame      Pointer to a buffer that contains PHR and PSDU of the transmitted frame.
 * @param[in]  p_metadata   Pointer to a metadata structure describing frame passed in @p p_frame.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_transmitted(uint8_t                             * p_frame,
                            nrf_802154_transmit_done_metadata_t * p_metadata)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type                      = NTF_TYPE_TRANSMITTED;
    p_slot->data.transmitted.p_frame  = p_frame;
    p_slot->data.transmitted.metadata = *p_metadata;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

/**
 * @brief Notifies the next higher layer that a frame was not transmitted from the SWI priority
 * level.
 *
 * @param[in]  p_frame      Pointer to a buffer that contains PHR and PSDU of the frame that failed
 *                          the transmission.
 * @param[in]  error        Reason of the transmission failure.
 * @param[in]  p_metadata   Pointer to a metadata structure describing frame passed in @p p_frame.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_transmit_failed(uint8_t                                   * p_frame,
                                nrf_802154_tx_error_t                       error,
                                const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type                          = NTF_TYPE_TRANSMIT_FAILED;
    p_slot->data.transmit_failed.p_frame  = p_frame;
    p_slot->data.transmit_failed.error    = error;
    p_slot->data.transmit_failed.metadata = *p_metadata;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

/**
 * @brief Notifies the next higher layer that the energy detection procedure ended from
 * the SWI priority level.
 *
 * @param[in]  p_result  Structure holding the result of energy detection procedure.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_energy_detected(const nrf_802154_energy_detected_t * p_result)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type                        = NTF_TYPE_ENERGY_DETECTED;
    p_slot->data.energy_detected.result = *p_result;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

/**
 * @brief Notifies the next higher layer that the energy detection procedure failed from
 * the SWI priority level.
 *
 * @param[in]  error  Reason of the energy detection failure.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_energy_detection_failed(nrf_802154_ed_error_t error)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type                               = NTF_TYPE_ENERGY_DETECTION_FAILED;
    p_slot->data.energy_detection_failed.error = error;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

/**
 * @brief Notifies the next higher layer that the Clear Channel Assessment (CCA) procedure ended.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  channel_free  If a free channel was detected.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_cca(bool channel_free)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type            = NTF_TYPE_CCA;
    p_slot->data.cca.result = channel_free;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

/**
 * @brief Notifies the next higher layer that the Clear Channel Assessment (CCA) procedure failed.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @param[in]  error  Reason of the CCA failure.
 *
 * @retval  true   Notification enqueued successfully.
 * @retval  false  Notification could not be performed.
 */
bool swi_notify_cca_failed(nrf_802154_cca_error_t error)
{
    uint8_t slot_id = ntf_slot_alloc(m_primary_ntf_pool, NTF_PRIMARY_POOL_SIZE);

    if (slot_id == NTF_INVALID_SLOT_ID)
    {
        // No slots are available.
        return false;
    }

    nrf_802154_ntf_data_t * p_slot = &m_primary_ntf_pool[slot_id];

    p_slot->type                  = NTF_TYPE_CCA_FAILED;
    p_slot->data.cca_failed.error = error;

    ntf_push(slot_id | NTF_PRIMARY_POOL_ID_MASK);

    return true;
}

void nrf_802154_notification_init(void)
{
    nrf_802154_queue_init(&m_notifications_queue,
                          m_notifications_queue_memory,
                          sizeof(m_notifications_queue_memory),
                          sizeof(m_notifications_queue_memory[0]));

    nrf_egu_int_enable(NRF_802154_EGU_INSTANCE, NTF_INT);
}

void nrf_802154_notify_received(uint8_t * p_data, int8_t power, uint8_t lqi)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_received(p_data, power, lqi);

    // It should always be possible to notify a successful reception
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

bool nrf_802154_notify_receive_failed(nrf_802154_rx_error_t error, uint32_t id, bool allow_drop)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_receive_failed(error, id, allow_drop);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return notified;
}

void nrf_802154_notify_transmitted(uint8_t                             * p_frame,
                                   nrf_802154_transmit_done_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // Update the transmitted frame contents and update frame status flags
    nrf_802154_tx_work_buffer_original_frame_update(p_frame,
                                                    &p_metadata->frame_props);

    bool notified = swi_notify_transmitted(p_frame, p_metadata);

    // It should always be possible to notify transmission result
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_notify_transmit_failed(uint8_t                                   * p_frame,
                                       nrf_802154_tx_error_t                       error,
                                       const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_transmit_failed(p_frame, error, p_metadata);

    // It should always be possible to notify transmission result
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_notify_energy_detected(const nrf_802154_energy_detected_t * p_result)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_energy_detected(p_result);

    // It should always be possible to notify energy detection result
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_notify_energy_detection_failed(nrf_802154_ed_error_t error)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_energy_detection_failed(error);

    // It should always be possible to notify energy detection result
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_notify_cca(bool is_free)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_cca(is_free);

    // It should always be possible to notify CCA result
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_notify_cca_failed(nrf_802154_cca_error_t error)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool notified = swi_notify_cca_failed(error);

    // It should always be possible to notify CCA result
    NRF_802154_ASSERT(notified);
    (void)notified;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/**@brief Handles NTF_EVENT on NRF_802154_EGU_INSTANCE */
static void irq_handler_ntf_event(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    while (!nrf_802154_queue_is_empty(&m_notifications_queue))
    {
        nrf_802154_queue_entry_t * p_entry =
            (nrf_802154_queue_entry_t *)nrf_802154_queue_pop_begin(&m_notifications_queue);

        uint8_t slot_id = p_entry->id & (~NTF_POOL_ID_MASK);

        nrf_802154_ntf_data_t * p_slot =
            (p_entry->id & NTF_POOL_ID_MASK) ? &m_primary_ntf_pool[slot_id] :
            &m_secondary_ntf_pool[slot_id];

        switch (p_slot->type)
        {
            case NTF_TYPE_RECEIVED:
                nrf_802154_co_received_raw(p_slot->data.received.p_data,
                                           p_slot->data.received.power,
                                           p_slot->data.received.lqi);
                break;

            case NTF_TYPE_RECEIVE_FAILED:
                nrf_802154_co_receive_failed(p_slot->data.receive_failed.error,
                                             p_slot->data.receive_failed.id);
                break;

            case NTF_TYPE_TRANSMITTED:
                nrf_802154_co_transmitted_raw(p_slot->data.transmitted.p_frame,
                                              &p_slot->data.transmitted.metadata);
                break;

            case NTF_TYPE_TRANSMIT_FAILED:
                nrf_802154_co_transmit_failed(p_slot->data.transmit_failed.p_frame,
                                              p_slot->data.transmit_failed.error,
                                              &p_slot->data.transmit_failed.metadata);
                break;

            case NTF_TYPE_ENERGY_DETECTED:
                nrf_802154_co_energy_detected(&p_slot->data.energy_detected.result);
                break;

            case NTF_TYPE_ENERGY_DETECTION_FAILED:
                nrf_802154_co_energy_detection_failed(
                    p_slot->data.energy_detection_failed.error);
                break;

            case NTF_TYPE_CCA:
                nrf_802154_co_cca_done(p_slot->data.cca.result);
                break;

            case NTF_TYPE_CCA_FAILED:
                nrf_802154_co_cca_failed(p_slot->data.cca_failed.error);
                break;

            default:
                NRF_802154_ASSERT(false);
        }

        nrf_802154_queue_pop_commit(&m_notifications_queue);
        ntf_slot_free(p_slot);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_notification_swi_irq_handler(void)
{
    if (nrf_egu_event_check(NRF_802154_EGU_INSTANCE, NTF_EVENT))
    {
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, NTF_EVENT);

        irq_handler_ntf_event();
    }
}

#if defined(TEST)
void nrf_802154_notification_swi_module_reset(void)
{
    m_mcu_cs = 0UL;
    memset(&m_notifications_queue_memory, 0U, sizeof(m_notifications_queue_memory));
    memset(&m_notifications_queue, 0U, sizeof(m_notifications_queue));
}

#endif // defined(TEST)

#endif /* NRF_802154_NOTIFICATION_IMPL == NRF_802154_NOTIFICATION_IMPL_SWI */
