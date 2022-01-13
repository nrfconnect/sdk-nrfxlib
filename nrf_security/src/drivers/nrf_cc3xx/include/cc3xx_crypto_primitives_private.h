/*
 * Copyright (c) 2021-2022 Nordic Semiconductor
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 *
 */

#ifndef CC3XX_CRYPTO_PRIMITIVES_H
#define CC3XX_CRYPTO_PRIMITIVES_H

#define PSA_DRIVER_CC3XX_HASH_OPERATION_CONTEXT_SIZE_WORDS      (52)
#define PSA_DRIVER_CC3XX_CIPHER_OPERATION_CONTEXT_SIZE_WORDS    (69)
#define PSA_DRIVER_CC3XX_MAC_OPERATION_CONTEXT_SIZE_WORDS       (102)
#define PSA_DRIVER_CC3XX_AEAD_OPERATION_CONTEXT_SIZE_WORDS      (1)

typedef struct cc3xx_hash_operation_s
{
    uint32_t internal[PSA_DRIVER_CC3XX_HASH_OPERATION_CONTEXT_SIZE_WORDS];
} cc3xx_hash_operation_t;

typedef struct cc3xx_cipher_operation_s{
    uint32_t internal[PSA_DRIVER_CC3XX_CIPHER_OPERATION_CONTEXT_SIZE_WORDS];
} cc3xx_cipher_operation_t;

typedef struct cc3xx_mac_operation_2{
    uint32_t internal[PSA_DRIVER_CC3XX_MAC_OPERATION_CONTEXT_SIZE_WORDS];
} cc3xx_mac_operation_t;

typedef struct cc3xx_aead_operation_s{
    uint32_t internal[PSA_DRIVER_CC3XX_AEAD_OPERATION_CONTEXT_SIZE_WORDS];
} cc3xx_aead_operation_t;

#endif /* CC3XX_CRYPTO_PRIMITIVES_H */
