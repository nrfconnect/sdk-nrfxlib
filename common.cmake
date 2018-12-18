# Header guard
if(__NRFXLIB_COMMON_CMAKE__)
  return()
endif()
set(__NRFXLIB_COMMON_CMAKE__ TRUE)

function(nrfxlib_calculate_lib_path lib_path)
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

  assert(GCC_M_CPU "GCC_M_CPU must be set to find correct lib.")

  set(${lib_path} lib/${GCC_M_CPU}/${float_dir} PARENT_SCOPE)
endfunction()
