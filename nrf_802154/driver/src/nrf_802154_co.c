/*
 * Copyright (c) 2024, Nordic Semiconductor ASA
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
 *   This file implements final exit point for calling callouts provided by an application.
 *   It is intended to be the only place where callouts provided by the application
 *   are called.
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_CO

#include "nrf_802154_co.h"
#include "nrf_802154_debug.h"

void nrf_802154_co_cca_done(bool channel_free)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_cca_done(channel_free);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_co_cca_failed(nrf_802154_cca_error_t error)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_cca_failed(error);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_co_energy_detected(const nrf_802154_energy_detected_t * p_result)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_energy_detected(p_result);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_co_energy_detection_failed(nrf_802154_ed_error_t error)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_energy_detection_failed(error);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_co_tx_ack_started(const uint8_t * p_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_tx_ack_started(p_data);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)

void nrf_802154_co_received_raw(uint8_t * p_data, int8_t power, uint8_t lqi)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_received_raw(p_data, power, lqi);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#endif // !NRF_802154_SERIALIZATION_HOST

void nrf_802154_co_receive_failed(nrf_802154_rx_error_t error, uint32_t id)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_receive_failed(error, id);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_co_transmitted_raw(uint8_t                                   * p_frame,
                                   const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_transmitted_raw(p_frame, p_metadata);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_co_transmit_failed(uint8_t                                   * p_frame,
                                   nrf_802154_tx_error_t                       error,
                                   const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
    nrf_802154_transmit_failed(p_frame, error, p_metadata);
    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}
