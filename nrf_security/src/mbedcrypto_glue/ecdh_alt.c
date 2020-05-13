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

#if defined(MBEDTLS_ECDH_C) && defined(CONFIG_GLUE_MBEDTLS_ECDH_C)

#include <string.h>
#include <stddef.h>

#include "mbedtls/ecdh.h"
#include "backend_ecdh.h"


#if defined(CONFIG_CC310_MBEDTLS_ECDH_C)
extern mbedtls_ecdh_funcs mbedtls_ecdh_cc310_backend_funcs;
#endif
#if defined(CONFIG_OBERON_MBEDTLS_ECDH_C)
extern mbedtls_ecdh_funcs mbedtls_ecdh_oberon_mbedtls_backend_funcs;
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_ECDH_C)
extern mbedtls_ecdh_funcs mbedtls_ecdh_vanilla_mbedtls_backend_funcs;
#endif


static mbedtls_ecdh_funcs* ecdh_backends[] = {
#if defined(CONFIG_CC310_MBEDTLS_ECDH_C)
    &mbedtls_ecdh_cc310_backend_funcs,
#endif
#if defined(CONFIG_OBERON_MBEDTLS_ECDH_C)
    &mbedtls_ecdh_oberon_mbedtls_backend_funcs,
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_ECDH_C)
    &mbedtls_ecdh_vanilla_mbedtls_backend_funcs,
#endif
};

static const mbedtls_ecdh_funcs* find_backend(mbedtls_ecp_group *grp, int function)
{
    int max_priority = 0;
    const mbedtls_ecdh_funcs* funcs = NULL;
    int priority;
    int i;
    for (i = 0; i < sizeof(ecdh_backends) / sizeof(ecdh_backends[0]); i++)
    {
        priority = ecdh_backends[i]->check(grp, function);
        if (priority > max_priority)
        {
            max_priority = priority;
            funcs = ecdh_backends[i];
        }
    }
    return funcs;
}

int mbedtls_ecdh_gen_public(mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_ecdh_funcs* funcs = find_backend(grp, MBEDTLS_ECDH_GEN_PUBLIC_FUNCTION);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
    return funcs->gen_public(grp, d, Q, f_rng, p_rng);
}

int mbedtls_ecdh_compute_shared(mbedtls_ecp_group *grp, mbedtls_mpi *z, const mbedtls_ecp_point *Q, const mbedtls_mpi *d, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    const mbedtls_ecdh_funcs* funcs = find_backend(grp, MBEDTLS_ECDH_COMPUTE_SHARED_FUNCTION);
    if (funcs == NULL)
    {
        return MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    }
    return funcs->compute_shared(grp, z, Q, d, f_rng, p_rng);
}

#endif /* MBEDTLS_ECDH_C && CONFIG_GLUE_MBEDTLS_ECDH_C */
