/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @addtogroup mbedcrypto_glue_cmac
 * @{
 */
#ifndef MBEDTLS_CMAC_ALT_H
#define MBEDTLS_CMAC_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_CMAC_ALT)

#include <stdint.h>

/** @brief  mbedcrypto CMAC glue context.
 *
 * @note All CMAC structure allocation/deallocation is handled by specific backend, so
 * there is no need to put any content here.
 */
typedef struct mbedtls_cmac_context_t
{
    void* handle;                       //!< Pointer to the function table in an initialized glue context.
} mbedtls_cmac_context_t;

/** @brief Free a CMAC context after use.
 */
void mbedtls_cipher_cmac_free(mbedtls_cipher_context_t *ctx);

#endif /* MBEDTLS_CMAC_ALT */

#endif /* MBEDTLS_CMAC_ALT_H */

/** @} */
