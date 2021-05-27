#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

get_target_property(NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH mbedcrypto_cc3xx_imported INTERFACE_INCLUDE_DIRECTORIES)

#
# Creating a "noglue" version of the mbedcrypto CC310 libray.
# This library will be compiled with configuration made for the backend
# and will rely on header-files from the backend to override vanilla
# context sizes
#
# Note that some files are added from mbedcrypto vanilla to ensure that
# the noglue library is amended with missing symbols
#
# The noglue library will not be linked with directly. It is to be combined with
# the cc3xx binary library to create mbedcrypto_cc3xx
#
nrf_security_library(NOGLUE cc3xx
  FILES
    ${ARM_MBEDTLS_PATH}/library/ecjpake.c
  INCLUDES
    ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}
  DEFINES
    -DMBEDTLS_CONFIG_FILE="nrf-config.h"
  OPTIONS
    $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_COMPILE_OPTIONS>
  LINK_LIBRARIES
    platform_cc3xx
)


#
# Create the mbedcrypto library for cc3xx backend
#
add_library(${mbedcrypto_target}_cc3xx INTERFACE)
target_link_libraries(${mbedcrypto_target}_cc3xx INTERFACE
                      mbedcrypto_cc3xx_imported
)

#
# Copy alternate files to generated folder (if enabled in backend)
#
nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_AES_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/aes_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_CCM_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/ccm_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_CMAC_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/cmac_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_DHM_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/dhm_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_GCM_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/gcm_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_RSA_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/rsa_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_SHA1_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/sha1_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_SHA256_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/sha256_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_ECP_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/ecp_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_CHACHA20_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/chacha20_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_POLY1305_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/poly1305_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_MBEDTLS_CHACHAPOLY_C
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/chachapoly_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_BACKEND
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/platform_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_BACKEND
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/threading_alt.h
)

nrf_security_configure_file(CONFIG_CC3XX_BACKEND
  ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}/cc3xx_kmu.h
)
