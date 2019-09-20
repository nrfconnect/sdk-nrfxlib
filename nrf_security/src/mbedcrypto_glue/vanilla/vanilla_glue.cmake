# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if (CONFIG_GLUE_MBEDTLS_AES_C AND CONFIG_VANILLA_MBEDTLS_AES_C)
  set(GLUE_VANILLA_MBEDTLS_AES_C TRUE)
  nrf_security_debug("Vanilla backend glue: AES")
endif()

if (CONFIG_GLUE_MBEDTLS_CCM_C AND CONFIG_VANILLA_MBEDTLS_CCM_C)
  set(GLUE_VANILLA_MBEDTLS_CCM_C TRUE)
  nrf_security_debug("Vanilla backend glue: CCM")
endif()

if (CONFIG_GLUE_MBEDTLS_DHM_C AND CONFIG_VANILLA_MBEDTLS_DHM_C)
  set(GLUE_VANILLA_MBEDTLS_DHM_C TRUE)
  nrf_security_debug("Vanilla backend glue: DHM")
endif()

nrf_security_debug("######### Creating vanilla noglue library #########")

#
# Create mbedcrypto vanilla (noglue) library
# Doesn't have ALT flags set (to ensure vanilla context sizes)
# This implements features that are to be glued but with the original contexts
# This must use be compiled with its own config-file (nrf-config-noglue.h)
# This library must not be linked with directly
#
add_library(${IMAGE}mbedcrypto_vanilla)

#
# Adding all standard compile/linker options (e.g. float ABI)
#
target_compile_options(${IMAGE}mbedcrypto_vanilla PRIVATE ${TOOLCHAIN_C_FLAGS})
target_ld_options(${IMAGE}mbedcrypto_vanilla PRIVATE ${TOOLCHAIN_LD_FLAGS})

#
# Adding original mbed TLS files
#
target_sources_ifdef(GLUE_VANILLA_MBEDTLS_AES_C
  ${IMAGE}mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/aes.c
)
target_sources_ifdef(GLUE_VANILLA_MBEDTLS_CCM_C
  ${IMAGE}mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ccm.c
)

target_sources_ifdef(GLUE_VANILLA_MBEDTLS_DHM_C
${IMAGE}mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/dhm.c
)

target_sources(${IMAGE}mbedcrypto_vanilla PRIVATE
  ${ZEPHYR_BASE}/misc/empty_file.c
)

target_include_directories(${IMAGE}mbedcrypto_vanilla PRIVATE
  ${common_includes}
  ${config_include}
)

target_compile_definitions(${IMAGE}mbedcrypto_vanilla PRIVATE
  -DMBEDTLS_BACKEND_PREFIX=vanilla
  -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
)

nrf_security_debug_list_target_files(${IMAGE}mbedcrypto_vanilla)

nrf_security_debug("######### Creating vanilla glue library #########")

#
# Create the vanilla glue library
#
zephyr_library_named(mbedcrypto_glue_vanilla)

#
# Adding vanilla backend glue files
#
zephyr_library_sources_ifdef(GLUE_VANILLA_MBEDTLS_AES_C
  ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c
)

zephyr_library_sources_ifdef(GLUE_VANILLA_MBEDTLS_CCM_C
  ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c
)

zephyr_library_sources_ifdef(GLUE_VANILLA_MBEDTLS_DHM_C
  ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c
)

zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)
zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=vanilla)
zephyr_library_link_libraries(${IMAGE}mbedtls_common_glue)
zephyr_library_link_libraries(${IMAGE}mbedcrypto_vanilla)
nrf_security_debug_list_target_files(${IMAGE}mbedcrypto_glue_vanilla)

#
# Rename vanilla symbols in for glue and vanilla library
#
foreach(mbedcrypto_target mbedcrypto_glue_vanilla mbedcrypto_vanilla)
  add_custom_command(
    TARGET ${IMAGE}${mbedcrypto_target}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
            --redefine-syms
            ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_vanilla.txt
            $<TARGET_FILE:${IMAGE}${mbedcrypto_target}>
  )
endforeach()


