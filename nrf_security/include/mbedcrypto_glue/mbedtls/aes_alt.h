#ifndef MBEDTLS_AES_ALT_H
#define MBEDTLS_AES_ALT_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#define CC310_MBEDTLS_AES_CONTEXT_WORDS         (24)
#define VANILLA_MBEDTLS_AES_CONTEXT_WORDS       (70)
#define VANILLA_MBEDTLS_AES_XTS_CONTEXT_WORDS   (140)

#if defined(MBEDTLS_AES_ALT)

#include <stdint.h>

/**
 * \brief The AES context-type definition.
 */
typedef struct mbedtls_aes_context
{
    void* handle;
    union
    {
#if defined(CONFIG_CC310_MBEDTLS_AES_C)
        uint32_t buffer_cc310[CC310_MBEDTLS_AES_CONTEXT_WORDS];
#endif
#if defined(CONFIG_VANILLA_MBEDTLS_AES_C)
        uint32_t buffer_vanilla_mbedtls[VANILLA_MBEDTLS_AES_CONTEXT_WORDS];
#endif
        uint32_t dummy;
    } buffer;
} mbedtls_aes_context;

#if defined(MBEDTLS_CIPHER_MODE_XTS)
/**
 * \brief The AES XTS context-type definition.
 */
typedef struct mbedtls_aes_xts_context
{
    void* handle;
    union
    {
#if defined(CONFIG_VANILLA_MBEDTLS_CIPHER_MODE_XTS)
        uint32_t buffer_vanilla_mbedtls[VANILLA_MBEDTLS_AES_XTS_CONTEXT_WORDS];
#endif
        uint32_t dummy;
    } buffer;
} mbedtls_aes_xts_context;
#endif /* MBEDTLS_CIPHER_MODE_XTS */

#endif /* MBEDTLS_AES_ALT */

#endif /* MBEDTLS_AES_ALT_H */
