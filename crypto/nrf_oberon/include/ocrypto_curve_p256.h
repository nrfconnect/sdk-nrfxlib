/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef OCRYPTO_CURVE_P256_H
#define OCRYPTO_CURVE_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ocrypto_sc_p256.h"


// (x,y) only jacobian coordinates
typedef struct {
    ocrypto_mod_p256 x;
    ocrypto_mod_p256 y;
} ocrypto_cp_p256;


// load r.x from bytes, recover r.y
// returns 0 if r is a legal curve point, -1 otherwise
int ocrypto_curve_p256_from32bytes(ocrypto_cp_p256 *r, const uint8_t p[32]);

// load point from bytes
// returns 0 if r is a legal curve point, -1 otherwise
int ocrypto_curve_p256_from64bytes(ocrypto_cp_p256 *r, const uint8_t p[64]);

// store p.x to bytes
void ocrypto_curve_p256_to32bytes(uint8_t r[32], ocrypto_cp_p256 *p);

// store point to bytes
void ocrypto_curve_p256_to64bytes(uint8_t r[64], ocrypto_cp_p256 *p);

// r = p * s
// r = [0,0] if p = [0,0] or s mod q = 0
// returns -1 if r = [0,0], 0 if 0 < s < q, 1 if s > q
int ocrypto_curve_p256_scalarmult(ocrypto_cp_p256 *r, const ocrypto_cp_p256 *p, const ocrypto_sc_p256 *s);

// r = basePoint * s
// r = [0,0] if s mod q = 0
// returns -1 if r = [0,0], 0 if 0 < s < q, 1 if s > q
int ocrypto_curve_p256_scalarmult_base(ocrypto_cp_p256 *r, const ocrypto_sc_p256 *s);

#ifdef __cplusplus
}
#endif

#endif
