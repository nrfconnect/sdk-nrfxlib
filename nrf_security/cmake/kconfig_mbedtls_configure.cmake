#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
# The purpose of this file is to set specific MBEDTLS config file defines based
# on the Kconfig settings.
# When all values has been processed, the mbedtls config file will be generated.


#
# Internal macro which will enable the define in mbed TLS config file based on
# the correspong Kconfig setting.
# It will additionally set VANILLA_ONLY_MBEDTLS_XXXX in case no other
# backends is enabling the same feature
#
macro(kconfig_mbedtls_config base)
  if (CONFIG_${base} OR
      CONFIG_CC310_${base} OR
      CONFIG_VANILLA_${base})
    nrf_security_debug("Setting ${base} to TRUE")
    set(${base} TRUE)
  endif()
  if ((CONFIG_${base} AND NOT CONFIG_CC310_BACKEND) OR
      (CONFIG_VANILLA_${base} AND NOT CONFIG_CC310_${base}))
    nrf_security_debug("Setting VANILLA_ONLY_${base} to TRUE")
    set(VANILLA_ONLY_${base} TRUE)
  endif()
  if (CONFIG_${base} AND NOT CONFIG_MBEDTLS_VANILLA_BACKEND)
    nrf_security_debug("Setting CC310_${base} to TRUE")
    set(CC310_${base} TRUE)
  endif()
  if (CONFIG_CC310_${base})
    nrf_security_debug("Setting CC310_${base} to TRUE")
    set(CC310_${base} TRUE)
  endif()
endmacro()

#
# Internal macro which will enable the define in mbed TLS config file based on
# the corresponding Kconfig setting.
# The macro will use the value provided as second argument in the config header.
#
macro(kconfig_mbedtls_config_val base val)
  if (CONFIG_${base})
    nrf_security_debug("Setting ${base} to ${val}")
    set(${base} ${val})
  endif()
endmacro()

#
# Internal macro which will enable the define in mbed TLS config file based on
# the correspong Kconfig setting (for _ALT flags)
# Any additional arguments given are dependencies to to first argument, such
# that those arguments will be set to TRUE if first argument is TRUE.
#
macro(kconfig_mbedtls_config_alt base)
  # Note that we don't add CONFIG_ to ${base}_C as it is not used in mbed TLS
  # config file
  if ((${base}_C AND (CONFIG_CC310_BACKEND AND
      NOT CONFIG_MBEDTLS_VANILLA_BACKEND)) OR
      CONFIG_CC310_${base}_C OR
      CONFIG_GLUE_${base}_C)
    nrf_security_debug("ALT Setting ${base}_ALT to TRUE")
    set(${base}_ALT TRUE)
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
    if (NOT (CONFIG_${arg} OR
            CONFIG_CC310_${arg} OR
            CONFIG_VANILLA_${arg}))
      unset(${mbedtls_config})
      break()
    endif()
  endforeach()
endmacro()

#
# Internal macro to configure file if mbed TLS config is set
#
macro(configure_file_ifdef mbedtls_config)
  if (${mbedtls_config})
    nrf_security_debug("Configure file: ${ARGV1}")
    configure_file(${ARGV1} ${ARGV2} ${ARGV3})
  endif()
endmacro()

#
# Remove previously generated files from include folder
#
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/include/)

# Enabling core functionality
#
kconfig_mbedtls_config("MBEDTLS_AES_C")
kconfig_mbedtls_config("MBEDTLS_AES_ROM_TABLES")
kconfig_mbedtls_config("MBEDTLS_AES_FEWER_TABLES")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CBC")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CFB")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CTR")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_OFB")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_XTS")
kconfig_mbedtls_config("MBEDTLS_CMAC_C")
kconfig_mbedtls_config("MBEDTLS_CCM_C")
kconfig_mbedtls_config("MBEDTLS_CTR_DRBG_C")
kconfig_mbedtls_config("MBEDTLS_DHM_C")
kconfig_mbedtls_config("MBEDTLS_HMAC_DRBG_C")
kconfig_mbedtls_config("MBEDTLS_SHA1_C")
kconfig_mbedtls_config("MBEDTLS_SHA256_C")
kconfig_mbedtls_config("MBEDTLS_SHA512_C")
kconfig_mbedtls_config("MBEDTLS_ECP_C")
kconfig_mbedtls_config("MBEDTLS_ECDH_C")
kconfig_mbedtls_config("MBEDTLS_ECDSA_C")
kconfig_mbedtls_config("MBEDTLS_ECJPAKE_C")
kconfig_mbedtls_config("MBEDTLS_GCM_C")
kconfig_mbedtls_config("MBEDTLS_RSA_C")
kconfig_mbedtls_config("MBEDTLS_PKCS1_V15")
kconfig_mbedtls_config("MBEDTLS_PKCS1_V21")
kconfig_mbedtls_config("MBEDTLS_MD_C")
kconfig_mbedtls_config("MBEDTLS_CHACHA20_C")
kconfig_mbedtls_config("MBEDTLS_POLY1305_C")
kconfig_mbedtls_config("MBEDTLS_CHACHAPOLY_C")
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


#
# Nordic added defines for features not supported in HW
# and might not be available (due to configuration)
#
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_ECB_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CBC_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CTR_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_AES_256_CCM_C")
kconfig_mbedtls_config("MBEDTLS_AES_256_CMAC_C")

#
# Advanced configuration setting for mbed TLS
#
kconfig_mbedtls_config_val("MBEDTLS_MPI_WINDOW_SIZE"       "${CONFIG_MBEDTLS_MPI_WINDOW_SIZE}")
kconfig_mbedtls_config_val("MBEDTLS_MPI_MAX_SIZE"          "${CONFIG_MBEDTLS_MPI_MAX_SIZE}")
kconfig_mbedtls_config_val("MBEDTLS_ECP_MAX_BITS"          "${CONFIG_MBEDTLS_ECP_MAX_BITS}")
kconfig_mbedtls_config_val("MBEDTLS_ECP_WINDOW_SIZE"       "${CONFIG_MBEDTLS_ECP_WINDOW_SIZE}")
kconfig_mbedtls_config_val("MBEDTLS_ECP_FIXED_POINT_OPTIM" "1")
kconfig_mbedtls_config_val("MBEDTLS_SSL_MAX_CONTENT_LEN"   "${CONFIG_MBEDTLS_SSL_MAX_CONTENT_LEN}")
kconfig_mbedtls_config_val("MBEDTLS_SSL_CIPHERSUITES"      "${CONFIG_MBEDTLS_SSL_CIPHERSUITES}")
kconfig_mbedtls_config("MBEDTLS_SHA256_SMALLER")

#
# CC310 flags for threading and platform zeroize
# Note: These must be set in noglue an and regular configuration for correct
# context sizes
#
if (CONFIG_CC310_BACKEND)
  set(MBEDTLS_PLATFORM_ZEROIZE_ALT TRUE)
  set(MBEDTLS_THREADING_C TRUE)
  set(MBEDTLS_THREADING_ALT TRUE)
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
  set(MBEDTLS_PLATFORM_EXIT_ALT TRUE)
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
# Enabling ALT functionality
# This includes GLUE, CC310 or other ALT backends
#
kconfig_mbedtls_config_alt("MBEDTLS_AES")
kconfig_mbedtls_config_alt("MBEDTLS_CCM")
kconfig_mbedtls_config_alt("MBEDTLS_CHACHA20")
kconfig_mbedtls_config_alt("MBEDTLS_CHACHAPOLY")
kconfig_mbedtls_config_alt("MBEDTLS_CMAC")
kconfig_mbedtls_config_alt("MBEDTLS_DHM")
kconfig_mbedtls_config_alt("MBEDTLS_ECDH")
kconfig_mbedtls_config_alt("MBEDTLS_ECDSA")
kconfig_mbedtls_config_alt("MBEDTLS_ECP")
kconfig_mbedtls_config_alt("MBEDTLS_POLY1305")
kconfig_mbedtls_config_alt("MBEDTLS_RSA")
kconfig_mbedtls_config_alt("MBEDTLS_SHA1")
kconfig_mbedtls_config_alt("MBEDTLS_SHA256")

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


if (CONFIG_CC310_BACKEND)
  set(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT TRUE)
endif()

if (CC310_MBEDTLS_ECDH_C)
  set(MBEDTLS_ECDH_GEN_PUBLIC_ALT TRUE)
  set(MBEDTLS_ECDH_COMPUTE_SHARED_ALT TRUE)
endif()

if (CC310_MBEDTLS_ECDSA_C)
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

set(generated_include_path
  "${CMAKE_CURRENT_BINARY_DIR}/include/mbedtls_generated/"
)

#
# Copy the correct alt include files to use for libraries
# from CC310 folder if in use
#
configure_file_ifdef(CC310_MBEDTLS_AES_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/aes_alt.h
  ${generated_include_path}/aes_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_CCM_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/ccm_alt.h
  ${generated_include_path}/ccm_alt.h COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_CMAC_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/cmac_alt.h
  ${generated_include_path}/cmac_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_DHM_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/dhm_alt.h
  ${generated_include_path}/dhm_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_RSA_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/rsa_alt.h
  ${generated_include_path}/rsa_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_SHA1_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/sha1_alt.h
  ${generated_include_path}/sha1_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_SHA256_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/sha256_alt.h
  ${generated_include_path}/sha256_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_ECP_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/ecp_alt.h
  ${generated_include_path}/ecp_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_CHACHA20_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/chacha20_alt.h
  ${generated_include_path}/chacha20_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_POLY1305_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/poly1305_alt.h
  ${generated_include_path}/poly1305_alt.h
  COPYONLY)

configure_file_ifdef(CC310_MBEDTLS_CHACHAPOLY_C
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/chachapoly_alt.h
  ${generated_include_path}/chachapoly_alt.h
  COPYONLY)

configure_file_ifdef(CONFIG_CC310_BACKEND
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/platform_alt.h
  ${generated_include_path}/platform_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_CC310_BACKEND
  ${NRF_CC310_MBEDCRYPTO_INCLUDE_PATH}/threading_alt.h
  ${generated_include_path}/threading_alt.h
  COPYONLY
)

#
# Copy the correct glue include files to use for libraries
# (potentially overwriting other copied backend files)
#
set(mbedcrypto_glue_include_path
  "${CMAKE_CURRENT_LIST_DIR}/../include/mbedcrypto_glue/mbedtls"
)
configure_file_ifdef(CONFIG_GLUE_MBEDTLS_AES_C
  ${mbedcrypto_glue_include_path}/aes_alt.h
  ${generated_include_path}/aes_alt.h
  COPYONLY
)
configure_file_ifdef(CONFIG_GLUE_MBEDTLS_CCM_C
  ${mbedcrypto_glue_include_path}/ccm_alt.h
  ${generated_include_path}/ccm_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_CMAC_C
  ${mbedcrypto_glue_include_path}/cmac_alt.h
  ${generated_include_path}/cmac_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_DHM_C
  ${mbedcrypto_glue_include_path}/dhm_alt.h
  ${generated_include_path}/dhm_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_RSA_C
  ${mbedcrypto_glue_include_path}/rsa_alt.h
  ${generated_include_path}/rsa_alt.h
  COPYONLY
)

#
# Include for generated mbed TLS config file
#
set(config_include
  ${CMAKE_CURRENT_BINARY_DIR}/include
)

#
# Includes for all generated header files
#
set(generated_includes
  ${config_include}
  ${generated_include_path}
)

zephyr_include_directories(${generated_includes})

#
# Generate the mbed TLS config files
#
if (CONFIG_GENERATE_MBEDTLS_CFG_FILE)
configure_file(${CMAKE_CURRENT_LIST_DIR}/../configs/nrf-config.h.template
               ${config_include}/${CONFIG_MBEDTLS_CFG_FILE})
endif()
