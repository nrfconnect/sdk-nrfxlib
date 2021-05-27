#
# Copyright (c) 2019-2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
set(NRF_SECURITY_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(mbedcrypto_target mbedcrypto)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_security_debug.cmake)

if(CONFIG_BUILD_WITH_TFM)
  nrf_security_debug("TF-M is enabled, nRF Security will build as part of TF-M")

  # TF-M is enabled and we are not inside the TF-M build system.
  # Thus, do not build regular `mbedcrypto` library, instead export needed
  # settings to the TF-M build system using the `nrf_security_target`.

  # NS-build: PSA APIs are already compiled in TF-M image
  set(COMPILE_PSA_APIS                              False)

  # NS-build can't set key ID encodes owner
  set(MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER       False)

  # Add a library to propagate configurations and paths to TF-M
  add_custom_target(nrf_security_target)

  # Pass any settings for nrf security build in TF-M context here in the form of:
  # "key0=value0;key1=value1;..."
  get_mbedtls_dir(ARM_MBEDTLS_PATH)
  set_property(TARGET nrf_security_target
    PROPERTY CMAKE_ARGS
      "ZEPHYR_DOTCONFIG=${DOTCONFIG}"
      "GCC_M_CPU=${GCC_M_CPU}"
      "ARM_MBEDTLS_PATH=${ARM_MBEDTLS_PATH}"
      "ZEPHYR_AUTOCONF=${AUTOCONF_H}"
    )

  # Set the MBEDTLS_CFG_FILE to the named configuration file
  set_property(TARGET nrf_security_target
    PROPERTY MBEDTLS_CFG_FILE
      ${CONFIG_MBEDTLS_CFG_FILE}
  )

  # Set up configurations for TF-M on zephyr_property_target
  set_property(TARGET zephyr_property_target
    APPEND PROPERTY TFM_CMAKE_OPTIONS
      -DNRF_SECURITY_SETTINGS=\"$<TARGET_PROPERTY:nrf_security_target,CMAKE_ARGS>\"
      -DTFM_MBEDCRYPTO_CONFIG_PATH=$<TARGET_PROPERTY:nrf_security_target,MBEDTLS_CFG_FILE>
      -DMCUBOOT_MBEDCRYPTO_CONFIG_PATH=$<TARGET_PROPERTY:nrf_security_target,MBEDTLS_CFG_FILE>
  )

  # Set the path used by TF-M to build mbedcrypto in the secure image
  # The CMakelists.txt in this folder is called when building mbed TLS
  set_property(TARGET zephyr_property_target
    PROPERTY TFM_MBEDCRYPTO_PATH ${ZEPHYR_NRFXLIB_MODULE_DIR}/nrf_security/tfm
  )

  # Threading is not used for TF-M NS build
  # Force this to false to disable it.
  set(MBEDTLS_THREADING_ALT False)
  set(MBEDTLS_THREADING_C False)

  # The current version of the mbed TLS deliverables requires mbedcrypto built
  # and linked in the NS image (e.g. for mbedtls and mbedx509 library). A temporary
  # fix is to convert all CC3XX_MBEDTLS_ Kconfigs into OBERON_MBEDTLS_.
  # This implies changing values of KConfig variables

  get_cmake_property(all_vars VARIABLES)

  # 1. Unset any glued configurations
  # form:
  # - CONFIG_GLUE_MBEDTLS_XXXX
  # - CONFIG_GLUE_CC3XX_MBEDTLS_XXXX
  set(cmake_vars ${all_vars})
  set(regex "^CONFIG_GLUE_(|CC3XX_|OBERON_|VANILLA_)MBEDTLS_.+")
  list(FILTER cmake_vars INCLUDE REGEX ${regex})
  foreach(var ${cmake_vars})
    set(${var} False)
    nrf_security_debug("NS: Setting ${var} to False")
  endforeach()

  # 2. Convert CC3XX_MBEDTLS_ configs to OBERON_MBEDTLS_ and unset
  #    the CC3XX_MBEDTLS_ configs
  set(cmake_vars ${all_vars})
  set(regex "^CONFIG_CC3XX_MBEDTLS_.+")
  list(FILTER cmake_vars INCLUDE REGEX ${regex})
  foreach(var ${cmake_vars})
    set(${var} False)
    nrf_security_debug("NS: Setting ${var} to False")
    string(REPLACE "CC3XX" "OBERON" var ${var})
    set(${var} True)
    nrf_security_debug("NS: Setting ${var} to True")
  endforeach()

  # 3. Special case: CHACHAPOLY/DHM/RSA is only avaialble in CC3XX or VANILLA
  #    Unset any CC3XX configs and enable in VANILLA instead
  set(cmake_vars ${all_vars})
  set(regex "^CONFIG_CC3XX_MBEDTLS_(CHACHAPOLY|DHM|RSA)_C")
  list(FILTER cmake_vars INCLUDE REGEX ${regex})
  foreach(var ${cmake_vars})
    set(${var} False)
    nrf_security_debug("NS: Setting ${var} to False")
    string(REPLACE "CC3XX" "VANILLA" var ${var})
    set(${var} True)
    nrf_security_debug("NS: Setting ${var} to True")
  endforeach()

  # 4. Generic variables setting backend (and disabling glue)
  set(CONFIG_NRF_OBERON                 True)
  set(CONFIG_OBERON_BACKEND             True)
  set(CONFIG_CC3XX_BACKEND              False)
  set(CONFIG_CC310_BACKEND              False)
  set(CONFIG_CC312_BACKEND              False)
  set(CONFIG_NRF_SECURITY_MULTI_BACKEND False)
  set(CONFIG_NRF_SECURITY_GLUE_LIBARY   False)

  # 5. Special case: _ALT in CC3XX and/or Glue, not in OBERON (set false)
  set(CONFIG_MBEDTLS_AES_ALT            False)
  set(CONFIG_MBEDTLS_CCM_ALT            False)
  set(CONFIG_MBEDTLS_CHACHAPOLY_ALT     False)
  set(CONFIG_MBEDTLS_CMAC_ALT           False)
  set(CONFIG_MBEDTLS_ECP_ALT            False)
  set(CONFIG_MBEDTLS_GCM_ALT            False)
  set(CONFIG_MBEDTLS_DHM_ALT            False)
  set(CONFIG_MBEDTLS_RSA_ALT            False)

  # 6. Special case: _ALT in ECJPAKE (only in OBERON, set to True)
  #    Only has effect if ECJPAKE is enabled
  set(CONFIG_MBEDTLS_ECJPAKE_ALT      True)

  # 7. Special case: Handle platform specific configurations
  set(MBEDTLS_PLATFORM_EXIT_ALT                     False)
  set(CONFIG_MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT    False)

else()
  nrf_security_debug("Building for pure Zephyr")

  # Pure Zephyr requires build of Zephyr APIs
  set(COMPILE_PSA_APIS                              True)

  # Pure Zephyr can't set key ID encodes owner
  set(MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER       False)

  set(MBEDTLS_PLATFORM_EXIT_ALT                     True)
  #
  # Populate ARM_MBEDTLS_PATH
  #
  get_mbedtls_dir(ARM_MBEDTLS_PATH)

endif()

# If nrf_security generates config file, include cmake script that
# adds configurations according to Kconfig and converts them from
# CONFIG_MBEDTLS_XXXX to MBEDTLS_XXXX.
if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/kconfig_mbedtls_configure.cmake)
endif()

# Add nrf_security extension functions.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/extensions.cmake)

#
#  Add nrf_security source folder
#
add_subdirectory(src)
