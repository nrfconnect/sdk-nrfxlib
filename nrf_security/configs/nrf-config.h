/*
 * Copyright (c) 2021 Nordic Semiconductor
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 *
 */

#ifndef MBEDTLS_CONFIG_PSA_H
#define MBEDTLS_CONFIG_PSA_H

#if defined(MBEDTLS_USER_CONFIG_FILE)
#include MBEDTLS_USER_CONFIG_FILE
#else
#error "MBEDTLS_USER_CONFIG_FILE expected to be set"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************/
/* Require built-in implementations based on PSA requirements */
/****************************************************************/

#if defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_DETERMINISTIC_ECDSA)
#define MBEDTLS_ECDSA_DETERMINISTIC
#define MBEDTLS_ECDSA_C
#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_MD_C
#endif
#endif

#if defined(PSA_WANT_ALG_ECDH)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_ECDH)
#define MBEDTLS_ECDH_C
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#endif
#endif

#if defined(PSA_WANT_ALG_ECDSA)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_ECDSA)
#define MBEDTLS_ECDSA_C
#endif
#endif

#if defined(PSA_WANT_ALG_MD5)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_MD5)
#define MBEDTLS_MD5_C
#endif
#endif

#if defined(PSA_WANT_ALG_RIPEMD160)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_RIPEMD160)
#define MBEDTLS_RIPEMD160_C
#endif
#endif

#if defined(PSA_WANT_ALG_RSA_OAEP)
//#if defined(MBEDTLS_PSA_BUILTIN_ALG_RSA_OAEP)
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V21
#define MBEDTLS_MD_C
//#endif
#endif

#if defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT)
//#if defined(MBEDTLS_PSA_BUILTIN_ALG_RSA_PKCS1V15_CRYPT)
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V15
//#endif
#endif

#if defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN)
//#if defined(MBEDTLS_PSA_BUILTIN_ALG_RSA_PKCS1V15_SIGN)
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V15
#define MBEDTLS_MD_C
//#endif
#endif

#if defined(PSA_WANT_ALG_RSA_PSS)
//#if defined(MBEDTLS_PSA_BUILTIN_ALG_RSA_PSS)
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PKCS1_V21
#define MBEDTLS_MD_C
//#endif
#endif

#if defined(PSA_WANT_ALG_SHA_1)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_SHA_1)
#define MBEDTLS_SHA1_C
#endif
#endif

#if defined(PSA_WANT_ALG_SHA_224)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_SHA_224)
#define MBEDTLS_SHA224_C
#endif
#endif

#if defined(PSA_WANT_ALG_SHA_256)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_SHA_256)
#define MBEDTLS_SHA256_C
#endif
#endif

#if defined(PSA_WANT_ALG_SHA_384)
//#if defined(MBEDTLS_PSA_BUILTIN_ALG_SHA_384)
#define MBEDTLS_SHA384_C
#define MBEDTLS_SHA512_C
//#endif
#endif

#if defined(PSA_WANT_ALG_SHA_512)
//#if defined(MBEDTLS_PSA_BUILTIN_ALG_SHA_512)
#define MBEDTLS_SHA512_C
//#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR)
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY)
#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_ECC_PUBLIC_KEY)
#define MBEDTLS_ECP_C
#define MBEDTLS_BIGNUM_C
#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR)
//#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_KEY_PAIR)
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_GENPRIME
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_PK_C
//#endif
#endif

#if defined(PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY)
//#if defined(MBEDTLS_PSA_BUILTIN_KEY_TYPE_RSA_PUBLIC_KEY)
#define MBEDTLS_RSA_C
#define MBEDTLS_BIGNUM_C
#define MBEDTLS_OID_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_PK_C
//#endif
#endif


// Not using PSA_HAS_CIPHER_SUPPORT
#if defined(PSA_WANT_KEY_TYPE_AES)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_AES)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_AES 1
#define MBEDTLS_AES_C
#endif
#endif

// Not using PSA_HAS_CIPHER_SUPPORT
#if defined(PSA_WANT_KEY_TYPE_CAMELLIA)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_CAMELLIA)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_CAMELLIA 1
#define MBEDTLS_CAMELLIA_C
#endif
#endif

// Not using PSA_HAS_CIPHER_SUPPORT
#if defined(PSA_WANT_KEY_TYPE_DES)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_DES)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_DES 1
#define MBEDTLS_DES_C
#endif
#endif

// Not using PSA_HAS_CIPHER_SUPPORT
#if defined(PSA_WANT_KEY_TYPE_CHACHA20)
#if !defined(MBEDTLS_PSA_ACCEL_KEY_TYPE_CHACHA20)
#define MBEDTLS_PSA_BUILTIN_KEY_TYPE_CHACHA20 1
#define MBEDTLS_CHACHA20_C
#endif
#endif

#if defined(PSA_WANT_ALG_CBC_MAC)
#if !defined(MBEDTLS_PSA_ACCEL_ALG_CBC_MAC)
#error "CBC-MAC is not yet supported via the PSA API in Mbed TLS."
#endif
#endif

#if defined(PSA_WANT_ALG_CMAC)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_CMAC)
#define MBEDTLS_CMAC_C
#endif
#endif

#if defined(PSA_WANT_ALG_CTR)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_CTR)
#define MBEDTLS_CIPHER_MODE_CTR
#endif
#endif

#if defined(PSA_WANT_ALG_CFB)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_CFB)
#define MBEDTLS_CIPHER_MODE_CFB
#endif
#endif

#if defined(PSA_WANT_ALG_OFB)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_OFB)
#define MBEDTLS_CIPHER_MODE_OFB
#endif
#endif

#if defined(PSA_WANT_ALG_XTS)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_XTS)
#define MBEDTLS_CIPHER_MODE_XTS
#endif
#endif

#if defined(PSA_WANT_ALG_CBC_NO_PADDING)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_CBC_NO_PADDING)
#define MBEDTLS_CIPHER_MODE_CBC
#endif
#endif

#if defined(PSA_WANT_ALG_CBC_PKCS7)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_CBC_PKCS7)
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_PADDING_PKCS7
#endif
#endif

#if defined(PSA_WANT_ALG_CCM)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_CCM)
#define MBEDTLS_CCM_C
#endif
#endif

#if defined(PSA_WANT_ALG_GCM)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_GCM)
#define MBEDTLS_GCM_C
#endif
#endif

#if defined(PSA_WANT_ALG_CHACHA20_POLY1305)
#if defined(PSA_WANT_KEY_TYPE_CHACHA20)
#if defined(MBEDTLS_PSA_BUILTIN_ALG_GCM)
#define MBEDTLS_CHACHAPOLY_C
#endif
#endif
#endif

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_256)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_BRAINPOOL_P_R1_256)
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_384)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_BRAINPOOL_P_R1_384)
#define MBEDTLS_ECP_DP_BP384R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_512)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_BRAINPOOL_P_R1_512)
#define MBEDTLS_ECP_DP_BP512R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_MONTGOMERY_255)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_MONTGOMERY_255)
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_MONTGOMERY_448)
#if defined(MBEDTLS_PSA_BUILTIN_ECC_MONTGOMERY_448)
#error "Curve448 is not yet supported via the PSA API in Mbed TLS."
#define MBEDTLS_ECP_DP_CURVE448_ENABLED
#endif
#endif

#if defined(PSA_WANT_ECC_SECP_R1_192)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_192)
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_R1_224)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_224)
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_R1_256)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_256)
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_R1_384)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_384)
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_R1_521)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_R1_521)
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_K1_192)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_K1_192)
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_K1_224)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_K1_224)
#error "SECP224K1 is buggy via the PSA API in Mbed TLS."
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
//#endif
#endif

#if defined(PSA_WANT_ECC_SECP_K1_256)
//#if defined(MBEDTLS_PSA_BUILTIN_ECC_SECP_K1_256)
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
//#endif
#endif

#if defined(MBEDTLS_PK_PARSE_C)
#define MBEDTLS_ASN1_PARSE_C
#endif

#if defined(MBEDTLS_PK_WRITE_C)
#define MBEDTLS_ASN1_WRITE_C
#endif

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_CONFIG_PSA_H */
