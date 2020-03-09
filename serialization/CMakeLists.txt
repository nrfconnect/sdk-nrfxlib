#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

zephyr_include_directories(include)

zephyr_library()
zephyr_library_sources(rp_ser.c)

zephyr_link_libraries(TINYCBOR)

zephyr_linker_sources(SECTIONS rp_ser.ld)
