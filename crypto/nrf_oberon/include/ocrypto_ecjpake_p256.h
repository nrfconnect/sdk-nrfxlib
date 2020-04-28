/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon_ecjpake EC-JPAKE
 * @ingroup nrf_oberon
 * @{
 * @brief Type declaration and APIs for EC-JPAKE.
 *
 */

#ifndef OCRYPTO_ECJPAKE_P256_H
#define OCRYPTO_ECJPAKE_P256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


/**
 * EC-JPAKE-P256 public key and zero knowledge proof generation.
 *
 * @param[out] X       Public key.
 * @param[out] V       ZKP ephemeral public key.
 * @param[out] r       ZKP signature.
 * @param      G       Generator. May be NULL to use the default generator.
 * @param      x       Secret key. 0 < x < group order.
 * @param      v       ZKP ephemeral secret key. 0 < v < group order.
 * @param      id      Identity of originator.
 * @param      id_len  Identity length.
 *
 * @retval 0  If inputs are valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecjpake_get_key(
    uint8_t X[64],
    uint8_t V[64],
    uint8_t r[32],
    const uint8_t G[64],
    const uint8_t x[32],
    const uint8_t v[32],
    const char *id, size_t id_len);

/**
 * EC-JPAKE-P256 zero knowledge proof verification.
 *
 * @param      G       Generator. May be NULL to use the default generator.
 * @param      X       Public key.
 * @param      V       ZKP ephemeral public key.
 * @param      r       ZKP signature.
 * @param      id      Identity of originator.
 * @param      id_len  Identity length.
 *
 * @retval 0  If proof is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecjpake_verify_key(
    const uint8_t G[64],
    const uint8_t X[64],
    const uint8_t V[64],
    const uint8_t r[32],
    const char *id, size_t id_len);

/**
 * EC-JPAKE-P256 generator derivation.
 *
 * @param[out] G       Generator.
 * @param      X1      Public key 1.
 * @param      X2      Public key 2.
 * @param      X3      Public key 3.
 *
 * @retval 0  If the generator is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecjpake_get_generator(
    uint8_t G[64],
    const uint8_t X1[64],
    const uint8_t X2[64],
    const uint8_t X3[64]);

/**
 * EC-JPAKE-P256 read shared secret.
 *
 * @param[out] rs          Reduced shared secret.
 * @param      secret      Shared secret.
 * @param      secret_len  Secret length.
 */
void ocrypto_ecjpake_read_shared_secret(
    uint8_t rs[32],
    const uint8_t *secret, size_t secret_len);

/**
 * EC-JPAKE-P256 shared secret handling.
 *
 * @param[out] xs          Client/server secret key.
 * @param      x2          Secret key 2.
 * @param      rs          Reduced shared secret.
 *
 * @retval 0  If the derived secret key is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecjpake_process_shared_secret(
    uint8_t xs[32],
    const uint8_t x2[32],
    const uint8_t rs[32]);

/**
 * EC-JPAKE-P256 secret key generation.
 *
 * @param[out] secret  Resulting premaster secret.
 * @param      Xr      Remote client/server public key.
 * @param      X2      Remote public key 2.
 * @param      xs      Client/server secret key.
 * @param      x2      Secret key 2.
 *
 * @retval 0  If the key is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_ecjpake_get_secret_key(
    uint8_t secret[32],
    const uint8_t Xr[64],
    const uint8_t X2[64],
    const uint8_t xs[32],
    const uint8_t x2[32]);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_ECJPAKE_P256_H */

/** @} */
