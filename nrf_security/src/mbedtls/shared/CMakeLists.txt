#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
# The purpose of this file is to provice a set of debug commands to nrf security
# module.

nrf_security_debug("######### Creating shared noglue library #########")

#
# Add vanilla as define if non-cc310 feature is enabled (e.g. single backend)
#
if (CONFIG_MBEDTLS_CHACHA20_C AND NOT CONFIG_CC310_MBEDTLS_CHACHA20_C)
    set(CONFIG_VANILLA_MBEDTLS_CHACHA20_C TRUE)
endif()
if (CONFIG_MBEDTLS_POLY1305_C AND NOT CONFIG_CC310_MBEDTLS_POLY1305_C)
    set(CONFIG_VANILLA_MBEDTLS_POLY1305_C TRUE)
endif()
if (CONFIG_MBEDTLS_CHACHAPOLY_C AND NOT CONFIG_CC310_MBEDTLS_CHACHAPOLY_C)
    set(CONFIG_VANILLA_MBEDTLS_CHACHAPOLY_C TRUE)
endif()
if (CONFIG_MBEDTLS_RSA_C AND NOT CONFIG_CC310_MBEDTLS_RSA_C)
    set(CONFIG_VANILLA_MBEDTLS_RSA_C TRUE)
endif()

#
# mbedcrypto_shared is a library containing features that are all
# provided by vanilla backend, but is expressively enabled to allow
# single backend being enabled in the system and still have full access to
# features not provided by the specific backend
#
# All features added here can't be glued
#
add_library(mbedcrypto_shared STATIC
  ${ZEPHYR_BASE}/misc/empty_file.c
)

target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_CHACHA20_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/chacha20.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_POLY1305_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/poly1305.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_CHACHAPOLY_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/chachapoly.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_RSA_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/rsa.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_RSA_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/rsa_internal.c
)
target_sources_ifdef(CONFIG_MBEDTLS_SHA512_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/sha512.c
)

#
# Add specific includes for threading and platform_cc310
#
if(TARGET platform_cc310)
  target_include_directories(mbedcrypto_shared PRIVATE
    ${mbedcrypto_glue_include_path}/threading
    $<TARGET_PROPERTY:platform_cc310,BASE_INCLUDE_PATH>
  )
endif()

target_compile_options(mbedcrypto_shared PRIVATE ${TOOLCHAIN_C_FLAGS})
target_ld_options(mbedcrypto_shared PRIVATE ${TOOLCHAIN_LD_FLAGS})

target_include_directories(mbedcrypto_shared PRIVATE
    ${common_includes}
    ${config_include}
)

target_compile_definitions(mbedcrypto_shared PRIVATE
  -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
)

nrf_security_debug_list_target_files(mbedcrypto_shared)
