# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
if (CONFIG_MBEDTLS_VANILLA_BACKEND)
  set(vanilla_backend_glue_src
      ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/cmac_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdh_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdsa_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/gcm_vanilla.c
      ${CMAKE_CURRENT_LIST_DIR}/rsa_vanilla.c
  )
  
  set_source_files_properties(${vanilla_backend_glue_src}
                              PROPERTIES COMPILE_DEFINITIONS MBEDTLS_BACKEND_PREFIX=vanilla)

  add_library(mbedcrypto_glue_vanilla STATIC ${vanilla_backend_glue_src})
  target_include_directories(mbedcrypto_glue_vanilla PRIVATE
                             $<TARGET_PROPERTY:mbedcrypto_glue,INCLUDE_DIRECTORIES>)
  target_compile_definitions(mbedcrypto_glue_vanilla PRIVATE
                             $<TARGET_PROPERTY:mbedcrypto_glue,COMPILE_DEFINITIONS>)
  add_custom_command(
    TARGET mbedcrypto_glue_vanilla
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} 
            --redefine-syms
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_vanilla.txt
            $<TARGET_FILE:mbedcrypto_glue_vanilla>
  )
  target_link_libraries(mbedcrypto_glue PRIVATE mbedcrypto_glue_vanilla)
endif()
