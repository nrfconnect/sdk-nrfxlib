#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# File including instructions to build PSA APIs.
#

#
# Internal macro to append all unnamed parameters with 'prefix' if condition
# is met
#
macro(append_with_prefix_ifdef var cond prefix)
  if (cond)
    append_with_prefix(prefix ${ARGN})
  endif()
endmacro()

function(append_with_prefix var prefix)
  set(listVar ${${var}})
  foreach(f ${ARGN})
    list(APPEND listVar "${prefix}/${f}")
  endforeach(f)
  set(${var} ${listVar} PARENT_SCOPE)
endfunction(append_with_prefix)

# Library for mbedcrypto includes and configurations
add_library(mbedcrypto_common INTERFACE)

# Add the config-file entry point
target_compile_definitions(mbedcrypto_common
  INTERFACE
    -DMBEDTLS_CONFIG_FILE="${CONFIG_MBEDTLS_CFG_FILE}"
    -DMBEDTLS_USER_CONFIG_FILE="${CONFIG_MBEDTLS_USER_CONFIG_FILE}"
)

set(generated_include_path "${CMAKE_CURRENT_BINARY_DIR}/include/generated")

# Add generated path first in list (order dependent)
target_include_directories(mbedcrypto_common
    INTERFACE
      ${generated_include_path}
      ${NRF_SECURITY_ROOT}/include/
      ${NRF_SECURITY_ROOT}/include/mbedtls
)

# Add custom PSA includes (if enabled)
if(COMPILE_PSA_APIS)
  target_include_directories(mbedcrypto_common
    INTERFACE
      ${NRF_SECURITY_ROOT}/include/psa
  )
endif()

# Add regular includes
target_include_directories(mbedcrypto_common
  INTERFACE
    ${ARM_MBEDTLS_PATH}/include
    ${ARM_MBEDTLS_PATH}/include/mbedtls
    ${ARM_MBEDTLS_PATH}/include/psa
    ${ARM_MBEDTLS_PATH}/library
)

# Generate mbed TLS configurations
if(CONFIG_MBEDTLS_LEGACY_CRYPTO_C OR NOT COMPILE_PSA_APIS)
  include(${NRF_SECURITY_ROOT}/cmake/legacy_crypto_config.cmake)
else()
  include(${NRF_SECURITY_ROOT}/cmake/psa_crypto_config.cmake)
endif()

# x509 files
append_with_prefix(src_x509 ${ARM_MBEDTLS_PATH}/library
    x509.c
    x509_create.c
    x509write_csr.c
    x509_crl.c
    x509_crt.c
    x509_csr.c
)

# TLS files
append_with_prefix(src_tls ${ARM_MBEDTLS_PATH}/library
    debug.c
    net_sockets.c
    ssl_cache.c
    ssl_ciphersuites.c
    ssl_cli.c
    ssl_cookie.c
    ssl_msg.c
    ssl_srv.c
    ssl_ticket.c
    ssl_tls.c
    ssl_tls13_keys.c
)

# Base mbed TLS files (not in drivers or builtin's)
append_with_prefix(src_crypto ${ARM_MBEDTLS_PATH}/library
    asn1parse.c
    asn1write.c
    base64.c
    bignum.c
    nist_kw.c
    oid.c
    padlock.c
    pem.c
    pkcs12.c
    pkcs5.c
    pkparse.c
    pkwrite.c
    platform.c
    platform_util.c
    version.c
)

if (COMPILE_PSA_APIS)
  # Add files for PSA build
  append_with_prefix(src_crypto ${ARM_MBEDTLS_PATH}/library/
    psa_crypto.c
    psa_crypto_aead.c
    psa_crypto_cipher.c
    psa_crypto_client.c
    psa_crypto_ecp.c
    psa_crypto_hash.c
    psa_crypto_mac.c
    psa_crypto_rsa.c
    psa_crypto_slot_management.c
    psa_crypto_storage.c
  )

  # Add the local version of psa_crypto_driver_wrappers.c
  append_with_prefix(src_crypto
    ${CMAKE_CURRENT_LIST_DIR}
      psa_crypto_driver_wrappers.c
  )
endif()

# Add PSA RNG API for the entropy driver
if(CONFIG_NRF_CC3XX_PLATFORM)
  list(APPEND src_crypto
    ${CMAKE_CURRENT_LIST_DIR}/random_ext.c
  )
endif()

# Library for mbed TLS crypto toolbox functionality
add_library(${mbedcrypto_target}
  STATIC
    ${src_crypto}
    ${src_crypto_replacements}
)

nrf_security_debug_list_target_files(${mbedcrypto_target})

# Add regular mbedcrypto common includes and configurations
target_link_libraries(${mbedcrypto_target}
  PRIVATE
    mbedcrypto_common
)

#target_link_libraries(${mbedcrypto_target}
#  INTERFACE
#    mbedcrypto_common
#)

# Add drivers if enabled
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/drivers)

# Add legacy mbed TLS APIs if enabled
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/legacy)

# Print all files in the mbedcrypto_target (with optional legacy-additions)
nrf_security_debug_list_target_files(${mbedcrypto_target})

# In TF-M build, a psa_interface lib is created and we must inherit its settings
set_property(TARGET ${mbedcrypto_target}
  APPEND PROPERTY
    PRIVATE_LINK_LIBRARIES
      $<TARGET_NAME_IF_EXISTS:psa_interface>
)

# NEWLIB requires linking with the C library
if (CONFIG_NEWLIB_LIBC)
  target_link_libraries(${mbedcrypto_target} PRIVATE -lc)
endif()

if (CONFIG_MBEDTLS_X509_LIBRARY)
  add_library(${mbedx509_target} STATIC
    ${src_x509}
  )

  # Add options from Zephyr build
  nrf_security_add_zephyr_options(${mbedx509_target})

  target_link_libraries(${mbedx509_target}
    PRIVATE
      mbedcrypto_common
      ${mbedcrypto_target}
  )
endif()


if(NOT CONFIG_MBEDTLS_PSA_CRYPTO_SPM)
  # Library for TLS protocol support
  if (CONFIG_MBEDTLS_TLS_LIBRARY)
    add_library(${mbedtls_target} STATIC
      ${src_tls}
      ${src_tls_replacement}
    )

    # Add options from Zephyr build
    nrf_security_add_zephyr_options(${mbedtls_target})

    target_link_libraries(${mbedtls_target}
      PRIVATE
        mbedcrypto_common
        ${MBEDTLS_TARGET_PREFIX}${mbedx509_target}
        ${mbedcrypto_target}
    )
  endif()
endif(NOT CONFIG_MBEDTLS_PSA_CRYPTO_SPM)

# Include EITS in Zephyr build
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/psa)
