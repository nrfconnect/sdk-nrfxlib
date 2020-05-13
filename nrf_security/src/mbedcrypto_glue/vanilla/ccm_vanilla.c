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

#if defined(CONFIG_VANILLA_GLUE_MBEDTLS_CCM_C)

#include <toolchain.h>

#include "mbedtls/ccm.h"
#include "mbedtls/cipher.h"
#include "backend_ccm.h"


BUILD_ASSERT(VANILLA_MBEDTLS_CCM_CONTEXT_WORDS == (sizeof(mbedtls_cipher_context_t) + 3) / 4, "Invalid VANILLA_MBEDTLS_CCM_CONTEXT_WORDS value");


static int mbedtls_ccm_check(mbedtls_cipher_id_t cipher, unsigned int keybits)
{
    return 1;
}

const mbedtls_ccm_funcs mbedtls_ccm_vanilla_mbedtls_backend_funcs = {
    .backend_context_size = (4 * VANILLA_MBEDTLS_CCM_CONTEXT_WORDS),
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
