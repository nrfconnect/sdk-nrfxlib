#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
# The purpose of this file is to provice a set of debug commands to nrf security
# module.

nrf_security_debug("######### Creating shared noglue library #########")

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


if (CONFIG_VANILLA_MBEDTLS_CHACHA20_C OR CONFIG_OBERON_MBEDTLS_CHACHA20_C OR
  (CONFIG_MBEDTLS_CHACHA20_C AND NOT CONFIG_CC3XX_MBEDTLS_CHACHA20_C))


  target_sources(mbedcrypto_shared PRIVATE
    ${ARM_MBEDTLS_PATH}/library/chacha20.c
  )
endif()


if (CONFIG_VANILLA_MBEDTLS_POLY1305_C OR CONFIG_OBERON_MBEDTLS_POLY1305_C OR
  (CONFIG_MBEDTLS_POLY1305_C AND NOT CONFIG_CC3XX_MBEDTLS_POLY1305_C))



  target_sources(mbedcrypto_shared PRIVATE
    ${ARM_MBEDTLS_PATH}/library/poly1305.c
  )
endif()

if (CONFIG_VANILLA_MBEDTLS_CHACHAPOLY_C OR
  (CONFIG_MBEDTLS_CHACHAPOLY_C AND NOT CONFIG_CC3XX_MBEDTLS_CHACHAPOLY_C))

  target_sources(mbedcrypto_shared PRIVATE
    ${ARM_MBEDTLS_PATH}/library/chachapoly.c
  )
endif()

if (CONFIG_VANILLA_MBEDTLS_RSA_C OR
   (CONFIG_MBEDTLS_RSA_C AND NOT CONFIG_CC3XX_MBEDTLS_RSA_C))

  target_sources(mbedcrypto_shared PRIVATE
    ${ARM_MBEDTLS_PATH}/library/rsa.c
    ${ARM_MBEDTLS_PATH}/library/rsa_internal.c
  )

endif()

target_sources_ifdef(CONFIG_MBEDTLS_SHA512_C
  mbedcrypto_shared PRIVATE ${ARM_MBEDTLS_PATH}/library/sha512.c
)

# Special case: Oberon only backend and enabled DHM
if (CONFIG_MBEDTLS_DHM_C AND CONFIG_OBERON_BACKEND)

  target_sources(mbedcrypto_shared PRIVATE
    ${ARM_MBEDTLS_PATH}/library/dhm.c
  )
endif()

#
# Add specific includes for threading and platform_cc3xx
#
if(TARGET platform_cc3xx)
  target_include_directories(mbedcrypto_shared PRIVATE
    ${mbedcrypto_glue_include_path}/threading
    $<TARGET_PROPERTY:platform_cc3xx,BASE_INCLUDE_PATH>
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
