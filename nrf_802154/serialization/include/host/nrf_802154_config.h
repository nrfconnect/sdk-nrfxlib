/*
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NRF_802154_CONFIG_H__
#define NRF_802154_CONFIG_H__

#ifdef NRF_802154_PROJECT_CONFIG
#include NRF_802154_PROJECT_CONFIG
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_config_csma CSMA/CA procedure configuration
 * @{
 */

/**
 * @def NRF_802154_CSMA_CA_ENABLED
 *
 * If CSMA-CA is to be enabled by the driver. Disabling CSMA-CA improves
 * the driver performance.
 *
 */
#if !defined(CONFIG_NRF_802154_SL_OPENSOURCE)
#ifndef NRF_802154_CSMA_CA_ENABLED
#define NRF_802154_CSMA_CA_ENABLED 1
#endif
#endif

/**
 *@}
 **/

/**
 * @defgroup nrf_802154_config_dtrx Delayed operations configuration
 * @{
 */

/**
 * @def NRF_802154_DELAYED_TRX_ENABLED
 *
 * If the delayed transmission and the receive window features are available.
 *
 */
#if !defined(CONFIG_NRF_802154_SL_OPENSOURCE)
#ifndef NRF_802154_DELAYED_TRX_ENABLED
#define NRF_802154_DELAYED_TRX_ENABLED 1
#endif
#endif

/**
 * @}
 * @defgroup nrf_802154_config_security Security configuration
 * @{
 */

/**
 * @def NRF_802154_SECURITY_KEY_STORAGE_SIZE
 *
 * Configures the number of keys which are available in the Key Storage.
 * This configuration is implementation-independent.
 */
#ifndef NRF_802154_SECURITY_KEY_STORAGE_SIZE
#define NRF_802154_SECURITY_KEY_STORAGE_SIZE 3
#endif

/**
 *@}
 **/

/**
 * @def NRF_802154_CARRIER_FUNCTIONS_ENABLED
 *
 * Enables functions used for test purposes: nrf_802154_continuous_carrier and
 * nrf_802154_modulated_carrier
 */
#ifndef NRF_802154_CARRIER_FUNCTIONS_ENABLED
#define NRF_802154_CARRIER_FUNCTIONS_ENABLED 1
#endif

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_CONFIG_H__
