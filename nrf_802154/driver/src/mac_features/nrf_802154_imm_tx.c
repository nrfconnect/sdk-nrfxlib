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

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_IMM_TX

#include "nrf_802154_imm_tx.h"

#include "nrf_802154_request.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_tx_power.h"
#include "rsch/nrf_802154_rsch.h"
#include "nrf_802154_debug_log.h"

static void imm_tx_done(uint8_t                                   * p_frame,
                        const nrf_802154_transmit_done_metadata_t * p_metadata,
                        const nrf_802154_tx_client_t              * p_client)
{
    nrf_802154_sl_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_notify_transmitted(p_frame, p_metadata);

    nrf_802154_sl_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static bool imm_tx_can_abort(nrf_802154_term_t              term_lvl,
                             req_originator_t               req_orig,
                             const nrf_802154_tx_client_t * p_client)
{
    nrf_802154_sl_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_sl_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return true;
}

static void imm_tx_started(const nrf_802154_tx_client_t * p_client)
{
    nrf_802154_sl_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // Intentionally empty.

    nrf_802154_sl_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void imm_tx_failed(uint8_t                                   * p_frame,
                          nrf_802154_tx_error_t                       error,
                          const nrf_802154_transmit_done_metadata_t * p_metadata,
                          const nrf_802154_tx_client_t              * p_client)
{
    nrf_802154_sl_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_notify_transmit_failed(p_frame, error, p_metadata);

    nrf_802154_sl_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static const nrf_802154_tx_client_interface_t m_imm_tx_tx_client_iface = {
    .can_abort = imm_tx_can_abort,
    .started   = imm_tx_started,
    .failed    = imm_tx_failed,
    .done      = imm_tx_done,
};

static nrf_802154_tx_client_t m_imm_tx_tx_client = {
    .p_iface = &m_imm_tx_tx_client_iface,
};

bool nrf_802154_imm_tx_transmit(const nrf_802154_frame_t             * p_frame,
                                const nrf_802154_transmit_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_tx_error_t result;
    uint8_t               channel;

    channel =
        p_metadata->tx_channel.use_metadata_value ? p_metadata->tx_channel.channel :
        nrf_802154_pib_channel_get();

    nrf_802154_transmit_params_t params =
    {
        .frame_props         = p_metadata->frame_props,
        .tx_power            = {0},
        .channel             = channel,
        .cca                 = p_metadata->cca,
        .immediate           = false,
        .extra_cca_attempts  = 0U,
        .tx_timestamp_encode = p_metadata->tx_timestamp_encode,
        .frame               = *p_frame,
        .p_client            = &m_imm_tx_tx_client,
        .rsch_timeslot_id    = NRF_802154_RSCH_DLY_TS_ID_INVALID,
    };

    (void)nrf_802154_tx_power_convert_metadata_to_tx_power_split(channel,
                                                                 p_metadata->tx_power,
                                                                 &params.tx_power);

    result = nrf_802154_request_transmit(NRF_802154_TERM_NONE,
                                         REQ_ORIG_HIGHER_LAYER,
                                         &params);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result == NRF_802154_TX_ERROR_NONE;
}
