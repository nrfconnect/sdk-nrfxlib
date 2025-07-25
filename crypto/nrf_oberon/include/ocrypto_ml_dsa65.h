/*
 * Copyright (c) 2016 - 2025 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**
 * @defgroup ocrypto_ml_dsa65 ML-DSA-65
 * @ingroup ocrypto_asym_sig
 * @{
 * @brief ML-DSA-65 signature algorithm based on NIST PQC standard.
 *
 * ML-DSA-65 is a specific implementation of CRYSTALS-Dilithium, a PQC digital
 * signature scheme. It is based on learning with errors (LWE) and is designed to
 * be resistant against quantum attacks.
 *
 * @see [FIPS - Module-Lattice-Based Digital Signature Standard](https://csrc.nist.gov/pubs/fips/204/final)
 */

/**
 * @file
 * @brief ML-DSA-65 (FIPS-204 variant of CRYSTALS-Dilithium) API.
 */

#ifndef OCRYPTO_ML_DSA65_H
#define OCRYPTO_ML_DSA65_H

#include <stddef.h>
#include <stdint.h>

#include "ocrypto_sha3.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Public key size in bytes.
 */
#define ocrypto_ml_dsa65_PK_SIZE  (1952)

/**
 * Secret key size in bytes.
 */
#define ocrypto_ml_dsa65_SK_SIZE  (4032)

/**
 * Signature size in bytes.
 */
#define ocrypto_ml_dsa65_SIG_SIZE (3309)


/**@cond */
typedef struct {
    ocrypto_sha3_ctx sha3;
    int32_t t[256];
#ifdef OCRYPTO_ML_DSA_SMALL
    int32_t c[256];
    int32_t s[256];
#else
    int32_t u[6][256];
#ifdef OCRYPTO_ML_DSA_FAST
    int32_t a[6][5][256];
    int32_t s1[5][256];
    int32_t s2[6][256];
    int32_t y[5][256];
    int32_t t0[6][256];
#endif
#endif
} ocrypto_ml_dsa65_ctx;
/**@endcond */


/**
 * Generates a key pair.
 *
 * @param[out] ctx  Context.
 * @param[out] sk   Secret key (can be NULL).
 * @param[out] pk   Public key.
 * @param[in]  zeta Random seed (32 bytes).
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_key_pair(
    ocrypto_ml_dsa65_ctx *ctx,
    uint8_t sk[ocrypto_ml_dsa65_SK_SIZE], 
    uint8_t pk[ocrypto_ml_dsa65_PK_SIZE],
    const uint8_t zeta[32]);

/**@cond */
/**
* Internal sign.
*
 * @param[in]  ctx  Context.
 * @param[out] sig  Generated signature.
 * @param[in]  mu   Message representative (64 bytes).
 * @param[in]  sk   Secret key.
 * @param[in]  rnd  Random seed (32 bytes, NULL for deterministic signing).
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_sign_internal(
    ocrypto_ml_dsa65_ctx *ctx,
    uint8_t sig[ocrypto_ml_dsa65_SIG_SIZE],
    const uint8_t mu[64],
    const uint8_t sk[ocrypto_ml_dsa65_SK_SIZE],
    const uint8_t rnd[32]);
/**@endcond */

/**
 * Signs a message.
 *
 * @param[in]  ctx     Context.
 * @param[out] sig     Generated signature.
 * @param[in]  m       Message to sign.
 * @param[in]  mlen    Length of the message.
 * @param[in]  ctx_str Context string (optional).
 * @param[in]  ctx_len Length of the context string (0 <= ctx_len < 256).
 * @param[in]  sk      Secret key.
 * @param[in]  rnd     Random seed (32 bytes, NULL for deterministic signing).
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_sign(
    ocrypto_ml_dsa65_ctx *ctx,
    uint8_t sig[ocrypto_ml_dsa65_SIG_SIZE],
    const uint8_t *m, size_t mlen,
    const uint8_t *ctx_str, size_t ctx_len,
    const uint8_t sk[ocrypto_ml_dsa65_SK_SIZE],
    const uint8_t rnd[32]);

/**
 * Signs a hash.
 *
 * @param[in]  ctx      Context.
 * @param[out] sig      Generated signature.
 * @param[in]  hash     Hash to sign.
 * @param[in]  hash_len Length of the hash.
 * @param[in]  oid      Object identifier (11 bytes).
 * @param[in]  ctx_str  Context string (optional).
 * @param[in]  ctx_len  Length of the context string (0 <= ctx_len < 256).
 * @param[in]  sk       Secret key.
 * @param[in]  rnd      Random seed (32 bytes, NULL for deterministic signing).
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_sign_hash(
    ocrypto_ml_dsa65_ctx *ctx,
    uint8_t sig[ocrypto_ml_dsa65_SIG_SIZE],
    const uint8_t *hash, size_t hash_len,
    const uint8_t oid[11],
    const uint8_t *ctx_str, size_t ctx_len,
    const uint8_t sk[ocrypto_ml_dsa65_SK_SIZE],
    const uint8_t rnd[32]);

/**@cond */
/**
 * Internal signature verify.
 *
 * @param[in] ctx Context.
 * @param[in] sig Signature to verify.
 * @param[in] mu  Message digest (64 bytes).
 * @param[in] pk  Public key.
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_verify_internal(
    ocrypto_ml_dsa65_ctx *ctx,
    const uint8_t sig[ocrypto_ml_dsa65_SIG_SIZE],
    const uint8_t mu[64],
    const uint8_t pk[ocrypto_ml_dsa65_PK_SIZE]);
/**@endcond */

/**
 * Verifies a signature against a message.
 *
 * @param[in] ctx     Context.
 * @param[in] sig     Signature to verify.
 * @param[in] m       Message to verify.
 * @param[in] mlen    Length of the message.
 * @param[in] ctx_str Context string (optional).
 * @param[in] ctx_len Length of the context string (0 <= ctx_len < 256).
 * @param[in] pk      Public key.
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_verify(
    ocrypto_ml_dsa65_ctx *ctx,
    const uint8_t sig[ocrypto_ml_dsa65_SIG_SIZE],
    const uint8_t *m, size_t mlen,
    const uint8_t *ctx_str, size_t ctx_len,
    const uint8_t pk[ocrypto_ml_dsa65_PK_SIZE]);

/**
 * Verifies a signature against a hash.
 *
 * @param[in] ctx      Context.
 * @param[in] sig      Signature to verify.
 * @param[in] hash     Hash to verify.
 * @param[in] hash_len Length of the hash.
 * @param[in] oid      Object identifier (11 bytes).
 * @param[in] ctx_str  Context string (optional).
 * @param[in] ctx_len  Length of the context string (0 <= ctx_len < 256).
 * @param[in] pk       Public key.
 * @return 0 on success, non-zero on failure.
 */
int ocrypto_ml_dsa65_verify_hash(
    ocrypto_ml_dsa65_ctx *ctx,
    const uint8_t sig[ocrypto_ml_dsa65_SIG_SIZE],
    const uint8_t *hash, size_t hash_len,
    const uint8_t oid[11],
    const uint8_t *ctx_str, size_t ctx_len,
    const uint8_t pk[ocrypto_ml_dsa65_PK_SIZE]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
