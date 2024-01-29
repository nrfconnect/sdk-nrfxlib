/*
 * Copyright (c) 2016 - 2024 Nordic Semiconductor ASA
 * Copyright (c) since 2013 Oberon microsystems AG
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**@file
 * @defgroup ocrypto_spake2p SPAKE2+ APIs
 * @ingroup ocrypto
 * @{
 * @brief Type declarations and APIs for SPAKE2+ P256.
 */

#ifndef OCRYPTO_SPAKE2P_P256_H
#define OCRYPTO_SPAKE2P_P256_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * SPAKE2+ P256 secret key reduction.
 *
 * @param[out] x       Reduced secret key.
 * @param      xs      Extended secret key.
 * @param      xs_len  Extended secret key length.
 */
void ocrypto_spake2p_p256_reduce(
    uint8_t x[32],
    const uint8_t *xs, size_t xs_len);

/**
 * SPAKE2+ P256 public key validation check.
 *
 * @param      K       Public key.
 *
 * @retval 0  If the key is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_spake2p_p256_check_key(
    const uint8_t K[65]);

/**
 * SPAKE2+ P256 key share calculation.
 *
 * @param[out] XY      Public key share.
 * @param      w0      First password hash.
 * @param      xy      Secret key.
 * @param      MN      Random element.
 *
 * @retval 0  If the key share is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_spake2p_p256_get_key_share(
    uint8_t XY[65],
    const uint8_t w0[32],
    const uint8_t xy[32],
    const uint8_t MN[65]);

/**
 * SPAKE2+ P256 common value calculation.
 *
 * @param[out] Z       Common value Z.
 * @param[out] V       Common value V.
 * @param      w0      First password hash.
 * @param      w1      Second password hash. NULL on server side.
 * @param      xy      Secret key.
 * @param      YX      Peer key share.
 * @param      NM      Peer random element.
 * @param      L       Password registration. NULL on client side.
 */
void ocrypto_spake2p_p256_get_ZV(
    uint8_t Z[65],
    uint8_t V[65],
    const uint8_t w0[32],
    const uint8_t w1[32],
    const uint8_t xy[32],
    const uint8_t YX[65],
    const uint8_t NM[65],
    const uint8_t L[65]);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
