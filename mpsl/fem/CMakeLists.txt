# Copyright (c) 2023 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

function(mpsl_fem_append_lib fem_lib_name fem_folder_name)
  set(lib_dir_path "${CMAKE_CURRENT_SOURCE_DIR}/${fem_lib_name}/lib/${fem_folder_name}")

  if(EXISTS ${lib_dir_path})
    zephyr_include_directories(${fem_lib_name}/include)
    zephyr_link_libraries(${lib_dir_path}/libmpsl_fem_${fem_lib_name}.a)
  endif()
endfunction()

if(CONFIG_SOC_SERIES_BSIM_NRFXX OR CONFIG_SOC_SERIES_NRF52 OR CONFIG_SOC_NRF5340_CPUNET OR
  CONFIG_SOC_NRF54H20_CPURAD OR CONFIG_SOC_SERIES_NRF54L OR CONFIG_SOC_SERIES_NRF71
)
  if(CONFIG_SOC_SERIES_NRF52)
    set(lib_dir_path nrf52)
  elseif(CONFIG_SOC_NRF5340_CPUNET)
    set(lib_dir_path nrf53)
  elseif(CONFIG_SOC_NRF54H20_CPURAD)
    set(lib_dir_path nrf54h)
  elseif(CONFIG_SOC_NRF54LS05B OR CONFIG_SOC_NRF54LS05A)
    set(lib_dir_path nrf54ls)
  elseif(CONFIG_SOC_NRF54LM20A OR CONFIG_SOC_NRF54LM20B)
    set(lib_dir_path nrf54lm)
  elseif(CONFIG_SOC_NRF54LV10A OR CONFIG_SOC_NRF54LC10A)
    set(lib_dir_path nrf54lv)
  elseif(CONFIG_SOC_SERIES_NRF54L)
    set(lib_dir_path nrf54l)
  elseif(CONFIG_SOC_SERIES_NRF71)
    set(lib_dir_path nrf71)
  elseif(CONFIG_SOC_SERIES_BSIM_NRF52X)
    set(lib_dir_path nrf52_bsim)
  elseif(CONFIG_SOC_SERIES_BSIM_NRF53X)
    set(lib_dir_path nrf53_bsim)
  elseif(CONFIG_SOC_SERIES_BSIM_NRF54LX)
    set(lib_dir_path nrf54l_bsim)
  endif()

  if(CONFIG_TRUSTED_EXECUTION_NONSECURE)
    set(lib_dir_path ${lib_dir_path}_ns)
  endif()

  if(CONFIG_FPU AND CONFIG_FP_HARDABI)
    set(lib_dir_path ${lib_dir_path}/hard-float)
  elseif(CONFIG_FPU AND CONFIG_FP_SOFTABI)
    set(lib_dir_path ${lib_dir_path}/softfp-float)
  else()
    set(lib_dir_path ${lib_dir_path}/soft-float)
  endif()

  # Call mpsl_fem_append_lib for each subdirectory
  file(GLOB subdirlist RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

  foreach(subdir ${subdirlist})
    if((IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}) AND NOT (${subdir} STREQUAL "include"))
      mpsl_fem_append_lib(${subdir} ${lib_dir_path})
    endif()
  endforeach()
endif()

zephyr_include_directories(include)
zephyr_include_directories(include/protocol)
