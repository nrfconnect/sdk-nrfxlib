#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# This file is included by Zephyr's zephyr_module CMake stage, which runs
# before the dts stage. nrf71_wifi's own zephyr/CMakeLists.txt is instead
# add_subdirectory()-ed after dts/kconfig are already finalized, so it is
# too late to influence which devicetree overlay gets applied. Registering
# nrfxlib itself as a MODULE_EXT_ROOT gives it this earlier hook, letting
# the nRF7120 Wi-Fi ROM patch region overlay below be pulled in without any
# board, sysbuild, or CLI changes.
#
# BOARD itself is not yet resolved for sysbuild image sub-builds at this
# stage (that happens later, in the "boards" CMake module), so read it
# straight out of the per-image sysbuild cache file sysbuild already points
# us at. A plain top-level (non-sysbuild) build has no SYSBUILD_CACHE and
# BOARD is a normal cache variable by this point instead.
set(patch_board)
if(DEFINED CACHE{SYSBUILD_CACHE} AND EXISTS "$CACHE{SYSBUILD_CACHE}")
  file(STRINGS "$CACHE{SYSBUILD_CACHE}" patch_board_line REGEX "^BOARD:")
  string(REGEX REPLACE "^BOARD:[^=]*=" "" patch_board "${patch_board_line}")
elseif(DEFINED CACHE{BOARD})
  set(patch_board $CACHE{BOARD})
endif()

# The SoC is not resolved as its own variable this early either (the "soc"
# CMake module only runs after dts/kconfig/arch), but Zephyr's board target
# format already embeds it as the second "/"-separated field of BOARD, e.g.
# nrf7120dk/nrf7120/cpuapp, so pull it out of the string directly instead of
# matching on the board name (which would miss any other board built around
# the same SoC).
set(patch_soc)
if(patch_board MATCHES "^[^/]+/([^/]+)")
  set(patch_soc "${CMAKE_MATCH_1}")
endif()

if(patch_soc MATCHES "^nrf7120" AND
   NOT APPLICATION_SOURCE_DIR MATCHES "/bootloader/mcuboot(/|$)")
  list(APPEND EXTRA_DTC_OVERLAY_FILE
    ${CMAKE_CURRENT_LIST_DIR}/../nrf71_wifi/bins/0.1.0/nrf7120_wifi_patch.dtsi
  )
endif()
unset(patch_board)
unset(patch_soc)
