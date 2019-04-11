/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_cmac mbedcrypto CMAC glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Glue layer for mbedcrypto CMAC APIs, including typedefs for backend API abstraction.
 */
#ifndef BACKEND_CMAC_H
#define BACKEND_CMAC_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_CMAC_ALT)

#include "mbedtls/cmac.h"

/**@brief Function pointer to check if the backend supports CMAC, depending on cipher_info,
 *        key, and key bits.
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the CMAC mode. If the value is positive, then the backend with the
 *          highest value is selected (priority based).
 *
 * @param[in]   cipher_info Cipher info for the CMAC operation.
 * @param[in]   key         Pointer to the array holding the key.
 * @param[in]   keybits     Key size in bits.
 *
 * @return 0 if the CMAC functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_cipher_cmac_check_fn)(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keybits);


/**@brief Function pointer to do CMAC starts operation using given key
 *
 * This function pointer has a signature equal to @c mbedtls_cipher_cmac_start.
 *
 * @note Run this function pointer to initialize the context.
 *
 * @param[in,out]   ctx     Pointer to context for the CMAC operation.
 * @param[in]       key     Pointer to array holding the key.
 * @param[in]       keybits Key size in bits.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_cipher_cmac_starts_fn)(mbedtls_cipher_context_t *ctx , const unsigned char *key, size_t keybits);


/**@brief Typedef for function pointer to do CMAC update operation given additional input
 *
 * This function pointer has a signature equal to @c mbedtls_cipher_cmac_update.
 *
 * @param[in,out]   ctx     Pointer to context for the CMAC operation.
 * @param[in]       input   Pointer to array holding additional input.
 * @param[in]       ilen    Length of additional input.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_cipher_cmac_update_fn)(mbedtls_cipher_context_t *ctx , const unsigned char *input, size_t ilen);


/**@brief Typedef for function pointer to do CMAC finish operation.
 *
 * This function pointer has a signature equal to @c mbedtls_cipher_cmac_finish.
 *
 * @param[in,out]   ctx     Pointer to context for the CMAC operation.
 * @param[out]      output  Output of the CMAC operation.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_cipher_cmac_finish_fn)(mbedtls_cipher_context_t *ctx , unsigned char *output);


/**@brief Typedef for function pointer to do CMAC reset operation
 *
 * This function pointer has a signature equal to @c mbedtls_cipher_cmac_reset.
 *
 * @param[in,out]   ctx     Pointer to context for the CMAC operation to reset.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_cipher_cmac_reset_fn)(mbedtls_cipher_context_t *ctx);


/**@brief Typedef for function pointer to do free a CMAC context operation
 *
 * This function pointer has a signature equal to @c mbedtls_cipher_cmac_free.
 *
 * @param[in,out]   ctx     Pointer to context for the CMAC operation to free.
 */
typedef void (*mbedtls_cipher_cmac_free_fn)(mbedtls_cipher_context_t *ctx);


/**@brief Typedef for function to do CMAC operation in one single step
 *
 * This function pointer has a signature equal to @c mbedtls_cipher_cmac.
 *
 * @param[in,out]   ctx     Pointer to context for the CMAC operation.
 * @param[in]       key     Pointer to array holding the key.
 * @param[in]       keybits Key size in bits.
 * @param[in]       input   Pointer to array holding additional input.
 * @param[in]       ilen    Length of additional input.
 * @param[out]      output  Output of the CMAC operation.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_cipher_cmac_fn)(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keylen , const unsigned char *input, size_t ilen , unsigned char *output);


/**@brief Typedef for function to do AES PRF 128 operation in one single step
 *
 * This function pointer has a signature equal to @c mbedtls_aes_cmac_prf_128.
 *
 * @param[in]       key     Pointer to array holding the key.
 * @param[in]       key_len Key size in bytes.
 * @param[in]       input   Pointer to array holding additional input.
 * @param[in]       in_len  Length of additional input.
 * @param[out]      output  Output of the CMAC operation.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_cmac_prf_128_fn)(const unsigned char *key, size_t key_len , const unsigned char *input, size_t in_len , unsigned char output[16]);


/**@brief Typedef for structure type holding the CMAC calling interface for a backend
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    mbedtls_cipher_cmac_check_fn check;         //!< Function to check for CMAC support in the backend.
    mbedtls_cipher_cmac_starts_fn starts;       //!< Function to start the a CMAC operation.
    mbedtls_cipher_cmac_update_fn update;       //!< Function to update the CMAC operation with additional data.
    mbedtls_cipher_cmac_finish_fn finish;       //!< Function to finish the CMAC operation.
    mbedtls_cipher_cmac_reset_fn reset;         //!< Function to reset the CMAC context.
    mbedtls_cipher_cmac_free_fn free;           //!< Function to free the CMAC context.
    mbedtls_cipher_cmac_fn cmac;                //!< Function to do CMAC in one single step.
#if defined(MBEDTLS_AES_C)
    mbedtls_aes_cmac_prf_128_fn aes_prf_128;    //!< Function to do AES PRF 128 in one single step.
#endif /* MBEDTLS_AES_C */
} mbedtls_cmac_funcs;

#endif /* MBEDTLS_CMAC_ALT */

#endif // BACKEND_CMAC_H

/** @} */
