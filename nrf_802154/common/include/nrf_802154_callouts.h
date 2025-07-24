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

/**@file nrf_802154_callouts.h
 * @brief Provides function prototypes required by nRF 802.15.4 Radio Driver
 *
 * Functions whose prototypes are defined in this file are to be implemented
 * by an application using the nRF 802.15.4 Radio Driver.
 */

/**
 * @defgroup nrf_802154_calls Calls to higher layer
 * @{
 */

#ifndef NRF_802154_CALLOUTS_H_
#define NRF_802154_CALLOUTS_H_

#include <stdint.h>
#include <stdbool.h>

#include "nrf_802154_config.h"
#include "nrf_802154_types.h"

/**
 * @brief Notifies that the CCA procedure has finished.
 *
 * @param[in]  channel_free  Indication if the channel is free.
 */
extern void nrf_802154_cca_done(bool channel_free);

/**
 * @brief Notifies that the CCA procedure failed.
 *
 * @param[in]  error  Reason of the failure.
 */
extern void nrf_802154_cca_failed(nrf_802154_cca_error_t error);

/**
 * @brief Notifies that the energy detection procedure finished.
 *
 * @param[in]  p_result     Pointer to structure containing the result of the operation.
 *                          The pointer is valid within the @ref nrf_802154_energy_detected only.
 */
extern void nrf_802154_energy_detected(const nrf_802154_energy_detected_t * p_result);

/**
 * @brief Notifies that the energy detection procedure failed.
 *
 * @param[in]  error  Reason of the failure.
 */
extern void nrf_802154_energy_detection_failed(nrf_802154_ed_error_t error);

/**
 * @brief Notifies about the start of the ACK frame transmission.
 *
 * @note If a call to this function is performed by the same CPU running core
 * of nRF 802.15.4 Radio Driver (non serialized call to @ref nrf_802154_tx_ack_started)
 * the function must be very short to prevent dropping frames by the driver.
 * If a call to this function is performed by a CPU through a serialization layer
 * the call can be slightly delayed. The call can happen even after an ACK frame
 * is fully transmitted. It is guaranteed that a call to @ref nrf_802154_tx_ack_started
 * occurs before a call to @ref nrf_802154_received_timestamp_raw related to
 * the same received frame.
 *
 * @param[in]  p_data  Pointer to a buffer with PHR and PSDU of the ACK frame.
 */
extern void nrf_802154_tx_ack_started(const uint8_t * p_data);

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Notifies that a frame was received.
 *
 * @note Currently this callout is only available on the CPU which is running the core of the radio driver.
 *       If the higher layer runs on a different core it should use nrf_802154_received_timestamp_raw instead.
 * @note The buffer pointed to by @p p_data is not modified by the radio driver (and cannot be used
 *       to receive a frame) until @ref nrf_802154_buffer_free_raw is called.
 * @note The buffer pointed to by @p p_data may be modified by the function handler (and other
 *       modules) until @ref nrf_802154_buffer_free_raw is called.
 * @note Default implementation of this function provided by the nRF 802.15.4 Radio Driver
 *       calls @ref nrf_802154_received_timestamp_raw .
 *
 * @verbatim
 * p_data
 * v
 * +-----+-----------------------------------------------------------+------------+
 * | PHR | MAC Header and payload                                    | FCS        |
 * +-----+-----------------------------------------------------------+------------+
 *       |                                                                        |
 *       | <---------------------------- PHR -----------------------------------> |
 * @endverbatim
 *
 * @param[in]  p_data  Pointer to a buffer that contains PHR and PSDU of the received frame.
 *                     The first byte in the buffer is the length of the frame (PHR). The following
 *                     bytes contain the frame itself (PSDU). The length byte (PHR) includes FCS.
 *                     FCS is already verified by the hardware and may be modified by the hardware.
 * @param[in]  power   RSSI of the received frame.
 * @param[in]  lqi     LQI of the received frame.
 */
extern void nrf_802154_received_raw(uint8_t * p_data, int8_t power, uint8_t lqi);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @brief Notifies that a frame was received at a given time.
 *
 * This function works like @ref nrf_802154_received_raw and adds a timestamp to the parameter
 * list.
 *
 * @note The received frame usually contains a timestamp. However, due to a race condition,
 *       the timestamp may be invalid. This erroneous situation is indicated by
 *       the @ref NRF_802154_NO_TIMESTAMP value of the @p time parameter.
 *
 * @param[in]  p_data  Pointer to a buffer that contains PHR and PSDU of the received frame.
 *                     The first byte in the buffer is the length of the frame (PHR). The following
 *                     bytes contain the frame itself (PSDU). The length byte (PHR) includes FCS.
 *                     FCS is already verified by the hardware and may be modified by the hardware.
 * @param[in]  power   RSSI of the received frame.
 * @param[in]  lqi     LQI of the received frame.
 * @param[in]  time    Timestamp taken when the last symbol of the frame was received, in
 *                     microseconds (us), or @ref NRF_802154_NO_TIMESTAMP if the timestamp
 *                     is invalid.
 */
extern void nrf_802154_received_timestamp_raw(uint8_t * p_data,
                                              int8_t    power,
                                              uint8_t   lqi,
                                              uint64_t  time);

/**
 * @brief Notifies that the reception of a frame failed.
 *
 * @param[in]  error  Error code that indicates the reason of the failure.
 * @param[in]  id     Identifier of reception window the error occurred in.
 *                    If the error is related to a delayed reception window requested through
 *                    @ref nrf_802154_receive_at, the value of @p id equals the identifier
 *                    of the scheduled reception window. Otherwise, the value of @p id equals
 *                    @ref NRF_802154_RESERVED_IMM_RX_WINDOW_ID.
 */
extern void nrf_802154_receive_failed(nrf_802154_rx_error_t error, uint32_t id);

/**
 * @brief Notifies that a frame was transmitted.
 *
 * @note If ACK was requested for the transmitted frame, this function is called after a proper ACK
 *       is received. If ACK was not requested, this function is called just after transmission has
 *       ended.
 * @note The buffer pointed to by @c nrf_802154_transmit_done_metadata_t::data.transmitted.p_ack
 *       is not modified by the radio driver (and cannot be used to receive a frame) until
 *       @ref nrf_802154_buffer_free_raw is called.
 * @note The buffer pointed to by @c nrf_802154_transmit_done_metadata_t::data.transmitted.p_ack
 *       may be modified by the function handler (and other modules) until
 *       @ref nrf_802154_buffer_free_raw is called.
 * @note @c nrf_802154_transmit_done_metadata_t::data.transmitted.time granularity depends on the
 *       granularity of the timer driver in the
 *       platform/timer directory.
 *
 * @param[in]  p_frame      Pointer to a buffer that contains PHR and PSDU of the transmitted frame.
 * @param[in]  p_metadata   Pointer to a metadata structure describing frame passed in @p p_frame.
 */
extern void nrf_802154_transmitted_raw(uint8_t                                   * p_frame,
                                       const nrf_802154_transmit_done_metadata_t * p_metadata);

/**
 * @brief Notifies that a frame was not transmitted due to a busy channel.
 *
 * This function is called if the transmission procedure fails.
 *
 * @note Frame data values in @ref nrf_802154_transmit_done_metadata_t::data are invalid for
 *       @ref nrf_802154_transmit_failed callout.
 *
 * @param[in]  p_frame      Pointer to a buffer that contains PHR and PSDU of the frame that was not
 *                          transmitted.
 * @param[in]  error        Reason of the failure.
 * @param[in]  p_metadata   Pointer to a metadata structure describing frame passed in @p p_frame.
 */
extern void nrf_802154_transmit_failed(uint8_t                                   * p_frame,
                                       nrf_802154_tx_error_t                       error,
                                       const nrf_802154_transmit_done_metadata_t * p_metadata);

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Perform some additional operations during initialization of the RADIO peripheral.
 *
 * By implementing this function the higher layer can provide some additional operations
 * to be performed at the beginning of each new timeslot. These can in particular be
 * modifications of RADIO peripheral register values.
 */
extern void nrf_802154_custom_part_of_radio_init(void);

#endif // !NRF_802154_SERIALIZATION_HOST

#endif /* NRF_802154_CALLOUTS_H_ */

/** @} */
