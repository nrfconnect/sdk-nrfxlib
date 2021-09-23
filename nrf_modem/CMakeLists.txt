#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_NRF_MODEM_LINK_BINARY)

  nrfxlib_calculate_lib_path(lib_path)

  set(NRF_MODEM_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

  if(NOT EXISTS ${NRF_MODEM_PATH})
     message(ERROR " This combination of SoC and floating point ABI is not supported by libmodem"
                   " (${NRF_MODEM_PATH} doesn't exist.)")
  endif()

  if(CONFIG_NRF_MODEM_LOG)
    zephyr_library_import(modem ${NRF_MODEM_PATH}/libmodem_log.a)
  else()
    zephyr_library_import(modem ${NRF_MODEM_PATH}/libmodem.a)
  endif()

  zephyr_include_directories(include)

endif()
