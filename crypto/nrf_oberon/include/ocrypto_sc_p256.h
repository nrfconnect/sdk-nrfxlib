/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef OCRYPTO_SC_P256_H
#define OCRYPTO_SC_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// P-256 scalar modulo group order
/**@cond */
typedef struct {
    uint32_t w[8]; // little endian
} ocrypto_sc_p256;
/**@endcond */

// modulo P-256 number
/**@cond */
typedef struct {
  uint32_t w[8]; // little endian
} ocrypto_mod_p256;
/**@endcond */

// 32 byte value to scalar
// return 0 if x < group order
int ocrypto_sc_p256_from32bytes(ocrypto_sc_p256 *r, const uint8_t x[32]);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_SC_P256_H */
