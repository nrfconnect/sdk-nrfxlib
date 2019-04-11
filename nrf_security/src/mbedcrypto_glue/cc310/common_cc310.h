/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * File containing macros used for mbedcrypto glue for CC310 backend
 */

#ifndef COMMON_CC3XX_H
#define COMMON_CC3XX_H

#define _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT2(a, b) \
    a ## _ ## b
#define _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT(a, b) \
    _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT2(a, b)
#define MBEDTLS_BACKEND_CC3XX_ALIAS(return_type, name, A, B) \
    return_type _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT(name, MBEDTLS_BACKEND_SUFFIX) A; \
    return_type name A { return _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT(name, MBEDTLS_BACKEND_SUFFIX) B; }
#define MBEDTLS_BACKEND_CC3XX_ALIAS_VOID(name, A, B) \
    void _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT(name, MBEDTLS_BACKEND_SUFFIX) A; \
    void name A { _MBEDTLS_BACKEND_CC3XX_ALIAS_CAT(name, MBEDTLS_BACKEND_SUFFIX) B; }

#endif

/**@}*/