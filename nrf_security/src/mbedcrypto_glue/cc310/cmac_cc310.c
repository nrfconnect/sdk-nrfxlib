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

#if defined(MBEDTLS_BACKEND_CMAC_CC3XX) && defined(MBEDTLS_BACKEND_CMAC_GLUE)

#include "mbedtls/cmac.h"
#include "backend_cmac.h"


static int mbedtls_cipher_cmac_check(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keybits)
{
    return (keybits == 128) ? 2 : 0;
}

const mbedtls_cmac_funcs mbedtls_cmac_cc3xx_backend_funcs = {
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

