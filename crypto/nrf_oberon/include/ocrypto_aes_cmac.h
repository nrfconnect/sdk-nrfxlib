/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_aes_cmac AES CMAC APIs
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definitions and APIS for AES CMAC (AES Cipher-based Message Authentication Code)
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 *
 * AES-CMAC (AES Cipher-based Message Authentication Code) is a block cipher-based message
 * authentication code algorithm. The AES block cipher primitive is used in variant of the
 * CBC mode to get the authentication tag.
 */

#ifndef OCRYPTO_AES_CMAC_H
#define OCRYPTO_AES_CMAC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Length of the pseudo random function.
 */
#define ocrypto_aes_cmac_prf128_BYTES (16)

/**
 * AES-CMAC authentication algorithm.
 *
 * @param[out] tag     Resulting tag.
 * @param      tag_len Tag length, 0 < @p tag_len <= 16.
 * @param      msg     Message to authenticate.
 * @param      msg_len Message length.
 * @param      key     AES key.
 * @param      size    Key size (16, 24, or 32).
 */
void ocrypto_aes_cmac_authenticate (
    uint8_t *tag, size_t tag_len,
    const uint8_t *msg, size_t msg_len,
    const uint8_t *key, size_t size);

/**
 * AES-CMAC-PRF-128 pseudo random function algorithm.
 *
 * @param[out] prf     16 byte PRF output.
 * @param      msg     Message input.
 * @param      msg_len Message length.
 * @param      key     Key.
 * @param      key_len Key length.
 */
void ocrypto_aes_cmac_prf128 (
    uint8_t prf[ocrypto_aes_cmac_prf128_BYTES],
    const uint8_t *msg, size_t msg_len,
    const uint8_t *key, size_t key_len);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_AES_CMAC_H */

/** @} */
