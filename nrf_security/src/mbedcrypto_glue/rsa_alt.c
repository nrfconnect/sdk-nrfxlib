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

#if defined(CONFIG_GLUE_MBEDTLS_RSA_C)

#include <string.h>
#include <stddef.h>

#include "mbedtls/rsa.h"
#include "backend_rsa.h"

#define RSA_CONTEXT_INIT(ctx) do { ctx->handle = NULL; } while (0)
#define RSA_CONTEXT_ALLOC(ctx, funcs, backend_context, new_funcs) do { ctx->handle = (void*)new_funcs; funcs = new_funcs; backend_context = &ctx->buffer; } while (0)
#define RSA_CONTEXT_UNPACK(ctx, funcs, backend_context) do { funcs = ctx->handle; backend_context = &ctx->buffer; } while (0)
#define RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context) do { \
        funcs = ctx->handle; \
        backend_context = &ctx->buffer; \
        if (funcs == NULL) \
        { \
            return MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION; \
        } \
    } while (0)
#define RSA_CONTEXT_FREE(ctx) do { ctx->handle = NULL; } while (0)


#if defined(CONFIG_CC310_MBEDTLS_RSA_C)
extern mbedtls_rsa_funcs mbedtls_rsa_cc310_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_RSA_C)
extern mbedtls_rsa_funcs mbedtls_rsa_vanilla_mbedtls_backend_funcs;
#endif


static mbedtls_rsa_funcs* rsa_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_RSA_C)
    &mbedtls_rsa_cc310_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_RSA_C)
    &mbedtls_rsa_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_rsa_funcs* find_backend(int padding, int hash_id, unsigned int nbits)
{
    int max_priority = 0;
    const mbedtls_rsa_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(rsa_backends) / sizeof(rsa_backends[0]); i++)
    {
        priority = rsa_backends[i]->check(padding, hash_id, nbits);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = rsa_backends[i];
        }
    }
    return funcs;
}

void mbedtls_rsa_init(mbedtls_rsa_context *ctx, int padding, int hash_id)
{
    RSA_CONTEXT_INIT(ctx);
    ctx->padding = padding;
    ctx->hash_id = hash_id;
}

int mbedtls_rsa_import(mbedtls_rsa_context *ctx, const mbedtls_mpi *N, const mbedtls_mpi *P, const mbedtls_mpi *Q, const mbedtls_mpi *D, const mbedtls_mpi *E)
{
    int res;
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    res = recheck_context(ctx, mbedtls_mpi_bitlen(N)); // TODO: N can be NULL then use P and Q to calculate N bits
    if (res != 0)
    {
        return res;
    }

    RSA_CONTEXT_UNPACK(ctx, funcs, backend_context);

    return funcs->import(backend_context, N, P, Q, D, E);
}

int mbedtls_rsa_import_raw(mbedtls_rsa_context *ctx, unsigned char const *N, size_t N_len, unsigned char const *P, size_t P_len, unsigned char const *Q, size_t Q_len, unsigned char const *D, size_t D_len, unsigned char const *E, size_t E_len)
{
    int len;
    int res;
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;
    mbedtls_mpi mpi;

    mbedtls_mpi_init(&mpi);
    mbedtls_mpi_read_binary(&mpi, N, N_len); // TODO: N can be NULL then use P and Q to calculate N bits
    len = mbedtls_mpi_bitlen(N);
    mbedtls_mpi_free(&mpi);

    res = recheck_context(ctx, len);
    if (res != 0)
    {
        return res;
    }

    RSA_CONTEXT_UNPACK(ctx, funcs, backend_context);

    return funcs->import_raw(backend_context, N, N_len, P, P_len, Q, Q_len, D, D_len, E, E_len);
}

int mbedtls_rsa_complete(mbedtls_rsa_context *ctx)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->complete(backend_context);
}

int mbedtls_rsa_export(const mbedtls_rsa_context *ctx, mbedtls_mpi *N, mbedtls_mpi *P, mbedtls_mpi *Q, mbedtls_mpi *D, mbedtls_mpi *E)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->export_fn(backend_context, N, P, Q, D, E);
}

int mbedtls_rsa_export_raw(const mbedtls_rsa_context *ctx, unsigned char *N, size_t N_len, unsigned char *P, size_t P_len, unsigned char *Q, size_t Q_len, unsigned char *D, size_t D_len, unsigned char *E, size_t E_len)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->export(backend_context, N, N_len, P, P_len, Q, Q_len, D, D_len, E, E_len);
}

int mbedtls_rsa_export_crt(const mbedtls_rsa_context *ctx, mbedtls_mpi *DP, mbedtls_mpi *DQ, mbedtls_mpi *QP)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->export(backend_context, DP, DQ, QP);
}

void mbedtls_rsa_set_padding(mbedtls_rsa_context *ctx, int padding, int hash_id)
{
    int res;
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    ctx->padding = padding;
    ctx->hash_id = hash_id;

    res = recheck_context(ctx, 0);
    if (res != 0)
    {
        return res;
    }

    // Changing backends using the same context is currently not supported
    RSA_CONTEXT_UNPACK(ctx, funcs, backend_context);

    return funcs->set_padding(backend_context, padding, hash_id);
}

size_t mbedtls_rsa_get_len(const mbedtls_rsa_context *ctx)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->get_len(backend_context);
}

#if defined(MBEDTLS_GENPRIME)

int mbedtls_rsa_gen_key(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, unsigned int nbits, int exponent)
{
    int res;
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    res = recheck_context(ctx, nbits);
    if (res != 0)
    {
        return res;
    }

    RSA_CONTEXT_UNPACK(ctx, funcs, backend_context);

    return funcs->gen_key(backend_context, f_rng, p_rng, nbits, exponent);
}

#endif

int mbedtls_rsa_check_pubkey(const mbedtls_rsa_context *ctx)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->check_pubkey(backend_context);
}

int mbedtls_rsa_check_privkey(const mbedtls_rsa_context *ctx)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->check_privkey(backend_context);
}

int mbedtls_rsa_check_pub_priv(const mbedtls_rsa_context *pub, const mbedtls_rsa_context *prv)
{
    const mbedtls_rsa_funcs* funcs_pub;
    void* backend_context_pub;
    const mbedtls_rsa_funcs* funcs_prv;
    void* backend_context_prv;

    RSA_CONTEXT_UNPACK_NOT_NULL(pub, funcs_pub, backend_context_pub);
    RSA_CONTEXT_UNPACK_NOT_NULL(prv, funcs_prv, backend_context_prv);

    if (funcs_pub != funcs_prv)
    {
        return MBEDTLS_ERR_RSA_KEY_CHECK_FAILED;
    }

    return funcs_pub->check_pub_priv(backend_context_pub, backend_context_prv);
}

int mbedtls_rsa_public(mbedtls_rsa_context *ctx, const unsigned char *input, unsigned char *output)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->public(backend_context, input, output);
}

int mbedtls_rsa_private(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, const unsigned char *input, unsigned char *output)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->private(backend_context, f_rng, p_rng, input, output);
}

int mbedtls_rsa_pkcs1_encrypt(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->pkcs1_encrypt(backend_context, f_rng, p_rng, mode, ilen, input, output);
}

#if defined(MBEDTLS_PKCS1_V15)

int mbedtls_rsa_rsaes_pkcs1_v15_encrypt(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsrsa_pkcs1_v15_encrypt(backend_context, f_rng, p_rng, mode, ilen, input, output);
}

#endif

#if defined(MBEDTLS_PKCS1_V21)

int mbedtls_rsa_rsaes_oaep_encrypt(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t ilen, const unsigned char *input, unsigned char *output)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsrsa_oaep_encrypt(backend_context, f_rng, p_rng, mode, label, label_len, ilen, input, output);
}

#endif

int mbedtls_rsa_pkcs1_decrypt(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->pkcs1_decrypt(backend_context, f_rng, p_rng, mode, olen, input, output, output_max_len);
}

#if defined(MBEDTLS_PKCS1_V15)

int mbedtls_rsa_rsaes_pkcs1_v15_decrypt(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsrsa_pkcs1_v15_decrypt(backend_context, f_rng, p_rng, mode, olen, input, output, output_max_len);
}

#endif

#if defined(MBEDTLS_PKCS1_V21)

int mbedtls_rsa_rsaes_oaep_decrypt(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsaes_oaep_decrypt(backend_context, f_rng, p_rng, mode, label, label_len, olen, input, output, output_max_len);
}

#endif

int mbedtls_rsa_pkcs1_sign(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsrsa_oaep_decrypt(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
}

#if defined(MBEDTLS_PKCS1_V15)

int mbedtls_rsa_rsassa_pkcs1_v15_sign(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsassa_pkcs1_v15_sign(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
}

#endif

#if defined(MBEDTLS_PKCS1_V21)

int mbedtls_rsa_rsassa_pss_sign(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsassa_pss_sign(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
}

#endif

int mbedtls_rsa_pkcs1_verify(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->pkcs1_verify(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
}

#if defined(MBEDTLS_PKCS1_V15)

int mbedtls_rsa_rsassa_pkcs1_v15_verify(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsassa_pkcs1_v15_verify(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
}

#endif

#if defined(MBEDTLS_PKCS1_V21)

int mbedtls_rsa_rsassa_pss_verify(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsassa_pss_verify(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, sig);
}

int mbedtls_rsa_rsassa_pss_verify_ext(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, mbedtls_md_type_t mgf1_hash_id, int expected_salt_len, const unsigned char *sig)
{
    const mbedtls_rsa_funcs* funcs;
    void* backend_context;

    RSA_CONTEXT_UNPACK_NOT_NULL(ctx, funcs, backend_context);

    return funcs->rsassa_pss_verify_ext(backend_context, f_rng, p_rng, mode, md_alg, hashlen, hash, mgf1_hash_id, expected_salt_len, sig);
}

#endif

int mbedtls_rsa_copy(mbedtls_rsa_context *dst, const mbedtls_rsa_context *src)
{
    const mbedtls_rsa_funcs* funcs_dst = NULL;
    void* backend_context_dst;
    const mbedtls_rsa_funcs* funcs_src;
    void* backend_context_src;

    RSA_CONTEXT_UNPACK_NOT_NULL(src, funcs_src, backend_context_src);

    RSA_CONTEXT_ALLOC(funcs_dst, backend_context_dst, funcs_src); // TODO: check if it is ok when dst is already initialized

    return funcs_dst->copy(backend_context_dst, backend_context_src);
}

void mbedtls_rsa_free(mbedtls_rsa_context *ctx)
{
    mbedtls_rsa_funcs* funcs;
    void* backend_context;
    RSA_CONTEXT_UNPACK(ctx, funcs, backend_context);
    if (funcs != NULL)
    {
        funcs->free(backend_context);
        RSA_CONTEXT_FREE(ctx);
    }
}

#endif /* MBEDTLS_BACKEND_RSA_GLUE */
