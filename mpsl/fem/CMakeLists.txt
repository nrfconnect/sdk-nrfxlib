# Copyright (c) 2023 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

function(mpsl_fem_append_lib fem_lib_name)
    if (CONFIG_SOC_SERIES_NRF54HX OR CONFIG_SOC_SERIES_NRF54LX)
      nrfxlib_calculate_lib_path(LIB_DIR SOC_MODE
        BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${fem_lib_name}
        SOFT_FLOAT_FALLBACK
      )
    else ()
      nrfxlib_calculate_lib_path(LIB_DIR
        BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${fem_lib_name}
        SOFT_FLOAT_FALLBACK
      )
    endif()

    if(LIB_DIR)
      zephyr_include_directories(${fem_lib_name}/include)
      zephyr_link_libraries(${LIB_DIR}/libmpsl_fem_${fem_lib_name}.a)
    endif()
endfunction()

FILE(GLOB subdirlist RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# call mpsl_fem_append_lib for each subdirectory
FOREACH(subdir ${subdirlist})
  IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    mpsl_fem_append_lib(${subdir})
  ENDIF()
ENDFOREACH()

zephyr_include_directories(include)
zephyr_include_directories(include/protocol)
