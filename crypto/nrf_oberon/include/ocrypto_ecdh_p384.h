/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * ECDH P-384 is a specific implementation of a key agreement protocol.
 *
 * @defgroup ocrypto_ecdh_p384 ECDH P384 APIs
 * @ingroup ocrypto_ecdh
 * @{
 * @brief Type declarations and APIs for low-level elliptic curve point operations
 *        to do Elliptic Curve Diffie-Hellman based on the NIST secp384r1 curve.
 */

#ifndef OCRYPTO_ECDH_P384_H
#define OCRYPTO_ECDH_P384_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * ECDH P-384 secret key check.
 *
 * @param     sk Secret key to check.
 *
 * @retval 0  If @p sk is a valid secret key.
 * @retval -1 Otherwise.
 *
 * @remark To generate a valid secret key use the following code pattern:
 *            @code{.c}
 *            do get_random(sk); while (ocrypto_ecdh_p384_secret_key_check(sk));
 *            @endcode
 */
int ocrypto_ecdh_p384_secret_key_check(const uint8_t sk[48]);

/**
 * ECDH P-384 public key check.
 *
 * @param     pk Public key to check.
 *
 * @retval 0  If @p pk is a valid public key.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdh_p384_public_key_check(const uint8_t pk[96]);

/**
 * ECDH P-384 public key generation.
 *
 * Given a secret key @p sk the corresponding public key is computed and put
 * into @p pk.
 *
 * @param[out] pk Generated public key.
 * @param      sk Secret key. Must be pre-filled with random data.
 *
 * @retval 0  If @p sk is a valid secret key.
 * @retval -1 Otherwise.
 *
 * @remark @p pk may be same as @p sk.
 * @remark To generate a valid key pair use the following code pattern:
 *            @code{.c}
 *            do get_random(sk); while (ocrypto_ecdh_p384_public_key(pk, sk));
 *            @endcode
 */
int ocrypto_ecdh_p384_public_key(uint8_t pk[96], const uint8_t sk[48]);

/**
 * ECDH P-384 common secret.
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
int ocrypto_ecdh_p384_common_secret(uint8_t r[48], const uint8_t sk[48], const uint8_t pk[96]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
