#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
# The purpose of this file is to set specific MBEDTLS config file defines based
# on the Kconfig settings.
# When all values has been processed, the mbedtls config file will be generated.

#
# Internal macro which will enable the define in mbed TLS config file based on
# the corresponding Kconfig setting.
#
# This macro strips away CONFIG_ to make a define usable in mbed TLS config file
#
macro(kconfig_mbedtls_config base)
  if (CONFIG_${base})
    nrf_security_debug("Setting ${base} to TRUE")
    set(${base} TRUE)
  endif()
endmacro()


#
# Internal macro which will enable the define in mbed TLS config file based on
# the corresponding Kconfig setting.
#
# This macro strips away CONFIG_ to make a define usable in mbed TLS config file
# This macro will use the value provided as second argument in the config header.
#
macro(kconfig_mbedtls_config_val base val)
  if (CONFIG_${base})
    nrf_security_debug("Setting ${base} to ${val}")
    set(${base} ${val})
  endif()
endmacro()

#
# Internal macro which will enable the define dependencies in mbed TLS config
# file if the any backend has enabled the setting in Kconfig specified by first
# argument.
#
macro(mbedtls_config_define_depends mbedtls_config)
  set(${mbedtls_config} TRUE)
  foreach(arg ${ARGN})
    if (NOT CONFIG_${arg})
      unset(${mbedtls_config})
      break()
    endif()
  endforeach()
endmacro()

#
# Internal macro to configure file if mbed TLS config is set
#
macro(nrf_security_configure_file mbedtls_config file)
  if (${mbedtls_config})

    nrf_security_debug("Configure file: ${file}")
    get_filename_component(file_name ${file} NAME)
    configure_file(${file} ${generated_include_path}/mbedtls/${file_name} COPYONLY)
  endif()
endmacro()

#
# Include PSA configuration
#
include(${CMAKE_CURRENT_LIST_DIR}/kconfig_psa_configure.cmake)

#
# Remove previously generated files from include folder
#
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/include/)

# User config file
#
kconfig_mbedtls_config_val("MBEDTLS_USER_CONFIG_FILE"	"${CONFIG_MBEDTLS_USER_CONFIG_FILE}")

# Enabling core functionality
#
kconfig_mbedtls_config("MBEDTLS_AES_C")
kconfig_mbedtls_config("MBEDTLS_CMAC_C")
kconfig_mbedtls_config("MBEDTLS_CCM_C")
kconfig_mbedtls_config("MBEDTLS_DHM_C")
kconfig_mbedtls_config("MBEDTLS_MD5_C")
kconfig_mbedtls_config("MBEDTLS_SHA1_C")
kconfig_mbedtls_config("MBEDTLS_SHA256_C")
kconfig_mbedtls_config("MBEDTLS_SHA512_C")
kconfig_mbedtls_config("MBEDTLS_ECP_C")
kconfig_mbedtls_config("MBEDTLS_ECDH_C")
kconfig_mbedtls_config("MBEDTLS_ECDSA_C")
kconfig_mbedtls_config("MBEDTLS_ECJPAKE_C")
kconfig_mbedtls_config("MBEDTLS_GCM_C")
kconfig_mbedtls_config("MBEDTLS_RSA_C")
kconfig_mbedtls_config("MBEDTLS_CHACHA20_C")
kconfig_mbedtls_config("MBEDTLS_POLY1305_C")
kconfig_mbedtls_config("MBEDTLS_CHACHAPOLY_C")

#
# Setting mbedtls defines for features not glued
#
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CBC")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CFB")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CTR")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_OFB")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_XTS")
kconfig_mbedtls_config("MBEDTLS_CTR_DRBG_C")
kconfig_mbedtls_config("MBEDTLS_HMAC_DRBG_C")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP192R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP224R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP256R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP384R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP521R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP192K1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP224K1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_SECP256K1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_BP256R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_BP384R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_BP512R1_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_CURVE25519_ENABLED")
kconfig_mbedtls_config("MBEDTLS_ECP_DP_CURVE448_ENABLED")
kconfig_mbedtls_config("MBEDTLS_CIPHER_PADDING_PKCS7")
kconfig_mbedtls_config("MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS")
kconfig_mbedtls_config("MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN")
kconfig_mbedtls_config("MBEDTLS_CIPHER_PADDING_ZEROS")
kconfig_mbedtls_config("MBEDTLS_MD_C")
kconfig_mbedtls_config("MBEDTLS_PKCS1_V15")
kconfig_mbedtls_config("MBEDTLS_PKCS1_V21")

#
# Nordic added defines for features not supported in HW
# and might not be available (due to configuration)
#
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_ECB_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CBC_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CTR_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CCM_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_OFB_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CFB_C")
kconfig_mbedtls_config("MBEDTLS_AES_256_CMAC_C")

#
# Advanced configuration setting for mbed TLS
#
kconfig_mbedtls_config("MBEDTLS_AES_ROM_TABLES")
kconfig_mbedtls_config("MBEDTLS_AES_FEWER_TABLES")
kconfig_mbedtls_config_val("MBEDTLS_MPI_WINDOW_SIZE"       "${CONFIG_MBEDTLS_MPI_WINDOW_SIZE}")
kconfig_mbedtls_config_val("MBEDTLS_MPI_MAX_SIZE"          "${CONFIG_MBEDTLS_MPI_MAX_SIZE}")
kconfig_mbedtls_config_val("MBEDTLS_ECP_MAX_BITS"          "${CONFIG_MBEDTLS_ECP_MAX_BITS}")
kconfig_mbedtls_config_val("MBEDTLS_ECP_WINDOW_SIZE"       "${CONFIG_MBEDTLS_ECP_WINDOW_SIZE}")
kconfig_mbedtls_config_val("MBEDTLS_ECP_FIXED_POINT_OPTIM" "1")
kconfig_mbedtls_config_val("MBEDTLS_SSL_MAX_CONTENT_LEN"   "${CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN}")
kconfig_mbedtls_config_val("MBEDTLS_SSL_CIPHERSUITES"      "${CONFIG_MBEDTLS_SSL_CIPHERSUITES}")
kconfig_mbedtls_config("MBEDTLS_SHA256_SMALLER")
kconfig_mbedtls_config("MBEDTLS_SSL_PROTO_TLS1_2")
kconfig_mbedtls_config("MBEDTLS_SSL_TLS_C")
kconfig_mbedtls_config("MBEDTLS_SSL_SRV_C")
kconfig_mbedtls_config("MBEDTLS_SSL_CLI_C")

#
# CC3XX flags for threading and platform zeroize
# Note: These must be set in noglue an and regular configuration for correct
# context sizes
#
if (CONFIG_CC3XX_BACKEND)
  set(MBEDTLS_PLATFORM_ZEROIZE_ALT TRUE)
  if(NOT DEFINED MBEDTLS_THREADING_C)
    set(MBEDTLS_THREADING_C TRUE)
  endif()

  if(NOT DEFINED MBEDTLS_THREADING_ALT)
    set(MBEDTLS_THREADING_ALT TRUE)
  endif()
endif()

#
# Additional flags for vanilla
#
set(MBEDTLS_MD_C TRUE)
set(MBEDTLS_CIPHER_C TRUE)

#
# Check if newlib is set, if so these configurations must be set
# for the vanilla noglue as well as regular configuration for correct API calls.
#
if (NOT CONFIG_NEWLIB_LIBC)
  set(MBEDTLS_PLATFORM_NO_STD_FUNCTIONS TRUE)
  set(MBEDTLS_PLATFORM_FPRINTF_ALT TRUE)
  set(MBEDTLS_PLATFORM_PRINTF_ALT TRUE)
  set(MBEDTLS_PLATFORM_SNPRINTF_ALT TRUE)
endif ()


#
# Generating glue header file (without alt flags set)
#
configure_file(${CMAKE_CURRENT_LIST_DIR}/../configs/nrf-config.h.template
               ${CMAKE_CURRENT_BINARY_DIR}/include/nrf-config-noglue.h)

#
# Generate glue header file for nrf_oberon (with oberon specific alt flags
# temporary set)
#
if (CONFIG_OBERON_BACKEND)
  # Temporary set the AES setkey and encrypt/decrypt
  set(MBEDTLS_AES_SETKEY_ENC_ALT TRUE)
  set(MBEDTLS_AES_SETKEY_DEC_ALT TRUE)
  set(MBEDTLS_AES_ENCRYPT_ALT TRUE)
  set(MBEDTLS_AES_DECRYPT_ALT TRUE)
  set(MBEDTLS_ECDH_GEN_PUBLIC_ALT TRUE)
  set(MBEDTLS_ECDH_COMPUTE_SHARED_ALT TRUE)
  set(MBEDTLS_ECDSA_GENKEY_ALT TRUE)
  set(MBEDTLS_ECDSA_SIGN_ALT TRUE)
  set(MBEDTLS_ECDSA_VERIFY_ALT TRUE)
  set(MBEDTLS_ECJPAKE_ALT TRUE)
  set(MBEDTLS_SHA1_ALT TRUE)
  set(MBEDTLS_SHA256_ALT TRUE)

  configure_file(${CMAKE_CURRENT_LIST_DIR}/../configs/nrf-config.h.template
                 ${CMAKE_CURRENT_BINARY_DIR}/include/nrf-config-noglue-oberon.h)

  # Reset the defines again
  set(MBEDTLS_AES_SETKEY_ENC_ALT FALSE)
  set(MBEDTLS_AES_SETKEY_DEC_ALT FALSE)
  set(MBEDTLS_AES_ENCRYPT_ALT FALSE)
  set(MBEDTLS_AES_DECRYPT_ALT FALSE)
  set(MBEDTLS_ECDH_GEN_PUBLIC_ALT FALSE)
  set(MBEDTLS_ECDH_COMPUTE_SHARED_ALT FALSE)
  set(MBEDTLS_ECDSA_GENKEY_ALT FALSE)
  set(MBEDTLS_ECDSA_SIGN_ALT FALSE)
  set(MBEDTLS_ECDSA_VERIFY_ALT FALSE)
  set(MBEDTLS_ECJPAKE_ALT FALSE)
  set(MBEDTLS_SHA1_ALT FALSE)
  set(MBEDTLS_SHA256_ALT FALSE)
endif()

#
# Alt flags set by no-prompt kconfig variables
# This must be done after any noglue header files are generated
#
kconfig_mbedtls_config("MBEDTLS_AES_ALT")
kconfig_mbedtls_config("MBEDTLS_CCM_ALT")
kconfig_mbedtls_config("MBEDTLS_CMAC_ALT")
kconfig_mbedtls_config("MBEDTLS_DHM_ALT")
kconfig_mbedtls_config("MBEDTLS_GCM_ALT")
kconfig_mbedtls_config("MBEDTLS_ECP_ALT")
kconfig_mbedtls_config("MBEDTLS_ECDH_GEN_PUBLIC_ALT")
kconfig_mbedtls_config("MBEDTLS_ECDH_COMPUTE_SHARED_ALT")
kconfig_mbedtls_config("MBEDTLS_ECDSA_GENKEY_ALT")
kconfig_mbedtls_config("MBEDTLS_ECDSA_SIGN_ALT")
kconfig_mbedtls_config("MBEDTLS_ECDSA_VERIFY_ALT")
kconfig_mbedtls_config("MBEDTLS_ECJPAKE_ALT")
kconfig_mbedtls_config("MBEDTLS_SHA1_ALT")
kconfig_mbedtls_config("MBEDTLS_SHA256_ALT")
kconfig_mbedtls_config("MBEDTLS_RSA_ALT")

#
# Defines currently not grouped
#
kconfig_mbedtls_config("MBEDTLS_PKCS1_V21")
kconfig_mbedtls_config("MBEDTLS_PKCS1_V15")
kconfig_mbedtls_config("MBEDTLS_PK_C")
kconfig_mbedtls_config("MBEDTLS_PK_PARSE_C")
kconfig_mbedtls_config("MBEDTLS_PK_WRITE_C")
kconfig_mbedtls_config("MBEDTLS_X509_USE_C")
kconfig_mbedtls_config("MBEDTLS_X509_CRT_PARSE_C")

if (CONFIG_CC3XX_BACKEND)
  set(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT TRUE)
endif()

if (CC3XX_MBEDTLS_ECDH_C OR OBERON_MBEDTLS_ECDH_C)
  set(MBEDTLS_ECDH_GEN_PUBLIC_ALT TRUE)
  set(MBEDTLS_ECDH_COMPUTE_SHARED_ALT TRUE)
endif()

if (CC3XX_MBEDTLS_ECDSA_C OR OBERON_MBEDTLS_ECDSA_C)
  set(MBEDTLS_ECDSA_VERIFY_ALT TRUE)
  set(MBEDTLS_ECDSA_SIGN_ALT TRUE)
  set(MBEDTLS_ECDSA_GENKEY_ALT TRUE)
endif()

if (CONFIG_MBEDTLS_TLS_LIBRARY)
  set(MBEDTLS_SSL_PROTO_DTLS TRUE)
  set(MBEDTLS_SSL_DTLS_HELLO_VERIFY TRUE)
  set(MBEDTLS_SSL_COOKIE_C TRUE)
endif ()

if (CONFIG_MBEDTLS_X509_LIBRARY)
  set(MBEDTLS_X509_USE_C TRUE)
  set(MBEDTLS_PK_C TRUE)
  set(MBEDTLS_ASN1_PARSE_C TRUE)
  set(MBEDTLS_PK_PARSE_C TRUE)
  set(MBEDTLS_X509_CRT_PARSE_C TRUE)
endif()

if (MBEDTLS_SSL_PROTO_TLS1_2)
  set(MBEDTLS_SSL_ENCRYPT_THEN_MAC TRUE)
  set(MBEDTLS_SSL_EXTENDED_MASTER_SECRET TRUE)
endif()

#
# Ciphers and configurations disabled due to not being recommended
# Please see nrf-config.h.template or mbed TLS documentation for details
#
set(MBEDTLS_ARC4_C False)

#
# Defines currently not grouped
#
kconfig_mbedtls_config("MBEDTLS_SSL_RENEGOTIATION")
kconfig_mbedtls_config("MBEDTLS_SSL_ALPN")
kconfig_mbedtls_config("MBEDTLS_SSL_DTLS_CLIENT_PORT_REUSE")
kconfig_mbedtls_config("MBEDTLS_SSL_DTLS_BADMAC_LIMIT")
kconfig_mbedtls_config("MBEDTLS_SSL_SESSION_TICKETS")
kconfig_mbedtls_config("MBEDTLS_SSL_SERVER_NAME_INDICATION")
kconfig_mbedtls_config("MBEDTLS_SSL_CACHE_C")
kconfig_mbedtls_config("MBEDTLS_SSL_TICKET_C")
kconfig_mbedtls_config("MBEDTLS_SSL_CONTEXT_SERIALIZATION")
kconfig_mbedtls_config("MBEDTLS_SSL_DTLS_CONNECTION_ID")
kconfig_mbedtls_config("MBEDTLS_X509_CHECK_KEY_USAGE")
kconfig_mbedtls_config("MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE")
kconfig_mbedtls_config("MBEDTLS_X509_CRL_PARSE_C")
kconfig_mbedtls_config("MBEDTLS_X509_CSR_PARSE_C")
kconfig_mbedtls_config("MBEDTLS_X509_CREATE_C")
kconfig_mbedtls_config("MBEDTLS_X509_CSR_WRITE_C")
kconfig_mbedtls_config_val("MBEDTLS_ENTROPY_MAX_SOURCES"          "${CONFIG_MBEDTLS_ENTROPY_MAX_SOURCES}")

if (CONFIG_TRUSTED_EXECUTION_SECURE AND CONFIG_MBEDTLS_ENTROPY_MAX_SOURCES GREATER_EQUAL 1)
  message(FATAL_ERROR "When building trusted firmware CryptoCell must be the only entropy source (CONFIG_MBEDTLS_ENTROPY_MAX_SOURCES set to 1)")
endif()

#
# Compare the following with check-config.h in mbed TLS
#
mbedtls_config_define_depends("MBEDTLS_SSL_DTLS_ANTI_REPLAY"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED"
  "MBEDTLS_ECDH_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED"
  "MBEDTLS_RSA_C"
  "MBEDTLS_ECDH_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED"
  "MBEDTLS_DHM_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED"
  "MBEDTLS_ECDH_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED"
  "MBEDTLS_DHM_C"
  "MBEDTLS_RSA_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
  "MBEDTLS_PKCS1_V15"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED"
  "MBEDTLS_ECDH_C"
  "MBEDTLS_RSA_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
  "MBEDTLS_PKCS1_V15"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED"
  "MBEDTLS_ECDH_C"
  "MBEDTLS_ECDSA_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_RSA_PSK_ENABLED"
  "MBEDTLS_RSA_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
  "MBEDTLS_PKCS1_V15"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_RSA_ENABLED"
  "MBEDTLS_RSA_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
  "MBEDTLS_PKCS1_V15"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED"
  "MBEDTLS_ECJPAKE_C"
  "MBEDTLS_TLS_LIBRARY"
  "MBEDTLS_X509_LIBRARY"
)

mbedtls_config_define_depends("MBEDTLS_ECDSA_DETERMINISTIC"
  "MBEDTLS_ECDSA_C"
  "MBEDTLS_HMAC_DRBG_C"
)

#
# Folder path for files to be copied (regular mbed TLS files)
#
set(copied_include_path
  "${CMAKE_CURRENT_BINARY_DIR}/include/mbedtls_copied"
)

#
# Folder path for files to be generated (alt-files and config)
#
set(generated_include_path
  "${CMAKE_CURRENT_BINARY_DIR}/include/mbedtls_generated"
)

#
# Include for generated mbed TLS config file(s)
#
# Note: This is required for all types of build (regular/glued)
#
set(config_include
  ${CMAKE_CURRENT_BINARY_DIR}/include
)

#
# Includes for all generated header files
#
# Note: This can't be used for glued builds
#
set(generated_includes
  ${config_include}
  ${generated_include_path}
)

#
# Add a target with interface includes for mbed TLS
#
# Note: This library does not include generated header files
#       Only copied generic files from mbed TLS
#
add_library(mbedcrypto_includes INTERFACE)

#
# Includes for all generic builds
#
target_include_directories(mbedcrypto_includes
  INTERFACE
    ${config_include}
    ${NRF_SECURITY_ROOT}/include
    ${ARM_MBEDTLS_PATH}/library
)

set(mbedtls_include_dir ${ARM_MBEDTLS_PATH}/include/mbedtls)
file(GLOB MBEDTLS_INCLUDE_FILES
  RELATIVE ${mbedtls_include_dir}
  "${mbedtls_include_dir}/*.h"
)

if (COMPILE_PSA_APIS)
  list(TRANSFORM MBEDTLS_INCLUDE_FILES PREPEND "${copied_include_path}/mbedtls/")
  file(REMOVE ${MBEDTLS_INCLUDE_FILES})

  # Provide the PSA includes directly to the build
  # Either for TF-M build or when TrustZone isn't used.
  target_include_directories(mbedcrypto_includes
    INTERFACE
      ${ARM_MBEDTLS_PATH}/include
      ${ARM_MBEDTLS_PATH}/include/mbedtls
      ${ARM_MBEDTLS_PATH}/include/psa
  )
else()
  # PSA APIs aren't compiled. This means that the mbed TLS includes list
  # need to be provided in a "clean root" to ensure there is no
  # folder collision with the TF-M NS interface provided PSA headers
  #
  # Do note that this is only done because mbed TLS shares the same root
  # for files included with #include <mbedtls/xxxx.h> and
  # #include <psa/xxxx.h>. This is not possible when TF-M is enabled as it
  # provides its own version of PSA headers
  set(tfm_mbedtls_include_files ${MBEDTLS_INCLUDE_FILES})
  list(TRANSFORM tfm_mbedtls_include_files PREPEND "${copied_include_path}/mbedtls/")
  list(TRANSFORM MBEDTLS_INCLUDE_FILES PREPEND "${mbedtls_include_dir}/")

  add_custom_command(OUTPUT ${tfm_mbedtls_include_files}
    COMMAND ${CMAKE_COMMAND} -E copy ${MBEDTLS_INCLUDE_FILES} ${copied_include_path}/mbedtls
    DEPENDS ${MBEDTLS_INCLUDE_FILES}
    COMMAND_EXPAND_LISTS
  )
  add_custom_target(tfm_mbedtls_headers_copy
    DEPENDS ${tfm_mbedtls_include_files}
  )

  target_include_directories(mbedcrypto_includes INTERFACE
    ${copied_include_path}
  )

  add_dependencies(tfm_cmake tfm_mbedtls_headers_copy)
endif()

#
# Add library for generated includes
#
add_library(mbedcrypto_generated_includes INTERFACE)

target_include_directories(mbedcrypto_generated_includes
  INTERFACE
    ${generated_include_path}/mbedtls
    ${generated_include_path}
)

#
# Generate the mbed TLS config files
#
if (CONFIG_GENERATE_MBEDTLS_CFG_FILE)
configure_file(${CMAKE_CURRENT_LIST_DIR}/../configs/nrf-config.h.template
               ${config_include}/${CONFIG_MBEDTLS_CFG_FILE})
endif()
