#ifndef BACKEND_CCM_H
#define BACKEND_CCM_H

#include "mbedtls/ccm.h"

typedef int (*mbedtls_ccm_check_fn)(mbedtls_cipher_id_t cipher, unsigned int keybits);
typedef void (*mbedtls_ccm_init_fn)(mbedtls_ccm_context *ctx);
typedef int (*mbedtls_ccm_setkey_fn)(mbedtls_ccm_context *ctx, mbedtls_cipher_id_t cipher, const unsigned char *key, unsigned int keybits);
typedef void (*mbedtls_ccm_free_fn)(mbedtls_ccm_context *ctx);
typedef int (*mbedtls_ccm_encrypt_and_tag_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, unsigned char *tag, size_t tag_len);
typedef int (*mbedtls_ccm_star_encrypt_and_tag_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, unsigned char *tag, size_t tag_len);
typedef int (*mbedtls_ccm_auth_decrypt_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, const unsigned char *tag, size_t tag_len);
typedef int (*mbedtls_ccm_star_auth_decrypt_fn)(mbedtls_ccm_context *ctx, size_t length, const unsigned char *iv, size_t iv_len, const unsigned char *add, size_t add_len, const unsigned char *input, unsigned char *output, const unsigned char *tag, size_t tag_len);


typedef struct
{
    size_t backend_context_size;
    mbedtls_ccm_check_fn check;
    mbedtls_ccm_init_fn init;
    mbedtls_ccm_setkey_fn setkey;
    mbedtls_ccm_free_fn free;
    mbedtls_ccm_encrypt_and_tag_fn encrypt_and_tag;
    mbedtls_ccm_star_encrypt_and_tag_fn star_encrypt_and_tag;
    mbedtls_ccm_auth_decrypt_fn auth_decrypt;
    mbedtls_ccm_star_auth_decrypt_fn star_auth_decrypt;
} mbedtls_ccm_funcs;


#endif // BACKEND_CCM_H
