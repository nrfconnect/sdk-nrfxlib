/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MPSL_ECB_H__
#define MPSL_ECB_H__

#include <stdint.h>

#define MPSL_ECB_KEY_LENGTH                (16)                       /**< ECB key length. */
#define MPSL_ECB_CLEARTEXT_LENGTH          (16)                       /**< ECB cleartext length. */
#define MPSL_ECB_CIPHERTEXT_LENGTH         (MPSL_ECB_CLEARTEXT_LENGTH) /**< ECB ciphertext length. */

/**@brief Encrypts a block according to the specified parameters.
 *
 * @note This function will only return once the ciphertext has been generated.
 * @note The plaintext must be in RAM.
 *
 * @param[in]  key        Encryption key
 * @param[in]  cleartext  Plaintext to be encrypted
 * @param[out] ciphertext Encrypted text
 */
void mpsl_ecb_block_encrypt(const uint8_t key[MPSL_ECB_KEY_LENGTH],
                            uint8_t cleartext[MPSL_ECB_CLEARTEXT_LENGTH],
                            uint8_t ciphertext[MPSL_ECB_CIPHERTEXT_LENGTH]);

#endif /* MPSL_ECB_H__ */
