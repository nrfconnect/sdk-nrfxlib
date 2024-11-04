#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

include_guard(GLOBAL)

# Usage:
#   nrfxlib_calculate_lib_path(<dir> [SOC_MODE] [BASE_DIR <dir> [SOFT_FLOAT_FALLBACK]])
#
# Calculate a library path based on current SoC architecture or SoC name and
# FPU ABI mode.
# The calculated library path is returned in the '<dir>' argument.
#
# If BASE_DIR is specified the calculated path will be converted to an absolute
# path starting from BASE_DIR. The absolute path will be checked for existence
# and if the absolute path does not exists, the returned value will be
# '<dir>-NOTFOUND'.
#
# SOC_MODE:            Use the SoC name instead of the SoC architecture when calculating the path.
#                      For example 'nrf52840' will be used for the path instead of 'cortex-m4'
# NS_PROVIDED          Use name identifying non-secure library version for builds with
#                      'CONFIG_TRUSTED_EXECUTION_NONSECURE' on supported SoCs.
#                      Libraries that do not distinguish between secure and non-secure version
#                      should not use this parameter.
# BASE_DIR:            Base path from where the calculated path should start from.
#                      When specifying BASE_DIR the path returned will be absolute, or
#                      '<dir>-NOTFOUND' if the path does not exists
# SOFT_FLOAT_FALLBACK: Allow soft float library path fallback if softfp-float is the FPU ABI in use
#                      and no 'softfp-float' is found.
#                      This flag requires 'BASE_DIR'
#
function(nrfxlib_calculate_lib_path lib_path)
  cmake_parse_arguments(CALC_LIB_PATH "SOFT_FLOAT_FALLBACK;SOC_MODE;NS_PROVIDED" "BASE_DIR" "" ${ARGN})

  if(CALC_LIB_PATH_SOFT_FLOAT_FALLBACK AND NOT DEFINED CALC_LIB_PATH_BASE_DIR)
    message(WARNING "nrfxlib_calculate_lib_path(SOFT_FLOAT_FALLBACK ...) "
      "depends on argument: BASE_DIR."
    )
  endif()
  if(${CALC_LIB_PATH_SOC_MODE})
    # CMake regex does not support {4}
    string(REGEX REPLACE "_[a-zA-Z][a-zA-Z][a-zA-Z][a-zA-Z]$" "" arch_soc_dir ${CONFIG_SOC})

    # Apply CPU cluster manually for devices that have multiple cores
    if(DEFINED CONFIG_SOC_NRF5340_CPUAPP)
      set(arch_soc_dir ${arch_soc_dir}_cpuapp)
    elseif(DEFINED CONFIG_SOC_NRF5340_CPUNET)
      set(arch_soc_dir ${arch_soc_dir}_cpunet)
    elseif(DEFINED CONFIG_SOC_NRF54L15_CPUAPP OR DEFINED CONFIG_SOC_NRF54L15_ENGA_CPUAPP
           OR DEFINED CONFIG_SOC_NRF54L20_ENGA_CPUAPP)
      set(arch_soc_dir ${arch_soc_dir}_cpuapp)
      if(DEFINED CONFIG_TRUSTED_EXECUTION_NONSECURE AND ${CALC_LIB_PATH_NS_PROVIDED})
        set(arch_soc_dir ${arch_soc_dir}_ns)
      endif()
    elseif(DEFINED CONFIG_SOC_NRF54H20_CPURAD OR DEFINED CONFIG_SOC_NRF54H20_ENGB_CPURAD)
      set(arch_soc_dir ${arch_soc_dir}_cpurad)
    endif()
  elseif(CONFIG_SOC_SERIES_BSIM_NRF52X)
    set(arch_soc_dir "bsim_nrf52")
  elseif(CONFIG_SOC_SERIES_BSIM_NRF53X)
    set(arch_soc_dir "bsim_nrf53")
  elseif(CONFIG_SOC_SERIES_BSIM_NRF54LX)
    set(arch_soc_dir "bsim_nrf54l")
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
      if(CALC_LIB_PATH_SOFT_FLOAT_FALLBACK AND DEFINED CALC_LIB_PATH_BASE_DIR)
        list(APPEND float_dir soft-float)
      endif()
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

  if(DEFINED CALC_LIB_PATH_BASE_DIR)
    foreach(dir ${float_dir})
      set(${lib_path} "${CALC_LIB_PATH_BASE_DIR}/lib/${arch_soc_dir}/${dir}${short_wchar}")
      if(EXISTS ${${lib_path}})
        set(${lib_path} ${${lib_path}} PARENT_SCOPE)
        return()
      endif()
    endforeach()
    set(${lib_path} ${lib_path}-NOTFOUND PARENT_SCOPE)
  else()
    set(${lib_path} "lib/${arch_soc_dir}/${float_dir}${short_wchar}" PARENT_SCOPE)
  endif()
endfunction()
