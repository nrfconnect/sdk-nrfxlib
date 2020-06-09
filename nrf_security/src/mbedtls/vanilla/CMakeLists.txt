#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

nrf_security_debug("######### Creating mbedcrypto vanilla noglue library #########")

#
# Create mbedcrypto vanilla (noglue) library
# Doesn't have ALT flags set (to ensure vanilla context sizes)
# This implements features that may be glued but with the original contexts
# This must use be compiled with its own config-file (nrf-config-noglue.h)
# This library must not be linked with directly
#
add_library(mbedcrypto_vanilla STATIC ${ZEPHYR_BASE}/misc/empty_file.c)

#
# Adding all standard compile/linker options (e.g. float ABI)
#
target_compile_options(mbedcrypto_vanilla PRIVATE ${TOOLCHAIN_C_FLAGS})
target_ld_options(mbedcrypto_vanilla PRIVATE ${TOOLCHAIN_LD_FLAGS})

#
# Adding original mbed TLS files
#
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_AES_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/aes.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_CCM_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ccm.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_DHM_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/dhm.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_CMAC_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/cmac.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_ECP_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ecp.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_ECP_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ecp_curves.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_ECDH_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ecdh.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_ECDSA_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ecdsa.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_ECJPAKE_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/ecjpake.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_SHA1_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/sha1.c
)
target_sources_ifdef(CONFIG_VANILLA_MBEDTLS_SHA256_C
  mbedcrypto_vanilla PRIVATE ${ARM_MBEDTLS_PATH}/library/sha256.c
)

target_include_directories(mbedcrypto_vanilla PRIVATE
  ${common_includes}
  ${config_include}
)

#
# Add specific includes for threading and platform_cc310
#
if(TARGET platform_cc310)
  target_include_directories(mbedcrypto_vanilla PRIVATE
    ${mbedcrypto_glue_include_path}/threading
    $<TARGET_PROPERTY:platform_cc310,BASE_INCLUDE_PATH>
  )
endif()

target_compile_options(mbedcrypto_vanilla PRIVATE ${TOOLCHAIN_C_FLAGS})
target_ld_options(mbedcrypto_vanilla PRIVATE ${TOOLCHAIN_LD_FLAGS})

target_compile_definitions(mbedcrypto_vanilla PRIVATE
  -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
)

nrf_security_debug_list_target_files(mbedcrypto_vanilla)
