#ifndef BACKEND_ECDH_H
#define BACKEND_ECDH_H

#include "mbedtls/ecdh.h"

#define MBEDTLS_ECDH_GEN_PUBLIC_FUNCTION 0
#define MBEDTLS_ECDH_COMPUTE_SHARED_FUNCTION 1

typedef int (*mbedtls_ecdh_check_fn)(mbedtls_ecp_group *grp, int function);
typedef int (*mbedtls_ecdh_gen_public_fn)(mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);
typedef int (*mbedtls_ecdh_compute_shared_fn)(mbedtls_ecp_group *grp, mbedtls_mpi *z, const mbedtls_ecp_point *Q, const mbedtls_mpi *d, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


typedef struct
{
    mbedtls_ecdh_check_fn check;
    mbedtls_ecdh_gen_public_fn gen_public;
    mbedtls_ecdh_compute_shared_fn compute_shared;
} mbedtls_ecdh_funcs;


#endif // BACKEND_ECDH_H
