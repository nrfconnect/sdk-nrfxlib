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

/**
 * @file
 *   This file implements storage of PIB attributes in nRF 802.15.4 radio driver.
 *
 */

#include "nrf_802154_pib.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_nrfx_addons.h"
#include "nrf_802154_utils.h"
#include "fal/nrf_802154_fal.h"

#define CSMACA_BE_MAXIMUM 8 ///< The maximum allowed CSMA-CA backoff exponent (BE) that results from the implementation

typedef struct
{
    nrf_802154_coex_rx_request_mode_t rx_request_mode; ///< Coex request mode in receive operation.
    nrf_802154_coex_tx_request_mode_t tx_request_mode; ///< Coex request mode in transmit operation.
} nrf_802154_pib_coex_t;

#if NRF_802154_CSMA_CA_ENABLED
typedef struct
{
    uint8_t min_be;       // The minimum value of the backoff exponent (BE) in the CSMA-CA algorithm
    uint8_t max_be;       // The maximum value of the backoff exponent (BE) in the CSMA-CA algorithm
    uint8_t max_backoffs; // The maximum number of backoffs that the CSMA-CA algorithm will attempt before declaring a channel access failure.
} nrf_802154_pib_csmaca_t;

#endif  // NRF_802154_CSMA_CA_ENABLED

#if NRF_802154_IFS_ENABLED
typedef struct
{
    nrf_802154_ifs_mode_t mode;               ///< Mode of Interframe Space insertion.
    uint16_t              min_sifs_period_us; ///< Minimum Short Interframe Space period in us.
    uint16_t              min_lifs_period_us; ///< Minimum Long Interframe Space period in us.
} nrf_802154_pib_ifs_t;

#endif  // NRF_802154_IFS_ENABLED

typedef struct
{
    int8_t                  tx_power;                             ///< Transmit power.
    uint8_t                 pan_id[PAN_ID_SIZE];                  ///< Pan Id of this node.
    uint8_t                 short_addr[SHORT_ADDRESS_SIZE];       ///< Short Address of this node.
    uint8_t                 extended_addr[EXTENDED_ADDRESS_SIZE]; ///< Extended Address of this node.
    nrf_802154_cca_cfg_t    cca;                                  ///< CCA mode and thresholds.
    bool                    promiscuous : 1;                      ///< Indicating if radio is in promiscuous mode.
    bool                    auto_ack    : 1;                      ///< Indicating if auto ACK procedure is enabled.
    bool                    pan_coord   : 1;                      ///< Indicating if radio is configured as the PAN coordinator.
    uint8_t                 channel     : 5;                      ///< Channel on which the node receives messages.
    nrf_802154_pib_coex_t   coex;                                 ///< Coex-related fields.

#if NRF_802154_CSMA_CA_ENABLED
    nrf_802154_pib_csmaca_t csmaca;                               ///< CSMA-CA related fields.

#endif

#if NRF_802154_IFS_ENABLED
    nrf_802154_pib_ifs_t ifs; ///< IFS-related fields.

#endif

} nrf_802154_pib_data_t;

// Static variables.
static nrf_802154_pib_data_t m_data; ///< Buffer containing PIB data.

/**
 * Converts TX power integer values to RADIO TX power allowed values.
 *
 * @param[in]  integer_tx_power  TX power integer value.
 *
 * @retval     RADIO TX power allowed value.
 */
static nrf_radio_txpower_t to_radio_tx_power_convert(int8_t integer_tx_power)
{
    const nrf_radio_txpower_t allowed_values[] =
    {
#if defined(RADIO_TXPOWER_TXPOWER_Neg40dBm)
        NRF_RADIO_TXPOWER_NEG40DBM, /**< -40 dBm. */
#endif
        NRF_RADIO_TXPOWER_NEG20DBM, /**< -20 dBm. */
        NRF_RADIO_TXPOWER_NEG16DBM, /**< -16 dBm. */
        NRF_RADIO_TXPOWER_NEG12DBM, /**< -12 dBm. */
        NRF_RADIO_TXPOWER_NEG8DBM,  /**< -8 dBm. */
        NRF_RADIO_TXPOWER_NEG4DBM,  /**< -4 dBm. */
        NRF_RADIO_TXPOWER_0DBM,     /**< 0 dBm. */
#if defined(RADIO_TXPOWER_TXPOWER_Pos2dBm)
        NRF_RADIO_TXPOWER_POS2DBM,  /**< 2 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos3dBm)
        NRF_RADIO_TXPOWER_POS3DBM,  /**< 3 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos4dBm)
        NRF_RADIO_TXPOWER_POS4DBM,  /**< 4 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos5dBm)
        NRF_RADIO_TXPOWER_POS5DBM,  /**< 5 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos6dBm)
        NRF_RADIO_TXPOWER_POS6DBM,  /**< 6 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos7dBm)
        NRF_RADIO_TXPOWER_POS7DBM,  /**< 7 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos8dBm)
        NRF_RADIO_TXPOWER_POS8DBM,  /**< 8 dBm. */
#endif
    };

    nrf_radio_txpower_t radio_tx_power = allowed_values[NUMELTS(allowed_values) - 1];

    if (integer_tx_power < (int8_t)radio_tx_power)
    {
        for (uint32_t i = 0; i < NUMELTS(allowed_values); i++)
        {
            if (integer_tx_power <= (int8_t)allowed_values[i])
            {
                radio_tx_power = allowed_values[i];
                break;
            }
        }
    }

    return radio_tx_power;
}

/**
 * @brief Checks if provided Coex transmit request mode is supported.
 *
 * @param[in] mode  Coex transmit request mode to check.
 *
 * @retval true     When value provided by @p mode param is supported.
 * @retval false    Otherwise.
 */
static bool coex_tx_request_mode_is_supported(nrf_802154_coex_tx_request_mode_t mode)
{
    bool result = false;

    switch (mode)
    {
        case NRF_802154_COEX_TX_REQUEST_MODE_FRAME_READY:
        case NRF_802154_COEX_TX_REQUEST_MODE_CCA_START:
        case NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE:
            result = true;
            break;

        default:
            break;
    }

    return result;
}

/**
 * @brief Checks if provided Coex receive request mode is supported.
 *
 * @param[in] mode  Coex receive request mode to check.
 *
 * @retval true     When value provided by @p mode param is supported.
 * @retval false    Otherwise.
 */
static bool coex_rx_request_mode_is_supported(nrf_802154_coex_rx_request_mode_t mode)
{
    bool result = false;

    switch (mode)
    {
#if !NRF_802154_DISABLE_BCC_MATCHING && defined(RADIO_INTENSET_SYNC_Msk)
        case NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION:
#endif
        case NRF_802154_COEX_RX_REQUEST_MODE_PREAMBLE:
#if !NRF_802154_DISABLE_BCC_MATCHING
        case NRF_802154_COEX_RX_REQUEST_MODE_DESTINED:
#endif
            result = true;
            break;

        default:
            break;
    }

    return result;
}

void nrf_802154_pib_init(void)
{
    m_data.promiscuous = false;
    m_data.auto_ack    = true;
    m_data.pan_coord   = false;
    m_data.channel     = 11;

    memset(m_data.pan_id, 0xff, sizeof(m_data.pan_id));
    m_data.short_addr[0] = 0xfe;
    m_data.short_addr[1] = 0xff;
    memset(m_data.extended_addr, 0, sizeof(m_data.extended_addr));

    m_data.cca.mode           = NRF_802154_CCA_MODE_DEFAULT;
    m_data.cca.ed_threshold   = NRF_802154_CCA_ED_THRESHOLD_DEFAULT;
    m_data.cca.corr_threshold = NRF_802154_CCA_CORR_THRESHOLD_DEFAULT;
    m_data.cca.corr_limit     = NRF_802154_CCA_CORR_LIMIT_DEFAULT;

#if NRF_802154_DISABLE_BCC_MATCHING
    m_data.coex.rx_request_mode = NRF_802154_COEX_RX_REQUEST_MODE_PREAMBLE;
#else
    m_data.coex.rx_request_mode = NRF_802154_COEX_RX_REQUEST_MODE_DESTINED;
#endif
    m_data.coex.tx_request_mode = NRF_802154_COEX_TX_REQUEST_MODE_FRAME_READY;

#if NRF_802154_CSMA_CA_ENABLED
    m_data.csmaca.min_be       = NRF_802154_CSMA_CA_MIN_BE_DEFAULT;
    m_data.csmaca.max_be       = NRF_802154_CSMA_CA_MAX_BE_DEFAULT;
    m_data.csmaca.max_backoffs = NRF_802154_CSMA_CA_MAX_CSMA_BACKOFFS_DEFAULT;
#endif // NRF_802154_CSMA_CA_ENABLED

#if NRF_802154_IFS_ENABLED
    m_data.ifs.min_sifs_period_us = MIN_SIFS_PERIOD_US;
    m_data.ifs.min_lifs_period_us = MIN_LIFS_PERIOD_US;
    m_data.ifs.mode               = NRF_802154_IFS_MODE_DISABLED;
#endif // NRF_802154_IFS_ENABLED
}

bool nrf_802154_pib_promiscuous_get(void)
{
    return m_data.promiscuous;
}

void nrf_802154_pib_promiscuous_set(bool enabled)
{
    m_data.promiscuous = enabled;
}

bool nrf_802154_pib_auto_ack_get(void)
{
    return m_data.auto_ack;
}

void nrf_802154_pib_auto_ack_set(bool enabled)
{
    m_data.auto_ack = enabled;
}

bool nrf_802154_pib_pan_coord_get(void)
{
    return m_data.pan_coord;
}

void nrf_802154_pib_pan_coord_set(bool enabled)
{
    m_data.pan_coord = enabled;
}

uint8_t nrf_802154_pib_channel_get(void)
{
    return m_data.channel;
}

void nrf_802154_pib_channel_set(uint8_t channel)
{
    m_data.channel = channel;
}

nrf_radio_txpower_t nrf_802154_pib_tx_power_get(void)
{
    int8_t tx_power = nrf_802154_fal_tx_power_get(m_data.channel, m_data.tx_power);

    return to_radio_tx_power_convert(tx_power);
}

void nrf_802154_pib_tx_power_set(int8_t dbm)
{
    m_data.tx_power = dbm;
}

const uint8_t * nrf_802154_pib_pan_id_get(void)
{
    return m_data.pan_id;
}

void nrf_802154_pib_pan_id_set(const uint8_t * p_pan_id)
{
    memcpy(m_data.pan_id, p_pan_id, PAN_ID_SIZE);
}

const uint8_t * nrf_802154_pib_extended_address_get(void)
{
    return m_data.extended_addr;
}

void nrf_802154_pib_extended_address_set(const uint8_t * p_extended_address)
{
    memcpy(m_data.extended_addr, p_extended_address, EXTENDED_ADDRESS_SIZE);
}

const uint8_t * nrf_802154_pib_short_address_get(void)
{
    return m_data.short_addr;
}

void nrf_802154_pib_short_address_set(const uint8_t * p_short_address)
{
    memcpy(m_data.short_addr, p_short_address, SHORT_ADDRESS_SIZE);
}

void nrf_802154_pib_cca_cfg_set(const nrf_802154_cca_cfg_t * p_cca_cfg)
{
    switch (p_cca_cfg->mode)
    {
        case NRF_RADIO_CCA_MODE_ED:
            m_data.cca.mode         = p_cca_cfg->mode;
            m_data.cca.ed_threshold = p_cca_cfg->ed_threshold;
            break;

        case NRF_RADIO_CCA_MODE_CARRIER:
            m_data.cca.mode           = p_cca_cfg->mode;
            m_data.cca.corr_threshold = p_cca_cfg->corr_threshold;
            m_data.cca.corr_limit     = p_cca_cfg->corr_limit;
            break;

        case NRF_RADIO_CCA_MODE_CARRIER_AND_ED:
        case NRF_RADIO_CCA_MODE_CARRIER_OR_ED:
            memcpy(&m_data.cca, p_cca_cfg, sizeof(m_data.cca));
            break;

        default:
            assert(false);
    }
}

void nrf_802154_pib_cca_cfg_get(nrf_802154_cca_cfg_t * p_cca_cfg)
{
    memcpy(p_cca_cfg, &m_data.cca, sizeof(m_data.cca));
}

bool nrf_802154_pib_coex_rx_request_mode_set(nrf_802154_coex_rx_request_mode_t mode)
{
    bool result = coex_rx_request_mode_is_supported(mode);

    if (result)
    {
        m_data.coex.rx_request_mode = mode;
    }

    return result;
}

nrf_802154_coex_rx_request_mode_t nrf_802154_pib_coex_rx_request_mode_get(void)
{
    return m_data.coex.rx_request_mode;
}

bool nrf_802154_pib_coex_tx_request_mode_set(nrf_802154_coex_tx_request_mode_t mode)
{
    bool result = coex_tx_request_mode_is_supported(mode);

    if (result)
    {
        m_data.coex.tx_request_mode = mode;
    }

    return result;
}

nrf_802154_coex_tx_request_mode_t nrf_802154_pib_coex_tx_request_mode_get(void)
{
    return m_data.coex.tx_request_mode;
}

#if NRF_802154_CSMA_CA_ENABLED
bool nrf_802154_pib_csmaca_min_be_set(uint8_t min_be)
{
    bool result = (min_be <= CSMACA_BE_MAXIMUM);

    if (result)
    {
        m_data.csmaca.min_be = min_be;
    }

    return result;
}

uint8_t nrf_802154_pib_csmaca_min_be_get(void)
{
    return m_data.csmaca.min_be;
}

bool nrf_802154_pib_csmaca_max_be_set(uint8_t max_be)
{
    bool result = (max_be <= CSMACA_BE_MAXIMUM);

    if (result)
    {
        m_data.csmaca.max_be = max_be;
    }

    return result;
}

uint8_t nrf_802154_pib_csmaca_max_be_get(void)
{
    return m_data.csmaca.max_be;
}

void nrf_802154_pib_csmaca_max_backoffs_set(uint8_t max_backoffs)
{
    m_data.csmaca.max_backoffs = max_backoffs;
}

uint8_t nrf_802154_pib_csmaca_max_backoffs_get(void)
{
    return m_data.csmaca.max_backoffs;
}

#endif // NRF_802154_CSMA_CA_ENABLED

#if NRF_802154_IFS_ENABLED
nrf_802154_ifs_mode_t nrf_802154_pib_ifs_mode_get(void)
{
    return m_data.ifs.mode;
}

bool nrf_802154_pib_ifs_mode_set(nrf_802154_ifs_mode_t mode)
{
    switch (mode)
    {
        case NRF_802154_IFS_MODE_DISABLED:
        case NRF_802154_IFS_MODE_MATCHING_ADDRESSES:
        case NRF_802154_IFS_MODE_ALWAYS:
            m_data.ifs.mode = mode;
            return true;

        default:
            return false;
    }
}

uint16_t nrf_802154_pib_ifs_min_sifs_period_get(void)
{
    return m_data.ifs.min_sifs_period_us;
}

void nrf_802154_pib_ifs_min_sifs_period_set(uint16_t period)
{
    assert(period >= TURNAROUND_TIME);

    m_data.ifs.min_sifs_period_us = period;
}

uint16_t nrf_802154_pib_ifs_min_lifs_period_get(void)
{
    return m_data.ifs.min_lifs_period_us;
}

void nrf_802154_pib_ifs_min_lifs_period_set(uint16_t period)
{
    m_data.ifs.min_lifs_period_us = period;
}

#endif // NRF_802154_IFS_ENABLED
