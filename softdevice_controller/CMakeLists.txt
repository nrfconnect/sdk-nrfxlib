#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if(CONFIG_BT_LL_SOFTDEVICE OR
   CONFIG_BT_LL_SOFTDEVICE_VS_INCLUDE)
  zephyr_include_directories(include)
endif()

if(CONFIG_BT_LL_SOFTDEVICE)
  nrfxlib_calculate_lib_path(lib_path)

  set(SOFTDEVICE_CONTROLLER_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path})

  if(NOT EXISTS ${SOFTDEVICE_CONTROLLER_LIB_PATH})
    message(WARNING "This combination of SoC and floating point ABI is not supported by the SoftDevice Controller lib."
      "(${SOFTDEVICE_CONTROLLER_LIB_PATH} doesn't exist.)")
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
    ${SOFTDEVICE_CONTROLLER_LIB_PATH}/libsoftdevice_controller_${softdevice_controller_variant}.a)
  zephyr_link_libraries(${SOFTDEVICE_CONTROLLER_LIB})

endif()
