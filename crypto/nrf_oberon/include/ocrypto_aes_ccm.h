/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_aes_ccm AES CCM APIs
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definitions and APIS for AES CCM (AES counter mode with CBC-MAC)
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 *
 * AES-CCM (AES counter mode with CBC-MAC) is an AES mode which effectively turns the block
 * cipher into a stream cipher. The AES block cipher primitive is used in CTR mode for
 * encryption and decryption. In addition an AES CBC-MAC is used for authentication.
 */
#ifndef OCRYPTO_AES_CCM_H
#define OCRYPTO_AES_CCM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * AES-CCM encryption.
 *
 * @param[out] ct      Cyphertext.
 * @param[out] tag     Authentication tag.
 * @param      tag_len Tag length (4, 6, 8, 10, 12, 14, or 16).
 * @param      pt      Plaintext.
 * @param      pt_len  Plaintext length, 0 <= @p pt_len < 2^(8*(15-n_len)).
 * @param      key     AES key.
 * @param      size    Key size (16, 24, or 32).
 * @param      nonce   Nonce.
 * @param      n_len   Nonce length, 7 <= @p n_len <= 13.
 * @param      aa      Additional authentication data.
 * @param      aa_len  Additional authentication data length.
 *
 * @remark @p ct may be same as @p pt.
 */
void ocrypto_aes_ccm_encrypt (
    uint8_t *ct,
    uint8_t *tag, size_t tag_len,
    const uint8_t *pt, size_t pt_len,
    const uint8_t *key, size_t size,
    const uint8_t *nonce, size_t n_len,
    const uint8_t *aa, size_t aa_len);

/**
 * AES-CCM decryption.
 *
 * @param[out] pt      Plaintext.
 * @param      tag     Authentication tag.
 * @param      tag_len Tag length (4, 6, 8, 10, 12, 14, or 16).
 * @param      ct      Cyphertext.
 * @param      ct_len  Cyphertext length, 0 <= @p ct_len < 2^(8*(15-n_len)).
 * @param      key     AES key.
 * @param      size    Key size (16, 24, or 32).
 * @param      nonce   Nonce.
 * @param      n_len   Nonce length, 7 <= @p n_len <= 13.
 * @param      aa      Additional authentication data.
 * @param      aa_len  Additional authentication data length.
 *
 * @retval 0  If @p tag is valid.
 * @retval -1 Otherwise.
 *
 * @remark @p ct may be same as @p pt.
 */
int ocrypto_aes_ccm_decrypt (
    uint8_t *pt,
    const uint8_t *tag, size_t tag_len,
    const uint8_t *ct, size_t ct_len,
    const uint8_t *key, size_t size,
    const uint8_t *nonce, size_t n_len,
    const uint8_t *aa, size_t aa_len);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_AES_CCM_H */

/** @} */
