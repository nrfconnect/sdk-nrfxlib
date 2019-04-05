#ifndef BACKEND_RSA_H
#define BACKEND_RSA_H

#include "mbedtls/rsa.h"

typedef int (*mbedtls_rsa_check_fn)(int padding, int hash_id, unsigned int nbits);
typedef void (*mbedtls_rsa_init_fn)(mbedtls_rsa_context *ctx, int padding, int hash_id);
typedef int (*mbedtls_rsa_import_fn)(mbedtls_rsa_context *ctx, const mbedtls_mpi *N, const mbedtls_mpi *P, const mbedtls_mpi *Q, const mbedtls_mpi *D, const mbedtls_mpi *E);
typedef int (*mbedtls_rsa_import_raw_fn)(mbedtls_rsa_context *ctx, unsigned char const *N, size_t N_len, unsigned char const *P, size_t P_len, unsigned char const *Q, size_t Q_len, unsigned char const *D, size_t D_len, unsigned char const *E, size_t E_len);
typedef int (*mbedtls_rsa_complete_fn)(mbedtls_rsa_context *ctx);
typedef int (*mbedtls_rsa_export_fn)(const mbedtls_rsa_context *ctx, mbedtls_mpi *N, mbedtls_mpi *P, mbedtls_mpi *Q, mbedtls_mpi *D, mbedtls_mpi *E);
typedef int (*mbedtls_rsa_export_raw_fn)(const mbedtls_rsa_context *ctx, unsigned char *N, size_t N_len, unsigned char *P, size_t P_len, unsigned char *Q, size_t Q_len, unsigned char *D, size_t D_len, unsigned char *E, size_t E_len);
typedef int (*mbedtls_rsa_export_crt_fn)(const mbedtls_rsa_context *ctx, mbedtls_mpi *DP, mbedtls_mpi *DQ, mbedtls_mpi *QP);
typedef void (*mbedtls_rsa_set_padding_fn)(mbedtls_rsa_context *ctx, int padding, int hash_id);
typedef size_t (*mbedtls_rsa_get_len_fn)(const mbedtls_rsa_context *ctx);
#if defined(MBEDTLS_GENPRIME)
typedef int (*mbedtls_rsa_gen_key_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, unsigned int nbits, int exponent);
#endif
typedef int (*mbedtls_rsa_check_pubkey_fn)(const mbedtls_rsa_context *ctx);
typedef int (*mbedtls_rsa_check_privkey_fn)(const mbedtls_rsa_context *ctx);
typedef int (*mbedtls_rsa_check_pub_priv_fn)(const mbedtls_rsa_context *pub, const mbedtls_rsa_context *prv);
typedef int (*mbedtls_rsa_public_fn)(mbedtls_rsa_context *ctx, const unsigned char *input, unsigned char *output);
typedef int (*mbedtls_rsa_private_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, const unsigned char *input, unsigned char *output);
typedef int (*mbedtls_rsa_pkcs1_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output);
#if defined(MBEDTLS_PKCS1_V15)
typedef int (*mbedtls_rsa_rsaes_pkcs1_v15_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t ilen, const unsigned char *input, unsigned char *output);
#endif
#if defined(MBEDTLS_PKCS1_V21)
typedef int (*mbedtls_rsa_rsaes_oaep_encrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t ilen, const unsigned char *input, unsigned char *output);
#endif
typedef int (*mbedtls_rsa_pkcs1_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);
#if defined(MBEDTLS_PKCS1_V15)
typedef int (*mbedtls_rsa_rsaes_pkcs1_v15_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);
#endif
#if defined(MBEDTLS_PKCS1_V21)
typedef int (*mbedtls_rsa_rsaes_oaep_decrypt_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, const unsigned char *label, size_t label_len, size_t *olen, const unsigned char *input, unsigned char *output, size_t output_max_len);
#endif
typedef int (*mbedtls_rsa_pkcs1_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);
#if defined(MBEDTLS_PKCS1_V15)
typedef int (*mbedtls_rsa_rsassa_pkcs1_v15_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);
#endif
#if defined(MBEDTLS_PKCS1_V21)
typedef int (*mbedtls_rsa_rsassa_pss_sign_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, unsigned char *sig);
#endif
typedef int (*mbedtls_rsa_pkcs1_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);
#if defined(MBEDTLS_PKCS1_V15)
typedef int (*mbedtls_rsa_rsassa_pkcs1_v15_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);
#endif
#if defined(MBEDTLS_PKCS1_V21)
typedef int (*mbedtls_rsa_rsassa_pss_verify_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, const unsigned char *sig);
typedef int (*mbedtls_rsa_rsassa_pss_verify_ext_fn)(mbedtls_rsa_context *ctx, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng, int mode, mbedtls_md_type_t md_alg, unsigned int hashlen, const unsigned char *hash, mbedtls_md_type_t mgf1_hash_id, int expected_salt_len, const unsigned char *sig);
#endif
typedef int (*mbedtls_rsa_copy_fn)(mbedtls_rsa_context *dst, const mbedtls_rsa_context *src);
typedef void (*mbedtls_rsa_free_fn)(mbedtls_rsa_context *ctx);


typedef struct
{
    size_t backend_context_size;
    mbedtls_rsa_check_fn check;
    mbedtls_rsa_init_fn init;
    mbedtls_rsa_import_fn import;
    mbedtls_rsa_import_raw_fn import_raw;
    mbedtls_rsa_complete_fn complete;
    mbedtls_rsa_export_fn export;
    mbedtls_rsa_export_raw_fn export_raw;
    mbedtls_rsa_export_crt_fn export_crt;
    mbedtls_rsa_set_padding_fn set_padding;
    mbedtls_rsa_get_len_fn get_len;
#if defined(MBEDTLS_GENPRIME)
    mbedtls_rsa_gen_key_fn gen_key;
#endif
    mbedtls_rsa_check_pubkey_fn check_pubkey;
    mbedtls_rsa_check_privkey_fn check_privkey;
    mbedtls_rsa_check_pub_priv_fn check_pub_priv;
    mbedtls_rsa_public_fn do_public;
    mbedtls_rsa_private_fn do_private;
    mbedtls_rsa_pkcs1_encrypt_fn pkcs1_encrypt;
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsaes_pkcs1_v15_encrypt_fn rsaes_pkcs1_v15_encrypt;
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsaes_oaep_encrypt_fn rsaes_oaep_encrypt;
#endif
    mbedtls_rsa_pkcs1_decrypt_fn pkcs1_decrypt;
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsaes_pkcs1_v15_decrypt_fn rsaes_pkcs1_v15_decrypt;
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsaes_oaep_decrypt_fn rsaes_oaep_decrypt;
#endif
    mbedtls_rsa_pkcs1_sign_fn pkcs1_sign;
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsassa_pkcs1_v15_sign_fn rsassa_pkcs1_v15_sign;
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsassa_pss_sign_fn rsassa_pss_sign;
#endif
    mbedtls_rsa_pkcs1_verify_fn pkcs1_verify;
#if defined(MBEDTLS_PKCS1_V15)
    mbedtls_rsa_rsassa_pkcs1_v15_verify_fn rsassa_pkcs1_v15_verify;
#endif
#if defined(MBEDTLS_PKCS1_V21)
    mbedtls_rsa_rsassa_pss_verify_fn rsassa_pss_verify;
    mbedtls_rsa_rsassa_pss_verify_ext_fn rsassa_pss_verify_ext;
#endif
    mbedtls_rsa_copy_fn copy;
    mbedtls_rsa_free_fn free;
} mbedtls_rsa_funcs;


#endif // BACKEND_RSA_H
