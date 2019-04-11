/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @addtogroup mbedcrypto_glue_dhm
 * @{
 */
#ifndef MBEDTLS_DHM_ALT_H
#define MBEDTLS_DHM_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_DHM_ALT)

#include <stdint.h>
#include "mbedtls/bignum.h"

/** @brief mbedcrypto DHM glue context.
 *
 * @note The context must be compatible with the original mbed TLS context,
 *       because other mbed TLS APIs are using internal fields of this context.
 *       Additional types in this context is added at the end.
 */
typedef struct mbedtls_dhm_context
{
    size_t len;             //!< Size of P in bytes.
    mbedtls_mpi P;          //!< The prime modulus.
    mbedtls_mpi G;          //!< The generator.
    mbedtls_mpi X;          //!< The local secret value.
    mbedtls_mpi GX;         //!< The X component of the remote public key.
    mbedtls_mpi GY;         //!< The Y component of the remote public key.
    mbedtls_mpi K;          //!< The shared secret.
    mbedtls_mpi RP;         //!< The cached value of R^2 mod P.
    mbedtls_mpi Vi;         //!< The blinding value.
    mbedtls_mpi Vf;         //!< The unblinding value.
    mbedtls_mpi pX;         //!< The previous X
#if defined(CONFIG_GLUE_MBEDTLS_DHM_C)
    void* handle;           //!< Pointer to the function table in an initialized glue context.
#endif /* CONFIG_GLUE_MBEDTLS_DHM_C */
} mbedtls_dhm_context;

#endif /* MBEDTLS_DHM_ALT */

#endif /* MBEDTLS_DHM_ALT_H */

/** @} */
