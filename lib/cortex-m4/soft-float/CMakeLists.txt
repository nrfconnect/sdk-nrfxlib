#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: BSD-5-Clause-Nordic
#

add_subdirectory_ifdef(CONFIG_NRFXLIB_NFC nfc)
add_subdirectory_ifdef(CONFIG_NRFXLIB_BLE ble)
