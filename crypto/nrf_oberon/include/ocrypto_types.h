/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
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
