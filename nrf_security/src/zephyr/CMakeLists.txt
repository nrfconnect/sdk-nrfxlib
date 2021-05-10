#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
target_link_libraries(zephyr_interface INTERFACE mbedcrypto_generated_includes)
target_link_libraries(zephyr_interface INTERFACE mbedcrypto_includes)


# Add mbed TLS heap library - Not for TF-M
if(CONFIG_MBEDTLS_ENABLE_HEAP)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/mbedtls/mbedtls_heap.c
  )

  # Add replacement for memory_buffer_alloc only for non TF-M builds
  # Use original non TF-M and for NS image when building TF-M
  if((CONFIG_CC3XX_BACKEND AND CONFIG_BUILD_TFM) OR
     (CONFIG_CC3XX_BACKEND AND NOT CONFIG_BUILD_WITH_TFM))
    list(APPEND src_zephyr
      ${NRF_SECURITY_ROOT}/src/mbedtls/replacements/memory_buffer_alloc.c
    )
  else()
    list(APPEND src_zephyr
      ${ARM_MBEDTLS_PATH}/library/memory_buffer_alloc.c
    )
  endif()
endif()

if(CONFIG_BUILD_WITH_TFM)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/mbedtls/exit.c
    ${ARM_MBEDTLS_PATH}/library/platform.c
  )
endif()

# This configuration ensures that MBEDTLS_CONFIG_FILE is set for the zephyr
# build (TF-M and non TF-M)
target_compile_definitions(${mbedcrypto_target}
  INTERFACE
    -DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}"
)

# Add entropy_poll only if NRF_CC3XX_PLATFORM is not added
# This file is not useful for TF-M build where entropy should come
# from SPE image, using psa_generate_random
if(NOT CONFIG_NRF_CC3XX_PLATFORM)
    list(APPEND src_zephyr
      ${NRF_SECURITY_ROOT}/src/backend/entropy/entropy_poll.c
    )
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
