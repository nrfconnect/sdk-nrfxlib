/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_sha1_alt mbedcrypto SHA-1 glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Type declarations for the mbedcrypto SHA-1 glue layer.
 */
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(CONFIG_GLUE_MBEDTLS_AES_C)

#include <string.h>
#include <stddef.h>

#include "mbedtls/aes.h"
#include "backend_aes.h"


#define AES_CONTEXT_INIT(ctx) do { ctx->handle = NULL; } while (0)
#define AES_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs) do { ctx->handle = (void*)new_funcs; funcs = new_funcs; backend_context = &ctx->buffer; } while (0)
#define AES_CONTEXT_UNPACK(ctx, funcs, backend_context) do { funcs = ctx->handle; backend_context = &ctx->buffer; } while (0)
#define AES_CONTEXT_FREE(ctx) do { ctx->handle = NULL; } while (0)

#define AES_XTS_CONTEXT_INIT(ctx) do { ctx->handle = NULL; } while (0)
#define AES_XTS_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs) do { ctx->handle = (void*)new_funcs; funcs = new_funcs; backend_context = &ctx->buffer; } while (0)
#define AES_XTS_CONTEXT_UNPACK(ctx, funcs, backend_context) do { funcs = ctx->handle; backend_context = &ctx->buffer; } while (0)
#define AES_XTS_CONTEXT_FREE(ctx) do { ctx->handle = NULL; } while (0)


#if defined(CONFIG_CC310_MBEDTLS_AES_C)
extern mbedtls_aes_funcs mbedtls_aes_cc310_backend_funcs;
#endif
#if defined(CONFIG_OBERON_MBEDTLS_AES_C)
extern mbedtls_aes_funcs mbedtls_aes_oberon_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_AES_C)
extern mbedtls_aes_funcs mbedtls_aes_vanilla_mbedtls_backend_funcs;
#endif


static mbedtls_aes_funcs* aes_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_AES_C)
    &mbedtls_aes_cc310_backend_funcs,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_AES_C)
    &mbedtls_aes_oberon_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_AES_C)
    &mbedtls_aes_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_aes_funcs* find_backend(unsigned int keybits, int mode, int xts)
{
    int max_priority = 0;
    const mbedtls_aes_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(aes_backends) / sizeof(aes_backends[0]); i++)
    {
        priority = aes_backends[i]->check(keybits, mode, xts);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = aes_backends[i];
        }
    }
    return funcs;
}

void mbedtls_aes_init(mbedtls_aes_context *ctx)
{
    AES_CONTEXT_INIT(ctx);
}

void mbedtls_aes_free(mbedtls_aes_context *ctx)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs != NULL)
    {
        funcs->free(backend_context);
        AES_CONTEXT_FREE(ctx);
    }
}

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)

void mbedtls_aes_xts_init(mbedtls_aes_xts_context *ctx)
{
    AES_XTS_CONTEXT_INIT(ctx);
}

void mbedtls_aes_xts_free(mbedtls_aes_xts_context *ctx)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_XTS_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs != NULL)
    {
        funcs->free(backend_context);
        AES_XTS_CONTEXT_FREE(ctx);
    }
}

#endif

static int mbedtls_aes_setkey( mbedtls_aes_context *ctx, const unsigned char *key,
                    unsigned int keybits, int mode )
{
    const mbedtls_aes_funcs* new_funcs;
    const mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);

    new_funcs = find_backend(keybits, mode, 0);

    if (new_funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }

    if (funcs == new_funcs)
    {
        if (mode == MBEDTLS_AES_ENCRYPT)
        {
            return funcs->setkey_enc(backend_context, key, keybits);
        }
        else
        {
            return funcs->setkey_dec(backend_context, key, keybits);
        }
    }

    if (funcs != NULL)
    {
        funcs->free(backend_context);
        AES_CONTEXT_FREE(ctx);
    }

    AES_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
    }

    funcs->init(backend_context);

    if (mode == MBEDTLS_AES_ENCRYPT)
    {
        return funcs->setkey_enc(backend_context, key, keybits);
    }
    else
    {
        return funcs->setkey_dec(backend_context, key, keybits);
    }
}

int mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    return mbedtls_aes_setkey(ctx, key, keybits, MBEDTLS_AES_ENCRYPT);
}

int mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
    return mbedtls_aes_setkey(ctx, key, keybits, MBEDTLS_AES_DECRYPT);
}

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)

static int mbedtls_aes_xts_setkey_internal(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits, int mode)
{
    // TODO: Check if reallocating is safe (e.g. some state have to transferred betweeen new and old context)
    const mbedtls_aes_funcs* new_funcs;
    const mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_XTS_CONTEXT_UNPACK(ctx, funcs, backend_context);

    new_funcs = find_backend(keybits, mode, 1);

    if (new_funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }

    if (funcs == new_funcs)
    {
        if (mode == MBEDTLS_AES_ENCRYPT)
        {
            return funcs->xts_setkey_enc(backend_context, key, keybits);
        }
        else
        {
            return funcs->xts_setkey_dec(backend_context, key, keybits);
        }
    }

    if (funcs != NULL)
    {
        funcs->xts_free(backend_context);
        AES_XTS_CONTEXT_FREE(ctx);
    }

    AES_XTS_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_HW_ACCEL_FAILED;
    }

    funcs->xts_init(backend_context);

    if (mode == MBEDTLS_AES_ENCRYPT)
    {
        return funcs->xts_setkey_enc(backend_context, key, keybits);
    }
    else
    {
        return funcs->xts_setkey_dec(backend_context, key, keybits);
    }
}

int mbedtls_aes_xts_setkey_enc(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits)
{
    return mbedtls_aes_xts_setkey_internal(ctx, key, keybits, MBEDTLS_AES_ENCRYPT);
}

int mbedtls_aes_xts_setkey_dec(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits)
{
    return mbedtls_aes_xts_setkey_internal(ctx, key, keybits, MBEDTLS_AES_DECRYPT);
}

#endif

int mbedtls_internal_aes_encrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->internal_encrypt(backend_context, input, output);
}

int mbedtls_internal_aes_decrypt(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16])
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->internal_decrypt(backend_context, input, output);
}

#if !defined(CONFIG_GLUE_MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_aes_encrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{
    mbedtls_internal_aes_encrypt( ctx, input, output );
}
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

#if !defined(CONFIG_GLUE_MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_aes_decrypt( mbedtls_aes_context *ctx,
                          const unsigned char input[16],
                          unsigned char output[16] )
{
    mbedtls_internal_aes_decrypt( ctx, input, output );
}
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                    int mode,
                    const unsigned char input[16],
                    unsigned char output[16] )
{
    if( mode == MBEDTLS_AES_ENCRYPT )
        return( mbedtls_internal_aes_encrypt( ctx, input, output ) );
    else
        return( mbedtls_internal_aes_decrypt( ctx, input, output ) );
}

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output )
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context); // TODO: replate UNPACK by static inline
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->crypt_cbc(backend_context, mode, length, iv, input, output);
}

#endif

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_XTS)

int mbedtls_aes_crypt_xts(mbedtls_aes_xts_context *ctx, int mode, size_t length, const unsigned char data_unit[16], const unsigned char *input, unsigned char *output)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_XTS_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->crypt_xts(backend_context, mode, length, data_unit, input, output);
}

#endif

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_CFB)

int mbedtls_aes_crypt_cfb128(mbedtls_aes_context *ctx, int mode, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->crypt_cfb128(backend_context, mode, length, iv_off, iv, input, output);
}

int mbedtls_aes_crypt_cfb8(mbedtls_aes_context *ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->crypt_cfb8(backend_context, mode, length, iv, input, output);
}

#endif

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_OFB)

int mbedtls_aes_crypt_ofb(mbedtls_aes_context *ctx, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->crypt_ofb(backend_context, length, iv_off, iv, input, output);
}

#endif

#if defined(CONFIG_GLUE_MBEDTLS_CIPHER_MODE_CTR)

int mbedtls_aes_crypt_ctr(mbedtls_aes_context *ctx, size_t length, size_t *nc_off, unsigned char nonce_counter[16], unsigned char stream_block[16], const unsigned char *input, unsigned char *output)
{
    mbedtls_aes_funcs* funcs;
    void* backend_context;
    AES_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_AES_FEATURE_UNAVAILABLE;
    }
    return funcs->crypt_ctr(backend_context, length, nc_off, nonce_counter, stream_block, input, output);
}

#endif

#endif /* CONFIG_GLUE_MBEDTLS_AES_C */
