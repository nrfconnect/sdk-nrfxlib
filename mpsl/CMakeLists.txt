#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrfxlib_calculate_lib_path(lib_path)

set(MPSL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${MPSL_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the MPSL lib."
                  "(${MPSL_LIB_PATH} doesn't exist.)")
endif()

zephyr_include_directories(include)
zephyr_include_directories(include/protocol)
zephyr_link_libraries(${MPSL_LIB_PATH}/libmpsl.a)
