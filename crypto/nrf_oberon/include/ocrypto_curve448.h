/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * Curve448 is an elliptic curve offering 224 bits of security. It is designed
 * for use in the elliptic curve Diffie-Hellman (ECDH) key agreement scheme.
 *
 * @see [RFC 7748 - Elliptic Curves for Security](https://tools.ietf.org/html/rfc7748)
 */

#ifndef OCRYPTO_CURVE448_H
#define OCRYPTO_CURVE448_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Length of a scalar.
 */
#define ocrypto_curve448_SCALAR_BYTES (56)

/**
 * Length of a curve point.
 */
#define ocrypto_curve448_BYTES (56)


/**
 * Curve448 scalar multiplication `r = n * basePoint`.
 *
 * Given a secret key @p n, the corresponding Curve448 public key is computed
 * and put into @p r.
 *
 * The inverse of this function is difficult to compute.
 *
 * @param[out] r Resulting curve point.
 * @param[in]  n Scalar factor.
 *
 * @remark @p r may be same as @p n.
 */
void ocrypto_curve448_scalarmult_base(
    uint8_t r[ocrypto_curve448_BYTES],
    const uint8_t n[ocrypto_curve448_SCALAR_BYTES]);

/**
 * Curve448 scalar multiplication `r = n * p`.
 *
 * A shared secret is computed from the local secret key @p n and another
 * party's public key @p p and put into @p r. The same shared secret is
 * generated when the other party combines its private key with the local public
 * key.
 *
 * @param[out] r Resulting curve point.
 * @param[in]  n Scalar factor.
 * @param[in]  p Point factor.
 *
 * @remark @p r may be same as @p n.
 */
void ocrypto_curve448_scalarmult(
    uint8_t r[ocrypto_curve448_BYTES],
    const uint8_t n[ocrypto_curve448_SCALAR_BYTES],
    const uint8_t p[ocrypto_curve448_BYTES]);


#ifdef __cplusplus
}
#endif

#endif
