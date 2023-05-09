/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_CHACHAPOLY_ALT_H
#define MBEDTLS_CHACHAPOLY_ALT_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#endif

#include "chacha20_alt.h"


#ifdef __cplusplus
extern "C"
{
#endif

/************************ Defines ******************************/
#define CHACHAPOLY_TAG_SIZE_BYTES       16

#if defined(MBEDTLS_CHACHAPOLY_ALT)

typedef struct
{
    mbedtls_chacha20_context chacha20_ctx;  /**< The ChaCha20 context. */
}mbedtls_chachapoly_context;

#endif

#ifdef __cplusplus
}
#endif


#endif /* MBEDTLS_CHACHAPOLY_ALT_H */
