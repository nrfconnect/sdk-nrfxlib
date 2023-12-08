/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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
 * @brief This module defines the nRF 802.15.4 Frontend Module abstraction layer.
 *
 */

#ifndef NRF_802154_FAL_H_
#define NRF_802154_FAL_H_

#include <stdint.h>
#include "protocol/mpsl_fem_protocol_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents components of tx_power to be applied for stages on transmit path.
 */
typedef struct
{
    int8_t          radio_tx_power; // !< TX power in dBm to be applied to the RADIO peripheral.
    mpsl_fem_gain_t fem;            // !< Data needed to set the FEM gain
} nrf_802154_fal_tx_power_split_t;

/** @brief Splits transmit power value into components to be applied on each stage on the transmit path.
 *
 * @note This is a stub implementation used when MPSL is not linked.
 *
 * @param[in]  channel          Ignored.
 * @param[in]  power            TX power in dBm requested for transmission on air.
 * @param[out] p_tx_power_split Components of tx_power to be applied for stages on transmit path.
 *
 * @returns The real achieved total transmission power in dBm.
 */
int8_t nrf_802154_fal_tx_power_split(const uint8_t                           channel,
                                     const int8_t                            power,
                                     nrf_802154_fal_tx_power_split_t * const p_tx_power_split);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_FAL_H_ */
