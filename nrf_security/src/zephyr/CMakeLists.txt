#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Additional build logic for Zephyr builds

# Add mbedcrypto common includes and definitions to zephyr build
target_link_libraries(zephyr_interface INTERFACE mbedcrypto_common)

# Add mbed TLS heap library - Not for TF-M build
if(CONFIG_MBEDTLS_ENABLE_HEAP)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/legacy/mbedtls_heap.c
  )
endif()

# Add entropy_poll only if NRF_CC3XX_PLATFORM is not added
# This file is not useful for TF-M build where entropy should come
# from SPE image, using psa_generate_random
if(NOT CONFIG_NRF_CC3XX_PLATFORM)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/legacy/entropy_poll.c
  )
endif()

if(CONFIG_BUILD_WITH_TFM)
  # Add replacement for memory_buffer_alloc.c for NS build
  if(CONFIG_MBEDTLS_ENABLE_HEAP)
    list(APPEND src_zephyr
      ${NRF_SECURITY_ROOT}/src/legacy/memory_buffer_alloc.c
    )
  endif()

  # Add the TF-M interface to zephyr includes
  zephyr_include_directories(
    ${ZEPHYR_TRUSTED_FIRMWARE_M_MODULE_DIR}/trusted-firmware-m/interface/include
  )
else()
  # Use the standard memory_buffer_alloc.c
  if (CONFIG_MBEDTLS_ENABLE_HEAP)
    list(APPEND src_zephyr
      ${ARM_MBEDTLS_PATH}/library/memory_buffer_alloc.c
    )
  endif()
endif()

# Add the zephyr-specific sources neccessary for nrf_security
# This library contains files that require knowledge of zephyr
if(DEFINED src_zephyr)
  zephyr_library_named(mbedtls_zephyr)

  zephyr_library_sources(${src_zephyr})

  zephyr_library_include_directories(
    $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_INCLUDE_DIRECTORIES>
  )

  zephyr_library_compile_definitions(
    $<TARGET_PROPERTY:${mbedcrypto_target},INTERFACE_COMPILE_DEFINITIONS>
  )

  zephyr_library_app_memory(k_mbedtls_partition)
endif()

# Add configuration/options from zephyr interface libraries
nrf_security_add_zephyr_options(${mbedcrypto_target})

# Add the generic mbedtls_external library to interface with zephyr
add_library(mbedtls_external INTERFACE)

# Link to the NSPE versions of the libraries providing TLS/DTLS and x509 APIs
if(TARGET mbedtls)
  target_link_libraries(mbedtls_external INTERFACE mbedtls)
endif()

if(TARGET mbedx509)
  target_link_libraries(mbedtls_external INTERFACE mbedx509)
endif()

# Link with mbedcrypto library (in NS build or by default)
target_link_libraries(mbedtls_external INTERFACE ${mbedcrypto_target})

if(CONFIG_BUILD_WITH_TFM)

  if(TARGET mbedtls)
    add_dependencies(mbedtls tfm_mbedtls_headers_copy)
  endif()

  if(TARGET mbedx509)
    add_dependencies(mbedx509 tfm_mbedtls_headers_copy)
  endif()

  if(TARGET ${mbedcrypto_target})
    add_dependencies(${mbedcrypto_target} tfm_mbedtls_headers_copy)
  endif()

  zephyr_include_directories(
    ${ZEPHYR_TRUSTED_FIRMWARE_M_MODULE_DIR}/trusted-firmware-m/interface/include
  )

endif()

zephyr_link_libraries(mbedtls_external)
