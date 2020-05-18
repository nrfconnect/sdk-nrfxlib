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

if(ARM_MBEDTLS_PATH)
  # Do nothing, just use the provided path
elseif(WEST)
  ## Use `west list` to find the mbedtls tree we use (this is not the
  ## mbedtls tree distributed as a zephyr module).
  execute_process(
    COMMAND
    ${WEST} list mbedtls-nrf --format={posixpath}
    OUTPUT_VARIABLE
    west_project_output
    RESULT_VARIABLE
    result
  )
  string(REGEX REPLACE "[\r\n]+" "" ARM_MBEDTLS_PATH "${west_project_output}")
  if(${result})
    message(FATAL_ERROR "Failed to find mbedtls, cannot build security libraries")
  endif()
else()
  message(FATAL_ERROR "west not installed, please provide ARM_MBEDTLS_PATH to CMake to support security libraries")
endif()

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

if (CONFIG_NRF_CRYPTO_BACKEND_COMBINATION_0)
  # a. cc310 and vanilla selection of ciphers --> Renaming and glue
  # b. vanilla selection of ciphers only --> Renaming, but no glue
  add_subdirectory(src/mbedcrypto_glue)
endif()
