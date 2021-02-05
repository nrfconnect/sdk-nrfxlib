#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
nrf_security_debug("######### Creating cc310 glue library #########")

#
# Create the cc3xx glue library
#
nrf_security_glue_library(BACKEND cc3xx
                          FILES ${CMAKE_CURRENT_LIST_DIR}/aes_cc310.c
                                ${CMAKE_CURRENT_LIST_DIR}/ccm_cc310.c
                                ${CMAKE_CURRENT_LIST_DIR}/dhm_cc310.c
)
