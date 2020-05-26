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

#if defined(CONFIG_CC310_GLUE_MBEDTLS_CCM_C)

#include "mbedtls/ccm.h"
#include "backend_ccm.h"

static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits)
{
    return (keybits == 128) ? 3 : 0;
}

const mbedtls_ccm_funcs mbedtls_ccm_cc310_backend_funcs = {
    .backend_context_size = (4 * CC310_MBEDTLS_CCM_CONTEXT_WORDS),
    .check = mbedtls_ccm_check,
    .init = mbedtls_ccm_init,
    .setkey = mbedtls_ccm_setkey,
    .free = mbedtls_ccm_free,
    .encrypt_and_tag = mbedtls_ccm_encrypt_and_tag,
    .star_encrypt_and_tag = mbedtls_ccm_star_encrypt_and_tag,
    .auth_decrypt = mbedtls_ccm_auth_decrypt,
    .star_auth_decrypt = mbedtls_ccm_star_auth_decrypt,
};

#endif
