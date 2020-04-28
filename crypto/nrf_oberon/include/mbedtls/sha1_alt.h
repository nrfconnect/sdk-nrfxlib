/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_mbed_tls_sha1 Oberon Mbed TLS SHA-1 type declarations
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations for an alternate implementation of SHA-256 for mbed TLS.
 */

#ifndef SHA1_ALT_H
#define SHA1_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "mbedtls/config.h"
#endif


typedef struct mbedtls_sha1_context {
    unsigned char data[96]; // size of ocrypto_sha1_ctx
} mbedtls_sha1_context;


#ifdef __cplusplus
}
#endif

#endif

/** @} */
