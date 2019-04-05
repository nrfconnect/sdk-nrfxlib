#ifndef MBEDTLS_SHA1_ALT_H
#define MBEDTLS_SHA1_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#define MBEDTLS_CC3XX_SHA1_CONTEXT_WORDS  (60)

#if defined(MBEDTLS_SHA1_ALT)

#include <stdint.h>

/**
 * \brief The SHA1 context-type definition.
 */
typedef struct mbedtls_sha1_context
{
    union
    {
#if defined(MBEDTLS_BACKEND_SHA1_CC3XX)
        uint32_t buffer_cc3xx[MBEDTLS_CC3XX_SHA1_CONTEXT_WORDS];
#endif
        uint32_t dummy;
    } buffer;
} mbedtls_sha1_context;

#endif /* MBEDTLS_SHA1_ALT */

#endif /* MBEDTLS_SHA1_ALT_H */
