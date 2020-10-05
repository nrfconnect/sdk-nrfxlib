#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
include(cmake/extensions.cmake)

set(OPENTHREAD_LIB_MBEDTLS_CONF_FILE "nrf_security_mbedtls_configuration.h")

if(CONFIG_OPENTHREAD_SOURCES)
  message(DEBUG "Building OT from sources, config file will be generated.")

  set(OPENTHREAD_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/openthread_lib_configuration.txt")

  openthread_libs_configuration_write(${OPENTHREAD_CONFIG_FILE})

  openthread_calculate_lib_path("v${CONFIG_OPENTHREAD_THREAD_VERSION}" OPENTHREAD_DST_DIR)

  set(OPENTHREAD_HEADERS_DIR "${ZEPHYR_OPENTHREAD_MODULE_DIR}/include")
  set(OPENTHREAD_HEADERS_DST_DIR "${NRFXLIB_DIR}/openthread/include")
  set(OPENTHREAD_SYSTEM_H "${ZEPHYR_OPENTHREAD_MODULE_DIR}/examples/platforms/openthread-system.h")
  set(NRF_SECURITY_MBEDTLS_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/../nrf_security/include/${CONFIG_MBEDTLS_CFG_FILE})

  if(CONFIG_OPENTHREAD_BUILD_OUTPUT_STRIPPED)
    foreach(target openthread-ftd openthread-mtd openthread-radio openthread-cli-ftd openthread-cli-mtd openthread-ncp-ftd openthread-ncp-mtd openthread-rcp)
      list(APPEND OPENTHREAD_STRIP_COMMAND
           COMMAND $<TARGET_PROPERTY:bintools,strip_command>
                   $<TARGET_PROPERTY:bintools,strip_flag>
                   $<TARGET_PROPERTY:bintools,strip_flag_debug>
                   $<TARGET_PROPERTY:bintools,strip_flag_infile>$<TARGET_FILE:${target}>
                   $<TARGET_PROPERTY:bintools,strip_flag_outfile>$<TARGET_FILE:${target}>.strip
                   $<TARGET_PROPERTY:bintools,strip_flag_final>
      )
    endforeach()
    add_custom_target(openthread_stripped ALL
                      ${OPENTHREAD_STRIP_COMMAND}
                      COMMAND_EXPAND_LISTS
    )
    set(OPENTHREAD_LIB_INSTALL_POSTFIX ".strip")
  endif()
  list(APPEND INSTALL_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E echo "Installing OpenThread libraries into: ${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${OPENTHREAD_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-ftd>${OPENTHREAD_LIB_INSTALL_POSTFIX}  ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-ftd>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-mtd>${OPENTHREAD_LIB_INSTALL_POSTFIX}  ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-mtd>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-radio>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-radio>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-cli-ftd>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-cli-ftd>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-cli-mtd>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-cli-mtd>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-ncp-ftd>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-ncp-ftd>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-ncp-mtd>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-ncp-mtd>
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:openthread-rcp>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:openthread-rcp>
  )

  list(APPEND INSTALL_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${OPENTHREAD_HEADERS_DIR} ${OPENTHREAD_HEADERS_DST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${OPENTHREAD_SYSTEM_H} ${OPENTHREAD_HEADERS_DST_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy ${OPENTHREAD_CONFIG_FILE} ${OPENTHREAD_DST_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
      ${NRF_SECURITY_MBEDTLS_CONFIG_FILE} ${OPENTHREAD_DST_DIR}/${OPENTHREAD_LIB_MBEDTLS_CONF_FILE}
  )

  add_custom_target(install_openthread_libraries ${INSTALL_COMMANDS})
endif()

if (CONFIG_OPENTHREAD_LIBRARY_1_1)
  openthread_calculate_lib_path("v1.1" OT_LIB_PATH)

  if(NOT EXISTS ${OT_LIB_PATH})
    message(FATAL_ERROR
      " This platform has no OpenThread libraries provided for the current configuration.\n"
      " Please use sources to build OpenThread.")
  endif()

  check_openthread_version()
  check_openthread_dependencies("${OT_LIB_PATH}/${OPENTHREAD_LIB_MBEDTLS_CONF_FILE}")

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
