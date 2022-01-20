/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup ocrypto_p256 ECC secp256r1 low-level APIs
 * @ingroup ocrypto
 * @{
 * @brief Type declarations and APIs for low-level elliptic curve point operations
 * based on the NIST secp256r1 curve.
 */

#ifndef OCRYPTO_CURVE_P256_H
#define OCRYPTO_CURVE_P256_H

#include "ocrypto_sc_p256.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@cond */

// (x,y) only jacobian coordinates
typedef struct {
    ocrypto_mod_p256 x;
    ocrypto_mod_p256 y;
} ocrypto_cp_p256;

// P-256 invert context
typedef struct {
    ocrypto_mod_p256 x, x3, xn, t;
    int step;
} ocrypto_p256_invert_context;


typedef struct {
    ocrypto_cp_p256 p, q0, q1;
    uint32_t e[8]; // bits 0-255 of extended scalar, bit 256 = ~bit 255
    ocrypto_p256_invert_context inv;
    int ret, prev, dec, step;
} ocrypto_p256_mult_context;
/**@endcond */


/** Load r.x from bytes, keep r.y.
 *
 * @param[out]      r       Point with r.x loaded, r.y kept.
 * @param           p       x as as array of bytes.
 *
 * @retval 0  If @p r is a valid curve point.
 * @retval -1 Otherwise.
 */
int ocrypto_curve_p256_from32bytes(ocrypto_cp_p256 *r, const uint8_t p[32]);


/** Load point from bytes.
 *
 * @param[out]      r       Loaded point.
 * @param           p       Point as array of bytes.
 *
 * @retval 0  If @p r is a valid curve point.
 * @retval -1 Otherwise.
 */
int ocrypto_curve_p256_from64bytes(ocrypto_cp_p256 *r, const uint8_t p[64]);

/** Store p.x to bytes.
 *
 * @param[out]           r       x stored as array.
 * @param                p       Point with x to be stored.
 */
void ocrypto_curve_p256_to32bytes(uint8_t r[32], ocrypto_cp_p256 *p);

/** Store p.x to bytes.
 *
 * @param[out]           r       Point stored as array.
 * @param                p       Point to be stored.
 */
void ocrypto_curve_p256_to64bytes(uint8_t r[64], ocrypto_cp_p256 *p);

/** P256 scalar multiplication.
 *
 * r = p * s
 * r = [0,0] if p = [0,0] or s mod q = 0
 *
 * @param[out]      r       Output point.
 * @param           p       Input point.
 * @param           s       Scalar.
 *
 * @retval -1 If r = [0,0].
 * @retval 0  If 0 < s < q.
 * @retval 1  If s > q.
 */
int ocrypto_curve_p256_scalarmult(ocrypto_cp_p256 *r, const ocrypto_cp_p256 *p, const ocrypto_sc_p256 *s);

/** P256 scalar base multiplication.
 *
 * r = basePoint * s
 * r = [0,0] if s mod q = 0
 *
 * @param[out]      r       Output point.
 * @param           s       Scalar.
 *
 * @retval -1 If r = [0,0].
 * @retval 0  If 0 < s < q.
 * @retval 1  If s > q.
 */
int ocrypto_curve_p256_scalarmult_base(ocrypto_cp_p256 *r, const ocrypto_sc_p256 *s);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_CURVE_P256_H */

/** @} */
