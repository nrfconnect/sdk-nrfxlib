# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

nrf_security_debug("######### Creating oberon glue library #########")

#
# Create the Oberon glue library
#
nrf_security_glue_library(BACKEND oberon
                          FILES ${CMAKE_CURRENT_LIST_DIR}/aes_oberon.c
                                ${CMAKE_CURRENT_LIST_DIR}/ccm_oberon.c
)
