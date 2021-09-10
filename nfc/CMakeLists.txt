#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrfxlib_calculate_lib_path(lib_path)

nrfxlib_calculate_lib_path(NFC_LIB_DIR
  BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  SOFT_FLOAT_FALLBACK
)

if(NOT EXISTS ${NFC_LIB_DIR})
  message(WARNING "This combination of SoC and floating point ABI is not supported by the nfc lib."
                  "(${NFC_LIB_DIR} doesn't exist.)")
endif()

zephyr_include_directories(include)
zephyr_sources(src/nfc_nrfx_error_to_nfc.c)

if (NOT CONFIG_SOC_SERIES)
	message(WARNING "CONFIG_SOC_SERIES must be specified.")
endif()

if(CONFIG_NFC_T2T_NRFXLIB)
	zephyr_link_libraries(${NFC_LIB_DIR}/libnfc_t2t.a)
endif()

if(CONFIG_NFC_T4T_NRFXLIB)
	zephyr_link_libraries(${NFC_LIB_DIR}/libnfc_t4t.a)
endif()

if (NOT CONFIG_NFC_T2T_NRFXLIB AND NOT CONFIG_NFC_T4T_NRFXLIB)
	message(WARNING "One of CONFIG_NFC_T2T_NRFXLIB or CONFIG_NFC_T4T_NRFXLIB must be specified.")
endif()
