/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
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
 * @defgroup ocrypto_aes_ecb AES-ECB - AES Electronic Codebook
 * @ingroup ocrypto_aes
 * @{
 * @brief Type definitions and APIs for AES-ECB (AES Electronic Codebook).
 *
 * AES-ECB (AES Electronic Codebook) is a simple AES block cipher mode.
 */

#ifndef OCRYPTO_AES_ECB_H
#define OCRYPTO_AES_ECB_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**@cond */
typedef struct {
    uint32_t xkey[60];
    uint8_t  key_size; // Key size (16, 24, or 32 bytes).
} ocrypto_aes_ecb_ctx;
/**@endcond */


/**@name Incremental AES-ECB encryption/decryption.
 *
 * This group of functions can be used to incrementally compute the
 * AES-ECB encryption/decryption for a given message.
 */
/**@{*/

/**
 * AES-ECB encrypt initialization.
 *
 * The context @p ctx is initialized using the given key @p key.
 *
 * @param[out] ctx   Context.
 * @param      key   AES key.
 * @param      size  Key size (16, 24, or 32 bytes).
 */
void ocrypto_aes_ecb_init_enc(ocrypto_aes_ecb_ctx *ctx, const uint8_t *key, size_t size);

/**
 * AES-ECB decrypt initialization.
 *
 * The context @p ctx is initialized using the given key @p key.
 *
 * @param[out] ctx   Context.
 * @param      key   AES key.
 * @param      size  Key size (16, 24, or 32 bytes).
 */
void ocrypto_aes_ecb_init_dec(ocrypto_aes_ecb_ctx *ctx, const uint8_t *key, size_t size);

/**
 * AES-ECB incremental encryption.
 *
 * The plaintext @p pt is encrypted to the ciphertext @p ct using the context @p ctx.
 *
 * This function can be called repeatedly until the whole message is processed.
 *
 * @param      ctx    Context.
 * @param[out] ct     Ciphertext.
 * @param      pt     Plaintext.
 * @param      pt_len Length of @p pt and @p ct. Must be a multiple of the block size.
 *
 * @remark @p ct and @p pt can point to the same address.
 * @remark Initialization of the context @p ctx through
 *         @c ocrypto_aes_ctr_init is required before this function can be called.
 */
void ocrypto_aes_ecb_update_enc(ocrypto_aes_ecb_ctx *ctx, uint8_t* ct, const uint8_t* pt, size_t pt_len);

/**
 * AES-ECB incremental decryption.
 *
 * The ciphertext @p ct is decrypted to the plaintext @p pt using the context @p ctx.
 *
 * This function can be called repeatedly until the whole message is processed.
 *
 * @param      ctx    Context.
 * @param[out] pt     Plaintext.
 * @param      ct     Ciphertext.
 * @param      ct_len Length of @p ct and @p pt. Must be a multiple of the block size.
 *
 * @remark @p ct and @p pt can point to the same address.
 * @remark Initialization of the context @p ctx through
 *         @c ocrypto_aes_ctr_init is required before this function can be called.
 */
void ocrypto_aes_ecb_update_dec(ocrypto_aes_ecb_ctx *ctx, uint8_t* pt, const uint8_t* ct, size_t ct_len);
/** @} */

/**
 * AES-ECB encryption.
 *
 * @param[out] ct     Ciphertext.
 * @param      pt     Plaintext.
 * @param      pt_len Length of @p ct and @p pt.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32).
 *
 * @remark @p ct may be same as @p pt.
 */
void ocrypto_aes_ecb_encrypt(
    uint8_t* ct, const uint8_t* pt, size_t pt_len, const uint8_t *key, size_t size);

/**
 * AES-ECB decryption.
 *
 * @param[out] pt     Plaintext.
 * @param      ct     Ciphertext.
 * @param      ct_len Length of @p ct and @p pt.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32).
 *
 * @remark @p ct may be same as @p pt.
 */
void ocrypto_aes_ecb_decrypt(
    uint8_t* pt, const uint8_t* ct, size_t ct_len, const uint8_t *key, size_t size);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
