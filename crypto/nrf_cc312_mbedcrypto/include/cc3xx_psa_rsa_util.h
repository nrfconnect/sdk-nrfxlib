/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_rsa_util.h
 *
 * This file contains the declarations of the internal utility functions used
 * to manipulate RSA types and key formats
 *
 */

#ifndef CC3XX_PSA_RSA_UTIL_H
#define CC3XX_PSA_RSA_UTIL_H

#include "psa/crypto.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Checks the validity/support of an RSA key
 *
 * \param[in] bits Bitsize of the RSA key
 *
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t cc3xx_check_rsa_key_size(size_t bits);

/**
 * \brief Extract the public key from the private key in PSA format
 * \param[in]  priv_key_buffer        Buffer holding the private key
 * \param[in]  priv_key_buffer_size   Size in bytes of the private key buffer
 * \param[out] publ_key_buffer        Buffer to hold the extracted public key
 * \param[in]  publ_key_buffer_size   Size in bytes of the public key buffer
 * \param[out] publ_key_buffer_length Size in bytes of the extracted public key
 * \retval  PSA_SUCCESS on success. Error code from psa_status_t on failure
 */
psa_status_t cc3xx_rsa_psa_priv_to_psa_publ(uint8_t *priv_key_buffer,
                                            size_t priv_key_buffer_size,
                                            uint8_t *publ_key_buffer,
                                            size_t publ_key_buffer_size,
                                            size_t *publ_key_buffer_length);
#ifdef __cplusplus
}
#endif
#endif /* CC3XX_PSA_RSA_UTIL_H */
