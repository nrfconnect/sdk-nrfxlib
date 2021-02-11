#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Helper script to allow expanding generator expressions
# in combine_archives function.
#
set(ARCHIVE_WORK_DIR ${CMAKE_CURRENT_BINARY_DIR}/extracted_archives)

file(WRITE ${ARCHIVE_WORK_DIR}/archive_content.txt "")

set(COUNT 0)
while(ARCHIVES_IN_${COUNT})
  execute_process(
    COMMAND ${CMAKE_AR} x ${ARCHIVES_IN_${COUNT}}
    COMMAND ${CMAKE_AR} t ${ARCHIVES_IN_${COUNT}}
    OUTPUT_VARIABLE archive_content
    WORKING_DIRECTORY ${ARCHIVE_WORK_DIR}
  )
  file(APPEND ${ARCHIVE_WORK_DIR}/archive_content.txt "${archive_content}")
  math(EXPR COUNT "${COUNT} + 1" OUTPUT_FORMAT DECIMAL)
endwhile()

file(STRINGS ${ARCHIVE_WORK_DIR}/archive_content.txt content)
execute_process(
  COMMAND ${CMAKE_AR} qc ${ARCHIVE_OUT} ${content}
  WORKING_DIRECTORY ${ARCHIVE_WORK_DIR}
)
