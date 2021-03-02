#
# Copyright (c) 2019-2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
set(NRF_SECURITY_ROOT ${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_security_debug.cmake)

# If nrf_security generates config file, include cmake script that
# adds configurations according to Kconfig and converts them from
# CONFIG_MBEDTLS_XXXX to MBEDTLS_XXXX.
if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/kconfig_mbedtls_configure.cmake)
endif()

# Add nrf_security extension functions.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/extensions.cmake)

#
# Populate ARM_MBEDTLS_PATH
#
get_mbedtls_dir(ARM_MBEDTLS_PATH)


#
# Add include folder for glued libraries
#
set(mbedcrypto_glue_include_path
  "${CMAKE_CURRENT_LIST_DIR}/include/mbedcrypto_glue/mbedtls"
)

#
# Common includes
#
set(common_includes
  ${ARM_MBEDTLS_PATH}/include
  ${ARM_MBEDTLS_PATH}/include/mbedtls
  ${ARM_MBEDTLS_PATH}/include/psa
  ${ARM_MBEDTLS_PATH}/library
)

#
# Create an interface library with all shared compile definition and include
# directories
#
add_library(mbedtls_common INTERFACE)
target_include_directories(mbedtls_common INTERFACE
  ${generated_includes}
  ${common_includes}
)
target_compile_definitions(mbedtls_common INTERFACE
  -DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}"
)

#
#  Add nrf_security source folder
#
add_subdirectory(src)
