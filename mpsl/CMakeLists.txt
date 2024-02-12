#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if (CONFIG_MPSL_BUILD_TYPE_LIB)

add_subdirectory(fem)

if (CONFIG_SOC_SERIES_NRF54HX OR CONFIG_SOC_SERIES_NRF54LX)
  nrfxlib_calculate_lib_path(MPSL_LIB_DIR SOC_MODE
    BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    SOFT_FLOAT_FALLBACK
  )
else ()
  nrfxlib_calculate_lib_path(MPSL_LIB_DIR
    BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    SOFT_FLOAT_FALLBACK
  )
endif ()

if(NOT MPSL_LIB_DIR)
  message(WARNING "This combination of SoC and floating point ABI is not supported by the MPSL lib.")
endif()

zephyr_include_directories(include)
zephyr_include_directories(include/protocol)
zephyr_link_libraries(${MPSL_LIB_DIR}/libmpsl.a)

endif()
