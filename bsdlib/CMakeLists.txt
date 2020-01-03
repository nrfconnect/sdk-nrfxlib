#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path)

set(BSD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${BSD_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by bsdlib."
                  "(${BSD_LIB_PATH} doesn't exist.)")
endif()

# Link in bsdlib and libc.
# libc is linked in because it is used by bsdlib
# and must therefore be placed after bsdlib on the linker's command line.
set(BSDLIB_TARGET bsd_nrf9160_xxaa)
zephyr_library_import(${BSDLIB_TARGET} ${BSD_LIB_PATH}/libbsd_nrf9160_xxaa.a)

zephyr_include_directories(include)
