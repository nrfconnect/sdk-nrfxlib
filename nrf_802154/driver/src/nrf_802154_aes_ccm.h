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

#ifndef NRF_802154_AES_CCM_H_
#define NRF_802154_AES_CCM_H_

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_const.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_802154_AES_CCM_L_VALUE    2                                 // Annex B3.2 Mode of operation d)
#define NRF_802154_AES_CCM_NONCE_SIZE (15 - NRF_802154_AES_CCM_L_VALUE) // Annex B4.1 CCM* mode encryption and authentication transformation b)

/**
 * @brief Data necessary for a single AES-CCM* operation.
 */
typedef struct
{
    uint8_t   key[AES_CCM_KEY_SIZE];                ///< Pointer to AES-CCM* key
    uint8_t * auth_data;                            ///< Pointer to AES-CCM* authorization data
    uint64_t  auth_data_len;                        ///< Length of AES-CCM* authorization data
    uint8_t * plain_text_data;                      ///< Pointer to AES-CCM* plain text data for encryption and authorization
    uint8_t   plain_text_data_len;                  ///< Length of plain text data
    uint8_t   nonce[NRF_802154_AES_CCM_NONCE_SIZE]; ///< Pointer to AES-CCM* nonce
    uint8_t   mic_level;                            ///< Message Integrity Code level
    uint8_t * raw_frame;                            ///< Pointer to the buffer that contains the PHR and PSDU of the transmitted frame.
} nrf_802154_aes_ccm_data_t;

/**
 * @brief Resets AES-CCM* transformation.
 */
void nrf_802154_aes_ccm_transform_reset(void);

/**
 * @brief Prepares AES-CCM* transformation.
 *
 * @param[in]  p_aes_ccm_data   Data to be used for the AES-CCM* transformation.
 *
 * @retval  true   The transformation was prepared successfully and can be performed.
 * @retval  false  Provided parameters do not allow for successful transformation.
 */
bool nrf_802154_aes_ccm_transform_prepare(const nrf_802154_aes_ccm_data_t * p_aes_ccm_data);

/**
 * @brief Starts AES-CCM* transformation.
 *
 * @note This function should not be called until the transformation is prepared with a call to
 *       @ref nrf_802154_aes_ccm_transform_prepare. Any call to this function that is not paired
 *       with an earlier prepare call will be ignored silently, i.e. the transformation will
 *       not be started at all.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains a frame being transmitted.
 */
void nrf_802154_aes_ccm_transform_start(uint8_t * p_frame);

/**
 * @brief Aborts AES-CCM* transformation.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains a frame being transmitted.
 */
void nrf_802154_aes_ccm_transform_abort(uint8_t * p_frame);

#endif // NRF_802154_AES_CCM_H_
