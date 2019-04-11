/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_p256 ECC secp256r1 low-level APIs.
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations and APIs for low-level elliptic curve point operations
 * based on the NIST secp256r1 curve.
 */

#ifndef OCRYPTO_CURVE_P256_H
#define OCRYPTO_CURVE_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ocrypto_sc_p256.h"


// (x,y) only jacobian coordinates
/**@cond */
typedef struct {
    ocrypto_mod_p256 x;
    ocrypto_mod_p256 y;
} ocrypto_cp_p256;
/**@endcond */

/** load r.x from bytes, recover r.y
 *
 * @param[out]      r       Point with r.x loaded, r.y kept
 * @param           p       x as as array of bytes
 *
 * @returns 0 if r is a legal curve point, -1 otherwise
 */
int ocrypto_curve_p256_from32bytes(ocrypto_cp_p256 *r, const uint8_t p[32]);


/** load point from bytes
 *
 * @param[out]      r       Loaded point
 * @param           p       Point as array of bytes
 *
 * @returns 0 if r is a legal curve point, -1 otherwise
 */
int ocrypto_curve_p256_from64bytes(ocrypto_cp_p256 *r, const uint8_t p[64]);

/** store p.x to bytes
 *
 * @param[out]           r       x stored as array
 * @param                p       Point with x to be stored
 */
void ocrypto_curve_p256_to32bytes(uint8_t r[32], ocrypto_cp_p256 *p);

/** store p.x to bytes
 *
 * @param[out]           r       Pont stored as array
 * @param                p       Point to be stored
 */
void ocrypto_curve_p256_to64bytes(uint8_t r[64], ocrypto_cp_p256 *p);

/** P256 scalar multiplication
 *
 * r = p * s
 * r = [0,0] if p = [0,0] or s mod q = 0
 *
 * @param[out]      r       Output point
 * @param           p       Input point
 * @param           s       Scalar
 *
 * @returns -1 if r = [0,0], 0 if 0 < s < q, 1 if s > q
 */
int ocrypto_curve_p256_scalarmult(ocrypto_cp_p256 *r, const ocrypto_cp_p256 *p, const ocrypto_sc_p256 *s);

/** P256 scalar base multiplication
 *
 * r = basePoint * s
 * r = [0,0] if s mod q = 0
 *
 * @param[out]      r       Output point
 * @param           s       Scalar
 * 
 * @returns -1 if r = [0,0], 0 if 0 < s < q, 1 if s > q
 */
int ocrypto_curve_p256_scalarmult_base(ocrypto_cp_p256 *r, const ocrypto_sc_p256 *s);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
