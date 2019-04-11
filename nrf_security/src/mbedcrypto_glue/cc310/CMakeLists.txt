#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
if(CONFIG_CC310_BACKEND)
  set(cc310_backend_glue_src
      ${CMAKE_CURRENT_LIST_DIR}/aes_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/ccm_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/cmac_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/dhm_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdh_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/ecdsa_cc310.c
      ${CMAKE_CURRENT_LIST_DIR}/rsa_cc310.c
  )
  
  set_source_files_properties(${cc310_backend_glue_src}
                              PROPERTIES COMPILE_DEFINITIONS MBEDTLS_BACKEND_PREFIX=cc310)

  add_library(mbedcrypto_glue_cc310 STATIC ${cc310_backend_glue_src})
  target_include_directories(mbedcrypto_glue_cc310 PRIVATE
                             $<TARGET_PROPERTY:mbedcrypto_glue,INCLUDE_DIRECTORIES>)
  target_compile_definitions(mbedcrypto_glue_cc310 PRIVATE
                             $<TARGET_PROPERTY:mbedcrypto_glue,COMPILE_DEFINITIONS>)
  add_custom_command(
    TARGET mbedcrypto_glue_cc310
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} 
            --redefine-syms
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_cc310.txt
            $<TARGET_FILE:mbedcrypto_glue_cc310>
  )
  target_link_libraries(mbedcrypto_glue PRIVATE mbedcrypto_glue_cc310)
endif()
