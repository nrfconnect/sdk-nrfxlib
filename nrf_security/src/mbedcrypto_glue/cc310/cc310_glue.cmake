#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
nrf_security_debug("######### Creating cc310 glue library #########")

#
# Create mbedcrypto_glue_cc3xx
#
nrf_security_library_glue(BACKEND cc3xx
  FILES
    ${CMAKE_CURRENT_LIST_DIR}/aes_cc310.c
    ${CMAKE_CURRENT_LIST_DIR}/ccm_cc310.c
    ${CMAKE_CURRENT_LIST_DIR}/dhm_cc310.c
  LINK_LIBRARIES
    ${mbedcrypto_target}_common_glue
    ${mbedcrypto_target}_cc3xx
)
