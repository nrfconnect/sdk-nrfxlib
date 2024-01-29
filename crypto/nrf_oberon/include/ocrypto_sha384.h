/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_sha_384 SHA-384 APIs
 * @ingroup ocrypto_sha
 * @{
 * @brief Type declarations and APIs for the SHA-384 algorithm.
 *
 * SHA-384 is part of the SHA-2 family that is a set of cryptographic hash
 * functions designed by the NSA. It is the successor of the SHA-1 algorithm.
 *
 * A fixed-sized message digest is computed from variable length input data.
 * The function is practically impossible to revert, and small changes in the
 * input message lead to major changes in the message digest.
 */

#ifndef OCRYPTO_SHA384_H
#define OCRYPTO_SHA384_H

#include "ocrypto_sha512.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Length of SHA-384 hash.
 */
#define ocrypto_sha384_BYTES (48)

/**@cond */
typedef ocrypto_sha512_ctx ocrypto_sha384_ctx;
/**@endcond */


/**@name Incremental SHA-384 generator.
 *
 * This group of functions can be used to incrementally compute the SHA-384
 * hash for a given message.
 */
/**@{*/
/**
 * SHA-384 initialization.
 *
 * The generator state @p ctx is initialized by this function.
 *
 * @param[out] ctx Generator state.
 */
void ocrypto_sha384_init(
    ocrypto_sha384_ctx *ctx);

/**
 * SHA-384 incremental data input.
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
 *         @c ocrypto_sha384_init is required before this function can be called.
 */
void ocrypto_sha384_update(
    ocrypto_sha384_ctx *ctx,
    const uint8_t *in, size_t in_len);

/**
 * SHA-384 output.
 *
 * The generator state @p ctx is updated to finalize the hash for the previously
 * processed message chunks. The hash is put into @p r.
 *
 * @param      ctx Generator state.
 * @param[out] r   Generated hash value.
 *
 * @remark Initialization of the generator state @p ctx through
 *         @c ocrypto_sha384_init is required before this function can be called.
 *
 * @remark After return, the generator state @p ctx must no longer be used with
 *         @c ocrypto_sha384_update and @c ocrypto_sha384_final unless it is
 *         reinitialized using @c ocrypto_sha384_init.
 */
void ocrypto_sha384_final(
    ocrypto_sha384_ctx *ctx,
    uint8_t r[ocrypto_sha384_BYTES]);
/**@}*/

/**
 * SHA-384 hash.
 *
 * The SHA-384 hash of a given input message @p in is computed and put into @p r.
 *
 * @param[out] r      Generated hash.
 * @param      in     Input data.
 * @param      in_len Length of @p in.
 */
void ocrypto_sha384(
    uint8_t r[ocrypto_sha384_BYTES],
    const uint8_t *in, size_t in_len);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
