#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path SOC_MODE)

set(NRF_802154_SL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${NRF_802154_SL_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_802154_sl lib."
                  "(${NRF_802154_SL_LIB_PATH} doesn't exist.)")
endif()

zephyr_include_directories(include)

zephyr_include_directories(include/fem)

zephyr_link_libraries(${NRF_802154_SL_LIB_PATH}/libnrf_802154_sl.a)
