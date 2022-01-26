/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**@file
 * @defgroup ocrypto_hkdf HKDF - HMAC-based Key Derivation Function
 * @ingroup ocrypto
 * @{
 * @brief HKDF is a key derivation function based on HMAC Extract-and-Expand
 * @}
 *
 * @defgroup ocrypto_hkdf_sha256 HKDF APIs using SHA-256
 * @ingroup ocrypto_hkdf
 * @{
 * @brief Type declarations and APIs for the HKDF-SHA256 algorithm.
 *
 * HKDF-SHA256 is a key derivation function based on HMAC-SHA256.
 *
 * @see [RFC 5869 - HMAC-based Extract-and-Expand Key Derivation Function (HKDF)](http://tools.ietf.org/html/rfc5869)
 */

#ifndef OCRYPTO_HKDF_SHA256_H
#define OCRYPTO_HKDF_SHA256_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * HKDF-SHA256 algorithm.
 *
 * A new pseudo-random key of length @p r_len is derived from an input key
 * @p key, a salt @p salt and additional information @p info. The new key is put
 * into @p r.
 *
 * @param[out] r        Output key.
 * @param      r_len    Length of @p r.
 * @param      key      Input key.
 * @param      key_len  Length of @p key.
 * @param      salt     Salt.
 * @param      salt_len Length of salt @p salt.
 * @param      info     Additional information.
 * @param      info_len Length of @p info.
 */
void ocrypto_hkdf_sha256(
    uint8_t* r, size_t r_len,
    const uint8_t* key, size_t key_len,
    const uint8_t* salt, size_t salt_len,
    const uint8_t* info, size_t info_len);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
