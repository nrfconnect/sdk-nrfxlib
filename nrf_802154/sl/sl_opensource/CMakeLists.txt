#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: BSD-3-Clause
#

target_include_directories(nrf-802154-driver-interface INTERFACE include)

add_library(nrf-802154-sl STATIC EXCLUDE_FROM_ALL)

target_link_libraries(nrf-802154-sl
  PRIVATE
    nrf-802154-driver-interface
    nrf-802154-platform
)

target_sources(nrf-802154-sl
  PRIVATE
    src/nrf_802154_sl_ant_div.c
    src/nrf_802154_sl_capabilities.c
    src/nrf_802154_sl_coex.c
    src/nrf_802154_sl_fem.c
    src/nrf_802154_sl_log.c
    src/nrf_802154_sl_rsch.c
    src/nrf_802154_sl_timer.c
)
