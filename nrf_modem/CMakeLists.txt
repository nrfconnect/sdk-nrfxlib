#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR OR CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY)

  cmake_path(SET lib_path ${CMAKE_CURRENT_SOURCE_DIR})
  cmake_path(APPEND lib_path lib)

  # Library variant
  if(CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR)
    cmake_path(APPEND lib_path cellular)
  elseif(CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY)
    cmake_path(APPEND lib_path dect_phy)
  endif()

  # SOC
  if(CONFIG_SOC_NRF9160)
    cmake_path(APPEND lib_path nrf9160)
  elseif(CONFIG_SOC_NRF9120)
    cmake_path(APPEND lib_path nrf9120)
  else()
    assert(0 "Unsupported SOC. Got '${CONFIG_SOC}'")
  endif()

  # Floating ABI
  if(CONFIG_FPU)
    if(CONFIG_FP_HARDABI)
      cmake_path(APPEND lib_path hard-float)
    elseif(CONFIG_FP_SOFTABI)
      cmake_path(APPEND lib_path soft-float)
    else()
      assert(0 "Unreachable code")
    endif()
  else()
    cmake_path(APPEND lib_path soft-float)
  endif()

  # Logging
  if(CONFIG_NRF_MODEM_LOG)
    cmake_path(APPEND lib_path libmodem_log.a)
  else()
    cmake_path(APPEND lib_path libmodem.a)
  endif()

  # Check if library file exists
  if(NOT EXISTS ${lib_path})
    assert(0 "Binary library not found: '${lib_path}'")
  endif()

  zephyr_library_import(modem ${lib_path})

  zephyr_include_directories(include)

  target_sources(app PRIVATE shmem.c)

endif()
