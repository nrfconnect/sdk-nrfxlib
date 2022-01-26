/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup ocrypto_ecdh ECDH - Elliptic Curve Diffie-Hellman APIs
 * @ingroup ocrypto
 * @{
 * @brief ECDH (Elliptic Curve Diffie-Hellman) elliptic curve point operations
 *        to do Elliptic Curve Diffie-Hellman Algorithm.
 * @}
 *
 * @defgroup ocrypto_ecdh_p256 ECDH P256 APIs
 * @ingroup ocrypto_ecdh
 * @{
 * @brief Type declarations and APIs for low-level elliptic curve point operations
 *        to do Elliptic Curve Diffie-Hellman based on the NIST secp256r1 curve.
 */

#ifndef OCRYPTO_ECDH_P256_H
#define OCRYPTO_ECDH_P256_H

#include "ocrypto_types.h"
#include "ocrypto_curve_p256.h"


#ifdef __cplusplus
extern "C" {
#endif


/**@cond */
typedef struct {
    ocrypto_p256_mult_context mul;
    int ret;
} ocrypto_ecdh_p256_context;
/**@endcond */


/**
 * ECDH P-256 public key generation `r = n * p`.
 *
 * Given a secret key @p sk the corresponding public key is computed and put
 * into @p r.
 *
 * @param[out] r  Generated public key.
 * @param      sk Secret key. Must be pre-filled with random data.
 *
 * @retval 0  If @p sk is a valid secret key.
 * @retval -1 Otherwise.
 *
 * @remark @p r may be same as @p sk.
 */
int ocrypto_ecdh_p256_public_key(uint8_t r[64], const uint8_t sk[32]);

/**
 * ECDH P-256 common secret.
 *
 * The common secret is computed from both the client's public key @p pk
 * and the server's secret key @p sk and put into @p r.
 *
 * @param[out] r  Generated common secret.
 * @param      sk Server private key.
 * @param      pk Client public key.
 *
 * @retval 0  If @p sk is a valid secret key and @p pk is a valid public key.
 * @retval -1 Otherwise.
 *
 * @remark @p r may be same as @p sk or @p pk.
 */
int ocrypto_ecdh_p256_common_secret(uint8_t r[32], const uint8_t sk[32], const uint8_t pk[64]);


/**@name Incremental ECDH P-256 calculation.
 *
 * This group of functions can be used to incrementally calculate
 * the ECDH P-256 public key and common secret.
 * Each call completes in less than 25ms on a 16MHz Cortex-M0.
 *
 * Use pattern:
 *
 * Public Key:
 * @code
 *   ocrypto_ecdh_p256_public_key_init(ctx, sKey);
 *   while (ocrypto_ecdh_p256_public_key_iterate(ctx));
 *   res = ocrypto_ecdh_p256_public_key_final(ctx, pKey);
 * @endcode
 * Common Secret:
 * @code
 *   ocrypto_ecdh_p256_common_secret_init(ctx, sKey, pKey);
 *   while (ocrypto_ecdh_p256_common_secret_iterate(ctx));
 *   res = ocrypto_ecdh_p256_common_secret_final(ctx, secret);
 * @endcode
 */
/**@{*/

/**
 * Incremental ECDH P-256 public key generation start.
 *
 * Key generation is started and the context @p ctx is initialized by this function.
 *
 * @param[out] ctx Context.
 * @param      sk  Secret key. Must be pre-filled with random data.
 */
void ocrypto_ecdh_p256_public_key_init(ocrypto_ecdh_p256_context *ctx, const uint8_t sk[32]);

/**
 * Incremental ECDH P-256 public key generation step.
 *
 * The key calculation is advanced and the context @p ctx is updated by this function.
 *
 * @param     ctx Context.
 *
 * @retval 1  If another call to @c ocrypto_ecdh_p256_public_key_init is needed.
 * @retval 0  If key generation should be completed by a call to @c ocrypto_ecdh_p256_public_key_final.
 */
int ocrypto_ecdh_p256_public_key_iterate(ocrypto_ecdh_p256_context *ctx);

/**
 * Incremental ECDH P-256 public key generation final step.
 *
 * Key generation is finalized and the context @p ctx is used to generate the key.
 *
 * @param      ctx Context.
 * @param[out] r   Generated public key.
 *
 * @retval 0  If @p sk is a valid secret key.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdh_p256_public_key_final(ocrypto_ecdh_p256_context *ctx, uint8_t r[64]);

/**
 * Incremental ECDH P-256 common secret generation start.
 *
 * Common secret calculation is started and the context @p ctx is initialized by this function.
 *
 * @param[out] ctx Context.
 * @param      sk  Server private key.
 * @param      pk  Client public key.
 */
void ocrypto_ecdh_p256_common_secret_init(ocrypto_ecdh_p256_context *ctx, const uint8_t sk[32], const uint8_t pk[64]);

/**
 * Incremental ECDH P-256 common secret generation step.
 *
 * Common secret calculation is advanced and the context @p ctx is updated by this function.
 *
 * @param     ctx Context.
 *
 * @retval 1  If another call to @c ocrypto_ecdh_p256_common_secret_iterate is needed.
 * @retval 0  If key generation should be completed by a call to @c ocrypto_ecdh_p256_common_secret_final.
 */
int ocrypto_ecdh_p256_common_secret_iterate(ocrypto_ecdh_p256_context *ctx);

/**
 * Incremental ECDH P-256 common secret generation final step.
 *
 * Common secret calculation is finalized and the context @p ctx is used to generate the secret.
 *
 * @param      ctx Context.
 * @param[out] r   Generated common secret.
 *
 * @retval 0  If @p sk is a valid secret key and @p pk is a valid public key.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdh_p256_common_secret_final(ocrypto_ecdh_p256_context *ctx, uint8_t r[32]);
/**@}*/


#ifdef __cplusplus
}
#endif

#endif

/** @} */
