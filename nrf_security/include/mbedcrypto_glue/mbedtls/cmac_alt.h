#ifndef MBEDTLS_CMAC_ALT_H
#define MBEDTLS_CMAC_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_CMAC_ALT)

#include <stdint.h>

/**
 * \brief The CMAC context-type definition.
 */
typedef struct mbedtls_cmac_context_t
{
    /* All CMAC structure allocation/deallocation is handled by specific backend, so
    there is no need to put here any content */
    uint32_t dummy;
} mbedtls_cmac_context_t;

void mbedtls_cipher_cmac_free(mbedtls_cipher_context_t *ctx);

#endif /* MBEDTLS_CMAC_ALT */

#endif /* MBEDTLS_CMAC_ALT_H */
