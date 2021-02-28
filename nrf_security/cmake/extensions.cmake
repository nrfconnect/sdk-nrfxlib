#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Function to create libraries for nrf_security
#
# One value arguments:
#   NOGLUE: Uses NOGLUE identifier as name, creates mbedcrypto_<arg>_noglue
#            mbed TLS config file (additional call), uses backend root as input for name
#   BASE: Must be combined, Will not be renamed. Name is full name of library (i.e. arg)
#
# Multi value arguments:
#   FILES: List of sources to add
#   INCLUDES: List of include directories
#   DEFINES: Defines used in compilation
#   LINK_LIBRARIES: List of libraries to link to
#
function(nrf_security_library)
  set(options NOT_USED)
  set(one_value_args NOGLUE BASE)
  set(multi_value_args FILES LINK_LIBRARIES INCLUDES DEFINES)
  cmake_parse_arguments(LIBRARY "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  set(sources "")

  # Resolve library name, backend name, and set library type.
  if(DEFINED LIBRARY_NOGLUE)
    set(lib_name mbedcrypto_${LIBRARY_NOGLUE}_noglue)
    set(backend_name ${LIBRARY_NOGLUE})
  elseif(DEFINED LIBRARY_BASE)
    set(lib_name ${LIBRARY_BASE})
  else()
    message(FATAL_ERROR "nrf_security_library: Did not provide "
                        "BACKEND, NOGLUE, INTERFACE, BASE, or TARGET")
  endif()

  # Create a list of files if configuration is set
  foreach(file ${LIBRARY_FILES})
    get_filename_component(file_name ${file} NAME_WE)

    string(REPLACE "cc310" "cc3xx" file_name ${file_name})

    if(DEFINED backend_name AND NOT DEFINED LIBRARY_BASE)
      nrf_security_filename_to_alg_root(${backend_name} ${file_name} ALG_ROOT)
      string(TOUPPER ${backend_name} BACKEND_UPPER)

      if(CONFIG_${BACKEND_UPPER}_MBEDTLS_${ALG_ROOT}_C)
          nrf_security_debug("Adding to ${lib_name}: ${file}")
          list(APPEND sources ${file})
      endif()
    else()
        # This is not a backend library. Just add the files
        nrf_security_debug("Adding to ${lib_name}: ${file}")
        list(APPEND sources ${file})
    endif()
  endforeach()

  # Add library if sources are enabled
  if(sources STREQUAL "")
    nrf_security_debug("nrf_security_library: ${lib_name} not created (no sources)")
    return()
  endif()

  nrf_security_debug("nrf_security_library: Creating ${lib_name}")
  add_library(${lib_name} OBJECT ${sources})

  # Add generic options from zephyr build if this is not a interface library
  nrf_security_add_zephyr_options(${lib_name})

  nrf_security_debug_list_target_files(${lib_name})

  # Add backend name if this is applicable
  if(DEFINED backend_name)
    set_property(TARGET ${lib_name} PROPERTY BACKEND_NAME ${backend_name})
  endif()

  # Add includes (if set)
  if(DEFINED LIBRARY_INCLUDES)
    nrf_security_debug("nrf_security_library: Add includes for "
                       "${lib_name}: ${LIBRARY_INCLUDES}")
    target_include_directories(${lib_name} PRIVATE ${LIBRARY_INCLUDES})
  endif()

  # Add the standard includes for this library
  target_include_directories(${lib_name} PRIVATE
      ${common_includes}
      ${config_include}
      $<$<TARGET_EXISTS:platform_cc3xx>:${mbedcrypto_glue_include_path}/threading>
      $<TARGET_PROPERTY:platform_cc3xx,INTERFACE_INCLUDE_DIRECTORIES>
  )

  # Add defines (if set)
  if(DEFINED LIBRARY_DEFINES)
    nrf_security_debug("Add compile definitions for ${lib_name}: ${LIBRARY_DEFINES}")
    target_compile_definitions(${lib_name} PRIVATE ${LIBRARY_DEFINES})
  endif()

  # Add dependencies (if set)
  if(DEFINED LIBRARY_LINK_LIBRARIES)
    nrf_security_debug("Add dependency for ${lib_name}: ${LIBRARY_LINK_LIBRARIES}")
    target_link_libraries(${lib_name} PRIVATE ${LIBRARY_LINK_LIBRARIES})
  endif()
endfunction()

#
# Function to add a glue library for nrf_security
# Options:
#   ALT: This indicates the glue frontend library.
# One value arguments:
#   BACKEND: The backend to which this glue library relates, cc3xx, Oberon,
#            Vanilla.
#
# Multi value arguments:
#   FILES: List of sources to add
#   INCLUDES: List of include directories
#   LINK_LIBRARIES: List of libraries to link to
#
function(nrf_security_library_glue)
  set(options ALT)
  set(one_arg BACKEND)
  set(multi_args FILES INCLUDES LINK_LIBRARIES)
  cmake_parse_arguments(GLUE_LIB "${options}" "${one_arg}" "${multi_args}" ${ARGN})
  set(sources "")

  if(GLUE_LIB_ALT)
    set(lib_name mbedcrypto_glue)
    set(backend_name dummy)
  else()
    set(lib_name mbedcrypto_glue_${GLUE_LIB_BACKEND})
    set(backend_name ${GLUE_LIB_BACKEND})
    string(TOUPPER ${GLUE_LIB_BACKEND} BACKEND_UPPER)
  endif()

  # Add the sources
  foreach(file ${GLUE_LIB_FILES})
    get_filename_component(file_name ${file} NAME_WE)

    string(REPLACE "cc310" "cc3xx" file_name ${file_name})

    # Get the algorithm root to use for matching configuration
    nrf_security_filename_to_alg_root(${backend_name}
      ${file_name}
      ALG_ROOT
    )

    if(DEFINED GLUE_LIB_BACKEND)
      if(CONFIG_GLUE_${BACKEND_UPPER}_MBEDTLS_${ALG_ROOT}_C)
        nrf_security_debug("Adding to ${backend_name}_glue: ${file}")
        list(APPEND sources ${file})
      endif()
    endif()

    # Add the glue frontend files
    if(DEFINED GLUE_LIB_ALT)
      if(CONFIG_GLUE_MBEDTLS_${ALG_ROOT}_C)
        nrf_security_debug("Adding to glue: ${file}")
        list(APPEND sources ${file})
      endif()
    endif()
  endforeach()

  if(sources STREQUAL "")
    return()
  endif()

  add_library(${lib_name} OBJECT ${sources})
  target_compile_definitions(${lib_name} PRIVATE MBEDTLS_BACKEND_PREFIX=${GLUE_LIB_BACKEND})
  # Add options from Zephyr build
  nrf_security_add_zephyr_options(${lib_name})

  if(GLUE_LIB_INCLUDES)
    target_include_directories(${lib_name} PRIVATE ${GLUE_LIB_INCLUDES})
  endif()

  if(GLUE_LIB_BACKEND)
    set_property(TARGET ${lib_name} PROPERTY BACKEND_NAME ${GLUE_LIB_BACKEND})
  endif()

  # Add any dependencies
  if(DEFINED GLUE_LIB_LINK_LIBRARIES)
    nrf_security_debug("Adding glue lib dependencies: ${GLUE_LIB_LINK_LIBRARIES}")
    target_link_libraries(${lib_name} PRIVATE ${GLUE_LIB_LINK_LIBRARIES})
  endif()

  # Listing files
  nrf_security_debug_list_target_files(${lib_name})
endfunction()

#
# Function to add a shared object library for nrf_security
#
# One value arguments:
#   NAME: Name of library to create
#
# Multi value arguments:
#   FILES: List of sources to add
#   INCLUDE: List of backends for which the list of files should be included
#   EXCLUDE: List of backends for which the list of files should be excluded
#
function(nrf_security_library_shared)
  cmake_parse_arguments(LIB "" "NAME" "" ${ARGN})
  set(sources "")
  set(ARGN_LOCAL ${ARGN})

  set(index 0)
  set(prev_index 0)
  set(length -1)
  foreach(arg ${ARGN_LOCAL})
    if(${arg} STREQUAL FILES)
      list(APPEND FILES_START_INDICES ${index})
      if(prev_index)
        math(EXPR  length "${index} - ${prev_index}")
        set(FILES_INDEX_${prev_index}_LENGTH ${length})
      endif()
      set(prev_index ${index})
    endif()
    math(EXPR  index "${index} + 1")
  endforeach()
  set(FILES_INDEX_${prev_index}_LENGTH -1)

  foreach(index ${FILES_START_INDICES})
    list(SUBLIST ARGN_LOCAL ${index} ${FILES_INDEX_${index}_LENGTH} SUB_LIST)

    # Create sublist and parse sub-args.
    cmake_parse_arguments(SOURCE_LIST "" "" "FILES;INCLUDE;EXCLUDE" ${SUB_LIST})

    list(GET SOURCE_LIST_FILES 0 file)
    get_filename_component(file_name ${file} NAME_WE)
    string(TOUPPER ${file_name} SETTING_UPPER)

    if(CONFIG_MBEDTLS_${SETTING_UPPER}_C)
      set(use_file True)
      foreach(backend ${SOURCE_LIST_INCLUDE})
        if(CONFIG_${backend}_MBEDTLS_${SETTING_UPPER}_C)
          # include file
          set(use_file True)
        endif()
      endforeach()

      foreach(backend ${SOURCE_LIST_EXCLUDE})
        if(CONFIG_${backend}_MBEDTLS_${SETTING_UPPER}_C)
          # exclude file
          set(use_file False)
        endif()
      endforeach()

      if(${use_file})
        nrf_security_debug("Adding to shared: ${SETTING_UPPER}")
        list(APPEND sources ${SOURCE_LIST_FILES})
      endif()
    endif()
  endforeach()

  # If there are no sources, then there is no reason to continue as no
  # target will be created
  if(sources STREQUAL "")
    return()
  endif()

  add_library(${LIB_NAME} OBJECT ${sources})

  # Add options from Zephyr build
  nrf_security_add_zephyr_options(${LIB_NAME})

  target_compile_options(${LIB_NAME} PRIVATE ${TOOLCHAIN_C_FLAGS})
  target_ld_options(${LIB_NAME} PRIVATE ${TOOLCHAIN_LD_FLAGS})

  target_include_directories(${LIB_NAME} PRIVATE
      ${common_includes}
      ${config_include}
      $<$<TARGET_EXISTS:platform_cc3xx>:${mbedcrypto_glue_include_path}/threading>
      $<TARGET_PROPERTY:platform_cc3xx,INTERFACE_INCLUDE_DIRECTORIES>
  )

  # Use configurations for vanilla as shared will always need configurations
  # that aren't enabling any alternate implemenatations.
  target_compile_definitions(${LIB_NAME} PRIVATE
    -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
  )

  nrf_security_debug_list_target_files(${LIB_NAME})
endfunction()
