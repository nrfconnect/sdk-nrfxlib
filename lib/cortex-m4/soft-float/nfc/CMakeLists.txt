#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: BSD-5-Clause-Nordic
#

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_NFC_T2T_HAL_ENABLED hal_nfc_t2t.c)
zephyr_link_libraries_ifdef(CONFIG_NFC_T2T_HAL_ENABLED
			    ${CMAKE_CURRENT_SOURCE_DIR}/libnfct2t_nrf52.a)

zephyr_library_sources_ifdef(CONFIG_NFC_T4T_HAL_ENABLED hal_nfc_t4t.c)
zephyr_link_libraries_ifdef(CONFIG_NFC_T4T_HAL_ENABLED
			    ${CMAKE_CURRENT_SOURCE_DIR}/libnfct4t_nrf52.a)
