/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SHA1_ALT_H
#define SHA1_ALT_H

#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct mbedtls_sha1_context {
    uint32_t data[104]; // size of ocrypto_sha1_ctx
} mbedtls_sha1_context;


#ifdef __cplusplus
}
#endif

#endif
