#
# Copyright (c) 2019 - 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

set(NRFXLIB_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "NRFXLIB root directory")
if (CONFIG_NFC_T2T_NRFXLIB OR
    CONFIG_NFC_T4T_NRFXLIB)
	add_subdirectory(nfc)
endif()

add_subdirectory_ifdef(CONFIG_BT                      ble_controller)
add_subdirectory_ifdef(CONFIG_MPSL                    mpsl)
add_subdirectory_ifdef(CONFIG_BSD_LIBRARY             bsdlib)
add_subdirectory_ifdef(CONFIG_NRFXLIB_CRYPTO          crypto)
add_subdirectory_ifdef(CONFIG_NORDIC_SECURITY_BACKEND nrf_security)
add_subdirectory_ifdef(CONFIG_ZIGBEE                  zboss)
