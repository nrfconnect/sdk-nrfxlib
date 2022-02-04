#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
get_target_property(NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH mbedcrypto_cc3xx_imported INTERFACE_INCLUDE_DIRECTORIES)

if(CONFIG_SOC MATCHES "nRF53")
set_property(TARGET cc3xx_psa_crypto PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/nrf5340/libcc3xx_psa_crypto.a")
elseif(CONFIG_SOC MATCHES "nRF91")
  set_property(TARGET cc3xx_psa_crypto PROPERTY
               IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/nrf9160/libcc3xx_psa_crypto.a")
else()
set_property(TARGET cc3xx_psa_crypto PROPERTY
             IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/nrf52840/libcc3xx_psa_crypto.a")
endif()

# Link to library containing PSA Crypto Driver
target_link_libraries(${mbedcrypto_target}
  INTERFACE
    cc3xx_psa_crypto
    c
)

target_link_libraries(${mbedcrypto_target}
  INTERFACE
    mbedcrypto_cc3xx_imported
)

# Add nrf_cc3xx PSA Crypto Driver includes to mbedcrypto_common
target_include_directories(mbedcrypto_common
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${NRF_CC3XX_MBEDCRYPTO_INCLUDE_PATH}
)
