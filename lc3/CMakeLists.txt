#
# Copyright (c) 2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_sources(
        src/sw_codec_lc3.c
        platform/os/baremetal/src/osal.c
)

zephyr_include_directories(
        codec/inc
        platform/os/inc
        platform/os/baremetal/inc
        include
)

nrfxlib_calculate_lib_path(LC3_LIB_DIR BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT LC3_LIB_DIR)
  message(WARNING "This combination of SoC and floating point ABI is not supported by the LC3 lib.")
endif()

target_link_libraries(app PRIVATE ${LC3_LIB_DIR}/libLC3.a)
