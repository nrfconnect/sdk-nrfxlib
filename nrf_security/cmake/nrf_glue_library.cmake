#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

function(nrf_security_glue_library)
  set(option ALT)
  set(single BACKEND)
  set(multi FILES)
  cmake_parse_arguments(GLUE_LIB "${option}" "${single}" "${multi}" ${ARGN})
  set(sources "")

  foreach(file ${GLUE_LIB_FILES})
    get_filename_component(file_name ${file} NAME_WE)

    if(DEFINED GLUE_LIB_BACKEND)
      string(REPLACE "cc310" "cc3xx" file_name ${file_name})
      string(REPLACE "_${GLUE_LIB_BACKEND}" "" setting ${file_name})
      string(TOUPPER ${setting} SETTING_UPPER)
      string(TOUPPER ${GLUE_LIB_BACKEND} BACKEND_UPPER)

      if(CONFIG_GLUE_${BACKEND_UPPER}_MBEDTLS_${SETTING_UPPER}_C)
        nrf_security_debug("Adding to ${BACKEND_UPPER}_glue: ${SETTING_UPPER}")
        list(APPEND sources ${file})
      endif()
    endif()

    if(GLUE_LIB_ALT)
      string(REPLACE "_alt" "" setting ${file_name})
      string(TOUPPER ${setting} SETTING_UPPER)

      if(CONFIG_GLUE_MBEDTLS_${SETTING_UPPER}_C)
        nrf_security_debug("Adding to glue: ${SETTING_UPPER}")
        list(APPEND sources ${file})
      endif()
    endif()
  endforeach()

  if(NOT sources STREQUAL "")
    if(GLUE_LIB_ALT)
      zephyr_library_named(mbedcrypto_glue)
    else()
      zephyr_library_named(mbedcrypto_glue_${GLUE_LIB_BACKEND})
      zephyr_library_compile_definitions(MBEDTLS_BACKEND_PREFIX=${GLUE_LIB_BACKEND})
      add_dependencies(mbedcrypto_glue_${GLUE_LIB_BACKEND} mbedcrypto_${GLUE_LIB_BACKEND}_renamed)

      add_custom_command(
        TARGET mbedcrypto_glue_${GLUE_LIB_BACKEND}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
                --redefine-syms
                ${CMAKE_CURRENT_BINARY_DIR}/symbol_rename_${GLUE_LIB_BACKEND}.txt
                $<TARGET_FILE:mbedcrypto_glue_${GLUE_LIB_BACKEND}>
        DEPENDS mbedcrypto_${GLUE_LIB_BACKEND}_renamed
      )
    endif()

    zephyr_library_sources(${sources})
    zephyr_library_link_libraries(mbedtls_common_glue)
  endif()
  nrf_security_debug_list_target_files(${ZEPHYR_CURRENT_LIBRARY})
endfunction()


function(nrf_security_shared_library)
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

  if(NOT sources STREQUAL "")
    zephyr_library_named(${LIB_NAME})
    target_sources(${LIB_NAME} PRIVATE ${sources})

    #
    # Add specific includes for threading and platform_cc3xx
    #
    if(TARGET platform_cc3xx)
      target_include_directories(${LIB_NAME} PRIVATE
        ${mbedcrypto_glue_include_path}/threading
        $<TARGET_PROPERTY:platform_cc3xx,BASE_INCLUDE_PATH>
      )
    endif()

    target_compile_options(${LIB_NAME} PRIVATE ${TOOLCHAIN_C_FLAGS})
    target_ld_options(${LIB_NAME} PRIVATE ${TOOLCHAIN_LD_FLAGS})

    # Where are ${common_includes} and ${config_include} defined ?
    target_include_directories(${LIB_NAME} PRIVATE
        ${common_includes}
        ${config_include}
    )

    target_compile_definitions(${LIB_NAME} PRIVATE
      -DMBEDTLS_CONFIG_FILE="nrf-config-noglue.h"
    )

    nrf_security_debug_list_target_files(${LIB_NAME})
  endif()
endfunction()
