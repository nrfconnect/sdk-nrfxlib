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

  if(CONFIG_SOFTDEVICE_CONTROLLER_S112)
    set(softdevice_controller_variant s112)
  elseif(CONFIG_SOFTDEVICE_CONTROLLER_S132)
    set(softdevice_controller_variant s132)
  elseif(CONFIG_SOFTDEVICE_CONTROLLER_S140)
    set(softdevice_controller_variant s140)
  else()
    message(WARNING "No SoftDevice Controller variant selected")
  endif()

  set(SOFTDEVICE_CONTROLLER_LIB
    ${SOFTDEVICE_CONTROLLER_LIB_PATH}/libsoftdevice_controller_${softdevice_controller_variant}.a)
  zephyr_link_libraries(${SOFTDEVICE_CONTROLLER_LIB})

endif()
