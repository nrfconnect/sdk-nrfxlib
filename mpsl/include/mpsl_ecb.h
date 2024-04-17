/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MPSL_ECB_H__
#define MPSL_ECB_H__

#include <stdint.h>

#define MPSL_ECB_KEY_LENGTH                (16)                        /**< ECB key length. */
#define MPSL_ECB_CLEARTEXT_LENGTH          (16)                        /**< ECB cleartext length. */
#define MPSL_ECB_CIPHERTEXT_LENGTH         (MPSL_ECB_CLEARTEXT_LENGTH) /**< ECB ciphertext length. */

/**@brief AES ECB data structure */
typedef struct
{
  uint32_t key[MPSL_ECB_KEY_LENGTH/sizeof(uint32_t)]; /**< Encryption key. */
  uint8_t  cleartext[MPSL_ECB_CLEARTEXT_LENGTH];      /**< Cleartext data. */
  uint8_t  ciphertext[MPSL_ECB_CIPHERTEXT_LENGTH];    /**< Ciphertext data. */
} mpsl_ecb_hal_data_t;

#define MPSL_ECB_NO_FLAGS               (0)
#define MPSL_ECB_INPUT_LE               (1u << 0)
#define MPSL_ECB_OUTPUT_LE              (1u << 1)
#define MPSL_ECB_CLEARTEXT_IN_LOCAL_RAM (1u << 2)

/**@brief Encrypts a block according to the specified parameters.
 *
 * @note This function will only return once the ciphertext has been generated.
 * @note This function may only be used on nRF52 and nRF53 series devices.
 * @note This function is deprecated. Do not use.
 *
 * @param p_ecb_data Pointer to ECB data structure. Its elements are assumed to be big-endian.
 */
void mpsl_ecb_block_encrypt(mpsl_ecb_hal_data_t *p_ecb_data);

/**@brief Encrypts a block according to the specified parameters.
 *
 * @note This function will only return once the ciphertext has been generated.
 *
 * @param[in]  key        Encryption key
 * @param[in]  cleartext  Plaintext to be encrypted
 * @param[out] ciphertext Encrypted text
 * @param[in]  flags      Any combination of the following flags:
 *  - MPSL_ECB_INPUT_LE               The key and cleartext are in little-endian format.
 *  - MPSL_ECB_OUTPUT_LE              The ciphertext will be returned in little-endian format.
 *  - MPSL_ECB_CLEARTEXT_IN_LOCAL_RAM Unless this is set, the @p cleartext will be copied to RAM
 *                                    since the ECB hardware cannot access other memory locations.
 */
void mpsl_ecb_block_encrypt_extended(const uint8_t key[MPSL_ECB_KEY_LENGTH],
                                     const uint8_t cleartext[MPSL_ECB_CLEARTEXT_LENGTH],
                                     uint8_t ciphertext[MPSL_ECB_CIPHERTEXT_LENGTH],
                                     uint32_t flags);

#endif /* MPSL_ECB_H__ */
