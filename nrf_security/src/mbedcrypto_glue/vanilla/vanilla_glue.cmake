# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

nrf_security_debug("######### Creating vanilla glue library #########")

#
# Create the vanilla glue library
#
zephyr_library_named(mbedcrypto_glue_vanilla)

#
# Adding vanilla backend glue files
#
zephyr_library_sources_ifdef(CONFIG_VANILLA_GLUE_MBEDTLS_AES_C
  ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c
)
zephyr_library_sources_ifdef(CONFIG_VANILLA_GLUE_MBEDTLS_CCM_C
  ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c
)
zephyr_library_sources_ifdef(CONFIG_VANILLA_GLUE_MBEDTLS_CMAC_C
  ${CMAKE_CURRENT_LIST_DIR}/cmac_vanilla.c
)
zephyr_library_sources_ifdef(CONFIG_VANILLA_GLUE_MBEDTLS_DHM_C
  ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c
)

zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)
zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=vanilla)
zephyr_library_link_libraries(mbedtls_common_glue)

add_dependencies(mbedcrypto_glue_vanilla mbedcrypto_vanilla_renamed)

nrf_security_debug_list_target_files(mbedcrypto_glue_vanilla)

add_custom_command(
  TARGET mbedcrypto_glue_vanilla
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY}
          --redefine-syms
          ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_vanilla.txt
          $<TARGET_FILE:mbedcrypto_glue_vanilla>
  DEPENDS mbedcrypto_vanilla_renamed
)
