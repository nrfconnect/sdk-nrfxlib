#ifndef MBEDTLS_CCM_ALT_H
#define MBEDTLS_CCM_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/cipher.h"

#define CC310_MBEDTLS_CCM_CONTEXT_WORDS     (24)
#define VANILLA_MBEDTLS_CCM_CONTEXT_WORDS   ((sizeof(mbedtls_cipher_context_t) + 3) / 4)

#if defined(MBEDTLS_CCM_ALT)

#include <stdint.h>

/**
 * \brief The CCM context-type definition.
 */
typedef struct mbedtls_ccm_context
{
#if defined(CONFIG_GLUE_MBEDTLS_CCM_C)
    void* handle;
#endif
    union
    {
#if defined(CONFIG_CC310_MBEDTLS_CCM_C)
        uint32_t buffer_cc310[CC310_MBEDTLS_CCM_CONTEXT_WORDS];
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_CCM_C)
        uint32_t buffer_vanilla_mbedtls[VANILLA_MBEDTLS_CCM_CONTEXT_WORDS];
#endif
        uint32_t dummy;
    } buffer;
} mbedtls_ccm_context;

#endif /* MBEDTLS_CCM_ALT */

#endif /* MBEDTLS_CCM_ALT_H */
