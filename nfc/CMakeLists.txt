#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

include(${NRFXLIB_DIR}/common.cmake)

nrfxlib_calculate_lib_path(lib_path)

set(NFC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

if(NOT EXISTS ${NFC_LIB_PATH})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nfc lib."
                  "(${NFC_LIB_PATH} doesn't exist.)")
endif()

zephyr_include_directories(include)

if (NOT CONFIG_SOC_SERIES)
	message(WARNING "CONFIG_SOC_SERIES must be specified.")
endif()

if(CONFIG_NFC_T2T_NRFXLIB)
	zephyr_link_libraries(${NFC_LIB_PATH}/libnfct2t_${CONFIG_SOC_SERIES}.a)
endif()

if(CONFIG_NFC_T4T_NRFXLIB)
	zephyr_link_libraries(${NFC_LIB_PATH}/libnfct4t_${CONFIG_SOC_SERIES}.a)
endif()

if (NOT CONFIG_NFC_T2T_NRFXLIB AND NOT CONFIG_NFC_T4T_NRFXLIB)
	message(WARNING "One of CONFIG_NFC_T2T_NRFXLIB or CONFIG_NFC_T4T_NRFXLIB must be specified.")
endif()
