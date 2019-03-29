/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */
#ifndef OCRYPTO_ECDSA_P256_H
#define OCRYPTO_ECDSA_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


// ecdsa P-256 signature key pair generation
// pk[64]: the generated public key
// sk[32]: the private key
// returns 0 if sk is a legal secret key
int ocrypto_ecdsa_p256_public_key(
    uint8_t pk[64],
    const uint8_t sk[32]);

// ecdsa P-256 signature generate
// sig[64]: the generated signature
// m[mlen]: the input message
// sk[32]:  the secret key
// ek[32]:  the session key
// returns 0 if ek is a valid session key
int ocrypto_ecdsa_p256_sign(
    uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t sk[32],
    const uint8_t ek[32]);

// ecdsa P-256 signature generate
// sig[64]: the generated signature
// hash[32]: SHA-256 hash of the input message
// sk[32]:  the secret key
// ek[32]:  the session key
// returns 0 if ek is a valid session key
int ocrypto_ecdsa_p256_sign_hash(uint8_t sig[64],
                                 const uint8_t hash[32],
                                 const uint8_t sk[32],
                                 const uint8_t ek[32]);


// ecdsa P-256 signature verification
// sig[64]: the input signature
// m[mlen]: the input message
// pk[64]:  the public key
// returns 0 if signature is valid, -1 otherwise
int ocrypto_ecdsa_p256_verify(
    const uint8_t sig[64],
    const uint8_t *m, size_t mlen,
    const uint8_t pk[64]);


// ecdsa P-256 signature verification
// sig[64]: the input signature
// hash[32]: SHA-256 hash of the input message
// pk[64]:  the public key
// returns 0 if signature is valid, -1 otherwise
int ocrypto_ecdsa_p256_verify_hash(const uint8_t sig[64],
                                   const uint8_t hash[32],
                                   const uint8_t pk[64]);


#ifdef __cplusplus
}
#endif

#endif
