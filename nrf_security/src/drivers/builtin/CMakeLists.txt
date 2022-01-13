#
# Copyright (c) 2021-2022 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

# Compiling builtin support

# Note: An identical configuration exists in crypto_builtin_primitives
if (MBEDTLS_PSA_BUILTIN_ALG_STREAM_CIPHER OR
    MBEDTLS_PSA_BUILTIN_ALG_CTR OR
    MBEDTLS_PSA_BUILTIN_ALG_CFB OR
    MBEDTLS_PSA_BUILTIN_ALG_OFB OR
    MBEDTLS_PSA_BUILTIN_ALG_XTS OR
    MBEDTLS_PSA_BUILTIN_ALG_ECB_NO_PADDING OR
    MBEDTLS_PSA_BUILTIN_ALG_CBC_NO_PADDING OR
    MBEDTLS_PSA_BUILTIN_ALG_CBC_PKCS7)

set(MBEDTLS_PSA_BUILTIN_CIPHER  True)
endif()

# Custom configuration stating that
if (MBEDTLS_PSA_BUILTIN_ALG_DETERMINISTIC_ECDSA OR
    MBEDTLS_PSA_BUILTIN_ALG_ECDH OR
    MBEDTLS_PSA_BUILTIN_ALG_ECDSA)

  set(MBEDTLS_PSA_BUILTIN_ECC True)
endif()


if (MBEDTLS_PSA_BUILTIN_ALG_RSA_PKCS1V15_CRYPT OR
    MBEDTLS_PSA_BUILTIN_ALG_RSA_PKCS1V15_SIGN OR
    MBEDTLS_PSA_BUILTIN_ALG_RSA_OAEP OR
    MBEDTLS_PSA_BUILTIN_ALG_PSS
)
  set(MBEDTLS_PSA_BUILTIN_RSA True)
endif()

# Custom variable stating that sha256.c should be compiled in
if(MBEDTLS_PSA_BUILTIN_ALG_SHA_224 OR MBEDTLS_PSA_BUILTIN_ALG_SHA_256)
  set(MBEDTLS_PSA_BUILTIN_SHA224_OR_SHA256 True)
endif()

# Custom variable stating that sha512.c should be compiled in
if(MBEDTLS_PSA_BUILTIN_ALG_SHA_384 OR MBEDTLS_PSA_BUILTIN_ALG_SHA_512)
  set(MBEDTLS_PSA_BUILTIN_SHA384_OR_SHA512 True)
endif()

# Standard builting configurations
append_with_prefix_ifdef(MBEDTLS_PSA_BUILTIN_ALG_CCM src_crypto
  ${ARM_MBEDTLS_PATH}/library ccm.c)

append_with_prefix_ifdef(src_crypto MBEDTLS_PSA_BUILTIN_ALG_CHACHA20_POLY1305
  ${ARM_MBEDTLS_PATH}/library
  chacha20.c
  chachapoly.c
)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_ALG_GCM
  ${ARM_MBEDTLS_PATH}/library gcm.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_ALG_CMAC
  ${ARM_MBEDTLS_PATH}/library cmac.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_ALG_HKDF
  ${ARM_MBEDTLS_PATH}/library hkdf.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_ALG_HMAC
  ${ARM_MBEDTLS_PATH}/library hmac.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_RIPEMD160
  ${ARM_MBEDTLS_PATH}/library ripemd160.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_ALG_MD5
  ${ARM_MBEDTLS_PATH}/library md5.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_ALG_SHA_1
  ${ARM_MBEDTLS_PATH}/library sha1.c)

# Custom configurations
append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_CIPHER
  ${ARM_MBEDTLS_PATH}/library aes.c)

append_with_prefix_ifdef(src_crypto_builtin  MBEDTLS_PSA_BUILTIN_ECC
  ${ARM_MBEDTLS_PATH}/library
  ecp.c
  ecp_curve.C
)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_RSA
  ${ARM_MBEDTLS_PATH}/library
  rsa.c
  rsa_alt_helpers.c
)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_SHA224_OR_SHA256
  ${ARM_MBEDTLS_PATH}/library sha256.c)

append_with_prefix_ifdef(src_crypto_builtin MBEDTLS_PSA_BUILTIN_SHA384_OR_SHA512
  ${ARM_MBEDTLS_PATH}/library sha512.c)

# Add builtin functionality (if enabled)
if(DEFINED src_crypto_builtin)
  target_sources(${mbedcrypto_target} 
    PRIVATE
      src_crypto_builtin
  )
endif()
