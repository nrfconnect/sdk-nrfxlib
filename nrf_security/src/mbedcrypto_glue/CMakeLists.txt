#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
if(CONFIG_NRF_CRYPTO_GLUE_LIBRARY)
  zephyr_include_directories(
      ${NRF_MBEDTLS_ROOT}/include/mbedcrypto_glue/mbedtls
  )

  zephyr_library_named(mbedcrypto_glue)
  zephyr_library_sources(
      aes_alt.c
      ccm_alt.c
      dhm_alt.c
      ecdh_alt.c
      ecdsa_alt.c
      platform_alt.c
      rsa_alt.c
  )

  zephyr_library_include_directories(
      ${NRF_MBEDTLS_ROOT}/include/mbedcrypto_glue
  )

  zephyr_library_compile_definitions(
      -DMBEDTLS_AES_ALT
      -DMBEDTLS_CCM_ALT
      -DMBEDTLS_CMAC_ALT
      -DMBEDTLS_DHM_ALT
      -DMBEDTLS_ECDH_GEN_PUBLIC_ALT
      -DMBEDTLS_ECDH_COMPUTE_SHARED_ALT
      -DMBEDTLS_ECDSA_VERIFY_ALT
      -DMBEDTLS_ECDSA_SIGN_ALT
      -DMBEDTLS_ECDSA_GENKEY_ALT
      -DMBEDTLS_GCM_ALT
      -DMBEDTLS_RSA_ALT
  )

  include(${CMAKE_CURRENT_LIST_DIR}/cc310/CMakeLists.txt)
  include(${CMAKE_CURRENT_LIST_DIR}/vanilla/CMakeLists.txt)
else()
  # Multiple backends was enabled, but glue layer is not needed.
  # Hence only ensure alt headers are available (namely platform_alt.h)
  add_library(${IMAGE}mbedcrypto_glue INTERFACE)
  zephyr_include_directories(
      ${NRF_MBEDTLS_ROOT}/include/mbedcrypto_glue/mbedtls
  )
endif()

# Platform implementation cannot be chosen Kconfig.
# To ensure platform symbols are not renamed cc310 implementation is used.
set(CONFIG_CC310_MBEDTLS_PLATFORM True)

library_redefine_symbols("cc310" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt)
library_redefine_symbols("vanilla" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt)

