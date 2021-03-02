#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

get_target_property(NRF_OBERON_MBEDTLS_INCLUDE_PATH mbedcrypto_oberon_mbedtls_imported INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(NRF_OBERON_INCLUDE_PATH mbedcrypto_oberon_imported INTERFACE_INCLUDE_DIRECTORIES)

nrf_security_debug("######### Creating nrf_oberon noglue library #########")

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
# the nrf_oberon binary library to create mbedcrypto_oberon
#
nrf_security_library(NOGLUE oberon
  FILES
    ${ARM_MBEDTLS_PATH}/library/aes.c
    ${ARM_MBEDTLS_PATH}/library/ccm.c
    ${ARM_MBEDTLS_PATH}/library/cmac.c
    ${ARM_MBEDTLS_PATH}/library/ecp.c
    ${ARM_MBEDTLS_PATH}/library/ecp_curves.c
    ${CMAKE_CURRENT_LIST_DIR}/ecp_oberon.c
    ${ARM_MBEDTLS_PATH}/library/ecdsa.c
    ${ARM_MBEDTLS_PATH}/library/ecdh.c
    ${ARM_MBEDTLS_PATH}/library/ecjpake.c
    ${ARM_MBEDTLS_PATH}/library/sha1.c
    ${ARM_MBEDTLS_PATH}/library/sha256.c
    ${CMAKE_CURRENT_LIST_DIR}/xts_oberon.c
  INCLUDES
    ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}
    ${NRF_OBERON_INCLUDE_PATH}
  DEFINES
    -DMBEDTLS_CONFIG_FILE="nrf-config-noglue-oberon.h"
)

add_library(mbedcrypto_oberon INTERFACE)
target_link_libraries(mbedcrypto_oberon INTERFACE
                      mbedcrypto_oberon_mbedtls_imported
                      mbedcrypto_oberon_imported
)

#
# Copy the replacement _alt.h files by from nrf_oberon by enabled feature
#
configure_file_ifdef(CONFIG_OBERON_MBEDTLS_SHA1_C
  ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}/sha1_alt.h
  ${generated_include_path}/sha1_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_OBERON_MBEDTLS_SHA256_C
  ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}/sha256_alt.h
  ${generated_include_path}/sha256_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_OBERON_MBEDTLS_ECJPAKE_C
  ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}/ecjpake_alt.h
  ${generated_include_path}/ecjpake_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_OBERON_MBEDTLS_CHACHA20_C
  ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}/chacha20_alt.h
  ${generated_include_path}/chacha20_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_OBERON_MBEDTLS_POLY1305_C
  ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}/poly1305_alt.h
  ${generated_include_path}/poly1305_alt.h
  COPYONLY)
