#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if (${CONFIG_FLOAT})
  if(CONFIG_FP_HARDABI)
    set(float_dir hard-float)
  elseif(CONFIG_FP_SOFTABI)
    set(float_dir softfp-float)
  else()
    assert(0 "Unreachable code")
  endif()
else ()
  set(float_dir soft-float)
endif()

assert(GCC_M_CPU "GCC_M_CPU must be set to find correct lib.")
set(lib_path lib/${GCC_M_CPU}/${float_dir})

add_subdirectory_ifdef(CONFIG_NRFXLIB_NFC    nfc)
add_subdirectory_ifdef(CONFIG_BT_LL_NRFXLIB  ble_controller)
add_subdirectory_ifdef(CONFIG_BSD_LIBRARY    bsdlib)
add_subdirectory_ifdef(CONFIG_NRFXLIB_CRYPTO crypto)
