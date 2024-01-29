/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_pbkdf2 PBKDF2 APIs
 * @ingroup ocrypto
 * @{
 * @brief Type declaration and APIs for PBKDF2 with AES-CMAC-PRF-128.
 *
 * PBKDF2 with HMAC-AES-CMAC-PRF-128 is password-based key derivation function defined
 * in RFC2898 and RFC4615.
 */

#ifndef OCRYPTO_PBKDF2_CMAC_PRF128_H
#define OCRYPTO_PBKDF2_CMAC_PRF128_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Computes the PBKDF2-AES-CMAC-PRF-128 key from password, salt, and iteration count.
 *
 * @param[out] key                  PBKDF2 key to generate.
 * @param      key_len              Length of key.
 * @param      password             Password to use.
 * @param      password_len         Length of password.
 * @param      salt                 Salt to use.
 * @param      salt_len             Length of salt. 0 < salt_len <= 32.
 * @param      count                Iteration count.
 */
void ocrypto_pbkdf2_aes_cmac_prf128(
    uint8_t *key, size_t key_len,
    const uint8_t *password, size_t password_len,
    const uint8_t *salt, size_t salt_len,
    uint32_t count);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
