/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef POLY1305_ALT_H
#define POLY1305_ALT_H

#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct mbedtls_poly1305_context
{
    uint32_t state[18];
} mbedtls_poly1305_context;


#ifdef __cplusplus
}
#endif

#endif
