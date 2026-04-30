# Copyright (c) 2023 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

function(mpsl_fem_append_lib fem_lib_name)
    set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${fem_lib_name}/lib/${CONFIG_MPSL_LIB_DIR}/${CONFIG_MPSL_LIB_FLOAT_ABI_DIR}")

    if(LIB_DIR)
      zephyr_include_directories(${fem_lib_name}/include)
      zephyr_link_libraries(${LIB_DIR}/libmpsl_fem_${fem_lib_name}.a)
    endif()
endfunction()

zephyr_include_directories(include)
zephyr_include_directories(include/protocol)

mpsl_fem_append_lib(common)

if (CONFIG_MPSL_FEM_NRF21540_GPIO)
  mpsl_fem_append_lib(nrf21540_gpio)
elseif (CONFIG_MPSL_FEM_NRF21540_GPIO_SPI)
  mpsl_fem_append_lib(nrf21540_gpio_spi)
elseif (CONFIG_MPSL_FEM_SIMPLE_GPIO)
  mpsl_fem_append_lib(simple_gpio)
elseif (CONFIG_MPSL_FEM_NRF2220)
  mpsl_fem_append_lib(nrf2220)
  mpsl_fem_append_lib(nrf22xx)
elseif (CONFIG_MPSL_FEM_NRF2240)
  mpsl_fem_append_lib(nrf2240)
  mpsl_fem_append_lib(nrf22xx)
else()
  message(FATAL_ERROR "No FEM library selected")
endif()
