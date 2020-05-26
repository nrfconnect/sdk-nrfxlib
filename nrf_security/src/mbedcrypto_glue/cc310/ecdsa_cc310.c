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

#if defined(CONFIG_CC310_MBEDTLS_ECDSA_C) && defined(CONFIG_GLUE_MBEDTLS_ECDSA_C)

#include "mbedtls/ecdsa.h"
#include "backend_ecdsa.h"


static int mbedtls_ecdsa_check(mbedtls_ecp_group *grp, mbedtls_ecp_group_id gid, int function)
{
    if (grp != NULL)
    {
        gid = grp->id;
    }
    switch (gid)
    {
        case MBEDTLS_ECP_DP_SECP192R1:
        case MBEDTLS_ECP_DP_SECP224R1:
        case MBEDTLS_ECP_DP_SECP256R1:
        case MBEDTLS_ECP_DP_SECP384R1:
        case MBEDTLS_ECP_DP_SECP521R1:
        case MBEDTLS_ECP_DP_CURVE25519:
        case MBEDTLS_ECP_DP_SECP192K1:
        case MBEDTLS_ECP_DP_SECP224K1:
        case MBEDTLS_ECP_DP_SECP256K1:
            return 3;
        case MBEDTLS_ECP_DP_BP256R1:
        case MBEDTLS_ECP_DP_BP384R1:
        case MBEDTLS_ECP_DP_BP512R1:
        case MBEDTLS_ECP_DP_CURVE448:
        default:
            return 0;
    }
}

const mbedtls_ecdsa_funcs mbedtls_ecdsa_cc310_backend_funcs = {
    .check = mbedtls_ecdsa_check,
    .sign = mbedtls_ecdsa_sign,
    .verify = mbedtls_ecdsa_verify,
    .genkey = mbedtls_ecdsa_genkey,
};

#endif
