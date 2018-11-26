/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * HKDF-SHA256 is a key derivation function based on HMAC-SHA256.
 * 
 * @see [RFC 5869 - HMAC-based Extract-and-Expand Key Derivation Function (HKDF)](http://tools.ietf.org/html/rfc5869)
 */
 
#ifndef OCC_HKDF_SHA256_H
#define OCC_HKDF_SHA256_H

#include <stdint.h>
#include <stddef.h>


/**
 * Maximum length of a derived key.
 */
#define occ_hkdf_sha256_LENGTH_MAX (32)

/**
 * Maximum salt length.
 */
#define occ_hkdf_sha256_SALT_LENGTH_MAX (64)


/**
 * HKDF-SHA256 algoritm.
 *
 * A new pseudorandom key of length @p r_len is derived from an input key
 * @p key, a salt @p salt and additional information @p info. The new key is put
 * into @p r.
 *
 * **Example**
 * @include occ_hkdf_sha256.c
 *
 * @param[out] r        Output key.
 * @param      r_len    Length of @p r, 0 < @p r_len <= @c occ_hkdf_sha256_LENGTH_MAX.
 * @param      key      Input key.
 * @param      key_len  Length of @p key.
 * @param      salt     Salt.
 * @param      salt_len Length of salt @p salt. 0 <= @p salt_len <= @c occ_hkdf_sha256_SALT_LENGTH_MAX.
 * @param      info     Additional information.
 * @param      info_len Length of @p info.
 */
void occ_hkdf_sha256(uint8_t* r, size_t r_len,
                     const uint8_t* key, size_t key_len,
                     const uint8_t* salt, size_t salt_len,
                     const uint8_t* info, size_t info_len);

#endif
