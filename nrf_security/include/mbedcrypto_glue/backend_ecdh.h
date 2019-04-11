/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup mbedcrypto_glue_ecdh mbedcrypto ECDH glue
 * @ingroup mbedcrypto_glue
 * @{
 * @brief Glue layer for mbedcrypto ECDH APIs, including typedefs for backend API abstraction.
 */
#ifndef BACKEND_ECDH_H
#define BACKEND_ECDH_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/ecdh.h"

#define MBEDTLS_ECDH_GEN_PUBLIC_FUNCTION        0           //!< Macro definition to identify an ECDH generate public functionality.
#define MBEDTLS_ECDH_COMPUTE_SHARED_FUNCTION    1           //!< Macro definition to identify an ECDH compute shared secret functionality.

/**@brief Function pointer to check if the backend supports ECDH, depending on the given ECP group.
 *
 * @details The value returned by the backend implementing this function pointer is
 *          dynamically checked. If the return value is 0, then the backend does not
 *          support the ECC curve. If the value is positive, then the backend with the
 *          highest value is selected (priority based).
 *
 * @param[in]       grp         ECP group.
 * @param[in]       function    Reserved type, currently not in use.
 *
 * @return 0 if the ECDH functionality is not supported, otherwise a priority where higher is better.
 */
typedef int (*mbedtls_ecdh_check_fn)(mbedtls_ecp_group *grp, int function);


/**@brief Function pointer to generate an ECDH key pair.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ecdh_gen_public.
 *
 * @param[in]       grp         Pointer to the MPI of the ECP group.
 * @param[out]      d           Pointer to the destination MPI for private key. Must be initialized.
 * @param[out]      Q           Pointer to the destination public key. Must be initialized.
 * @param[in]       f_rng       RNG function.
 * @param[in,out]   p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ecdh_gen_public_fn)(mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Function pointer to generate an ECDH shared secret.
 *
 * @details This function pointer has a signature equal to @c mbedtls_ecdh_compute_shared.
 *
 * @note Note that @p f_rng may be NULL if randomization of intermediate result is not needed.
 *
 * @param[in]       grp         Pointer to the MPI of the ECP group.
 * @param[out]      z           Pointer to the destination MPI for shared secret. Must be initialized.
 * @param[in]       Q           Pointer to a public key from another party. Must be initialized.
 * @param[in]       d           Pointer to a local private key. Must be initialized.
 * @param[in]       f_rng       RNG function.
 * @param[in,out]   p_rng       RNG context.
 *
 * @return 0 if operation was successful, otherwise a negative value corresponding to the error.
 */
typedef int (*mbedtls_ecdh_compute_shared_fn)(mbedtls_ecp_group *grp, mbedtls_mpi *z, const mbedtls_ecp_point *Q, const mbedtls_mpi *d, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


/**@brief Structure type holding the ECDH calling interface for a backend.
 *
 * @details The backend must provide an instance of this structure to
 *          enable mbedcrypto glue functionality.
 */
typedef struct
{
    mbedtls_ecdh_check_fn check;                    //!< Check for ECDH support in the backend.
    mbedtls_ecdh_gen_public_fn gen_public;          //!< Generate ECDH key pair.
    mbedtls_ecdh_compute_shared_fn compute_shared;  //!< Calculate ECDH shared secret.
} mbedtls_ecdh_funcs;

#endif /* BACKEND_ECDH_H */

/** @} */