/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * Curve25519 is an elliptic curve offering 128 bits of security. It is designed
 * for use in the elliptic curve Diffie-Hellman (ECDH) key agreement scheme.
 *
 * @see [RFC 7748 - Elliptic Curves for Security](https://tools.ietf.org/html/rfc7748)
 * @see [Curve25519: high-speed elliptic-curve cryptography](http://cr.yp.to/ecdh.html)
 */

#ifndef OCC_CURVE25519_H
#define OCC_CURVE25519_H

#include <stdint.h>


/**
 * Length of a scalar.
 */
#define occ_curve25519_SCALAR_BYTES (32)

/**
 * Length of a curve point.
 */
#define occ_curve25519_BYTES (32)


/**
 * Curve25519 scalar multiplication `r = n * basePoint`.
 *
 * Given a secret key @p n, the corresponding Curve25519 public key is computed
 * and put into @p r.
 *
 * The inverse of this function is difficult to compute.
 *
 * **Example**
 * @include occ_curve25519_scalarmult_base.c
 *
 * @param[out] r Resulting curve point.
 * @param[in]  n Scalar factor.
 *
 * @remark @p r and @p n can point to the same address.
 */
void occ_curve25519_scalarmult_base(uint8_t r[occ_curve25519_BYTES],
                                    const uint8_t n[occ_curve25519_SCALAR_BYTES]);

/**
 * Curve25519 scalar multiplication `r = n * p`.
 *
 * A shared secret is computed from the local secret key @p n and another
 * party's public key @p p and put into @p r. The same shared secret is
 * generated when the other party combines its private key with the local public
 * key.
 *
 * **Example**
 * @include occ_curve25519_scalarmult.c
 *
 * @param[out] r Resulting curve point.
 * @param[in]  n Scalar factor.
 * @param[in]  p Point factor.
 *
 * @remark @p r and @p n can point to the same address.
 */
void occ_curve25519_scalarmult(uint8_t r[occ_curve25519_BYTES],
                               const uint8_t n[occ_curve25519_SCALAR_BYTES],
                               const uint8_t p[occ_curve25519_BYTES]);

#endif
