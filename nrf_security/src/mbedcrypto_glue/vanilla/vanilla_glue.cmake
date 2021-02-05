# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#

nrf_security_debug("######### Creating vanilla glue library #########")

#
# Create the vanilla glue library
#
nrf_security_glue_library(BACKEND vanilla
                          FILES ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c
                                ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c
                                ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c
)
