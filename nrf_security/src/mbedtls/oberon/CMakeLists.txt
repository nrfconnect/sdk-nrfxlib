#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

get_target_property(NRF_OBERON_MBEDTLS_INCLUDE_PATH mbedcrypto_oberon_mbedtls_imported BASE_INCLUDE_PATH)
get_target_property(NRF_OBERON_INCLUDE_PATH mbedcrypto_oberon_imported BASE_INCLUDE_PATH)

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
# the nrf_oberon binary library to create mbedcrypto_oberon (naming neccesary for
# renaming in glue )
#
add_library(mbedcrypto_oberon_noglue STATIC
  ${ZEPHYR_BASE}/misc/empty_file.c
)

#
# mbed TLS files not present in nrf_oberon_mbedtls
# Added if oberon enables the specific functionality
#
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_AES_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/aes.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_CCM_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ccm.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_CMAC_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/cmac.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECP_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ecp.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECP_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ecp_curves.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECP_C
  mbedcrypto_oberon_noglue PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ecp_oberon.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECDSA_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ecdsa.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECDH_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ecdh.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECJPAKE_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/ecjpake.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_SHA1_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/sha1.c
)
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_SHA256_C
  mbedcrypto_oberon_noglue PRIVATE ${ARM_MBEDTLS_PATH}/library/sha256.c
)

#
# Add file required to to make nrf_oberon_mbedtls work with mbed TLS v2.16.x
#
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_ECP_C
  mbedcrypto_oberon_noglue PRIVATE ${CMAKE_CURRENT_LIST_DIR}/ecp_oberon.c
)

#
# Add file required to make xts work with alternate aes enc/dec setkey
#
target_sources_ifdef(CONFIG_OBERON_MBEDTLS_AES_C
  mbedcrypto_oberon_noglue PRIVATE ${CMAKE_CURRENT_LIST_DIR}/xts_oberon.c
)

target_include_directories(mbedcrypto_oberon_noglue PRIVATE
  ${NRF_OBERON_MBEDTLS_INCLUDE_PATH}
  ${NRF_OBERON_MBEDTLS_INCLUDE}
  ${common_includes}
  ${config_include}
)


#
# Add specific includes for threading and platform_cc310
#
if(TARGET platform_cc310)
  target_include_directories(mbedcrypto_oberon_noglue PRIVATE
    ${mbedcrypto_glue_include_path}/threading
    $<TARGET_PROPERTY:platform_cc310,BASE_INCLUDE_PATH>
  )
endif()

target_compile_options(mbedcrypto_oberon_noglue PRIVATE ${TOOLCHAIN_C_FLAGS})
target_ld_options(mbedcrypto_oberon_noglue PRIVATE ${TOOLCHAIN_LD_FLAGS})

target_compile_definitions(mbedcrypto_oberon_noglue PRIVATE
  -DMBEDTLS_CONFIG_FILE="nrf-config-noglue-oberon.h"
)

nrf_security_debug_list_target_files(mbedcrypto_oberon_noglue)

set(oberon_libs
  mbedcrypto_oberon_mbedtls_imported
  mbedcrypto_oberon_imported
  mbedcrypto_oberon_noglue
)

#
# Create the actual mbedcrypto_oberon library containing
# all backend-specific 
#
combine_archives(mbedcrypto_oberon "${oberon_libs}")
symbol_strip_func(oberon)

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
