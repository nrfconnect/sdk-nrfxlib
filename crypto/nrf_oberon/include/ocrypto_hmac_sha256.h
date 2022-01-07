/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup ocrypto_hmac HMAC - Hash-based Message Authentication Code
 * @ingroup ocrypto
 * @{
 * @brief HMAC is a hash-based Message Authentication Code utilizing a secure hash function.
 * @}
 *
 * @defgroup ocrypto_hmac_sha256 HMAC APIs using SHA-256
 * @ingroup ocrypto_hmac
 * @{
 * @brief Type declarations and APIs for the HMAC-SHA256 algorithm.
 *
 * HMAC-SHA256 is an algorithm for message authentication using the
 * cryptographic hash function SHA256 and a reusable secret key. Users in
 * possession of the key can verify the integrity and authenticity of the
 * message.
 *
 * @see [RFC 2104 - HMAC: Keyed-Hashing for Message Authentication](http://tools.ietf.org/html/rfc2104)
 */

#ifndef OCRYPTO_HMAC_SHA256_H
#define OCRYPTO_HMAC_SHA256_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Length of the authenticator.
 */
#define ocrypto_hmac_sha256_BYTES (32)


/**
 * HMAC-SHA256 algorithm.
 *
 * The input message @p in is authenticated using the key @p key. The computed
 * authenticator is put into @p r. To verify the authenticator, the recipient
 * needs to recompute the HMAC authenticator and can then compare it with the
 * received authenticator.
 *
 * @param[out] r       HMAC output.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 */
void ocrypto_hmac_sha256(
    uint8_t r[ocrypto_hmac_sha256_BYTES],
    const uint8_t* key, size_t key_len,
    const uint8_t* in, size_t in_len);

/**
 * HMAC-SHA256 algorithm with AAD.
 *
 * @param[out] r       HMAC output
 * @param      key     HMAC key.
 * @param      key_len Length of @p key.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 * @param      aad     Additional authentication data. May be NULL.
 * @param      aad_len Length of @p aad.
 */
void ocrypto_hmac_sha256_aad(
    uint8_t r[ocrypto_hmac_sha256_BYTES],
    const uint8_t* key, size_t key_len,
    const uint8_t* in, size_t in_len,
    const uint8_t* aad, size_t aad_len);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
