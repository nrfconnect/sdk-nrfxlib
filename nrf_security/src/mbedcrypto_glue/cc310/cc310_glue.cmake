#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
nrf_security_debug("######### Creating cc310 glue library #########")

zephyr_library_named(mbedcrypto_glue_cc3xx)

#
# Adding cc310 backend glue files
#
zephyr_library_sources_ifdef(CONFIG_GLUE_CC3XX_MBEDTLS_AES_C
  ${CMAKE_CURRENT_LIST_DIR}/aes_cc310.c
)
zephyr_library_sources_ifdef(CONFIG_GLUE_CC3XX_MBEDTLS_CCM_C
  ${CMAKE_CURRENT_LIST_DIR}/ccm_cc310.c
)
zephyr_library_sources_ifdef(CONFIG_GLUE_CC3XX_MBEDTLS_CMAC_C
  ${CMAKE_CURRENT_LIST_DIR}/cmac_cc310.c
)
zephyr_library_sources_ifdef(CONFIG_GLUE_CC3XX_MBEDTLS_DHM_C
  ${CMAKE_CURRENT_LIST_DIR}/dhm_cc310.c
)

zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)

zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=cc3xx)
zephyr_library_link_libraries(mbedtls_common_glue)
nrf_security_debug_list_target_files(mbedcrypto_glue_cc3xx)

add_dependencies(mbedcrypto_glue_cc3xx mbedcrypto_cc3xx_renamed)

#
# Rename the external symbols as referenced through the glue files
# The APIs will match cc3xx_mbedtls_<xxxx> after this.
#
add_custom_command(
  TARGET mbedcrypto_glue_cc3xx
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY}
          --redefine-syms
          ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_cc3xx.txt
          $<TARGET_FILE:mbedcrypto_glue_cc3xx>
  DEPENDS mbedcrypto_cc3xx_renamed
)
