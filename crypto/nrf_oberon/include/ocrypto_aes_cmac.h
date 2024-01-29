/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_aes AES - Advanced Encryption Standard APIs
 * @ingroup ocrypto
 * @{
 * @brief AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 * @}
 *
 * @defgroup ocrypto_aes_cmac AES-CMAC - AES Cipher-based Message Authentication Code APIs
 * @ingroup ocrypto_aes
 * @{
 * @brief Type definitions and APIs for AES-CMAC (AES Cipher-based Message Authentication Code).
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
void ocrypto_aes_cmac_authenticate(
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
void ocrypto_aes_cmac_prf128(
    uint8_t prf[ocrypto_aes_cmac_prf128_BYTES],
    const uint8_t *msg, size_t msg_len,
    const uint8_t *key, size_t key_len);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
