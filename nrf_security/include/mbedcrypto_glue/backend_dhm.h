#ifndef BACKEND_DHM_H
#define BACKEND_DHM_H

#include "mbedtls/dhm.h"

typedef int (*mbedtls_dhm_check_fn)(unsigned int pbits);
typedef void (*mbedtls_dhm_init_fn)(mbedtls_dhm_context *ctx);
typedef int (*mbedtls_dhm_read_params_fn)(mbedtls_dhm_context *ctx, unsigned char **p, const unsigned char *end);
typedef int (*mbedtls_dhm_make_params_fn)(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);
typedef int (*mbedtls_dhm_set_group_fn)(mbedtls_dhm_context *ctx, const mbedtls_mpi *P, const mbedtls_mpi *G);
typedef int (*mbedtls_dhm_read_public_fn)(mbedtls_dhm_context *ctx, const unsigned char *input, size_t ilen);
typedef int (*mbedtls_dhm_make_public_fn)(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);
typedef int (*mbedtls_dhm_calc_secret_fn)(mbedtls_dhm_context *ctx, unsigned char *output, size_t output_size, size_t *olen, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng);
typedef void (*mbedtls_dhm_free_fn)(mbedtls_dhm_context *ctx);
#if defined(MBEDTLS_ASN1_PARSE_C)
typedef int (*mbedtls_dhm_parse_dhm_fn)(mbedtls_dhm_context *dhm, const unsigned char *dhmin, size_t dhminlen);
#if defined(MBEDTLS_FS_IO)
typedef int (*mbedtls_dhm_parse_dhmfile_fn)(mbedtls_dhm_context *dhm, const char *path);
#endif
#endif


typedef struct
{
    mbedtls_dhm_check_fn check;
    mbedtls_dhm_init_fn init;
    mbedtls_dhm_read_params_fn read_params;
    mbedtls_dhm_make_params_fn make_params;
    mbedtls_dhm_set_group_fn set_group;
    mbedtls_dhm_read_public_fn read_public;
    mbedtls_dhm_make_public_fn make_public;
    mbedtls_dhm_calc_secret_fn calc_secret;
    mbedtls_dhm_free_fn free;
#if defined(MBEDTLS_ASN1_PARSE_C)
    mbedtls_dhm_parse_dhm_fn parse_dhm;
#if defined(MBEDTLS_FS_IO)
    mbedtls_dhm_parse_dhmfile_fn parse_dhmfile;
#endif
#endif
} mbedtls_dhm_funcs;


#endif // BACKEND_DHM_H
