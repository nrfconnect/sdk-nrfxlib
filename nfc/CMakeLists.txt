#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

if(CONFIG_FLOAT)
  if(CONFIG_FP_HARDABI)
	set(float_abi hard)
  elseif(CONFIG_FP_SOFTABI)
	# set(float_abi softfp)
	assert(0 "CONFIG_FP_SOFTABI is not supported yet")
  else()
	assert(0 "Unreachable code")
  endif()
else()
  set(float_abi soft)
endif()

set(float_dir ${float_abi}-float)

assert(GCC_M_CPU "GCC_M_CPU must be set")

set(NFC_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${GCC_M_CPU}/${float_dir})

if(CONFIG_NFC_T2T_HAL_ENABLED)
  set(tag_type t2t)
elseif(CONFIG_NFC_T4T_HAL_ENABLED)
  set(tag_type t4t)
else()
	assert(0 "Unreachable code")
endif()

zephyr_library()
zephyr_include_directories(include)
zephyr_library_sources(src/hal_nfc_${tag_type}.c)
zephyr_link_libraries(${NFC_LIB_PATH}/libnfc${tag_type}_nrf52.a)
