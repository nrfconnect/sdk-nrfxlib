/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue nrf_security mbedcrypto glue library
 * @{
 * @brief nrf_security mbedcrypto glue library containing glue functionality for
 *        mbedcrypto APIs when using multiple backends.
 * @}
 *
 * @defgroup mbedcrypto_glue_aes mbedcrypto AES glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Glue layer for mbedcrypto AES APIs, including typedefs for backend API abstraction.
 */
#ifndef BACKEND_AES_H
#define BACKEND_AES_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AES_ALT)

#include "mbedtls/aes.h"

/**@brief Function pointer that checks if AES is supported by the
 * backend, depending on the given key size in bits, mode, and usage of XTS.
 * 
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the AES cipher (mode, key size etc). If the value is positive,
 *          then the backend with the highest value is selected (priority based).
 *
 * @param[in]   keybits     Key size in bits for the AES functionality.
 * @param[in]   mode        AES mode.
 * @param[in]   xts         If 1, XTS mode is used. Otherwise 0.
 *
 * @return 0 if the AES functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_aes_check_fn)(unsigned int keybits, int mode, int xts);


/**@brief Function pointer to initialize a glue AES context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_init.
 * This function inits or resets the glue context for an AES operation.
 *
 * @param[in,out]       ctx         Pointer to the context to initialize.
 */
typedef void (*mbedtls_aes_init_fn)(mbedtls_aes_context *ctx);


/**@brief Function pointer to free a glue AES context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_free.
 * This function resets the glue context for an AES operation.
 *
 * @param[in,out]       ctx         Pointer to the context to free.
 */
typedef void (*mbedtls_aes_free_fn)(mbedtls_aes_context *ctx);


/**@brief Function pointer to initialize a glue AES XTS context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_xts_init.
 * This function initializes or resets the glue context for an AES XTS operation.
 *
 * @param[in,out]       ctx         Pointer to the context to initialize.
 */
typedef void (*mbedtls_aes_xts_init_fn)(mbedtls_aes_xts_context *ctx);


/**@brief Function pointer to free a glue AES XTS context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_xts_free.
 * This function frees the glue context for an AES XTS operation.
 *
 * @param[in,out]       ctx         Pointer to the context to free.
 */
typedef void (*mbedtls_aes_xts_free_fn)(mbedtls_aes_xts_context *ctx);


/**@brief Function pointer to set AES Encryption key in a glue context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_setkey_enc.
 *
 * @param[in,out]       ctx         Pointer to the context to set the encryption key in.
 * @param[in]           key         Pointer to the array holding the encryption key.
 * @param[in]           keybits     Key size in bits.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_setkey_enc_fn)(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);


/**@brief Function pointer to set AES decryption key in a glue context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_setkey_dec.
 *
 * @param[in,out]       ctx         Pointer to the context to set the decryption key in.
 * @param[in]           key         Pointer to the array holding the decryption key.
 * @param[in]           keybits     Key size in bits.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_setkey_dec_fn)(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);


/**@brief Function pointer to set AES XTS encryption key in a glue context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_xts_setkey_enc.
 *
 * @param[in,out]       ctx         Pointer to the context to set the encryption key in.
 * @param[in]           key         Pointer to the array holding the encryption key.
 * @param[in]           keybits     Key size in bits.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_xts_setkey_enc_fn)(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits);


/**@brief Function pointer to set AES XTS decryption key in a glue context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_xts_setkey_dec.
 *
 * @param[in,out]       ctx         Pointer to the context to set the decryption key in.
 * @param[in]           key         Pointer to the array holding the decryption key.
 * @param[in]           keybits     Key size in bits.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_xts_setkey_dec_fn)(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits);


/**@brief Function pointer to perform an AES encryption.
 *
 * @details This function pointer has a signature equal to @c mbedtls_internal_aes_encrypt.
 *
 * @param[in,out]       ctx         Pointer to the context for the encrypt operation.
 * @param[in]           input       Array of the block to encrypt (16 bytes).
 * @param[out]          output      Array of the buffer to hold the encrypted block (16 bytes).
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_internal_aes_encrypt_fn)(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16]);


/**@brief Function pointer to perform an AES decryption.
 *
 * @details This function pointer has a signature equal to @c mbedtls_internal_aes_decrypt.
 *
 * @param[in,out]       ctx         Pointer to the context for the decrypt operation.
 * @param[in]           input       Array of the block to decrypt (16 bytes).
 * @param[out]          output      Array of the buffer to hold the decrypted block (16 bytes).
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_internal_aes_decrypt_fn)(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16]);


/**@brief Function pointer to encrypt/decrypt using AES CBC.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_crypt_cbc.
 *
 * @param[in,out]       ctx         Pointer to the context for the encrypt/decrypt operation.
 * @param[in]           mode        Mode of AES operation (encrypt/decrypt).
 * @param[in]           length      Length of the input/output data.
 * @param[in]           iv          Array with the initialization vector used for the encrypt/decrypt operation.
 * @param[in]           input       Pointer to the buffer holding the input data.
 * @param[out]          output      Pointer to the buffer to hold the output data.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_crypt_cbc_fn)(mbedtls_aes_context *ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output);


/**@brief Function pointer to encrypt/decrypt using AES XTS.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_crypt_xts.
 *
 * @param[in,out]       ctx         Pointer to the context for the encrypt/decrypt operation.
 * @param[in]           mode        Mode of AES operation (encrypt/decrypt).
 * @param[in]           length      Length of the input/output data.
 * @param[in]           data_unit   Array with @c data_unit used for the encrypt/decrypt operation.
 * @param[in]           input       Pointer to the buffer holding the input data.
 * @param[out]          output      Pointer to the buffer to hold the output data.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_crypt_xts_fn)(mbedtls_aes_xts_context *ctx, int mode, size_t length, const unsigned char data_unit[16], const unsigned char *input, unsigned char *output);


/**@brief Function pointer to encrypt/decrypt using AES CFB128.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_crypt_cfb128.
 *
 * @param[in,out]       ctx         Pointer to the context for the encrypt/decrypt operation.
 * @param[in]           mode        Mode of AES operation (encrypt/decrypt).
 * @param[in]           length      Length of the input/output data.
 * @param[in,out]       iv_off      Pointer to the variable holding the initialization vector offset.
 * @param[in]           iv          Array with the initialization vector used for the encrypt/decrypt operation.
 * @param[in]           input       Pointer to the buffer holding the input data.
 * @param[out]          output      Pointer to the buffer to hold the output data.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_crypt_cfb128_fn)(mbedtls_aes_context *ctx, int mode, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output);


/**@brief Function pointer to encrypt/decrypt using AES CFB8.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_crypt_cfb8.
 *
 * @param[in,out]       ctx         Pointer to the context for the encrypt/decrypt operation.
 * @param[in]           mode        Mode of AES operation (encrypt/decrypt).
 * @param[in]           length      Length of the input/output data.
 * @param[in]           iv          Array with the initialization vector used for the encrypt/decrypt operation.
 * @param[in]           input       Pointer to the buffer holding the input data.
 * @param[out]          output      Pointer to the buffer to hold the output data.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_crypt_cfb8_fn)(mbedtls_aes_context *ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output);


/**@brief Function pointer to encrypt/decrypt using AES OFB.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_crypt_ofb.
 *
 * @param[in,out]       ctx         Pointer to the context for the encrypt/decrypt operation.
 * @param[in]           length      Length of the input/output data.
 * @param[in,out]       iv_off      Pointer to the variable holding the initialization vector offset.
 * @param[in]           iv          Array with the initialization vector used for the encrypt/decrypt operation.
 * @param[in]           input       Pointer to the buffer holding the input data.
 * @param[out]          output      Pointer to the buffer to hold the output data.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_crypt_ofb_fn)(mbedtls_aes_context *ctx, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output);


/**@brief Function pointer to encrypt/decrypt using AES CTR.
 *
 * @details This function pointer has a signature equal to @c mbedtls_aes_crypt_ctr.
 *
 * @param[in,out]       ctx             Pointer to the context for the encrypt/decrypt operation.
 * @param[in]           length          Length of the input/output data.
 * @param[in,out]       nc_off          Pointer to the variable holding the nonce offset.
 * @param[in]           nonce_counter   Array with nonce counter.
 * @param[in]           stream_block    Array with stream block.
 * @param[in]           input           Pointer to the buffer holding the input data.
 * @param[out]          output          Pointer to the buffer to hold the output data.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_aes_crypt_ctr_fn)(mbedtls_aes_context *ctx, size_t length, size_t *nc_off, unsigned char nonce_counter[16], unsigned char stream_block[16], const unsigned char *input, unsigned char *output);


/**@brief Structure type holding the AES calling interface for a backend.
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality. Unsupported features
 *          can be set to NULL.
 */
typedef struct
{
    size_t backend_context_size;                        //!< Size of the AES context according to the backend.
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    size_t backend_xts_context_size;                    //!< Size of the AES XTS context according to the backend.
#endif /* MBEDTLS_CIPHER_MODE_XTS */
    mbedtls_aes_check_fn check;                         //!< Check for AES support in the backend.
    mbedtls_aes_init_fn init;                           //!< Initialize the AES context.
    mbedtls_aes_free_fn free;                           //!< Free the AES context.
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    mbedtls_aes_xts_init_fn xts_init;                   //!< Initialize the AES XTS context.
    mbedtls_aes_xts_free_fn xts_free;                   //!< Free the AES XTS context.
#endif /* MBEDTLS_CIPHER_MODE_XTS */
    mbedtls_aes_setkey_enc_fn setkey_enc;               //!< Set an AES encryption key.
    mbedtls_aes_setkey_dec_fn setkey_dec;               //!< Set an AES decryption key.
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    mbedtls_aes_xts_setkey_enc_fn xts_setkey_enc;       //!< Set an AES XTS encryption key.
    mbedtls_aes_xts_setkey_dec_fn xts_setkey_dec;       //!< Set an AES XTS decryption key.
#endif /* MBEDTLS_CIPHER_MODE_XTS */
    mbedtls_internal_aes_encrypt_fn internal_encrypt;   //!< Perform AES encrypt operation.
    mbedtls_internal_aes_decrypt_fn internal_decrypt;   //!< Perform AES decrypt operation.
#if defined(MBEDTLS_CIPHER_MODE_CBC)
    mbedtls_aes_crypt_cbc_fn crypt_cbc;                 //!< Perform AES CBC encrypt/decrypt operation.
#endif /* MBEDTLS_CIPHER_MODE_CBC */
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    mbedtls_aes_crypt_xts_fn crypt_xts;                 //!< Perform AES XTS encrypt/decrypt operation.
#endif /* MBEDTLS_CIPHER_MODE_XTS */
#if defined(MBEDTLS_CIPHER_MODE_CFB)
    mbedtls_aes_crypt_cfb128_fn crypt_cfb128;           //!< Perform AES CFB128 encrypt/decrypt operation.
    mbedtls_aes_crypt_cfb8_fn crypt_cfb8;               //!< Perform AES CFB8 encrypt/decrypt operation.
#endif /* MBEDTLS_CIPHER_MODE_CFB */
#if defined(MBEDTLS_CIPHER_MODE_OFB)
    mbedtls_aes_crypt_ofb_fn crypt_ofb;                 //!< Perform AES OFB encrypt/decrypt operation.
#endif /* MBEDTLS_CIPHER_MODE_OFB */
#if defined(MBEDTLS_CIPHER_MODE_CTR)
    mbedtls_aes_crypt_ctr_fn crypt_ctr;                 //!< Perform AES CTR encrypt/decrypt operation.
#endif /* MBEDTLS_CIPHER_MODE_CTR */
} mbedtls_aes_funcs;

#endif /* MBEDTLS_AES_ALT */

#endif /* BACKEND_AES_H */

/** @} */
