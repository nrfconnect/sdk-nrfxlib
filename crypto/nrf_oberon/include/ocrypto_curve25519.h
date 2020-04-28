/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_curve25519 ECC Curve25519 low-level APIs
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations and APIs for low-level elliptic curve point operations
 * based on Curve25519.
 *
 * Curve25519 is an elliptic curve offering 128 bits of security. It is designed
 * for use in the Elliptic Curve Diffie-Hellman (ECDH) key agreement scheme.
 *
 * @see [RFC 7748 - Elliptic Curves for Security](https://tools.ietf.org/html/rfc7748)
 * @see [Curve25519: high-speed elliptic-curve cryptography](http://cr.yp.to/ecdh.html)
 */

#ifndef OCRYPTO_CURVE25519_H
#define OCRYPTO_CURVE25519_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Length of a scalar.
 */
#define ocrypto_curve25519_SCALAR_BYTES (32)

/**
 * Length of a curve point.
 */
#define ocrypto_curve25519_BYTES (32)


/**
 * Curve25519 scalar multiplication `r = n * basePoint`.
 *
 * Given a secret key @p n, the corresponding Curve25519 public key is computed
 * and put into @p r.
 *
 * The inverse of this function is difficult to compute.
 *
 * @param[out] r Resulting curve point.
 * @param[in]  n Scalar factor.
 *
 * @remark @p r and @p n can point to the same address.
 */
void ocrypto_curve25519_scalarmult_base(
    uint8_t r[ocrypto_curve25519_BYTES],
    const uint8_t n[ocrypto_curve25519_SCALAR_BYTES]);

/**
 * Curve25519 scalar multiplication `r = n * p`.
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
 * @remark @p r and @p n can point to the same address.
 */
void ocrypto_curve25519_scalarmult(
    uint8_t r[ocrypto_curve25519_BYTES],
    const uint8_t n[ocrypto_curve25519_SCALAR_BYTES],
    const uint8_t p[ocrypto_curve25519_BYTES]);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_CURVE25519_H */

/** @} */
