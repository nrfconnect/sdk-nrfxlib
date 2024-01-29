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
 * @defgroup ocrypto_aes_eax AES-EAX - AES Encrypt-then-authenticate-then-translate
 * @ingroup ocrypto_aes
 * @{
 * @brief Type definitions and APIS for AES-EAX (Encrypt-then-authenticate-then-translate)
 *
 * AES-EAX (encrypt-then-authenticate-then-translate) is an AES mode which effectively turns the
 * block cipher into a stream cipher. The AES block cipher primitive is used in CTR mode for
 * encryption and as OMAC for authentication over each block.
 */

#ifndef OCRYPTO_AES_EAX_H
#define OCRYPTO_AES_EAX_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * AES-EAX encryption.
 *
 * @param[out] ct     Ciphertext.
 * @param[out] tag    Authentication tag.
 * @param      pt     Plaintext.
 * @param      pt_len Length of @p pt and @p ct.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32 bytes).
 * @param      iv     Initial vector.
 * @param      iv_len Initial vector length.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct may be same as @p pt.
 */
void ocrypto_aes_eax_encrypt (
    uint8_t* ct, uint8_t tag[16], const uint8_t* pt, size_t pt_len,  const uint8_t *key, size_t size,
    const uint8_t* iv, size_t iv_len, const uint8_t *aa, size_t aa_len);

/**
 * AES-EAX decryption.
 *
 * @param[out] pt     Plaintext.
 * @param      tag    Authentication tag.
 * @param      ct     Ciphertext.
 * @param      ct_len Length of @p pt and @p ct.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32 bytes).
 * @param      iv     Initial vector.
 * @param      iv_len Initial vector length.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct may be same as @p pt.
 *
 * @retval 0  If @p tag is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_aes_eax_decrypt (
    uint8_t* pt, const uint8_t tag[16], const uint8_t* ct, size_t ct_len, const uint8_t *key, size_t size,
    const uint8_t* iv, size_t iv_len, const uint8_t *aa, size_t aa_len);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
