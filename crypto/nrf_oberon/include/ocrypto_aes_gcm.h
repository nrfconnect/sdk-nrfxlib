/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_aes_gcm AES GCM - AES Galois/Counter Mode APIs
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definitions and APIs for AES-GCM (AES Galois/Counter Mode).
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 *
 * AES-GCM (AES Galois/Counter Mode) is an AES mode which effectively turns the block cipher into
 * a stream cipher. The AES block cipher primitive is used in CTR mode for encryption and decryption.
 * In addition, 128-bit Galois Field multiplication is used for authentication.
 */

#ifndef OCRYPTO_AES_GCM_H
#define OCRYPTO_AES_GCM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * AES-GCM encryption.
 *
 * @param[out] ct     Cyphertext.
 * @param[out] tag    Authentication tag.
 * @param      pt     Plaintext.
 * @param      pt_len Plaintext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32 bytes).
 * @param      iv     Initial vector.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct and @p pt can point to the same address.
 */
void ocrypto_aes_gcm_encrypt (
    uint8_t* ct, uint8_t tag[16], const uint8_t* pt, size_t pt_len,
    const uint8_t *key, size_t size, const uint8_t iv[12], const uint8_t *aa, size_t aa_len);

/**
 * AES-GCM decryption.
 *
 * @param[out] pt     Plaintext.
 * @param      tag    Authentication tag.
 * @param      ct     Cyphertext.
 * @param      ct_len Cyphertext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32 bytes).
 * @param      iv     Initial vector.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct and @p pt can point to the same address.
 *
 * @retval 0  If @p tag is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_aes_gcm_decrypt (
    uint8_t* pt, const uint8_t tag[16], const uint8_t* ct, size_t ct_len,
    const uint8_t *key, size_t size, const uint8_t iv[12], const uint8_t *aa, size_t aa_len);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_AES_GCM_H */

/** @} */
