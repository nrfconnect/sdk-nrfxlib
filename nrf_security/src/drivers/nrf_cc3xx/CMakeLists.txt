#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

add_library(cc3xx_mbedcrypto STATIC IMPORTED)
add_library(cc3xx_psa_crypto STATIC IMPORTED)

if(CONFIG_CC3XX_RELEASE_VERSION)
set_property(TARGET cc3xx_mbedcrypto PROPERTY
              IMPORTED_LOCATION "${NRF_SECURITY_ROOT}/../../sdk-nrf-cc3xx/build/release-0.9.11-out/external/nrf_cc312_mbedcrypto/lib/cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.11.a")
set_property(TARGET cc3xx_psa_crypto PROPERTY
             IMPORTED_LOCATION "${NRF_SECURITY_ROOT}/../../sdk-nrf-cc3xx/build/release-0.9.11-nrf5340/libcc3xx_psa_crypto.a")
else()
set_property(TARGET cc3xx_mbedcrypto PROPERTY
              IMPORTED_LOCATION "${NRF_SECURITY_ROOT}/../../sdk-nrf-cc3xx/build/debug-0.9.11-out/external/nrf_cc312_mbedcrypto/lib/cortex-m33/soft-float/no-interrupts/libnrf_cc312_mbedcrypto_0.9.11.a")
set_property(TARGET cc3xx_psa_crypto PROPERTY
             IMPORTED_LOCATION "${NRF_SECURITY_ROOT}/../../sdk-nrf-cc3xx/build/debug-0.9.11-nrf5340/libcc3xx_psa_crypto.a")
endif()

# Link to library containing PSA Crypto Driver
target_link_libraries(${mbedcrypto_target}
  PRIVATE
    mbedcrypto_common
    cc3xx_psa_crypto
    cc3xx_mbedcrypto
    c
)

# Add nrf_cc3xx PSA Crypto Driver includes to mbedcrypto_common
target_include_directories(mbedcrypto_common
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
