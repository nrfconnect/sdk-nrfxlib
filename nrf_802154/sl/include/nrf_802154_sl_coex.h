/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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

#ifndef NRF_802154_SL_COEX_H__
#define NRF_802154_SL_COEX_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_sl_coex Wifi Coexistence Service
 * @{
 * @ingroup nrf_802154_sl_coex
 * @brief The Wifi Coexistence Service for the 802.15.4 Service Layer
 */

/** @brief Coex return codes.
 *
 * Possible values:
 * @ref NRF_802154_WIFI_COEX_RET_SUCCESS
 * @ref NRF_802154_WIFI_COEX_RET_INVALID_PIN
 * @ref NRF_802154_WIFI_COEX_RET_DEFAULT_ERR
 */

typedef uint32_t nrf_802154_wifi_coex_ret_t;

#define NRF_802154_WIFI_COEX_RET_SUCCESS     0x00 /** Operation finished succesfully. */
#define NRF_802154_WIFI_COEX_RET_INVALID_PIN 0x01 /** Operation failed because coex pins have not been initialized. */
#define NRF_802154_WIFI_COEX_RET_DEFAULT_ERR 0xFF /** Default error code. */

/** @brief Coex interface types.
 *
 * Possible values:
 * @ref NRF_802154_WIFI_COEX_IF_NONE
 * @ref NRF_802154_WIFI_COEX_IF_3WIRE
 */
typedef uint32_t nrf_802154_wifi_coex_interface_type_id_t;

#define NRF_802154_WIFI_COEX_IF_NONE  0x00 /** No coex interface is selected */
#define NRF_802154_WIFI_COEX_IF_3WIRE 0x01 /** 3-wire coex interface is selected */

/**@brief Pin configuration for wifi coex */
typedef struct
{
    uint8_t gpio_pin;    /**< The GPIO pin number. */
    uint8_t active_high; /**< Active pin level: 1 - high level, 0 - low level. */
    uint8_t drive;       /**< Pin drive configuration. */
    uint8_t pull;        /**< Pull-up/down configuration. */
} nrf_802154_wifi_coex_pin_out_config_t;

typedef struct
{
    uint8_t gpio_pin;     /**< The GPIO pin number. */
    uint8_t active_high;  /**< Active pin level: 1 - high level, 0 - low level. */
    uint8_t gpiote_ch_id; /**< GPIOTE channel number. */
} nrf_802154_wifi_coex_pin_in_config_t;

/**@brief 3-wire interface configuration for wifi coex */
typedef struct
{
    nrf_802154_wifi_coex_pin_out_config_t request_cfg;  /**< Request line configuration. */
    nrf_802154_wifi_coex_pin_out_config_t priority_cfg; /**< Priority and Status (sometiems also called CONFIG or REQUEST_TYPE) line configuration. */
    nrf_802154_wifi_coex_pin_in_config_t  grant_cfg;    /**< Grant line configuration. */
} nrf_802154_wifi_coex_3wire_if_config_t;

#define COEX_GPIO_PIN_INVALID 0xFF                      /**< Value indicating that the pin has not yet been configured. */

/** Macro with the default configuration of 3-wire coex interface */
#define NRF_802154_COEX_3WIRE_DEFAULT_CONFIG    \
    ((nrf_802154_wifi_coex_3wire_if_config_t) { \
        .request_cfg = {                        \
            .gpio_pin = COEX_GPIO_PIN_INVALID,  \
            .active_high = true,                \
            .drive = GPIO_PIN_CNF_DRIVE_S0S1,   \
            .pull = GPIO_PIN_CNF_PULL_Disabled  \
        },                                      \
        .priority_cfg = {                       \
            .gpio_pin = COEX_GPIO_PIN_INVALID,  \
            .active_high = false,               \
            .drive = GPIO_PIN_CNF_DRIVE_S0S1,   \
            .pull = GPIO_PIN_CNF_PULL_Disabled  \
        },                                      \
        .grant_cfg = {                          \
            .gpio_pin = COEX_GPIO_PIN_INVALID,  \
            .active_high = false,               \
            .gpiote_ch_id = 4                   \
        }                                       \
    })

/**
 * @brief Gets current coex interface type.
 *
 * @return Current coex interface type.
 */
nrf_802154_wifi_coex_interface_type_id_t nrf_802154_wifi_coex_interface_type_id_get(void);

/**
 * @brief Gets current wifi coex 3wire configuration.
 *
 * @param[out] p_cfg Pointer to the configuration structure where current configuration will be stored.
 */
void nrf_802154_wifi_coex_cfg_3wire_get(nrf_802154_wifi_coex_3wire_if_config_t * p_cfg);

/**
 * @brief Sets wifi coex 3wire configuration.
 *
 * For the configuration to be applied, @ref nrf_802154_wifi_coex_init must be called.
 * Should be called once, before @ref nrf_802154_wifi_coex_init.
 *
 * @param[in] p_cfg Pointer to the configuration structure with coex configuration to be set.
 *                  Structure will be copied and can be freed by the caller afterwards.
 *
 * @sa nrf_802154_wifi_coex_cfg_3wire_get
 */
void nrf_802154_wifi_coex_cfg_3wire_set(const nrf_802154_wifi_coex_3wire_if_config_t * p_cfg);

/**
 * @brief Initializes the Wi-Fi Coexistence module.
 *
 * For this function to be called, driver needs to be already initialized through @ref nrf_802154_init, and interface type with valid pins
 * must be configured.
 *
 * @retval ::NRF_802154_WIFI_COEX_RET_SUCCESS      Coexistence interface was configured successfully
 * @retval ::NRF_802154_WIFI_COEX_RET_INVALID_PIN  Not all gpio pin numbers have been set and coexistence interface could not be configured.
 */
nrf_802154_wifi_coex_ret_t nrf_802154_wifi_coex_init(void);

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_SL_COEX_H__
