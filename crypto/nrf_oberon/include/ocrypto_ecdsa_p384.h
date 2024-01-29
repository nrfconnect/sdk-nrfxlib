/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * ECDSA P-384 is a specific implementation of a digital signature scheme.
 *
 * @defgroup ocrypto_ecdsa_p384 ECDSA secp384r1 low-level APIs
 * @ingroup ocrypto_ecdsa
 * @{
 * @brief Type declarations and APIs to do Elliptic Curve Digital Signature Algorithm using the
 *        NIST secp384r1 curve.
 */

#ifndef OCRYPTO_ECDSA_P384_H
#define OCRYPTO_ECDSA_P384_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * ECDSA P-384 public key generation.
 *
 * Given a secret key @p sk the corresponding public key is computed and put
 * into @p pk.
 *
 * @param[out] pk Generated public key.
 * @param      sk Secret key. Must be pre-filled with random data.
 *
 * @retval 0  If @p sk is a valid secret key.
 * @retval -1 Otherwise.
 *
 * @remark To generate a valid key pair use the following code pattern:
 *            @code{.c}
 *            do get_random(sk); while (ocrypto_ecdsa_p384_public_key(pk, sk));
 *            @endcode
 */
int ocrypto_ecdsa_p384_public_key(
    uint8_t pk[96],
    const uint8_t sk[48]);

/**
 * ECDSA P-384 signature generation.
 *
 * The message @p m is signed using the secret key @p sk and the ephemeral
 * session key @p ek. The signature is put into @p sig.
 *
 * @param[out] sig  Generated signature.
 * @param      m    Input message.
 * @param      mlen Length of @p m.
 * @param      sk   Secret key.
 * @param      ek   Session key. Must be pre-filled with random data.
 *
 * @retval 0  If @p ek is a valid session key.
 * @retval -1 Otherwise.
 *
 * @remark To generate a valid signature use the following code pattern:
 *            @code{.c}
 *            do get_random(ek); while (ocrypto_ecdsa_p384_sign(sig, m, mlen, sk, ek));
 *            @endcode
 */
int ocrypto_ecdsa_p384_sign(
    uint8_t sig[96],
    const uint8_t *m, size_t mlen,
    const uint8_t sk[48],
    const uint8_t ek[48]);

/**
 * ECDSA P-384 signature generation from SHA384 hash.
 *
 * The message hash @p hash is signed using the secret key @p sk and the ephemeral
 * session key @p ek. The signature is put into @p sig.
 *
 * @param[out] sig  Generated signature.
 * @param      hash Input hash.
 * @param      sk   Secret key.
 * @param      ek   Session key. Must be pre-filled with random data.
 *
 * @retval 0  If @p ek is a valid session key.
 * @retval -1 Otherwise.
 *
 * @remark To generate a valid signature use the following code pattern:
 *            @code{.c}
 *            do get_random(ek); while (ocrypto_ecdsa_p384_sign_hash(sig, hash, sk, ek));
 *            @endcode
 */
int ocrypto_ecdsa_p384_sign_hash(
    uint8_t sig[96],
    const uint8_t hash[48],
    const uint8_t sk[48],
    const uint8_t ek[48]);

/**
 * ECDSA P-384 signature verification.
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
int ocrypto_ecdsa_p384_verify(
    const uint8_t sig[96],
    const uint8_t *m, size_t mlen,
    const uint8_t pk[96]);

/**
 * ECDSA P-384 signature verification from SHA384 hash.
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
int ocrypto_ecdsa_p384_verify_hash(
    const uint8_t sig[96],
    const uint8_t hash[48],
    const uint8_t pk[96]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
