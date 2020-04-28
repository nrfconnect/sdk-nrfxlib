/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
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

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "mbedtls/config.h"
#endif

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"


typedef struct mbedtls_ecjpake_context {
    mbedtls_ecjpake_role role; // Are we client or server?

    unsigned char secret[32];
    unsigned char s_key2[32];  // secret key 2
    unsigned char p_key1[64];  // public key 1
    unsigned char p_key2[64];  // public key 2
    unsigned char r_key1[64];  // remote key 1
    unsigned char r_key2[64];  // remote key 2
    unsigned char r_key3[64];  // remote key 3
} mbedtls_ecjpake_context;


#ifdef __cplusplus
}
#endif

#endif

/** @} */