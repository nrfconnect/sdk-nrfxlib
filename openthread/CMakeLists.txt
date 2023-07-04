#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
include(cmake/extensions.cmake)

set(OPENTHREAD_LIB_MBEDTLS_CONF_FILE "nrf_security_mbedtls_configuration.h")

get_openthread_libraries(OPENTHREAD_LIBRARIES)

if(CONFIG_OPENTHREAD_SOURCES)
  message(DEBUG "Building OT from sources, config file will be generated.")

  set(OPENTHREAD_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/openthread_lib_configuration.txt")

  openthread_libs_configuration_write(${OPENTHREAD_CONFIG_FILE})

  openthread_calculate_lib_path("v${CONFIG_OPENTHREAD_THREAD_VERSION}" OPENTHREAD_DST_DIR)

  set(OPENTHREAD_HEADERS_DIR "${ZEPHYR_OPENTHREAD_MODULE_DIR}/include")
  set(OPENTHREAD_HEADERS_DST_DIR "${NRFXLIB_DIR}/openthread/include")
  set(OPENTHREAD_UTILS_DST_DIR "${NRFXLIB_DIR}/openthread/include/utils")
  set(OPENTHREAD_SYSTEM_H "${ZEPHYR_OPENTHREAD_MODULE_DIR}/examples/platforms/openthread-system.h")
  set(OPENTHREAD_UART_H "${ZEPHYR_OPENTHREAD_MODULE_DIR}/examples/platforms/utils/uart.h")
  set(NRF_SECURITY_MBEDTLS_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/../../../nrf/subsys/nrf_security/src/include/generated/${CONFIG_MBEDTLS_CFG_FILE})

  if(CONFIG_OPENTHREAD_BUILD_OUTPUT_STRIPPED)
    foreach(target IN LISTS OPENTHREAD_LIBRARIES)

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
  )

  foreach(target IN LISTS OPENTHREAD_LIBRARIES)
    list(APPEND INSTALL_COMMANDS
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}>${OPENTHREAD_LIB_INSTALL_POSTFIX} ${OPENTHREAD_DST_DIR}/$<TARGET_FILE_NAME:${target}>)
  endforeach()

  list(APPEND INSTALL_COMMANDS
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${OPENTHREAD_HEADERS_DIR} ${OPENTHREAD_HEADERS_DST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${OPENTHREAD_UTILS_DST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy ${OPENTHREAD_SYSTEM_H} ${OPENTHREAD_HEADERS_DST_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy ${OPENTHREAD_UART_H} ${OPENTHREAD_UTILS_DST_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy ${OPENTHREAD_CONFIG_FILE} ${OPENTHREAD_DST_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
      ${NRF_SECURITY_MBEDTLS_CONFIG_FILE} ${OPENTHREAD_DST_DIR}/${OPENTHREAD_LIB_MBEDTLS_CONF_FILE}
  )

  add_custom_target(install_openthread_libraries ${INSTALL_COMMANDS})
endif()

if (CONFIG_OPENTHREAD_NORDIC_LIBRARY)
  openthread_calculate_lib_path("v${CONFIG_OPENTHREAD_THREAD_VERSION}" OT_LIB_PATH)

  if(NOT EXISTS ${OT_LIB_PATH})
    message(FATAL_ERROR
      " This platform has no OpenThread libraries provided for the current configuration.\n"
      " Please use sources to build OpenThread.")
  endif()

  check_openthread_version()

  if(NOT CONFIG_OPENTHREAD_COPROCESSOR_RCP)
    check_openthread_dependencies("${OT_LIB_PATH}/${OPENTHREAD_LIB_MBEDTLS_CONF_FILE}")
  endif()

  foreach(target IN LISTS OPENTHREAD_LIBRARIES)
    zephyr_library_import("lib${target}" "${OT_LIB_PATH}/lib${target}.a")
  endforeach()

  zephyr_include_directories(include)

endif()
