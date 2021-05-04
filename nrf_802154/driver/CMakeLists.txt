#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: BSD-3-Clause
#

target_include_directories(nrf-802154-driver-interface INTERFACE include)

add_library(nrf-802154-driver STATIC EXCLUDE_FROM_ALL)

target_include_directories(nrf-802154-driver
  PRIVATE
    src
)

target_sources(nrf-802154-driver
  PRIVATE
    src/nrf_802154.c
    src/nrf_802154_core.c
    src/nrf_802154_core_hooks.c
    src/nrf_802154_critical_section.c
    src/nrf_802154_debug.c
    src/nrf_802154_debug_assert.c
    src/nrf_802154_pib.c
    src/nrf_802154_peripherals_alloc.c
    src/nrf_802154_queue.c
    src/nrf_802154_rssi.c
    src/nrf_802154_rx_buffer.c
    src/nrf_802154_stats.c
    src/nrf_802154_swi.c
    src/nrf_802154_trx.c
    src/mac_features/nrf_802154_csma_ca.c
    src/mac_features/nrf_802154_delayed_trx.c
    src/mac_features/nrf_802154_filter.c
    src/mac_features/nrf_802154_frame_parser.c
    src/mac_features/nrf_802154_ie_writer.c
    src/mac_features/nrf_802154_ifs.c
    src/mac_features/nrf_802154_security_pib_ram.c
    src/mac_features/nrf_802154_precise_ack_timeout.c
    src/mac_features/ack_generator/nrf_802154_ack_data.c
    src/mac_features/ack_generator/nrf_802154_ack_generator.c
    src/mac_features/ack_generator/nrf_802154_enh_ack_generator.c
    src/mac_features/ack_generator/nrf_802154_imm_ack_generator.c
    src/platform/temperature/nrf_802154_temperature_none.c
)

if (NRF52_SERIES)
  target_sources(nrf-802154-driver PRIVATE src/nrf_802154_trx_ppi.c)
elseif (NRF53_SERIES)
  target_sources(nrf-802154-driver PRIVATE src/nrf_802154_trx_dppi.c)
endif()

if (SL_OPENSOURCE)
  target_sources(nrf-802154-driver
    PRIVATE
      src/nrf_802154_notification_direct.c
      src/nrf_802154_request_direct.c
  )
else ()
  target_sources(nrf-802154-driver
    PRIVATE
      src/nrf_802154_notification_swi.c
      src/nrf_802154_priority_drop_swi.c
      src/nrf_802154_request_swi.c
  )
endif ()

if (SL_OPENSOURCE OR NRF53_SERIES)
  target_compile_definitions(nrf-802154-driver-interface
    INTERFACE
      # Disable Frame Timestamps
      NRF_802154_FRAME_TIMESTAMP_ENABLED=0
      # Disable DTRX
      NRF_802154_DELAYED_TRX_ENABLED=0
      # Disable IFS
      NRF_802154_IFS_ENABLED=0
      # Disable IE writer
      NRF_802154_IE_WRITER_ENABLED=0
  )
else()
  target_compile_definitions(nrf-802154-driver-interface
    INTERFACE
      # Enable Frame Timestamps
      NRF_802154_FRAME_TIMESTAMP_ENABLED=1
      # Enable DTRX
      NRF_802154_DELAYED_TRX_ENABLED=1
      # Enable IFS
      NRF_802154_IFS_ENABLED=1
  )
endif()

target_link_libraries(nrf-802154-driver
  PUBLIC
    nrf-802154-driver-interface
    nrf-802154-platform
    nrf-802154-sl
)
