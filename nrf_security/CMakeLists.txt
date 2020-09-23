#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
set(NRF_SECURITY_ROOT ${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_security_debug.cmake)
if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/kconfig_mbedtls_configure.cmake)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/symbol_rename.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/symbol_strip.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/combine_archives.cmake)

#
# Create an interface library with all shared compile definition and include
# directories
#
zephyr_interface_library_named(mbedtls_common)
target_compile_definitions(mbedtls_common INTERFACE
  -DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}"
)
target_include_directories(mbedtls_common INTERFACE
  ${generated_includes}
)

get_mbedtls_dir(ARM_MBEDTLS_PATH)

set(mbedcrypto_glue_include_path
  "${CMAKE_CURRENT_LIST_DIR}/include/mbedcrypto_glue/mbedtls"
)

#
# Common includes
#
set(common_includes
  ${ARM_MBEDTLS_PATH}/include
  ${ARM_MBEDTLS_PATH}/include/mbedtls
)

#
#  Add mbed TLS sources
#
add_subdirectory(src/mbedtls)
nrf_security_debug("-------- Adding src/mbedtls")

if (CONFIG_NRF_SECURITY_MULTI_BACKEND)
  # a. cc310 and vanilla selection of ciphers --> Renaming and glue
  # b. vanilla selection of ciphers only --> Renaming, but no glue
  add_subdirectory(src/mbedcrypto_glue)
endif()
