/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_rsa mbedcrypto RSA glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief This is a glue layer for mbedcrypto RSA APIs.
 *
 * This includes typedefs for backend API abstraction.
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

/**@brief Typedef of function pointer to check if the backend support RSA given
 *        padding type, hash-id and modulus bit size
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the RSA cipher. If the value is positive then the backend with the
 *          highest value will be selected (priority based).
 *
 * @param[in]       padding     Reserved type, not in current use.
 * @param[in]       hash_id     Reserved type, not in current use.
 * @param[in]       nbits       Modulus bit size.
 *
 * @returns 0 if the RSA functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_rsa_check_fn)(int padding, int hash_id, unsigned int nbits);


/**@brief Typedef for function pointer to initialize an mbedcrypto glue RSA context
 *
 * @details This function inits or resets the glue context for an RSA operation.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_init.
 *
 * @param[in,out]       ctx         Pointer to the context to initialize.
 * @param[in]           padding     Either MBEDTLS_RSA_PKCS_V21 or MBEDTLS_RSA_PKCS_V15.
 * @param[in]           hash_id     Hash identifier.
 */
typedef void (*mbedtls_rsa_init_fn)(mbedtls_rsa_context *ctx, int padding, int hash_id);


/**@brief Typedef for function pointer to import core RSA parameters into a glue RSA context.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_import.
 *
 * @param[in,out]       ctx         Pointer to the context, must be initialized.
 * @param[in]           N           Pointer to the RSA modulus, may be NULL.
 * @param[in]           P           Pointer to the first RSA prime factor, may be NULL.
 * @param[in]           Q           Pointer to the second RSA prime factor, may be NULL.
 * @param[in]           D           Pointer to the private exponent, may be NULL.
 * @param[in]           E           Pointer to the public exponent, may be NULL.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_import_fn)(mbedtls_rsa_context *ctx, const mbedtls_mpi *N, const mbedtls_mpi *P, const mbedtls_mpi *Q, const mbedtls_mpi *D, const mbedtls_mpi *E);


/**@brief Typedef for function pointer to import core RSA parameters into a glue RSA context
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_import_raw.
 *
 * @note The raw input is expected to be in big-endian format.
 *
 * @param[in,out]       ctx         Pointer to the context, must be initialized.
 * @param[in]           N           Pointer to an array holding the RSA modulus, may be NULL.
 * @param[in]           N_len       The length of N, ignored if N is NULL.
 * @param[in]           P           Pointer to an array holding the first RSA prime factor, may be NULL.
 * @param[in]           P_len       The length of P, ignored if P is NULL.
 * @param[in]           Q           Pointer to an array holding the second RSA prime factor, may be NULL.
 * @param[in]           Q_len       The length of Q, ignored if Q is NULL.
 * @param[in]           D           Pointer to an array holding the private exponent, may be NULL.
 * @param[in]           D_len       The length of D, ignored if D is NULL.
 * @param[in]           E           Pointer to an array holding the public exponent, may be NULL.
 * @param[in]           E_len       The length of E, ignored if E is NULL.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_import_raw_fn)(mbedtls_rsa_context *ctx, unsigned char const *N, size_t N_len, unsigned char const *P, size_t P_len, unsigned char const *Q, size_t Q_len, unsigned char const *D, size_t D_len, unsigned char const *E, size_t E_len);


/**@brief Typedef for function pointer to complete an RSA context from a set of imported core parameters.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_complete.
 *
 * @param[in,out]       ctx         The RSA context, initialized and holding the imported parameters.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_complete_fn)(mbedtls_rsa_context *ctx);


/**@brief Typedef for function pointer to export an RSA context
 *
 * This function pointer has a signature equal to mbedtls_rsa_export.
 *
 * @param[in]           ctx         Pointer to the context, initialized and holding core parameters.
 * @param[out]          N           Pointer to an MPI to hold the RSA modulus, or NULL if not to export.
 * @param[out]          P           Pointer to an MPI to hold the first prime factor, or NULL if not to export.
 * @param[out]          Q           Pointer to an MPI to hold the second prime factor, or NULL if not to export.
 * @param[out]          D           Pointer to an MPI to hold the private exponent, or NULL if not to export.
 * @param[out]          Q           Pointer to an MPI to hold the public exponent, or NULL if not to export.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_export_fn)(const mbedtls_rsa_context *ctx, mbedtls_mpi *N, mbedtls_mpi *P, mbedtls_mpi *Q, mbedtls_mpi *D, mbedtls_mpi *E);


/**@brief Typedef for function pointer to export an RSA context to raw form
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_export_raw.
 *
 * @note The raw output is in big-endian format.
 *
 * @param[in]           ctx         Pointer to the context, initialized and holding core parameters.
 * @param[out]          N           Pointer to an array to hold the RSA modulus, or NULL if not to export.
 * @param[out]          N_len       The length of N, ignored if N is NULL.
 * @param[out]          P           Pointer to an array to hold the first prime factor, or NULL if not to export.
 * @param[out]          P_len       The length of P, ignored if P is NULL.
 * @param[out]          Q           Pointer to an array to hold the second prime factor, or NULL if not to export.
 * @param[out]          Q_len       The length of Q, ignored if Q is NULL.
 * @param[out]          D           Pointer to an array to hold the private exponent, or NULL if not to export.
 * @param[out]          D_len       The length of D, ignored if D is NULL.
 * @param[out]          E           Pointer to an array to hold the public exponent, or NULL if not to export.
 * @param[out]          E_len       The length of E, ignored if E is NULL.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_export_raw_fn)(const mbedtls_rsa_context *ctx, unsigned char *N, size_t N_len, unsigned char *P, size_t P_len, unsigned char *Q, size_t Q_len, unsigned char *D, size_t D_len, unsigned char *E, size_t E_len);


/**@brief Typedef for function pointer to export RSA CRT parameters of a private RSA key.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_export_crt.
 *
 * @param[in]           ctx         Pointer to the context, initialized and holding core parameters.
 * @param[out]          DP          Pointer to an MPI to hold D modulo P-1, or NULL if not to export.
 * @param[out]          DQ          Pointer to an MPI to hold D modulo Q-1, or NULL if not to export.
 * @param[out]          QP          Pointer to an MPI to hold modular inverse of Q modulo P, or NULL if not to export.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_export_crt_fn)(const mbedtls_rsa_context *ctx, mbedtls_mpi *DP, mbedtls_mpi *DQ, mbedtls_mpi *QP);


/**@brief Typedef for function pointer to set RSA padding and hash type
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_set_padding.
 *
 * @param[in,out]       ctx         Pointer to the context to update.
 * @param[in]           padding     Either MBEDTLS_RSA_PKCS_V21 or MBEDTLS_RSA_PKCS_V15.
 * @param[in]           hash_id     Hash identifier.
 */
typedef void (*mbedtls_rsa_set_padding_fn)(mbedtls_rsa_context *ctx, int padding, int hash_id);


/**@brief Typedef for function pointer to get the length of the RSA modulus in bytes
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_get_len.
 *
 * @param[in]           ctx         Pointer to the context, must be initialized and configured.
 *
 * @returns The length of the RSA modulus in bytes.
 */
typedef size_t (*mbedtls_rsa_get_len_fn)(const mbedtls_rsa_context *ctx);


/**@brief Typedef for function pointer to generate an RSA key
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_gen_key.
 *
 * @param[in,out]       ctx         Pointer to an initialized context, used to hold the private key after call.
 * @param[in]           f_rng       The RNG function.
 * @param[in,out]       p_rng       The RNG context.
 * @param[in]           nbits       The size of the public key in bits.
 * @param[in]           exponent    The public exponent for use (e.g. 65537)
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_gen_key_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, unsigned int nbits, int exponent);


/**@brief Typedef for function pointer to check if an RSA context contains a public key
 *
 * @details A successful check of this functions means that the context can be use for
 *          RSA public key operations.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_check_pubkey.
 *
 * @param[in]           ctx         Pointer to an initialized context to check.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_check_pubkey_fn)(const mbedtls_rsa_context *ctx);


/**@brief Typedef for function to check if an RSA context contains a private key
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_check_privkey.
 *
 * @param[in]           ctx         Pointer to an initialized context to check.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_check_privkey_fn)(const mbedtls_rsa_context *ctx);


/**@brief Typedef for function to check an RSA public/private keypair.
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_check_pub_priv.
 *
 * @param[in]       pub     Pointer to a context holding an RSA public key.
 * @param[in]       priv    Pointer to a context holding an RSA private key.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_check_pub_priv_fn)(const mbedtls_rsa_context *pub, const mbedtls_rsa_context *prv);


/**@brief Typedef for function to do an RSA public key operation
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_public.
 *
 * @note The size of the input and output buffer must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialised context.
 * @param[in]       input   Pointer to a buffer holding the input.
 * @param[out]      output  Pointer to a buffer to hold the output.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_public_fn)(mbedtls_rsa_context *ctx, const unsigned char *input, unsigned char *output);


/**@brief Typedef for function to do an RSA private key operation
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_private.
 *
 * @note The size of the input and output buffer must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized RSA context.
 * @param[in]       f_rng   The RNG function.
 * @param[in,out]   p_rng   The RNG context.
 * @param[in]       input   Pointer to a buffer holding the input.
 * @param[out]      output  Pointer to a buffer to hold the output.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_private_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, const unsigned char *input, unsigned char *output);


/**@brief Typedef for function to do a PKCS#1 encryption
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_encrypt.
 *
 * @note The size of the input and output buffer must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized RSA context.
 * @param[in]       f_rng   The RNG function.
 * @param[in,out]   p_rng   The RNG context.
 * @param[in]       mode    The mode of operation.
 * @param[in]       input   Pointer to a buffer holding the input.
 * @param[out]      output  Pointer to a buffer to hold the output.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output);


/**@brief Typedef for function to do a PKCS#1 v1.5 encryption
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsaes_pkcs1_v15_encrypt.
 *
 * @note The size of the output buffer must correspond to ctx->len bytes.
 *       For 2048-bit RSA, this means 256 bytes.
 *
 * @param[in]       ctx     Pointer to an initialized RSA context.
 * @param[in]       f_rng   The RNG function.
 * @param[in,out]   p_rng   The RNG context.
 * @param[in]       mode    The mode of operation.
 * @param[in]       ilen    The length of the plaintext in bytes.
 * @param[in]       input   Pointer to a buffer holding the input in plaintext.
 * @param[out]      output  Pointer to a buffer to hold the encrypted output.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_pkcs1_v15_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output);


/**@brief Typedef for function to do a PKCS1 v2.1 encryption (RSAES OAEP)
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsaes_oaep_encrypt.
 *
 * @param[in]       ctx         Pointer to an initialized RSA context.
 * @param[in]       f_rng       The RNG function.
 * @param[in,out]   p_rng       The RNG context.
 * @param[in]       mode        The mode of operation.
 * @param[in]       label       The buffer holding the custom label to use, or NULL if not used.
 * @param[in]       label_len   The length of the label, skipped if label is NULL.
 * @param[in]       ilen        Length of the plaintext in bytes
 * @param[in]       input       Pointer to a buffer holding the input in plaintext.
 * @param[out]      output      Pointer to a buffer to hold the encrypted output.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_oaep_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t ilen, const unsigned char *input, unsigned char *output);


/**@brief Typedef for function to do a PKCS#1 decryption
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_decrypt.
 *
 * @note This function will remove message padding. The output buffer
 *       should be ctx->len in size (e.g. 256 bytes for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[out]      olen            Pointer to a variable to hold the length of the decrypted plaintext in bytes.
 * @param[in]       input           Pointer to a buffer holding the input in encrypted form.
 * @param[out]      output          Pointer to a buffer to hold the output in plaintext.
 * @param[in]       output_max_len  Max length available to hold the decrypted plaintext.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);


/**@brief Typedef for function pointer to do a PKCS#1 v1.5 decryption
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsaes_pkcs1_v15_decrypt.
 *
 * @note The output buffer should be ctx->len in size (e.g. 256 bytes
 *       for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[out]      olen            Pointer to a variable to hold the length of the decrypted plaintext in bytes.
 * @param[in]       input           Pointer to a buffer holding the input in encrypted form.
 * @param[out]      output          Pointer to a buffer to hold the output in plaintext.
 * @param[in]       output_max_len  Max length available to hold the decrypted plaintext.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_pkcs1_v15_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);


/**@brief Typedef for function pointer to do a PKCS#1 v2.1 decryption (RSAES OAEP)
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsaes_oaep_decrypt.
 *
 * @note The output buffer should be ctx->len in size (e.g. 256 bytes
 *       for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       label           The buffer holding the custom label to use, or NULL if not used
 * @param[in]       label_len       The length of the label. Set to 0 if custom label isn't used.
 * @param[in]       olen            Pointer to a variable holding the length of the decrypted plaintext in bytes.
 * @param[in]       input           Pointer to a buffer holding the input in encrypted form.
 * @param[out]      output          Pointer to a buffer to hold the output in plaintext.
 * @param[in]       output_max_len  Max length available to hold the decrypted plaintext.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsaes_oaep_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);


/**@brief Typedef for function pointer to do PKCS#1 signature generation
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_sign.
 *
 * @note The signature buffer should be ctx->len in size (e.g. 256 bytes
 *       for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       md_alg          The message digest algorithm used to hash the data.
 * @param[in]       hashlen         The length of the message digest.
 * @param[in]       hash            Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[out]      sig             Pointer to a buffer to hold the signature
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);


/**@brief Typedef for function pointer to do PKCS#1 v1.5 signature generation
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsaassa_pkcs1_v15_sign.
 *
 * @note The signature buffer should be ctx->len in size (e.g. 256 bytes
 *       for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       md_alg          The message digest algorithm used to hash the data.
 * @param[in]       hashlen         The length of the message digest.
 * @param[in]       hash            Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[out]      sig             Pointer to a buffer to hold the signature.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pkcs1_v15_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);


/**@brief Typedef for function pointer to do PKCS#1 v2.1 signature generation (RSA-PSS)
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pss_sign.
 *
 * @note The signature buffer should be ctx->len in size (e.g. 256 bytes
 *       for 2048-bit RSA)
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       md_alg          The message digest algorithm used to hash the data.
 * @param[in]       hashlen         The length of the message digest.
 * @param[in]       hash            Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[out]      sig             Pointer to a buffer to hold the signature.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pss_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);


/**@brief Typedef for function pointer to do PKCS#1 signature verification
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_pkcs1_verify.
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       md_alg          The message digest algorithm used to hash the data.
 * @param[in]       hashlen         The length of the message digest.
 * @param[in]       hash            Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[in]       sig             Pointer to a buffer holding the signature
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_pkcs1_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);


/**@brief Typedef for function pointer to do PKCS#1 v1.5 signature verification
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pkcs1_v15_verify
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       md_alg          The message digest algorithm used to hash the data.
 * @param[in]       hashlen         The length of the message digest.
 * @param[in]       hash            Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[in]       sig             Pointer to a buffer holding the signature
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pkcs1_v15_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);


/**@brief Typedef for function pointer to do PKCS#1 v2.1 signature verification (RSA-PSS)
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pss_verify.
 *
 * @param[in]       ctx             Pointer to an initialized RSA context.
 * @param[in]       f_rng           The RNG function.
 * @param[in,out]   p_rng           The RNG context.
 * @param[in]       mode            The mode of operation.
 * @param[in]       md_alg          The message digest algorithm used to hash the data.
 * @param[in]       hashlen         The length of the message digest.
 * @param[in]       hash            Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[in]       sig             Pointer to a buffer holding the signature.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pss_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);


/**@brief Typedef for function pointer to do PKCS#1 v2.1 signature verification (RSA-PSS extended features)
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_rsassa_pss_verify.
 *
 * @param[in]       ctx                 Pointer to an initialized RSA context.
 * @param[in]       f_rng               The RNG function.
 * @param[in,out]   p_rng               The RNG context.
 * @param[in]       mode                The mode of operation.
 * @param[in]       md_alg              The message digest algorithm used to hash the data.
 * @param[in]       hashlen             The length of the message digest.
 * @param[in]       hash                Pointer to a buffer holding the digest or raw data (if MBEDTLS_MD_NONE is used).
 * @param[in]       mgf1_hash_id        The message digest used for mask generation.
 * @param[in]       expected_salt_len   The length of the salt used in padding or MBEDTLS_RSA_SALT_LEN_ANY to accept all lengths.
 * @param[out]      sig                 Pointer to a buffer holding the signature.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_rsassa_pss_verify_ext_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, mbedtls_md_type_t mgf1_hash_id, int expected_salt_len, const unsigned char *sig);

/**@brief Typedef for function pointer to copy between two RSA contexts (clone)
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_copy.
 *
 * @param[out]  dst     The initialized destination context.
 * @param[in]   src     The initialized source context to clone.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_rsa_copy_fn)(mbedtls_rsa_context *dst, const mbedtls_rsa_context *src);


/**@brief Typedef for function pointer to free an RSA context
 *
 * This function pointer has a signature equal to @c mbedtls_rsa_free.
 *
 * param[in,out]    ctx     Pointer to context to free.
 */
typedef void (*mbedtls_rsa_free_fn)(mbedtls_rsa_context *ctx);


/**@brief Typedef for structure type holding the RSA calling interface for a backend
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    size_t backend_context_size;                                        //!< Size of the context according to the backend.
    mbedtls_rsa_check_fn check;                                         //!< Function to check for RSA support in the backend.
    mbedtls_rsa_init_fn init;                                           //!< Function to initialize the RSA context.
    mbedtls_rsa_import_fn import;                                       //!< Function to import core RSA parameters.
    mbedtls_rsa_import_raw_fn import_raw;                               //!< Function to import raw core RSA parameters.
    mbedtls_rsa_complete_fn complete;                                   //!< Function to complete an RSA context from imported parameters.
    mbedtls_rsa_export_fn export_fn;                                    //!< Function to export an RSA context parameters.
    mbedtls_rsa_export_raw_fn export_raw;                               //!< Function to export an RSA context parameters into raw buffers.
    mbedtls_rsa_export_crt_fn export_crt;                               //!< Function to export an RSA context CRT parameters.
    mbedtls_rsa_set_padding_fn set_padding;                             //!< Function to set RSA padding and hash type.
    mbedtls_rsa_get_len_fn get_len;                                     //!< Function to get the length of the RSA modulus in bytes.
#if defined(MBEDTLS_GENPRIME)
    mbedtls_rsa_gen_key_fn gen_key;                                     //!< Function to generate an RSA key.
#endif /*MBEDTLS_GENPRIME*/
    mbedtls_rsa_check_pubkey_fn check_pubkey;                           //!< Function to check if RSA context contains a public key.
    mbedtls_rsa_check_privkey_fn check_privkey;                         //!< Function to check if the RSA context contains a private key.
    mbedtls_rsa_check_pub_priv_fn check_pub_priv;                       //!< Function to check an RSA private/public keypair.
    mbedtls_rsa_public_fn do_public;                                    //!< Function to do an RSA public key operation.
    mbedtls_rsa_private_fn do_private;                                  //!< Function to do an RSA private key operation.
    mbedtls_rsa_pkcs1_encrypt_fn pkcs1_encrypt;                         //!< Function to do a PKCS#1 encryption.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsaes_pkcs1_v15_encrypt_fn rsaes_pkcs1_v15_encrypt;     //!< Function to do a PKCS#1 v1.5 encryption.
#endif /* */
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsaes_oaep_encrypt_fn rsaes_oaep_encrypt;               //!< Function to do a PKCS#1 v2.1 encryption (RSAES OEAP).
#endif
    mbedtls_rsa_pkcs1_decrypt_fn pkcs1_decrypt;                         //!< Function to do a PKCS#1 decryption.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsaes_pkcs1_v15_decrypt_fn rsaes_pkcs1_v15_decrypt;     //!< Function to do a PKCS#1 v1.5 decryption.
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsaes_oaep_decrypt_fn rsaes_oaep_decrypt;               //!< Function to do a PKCS#1 v2.1 decryption (RSAES OAEP)
#endif
    mbedtls_rsa_pkcs1_sign_fn pkcs1_sign;                               //!< Function to do a PKCS#1 signature generation
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsassa_pkcs1_v15_sign_fn rsassa_pkcs1_v15_sign;         //!< Function to do a PKCS#1 v1.5 signature generation
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsassa_pss_sign_fn rsassa_pss_sign;                     //!< Function to do a PKCS#1 v2.1 signature generation (RSASSA PSS).
#endif
    mbedtls_rsa_pkcs1_verify_fn pkcs1_verify;                           //!< Function to do a PKCS#1 verify operation.
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsassa_pkcs1_v15_verify_fn rsassa_pkcs1_v15_verify;     //!< Function to do a PKCS#1 v1.5 verify operation.
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsassa_pss_verify_fn rsassa_pss_verify;                 //!< Function to do a PKCS#1 v2.1 verify (RSASSA PSS).
    mbedtls_rsa_rsassa_pss_verify_ext_fn rsassa_pss_verify_ext;         //!< Function to do a PKCS#1 v2.1 verify (RSASSA PSS, extended functionality).
#endif
    mbedtls_rsa_copy_fn copy;                                           //!< Function to copy an RSA context (clone).
    mbedtls_rsa_free_fn free;                                           //!< Function to free an RSA context.
} mbedtls_rsa_funcs;

#endif /* MBEDTLS_AES_ALT */

#endif /* BACKEND_RSA_H */

/** @} */
