#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if(CONFIG_NRF_CRYPTO_GLUE_LIBRARY)

  nrf_security_debug("######### Creating mbedcrypto glue library #########")
  #
  # Create a library with common compilation options and include folders
  #
  zephyr_interface_library_named(mbedtls_common_glue)
  target_include_directories(${IMAGE}mbedtls_common_glue INTERFACE "${NRF_SECURITY_ROOT}/include/mbedcrypto_glue")
  target_link_libraries(${IMAGE}mbedtls_common_glue INTERFACE ${IMAGE}mbedtls_common)


  if(CONFIG_GLUE_MBEDTLS_AES_C)
    nrf_security_debug("Adding to glue: AES")
  endif()

  if(CONFIG_GLUE_MBEDTLS_CCM_C)
    nrf_security_debug("Adding to glue: CCM")
  endif()

  if(CONFIG_GLUE_MBEDTLS_DHM_C)
    nrf_security_debug("Adding to glue: DHM")
  endif()

  #
  # Create the glue wrapper library
  #
  zephyr_library_named(mbedcrypto_glue)

  zephyr_library_compile_options_ifdef(CONFIG_SHORT_WCHAR_T -fshort-wchar)

  #
  # Add glued files if enabled
  #
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_AES_C    aes_alt.c)
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_CCM_C    ccm_alt.c)
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_DHM_C    dhm_alt.c)

  zephyr_library_link_libraries(${IMAGE}mbedtls_common_glue)
  nrf_security_debug_list_target_files(${IMAGE}mbedcrypto_glue)

  #
  # Create glue libraries for backends (if enabled)
  #
  if (CONFIG_CC310_BACKEND)
    include(${CMAKE_CURRENT_LIST_DIR}/cc310/cc310_glue.cmake)
  endif()

  if (CONFIG_MBEDTLS_VANILLA_BACKEND)
    include(${CMAKE_CURRENT_LIST_DIR}/vanilla/vanilla_glue.cmake)
  endif()

endif() # CONFIG_NRF_CRYPTO_GLUE_LIBRARY

#
# CC310: Rename glued symbols to prevent collision
# CC310: Remove obj files not in use in backend (param: TRUE)
#
if (CONFIG_CC310_BACKEND)
  symbol_rename_func("cc310" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt TRUE)
endif()

#
# Vanilla: Rename glued symbols to prevent collision
#
if (CONFIG_MBEDTLS_VANILLA_BACKEND AND CONFIG_NRF_CRYPTO_GLUE_LIBRARY)
  symbol_rename_func("vanilla" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt FALSE)
endif()
