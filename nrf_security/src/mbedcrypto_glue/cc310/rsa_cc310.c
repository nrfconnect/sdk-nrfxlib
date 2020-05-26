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

#if defined(CONFIG_CC310_MBEDTLS_RSA_C) && defined(CONFIG_GLUE_MBEDTLS_RSA_C)

#include "mbedtls/rsa.h"
#include "backend_rsa.h"


static int mbedtls_rsa_check(int padding, int hash_id, unsigned int nbits)
{
    if (nbits <= 2048)
    {
        return 3;
    }
    return 0;
}

const mbedtls_rsa_funcs mbedtls_rsa_cc310_backend_funcs = {
    .backend_context_size = (4 * CC310_MBEDTLS_RSA_CONTEXT_WORDS),
    .check = mbedtls_rsa_check,
    .init = mbedtls_rsa_init,
    .import = mbedtls_rsa_import,
    .import_raw = mbedtls_rsa_import_raw,
    .complete = mbedtls_rsa_complete,
    .export_fn = mbedtls_rsa_export,
    .export_raw = mbedtls_rsa_export_raw,
    .export_crt = mbedtls_rsa_export_crt,
    .set_padding = mbedtls_rsa_set_padding,
    .get_len = mbedtls_rsa_get_len,
#if defined(MBEDTLS_GENPRIME)
    .gen_key = mbedtls_rsa_gen_key,
#endif
    .check_pubkey = mbedtls_rsa_check_pubkey,
    .check_privkey = mbedtls_rsa_check_privkey,
    .check_pub_priv = mbedtls_rsa_check_pub_priv,
    .do_public = mbedtls_rsa_public,
    .do_private = mbedtls_rsa_private,
    .pkcs1_encrypt = mbedtls_rsa_pkcs1_encrypt,
#if defined(MBEDTLS_PKCS1_V15)
    .rsaes_pkcs1_v15_encrypt = mbedtls_rsa_rsaes_pkcs1_v15_encrypt,
#endif
#if defined(MBEDTLS_PKCS1_V21)
    .rsaes_oaep_encrypt = mbedtls_rsa_rsaes_oaep_encrypt,
#endif
    .pkcs1_decrypt = mbedtls_rsa_pkcs1_decrypt,
#if defined(MBEDTLS_PKCS1_V15)
    .rsaes_pkcs1_v15_decrypt = mbedtls_rsa_rsaes_pkcs1_v15_decrypt,
#endif
#if defined(MBEDTLS_PKCS1_V21)
    .rsaes_oaep_decrypt = mbedtls_rsa_rsaes_oaep_decrypt,
#endif
    .pkcs1_sign = mbedtls_rsa_pkcs1_sign,
#if defined(MBEDTLS_PKCS1_V15)
    .rsassa_pkcs1_v15_sign = mbedtls_rsa_rsassa_pkcs1_v15_sign,
#endif
#if defined(MBEDTLS_PKCS1_V21)
    .rsassa_pss_sign = mbedtls_rsa_rsassa_pss_sign,
#endif
    .pkcs1_verify = mbedtls_rsa_pkcs1_verify,
#if defined(MBEDTLS_PKCS1_V15)
    .rsassa_pkcs1_v15_verify = mbedtls_rsa_rsassa_pkcs1_v15_verify,
#endif
#if defined(MBEDTLS_PKCS1_V21)
    .rsassa_pss_verify = mbedtls_rsa_rsassa_pss_verify,
    .rsassa_pss_verify_ext = mbedtls_rsa_rsassa_pss_verify_ext,
#endif
    .copy = mbedtls_rsa_copy,
    .free = mbedtls_rsa_free,
};

#endif
