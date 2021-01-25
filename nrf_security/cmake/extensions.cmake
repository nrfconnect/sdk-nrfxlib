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
