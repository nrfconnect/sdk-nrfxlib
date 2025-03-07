/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**
 * @defgroup ocrypto_ecdh_p256k1 ECDH secp256k1
 * @ingroup ocrypto_key_agreement
 * @{
 * @brief Elliptic curve Diffie-Hellman algorithm based on the NIST secp256k1 curve.
 *
 * ECDH secp256k1 is a specific implementation of a key agreement protocol.
 * 
 * @see [NIST - SP 800-56A Rev. 3](https://csrc.nist.gov/pubs/sp/800/56/a/r3/final)
 */

/**
 * @file
 * @brief Elliptic curve Diffie-Hellman algorithm based on the NIST secp256k1 curve.
 */

#ifndef OCRYPTO_ECDH_P256K1_H
#define OCRYPTO_ECDH_P256K1_H

#include "ocrypto_types.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * ECDH secp256k1 public key generation `r = n * p`.
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
int ocrypto_ecdh_p256k1_public_key(uint8_t r[64], const uint8_t sk[32]);

/**
 * ECDH secp256k1 common secret.
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
int ocrypto_ecdh_p256k1_common_secret(uint8_t r[32], const uint8_t sk[32], const uint8_t pk[64]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
