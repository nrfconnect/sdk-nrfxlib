/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup nrf_oberon_mbed_tls_sha1 Oberon Mbed TLS SHA-1 type declarations
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations for an alternate implementation of SHA-1 for mbed TLS.
 */


#ifndef SHA1_ALT_H
#define SHA1_ALT_H

#include <stdint.h>

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "mbedtls/config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OBERON_SHA1_CONTEXT_SIZE_WORDS (24) //!< SHA-1 context size in words.

/* @brief Oberon replacement SHA-1 context */
typedef struct mbedtls_sha1_context {
    uint32_t data[OBERON_SHA1_CONTEXT_SIZE_WORDS]; //!< Opaque SHA-1 context.
} mbedtls_sha1_context;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SHA1_ALT_H */

/** @} */
