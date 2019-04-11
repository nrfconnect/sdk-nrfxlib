/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @addtogroup mbedcrypto_glue_rsa
 * @{
 */
#ifndef MBEDTLS_RSA_ALT_H
#define MBEDTLS_RSA_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#define MBEDTLS_CC3XX_RSA_CONTEXT_WORDS         (52)    //!< Context size in words in nrf_cc310_mbedcrypto library in words.
#define MBEDTLS_DEFAULT_RSA_CONTEXT_WORDS       (43)    //!< Context size in words in vanilla mbed TLS in words.

#if defined(MBEDTLS_RSA_ALT)

#include <stdint.h>

/**@brief Typedef for the mbedtls RSA glue context.
 */
typedef struct mbedtls_rsa_context
{
#if defined(CONFIG_GLUE_MBEDTLS_RSA_C)
    void* handle;                                   //!< Pointer to the function table in an initialized glue context.
    int padding;                                    //!< The padding mode for the RSA operations.
    int hash_id;                                    //!< The hash digest algorithm used for RSA operations.
#endif /* CONFIG_GLUE_MBEDTLS_RSA_C */
    union
    {
#if defined(CONFIG_CC310_MBEDTLS_RSA_C)
        uint32_t buffer_cc3xx[MBEDTLS_CC3XX_RSA_CONTEXT_WORDS];     //!< Array the size of an RSA context in the nrf_cc310_mbedcrypto library.
#endif /* CONFIG_CC310_MBEDTLS_RSA_C */
#if defined(CONFIG_VANILLA_MBEDTLS_RSA_C)
        uint32_t buffer_default[MBEDTLS_DEFAULT_RSA_CONTEXT_WORDS]; //!< Array the size of an RSA context in vanilla mbed TLS.
#endif /* CONFIG_VANILLA_MBEDTLS_RSA_C */
        uint32_t dummy;                             //!< Dummy value in case no backend is enabled.
    } buffer;                                       //!< Union with size of the largest enabled backend context.
} mbedtls_rsa_context;

#endif /* MBEDTLS_RSA_ALT */

#endif /* MBEDTLS_RSA_ALT_H */

/** @} */
