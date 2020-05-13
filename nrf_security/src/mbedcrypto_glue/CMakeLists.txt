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
  target_include_directories(mbedtls_common_glue INTERFACE
    "${NRF_SECURITY_ROOT}/include/mbedcrypto_glue"
  )
  target_link_libraries(mbedtls_common_glue INTERFACE mbedtls_common)


  if(CONFIG_GLUE_MBEDTLS_AES_C)
    nrf_security_debug("Adding to glue: AES")
  endif()

  if(CONFIG_GLUE_MBEDTLS_CCM_C)
    nrf_security_debug("Adding to glue: CCM")
  endif()

  if(CONFIG_GLUE_MBEDTLS_CMAC_C)
    nrf_security_debug("Adding to glue: CMAC")
  endif()

  if(CONFIG_GLUE_MBEDTLS_DHM_C)
    nrf_security_debug("Adding to glue: DHM")
  endif()

  #
  # Create the glue wrapper library
  #
  zephyr_library_named(mbedcrypto_glue)
  zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)

  #
  # Add glued files if enabled
  #
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_AES_C    aes_alt.c)
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_CCM_C    ccm_alt.c)
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_CMAC_C   cmac_alt.c)
  zephyr_library_sources_ifdef(CONFIG_GLUE_MBEDTLS_DHM_C    dhm_alt.c)

  zephyr_library_link_libraries(mbedtls_common_glue)
  nrf_security_debug_list_target_files(mbedcrypto_glue)


  #
  # Create glue libraries for backends (if enabled)
  #
  if (CONFIG_CC310_BACKEND)
    nrf_security_debug("Adding cc310_glue.cmake")
    include(${CMAKE_CURRENT_LIST_DIR}/cc310/cc310_glue.cmake)
  endif()

  if (CONFIG_OBERON_BACKEND)
    nrf_security_debug("Adding oberon_glue.cmake")
    include(${CMAKE_CURRENT_LIST_DIR}/oberon/oberon_glue.cmake)
  endif()

  if (CONFIG_MBEDTLS_VANILLA_BACKEND)
    nrf_security_debug("Adding vanilla_glue.cmake")
    include(${CMAKE_CURRENT_LIST_DIR}/vanilla/vanilla_glue.cmake)
  endif()

#
# Copy the correct glue include files to use for libraries
# (potentially overwriting other copied backend files)
#
configure_file_ifdef(CONFIG_GLUE_MBEDTLS_AES_C
  ${mbedcrypto_glue_include_path}/aes_alt.h
  ${generated_include_path}/aes_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_CCM_C
  ${mbedcrypto_glue_include_path}/ccm_alt.h
  ${generated_include_path}/ccm_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_CMAC_C
  ${mbedcrypto_glue_include_path}/cmac_alt.h
  ${generated_include_path}/cmac_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_DHM_C
  ${mbedcrypto_glue_include_path}/dhm_alt.h
  ${generated_include_path}/dhm_alt.h
  COPYONLY
)

configure_file_ifdef(CONFIG_GLUE_MBEDTLS_RSA_C
  ${mbedcrypto_glue_include_path}/rsa_alt.h
  ${generated_include_path}/rsa_alt.h
  COPYONLY
)

endif() # CONFIG_NRF_CRYPTO_GLUE_LIBRARY

#
# CC310: Rename glued symbols to prevent collision
#
if (CONFIG_CC310_BACKEND)
  symbol_rename_func("cc310" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt)
endif()

#
# nrf_oberon: Rename glued symbols to prevent collisions
#
if (CONFIG_OBERON_BACKEND AND CONFIG_NRF_CRYPTO_GLUE_LIBRARY)
  symbol_rename_func("oberon" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt)
endif()

#
# Vanilla: Rename glued symbols to prevent collision
#
if (CONFIG_MBEDTLS_VANILLA_BACKEND AND CONFIG_NRF_CRYPTO_GLUE_LIBRARY)
  symbol_rename_func("vanilla" ${CMAKE_CURRENT_LIST_DIR}/symbol_rename.template.txt)
endif()
