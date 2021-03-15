/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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
 * @file nrf_802154.h
 * @brief This file mimics API provided by nrf_802154.h but provides only serialized functions.
 *        We are trying to keep them as simillar as currently possible.
 */

#ifndef NRF_802154_H_
#define NRF_802154_H_

#include <stdint.h>
#include <stdbool.h>

#include "nrf_802154_callouts.h"
#include "nrf_802154_config.h"
#include "nrf_802154_types.h"

/**
 * @brief Select the source matching algorithm.
 *
 * @note This method should be called after driver initialization, but before transceiver is enabled.
 *
 * When calling @ref nrf_802154_ack_data_pending_bit_should_be_set, one of several algorithms
 * for source address matching will be chosen. To ensure a specific algorithm is selected,
 * call this function before @ref rf_802154_ack_data_pending_bit_should_be_set.
 *
 * @param[in]  match_method Source address matching method to be used.
 */
void nrf_802154_src_addr_matching_method_set(nrf_802154_src_addr_match_t match_method);

/**
 * @brief Enables or disables setting a pending bit in automatically transmitted ACK frames.
 *
 * @note Setting a pending bit in automatically transmitted ACK frames is enabled by default.
 *
 * The radio driver automatically sends ACK frames in response frames destined for this node with
 * the ACK Request bit set. The pending bit in the ACK frame can be set or cleared regarding data
 * in the indirect queue destined for the ACK destination.
 *
 * If setting a pending bit in ACK frames is disabled, the pending bit in every ACK frame is set.
 * If setting a pending bit in ACK frames is enabled, the radio driver checks if there is data
 * in the indirect queue destined for the  ACK destination. If there is no such data,
 * the pending bit is cleared.
 *
 * @note Due to the ISR latency, the radio driver might not be able to verify if there is data
 *       in the indirect queue before ACK is sent. In this case, the pending bit is set.
 *
 * @param[in]  enabled  If setting a pending bit in ACK frames is enabled.
 */
void nrf_802154_auto_pending_bit_set(bool enabled);

/**
 * @brief Adds the address of a peer node to the pending bit list.
 *
 * The pending bit list works differently, depending on the upper layer for which the source
 * address matching method is selected:
 *   - For Thread, @ref NRF_802154_SRC_ADDR_MATCH_THREAD
 *   - For Zigbee, @ref NRF_802154_SRC_ADDR_MATCH_ZIGBEE
 *   - For Standard-compliant, @ref NRF_802154_SRC_ADDR_MATCH_ALWAYS_1
 * For more information, see @ref nrf_802154_src_addr_match_t.
 *
 * The method can be set during initialization phase by calling @ref nrf_802154_src_matching_method.
 *
 * @note This function makes a copy of the given address.
 *
 * @param[in]  p_addr    Array of bytes containing the address of the node (little-endian).
 * @param[in]  extended  If the given address is an extended MAC address or a short MAC address.
 *
 * @retval True   The address is successfully added to the list.
 * @retval False  Not enough memory to store the address in the list.
 */
bool nrf_802154_pending_bit_for_addr_set(const uint8_t * p_addr, bool extended);

/**
 * @brief Removes address of a peer node from the pending bit list.
 *
 * The pending bit list works differently, depending on the upper layer for which the source
 * address matching method is selected:
 *   - For Thread, @ref NRF_802154_SRC_ADDR_MATCH_THREAD
 *   - For Zigbee, @ref NRF_802154_SRC_ADDR_MATCH_ZIGBEE
 *   - For Standard-compliant, @ref NRF_802154_SRC_ADDR_MATCH_ALWAYS_1
 * For more information, see @ref nrf_802154_src_addr_match_t.
 *
 * The method can be set during initialization phase by calling @ref nrf_802154_src_matching_method.
 *
 * @param[in]  p_addr    Array of bytes containing the address of the node (little-endian).
 * @param[in]  extended  If the given address is an extended MAC address or a short MAC address.
 *
 * @retval True   The address is successfully removed from the list.
 * @retval False  No such address in the list.
 */
bool nrf_802154_pending_bit_for_addr_clear(const uint8_t * p_addr, bool extended);

/**
 * @brief Removes all addresses of a given type from the pending bit list.
 *
 * The pending bit list works differently, depending on the upper layer for which the source
 * address matching method is selected:
 *   - For Thread, @ref NRF_802154_SRC_ADDR_MATCH_THREAD
 *   - For Zigbee, @ref NRF_802154_SRC_ADDR_MATCH_ZIGBEE
 *   - For Standard-compliant, @ref NRF_802154_SRC_ADDR_MATCH_ALWAYS_1
 * For more information, see @ref nrf_802154_src_addr_match_t.
 *
 * The method can be set during initialization phase by calling @ref nrf_802154_src_matching_method.
 *
 * @param[in]  extended  If the function is to remove all extended MAC addresses or all short
 *                       addresses.
 */
void nrf_802154_pending_bit_for_addr_reset(bool extended);

/**
 * @brief Initializes the 802.15.4 driver.
 *
 * This function initializes the RADIO peripheral in the @ref RADIO_STATE_SLEEP state.
 *
 * @note This function is to be called once, before any other functions from this module.
 */
void nrf_802154_init(void);

/**
 * @brief Changes the radio state to the @ref RADIO_STATE_SLEEP state.
 *
 * The sleep state is the lowest power state. In this state, the radio cannot transmit or receive
 * frames. It is the only state in which the driver releases the high-frequency clock and does not
 * request timeslots from a radio arbiter.
 *
 * @note If another module requests it, the high-frequency clock may be enabled even in the radio
 *       sleep state.
 *
 * @retval  true   The radio changes its state to the low power mode.
 * @retval  false  The driver could not schedule changing state.
 */
bool nrf_802154_sleep(void);

/**
 * @brief Changes the radio state to @ref RADIO_STATE_RX.
 *
 * In the receive state, the radio receives frames and may automatically send ACK frames when
 * appropriate. The received frame is reported to the higher layer by a call to
 * @ref nrf_802154_received.
 *
 * @retval  true   The radio enters the receive state.
 * @retval  false  The driver could not enter the receive state.
 */
bool nrf_802154_receive(void);

/** @brief Sets the channel on which the radio is to operate.
 *
 * @param[in]  channel  Channel number (11-26).
 */
void nrf_802154_channel_set(uint8_t channel);

/**
 * @brief Gets the channel on which the radio operates.
 *
 * @returns  Channel number (11-26).
 */
uint8_t nrf_802154_channel_get(void);

/**
 * @brief Sets the PAN ID used by the device.
 *
 * @param[in]  p_pan_id  Pointer to the PAN ID (2 bytes, little-endian).
 *
 * This function makes a copy of the PAN ID.
 */
void nrf_802154_pan_id_set(const uint8_t * p_pan_id);

/**
 * @brief Sets the short address of the device.
 *
 * @param[in]  p_short_address  Pointer to the short address (2 bytes, little-endian).
 *
 * This function makes a copy of the address.W
 */
void nrf_802154_short_address_set(const uint8_t * p_short_address);

/**
 * @brief Sets the extended address of the device.
 *
 * @param[in]  p_extended_address  Pointer to the extended address (8 bytes, little-endian).
 *
 * This function makes a copy of the address.
 */
void nrf_802154_extended_address_set(const uint8_t * p_extended_address);

/**
 * @brief Configures the device as the PAN coordinator.
 *
 * @note That information is used for packet filtering.
 *
 * @param[in]  enabled  The radio is configured as the PAN coordinator.
 */
void nrf_802154_pan_coord_set(bool enabled);

/**
 * @brief Enables or disables the promiscuous radio mode.
 *
 * @note The promiscuous mode is disabled by default.
 *
 * In the promiscuous mode, the driver notifies the higher layer that it received any frame
 * (regardless frame type or destination address).
 * In normal mode (not promiscuous), the higher layer is not notified about ACK frames and frames
 * with unknown type. Also, frames with a destination address not matching the device address are
 * ignored.
 *
 * @param[in]  enabled  If the promiscuous mode is to be enabled.
 */
void nrf_802154_promiscuous_set(bool enabled);

/*
 * @brief Changes the radio state to @ref RADIO_STATE_CCA.
 *
 * @note @ref nrf_802154_cca_done can be called before this function returns a result.
 *
 * In the CCA state, the radio verifies if the channel is clear. The result of the verification is
 * reported to the higher layer by @ref nrf_802154_cca_done.
 *
 * @retval  true   The CCA procedure was scheduled.
 * @retval  false  The driver could not schedule the CCA procedure.
 */
bool nrf_802154_cca(void);

/**
 * @brief Changes the radio state to energy detection.
 *
 * In the energy detection state, the radio detects the maximum energy for a given time.
 * The result of the detection is reported to the higher layer by @ref nrf_802154_energy_detected.
 *
 * @note @ref nrf_802154_energy_detected can be called before this function returns a result.
 * @note Performing the energy detection procedure can take longer than requested in @p time_us.
 *       The procedure is performed only during the timeslots granted by a radio arbiter.
 *       It can be interrupted by other protocols using the radio hardware. If the procedure is
 *       interrupted, it is automatically continued and the sum of time periods during which the
 *       procedure is carried out is not less than the requested @p time_us.
 *
 * @param[in]  time_us   Duration of energy detection procedure. The given value is rounded up to
 *                       multiplication of 8 symbols (128 us).
 *
 * @retval  true   The energy detection procedure was scheduled.
 * @retval  false  The driver could not schedule the energy detection procedure.
 */
bool nrf_802154_energy_detection(uint32_t time_us);

/**
 * @brief Changes the radio state to @ref RADIO_STATE_TX.
 *
 * @note If the CPU is halted or interrupted while this function is executed,
 *       @ref nrf_802154_transmitted or @ref nrf_802154_transmit_failed can be called before this
 *       function returns a result.
 *
 * @note This function is implemented in zero-copy fashion. It passes the given buffer pointer to
 *       the RADIO peripheral.
 *
 * In the transmit state, the radio transmits a given frame. If requested, it waits for
 * an ACK frame. Depending on @ref NRF_802154_ACK_TIMEOUT_ENABLED, the radio driver automatically
 * stops waiting for an ACK frame or waits indefinitely for an ACK frame. If it is configured to
 * wait, the MAC layer is responsible for calling @ref nrf_802154_receive or
 * @ref nrf_802154_sleep after the ACK timeout.
 * The transmission result is reported to the higher layer by calls to @ref nrf_802154_transmitted
 * or @ref nrf_802154_transmit_failed.
 *
 * @verbatim
 * p_data
 * v
 * +-----+-----------------------------------------------------------+------------+
 * | PHR | MAC header and payload                                    | FCS        |
 * +-----+-----------------------------------------------------------+------------+
 *       |                                                                        |
 *       | <---------------------------- PHR -----------------------------------> |
 * @endverbatim
 *
 * @param[in]  p_data  Pointer to the array with data to transmit. The first byte must contain frame
 *                     length (including PHR and FCS). The following bytes contain data. The CRC is
 *                     computed automatically by the radio hardware. Therefore, the FCS field can
 *                     contain any bytes.
 * @param[in]  cca     If the driver is to perform a CCA procedure before transmission.
 *
 * @retval  true   The transmission procedure was scheduled.
 * @retval  false  The driver could not schedule the transmission procedure.
 */
bool nrf_802154_transmit_raw(const uint8_t * p_data, bool cca);

/**
 * @brief Performs the CSMA-CA procedure and transmits a frame in case of success.
 *
 * The end of the CSMA-CA procedure is notified by @ref nrf_802154_transmitted_raw or
 * @ref nrf_802154_transmit_failed.
 *
 * @note The driver may be configured to automatically time out waiting for an ACK frame depending
 *       on @ref NRF_802154_ACK_TIMEOUT_ENABLED. If the automatic ACK timeout is disabled,
 *       the CSMA-CA procedure does not time out waiting for an ACK frame if a frame
 *       with the ACK request bit set was transmitted. The MAC layer is expected to manage the timer
 *       to time out waiting for the ACK frame. This timer can be started
 *       by @ref nrf_802154_tx_started. When the timer expires, the MAC layer is expected
 *       to call @ref nrf_802154_receive or @ref nrf_802154_sleep to stop waiting for the ACK frame.
 *
 * @param[in]  p_data  Pointer to the frame to transmit. See also @ref nrf_802154_transmit_raw.
 */
void nrf_802154_transmit_csma_ca_raw(const uint8_t * p_data);

/**
 * @brief Notifies the driver that the buffer containing the received frame is not used anymore.
 *
 * @note The buffer pointed to by @p p_data may be modified by this function.
 * @note This function can be safely called only from the main context. To free the buffer from
 *       a callback or the IRQ context, use @ref nrf_802154_buffer_free_immediately_raw.
 *
 * @param[in]  p_data  Pointer to the buffer containing the received data that is no longer needed
 *                     by the higher layer.
 */
void nrf_802154_buffer_free_raw(uint8_t * p_data);

/**
 * @brief Sets the transmit power.
 *
 * @note The driver recalculates the requested value to the nearest value accepted by the hardware.
 *       The calculation result is rounded up.
 *
 * @param[in]  power  Transmit power in dBm.
 */
void nrf_802154_tx_power_set(int8_t power);

/**
 * @brief Gets the currently set transmit power.
 *
 * @returns Currently used transmit power, in dBm.
 */
int8_t nrf_802154_tx_power_get(void);

/**
 * @brief  Converts the energy level received during the energy detection procedure to a dBm value.
 *
 * @param[in]  energy_level  Energy level passed by @ref nrf_802154_energy_detected.
 *
 * @return  Result of the energy detection procedure in dBm.
 */
int8_t nrf_802154_dbm_from_energy_level_calculate(uint8_t energy_level);

/**
 * @brief Gets nRF 802.15.4 Radio Diver Capabilities.
 *
 * @return Capabilities of the radio driver.
 */
nrf_802154_capabilities_t nrf_802154_capabilities_get(void);

#endif
