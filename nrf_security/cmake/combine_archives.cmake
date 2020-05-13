#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# Function to combine multiple libraries to one single
# This uses CMAKE_AR and an MRI script to combine multiple libraries
#
# Items in the list "input_archives" will be resolved by the generator
# expression $<TARGET_FILE:library_name>
#
function(combine_archives output_archive input_archives)
  nrf_security_debug("Combining libraries to: ${output_archive}")
  set(mri_file ${CMAKE_CURRENT_BINARY_DIR}/${output_archive}.mri)
  set(mri_file_in ${CMAKE_CURRENT_BINARY_DIR}/${output_archive}.mri.in)
  nrf_security_debug("mri_file_in: ${mri_file_in}")
  nrf_security_debug("mri_file: ${mri_file}")
  set(FULL_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib${output_archive}.a)
  file(WRITE ${mri_file_in} "create ${FULL_OUTPUT_PATH}\n")
  foreach(in_archive ${input_archives})
    file(APPEND ${mri_file_in} "addlib $<TARGET_FILE:${in_archive}>\n")
    nrf_security_debug("Adding ${in_archive} to ${output_archive}")
  endforeach()
  file(APPEND ${mri_file_in} "save\n")
  file(APPEND ${mri_file_in} "end\n")

  add_library(${output_archive} STATIC ${ZEPHYR_BASE}/misc/empty_file.c)
  add_dependencies(${output_archive} ${input_archives})

  file(GENERATE
    OUTPUT ${mri_file}
    INPUT ${mri_file_in})

  add_custom_command(
    TARGET ${output_archive}
    POST_BUILD
    COMMAND ${CMAKE_AR} -M < ${mri_file}
  )
endfunction(combine_archives)