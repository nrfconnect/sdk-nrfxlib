/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef POLY1305_ALT_H
#define POLY1305_ALT_H

#include "mbedtls/config.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct mbedtls_poly1305_context
{
    uint32_t state[5];
    uint8_t key[32];
    uint8_t data[16];
    size_t bytes;
} mbedtls_poly1305_context;


#ifdef __cplusplus
}
#endif

#endif
