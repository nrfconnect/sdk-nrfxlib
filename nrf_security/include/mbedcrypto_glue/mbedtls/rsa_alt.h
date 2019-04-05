#ifndef MBEDTLS_RSA_ALT_H
#define MBEDTLS_RSA_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#define MBEDTLS_CC3XX_RSA_CONTEXT_WORDS 52
#define MBEDTLS_DEFAULT_RSA_CONTEXT_WORDS 43

#if defined(MBEDTLS_RSA_ALT)

#include <stdint.h>

/**
 * \brief The RSA context-type definition.
 */
typedef struct mbedtls_rsa_context
{
#if defined(CONFIG_GLUE_MBEDTLS_RSA_C)
    void* handle;
    int padding;
    int hash_id;
#endif
    union
    {
#if defined(CONFIG_CC310_MBEDTLS_RSA_C)
        uint32_t buffer_cc3xx[MBEDTLS_CC3XX_RSA_CONTEXT_WORDS];
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_RSA_C)
        uint32_t buffer_default[MBEDTLS_DEFAULT_RSA_CONTEXT_WORDS];
#endif
        uint32_t dummy;
    } buffer;
} mbedtls_rsa_context;

#endif /* MBEDTLS_RSA_ALT */

#endif /* MBEDTLS_RSA_ALT_H */
