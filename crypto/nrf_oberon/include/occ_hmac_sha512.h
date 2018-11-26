/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * HMAC-SHA512 is an algorithm for message authentication using the
 * cryptographic hash function SHA512 and a reusable secret key. Users in
 * possession of the key can verify the integrity and authenticity of the
 * message.
 *
 * @see [RFC 2104 - HMAC: Keyed-Hashing for Message Authentication](http://tools.ietf.org/html/rfc2104)
 */
 
#ifndef OCC_HMAC_SHA512_H
#define OCC_HMAC_SHA512_H

#include <stdint.h>
#include <stddef.h>
#include "include/occ_sha512.h"


/**
 * Maximum key length.
 */
#define occ_hmac_sha512_KEY_BYTES_MAX (128)

/**
 * Length of the authenticator.
 */
#define occ_hmac_sha512_BYTES (64)


/**@cond */
typedef struct
{
    occ_sha512_ctx hash_ctx;
    uint8_t        ikey[occ_hmac_sha512_KEY_BYTES_MAX];
    uint8_t        okey[occ_hmac_sha512_KEY_BYTES_MAX];
    uint8_t        key[occ_hmac_sha512_KEY_BYTES_MAX];
} occ_hmac_sha512_ctx;
/**@endcond */

/**@name Incremental HMAC-SHA512 generator.
 * 
 * This group of functions can be used to incrementally compute HMAC-SHA512
 * for a given message.
 */
/**@{*/


/**
 * HMAC-SHA512 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx     Generator state.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key.
 */
void occ_hmac_sha512_init(occ_hmac_sha512_ctx * ctx,
                          const uint8_t* key, size_t key_len);

/**
 * HMAC-SHA512 incremental data input.
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
 *         @c occ_hmac_sha512_init is required before this function can be called.
 */
void occ_hmac_sha512_update(occ_hmac_sha512_ctx * ctx,
                            const uint8_t* in, size_t in_len);

/**
 * HMAC-SHA512 output.
 *
 * The generator state @p ctx is updated to finalize the HMAC calculation.
 * The HMAC digest is put into @p r.
 *
 * @param[out]    r   Generated HMAC digest.
 * @param[in,out] ctx Generator state.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c occ_hmac_sha512_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c occ_hmac_sha512_update and @c occ_hmac_sha512_final unless it is
 *         reinitialized using @c occ_hmac_sha512_init.
 */
void occ_hmac_sha512_final(uint8_t r[occ_hmac_sha512_BYTES],
                           occ_hmac_sha512_ctx * ctx);
/**@}*/


/**
 * HMAC-SHA512 algorithm.
 *
 * The input message @p in is authenticated using the key @p k. The computed
 * authenticator is put into @p r. To verify the authenticator, the recipient
 * needs to recompute the HMAC authenticator and can then compare it with the
 * received authenticator.
 *
 * **Example**
 * @include occ_hmac_sha512.c
 *
 * @param[out] r       HMAC output.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key. 0 <= @p key_len <= @c occ_hmac_sha512_KEY_BYTES_MAX.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 */
void occ_hmac_sha512(uint8_t r[occ_hmac_sha512_BYTES],
                     const uint8_t* key, size_t key_len,
                     const uint8_t* in, size_t in_len);

#endif
