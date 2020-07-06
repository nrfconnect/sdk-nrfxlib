#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
include(${NRFXLIB_DIR}/common.cmake)
nrfxlib_calculate_lib_path(lib_path)

if (CONFIG_OPENTHREAD_LIBRARY_1_1)
  set(version_dir "v1.1")
  set(OT_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path}/${version_dir})

  if(NOT EXISTS ${OT_LIB_PATH})
    message(WARNING "This platform has no libraries provided for OpenThread.
                     Please use sources to build OpenThread.")
  endif()

  if(CONFIG_OPENTHREAD_SHELL)
    if(CONFIG_OPENTHREAD_FTD)
      zephyr_library_import(
        openthread-cli-ftd
        ${OT_LIB_PATH}/libopenthread-cli-ftd.a
      )
    elseif(CONFIG_OPENTHREAD_MTD)
      zephyr_library_import(
        openthread-cli-mtd
        ${OT_LIB_PATH}/libopenthread-cli-mtd.a
      )
    endif()
  endif()

  if(CONFIG_OPENTHREAD_NCP)
    if(CONFIG_OPENTHREAD_FTD)
      zephyr_library_import(
        openthread-ncp-ftd
        ${OT_LIB_PATH}/libopenthread-ncp-ftd.a
      )
    elseif(CONFIG_OPENTHREAD_MTD)
      zephyr_library_import(
        openthread-ncp-mtd
        ${OT_LIB_PATH}/libopenthread-ncp-mtd.a
      )
    endif()
  endif()

  if(CONFIG_OPENTHREAD_FTD)
    zephyr_library_import(
      openthread-ftd
      ${OT_LIB_PATH}/libopenthread-ftd.a
    )
  elseif(CONFIG_OPENTHREAD_MTD)
    zephyr_library_import(
      openthread-mtd
      ${OT_LIB_PATH}/libopenthread-mtd.a
    )
  endif()

  zephyr_include_directories(include)

endif()
