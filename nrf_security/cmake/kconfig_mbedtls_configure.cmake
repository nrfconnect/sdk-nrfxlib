#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
# The purpose of this file is to set specific MBEDTLS config file defines based
# on the Kconfig settings.
# When all values has been processed, the mbedtls config file will be generated.


#
# Internal macro that shows debug output in for nrf_security related
# configurations when Cmake is run with -DDEBUG_KCONFIG_MBEDTLS=1
#
macro(kconfig_mbedtls_debug)
  if(DEBUG_KCONFIG_MBEDTLS)
    message("${ARGN}")
  endif()
endmacro()

#
# Internal macro that shows source files in a mbed TLS library (lib_name)
# when CMake is run with -DDEBUG_KCONFIG_MBEDTLS=1
# Note that inspecting the zephyr.map file is safer
#
macro(kconfig_mbedtls_debug_list_target_files lib_name)
  if(DEBUG_KCONFIG_MBEDTLS)
    message("--- Sources in ${lib_name} ---")
    get_target_property(file_list ${lib_name} SOURCES)
    foreach(line IN LISTS file_list)
      message("${line}")
    endforeach()
    message("--- End sources in ${lib_name} ---")
  endif()
endmacro()

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
    kconfig_mbedtls_debug("Setting ${base} to TRUE")
    set(${base} TRUE)
  endif()
  if ((CONFIG_${base} AND NOT CONFIG_CC310_BACKEND) OR
      (CONFIG_VANILLA_${base} AND NOT CONFIG_CC310_${base}))
    kconfig_mbedtls_debug("Setting VANILLA_ONLY_${base} to TRUE")
    set(VANILLA_ONLY_${base} TRUE)
  endif()
  if (CONFIG_${base} AND NOT CONFIG_MBEDTLS_VANILLA_BACKEND)
    kconfig_mbedtls_debug("Setting CC310_${base} to TRUE")
    set(CC310_${base} TRUE)
  endif()
  if (CONFIG_CC310_${base})
    kconfig_mbedtls_debug("Setting CC310_${base} to TRUE")
    set(CC310_${base} TRUE)
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
    kconfig_mbedtls_debug("ALT Setting ${base}_ALT to TRUE")
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
    kconfig_mbedtls_debug("Configure file: ${ARGV1}")
    configure_file(${ARGV1} ${ARGV2} ${ARGV3})
  endif()
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
