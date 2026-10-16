#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

if(NOT NRFXLIB_MPSL_PATH)
  set(NRFXLIB_MPSL_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if (CONFIG_MPSL_BUILD_TYPE_LIB)
  add_subdirectory(fem)
  set(MPSL_LIB_DIR "${NRFXLIB_MPSL_PATH}/${NRFXLIB_COMPILER_VARIANT_PATH}/lib/${CONFIG_MPSL_LIB_DIR}/${CONFIG_MPSL_LIB_FLOAT_ABI_DIR}")

  if(NOT EXISTS "${MPSL_LIB_DIR}/libmpsl.a")
    message(FATAL_ERROR "This combination of SoC and floating point ABI is not supported by the MPSL lib.")
  endif()

  zephyr_include_directories(${NRFXLIB_MPSL_PATH}/include)
  zephyr_include_directories(${NRFXLIB_MPSL_PATH}/include/protocol)
  zephyr_link_libraries(${MPSL_LIB_DIR}/libmpsl.a)
endif()
