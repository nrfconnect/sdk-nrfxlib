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

#ifndef NRF_802154_NOTIFICATION_H__
#define NRF_802154_NOTIFICATION_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154.h"
#include "nrf_802154_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_notification 802.15.4 driver notification
 * @{
 * @ingroup nrf_802154
 * @brief Notifications to the next higher layer triggered from the 802.15.4 radio driver.
 */

/**
 * @brief Function type used for notifications from critical sections in the core module.
 *
 * This function is called instead of the default notification. The function is passed to request
 * notifications from a critical section during the request processing.
 *
 * @param[in]  result  If the called request succeeded.
 */
typedef void (* nrf_802154_notification_func_t)(bool result);

/**
 * @brief Initializes the notification module.
 */
void nrf_802154_notification_init(void);

/**
 * @brief Notifies the next higher layer that a frame was received.
 *
 * @param[in]  p_data  Pointer to a buffer that contains PHR and PSDU of the received frame.
 * @param[in]  power   RSSI measured during the frame reception.
 * @param[in]  lqi     LQI indicating measured link quality during the frame reception.
 */
void nrf_802154_notify_received(uint8_t * p_data, int8_t power, uint8_t lqi);

/**
 * @brief Notifies the next higher layer that the reception of a frame failed.
 *
 * @param[in]  error       Error code that indicates the reason of the failure.
 * @param[in]  id          Identifier of reception window the error occurred in.
 *                         If the error is related to a delayed reception window requested through
 *                         @ref nrf_802154_receive_at, the value of @p id equals the identifier
 *                         of the scheduled reception window. Otherwise, the value of @p id equals
 *                         @ref NRF_802154_RESERVED_IMM_RX_WINDOW_ID.
 * @param[in]  allow_drop  Indicates if the notification can be dropped safely.
 *
 * @retval   true    The next higher layer is bound to be notified about the frame reception failure.
 * @retval   false   Notification could not be executed.
 */
bool nrf_802154_notify_receive_failed(nrf_802154_rx_error_t error, uint32_t id, bool allow_drop);

/**
 * @brief Notifies the next higher layer that a frame was transmitted.
 *
 * @note This function performs an update of the original frame and modifies @p p_metadata structure
 *       passed to the public callout.
 *
 * @param[in]  p_frame      Pointer to a buffer that contains PHR and PSDU of the transmitted frame.
 * @param[in]  p_metadata   Pointer to a metadata structure describing frame passed in @p p_frame.
 */
void nrf_802154_notify_transmitted(uint8_t                             * p_frame,
                                   nrf_802154_transmit_done_metadata_t * p_metadata);

/**
 * @brief Notifies the next higher layer that a frame was not transmitted.
 *
 * @note This function performs an update of the original frame and prepares
 *       @ref nrf_802154_transmit_done_metadata_t structure passed to the public callout.
 *
 * @param[in]  p_frame      Pointer to a buffer that contains PHR and PSDU of the frame that failed
 *                          the transmission operation.
 * @param[in]  error        An error code indicating the reason of the failure.
 * @param[in]  p_metadata   Pointer to a metadata structure to be notified.
 */
void nrf_802154_notify_transmit_failed(uint8_t                                   * p_frame,
                                       nrf_802154_tx_error_t                       error,
                                       const nrf_802154_transmit_done_metadata_t * p_metadata);

/**
 * @brief Notifies the next higher layer that the energy detection procedure ended.
 *
 * @param[in]  p_result  Pointer to structure containing the result of the ED operation.
 */
void nrf_802154_notify_energy_detected(const nrf_802154_energy_detected_t * p_result);

/**
 * @brief Notifies the next higher layer that the energy detection procedure failed.
 *
 * @param[in]  error  Error code indicating the reason of the failure.
 */
void nrf_802154_notify_energy_detection_failed(nrf_802154_ed_error_t error);

/**
 * @brief Notifies the next higher layer that the CCA procedure ended.
 *
 * @param[in]  is_free  If detected that channel is free.
 */
void nrf_802154_notify_cca(bool is_free);

/**
 * @brief Notifies the next higher layer that the CCA procedure failed.
 *
 * @param[in]  error  Error code indicating reason of the failure.
 */
void nrf_802154_notify_cca_failed(nrf_802154_cca_error_t error);

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_NOTIFICATION_H__
