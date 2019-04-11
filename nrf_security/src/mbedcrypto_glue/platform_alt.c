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

#include "mbedtls/platform.h"

/* Currently no implementation is needed, because there is only one backend
 * that needs mbedtls_platform_setup() and mbedtls_platform_teardown().
 * If CC310 platform backend is enabled then mbedtls_platform_setup() and
 * mbedtls_platform_teardown() functions from it will be used. If it is
 * disabled then platform_alt implementation is also disabled, so original
 * (empty) implementation from mbedTLS is used.
*/
