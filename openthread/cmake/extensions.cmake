#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

# Store the configuration of the compiled OpenThread libraries
# and set source and destination paths.
function(openthread_libs_configuration_write CONFIG_FILE)
  find_package(Git QUIET)
  if(GIT_FOUND)
    execute_process(
      COMMAND           ${GIT_EXECUTABLE} rev-parse HEAD
      WORKING_DIRECTORY ${ZEPHYR_OPENTHREAD_MODULE_DIR}
      RESULT_VARIABLE   git_return
      OUTPUT_VARIABLE   openthread_git_hash
      )
endif()

  # Store all OT related variables
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    if("${_variableName}" MATCHES "^CONFIG_OPENTHREAD_.*|^OT_.*")
      list(APPEND OPENTHREAD_SETTINGS "${_variableName}=${${_variableName}}\n")
    endif()
  endforeach()

  list(SORT OPENTHREAD_SETTINGS)
  list(INSERT OPENTHREAD_SETTINGS 0 "OpenThread_commit=${openthread_git_hash}\n")
  FILE(WRITE ${CONFIG_FILE} ${OPENTHREAD_SETTINGS})

endfunction(openthread_libs_configuration_write)
