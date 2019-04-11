/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_ecdsa mbedcrypto ECDSA glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Glue layer for mbedcrypto ECDSA APIs, including typedefs for backend API abstraction.
 */
#ifndef BACKEND_ECDSA_H
#define BACKEND_ECDSA_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/ecdsa.h"

#define MBEDTLS_ECDSA_SIGN_FUNCTION     0       //!< Macro definition to identify an ECDSA sign function.
#define MBEDTLS_ECDSA_VERIFY_FUNCTION   1       //!< Macro definition to identify an ECDSA verify function.
#define MBEDTLS_ECDSA_GENKEY_FUNCTION   2       //!< Macro definition to identify an ECDSA generate key pair function.

/**@brief Function pointer to check if the backend supports ECDSA, depending on the given group and curve type.
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the ECC curve. If the value is positive, then the backend with the
 *          highest value is selected (priority based).
 *
 * @param[in]       grp         ECP group. If NULL, @p gid is used instead.
 * @param[in]       gid         Curve type, might be overridden by @p grp.
 * @param[in]       function    Reserved type, currently not in use.
 *
 * @return 0 if the ECDSA functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_ecdsa_check_fn)(mbedtls_ecp_group *grp, mbedtls_ecp_group_id gid, int function);


/**@brief Function pointer to sign a hash using ECDSA.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ecdsa_sign.
 *
 * @param[in]       grp         Pointer to an ECP group.
 * @param[out]      r           Pointer to the MPI context to store the first part of the signature. Must be initialized.
 * @param[out]      s           Pointer to the MPI context to store the second part of the signature. Must be initialized.
 * @param[in]       buf         Pointer to the buffer holding the hash to be signed.
 * @param[in]       blen        Length of the buffer to sign.
 * @param[in]       f_rng       RNG function.
 * @param[in,out]   p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ecdsa_sign_fn)(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s, const mbedtls_mpi *d, const unsigned char *buf, size_t blen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Function pointer to verify a hash using ECDSA.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ecdsa_verify.
 *
 * @param[in]       grp         Pointer to an ECP group.
 * @param[in]       buf         Pointer to the buffer holding the hash to be verified.
 * @param[in]       blen        Length of the buffer to verify.
 * @param[in]       Q           Pointer to the ECP point used for verification (the public key).
 * @param[in]       r           Pointer to the MPI holding the first part of the signature. Must be initialized.
 * @param[in]       s           Pointer to the MPI holding the second part of the signature. Must be initialized.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ecdsa_verify_fn)(mbedtls_ecp_group *grp, const unsigned char *buf, size_t blen, const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s);


/**@brief Function pointer to generate an ECDSA key pair.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ecdsa_genkey.
 *
 * @param[in,out]   ctx         Pointer to the ECDSA context to store the keypair in. Must be initialized.
 * @param[in]       gid         Curve type.
 * @param[in]       f_rng       RNG function.
 * @param[in,out]   p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ecdsa_genkey_fn)(mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Typedef for structure type holding the ECDSA calling interface for a backend.
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    mbedtls_ecdsa_check_fn check;           //!< Check for ECDSA support in the backend.
    mbedtls_ecdsa_sign_fn sign;             //!< Create a signature using ECDSA.
    mbedtls_ecdsa_verify_fn verify;         //!< Verify a signature using ECDSA.
    mbedtls_ecdsa_genkey_fn genkey;         //!< Generate ECDSA key pair.
} mbedtls_ecdsa_funcs;


#endif /* BACKEND_ECDSA_H */

/** @} */
