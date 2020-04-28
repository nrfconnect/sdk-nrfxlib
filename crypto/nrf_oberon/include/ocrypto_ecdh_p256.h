/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_ecdh ECDH APIs
 * @ingroup nrf_oberon
 * @{
 * @brief APIs to do Elliptic Curve Diffie-Hellman using the NIST secp256r1 curve.
 */

#ifndef OCRYPTO_ECDH_P256_H
#define OCRYPTO_ECDH_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * ECDH P-256 public key generation `r = n * p`.
 *
 * Given a secret key @p s the corresponding public key is computed and put
 * into @p r.
 *
 * @param[out] r Generated public key.
 * @param      s Secret key. Must be pre-filled with random data.
 *
 * @retval 0  If @p s is a valid secret key.
 * @retval -1 Otherwise.
 *
 * @remark @p r may be same as @p s.
 */
int ocrypto_ecdh_p256_public_key(uint8_t r[64], const uint8_t s[32]);

/**
 * ECDH P-256 common secret.
 *
 * The common secret is computed from both the client's public key @p p
 * and the server's secret key @p s and put into @p r.
 *
 * @param[out] r Generated common secret.
 * @param      s Server private key.
 * @param      p Client public key.
 *
 * @retval 0  If @p s is a valid secret key and @p p is a valid public key.
 * @retval -1 Otherwise.
 *
 * @remark @p r may be same as @p s or @p p.
 */
int ocrypto_ecdh_p256_common_secret(uint8_t r[32], const uint8_t s[32], const uint8_t p[64]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
