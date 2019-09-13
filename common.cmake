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
  # Add Arch type
  assert(GCC_M_CPU "GCC_M_CPU must be set to find correct lib.")
  # Set floating ABI
  if(CONFIG_FLOAT)
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
  if (CONFIG_SHORT_WCHAR_T)
    set(short_wchar "/short-wchar")
  endif()
  set(${lib_path} "lib/${GCC_M_CPU}/${float_dir}${short_wchar}" PARENT_SCOPE)
endfunction()
