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

#if defined(MBEDTLS_CMAC_C) && defined(CONFIG_GLUE_MBEDTLS_CMAC_C)

#include <string.h>
#include <stddef.h>

#include "mbedtls/cmac.h"
#include "backend_cmac.h"


#if defined(CONFIG_CC310_MBEDTLS_CMAC_C)
extern mbedtls_cmac_funcs mbedtls_cmac_cc310_backend_funcs;
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CMAC_C)
extern mbedtls_cmac_funcs mbedtls_cmac_oberon_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_CMAC_C)
extern mbedtls_cmac_funcs mbedtls_cmac_vanilla_mbedtls_backend_funcs;
#endif

static mbedtls_cmac_funcs* cmac_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_CMAC_C)
    &mbedtls_cmac_cc310_backend_funcs,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CMAC_C)
    &mbedtls_cmac_oberon_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_CMAC_C)
    &mbedtls_cmac_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_cmac_funcs* find_backend(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keybits)
{
    int max_priority = 0;
    const mbedtls_cmac_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(cmac_backends) / sizeof(cmac_backends[0]); i++)
    {
        priority = cmac_backends[i]->check(cipher_info, key, keybits);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = cmac_backends[i];
        }
    }
    return funcs;
}

int mbedtls_cipher_cmac_starts(mbedtls_cipher_context_t *ctx , const unsigned char *key, size_t keybits)
{
    const mbedtls_cmac_funcs* funcs;
    mbedtls_cmac_context_t* cmac_ctx = (mbedtls_cmac_context_t*)ctx->cipher_ctx;

    funcs = find_backend(ctx->cipher_info, key, keybits);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
    }

    cmac_ctx->handle = (mbedtls_cmac_funcs*)funcs;

    return funcs->starts(ctx, key, keybits);;
}

int mbedtls_cipher_cmac_update(mbedtls_cipher_context_t *ctx , const unsigned char *input, size_t ilen)
{
    mbedtls_cmac_context_t* cmac_ctx = (mbedtls_cmac_context_t*)ctx->cipher_ctx;
    const mbedtls_cmac_funcs* funcs = (const mbedtls_cmac_funcs*)cmac_ctx->handle;

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_INVALID_CONTEXT;
    }

    return funcs->update(ctx, input, ilen);
}

int mbedtls_cipher_cmac_finish(mbedtls_cipher_context_t *ctx , unsigned char *output)
{
    mbedtls_cmac_context_t* cmac_ctx = (mbedtls_cmac_context_t*)ctx->cipher_ctx;
    const mbedtls_cmac_funcs* funcs = (const mbedtls_cmac_funcs*)cmac_ctx->handle;

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_INVALID_CONTEXT;
    }

    return funcs->finish(ctx, output);
}

int mbedtls_cipher_cmac_reset(mbedtls_cipher_context_t *ctx)
{
    mbedtls_cmac_context_t* cmac_ctx = (mbedtls_cmac_context_t*)ctx->cipher_ctx;
    const mbedtls_cmac_funcs* funcs = (const mbedtls_cmac_funcs*)cmac_ctx->handle;

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_INVALID_CONTEXT;
    }

    return funcs->reset(ctx);
}

void mbedtls_cipher_cmac_free(mbedtls_cipher_context_t *ctx)
{
    mbedtls_cmac_context_t* cmac_ctx = (mbedtls_cmac_context_t*)ctx->cipher_ctx;
    const mbedtls_cmac_funcs* funcs = (const mbedtls_cmac_funcs*)cmac_ctx->handle;

    if (funcs != NULL)
    {
        funcs->free(ctx);
    }
}

int mbedtls_cipher_cmac(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keylen , const unsigned char *input, size_t ilen , unsigned char *output)
{
    const mbedtls_cmac_funcs* funcs;

    funcs = find_backend(cipher_info, key, keylen);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
    }

    return funcs->cmac(cipher_info, key, keylen, input, ilen, output);
}

#if defined(MBEDTLS_AES_C)

int mbedtls_aes_cmac_prf_128(const unsigned char *key, size_t key_len , const unsigned char *input, size_t in_len , unsigned char output[16])
{
    const mbedtls_cmac_funcs* funcs;

    funcs = find_backend(NULL, key, key_len);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
    }

    return funcs->aes_prf_128(key, key_len, input, in_len , output);
}

#endif /* MBEDTLS_AES_C */

#endif /* CONFIG_GLUE_MBEDTLS_CMAC_C */
