#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path)

set(MPSL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${MPSL_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the MPSL lib."
                  "(${MPSL_LIB_PATH} doesn't exist.)")
endif()

if(CONFIG_TEMP_NRF5 AND CONFIG_MPSL)
  message(WARNING "CONFIG_TEMP_NRF5 and CONFIG_MPSL have both been enabled, but are not designed to co-exist.")
endif()

zephyr_include_directories(include)
zephyr_link_libraries(${MPSL_LIB_PATH}/libmpsl.a)

