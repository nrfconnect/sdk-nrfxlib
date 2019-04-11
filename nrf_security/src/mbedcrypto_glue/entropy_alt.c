

/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
#include "entropy_poll.h"
#include "entropy_alt.h"

#if defined(MBEDTLS_BACKEND_ENTROPY_GLUE)

#if defined(MBEDTLS_BACKEND_ENTROPY_CC310)
extern entropy_backend mbedtls_entropy_backend_cc310;
#endif
#if defined(MBEDTLS_BACKEND_ENTROPY_NRF)
extern entropy_backend mbedtls_entropy_backend_nrf;
#endif

static entropy_backend* entropy_backends[] = {
#if defined(MBEDTLS_BACKEND_ENTROPY_CC310)
    &mbedtls_entropy_backend_cc310,
#endif
#if defined(MBEDTLS_BACKEND_ENTROPY_NRF)
    &mbedtls_entropy_backend_nrf,
#endif
};


void mbedtls_entropy_add_backend_sources( mbedtls_entropy_context *ctx )
{
    int i;
    int ret = 0;
    int context_words = 0;
    int context_words_offset = 0;

    for (i = 0; i < sizeof(entropy_backends) / sizeof(entropy_backends[0]); i++)
    {
        context_words += entropy_backends[i]->context_words;
    }

    if (context_words > 0)
    {
        ctx->backend_context_buffer = mbedtls_calloc(4 * context_words);
        if (ctx->backend_context_buffer == NULL)
        {
            // TODO: show error log
            return;
        }
    }
    else
    {
        ctx->backend_context_buffer = NULL;
    }

    for (i = 0; i < sizeof(entropy_backends) / sizeof(entropy_backends[0]); i++)
    {
        ret = entropy_backends[i]->f_init(ctx, &ctx->backend_context_buffer[context_words_offset]);
        if (ret != 0)
        {
            // TODO: show error log
            continue;
        }
        ret = mbedtls_entropy_add_source( ctx, entropy_backends[i]->f_source,
                                    &ctx->backend_context_buffer[context_words_offset],
                                    entropy_backends[i]->threshold,
                                    entropy_backends[i]->strong );
        if (ret != 0)
        {
            // TODO: show error log
        }
        context_words_offset += entropy_backends[i]->context_words;
    }
}


void mbedtls_entropy_free_backend_sources( mbedtls_entropy_context *ctx )
{
    int i;
    int ret = 0;
    int context_words_offset = 0;

    for (i = 0; i < sizeof(entropy_backends) / sizeof(entropy_backends[0]); i++)
    {
        entropy_backends[i]->f_free(ctx, &ctx->backend_context_buffer[context_words_offset]);
        context_words_offset += entropy_backends[i]->context_words;
    }

    if (ctx->backend_context_buffer != NULL)
    {
        mbedtls_free(ctx->backend_context_buffer);
    }
}

#endif
