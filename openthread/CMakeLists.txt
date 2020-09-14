#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
include(${NRFXLIB_DIR}/common.cmake)
nrfxlib_calculate_lib_path(lib_path)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/extensions.cmake)

function(checkOpenthreadVersion)
  file(READ "${ZEPHYR_OPENTHREAD_MODULE_DIR}/../include/openthread/instance.h" INSTANCE_FILE_CONTENT)
  string(REGEX MATCH "#define[ ]*OPENTHREAD_API_VERSION[ ]*\\(([0-9]+)\\)" MACRO_DEF "${INSTANCE_FILE_CONTENT}")
  set(OPENTHREAD_SOURCE_API_VERSION ${CMAKE_MATCH_1})

  file(READ "include/openthread/instance.h" INSTANCE_FILE_CONTENT)
  string(REGEX MATCH "#define[ ]*OPENTHREAD_API_VERSION[ ]*\\(([0-9]+)\\)" MACRO_DEF "${INSTANCE_FILE_CONTENT}")
  set(OPENTHREAD_LIB_API_VERSION ${CMAKE_MATCH_1})

  #check if the Openthread API changed
  if(NOT (${OPENTHREAD_SOURCE_API_VERSION} EQUAL ${OPENTHREAD_LIB_API_VERSION}))
    message(WARNING "The Openthread source code version (${OPENTHREAD_SOURCE_API_VERSION}) is not equal to compiled Openthread library version (${OPENTHREAD_LIB_API_VERSION}).")
  endif()
endfunction()

if(CONFIG_OPENTHREAD_SOURCES)
  message(DEBUG "Building OT from sources, config file will be generated.")

  set(OPENTHREAD_CONFIG_FILE "${CMAKE_BINARY_DIR}/openthread_lib_configuration.txt")

  openthread_libs_configuration_write(${OPENTHREAD_CONFIG_FILE})

  nrfxlib_calculate_lib_path(lib_path)
  set(OPENTHREAD_DST_DIR "${CMAKE_CURRENT_LIST_DIR}/${lib_path}/v${CONFIG_OPENTHREAD_THREAD_VERSION}")
  set(OPENTHREAD_HEADERS_DIR "${ZEPHYR_OPENTHREAD_MODULE_DIR}/../include")

  add_custom_target(install_openthread_libraries
    COMMAND ${CMAKE_COMMAND} -E make_directory "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-ftd> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-mtd> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-radio> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-cli-ftd> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-cli-mtd> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-ncp-ftd> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-ncp-mtd> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-rcp> "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${OPENTHREAD_HEADERS_DIR}
      ${NRFXLIB_DIR}/openthread/include
    COMMAND ${CMAKE_COMMAND} -E copy
     ${OPENTHREAD_CONFIG_FILE}
     ${NRFXLIB_DIR}/openthread/
    )
endif()

if (CONFIG_OPENTHREAD_LIBRARY_1_1)
  set(version_dir "v1.1")
  set(OT_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${lib_path}/${version_dir})

  if(NOT EXISTS ${OT_LIB_PATH})
    message(WARNING "This platform has no libraries provided for OpenThread.
                     Please use sources to build OpenThread.")
  endif()

  checkOpenthreadVersion()

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
