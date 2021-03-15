#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: BSD-3-Clause
#

target_include_directories(nrf-802154-serialization-interface
  INTERFACE
    include
    include/platform
    include/serialization
)

add_library(nrf-802154-serialization STATIC EXCLUDE_FROM_ALL)

target_include_directories(nrf-802154-serialization
  PRIVATE
    src/include
)

target_sources(nrf-802154-serialization
  PRIVATE
    spinel_base/spinel.c
    src/nrf_802154_buffer_allocator.c
    src/nrf_802154_buffer_mgr_dst.c
    src/nrf_802154_buffer_mgr_src.c
    src/nrf_802154_kvmap.c
    src/nrf_802154_spinel.c
    src/nrf_802154_spinel_dec.c
)

if (SER_HOST)
  target_include_directories(nrf-802154-serialization-interface
    INTERFACE
      include/host
  )
  target_sources(nrf-802154-serialization
    PRIVATE
      src/nrf_802154_spinel_app.c
      src/nrf_802154_spinel_dec_app.c
  )
else ()
  target_sources(nrf-802154-serialization
    PRIVATE
      src/nrf_802154_spinel_net.c
      src/nrf_802154_spinel_dec_net.c
  )
endif()

target_link_libraries(nrf-802154-serialization
  PRIVATE
    nrf-802154-serialization-interface
    nrf-802154-platform
)
