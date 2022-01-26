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
 * @defgroup ocrypto_aes_ccm AES-CCM - Counter mode with CBC-MAC
 * @ingroup ocrypto_aes
 * @{
 * @brief Type definitions and APIs for AES-CCM (AES Counter mode with CBC-MAC).
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


/**@cond */
typedef struct {
    uint32_t xkey[60];
    uint8_t  counter[16];
    uint8_t  cipher[16];
    uint8_t  x[16];
    uint8_t  position; // Current position in cipher.
    uint8_t  length;   // Data length added to x.
    uint8_t  cnt_pos;  // Count position in counter.
    uint8_t  key_size; // Key size (16, 24, or 32 bytes).
    size_t   pt_len;   // Total plaintext length.
    size_t   aa_len;   // Total aad length.
} ocrypto_aes_ccm_ctx;
/**@endcond */


/**@name Incremental AES-CCM encryption/decryption.
 *
 * This group of functions can be used to incrementally compute the
 * AES-CCM encryption/decryption for a given message.
 */
/**@{*/

/**
 * AES-CCM initialization.
 *
 * The context @p ctx is initialized using the given key @p key.
 *
 * @param[out] ctx     Context.
 * @param      key     AES key.
 * @param      size    Key size (16, 24, or 32 bytes).
 * @param      nonce   Nonce.
 * @param      n_len   Nonce length, 7 <= @p n_len <= 13.
 * @param      tag_len Tag length (4, 6, 8, 10, 12, 14, or 16).
 * @param      pt_len  Plaintext length, 0 <= @p pt_len < 2^(8*(15-n_len)).
 * @param      aa_len  Additional authentication data length.
 */
void ocrypto_aes_ccm_init(ocrypto_aes_ccm_ctx *ctx, const uint8_t *key, size_t size,
                          const uint8_t *nonce, size_t n_len, size_t tag_len, size_t pt_len, size_t aa_len);


/**
 * AES-CCM incremental aad input.
 *
 * The generator state @p ctx is updated to include a data chunk @p aa.
 *
 * This function can be called repeatedly until the whole data is processed.
 *
 * @param      ctx    Generator state.
 * @param      aa     Additional authenticated data.
 * @param      aa_len Length of @p aa.
 *
 * @remark Initialization of the context @p ctx through
 *         @c ocrypto_aes_ccm_init is required before this function can be called.
 * @remark @c ocrypto_aes_ccm_update_aad must be called before any call to
 *         @c ocrypto_aes_ccm_update_enc or @c ocrypto_aes_ccm_update_dec.
 */
void ocrypto_aes_ccm_update_aad(ocrypto_aes_ccm_ctx *ctx, const uint8_t *aa, size_t aa_len);

/**
 * AES-CCM incremental encryption.
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
 *         @c ocrypto_aes_ccm_init is required before this function can be called.
 */
void ocrypto_aes_ccm_update_enc(ocrypto_aes_ccm_ctx *ctx, uint8_t* ct, const uint8_t* pt, size_t pt_len);

/**
 * AES-CCM incremental decryption.
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
 *         @c ocrypto_aes_ccm_init is required before this function can be called.
 */
void ocrypto_aes_ccm_update_dec(ocrypto_aes_ccm_ctx *ctx, uint8_t* pt, const uint8_t* ct, size_t ct_len);


/**
 * AES-CCM final encoder step.
 *
 * The generator state @p ctx is used to finalize the encryption and generate the tag.
 *
 * @param      ctx     Generator state.
 * @param[out] tag     Generated authentication tag.
 * @param      tag_len Length of @p tag.
 */
void ocrypto_aes_ccm_final_enc(ocrypto_aes_ccm_ctx *ctx, uint8_t *tag, size_t tag_len);

/**
 * AES-CCM final decoder step.
 *
 * The generator state @p ctx is used to finalize the decryption and check the tag.
 *
 * @param      ctx   Generator state.
 * @param      tag   Received authentication tag.
 * @param      tag_len Length of @p tag.
 *
 * @retval 0  If @p tag is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_aes_ccm_final_dec(ocrypto_aes_ccm_ctx *ctx, const uint8_t *tag, size_t tag_len);

/**@}*/


/**
 * AES-CCM encryption.
 *
 * @param[out] ct      Ciphertext.
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
void ocrypto_aes_ccm_encrypt(
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
 * @param      ct      Ciphertext.
 * @param      ct_len  Ciphertext length, 0 <= @p ct_len < 2^(8*(15-n_len)).
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
int ocrypto_aes_ccm_decrypt(
    uint8_t *pt,
    const uint8_t *tag, size_t tag_len,
    const uint8_t *ct, size_t ct_len,
    const uint8_t *key, size_t size,
    const uint8_t *nonce, size_t n_len,
    const uint8_t *aa, size_t aa_len);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
