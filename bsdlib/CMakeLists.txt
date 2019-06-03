#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path)

set(BSD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${BSD_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the bsd lib."
                  "(${BSD_LIB_PATH} doesn't exist.)")
endif()

# Link in the bsd library, the oberon library, and lib C. Lib C and
# lib oberon are linked in because they are used by the bsd library
# and must therefore be placed after the bsd library on the linker's
# command line.
set(                    BSDLIB_TARGET             ${IMAGE}bsd_nrf9160_xxaa)
zephyr_library_import(${BSDLIB_TARGET} ${BSD_LIB_PATH}/libbsd_nrf9160_xxaa.a)
target_link_libraries(${BSDLIB_TARGET} INTERFACE ${IMAGE}nrfxlib_crypto -lc)

zephyr_include_directories(include)
