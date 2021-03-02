#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrf_security_debug("######### Creating mbedcrypto glue library #########")

#
# Create a library with common include folders (used by this and backend libs)
#
add_library(mbedcrypto_common_glue INTERFACE)

target_include_directories(mbedcrypto_common_glue INTERFACE
  ${NRF_SECURITY_ROOT}/include/mbedcrypto_glue
)

target_compile_definitions(mbedcrypto_common_glue INTERFACE
  -DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}"
)

target_link_libraries(mbedcrypto_common_glue INTERFACE mbedtls_common)

#
# Create glue libraries for backends (if enabled)
#
if (CONFIG_CC3XX_BACKEND)
  nrf_security_debug("Adding cc310_glue.cmake")
  include(${CMAKE_CURRENT_LIST_DIR}/cc310/cc310_glue.cmake)
endif()

if (CONFIG_OBERON_BACKEND)
  nrf_security_debug("Adding oberon_glue.cmake")
  include(${CMAKE_CURRENT_LIST_DIR}/oberon/oberon_glue.cmake)
endif()

if (CONFIG_MBEDTLS_VANILLA_BACKEND)
  nrf_security_debug("Adding vanilla_glue.cmake")
  include(${CMAKE_CURRENT_LIST_DIR}/vanilla/vanilla_glue.cmake)
endif()

#
# Create the glue library
#
nrf_security_library_glue(ALT
  FILES
    ${CMAKE_CURRENT_LIST_DIR}/aes_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/ccm_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/cmac_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/dhm_alt.c
  LINK_LIBRARIES
    mbedcrypto_common_glue
    $<TARGET_NAME_IF_EXISTS:mbedcrypto_glue_cc3xx>
    $<TARGET_NAME_IF_EXISTS:mbedcrypto_glue_oberon>
    $<TARGET_NAME_IF_EXISTS:mbedcrypto_glue_vanilla>
)

#
# Copy the correct glue include files to use for libraries
# (potentially overwriting other copied backend files)
#
nrf_security_configure_file(CONFIG_GLUE_MBEDTLS_AES_C
  ${mbedcrypto_glue_include_path}/aes_alt.h
)

nrf_security_configure_file(CONFIG_GLUE_MBEDTLS_CCM_C
  ${mbedcrypto_glue_include_path}/ccm_alt.h
)

nrf_security_configure_file(CONFIG_GLUE_MBEDTLS_CMAC_C
  ${mbedcrypto_glue_include_path}/cmac_alt.h
)

nrf_security_configure_file(CONFIG_GLUE_MBEDTLS_DHM_C
  ${mbedcrypto_glue_include_path}/dhm_alt.h
)

nrf_security_configure_file(CONFIG_GLUE_MBEDTLS_RSA_C
  ${mbedcrypto_glue_include_path}/rsa_alt.h
)
