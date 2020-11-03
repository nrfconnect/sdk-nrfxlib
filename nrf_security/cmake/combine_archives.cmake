#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# Function to combine multiple nrf security libraries to one single backend library
# This uses CMAKE_AR to combine multiple libraries
#
# Items in the list "input_archives" will be resolved by the generator
# expression $<TARGET_FILE:library_name>
#
function(nrf_security_combine_archives backend input_archives)
  nrf_security_debug("Combining libraries to: mbedcrypto_${backend}")

  set(COUNT 0)
  foreach(in_archive ${input_archives})
    list(APPEND archives_in -DARCHIVES_IN_${COUNT}=$<TARGET_FILE:${in_archive}>)
    nrf_security_debug("Adding ${in_archive} to mbedcrypto_${backend}")
    math(EXPR COUNT "${COUNT} + 1" OUTPUT_FORMAT DECIMAL)
  endforeach()

  # This interface library allows other targets to link to the custom generated library.
  # Together with the custom mbedcrypto_${backend}_target this ensures that dependencies
  # are setup so that linking with mbedcrypto_${backend} will drive the dependencies.
  add_library(mbedcrypto_${backend} INTERFACE)
  target_link_libraries(mbedcrypto_${backend} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/libmbedcrypto_${backend}.a)
  add_dependencies(mbedcrypto_${backend} mbedcrypto_${backend}_target)

  add_custom_target(mbedcrypto_${backend}_target DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libmbedcrypto_${backend}.a)

  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libmbedcrypto_${backend}.a
                     COMMAND ${CMAKE_COMMAND}
                             -DCMAKE_AR=${CMAKE_AR}
                             -DARCHIVE_OUT=${CMAKE_CURRENT_BINARY_DIR}/libmbedcrypto_${backend}.a
                             ${archives_in}
                             -P ${ZEPHYR_NRFXLIB_MODULE_DIR}/nrf_security/cmake/combine_archives_script.cmake
                     ${strip_command}
                     DEPENDS ${in_archive}
  )
endfunction(nrf_security_combine_archives)
