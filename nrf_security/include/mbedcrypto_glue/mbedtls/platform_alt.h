#ifndef PLATFORM_ALT_H
#define PLATFORM_ALT_H

#include <stdint.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#define CC310_MBEDTLS_PLATFORM_CONTEXT_WORDS    (6)

typedef struct
{
#if defined(CONFIG_CC310_BACKEND)
    uint32_t cc310[CC310_MBEDTLS_PLATFORM_CONTEXT_WORDS];
#endif
#if !defined(CONFIG_CC310_BACKEND)
    uint8_t dummy;
#endif
} mbedtls_platform_context;

#endif
