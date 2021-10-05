#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_MODEM_LINK_BINARY)

  nrfxlib_calculate_lib_path(NRF_MODEM_LIB_DIR
    BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    SOFT_FLOAT_FALLBACK
  )

  if(NOT NRF_MODEM_LIB_DIR)
     message(ERROR " This combination of SoC and floating point ABI is not supported by libmodem.")
  endif()

  zephyr_library_import(modem ${NRF_MODEM_LIB_DIR}/libmodem.a)
  zephyr_include_directories(include)

endif()
