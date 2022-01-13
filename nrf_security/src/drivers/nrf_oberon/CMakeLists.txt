#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Link to libraries
if(CONFIG_MBEDTLS_PSA_CRYPTO_C)
  target_link_libraries(${mbedcrypto_target} PRIVATE
    mbedcrypto_oberon_psa_imported
    mbedcrypto_oberon_mbedtls_imported
    mbedcrypto_oberon_imported
  )

  # Add nrf_oberon PSA Crypto Driver includes to mbedcrypto_common
   target_include_directories(mbedcrypto_common
     INTERFACE
      ${NRFXLIB_ROOT}/crypto/nrf_oberon/include/
      ${NRFXLIB_ROOT}/crypto/nrf_oberon/include/mbedtls
      ${NRFXLIB_ROOT}/crypto/nrf_oberon/include/psa
  )

endif()
