/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_types ocrypto internal types
 * @ingroup ocrypto
 * @{
 * @brief Declarations of internal types used in public interfaces. Their fields and sizes are subject to change.
 */

#ifndef OCRYPTO_TYPES_H
#define OCRYPTO_TYPES_H

#include <stddef.h>
#include <stdint.h>

#include "ocrypto_sha512.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
/**@cond */
  uint32_t w[8];
/**@endcond */
} ocrypto_mod25519;

typedef struct {
/**@cond */
    uint32_t w[8];
/**@endcond */
} ocrypto_sc25519;

typedef struct {
/**@cond */
    ocrypto_mod25519 x;
    ocrypto_mod25519 y;
    ocrypto_mod25519 z;
/**@endcond */
} ocrypto_ge25519;

typedef struct {
/**@cond */
    ocrypto_mod25519 x;
    ocrypto_mod25519 y;
    ocrypto_mod25519 z;
    ocrypto_mod25519 t;
/**@endcond */
} ocrypto_ge25519_ext;

typedef struct {
/**@cond */
    ocrypto_mod25519 e;
    ocrypto_mod25519 f;
    ocrypto_mod25519 g;
    ocrypto_mod25519 h;
/**@endcond */
} ocrypto_ge25519_comp;


typedef struct {
/**@cond */
    ocrypto_ge25519_comp comp;
    ocrypto_ge25519_ext ext;
    ocrypto_ge25519_ext pre[3];
/**@endcond */
} ocrypto_ge25519_ctx;

typedef struct {
/**@cond */
    union {
        ocrypto_sha512_ctx sha;
        ocrypto_ge25519_ctx ge;
    } c;
    ocrypto_sc25519 r, k, s;
    ocrypto_ge25519 R;
    uint8_t ext[64];
/**@endcond */
} ocrypto_ed25519_ctx;


typedef struct {
/**@cond */
    ocrypto_mod25519 x0, xn, zn, xm, zm, a, c;
/**@endcond */
} ocrypto_curve25519_ctx;


typedef struct {
/**@cond */
  uint32_t w[8];
/**@endcond */
} ocrypto_mod_p256;

typedef struct {
/**@cond */
    uint32_t w[8];
/**@endcond */
} ocrypto_sc_p256;

typedef struct {
/**@cond */
    ocrypto_mod_p256 x, x3, xn, t;
    int step;
/**@endcond */
} ocrypto_p256_inv_ctx;

typedef struct {
/**@cond */
    ocrypto_mod_p256 x;
    ocrypto_mod_p256 y;
/**@endcond */
} ocrypto_cp_p256;

typedef struct {
/**@cond */
    ocrypto_cp_p256 p, q0, q1;
    uint32_t e[8];
    ocrypto_p256_inv_ctx inv;
    int ret, prev, dec, step;
/**@endcond */
} ocrypto_p256_mult_ctx;


typedef struct {
/**@cond */
    uint32_t w[96];
/**@endcond */
} ocrypto_srp_mg;

typedef struct {
/**@cond */
    uint32_t mem[96];
    uint32_t m[192], x[96];
    ocrypto_srp_mg a2, a3, ax;
/**@endcond */
} ocrypto_srp_math_ctx;

typedef struct {
/**@cond */
    union {
        ocrypto_sha512_ctx sha;
        ocrypto_srp_math_ctx mul;
    } c;
    ocrypto_srp_mg x, y;
/**@endcond */
} ocrypto_srp_ctx;


#ifdef __cplusplus
}
#endif

#endif

/** @} */
