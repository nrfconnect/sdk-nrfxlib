/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_mbed_tls_ecjpake Oberon Mbed ECJPAKE APIs
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations for an alternate implementation of EC-JPAKE for mbed TLS.
 */

#ifndef ECJPAKE_ALT_H
#define ECJPAKE_ALT_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "mbedtls/config.h"
#endif

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OBERON_ECJPAKE_P256_SECRET_KEY_SIZE (32) //!< ECJPAKE P-256 secret key size in bytes.
#define OBERON_ECJPAKE_P256_PUBLIC_KEY_SIZE (64) //!< ECJPAKE P-256 public key size in bytes.

/* @brief Oberon replacement ECJPAKE context */
typedef struct mbedtls_ecjpake_context {
    mbedtls_ecjpake_role role;                                  //!< Role, either client or server.
    unsigned char secret[OBERON_ECJPAKE_P256_SECRET_KEY_SIZE];  //!< Secret.
    unsigned char s_key2[OBERON_ECJPAKE_P256_SECRET_KEY_SIZE];  //!< Secret key 2.
    unsigned char p_key1[OBERON_ECJPAKE_P256_PUBLIC_KEY_SIZE];  //!< Public key 1.
    unsigned char p_key2[OBERON_ECJPAKE_P256_PUBLIC_KEY_SIZE];  //!< Public key 2.
    unsigned char r_key1[OBERON_ECJPAKE_P256_PUBLIC_KEY_SIZE];  //!< Remote key 1.
    unsigned char r_key2[OBERON_ECJPAKE_P256_PUBLIC_KEY_SIZE];  //!< Remote key 2.
    unsigned char r_key3[OBERON_ECJPAKE_P256_PUBLIC_KEY_SIZE];  //!< Remote key 3.
} mbedtls_ecjpake_context;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ECJPAKE_ALT_H */

/** @} */
