/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
/**@file
 * @defgroup nrf_oberon_ecdsa ECDSA APIs
 * @ingroup nrf_oberon
 * @{
 * @brief Type declarations and APIs to do Elliptic Curve Digital Signature Algorith using the
 * NIST secp256r1 curve.
 */
#ifndef OCRYPTO_ECDSA_P256_H
#define OCRYPTO_ECDSA_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * ECDSA P-256 signature key pair generation.
 *
 * @param[out]  pk  Generated public key.
 * @param[out]  sk  Private key.
 *
 * @retval 0 If @p sk is a legal secret key.
 */
int ocrypto_ecdsa_p256_public_key(
    uint8_t pk[64],
    const uint8_t sk[32]);


/**
 * ECDSA P-256 signature generation.
 *
 * @param[out]  sig     Generated signature.
 * @param       m       Input message.
 * @param       mlen    Length of the message.
 * @param       sk      Secret key.
 * @param       ek      Session key.
 *
 * @retval 0 If @p ek is a valid session key.
 */
int ocrypto_ecdsa_p256_sign(
    uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t sk[32],
    const uint8_t ek[32]);


/**
 * ECDSA P-256 signature generation from hash.
 *
 * @param[out]  sig     Generated signature.
 * @param       hash    SHA-256 hash of the input message.
 * @param       sk      Secret key.
 * @param       ek      Session key.
 *
 * @retval 0 If @p ek is a valid session key.
 */
int ocrypto_ecdsa_p256_sign_hash(uint8_t sig[64],
                                 const uint8_t hash[32],
                                 const uint8_t sk[32],
                                 const uint8_t ek[32]);


/**
 * ECDSA P-256 signature verification.
 *
 * @param       sig     Input signature.
 * @param       m       Input message.
 * @param       mlen    Input length.
 * @param       pk      Public key.
 *
 * @retval 0  If signature is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_verify(
    const uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t pk[64]);


/**
 * ECDSA P-256 signature verification from hash.
 *
 * @param        sig     Input signature.
 * @param        hash    SHA-256 hash of the input message.
 * @param        pk      Public key.
 *
 * @retval 0  If signature is valid.
*  @retval -1 Otherwise.
 */
int ocrypto_ecdsa_p256_verify_hash(const uint8_t sig[64],
                                   const uint8_t hash[32],
                                   const uint8_t pk[64]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
