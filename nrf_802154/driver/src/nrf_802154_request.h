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

#ifndef NRF_802154_REQUEST_H__
#define NRF_802154_REQUEST_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_const.h"
#include "nrf_802154_notification.h"
#include "nrf_802154_types_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_request 802.15.4 driver request
 * @{
 * @ingroup nrf_802154
 * @brief Requests to the driver triggered from the MAC layer.
 */

/**
 * @brief Initializes the request module.
 */
void nrf_802154_request_init(void);

/**
 * @brief Requests entering the @ref RADIO_STATE_SLEEP state for the driver.
 *
 * @param[in]  term_lvl  Termination level of this request. Selects procedures to abort.
 *
 * @retval  true   The driver will enter sleep state.
 * @retval  false  The driver cannot enter the sleep state due to an ongoing operation.
 */
bool nrf_802154_request_sleep(nrf_802154_term_t term_lvl);

/**
 * @brief Requests entering the @ref RADIO_STATE_RX state for the driver.
 *
 * @param[in]  term_lvl         Termination level of this request. Selects procedures to abort.
 * @param[in]  req_orig         Module that originates this request.
 * @param[in]  notify_function  Function called to notify the status of this procedure. May be NULL.
 * @param[in]  notify_abort     If the abort notification is to be triggered automatically.
 * @param[in]  id               Identifier of a reception window.
 *
 * @retval  true   The driver will enter the receive state.
 * @retval  false  The driver cannot enter the receive state due to ongoing operation.
 */
bool nrf_802154_request_receive(nrf_802154_term_t              term_lvl,
                                req_originator_t               req_orig,
                                nrf_802154_notification_func_t notify_function,
                                bool                           notify_abort,
                                uint32_t                       id);

/**
 * @brief Request entering the @ref RADIO_STATE_TX state for the driver.
 *
 * @param[in]  term_lvl         Termination level of this request. Selects procedures to abort.
 * @param[in]  req_orig         Module that originates this request.
 * @param[in]  p_data           Pointer to the frame to transmit.
 * @param[in]  p_params         Pointer to transmission parameters.
 * @param[in]  notify_function  Function called to notify the status of this procedure. May be NULL.
 *
 * @retval  true   The driver will enter the transmit state.
 * @retval  false  The driver cannot enter the transmit state due to an ongoing operation.
 */
bool nrf_802154_request_transmit(nrf_802154_term_t              term_lvl,
                                 req_originator_t               req_orig,
                                 uint8_t                      * p_data,
                                 nrf_802154_transmit_params_t * p_params,
                                 nrf_802154_notification_func_t notify_function);

/**
 * @brief Request to handle Ack timeout by the core module.
 *
 * @param[in]  p_param  Parameter to pass to nrf_802154_core_ack_timeout_handle
 *
 */
bool nrf_802154_request_ack_timeout_handle(const nrf_802154_ack_timeout_handle_params_t * p_param);

/**
 * @brief Requests entering the @ref RADIO_STATE_ED state.
 *
 * @param[in]  term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[in]  time_us   Requested duration of the energy detection procedure.
 *
 * @retval  true   The driver will enter energy detection state.
 * @retval  false  The driver cannot enter the energy detection state due to an ongoing operation.
 */
bool nrf_802154_request_energy_detection(nrf_802154_term_t term_lvl, uint32_t time_us);

/**
 * @brief Requests entering the @ref RADIO_STATE_CCA state.
 *
 * @param[in]  term_lvl  Termination level of this request. Selects procedures to abort.
 *
 * @retval  true   The driver will enter the CCA state.
 * @retval  false  The driver cannot enter the CCA state due to an ongoing operation.
 */
bool nrf_802154_request_cca(nrf_802154_term_t term_lvl);

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Requests entering the @ref RADIO_STATE_CONTINUOUS_CARRIER state.
 *
 * @param[in]  term_lvl  Termination level of this request. Selects procedures to abort.
 *
 * @retval  true   The driver will enter the continuous carrier state.
 * @retval  false  The driver cannot enter the continuous carrier state due to an ongoing operation.
 */
bool nrf_802154_request_continuous_carrier(nrf_802154_term_t term_lvl);

/**
 * @brief Requests entering the @ref RADIO_STATE_MODULATED_CARRIER state.
 *
 * @param[in]  term_lvl  Termination level of this request. Selects procedures to abort.
 * @param[in]  p_data    Pointer to a buffer to modulate the carrier with.
 *
 * @retval  true   The driver will enter the modulated carrier state.
 * @retval  false  The driver cannot enter the modulated carrier state due to an ongoing operation.
 */
bool nrf_802154_request_modulated_carrier(nrf_802154_term_t term_lvl,
                                          const uint8_t   * p_data);

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Requests the driver to free the given buffer.
 *
 * @param[in]  p_data  Pointer to the buffer to be freed.
 */
bool nrf_802154_request_buffer_free(uint8_t * p_data);

/**
 * @brief Requests the driver to update the antenna used.
 */
bool nrf_802154_request_antenna_update(void);

/**
 * @brief Requests the driver to update the channel number used by the RADIO peripheral.
 *
 * @param[in]  req_orig   Module that originates this request.
 */
bool nrf_802154_request_channel_update(req_originator_t req_orig);

/**
 * @brief Requests the driver to update the CCA configuration used by the RADIO peripheral.
 */
bool nrf_802154_request_cca_cfg_update(void);

/**
 * @brief Requests the RSSI measurement.
 */
bool nrf_802154_request_rssi_measure(void);

/**
 * @brief Requests getting the last RSSI measurement result.
 */
bool nrf_802154_request_rssi_measurement_get(int8_t * p_rssi);

#if NRF_802154_DELAYED_TRX_ENABLED
/**
 * @brief Requests a call to @ref nrf_802154_delayed_trx_transmit.
 *
 * @param[in]  p_data      Pointer to the array with data to transmit. The first byte must contain
 *                         the frame length (including FCS). The following bytes contain data.
 *                         The CRC is computed automatically by the radio hardware. Therefore,
 *                         the FCS field can contain any bytes.
 * @param[in]  tx_time     Absolute time used by the SL timer, in microseconds (us).
 * @param[in]  p_metadata  Pointer to metadata structure. Contains detailed properties of data
 *                         to transmit. If @c NULL following metadata are used:
 *                         Field           | Value
 *                         ----------------|-----------------------------------------------------
 *                         @c frame_props  | @ref NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT
 *                         @c cca          | @c true
 *                         @c channel      | As returned by @ref nrf_802154_channel_get
 *
 * @retval  true   The transmission procedure was scheduled.
 * @retval  false  The driver could not schedule the transmission procedure.
 */
bool nrf_802154_request_transmit_raw_at(uint8_t                                 * p_data,
                                        uint64_t                                  tx_time,
                                        const nrf_802154_transmit_at_metadata_t * p_metadata);

/**
 * @brief Requests a call to @ref nrf_802154_delayed_trx_transmit_cancel.
 *
 * @retval  true    The delayed transmission was scheduled and successfully cancelled.
 * @retval  false   No delayed transmission was scheduled.
 */
bool nrf_802154_request_transmit_at_cancel(void);

/**
 * @brief Requests a call to @ref nrf_802154_delayed_trx_receive.
 *
 * @param[in]   rx_time  Absolute time used by the SL Timer, in microseconds (us).
 * @param[in]   timeout  Reception timeout (counted from @p rx_time), in microseconds (us).
 * @param[in]   channel  Radio channel on which the frame is to be received.
 * @param[in]   id       Identifier of the scheduled reception window. If the reception has been
 *                       scheduled successfully, the value of this parameter can be used in
 *                       @ref nrf_802154_receive_at_cancel to cancel it.
 *
 * @retval  true   The reception procedure was scheduled.
 * @retval  false  The driver could not schedule the reception procedure.
 */
bool nrf_802154_request_receive_at(uint64_t rx_time,
                                   uint32_t timeout,
                                   uint8_t  channel,
                                   uint32_t id);

/**
 * @brief Requests a call to @ref nrf_802154_delayed_trx_receive_cancel.
 *
 * @param[in]  id  Identifier of the delayed reception window to be cancelled. If the provided
 *                 value does not refer to any scheduled or active receive window, the function
 *                 returns false.
 *
 * @retval  true    The delayed reception was scheduled and successfully cancelled.
 * @retval  false   No delayed reception was scheduled.
 */
bool nrf_802154_request_receive_at_cancel(uint32_t id);

/**
 * @brief Requests a call to @ref nrf_802154_request_receive_at_scheduled_cancel.
 *
 * @param[in]  id  Identifier of the delayed reception window to be cancelled. If the provided
 *                 value does not refer to any scheduled or active receive window, the function
 *                 returns true.
 *
 * @retval  true    The delayed reception was scheduled and successfully cancelled
 *                  or no receive window with given ID was found.
 * @retval  false   No delayed reception was scheduled.
 */
bool nrf_802154_request_receive_at_scheduled_cancel(uint32_t id);

#endif // NRF_802154_DELAYED_TRX_ENABLED

/**
 * @brief Requests CSMA-CA procedure for the transmission of a given frame.
 *
 * @param[in]  p_data      Pointer to a buffer the contains PHR and PSDU of the frame that is
 *                         to be transmitted.
 * @param[in]  p_metadata  Pointer to metadata structure. Contains detailed properties of data
 *                         to transmit.
 */
bool nrf_802154_request_csma_ca_start(uint8_t                                      * p_data,
                                      const nrf_802154_transmit_csma_ca_metadata_t * p_metadata);

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_REQUEST_H__
