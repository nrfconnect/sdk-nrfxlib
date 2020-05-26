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

#if defined(CONFIG_CC310_GLUE_MBEDTLS_CMAC_C)

#include "mbedtls/cmac.h"
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "backend_cmac.h"

static int mbedtls_cipher_cmac_check(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keybits)
{
    return (keybits == 128) ? 3 : 0;
}

void mbedtls_cipher_cmac_free(mbedtls_cipher_context_t *ctx)
{
    if( ctx->cmac_ctx )
    {
       mbedtls_platform_zeroize( ctx->cmac_ctx,
                                 sizeof( mbedtls_cmac_context_t ) );
       mbedtls_free( ctx->cmac_ctx );
    }
}

const mbedtls_cmac_funcs mbedtls_cmac_cc310_backend_funcs = {
    .check = mbedtls_cipher_cmac_check,
    .starts = mbedtls_cipher_cmac_starts,
    .update = mbedtls_cipher_cmac_update,
    .finish = mbedtls_cipher_cmac_finish,
    .reset = mbedtls_cipher_cmac_reset,
    .free = mbedtls_cipher_cmac_free,
    .cmac = mbedtls_cipher_cmac,
#if defined(MBEDTLS_AES_C)
    .aes_prf_128 = mbedtls_aes_cmac_prf_128,
#endif /* MBEDTLS_AES_C */
};

#endif
