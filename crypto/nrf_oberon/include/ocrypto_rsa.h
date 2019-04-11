/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_rsa RSA - Rivest-Shamir-Adleman algorithm
 * @ingroup nrf_oberon
 * @{
 * @brief RSA is a number theoretic public-key encryption and signature algorithm.
 * @}
 * @defgroup nrf_oberon_rsa_api RSA APIs.
 * @ingroup nrf_oberon_rsa
 * @{
 * @brief APIs to for RSA encryption/decryption and sign/verify using PKCS1 v1.5, OEAP and PSS
 *
 * These functions support RSA encryption and signatures with 1024 and 2048 bit
 * modulo and PKCS1 V1.5 padding.
 */

#ifndef OCRYPTO_RSA_H
#define OCRYPTO_RSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "ocrypto_rsa_key.h"


/**@name 1024 Bit RSA Functions.
 * 
 * This group of functions is used for 1024 bit RSA.
 */
/**@{*/
/**
 * 1024 bit RSA PKCS1 V1.5 encryption.
 *
 * The message @p m is encrypted to a ciphertext returned in @p c.
 *
 * @param[out] c         The generated 128 byte ciphertext.
 * @param      m         The message to be encrypted.
 * @param      mlen      Length of @p m. 0 <= mlen <= 117.
 * @param      seed      The random seed to be used for the padding.
 * @param      slen      Length of @p seed. @p slen >= 125 - @p mlen
 * @param      pk        A valid 1024 bit RSA public key.
 *
 * @returns -1 If the message is too long (mlen > 117).
 * @returns -2 If the seed is too short (slen < 125 - mlen).
 * @returns 0  Otherwise.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa1024_init_pub_key.
 * @remark The @p seed should consist of non-zero random bytes.
 * @remark @p c and @p m can point to the same address.
 */
int ocrypto_rsa1024_pkcs1_v15_encrypt(
    uint8_t c[128],
    const uint8_t *m, size_t mlen,
    const uint8_t *seed, size_t slen,
    const ocrypto_rsa1024_pub_key *pk);

/**
 * 1024 bit RSA PKCS1 V1.5 decryption.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 128 byte ciphertext to decrypt.
 * @param      k       A valid 1024 bit RSA secret key.
 *
 * @returns -1 If decryption failed.
 * @returns -2 If the output buffer is too short (mlen < length of message).
 * @returns  n If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa1024_pkcs1_v15_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[128],
    const ocrypto_rsa1024_key *k);

/**
 * 1024 bit RSA PKCS1 V1.5 decryption with CRT acceleration.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 128 byte ciphertext to decrypt.
 * @param      k       A valid 1024 bit RSA secret key with CRT coefficients.
 *
 * @returns -1  If decryption failed.
 * @returns -2  If the output buffer is too short (mlen < length of message).
 * @returns  n  If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_crt_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa1024_pkcs1_v15_crt_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[128],
    const ocrypto_rsa1024_crt_key *k);

/**
 * 1024 bit RSA OAEP SHA256 encryption.
 *
 * The message @p m is encrypted to a ciphertext returned in @p c.
 *
 * @param[out] c         The generated 128 byte ciphertext.
 * @param      m         The message to be encrypted.
 * @param      mlen      Length of @p m. 0 <= mlen <= 62.
 * @param      label     The label associated with the message.
 * @param      llen      Length of @p label. May be 0.
 * @param      seed      32 bytes random seed.
 * @param      pk        A valid 1024 bit RSA public key.
 *
 * @returns -1 If the message is too long (mlen > 62).
 * @returns 0  Otherwise.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa1024_init_pub_key.
 * @remark @p c and @p m can point to the same address.
 */
int ocrypto_rsa1024_oaep_sha256_encrypt(
    uint8_t c[128],
    const uint8_t *m, size_t mlen,
    const uint8_t *label, size_t llen,
    const uint8_t seed[32],
    const ocrypto_rsa1024_pub_key *pk);

/**
 * 1024 bit RSA OAEP SHA256 decryption.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 128 byte ciphertext to decrypt.
 * @param      label   The label associated with the message.
 * @param      llen    Length of @p label. May be 0.
 * @param      k       A valid 1024 bit RSA secret key.
 *
 * @returns -1 If decryption failed.
 * @returns -2 If the output buffer is too short (mlen < length of message).
 * @returns  n If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa1024_oaep_sha256_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[128],
    const uint8_t *label, size_t llen,
    const ocrypto_rsa1024_key *k);

/**
 * 1024 bit RSA OAEP SHA256 decryption with CRT acceleration.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 128 byte ciphertext to decrypt.
 * @param      label   The label associated with the message.
 * @param      llen    Length of @p label. May be 0.
 * @param      k       A valid 1024 bit RSA secret key with CRT coefficients.
 *
 * @returns -1  If decryption failed.
 * @returns -2  If the output buffer is too short (mlen < length of message).
 * @returns  n  If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_crt_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa1024_oaep_sha256_crt_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[128],
    const uint8_t *label, size_t llen,
    const ocrypto_rsa1024_crt_key *k);

/**
 * 1024 bit RSA PKCS1 V1.5 SHA-256 sign.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 128 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      k       A valid 1024 bit RSA secret key.
 *
 * @returns 0
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa1024_pkcs1_v15_sha256_sign(
    uint8_t s[128],
    const uint8_t *m, size_t mlen,
    const ocrypto_rsa1024_key *k);

/**
 * 1024 bit RSA PKCS1 V1.5 SHA-256 sign with CRT acceleration.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 128 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      k       A valid 1024 bit RSA secret key with CRT coefficients.
 *
 * @returns 0
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_crt_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa1024_pkcs1_v15_sha256_crt_sign(
    uint8_t s[128],
    const uint8_t *m, size_t mlen,
    const ocrypto_rsa1024_crt_key *k);

/**
 * 1024 bit RSA PKCS1 V1.5 SHA-256 signature verify.
 *
 * The signature @p s is verified for a correct signature of message @p m.
 *
 * @param s      The 128 byte signature.
 * @param m      The signed message.
 * @param mlen   Length of @p m.
 * @param pk     A valid 1024 bit RSA public key.
 *
 * @returns 0  If the signature is successfully verified.
 * @returns -1 If verification failed.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa1024_init_pub_key.
 */
int ocrypto_rsa1024_pkcs1_v15_sha256_verify(
    const uint8_t s[128],
    const uint8_t *m, size_t mlen,
    const ocrypto_rsa1024_pub_key *pk);

/**
 * 1024 bit RSA PSS SHA-256 sign.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 128 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      salt    The salt to be used.
 * @param      slen    Length of @p salt.
 * @param      k       A valid 1024 bit RSA secret key.
 *
 * @returns -2  If the salt is too long.
 * @returns 0   Otherwise
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa1024_pss_sha256_sign(
    uint8_t s[128],
    const uint8_t *m, size_t mlen,
    const uint8_t *salt, size_t slen,
    const ocrypto_rsa1024_key *k);

/**
 * 1024 bit RSA PSS SHA-256 sign with CRT acceleration.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 128 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      salt    The salt to be used.
 * @param      slen    Length of @p salt.
 * @param      k       A valid 1024 bit RSA secret key with CRT coefficients.
 *
 * @returns -2  If the salt is too long.
 * @returns 0   Otherwise
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa1024_init_crt_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa1024_pss_sha256_crt_sign(
    uint8_t s[128],
    const uint8_t *m, size_t mlen,
    const uint8_t *salt, size_t slen,
    const ocrypto_rsa1024_crt_key *k);

/**
 * 1024 bit RSA PSS SHA-256 signature verify.
 *
 * The signature @p s is verified for a correct signature of message @p m.
 *
 * @param s      The 128 byte signature.
 * @param m      The signed message.
 * @param mlen   Length of @p m.
 * @param slen   The length of the salt.
 * @param pk     A valid 1024 bit RSA public key.
 *
 * @returns 0   If the signature is successfully verified.
 * @returns -1  If verification failed.
 * @returns -2  If the salt is too long.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa1024_init_pub_key.
 */
int ocrypto_rsa1024_pss_sha256_verify(
    const uint8_t s[128],
    const uint8_t *m, size_t mlen,
    size_t slen, // salt length
    const ocrypto_rsa1024_pub_key *pk);
/**@}*/


/**@name 2048 Bit RSA Functions.
 * 
 * This group of functions is used for 2048 bit RSA.
 */
/**@{*/
/**
 * 2048 bit RSA PKCS1 V1.5 encryption.
 *
 * The message @p m is encrypted to a ciphertext returned in @p c.
 *
 * @param[out] c         The generated 256 byte ciphertext.
 * @param      m         The message to be encrypted.
 * @param      mlen      Length of @p m. 0 <= @p mlen <= 245.
 * @param      seed      The random seed to be used for the padding.
 * @param      slen      Length of @p seed. @p slen >= 253 - @p mlen.
 * @param      pk        A valid 2048 bit RSA public key.
 *
 * @returns -1 If the message is too long (mlen > 245).
 * @returns -2 If the seed is too short (slen < 253 - mlen).
 * @returns 0  Otherwise.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa2048_init_pub_key.
 * @remark The @p seed should consist of non-zero random bytes.
 * @remark @p c and @p m can point to the same address.
 */
int ocrypto_rsa2048_pkcs1_v15_encrypt(
    uint8_t c[256],
    const uint8_t *m, size_t mlen,
    const uint8_t *seed, size_t slen,
    const ocrypto_rsa2048_pub_key *pk);

/**
 * 2048 bit RSA PKCS1 V1.5 decryption.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 256 byte ciphertext to decrypt.
 * @param      k       A valid 2048 bit RSA secret key.

 * @returns -1 If decryption failed.
 * @returns -2 If the output buffer is too short (mlen < length of message).
 * @returns n  If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa2048_pkcs1_v15_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[256],
    const ocrypto_rsa2048_key *k);

/**
 * 2048 bit RSA PKCS1 V1.5 decryption with CRT acceleration.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 256 byte ciphertext to decrypt.
 * @param      k       A valid 2048 bit RSA secret key with CRT coefficients.

 * @returns -1  If decryption failed.
 * @returns -2  If the output buffer is too short (mlen < length of message).
 * @returns  n  If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_crt_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa2048_pkcs1_v15_crt_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[256],
    const ocrypto_rsa2048_crt_key *k);

/**
 * 2048 bit RSA OAEP SHA256 encryption.
 *
 * The message @p m is encrypted to a ciphertext returned in @p c.
 *
 * @param[out] c         The generated 256 byte ciphertext.
 * @param      m         The message to be encrypted.
 * @param      mlen      Length of @p m. 0 <= mlen <= 190.
 * @param      label     The label associated with the message.
 * @param      llen      Length of @p label. May be 0.
 * @param      seed      32 bytes random seed.
 * @param      pk        A valid 2048 bit RSA public key.
 *
 * @returns -1 If the message is too long (mlen > 190).
 * @returns 0  Otherwise.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa2048_init_pub_key.
 * @remark @p c and @p m can point to the same address.
 */
int ocrypto_rsa2048_oaep_sha256_encrypt(
    uint8_t c[256],
    const uint8_t *m, size_t mlen,
    const uint8_t *label, size_t llen,
    const uint8_t seed[32],
    const ocrypto_rsa2048_pub_key *pk);

/**
 * 2048 bit RSA OAEP SHA256 decryption.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 256 byte ciphertext to decrypt.
 * @param      label   The label associated with the message.
 * @param      llen    Length of @p label. May be 0.
 * @param      k       A valid 2048 bit RSA secret key.
 *
 * @returns -1 If decryption failed.
 * @returns -2 If the output buffer is too short (mlen < length of message).
 * @returns  n If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa2048_oaep_sha256_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[256],
    const uint8_t *label, size_t llen,
    const ocrypto_rsa2048_key *k);

/**
 * 2048 bit RSA OAEP SHA256 decryption with CRT acceleration.
 *
 * The ciphertext @p c is decrypted to the message returned in @p m.
 *
 * @param[out] m       The decrypted message. The buffer must be long enough to hold the message.
 * @param      mlen    Length of @p m.
 * @param      c       The 256 byte ciphertext to decrypt.
 * @param      label   The label associated with the message.
 * @param      llen    Length of @p label. May be 0.
 * @param      k       A valid 2048 bit RSA secret key with CRT coefficients.
 *
 * @returns -1  If decryption failed.
 * @returns -2  If the output buffer is too short (mlen < length of message).
 * @returns  n  If a message of length n was successfully decrypted. 
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_crt_key.
 * @remark @p m and @p c can point to the same address.
 */
int ocrypto_rsa2048_oaep_sha256_crt_decrypt(
    uint8_t *m, size_t mlen,
    const uint8_t c[256],
    const uint8_t *label, size_t llen,
    const ocrypto_rsa2048_crt_key *k);

/**
 * 2048 bit RSA PKCS1 V1.5 SHA-256 sign.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 256 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      k       A valid 2048 bit RSA secret key.

 * @returns 0
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa2048_pkcs1_v15_sha256_sign(
    uint8_t s[256],
    const uint8_t *m, size_t mlen,
    const ocrypto_rsa2048_key *k);

/**
 * 2048 bit RSA PKCS1 V1.5 SHA-256 sign with CRT acceleration.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 256 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      k       A valid 2048 bit RSA secret key with CRT coefficients.

 * @returns 0
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_crt_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa2048_pkcs1_v15_sha256_crt_sign(
    uint8_t s[256],
    const uint8_t *m, size_t mlen,
    const ocrypto_rsa2048_crt_key *k);

/**
 * 2048 bit RSA PKCS1 V1.5 SHA-256 signature verify.
 *
 * The signature @p s is verified for a correct signature of message @p m.
 *
 * @param s      The 256 byte signature.
 * @param m      The signed message.
 * @param mlen   Length of @p m.
 * @param pk     A valid 2048 bit RSA public key.
 *
 * @returns 0  If the signature is successfully verified.
 * @returns -1 If verification failed.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa2048_init_pub_key.
 */
int ocrypto_rsa2048_pkcs1_v15_sha256_verify(
    const uint8_t s[256],
    const uint8_t *m, size_t mlen,
    const ocrypto_rsa2048_pub_key *pk);

/**
 * 2048 bit RSA PSS SHA-256 sign.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 256 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      salt    The salt to be used.
 * @param      slen    Length of @p salt.
 * @param      k       A valid 2048 bit RSA secret key.
 *
 * @returns -2  If the salt is too long.
 * @returns 0   Otherwise
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa2048_pss_sha256_sign(
    uint8_t s[256],
    const uint8_t *m, size_t mlen,
    const uint8_t *salt, size_t slen,
    const ocrypto_rsa2048_key *k);

/**
 * 2048 bit RSA PSS SHA-256 sign with CRT acceleration.
 *
 * The message @p m is signed and the signature returned in @p s.
 *
 * @param[out] s       The generated 256 byte signature.
 * @param      m       The message to be signed.
 * @param      mlen    Length of @p m.
 * @param      salt    The salt to be used.
 * @param      slen    Length of @p salt.
 * @param      k       A valid 2048 bit RSA secret key with CRT coefficients.
 *
 * @returns -2  If the salt is too long.
 * @returns 0   Otherwise
 *
 * @remark The key @p k should be initialized with @c ocrypto_rsa2048_init_crt_key.
 * @remark @p s and @p m can point to the same address.
 */
int ocrypto_rsa2048_pss_sha256_crt_sign(
    uint8_t s[256],
    const uint8_t *m, size_t mlen,
    const uint8_t *salt, size_t slen,
    const ocrypto_rsa2048_crt_key *k);

/**
 * 2048 bit RSA PSS SHA-256 signature verify.
 *
 * The signature @p s is verified for a correct signature of message @p m.
 *
 * @param s      The 256 byte signature.
 * @param m      The signed message.
 * @param mlen   Length of @p m.
 * @param slen   The length of the salt.
 * @param pk     A valid 2048 bit RSA public key.
 *
 * @returns 0   If the signature is successfully verified.
 * @returns -1  If verification failed.
 * @returns -2  If the salt is too long.
 *
 * @remark The key @p pk should be initialized with @c ocrypto_rsa2048_init_pub_key.
 */
int ocrypto_rsa2048_pss_sha256_verify(
    const uint8_t s[256],
    const uint8_t *m, size_t mlen,
    size_t slen, // salt length
    const ocrypto_rsa2048_pub_key *pk);
/**@}*/


#ifdef __cplusplus
}
#endif

#endif

/** @} */
