/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(CONFIG_OBERON_MBEDTLS_ECDH_C) && defined(CONFIG_GLUE_MBEDTLS_ECDH_C)

#include "mbedtls/ecdh.h"
#include "backend_ecdh.h"


static int mbedtls_ecdh_check(mbedtls_ecp_group *grp, int function)
{
    switch (grp->id)
    {
        case MBEDTLS_ECP_DP_SECP256R1:
            return 2;
        case MBEDTLS_ECP_DP_SECP384R1:
        case MBEDTLS_ECP_DP_SECP521R1:
        case MBEDTLS_ECP_DP_SECP192K1:
        case MBEDTLS_ECP_DP_SECP224K1:
        case MBEDTLS_ECP_DP_SECP256K1:
        case MBEDTLS_ECP_DP_SECP192R1:
        case MBEDTLS_ECP_DP_SECP224R1:
        case MBEDTLS_ECP_DP_CURVE25519:
        case MBEDTLS_ECP_DP_BP256R1:
        case MBEDTLS_ECP_DP_BP384R1:
        case MBEDTLS_ECP_DP_BP512R1:
        case MBEDTLS_ECP_DP_CURVE448:
        default:
            return 0;
    }
}

const mbedtls_ecdh_funcs mbedtls_ecdh_oberon_backend_funcs = {
    .check = mbedtls_ecdh_check,
    .gen_public = mbedtls_ecdh_gen_public,
    .compute_shared = mbedtls_ecdh_compute_shared,
};

#endif
