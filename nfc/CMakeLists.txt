#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

set(NFC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${GCC_M_CPU}/${float_dir})

if(NOT EXISTS ${NFC_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nfc lib."
                  "(${NFC_LIB_PATH} doesn't exist.)")
endif()

zephyr_library()
zephyr_include_directories(include)
zephyr_library_sources(src/nfc_platform_zephyr.c)

if(CONFIG_NFC_T2T_LIB_ENABLED)
	zephyr_link_libraries(${NFC_LIB_PATH}/libnfct2t_nrf52.a)
endif()

if(CONFIG_NFC_T4T_LIB_ENABLED)
	zephyr_link_libraries(${NFC_LIB_PATH}/libnfct4t_nrf52.a)
endif()

if (NOT CONFIG_NFC_T2T_LIB_ENABLED AND NOT CONFIG_NFC_T4T_LIB_ENABLED)
	message(WARNING "One of CONFIG_NFC_T2T_LIB_ENABLED or CONFIG_NFC_T4T_LIB_ENABLED must be specified.")
endif()
