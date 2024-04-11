#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# The Build the path of the library taking in nrf_security backend type,
# OpenThread version and feature set
function(openthread_calculate_lib_path ot_version lib_path)
  if(CONFIG_OPENTHREAD_NORDIC_LIBRARY_MASTER)
    set(ot_feature_set "master")
  elseif(CONFIG_OPENTHREAD_NORDIC_LIBRARY_FTD)
    set(ot_feature_set "ftd")
  elseif(CONFIG_OPENTHREAD_NORDIC_LIBRARY_MTD)
    set(ot_feature_set "mtd")
  elseif(CONFIG_OPENTHREAD_NORDIC_LIBRARY_RCP)
    set(ot_feature_set "rcp")
  else()
    set(ot_feature_set "custom")
  endif()

  nrfxlib_calculate_lib_path(nrfxlib_path)
  if(CONFIG_OPENTHREAD_COPROCESSOR_RCP)
    set(${lib_path}
      "${ZEPHYR_NRFXLIB_MODULE_DIR}/openthread/${nrfxlib_path}/${ot_version}/rcp"
      PARENT_SCOPE)
  else()
    set(${lib_path}
      "${ZEPHYR_NRFXLIB_MODULE_DIR}/openthread/${nrfxlib_path}/${ot_version}/${ot_feature_set}/${nrf_security_backend}"
      PARENT_SCOPE)
  endif()

endfunction()

macro(get_git_decribe repo)
  execute_process(
    COMMAND           ${GIT_EXECUTABLE} describe --long
    WORKING_DIRECTORY ${repo}
    RESULT_VARIABLE   git_return
    OUTPUT_VARIABLE   git_describe
    ERROR_QUIET
    )

  if(NOT ${git_return} EQUAL 0)
    execute_process(
      COMMAND           ${GIT_EXECUTABLE} rev-parse HEAD
      WORKING_DIRECTORY ${repo}
      RESULT_VARIABLE   git_return
      OUTPUT_VARIABLE   git_describe
      )
  endif()
endmacro()

# Store the configuration of the compiled OpenThread libraries
# and set source and destination paths.
function(openthread_libs_configuration_write CONFIG_FILE)
  # Store all OT related variables
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    if("${_variableName}" MATCHES "^CONFIG_OPENTHREAD_.*|^OT_.*")
      list(APPEND OPENTHREAD_SETTINGS "${_variableName}=${${_variableName}}\n")
    endif()
  endforeach()

  list(SORT OPENTHREAD_SETTINGS)

  find_package(Git QUIET)
  if(GIT_FOUND)
    get_git_decribe(${ZEPHYR_MBEDTLS_MODULE_DIR})
    list(INSERT OPENTHREAD_SETTINGS 0 "MBEDTLS_commit=${git_describe}\n")

    get_git_decribe(${ZEPHYR_NRFXLIB_MODULE_DIR})
    list(INSERT OPENTHREAD_SETTINGS 0 "NRFXLIB_commit=${git_describe}")

    get_git_decribe(${ZEPHYR_OPENTHREAD_MODULE_DIR})
    list(INSERT OPENTHREAD_SETTINGS 0 "OpenThread_commit=${git_describe}")
  endif()

  # Store compiler and Zephyr SDK version
  execute_process(COMMAND ${CMAKE_C_COMPILER} --version
    OUTPUT_VARIABLE GCC_VERSION)
  string(REPLACE "\n" ";" GCC_VERSION_LINES ${GCC_VERSION})
  list(GET GCC_VERSION_LINES 0 GCC_VERSION_LINE)
  list(INSERT OPENTHREAD_SETTINGS 0 "GCC_version: ${GCC_VERSION_LINE}\n\n")

  file(WRITE ${CONFIG_FILE} ${OPENTHREAD_SETTINGS})
endfunction()

function(get_active_mbedtls_configs_from_file fileName returnMatch1List)
  file(READ ${fileName} FILE_CONTENT)
  set(PATTERN "\n[ \t]*#define[ \t]+(MBEDTLS_[A-Za-z0-9_]*[ \t]*[0-9A-Fx]*)")
  string(REGEX MATCHALL ${PATTERN} match_list ${FILE_CONTENT})
    foreach(element ${match_list})
      string(REGEX MATCH ${PATTERN} MACRO_DEF ${element})
      list(APPEND match1_list "${CMAKE_MATCH_1}\n")
      set(${returnMatch1List} ${match1_list} PARENT_SCOPE)
    endforeach()
endfunction()

function(check_openthread_dependencies ot_lib_nrf_security_mbedtls_config_file)
  set(nrf_security_mbedtls_config_file "${CMAKE_CURRENT_BINARY_DIR}/../../../nrf/subsys/nrf_security/src/include/generated/${CONFIG_MBEDTLS_CFG_FILE}")
  get_active_mbedtls_configs_from_file(${nrf_security_mbedtls_config_file} mbedtls_conf_list)
  get_active_mbedtls_configs_from_file(${ot_lib_nrf_security_mbedtls_config_file} ot_mbedtls_conf_list)

  foreach(config_option ${ot_mbedtls_conf_list})
    if(NOT (${config_option} IN_LIST mbedtls_conf_list))
      message(WARNING
        " \n"
        " Current nrf_security configuration does not provide all MBEDTLS"
        " options which are required by precompiled OpenThread libraries.\n"
        " The minimum of MBEDTLS configuration is stored at:"
        " ${ot_lib_nrf_security_mbedtls_config_file}\n")
        break()
    endif()
  endforeach(config_option)
endfunction()

function(check_openthread_version)
  file(READ "${ZEPHYR_OPENTHREAD_MODULE_DIR}/include/openthread/instance.h" INSTANCE_FILE_CONTENT)
  string(REGEX MATCH "#define[ ]*OPENTHREAD_API_VERSION[ ]*\\(([0-9]+)\\)" MACRO_DEF "${INSTANCE_FILE_CONTENT}")
  set(OPENTHREAD_SOURCE_API_VERSION ${CMAKE_MATCH_1})

  file(READ "include/openthread/instance.h" INSTANCE_FILE_CONTENT)
  string(REGEX MATCH "#define[ ]*OPENTHREAD_API_VERSION[ ]*\\(([0-9]+)\\)" MACRO_DEF "${INSTANCE_FILE_CONTENT}")
  set(OPENTHREAD_LIB_API_VERSION ${CMAKE_MATCH_1})

  if(NOT (${OPENTHREAD_SOURCE_API_VERSION} EQUAL ${OPENTHREAD_LIB_API_VERSION}))
    message(WARNING "The Openthread source code version (${OPENTHREAD_SOURCE_API_VERSION}) is not equal to compiled Openthread library version (${OPENTHREAD_LIB_API_VERSION}).")
  endif()
endfunction()

macro(get_openthread_libraries ot_libs)
  if(CONFIG_OPENTHREAD_FTD)
    set(CLI_LIBRARIES openthread-cli-ftd)
  elseif(CONFIG_OPENTHREAD_MTD)
    set(CLI_LIBRARIES openthread-cli-mtd)
  endif()

  if(CONFIG_OPENTHREAD_SHELL)
    list(APPEND ${ot_libs} ${CLI_LIBRARIES})
  endif()

  if(CONFIG_OPENTHREAD_COPROCESSOR_RCP)
    list(APPEND ${ot_libs} openthread-rcp)
    list(APPEND ${ot_libs} openthread-spinel-rcp)
    list(APPEND ${ot_libs} openthread-radio)
    list(APPEND ${ot_libs} openthread-hdlc)
  endif()

  if(CONFIG_OPENTHREAD_COPROCESSOR_NCP)
    if(CONFIG_OPENTHREAD_FTD)
      list(APPEND ${ot_libs} openthread-ncp-ftd)
    elseif(CONFIG_OPENTHREAD_MTD)
      list(APPEND ${ot_libs} openthread-ncp-mtd)
    endif()
    list(APPEND ${ot_libs} openthread-hdlc)
    list(APPEND ${ot_libs} openthread-spinel-ncp)
  endif()

  if(NOT CONFIG_OPENTHREAD_COPROCESSOR_RCP)
    if(CONFIG_OPENTHREAD_FTD)
      list(APPEND ${ot_libs} openthread-ftd)
    elseif(CONFIG_OPENTHREAD_MTD)
      list(APPEND ${ot_libs} openthread-mtd)
    endif()
  endif()
endmacro()
