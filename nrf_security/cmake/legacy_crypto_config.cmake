#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

#
# Remove previously generated files from include folder
#
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/include/)

kconfig_check_and_set_base(MBEDTLS_PLATFORM_MEMORY)
kconfig_check_and_set_base(MBEDTLS_PLATFORM_C)
kconfig_check_and_set_base(MBEDTLS_MEMORY_C)
kconfig_check_and_set_base(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
kconfig_check_and_set_base(MBEDTLS_CIPHER_C)
kconfig_check_and_set_base(MBEDTLS_NO_PLATFORM_ENTROPY)
kconfig_check_and_set_base(MBEDTLS_ENTROPY_C)
kconfig_check_and_set_base(MBEDTLS_CTR_DRBG_C)
kconfig_check_and_set_base(MBEDTLS_CTR_DRBG_USE_128_BIT_KEY)
kconfig_check_and_set_base(MBEDTLS_HMAC_DRBG_C)
kconfig_check_and_set_base(MBEDTLS_ENTROPY_FORCE_SHA256)
kconfig_check_and_set_base(MBEDTLS_AES_C)
kconfig_check_and_set_base(MBEDTLS_AES_SETKEY_ENC_ALT)
kconfig_check_and_set_base(MBEDTLS_AES_SETKEY_DEC_ALT)
kconfig_check_and_set_base(MBEDTLS_AES_ENCRYPT_ALT)
kconfig_check_and_set_base(MBEDTLS_AES_DECRYPT_ALT)
kconfig_check_and_set_base(MBEDTLS_CIPHER_MODE_CBC)
kconfig_check_and_set_base(MBEDTLS_CIPHER_PADDING_PKCS7)
kconfig_check_and_set_base(MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS)
kconfig_check_and_set_base(MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN)
kconfig_check_and_set_base(MBEDTLS_CIPHER_PADDING_ZEROS)
kconfig_check_and_set_base(MBEDTLS_CIPHER_MODE_CTR)
kconfig_check_and_set_base(MBEDTLS_CIPHER_MODE_CFB)
kconfig_check_and_set_base(MBEDTLS_CIPHER_MODE_OFB)
kconfig_check_and_set_base(MBEDTLS_CIPHER_MODE_XTS)
kconfig_check_and_set_base(MBEDTLS_CMAC_C)
kconfig_check_and_set_base(MBEDTLS_CCM_C)
kconfig_check_and_set_base(MBEDTLS_GCM_C)
kconfig_check_and_set_base(MBEDTLS_CHACHA20_ALT)
kconfig_check_and_set_base(MBEDTLS_CHACHA20_C)
kconfig_check_and_set_base(MBEDTLS_POLY1305_ALT)
kconfig_check_and_set_base(MBEDTLS_POLY1305_C)
kconfig_check_and_set_base(MBEDTLS_CHACHAPOLY_C)
kconfig_check_and_set_base(MBEDTLS_DHM_C)
kconfig_check_and_set_base(MBEDTLS_ECP_C)
kconfig_check_and_set_base(MBEDTLS_ECDH_C)
kconfig_check_and_set_base(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
kconfig_check_and_set_base(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
kconfig_check_and_set_base(MBEDTLS_ECDSA_C)
kconfig_check_and_set_base(MBEDTLS_ECDSA_DETERMINISTIC)
kconfig_check_and_set_base(MBEDTLS_ECDSA_GENKEY_ALT)
kconfig_check_and_set_base(MBEDTLS_ECDSA_SIGN_ALT)
kconfig_check_and_set_base(MBEDTLS_ECDSA_VERIFY_ALT)
kconfig_check_and_set_base(MBEDTLS_ECJPAKE_C)
kconfig_check_and_set_base(MBEDTLS_ECJPAKE_ALT)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP192K1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP224K1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_SECP256K1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_BP256R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_BP384R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_BP512R1_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
kconfig_check_and_set_base(MBEDTLS_ECP_DP_CURVE448_ENABLED)
kconfig_check_and_set_base(MBEDTLS_RSA_C)
kconfig_check_and_set_base(MBEDTLS_RSA_ALT)
Kconfig_check_and_set_base(MBEDTLS_GENPRIME)
kconfig_check_and_set_base(MBEDTLS_PKCS1_V15)
kconfig_check_and_set_base(MBEDTLS_PKCS1_V21)
kconfig_check_and_set_base(MBEDTLS_MD5_C)
kconfig_check_and_set_base(MBEDTLS_SHA1_ALT)
kconfig_check_and_set_base(MBEDTLS_SHA1_C)
kconfig_check_and_set_base(MBEDTLS_SHA224_ALT)
kconfig_check_and_set_base(MBEDTLS_SHA224_C)
kconfig_check_and_set_base(MBEDTLS_SHA256_ALT)
kconfig_check_and_set_base(MBEDTLS_SHA256_C)
kconfig_check_and_set_base(MBEDTLS_SHA384_C)
kconfig_check_and_set_base(MBEDTLS_SHA384_ALT)
kconfig_check_and_set_base(MBEDTLS_SHA512_C)
kconfig_check_and_set_base(MBEDTLS_SHA512_ALT)
kconfig_check_and_set_base(MBEDTLS_HKDF_C)
kconfig_check_and_set_base(MBEDTLS_MD_C)
kconfig_check_and_set_base(MBEDTLS_PK_C)
kconfig_check_and_set_base(MBEDTLS_PK_PARSE_C)
kconfig_check_and_set_base(MBEDTLS_PK_WRITE_C)

kconfig_check_and_set_base_to_one(MBEDTLS_PLATFORM_EXIT_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_PLATFORM_FPRINTF_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_PLATFORM_PRINTF_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_PLATFORM_SNPRINTF_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_ENTROPY_HARDWARE_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_THREADING_ALT)
kconfig_check_and_set_base_to_one(MBEDTLS_PLATFORM_ZEROIZE_ALT)


Kconfig_check_and_set_base(MBEDTLS_X509_USE_C)
Kconfig_check_and_set_base(MBEDTLS_X509_CREATE_C)
Kconfig_check_and_set_base(MBEDTLS_X509_CSR_WRITE_C)
Kconfig_check_and_set_base(MBEDTLS_X509_CHECK_KEY_USAGE)
Kconfig_check_and_set_base(MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE)
Kconfig_check_and_set_base(MBEDTLS_X509_CRL_PARSE_C)
Kconfig_check_and_set_base(MBEDTLS_X509_CRT_PARSE_C)
Kconfig_check_and_set_base(MBEDTLS_X509_CSR_PARSE_C)

Kconfig_check_and_set_base(MBEDTLS_SSL_PROTO_DTLS)
Kconfig_check_and_set_base(MBEDTLS_SSL_TLS_C)
Kconfig_check_and_set_base(MBEDTLS_SSL_PROTO_TLS1_2)

kconfig_check_and_set_base(MBEDTLS_SSL_RENEGOTIATION)
kconfig_check_and_set_base(MBEDTLS_SSL_DTLS_CLIENT_PORT_REUSE)
kconfig_check_and_set_base(MBEDTLS_SSL_DTLS_BADMAC_LIMIT)
kconfig_check_and_set_base(MBEDTLS_SSL_SESSION_TICKETS)
kconfig_check_and_set_base(MBEDTLS_SSL_SERVER_NAME_INDICATION)
kconfig_check_and_set_base(MBEDTLS_SSL_CACHE_C)
kconfig_check_and_set_base(MBEDTLS_SSL_TICKET_C)
kconfig_check_and_set_base(MBEDTLS_SSL_CONTEXT_SERIALIZATION)
kconfig_check_and_set_base(MBEDTLS_SSL_DTLS_CONNECTION_ID)

Kconfig_check_and_set_base(MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG)
#
# CC3XX flags for threading and platform zeroize
#
kconfig_check_and_set_base(MBEDTLS_THREADING_C)
kconfig_check_and_set_base(MBEDTLS_THREADING_ALT)
if (CONFIG_CC3XX_BACKEND)
  set(MBEDTLS_PLATFORM_ZEROIZE_ALT TRUE)
  if(NOT DEFINED MBEDTLS_THREADING_C)
    set(MBEDTLS_THREADING_C TRUE)
  endif()

  if(NOT DEFINED MBEDTLS_THREADING_C)
    set(MBEDTLS_THREADING_ALT TRUE)
  endif()
endif()




# Compare the following with check-config.h in mbed TLS
#
kconfig_check_and_set_base_depends(MBEDTLS_SSL_DTLS_ANTI_REPLAY MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
  MBEDTLS_SSL_PROTO_DTLS
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
  MBEDTLS_ECDH_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED
  MBEDTLS_RSA_C
  MBEDTLS_ECDH_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED
  MBEDTLS_DHM_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED
  MBEDTLS_ECDH_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
  MBEDTLS_DHM_C
  MBEDTLS_RSA_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
  MBEDTLS_PKCS1_V15
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
  MBEDTLS_ECDH_C
  MBEDTLS_RSA_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
  MBEDTLS_PKCS1_V15
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
  MBEDTLS_ECDH_C
  MBEDTLS_ECDSA_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_RSA_PSK_ENABLED
  MBEDTLS_RSA_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
  MBEDTLS_PKCS1_V15
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
  MBEDTLS_RSA_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
  MBEDTLS_PKCS1_V15
)

kconfig_check_and_set_base_depends(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED
  MBEDTLS_ECJPAKE_C
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
)

kconfig_check_and_set_base_depends(MBEDTLS_ECDSA_DETERMINISTIC
  MBEDTLS_ECDSA_C
  MBEDTLS_HMAC_DRBG_C
)

kconfig_check_and_set_base_depends(MBEDTLS_SSL_ENCRYPT_THEN_MAC
  MBEDTLS_SSL_PROTO_TLS1_2
)

kconfig_check_and_set_base_depends(MBEDTLS_SSL_EXTENDED_MASTER_SECRET
  MBEDTLS_SSL_PROTO_TLS1_2
)

kconfig_check_and_set_base_depends(MBEDTLS_SSL_SERVER_NAME_INDICATION
  MBEDTLS_X509_CRT_PARSE_C
)

# Set generated_include_path
set(generated_include_path "${CMAKE_CURRENT_BINARY_DIR}/include/generated")

if(CONFIG_GENERATE_MBEDTLS_CFG_FILE)
  # Generate the mbed TLS config file (default nrf-config.h)
  configure_file(${NRF_SECURITY_ROOT}/configs/legacy_crypto_config.h.template
    ${generated_include_path}/${CONFIG_MBEDTLS_CFG_FILE}
  )

  # Copy an empty user-config to help with legacy build
  configure_file(${NRF_SECURITY_ROOT}/configs/nrf-config-user-empty.h
    ${generated_include_path}/${CONFIG_MBEDTLS_USER_CONFIG_FILE}
  )
endif()
