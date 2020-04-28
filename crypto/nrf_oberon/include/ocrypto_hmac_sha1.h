/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_hmac_1 HMAC APIs using SHA-1
 * @ingroup nrf_oberon_hmac
 * @{
 * @brief Type declarations and APIs for the HMAC-SHA1 algorithm.
 *
 * HMAC-SHA1 is an algorithm for message authentication using the
 * cryptographic hash function SHA512 and a reusable secret key. Users in
 * possession of the key can verify the integrity and authenticity of the
 * message.
 *
 * @see [RFC 2104 - HMAC: Keyed-Hashing for Message Authentication](http://tools.ietf.org/html/rfc2104)
 */

#ifndef OCRYPTO_HMAC_SHA1_H
#define OCRYPTO_HMAC_SHA1_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maximum key length.
 */
#define ocrypto_hmac_sha1_KEY_BYTES_MAX (64)

/**
 * Length of the authenticator.
 */
#define ocrypto_hmac_sha1_BYTES (20)


/**
 * HMAC-SHA1 algorithm.
 *
 * The input message @p in is authenticated using the key @p k. The computed
 * authenticator is put into @p r. To verify the authenticator, the recipient
 * needs to recompute the HMAC authenticator and can then compare it with the
 * received authenticator.
 *
 * @param[out] r       HMAC output.
 * @param      key     HMAC key.
 * @param      key_len Length of @p key. 0 <= @p key_len <= @c ocrypto_hmac_sha1_KEY_BYTES_MAX.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 */
void ocrypto_hmac_sha1(
    uint8_t r[ocrypto_hmac_sha1_BYTES],
    const uint8_t* key, size_t key_len,
    const uint8_t* in, size_t in_len);

/**
 * HMAC-SHA1 algorithm with AAD.
 *
 * @param[out] r       HMAC output
 * @param      key     HMAC key.
 * @param      key_len Length of @p key. 0 <= @p key_len <= @c ocrypto_hmac_sha1_KEY_BYTES_MAX.
 * @param      in      Input data.
 * @param      in_len  Length of @p in.
 * @param      aad     Additional authentication data. May be NULL.
 * @param      aad_len Length of @p aad.
 */
void ocrypto_hmac_sha1_aad(
    uint8_t r[20],
    const uint8_t* key, size_t key_len,
    const uint8_t* in, size_t in_len,
    const uint8_t* aad, size_t aad_len);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_HMAC_SHA1_H */

/** @} */
