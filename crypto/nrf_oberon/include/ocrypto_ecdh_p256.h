/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
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


/** ECDH P-256 public key r = n * p.
 *
 * @param[out]  r   Resulting public key.
 * @param       s   Secret key.
 *
 * @remark @p r may be the same as @p s.
 * @retval 0 If @p s is a legal secret key.
 */
int ocrypto_ecdh_p256_public_key(uint8_t r[64], const uint8_t s[32]);

/** ECDH P-256 common secret.
 *
 * @param[out]   r  Resulting common secret.
 * @param        s  Secret key.
 * @param        p  Public key.
 *
 * @remark @p r may be the same as @p s or @p p.
 * @retval 0 If @p s is a legal secret key and @p p is a legal public key.
 */
int ocrypto_ecdh_p256_common_secret(uint8_t r[32], const uint8_t s[32], const uint8_t p[64]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
