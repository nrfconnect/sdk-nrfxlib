#
# Copyright (c) 2019 - 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
include(common.cmake)

set(NRFXLIB_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "NRFXLIB root directory")
if (CONFIG_NFC_T2T_NRFXLIB OR
    CONFIG_NFC_T4T_NRFXLIB)
	add_subdirectory(nfc)
endif()

add_subdirectory_ifdef(CONFIG_NRF_MODEM               nrf_modem)
add_subdirectory_ifdef(CONFIG_NRFXLIB_CRYPTO          crypto)
add_subdirectory_ifdef(CONFIG_BT                      softdevice_controller)
add_subdirectory_ifdef(CONFIG_MPSL                    mpsl)
add_subdirectory_ifdef(CONFIG_NET_L2_OPENTHREAD       openthread)
add_subdirectory_ifdef(CONFIG_NRF_RPC                 nrf_rpc)
add_subdirectory_ifdef(CONFIG_ZIGBEE                  zboss)
add_subdirectory_ifdef(CONFIG_GZLL                    gzll)
add_subdirectory_ifdef(CONFIG_NRF_DM                  nrf_dm)
add_subdirectory_ifdef(CONFIG_NRF_FUEL_GAUGE          nrf_fuel_gauge)
add_subdirectory_ifdef(CONFIG_SW_CODEC_LC3_T2_SOFTWARE lc3)
