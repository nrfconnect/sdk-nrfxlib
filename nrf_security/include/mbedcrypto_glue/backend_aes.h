#ifndef BACKEND_AES_H
#define BACKEND_AES_H

#include "mbedtls/aes.h"

typedef int (*mbedtls_aes_check_fn)(unsigned int keybits, int mode, int xts);
typedef void (*mbedtls_aes_init_fn)(mbedtls_aes_context *ctx);
typedef void (*mbedtls_aes_free_fn)(mbedtls_aes_context *ctx);
#if defined(MBEDTLS_CIPHER_MODE_XTS)
typedef void (*mbedtls_aes_xts_init_fn)(mbedtls_aes_xts_context *ctx);
typedef void (*mbedtls_aes_xts_free_fn)(mbedtls_aes_xts_context *ctx);
#endif
typedef int (*mbedtls_aes_setkey_enc_fn)(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);
typedef int (*mbedtls_aes_setkey_dec_fn)(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);
#if defined(MBEDTLS_CIPHER_MODE_XTS)
typedef int (*mbedtls_aes_xts_setkey_enc_fn)(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits);
typedef int (*mbedtls_aes_xts_setkey_dec_fn)(mbedtls_aes_xts_context *ctx, const unsigned char *key, unsigned int keybits);
#endif
typedef int (*mbedtls_internal_aes_encrypt_fn)(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16]);
typedef int (*mbedtls_internal_aes_decrypt_fn)(mbedtls_aes_context *ctx, const unsigned char input[16], unsigned char output[16]);
#if defined(MBEDTLS_CIPHER_MODE_CBC)
typedef int (*mbedtls_aes_crypt_cbc_fn)(mbedtls_aes_context *ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output);
#endif
#if defined(MBEDTLS_CIPHER_MODE_XTS)
typedef int (*mbedtls_aes_crypt_xts_fn)(mbedtls_aes_xts_context *ctx, int mode, size_t length, const unsigned char data_unit[16], const unsigned char *input, unsigned char *output);
#endif
#if defined(MBEDTLS_CIPHER_MODE_CFB)
typedef int (*mbedtls_aes_crypt_cfb128_fn)(mbedtls_aes_context *ctx, int mode, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output);
typedef int (*mbedtls_aes_crypt_cfb8_fn)(mbedtls_aes_context *ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output);
#endif
#if defined(MBEDTLS_CIPHER_MODE_OFB)
typedef int (*mbedtls_aes_crypt_ofb_fn)(mbedtls_aes_context *ctx, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output);
#endif
#if defined(MBEDTLS_CIPHER_MODE_CTR)
typedef int (*mbedtls_aes_crypt_ctr_fn)(mbedtls_aes_context *ctx, size_t length, size_t *nc_off, unsigned char nonce_counter[16], unsigned char stream_block[16], const unsigned char *input, unsigned char *output);
#endif


typedef struct
{
    size_t backend_context_size;
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    size_t backend_xts_context_size;
#endif /* MBEDTLS_CIPHER_MODE_XTS */
    mbedtls_aes_check_fn check;
    mbedtls_aes_init_fn init;
    mbedtls_aes_free_fn free;
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    mbedtls_aes_xts_init_fn xts_init;
    mbedtls_aes_xts_free_fn xts_free;
#endif
    mbedtls_aes_setkey_enc_fn setkey_enc;
    mbedtls_aes_setkey_dec_fn setkey_dec;
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    mbedtls_aes_xts_setkey_enc_fn xts_setkey_enc;
    mbedtls_aes_xts_setkey_dec_fn xts_setkey_dec;
#endif
    mbedtls_internal_aes_encrypt_fn internal_encrypt;
    mbedtls_internal_aes_decrypt_fn internal_decrypt;
#if defined(MBEDTLS_CIPHER_MODE_CBC)
    mbedtls_aes_crypt_cbc_fn crypt_cbc;
#endif
#if defined(MBEDTLS_CIPHER_MODE_XTS)
    mbedtls_aes_crypt_xts_fn crypt_xts;
#endif
#if defined(MBEDTLS_CIPHER_MODE_CFB)
    mbedtls_aes_crypt_cfb128_fn crypt_cfb128;
    mbedtls_aes_crypt_cfb8_fn crypt_cfb8;
#endif
#if defined(MBEDTLS_CIPHER_MODE_OFB)
    mbedtls_aes_crypt_ofb_fn crypt_ofb;
#endif
#if defined(MBEDTLS_CIPHER_MODE_CTR)
    mbedtls_aes_crypt_ctr_fn crypt_ctr;
#endif
} mbedtls_aes_funcs;


#endif // BACKEND_AES_H
