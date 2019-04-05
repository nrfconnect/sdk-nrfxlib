#ifndef PLATFORM_ALT_H
#define PLATFORM_ALT_H

#include <stdint.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#define MBEDTLS_CC3XX_PLATFORM_CONTEXT_WORDS    (6)

typedef struct
{
#if defined(MBEDTLS_CC3XX_BACKEND)
    uint32_t cc3xx[MBEDTLS_CC3XX_PLATFORM_CONTEXT_WORDS];
#endif
#if !defined(MBEDTLS_CC3XX_BACKEND)
    uint8_t dummy;
#endif
} mbedtls_platform_context;

#endif
