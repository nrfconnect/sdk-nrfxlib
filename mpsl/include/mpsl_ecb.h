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

/**@brief AES ECB parameter typedefs */
typedef uint32_t mpsl_ecb_key_t[MPSL_ECB_KEY_LENGTH/sizeof(uint32_t)]; /**< Encryption key type. */
typedef uint8_t mpsl_ecb_cleartext_t[MPSL_ECB_CLEARTEXT_LENGTH];       /**< Cleartext data type. */
typedef uint8_t mpsl_ecb_ciphertext_t[MPSL_ECB_CIPHERTEXT_LENGTH];     /**< Ciphertext data type. */

/**@brief AES ECB data structure */
typedef struct
{
  mpsl_ecb_key_t        key;            /**< Encryption key. */
  mpsl_ecb_cleartext_t  cleartext;      /**< Cleartext data. */
  mpsl_ecb_ciphertext_t ciphertext;     /**< Ciphertext data. */
} mpsl_ecb_hal_data_t;

/**@brief Encrypts a block according to the specified parameters.
 *
 * @note This function will only return once the ciphertext has been generated.
 *
 * @param[in, out] p_ecb_data pointer to the ECB paramters' struct (two input
 *                            parameters and one output parameter).
 */
void mpsl_ecb_block_encrypt(mpsl_ecb_hal_data_t * p_ecb_data);

#endif /* MPSL_ECB_H__ */
