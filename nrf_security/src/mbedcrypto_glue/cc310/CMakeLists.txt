#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
if(CONFIG_CC310_BACKEND)
  zephyr_library_named(mbedcrypto_glue_cc310)
  zephyr_library_sources(
      ${CMAKE_CURRENT_LIST_DIR}/aes_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/ccm_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/cmac_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/dhm_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdh_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdsa_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/rsa_cc310.c
  )
  zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=cc310)

  zephyr_library_include_directories($<TARGET_PROPERTY:${IMAGE}mbedcrypto_glue,INCLUDE_DIRECTORIES>)
  zephyr_library_compile_definitions($<TARGET_PROPERTY:${IMAGE}mbedcrypto_glue,COMPILE_DEFINITIONS>)
  add_custom_command(
    TARGET ${IMAGE}mbedcrypto_glue_cc310
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} 
            --redefine-syms
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_cc310.txt
            $<TARGET_FILE:${IMAGE}mbedcrypto_glue_cc310>
  )
endif()
