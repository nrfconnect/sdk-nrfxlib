#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# This file contain a function which allow symbols in a library that implements
# the arm mbed TLS API to be rename into unique names.
# This allows nRF Connect SDK Security to support multiple crypto backends
# that share the same front-end API.
# If two or more backends are enabled for a given function, each of them will
# be renamed to be unique and a glue layer will be used as new front-end.
#
macro(keep_config_test_glue mbedtls_config backend_name)
  if(NOT CONFIG_GLUE_${mbedtls_config}
     AND CONFIG_${backend_name}_${mbedtls_config})
    set(KEEP_${mbedtls_config} "#")
  endif()
endmacro()

macro(keep_config_test mbedtls_config backend_name)
  if(CONFIG_${backend_name}_${mbedtls_config})
    set(KEEP_${mbedtls_config} "#")
  endif()
endmacro()

function(library_redefine_symbols backend template)
  string(TOUPPER "${backend}" BACKEND_NAME)
  keep_config_test_glue("MBEDTLS_AES_C" ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CIPHER_MODE_XTS" ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CIPHER_MODE_CBC" ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CIPHER_MODE_CFB" ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CIPHER_MODE_OFB" ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CIPHER_MODE_CTR" ${BACKEND_NAME})
  keep_config_test("MBEDTLS_SHA1_C"               ${BACKEND_NAME})
  keep_config_test("MBEDTLS_SHA256_C"             ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_RSA_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_DHM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CCM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CMAC_C"          ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_ECDH_C"          ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_ECDSA_C"         ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_GCM_C"           ${BACKEND_NAME})
  keep_config_test("MBEDTLS_PLATFORM"             ${BACKEND_NAME})
  
  string(TOLOWER "${backend}" MBEDTLS_BACKEND_PREFIX)
  configure_file(${template}
                 symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt)
  
  set(BACKEND_RENAMED_LIBARY lib${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend.a)
  add_custom_command(
    OUTPUT ${BACKEND_RENAMED_LIBARY}
    COMMAND ${CMAKE_OBJCOPY}
            --redefine-syms
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt
            $<TARGET_FILE:${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}>
            ${BACKEND_RENAMED_LIBARY}
    DEPENDS ${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt
  )
  add_custom_target(${IMAGE}${MBEDTLS_BACKEND_PREFIX}_backend_target
                    DEPENDS ${BACKEND_RENAMED_LIBARY}
  )
  add_library(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend STATIC IMPORTED GLOBAL)
  add_dependencies(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend
                   ${IMAGE}${MBEDTLS_BACKEND_PREFIX}_backend_target)
  set_target_properties(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend
                        PROPERTIES IMPORTED_LOCATION
                        "${CMAKE_CURRENT_BINARY_DIR}/${BACKEND_RENAMED_LIBARY}")

  zephyr_append_cmake_library(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend)
endfunction()

