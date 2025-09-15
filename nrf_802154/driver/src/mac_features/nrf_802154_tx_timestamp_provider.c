/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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
 *   This file implements the transmit timestamp provider for the 802.15.4 driver.
 *
 */

#include "mac_features/nrf_802154_tx_timestamp_provider.h"

#include "mac_features/nrf_802154_frame.h"
#include "nrf_802154_utils_byteorder.h"
#include "nrf_802154_sl_timer.h"

#if NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED

static uint64_t  m_tx_timestamp_us;    ///< Cached transmit timestamp.
static uint8_t * mp_tx_timestamp_addr; ///< Cached tx timestamp placeholder address.

nrf_802154_tx_error_t nrf_802154_tx_timestamp_provider_tx_setup(
    nrf_802154_transmit_params_t * p_params)
{
    NRF_802154_ASSERT(nrf_802154_frame_parse_level_get(&p_params->frame) >=
                      PARSE_LEVEL_FULL);

    bool result = true;

    m_tx_timestamp_us    = 0;
    mp_tx_timestamp_addr = NULL;

    if (p_params->tx_timestamp_encode == false)
    {
        // There is nothing to do.
        return NRF_802154_TX_ERROR_NONE;
    }

    do
    {
        if (p_params->frame_props.is_secured)
        {
            result = false;
            break;
        }

        if (nrf_802154_frame_type_get(&p_params->frame) != FRAME_TYPE_DATA)
        {
            result = false;
            break;
        }

        if (nrf_802154_frame_mac_payload_get(&p_params->frame) == NULL)
        {
            result = false;
            break;
        }

        uint8_t payload_len = nrf_802154_frame_mac_payload_length_get(&p_params->frame);

        if (payload_len < NRF_802154_TX_TIMESTAMP_PROVIDER_TIMESTAMP_SIZE)
        {
            result = false;
            break;
        }

        mp_tx_timestamp_addr =
            (uint8_t *)(nrf_802154_frame_mac_payload_get(&p_params->frame) +
                        payload_len -
                        NRF_802154_TX_TIMESTAMP_PROVIDER_TIMESTAMP_SIZE);
    }
    while (0);

    if (result == false)
    {
        return NRF_802154_TX_ERROR_TIMESTAMP_ENCODING_ERROR;
    }

    return NRF_802154_TX_ERROR_NONE;
}

void nrf_802154_tx_timestamp_provider_tx_started_hook(uint8_t * p_frame)
{
    NRF_802154_ASSERT(m_tx_timestamp_us == 0);

    // Verify that the setup phase has been completed.
    if (mp_tx_timestamp_addr == NULL)
    {
        return;
    }
    /* This function is executed in the handler of RADIO.ADDRESS event. According to the IPS,
     * in 802.15.4 transmit sequence RADIO.FRAMESTART event is triggered after the SHR is
     * transmitted (nRF52840 PS v1.7 -- 6.20.12.6 Transmit sequence). However, RADIO.ADDRESS
     * event is also triggered in 802.15.4 transmit sequence with a constant 32us offset.
     * This handler is therefore expected to execute 32us before the SHR transmission ends.
     *
     * This function executes approximately 32us before the first bit of PHR.
     * The calculation takes it into account by adding 32us to the current time.
     */
    m_tx_timestamp_us = nrf_802154_sl_timer_current_time_get() + 32;
    host_64_to_big(m_tx_timestamp_us, mp_tx_timestamp_addr);

    mp_tx_timestamp_addr = NULL;
}

#endif // NRF_802154_TX_TIMESTAMP_PROVIDER_ENABLED
