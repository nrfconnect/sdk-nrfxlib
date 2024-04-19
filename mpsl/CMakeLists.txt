#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if (CONFIG_MPSL_BUILD_TYPE_LIB)
  add_subdirectory(fem)
  set(MPSL_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/${CONFIG_MPSL_LIB_DIR}/${CONFIG_MPSL_LIB_FLOAT_ABI_DIR}")

  if(NOT EXISTS "${MPSL_LIB_DIR}/libmpsl.a")
    message(FATAL_ERROR "This combination of SoC and floating point ABI is not supported by the MPSL lib.")
  endif()

  zephyr_include_directories(include)
  zephyr_include_directories(include/protocol)
  zephyr_link_libraries(${MPSL_LIB_DIR}/libmpsl.a)
endif()
