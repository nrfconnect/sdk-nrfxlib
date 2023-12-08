#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if (CONFIG_NRF_802154_RADIO_DRIVER)

add_library(nrf-802154-sl INTERFACE)

nrfxlib_calculate_lib_path(lib_path SOC_MODE)

set(NRF_802154_SL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${NRF_802154_SL_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_802154_sl lib."
                  "(${NRF_802154_SL_LIB_PATH} doesn't exist.)")
endif()

target_link_libraries(nrf-802154-sl
  INTERFACE
    ${NRF_802154_SL_LIB_PATH}/libnrf-802154-sl.a
)

endif ()
