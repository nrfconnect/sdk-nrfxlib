/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * SHA-512 is part of the SHA-2 family that is a set of cryptographic hash
 * functions designed by the NSA. It is the successor of the SHA-1 algorithm.
 *
 * A fixed-sized message digest is computed from variable length input data.
 * The function is practically impossible to revert, and small changes in the
 * input message lead to major changes in the message digest.
 */

#ifndef OCC_SHA512_H
#define OCC_SHA512_H

#include <stdint.h>
#include <stddef.h>

/**
 * Length of SHA-512 hash.
 */
#define occ_sha512_BYTES (64)


/**@cond */
typedef struct {
    uint64_t h[8];
    uint8_t  padded[128];
    uint32_t length;
    size_t   bytes;
} occ_sha512_ctx;
/**@endcond */


/**@name Incremental SHA-512 generator.
 * 
 * This group of functions can be used to incrementally compute the SHA-512
 * hash for a given message.
 *
 * **Example**
 * @include occ_sha512_incremental.c
 */
/**@{*/
/**
 * SHA-512 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx Generator state.
 */
void occ_sha512_init(occ_sha512_ctx *ctx);

/**
 * SHA-512 incremental data input.
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
 *         @c occ_sha512_init is required before this function can be called.
 */
void occ_sha512_update(occ_sha512_ctx *ctx,
                       const uint8_t *in, size_t in_len);

/**
 * SHA-512 output.
 *
 * The generator state @p ctx is updated to finalize the hash for the previously
 * processed message chunks. The hash is put into @p r.
 *
 * @param[out]    r   Generated hash value.
 * @param[in,out] ctx Generator state.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c occ_sha512_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c occ_sha512_update and @c occ_sha512_final unless it is
 *         reinitialized using @c occ_sha512_init.
 */
void occ_sha512_final(uint8_t r[occ_sha512_BYTES], occ_sha512_ctx *ctx);
/**@}*/

/**
 * SHA-512 hash.
 *
 * The SHA-512 hash of a given input message @p in is computed and put into @p r.
 *
 * **Example**
 * @include occ_sha512.c
 *
 * @param[out] r      Generated hash.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 */
void occ_sha512(uint8_t r[occ_sha512_BYTES],
                const uint8_t *in, size_t in_len);

#endif
