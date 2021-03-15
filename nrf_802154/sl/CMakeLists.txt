#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: BSD-3-Clause
#

target_include_directories(nrf-802154-driver-interface INTERFACE include)

if (SL_OPENSOURCE)
  add_subdirectory(sl_opensource)
elseif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sl/CMakeLists.txt)
  add_subdirectory(sl)
endif ()
