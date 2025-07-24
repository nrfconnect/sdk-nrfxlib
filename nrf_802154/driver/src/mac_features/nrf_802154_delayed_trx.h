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

#ifndef NRF_802154_DELAYED_TRX_H__
#define NRF_802154_DELAYED_TRX_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_const.h"
#include "nrf_802154_config.h"
#include "nrf_802154_types.h"

#if NRF_802154_DELAYED_TRX_ENABLED || defined(__DOXYGEN__)

/**
 * @defgroup nrf_802154_delayed_trx Delayed transmission and reception window features
 * @{
 * @ingroup nrf_802154
 * @brief Delayed transmission or receive window.
 *
 * This module implements delayed transmission and receive window features used in the CSL and TSCH
 * modes.
 */

/**
 * @brief Initializes delayed transmission and reception window features.
 */
void nrf_802154_delayed_trx_init(void);

/**
 * @brief Deinitializes delayed transmission and reception window features.
 */
void nrf_802154_delayed_trx_deinit(void);

/**
 * @brief Requests transmission of a frame at a given time.
 *
 * If the requested transmission is successful and the frame is transmitted,
 * the @ref nrf_802154_transmitted_raw function is called. If the requested frame
 * cannot be transmitted at the given time, the @ref nrf_802154_transmit_failed
 * function is called.
 *
 * @param[in]  p_data       Pointer to a buffer containing PHR and PSDU of the frame to be
 *                          transmitted.
 * @param[in]  tx_time      Absolute time used by the SL Timer, in microseconds (us).
 * @param[in]  p_metadata   Pointer to metadata structure. Contains detailed properties of data
 *                          to transmit and additional parameters for the procedure.
 */
bool nrf_802154_delayed_trx_transmit(uint8_t                                 * p_data,
                                     uint64_t                                  tx_time,
                                     const nrf_802154_transmit_at_metadata_t * p_metadata);

/**
 * @brief Cancels a transmission scheduled by a call to @ref nrf_802154_delayed_trx_transmit.
 *
 * This function does not cancel transmission if the transmission is already ongoing.
 *
 * @retval true     Successfully cancelled a scheduled transmission.
 * @retval false    No delayed transmission was scheduled.
 */
bool nrf_802154_delayed_trx_transmit_cancel(void);

/**
 * @brief Requests the reception of a frame at a given time.
 *
 * If the request is accepted and a frame is received during the defined time slot,
 * the @ref nrf_802154_received_raw function is called. If the request is rejected due
 * to a denied timeslot request or the reception timeout expires,
 * the @ref nrf_802154_receive_failed function is called.
 *
 * @note The identifier @p id must be unique. It must not have the same value as identifiers
 * of other delayed timeslots active at the moment, so that it can be mapped unambiguously
 * to an active delayed operation if the request is successful. In particular, none of the reserved
 * identifiers can be used.
 *
 * @param[in]  rx_time  Absolute time used by the SL Timer, in microseconds (us).
 * @param[in]  timeout  Reception timeout (counted from @p rx_time) in microseconds.
 * @param[in]  channel  Number of the channel on which the frame is to be received.
 * @param[in]  id       Identifier of the scheduled reception window. If the reception has been
 *                      scheduled successfully, the value of this parameter can be used in
 *                      @ref nrf_802154_delayed_trx_receive_cancel to cancel it.
 *
 * @retval  true   The reception procedure was scheduled.
 * @retval  false  The driver could not schedule the reception procedure.
 */
bool nrf_802154_delayed_trx_receive(uint64_t rx_time,
                                    uint32_t timeout,
                                    uint8_t  channel,
                                    uint32_t id);

/**
 * @brief Cancels a reception scheduled by a call to @ref nrf_802154_delayed_trx_receive.
 *
 * After a call to this function, no reception timeout event will be notified.
 *
 * @param[in]  id  Identifier of the delayed reception window to be cancelled. If the provided
 *                 value does not refer to any scheduled or active receive window, the function
 *                 returns false.
 *
 * @retval true     Successfully cancelled a scheduled transmission.
 * @retval false    No delayed reception was scheduled.
 */
bool nrf_802154_delayed_trx_receive_cancel(uint32_t id);

/**
 * @brief Cancels a scheduled reception scheduled by a call to @ref nrf_802154_delayed_trx_receive.
 *
 * If the receive window is currently ongoing, it will not be affected and a timeout event will
 * be notified at scheduled time.
 *
 * @param[in]  id  Identifier of the delayed reception window to be cancelled. If the provided
 *                 value does not refer to any scheduled or active receive window, the function
 *                 returns true.
 *
 * @retval true     Successfully cancelled a scheduled transmission or no window
 *                  with given ID is scheduled.
 * @retval false    The receive window is currently ongoing.
 */
bool nrf_802154_delayed_trx_receive_scheduled_cancel(uint32_t id);

/**
 * @brief Aborts an ongoing delayed reception procedure.
 *
 * @param[in]  term_lvl  Termination level set by the request to abort the ongoing operation.
 * @param[in]  req_orig  Module that originates this request.
 *
 * If the delayed transmission/reception procedures are not running during the call,
 * this function does nothing.
 *
 * @retval  true   Transmission/reception procedures have been stopped.
 * @retval  false  Transmission/reception procedures were not running.
 *
 */
bool nrf_802154_delayed_trx_abort(nrf_802154_term_t term_lvl, req_originator_t req_orig);

/**
 * @brief Extends the timeout timer when the reception start is detected and there is not enough
 *        time left for a delayed RX operation.
 *
 * @param[in]  p_frame  Pointer to a buffer that contains PHR and PSDU of the frame
 *                      that is being received.
 *
 * If the delayed transmission/reception procedures are not running during call,
 * this function does nothing.
 *
 */
void nrf_802154_delayed_trx_rx_started_hook(const uint8_t * p_frame);

/**
 * @brief Gets the time in microseconds to the midpoint of the nearest scheduled DRX window.
 *
 * @param[out]  p_drx_time_to_midpoint  Pointer to a 32-bit variable where the time to the
 *                                      midpoint of the nearest DRX will be stored.
 *
 * @retval  true   A pending DRX was scheduled and p_drx_time_to_start contains a valid value.
 * @retval  false  A pending DRX was not scheduled and p_drx_time_to_start was not modified.
 *
 */
bool nrf_802154_delayed_trx_nearest_drx_time_to_midpoint_get(uint32_t * p_drx_time_to_midpoint);

/**
 *@}
 **/

#endif // NRF_802154_DELAYED_TRX_ENABLED

#endif // NRF_802154_DELAYED_TRX_H__
