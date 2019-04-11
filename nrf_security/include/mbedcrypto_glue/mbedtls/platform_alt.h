/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue
 * @{
 * @brief The mbedcrypto glue is a crypto toolbox glue layer coupling
 *        software and hardware-accelerated cryptography. The glue layer is implemented 
 *        using the mbed TLS ALT layer (also knonwn as mbed TLS backend) and 
 *        is usable through regluar mbed TLS front ends.
 *
 * @note    If the functionality doesn't benefit from hardware-accelerated implementation
 *          or isn't supported, the default behavior is to use vanilla mbed TLS.
 * @}
 * @defgroup mbedcrypto_glue_platform_alt mbedcrypto platform glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief This is a glue layer for mbedcrypto platform abstraction
 */
#ifndef PLATFORM_ALT_H
#define PLATFORM_ALT_H

#include <stdint.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#define MBEDTLS_CC3XX_PLATFORM_CONTEXT_WORDS    (6)     //!<  Context size of of type used to initialize mbedtls in words in the mbed_cc310_mbedcrypto library.

/**@brief Typedef of mbedcrypto platform glue context.
 *
 * @details This context is used when initializing/deinitializing mbed TLS using
 *          @c mbedtls_platform_setup and @c mbedtls_platform_teardown, which is
 *          required when using hardware-accelerated cryptograpy.
 */
typedef struct
{
#if defined(MBEDTLS_CC3XX_BACKEND)
    uint32_t cc3xx[MBEDTLS_CC3XX_PLATFORM_CONTEXT_WORDS];   //!< Array the size of mbed TLS init context in the nrf_cc310_mbedcrypto library.
#else /* MBEDTLS_CC3XX_BACKEND */
    uint8_t dummy;                                          //!< Dummy value in case nrc_cc310_mbedcrypto backend is not enabled.
#endif
} mbedtls_platform_context;

#endif

/** @} */
