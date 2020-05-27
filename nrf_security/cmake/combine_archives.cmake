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
  nrf_security_debug("mri_file: ${mri_file}")
 
  foreach(in_archive ${input_archives})
    list(APPEND input_archives_path "$<TARGET_FILE:${in_archive}>")
    nrf_security_debug("Adding ${in_archive} to ${output_archive}")
  endforeach()

  add_library(${output_archive} STATIC ${ZEPHYR_BASE}/misc/empty_file.c)
  add_dependencies(${output_archive} ${input_archives})

  add_custom_command(TARGET ${output_archive}
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND}
		             -DCMAKE_AR=${CMAKE_AR}
			     -DARCHIVE_OUT=${CMAKE_CURRENT_BINARY_DIR}/lib${output_archive}.a
			     -DARCHIVES_IN="${input_archives_path}"
			     -P ${ZEPHYR_NRFXLIB_MODULE_DIR}/nrf_security/cmake/combine_archives_script.cmake
  )
endfunction(combine_archives)
