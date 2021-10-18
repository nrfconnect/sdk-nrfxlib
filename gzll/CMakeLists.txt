#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path)

set(GZLL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${GZLL_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the gzll lib."
                  "(${GZLL_LIB_PATH} doesn't exist.)")
endif()

zephyr_include_directories(include)

if(CONFIG_SOC_NRF52832)
  set(GZLL_LIB_VARIANT nrf52)
elseif(CONFIG_SOC_NRF52810)
  set(GZLL_LIB_VARIANT nrf52810)
elseif(CONFIG_SOC_NRF52811)
  set(GZLL_LIB_VARIANT nrf52811)
elseif(CONFIG_SOC_NRF52820)
  set(GZLL_LIB_VARIANT nrf52820)
elseif(CONFIG_SOC_NRF52833)
  set(GZLL_LIB_VARIANT nrf52833)
elseif(CONFIG_SOC_NRF52840)
  set(GZLL_LIB_VARIANT nrf52840)
else()
  set(GZLL_LIB_VARIANT nrf5x)
endif()

set(GZLL_LIB ${GZLL_LIB_PATH}/libgzll_${GZLL_LIB_VARIANT}.a)

if(NOT EXISTS ${GZLL_LIB})
  message(WARNING "SoC is not supported by the gzll lib. "
                  "(${GZLL_LIB} doesn't exist.)")
endif()

zephyr_link_libraries(${GZLL_LIB})
