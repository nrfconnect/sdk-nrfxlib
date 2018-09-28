#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if (${CONFIG_FLOAT})
	if(CONFIG_FP_HARDABI)
		set(float_abi hard)
	elseif(CONFIG_FP_SOFTABI)
		set(float_abi softfp)
	else()
		assert(0 "Unreachable code")
	endif()
else ()
	set(float_abi soft)
endif()

set(float_dir ${float_abi}-float)

assert(GCC_M_CPU "GCC_M_CPU is not set")

set(BLE_CONTROLLER_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${GCC_M_CPU}/${float_dir})

if(CONFIG_BLE_CONTROLLER_S112)
	set(ble_controller_variant s112)
elseif(CONFIG_BLE_CONTROLLER_S132)
	set(ble_controller_variant s132)
elseif(CONFIG_BLE_CONTROLLER_S140)
	set(ble_controller_variant s140)
else()
	message(FATAL_ERROR "No BLE Controller variant selected")
endif()

set(ver 0.1.0-1.prealpha)

zephyr_include_directories(include)
zephyr_link_libraries(${BLE_CONTROLLER_LIB_PATH}/libble_controller_${ble_controller_variant}_nrf52_${ver}.a)

