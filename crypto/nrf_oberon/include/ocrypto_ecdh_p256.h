/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef OCRYPTO_ECDH_P256_H
#define OCRYPTO_ECDH_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


// ecdh P-256 public key r = n * p
// r[64]: the resulting public key
// s[32]: the secret key
// r may be same as s
// returns 0 if s is a legal secret key
int ocrypto_ecdh_p256_public_key(uint8_t r[64], const uint8_t s[32]);

// ecdh P-256 common secret
// r[32]: the resulting secret
// s[32]: the secret key
// p[64]: the public key
// r may be same as s or p
// returns 0 if s is a legal secret key and p is a legal public key
int ocrypto_ecdh_p256_common_secret(uint8_t r[32], const uint8_t s[32], const uint8_t p[64]);


#ifdef __cplusplus
}
#endif

#endif
