#
# Copyright (c) 2023 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrfxlib_calculate_lib_path(NRF_FUEL_GAUGE_DIR
  BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  SOFT_FLOAT_FALLBACK
)

if(NOT EXISTS ${NRF_FUEL_GAUGE_DIR})
  message(WARNING "This combination of SoC and floating point ABI is not supported by nrf Fuel Gauge."
                  "(${NRF_FUEL_GAUGE_DIR} doesn't exist.)")
endif()

zephyr_include_directories(include)
target_link_libraries(app PRIVATE ${NRF_FUEL_GAUGE_DIR}/libnrf_fuel_gauge.a)
