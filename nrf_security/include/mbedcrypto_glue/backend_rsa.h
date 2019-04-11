/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_rsa mbedcrypto RSA glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Glue layer for mbedcrypto RSA APIs, including typedefs for backend API abstraction.
 */
#ifndef BACKEND_RSA_H
#define BACKEND_RSA_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_RSA_ALT)

#include "mbedtls/rsa.h"

/**@brief Function pointer to check if the backend support RSA given
 *        padding type, hash-id and modulus bit size
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the RSA cipher. If the value is positive, then the backend with the
 *          highest value is selected (priority based).
 *
 * @param[in]       padding     Reserved type, currently not in use.
 * @param[in]       hash_id     Reserved type, currently not in use.
 * @param[in]       nbits       Modulus bit size.
 *
 * @return 0 if the RSA functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_rsa_check_fn)(int padding, int hash_id, unsigned int nbits);


/**@brief Function pointer to initialize an mbedcrypto glue RSA context.
 *
 * @details This function inits or resets the glue context for an RSA operation.
 * This function pointer has a signature equal to @c mbedtls_rsa_init.
 * 
 *
 * @param[in,out]       ctx         Pointer to the context to initialize.
 * @param[in]           padding     Either @c MBEDTLS_RSA_PKCS_V21 or @c MBEDTLS_RSA_PKCS_V15.
 * @param[in]           hash_id     Hash identifier.
 */
typedef void (*mbedtls_rsa_init_fn)(mbedtls_rsa_context *ctx, int padding, int hash_id);


/**@brief Function pointer to import core RSA parameters into a glue RSA context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_import.
 *
 * @param[in,out]       ctx         Pointer to the context. Must be initialized.
 * @param[in]           N           Pointer to the RSA modulus. May be NULL.
 * @param[in]           P           Pointer to the first RSA prime factor. May be NULL.
 * @param[in]           Q           Pointer to the second RSA prime factor. May be NULL.
 * @param[in]           D           Pointer to the private exponent. May be NULL.
 * @param[in]           E           Pointer to the public exponent. May be NULL.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_import_fn)(mbedtls_rsa_context *ctx, const mbedtls_mpi *N, const mbedtls_mpi *P, const mbedtls_mpi *Q, const mbedtls_mpi *D, const mbedtls_mpi *E);


/**@brief Function pointer to import core RSA parameters into a glue RSA context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_import_raw.
 *
 * @note The raw input is expected to be in big-endian format.
 *
 * @param[in,out]       ctx         Pointer to the context. Must be initialized.
 * @param[in]           N           Pointer to the array holding the RSA modulus. May be NULL.
 * @param[in]           N_len       The length of @p N. Ignored if @p N is NULL.
 * @param[in]           P           Pointer to the array holding the first RSA prime factor. May be NULL.
 * @param[in]           P_len       The length of @p P. Ignored if @p P is NULL.
 * @param[in]           Q           Pointer to the array holding the second RSA prime factor. May be NULL.
 * @param[in]           Q_len       The length of @p Q. Ignored if @p Q is NULL.
 * @param[in]           D           Pointer to the array holding the private exponent. May be NULL.
 * @param[in]           D_len       The length of @p D. Ignored if @p D is NULL.
 * @param[in]           E           Pointer to the array holding the public exponent. May be NULL.
 * @param[in]           E_len       The length of @p E. Ignored if @p E is NULL.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_import_raw_fn)(mbedtls_rsa_context *ctx, unsigned char const *N, size_t N_len, unsigned char const *P, size_t P_len, unsigned char const *Q, size_t Q_len, unsigned char const *D, size_t D_len, unsigned char const *E, size_t E_len);


/**@brief Function pointer to complete an RSA context from a set of imported core parameters.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_complete.
 *
 * @param[in,out]       ctx         The RSA context that is initialized and holds the imported parameters.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_complete_fn)(mbedtls_rsa_context *ctx);


/**@brief Function pointer to export an RSA context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_export.
 *
 * @param[in]           ctx         Pointer to the context that is initialized and holds the core parameters.
 * @param[out]          N           Pointer to an MPI to hold the RSA modulus, or NULL if not to export.
 * @param[out]          P           Pointer to an MPI to hold the first prime factor, or NULL if not to export.
 * @param[out]          Q           Pointer to an MPI to hold the second prime factor, or NULL if not to export.
 * @param[out]          D           Pointer to an MPI to hold the private exponent, or NULL if not to export.
 * @param[out]          Q           Pointer to an MPI to hold the public exponent, or NULL if not to export.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_export_fn)(const mbedtls_rsa_context *ctx, mbedtls_mpi *N, mbedtls_mpi *P, mbedtls_mpi *Q, mbedtls_mpi *D, mbedtls_mpi *E);


/**@brief Function pointer to export an RSA context to raw form.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_export_raw.
 *
 * @note The raw output is in big-endian format.
 *
 * @param[in]           ctx         Pointer to the context that is initialized and holds the core parameters.
 * @param[out]          N           Pointer to an array to hold the RSA modulus, or NULL if not to export.
 * @param[out]          N_len       The length of @p N. Ignored if @p N is NULL.
 * @param[out]          P           Pointer to an array to hold the first prime factor, or NULL if not to export.
 * @param[out]          P_len       The length of @p P. Ignored if @p P is NULL.
 * @param[out]          Q           Pointer to an array to hold the second prime factor, or NULL if not to export.
 * @param[out]          Q_len       The length of @p Q. Ignored if @p Q is NULL.
 * @param[out]          D           Pointer to an array to hold the private exponent, or NULL if not to export.
 * @param[out]          D_len       The length of @p D. Ignored if @p D is NULL.
 * @param[out]          E           Pointer to an array to hold the public exponent, or NULL if not to export.
 * @param[out]          E_len       The length of @p E. Ignored if @p E is NULL.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_export_raw_fn)(const mbedtls_rsa_context *ctx, unsigned char *N, size_t N_len, unsigned char *P, size_t P_len, unsigned char *Q, size_t Q_len, unsigned char *D, size_t D_len, unsigned char *E, size_t E_len);


/**@brief Function pointer to export RSA CRT parameters of a private RSA key.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_export_crt.
 *
 * @param[in]           ctx         Pointer to the context that is initialized and holds the core parameters.
 * @param[out]          DP          Pointer to an MPI to hold D modulo P-1, or NULL if not to export.
 * @param[out]          DQ          Pointer to an MPI to hold D modulo Q-1, or NULL if not to export.
 * @param[out]          QP          Pointer to an MPI to hold modular inverse of Q modulo P, or NULL if not to export.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_export_crt_fn)(const mbedtls_rsa_context *ctx, mbedtls_mpi *DP, mbedtls_mpi *DQ, mbedtls_mpi *QP);


/**@brief Function pointer to set RSA padding and hash type.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_set_padding.
 *
 * @param[in,out]       ctx         Pointer to the context to update.
 * @param[in]           padding     Either @c MBEDTLS_RSA_PKCS_V21 or @c MBEDTLS_RSA_PKCS_V15.
 * @param[in]           hash_id     Hash identifier.
 */
typedef void (*mbedtls_rsa_set_padding_fn)(mbedtls_rsa_context *ctx, int padding, int hash_id);


/**@brief Function pointer to get the length of the RSA modulus in bytes.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_get_len.
 *
 * @param[in]           ctx         Pointer to the context. Must be initialized and configured.
 *
 * @return Length of the RSA modulus in bytes.
 */
typedef size_t (*mbedtls_rsa_get_len_fn)(const mbedtls_rsa_context *ctx);


/**@brief Function pointer to generate an RSA key.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_gen_key.
 *
 * @param[in,out]       ctx         Pointer to an initialized context used to hold the private key after the call.
 * @param[in]           f_rng       RNG function.
 * @param[in,out]       p_rng       RNG context.
 * @param[in]           nbits       The size of the public key in bits.
 * @param[in]           exponent    The public exponent for use (for example, 65537).
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_gen_key_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, unsigned int nbits, int exponent);


/**@brief Function pointer to check if an RSA context contains a public key.
 *
 * @details A successful check of this function means that the context can be used for
 *          RSA public key operations.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_check_pubkey.
 *
 * @param[in]           ctx         Pointer to an initialized context to check.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_check_pubkey_fn)(const mbedtls_rsa_context *ctx);


/**@brief Function pointer to check if an RSA context contains a private key.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_check_privkey.
 *
 * @param[in]           ctx         Pointer to an initialized context to check.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_check_privkey_fn)(const mbedtls_rsa_context *ctx);


/**@brief Function pointer to check an RSA public/private key pair.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_check_pub_priv.
 *
 * @param[in]       pub     Pointer to a context holding an RSA public key.
 * @param[in]       priv    Pointer to a context holding an RSA private key.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_check_pub_priv_fn)(const mbedtls_rsa_context *pub, const mbedtls_rsa_context *prv);


/**@brief Function pointer to perform an RSA public key operation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_public.
 *
 * @note The size of the input and output buffer must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized context.
 * @param[in]       input   Pointer to the buffer holding the input.
 * @param[out]      output  Pointer to the buffer to hold the output.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_public_fn)(mbedtls_rsa_context *ctx, const unsigned char *input, unsigned char *output);


/**@brief Function pointer to perform an RSA private key operation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_private.
 *
 * @note The size of the input and output buffers must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized RSA context.
 * @param[in]       f_rng   RNG function.
 * @param[in,out]   p_rng   RNG context.
 * @param[in]       input   Pointer to the buffer holding the input.
 * @param[out]      output  Pointer to the buffer to hold the output.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_private_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, const unsigned char *input, unsigned char *output);


/**@brief Function pointer to perform PKCS#1 encryption.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_encrypt.
 *
 * @note The size of the input and output buffers must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized RSA context.
 * @param[in]       f_rng   RNG function.
 * @param[in,out]   p_rng   RNG context.
 * @param[in]       mode    Mode of operation.
 * @param[in]       input   Pointer to the buffer holding the input.
 * @param[out]      output  Pointer to the buffer to hold the output.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output);


/**@brief Function pointer to perform PKCS#1 v1.5 encryption.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsaes_pkcs1_v15_encrypt.
 *
 * @note The size of the output buffer must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized RSA context.
 * @param[in]       f_rng   RNG function.
 * @param[in,out]   p_rng   RNG context.
 * @param[in]       mode    Mode of operation.
 * @param[in]       ilen    Length of the plain text in bytes.
 * @param[in]       input   Pointer to the buffer holding the input in plain text.
 * @param[out]      output  Pointer to the buffer to hold the encrypted output.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_pkcs1_v15_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output);


/**@brief Function pointer to perform PKCS1 v2.1 encryption (RSAES OAEP).
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsaes_oaep_encrypt.
 *
 * @param[in]       ctx         Pointer to an initialized RSA context.
 * @param[in]       f_rng       RNG function.
 * @param[in,out]   p_rng       RNG context.
 * @param[in]       mode        Mode of operation.
 * @param[in]       label       Buffer holding the custom label to use, or NULL if not used.
 * @param[in]       label_len   Length of the label. Skipped if label is NULL.
 * @param[in]       ilen        Length of the plain text in bytes.
 * @param[in]       input       Pointer to the buffer holding the input in plain text.
 * @param[out]      output      Pointer to the buffer to hold the encrypted output.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_oaep_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t ilen, const unsigned char *input, unsigned char *output);


/**@brief Function pointer to perform PKCS#1 decryption.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_decrypt.
 *
 * @note This function removes message padding. The output buffer
 *       should be ctx->len in size (for example, 256 bytes for 2048-bit RSA).
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[out]      olen            Pointer to the variable to hold the length of the decrypted plain text in bytes.
 * @param[in]       input           Pointer to the buffer holding the input in encrypted form.
 * @param[out]      output          Pointer to the buffer to hold the output in plain text.
 * @param[in]       output_max_len  Max length available to hold the decrypted plain text.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);


/**@brief Function pointer to perform PKCS#1 v1.5 decryption.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsaes_pkcs1_v15_decrypt.
 *
 * @note The output buffer should be ctx->len in size (for example, 256 bytes
 *       for 2048-bit RSA).
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[out]      olen            Pointer to the variable to hold the length of the decrypted plain text in bytes.
 * @param[in]       input           Pointer to the buffer holding the input in encrypted form.
 * @param[out]      output          Pointer to the buffer to hold the output in plain text.
 * @param[in]       output_max_len  Max length available to hold the decrypted plain text.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_pkcs1_v15_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);


/**@brief Function pointer to perform PKCS#1 v2.1 decryption (RSAES OAEP).
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsaes_oaep_decrypt.
 *
 * @note The output buffer should be ctx->len in size (for example, 256 bytes
 *       for 2048-bit RSA).
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       label           Buffer holding the custom label to use, or NULL if not used.
 * @param[in]       label_len       Length of the label. Set to 0 if custom label is not used.
 * @param[in]       olen            Pointer to the variable holding the length of the decrypted plain text in bytes.
 * @param[in]       input           Pointer to the buffer holding the input in encrypted form.
 * @param[out]      output          Pointer to the buffer to hold the output in plain text.
 * @param[in]       output_max_len  Max length available to hold the decrypted plain text.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_oaep_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);


/**@brief Function pointer to perform PKCS#1 signature generation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_sign.
 *
 * @note The signature buffer should be ctx->len in size (for example, 256 bytes
 *       for 2048-bit RSA).
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       md_alg          Message digest algorithm used to hash the data.
 * @param[in]       hashlen         Length of the message digest.
 * @param[in]       hash            Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[out]      sig             Pointer to the buffer to hold the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);


/**@brief Function pointer to perform PKCS#1 v1.5 signature generation.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsaassa_pkcs1_v15_sign.
 *
 * @note The signature buffer should be ctx->len in size (for example, 256 bytes
 *       for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       md_alg          Message digest algorithm used to hash the data.
 * @param[in]       hashlen         Length of the message digest.
 * @param[in]       hash            Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[out]      sig             Pointer to the buffer to hold the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pkcs1_v15_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);


/**@brief Function pointer to perform PKCS#1 v2.1 signature generation (RSA-PSS).
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pss_sign.
 *
 * @note The signature buffer should be ctx->len in size (for example, 256 bytes
 *       for 2048-bit RSA).
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       md_alg          Message digest algorithm used to hash the data.
 * @param[in]       hashlen         Length of the message digest.
 * @param[in]       hash            Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[out]      sig             Pointer to the buffer to hold the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pss_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);


/**@brief Function pointer to perform PKCS#1 signature verification.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_verify.
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       md_alg          Message digest algorithm used to hash the data.
 * @param[in]       hashlen         Length of the message digest.
 * @param[in]       hash            Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[in]       sig             Pointer to the buffer holding the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);


/**@brief Function pointer to perform PKCS#1 v1.5 signature verification.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pkcs1_v15_verify.
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       md_alg          Message digest algorithm used to hash the data.
 * @param[in]       hashlen         Length of the message digest.
 * @param[in]       hash            Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[in]       sig             Pointer to the buffer holding the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pkcs1_v15_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);


/**@brief Function pointer to perform PKCS#1 v2.1 signature verification (RSA-PSS).
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pss_verify.
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           RNG function.
 * @param[in,out]   p_rng           RNG context.
 * @param[in]       mode            Mode of operation.
 * @param[in]       md_alg          Message digest algorithm used to hash the data.
 * @param[in]       hashlen         Length of the message digest.
 * @param[in]       hash            Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[in]       sig             Pointer to the buffer holding the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pss_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);


/**@brief Function pointer to perform PKCS#1 v2.1 signature verification (RSA-PSS extended features).
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pss_verify.
 *
 * @param[in]       ctx                 Pointer to an initialized RSA context.
 * @param[in]       f_rng               RNG function.
 * @param[in,out]   p_rng               RNG context.
 * @param[in]       mode                Mode of operation.
 * @param[in]       md_alg              Message digest algorithm used to hash the data.
 * @param[in]       hashlen             Length of the message digest.
 * @param[in]       hash                Pointer to the buffer holding the digest or raw data (if @c MBEDTLS_MD_NONE is used).
 * @param[in]       mgf1_hash_id        Message digest used for mask generation.
 * @param[in]       expected_salt_len   Length of the salt used in padding or @c MBEDTLS_RSA_SALT_LEN_ANY to accept all lengths.
 * @param[out]      sig                 Pointer to the buffer holding the signature.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pss_verify_ext_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, mbedtls_md_type_t mgf1_hash_id, int expected_salt_len, const unsigned char *sig);

/**@brief Function pointer to copy between two RSA contexts (clone).
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_copy.
 *
 * @param[out]  dst     Initialized destination context.
 * @param[in]   src     Initialized source context to clone.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_copy_fn)(mbedtls_rsa_context *dst, const mbedtls_rsa_context *src);


/**@brief Function pointer to free an RSA context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_rsa_free.
 *
 * param[in,out]    ctx     Pointer to the context to free.
 */
typedef void (*mbedtls_rsa_free_fn)(mbedtls_rsa_context *ctx);


/**@brief Structure type holding the RSA calling interface for a backend.
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    size_t backend_context_size;                                        //!< Size of the context according to the backend.
    mbedtls_rsa_check_fn check;                                         //!< Check for RSA support in the backend.
    mbedtls_rsa_init_fn init;                                           //!< Initialize an RSA context.
    mbedtls_rsa_import_fn import;                                       //!< Import core RSA parameters.
    mbedtls_rsa_import_raw_fn import_raw;                               //!< Import raw core RSA parameters.
    mbedtls_rsa_complete_fn complete;                                   //!< Complete an RSA context from imported parameters.
    mbedtls_rsa_export_fn export_fn;                                    //!< Export RSA context parameters.
    mbedtls_rsa_export_raw_fn export_raw;                               //!< Export RSA context parameters into raw buffers.
    mbedtls_rsa_export_crt_fn export_crt;                               //!< Export RSA context CRT parameters.
    mbedtls_rsa_set_padding_fn set_padding;                             //!< Set RSA padding and hash type.
    mbedtls_rsa_get_len_fn get_len;                                     //!< Get the length of the RSA modulus in bytes.
#if defined(MBEDTLS_GENPRIME)
    mbedtls_rsa_gen_key_fn gen_key;                                     //!< Generate an RSA key.
#endif /*MBEDTLS_GENPRIME*/
    mbedtls_rsa_check_pubkey_fn check_pubkey;                           //!< Check if RSA context contains a public key.
    mbedtls_rsa_check_privkey_fn check_privkey;                         //!< Check if the RSA context contains a private key.
    mbedtls_rsa_check_pub_priv_fn check_pub_priv;                       //!< Check an RSA private/public key pair.
    mbedtls_rsa_public_fn do_public;                                    //!< Perform an RSA public key operation.
    mbedtls_rsa_private_fn do_private;                                  //!< Perform an RSA private key operation.
    mbedtls_rsa_pkcs1_encrypt_fn pkcs1_encrypt;                         //!< Perform PKCS#1 encryption.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsaes_pkcs1_v15_encrypt_fn rsaes_pkcs1_v15_encrypt;     //!< Perform PKCS#1 v1.5 encryption.
#endif /* */
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsaes_oaep_encrypt_fn rsaes_oaep_encrypt;               //!< Perform PKCS#1 v2.1 encryption (RSAES OEAP).
#endif
    mbedtls_rsa_pkcs1_decrypt_fn pkcs1_decrypt;                         //!< Perform PKCS#1 decryption.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsaes_pkcs1_v15_decrypt_fn rsaes_pkcs1_v15_decrypt;     //!< Perform PKCS#1 v1.5 decryption.
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsaes_oaep_decrypt_fn rsaes_oaep_decrypt;               //!< Perform PKCS#1 v2.1 decryption (RSAES OAEP).
#endif
    mbedtls_rsa_pkcs1_sign_fn pkcs1_sign;                               //!< Perform PKCS#1 signature generation.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsassa_pkcs1_v15_sign_fn rsassa_pkcs1_v15_sign;         //!< Perform PKCS#1 v1.5 signature generation.
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsassa_pss_sign_fn rsassa_pss_sign;                     //!< Perform PKCS#1 v2.1 signature generation (RSASSA PSS).
#endif
    mbedtls_rsa_pkcs1_verify_fn pkcs1_verify;                           //!< Perform a PKCS#1 verify operation.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsassa_pkcs1_v15_verify_fn rsassa_pkcs1_v15_verify;     //!< Perform a PKCS#1 v1.5 verify operation.
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsassa_pss_verify_fn rsassa_pss_verify;                 //!< Perform a PKCS#1 v2.1 verify operation (RSASSA PSS).
    mbedtls_rsa_rsassa_pss_verify_ext_fn rsassa_pss_verify_ext;         //!< Perform PKCS#1 v2.1 verify operation (RSASSA PSS, extended functionality).
#endif
    mbedtls_rsa_copy_fn copy;                                           //!< Copy an RSA context (clone).
    mbedtls_rsa_free_fn free;                                           //!< Free an RSA context.
} mbedtls_rsa_funcs;

#endif /* MBEDTLS_AES_ALT */

#endif /* BACKEND_RSA_H */

/** @} */
