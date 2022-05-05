/*
 * Copyright (c) 2022, Nordic Semiconductor ASA
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

#include "nrf_802154_tx_power.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_utils.h"
#include "nrf_802154_fal.h"

/**
 * Converts TX power integer values to RADIO TX power allowed values.
 *
 * @param[in]  integer_tx_power  TX power integer value.
 *
 * @retval     RADIO TX power allowed value.
 */
static nrf_radio_txpower_t to_radio_tx_power_convert(int8_t integer_tx_power)
{
    static const nrf_radio_txpower_t allowed_values[] =
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
 * Constrains the TX power by the maximum allowed TX power allowed for a specific channel, splits it into
 * components to be applied on each stage of the transmit path and for the TX power applied to the RADIO peripheral
 * converts the integer value to a RADIO TX power allowed value.
 *
 * @param[in]  channel      The channel based on which the power should be constrained
 * @param[in]  tx_power     Unconstrained TX power integer value.
 * @param[out] split_power  Pointer to the structure holding TX power split into constrained and converted components.
 *
 * @retval  true            Calculation performed successfully.
 * @retval  false           Given @p power cannot be achieved. If requested value is too high
 *                          the @p p_tx_power_split will be set to a value representing maximum
 *                          achievable power. If the requested value is too low, the
 *                          @p p_tx_power_split will be set to a value representing minimum
 *                          achievable power.
 */
static bool constrain_split_and_convert_tx_power(
    uint8_t                             channel,
    int8_t                              tx_power,
    nrf_802154_tx_power_split_t * const split_power)
{
    int32_t                         ret             = 0;
    nrf_802154_fal_tx_power_split_t fal_split_power = {0};

    ret = nrf_802154_fal_tx_power_split(channel, tx_power, &fal_split_power);

    split_power->fem_gain       = fal_split_power.fem_gain;
    split_power->radio_tx_power = to_radio_tx_power_convert(fal_split_power.radio_tx_power);

    return (0 == ret);
}

bool nrf_802154_tx_power_convert_metadata_to_tx_power_split(
    uint8_t                             channel,
    nrf_802154_tx_power_metadata_t      tx_power,
    nrf_802154_tx_power_split_t * const p_tx_power_split)
{
    int8_t power_unconstrained =
        tx_power.use_metadata_value ? tx_power.power : nrf_802154_pib_tx_power_get();

    return constrain_split_and_convert_tx_power(channel, power_unconstrained, p_tx_power_split);
}

bool nrf_802154_tx_power_split_pib_power_get(nrf_802154_tx_power_split_t * const p_split_power)
{
    return constrain_split_and_convert_tx_power(nrf_802154_pib_channel_get(),
                                                nrf_802154_pib_tx_power_get(),
                                                p_split_power);
}

bool nrf_802154_tx_power_split_pib_power_for_channel_get(
    uint8_t                             channel,
    nrf_802154_tx_power_split_t * const p_split_power)
{
    return constrain_split_and_convert_tx_power(channel,
                                                nrf_802154_pib_tx_power_get(),
                                                p_split_power);
}
