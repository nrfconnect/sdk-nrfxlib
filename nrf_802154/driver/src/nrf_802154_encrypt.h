/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
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

#ifndef NRF_802154_ENCRYPT_H_
#define NRF_802154_ENCRYPT_H_

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_types_internal.h"
#include "mac_features/nrf_802154_frame_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Prepares encryption data for provided Ack.
 *
 * @param[in]  p_ack_data  Pointer to the ACK frame parser data.
 */
bool nrf_802154_encrypt_ack_prepare(const nrf_802154_frame_parser_data_t * p_ack_data);

/**
 * @brief Resets encryption of ACK
 */
void nrf_802154_encrypt_ack_reset(void);

/**
 * @brief Transmission setup hook for the encryption module.
 *
 * This hook prepares encryption data for provided frame.
 *
 * @param[in]  p_frame          Pointer to the buffer that contains the PHR and PSDU
 *                              of the transmitted frame.
 * @param[in]  p_params         Pointer to the transmission parameters.
 * @param[in]  notify_function  Function to be called to notify transmission failure.
 *
 * @retval  true         Encryption was prepared successfully.
 * @retval  false        Encryption prepare failed.
 */
bool nrf_802154_encrypt_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function);

/**
 * @brief TX started hook for the encryption module.
 *
 * This function triggers the transformation procedure of the provided frame.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains a frame being transmitted.
 *
 * @retval  true        Always succeeds.
 */
bool nrf_802154_encrypt_tx_started_hook(uint8_t * p_frame);

/**
 * @brief ACK TX started hook for the encryption module.
 *
 * This function triggers the transformation procedure of the provided Ack.
 *
 * @param[in]  p_ack  Pointer to the buffer that contains the PHR and PSDU of the ACK frame.
 */
void nrf_802154_encrypt_tx_ack_started_hook(uint8_t * p_ack);

/**
 * @brief TX failed hook for the encryption module.
 *
 * This function aborts the transformation procedure of the provided frame.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains a frame being transmitted.
 * @param[in]  error    Cause of the failed transmission.
 *
 * @retval  true        Always succeeds.
 */
bool nrf_802154_encrypt_tx_failed_hook(uint8_t * p_frame, nrf_802154_tx_error_t error);

/**
 * @brief ACK TX failed hook for the encryption module.
 *
 * This function aborts the transformation procedure of the provided ACK frame.
 *
 * @param[in]  p_ack  Pointer to the buffer that contains a frame being transmitted.
 * @param[in]  error  Cause of the failed transmission.
 */
void nrf_802154_encrypt_tx_ack_failed_hook(uint8_t * p_ack, nrf_802154_tx_error_t error);

#endif /* NRF_802154_ENCRYPT_H_ */
