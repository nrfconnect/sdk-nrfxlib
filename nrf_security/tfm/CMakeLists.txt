#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# This file is the entry point for mbed TLS built through TF-M.

set(NRFXLIB_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(NRF_SECURITY_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

include(${NRF_SECURITY_ROOT}/cmake/extensions.cmake)
include(${NRF_SECURITY_ROOT}/cmake/nrf_security_debug.cmake)

# Prefix targets as expected from TF-M
set(mbedcrypto_target ${MBEDTLS_TARGET_PREFIX}mbedcrypto)
set(mbedx509_target   ${MBEDTLS_TARGET_PREFIX}mbedx509)
set(mbedtls_target    ${MBEDTLS_TARGET_PREFIX}mbedtls)

# Building nrf security for TF-M implies the following:
# - Parse NRF_SECURITY_SETTINGS into CMake variables.
#   Variables transfered from Zephyr to nRF Security TF-M build includes:
#   - <build>/zephyr/.config
#   - GCC_M_CPU
#   - ARM_MBEDTLS_PATH
#   - autoconf.h
separate_arguments(NRF_SECURITY_SETTINGS)

# Convert each NRF_SECURITY_SETTING `key=value` pair to CMake variables.
foreach(setting ${NRF_SECURITY_SETTINGS})
  string(REGEX MATCH "^([^=]*)=(.*)" ignore ${setting})
  set(${CMAKE_MATCH_1} ${CMAKE_MATCH_2})
endforeach()

# Standard extensions of Zephyr included in the TF-M build
include(${ZEPHYR_BASE}/cmake/extensions.cmake)
import_kconfig(CONFIG_ ${ZEPHYR_DOTCONFIG})

# Include nrfxlib common to get nrf_platform_cc3xx and other crypto libs
include(../../common.cmake)

# Additional TF-M build only settings:
# Set the MBEDTLS_PSA_CRYPTO_SPM configuration to ensure the PSA has
# prefixed names for mbedcrypto symbols (mbedcrypto__)
set(CONFIG_MBEDTLS_PSA_CRYPTO_SPM                   True)

# Platform cannot be selected when building for TF-M, because TF-M itself has
# control of the CryptoCell. Therefore, specifically for building TF-M we
# enable it manually.
set(CONFIG_NRF_CC3XX_PLATFORM                       True)

# Set the a define stating that KEY_ID encodes owner
set(CONFIG_MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER  True)

# Set the a _usage_ of PSA crypto to false for TF-M image build
set(CONFIG_MBEDTLS_USE_PSA_CRYPTO                   False)

# By setting CONFIG_BUILD_WITH_TFM to false, the nrf_security will make a
# complete build with all libraries and a full mbedcrypto library for linking.
set(CONFIG_BUILD_WITH_TFM                           False)

# TF-M build require that PSA Crypto APIs are compiled
set(COMPILE_PSA_APIS True)

if (${PROJECT_NAME} STREQUAL "Bootloader")
  set(CONFIG_CC3XX_MBEDTLS_RSA_C    True)
  set(CONFIG_MBEDTLS_RSA_C          True)
  set(CONFIG_MBEDTLS_PKCS1_V21      True)
  set(CONFIG_MBEDTLS_PSA_CRYPTO_SPM False)
endif()
set(COMPILE_PSA_APIS                                True)

# The TF-M build needs a standard version of memory_buffer_alloc.c
# This currently doesn't do a mutex on heap access according to cc3xx rtos
# methods...
list(APPEND src_crypto ${ARM_MBEDTLS_PATH}/library/memory_buffer_alloc.c)

# Add mbed TLS libraries
add_subdirectory(${NRF_SECURITY_ROOT}/src ${CMAKE_CURRENT_BINARY_DIR}/nrf_security_src)
