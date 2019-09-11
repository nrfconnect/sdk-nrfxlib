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
  if(NOT CONFIG_GLUE_${mbedtls_config} AND ${backend_name}_${mbedtls_config})
    set(KEEP_${mbedtls_config} "#")
    kconfig_mbedtls_debug("${backend_name}: Not renamed ${mbedtls_config}")
  else()
    kconfig_mbedtls_debug("${backend_name}: Renaming ${mbedtls_config}")
  endif()
endmacro()

#
# This macro comments out the instruction to rename a symbol if two dependent
# configurations are enabled, but the feature (as a whole) is not glued
#
macro(keep_config_test_glue_depends mbedtls_config mbedtls_config_inner backend_name)
  if((NOT CONFIG_GLUE_${mbedtls_config}) AND
      ${backend_name}_${mbedtls_config} AND
      CONFIG_${mbedtls_config_inner})
    kconfig_mbedtls_debug("${backend_name}: Has ${backend_name}_${mbedtls_config}: Not renamed ${mbedtls_config_inner}")
    set(KEEP_${mbedtls_config_inner} "#")
  else()
    kconfig_mbedtls_debug("${backend_name}: Renaming ${mbedtls_config_inner}")
  endif()
endmacro()

#
# This macro will create a string to remove objects
# if the given configuration is not enabled in the backend
#
macro(remove_objects mbedtls_config backend_name object_list object_name)
  if (NOT ${backend_name}_${mbedtls_config})
    kconfig_mbedtls_debug("${backend_name}: Removing object ${object_name}")
    set(${object_list} ${${object_list}} ${object_name})
  endif()
endmacro()

function(symbol_rename_func backend rename_template is_stripped)
  string(TOUPPER "${backend}" BACKEND_NAME)
  #
  # Functionality that can be glued
  #
  keep_config_test_glue("MBEDTLS_AES_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CCM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_DHM_C"           ${BACKEND_NAME})

  keep_config_test_glue("MBEDTLS_CMAC_C"          ${BACKEND_NAME})
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


  #
  # Files that are either in one of the backends
  # These will be removed with a call to "ar"
  #
  if (${is_stripped})
    remove_objects("MBEDTLS_CMAC_C"       ${BACKEND_NAME} remove_line "cmac_alt.c.obj")
    remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME} remove_line "ecp.c.obj")
    remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME} remove_line "ecp_common.c.obj")
    remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME} remove_line "ecp_curves.c.obj")
    remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME} remove_line "ecp_alt_cc310.c.obj")
    remove_objects("MBEDTLS_ECP_C"        ${BACKEND_NAME} remove_line "ecp_curved_alt.c.obj")
    remove_objects("MBEDTLS_ECJPAKE_C"    ${BACKEND_NAME} remove_line "ecjpake.c.obj")
    remove_objects("MBEDTLS_CHACHA20_C"   ${BACKEND_NAME} remove_line "chacha20.c.obj")
    remove_objects("MBEDTLS_CHACHA20_C"   ${BACKEND_NAME} remove_line "chacha20_alt.c.obj")
    remove_objects("MBEDTLS_CHACHAPOLY_C" ${BACKEND_NAME} remove_line "chachapoly.c.obj")
    remove_objects("MBEDTLS_CHACHAPOLY_C" ${BACKEND_NAME} remove_line "chachapoly_alt.c.obj")
    remove_objects("MBEDTLS_POLY1305_C"   ${BACKEND_NAME} remove_line "poly1305_alt.c.obj")
    remove_objects("MBEDTLS_POLY1305_C"   ${BACKEND_NAME} remove_line "poly1305.c.obj")
    remove_objects("MBEDTLS_POLY1305_C"   ${BACKEND_NAME} remove_line "poly.c.obj")
    remove_objects("MBEDTLS_SHA1_C"       ${BACKEND_NAME} remove_line "sha1.c.obj")
    remove_objects("MBEDTLS_SHA1_C"       ${BACKEND_NAME} remove_line "sha1_alt.c.obj")
    remove_objects("MBEDTLS_SHA256_C"     ${BACKEND_NAME} remove_line "sha256.c.obj")
    remove_objects("MBEDTLS_SHA256_C"     ${BACKEND_NAME} remove_line "sha256_alt.c.obj")
  endif()

  string(TOLOWER "${backend}" MBEDTLS_BACKEND_PREFIX)
  configure_file(${rename_template}
                 symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt)

  set(redefine_line "--redefine-syms;${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt")

  set(BACKEND_RENAMED_LIBRARY lib${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend.a)

  if (remove_line)
    set(remove_object_command ${CMAKE_AR} d ${BACKEND_RENAMED_LIBRARY} ${remove_line})
    kconfig_mbedtls_debug("Objects stripped from ${BACKEND_RENAMED_LIBRARY}: ${remove_line}")
  endif()

  add_custom_command(
    OUTPUT  ${BACKEND_RENAMED_LIBRARY}
    COMMAND ${CMAKE_OBJCOPY} ${redefine_line}
            $<TARGET_FILE:${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}>
            ${BACKEND_RENAMED_LIBRARY}
    COMMAND ${remove_object_command}
    DEPENDS ${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${MBEDTLS_BACKEND_PREFIX}.txt
  )
  add_custom_target(${IMAGE}${MBEDTLS_BACKEND_PREFIX}_backend_target
                    DEPENDS ${BACKEND_RENAMED_LIBRARY}
  )
  add_library(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend STATIC IMPORTED GLOBAL)
  add_dependencies(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend
                   ${IMAGE}${MBEDTLS_BACKEND_PREFIX}_backend_target)
  set_target_properties(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend
                        PROPERTIES IMPORTED_LOCATION
                        "${CMAKE_CURRENT_BINARY_DIR}/${BACKEND_RENAMED_LIBRARY}")

  zephyr_append_cmake_library(${IMAGE}mbedcrypto_${MBEDTLS_BACKEND_PREFIX}_backend)
endfunction()
