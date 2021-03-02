#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Add mbed TLS heap library - Not for TF-M
if(CONFIG_MBEDTLS_ENABLE_HEAP)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/mbedtls/mbedtls_heap.c
  )
endif()

# Add nRF5 HW entropy - Not for TF-M
if(CONFIG_ENTROPY_CC3XX)
elseif(CONFIG_ENTROPY_NRF5_RNG OR CONFIG_ENTROPY_NRF_LL_SOFTDEVICE)
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/backend/nrf5x/entropy_nrf5x.c
  )
endif()

# Add entropy_poll - Not for TF-M
if ((CONFIG_ENTROPY_CC3XX AND CONFIG_TRUSTED_EXECUTION_NONSECURE) OR
    CONFIG_ENTROPY_NRF5_RNG OR
    CONFIG_ENTROPY_NRF_LL_SOFTDEVICE)
    list(APPEND src_zephyr
      ${NRF_SECURITY_ROOT}/src/backend/entropy/entropy_poll.c
    )
endif()

#
# Add the zephyr-specific sources neccessary for nrf_security
# This library contains files that require knowledge of zephyr
#
if(DEFINED src_zephyr)
  zephyr_library_named(mbedtls_zephyr)

  zephyr_library_sources(${src_zephyr})

  zephyr_library_link_libraries(mbedtls_common)

  zephyr_library_app_memory(k_mbedtls_partition)
endif()

#
# Handle TF-M and non-TFM build dependencies with alias library
#
if(CONFIG_BUILD_WITH_TFM)
  # When building with TF-M, link to the NSPE tfm APIs.
  add_library(mbedcrypto_static_alias ALIAS tfm_api)
else()
  # When building without TF-M, link directly to mbedcrypto
  add_library(mbedcrypto_static_alias ALIAS mbedcrypto)
endif()

#
# Add the generic mbedtls_external library to interface with zephyr
#
add_library(mbedtls_external INTERFACE)

# Link to the NSPE versions of the libraries providing TLS/DTLS and x509 APIs
if(TARGET mbedtls)
  target_link_libraries(mbedtls_external INTERFACE mbedtls)
endif()

if(TARGET mbedx509)
  target_link_libraries(mbedtls_external INTERFACE mbedx509)
endif()

# mbedcrypto_static_alias will be resolved to either the mbedcrypto library or
# tfm_api
target_link_libraries(mbedtls_external INTERFACE mbedcrypto_static_alias)
