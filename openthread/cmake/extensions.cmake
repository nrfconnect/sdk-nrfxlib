#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# The Build the path of the library taking in nrf_security backend type,
# OpenThread version and feature set
function(openthread_calculate_lib_path ot_version lib_path)
  set(OPENTHREAD_LIB_BASE_DIR "${ZEPHYR_NRFXLIB_MODULE_DIR}/openthread")

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

  nrfxlib_calculate_lib_path(nrfxlib_path BASE_DIR ${OPENTHREAD_LIB_BASE_DIR} SOFT_FLOAT_FALLBACK SOC_MODE)

  if(CONFIG_OPENTHREAD_COPROCESSOR_RCP)
    set(${lib_path}
      "${nrfxlib_path}/${ot_version}/rcp"
      PARENT_SCOPE)
  else()
    set(${lib_path}
      "${nrfxlib_path}/${ot_version}/${ot_feature_set}/${nrf_security_backend}"
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
function(openthread_libs_configuration_write CONFIG_FILE NRFXLIB_RELEASE_TAG)
  # Store compiler and Zephyr SDK version
  execute_process(COMMAND ${CMAKE_C_COMPILER} --version
    OUTPUT_VARIABLE GCC_VERSION)
  string(REPLACE "\n" ";" GCC_VERSION_LINES ${GCC_VERSION})
  list(GET GCC_VERSION_LINES 0 GCC_VERSION_LINE)
  list(APPEND LIB_CONFIGURATION "###################### Compiler Version ######################\n\n")
  list(APPEND LIB_CONFIGURATION "GCC_version: ${GCC_VERSION_LINE}\n")

  # Store SDK Version
  find_package(Git QUIET)
  if(GIT_FOUND)
    list(APPEND LIB_CONFIGURATION "\n###################### SDK Version ###########################\n\n")
    get_git_decribe(${ZEPHYR_MBEDTLS_MODULE_DIR})
    list(APPEND LIB_CONFIGURATION "MBEDTLS_commit=${git_describe}")

    get_git_decribe(${ZEPHYR_NRFXLIB_MODULE_DIR})
    list(APPEND LIB_CONFIGURATION "NRFXLIB_commit=${git_describe}")

    get_git_decribe(${ZEPHYR_OPENTHREAD_MODULE_DIR})
    list(APPEND LIB_CONFIGURATION "OpenThread_commit=${git_describe}")

    list(APPEND LIB_CONFIGURATION "NRFXLIB_RELEASE_TAG=${NRFXLIB_RELEASE_TAG}\n")
  endif()

  # Store all OT related variables
  get_cmake_property(_variableNames VARIABLES)
  list(APPEND OPENTHREAD_SETTINGS "\n###################### OpenThread configuration ##############\n\n")
  foreach (_variableName ${_variableNames})
    if("${_variableName}" MATCHES "^CONFIG_OPENTHREAD_.*|^OT_.*")
      list(APPEND OPENTHREAD_SETTINGS "${_variableName}=${${_variableName}}\n")
    endif()
  endforeach()
  list(SORT OPENTHREAD_SETTINGS)
  list(APPEND LIB_CONFIGURATION ${OPENTHREAD_SETTINGS})

  # Store Crypto configuration
  list(APPEND LIB_CONFIGURATION "\n###################### Crypto configuration ##################\n\n")
  foreach (_variableName ${_variableNames})
    if("${_variableName}" MATCHES "^CONFIG_MBEDTLS_.*|^CONFIG_PSA_WANT_.*")
      list(APPEND CRYPTO_SETTINGS "${_variableName}=${${_variableName}}\n")
    endif()
  endforeach()
  list(SORT CRYPTO_SETTINGS)
  list(APPEND LIB_CONFIGURATION ${CRYPTO_SETTINGS})

  file(WRITE ${CONFIG_FILE} ${LIB_CONFIGURATION})
endfunction()

function(check_openthread_dependencies CONFIG_FILE)
  get_cmake_property(_variableNames VARIABLES)
  file(READ ${CONFIG_FILE} FILE_CONTENT)
  file(READ "cmake/crypto_config_tfm_whitelist.txt" TFM_WHITELIST)
  string(REGEX MATCHALL "CONFIG_(MBEDTLS_|PSA_WANT_)[^\n]*" match_list ${FILE_CONTENT})
  foreach(element ${match_list})
    string(REGEX MATCH "([^=]+)=(.*)" _ ${element})
    set(config_name "${CMAKE_MATCH_1}")
    set(config_value "${CMAKE_MATCH_2}")
    if (NOT ((config_name IN_LIST _variableNames AND ${${config_name}} STREQUAL ${config_value}) OR
             (CONFIG_BUILD_WITH_TFM AND ${TFM_WHITELIST} MATCHES ${config_name})))
      message(WARNING
        " \n"
        " ${config_name}=${config_value} is required by the precompiled OpenThread libraries,"
        " but it has been configured differently in the current build.\n"
        " The minimal crypto configuration is stored under \"Crypto configuration\" section at:"
        " ${CONFIG_FILE}\n")
    endif()
  endforeach()
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

  if(CONFIG_OPENTHREAD_TCP_ENABLE)
    if(CONFIG_OPENTHREAD_FTD)
      list(APPEND ${ot_libs} tcplp-ftd)
    elseif(CONFIG_OPENTHREAD_MTD)
      list(APPEND ${ot_libs} tcplp-mtd)
    endif()
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
