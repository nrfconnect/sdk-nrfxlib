#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
# The purpose of this file is to set specific MBEDTLS config file defines based
# on the Kconfig settings.
# When all values has been processed, the mbedtls config file will be generated.

# Internal macro which will enable the define in mbed TLS config file based on
# the correspong Kconfig setting.
# Any additional arguments given are dependencies to to first argument, such
# that those arguments will be set to TRUE if first argument is TRUE.
macro(kconfig_mbedtls_config mbedtls_config_define)
  if (CONFIG_${mbedtls_config_define} OR
      CONFIG_CC310_${mbedtls_config_define} OR
      CONFIG_VANILLA_${mbedtls_config_define})
    set(${mbedtls_config_define} TRUE)
    foreach(arg ${ARGN})
      set(${arg} TRUE)
    endforeach()
  endif()
endmacro()

# Internal macro which will enable the define dependencies in mbed TLS config
# file if the any backend has enabled the setting in Kconfig specified by first
# argument.
macro(mbedtls_config_define_depends mbedtls_config)
  set(${mbedtls_config} TRUE)
  foreach(arg ${ARGN})
  if (NOT (CONFIG_${arg} OR
           CONFIG_CC310_${arg} OR
           CONFIG_VANILLA_${arg}))
    unset(${mbedtls_config})
  endif()
  endforeach()
endmacro()

# If vanilla is not enabled, then select ALT impl to ensure rest of dependent
# functionality is still compiled.
if (NOT CONFIG_MBEDTLS_VANILLA_BACKEND)
  set(MBEDTLS_AES_ALT TRUE)
  set(MBEDTLS_CCM_ALT TRUE)
  set(MBEDTLS_CMAC_ALT TRUE)
  set(MBEDTLS_DHM_ALT TRUE)
  set(MBEDTLS_SHA1_ALT TRUE)
  set(MBEDTLS_SHA256_ALT TRUE)
  set(MBEDTLS_ECDH_GEN_PUBLIC_ALT TRUE)
  set(MBEDTLS_ECDH_COMPUTE_SHARED_ALT TRUE)
  set(MBEDTLS_ECDSA_VERIFY_ALT TRUE)
  set(MBEDTLS_ECDSA_SIGN_ALT TRUE)
  set(MBEDTLS_ECDSA_GENKEY_ALT TRUE)
  set(MBEDTLS_GCM_ALT TRUE)
  set(MBEDTLS_RSA_ALT TRUE)
endif()

if (CONFIG_CC310_BACKEND)
  set(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT TRUE)
endif()

if (NOT CONFIG_NEWLIB_LIBC)
  set(MBEDTLS_PLATFORM_NO_STD_FUNCTIONS TRUE)
  set(MBEDTLS_PLATFORM_EXIT_ALT TRUE)
  set(MBEDTLS_PLATFORM_FPRINTF_ALT TRUE)
  set(MBEDTLS_PLATFORM_PRINTF_ALT TRUE)
  set(MBEDTLS_PLATFORM_SNPRINTF_ALT TRUE)
endif ()

if (CONFIG_MBED_TLS_LIBRARY)
  set(MBEDTLS_SSL_PROTO_DTLS TRUE)
  set(MBEDTLS_SSL_DTLS_HELLO_VERIFY TRUE)
  set(MBEDTLS_SSL_COOKIE_C TRUE)
endif ()

kconfig_mbedtls_config("MBEDTLS_AES_C")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CBC")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CFB")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_CTR")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_OFB")
kconfig_mbedtls_config("MBEDTLS_CIPHER_MODE_XTS")
kconfig_mbedtls_config("MBEDTLS_CMAC_C")
kconfig_mbedtls_config("MBEDTLS_CCM_C")
kconfig_mbedtls_config("MBEDTLS_ECDH_C")
kconfig_mbedtls_config("MBEDTLS_ECDSA_C")
kconfig_mbedtls_config("MBEDTLS_GCM_C")

kconfig_mbedtls_config("MBEDTLS_SHA1_C")
kconfig_mbedtls_config("MBEDTLS_SHA256_C")

kconfig_mbedtls_config("MBEDTLS_DHM_C"
  "MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED"
)

kconfig_mbedtls_config("MBEDTLS_CHACHA20_C"
  "MBEDTLS_CHACHAPOLY_C"
  "MBEDTLS_POLY1305_C"
)

kconfig_mbedtls_config("MBEDTLS_RSA_C"
  "MBEDTLS_KEY_EXCHANGE_RSA_PSK_ENABLED"
  "MBEDTLS_KEY_EXCHANGE_RSA_ENABLED"
  "MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED"
  "MBEDTLS_PKCS1_V15"
  "MBEDTLS_PKCS1_V21"
  "MBEDTLS_PK_C"
  "MBEDTLS_PK_PARSE_C" # Dependent upon MBEDTLS_PK_C
  "MBEDTLS_PK_WRITE_C"
  "MBEDTLS_X509_USE_C" # Dependent upon MBEDTLS_PK_PARSE_C
  "MBEDTLS_X509_CRT_PARSE_C" # Dependent upon MBEDTLS_X509_USE_C
  "MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED" # Dependent upon MBEDTLS_X509_CRT_PARSE_C
  "MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED"
  "MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED"
)

mbedtls_config_define_depends("MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED"
  "MBEDTLS_DHM_C"
  "MBEDTLS_RSA_C"
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/../configs/nrf-config.h.template
               ${CMAKE_CURRENT_BINARY_DIR}/include/${CONFIG_MBEDTLS_CFG_FILE})
#set(MBEDTLS_CONFIGURATION \"${CMAKE_CURRENT_BINARY_DIR}/include/nrf-config.h\")
