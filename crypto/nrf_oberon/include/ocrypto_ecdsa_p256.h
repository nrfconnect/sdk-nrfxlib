/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
/**@file
 * @defgroup nrf_oberon_ecdsa ECDSA APIs.
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

/**@brief ecdsa P-256 signature key pair generation
 * @param[out]  pk  The generated public key
 * @param       sk  The Private key
 *
 * @returns 0 if sk is a legal secret key
 */
int ocrypto_ecdsa_p256_public_key(
    uint8_t pk[64],
    const uint8_t sk[32]);


/**@brief ecdsa P-256 signature generate
 *
 * @param[out]  sig     The generated signature
 * @param       m       The input message
 * @param       mlen    Length of the message
 * @param       sk      The secret key
 * @param       ek      The session key
 *
 * @returns 0 if ek is a valid session key
 */
int ocrypto_ecdsa_p256_sign(
    uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t sk[32],
    const uint8_t ek[32]);


/**@brief ecdsa P-256 signature generate from hash
 *
 * @param[out]  sig     The generated signature
 * @param       hash    SHA-256 hash of the input message
 * @param       sk      The secret key
 * @param       ek      The session key
 *
 * @returns 0 if ek is a valid session key
 */
int ocrypto_ecdsa_p256_sign_hash(uint8_t sig[64],
                                 const uint8_t hash[32],
                                 const uint8_t sk[32],
                                 const uint8_t ek[32]);


/**@brief ecdsa P-256 signature verification
 *
 * @param       sig     The input signature
 * @param       m       The input message
 * @param       mlen    The input length
 * @param       pk      The public key
 *
 * @returns 0 if signature is valid, -1 otherwise
 */
int ocrypto_ecdsa_p256_verify(
    const uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t pk[64]);


/**@brief ecdsa P-256 signature verification from hash
 *
 * param        sig     The input signature
 * param        hash    SHA-256 hash of the input message
 * param        pk      The public key
 *
 * returns 0 if signature is valid, -1 otherwise
 */
int ocrypto_ecdsa_p256_verify_hash(const uint8_t sig[64],
                                   const uint8_t hash[32],
                                   const uint8_t pk[64]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
