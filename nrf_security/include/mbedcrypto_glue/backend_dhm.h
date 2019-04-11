/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_dhm mbedcrypto DHM glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief This is a glue layer for mbedcrypto DHM APIs.
 *
 * This includes typedefs for backend API abstraction.
 */
#ifndef BACKEND_DHM_H
#define BACKEND_DHM_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_DHM_ALT)

#include "mbedtls/dhm.h"

/**@brief Typedef of function pointer to check if the backend supports DHM given
 *        size of a private key
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the DHM mode. If the value is positive then the backend with the
 *          highest value will be selected (priority based).
 *
 * @param[in]   pbits       Private key bit size.
 *
 * @returns 0 if the DHM functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_dhm_check_fn)(unsigned int pbits);


/**@brief Typedef for function pointer to do a initialize a glue DHM context
 *
 * @details This function inits or resets the glue context for an DHM operation.
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_init.
 *
 * @param[in,out]       ctx         Pointer to context to initialize.
 */
typedef void (*mbedtls_dhm_init_fn)(mbedtls_dhm_context *ctx);


/**@brief Typedef for function pointer to read DHM parameters from server exchange
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_read_params.
 *
 * @param[in,out]       ctx         Pointer to context for the operation.
 * @param[in,out]       p           On input, start of the input buffer, on output, sets pointer to end of data.
 * @param[in]           end         Pointer to the end of the input buffer
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_read_params_fn)(mbedtls_dhm_context *ctx, unsigned char **p, const unsigned char *end);


/**@brief Typedef for function pointer to generate DHM key pair exported to TLS format
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_make_params.
 *
 * @param[in,out]       ctx         Pointer to context for the operation.
 * @param[in]           x_size      Private key size in bytes.
 * @param[out]          output      Pointer to the destination buffer.
 * @param[out]          olen        Pointer to variable to write the output length.
 * @param[in]           f_rng       The RNG function.
 * @param[in,out]       p_rng       The RNG context.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_make_params_fn)(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);

/**@brief Typedef for function pointer to set prime modulus and generator in a DHM context
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_set_group.
 *
 * @param[in,out]       ctx         Pointer to context to set the prime modulus and generator.
 * @param[in]           P           Pointer to MPI holding an initialized DHM prime modulus.
 * @param[in]           G           Pointer to MPI holding an initialized DHM generator.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_set_group_fn)(mbedtls_dhm_context *ctx, const mbedtls_mpi *P, const mbedtls_mpi *G);


/**@brief Typedef for function pointer to import raw public value of peer in DHM context
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_read_public.
 *
 * @note The DHM context must already be initialized and have DHM parameters set.
 *
 * @param[in,out]       ctx         Pointer to context to set the public value.
 * @param[in]           input       Pointer to the input buffer containing the G^Y value of the peer.
 * @param[in]           ilen        The size of the input buffer in bytes.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_read_public_fn)(mbedtls_dhm_context *ctx, const unsigned char *input, size_t ilen);


/**@brief Typedef for function pointer to create DHM key pair and export the raw public key in big-endian format
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_make_public.
 *
 * @param[in,out]       ctx         Pointer to context for the operation.
 * @param[in]           x_size      The private key in bytes.
 * @param[out]          output      Pointer to an array to hold the output.
 * @param[out]          olen        Pointer to a variable to write the output length.
 * @param[in]           f_rng       The RNG function.
 * @param[in,out]       p_rng       The RNG context.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_make_public_fn)(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Typedef for function pointer to derive and export the shared secret
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_calc_secret.
 *
 * @param[in,out]       ctx         Pointer to an initialized context with private key generated and
 *                                  public key imported.
 * @param[out]          output      Pointer to an array to hold the output.
 * @param[in]           output_size Size of the destination buffer.
 * @param[out]          olen        Pointer to variable to write the output length.
 * @param[in]           f_rng       The RNG function.
 * @param[in,out]       p_rng       The RNG context.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_calc_secret_fn)(mbedtls_dhm_context *ctx, unsigned char *output, size_t output_size, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Typedef for function pointer to free a glue DHM context.
 *
 * @details This function frees the glue context for a DHM operation.
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_free.
 *
 * @param[in,out]       ctx         The DHM Context to free.
 */
typedef void (*mbedtls_dhm_free_fn)(mbedtls_dhm_context *ctx);


/**@brief Typedef for function pointer to parse DHM parameters in PEM or DER format
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_parse_dhm.
 *
 * @param[in,out]       ctx         Pointer to context for the operation.
 * @param[in]           dhmin       Pointer to an array holding the input.
 * @param[in]           dhminlen    The size of the input buffer including null termination.
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_parse_dhm_fn)(mbedtls_dhm_context *dhm, const unsigned char *dhmin, size_t dhminlen);


/**@brief Typedef for function pointer to parse DHM parameters from a file
 *
 * This function pointer has a signature equal to @c mbedtls_dhm_parse_dhmfile.
 *
 * @param[in,out]       ctx         Pointer to context for the operation.
 * param[in]            path        Path of the filename to read DHM parameters from
 *
 * @returns 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_parse_dhmfile_fn)(mbedtls_dhm_context *dhm, const char *path);


/**@brief Typedef for structure type holding the CMAC calling interface for a backend
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    mbedtls_dhm_check_fn check;                 //!< Function to check for DHM support in the backend.
    mbedtls_dhm_init_fn init;                   //!< Function to initialize the DHM context.
    mbedtls_dhm_read_params_fn read_params;     //!< Function to read DHM params.
    mbedtls_dhm_make_params_fn make_params;     //!< Function to make DHM params.
    mbedtls_dhm_set_group_fn set_group;         //!< Function to set DHM group.
    mbedtls_dhm_read_public_fn read_public;     //!< Function to read DHM public key.
    mbedtls_dhm_make_public_fn make_public;     //!< Function to make DHM public key.
    mbedtls_dhm_calc_secret_fn calc_secret;     //!< Function to calculate a shared secret.
    mbedtls_dhm_free_fn free;                   //!< Function to free the DHM context.
#if defined(MBEDTLS_ASN1_PARSE_C)
    mbedtls_dhm_parse_dhm_fn parse_dhm;         //!< Function to parse DHM from PEM/DER.
#if defined(MBEDTLS_FS_IO)
    mbedtls_dhm_parse_dhmfile_fn parse_dhmfile; //!< Function to parse DHM parameters from a file.
#endif /* MBEDTLS_FS_IO */
#endif /* MBEDTLS_ASN1_PARSE_C */
} mbedtls_dhm_funcs;

#endif /* MBEDTLS_DHM_ALT */

#endif /* BACKEND_DHM_H */

/** @} */
