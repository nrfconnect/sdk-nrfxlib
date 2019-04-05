#ifndef BACKEND_CMAC_H
#define BACKEND_CMAC_H

#include "mbedtls/cmac.h"

typedef int (*mbedtls_cipher_cmac_check_fn)(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keybits);
typedef int (*mbedtls_cipher_cmac_starts_fn)(mbedtls_cipher_context_t *ctx , const unsigned char *key, size_t keybits);
typedef int (*mbedtls_cipher_cmac_update_fn)(mbedtls_cipher_context_t *ctx , const unsigned char *input, size_t ilen);
typedef int (*mbedtls_cipher_cmac_finish_fn)(mbedtls_cipher_context_t *ctx , unsigned char *output);
typedef int (*mbedtls_cipher_cmac_reset_fn)(mbedtls_cipher_context_t *ctx);
typedef void (*mbedtls_cipher_cmac_free_fn)(mbedtls_cipher_context_t *ctx);
typedef int (*mbedtls_cipher_cmac_fn)(const mbedtls_cipher_info_t *cipher_info , const unsigned char *key, size_t keylen , const unsigned char *input, size_t ilen , unsigned char *output);
#if defined(MBEDTLS_AES_C)
typedef int (*mbedtls_aes_cmac_prf_128_fn)(const unsigned char *key, size_t key_len , const unsigned char *input, size_t in_len , unsigned char output[16]);
#endif /* MBEDTLS_AES_C */


typedef struct
{
    mbedtls_cipher_cmac_check_fn check;
    mbedtls_cipher_cmac_starts_fn starts;
    mbedtls_cipher_cmac_update_fn update;
    mbedtls_cipher_cmac_finish_fn finish;
    mbedtls_cipher_cmac_reset_fn reset;
    mbedtls_cipher_cmac_free_fn free;
    mbedtls_cipher_cmac_fn cmac;
#if defined(MBEDTLS_AES_C)
    mbedtls_aes_cmac_prf_128_fn aes_prf_128;
#endif /* MBEDTLS_AES_C */
} mbedtls_cmac_funcs;


#endif // BACKEND_CMAC_H
