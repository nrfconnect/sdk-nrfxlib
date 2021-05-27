# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrf_security_debug("######### Creating oberon glue library #########")

#
# Create the mbedcrypto_glue_oberon
#
nrf_security_library_glue(BACKEND oberon
  FILES
    ${CMAKE_CURRENT_LIST_DIR}/aes_oberon.c
    ${CMAKE_CURRENT_LIST_DIR}/ccm_oberon.c
  LINK_LIBRARIES
    ${mbedcrypto_target}_common_glue
    ${mbedcrypto_target}_oberon
)
