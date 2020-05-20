#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

# Header guard
if(__NRFXLIB_COMMON_CMAKE__)
  return()
endif()
set(__NRFXLIB_COMMON_CMAKE__ TRUE)

#
# Build the path of the library taking in
# -Arch type
# -Float ABI
# -wchar_t size
#
function(nrfxlib_calculate_lib_path lib_path)
  cmake_parse_arguments(CALC_LIB_PATH "SOC_MODE" "" "" ${ARGN})

  if(${CALC_LIB_PATH_SOC_MODE})
    # CMake regex does not support {4}
    string(REGEX REPLACE "_[a-zA-Z][a-zA-Z][a-zA-Z][a-zA-Z]$" "" arch_soc_dir ${CONFIG_SOC})
  else()
    # Add Arch type
    assert(GCC_M_CPU "GCC_M_CPU must be set to find correct lib.")
    set(arch_soc_dir ${GCC_M_CPU})
  endif()

  # Set floating ABI
  if(CONFIG_FPU)
    if(CONFIG_FP_HARDABI)
        set(float_dir hard-float)
    elseif(CONFIG_FP_SOFTABI)
        set(float_dir softfp-float)
    else()
        assert(0 "Unreachable code")
    endif()
  else()
	  set(float_dir soft-float)
  endif()
  list(FIND COMPILER_OPT_AS_LIST "-fshort-wchar" SHORT_WCHAR_INDEX)
  if (NOT (SHORT_WCHAR_INDEX EQUAL -1))
    set(short_wchar "/short-wchar")
  endif()
  set(${lib_path} "lib/${arch_soc_dir}/${float_dir}${short_wchar}" PARENT_SCOPE)
endfunction()
