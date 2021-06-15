#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Script used to add object files to archive.
#
# The script requires the following parameters to be set
# CMAKE_AR : Archiver to use for extracting
# LIBRARY  : Library to which objects from FOLDER should be added.
# FOLDER   : Folder containing objects to add to archive.
#
file(GLOB files ${FOLDER}/*)

execute_process(COMMAND ${CMAKE_AR} q ${LIBRARY} ${files})
