#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

include_guard(GLOBAL)

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path)

add_library(nrfxlib_crypto INTERFACE)

if(CONFIG_NRF_OBERON OR CONFIG_BUILD_WITH_TFM OR CONFIG_OBERON_BACKEND OR CONFIG_PSA_CRYPTO_DRIVER_OBERON)

  set(OBERON_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_oberon)
  set(OBERON_VER 3.0.15)

  nrfxlib_calculate_lib_path(OBERON_LIB_DIR
    BASE_DIR ${OBERON_BASE}
    SOFT_FLOAT_FALLBACK
  )

  set(OBERON_LIB ${OBERON_LIB_DIR}/liboberon_${OBERON_VER}.a)
  set(OBERON_MBEDTLS_LIB ${OBERON_LIB_DIR}/liboberon_mbedtls_${OBERON_VER}.a)

  if(NOT EXISTS ${OBERON_LIB} OR
     NOT EXISTS ${OBERON_MBEDTLS_LIB})
    message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_oberon lib."
                    "(${OBERON_LIB} or ${OBERON_MBEDTLS_LIB} doesn't exist.)")
  endif()

  if (NOT CONFIG_BUILD_WITH_TFM AND NOT CONFIG_OBERON_BACKEND AND NOT CONFIG_PSA_CRYPTO_DRIVER_OBERON)
    target_include_directories(nrfxlib_crypto INTERFACE ${OBERON_BASE}/include)
    target_link_libraries(nrfxlib_crypto INTERFACE ${OBERON_LIB})
  else()
    if(NOT EXISTS ${OBERON_MBEDTLS_LIB})
      message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_oberon_mbedtls lib."
                      "(${OBERON_MBEDTLS_LIB} doesn't exist.)")
    endif()

    #
    # Add mbedcrypto_oberon_imported - Contains proprietary nrf_oberon APIs
    #
    add_library(mbedcrypto_oberon_imported STATIC IMPORTED GLOBAL)
    set_target_properties(mbedcrypto_oberon_imported PROPERTIES
      IMPORTED_LOCATION
        "${OBERON_LIB}"
    )
    target_include_directories(mbedcrypto_oberon_imported
      INTERFACE
        "${OBERON_BASE}/include"
    )

    #
    # Add mbedcrypto_oberon_mbedtls_imported - contains legacy crypto APIs
    #
    add_library(mbedcrypto_oberon_mbedtls_imported STATIC IMPORTED GLOBAL)
    set_target_properties(mbedcrypto_oberon_mbedtls_imported PROPERTIES
      IMPORTED_LOCATION
        "${OBERON_MBEDTLS_LIB}"
    )
    target_include_directories(mbedcrypto_oberon_mbedtls_imported
      INTERFACE
        "${OBERON_BASE}/include/mbedtls"
    )
    target_link_libraries(mbedcrypto_oberon_mbedtls_imported
      INTERFACE
        mbedcrypto_oberon_imported
    )
  endif()
endif()

if (CONFIG_NRF_CC310_BL)
  assert(CONFIG_HAS_HW_NRF_CC310 "nrf_cc310_bl requires CryptoCell hardware.")
  set(CC310_BL_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_cc310_bl)
  set(CC310_FLAVOR no-interrupts)
  set(CC310_BL_VER 0.9.12)

  nrfxlib_calculate_lib_path(CC310_BL_LIB_DIR
    BASE_DIR ${CC310_BL_BASE}
    SOFT_FLOAT_FALLBACK
  )

  set(CC310_BL_LIB
    ${CC310_BL_LIB_DIR}/${CC310_FLAVOR}/libnrf_cc310_bl_${CC310_BL_VER}.a
  )

  if(NOT EXISTS ${CC310_BL_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_cc310_bl lib."
                    "(${CC310_BL_LIB} doesn't exist.)")
  endif()
  target_include_directories(nrfxlib_crypto INTERFACE ${CC310_BL_BASE}/include)
  target_link_libraries(nrfxlib_crypto INTERFACE ${CC310_BL_LIB})
endif()

if(CONFIG_HAS_HW_NRF_CC312)
  set(CC3XX_ARCH cc312)
elseif(CONFIG_HAS_HW_NRF_CC310)
  set(CC3XX_ARCH cc310)
endif()

if (CONFIG_NRF_CC3XX_PLATFORM)
  set(NRF_CC3XX_PLATFORM_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_${CC3XX_ARCH}_platform)
  set(NRF_CC3XX_PLATFORM_VER 0.9.19)
  set(CC3XX_PLATFORM_FLAVOR no-interrupts)

  nrfxlib_calculate_lib_path(NRF_CC3XX_PLATFORM_LIB_DIR
    BASE_DIR ${NRF_CC3XX_PLATFORM_BASE}
    SOFT_FLOAT_FALLBACK
  )

  set(NRF_CC3XX_PLATFORM_LIB
    ${NRF_CC3XX_PLATFORM_LIB_DIR}/${CC3XX_PLATFORM_FLAVOR}/libnrf_${CC3XX_ARCH}_platform_${NRF_CC3XX_PLATFORM_VER}.a
  )

  if (NOT EXISTS ${NRF_CC3XX_PLATFORM_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_${CC3XX_ARCH}_platform lib."
                    "(${NRF_CC3XX_PLATFORM_LIB} doesn't exist.)")
  endif()
  #
  # Uses a normal add_library to avoid --whole-archive
  #
  add_library(nrf_cc3xx_platform STATIC IMPORTED GLOBAL)
  set_target_properties(nrf_cc3xx_platform PROPERTIES
    IMPORTED_LOCATION "${NRF_CC3XX_PLATFORM_LIB}"
  )
  target_include_directories(nrf_cc3xx_platform INTERFACE
      "${NRF_CC3XX_PLATFORM_BASE}/include")

  # Make sure that C library is linked after nrf_cc3xx_platform library to
  # prevent linker errors
  if (CONFIG_NEWLIB_LIBC)
    target_link_libraries(nrf_cc3xx_platform INTERFACE -lc)
  endif()

  # Add fallback to play nice with
  add_library(platform_cc3xx INTERFACE)
  target_link_libraries(platform_cc3xx INTERFACE nrf_cc3xx_platform)

  if (TARGET zephyr_interface)
    zephyr_link_libraries(nrf_cc3xx_platform)
    #
    # Add companion sources to directly to zephyr
    #
    zephyr_sources(${NRF_CC3XX_PLATFORM_BASE}/src/nrf_cc3xx_platform_abort_zephyr.c)
    if (CONFIG_MULTITHREADING)
      zephyr_sources(${NRF_CC3XX_PLATFORM_BASE}/src/nrf_cc3xx_platform_mutex_zephyr.c)
    else()
      zephyr_sources(${NRF_CC3XX_PLATFORM_BASE}/src/nrf_cc3xx_platform_no_mutex_zephyr.c)
    endif()
  endif()
endif()

if (CONFIG_CC3XX_BACKEND OR (CONFIG_PSA_CRYPTO_DRIVER_CC3XX AND NOT BUILD_WITH_TFM))
  set(NRF_CC3XX_BASE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_${CC3XX_ARCH}_mbedcrypto)
  set(NRF_CC3XX_VER 0.9.19)
  set(CC3XX_FLAVOR no-interrupts)

  nrfxlib_calculate_lib_path(NRF_CC3XX_LIB_DIR
    BASE_DIR ${NRF_CC3XX_BASE}
    SOFT_FLOAT_FALLBACK
  )

  set(NRF_CC3XX_CORE_LIB
    ${NRF_CC3XX_LIB_DIR}/${CC3XX_FLAVOR}/libnrf_${CC3XX_ARCH}_core_${NRF_CC3XX_VER}.a
  )

  if(NOT EXISTS ${NRF_CC3XX_CORE_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_${CC3XX_ARCH}_mbedcryto lib."
                    "(${NRF_CC3XX_LIB} doesn't exist.)")
  endif()

  #
  # Add nrf_cc3xx_core_imported library
  #
  add_library(nrf_cc3xx_core_imported STATIC IMPORTED GLOBAL)
  set_target_properties(nrf_cc3xx_core_imported PROPERTIES
    IMPORTED_LOCATION "${NRF_CC3XX_CORE_LIB}"
	)

  # *_alt.h headers and cc3xx_kmu.h
  set(mbedcrypto_includes ${NRF_CC3XX_BASE}/include/
                       ${NRF_CC3XX_BASE}/include/mbedtls)


  target_include_directories(nrf_cc3xx_core_imported INTERFACE ${mbedcrypto_includes})

  target_link_libraries(nrf_cc3xx_core_imported INTERFACE nrf_cc3xx_platform)

  target_link_libraries(nrfxlib_crypto INTERFACE nrf_cc3xx_core_imported)

  if (TARGET zephyr_interface)
	  zephyr_include_directories(${mbedcrypto_includes})
  endif()

  #
  # Add nrf_cc3xx_legacy_crypto_imported library (for mbedtls_ prfixed APIs)
  #
  add_library(nrf_cc3xx_legacy_crypto_imported STATIC IMPORTED GLOBAL)

  target_link_libraries(nrf_cc3xx_legacy_crypto_imported
    INTERFACE
      nrf_cc3xx_core_imported
      mbedcrypto_base
    )

  set(NRF_CC3XX_MBEDTLS_LEGACY_LIB
    ${NRF_CC3XX_LIB_DIR}/${CC3XX_FLAVOR}/libnrf_${CC3XX_ARCH}_legacy_crypto_${NRF_CC3XX_VER}.a
  )

  if(NOT EXISTS ${NRF_CC3XX_MBEDTLS_LEGACY_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_${CC3XX_ARCH}_legacy_crypto lib."
                  "(${NRF_CC3XX_MBEDTLS_LEGACY_LIB} doesn't exist.)")
  endif()

  set_property(TARGET nrf_cc3xx_legacy_crypto_imported PROPERTY
               IMPORTED_LOCATION "${NRF_CC3XX_MBEDTLS_LEGACY_LIB}")

  #
  # Add nrf_cc3xx_psa_crypto_imported_imported library (for PSA APIs)
  #
  add_library(nrf_cc3xx_psa_crypto_imported STATIC IMPORTED GLOBAL)

  target_link_libraries(nrf_cc3xx_psa_crypto_imported 
    INTERFACE 
      nrf_cc3xx_legacy_crypto_imported
      nrf_cc3xx_core_imported
      nrf_cc3xx_platform
    )

  set(NRF_CC3XX_PSA_LIB
    ${NRF_CC3XX_LIB_DIR}/${CC3XX_FLAVOR}/libnrf_${CC3XX_ARCH}_psa_crypto_${NRF_CC3XX_VER}.a
  )

  if(NOT EXISTS ${NRF_CC3XX_PSA_LIB})
    message(WARNING "This combination of SoC, floating point ABI, and interrupts settings is not supported by the nrf_${CC3XX_ARCH}_psa_crypto lib."
                  "(${NRF_CC3XX_PSA_LIB} doesn't exist.)")
  endif()

  set_property(TARGET nrf_cc3xx_psa_crypto_imported PROPERTY
               IMPORTED_LOCATION "${NRF_CC3XX_PSA_LIB}")
endif()
