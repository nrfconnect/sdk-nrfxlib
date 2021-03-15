#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: BSD-3-Clause
#

add_library(nrf-802154-driver-interface INTERFACE)
add_library(nrf-802154-serialization-interface INTERFACE)

add_subdirectory(driver)
add_subdirectory(sl)

add_subdirectory(serialization)
