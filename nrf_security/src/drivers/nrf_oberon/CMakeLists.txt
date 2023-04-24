#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Link to libraries
if(CONFIG_MBEDTLS_PSA_CRYPTO_C)
  target_link_libraries(${mbedcrypto_target} PRIVATE
    mbedcrypto_oberon_mbedtls_imported
    mbedcrypto_oberon_imported
  )

  # Add nrf_oberon PSA Crypto Driver includes to mbedcrypto_common
   target_include_directories(mbedcrypto_common
     INTERFACE
      ${NRFXLIB_ROOT}/crypto/nrf_oberon/include/
      ${NRFXLIB_ROOT}/crypto/nrf_oberon/include/mbedtls
      ${OBERON_PSA_CRYPTO_PATH}/oberon/drivers
  )

endif()

if (COMPILE_PSA_APIS)
  append_with_prefix(src_crypto_oberon ${OBERON_PSA_CRYPTO_PATH}/oberon/drivers/
    oberon_aead.c
    oberon_cipher.c
    oberon_ecdh.c
    oberon_ecdsa.c
    oberon_hash.c
    oberon_helpers.c
    oberon_kdf.c
    oberon_key_pair.c
    oberon_mac.c
    oberon_rsa.c
  )

  # Add builtin functionality (if enabled)
  if(DEFINED src_crypto_oberon)
    target_sources(${mbedcrypto_target}
      PRIVATE
        ${src_crypto_oberon}
    )
  endif()
endif()
