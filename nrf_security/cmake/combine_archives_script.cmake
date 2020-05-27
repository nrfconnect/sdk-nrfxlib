#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

#
# Helper script to allow expanding generator expressions
# in combine_archives function.
#
set(ARCHIVE_WORK_DIR ${CMAKE_CURRENT_BINARY_DIR}/extracted_archives)

string(REPLACE " " ";" list "${ARCHIVES_IN}")
file(WRITE ${ARCHIVE_WORK_DIR}/archive_content.txt "")

foreach(archive ${list})
  execute_process(
    COMMAND ${CMAKE_AR} x ${archive}
    COMMAND ${CMAKE_AR} t ${archive}
    OUTPUT_VARIABLE archive_content
    WORKING_DIRECTORY ${ARCHIVE_WORK_DIR}
  )
  file(APPEND ${ARCHIVE_WORK_DIR}/archive_content.txt "${archive_content}")
endforeach()

file(STRINGS ${ARCHIVE_WORK_DIR}/archive_content.txt content)
execute_process(
  COMMAND ${CMAKE_AR} qc ${ARCHIVE_OUT} ${content}
  WORKING_DIRECTORY ${ARCHIVE_WORK_DIR}
)
