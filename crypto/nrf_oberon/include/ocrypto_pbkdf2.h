/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup ocrypto_pbkdf2 PBKDF2 APIs
 * @ingroup ocrypto
 * @{
 * @brief Type declaration and APIs for PBKDF2 with HMAC-SHA1.
 *
 * PBKDF2 with HMAC-SHA1 is a password-based key derivation function defined in RFC2898.

 */

#ifndef OCRYPTO_PBKDF2_H
#define OCRYPTO_PBKDF2_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Computes the PBKDF2-HMAC-SHA1 key from password, salt, and iteration count.
 *
 * @param[out] key                  PBKDF2 key to generate.
 * @param      key_len              Length of key.
 * @param      password             Password to use.
 * @param      password_len         Length of password. password_len <= 64.
 * @param      salt                 Salt to use.
 * @param      salt_len             Length of salt.
 * @param      count                Iteration count.
 */
void ocrypto_pbkdf2_hmac_sha1(
    uint8_t *key, size_t key_len,
    const uint8_t *password, size_t password_len,
    const uint8_t *salt, size_t salt_len,
    uint32_t count);

/**
 * Computes the PBKDF2-HMAC-SHA256 key from password, salt, and iteration count.
 *
 * @param[out] key                  PBKDF2 key to generate.
 * @param      key_len              Length of key.
 * @param      password             Password to use.
 * @param      password_len         Length of password. password_len <= 64.
 * @param      salt                 Salt to use.
 * @param      salt_len             Length of salt.
 * @param      count                Iteration count.
 */
void ocrypto_pbkdf2_hmac_sha256(
    uint8_t *key, size_t key_len,
    const uint8_t *password, size_t password_len,
    const uint8_t *salt, size_t salt_len,
    uint32_t count);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_PBKDF2_H */

/** @} */
