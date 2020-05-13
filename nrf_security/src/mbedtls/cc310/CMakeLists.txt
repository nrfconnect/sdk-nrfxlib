#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

get_target_property(NRF_CC310_MBEDCRYPTO_INCLUDE_PATH mbedcrypto_cc310_imported BASE_INCLUDE_PATH)

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
# the CC310 binary library to create mbedcrypto_cc310 (naming neccesary for
# renaming in glue )
#
add_library(mbedcrypto_cc310_noglue STATIC
  ${ZEPHYR_BASE}/misc/empty_file.c
)

target_sources_ifdef(CONFIG_CC310_MBEDTLS_ECJPAKE_C
  mbedcrypto_cc310_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ecjpake.c
)

target_include_directories(mbedcrypto_cc310_noglue PRIVATE
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}
  ${common_includes}
  ${config_include}
)

if(TARGET platform_cc310)
  target_include_directories(mbedcrypto_cc310_noglue PRIVATE
    ${mbedcrypto_glue_include_path}/threading
    ${NRF_PLATFORM_CC310_INCLUDE_PATH}
  )
endif()

target_compile_options(mbedcrypto_cc310_noglue PRIVATE ${TOOLCHAIN_C_FLAGS})
target_ld_options(mbedcrypto_cc310_noglue PRIVATE ${TOOLCHAIN_LD_FLAGS})

target_compile_definitions(mbedcrypto_cc310_noglue PRIVATE
  -DMBEDTLS_CONFIG_FILE="nrf-config.h"
)

nrf_security_debug_list_target_files(mbedcrypto_cc310_noglue)

set(cc310_libs
  mbedcrypto_cc310_imported
  mbedcrypto_cc310_noglue
)

combine_archives(mbedcrypto_cc310 "${cc310_libs}")
symbol_strip_func(cc310)

#
# Copy alternate files to generated folder (if enabled in backend)
#
configure_file_ifdef(CONFIG_CC310_MBEDTLS_AES_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/aes_alt.h
  ${generated_include_path}/aes_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_CCM_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/ccm_alt.h
  ${generated_include_path}/ccm_alt.h COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_CMAC_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/cmac_alt.h
  ${generated_include_path}/cmac_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_DHM_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/dhm_alt.h
  ${generated_include_path}/dhm_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_RSA_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/rsa_alt.h
  ${generated_include_path}/rsa_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_SHA1_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/sha1_alt.h
  ${generated_include_path}/sha1_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_SHA256_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/sha256_alt.h
  ${generated_include_path}/sha256_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_ECP_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/ecp_alt.h
  ${generated_include_path}/ecp_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_CHACHA20_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/chacha20_alt.h
  ${generated_include_path}/chacha20_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_POLY1305_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/poly1305_alt.h
  ${generated_include_path}/poly1305_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_MBEDTLS_CHACHAPOLY_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/chachapoly_alt.h
  ${generated_include_path}/chachapoly_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_BACKEND
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/platform_alt.h
  ${generated_include_path}/platform_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_CC310_BACKEND
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/threading_alt.h
  ${generated_include_path}/threading_alt.h
  COPYONLY
)
