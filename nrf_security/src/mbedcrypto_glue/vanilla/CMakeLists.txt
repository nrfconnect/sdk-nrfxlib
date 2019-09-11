# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# Create vanilla noglue library
# Doesn't have ALT flags set (for correct context size)
# This implements features that are to be glued (but with context-sizes that are corresponding)
# This must use be compiled with its own config-file
# This library must not be linked with directly
#
append_with_prefix_ifdef(CONFIG_VANILLA_MBEDTLS_AES_C   src_vanilla
  ${ARM_MBEDTLS_ROOT}/library/ aes.c
)
append_with_prefix_ifdef(CONFIG_VANILLA_MBEDTLS_CCM_C   src_vanilla
  ${ARM_MBEDTLS_ROOT}/library/ ccm.c
)
append_with_prefix_ifdef(CONFIG_VANILLA_MBEDTLS_RSA_C   src_vanilla
  ${ARM_MBEDTLS_ROOT}/library/ rsa.c
)

add_library(${IMAGE}mbedcrypto_vanilla)
set(ZEPHYR_CURRENT_LIBRARY ${IMAGE}mbedcrypto_vanilla)

zephyr_library_sources(${src_vanilla})

zephyr_library_include_directories(${common_includes})
zephyr_library_include_directories(${config_include})
zephyr_library_include_directories(${ARM_MBEDTLS_ROOT}/include)
zephyr_library_include_directories(${ARM_MBEDTLS_ROOT}/include/mbedtls)
zephyr_library_compile_definitions(-DMBEDTLS_BACKEND_PREFIX=vanilla)
zephyr_library_compile_definitions(-DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h")
zephyr_library_link_libraries(${IMAGE}zephyr_interface)

add_custom_command(
  TARGET ${IMAGE}mbedcrypto_vanilla
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY}
          --redefine-syms
          ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_vanilla.txt
          $<TARGET_FILE:${IMAGE}mbedcrypto_vanilla>
)

#
# Create the vanilla glue library
#
zephyr_library_named(mbedcrypto_glue_vanilla)
zephyr_library_sources_ifdef(CONFIG_VANILLA_MBEDTLS_AES_C   ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c)
zephyr_library_sources_ifdef(CONFIG_VANILLA_MBEDTLS_CCM_C   ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c)
zephyr_library_sources_ifdef(CONFIG_VANILLA_MBEDTLS_DHM_C   ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c)
zephyr_library_sources_ifdef(CONFIG_VANILLA_MBEDTLS_RSA_C   ${CMAKE_CURRENT_LIST_DIR}/rsa_vanilla.c)

zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=vanilla)

zephyr_library_include_directories($<TARGET_PROPERTY:${IMAGE}mbedcrypto_glue,INCLUDE_DIRECTORIES>)
zephyr_library_compile_definitions($<TARGET_PROPERTY:${IMAGE}mbedcrypto_glue,COMPILE_DEFINITIONS>)
zephyr_library_link_libraries(${IMAGE}mbedcrypto_vanilla)
add_custom_command(
  TARGET ${IMAGE}mbedcrypto_glue_vanilla
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY}
          --redefine-syms
          ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_vanilla.txt
          $<TARGET_FILE:${IMAGE}mbedcrypto_glue_vanilla>
)
