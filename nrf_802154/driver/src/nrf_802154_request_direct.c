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
 * @file
 *   This file implements requests to the driver triggered directly by the MAC layer.
 *
 */

#include "nrf_802154_request.h"

#if NRF_802154_REQUEST_IMPL == NRF_802154_REQUEST_IMPL_DIRECT

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_core.h"
#include "mac_features/nrf_802154_delayed_trx.h"
#include "mac_features/nrf_802154_csma_ca.h"
#include "hal/nrf_radio.h"

#define REQUEST_FUNCTION_PARMS(func_core, ...) \
    bool result;                               \
                                               \
    result = func_core(__VA_ARGS__);           \
                                               \
    return result;

#define REQUEST_FUNCTION(func_core) \
    bool result;                    \
                                    \
    result = func_core();           \
                                    \
    return result;

void nrf_802154_request_init(void)
{
    // Intentionally empty
}

bool nrf_802154_request_sleep(nrf_802154_term_t term_lvl)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_sleep, term_lvl)
}

bool nrf_802154_request_receive(nrf_802154_term_t              term_lvl,
                                req_originator_t               req_orig,
                                nrf_802154_notification_func_t notify_function,
                                bool                           notify_abort,
                                uint32_t                       id)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_receive,
                           term_lvl,
                           req_orig,
                           notify_function,
                           notify_abort,
                           id)
}

bool nrf_802154_request_transmit(nrf_802154_term_t              term_lvl,
                                 req_originator_t               req_orig,
                                 uint8_t                      * p_data,
                                 nrf_802154_transmit_params_t * p_params,
                                 nrf_802154_notification_func_t notify_function)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_transmit,
                           term_lvl,
                           req_orig,
                           p_data,
                           p_params,
                           notify_function)
}

bool nrf_802154_request_ack_timeout_handle(const nrf_802154_ack_timeout_handle_params_t * p_param)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_ack_timeout_handle, p_param);
}

bool nrf_802154_request_energy_detection(nrf_802154_term_t term_lvl, uint32_t time_us)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_energy_detection, term_lvl, time_us)
}

bool nrf_802154_request_cca(nrf_802154_term_t term_lvl)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_cca, term_lvl)
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

bool nrf_802154_request_continuous_carrier(nrf_802154_term_t term_lvl)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_continuous_carrier, term_lvl)
}

bool nrf_802154_request_modulated_carrier(nrf_802154_term_t term_lvl,
                                          const uint8_t   * p_data)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_modulated_carrier, term_lvl, p_data)
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

bool nrf_802154_request_buffer_free(uint8_t * p_data)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_notify_buffer_free, p_data)
}

bool nrf_802154_request_antenna_update(void)
{
    REQUEST_FUNCTION(nrf_802154_core_antenna_update)
}

bool nrf_802154_request_channel_update(req_originator_t req_orig)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_channel_update, req_orig)
}

bool nrf_802154_request_cca_cfg_update(void)
{
    REQUEST_FUNCTION(nrf_802154_core_cca_cfg_update)
}

bool nrf_802154_request_rssi_measure(void)
{
    REQUEST_FUNCTION(nrf_802154_core_rssi_measure)
}

bool nrf_802154_request_rssi_measurement_get(int8_t * p_rssi)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_core_last_rssi_measurement_get, p_rssi)
}

static inline bool are_frame_properties_valid(const nrf_802154_transmitted_frame_props_t * p_props)
{
    return p_props->dynamic_data_is_set || !(p_props->is_secured);
}

#if NRF_802154_DELAYED_TRX_ENABLED
bool nrf_802154_request_transmit_raw_at(uint8_t                                 * p_data,
                                        uint64_t                                  tx_time,
                                        const nrf_802154_transmit_at_metadata_t * p_metadata)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_delayed_trx_transmit, p_data, tx_time, p_metadata);
}

bool nrf_802154_request_transmit_at_cancel(void)
{
    REQUEST_FUNCTION(nrf_802154_delayed_trx_transmit_cancel);
}

bool nrf_802154_request_receive_at(uint64_t rx_time,
                                   uint32_t timeout,
                                   uint8_t  channel,
                                   uint32_t id)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_delayed_trx_receive, rx_time, timeout, channel, id);
}

bool nrf_802154_request_receive_at_cancel(uint32_t id)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_delayed_trx_receive_cancel, id);
}

bool nrf_802154_request_receive_at_scheduled_cancel(uint32_t id)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_delayed_trx_receive_scheduled_cancel, id);
}

#endif

bool nrf_802154_request_csma_ca_start(uint8_t                                      * p_data,
                                      const nrf_802154_transmit_csma_ca_metadata_t * p_metadata)
{
    REQUEST_FUNCTION_PARMS(nrf_802154_csma_ca_start, p_data, p_metadata);
}

#endif /* NRF_802154_REQUEST_IMPL == NRF_802154_REQUEST_IMPL_DIRECT */
