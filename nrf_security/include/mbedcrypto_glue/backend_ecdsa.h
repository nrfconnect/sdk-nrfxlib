#ifndef BACKEND_ECDSA_H
#define BACKEND_ECDSA_H

#include "mbedtls/ecdsa.h"

#define MBEDTLS_ECDSA_SIGN_FUNCTION 0
#define MBEDTLS_ECDSA_VERIFY_FUNCTION 1
#define MBEDTLS_ECDSA_GENKEY_FUNCTION 2

typedef int (*mbedtls_ecdsa_check_fn)(mbedtls_ecp_group *grp, mbedtls_ecp_group_id gid, int function);
typedef int (*mbedtls_ecdsa_sign_fn)(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s, const mbedtls_mpi *d, const unsigned char *buf, size_t blen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);
typedef int (*mbedtls_ecdsa_verify_fn)(mbedtls_ecp_group *grp, const unsigned char *buf, size_t blen, const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s);
typedef int (*mbedtls_ecdsa_genkey_fn)(mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);


typedef struct
{
    mbedtls_ecdsa_check_fn check;
    mbedtls_ecdsa_sign_fn sign;
    mbedtls_ecdsa_verify_fn verify;
    mbedtls_ecdsa_genkey_fn genkey;
} mbedtls_ecdsa_funcs;


#endif // BACKEND_ECDSA_H
