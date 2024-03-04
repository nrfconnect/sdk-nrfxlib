#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR OR CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY)

  string(REGEX REPLACE "_[a-zA-Z][a-zA-Z][a-zA-Z][a-zA-Z]$" "" arch_soc_dir ${CONFIG_SOC})

  if(NOT arch_soc_dir MATCHES "nRF9160")
  string(REGEX REPLACE "nRF91[0-9]*" "nRF9120" arch_soc_dir ${arch_soc_dir})
  endif()

  if(CONFIG_FPU)
    if(CONFIG_FP_HARDABI)
      set(float_dir hard-float)
    elseif(CONFIG_FP_SOFTABI)
      set(float_dir soft-float)
    else()
      assert(0 "Unreachable code")
    endif()
  else()
    set(float_dir soft-float)
  endif()

  if(CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR)
    zephyr_include_directories(cellular/include)
    set(NRF_MODEM_LIB_DIR "cellular/lib/${arch_soc_dir}/${float_dir}")
  elseif(CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY)
    zephyr_include_directories(dect_phy/include)
    set(NRF_MODEM_LIB_DIR "dect_phy/lib/${arch_soc_dir}/${float_dir}")
  endif()

  if(CONFIG_NRF_MODEM_LOG)
    zephyr_library_import(modem ${CMAKE_CURRENT_SOURCE_DIR}/${NRF_MODEM_LIB_DIR}/libmodem_log.a)
  else()
    zephyr_library_import(modem ${CMAKE_CURRENT_SOURCE_DIR}/${NRF_MODEM_LIB_DIR}/libmodem.a)
  endif()

endif()
