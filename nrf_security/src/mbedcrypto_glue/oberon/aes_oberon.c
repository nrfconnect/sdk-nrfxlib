/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(CONFIG_OBERON_GLUE_MBEDTLS_AES_C)

#include "mbedtls/aes.h"
#include "backend_aes.h"

static int mbedtls_aes_check(unsigned int keybits, int mode, int xts)
{
    return 2;
}

const mbedtls_aes_funcs mbedtls_aes_oberon_backend_funcs = {
    .backend_context_size = (4 * OBERON_MBEDTLS_AES_CONTEXT_WORDS),
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_XTS) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)
    .backend_xts_context_size = (4 * OBERON_MBEDTLS_AES_XTS_CONTEXT_WORDS),
#endif /* MBEDTLS_CIPHER_MODE_XTS */
    .check = mbedtls_aes_check,
    .init = mbedtls_aes_init,
    .free = mbedtls_aes_free,
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_XTS) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)
    .xts_init = NULL,
    .xts_free = NULL,
#endif
    .setkey_enc = mbedtls_aes_setkey_enc,
    .setkey_dec = mbedtls_aes_setkey_dec,
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_XTS) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)
    .xts_setkey_enc = NULL,
    .xts_setkey_dec = NULL,
#endif
    .internal_encrypt = mbedtls_internal_aes_encrypt,
    .internal_decrypt = mbedtls_internal_aes_decrypt,
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_CBC) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_CBC)
    .crypt_cbc = mbedtls_aes_crypt_cbc,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_XTS) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)
    .crypt_xts = NULL,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_CFB) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_CFB)
    .crypt_cfb128 = mbedtls_aes_crypt_cfb128,
    .crypt_cfb8 = mbedtls_aes_crypt_cfb8,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_OFB) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_OFB)
//    .crypt_ofb = mbedtls_aes_crypt_ofb,
    // Needs to be updated before it can be supported.
    .crypt_ofb = NULL,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CIPHER_MODE_CTR) && defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_CTR)
    .crypt_ctr = mbedtls_aes_crypt_ctr,
#endif
};

#endif /* defined(CONFIG_OBERON_MBEDTLS_AES_C) && defined(CONFIG_GLUE_MBEDTLS_AES_C) */
