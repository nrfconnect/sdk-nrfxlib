#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if (CONFIG_BT_LL_SOFTDEVICE_HEADERS_INCLUDE)
  zephyr_interface_library_named (SOFTDEVICE_CONTROLLER_LIB_HEADERS)
  target_include_directories(SOFTDEVICE_CONTROLLER_LIB_HEADERS
    INTERFACE include)
  zephyr_link_libraries(INTERFACE SOFTDEVICE_CONTROLLER_LIB_HEADERS)
endif()

if(CONFIG_BT_LL_SOFTDEVICE AND CONFIG_BT_LL_SOFTDEVICE_BUILD_TYPE_LIB)
  if (CONFIG_SOC_SERIES_NRF54HX OR CONFIG_SOC_SERIES_NRF54LX)
    nrfxlib_calculate_lib_path(SOFTDEVICE_CONTROLLER_LIB_DIR SOC_MODE
      BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
      SOFT_FLOAT_FALLBACK
    )
  else()
    nrfxlib_calculate_lib_path(SOFTDEVICE_CONTROLLER_LIB_DIR
      BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
      SOFT_FLOAT_FALLBACK
    )
  endif()

  if(NOT SOFTDEVICE_CONTROLLER_LIB_DIR)
    message(WARNING "This combination of SoC and floating point ABI is not"
      "supported by the SoftDevice Controller lib.")
  endif()

  if(CONFIG_BT_LL_SOFTDEVICE_PERIPHERAL)
    set(softdevice_controller_variant peripheral)
  elseif(CONFIG_BT_LL_SOFTDEVICE_CENTRAL)
    set(softdevice_controller_variant central)
  elseif(CONFIG_BT_LL_SOFTDEVICE_MULTIROLE)
    set(softdevice_controller_variant multirole)
  else()
    message(WARNING "No SoftDevice Controller variant selected")
  endif()

  set(SOFTDEVICE_CONTROLLER_LIB
    ${SOFTDEVICE_CONTROLLER_LIB_DIR}/libsoftdevice_controller_${softdevice_controller_variant}.a)
  zephyr_link_libraries(${SOFTDEVICE_CONTROLLER_LIB})
endif()
