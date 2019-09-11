#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
set(NRF_MBEDTLS_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(NRF_CC310_MBEDCRYPTO_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/../crypto/nrf_cc310_mbedcrypto/include/mbedtls")

if(CONFIG_GENERATE_MBEDTLS_CFG_FILE OR CONFIG_GENERATE_THREAD_MBEDTLS_CFG_FILE)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/kconfig_mbedtls_configure.cmake)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/symbol_rename.cmake)

set(common_includes "${CMAKE_CURRENT_LIST_DIR}/include/mbedcrypto")

zephyr_include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/)
zephyr_include_directories(${CMAKE_CURRENT_LIST_DIR}/include/mbedcrypto)

if(ARM_MBEDTLS_PATH)
  set(ARM_MBEDTLS_ROOT ${ARM_MBEDTLS_PATH})
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

