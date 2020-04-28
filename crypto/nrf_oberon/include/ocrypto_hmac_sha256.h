/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_hmac HMAC - Hash-based Aessage Authentication Code
 * @ingroup nrf_oberon
 * @{
 * @brief HMAC is a hash-based Message Authentication Code utilizing a secure hash function.
 * @}
 * @defgroup nrf_oberon_hmac_256 HMAC APIs using SHA-256
 * @ingroup nrf_oberon_hmac
 * @{
 * @brief Type declarations and APIs for the HMAC-SHA256 algorithm.
 *
 * HMAC-SHA256 is an algorithm for message authentication using the
 * cryptographic hash function SHA256 and a reusable secret key. Users in
 * possession of the key can verify the integrity and authenticity of the
 * message.
 *
 * @see [RFC 2104 - HMAC: Keyed-Hashing for Message Authentication](http://tools.ietf.org/html/rfc2104)
 */

#ifndef OCRYPTO_HMAC_SHA256_H
#define OCRYPTO_HMAC_SHA256_H

#include <stddef.h>
#include <stdint.h>
#include "include/ocrypto_sha256.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maximum key length.
 */
#define ocrypto_hmac_sha256_KEY_BYTES_MAX (64)

/**
 * Length of the authenticator.
 */
#define ocrypto_hmac_sha256_BYTES (32)

/**@cond */
typedef struct
{
    ocrypto_sha256_ctx hash_ctx;
    uint8_t        ikey[ocrypto_hmac_sha256_KEY_BYTES_MAX];
    uint8_t        okey[ocrypto_hmac_sha256_KEY_BYTES_MAX];
    uint8_t        key[ocrypto_hmac_sha256_KEY_BYTES_MAX];
} ocrypto_hmac_sha256_ctx;
/**@endcond */


/**@name Incremental HMAC-SHA256 generator.
 *
 * This group of functions can be used to incrementally compute HMAC-SHA256
 * for a given message.
 */
/**@{*/

/**
 * HMAC-SHA256 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx     Generator state.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key.
 */
void ocrypto_hmac_sha256_init(ocrypto_hmac_sha256_ctx * ctx,
                              const uint8_t* key, size_t key_len);

/**
 * HMAC-SHA256 incremental data input.
 *
 * The generator state @p ctx is updated to hash a message chunk @p in.
 *
 * This function can be called repeatedly until the whole message is processed.
 *
 * @param[in,out] ctx    Generator state.
 * @param         in     Input data.
 * @param         in_len Length of @p in.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c ocrypto_hmac_sha256_init is required before this function can be called.
 */
void ocrypto_hmac_sha256_update(ocrypto_hmac_sha256_ctx * ctx,
                                const uint8_t* in, size_t in_len);

/**
 * HMAC-SHA256 output.
 *
 * The generator state @p ctx is updated to finalize the HMAC calculation.
 * The HMAC digest is put into @p r.
 *
 * @param[in,out] ctx Generator state.
 * @param[out]    r   Generated HMAC digest.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c ocrypto_hmac_sha256_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c ocrypto_hmac_sha256_update and @c ocrypto_hmac_sha256_final unless it is
 *         reinitialized using @c ocrypto_hmac_sha256_init.
 */
void ocrypto_hmac_sha256_final(ocrypto_hmac_sha256_ctx * ctx,
                               uint8_t r[ocrypto_hmac_sha256_BYTES]);
/**@}*/


/**
 * HMAC-SHA256 algorithm.
 *
 * The input message @p in is authenticated using the key @p k. The computed
 * authenticator is put into @p r. To verify the authenticator, the recipient
 * needs to recompute the HMAC authenticator and can then compare it with the
 * received authenticator.
 *
 * @param[out] r       HMAC output.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key. 0 <= @p key_len <= @c ocrypto_hmac_sha256_KEY_BYTES_MAX.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 */
void ocrypto_hmac_sha256(
    uint8_t r[ocrypto_hmac_sha256_BYTES],
    const uint8_t* key, size_t key_len,
    const uint8_t* in, size_t in_len);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_HMAC_SHA256_H */

/** @} */
