/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
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
 * @brief Type declarations and APIs to do Elliptic Curve Digital Signature Algorith using the
 *        NIST secp256r1 curve.
 *
 * ECDSA P-256 is a specific implementation of a digital signature scheme.
 */

#ifndef OCRYPTO_DET_ECDSA_P256_H
#define OCRYPTO_DET_ECDSA_P256_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


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


#ifdef __cplusplus
}
#endif

#endif

/** @} */
