/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef OCC_CURVE_P256_H
#define OCC_CURVE_P256_H

#include "occ_sc_p256.h"


// (x,y) only jacobian coordinates
typedef struct {
    occ_mod_p256 x;
    occ_mod_p256 y;
} occ_cp_p256;


// load r.x from bytes, recover r.y
// returns 0 if r is a legal curve point, -1 otherwise
int occ_curve_p256_from32bytes(occ_cp_p256 *r, const uint8_t p[32]);

// load point from bytes
// returns 0 if r is a legal curve point, -1 otherwise
int occ_curve_p256_from64bytes(occ_cp_p256 *r, const uint8_t p[64]);

// store p.x to bytes
void occ_curve_p256_to32bytes(uint8_t r[32], occ_cp_p256 *p);

// store point to bytes
void occ_curve_p256_to64bytes(uint8_t r[64], occ_cp_p256 *p);

// r = p * s
// r = [0,0] if p = [0,0] or s mod q = 0
// returns -1 if r = [0,0], 0 if 0 < s < q, 1 if s > q
int occ_curve_p256_scalarmult(occ_cp_p256 *r, const occ_cp_p256 *p, const occ_sc_p256 *s);

// r = basePoint * s
// r = [0,0] if s mod q = 0
// returns -1 if r = [0,0], 0 if 0 < s < q, 1 if s > q
int occ_curve_p256_scalarmult_base(occ_cp_p256 *r, const occ_sc_p256 *s);

#endif
