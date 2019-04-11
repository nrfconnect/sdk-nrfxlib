/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_aes_key AES key sizes
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definition of AES key sizes.
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 */

#ifndef OCRYPTO_AES_KEY_H
#define OCRYPTO_AES_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ocrypto_aes128_KEY_BYTES ((size_t) 16)  //!< AES-128 key size in bytes
#define ocrypto_aes192_KEY_BYTES ((size_t) 24)  //!< AES-192 key size in bytes
#define ocrypto_aes256_KEY_BYTES ((size_t) 32)  //!< AES-256 key size in bytes

#ifdef __cplusplus
}
#endif

#endif

/** @} */
