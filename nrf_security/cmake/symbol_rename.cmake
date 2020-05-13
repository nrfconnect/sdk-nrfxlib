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

#
# This macro comments out the instruction to rename a symbol
# in case a configuration is enabled in the backend, but not glued
#
macro(keep_config_test_glue mbedtls_config backend_name)
  if(NOT CONFIG_GLUE_${mbedtls_config} AND CONFIG_${backend_name}_${mbedtls_config})
    set(KEEP_${mbedtls_config} "#")
    nrf_security_debug("${backend_name}: Not renamed ${mbedtls_config}")
  else()
    nrf_security_debug("${backend_name}: Renaming ${mbedtls_config}")
  endif()
endmacro()

#
# This macro comments out the instruction to rename a symbol if two dependent
# configurations are enabled, but the feature (as a whole) is not glued
#
macro(keep_config_test_glue_depends mbedtls_config mbedtls_config_inner backend_name)
  if((NOT CONFIG_GLUE_${mbedtls_config}) AND
      CONFIG_${backend_name}_${mbedtls_config} AND
      CONFIG_${mbedtls_config_inner})
    nrf_security_debug("${backend_name}: Has ${backend_name}_${mbedtls_config}: Not renamed ${mbedtls_config_inner}")
    set(KEEP_${mbedtls_config_inner} "#")
  else()
    nrf_security_debug("${backend_name}: Renaming ${mbedtls_config_inner}")
  endif()
endmacro()

#
# This function will create a symbol renaming script for any library
# named mbedcrypto_<backend> corresponding to the parameter "backend"
#
function(symbol_rename_func backend rename_template)
  nrf_security_debug("========== Running symbol_rename_func for ${backend} ==========")
  string(TOUPPER "${backend}" BACKEND_NAME)
  #
  # Functionality that can be glued
  #
  keep_config_test_glue("MBEDTLS_AES_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CCM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CMAC_C"          ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_DHM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_ECDH_C"          ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_ECDSA_C"         ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_RSA_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_DHM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_GCM_C"           ${BACKEND_NAME})

  #
  # Functionality that can be glued (dependent configuration)
  #
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_XTS" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_CBC" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_CFB" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_OFB" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_CTR" ${BACKEND_NAME})

  string(TOLOWER "${backend}" MBEDTLS_BACKEND_PREFIX)
  configure_file(${rename_template}
                 symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt)

  set(redefine_line "--redefine-syms;${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt")

  set(BACKEND_RENAMED_LIBRARY libmbedcrypto_${MBEDTLS_BACKEND_PREFIX}_renamed.a)

  add_custom_command(
    OUTPUT  ${BACKEND_RENAMED_LIBRARY}
    COMMAND ${CMAKE_OBJCOPY} ${redefine_line}
            $<TARGET_FILE:mbedcrypto_${MBEDTLS_BACKEND_PREFIX}>
            ${BACKEND_RENAMED_LIBRARY}
    DEPENDS mbedcrypto_${MBEDTLS_BACKEND_PREFIX}
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt
  )
  add_custom_target(${MBEDTLS_BACKEND_PREFIX}_renamed_target
                    DEPENDS ${BACKEND_RENAMED_LIBRARY}
  )
  add_library(mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_renamed STATIC IMPORTED GLOBAL)
  add_dependencies(mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_renamed
                   ${MBEDTLS_BACKEND_PREFIX}_renamed_target)
  set_target_properties(mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_renamed
                        PROPERTIES IMPORTED_LOCATION
                        "${CMAKE_CURRENT_BINARY_DIR}/${BACKEND_RENAMED_LIBRARY}")

  zephyr_append_cmake_library(mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_renamed)
endfunction()
