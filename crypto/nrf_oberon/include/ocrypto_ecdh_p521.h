/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**
 * @defgroup ocrypto_ecdh_521 ECDH P-521
 * @ingroup ocrypto_key_agreement
 * @{
 * @brief Elliptic curve Diffie-Hellman algorithm based on the NIST P-521 curve (aka secp521r1).
 *
 * ECDH P-521 is a specific implementation of a key agreement protocol.
 * 
 * @see [NIST - SP 800-56A Rev. 3](https://csrc.nist.gov/pubs/sp/800/56/a/r3/final)
 */

/**
 * @file
 * @brief Elliptic curve Diffie-Hellman algorithm based on the NIST P-521 curve (aka secp521r1).
 */

#ifndef OCRYPTO_ECDH_P521_H
#define OCRYPTO_ECDH_P521_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
* ECDH P-521 secret key check.
*
* @param     sk Secret key to check.
*
* @retval 0  If @p sk is a valid secret key.
* @retval -1 Otherwise.
*
* @remark To generate a valid secret key use the following code pattern:
*            @code{.c}
*            do get_random(sk); while (ocrypto_ecdh_p521_secret_key_check(sk));
*            @endcode
*/
int ocrypto_ecdh_p521_secret_key_check(const uint8_t sk[66]);

/**
* ECDH P-521 public key check.
*
* @param     pk Public key to check.
*
* @retval 0  If @p pk is a valid public key.
* @retval -1 Otherwise.
*/
int ocrypto_ecdh_p521_public_key_check(const uint8_t pk[132]);

/**
 * ECDH P-521 public key generation.
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
 *            do get_random(sk); while (ocrypto_ecdh_p521_public_key(pk, sk));
 *            @endcode
 */
int ocrypto_ecdh_p521_public_key(uint8_t pk[132], const uint8_t sk[66]);

/**
 * ECDH P-521 common secret.
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
int ocrypto_ecdh_p521_common_secret(uint8_t r[66], const uint8_t sk[66], const uint8_t pk[132]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
