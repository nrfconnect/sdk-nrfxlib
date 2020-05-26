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

#if defined(MBEDTLS_ECDSA_C) && defined(CONFIG_GLUE_MBEDTLS_ECDSA_C)

#include <string.h>
#include <stddef.h>

#include "mbedtls/ecdsa.h"
#include "backend_ecdsa.h"


#if defined(CONFIG_CC310_MBEDTLS_ECDSA_C)
extern mbedtls_ecdsa_funcs mbedtls_ecdsa_cc310_backend_funcs;
#endif
#if defined(CONFIG_OBERON_MBEDTLS_ECDSA_C)
extern mbedtls_ecdsa_funcs mbedtls_ecdsa_oberon_mbedtls_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_ECDSA_C)
extern mbedtls_ecdsa_funcs mbedtls_ecdsa_vanilla_mbedtls_backend_funcs;
#endif


static mbedtls_ecdsa_funcs* ecdsa_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_ECDSA_C)
    &mbedtls_ecdsa_cc310_backend_funcs,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_ECDSA_C)
    &mbedtls_ecdsa_oberon_mbedtls_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_ECDSA_C)
    &mbedtls_ecdsa_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_ecdsa_funcs* find_backend(mbedtls_ecp_group *grp, mbedtls_ecp_group_id gid, int function)
{
    int max_priority = 0;
    const mbedtls_ecdsa_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(ecdsa_backends) / sizeof(ecdsa_backends[0]); i++)
    {
        priority = ecdsa_backends[i]->check(grp, gid, function);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = ecdsa_backends[i];
        }
    }
    return funcs;
}

int mbedtls_ecdsa_sign(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s, const mbedtls_mpi *d, const unsigned char *buf, size_t blen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_ecdsa_funcs* funcs = find_backend(grp, MBEDTLS_ECP_DP_NONE, MBEDTLS_ECDSA_SIGN_FUNCTION);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
    return funcs->sign(grp, r, s, d, buf, blen, f_rng, p_rng);
}

int mbedtls_ecdsa_verify(mbedtls_ecp_group *grp, const unsigned char *buf, size_t blen, const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s)
{
    const mbedtls_ecdsa_funcs* funcs = find_backend(grp, MBEDTLS_ECP_DP_NONE, MBEDTLS_ECDSA_VERIFY_FUNCTION);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
    return funcs->verify(grp, buf, blen, Q, r, s);
}

int mbedtls_ecdsa_genkey(mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_ecdsa_funcs* funcs = find_backend(NULL, gid, MBEDTLS_ECDSA_GENKEY_FUNCTION);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
    return funcs->genkey(ctx, gid, f_rng, p_rng);
}

#endif /* MBEDTLS_ECDSA_C && CONFIG_GLUE_MBEDTLS_ECDSA_C*/
