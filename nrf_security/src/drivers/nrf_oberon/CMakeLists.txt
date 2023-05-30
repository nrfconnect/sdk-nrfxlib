#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

set(drivers_path ${OBERON_PSA_CRYPTO_PATH}/oberon/drivers)

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
      ${drivers_path}
  )
endif()

if (COMPILE_PSA_APIS)
  list(APPEND                                                                  src_crypto_oberon ${drivers_path}/oberon_helpers.c)

  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_ACCEL_KEY_TYPES_OBERON src_crypto_oberon ${drivers_path} oberon_key_pair.c)

  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_AEAD_SUPPORT_OBERON    src_crypto_oberon ${drivers_path} oberon_aead.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_KDF_SUPPORT_OBERON     src_crypto_oberon ${drivers_path} oberon_kdf.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_MAC_SUPPORT_OBERON     src_crypto_oberon ${drivers_path} oberon_mac.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_CIPHER_SUPPORT_OBERON  src_crypto_oberon ${drivers_path} oberon_cipher.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_RSA_SUPPORT_OBERON     src_crypto_oberon ${drivers_path} oberon_rsa.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_HAS_HASH_SUPPORT_OBERON    src_crypto_oberon ${drivers_path} oberon_hash.c)

  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_ALG_ECDH_OBERON            src_crypto_oberon ${drivers_path} oberon_ecdh.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_ALG_ECDSA_OBERON           src_crypto_oberon ${drivers_path} oberon_ecdsa.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_ALG_CTR_DRBG_OBERON        src_crypto_oberon ${drivers_path} oberon_ctr_drbg.c)
  append_with_prefix_ifdef(CONFIG_PSA_CRYPTO_DRIVER_ALG_HMAC_DRBG_OBERON       src_crypto_oberon ${drivers_path} oberon_hmac_drbg.c)

  target_sources(${mbedcrypto_target} PRIVATE ${src_crypto_oberon})
endif()
