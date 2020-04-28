/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_mbed_tls_sha1 Oberon Mbed TLS SHA-1 type declarations
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations for an alternate implementation of SHA-1 for mbed TLS.
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

#define OCRYPTO_SHA1_CONTEXT_SIZE (96) //!< SHA-1 context size in bytes.

typedef struct mbedtls_sha1_context {
    unsigned char data[OCRYPTO_SHA1_CONTEXT_SIZE]; // Opaque SHA-1 context.
} mbedtls_sha1_context;


#ifdef __cplusplus
}
#endif

#endif /* #ifndef SHA1_ALT_H */

/** @} */
