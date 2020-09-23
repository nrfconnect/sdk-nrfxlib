#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

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

function(get_mbedtls_dir ARM_MBEDTLS_PATH)
  if(EXISTS ${ARM_MBEDTLS_PATH})
  # Do nothing, just use the provided path
  elseif(WEST)
  ## Use `west` to find the mbedtls tree we use (this is not the
  ## mbedtls tree distributed as a zephyr module).
    execute_process(
      COMMAND ${PYTHON_EXECUTABLE} -c
      "from west.manifest import Manifest; manifest = Manifest.from_file(); print(manifest.get_projects(['mbedtls-nrf'])[0].posixpath)"
      OUTPUT_VARIABLE west_project_output
      RESULT_VARIABLE result
    )
    string(REGEX REPLACE "[\r\n]+" "" ${ARM_MBEDTLS_PATH} "${west_project_output}")
    if(${result})
      message(FATAL_ERROR "Failed to find mbedtls, cannot build security libraries")
    endif()
  else()
    message(FATAL_ERROR "west not installed, please provide ARM_MBEDTLS_PATH to CMake to support security libraries")
  endif()

  set(ARM_MBEDTLS_PATH ${ARM_MBEDTLS_PATH} PARENT_SCOPE)
endfunction()
