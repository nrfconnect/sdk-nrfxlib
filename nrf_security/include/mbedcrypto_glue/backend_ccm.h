/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_aes_ccm mbedcrypto AES CCM glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Glue layer for mbedcrypto AES CCM APIs, including typedefs for backend API abstraction.
 */
#ifndef BACKEND_CCM_H
#define BACKEND_CCM_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_CCM_ALT)

#include "mbedtls/ccm.h"

/**@brief Function pointer to check if the backend supports CCM, depending on the cipher
 *        and number of bits in key.
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the AES CCM cipher (mode, keysize etc). If the value is positive,
 *          then the backend with the highest value is selected (priority based).
 *
 * @param[in]   mode        AES CCM mode.
 * @param[in]   keybits     Key size in bits for the AES functionality.
 *
 * @return 0 if the AES CCM functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_ccm_check_fn)(mbedtls_cipher_id_t cipher, unsigned int keybits);


/**@brief Function pointer to initialize a glue AES CCM context.
 *
 * @details This function inits or resets the glue context for an AES CCM operation.
 * This function pointer has a signature equal to @c mbedtls_ccm_init.
 *			
 * @param[in,out]       ctx         Pointer to the context to initialize.
 */
typedef void (*mbedtls_ccm_init_fn)(mbedtls_ccm_context *ctx);


/**@brief Function pointer to free a glue AES CCM context.
 *
 * @details This function frees the glue context for an AES CCM operation.
 * This function pointer has a signature equal to @c mbedtls_ccm_free.
 * 
 * @param[in,out]       ctx        Pointer to the context to free.
 */
typedef void (*mbedtls_ccm_free_fn)(mbedtls_ccm_context *ctx);


/**@brief Function pointer to set AES CCM key in a glue context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ccm_setkey.
 *
 * @param[in,out]       ctx         Pointer to the context to set the encryption key in.
 * @param[in]           cipher      Cipher to set (CCM or CCM*).
 * @param[in]           key         Pointer to the array holding the encryption key.
 * @param[in]           keybits     Key size in bits.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ccm_setkey_fn)(mbedtls_ccm_context *ctx, mbedtls_cipher_id_t cipher, const unsigned char *key, unsigned int keybits);


/**@brief Function pointer to perform an AES CCM encrypt-and-tag operation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ccm_encrypt_and_tag.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           length      Length of the operation.
 * @param[in]           iv          Pointer to the array holding the initialization vector.
 * @param[in]           iv_len      Length of the initialization vector.
 * @param[in]           add         Pointer to the array holding optional associated data.
 * @param[in]           add_len     Length of optional associated data.
 * @param[in]           input       Pointer to the array holding the input.
 * @param[out]          output      Pointer to the array to hold the output.
 * @param[out]          tag         Pointer to the array to hold the tag.
 * @param[in]           tag_len     Length of the tag.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ccm_encrypt_and_tag_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, unsigned char *tag, size_t tag_len);


/**@brief Function pointer to perform an AES CCM* encrypt-and-tag operation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ccm_star_encrypt_and_tag.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           length      Length of the operation.
 * @param[in]           iv          Pointer to the array holding the initialization vector.
 * @param[in]           iv_len      Length of the initialization vector.
 * @param[in]           add         Pointer to the array holding optional associated data.
 * @param[in]           add_len     Length of optional associated data.
 * @param[in]           input       Pointer to the array holding the input.
 * @param[out]          output      Pointer to the array to hold the output.
 * @param[out]          tag         Pointer to the array to hold the tag.
 * @param[in]           tag_len     Length of the tag.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ccm_star_encrypt_and_tag_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, unsigned char *tag, size_t tag_len);


/**@brief Function pointer to perform an AES CCM decrypt operation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ccm_auth_decrypt.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           length      Length of the operation.
 * @param[in]           iv          Pointer to the array holding the initialization vector.
 * @param[in]           iv_len      Length of the initialization vector.
 * @param[in]           add         Pointer to the array holding optional associated data.
 * @param[in]           add_len     Length of optional associated data.
 * @param[in]           input       Pointer to the array holding the input.
 * @param[out]          output      Pointer to the array to hold the output.
 * @param[in]           tag         Pointer to the array holding the tag.
 * @param[in]           tag_len     Length of the tag.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ccm_auth_decrypt_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, const unsigned char *tag, size_t tag_len);


/**@brief Function pointer to perform an AES CCM* decrypt operation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ccm_star_auth_decrypt.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           length      Length of the operation.
 * @param[in]           iv          Pointer to the array holding the initialization vector.
 * @param[in]           iv_len      Length of the initialization vector.
 * @param[in]           add         Pointer to the array holding optional associated data.
 * @param[in]           add_len     Length of optional associated data.
 * @param[in]           input       Pointer to the array holding the input.
 * @param[out]          output      Pointer to the array to hold the output.
 * @param[in]           tag         Pointer to the array holding the tag.
 * @param[in]           tag_len     Length of the tag.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ccm_star_auth_decrypt_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, const unsigned char *tag, size_t tag_len);


/**@brief Structure type holding the AES CCM calling interface for a backend.
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    size_t backend_context_size;                                //!< Size of the AES CCM/CCM* context according to the backend.
    mbedtls_ccm_check_fn check;                                 //!< Check for AES CCM/CCM* support.
    mbedtls_ccm_init_fn init;                                   //!< Initialize the AES CCM/CCM* context.
    mbedtls_ccm_free_fn free;                                   //!< Free the AES CCM/CCM* context.
    mbedtls_ccm_setkey_fn setkey;                               //!< Set an AES CCM/CCM* crypt key.
    mbedtls_ccm_encrypt_and_tag_fn encrypt_and_tag;             //!< Perform an AES CCM encrypt-and-tag operation.
    mbedtls_ccm_star_encrypt_and_tag_fn star_encrypt_and_tag;   //!< Perform an AES CCM* encrypt-and-tag operation.
    mbedtls_ccm_auth_decrypt_fn auth_decrypt;                   //!< Perform an AES CCM decrypt operation.
    mbedtls_ccm_star_auth_decrypt_fn star_auth_decrypt;         //!< Perform an AES CCM* decrypt operation.
} mbedtls_ccm_funcs;

#endif /* MBEDTLS_CCM_ALT */

#endif /* BACKEND_CCM_H */

/** @} */
