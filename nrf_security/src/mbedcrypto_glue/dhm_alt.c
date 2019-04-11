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

#if defined(MBEDTLS_DHM_C) && defined(CONFIG_GLUE_MBEDTLS_DHM_C)

#include <string.h>
#include <stddef.h>

#if defined(MBEDTLS_PEM_PARSE_C)
#include "mbedtls/pem.h"
#endif

#if defined(MBEDTLS_ASN1_PARSE_C)
#include "mbedtls/asn1.h"
#endif

#include "mbedtls/dhm.h"
#include "backend_dhm.h"


#define DHM_CONTEXT_INIT(ctx) do { ctx->handle = NULL; } while (0)
#define DHM_CONTEXT_ALLOC(ctx, funcs, new_funcs) do { ctx->handle = (void*)new_funcs; funcs = new_funcs; } while (0)
#define DHM_CONTEXT_UNPACK(ctx, funcs) do { funcs = ctx->handle; } while (0)
#define DHM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs) do { \
        funcs = ctx->handle; \
        if (funcs == NULL) \
        { \
            return MBEDTLS_ERR_DHM_BAD_INPUT_DATA; \
        } \
    } while (0)
#define DHM_CONTEXT_FREE(ctx) do { ctx->handle = NULL; } while (0)


#if defined(CONFIG_CC310_MBEDTLS_DHM_C)
extern mbedtls_dhm_funcs mbedtls_dhm_cc310_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_DHM_C)
extern mbedtls_dhm_funcs mbedtls_dhm_vanilla_mbedtls_backend_funcs;
#endif


static mbedtls_dhm_funcs* dhm_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_DHM_C)
    &mbedtls_dhm_cc310_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_DHM_C)
    &mbedtls_dhm_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_dhm_funcs* find_backend(unsigned int pbits)
{
    int max_priority = 0;
    const mbedtls_dhm_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(dhm_backends) / sizeof(dhm_backends[0]); i++)
    {
        priority = dhm_backends[i]->check(pbits);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = dhm_backends[i];
        }
    }
    return funcs;
}

int recheck_context(mbedtls_dhm_context *ctx, unsigned int pbits)
{
    const mbedtls_dhm_funcs* new_funcs;
    const mbedtls_dhm_funcs* funcs;
    DHM_CONTEXT_UNPACK(ctx, funcs);

    new_funcs = find_backend(pbits);

    if (new_funcs == NULL)
    {
        return MBEDTLS_ERR_DHM_READ_PARAMS_FAILED;
    }

    if (funcs == new_funcs)
    {
        return 0;
    }

    if (funcs != NULL)
    {
        funcs->free(ctx);
        DHM_CONTEXT_FREE(ctx);
    }

    DHM_CONTEXT_ALLOC(ctx, funcs, new_funcs);

    if (funcs == NULL)
    {
        return MBEDTLS_ERR_DHM_ALLOC_FAILED;
    }

    funcs->init(ctx);

    return 0;
}

static unsigned int read_pbits(unsigned char **p, const unsigned char *end)
{
    unsigned char *src = *p;
    int bits = 8 * ((end - src) - 2);
    unsigned char last_byte;

    if (bits <= 0)
    {
        return 0;
    }

    src += 2;

    while (bits > 8)
    {
        if (*src != 0) break;
        bits -= 8;
        src++;
    }

    last_byte = *src;

    while (last_byte != 0 && (0x80 & last_byte) == 0)
    {
        last_byte <<= 1;
        bits--;
    }
    
    return (unsigned int)bits;
}

void mbedtls_dhm_init(mbedtls_dhm_context *ctx)
{
    DHM_CONTEXT_INIT(ctx);
}

int mbedtls_dhm_read_params(mbedtls_dhm_context *ctx, unsigned char **p, const unsigned char *end)
{
    unsigned int pbits;
    int res;
    const mbedtls_dhm_funcs* funcs;

    pbits = read_pbits(p, end);
    if (pbits == 0)
    {
        return MBEDTLS_ERR_DHM_BAD_INPUT_DATA;
    }

    res = recheck_context(ctx, pbits);
    if (res != 0)
    {
        return res;
    }

    DHM_CONTEXT_UNPACK(ctx, funcs);

    return funcs->read_params(ctx, p, end);
}

int mbedtls_dhm_make_params(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_dhm_funcs* funcs;

    DHM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs);

    return funcs->make_params(ctx, x_size, output, olen, f_rng, p_rng);
}

int mbedtls_dhm_set_group(mbedtls_dhm_context *ctx, const mbedtls_mpi *P, const mbedtls_mpi *G)
{
    int res;
    const mbedtls_dhm_funcs* funcs;

    res = recheck_context(ctx, mbedtls_mpi_bitlen(P));
    if (res != 0)
    {
        return res;
    }

    DHM_CONTEXT_UNPACK(ctx, funcs);

    return funcs->set_group(ctx, P, G);
}

int mbedtls_dhm_read_public(mbedtls_dhm_context *ctx, const unsigned char *input, size_t ilen)
{
    const mbedtls_dhm_funcs* funcs;

    DHM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs);

    return funcs->read_public(ctx, input, ilen);
}

int mbedtls_dhm_make_public(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_dhm_funcs* funcs;

    DHM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs);

    return funcs->make_public(ctx, x_size, output, olen, f_rng, p_rng);
}

int mbedtls_dhm_calc_secret(mbedtls_dhm_context *ctx, unsigned char *output, size_t output_size, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_dhm_funcs* funcs;

    DHM_CONTEXT_UNPACK_NOT_NULL(ctx, funcs);

    return funcs->calc_secret(ctx, output, output_size, olen, f_rng, p_rng);
}

void mbedtls_dhm_free(mbedtls_dhm_context *ctx)
{
    mbedtls_dhm_funcs* funcs;
    DHM_CONTEXT_UNPACK(ctx, funcs);
    if (funcs != NULL)
    {
        funcs->free(ctx);
        DHM_CONTEXT_FREE(ctx);
    }
}

#if defined(MBEDTLS_ASN1_PARSE_C)

int mbedtls_dhm_parse_dhm(mbedtls_dhm_context *ctx, const unsigned char *dhmin, size_t dhminlen)
{
    int ret;
    size_t len;
    unsigned char *end;
    unsigned char *p;
    mbedtls_mpi dhm_p;
    unsigned int pbits;
    const mbedtls_dhm_funcs* funcs;

#if defined(MBEDTLS_PEM_PARSE_C)

    size_t pemlen;
    mbedtls_pem_context pem;

    mbedtls_pem_init(&pem);

    if( dhminlen == 0 || dhmin[dhminlen - 1] != 0 )
    {
        mbedtls_pem_free(&pem);
        return MBEDTLS_ERR_PEM_NO_HEADER_FOOTER_PRESENT;
    }

    pemlen = dhminlen;
    ret = mbedtls_pem_read_buffer(&pem, "-----BEGIN DH PARAMETERS-----", "-----END DH PARAMETERS-----", dhmin, NULL, 0, &pemlen);

    if (ret == 0)
    {
        dhminlen = pem.buflen;
        dhmin = pem.buf;
    }
    else if (ret != MBEDTLS_ERR_PEM_NO_HEADER_FOOTER_PRESENT)
    {
        mbedtls_pem_free(&pem);
        return ret;
    }

#endif /* MBEDTLS_PEM_PARSE_C */

    p = (unsigned char *)dhmin;
    end = p + dhminlen;

    ret = mbedtls_asn1_get_tag( &p, end, &len, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE);

    if (ret != 0 )
    {
#if defined(MBEDTLS_PEM_PARSE_C)
        mbedtls_pem_free(&pem);
#endif
        return MBEDTLS_ERR_DHM_INVALID_FORMAT;
    }

    end = p + len;

    mbedtls_mpi_init(&dhm_p);

    ret = mbedtls_asn1_get_mpi(&p, end, &dhm_p);

    pbits = mbedtls_mpi_bitlen(&dhm_p);

    mbedtls_mpi_free(&dhm_p);

    if (ret != 0)
    {
#if defined(MBEDTLS_PEM_PARSE_C)
        mbedtls_pem_free(&pem);
#endif
        return MBEDTLS_ERR_DHM_INVALID_FORMAT;
    }

    ret = recheck_context(ctx, pbits);
    if (ret != 0)
    {
#if defined(MBEDTLS_PEM_PARSE_C)
        mbedtls_pem_free(&pem);
#endif
        return ret;
    }

    DHM_CONTEXT_UNPACK(ctx, funcs);

    ret = funcs->parse_dhm(ctx, dhmin, dhminlen);

#if defined(MBEDTLS_PEM_PARSE_C)
    mbedtls_pem_free(&pem);
#endif

    return ret;
}

#if defined(MBEDTLS_FS_IO)

int mbedtls_dhm_parse_dhmfile(mbedtls_dhm_context *dhm, const char *path)
{
    /* File input is not supported by the backend glue layer */
    return MBEDTLS_ERR_DHM_INVALID_FORMAT;
}

#endif

#endif

#endif /* CONFIG_GLUE_MBEDTLS_DHM_C */
