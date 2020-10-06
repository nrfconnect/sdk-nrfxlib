/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef CHACHA20_ALT_H
#define CHACHA20_ALT_H

#include "mbedtls/config.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct mbedtls_chacha20_context
{
    uint8_t key[32];
    uint8_t nonce[12];
    uint32_t count;
    uint8_t cipher[64];
    size_t bytes;
}
mbedtls_chacha20_context;


#ifdef __cplusplus
}
#endif

#endif
