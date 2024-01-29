/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_aes AES - Advanced Encryption Standard APIs
 * @ingroup ocrypto
 * @{
 * @brief AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 * @}
 *
 * @defgroup ocrypto_aes_key AES key sizes
 * @ingroup ocrypto_aes
 * @{
 * @brief Type definition of AES key sizes.
 *
 * AES key sizes in bytes.
 */

#ifndef OCRYPTO_AES_KEY_H
#define OCRYPTO_AES_KEY_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Length of AES-128 key.
 */
#define ocrypto_aes128_KEY_BYTES ((size_t) 16)

/**
 * Length of AES-192 key.
 */
#define ocrypto_aes192_KEY_BYTES ((size_t) 24)

/**
 * Length of AES-256 key.
 */
#define ocrypto_aes256_KEY_BYTES ((size_t) 32)

#ifdef __cplusplus
}
#endif

#endif

/** @} */
