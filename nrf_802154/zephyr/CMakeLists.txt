#
# Copyright (c) 2026 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_802154_SOURCE_NRFXLIB)
  if(CONFIG_NRF_802154_RADIO_DRIVER OR CONFIG_NRF_802154_SERIALIZATION)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/nrf_802154_zephyr.cmake)

    nrf_802154_zephyr_add_platform_glue(${CMAKE_CURRENT_SOURCE_DIR}/../sl/platform/zephyr)
    nrf_802154_zephyr_apply_interface_defs()

    set(NRF53_SERIES  ${CONFIG_SOC_SERIES_NRF53})
    set(SER_HOST      ${CONFIG_NRF_802154_SER_HOST})
    set(SL_OPENSOURCE ${CONFIG_NRF_802154_SL_OPENSOURCE})

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ nrf_802154)

    target_link_libraries(nrf-802154-driver-interface INTERFACE zephyr-802154-interface)
    target_link_libraries(nrf-802154-serialization-interface INTERFACE zephyr-802154-interface)
  endif(CONFIG_NRF_802154_RADIO_DRIVER OR CONFIG_NRF_802154_SERIALIZATION)
endif(CONFIG_NRF_802154_SOURCE_NRFXLIB)
