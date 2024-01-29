/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_sha_512 SHA-512 APIs
 * @ingroup ocrypto_sha
 * @{
 * @brief Type declarations and APIs for the SHA-512 algorithm.
 *
 * SHA-512 is part of the SHA-2 family that is a set of cryptographic hash
 * functions designed by the NSA. It is the successor of the SHA-1 algorithm.
 *
 * A fixed-sized message digest is computed from variable length input data.
 * The function is practically impossible to revert, and small changes in the
 * input message lead to major changes in the message digest.
 */

#ifndef OCRYPTO_SHA512_H
#define OCRYPTO_SHA512_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Length of SHA-512 hash.
 */
#define ocrypto_sha512_BYTES (64)


/**@cond */
typedef struct {
    uint64_t h[8], v[8], w[16];
    uint8_t  buffer[128];
    uint32_t length;
    size_t   in_length;
} ocrypto_sha512_ctx;
/**@endcond */


/**@name Incremental SHA-512 generator.
 *
 * This group of functions can be used to incrementally compute the SHA-512
 * hash for a given message.
 */
/**@{*/
/**
 * SHA-512 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx Generator state.
 */
void ocrypto_sha512_init(
    ocrypto_sha512_ctx *ctx);

/**
 * SHA-512 incremental data input.
 *
 * The generator state @p ctx is updated to hash a message chunk @p in.
 *
 * This function can be called repeatedly until the whole message is processed.
 *
 * @param      ctx    Generator state.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c ocrypto_sha512_init is required before this function can be called.
 */
void ocrypto_sha512_update(
    ocrypto_sha512_ctx *ctx,
    const uint8_t *in, size_t in_len);

/**
 * SHA-512 output.
 *
 * The generator state @p ctx is updated to finalize the hash for the previously
 * processed message chunks. The hash is put into @p r.
 *
 * @param      ctx Generator state.
 * @param[out] r   Generated hash value.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c ocrypto_sha512_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c ocrypto_sha512_update and @c ocrypto_sha512_final unless it is
 *         reinitialized using @c ocrypto_sha512_init.
 */
void ocrypto_sha512_final(
    ocrypto_sha512_ctx *ctx,
    uint8_t r[ocrypto_sha512_BYTES]);
/**@}*/

/**
 * SHA-512 hash.
 *
 * The SHA-512 hash of a given input message @p in is computed and put into @p r.
 *
 * @param[out] r      Generated hash.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 */
void ocrypto_sha512(
    uint8_t r[ocrypto_sha512_BYTES],
    const uint8_t *in, size_t in_len);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
