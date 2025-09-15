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

#ifndef NRF_802154_TX_TIMESTAMP_PROVIDER_H__
#define NRF_802154_TX_TIMESTAMP_PROVIDER_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_802154_types_internal.h"

#define NRF_802154_TX_TIMESTAMP_PROVIDER_TIMESTAMP_SIZE (sizeof(uint64_t))

/**
 * @brief TX setup hook for the tx timestamp provider module.
 *
 * Checks transmission parameters and if required, latches a frame payload address
 * where the transmission timestamp shall be written once the transmission starts.
 *
 * If the transmission parameters do not request transmit timestamp encoding, this hook exits
 * without performing any actions.
 *
 * @param[in]  p_params  Pointer to the transmission parameters.
 *
 * @retval  NRF_802154_TX_ERROR_NONE                      The procedure was successful.
 * @retval  NRF_802154_TX_ERROR_TIMESTAMP_ENCODING_ERROR  The timestamp cannot be inserted into the frame.
 */
nrf_802154_tx_error_t nrf_802154_tx_timestamp_provider_tx_setup(
    nrf_802154_transmit_params_t * p_params);

/**
 * @brief TX started hook for the tx timestamp provider module.
 *
 * This hook is triggered when a transmission starts.
 * If @ref nrf_802154_tx_timestamp_provider_tx_setup has configured a valid frame
 * payload address for timestamping, the hook writes the current transmission
 * time (in microseconds) to that address.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains the PHR and PSDU of the transmitted frame.
 */
void nrf_802154_tx_timestamp_provider_tx_started_hook(uint8_t * p_frame);

#endif // NRF_802154_TX_TIMESTAMP_PROVIDER_H__
