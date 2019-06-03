# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
if (CONFIG_MBEDTLS_VANILLA_BACKEND)
  zephyr_library_named(mbedcrypto_glue_vanilla)
  zephyr_library_sources(
      ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/cmac_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdh_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdsa_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/rsa_vanilla.c
  )
  
  zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=vanilla)

  zephyr_library_include_directories($<TARGET_PROPERTY:${IMAGE}mbedcrypto_glue,INCLUDE_DIRECTORIES>)
  zephyr_library_compile_definitions($<TARGET_PROPERTY:${IMAGE}mbedcrypto_glue,COMPILE_DEFINITIONS>)
  add_custom_command(
    TARGET ${IMAGE}mbedcrypto_glue_vanilla
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} 
            --redefine-syms
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_vanilla.txt
            $<TARGET_FILE:${IMAGE}mbedcrypto_glue_vanilla>
  )
endif()
