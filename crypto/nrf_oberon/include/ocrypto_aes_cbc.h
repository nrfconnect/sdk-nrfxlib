/*
* Copyright (c) 2020 Nordic Semiconductor ASA
*
* SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
*/

/**@file
 * @defgroup nrf_oberon_aes_cbc AES CBC APIs
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definitions and APIS for AES CBC (AES Cipher Block Chaining)
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 *
 * AES-CBC (AES Cipher Block Chaining) is an AES block cipher mode which avoids the problems of the
 * ECB mode by xoring each plaintext block with the previous ciphertext block before being encrypted.
 */

#ifndef OCRYPTO_AES_CBC_H
#define OCRYPTO_AES_CBC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * AES-CBC encryption.
 *
 * @param[out] ct     Cyphertext.
 * @param      pt     Plaintext.
 * @param      pt_len Plaintext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32).
 * @param      iv     Initial vector.
 *
 * @remark @p ct may be same as @p pt.
 */
void ocrypto_aes_cbc_encrypt (
    uint8_t* ct, const uint8_t* pt, size_t pt_len, const uint8_t *key, size_t size, const uint8_t iv[16]);

/**
 * AES-CBC decryption.
 *
 * @param[out] pt     Plaintext.
 * @param      ct     Cyphertext.
 * @param      ct_len Cyphertext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32).
 * @param      iv     Initial vector.
 *
 * @remark @p ct may be same as @p pt.
 */
void ocrypto_aes_cbc_decrypt (
    uint8_t* pt, const uint8_t* ct, size_t ct_len, const uint8_t *key, size_t size, const uint8_t iv[16]);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_AES_CBC_H */

/** @} */
