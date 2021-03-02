# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

nrf_security_debug("######### Creating vanilla glue library #########")

#
# Create mbedcrypto_glue_vanilla
#
nrf_security_library_glue(BACKEND vanilla
  FILES
    ${CMAKE_CURRENT_LIST_DIR}/aes_vanilla.c
    ${CMAKE_CURRENT_LIST_DIR}/ccm_vanilla.c
    ${CMAKE_CURRENT_LIST_DIR}/dhm_vanilla.c
  LINK_LIBRARIES
    mbedcrypto_common_glue
    mbedcrypto_vanilla
)
