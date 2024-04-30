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

FILE(GLOB subdirlist RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# call mpsl_fem_append_lib for each subdirectory
FOREACH(subdir ${subdirlist})
  IF((IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}) AND NOT (${subdir} STREQUAL "include"))
    mpsl_fem_append_lib(${subdir})
  ENDIF()
ENDFOREACH()

zephyr_include_directories(include)
zephyr_include_directories(include/protocol)
