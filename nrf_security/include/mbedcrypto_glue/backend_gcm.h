#ifndef BACKEND_GCM_H
#define BACKEND_GCM_H

#include "mbedtls/gcm.h"

typedef int (*mbedtls_gcm_check_fn)(mbedtls_cipher_id_t cipher , const unsigned char *key , unsigned int keybits);
typedef void (*mbedtls_gcm_init_fn)(mbedtls_gcm_context *ctx);
typedef int (*mbedtls_gcm_setkey_fn)(mbedtls_gcm_context *ctx , mbedtls_cipher_id_t cipher , const unsigned char *key , unsigned int keybits);
typedef int (*mbedtls_gcm_crypt_and_tag_fn)(mbedtls_gcm_context *ctx , int mode , size_t length , const unsigned char *iv , size_t iv_len , const unsigned char *add , size_t add_len , const unsigned char *input , unsigned char *output , size_t tag_len , unsigned char *tag);
typedef int (*mbedtls_gcm_auth_decrypt_fn)(mbedtls_gcm_context *ctx , size_t length , const unsigned char *iv , size_t iv_len , const unsigned char *add , size_t add_len , const unsigned char *tag , size_t tag_len , const unsigned char *input , unsigned char *output);
typedef int (*mbedtls_gcm_starts_fn)(mbedtls_gcm_context *ctx , int mode , const unsigned char *iv , size_t iv_len , const unsigned char *add , size_t add_len);
typedef int (*mbedtls_gcm_update_fn)(mbedtls_gcm_context *ctx , size_t length , const unsigned char *input , unsigned char *output);
typedef int (*mbedtls_gcm_finish_fn)(mbedtls_gcm_context *ctx , unsigned char *tag , size_t tag_len);
typedef void (*mbedtls_gcm_free_fn)(mbedtls_gcm_context *ctx);


typedef struct
{
    size_t backend_context_size;
    mbedtls_gcm_check_fn check;
    mbedtls_gcm_init_fn init;
    mbedtls_gcm_setkey_fn setkey;
    mbedtls_gcm_crypt_and_tag_fn crypt_and_tag;
    mbedtls_gcm_auth_decrypt_fn auth_decrypt;
    mbedtls_gcm_starts_fn starts;
    mbedtls_gcm_update_fn update;
    mbedtls_gcm_finish_fn finish;
    mbedtls_gcm_free_fn free;
} mbedtls_gcm_funcs;


#endif // BACKEND_GCM_H
