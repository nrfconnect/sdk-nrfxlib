#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Script used to extract and strip imported libraries.
# This script is called with add_custom_command command on imported libraries
#
# The script requires the following parameters to be set
# CMAKE_AR    : Archiver to use for extracting
# LIBRARIES   : List of libraries to extract
# STRIP       : List of extracted object files that should be removed
# MONITOR_FILE: Monitor file to create when library has been extracted
#
# Note: Files will be extracted to the current directory.
#
foreach(lib ${LIBRARIES})
  execute_process(COMMAND ${CMAKE_AR} x ${lib})
endforeach()

execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${STRIP})

execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${MONITOR_FILE})
