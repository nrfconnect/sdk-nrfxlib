/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_mbed_tls_sha256 Oberon Mbed TLS SHA-256 type declarations
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations for an alternate implementation of SHA-256 for Mbed TLS.
 */

#ifndef SHA256_ALT_H
#define SHA256_ALT_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "mbedtls/config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OCRYPTO_SHA256_CONTEXT_SIZE (108) //!< SHA-256 context size in bytes.

/* @brief Oberon replacement SHA-256 context */
typedef struct mbedtls_sha256_context {
    unsigned char data[OCRYPTO_SHA256_CONTEXT_SIZE]; //!< Opaque SHA-256 context.
} mbedtls_sha256_context;


#ifdef __cplusplus
}
#endif

#endif /* #ifndef SHA256_ALT_H */

/** @} */
