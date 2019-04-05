#ifndef MBEDTLS_DHM_ALT_H
#define MBEDTLS_DHM_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#if defined(MBEDTLS_DHM_ALT)

#include <stdint.h>
#include "mbedtls/bignum.h"

/**
 * \brief The DHM context-type definition.
 */
typedef struct mbedtls_dhm_context
{
    /*
     * Context must be compatible with original mbedTLS context, because
     * other mbedTLS modules are using internal fields of this context.
     */
    size_t len;
    mbedtls_mpi P;
    mbedtls_mpi G;
    mbedtls_mpi X;
    mbedtls_mpi GX;
    mbedtls_mpi GY;
    mbedtls_mpi K;
    mbedtls_mpi RP;
    mbedtls_mpi Vi;
    mbedtls_mpi Vf;
    mbedtls_mpi pX;
#if defined(CONFIG_GLUE_MBEDTLS_DHM_C)
    void* handle;
#endif
} mbedtls_dhm_context;

#endif /* MBEDTLS_DHM_ALT */

#endif /* MBEDTLS_DHM_ALT_H */
