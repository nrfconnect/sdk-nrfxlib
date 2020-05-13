/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @addtogroup mbedcrypto_glue_aes_ccm
 * @{
 */
#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/cipher.h"

/**
 * @brief Context size of AES CCM in words in the mbed_cc310_mbedcrypto library.
 */
#define CC310_MBEDTLS_CCM_CONTEXT_WORDS     (96)
#define OBERON_MBEDTLS_CCM_CONTEXT_WORDS    ((sizeof(mbedtls_cipher_context_t) + 3) / 4)
#define VANILLA_MBEDTLS_CCM_CONTEXT_WORDS   ((sizeof(mbedtls_cipher_context_t) + 3) / 4)

/**
 * @brief Context size of AES CCM in words in standard mbed TLS.
 */

#if defined(MBEDTLS_CCM_ALT)

#include <stdint.h>

/**
 * @brief mbedcrypto AES CCM glue context.
 */
typedef struct mbedtls_ccm_context
{
#if defined(CONFIG_GLUE_MBEDTLS_CCM_C)
#endif
    union _buffer
    {
#if defined(CONFIG_CC310_MBEDTLS_CCM_C)
        uint32_t buffer_cc310[CC310_MBEDTLS_CCM_CONTEXT_WORDS];               //!< Array the size of an AES CCM context in the nrf_cc310_mbedcrypto library.

#endif /* CONFIG_CC310_MBEDTLS_CCM_C */
#if defined(CONFIG_OBERON_MBEDTLS_CCM_C)
        uint32_t buffer_oberon_mbedtls[OBERON_MBEDTLS_CCM_CONTEXT_WORDS];   //!< Array the size of an AES CCM context in the nrf_oberon library.
#endif /* CONFIG_OBERON_MBEDTLS_CCM_C */
#if defined(CONFIG_VANILLA_MBEDTLS_CCM_C)
        uint32_t buffer_vanilla_mbedtls[VANILLA_MBEDTLS_CCM_CONTEXT_WORDS];   //!< Array the size of an AES CCM context in standard mbed TLS.
#endif /* CONFIG_VANILLA_MBEDTLS_CCM_C */
        uint32_t dummy;                                                       //!< Dummy value in case no backend is enabled.
    } buffer;                                                                 //!< Union with size of the largest enabled backend context.
    void* handle;                                                             //!< Pointer to the function table in an initialized glue context.
} mbedtls_ccm_context;

#endif /* MBEDTLS_CCM_ALT */

#endif /* MBEDTLS_CCM_ALT_H */

/** @} */
