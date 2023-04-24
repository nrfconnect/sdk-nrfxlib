/*
 * Copyright (c) 2016 - 2023 Nordic Semiconductor ASA
 * Copyright (c) since 2020 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


#ifndef OBERON_CONFIG_H
#define OBERON_CONFIG_H

#include "psa/crypto_driver_config.h"


/* Oberon AEAD Driver */

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CCM)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CCM)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_AES_CCM 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CCM)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_AES_CCM 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CCM)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_AES_CCM 1
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_GCM)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_GCM)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_AES_GCM 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_GCM)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_AES_GCM 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_GCM)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_AES_GCM 1
#endif
#endif

#if defined(PSA_WANT_ALG_CHACHA20_POLY1305) && !defined(PSA_ACCEL_CHACHA20_POLY1305)
#define PSA_NEED_OBERON_AEAD_DRIVER 1
#define PSA_NEED_OBERON_CHACHA20_POLY1305 1
#endif


/* Oberon Cipher Driver */

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CTR)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CTR)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CTR 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CTR)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CTR 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CTR)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CTR 1
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CBC_PKCS7)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CBC_PKCS7)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CBC_PKCS7 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CBC_PKCS7)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CBC_PKCS7 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CBC_PKCS7)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CBC_PKCS7 1
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CBC_NO_PADDING)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CBC_NO_PADDING)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CBC_NO_PADDING 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CBC_NO_PADDING)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CBC_NO_PADDING 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CBC_NO_PADDING)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CBC_NO_PADDING 1
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_ECB_NO_PADDING)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_ECB_NO_PADDING)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_ECB_NO_PADDING 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_ECB_NO_PADDING)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_ECB_NO_PADDING 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_ECB_NO_PADDING)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_ECB_NO_PADDING 1
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CCM_STAR_NO_TAG)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CCM_STAR_NO_TAG)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CCM_STAR_NO_TAG 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CCM_STAR_NO_TAG)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CCM_STAR_NO_TAG 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CCM_STAR_NO_TAG)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_AES_CCM_STAR_NO_TAG 1
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_CHACHA20) && defined(PSA_WANT_ALG_STREAM_CIPHER) && !defined(PSA_ACCEL_CHACHA20)
#define PSA_NEED_OBERON_CIPHER_DRIVER 1
#define PSA_NEED_OBERON_CHACHA20 1
#endif


/* Oberon ECDH Driver */

#if defined(PSA_WANT_ALG_ECDH) && defined(PSA_WANT_ECC_SECP_R1_224) && !defined(PSA_ACCEL_ECDH_P224)
#define PSA_NEED_OBERON_ECDH_DRIVER 1
#define PSA_NEED_OBERON_ECDH_P224 1
#endif

#if defined(PSA_WANT_ALG_ECDH) && defined(PSA_WANT_ECC_SECP_R1_256) && !defined(PSA_ACCEL_ECDH_P256)
#define PSA_NEED_OBERON_ECDH_DRIVER 1
#define PSA_NEED_OBERON_ECDH_P256 1
#endif

#if defined(PSA_WANT_ALG_ECDH) && defined(PSA_WANT_ECC_SECP_R1_384) && !defined(PSA_ACCEL_ECDH_P384)
#define PSA_NEED_OBERON_ECDH_DRIVER 1
#define PSA_NEED_OBERON_ECDH_P384 1
#endif

#if defined(PSA_WANT_ALG_ECDH) && defined(PSA_WANT_ECC_MONTGOMERY_255) && !defined(PSA_ACCEL_ECDH_X25519)
#define PSA_NEED_OBERON_ECDH_DRIVER 1
#define PSA_NEED_OBERON_ECDH_X25519 1
#endif


/* Oberon ECDSA Driver */

#if defined(PSA_WANT_ALG_ECDSA) || defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA)
#define PSA_WANT_ALG_ANY_ECDSA
#endif

#if defined(PSA_WANT_ALG_ANY_ECDSA) && defined(PSA_WANT_ECC_SECP_R1_224)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_ECDSA_P224_SHA1)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P224 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_ECDSA_P224_SHA224)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P224 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_ECDSA_P224_SHA256)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P224 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_ECDSA_P224_SHA384)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P224 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_ECDSA_P224_SHA512)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P224 1
#endif
#endif

#if defined(PSA_WANT_ALG_ANY_ECDSA) && defined(PSA_WANT_ECC_SECP_R1_256)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_ECDSA_P256_SHA1)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P256 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_ECDSA_P256_SHA224)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P256 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_ECDSA_P256_SHA256)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P256 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_ECDSA_P256_SHA384)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P256 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_ECDSA_P256_SHA512)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P256 1
#endif
#endif

#if defined(PSA_WANT_ALG_ANY_ECDSA) && defined(PSA_WANT_ECC_SECP_R1_384)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_ECDSA_P384_SHA1)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P384 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_ECDSA_P384_SHA224)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P384 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_ECDSA_P384_SHA256)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P384 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_ECDSA_P384_SHA384)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P384 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_ECDSA_P384_SHA512)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_P384 1
#endif
#endif

#if defined(PSA_WANT_ALG_PURE_EDDSA) && defined(PSA_WANT_ECC_TWISTED_EDWARDS_255) && !defined(PSA_ACCEL_ECDSA_ED25519)
#define PSA_NEED_OBERON_ECDSA_DRIVER 1
#define PSA_NEED_OBERON_ECDSA_ED25519 1
#endif

#if defined(PSA_NEED_OBERON_ECDSA_DRIVER) && defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA)
#define PSA_NEED_OBERON_DETERMINISTIC_ECDSA 1
#endif

#if defined(PSA_NEED_OBERON_ECDSA_DRIVER) && defined(PSA_WANT_ALG_ECDSA)
#define PSA_NEED_OBERON_RANDOMIZED_ECDSA 1
#endif


/* Oberon Hash Driver */

#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_SHA1)
#define PSA_NEED_OBERON_HASH_DRIVER 1
#define PSA_NEED_OBERON_SHA_1 1
#endif

#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_SHA224)
#define PSA_NEED_OBERON_HASH_DRIVER 1
#define PSA_NEED_OBERON_SHA_224 1
#endif

#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_SHA256)
#define PSA_NEED_OBERON_HASH_DRIVER 1
#define PSA_NEED_OBERON_SHA_256 1
#endif

#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_SHA384)
#define PSA_NEED_OBERON_HASH_DRIVER 1
#define PSA_NEED_OBERON_SHA_384 1
#endif

#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_SHA512)
#define PSA_NEED_OBERON_HASH_DRIVER 1
#define PSA_NEED_OBERON_SHA_512 1
#endif


/* Oberon Key Pair Driver */

#if defined(PSA_WANT_ECC_SECP_R1_224) && !defined(PSA_ACCEL_KEY_PAIR_P224)
#define PSA_NEED_OBERON_KEY_PAIR_DRIVER 1
#define PSA_NEED_OBERON_KEY_PAIR_P224 1
#define PSA_NEED_OBERON_KEY_PAIR_SECP 1
#endif

#if defined(PSA_WANT_ECC_SECP_R1_256) && !defined(PSA_ACCEL_KEY_PAIR_P256)
#define PSA_NEED_OBERON_KEY_PAIR_DRIVER 1
#define PSA_NEED_OBERON_KEY_PAIR_P256 1
#define PSA_NEED_OBERON_KEY_PAIR_SECP 1
#endif

#if defined(PSA_WANT_ECC_SECP_R1_384) && !defined(PSA_ACCEL_KEY_PAIR_P384)
#define PSA_NEED_OBERON_KEY_PAIR_DRIVER 1
#define PSA_NEED_OBERON_KEY_PAIR_P384 1
#define PSA_NEED_OBERON_KEY_PAIR_SECP 1
#endif

#if defined(PSA_WANT_ECC_MONTGOMERY_255) && !defined(PSA_ACCEL_KEY_PAIR_X25519)
#define PSA_NEED_OBERON_KEY_PAIR_DRIVER 1
#define PSA_NEED_OBERON_KEY_PAIR_X25519 1
#define PSA_NEED_OBERON_KEY_PAIR_25519 1
#endif

#if defined(PSA_WANT_ECC_TWISTED_EDWARDS_255) && !defined(PSA_ACCEL_KEY_PAIR_ED25519)
#define PSA_NEED_OBERON_KEY_PAIR_DRIVER 1
#define PSA_NEED_OBERON_KEY_PAIR_ED25519 1
#define PSA_NEED_OBERON_KEY_PAIR_25519 1
#endif


/* Oberon MAC Driver */

#if defined(PSA_WANT_ALG_HMAC)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_HMAC_SHA1)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_HMAC_SHA224)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_HMAC_SHA256)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_HMAC_SHA384)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_HMAC_SHA512)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_HMAC 1
#endif
#endif

#if defined(PSA_WANT_ALG_CMAC)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CMAC)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_CMAC 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CMAC)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_CMAC 1
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CMAC)
#define PSA_NEED_OBERON_MAC_DRIVER 1
#define PSA_NEED_OBERON_CMAC 1
#endif
#endif


/* Oberon KDF Driver */

#if defined(PSA_WANT_ALG_HKDF)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_HKDF_SHA1)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_HKDF_SHA224)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_HKDF_SHA256)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_HKDF_SHA384)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_HKDF_SHA512)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF 1
#endif
#endif

#if defined(PSA_WANT_ALG_HKDF_EXTRACT)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_HKDF_EXTRACT_SHA1)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXTRACT 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_HKDF_EXTRACT_SHA224)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXTRACT 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_HKDF_EXTRACT_SHA256)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXTRACT 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_HKDF_EXTRACT_SHA384)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXTRACT 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_HKDF_EXTRACT_SHA512)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXTRACT 1
#endif
#endif

#if defined(PSA_WANT_ALG_HKDF_EXPAND)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_HKDF_EXPAND_SHA1)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXPAND 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_HKDF_EXPAND_SHA224)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXPAND 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_HKDF_EXPAND_SHA256)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXPAND 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_HKDF_EXPAND_SHA384)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXPAND 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_HKDF_EXPAND_SHA512)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_HKDF_EXPAND 1
#endif
#endif

#if defined(PSA_WANT_ALG_TLS12_PRF)
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_TLS12_PRF_SHA256)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_TLS12_PRF 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_TLS12_PRF_SHA384)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_TLS12_PRF 1
#endif
#endif

#if defined(PSA_WANT_ALG_TLS12_PSK_TO_MS)
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_TLS12_PSK_TO_MS_SHA256)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_TLS12_PSK_TO_MS 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_TLS12_PSK_TO_MS_SHA384)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_TLS12_PSK_TO_MS 1
#endif
#endif

#if defined(PSA_WANT_ALG_PBKDF2_HMAC)
#if defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_PBKDF2_HMAC_SHA1)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_PBKDF2_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_PBKDF2_HMAC_SHA224)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_PBKDF2_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_PBKDF2_HMAC_SHA256)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_PBKDF2_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_PBKDF2_HMAC_SHA384)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_PBKDF2_HMAC 1
#endif
#if defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_PBKDF2_HMAC_SHA512)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_PBKDF2_HMAC 1
#endif
#endif

#if defined(PSA_WANT_ALG_PBKDF2_AES_CMAC_PRF_128) && !defined(PSA_ACCEL_AES_CMAC_PRF_128)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_PBKDF2_AES_CMAC_PRF_128 1
#endif

#if defined(PSA_WANT_ALG_TLS12_ECJPAKE_TO_PMS) && !defined(PSA_ACCEL_ECJPAKE_TO_PMS)
#define PSA_NEED_OBERON_KDF_DRIVER 1
#define PSA_NEED_OBERON_ECJPAKE_TO_PMS 1
#endif


/* Oberon PAKE Driver */

#if defined(PSA_WANT_ALG_JPAKE)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_ECJPAKE_P256_SHA1)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_ECJPAKE_P256_SHA224)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_ECJPAKE_P256_SHA256)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_ECJPAKE_P256_SHA384)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_ECJPAKE_P256_SHA512))
#define PSA_NEED_OBERON_JPAKE_DRIVER 1
#define PSA_NEED_OBERON_ECJPAKE_P256 1
#endif
#endif

#if defined(PSA_WANT_ALG_SPAKE2P)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_SPAKE2P_P256_SHA1)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_SPAKE2P_P256_SHA224)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_SPAKE2P_P256_SHA256)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_SPAKE2P_P256_SHA384)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_SPAKE2P_P256_SHA512))
#define PSA_NEED_OBERON_SPAKE2P_DRIVER 1
#define PSA_NEED_OBERON_SPAKE2P_P256 1
#endif
#endif

#if defined(PSA_WANT_ALG_SRP_6)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_SRP_6_3072_SHA1)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_SRP_6_3072_SHA224)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_SRP_6_3072_SHA256)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_SRP_6_3072_SHA384)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_SRP_6_3072_SHA512))
#define PSA_NEED_OBERON_SRP_DRIVER 1
#define PSA_NEED_OBERON_SRP_6_3072 1
#endif
#endif


/* Oberon RSA Driver */

#if defined(PSA_WANT_ALG_RSA_PSS)
#if defined(PSA_WANT_RSA_KEY_SIZE_1024)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA1024_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA1024_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA1024_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA1024_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA1024_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1024 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_1536)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA1536_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA1536_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA1536_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA1536_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA1536_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1536 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_2048)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA2048_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA2048_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA2048_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA2048_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA2048_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_2048 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_3072)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA3072_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA3072_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA3072_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA3072_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA3072_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_3072 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_4096)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA4096_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA4096_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA4096_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA4096_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA4096_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_4096 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_6144)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA6144_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA6144_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA6144_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA6144_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA6144_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_6144 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_8192)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA8192_SHA1_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA8192_SHA224_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA8192_SHA256_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA8192_SHA384_PSS)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA8192_SHA512_PSS))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PSS 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_8192 1
#endif
#endif
#endif

#if defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN)
#if defined(PSA_WANT_RSA_KEY_SIZE_1024)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA1024_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA1024_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA1024_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA1024_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA1024_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1024 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_1536)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA1536_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA1536_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA1536_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA1536_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA1536_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1536 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_2048)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA2048_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA2048_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA2048_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA2048_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA2048_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_2048 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_3072)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA3072_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA3072_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA3072_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA3072_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA3072_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_3072 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_4096)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA4096_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA4096_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA4096_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA4096_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA4096_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_4096 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_6144)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA6144_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA6144_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA6144_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA6144_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA6144_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_6144 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_8192)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA8192_SHA1_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA8192_SHA224_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA8192_SHA256_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA8192_SHA384_PKCS1V15_SIGN)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA8192_SHA512_PKCS1V15_SIGN))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_SIGN 1
#define PSA_NEED_OBERON_RSA_SIGN 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_8192 1
#endif
#endif
#endif

#if defined(PSA_WANT_ALG_RSA_OAEP)
#if defined(PSA_WANT_RSA_KEY_SIZE_1024)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA1024_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA1024_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA1024_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA1024_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA1024_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1024 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_1536)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA1536_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA1536_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA1536_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA1536_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA1536_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1536 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_2048)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA2048_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA2048_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA2048_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA2048_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA2048_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_2048 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_3072)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA3072_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA3072_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA3072_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA3072_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA3072_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_3072 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_4096)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA4096_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA4096_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA4096_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA4096_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA4096_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_4096 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_6144)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA6144_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA6144_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA6144_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA6144_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA6144_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_6144 1
#endif
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_8192)
#if (defined(PSA_WANT_ALG_SHA_1) && !defined(PSA_ACCEL_RSA8192_SHA1_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_224) && !defined(PSA_ACCEL_RSA8192_SHA224_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_256) && !defined(PSA_ACCEL_RSA8192_SHA256_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_384) && !defined(PSA_ACCEL_RSA8192_SHA384_OAEP)) || \
    (defined(PSA_WANT_ALG_SHA_512) && !defined(PSA_ACCEL_RSA8192_SHA512_OAEP))
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_OAEP 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_8192 1
#endif
#endif
#endif

#if defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT)
#if defined(PSA_WANT_RSA_KEY_SIZE_1024) && !defined(PSA_ACCEL_RSA1024_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1024 1
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_1536) && !defined(PSA_ACCEL_RSA1536_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_1536 1
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_2048) && !defined(PSA_ACCEL_RSA2048_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_2048 1
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_3072) && !defined(PSA_ACCEL_RSA3072_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_3072 1
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_4096) && !defined(PSA_ACCEL_RSA4096_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_4096 1
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_6144) && !defined(PSA_ACCEL_RSA6144_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_6144 1
#endif
#if defined(PSA_WANT_RSA_KEY_SIZE_8192) && !defined(PSA_ACCEL_RSA8192_PKCS1V15_CRYPT)
#define PSA_NEED_OBERON_RSA_DRIVER 1
#define PSA_NEED_OBERON_RSA_PKCS1V15_CRYPT 1
#define PSA_NEED_OBERON_RSA_CRYPT 1
#define PSA_NEED_OBERON_RSA_KEY_SIZE_8192 1
#endif
#endif

#if defined(PSA_WANT_RSA_KEY_SIZE_8192)
#define PSA_MAX_RSA_KEY_BITS 8192
#elif defined(PSA_WANT_RSA_KEY_SIZE_6144)
#define PSA_MAX_RSA_KEY_BITS 6144
#elif defined(PSA_WANT_RSA_KEY_SIZE_4096)
#define PSA_MAX_RSA_KEY_BITS 4096
#elif defined(PSA_WANT_RSA_KEY_SIZE_3072)
#define PSA_MAX_RSA_KEY_BITS 3072
#elif defined(PSA_WANT_RSA_KEY_SIZE_2048)
#define PSA_MAX_RSA_KEY_BITS 2048
#elif defined(PSA_WANT_RSA_KEY_SIZE_1536)
#define PSA_MAX_RSA_KEY_BITS 1536
#elif defined(PSA_WANT_RSA_KEY_SIZE_1024)
#define PSA_MAX_RSA_KEY_BITS 1024
#else
#define PSA_MAX_RSA_KEY_BITS 0
#endif

#define PSA_MAX_RSA_KEY_SIZE PSA_BITS_TO_BYTES(PSA_MAX_RSA_KEY_BITS)


/* Oberon Random Driver */

#if defined(PSA_WANT_GENERATE_RANDOM)

#if defined(PSA_USE_CTR_DRBG_DRIVER) && !defined(PSA_ACCEL_RANDOM)
#define PSA_NEED_OBERON_CTR_DRBG_DRIVER 1
#define PSA_ACCEL_RANDOM
#endif
#if defined(PSA_USE_HMAC_DRBG_DRIVER) && !defined(PSA_ACCEL_RANDOM)
#define PSA_NEED_OBERON_HMAC_DRBG_DRIVER 1
#define PSA_ACCEL_RANDOM
#endif

#if !defined(PSA_ACCEL_RANDOM)
#error "PSA_WANT_GENERATE_RANDOM defined, but no random driver"
#endif

#if !defined(PSA_ACCEL_ENTROPY)
#error "PSA_WANT_GENERATE_RANDOM defined, but no entropy driver"
#endif

#endif


/* Currently Unsupported Algorithms */

#if defined(PSA_WANT_ALG_SHA_512_224) && !defined(PSA_ACCEL_SHA_512_224)
#error "No software implementation for SHA-512-224"
#endif
#if defined(PSA_WANT_ALG_SHA_512_256) && !defined(PSA_ACCEL_SHA_512_256)
#error "No software implementation for SHA-512-256"
#endif
#if defined(PSA_WANT_ALG_SHA3_224) && !defined(PSA_ACCEL_SHA3_224)
#error "No software implementation for SHA3-224"
#endif
#if defined(PSA_WANT_ALG_SHA3_256) && !defined(PSA_ACCEL_SHA3_256)
#error "No software implementation for SHA3-256"
#endif
#if defined(PSA_WANT_ALG_SHA3_384) && !defined(PSA_ACCEL_SHA3_384)
#error "No software implementation for SHA3-384"
#endif
#if defined(PSA_WANT_ALG_SHA3_512) && !defined(PSA_ACCEL_SHA3_512)
#error "No software implementation for SHA3-512"
#endif
#if defined(PSA_WANT_ALG_SHAKE256_512) && !defined(PSA_ACCEL_SHAKE256_512)
#error "No software implementation for SHAKE-256"
#endif
#if defined(PSA_WANT_ALG_MD5) && !defined(PSA_ACCEL_MD5)
#error "No software implementation for MD5"
#endif
#if defined(PSA_WANT_ALG_RIPEMD160) && !defined(PSA_ACCEL_RIPEMD160)
#error "No software implementation for RIPEMD160"
#endif

#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CFB)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CFB)
#error "No software implementation for 128 bit AES-CFB"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CFB)
#error "No software implementation for 192 bit AES-CFB"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CFB)
#error "No software implementation for 256 bit AES-CFB"
#endif
#endif
#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_OFB)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_OFB)
#error "No software implementation for 128 bit AES-OFB"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_OFB)
#error "No software implementation for 192 bit AES-OFB"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_OFB)
#error "No software implementation for 256 bit AES-OFB"
#endif
#endif
#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_XTS)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_XTS)
#error "No software implementation for 128 bit AES-XTS"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_XTS)
#error "No software implementation for 192 bit AES-XTS"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_XTS)
#error "No software implementation for 256 bit AES-XTS"
#endif
#endif
#if defined(PSA_WANT_KEY_TYPE_AES) && defined(PSA_WANT_ALG_CBC_MAC)
#if defined(PSA_WANT_AES_KEY_SIZE_128) && !defined(PSA_ACCEL_AES128_CBC_MAC)
#error "No software implementation for 128 bit AES-CBC-MAC"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_192) && !defined(PSA_ACCEL_AES192_CBC_MAC)
#error "No software implementation for 192 bit AES-CBC-MAC"
#endif
#if defined(PSA_WANT_AES_KEY_SIZE_256) && !defined(PSA_ACCEL_AES256_CBC_MAC)
#error "No software implementation for 256 bit AES-CBC-MAC"
#endif
#endif

#if defined(PSA_WANT_ALG_ECDH) && defined(PSA_WANT_ECC_MONTGOMERY_448) && !defined(PSA_ACCEL_ECDH_X448)
#error "No software implementation for X448"
#endif
#if defined(PSA_WANT_ALG_PURE_EDDSA) && defined(PSA_WANT_ECC_TWISTED_EDWARDS_448) && !defined(PSA_ACCEL_ECDSA_ED448)
#error "No software implementation for ED448"
#endif
#if defined(PSA_WANT_ALG_ED25519PH) && !defined(PSA_ACCEL_ED25519PH)
#error "No software implementation for pre-hashed ED25519"
#endif
#if defined(PSA_WANT_ALG_ED448PH) && !defined(PSA_ACCEL_ED448PH)
#error "No software implementation for pre-hashed ED448"
#endif

#if defined(PSA_WANT_ALG_FFDH)
#if defined(PSA_WANT_DH_KEY_SIZE_2048) && !defined(PSA_ACCEL_FFDH_2048)
#error "No software implementation for 2048 bit FFDH"
#endif
#if defined(PSA_WANT_DH_KEY_SIZE_3072) && !defined(PSA_ACCEL_FFDH_3072)
#error "No software implementation for 3072 bit FFDH"
#endif
#if defined(PSA_WANT_DH_KEY_SIZE_4096) && !defined(PSA_ACCEL_FFDH_4096)
#error "No software implementation for 4096 bit FFDH"
#endif
#if defined(PSA_WANT_DH_KEY_SIZE_6144) && !defined(PSA_ACCEL_FFDH_6144)
#error "No software implementation for 6144 bit FFDH"
#endif
#if defined(PSA_WANT_DH_KEY_SIZE_8192) && !defined(PSA_ACCEL_FFDH_8192)
#error "No software implementation for 8192 bit FFDH"
#endif
#endif

#if defined(PSA_WANT_ECC_SECP_K1_192) && !defined(PSA_ACCEL_KEY_PAIR_K192)
#error "No software implementation for secp-k1-192"
#endif
#if defined(PSA_WANT_ECC_SECP_K1_224) && !defined(PSA_ACCEL_KEY_PAIR_K224)
#error "No software implementation for secp-k1-224"
#endif
#if defined(PSA_WANT_ECC_SECP_K1_256) && !defined(PSA_ACCEL_KEY_PAIR_K256)
#error "No software implementation for secp-k1-256"
#endif

#if defined(PSA_WANT_ECC_SECP_R1_192) && !defined(PSA_ACCEL_KEY_PAIR_R192)
#error "No software implementation for secp-r1-192"
#endif
#if defined(PSA_WANT_ECC_SECP_R1_521) && !defined(PSA_ACCEL_KEY_PAIR_R521)
#error "No software implementation for secp-r1-521"
#endif

#if defined(PSA_WANT_ECC_SECT_K1_163) && !defined(PSA_ACCEL_KEY_PAIR_K163)
#error "No software implementation for sect-k1-163"
#endif
#if defined(PSA_WANT_ECC_SECT_K1_233) && !defined(PSA_ACCEL_KEY_PAIR_K233)
#error "No software implementation for sect-k1-233"
#endif
#if defined(PSA_WANT_ECC_SECT_K1_239) && !defined(PSA_ACCEL_KEY_PAIR_K239)
#error "No software implementation for sect-k1-239"
#endif
#if defined(PSA_WANT_ECC_SECT_K1_283) && !defined(PSA_ACCEL_KEY_PAIR_K283)
#error "No software implementation for sect-k1-283"
#endif
#if defined(PSA_WANT_ECC_SECT_K1_409) && !defined(PSA_ACCEL_KEY_PAIR_K409)
#error "No software implementation for sect-k1-409"
#endif
#if defined(PSA_WANT_ECC_SECT_K1_571) && !defined(PSA_ACCEL_KEY_PAIR_K571)
#error "No software implementation for sect-k1-571"
#endif

#if defined(PSA_WANT_ECC_SECT_R1_163) && !defined(PSA_ACCEL_KEY_PAIR_R163)
#error "No software implementation for sect-r1-163"
#endif
#if defined(PSA_WANT_ECC_SECT_R1_233) && !defined(PSA_ACCEL_KEY_PAIR_R233)
#error "No software implementation for sect-r1-233"
#endif
#if defined(PSA_WANT_ECC_SECT_R1_283) && !defined(PSA_ACCEL_KEY_PAIR_R283)
#error "No software implementation for sect-r1-283"
#endif
#if defined(PSA_WANT_ECC_SECT_R1_409) && !defined(PSA_ACCEL_KEY_PAIR_R409)
#error "No software implementation for sect-r1-409"
#endif
#if defined(PSA_WANT_ECC_SECT_R1_571) && !defined(PSA_ACCEL_KEY_PAIR_R571)
#error "No software implementation for sect-r1-571"
#endif

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_160) && !defined(PSA_ACCEL_KEY_PAIR_PB160)
#error "No software implementation for brainpoolP160r1"
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_192) && !defined(PSA_ACCEL_KEY_PAIR_PB192)
#error "No software implementation for brainpoolP192r1"
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_224) && !defined(PSA_ACCEL_KEY_PAIR_PB224)
#error "No software implementation for brainpoolP224r1"
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_256) && !defined(PSA_ACCEL_KEY_PAIR_PB256)
#error "No software implementation for brainpoolP256r1"
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_320) && !defined(PSA_ACCEL_KEY_PAIR_PB320)
#error "No software implementation for brainpoolP320r1"
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_384) && !defined(PSA_ACCEL_KEY_PAIR_PB384)
#error "No software implementation for brainpoolP384r1"
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_512) && !defined(PSA_ACCEL_KEY_PAIR_PB512)
#error "No software implementation for brainpoolP512r1"
#endif

#if defined(PSA_WANT_KEY_TYPE_ARIA) && !defined(PSA_ACCEL_ARIA)
#error "No software implementation for ARIA"
#endif
#if defined(PSA_WANT_KEY_TYPE_CAMELLIA) && !defined(PSA_ACCEL_CAMELLIA)
#error "No software implementation for CAMELLIA"
#endif
#if defined(PSA_WANT_KEY_TYPE_DES) && !defined(PSA_ACCEL_DES)
#error "No software implementation for DES"
#endif


/* Set all mbedTLS algorithms to accelerated to disable all mbedTLS fallback functions */

#define MBEDTLS_PSA_ACCEL_ALG_CBC_NO_PADDING 1
#define MBEDTLS_PSA_ACCEL_ALG_CBC_PKCS7 1
#define MBEDTLS_PSA_ACCEL_ALG_CFB 1
#define MBEDTLS_PSA_ACCEL_ALG_CTR 1
#define MBEDTLS_PSA_ACCEL_ALG_DETERMINISTIC_ECDSA 1
#define MBEDTLS_PSA_ACCEL_ALG_ECDSA 1
#define MBEDTLS_PSA_ACCEL_ALG_MD5 1
#define MBEDTLS_PSA_ACCEL_ALG_OFB 1
#define MBEDTLS_PSA_ACCEL_ALG_RIPEMD160 1
#define MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_SIGN 1
#define MBEDTLS_PSA_ACCEL_ALG_RSA_PSS 1
#define MBEDTLS_PSA_ACCEL_ALG_SHA_1 1
#define MBEDTLS_PSA_ACCEL_ALG_SHA_224 1
#define MBEDTLS_PSA_ACCEL_ALG_SHA_256 1
#define MBEDTLS_PSA_ACCEL_ALG_SHA_384 1
#define MBEDTLS_PSA_ACCEL_ALG_SHA_512 1
#define MBEDTLS_PSA_ACCEL_ALG_XTS 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_AES 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ARIA 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_CAMELLIA 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_PUBLIC_KEY 1

#define MBEDTLS_PSA_ACCEL_ALG_CBC_MAC 1
#define MBEDTLS_PSA_ACCEL_ALG_CCM 1
#define MBEDTLS_PSA_ACCEL_ALG_CHACHA20_POLY1305 1
#define MBEDTLS_PSA_ACCEL_ALG_CMAC 1
#define MBEDTLS_PSA_ACCEL_ALG_ECB_NO_PADDING 1
#define MBEDTLS_PSA_ACCEL_ALG_ECDH 1
#define MBEDTLS_PSA_ACCEL_ALG_GCM 1
#define MBEDTLS_PSA_ACCEL_ALG_HKDF 1
#define MBEDTLS_PSA_ACCEL_ALG_HKDF_EXTRACT 1
#define MBEDTLS_PSA_ACCEL_ALG_HKDF_EXPAND 1
#define MBEDTLS_PSA_ACCEL_ALG_HMAC 1
#define MBEDTLS_PSA_ACCEL_ALG_RSA_OAEP 1
#define MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_CRYPT 1
#define MBEDTLS_PSA_ACCEL_ALG_STREAM_CIPHER 1
#define MBEDTLS_PSA_ACCEL_ALG_TLS12_ECJPAKE_TO_PMS 1
#define MBEDTLS_PSA_ACCEL_ALG_TLS12_PRF 1
#define MBEDTLS_PSA_ACCEL_ALG_TLS12_PSK_TO_MS 1

#define MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_256 1
#define MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_384 1
#define MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_512 1
#define MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_255 1
#define MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_448 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_K1_192 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_K1_224 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_K1_256 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_192 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_224 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_384 1
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_521 1

#define MBEDTLS_PSA_ACCEL_KEY_TYPE_AES 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DERIVE 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_HMAC 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_CHACHA20 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DES 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_PUBLIC_KEY 1
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RAW_DATA 1



#endif /* OBERON_CONFIG_H */
