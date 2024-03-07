/*
 * Copyright (c) 2024, Nordic Semiconductor ASA
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

#ifndef NRF_802154_SL_ECB_H__
#define NRF_802154_SL_ECB_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_sl_ecb ECB encryption
 * @{
 * @ingroup nrf_802154_sl_ecb
 * @brief The ECB encryption API for nRF 802.15.4 Radio Driver
 */

#define NRF_802154_SL_ECB_KEY_LENGTH        (16)                                           /**< ECB key length in bytes. */
#define NRF_802154_SL_ECB_CLEARTEXT_LENGTH  (16)                                           /**< ECB cleartext length in bytes. */
#define NRF_802154_SL_ECB_CIPHERTEXT_LENGTH (16)                                           /**< ECB ciphertext length in bytes. */

/**@brief AES ECB parameter typedefs */
typedef uint32_t nrf_802154_sl_ecb_key_t[NRF_802154_SL_ECB_KEY_LENGTH / sizeof(uint32_t)]; /**< Encryption key type. */
typedef uint8_t nrf_802154_sl_ecb_cleartext_t[NRF_802154_SL_ECB_CLEARTEXT_LENGTH];         /**< Cleartext data type. */
typedef uint8_t nrf_802154_sl_ecb_ciphertext_t[NRF_802154_SL_ECB_CIPHERTEXT_LENGTH];       /**< Ciphertext data type. */

/**@brief AES ECB data structure */
typedef struct
{
    nrf_802154_sl_ecb_key_t        key;        /**< Encryption key. */
    nrf_802154_sl_ecb_cleartext_t  cleartext;  /**< Cleartext data. */
    nrf_802154_sl_ecb_ciphertext_t ciphertext; /**< Ciphertext data. */
} nrf_802154_sl_ecb_data_t;

/**@brief Encrypts a block according to the specified parameters.
 *
 * @param[in, out] p_ecb_data  Pointer to the ECB parameters' struct.
 */
void nrf_802154_sl_ecb_block_encrypt(nrf_802154_sl_ecb_data_t * p_ecb_data);

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_SL_ECB_H__
