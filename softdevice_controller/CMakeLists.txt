#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(CONFIG_BT_LL_SOFTDEVICE OR
   CONFIG_BT_LL_SOFTDEVICE_VS_INCLUDE)
  zephyr_include_directories(include)
endif()

if(CONFIG_BT_LL_SOFTDEVICE)
  nrfxlib_calculate_lib_path(SOFTDEVICE_CONTROLLER_LIB_DIR
    BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    SOFT_FLOAT_FALLBACK
  )

  if(NOT SOFTDEVICE_CONTROLLER_LIB_DIR)
    message(WARNING "This combination of SoC and floating point ABI is not"
      "supported by the SoftDevice Controller lib.")
  endif()

  if(CONFIG_SOFTDEVICE_CONTROLLER_PERIPHERAL)
    set(softdevice_controller_variant peripheral)
  elseif(CONFIG_SOFTDEVICE_CONTROLLER_CENTRAL)
    set(softdevice_controller_variant central)
  elseif(CONFIG_SOFTDEVICE_CONTROLLER_MULTIROLE)
    set(softdevice_controller_variant multirole)
  else()
    message(WARNING "No SoftDevice Controller variant selected")
  endif()

  set(SOFTDEVICE_CONTROLLER_LIB
    ${SOFTDEVICE_CONTROLLER_LIB_DIR}/libsoftdevice_controller_${softdevice_controller_variant}.a)
  zephyr_link_libraries(${SOFTDEVICE_CONTROLLER_LIB})

endif()
