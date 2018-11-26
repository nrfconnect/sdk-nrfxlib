/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * SHA-256 is part of the SHA-2 family that is a set of cryptographic hash
 * functions designed by the NSA. It is the successor of the SHA-1 algorithm.
 *
 * A fixed-sized message digest is computed from variable length input data.
 * The function is practically impossible to revert, and small changes in the
 * input message lead to major changes in the message digest.
 */

#ifndef OCC_SHA256_H
#define OCC_SHA256_H

#include <stdint.h>
#include <stddef.h>

/**
 * Length of SHA-256 hash.
 */
#define occ_sha256_BYTES (32)


/**@cond */
typedef struct {
    uint32_t h[8];
    uint8_t  padded[64];
    uint32_t length;
    size_t   bytes;
} occ_sha256_ctx;
/**@endcond */


/**@name Incremental SHA-256 generator.
 * 
 * This group of functions can be used to incrementally compute the SHA-256
 * hash for a given message.
 *
 * **Example**
 * @include occ_sha256_incremental.c
 */
/**@{*/
/**
 * SHA-256 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx Generator state.
 */
void occ_sha256_init(occ_sha256_ctx *ctx);

/**
 * SHA-256 incremental data input.
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
 *         @c occ_sha256_init is required before this function can be called.
 */
void occ_sha256_update(occ_sha256_ctx *ctx,
                       const uint8_t *in, size_t in_len);

/**
 * SHA-256 output.
 *
 * The generator state @p ctx is updated to finalize the hash for the previously
 * processed message chunks. The hash is put into @p r.
 *
 * @param[out]    r   Generated hash value.
 * @param[in,out] ctx Generator state.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c occ_sha256_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c occ_sha256_update and @c occ_sha256_final unless it is
 *         reinitialized using @c occ_sha256_init.
 */
void occ_sha256_final(uint8_t r[occ_sha256_BYTES], occ_sha256_ctx *ctx);
/**@}*/

/**
 * SHA-256 hash.
 *
 * The SHA-256 hash of a given input message @p in is computed and put into @p r.
 *
 * **Example**
 * @include occ_sha256.c
 *
 * @param[out] r      Generated hash.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 */
void occ_sha256(uint8_t r[occ_sha256_BYTES],
                const uint8_t *in, size_t in_len);

#endif
