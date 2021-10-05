#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

add_library(nrf-802154-sl INTERFACE)

nrfxlib_calculate_lib_path(NRF_802154_SL_LIB_DIR
  BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  SOFT_FLOAT_FALLBACK
  SOC_MODE
)

if(NOT NRF_802154_SL_LIB_DIR)
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nrf_802154_sl lib.")
endif()

target_link_libraries(nrf-802154-sl
  INTERFACE
    ${NRF_802154_SL_LIB_DIR}/libnrf-802154-sl.a
)
