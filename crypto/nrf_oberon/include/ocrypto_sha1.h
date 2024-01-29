/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_sha SHA APIs
 * @ingroup ocrypto
 * @{
 * @brief SHA algorithms.
 * @}
 *
 * @defgroup ocrypto_sha_1 SHA-1 APIs
 * @ingroup ocrypto_sha
 * @{
 * @brief Type declarations and APIs for the SHA-1 algorithm.
 *
 * A fixed-sized message digest is computed from input data with arbitrary
 * length. The function is practically impossible to revert, and small changes
 * in the input message lead to major changes in the message digest.
 *
 * SHA-1 is no longer considered secure against well-funded opponents;
 * replacement by SHA-2 or SHA-3 is recommended.
 */

#ifndef OCRYPTO_SHA1_H
#define OCRYPTO_SHA1_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Length of SHA-1 hash.
 */
#define ocrypto_sha1_BYTES (20)


/**@cond */
typedef struct {
    uint32_t h[5];
    uint8_t  buffer[80 * 4];
    uint32_t length;
    size_t   in_length;
} ocrypto_sha1_ctx;
/**@endcond */


/**@name Incremental SHA-1 generator.
 *
 * This group of functions can be used to incrementally compute the SHA-1
 * hash for a given message.
 */
/**@{*/
/**
 * SHA-1 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx Generator state.
 */
void ocrypto_sha1_init(
    ocrypto_sha1_ctx *ctx);

/**
 * SHA-1 incremental data input.
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
 *         @c ocrypto_sha1_init is required before this function can be called.
 */
void ocrypto_sha1_update(
    ocrypto_sha1_ctx *ctx,
    const uint8_t *in, size_t in_len);

/**
 * SHA-1 output.
 *
 * The generator state @p ctx is updated to finalize the hash for the previously
 * processed message chunks. The hash is put into @p r.
 *
 * @param      ctx Generator state.
 * @param[out] r   Generated hash value.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c ocrypto_sha1_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c ocrypto_sha1_update and @c ocrypto_sha1_final unless it is
 *         reinitialized using @c ocrypto_sha1_init.
 */
void ocrypto_sha1_final(
    ocrypto_sha1_ctx *ctx,
    uint8_t r[ocrypto_sha1_BYTES]);
/**@}*/


/**
 * SHA-1 hash.
 *
 * The SHA-1 hash of a given input message @p in is computed and put into @p r.
 *
 * @param[out] r      Generated hash.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 */
void ocrypto_sha1(
    uint8_t r[ocrypto_sha1_BYTES],
    const uint8_t *in, size_t in_len);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
