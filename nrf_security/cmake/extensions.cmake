#
# Copyright (c) 2020-2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Add common configurations/options from the zephyr interface libraries
#
# This includes
# Compile options
# Standard includes
# C flags/Linker flags
#
macro(nrf_security_add_zephyr_options lib_name)
  if(TARGET zephyr_interface)
    # Add compile options and includes from zephyr
    target_compile_options(${lib_name} PRIVATE $<TARGET_PROPERTY:zephyr_interface,INTERFACE_COMPILE_OPTIONS>)
    target_include_directories(${lib_name} PRIVATE $<TARGET_PROPERTY:zephyr_interface,INTERFACE_INCLUDE_DIRECTORIES>)
    target_include_directories(${lib_name} PRIVATE $<TARGET_PROPERTY:zephyr_interface,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)

    # Unsure if these are needed any more
    target_compile_options(${lib_name} PRIVATE ${TOOLCHAIN_C_FLAGS})
    target_ld_options(${lib_name} PRIVATE ${TOOLCHAIN_LD_FLAGS})
  else()
    target_compile_options(${lib_name} PRIVATE "SHELL: -imacros ${ZEPHYR_AUTOCONF}")
    target_include_directories(${lib_name} PRIVATE
      $<$<TARGET_EXISTS:platform_cc3xx>:$<TARGET_PROPERTY:platform_cc3xx,INTERFACE_INCLUDE_DIRECTORIES>>
    )
  endif()
endmacro()

#
# Macro to strip away function names into specific algoritm root
#
# Examples:
#   ecp => ECP
#   ecp_curves => ECP
#   ecp_<backend><_xxxx> => ECP
#   ecp_alt<_xxxx> => ECP
#   ecp_common => ECP
#   ecp_xxxx.c.obj => ECP
#
# backend - Input backend name (in case this is part of the file name)
# alg_root - Input (potentially a file nameFile name that will be stripped to alg-root and capitalized
macro(nrf_security_filename_to_alg_root backend file_name alg_root)
  nrf_security_debug("nrf_security_filename_to_alg_root: Input: ${file_name}")

  # Strip file extension
  string(REGEX REPLACE "\\..*$" "" ${alg_root} "${file_name}")

  # Strip _<backend>
  string(REGEX REPLACE "_${backend}" "" ${alg_root} "${${alg_root}}")

  # Strip _alt
  string(REGEX REPLACE "_alt" "" ${alg_root} "${${alg_root}}")

  # Strip _common
  string(REGEX REPLACE "_common" "" ${alg_root} "${${alg_root}}")

  # Strip _curves
  string(REGEX REPLACE "_curves" "" ${alg_root} "${${alg_root}}")

  # Convert to a capitalized version
  string(TOUPPER "${${alg_root}}" ${alg_root})

  nrf_security_debug("nrf_security_filename_to_alg_root: Output: ${${alg_root}}")
endmacro()

#
# This function will create a list of files that should be dropped from the
# library named mbedcrypto_<backend> corresponding to the parameter "backend"
#
function(nrf_security_symbol_strip backend out_file_list)
  nrf_security_debug("========== Running symbol_strip_function for ${backend} ==========")
  string(TOUPPER "${backend}" BACKEND_NAME_UPPER)

  set(file_list
      "aes.c.obj"        "aes_alt.c.obj"
      "ccm.c.obj"        "ccm_alt.c.obj"
      "cmac.c.obj"       "cmac_alt.c.obj"
      "ecp.c.obj"        "ecp_alt_cc310.c.obj" "ecp_alt_cc3xx.c.obj"
      "ecp_curves.c.obj" "ecp_curves_alt.c.obj"
      "ecp_common.c.obj"
      "ecdh.c.obj"       "ecdh_alt.c.obj"
      "ecdsa.c.obj"      "ecdsa_alt.c.obj"
      "ecjpake.c.obj"    "ecjpake_alt.c.obj"
      "chacha20.c.obj"   "chacha20_alt.c.obj"
      "chachapoly.c.obj" "chachapoly_alt.c.obj"
      "gcm.c.obj"        "gcm_alt.c.obj"
      "poly1305.c.obj"   "poly1305_alt.c.obj"
      "rsa.c.obj"        "rsa_alt.c.obj"
      "sha1.c.obj"       "sha1_alt.c.obj"
      "sha256.c.obj"     "sha256_alt.c.obj"
  )

  foreach(file ${file_list})
    nrf_security_filename_to_alg_root(${backend} ${file} alg)
    if (NOT CONFIG_${BACKEND_NAME_UPPER}_MBEDTLS_${alg}_C)
      nrf_security_debug("${backend}: Removing object ${file}")
      list(APPEND ${out_file_list} ${file})
    else ()
      nrf_security_debug("Not removing: CONFIG_${BACKEND_NAME_UPPER}_MBEDTLS_${alg}_C is set to TRUE (${file})")
    endif()
  endforeach()

  # Special handling of files which cannot be converted into a Kconfig setting.
  set(indices 1)
  set(file_1   "poly.c.obj")
  set(symbol_1 MBEDTLS_POLY1305_C)

  foreach(index ${indices})
    if (NOT CONFIG_${BACKEND_NAME_UPPER}_${symbol_${index}})
      nrf_security_debug("${backend}: Removing object ${file}")
      list(APPEND ${out_file_list} ${file_${index}})
    else ()
      nrf_security_debug("Not removing: CONFIG_${BACKEND_NAME_UPPER}_${symbol_${index}} is set to TRUE (${file})")
    endif()
  endforeach()

  set(${out_file_list} ${${out_file_list}} PARENT_SCOPE)
endfunction()

#
# This macro comments out the instruction to rename a symbol
# in case a configuration is enabled in the backend, but not glued
# Note: Internal use for `nrf_security_symbol_rename` only
#
macro(keep_config_test_glue mbedtls_config backend_name)
  if(NOT CONFIG_GLUE_${mbedtls_config} AND CONFIG_${backend_name}_${mbedtls_config})
    set(KEEP_${mbedtls_config} "#")
    nrf_security_debug("${backend_name}: Not renamed ${mbedtls_config}")
  else()
    nrf_security_debug("${backend_name}: Renaming ${mbedtls_config}")
  endif()
endmacro()

#
# This macro comments out the instruction to rename a symbol if two dependent
# configurations are enabled, but the feature (as a whole) is not glued
# Note: Internal use for `nrf_security_symbol_rename` only
#
macro(keep_config_test_glue_depends mbedtls_config mbedtls_config_inner backend_name)
  if((NOT CONFIG_GLUE_${mbedtls_config}) AND
      CONFIG_${backend_name}_${mbedtls_config} AND
      CONFIG_${mbedtls_config_inner})
    nrf_security_debug("${backend_name}: Has ${backend_name}_${mbedtls_config}: Not renamed ${mbedtls_config_inner}")
    set(KEEP_${mbedtls_config_inner} "#")
  else()
    nrf_security_debug("${backend_name}: Renaming ${mbedtls_config_inner}")
  endif()
endmacro()

#
# This function will create a symbol renaming script for any library
# named mbedcrypto_<backend> corresponding to the parameter "backend"
#
function(nrf_security_configure_rename_file backend input output)
  nrf_security_debug("========== Running nrf_security_symbol_rename for ${backend} ==========")
  string(TOUPPER "${backend}" BACKEND_NAME)
  #
  # Functionality that can be glued
  #
  keep_config_test_glue("MBEDTLS_AES_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CCM_C"           ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_CMAC_C"          ${BACKEND_NAME})
  keep_config_test_glue("MBEDTLS_DHM_C"           ${BACKEND_NAME})

  #
  # Functionality that can be glued (dependent configuration)
  #
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_XTS" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_CBC" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_CFB" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_OFB" ${BACKEND_NAME})
  keep_config_test_glue_depends("MBEDTLS_AES_C" "MBEDTLS_CIPHER_MODE_CTR" ${BACKEND_NAME})

  #
  # Set variable for the backend in rename
  #
  set(MBEDTLS_BACKEND_PREFIX ${backend})

  configure_file(${input}
                 ${output}
  )
endfunction()

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
  set(multi_value_args FILES LINK_LIBRARIES INCLUDES DEFINES OPTIONS)
  cmake_parse_arguments(LIBRARY "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  set(sources "")

  # Resolve library name, backend name, and set library type.
  if(DEFINED LIBRARY_NOGLUE)
    set(lib_name ${mbedcrypto_target}_${LIBRARY_NOGLUE}_noglue)
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

  # Add includes for mbed TLS
  target_link_libraries(${lib_name} PRIVATE mbedcrypto_includes)

  # Add dependency for copied mbed TLS headers when building with TF-M
  if (TARGET tfm_mbedtls_headers_copy)
    add_dependencies(${lib_name} tfm_mbedtls_headers_copy)
  endif()

  # Add defines (if set)
  if(DEFINED LIBRARY_DEFINES)
    nrf_security_debug("Add compile definitions for ${lib_name}: ${LIBRARY_DEFINES}")
    target_compile_definitions(${lib_name} PRIVATE ${LIBRARY_DEFINES})
  endif()

  # Add options (if set)
  if(DEFINED LIBRARY_OPTIONS)
    nrf_security_debug("Add compile definitions for ${lib_name}: ${LIBRARY_OPTIONS}")
    target_compile_options(${lib_name} PRIVATE ${LIBRARY_OPTIONS})
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
    set(lib_name ${mbedcrypto_target}_glue)
    set(backend_name dummy)
  else()
    set(lib_name ${mbedcrypto_target}_glue_${GLUE_LIB_BACKEND})
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

  # All regular includes, no generated includes (no xxxx-alt.h is added)
  target_link_libraries(${LIB_NAME} PRIVATE mbedcrypto_includes)

  # Add dependency for copied mbed TLS headers when building with TF-M
  if (TARGET tfm_mbedtls_headers_copy)
    add_dependencies(${LIB_NAME} tfm_mbedtls_headers_copy)
  endif()

  # Use configurations for vanilla as shared will always need configurations
  # that aren't enabling any alternate implemenatations.
  target_compile_definitions(${LIB_NAME} PRIVATE
    -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
  )

  nrf_security_debug_list_target_files(${LIB_NAME})
endfunction()

#
# Function to extract any imported libraries defined at the given locations.
#
# Each imported library will be extracted and renamed according to the algorithm
# defined by the `nrf_security_symbol_rename` function.
#
# The `nrf_security_symbol_strip` will determine any object to be dropped before
# appending to the target provided as argument.
#
# Arguments:
# TARGET:      Name of target where extracted object should be appended
# DIRECTORIES: List of CMake directories to search recursively for imported
#              targets
#
function(nrf_security_target_embed_libraries)
  cmake_parse_arguments(SEC_LIBS "" "TARGET" "DIRECTORIES" ${ARGN})
  foreach(dir ${SEC_LIBS_DIRECTORIES})
    zephyr_get_targets(${CMAKE_CURRENT_SOURCE_DIR}/${dir}
                       INTERFACE_LIBRARY interface_libraries
    )
  endforeach()

  foreach(target ${interface_libraries})
    get_property(libraries TARGET ${target} PROPERTY INTERFACE_LINK_LIBRARIES)

    foreach(library ${libraries})
      get_property(target_type TARGET ${library} PROPERTY TYPE)

      if(${target_type} STREQUAL STATIC_LIBRARY)
        get_property(imported_location
                     TARGET ${library}
                     PROPERTY IMPORTED_LOCATION
        )
        if(DEFINED imported_location)
          list(APPEND imported_libraries ${library})
        endif()
      endif()
    endforeach()
  endforeach()

  foreach(target ${imported_libraries})
    # add the library dep-list
    list(APPEND dep_list ${target})

    # if backend name is not set, there is no need to strip/rename
    get_property(backend_name TARGET ${target} PROPERTY BACKEND_NAME)

    if (CONFIG_NRF_SECURITY_MULTI_BACKEND AND DEFINED backend_name)
      # Rename functions in object files
      # Rename functions in object files
      nrf_security_configure_rename_file(${backend_name}
        ${NRF_SECURITY_ROOT}/src/mbedcrypto_glue/symbol_rename.template.txt
        ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${backend_name}.txt
      )
      get_property(imported_location TARGET ${target} PROPERTY IMPORTED_LOCATION)
      get_filename_component(libraryname ${imported_location} NAME)

      set(renamed_location ${CMAKE_CURRENT_BINARY_DIR}/rename/${backend_name}/${libraryname})

      add_custom_command(OUTPUT ${renamed_location}
        COMMAND
          ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/rename/${backend}
        COMMAND
          ${CMAKE_OBJCOPY}
            --redefine-syms ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${backend_name}.txt
            ${imported_location}
            ${renamed_location}
        DEPENDS
          ${imported_location}
      )
      add_custom_target(${target}_rename DEPENDS ${renamed_location})

      set(imported_library_location ${renamed_location})
      list(APPEND dep_list ${target}_rename)
    else()
      get_property(imported_library_location TARGET ${target} PROPERTY IMPORTED_LOCATION)
    endif()

    get_filename_component(library_file ${imported_library_location} NAME_WE)

    execute_process(
      COMMAND
        ${CMAKE_COMMAND}
          -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name}
    )
    execute_process(
      COMMAND
        ${CMAKE_COMMAND}
          -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name}.mon
    )

    if(NOT TARGET ${backend_name}_extract)
      add_custom_target(${backend_name}_extract
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name}.mon
      )
    endif()

    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name}.mon
      COMMAND ${CMAKE_COMMAND}
          -DCMAKE_AR=${CMAKE_AR}
          -DSTRIP="$<TARGET_PROPERTY:${backend_name}_extract,STRIP_LIST>"
          -DLIBRARIES="$<TARGET_PROPERTY:${backend_name}_extract,LIBRARIES>"
          -DMONITOR_FILE=${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name}.mon
          -P ${NRF_SECURITY_ROOT}/cmake/libraries_extract.cmake
      DEPENDS $<TARGET_PROPERTY:${backend_name}_extract,LIBRARIES>
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name}
    )

    set(strip_list)
    nrf_security_symbol_strip(${backend_name} strip_list)

    if(DEFINED strip_list)
      set_property(TARGET ${backend_name}_extract APPEND PROPERTY STRIP_LIST ${strip_list})
    endif()
    set_property(TARGET ${backend_name}_extract APPEND PROPERTY LIBRARIES ${imported_library_location})

    add_dependencies(${SEC_LIBS_TARGET} ${backend_name}_extract)

    list(APPEND archive ${CMAKE_CURRENT_BINARY_DIR}/objects/${backend_name})
  endforeach()

  if(DEFINED dep_list)
    add_dependencies(${SEC_LIBS_TARGET} ${dep_list})
  endif()

  list(REMOVE_DUPLICATES archive)
  foreach(arc ${archive})
    add_custom_command(TARGET ${SEC_LIBS_TARGET}
      POST_BUILD
      COMMAND ${CMAKE_AR} q $<TARGET_FILE:${SEC_LIBS_TARGET}> ${arc}/*
    )
  endforeach()
endfunction()

#
# Function to include objects from any CMake object libraries defined at the
# given locations.
#
# Each object library will be renamed according to the algorithm defined by the
# `nrf_security_symbol_rename` function.
#
# Arguments:
# TARGET:      Name of target where extracted object should be appended
# DIRECTORIES: List of CMake directories to search recursively for CMake Object
#              libraries
#
function(nrf_security_target_embed_objects)
  cmake_parse_arguments(SEC_LIBS "" "TARGET" "DIRECTORIES" ${ARGN})
  # Each interface lib may contain imported libs to re-archive.
  set(object_libraries)

  foreach(dir ${SEC_LIBS_DIRECTORIES})
    zephyr_get_targets(${CMAKE_CURRENT_SOURCE_DIR}/${dir} OBJECT_LIBRARY object_libraries)
  endforeach()

  foreach(target ${object_libraries})
    # add the library dep-list
    list(APPEND dep_list ${target})

    # if backend name is not set, there is no need to strip/rename
    get_property(backend_name TARGET ${target} PROPERTY BACKEND_NAME)

    # Get a copy of the objects in the target
    set(target_objs $<TARGET_OBJECTS:${target}>)

    if (CONFIG_NRF_SECURITY_MULTI_BACKEND AND DEFINED backend_name)
      # Rename functions in object files
      nrf_security_configure_rename_file(${backend_name}
        ${NRF_SECURITY_ROOT}/src/mbedcrypto_glue/symbol_rename.template.txt
        ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${backend_name}.txt
      )

      add_custom_command(TARGET ${SEC_LIBS_TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
          -DCMAKE_OBJCOPY=${CMAKE_OBJCOPY}
          -DCMAKE_AR=${CMAKE_AR}
          -DOBJECTS="$<TARGET_OBJECTS:${target}>"
          -DARCHIVE=$<TARGET_FILE:${SEC_LIBS_TARGET}>
          -DRENAME="${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${backend_name}.txt"
          -DOUT_FOLDER=${CMAKE_CURRENT_BINARY_DIR}/${backend_name}
          -P ${NRF_SECURITY_ROOT}/cmake/symbol_rename_archive_script.cmake
      )
    else()
      # Copy the list back (now stripped and/or renamed)
      list(APPEND obj_list ${target_objs})
    endif()
    list(APPEND dep_list ${target})
  endforeach()

  if(DEFINED dep_list)
    add_dependencies(${SEC_LIBS_TARGET} ${dep_list})
  endif()

  # Append to the static library.
  target_sources(${SEC_LIBS_TARGET} PRIVATE ${obj_list})
endfunction()
