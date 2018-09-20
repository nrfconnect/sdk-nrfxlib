#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: BSD-5-Clause-Nordic
#

if (${CONFIG_FLOAT})
	set(NFC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/hard-float)
else ()
	set(NFC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/soft-float)
endif()

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_NFC_T2T_HAL_ENABLED src/hal_nfc_t2t.c)
zephyr_link_libraries_ifdef(CONFIG_NFC_T2T_HAL_ENABLED
			    ${NFC_LIB_PATH}/libnfct2t_nrf52.a)

zephyr_library_sources_ifdef(CONFIG_NFC_T4T_HAL_ENABLED src/hal_nfc_t4t.c)
zephyr_link_libraries_ifdef(CONFIG_NFC_T4T_HAL_ENABLED
			    ${NFC_LIB_PATH}/libnfct4t_nrf52.a)
