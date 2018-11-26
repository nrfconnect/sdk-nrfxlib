/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * HMAC-SHA256 is an algorithm for message authentication using the
 * cryptographic hash function SHA256 and a reusable secret key. Users in
 * possession of the key can verify the integrity and authenticity of the
 * message.
 *
 * @see [RFC 2104 - HMAC: Keyed-Hashing for Message Authentication](http://tools.ietf.org/html/rfc2104)
 */
 
// REPLACEMENT

#ifndef OCC_HMAC_SHA256_H
#define OCC_HMAC_SHA256_H

#include <stdint.h>
#include <stddef.h>
#include "include/occ_sha256.h"


/**
 * Maximum key length.
 */
#define occ_hmac_sha256_KEY_BYTES_MAX (64)

/**
 * Length of the authenticator.
 */
#define occ_hmac_sha256_BYTES (32)


/**@cond */
typedef struct
{
    occ_sha256_ctx hash_ctx;
    uint8_t        ikey[occ_hmac_sha256_KEY_BYTES_MAX];
    uint8_t        okey[occ_hmac_sha256_KEY_BYTES_MAX];
    uint8_t        key[occ_hmac_sha256_KEY_BYTES_MAX];
} occ_hmac_sha256_ctx;
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
void occ_hmac_sha256_init(occ_hmac_sha256_ctx * ctx,
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
 *         @c occ_hmac_sha256_init is required before this function can be called.
 */
void occ_hmac_sha256_update(occ_hmac_sha256_ctx * ctx,
                            const uint8_t* in, size_t in_len);

/**
 * HMAC-SHA256 output.
 *
 * The generator state @p ctx is updated to finalize the HMAC calculation.
 * The HMAC digest is put into @p r.
 *
 * @param[out]    r   Generated HMAC digest.
 * @param[in,out] ctx Generator state.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c occ_hmac_sha256_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c occ_hmac_sha256_update and @c occ_hmac_sha256_final unless it is
 *         reinitialized using @c occ_hmac_sha256_init.
 */
void occ_hmac_sha256_final(uint8_t r[occ_hmac_sha256_BYTES],
                           occ_hmac_sha256_ctx * ctx);
/**@}*/


/**
 * HMAC-SHA256 algorithm.
 *
 * The input message @p in is authenticated using the key @p k. The computed
 * authenticator is put into @p r. To verify the authenticator, the recipient
 * needs to recompute the HMAC authenticator and can then compare it with the
 * received authenticator.
 *
 * **Example**
 * @include occ_hmac_sha256.c
 *
 * @param[out] r       HMAC output.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key. 0 <= @p key_len <= @c occ_hmac_sha256_KEY_BYTES_MAX.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 */
void occ_hmac_sha256(uint8_t r[occ_hmac_sha256_BYTES],
                     const uint8_t* key, size_t key_len,
                     const uint8_t* in, size_t in_len);

#endif
