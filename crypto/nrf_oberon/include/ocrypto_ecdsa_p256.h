/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_ecdsa ECDSA - Elliptic Curve Digital Signature Algorithm APIs
 * @ingroup ocrypto
 * @{
 * @brief ECDSA (Elliptic Curve Digital Signature Algorith) Elliptic Curve Digital Signature Algorithm.
 * @}
 *
 * @defgroup ocrypto_ecdsa_p256 ECDSA secp256r1 low-level APIs
 * @ingroup ocrypto_ecdsa
 * @{
 * @brief Type declarations and APIs to do Elliptic Curve Digital Signature Algorithm using the
 *        NIST secp256r1 curve.
 *
 * ECDSA P-256 is a specific implementation of a digital signature scheme.
 */

#ifndef OCRYPTO_ECDSA_P256_H
#define OCRYPTO_ECDSA_P256_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * ECDSA P-256 public key generation.
 *
 * Given a secret key @p sk the corresponding public key is computed and put
 * into @p pk.
 *
 * @param[out] pk Generated public key.
 * @param      sk Secret key. Must be pre-filled with random data.
 *
 * @retval 0  If @p sk is a valid secret key.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_public_key(
    uint8_t pk[64],
    const uint8_t sk[32]);

/**
 * ECDSA P-256 signature generation.
 *
 * The message @p m is signed using the secret key @p sk and the ephemeral
 * session key @p ek. The signature is put into @p sig.
 *
 * @param[out] sig  Generated signature.
 * @param      m    Input message.
 * @param      mlen Length of @p m.
 * @param      sk   Secret key.
 * @param      ek   Session key.
 *
 * @retval 0  If @p ek is a valid session key.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_sign(
    uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t sk[32],
    const uint8_t ek[32]);

/**
 * ECDSA P-256 signature generation from SHA256 hash.
 *
 * The message hash @p hash is signed using the secret key @p sk and the ephemeral
 * session key @p ek. The signature is put into @p sig.
 *
 * @param[out] sig  Generated signature.
 * @param      hash Input hash.
 * @param      sk   Secret key.
 * @param      ek   Session key.
 *
 * @retval 0  If @p ek is a valid session key.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_sign_hash(
    uint8_t sig[64],
    const uint8_t hash[32],
    const uint8_t sk[32],
    const uint8_t ek[32]);

/**
 * ECDSA P-256 deterministic signature generation.
 *
 * The message @p m is signed using the secret key @p sk and a session key calculated
 * from message hash and key. The signature is put into @p sig.
 *
 * @param[out] sig  Generated signature.
 * @param      m    Input message.
 * @param      mlen Length of @p m.
 * @param      sk   Secret key.
 */
void ocrypto_ecdsa_p256_det_sign(
    uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t sk[32]);

/**
 * ECDSA P-256 deterministic signature generation from SHA256 hash.
 *
 * The message hash @p hash is signed using the secret key @p sk and a session key calculated
 * from hash and key. The signature is put into @p sig.
 *
 * @param[out] sig  Generated signature.
 * @param      hash Input hash.
 * @param      sk   Secret key.
 */
void ocrypto_ecdsa_p256_det_sign_hash(
    uint8_t sig[64],
    const uint8_t hash[32],
    const uint8_t sk[32]);

/**
 * ECDSA P-256 signature verification.
 *
 * The signature @p sig of the input message @p m is verified using the signer's
 * public key @p pk.
 *
 * @param sig  Input signature.
 * @param m    Input message.
 * @param mlen Length of @p m.
 * @param pk   Signer's public key.
 *
 * @retval 0  If the signature is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_verify(
    const uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t pk[64]);

/**
 * ECDSA P-256 signature verification from SHA256 hash.
 *
 * The signature @p sig of the message hash @p hash is verified using the signer's
 * public key @p pk.
 *
 * @param sig  Input signature.
 * @param hash Input hash.
 * @param pk   Signer's public key.
 *
 * @retval 0  If the signature is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_verify_hash(
    const uint8_t sig[64],
    const uint8_t hash[32],
    const uint8_t pk[64]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
