# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

nrf_security_debug("######### Creating oberon glue library #########")

#
# Create the Oberon glue library
#
zephyr_library_named(mbedcrypto_glue_oberon)

#
# Adding Oberon backend glue files
#
zephyr_library_sources_ifdef(CONFIG_OBERON_GLUE_MBEDTLS_AES_C
  ${CMAKE_CURRENT_LIST_DIR}/aes_oberon.c
)

zephyr_library_sources_ifdef(CONFIG_OBERON_GLUE_MBEDTLS_CCM_C
  ${CMAKE_CURRENT_LIST_DIR}/ccm_oberon.c
)

zephyr_library_sources_ifdef(CONFIG_OBERON_GLUE_MBEDTLS_CMAC_C
  ${CMAKE_CURRENT_LIST_DIR}/cmac_oberon.c
)

zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)
zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=oberon)
zephyr_library_link_libraries(mbedtls_common_glue)

add_dependencies(mbedcrypto_glue_oberon mbedcrypto_oberon_renamed)

nrf_security_debug_list_target_files(mbedcrypto_glue_oberon)

#
# Rename the external symbols as referenced through the glue files
# The APIs will match oberon_mbedtls_<xxxx> after this.
#

add_custom_command(
  TARGET mbedcrypto_glue_oberon
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY}
          --redefine-syms ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_oberon.txt
          $<TARGET_FILE:mbedcrypto_glue_oberon>
  DEPENDS mbedcrypto_oberon_renamed
)