#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

set(BLE_CONTROLLER_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${BLE_CONTROLLER_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the ble controller lib."
                  "(${BLE_CONTROLLER_LIB_PATH} doesn't exist.)")
endif()

if(CONFIG_BLE_CONTROLLER_S112)
	set(ble_controller_variant s112)
elseif(CONFIG_BLE_CONTROLLER_S132)
	set(ble_controller_variant s132)
elseif(CONFIG_BLE_CONTROLLER_S140)
	set(ble_controller_variant s140)
else()
	message(WARNING "No BLE Controller variant selected")
endif()

set(ver 0.1.0-2.prealpha)

set(BLE_CONTROLLER_LIB ${BLE_CONTROLLER_LIB_PATH}/libble_controller_${ble_controller_variant}_nrf52_${ver}.a)

zephyr_include_directories(include)
zephyr_link_libraries(${BLE_CONTROLLER_LIB})

