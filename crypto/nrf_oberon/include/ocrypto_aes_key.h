/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128 bit block of data into an encrypted block of same size.
 */

#ifndef OCRYPTO_AES_KEY_H
#define OCRYPTO_AES_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * AES key sizes in bytes.
 */
/**@{*/
#define ocrypto_aes128_KEY_BYTES ((size_t) 16)
#define ocrypto_aes192_KEY_BYTES ((size_t) 24)
#define ocrypto_aes256_KEY_BYTES ((size_t) 32)
/**@}*/

#ifdef __cplusplus
}
#endif

#endif
