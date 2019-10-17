#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

set(NRFXLIB_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "NRFXLIB root directory")
add_subdirectory_ifdef(CONFIG_NRFXLIB_NFC             nfc)
add_subdirectory_ifdef(CONFIG_BT_LL_NRFXLIB           ble_controller)
add_subdirectory_ifdef(CONFIG_MPSL                    mpsl)
add_subdirectory_ifdef(CONFIG_BSD_LIBRARY             bsdlib)
add_subdirectory_ifdef(CONFIG_NRFXLIB_CRYPTO          crypto)
add_subdirectory_ifdef(CONFIG_NORDIC_SECURITY_BACKEND nrf_security)
