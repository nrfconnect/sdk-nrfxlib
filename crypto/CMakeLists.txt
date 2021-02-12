#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

nrfxlib_calculate_lib_path(lib_path)

zephyr_interface_library_named(nrfxlib_crypto)

if (CONFIG_NRF_OBERON)
  set(OBERON_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_oberon)
  set(OBERON_VER 3.0.8)
  set(OBERON_LIB ${OBERON_BASE}/${lib_path}/liboberon_${OBERON_VER}.a)
  set(OBERON_MBEDTLS_LIB ${OBERON_BASE}/${lib_path}/liboberon_mbedtls_${OBERON_VER}.a)
  if(NOT EXISTS ${OBERON_LIB})
    message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_oberon lib."
                    "(${OBERON_LIB} doesn't exist.)")
  endif()

  if (NOT CONFIG_OBERON_BACKEND)
    target_include_directories(nrfxlib_crypto INTERFACE ${OBERON_BASE}/include)
    target_link_libraries(nrfxlib_crypto INTERFACE ${OBERON_LIB})
  else()
    if(NOT EXISTS ${OBERON_MBEDTLS_LIB})
      message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_oberon_mbedtls lib."
                      "(${OBERON_MBEDTLS_LIB} doesn't exist.)")
    endif()

    #
    # Add mbedcrypto_oberon_imported that points to the actual library
    # This library will be stripped for unneeded symbols and combined into
    # library target mbedcrypto_oberon
    #
    add_library(mbedcrypto_oberon_imported STATIC IMPORTED GLOBAL)
    set_target_properties(
      mbedcrypto_oberon_imported PROPERTIES
        IMPORTED_LOCATION "${OBERON_LIB}"
        BASE_INCLUDE_PATH "${OBERON_BASE}/include"
    )
    target_include_directories(mbedcrypto_oberon_imported INTERFACE
                              "${OBERON_BASE}/include")


    #
    # Add mbedcrypto_oberon_mbedtls_imported that points to the actual library
    # This library will be stripped for unneeded symbols and combined into
    # library target mbedcrypto_oberon
    #
    add_library(mbedcrypto_oberon_mbedtls_imported STATIC IMPORTED GLOBAL)
    set_target_properties(
      mbedcrypto_oberon_mbedtls_imported PROPERTIES
        IMPORTED_LOCATION "${OBERON_MBEDTLS_LIB}"
        BASE_INCLUDE_PATH "${OBERON_BASE}/include/mbedtls"
    )
    target_include_directories(mbedcrypto_oberon_mbedtls_imported INTERFACE
                              "${OBERON_BASE}/include"
                              "${OBERON_BASE}/include/mbedtls")
    target_link_libraries(mbedcrypto_oberon_mbedtls_imported INTERFACE mbedcrypto_oberon_imported)
  endif()
endif()

if (CONFIG_NRF_CC310_BL)
  assert(CONFIG_HAS_HW_NRF_CC310 "nrf_cc310_bl requires CryptoCell hardware.")
  set(CC310_BL_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_cc310_bl)
  if (NOT CONFIG_HW_CC3XX_INTERRUPT)
    set(CC310_FLAVOR no-interrupts)
  endif()
  set(CC310_BL_VER 0.9.12)
  set(CC310_BL_LIB ${CC310_BL_BASE}/${lib_path}/${CC310_FLAVOR}/libnrf_cc310_bl_${CC310_BL_VER}.a)
  if(NOT EXISTS ${CC310_BL_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_cc310_bl lib."
                    "(${CC310_BL_LIB} doesn't exist.)")
  endif()
  target_include_directories(nrfxlib_crypto INTERFACE ${CC310_BL_BASE}/include)
  target_link_libraries(nrfxlib_crypto INTERFACE ${CC310_BL_LIB})
endif()

if (CONFIG_NRF_CC3XX_PLATFORM)
  if(CONFIG_SOC_NRF5340_CPUAPP)
    set (CC3XX_ARCH cc312)
  else()
    set (CC3XX_ARCH cc310)
  endif()
  set (NRF_CC3XX_PLATFORM_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_${CC3XX_ARCH}_platform)
  set (NRF_CC3XX_PLATFORM_VER 0.9.7)
  if (NOT CONFIG_HW_CC3XX_INTERRUPT)
    set(CC3XX_PLATFORM_FLAVOR no-interrupts)
  endif()
  set(NRF_CC3XX_PLATFORM_LIB
    ${NRF_CC3XX_PLATFORM_BASE}/${lib_path}/${CC3XX_PLATFORM_FLAVOR}/libnrf_${CC3XX_ARCH}_platform_${NRF_CC3XX_PLATFORM_VER}.a
  )

  if (NOT EXISTS ${NRF_CC3XX_PLATFORM_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_${CC3XX_ARCH}_platform lib."
                    "(${NRF_CC3XX_PLATFORM_LIB} doesn't exist.)")
  endif()
  #
  # Uses a normal add_library to avoid --whole-archive
  #
  add_library(platform_cc3xx STATIC IMPORTED GLOBAL)
  set_target_properties(platform_cc3xx PROPERTIES
    IMPORTED_LOCATION "${NRF_CC3XX_PLATFORM_LIB}"
    BASE_INCLUDE_PATH "${NRF_CC3XX_PLATFORM_BASE}/include"
  )
  target_include_directories(platform_cc3xx INTERFACE
      "${NRF_CC3XX_PLATFORM_BASE}/include")
  zephyr_link_libraries(platform_cc3xx)
  #
  # Add companion sources to directly to zephyr
  #
  zephyr_sources(${NRF_CC3XX_PLATFORM_BASE}/src/nrf_cc3xx_platform_abort_zephyr.c)
  zephyr_sources(${NRF_CC3XX_PLATFORM_BASE}/src/nrf_cc3xx_platform_mutex_zephyr.c)
endif()

if (CONFIG_CC3XX_BACKEND)
  if(CONFIG_SOC_NRF5340_CPUAPP)
    set (CC3XX_ARCH cc312)
  else()
    set (CC3XX_ARCH cc310)
  endif()
  set(NRF_CC3XX_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_${CC3XX_ARCH}_mbedcrypto)
  set(NRF_CC3XX_VER 0.9.7)
  if (NOT CONFIG_HW_CC3XX_INTERRUPT)
    set(CC3XX_FLAVOR no-interrupts)
  endif()
  set(NRF_CC3XX_LIB
    ${NRF_CC3XX_BASE}/${lib_path}/${CC3XX_FLAVOR}/libnrf_${CC3XX_ARCH}_mbedcrypto_${NRF_CC3XX_VER}.a
  )
  if(NOT EXISTS ${NRF_CC3XX_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_${CC3XX_ARCH}_mbedcryto lib."
                    "(${NRF_CC3XX_LIB} doesn't exist.)")
  endif()

  #
  # Add mbedcrypto_cc3xx_imported that points links to the actual library
  # This library will be stripped for unneeded symbols and combined into
  # library target mbedcrypto_cc3xx
  #
  add_library(mbedcrypto_cc3xx_imported STATIC IMPORTED GLOBAL)
  set_target_properties(mbedcrypto_cc3xx_imported PROPERTIES
    IMPORTED_LOCATION "${NRF_CC3XX_LIB}"
    BASE_INCLUDE_PATH "${NRF_CC3XX_BASE}/include/mbedtls"
  )
  target_include_directories(mbedcrypto_cc3xx_imported INTERFACE
                             "${NRF_CC3XX_BASE}/include"
                             "${NRF_CC3XX_BASE}/include/mbedtls")
  target_link_libraries(mbedcrypto_cc3xx_imported INTERFACE platform_cc3xx)
  if (NOT CONFIG_NRF_SECURITY_MULTI_BACKEND)
    # Only CC3XX backend is selected for mbedtls API.
    # Thus export the alt headers through Zephyr interface library.
    zephyr_include_directories(${NRF_CC3XX_BASE}/include/mbedtls
                               ${NRF_CC3XX_BASE}/include)
  endif()
endif()
