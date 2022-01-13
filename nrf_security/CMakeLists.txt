#
# Copyright (c) 2019-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# This file is the entry point for mbed TLS built through Zephyr.

# Set root folders as variables
set(NRFXLIB_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(NRF_SECURITY_ROOT ${CMAKE_CURRENT_LIST_DIR})

# Include generic macros
include(${CMAKE_CURRENT_LIST_DIR}/cmake/extensions.cmake)

# Include debugging
include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_security_debug.cmake)

  # pelion-dm creates mbedcrypto and mbedx509 as INTERFACE libraries
  # This results in library name collision, so therefore we are renaming the
  # mbedcrypto_target to real_mbedcrypto if mbedcrypto already exists.
if(TARGET mbedcrypto)
  set(mbedcrypto_target real_mbedcrypto)
  set(mbedx509_target   real_mbedx509)
  set(mbedtls_target    real_mbedtls)
else()
  set(mbedcrypto_target mbedcrypto)
  set(mbedx509_target   mbedx509)
  set(mbedtls_target    mbedtls)
endif()

# Populate ARM_MBEDTLS_PATH
get_mbedtls_dir(ARM_MBEDTLS_PATH)

# MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER must be disabled for Zephyr
# builds or when MBEDTLS_USE_PSA_CRYPTO is enabled (e.g. for TLS/DTLS
# and x.509 support) Note: This configuration is internal and may be
# removed with a new mbed TLS version
set(MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER     False)

if(NOT CONFIG_BUILD_WITH_TFM)
  nrf_security_debug("Building for pure Zephyr")

  if(CONFIG_MBEDTLS_PSA_CRYPTO_C)
    # Build PSA APIs if requested
    set(COMPILE_PSA_APIS                        True)
  endif()

  # Add external platform exit function
  set(MBEDTLS_PLATFORM_EXIT_ALT                 True)

else()
  # TF-M is enabled and we are not inside the TF-M build system.
  # Thus, do not build regular `mbedcrypto` library, instead export
  # needed settings to the TF-M build system using TFM_CMAKE_OPTIONS.

  # NS-build: PSA APIs are already compiled in TF-M image
  set(COMPILE_PSA_APIS                          False)

  # Execute Cmake logic to forward configurations to TF-M build
  include(${NRF_SECURITY_ROOT}/cmake/config_to_tf-m.cmake)

  # Add replacement exit and platform.c for NS build
  list(APPEND src_zephyr
    ${NRF_SECURITY_ROOT}/src/legacy/exit.c
    ${ARM_MBEDTLS_PATH}/library/platform.c
  )

  # The current version of the mbed TLS deliverables requires mbedcrypto built
  # and linked in the NS image (e.g. for mbedtls and mbedx509 library).
  # If CC3XX_BACKEND is enabled, configurations need to be converted to
  # OBERON_BACKEND (enabled by default).

  get_cmake_property(all_vars VARIABLES)

  # 1. Enable OBERON_BACKEND, disable CC3XX_BACKEND
  set(CONFIG_NRF_OBERON                         True)
  set(CONFIG_OBERON_BACKEND                     True)
  set(CONFIG_CC3XX_BACKEND                      False)
  set(CONFIG_CC310_BACKEND                      False)
  set(CONFIG_CC312_BACKEND                      False)

  # 2. Special case: _ALT in CC3XX, not in OBERON (set  to False)
  set(CONFIG_MBEDTLS_AES_ALT                    False)
  set(CONFIG_MBEDTLS_CCM_ALT                    False)
  set(CONFIG_MBEDTLS_CHACHAPOLY_ALT             False)
  set(CONFIG_MBEDTLS_CMAC_ALT                   False)
  set(CONFIG_MBEDTLS_ECP_ALT                    False)
  set(CONFIG_MBEDTLS_GCM_ALT                    False)
  set(CONFIG_MBEDTLS_DHM_ALT                    False)
  set(CONFIG_MBEDTLS_RSA_ALT                    False)

  # 3. Special case: _ALT in ECJPAKE (only in OBERON, set to True)
  #    Only has effect if ECJPAKE is enabled
  set(CONFIG_MBEDTLS_ECJPAKE_ALT                True)

  # 4. Special case: Handle platform specific configurations
  set(MBEDTLS_PLATFORM_EXIT_ALT                 False)
  set(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT       False)

  # 5. Threading is not used for TF-M NS build
  # Force this to false to disable it.
  set(MBEDTLS_THREADING_ALT                     False)
  set(MBEDTLS_THREADING_C                       False)

endif()

# Add mbed TLS Libraries
add_subdirectory(src)

# Add zephyr-specific code
add_subdirectory(src/zephyr)
