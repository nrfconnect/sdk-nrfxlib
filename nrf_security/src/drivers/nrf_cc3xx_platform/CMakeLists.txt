#
# Copyright (c) 2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Get include directories for nrf_cc3xx_platform
get_target_property(NRF_CC3XX_PLATFORM_INCLUDE_DIRECTORIES
  platform_cc3xx INTERFACE_INCLUDE_DIRECTORIES
)

# Link with nrf_platform_cc3xx for driver usage
target_link_libraries(${mbedcrypto_target}
  PRIVATE
    platform_cc3xx
)

# Add nrf_cc3xx_platform includes for mbedcrypto
target_include_directories(${mbedcrypto_target}
  INTERFACE
    ${NRF_CC3XX_PLATFORM_INCLUDE_DIRECTORIES}
)
