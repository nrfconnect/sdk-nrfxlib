#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

include(${NRFXLIB_DIR}/common.cmake)

zephyr_include_directories(include)

nrfxlib_calculate_lib_path(lib_path SOC_MODE)

set(GZLL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path}/libgzll.a)

if(NOT EXISTS ${GZLL_LIB})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the GZLL lib. "
                  "(${GZLL_LIB} doesn't exist.)")
endif()

zephyr_link_libraries(${GZLL_LIB})
