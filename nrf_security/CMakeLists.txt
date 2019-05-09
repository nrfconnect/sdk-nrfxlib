#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
set(NRF_MBEDTLS_ROOT ${CMAKE_CURRENT_LIST_DIR})

if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/kconfig_mbedtls_configure.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/cmake/symbol_rename.cmake)

zephyr_compile_definitions(-DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}")
zephyr_include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/)

if(ARM_MBEDTLS_PATH)
  set(NRF_MBEDTLS_ROOT ${ARM_MBEDTLS_PATH})
elseif(WEST)
  ## Use `west list` to find mbedtls.
  execute_process(
    COMMAND
    ${WEST} list mbedtls --format={posixpath}
    OUTPUT_VARIABLE
    west_project_output
    RESULT_VARIABLE
    result
  )
  string(REGEX REPLACE "[\r\n]+" "" ARM_MBEDTLS_ROOT "${west_project_output}")
  if(${result})
    message(FATAL_ERROR "Failed to find mbedtls, cannot build security libraries")
  endif()
else()
  message(FATAL_ERROR "west not installed, please provide ARM_MBEDTLS_PATH to CMake to support security libraries")
endif()

add_subdirectory(src/mbedtls)

if (CONFIG_NRF_CRYPTO_BACKEND_COMBINATION_0)
  # a. cc310 and vanilla selection of ciphers --> Renaming and glue
  # b. vanilla selection of ciphers only --> Renaming, but no glue
  add_subdirectory(src/mbedcrypto_glue)
endif()

