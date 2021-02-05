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
