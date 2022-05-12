#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_include_directories(include)

nrfxlib_calculate_lib_path(lib_path SOC_MODE)

if(CONFIG_SOC_NRF5340_CPUAPP)
  set(NRF_DM_LIB ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path}/libnrf_dm_calc.a)
else()
  set(NRF_DM_LIB ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path}/libnrf_dm.a)
endif()

if(NOT EXISTS ${NRF_DM_LIB})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_dm lib. "
                  "(${NRF_DM_LIB} doesn't exist.)")
endif()

zephyr_library_import(nrf_lib_dm ${NRF_DM_LIB})
