/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2026 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 *
 */

/** \file cc3xx_ecdh_util.h
 *
 * This file contains the declarations of the internal functions required to
 * perform Elliptic Curve Diffie-Hellman, i.e. ECDH raw key agreement
 *
 */

#ifndef CC3XX_ECDH_UTIL_H
#define CC3XX_ECDH_UTIL_H

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Calculate a shared secret value (Short Weierstrass)
 *
 * @param[in]  curve           Curve family
 * @param[in]  key_bits        Key size in bits
 * @param[in]  priv_key        Priv. key
 * @param[in]  priv_key_size   Priv. key size
 * @param[in]  publ_key        Publ. key
 * @param[in]  publ_key_size   Publ. key size
 * @param[out] secret          Shared secret value
 * @param[out] secret_size     Shared secret value size
 * @param[out] secret_length   The actual size of the shared secret value
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecdh_calc_secret_wrst(
    psa_ecc_family_t curve,
    psa_key_bits_t key_bits,
    const uint8_t *priv_key, size_t priv_key_size,
    const uint8_t *publ_key, size_t publ_key_size,
    uint8_t *secret, size_t secret_size, size_t *secret_length);

/**
 * @brief   Calculate a shared secret value (Montgomery)
 *
 * @param[in]  priv_key      Priv. key
 * @param[in]  priv_key_size Priv. key size
 * @param[in]  publ_key      Publ. key
 * @param[in]  publ_key_size Publ. key size
 * @param[out] secret        Shared secret value
 * @param[out] secret_size   Shared secret value size
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecdh_calc_secret_mont(
        const uint8_t *priv_key, size_t priv_key_size,
        const uint8_t *publ_key, size_t publ_key_size,
        uint8_t *secret, size_t *secret_size);

#ifdef __cplusplus
}
#endif
#endif /* CC3XX_ECDH_UTIL_H */
