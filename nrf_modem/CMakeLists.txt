#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_MODEM_LINK_BINARY_CELLULAR OR CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY)

  if(CONFIG_SOC_NRF9160)
    set(arch_soc_dir "nrf9160")
  elseif(CONFIG_SOC_NRF9120)
    set(arch_soc_dir "nrf9120")
  else()
    assert(0 "Unknown SOC for nrf_modem. Got '${CONFIG_SOC}'")
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
    set(NRF_MODEM_LIB_DIR "lib/cellular/${arch_soc_dir}/${float_dir}")
  elseif(CONFIG_NRF_MODEM_LINK_BINARY_DECT_PHY)
    set(NRF_MODEM_LIB_DIR "lib/dect_phy/${arch_soc_dir}/${float_dir}")
  endif()

  if(CONFIG_NRF_MODEM_LOG)
    zephyr_library_import(modem ${CMAKE_CURRENT_SOURCE_DIR}/${NRF_MODEM_LIB_DIR}/libmodem_log.a)
  else()
    zephyr_library_import(modem ${CMAKE_CURRENT_SOURCE_DIR}/${NRF_MODEM_LIB_DIR}/libmodem.a)
  endif()

  zephyr_include_directories(include)

  target_sources(app PRIVATE shmem.c)

endif()
