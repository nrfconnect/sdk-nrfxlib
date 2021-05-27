#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Script used to redefine symbols using OBJCOPY
# This script is called with add_custom_target command on object libraries
#
# The script requires the following parameters to be set
# OBJECTS: List of objects to rename
# RENAME: Path to script containing information about renaming
#
# Note: The OUTPUT is the same as input in the current version of the script.
#       This will change in an upcoming refactoring
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E make_directory ${OUT_FOLDER}
)

foreach(obj ${OBJECTS})
  get_filename_component(obj_filename ${obj} NAME)
  execute_process(COMMAND ${CMAKE_OBJCOPY}
                          --redefine-syms ${RENAME}
                          ${obj}
                          ${OUT_FOLDER}/${obj_filename}
  )
  list(APPEND renamed_objects ${OUT_FOLDER}/${obj_filename})
endforeach()

execute_process(COMMAND ${CMAKE_AR}
                        q
                        ${ARCHIVE}
                        ${renamed_objects}
)
