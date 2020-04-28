/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_aes AES - Advanced Encryption Standard APIs
 * @ingroup nrf_oberon
 * @{
 * @brief AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 * @}
 *
 * @defgroup nrf_oberon_aes_ctr AES-CTR - AES Counter Mode
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definitions and APIs for AES-CTR (AES Counter mode).
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 *
 * AES-CTR (AES counter mode) is an AES mode which effectively turns the block cipher into a stream
 * cipher. The AES block encryption is used on a value which is incremented for each new block.
 * The resulting cypher stream is then xor combined with the plaintext to get the ciphertext.
 * In contrast to AES itself, encryption and decryption operations are identical for AES-CTR.
 */

#ifndef OCRYPTO_AES_CTR_H
#define OCRYPTO_AES_CTR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@cond */
typedef struct {
    uint32_t xkey[60];
    uint8_t  counter[16];
    uint8_t  cypher[16];
    uint8_t  size;  // Key size (16, 24, or 32 bytes).
    uint32_t valid; // Valid bytes in cypher.
} ocrypto_aes_ctr_ctx;
/**@endcond */


/**@name Incremental AES-CTR encryption/decryption.
 *
 * This group of functions can be used to incrementally compute the
 * AES-CTR encryption/decryption for a given message.
 */
/**@{*/
/**
 * AES-CTR initialization.
 *
 * The context @p ctx is initialized using the given key @p key and initial vector @p iv.
 *
 * @param[out] ctx   Context.
 * @param      key   AES key.
 * @param      size  Key size (16, 24, or 32 bytes).
 * @param      iv    Initial vector.
 */
void ocrypto_aes_ctr_init(ocrypto_aes_ctr_ctx *ctx, const uint8_t *key, size_t size, const uint8_t iv[16]);

/**
 * AES-CTR incremental encryption.
 *
 * The plaintext @p pt is encrypted to the ciphertext @p ct using the context @p ctx.
 *
 * This function can be called repeatedly until the whole message is processed.
 *
 * @param      ctx    Context.
 * @param[out] ct     Ciphertext.
 * @param      pt     Plaintext.
 * @param      pt_len Length of @p pt and @p ct.
 *
 * @remark @p ct and @p pt can point to the same address.
 * @remark Initialization of the context @p ctx through
 *         @c ocrypto_aes_ctr_init is required before this function can be called.
 */
void ocrypto_aes_ctr_encrypt(ocrypto_aes_ctr_ctx *ctx, uint8_t* ct, const uint8_t* pt, size_t pt_len);

/**
 * AES-CTR incremental decryption.
 *
 * The ciphertext @p ct is decrypted to the plaintext @p pt using the context @p ctx.
 *
 * This function can be called repeatedly until the whole message is processed.
 *
 * @param      ctx    Context.
 * @param[out] pt     Plaintext.
 * @param      ct     Ciphertext.
 * @param      ct_len Length of @p ct and @p pt.
 *
 * @remark @p ct and @p pt can point to the same address.
 * @remark Initialization of the context @p ctx through
 *         @c ocrypto_aes_ctr_init is required before this function can be called.
 */
void ocrypto_aes_ctr_decrypt(ocrypto_aes_ctr_ctx *ctx, uint8_t* pt, const uint8_t* ct, size_t ct_len);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef OCRYPTO_AES_CTR_H */

/** @} */
