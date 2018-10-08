/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-5-Clause-Nordic
 */

/**@file nrf_key_mgmt.h
 *
 * @defgroup nrf_key_mgmt nRF91 Key Management
 * @{
 */
#ifndef NRF_KEY_MGMT_H__
#define NRF_KEY_MGMT_H__

/**@brief Enumeration values for credentials types
 *        available in the key management module.
 */
typedef enum {
    NRF_KEY_MGMT_CRED_TYPE_CA_CHAIN,
    NRF_KEY_MGMT_CRED_TYPE_PUBLIC_CERT,
    NRF_KEY_MGMT_CRED_TYPE_PRIVATE_CERT,
    NRF_KEY_MGMT_CRED_TYPE_PSK,
    NRF_KEY_MGMT_CRED_TYPE_IDENTITY
} nrf_key_mgnt_cred_type_t;

#endif // NRF_KEY_MGMT_H__
/**@} */
