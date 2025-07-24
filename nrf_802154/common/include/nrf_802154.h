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
 * @defgroup nrf_802154 802.15.4 radio driver
 * @{
 *
 */

#ifndef NRF_802154_H_
#define NRF_802154_H_

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_callouts.h"
#include "nrf_802154_config.h"
#include "nrf_802154_types.h"
#include "nrf_802154_common_utils.h"

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
#include "nrf_802154_irq_handlers.h"
#include "nrf_802154_sl_ant_div.h"
#endif // !NRF_802154_SERIALIZATION_HOST

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Timestamp value indicating that the timestamp is inaccurate.
 */
#define NRF_802154_NO_TIMESTAMP                0

/**
 * @brief Invalid delayed timeslot identifier.
 */
#define NRF_802154_RESERVED_INVALID_ID         UINT32_MAX

/**
 * @brief Reception window identifier reserved for immediate reception.
 */
#define NRF_802154_RESERVED_IMM_RX_WINDOW_ID   (UINT32_MAX - 1)

/**
 * @brief Upper bound for delayed reception window identifiers used by the application.
 *
 * All integers ranging from 0 to @ref NRF_802154_RESERVED_DRX_ID_UPPER_BOUND (inclusive)
 * can be used by the application as identifiers of delayed reception windows.
 */
#define NRF_802154_RESERVED_DRX_ID_UPPER_BOUND (UINT32_MAX - 4)

/**
 * @brief Maximum number of simultaneously pending notifications the driver can issue.
 *
 * This parameter allows to determine the correct size for structures that process notifications
 * issued by the driver. It accumulates the maximum number of simultaneously pending notifications
 * that can result from successfully received frames, disregardable notifications, all supported
 * delayed operations and the latest requested immediate operation.
 */
#define NRF_802154_MAX_PENDING_NOTIFICATIONS \
    (NRF_802154_RX_BUFFERS + NRF_802154_MAX_DISREGARDABLE_NOTIFICATIONS + 4 + 1)

/**
 * @brief Initializes the 802.15.4 driver.
 *
 * This function initializes the RADIO peripheral in the @ref RADIO_STATE_SLEEP state.
 *
 * @note This function is to be called once, before any other functions from this module.
 *       Only the functions setting the configuration can be called before this call.
 */
void nrf_802154_init(void);

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Deinitializes the 802.15.4 driver.
 *
 * This function deinitializes the RADIO peripheral and resets it to the default state.
 */
void nrf_802154_deinit(void);
#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @brief Sets the channel on which the radio is to operate.
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
 * @returns Currently used real total transmit power, in dBm.
 */
int8_t nrf_802154_tx_power_get(void);

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)

/**
 * @brief Sets the antenna diversity rx mode.
 *
 * @note This function should not be called while reception or transmission are currently ongoing.
 *
 * @param[in] mode Antenna diversity rx mode to be set.
 *
 * @retval true  Antenna diversity rx mode set successfully.
 * @retval false Invalid mode passed as argument.
 */
bool nrf_802154_antenna_diversity_rx_mode_set(nrf_802154_sl_ant_div_mode_t mode);

/**
 * @brief Gets current antenna diversity rx mode.
 *
 * @return Current antenna diversity mode for rx.
 */
nrf_802154_sl_ant_div_mode_t nrf_802154_antenna_diversity_rx_mode_get(void);

/**
 * @brief Sets the antenna diversity tx mode.
 *
 * @note This function should not be called while reception or transmission are currently ongoing.
 * @note NRF_802154_SL_ANT_DIV_MODE_AUTO is not supported for transmission.
 *
 * @param[in] mode Antenna diversity tx mode to be set.
 *
 * @retval true  Antenna diversity tx mode set successfully.
 * @retval false Invalid mode passed as argument.
 */
bool nrf_802154_antenna_diversity_tx_mode_set(nrf_802154_sl_ant_div_mode_t mode);

/**
 * @brief Gets current antenna diversity tx mode.
 *
 * @return Current antenna diversity mode for tx.
 */
nrf_802154_sl_ant_div_mode_t nrf_802154_antenna_diversity_tx_mode_get(void);

/**
 * @brief Manually selects the antenna to be used for rx.
 *
 * For antenna to be switched, antenna diversity rx mode needs
 * to be @c NRF_802154_SL_ANT_DIV_MODE_MANUAL. Otherwise, antenna will
 * be only switched after @c NRF_802154_SL_ANT_DIV_MODE_MANUAL is set.
 *
 * @param[in] antenna Antenna to be used.
 *
 * @retval true  Antenna set successfully.
 * @retval false Invalid antenna passed as argument.
 */
bool nrf_802154_antenna_diversity_rx_antenna_set(nrf_802154_sl_ant_div_antenna_t antenna);

/**
 * @brief Gets antenna currently used for rx.
 *
 * @note The antenna read by this function is currently used rx antenna only if
 * antenna diversity rx mode is set to @c NRF_802154_SL_ANT_DIV_MODE_MANUAL. Otherwise,
 * currently used antenna may be different.
 *
 * @return Currently used antenna.
 */
nrf_802154_sl_ant_div_antenna_t nrf_802154_antenna_diversity_rx_antenna_get(void);

/**
 * @brief Manually selects the antenna to be used for tx.
 *
 * For antenna to be switched, antenna diversity tx mode needs
 * to be @c NRF_802154_SL_ANT_DIV_MODE_MANUAL. Otherwise, antenna will
 * be only switched after @c NRF_802154_SL_ANT_DIV_MODE_MANUAL is set.
 *
 * @param[in] antenna Antenna to be used.
 *
 * @retval true  Antenna set successfully.
 * @retval false Invalid antenna passed as argument.
 */
bool nrf_802154_antenna_diversity_tx_antenna_set(nrf_802154_sl_ant_div_antenna_t antenna);

/**
 * @brief Gets antenna currently used for tx.
 *
 * @note The antenna read by this function is currently used tx antenna only if
 * antenna diversity tx mode is set to @c NRF_802154_SL_ANT_DIV_MODE_MANUAL. Otherwise,
 * currently used antenna may be different.
 *
 * @return Currently used antenna.
 */
nrf_802154_sl_ant_div_antenna_t nrf_802154_antenna_diversity_tx_antenna_get(void);

/**
 * @brief Gets which antenna was selected as best for the last reception.
 *
 * @note In three cases @c NRF_802154_SL_ANT_DIV_ANTENNA_NONE may be returned:
 *  - No frame was received yet.
 *  - Last frame was received with antenna diversity auto mode disabled.
 *  - RSSI measurements didn't have enough time to finish during last frame reception
 *    and it is unspecified which antenna was selected.
 *
 * @return Antenna selected during last successful reception in automatic mode.
 */
nrf_802154_sl_ant_div_antenna_t nrf_802154_antenna_diversity_last_rx_best_antenna_get(void);

/**
 * @brief Sets antenna diversity configuration.
 *
 * @note If antenna diversity feature is to be used, this function must be called before
 * @ref nrf_802154_antenna_diversity_init.
 *
 * @note This function must be called only once.
 *
 * @param[in] p_cfg Pointer to antenna diversity interface configuration.
 */
void nrf_802154_antenna_diversity_config_set(const nrf_802154_sl_ant_div_cfg_t * p_cfg);

/**
 * @brief Gets the antenna diversity interface configuration.
 *
 * @param[out] p_cfg  Antenna diversity interface configuration.
 *
 * @retval true  The configuration was retrieved successfully.
 * @retval false The configuration could not be retrieved.
 */
bool nrf_802154_antenna_diversity_config_get(nrf_802154_sl_ant_div_cfg_t * p_cfg);

/**
 * @brief Initializes antenna diversity module.
 *
 * This function should be called before starting radio operations, but at any time
 * after driver initialization. In order for it to succeed, antenna diversity interface
 * configuration must be provided before it's called with
 * @ref nrf_802154_antenna_diversity_config_set. Example usage:
 *
 * @code
 * nrf_802154_init();
 *
 * nrf_802154_sl_ant_div_cfg_t cfg =
 * {
 *     // Set the configuration parameters accordingly
 * };
 *
 * nrf_802154_antenna_config_set(&cfg);
 * nrf_802154_antenna_diversity_init();
 *
 * // At any later time
 * nrf_802154_receive();
 * @endcode
 *
 * @retval true   Initialization was successful.
 * @retval false  Initialization could not be performed due to unconfigured interface.
 */
bool nrf_802154_antenna_diversity_init(void);

/**
 * @brief Handles TIMER IRQ of the antenna diversity interface.
 *
 * This function should be called when the timer instance provided to the antenna diversity
 * interface reports an interrupt.
 */
void nrf_802154_antenna_diversity_timer_irq_handler(void);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @brief Gets the current time.
 *
 * The time returned by this function is to be used to calculate timing parameters for
 * @ref nrf_802154_transmit_raw_at and @ref nrf_802154_receive_at functions.
 *
 * @returns Current time in microseconds.
 */
uint64_t nrf_802154_time_get(void);

/**
 * @defgroup nrf_802154_addresses Setting addresses and PAN ID of the device
 * @{
 */

/**
 * @brief Sets the PAN ID used by the device.
 *
 * @param[in]  p_pan_id  Pointer to the PAN ID (2 bytes, little-endian).
 *
 * This function makes a copy of the PAN ID.
 */
void nrf_802154_pan_id_set(const uint8_t * p_pan_id);

/**
 * @brief Sets the extended address of the device.
 *
 * @param[in]  p_extended_address  Pointer to the extended address (8 bytes, little-endian).
 *
 * This function makes a copy of the address.
 */
void nrf_802154_extended_address_set(const uint8_t * p_extended_address);

/**
 * @brief Sets the short address of the device.
 *
 * @param[in]  p_short_address  Pointer to the short address (2 bytes, little-endian).
 *
 * This function makes a copy of the address.
 */
void nrf_802154_short_address_set(const uint8_t * p_short_address);

/**
 * @brief Sets the alternate short address of the device.
 *
 * The alternate short address should be used when you need to change the
 * device short address without loss of connectivity.
 * The API addresses a race condition, where a remote peer sends a frame destined
 * to the old device address, but this device has already changed the short address
 * to a new one. The alternate address should be cleared after a while, after the new
 * short address becomes known to the peers.
 *
 * The API is meant to be used as follows:
 * 1. Set the alternate short address to the current old address.
 * 2. Set the primary short address (@ref nrf_802154_short_address_set) to the new address.
 * 3. Wait until the new short address becomes known among the peers.
 * 4. Clear the alternate short address.
 *
 * @param[in]  p_short_address  Pointer to the short address (2 bytes, little-endian).
 *                              Setting this value to NULL clears the alternate address.
 *
 * This function makes a copy of the address.
 */
void nrf_802154_alternate_short_address_set(const uint8_t * p_short_address);

/**
 * @}
 * @defgroup nrf_802154_transitions Functions to request FSM transitions
 * @{
 */

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
 * @brief Changes the radio state to the @ref RADIO_STATE_SLEEP state if the radio is idle.
 *
 * The sleep state is the lowest power state. In this state, the radio cannot transmit or receive
 * frames. It is the only state in which the driver releases the high-frequency clock and does not
 * request timeslots from a radio arbiter.
 *
 * @note If another module requests it, the high-frequency clock may be enabled even in the radio
 *       sleep state.
 *
 * @retval  NRF_802154_SLEEP_ERROR_NONE  The radio changes its state to the low power mode.
 * @retval  NRF_802154_SLEEP_ERROR_BUSY  The driver could not schedule changing state.
 */
nrf_802154_sleep_error_t nrf_802154_sleep_if_idle(void);

/**
 * @brief Changes the radio state to @ref RADIO_STATE_RX.
 *
 * In the receive state, the radio receives frames and may automatically send ACK frames when
 * appropriate. The received frame is reported to the higher layer by a call to
 * @ref nrf_802154_received_raw or @ref nrf_802154_received_timestamp_raw .
 *
 * @retval  true   The radio enters the receive state.
 * @retval  false  The driver could not enter the receive state.
 */
bool nrf_802154_receive(void);

/**
 * @brief Requests reception at the specified time.
 *
 * This function works as a delayed version of @ref nrf_802154_receive. It is asynchronous.
 * It queues the delayed reception using the Radio Scheduler module.
 * If the delayed reception cannot be performed (@ref nrf_802154_receive_at would return false)
 * or the requested reception timeslot is denied, @ref nrf_802154_receive_failed is called
 * with the @ref NRF_802154_RX_ERROR_DELAYED_TIMESLOT_DENIED argument.
 *
 * If the requested reception time is in the past, the function returns false and does not
 * schedule reception.
 *
 * The reception ends after the configured timeout has elapsed. This event is notified to the
 * higher layer through the @ref nrf_802154_receive_failed notification with
 * @ref NRF_802154_RX_ERROR_DELAYED_TIMEOUT status code.
 *
 * A scheduled reception can be cancelled by a call to @ref nrf_802154_receive_at_cancel or
 * @ref nrf_802154_receive_at_scheduled_cancel.
 *
 * @note The identifier @p id must be unique. It must not have the same value as identifiers
 * of other delayed timeslots active at the moment, so that it can be mapped unambiguously
 * to an active delayed operation if the request is successful. In particular, none of the reserved
 * identifiers can be used.
 *
 * @param[in]   rx_time  Absolute time used by the SL Timer, in microseconds (us).
 * @param[in]   timeout  Reception timeout (counted from @p rx_time), in microseconds (us).
 * @param[in]   channel  Radio channel on which the frame is to be received.
 * @param[in]   id       Identifier of the scheduled reception window. If the reception has been
 *                       scheduled successfully, the value of this parameter can be used in
 *                       @ref nrf_802154_receive_at_cancel or
 *                       @ref nrf_802154_receive_at_scheduled_cancel to cancel it.
 *
 * @retval  true   The reception procedure was scheduled.
 * @retval  false  The driver could not schedule the reception procedure.
 */
bool nrf_802154_receive_at(uint64_t rx_time,
                           uint32_t timeout,
                           uint8_t  channel,
                           uint32_t id);

/**
 * @brief Cancels a delayed reception scheduled by a call to @ref nrf_802154_receive_at.
 *
 * If the receive window has been scheduled but has not started yet, this function prevents
 * entering the receive window. If the receive window has been scheduled and has already started,
 * the radio remains in the receive state, but a window timeout will not be reported.
 *
 * @param[in]  id  Identifier of the delayed reception window to be cancelled. If the provided
 *                 value does not refer to any scheduled or active receive window, the function
 *                 returns false.
 *
 * @retval  true    The delayed reception was scheduled and successfully cancelled.
 * @retval  false   No delayed reception was scheduled.
 */
bool nrf_802154_receive_at_cancel(uint32_t id);

/**
 * @brief Cancels a delayed reception scheduled by a call to @ref nrf_802154_receive_at.
 *
 * If the receive window has been scheduled but has not started yet, this function prevents
 * entering the receive window. If the receive window has been scheduled and has already started,
 * the receive window is not affected and will continue until its scheduled timeout.
 *
 * The function also returns success when no window with given ID is scheduled and is not currently
 * ongoing.
 *
 * @note This function differs from @ref nrf_802154_receive_at_cancel in two aspects:
 *       1. This function can only cancel receive windows that are scheduled, but haven't started yet.
 *          If the receive window has already started, the cancel will end with failure and the
 *          receive window will last for the planned duration, ending with
 *          @ref nrf_802154_receive_failed notification with @ref NRF_802154_RX_ERROR_DELAYED_TIMEOUT
 *          status.
 *       2. If there are no scheduled and ongoing receive windows matching the given ID,
 *          the function ends with a success.
 *
 * @param[in]  id  Identifier of the delayed reception window to be cancelled. If the provided
 *                 value does not refer to any scheduled or active receive window, the function
 *                 returns true.
 *
 * @retval  true    The delayed reception was scheduled and successfully cancelled or the
 *                  receive window was not scheduled at all.
 * @retval  false   The scheduled window is currently ongoing.
 */
bool nrf_802154_receive_at_scheduled_cancel(uint32_t id);

/**
 * @brief Changes the radio state to @ref RADIO_STATE_TX.
 *
 * @note If the CPU is halted or interrupted while this function is executed,
 *       @ref nrf_802154_transmitted_raw or @ref nrf_802154_transmit_failed can be called before this
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
 * The transmission result is reported to the higher layer by calls to @ref nrf_802154_transmitted_raw
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
 * @param[in]  p_data      Pointer to the array with data to transmit. The first byte must contain
 *                         frame length (including FCS). The following bytes contain data.
 *                         The CRC is computed automatically by the radio hardware. Therefore,
 *                         the FCS field can contain any bytes.
 * @param[in]  p_metadata  Pointer to metadata structure. Contains detailed properties of data
 *                         to transmit. If @c NULL following metadata are used:
 *                         Field           | Value
 *                         ----------------|-----------------------------------------------------
 *                         @c frame_props  | @ref NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT
 *                         @c cca          | @c true
 *
 * @retval  true   The transmission procedure was scheduled.
 * @retval  false  The driver could not schedule the transmission procedure.
 */
bool nrf_802154_transmit_raw(uint8_t                              * p_data,
                             const nrf_802154_transmit_metadata_t * p_metadata);

/**
 * @brief Requests transmission at the specified time.
 *
 * @note This function is implemented in a zero-copy fashion. It passes the given buffer pointer to
 *       the RADIO peripheral.
 *
 *
 * This function works as a delayed version of @ref nrf_802154_transmit_raw. It is asynchronous.
 * It queues the delayed transmission using the Radio Scheduler module and performs it
 * at the specified time.
 *
 * If the delayed transmission is successfully performed, @ref nrf_802154_transmitted_raw is called.
 * If the delayed transmission cannot be performed ( @ref nrf_802154_transmit_raw would return @c false)
 * or the requested transmission timeslot is denied, @ref nrf_802154_transmit_failed with the
 * @ref NRF_802154_TX_ERROR_TIMESLOT_DENIED argument is called.
 *
 * Additional CCA attempts can be requested by the caller through @p p_metadata. In that case CCA
 * procedure is repeated back-to-back either until it returns idle channel and the transmission
 * starts, or until 1 + @p p_metadata->extra_cca_attempts CCA attempts are performed. The maximum
 * allowed number of additional CCA attempts is 254 so that the total number of CCA attempts doesn't
 * exceed 255.
 *
 * This function is designed to transmit the first symbol of SHR at @p tx_time provided that
 * the number of CCA attempts that will be performed is deterministic, which is only the case when
 * no attempts are performed or when a single attempt is performed. Otherwise, the function assumes
 * that @p tx_time points to a moment where the transmission would start if the first CCA attempt
 * detected idle channel. If the first CCA detects busy channel and additional CCA attempts follow,
 * the moment of transmission (first symbol of SHR) is delayed by the time taken by additional CCA
 * attempts, which is a multiple of the duration of a single CCA attempt.
 *
 * The below diagram shows an example sequence where two additional CCA attempts are performed.
 * TT denotes turnaround time necessary to switch from CCA to Tx. @p tx_time points to a moment in
 * time that occurs TT after the first CCA finishes.
 *
 * @verbatim
 *           tx_time           frame start
 *              |                   |
 *              |                   |
 *              v                   v
 * +---------+---------+---------+--+----------+-----+-------------------------+
 * |   CCA   |   CCA   |   CCA   |TT|    SHR   | PHR | MAC Header and payload  |
 * +---------+---------+---------+--+----------+-----+-------------------------+
 *           |         |         |
 *           v         v         v
 *          BUSY      BUSY      IDLE
 * @endverbatim
 *
 * If the requested transmission time is in the past, the function returns @c false and does not
 * schedule transmission.
 *
 * A successfully scheduled transmission can be cancelled by a call
 * to @ref nrf_802154_transmit_at_cancel.
 *
 * @param[in]  p_data      Pointer to the array with data to transmit. The first byte must contain
 *                         the frame length (including FCS). The following bytes contain data.
 *                         The CRC is computed automatically by the radio hardware. Therefore,
 *                         the FCS field can contain any bytes.
 * @param[in]  tx_time     Absolute time used by the SL Timer, in microseconds (us).
 * @param[in]  p_metadata  Pointer to metadata structure. Contains detailed properties of data
 *                         to transmit. If @c NULL following metadata are used:
 *                         Field                 | Value
 *                         ----------------------|-----------------------------------------------------
 *                         @c frame_props        | @ref NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT
 *                         @c cca                | @c true
 *                         @c channel            | As returned by @ref nrf_802154_channel_get
 *                         @c tx_power           | As set with @ref nrf_802154_tx_power_set
 *                         @c extra_cca_attempts | @c 0
 *
 * @retval  true   The transmission procedure was scheduled.
 * @retval  false  The driver could not schedule the transmission procedure.
 */
bool nrf_802154_transmit_raw_at(uint8_t                                 * p_data,
                                uint64_t                                  tx_time,
                                const nrf_802154_transmit_at_metadata_t * p_metadata);

/**
 * @brief Cancels a delayed transmission scheduled by a call to @ref nrf_802154_transmit_raw_at.
 *
 * If a delayed transmission has been scheduled but the transmission has not been started yet,
 * a call to this function prevents the transmission. If the transmission is ongoing,
 * it will not be aborted.
 *
 * If a delayed transmission has not been scheduled (or has already finished), this function does
 * not change state and returns false.
 *
 * @retval  true    The delayed transmission was scheduled and successfully cancelled.
 * @retval  false   No delayed transmission was scheduled.
 */
bool nrf_802154_transmit_at_cancel(void);

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

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Changes the radio state to continuous carrier.
 *
 * @note When the radio is emitting continuous carrier signals, it blocks all transmissions on the
 *       selected channel. This function is to be called only during radio tests. Do not
 *       use it during normal device operation.
 *
 * @retval  true   The continuous carrier procedure was scheduled.
 * @retval  false  The driver could not schedule the continuous carrier procedure.
 */
bool nrf_802154_continuous_carrier(void);

/**
 * @brief Changes the radio state to modulated carrier.
 *
 * @note When the radio is emitting modulated carrier signals, it blocks all transmissions on the
 *       selected channel. This function is to be called only during radio tests. Do not
 *       use it during normal device operation.
 *
 * @param[in] p_data Pointer to a buffer to modulate the carrier with. The first byte is the data length.
 *
 * @retval  true   The modulated carrier procedure was scheduled.
 * @retval  false  The driver could not schedule the modulated carrier procedure.
 */
bool nrf_802154_modulated_carrier(const uint8_t * p_data);

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @}
 * @defgroup nrf_802154_memman Driver memory management
 * @{
 */

/**
 * @brief Notifies the driver that the buffer containing the received frame is not used anymore.
 *
 * @note The buffer pointed to by @p p_data may be modified by this function.
 * @note This function can be safely called from the main context or callouts running on the
 *       same core as the driver. Do not call this function directly in the serialized callouts.
 *
 * @param[in]  p_data  Pointer to the buffer containing the received data that is no longer needed
 *                     by the higher layer.
 */
void nrf_802154_buffer_free_raw(uint8_t * p_data);

/**
 * @}
 * @defgroup nrf_802154_rssi RSSI measurement function
 * @{
 */

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Begins the RSSI measurement.
 *
 * @note This function is to be called in the @ref RADIO_STATE_RX state.
 *
 * The result will be available after the measurement process is finished. The result can be read by
 * @ref nrf_802154_rssi_last_get. Check the documentation of the RADIO peripheral to check
 * the duration of the RSSI measurement procedure.
 *
 * @retval true  RSSI measurement successfully requested.
 * @retval false RSSI measurement cannot be scheduled at the moment.
 */
bool nrf_802154_rssi_measure_begin(void);

/**
 * @brief Gets the result of the last RSSI measurement.
 *
 * @returns RSSI measurement result, in dBm.
 */
int8_t nrf_802154_rssi_last_get(void);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @}
 * @defgroup nrf_802154_prom Promiscuous mode
 * @{
 */

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

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Checks if the radio is in the promiscuous mode.
 *
 * @retval True   Radio is in the promiscuous mode.
 * @retval False  Radio is not in the promiscuous mode.
 */
bool nrf_802154_promiscuous_get(void);

/**
 * @}
 * @defgroup nrf_802154_autoack Auto ACK management
 * @{
 */

/**
 * @brief Enables or disables the automatic acknowledgments (auto ACK).
 *
 * @note The auto ACK is enabled by default.
 *
 * If the auto ACK is enabled, the driver prepares and sends ACK frames automatically
 * aTurnaroundTime (192 us) after the proper frame is received. The driver prepares an ACK frame
 * according to the data provided by @ref nrf_802154_ack_data_set.
 * When the auto ACK is enabled, the driver notifies the next higher layer about the received frame
 * after the ACK frame is transmitted.
 * If the auto ACK is disabled, the driver does not transmit ACK frames. It notifies the next higher
 * layer about the received frames when a frame is received. In this mode, the next higher layer is
 * responsible for sending the ACK frame. ACK frames should be sent using @ref nrf_802154_transmit_raw.
 *
 * @param[in]  enabled  If the auto ACK should be enabled.
 */
void nrf_802154_auto_ack_set(bool enabled);

/**
 * @brief Checks if the auto ACK is enabled.
 *
 * @retval True   Auto ACK is enabled.
 * @retval False  Auto ACK is disabled.
 */
bool nrf_802154_auto_ack_get(void);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @brief Configures the device as the PAN coordinator.
 *
 * @note That information is used for packet filtering.
 *
 * @param[in]  enabled  The radio is configured as the PAN coordinator.
 */
void nrf_802154_pan_coord_set(bool enabled);

#if NRF_802154_PAN_COORD_GET_ENABLED || defined(DOXYGEN)
/**
 * @brief Checks if the radio is configured as the PAN coordinator.
 *
 * @retval  true   The radio is configured as the PAN coordinator.
 * @retval  false  The radio is not configured as the PAN coordinator.
 */
bool nrf_802154_pan_coord_get(void);

#endif // NRF_802154_PAN_COORD_GET_ENABLED

/**
 * @brief Select the source matching algorithm.
 *
 * @note This method should be called after driver initialization, but before transceiver is enabled.
 *
 * When calling @ref nrf_802154_ack_data_pending_bit_should_be_set, one of several algorithms
 * for source address matching will be chosen. To ensure a specific algorithm is selected,
 * call this function before @ref nrf_802154_ack_data_pending_bit_should_be_set.
 *
 * @param[in]  match_method Source address matching method to be used.
 */
void nrf_802154_src_addr_matching_method_set(nrf_802154_src_addr_match_t match_method);

/**
 * @brief Adds the address of a peer node for which the provided ACK data
 * is to be injected into generated ACKs.
 *
 * Data passed to this function can be either pending bit data or Header IE data.
 *
 * The pending bit list works differently, depending on the upper layer for which the source
 * address matching method is selected:
 *   - For Thread, @ref NRF_802154_SRC_ADDR_MATCH_THREAD
 *   - For Zigbee, @ref NRF_802154_SRC_ADDR_MATCH_ZIGBEE
 *   - For Standard-compliant, @ref NRF_802154_SRC_ADDR_MATCH_ALWAYS_1
 * For more information, see @ref nrf_802154_src_addr_match_t.
 *
 * For IE data, specific cases are supported, where additional data will be injected into the IE on pre-transmission:
 *   - CSL IE - CSL phase will be injected if IE ID is set to @ref IE_CSL_ID
 *   - Thread link metrics - Link metrics will be injected if
 *      - IE ID is set to IE_VENDOR_ID
 *      - OUI is set to IE_VENDOR_THREAD_OUI
 *      - Thread IE subtype is set to IE_VENDOR_THREAD_ACK_PROBING_ID
 *
 * For Link metrics to be injected, additional preparation is required. Each byte of injected link metrics needs to be filled
 * with a token, indicating what type of data is to be injected pre-transmission. Supported tokens are:
 *   - @ref IE_VENDOR_THREAD_RSSI_TOKEN - RSSI of the last received frame will be injected,
 *   - @ref IE_VENDOR_THREAD_MARGIN_TOKEN - RSSI above sensitivity margin of the last received frame will be injected,
 *   - @ref IE_VENDOR_THREAD_LQI_TOKEN - LQI of the last received frame will be injected.
 * Additionally, tokens must be unique in given IE, all bytes prepared for link metrics must be filled with tokens and no more
 * than two bytes must be prepared for link metrics.
 * If any of those conditions is not met, no data will be injected into the ACK pre-transmission.
 *
 * To better illustrate, if RSSI is to be inserted into ACKs for specific address,
 * following ie data needs to be prepared:
 *
 * @verbatim
 * +------------+----------------------+---------------------------------+-----------------------------+
 * | Bytes: 0-1 |          2-4         |                5                |              6              |
 * +------------+----------------------+---------------------------------+-----------------------------+
 * | IE header  | IE_VENDOR_THREAD_OUI | IE_VENDOR_THREAD_ACK_PROBING_ID | IE_VENDOR_THREAD_RSSI_TOKEN |
 * +------------+----------------------+---------------------------------+-----------------------------+
 *                                     |                                                               |
 *                                     | <------------------IE Vendor-specific data------------------> |
 * @endverbatim
 *
 * When sending ACK with this data, before transmission, RSSI of the last received frame
 * will be written into byte 6.
 *
 * The method can be set during initialization phase by calling @ref nrf_802154_src_addr_matching_method_set.
 *
 * @param[in]  p_addr    Array of bytes containing the address of the node (little-endian).
 * @param[in]  extended  If the given address is an extended MAC address or a short MAC address.
 * @param[in]  p_data    Pointer to the buffer containing data to be set.
 * @param[in]  length    Length of @p p_data.
 * @param[in]  data_type Type of data to be set. Refer to the @ref nrf_802154_ack_data_t type.
 *
 * @retval True   Address successfully added to the list.
 * @retval False  Not enough memory to store this address in the list.
 */
bool nrf_802154_ack_data_set(const uint8_t       * p_addr,
                             bool                  extended,
                             const void          * p_data,
                             uint16_t              length,
                             nrf_802154_ack_data_t data_type);

/**
 * @brief Removes the address of a peer node for which the ACK data is set from the pending bit list.
 *
 * The ACK data that was previously set for the given address is automatically removed.
 *
 * The pending bit list works differently, depending on the upper layer for which the source
 * address matching method is selected:
 *   - For Thread, @ref NRF_802154_SRC_ADDR_MATCH_THREAD
 *   - For Zigbee, @ref NRF_802154_SRC_ADDR_MATCH_ZIGBEE
 *   - For Standard-compliant, @ref NRF_802154_SRC_ADDR_MATCH_ALWAYS_1
 * For more information, see @ref nrf_802154_src_addr_match_t.
 *
 * The method can be set during initialization phase by calling @ref nrf_802154_src_addr_matching_method_set.
 *
 * @param[in]  p_addr    Array of bytes containing the address of the node (little-endian).
 * @param[in]  extended  If the given address is an extended MAC address or a short MAC address.
 * @param[in]  data_type Type of data to be removed. Refer to the @ref nrf_802154_ack_data_t type.
 *
 * @retval True   Address removed from the list.
 * @retval False  Address not found in the list.
 */
bool nrf_802154_ack_data_clear(const uint8_t       * p_addr,
                               bool                  extended,
                               nrf_802154_ack_data_t data_type);

/**
 * @brief Removes all addresses of a given length from the ACK data list.
 *
 * @param[in]  extended  Indication if all extended addresses or all short addresses are
 *                       to be removed from the list.
 * @param[in]  data_type Type of data that is to be cleared for all addresses of a given length.
 */
void nrf_802154_ack_data_remove_all(bool extended, nrf_802154_ack_data_t data_type);

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
 * The method can be set during initialization phase by calling @ref nrf_802154_src_addr_matching_method_set.
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
 * The method can be set during initialization phase by calling @ref nrf_802154_src_addr_matching_method_set.
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
 * The method can be set during initialization phase by calling @ref nrf_802154_src_addr_matching_method_set.
 *
 * @param[in]  extended  If the function is to remove all extended MAC addresses or all short
 *                       addresses.
 */
void nrf_802154_pending_bit_for_addr_reset(bool extended);

/**
 * @}
 * @defgroup nrf_802154_cca CCA configuration management
 * @{
 */

/**
 * @brief Configures the radio CCA mode and threshold.
 *
 * @param[in]  p_cca_cfg  Pointer to the CCA configuration structure. Only fields relevant to
 *                        the selected mode are updated.
 */
void nrf_802154_cca_cfg_set(const nrf_802154_cca_cfg_t * p_cca_cfg);

/**
 * @brief Gets the current radio CCA configuration.
 *
 * @param[out]  p_cca_cfg  Pointer to the structure for the current CCA configuration.
 */
void nrf_802154_cca_cfg_get(nrf_802154_cca_cfg_t * p_cca_cfg);

/**
 * @}
 * @defgroup nrf_802154_csma CSMA-CA procedure
 * @{
 */
#if NRF_802154_CSMA_CA_ENABLED || defined(DOXYGEN)

/**
 * @brief Performs the CSMA-CA procedure and transmits a frame in case of success.
 *
 * The end of the CSMA-CA procedure is notified by @ref nrf_802154_transmitted_raw or
 * @ref nrf_802154_transmit_failed.
 *
 * @note The driver may be configured to automatically time out waiting for an ACK frame depending
 *       on @ref NRF_802154_ACK_TIMEOUT_ENABLED. If the automatic ACK timeout is disabled,
 *       the CSMA-CA procedure does not time out waiting for an ACK frame if a frame
 *       with the ACK request bit set was transmitted.
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @param[in]  p_data      Pointer to the frame to transmit. See also @ref nrf_802154_transmit_raw.
 * @param[in]  p_metadata  Pointer to metadata structure. Contains detailed properties of data
 *                         to transmit. If @c NULL following metadata are used:
 *                         Field           | Value
 *                         ----------------|-----------------------------------------------------
 *                         @c frame_props  | @ref NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT
 *
 * @retval  true   The chain of CSMA-CA and transmission procedure was scheduled.
 * @retval  false  The driver could not schedule the procedure chain.
 */
bool nrf_802154_transmit_csma_ca_raw(uint8_t                                      * p_data,
                                     const nrf_802154_transmit_csma_ca_metadata_t * p_metadata);

/**
 * @brief Sets the minimum value of the backoff exponent (BE) in the CSMA-CA algorithm.
 *
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @param[in] min_be  Minimum value of the backoff exponent.
 *
 * @retval true   When value provided by @p min_be has been set successfully.
 * @retval false  Otherwise.
 */
bool nrf_802154_csma_ca_min_be_set(uint8_t min_be);

/**
 * @brief Gets the minimum value of the backoff exponent (BE) in the CSMA-CA algorithm.
 *
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @return Current minimum value of the backoff exponent.
 */
uint8_t nrf_802154_csma_ca_min_be_get(void);

/**
 * @brief Sets the maximum value of the backoff exponent (BE) in the CSMA-CA algorithm.
 *
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @param[in] max_be  Maximum value of the backoff exponent.
 *
 * @retval true   When value provided by @p max_be has been set successfully.
 * @retval false  Otherwise.
 */
bool nrf_802154_csma_ca_max_be_set(uint8_t max_be);

/**
 * @brief Gets the maximum value of the backoff exponent (BE) in the CSMA-CA algorithm.
 *
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @return Current maximum value of the backoff exponent.
 */
uint8_t nrf_802154_csma_ca_max_be_get(void);

/**
 * @brief Sets the maximum number of backoffs the CSMA-CA algorithm will attempt before declaring
 *        a channel access failure.
 *
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @param[in] max_backoffs  Maximum number of backoffs.
 */
void nrf_802154_csma_ca_max_backoffs_set(uint8_t max_backoffs);

/**
 * @brief Gets the maximum number of backoffs the CSMA-CA algorithm will attempt before declaring
 *        a channel access failure.
 *
 * @note This function is available if @ref NRF_802154_CSMA_CA_ENABLED is enabled.
 *
 * @return Current maximum number of backoffs.
 */
uint8_t nrf_802154_csma_ca_max_backoffs_get(void);

#endif // NRF_802154_CSMA_CA_ENABLED

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @}
 * @defgroup nrf_802154_timeout ACK timeout procedure
 * @{
 */
#if NRF_802154_ACK_TIMEOUT_ENABLED || defined(DOXYGEN)

/**
 * @brief Sets timeout for the ACK timeout feature.
 *
 * A timeout is notified by @ref nrf_802154_transmit_failed.
 *
 * @note This function is available if @ref NRF_802154_ACK_TIMEOUT_ENABLED is enabled.
 *
 * @param[in]  time  Timeout in microseconds (us).
 *                   A default value is defined in nrf_802154_config.h.
 */
void nrf_802154_ack_timeout_set(uint32_t time);

#endif // !NRF_802154_SERIALIZATION_HOST

#endif // NRF_802154_ACK_TIMEOUT_ENABLED

/**
 * @}
 * @defgroup nrf_802154_coex Wifi Coex feature
 * @{
 */


#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Checks if wifi coex signaling is enabled.
 *
 * @retval true     Wifi coex signaling is enabled.
 * @retval false    Wifi coex signaling is disabled.
 */
bool nrf_802154_wifi_coex_is_enabled(void);

/**
 * @brief Sets Coex request mode used in receive operations.
 *
 * @param[in] mode  Coex receive request mode. For allowed values see @ref nrf_802154_coex_rx_request_mode_t type.
 *
 * @retval true     Operation succeeded.
 * @retval false    Requested mode is not supported.
 */
bool nrf_802154_coex_rx_request_mode_set(nrf_802154_coex_rx_request_mode_t mode);

/**
 * @brief Gets Coex request mode used in receive operations.
 *
 * @return Current Coex receive request mode. For allowed values see @ref nrf_802154_coex_rx_request_mode_t type.
 */
nrf_802154_coex_rx_request_mode_t nrf_802154_coex_rx_request_mode_get(void);

/**
 * @brief Sets Coex request mode used in transmit operations.
 *
 * @param[in] mode  Coex transmit request mode. For allowed values see @ref nrf_802154_coex_tx_request_mode_t type.
 *
 * @retval true     Operation succeeded.
 * @retval false    Requested mode is not supported.
 */
bool nrf_802154_coex_tx_request_mode_set(nrf_802154_coex_tx_request_mode_t mode);

/**
 * @brief Gets Coex request mode used in transmit operations.
 *
 * @return Current Coex transmit request mode. For allowed values see @ref nrf_802154_coex_tx_request_mode_t type.
 */
nrf_802154_coex_tx_request_mode_t nrf_802154_coex_tx_request_mode_get(void);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @}
 * @defgroup nrf_802154_stats Statistics and measurements
 * @{
 */

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Gets current statistics.
 *
 * @param[out] p_stats    Structure that will be filled with current stats values.
 */
void nrf_802154_stats_get(nrf_802154_stats_t * p_stats);

/**
 * @brief Get current statistics.
 *
 * @note This returns part of information returned by @ref nrf_802154_stats_get
 *
 * @param[out] p_stat_counters    Structure that will be filled with current stats counter values.
 */
void nrf_802154_stat_counters_get(nrf_802154_stat_counters_t * p_stat_counters);

/**
 * @brief Decreases current statistic counter values by the provided ones.
 *
 * This function is intended to be called together with @ref nrf_802154_stats_get
 * to avoid missing any counted events.
 *
 * @param[in] p_stat_counters Current stat counter values will be decreased by values provided
 *                            behind this pointer.
 */
void nrf_802154_stat_counters_subtract(const nrf_802154_stat_counters_t * p_stat_counters);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @brief Get time stamps of events gathered by the last operation.
 *
 * @param[out] p_stat_timestamps Structure that will be filled with current time stamps of events.
 */
void nrf_802154_stat_timestamps_get(nrf_802154_stat_timestamps_t * p_stat_timestamps);

#if !NRF_802154_SERIALIZATION_HOST || defined(DOXYGEN)
/**
 * @brief Resets current stat counters to 0.
 *
 * @note @ref nrf_802154_stat_counters_get and @ref nrf_802154_stat_counters_reset may lead to
 * missing events if an counted event occurs between these calls. Use
 * @ref nrf_802154_stat_counters_subtract to avoid such condition if necessary.
 */
void nrf_802154_stat_counters_reset(void);

#endif // !NRF_802154_SERIALIZATION_HOST

/**
 * @}
 * @defgroup nrf_802154_ifs Inter-frame spacing feature
 * @{
 */
#if NRF_802154_IFS_ENABLED || defined(DOXYGEN)

/**
 * @brief Gets IFS operation mode.
 *
 * @note This function is available if @ref NRF_802154_IFS_ENABLED is enabled.
 *
 * @return Current IFS operation mode. Refer to @ref nrf_802154_ifs_mode_t for details.
 */
nrf_802154_ifs_mode_t nrf_802154_ifs_mode_get(void);

/**
 * @brief Sets IFS operation mode.
 *
 * @note This function is available if @ref NRF_802154_IFS_ENABLED is enabled.
 *
 * @param[in] mode  IFS operation mode. Refer to @ref nrf_802154_ifs_mode_t for details.
 *
 * @retval    true  The update of IFS operation mode was successful.
 * @retval    false The update of IFS operation mode failed. Provided mode is unsupported
 */
bool nrf_802154_ifs_mode_set(nrf_802154_ifs_mode_t mode);

/**
 * @brief Gets Short IFS period in microseconds.
 *
 * @note This function is available if @ref NRF_802154_IFS_ENABLED is enabled.
 *
 * @return Current Short IFS period in microseconds.
 */
uint16_t nrf_802154_ifs_min_sifs_period_get(void);

/**
 * @brief Sets Short IFS period in microseconds.
 *
 * @note This function is available if @ref NRF_802154_IFS_ENABLED is enabled.
 *
 * @param[in] period Short IFS period in microseconds.
 */
void nrf_802154_ifs_min_sifs_period_set(uint16_t period);

/**
 * @brief Gets Long IFS period in microseconds.
 *
 * @note This function is available if @ref NRF_802154_IFS_ENABLED is enabled.
 *
 * @return Current Long IFS period in microseconds.
 */
uint16_t nrf_802154_ifs_min_lifs_period_get(void);

/**
 * @brief Sets Long IFS period in microseconds.
 *
 * @note This function is available if @ref NRF_802154_IFS_ENABLED is enabled.
 *
 * @param[in] period Long IFS period in microseconds.
 */
void nrf_802154_ifs_min_lifs_period_set(uint16_t period);

#endif // NRF_802154_IFS_ENABLED

/**
 * @}
 * @defgroup nrf_802154_capabilities Radio driver run-time capabilities feature.
 * @{
 */

/**
 * @brief Gets nRF 802.15.4 Radio Driver Capabilities.
 *
 * @return Capabilities of the radio driver.
 */
nrf_802154_capabilities_t nrf_802154_capabilities_get(void);

/**
 * @}
 * @defgroup nrf_802154_security Radio driver MAC security feature.
 * @{
 */

/**
 * @brief Sets nRF 802.15.4 Radio Driver Global MAC Frame Counter.
 *
 * The driver automatically increments the counter in every outgoing frame
 * which uses the Global MAC Frame Counter.
 * This call is meant to set the initial value of the frame counter.
 *
 * @param[in] frame_counter Global MAC Frame Counter to set.
 */
void nrf_802154_security_global_frame_counter_set(uint32_t frame_counter);

/**
 * @brief Sets nRF 802.15.4 Radio Driver MAC Global Frame Counter if the value passed is larger than current.
 *
 * @param[in] frame_counter Frame counter to set.
 */
void nrf_802154_security_global_frame_counter_set_if_larger(uint32_t frame_counter);

/**
 * @brief Store the 802.15.4 MAC Security Key inside the nRF 802.15.4 Radio Driver.
 *
 * @param[in] p_key Pointer to the key to store. Refer to @ref nrf_802154_key_t for details.
 *                  Storing the key copies the content of the key and key ID into the Radio Driver.
 *                  This input parameter can be destroyed after the call.
 *
 * @note This function is not reentrant and must be called from thread context only.
 *
 * @retval NRF_802154_SECURITY_ERROR_NONE               Storing of key is successful.
 * @retval NRF_802154_SECURITY_ERROR_TYPE_NOT_SUPPORTED Type of the key is not supported.
 * @retval NRF_802154_SECURITY_ERROR_MODE_NOT_SUPPORTED ID mode of the key is not supported.
 * @retval NRF_802154_SECURITY_ERROR_ALREADY_PRESENT    Failed to store the key - key of such id is already
 *                                                      present. Remove the key first to overwrite.
 * @retval NRF_802154_SECURITY_ERROR_STORAGE_FULL       Failed to store the key - storage full.
 */
nrf_802154_security_error_t nrf_802154_security_key_store(nrf_802154_key_t * p_key);

/**
 * @brief Remove the 802.15.4 MAC Security Key from the nRF 802.15.4 Radio Driver.
 *
 * @param[in] p_id Pointer to the ID of the key to remove.
 *
 * @note This function is not reentrant and must be called from thread context only.
 *
 * @retval NRF_802154_SECURITY_ERROR_NONE          Removal of key is successful.
 * @retval NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND Failed to remove the key - no such key found.
 */
nrf_802154_security_error_t nrf_802154_security_key_remove(nrf_802154_key_id_t * p_id);

/**
 * @brief Remove all stored 802.15.4 MAC Security Keys from the nRF 802.15.4 Radio Driver.
 *
 * @note This function is not reentrant and must be called from thread context only.
 */
void nrf_802154_security_key_remove_all(void);

/**
 * @}
 * @defgroup nrf_802154_ie_writer Radio driver Information Element data injection feature.
 * @{
 */

/**
 * @brief Sets the value of CSL period to inject into the CSL information element.
 *
 * @param[in]  period  CSL period value.
 */
void nrf_802154_csl_writer_period_set(uint16_t period);

/**
 * @brief Sets the anchor time based on which the next CSL window time and the CSL phase is calculated.
 *
 * This function sets an anchor time based on which the times of future CSL windows are calculated.
 * When this anchor time is used for calculations, it is assumed that it points to a time where
 * the first bit of MAC header of the frame received from a peer happens. In other words, the anchor
 * time should point to a time where CSL phase would be equal 0. As a result, CSL phase can always
 * be calculated relatively to a time given by the equation @c anchor_time + @c n * @c csl_period
 * where @c n is an integer. Note that the reasoning holds irrespectively of signedness of @c n
 * so the anchor time can be either in the past or in the future.
 *
 * This function should be called after calling @ref nrf_802154_csl_writer_period_set and every time
 * when the CSL communication desynchronizes.
 *
 * If this function is not called a legacy CSL operation mode is chosen. The CSL phase is then
 * calculated based on the time of the nearest scheduled CSL reception window and can be undefined,
 * if no such window was scheduled.
 *
 * @param[in]  anchor_time  Anchor time in microseconds.
 */
void nrf_802154_csl_writer_anchor_time_set(uint64_t anchor_time);

/**
 * @brief Sets the RxOnWhenIdle mode value.
 *
 * This function sets the RxOnWhenIdle mode value, which defaults to true. When enabled, the radio will
 * stay in receive state during idle periods. When disabled, the radio will stay in sleep state during
 * idle periods. The new value will only take effect after a completed operation.
 *
 * Notice that the period following a failed reception is not considered idle, rather continuation of
 * the reception state. Same goes for the finalization of a reception during a receive slot.
 *
 * An idle period is started in the following situations:
 * - After @ref nrf_802154_receive starts a receive state and a frame is correctly received and passed
 *   the filtering:
 *   - If the received frame requests an ACK: after the ACK was properly transmitted or immediately
 *     if for some internal condition the ACK is not transmitted.
 *   - If the received frame does not request an ACK: immediately, unless the received frame is an
 *     spurious ACK.
 * - Immediately after a frame is transmitted, when no ACK is requested.
 * - After a frame is transmitted, when ACK is requested, if:
 *   - ACK timeout expires.
 *   - An invalid ACK is received (or not an ACK frame).
 *   - The matching ACK is received, unless the transmitted frame was a Data Request Command and the
 *     frame pending bit is set to true in the ACK.
 * - After a standalone CCA is completed.
 * - After a failed CCA during CSMA/CA procedure.
 * - After every Energy Detection operation.
 * - After a delayed reception times out.
 *
 * Combining disabled RxOnWhenIdle mode and enabled promiscuous mode is unsupported. Such configuration
 * may result in an undefined behavior.
 *
 * @param[in]  rx_on_when_idle  If RxOnWhenIdle mode should be enabled.
 */
void nrf_802154_rx_on_when_idle_set(bool rx_on_when_idle);

/**
 * @brief Sets the value of CST period to inject into the CST information element.
 *
 * @param[in]  period  CST period value.
 */
void nrf_802154_cst_writer_period_set(uint16_t period);

/**
 * @brief Sets the anchor time based on which the next CST window time and the CST phase is calculated.
 *
 * This function sets an anchor time which is a time of a CST window, based which on the times of future CST windows are
 * calculated. It is assumed that all other CST windows occur at time @c anchor_time + @c n * @c cst_period where @c n is
 * an integer. Note that the anchor time can be both in the past and in the future.
 *
 * @param[in]  anchor_time  Anchor time value.
 */
void nrf_802154_cst_writer_anchor_time_set(uint64_t anchor_time);

/**
 * @}
 * @defgroup nrf_802154_test_modes Test modes
 * @{
 */

#if NRF_802154_TEST_MODES_ENABLED
/**
 * @brief Gets the current CSMA/CA backoff test mode.
 *
 * @return Current CSMA/CA backoff test mode.
 */
nrf_802154_test_mode_csmaca_backoff_t nrf_802154_test_mode_csmaca_backoff_get(void);

/**
 * @brief Sets the csmaca backoff test mode.
 *
 * @param[in] value     CSMA/CA backoff test mode (See @ref nrf_802154_test_mode_csmaca_backoff_t
 *                      for defined values).
 */
void nrf_802154_test_mode_csmaca_backoff_set(nrf_802154_test_mode_csmaca_backoff_t value);

#endif // NRF_802154_TEST_MODES_ENABLED

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_H_ */

/** @} */
