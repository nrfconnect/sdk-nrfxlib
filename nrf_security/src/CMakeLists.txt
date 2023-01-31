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

if (CONFIG_MBEDTLS_PSA_CRYPTO_SPM)
  # Building as part of the TF-M build system.
  # In order to support FPU in TF-M the following is documented by the TF-M
  # build system:
  #
  #   Secure and non-secure libraries are compiled with COMPILER_CP_FLAG and
  #   linked with LINKER_CP_OPTION for different FP ABI types. All those
  #   libraries shall be built with COMPILER_CP_FLAG
  target_compile_options(mbedcrypto_common
    INTERFACE
      ${COMPILER_CP_FLAG}
  )

  target_link_options(mbedcrypto_common
    INTERFACE
      ${LINKER_CP_OPTION}
  )
endif()

set(generated_include_path "${CMAKE_CURRENT_BINARY_DIR}/include/generated")

# Empty out previous versions of config-files
file(REMOVE_RECURSE ${generated_include_path})
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

if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  # Add the generated CONFIG_MBEDTLS_CFG_FILE file to the include path
  target_include_directories(mbedcrypto_common
    INTERFACE
    ${generated_include_path} # From cmake/*_crypto_config.cmake
)
endif()

# x509 files
append_with_prefix(src_x509 ${ARM_MBEDTLS_PATH}/library
    x509.c
    x509_create.c
    x509_crl.c
    x509_crt.c
    x509_csr.c
    x509write_crt.c
    x509write_csr.c
)

# TLS files
append_with_prefix(src_tls ${ARM_MBEDTLS_PATH}/library
    debug.c
    net_sockets.c
    ssl_cache.c
    ssl_ciphersuites.c
    ssl_client.c
    ssl_cookie.c
    ssl_msg.c
    ssl_ticket.c
    ssl_tls.c
    ssl_tls12_client.c
    ssl_tls12_server.c
    ssl_tls13_keys.c
    ssl_tls13_server.c
    ssl_tls13_client.c
    ssl_tls13_generic.c
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
  append_with_prefix(src_crypto ${CMAKE_CURRENT_LIST_DIR}
    psa_crypto_driver_wrappers.c
  )
endif()

# Add PSA RNG API for the entropy driver
if(CONFIG_MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG AND NOT CONFIG_BUILD_WITH_TFM)
  list(APPEND src_crypto
    ${CMAKE_CURRENT_LIST_DIR}/random_ext.c
  )
endif()

append_with_prefix(src_crypto ${ARM_MBEDTLS_PATH}/library
  pem.c
  pkcs12.c
  pkcs5.c
  pkparse.c
  pkwrite.c
)

# Library for mbed TLS crypto toolbox functionality
add_library(${mbedcrypto_target}
  STATIC
    ${src_crypto}
    ${src_crypto_replacements}
)

nrf_security_add_zephyr_options(${mbedcrypto_target})

# Base mbed TLS files (not in drivers or builtin's)
append_with_prefix(src_crypto_base ${ARM_MBEDTLS_PATH}/library
  asn1parse.c
  asn1write.c
  base64.c
  bignum.c
  nist_kw.c
  oid.c
  padlock.c
  platform.c
  platform_util.c
  version.c
  constant_time.c
)

# Add base library with files required by all drivers/backends.
add_library(mbedcrypto_base
  STATIC
    ${src_crypto_base}
)

target_link_libraries(mbedcrypto_base
  PRIVATE
    mbedcrypto_common
)

target_link_libraries(${mbedcrypto_target}
  PRIVATE
    mbedcrypto_base
)

# Add regular mbedcrypto common includes and configurations
target_link_libraries(${mbedcrypto_target}
  PUBLIC
    mbedcrypto_common
)

# Don't be too strict with warnings on mbedtls code. This code is not
# ours, so it's not necessary to do too much static analysis on
# it. And any warnings that do pop up are difficult to fix as they
# require changes far upstream.
target_compile_options(${mbedcrypto_target} PRIVATE
  -Wno-unused-function
  -Wno-unused-variable
  )

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
  target_link_libraries(mbedcrypto_base PRIVATE -lc)
endif()

if (CONFIG_MBEDTLS_X509_LIBRARY)
  add_library(${mbedx509_target} STATIC
    ${src_x509}
  )

  # Add options from Zephyr build
  nrf_security_add_zephyr_options(${mbedx509_target})

  target_link_libraries(${mbedx509_target}
    PRIVATE
      ${mbedcrypto_target}
      mbedcrypto_base
    PUBLIC
      mbedcrypto_common
  )
endif()

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
      ${mbedx509_target}
      ${mbedcrypto_target}
      mbedcrypto_base
    PUBLIC
      mbedcrypto_common
  )
endif()

# Include EITS in Zephyr build
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/psa)
