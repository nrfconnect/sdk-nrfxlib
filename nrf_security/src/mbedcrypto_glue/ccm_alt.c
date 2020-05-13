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

#if defined(MBEDTLS_CCM_C) && defined(CONFIG_GLUE_MBEDTLS_CCM_C)

#include <string.h>
#include <stddef.h>

#include "mbedtls/ccm.h"
#include "backend_ccm.h"


#define CCM_CONTEXT_INIT(ctx) do { ctx->handle = NULL; } while (0)
#define CCM_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs) do { ctx->handle = (void*)new_funcs; funcs = new_funcs; backend_context = &ctx->buffer; } while (0)
#define CCM_CONTEXT_UNPACK(ctx, funcs, backend_context) do { funcs = ctx->handle; backend_context = &ctx->buffer; } while (0)
#define CCM_CONTEXT_FREE(ctx) do { ctx->handle = NULL; } while (0)

#define CCM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context) do { \
        CCM_CONTEXT_UNPACK(ctx, funcs, backend_context); \
        if (funcs == NULL) \
        { \
            return MBEDTLS_ERR_CCM_AUTH_FAILED; \
        } \
    } while (0)


#if defined(CONFIG_CC310_MBEDTLS_CCM_C)
extern mbedtls_ccm_funcs mbedtls_ccm_cc310_backend_funcs;
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CCM_C)
extern mbedtls_ccm_funcs mbedtls_ccm_oberon_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_CCM_C)
extern mbedtls_ccm_funcs mbedtls_ccm_vanilla_mbedtls_backend_funcs;
#endif


static mbedtls_ccm_funcs* ccm_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_CCM_C)
    &mbedtls_ccm_cc310_backend_funcs,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_CCM_C)
    &mbedtls_ccm_oberon_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_CCM_C)
    &mbedtls_ccm_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_ccm_funcs* find_backend(mbedtls_cipher_id_t cipher, unsigned int keybits)
{
    int max_priority = 0;
    const mbedtls_ccm_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(ccm_backends) / sizeof(ccm_backends[0]); i++)
    {
        priority = ccm_backends[i]->check(cipher, keybits);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = ccm_backends[i];
        }
    }
    return funcs;
}

void mbedtls_ccm_init(mbedtls_ccm_context *ctx)
{
    CCM_CONTEXT_INIT(ctx);
}

int mbedtls_ccm_setkey(mbedtls_ccm_context *ctx, mbedtls_cipher_id_t cipher, const unsigned char *key, unsigned int keybits)
{
    const mbedtls_ccm_funcs* new_funcs;
    const mbedtls_ccm_funcs* funcs;
    void* backend_context;
    CCM_CONTEXT_UNPACK(ctx, funcs, backend_context);

    new_funcs = find_backend(cipher, keybits);

    if (new_funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE;
    }

    if (funcs == new_funcs)
    {
        return funcs->setkey(backend_context, cipher, key, keybits);
    }

    if (funcs != NULL)
    {
        funcs->free(backend_context);
        CCM_CONTEXT_FREE(ctx);
    }

    CCM_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_CIPHER_ALLOC_FAILED;
    }

    funcs->init(backend_context);

    return funcs->setkey(backend_context, cipher, key, keybits);
}

void mbedtls_ccm_free(mbedtls_ccm_context *ctx)
{
    mbedtls_ccm_funcs* funcs;
    void* backend_context;
    CCM_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs != NULL)
    {
        funcs->free(backend_context);
        CCM_CONTEXT_FREE(ctx);
    }
    memset(ctx, 0, sizeof(mbedtls_ccm_context));
}

int mbedtls_ccm_encrypt_and_tag(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, unsigned char *tag, size_t tag_len)
{
    mbedtls_ccm_funcs* funcs;
    void* backend_context;
    CCM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);
    return funcs->encrypt_and_tag(backend_context, length, iv, iv_len, add, add_len, input, output, tag, tag_len);
}

int mbedtls_ccm_star_encrypt_and_tag(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, unsigned char *tag, size_t tag_len)
{
    mbedtls_ccm_funcs* funcs;
    void* backend_context;
    CCM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);
    return funcs->star_encrypt_and_tag(backend_context, length, iv, iv_len, add, add_len, input, output, tag, tag_len);
}

int mbedtls_ccm_auth_decrypt(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, const unsigned char *tag, size_t tag_len)
{
    mbedtls_ccm_funcs* funcs;
    void* backend_context;
    CCM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);
    return funcs->auth_decrypt(backend_context, length, iv, iv_len, add, add_len, input, output, tag, tag_len);
}

int mbedtls_ccm_star_auth_decrypt(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, const unsigned char *tag, size_t tag_len)
{
    mbedtls_ccm_funcs* funcs;
    void* backend_context;
    CCM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);
    return funcs->star_auth_decrypt(backend_context, length, iv, iv_len, add, add_len, input, output, tag, tag_len);
}

#endif /* CONFIG_GLUE_MBEDTLS_CCM_C */
