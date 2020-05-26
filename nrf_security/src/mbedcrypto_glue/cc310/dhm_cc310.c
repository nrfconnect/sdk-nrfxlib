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

#if defined(CONFIG_CC310_GLUE_MBEDTLS_DHM_C)

#include "mbedtls/dhm.h"
#include "backend_dhm.h"


static int mbedtls_dhm_check(unsigned int pbits)
{
    if (pbits <= 2048)
    {
        return 3;
    }
    return 0;
}

const mbedtls_dhm_funcs mbedtls_dhm_cc310_backend_funcs = {
    .check = mbedtls_dhm_check,
    .init = mbedtls_dhm_init,
    .read_params = mbedtls_dhm_read_params,
    .make_params = mbedtls_dhm_make_params,
    .set_group = mbedtls_dhm_set_group,
    .read_public = mbedtls_dhm_read_public,
    .make_public = mbedtls_dhm_make_public,
    .calc_secret = mbedtls_dhm_calc_secret,
    .free = mbedtls_dhm_free,
#if defined(MBEDTLS_ASN1_PARSE_C)
    .parse_dhm = mbedtls_dhm_parse_dhm,
#if defined(MBEDTLS_FS_IO)
    .parse_dhmfile = mbedtls_dhm_parse_dhmfile,
#endif
#endif
};

#endif
