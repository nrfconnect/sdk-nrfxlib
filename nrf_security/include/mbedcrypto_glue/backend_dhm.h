/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_dhm mbedcrypto DHM glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief This is a glue layer for mbedcrypto DHM APIs, including typedefs for backend API abstraction.
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

/**@brief Function pointer to check if the backend supports DHM given
 *        size of a private key.
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the DHM mode. If the value is positive, then the backend with the
 *          highest value is selected (priority based).
 *
 * @param[in]   pbits       Private key bit size.
 *
 * @return 0 if the DHM functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_dhm_check_fn)(unsigned int pbits);


/**@brief Function pointer to initialize a glue DHM context.
 *
 * @details This function inits or resets the glue context for an DHM operation.
 * This function pointer has a signature equal to @c mbedtls_dhm_init.
 *
 * @param[in,out]       ctx         Pointer to the context to initialize.
 */
typedef void (*mbedtls_dhm_init_fn)(mbedtls_dhm_context *ctx);


/**@brief Function pointer to read DHM parameters from a server exchange.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_read_params.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in,out]       p           Sets pointer to end of data on input, start of the input buffer, and on output.
 * @param[in]           end         Pointer to the end of the input buffer.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_read_params_fn)(mbedtls_dhm_context *ctx, unsigned char **p, const unsigned char *end);


/**@brief Function pointer to generate DHM key pair exported to TLS format.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_make_params.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           x_size      Private key size in bytes.
 * @param[out]          output      Pointer to the destination buffer.
 * @param[out]          olen        Pointer to the variable to write the output length.
 * @param[in]           f_rng       RNG function.
 * @param[in,out]       p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_make_params_fn)(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);

/**@brief Function pointer to set prime modulus and generator in a DHM context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_set_group.
 *
 * @param[in,out]       ctx         Pointer to the context to set the prime modulus and generator.
 * @param[in]           P           Pointer to the MPI holding an initialized DHM prime modulus.
 * @param[in]           G           Pointer to the MPI holding an initialized DHM generator.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_set_group_fn)(mbedtls_dhm_context *ctx, const mbedtls_mpi *P, const mbedtls_mpi *G);


/**@brief Function pointer to import raw public value of a peer in DHM context.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_read_public.
 *
 * @note The DHM context must already be initialized and have DHM parameters set.
 *
 * @param[in,out]       ctx         Pointer to the context to set the public value.
 * @param[in]           input       Pointer to the input buffer containing the G^Y value of the peer.
 * @param[in]           ilen        Size of the input buffer in bytes.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_read_public_fn)(mbedtls_dhm_context *ctx, const unsigned char *input, size_t ilen);


/**@brief Function pointer to create a DHM key pair and export the raw public key in big-endian format.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_make_public.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           x_size      Private key in bytes.
 * @param[out]          output      Pointer to the array to hold the output.
 * @param[out]          olen        Pointer to a variable to write the output length.
 * @param[in]           f_rng       RNG function.
 * @param[in,out]       p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_make_public_fn)(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Function pointer to derive and export the shared secret.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_calc_secret.
 *
 * @param[in,out]       ctx         Pointer to an initialized context with private key generated and
 *                                  public key imported.
 * @param[out]          output      Pointer to the array to hold the output.
 * @param[in]           output_size Size of the destination buffer.
 * @param[out]          olen        Pointer to the variable to write the output length.
 * @param[in]           f_rng       RNG function.
 * @param[in,out]       p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_calc_secret_fn)(mbedtls_dhm_context *ctx, unsigned char *output, size_t output_size, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Function pointer to free a glue DHM context.
 *
 * @details This function frees the glue context for a DHM operation.
 * This function pointer has a signature equal to @c mbedtls_dhm_free.
 * 
 * @param[in,out]       ctx         DHM context to free.
 */
typedef void (*mbedtls_dhm_free_fn)(mbedtls_dhm_context *ctx);


/**@brief Function pointer to parse DHM parameters in PEM or DER format.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_parse_dhm.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * @param[in]           dhmin       Pointer to the array holding the input.
 * @param[in]           dhminlen    Size of the input buffer including null termination.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_parse_dhm_fn)(mbedtls_dhm_context *dhm, const unsigned char *dhmin, size_t dhminlen);


/**@brief Function pointer to parse DHM parameters from a file.
 *
 * @details This function pointer has a signature equal to @c mbedtls_dhm_parse_dhmfile.
 *
 * @param[in,out]       ctx         Pointer to the context for the operation.
 * param[in]            path        Path of the file to read DHM parameters from.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_dhm_parse_dhmfile_fn)(mbedtls_dhm_context *dhm, const char *path);


/**@brief Structure type holding the CMAC calling interface for a backend.
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    mbedtls_dhm_check_fn check;                 //!< Check for DHM support in the backend.
    mbedtls_dhm_init_fn init;                   //!< Initialize the DHM context.
    mbedtls_dhm_read_params_fn read_params;     //!< Read DHM parameters.
    mbedtls_dhm_make_params_fn make_params;     //!< Make DHM parameters.
    mbedtls_dhm_set_group_fn set_group;         //!< Set DHM group.
    mbedtls_dhm_read_public_fn read_public;     //!< Read DHM public key.
    mbedtls_dhm_make_public_fn make_public;     //!< Make DHM public key.
    mbedtls_dhm_calc_secret_fn calc_secret;     //!< Calculate a shared secret.
    mbedtls_dhm_free_fn free;                   //!< Free DHM context.
#if defined(MBEDTLS_ASN1_PARSE_C)
    mbedtls_dhm_parse_dhm_fn parse_dhm;         //!< Parse DHM parameters from PEM/DER.
#if defined(MBEDTLS_FS_IO)
    mbedtls_dhm_parse_dhmfile_fn parse_dhmfile; //!< Parse DHM parameters from a file.
#endif /* MBEDTLS_FS_IO */
#endif /* MBEDTLS_ASN1_PARSE_C */
} mbedtls_dhm_funcs;

#endif /* MBEDTLS_DHM_ALT */

#endif /* BACKEND_DHM_H */

/** @} */
