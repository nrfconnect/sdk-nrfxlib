#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrf_security_debug("######### Creating mbedcrypto vanilla noglue library #########")

#
# Create mbedcrypto_vanilla_noglue library
# Doesn't have ALT flags set (to ensure vanilla context sizes)
# This implements features that may be glued but with the original contexts
# This must use be compiled with its own config-file (nrf-config-noglue.h)
# This library must not be linked with directly
#
nrf_security_library(NOGLUE vanilla
  FILES
    ${ARM_MBEDTLS_PATH}/library/aes.c
    ${ARM_MBEDTLS_PATH}/library/ccm.c
    ${ARM_MBEDTLS_PATH}/library/dhm.c
    ${ARM_MBEDTLS_PATH}/library/cmac.c
    ${ARM_MBEDTLS_PATH}/library/ecp.c
    ${ARM_MBEDTLS_PATH}/library/ecp_curves.c
    ${ARM_MBEDTLS_PATH}/library/ecdh.c
    ${ARM_MBEDTLS_PATH}/library/ecdsa.c
    ${ARM_MBEDTLS_PATH}/library/ecjpake.c
    ${ARM_MBEDTLS_PATH}/library/sha1.c
    ${ARM_MBEDTLS_PATH}/library/sha256.c
  DEFINES
    -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
)
