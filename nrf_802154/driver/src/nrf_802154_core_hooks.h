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

#ifndef NRF_802154_CORE_HOOKS_H__
#define NRF_802154_CORE_HOOKS_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_const.h"
#include "nrf_802154_types_internal.h"

/**
 * @defgroup nrf_802154_hooks Hooks for the 802.15.4 driver core
 * @{
 * @ingroup nrf_802154
 * @brief Hooks for the 802.15.4 driver core module.
 *
 * Hooks are used by the optional driver features to modify the way in which notifications
 * are propagated through the driver.
 */

/**
 * @brief Processes hooks for the termination request.
 *
 * @param[in]     term_lvl  Termination level of the request that terminates the current operation.
 * @param[in]     req_orig  Module that originates this request.
 *
 * @retval true   All procedures are aborted.
 * @retval false  There is an ongoing procedure that cannot be aborted due to a too low @p term_lvl.
 */
bool nrf_802154_core_hooks_terminate(nrf_802154_term_t term_lvl, req_originator_t req_orig);

/**
 * @brief Processes hooks which are to fire before the transmission request and before
 *        attempt to terminate previous operation.
 *
 * @param[in] p_frame          Pointer to a buffer that contains PHR and PSDU of the frame
 *                             that is to be transmitted.
 * @param[in] p_params         Pointer to the transmission parameters.
 * @param[in] notify_function  Function to be called to notify transmission failure.
 *
 * @retval true         Frame can be sent immediately.
 * @retval false        Hooks have handled the frame - upper layer should not worry about it anymore.
 */
bool nrf_802154_core_hooks_pre_transmission(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function);

/**
 * @brief Processes hooks which are to fire before the transmission but after previous operation
 *        has been already terminated.
 *
 * @param[in] p_frame          Pointer to a buffer that contains PHR and PSDU of the frame
 *                             that is to be transmitted.
 * @param[in] p_params         Pointer to the transmission parameters.
 * @param[in] notify_function  Function to be called to notify transmission failure.
 *
 * @retval true         Frame can be sent immediately.
 * @retval false        Hooks have handled the frame - upper layer should not worry about it anymore.
 */
bool nrf_802154_core_hooks_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function);

/**
 * @brief Processes hooks for the transmitted event.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the frame
 *                      that was transmitted.
 */
void nrf_802154_core_hooks_transmitted(const uint8_t * p_frame);

/**
 * @brief Processes hooks for the TX failed event.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the frame
 *                      that was not transmitted.
 * @param[in]  error    Cause of the failed transmission.
 *
 * @retval  true   TX failed event is to be propagated to the MAC layer.
 * @retval  false  TX failed event is not to be propagated to the MAC layer. It is handled
 *                 internally.
 */
bool nrf_802154_core_hooks_tx_failed(uint8_t * p_frame, nrf_802154_tx_error_t error);

/**
 * @brief Processes hooks for the ACK TX failed event.
 *
 * @param[in]  p_ack    Pointer to a buffer that contains PHR and PSDU of the ACK frame
 *                      that was not transmitted.
 * @param[in]  error    Cause of the failed transmission.
 */
void nrf_802154_core_hooks_tx_ack_failed(uint8_t * p_ack, nrf_802154_tx_error_t error);

/**
 * @brief Processes hooks for the TX started event.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the frame
 *                      that is being transmitted.
 *
 * @retval  true   TX started event is to be propagated to the MAC layer.
 * @retval  false  TX started event is not to be propagated to the MAC layer. It is handled
 *                 internally.
 */
bool nrf_802154_core_hooks_tx_started(uint8_t * p_frame);

/**
 * @brief Processes hooks for the RX started event.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the frame
 *                      that is being received.
 */
void nrf_802154_core_hooks_rx_started(const uint8_t * p_frame);

/**
 * @brief Processes hooks for the RX ACK started event.
 */
void nrf_802154_core_hooks_rx_ack_started(void);

/**
 * @brief Processes hooks for the TX ACK started event.
 *
 * @param[in]  p_ack  Pointer to a buffer that contains PHR and PSDU of the ACK frame
 *                    that is being transmitted.
 */
void nrf_802154_core_hooks_tx_ack_started(uint8_t * p_ack);

/**
 *@}
 **/

#endif // NRF_802154_CORE_HOOKS_H__
