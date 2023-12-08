/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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
 * @brief 802.15.4 antenna diversity module.
 */

#ifndef NRF_802154_SL_ANT_DIV_H
#define NRF_802154_SL_ANT_DIV_H

#include <stdint.h>
#include <stdbool.h>

#include "nrfx.h"

/**
 * @brief RSSI measurement results.
 */
#define NRF_802154_SL_ANT_DIV_RSSI_INVALID           INT8_MAX

/**
 * @brief Priority of TIMER IRQ.
 */
#define NRF_802154_SL_ANT_DIV_IRQ_PRIORITY           1

/**
 * @brief Recommended value of time in microseconds between consecutive antenna switches.
 *
 * When antenna diversity interface operates in @ref NRF_802154_SL_ANT_DIV_MODE_AUTO mode,
 * the antenna is toggled periodically in order to maximize the chance of receiving a frame
 * preamble. The time between consecutive antenna toggles is configured through
 * @ref nrf_802154_sl_ant_div_cfg_set function. The value specified by this macro is the
 * recommended value of @p toggle_time parameter that should be set if no particular reasons
 * for a different configuration exist.
 */
#define NRF_802154_SL_ANT_DIV_DEFAULT_TOGGLE_TIME_US 40

/**
 * @brief Mode of the antenna diversity module.
 *
 * Possible values:
 * - @ref NRF_802154_SL_ANT_DIV_MODE_DISABLED,
 * - @ref NRF_802154_SL_ANT_DIV_MODE_MANUAL,
 * - @ref NRF_802154_SL_ANT_DIV_MODE_AUTO
 */
typedef uint8_t nrf_802154_sl_ant_div_mode_t;

#define NRF_802154_SL_ANT_DIV_MODE_DISABLED 0x00 // !< Antenna diversity is disabled - Antenna will not be controlled by sl_ant_div module. While in this mode, current antenna is unspecified.
#define NRF_802154_SL_ANT_DIV_MODE_MANUAL   0x01 // !< Antenna is selected manually
#define NRF_802154_SL_ANT_DIV_MODE_AUTO     0x02 // !< Antenna is selected automatically based on RSSI - not supported for transmission.

/**
 * @brief Available antennas
 *
 * Possible values:
 * - @ref NRF_802154_SL_ANT_DIV_ANTENNA_1,
 * - @ref NRF_802154_SL_ANT_DIV_ANTENNA_2,
 * - @ref NRF_802154_SL_ANT_DIV_ANTENNA_NONE
 */
typedef uint8_t nrf_802154_sl_ant_div_antenna_t;

#define NRF_802154_SL_ANT_DIV_ANTENNA_1    0x00 // !< First antenna
#define NRF_802154_SL_ANT_DIV_ANTENNA_2    0x01 // !< Second antenna
#define NRF_802154_SL_ANT_DIV_ANTENNA_NONE 0x02 // !< Used to indicate that antenna for the last reception was not selected via antenna diversity algorithm.

/**
 * @brief Types of operations supported by antenna diversity module.
 *
 * Possible values:
 * - @ref NRF_802154_SL_ANT_DIV_OP_RX,
 * - @ref NRF_802154_SL_ANT_DIV_OP_TX
 */
typedef uint8_t nrf_802154_sl_ant_div_op_t;

#define NRF_802154_SL_ANT_DIV_OP_RX 0x00 // !< RX-related operations
#define NRF_802154_SL_ANT_DIV_OP_TX 0x01 // !< TX-related operations

/**
 * @brief Configuration of the antenna diversity module.
 *
 * The following configuration parameters are to be set before
 * @ref nrf_802154_sl_ant_div_init is called.
 */
typedef struct
{
    /**
     * @brief Pin used for antenna selection.
     */
    uint8_t ant_sel_pin;

    /**
     * @brief Time in microseconds between antenna switches in @ref NRF_802154_SL_ANT_DIV_MODE_AUTO.
     */
    uint8_t toggle_time;

    /**
     * @brief PPI channel instance used to connect TIMER event with antenna toggling task.
     *
     * Antenna diversity interface claims exclusive access to the provided PPI channel.
     */
    uint8_t ppi_ch;

    /**
     * @brief GPIOTE channel instance used to toggle antenna when triggered with PPI.
     *
     * Antenna diversity interface claims exclusive access to the provided GPIOTE channel.
     */
    uint8_t gpiote_ch;

    /**
     * @brief Timer instance used to measure time between consecutive antenna toggles.
     *
     * Antenna diversity interface claims exclusive access to the provided TIMER peripheral.
     */
    NRF_TIMER_Type * p_timer;
} nrf_802154_sl_ant_div_cfg_t;

/**
 * @}
 * @defgroup nrf_802154_ant_div_cfg Antenna diversity configuration
 * @{
 */

/**
 * @brief Configures the antenna diversity interface.
 *
 * @note This function must be called before @ref nrf_802154_sl_ant_div_init
 * and can only be called once.
 *
 * @param[in] p_cfg  Antenna diversity interface configuration.
 */
void nrf_802154_sl_ant_div_cfg_set(const nrf_802154_sl_ant_div_cfg_t * p_cfg);

/**
 * @brief Gets the antenna diversity interface configuration.
 *
 * @param[out] p_cfg  Antenna diversity interface configuration.
 *
 * @retval true  The configuration was retrieved successfully.
 * @retval false The configuration could not be retrieved.
 */
bool nrf_802154_sl_ant_div_cfg_get(nrf_802154_sl_ant_div_cfg_t * p_cfg);

/**
 * @brief Sets the antenna diversity mode.
 *
 * @param[in] op   Type of antenna diversity operations the mode should be changed for.
 * @param[in] mode Antenna diversity mode to be set.
 *
 * @retval true  Antenna diversity mode set successfully.
 * @retval false Invalid antenna diversity mode or operation type passed as arguments.
 */
bool nrf_802154_sl_ant_div_cfg_mode_set(nrf_802154_sl_ant_div_op_t   op,
                                        nrf_802154_sl_ant_div_mode_t mode);

/**
 * @brief Gets the antenna diversity mode.
 *
 * @param[in] op  Type of antenna diversity operations the mode should be retrieved for.
 *
 * @return Current antenna diversity mode.
 */
nrf_802154_sl_ant_div_mode_t nrf_802154_sl_ant_div_cfg_mode_get(nrf_802154_sl_ant_div_op_t op);

/**
 * @brief Selects antenna to be used in @ref NRF_802154_SL_ANT_DIV_MODE_MANUAL mode.
 *
 * @note Takes effect only if antenna diversity mode is set to
 * @ref NRF_802154_SL_ANT_DIV_MODE_MANUAL. See @ref nrf_802154_sl_ant_div_mode_set.
 *
 * @param[in] op       Type of antenna diversity operations the antenna should be changed for.
 * @param[in] antenna  Antenna to be selected.
 *
 * @retval true  Antenna selected successfully.
 * @retval false Invalid antenna or operation type passed as arguments.
 */
bool nrf_802154_sl_ant_div_cfg_antenna_set(nrf_802154_sl_ant_div_op_t      op,
                                           nrf_802154_sl_ant_div_antenna_t antenna);

/**
 * @brief Reads the currently used antenna.
 *
 * @note The antenna read by this function is currently used antenna only if antenna diversity
 * mode is set to @ref NRF_802154_SL_ANT_DIV_MODE_MANUAL. Otherwise, currently used antenna
 * may be different. @sa nrf_802154_sl_ant_div_mode_set
 *
 * @param[in] op       Type of antenna diversity operations the antenna should be retrieved for.
 *
 * @return Currently selected antenna.
 */
nrf_802154_sl_ant_div_antenna_t nrf_802154_sl_ant_div_cfg_antenna_get(
    nrf_802154_sl_ant_div_op_t op);

/**
 * @}
 * @defgroup nrf_802154_ant_div Antenna diversity API
 * @{
 */

/**
 * @brief Initializes antenna diversity module.
 *
 * @note Before this function is called, antenna diversity interface must be configured with
 * @ref nrf_802154_sl_ant_div_cfg_set.
 *
 * @retval true   Initialization was successful.
 * @retval false  Initialization could not be performed due to unconfigured interface.
 */
bool nrf_802154_sl_ant_div_init(void);

/**
 * @brief Selects an antenna to use.
 *
 * This function has no effect if called when antenna diversity module is toggling antenna, i.e.:
 *  - @ref NRF_802154_SL_ANT_DIV_MODE_AUTO is set
 *  - receiver is turned on
 *  - no PPDU is currently being received
 *
 * @param[in] antenna  Antenna to be used.
 *
 * @retval true  Antenna switched successfully.
 * @retval false Invalid antenna passed to the function.
 */
bool nrf_802154_sl_ant_div_antenna_set(nrf_802154_sl_ant_div_antenna_t antenna);

/**
 * @brief Gets currently used antenna.
 *
 * @retval NRF_802154_SL_ANT_DIV_ANTENNA_1     If antenna 1 is used.
 * @retval NRF_802154_SL_ANT_DIV_ANTENNA_2     If antenna 2 is used.
 * @retval NRF_802154_SL_ANT_DIV_ANTENNA_NONE  If the antenna diversity interface is not configured.
 */
nrf_802154_sl_ant_div_antenna_t nrf_802154_sl_ant_div_antenna_get(void);

/**
 * @brief Gets which antenna was selected as best for the last reception.
 *
 * @note In three cases @ref NRF_802154_SL_ANT_DIV_ANTENNA_NONE may be returned:
 *  - No frame was received yet.
 *  - Last frame was received with antenna diversity auto mode disabled.
 *  - RSSI measurements didn't have enough time to finish during last frame reception
 *    and it is unspecified which antenna was selected.
 *
 * @return Antenna selected during last successful reception in automatic mode.
 */
nrf_802154_sl_ant_div_antenna_t nrf_802154_sl_ant_div_last_rx_best_antenna_get(void);

/**
 * @brief Handles TIMER IRQ of the antenna diversity interface.
 *
 * This function should be called when the timer instance provided to the antenna diversity
 * interface reports an interrupt.
 */
void nrf_802154_sl_ant_div_timer_irq_handle(void);

/**
 * @}
 * @defgroup nrf_802154_ant_div_callout Antenna diversity callouts
 * @{
 */

/**
 * @brief Callout for RSSI measurement.
 *
 * RSSI needs to be settled already after enabling RX or switching antenna.
 *
 * @note This function must be called from critical section.
 *
 * @return Corrected measured RSSI value.
 */
extern int8_t nrf_802154_sl_ant_div_rssi_measure_get(void);

/**
 * @}
 * @defgroup nrf_802154_ant_div_notification Antenna diversity notifications
 * @{
 */

/**
 * @brief Notification to be called when antenna diversity auto mode is enabled.
 */
void nrf_802154_sl_ant_div_rx_auto_mode_enable_notify(void);

/**
 * @brief Notification to be called when antenna diversity auto mode is disabled.
 */
void nrf_802154_sl_ant_div_rx_auto_mode_disable_notify(void);

/**
 * @brief Notification to be called when radio rx is started.
 */
void nrf_802154_sl_ant_div_rx_started_notify(void);

/**
 * @brief Notification to be called when radio rx is aborted.
 */
void nrf_802154_sl_ant_div_rx_aborted_notify(void);

/**
 * @brief Notification to be called when preamble is detected.
 */
void nrf_802154_sl_ant_div_rx_preamble_detected_notify(void);

/**
 * @brief Notification to be called when frame start is detected during reception.
 *
 * @retval true  RSSI measurements have finished and currently selected antenna is optimal for reception.
 * @retval false RSSI measurements have not yet finished and currently selected antenna is random.
 */
bool nrf_802154_sl_ant_div_rx_frame_started_notify(void);

/**
 * @brief Notification to be called when frame is received successfully.
 */
void nrf_802154_sl_ant_div_rx_frame_received_notify(void);

/**
 * @brief Notification to be called when timeout expires after preamble detection.
 */
void nrf_802154_sl_ant_div_rx_preamble_timeout_notify(void);

/**
 * @brief Notification to be called when energy detection procedure is requested.
 *
 * This notification will update the antenna and inform the caller for how long the energy
 * detection operation should be scheduled. This notification should also be called
 * after @ref nrf_802154_sl_ant_div_energy_detection_finished_notify requests repeating
 * the energy detection procedure. In that case, p_ed_time value must be set to 0
 * when passed to the function.
 *
 * @param[inout] p_ed_time Time of the energy detection procedure requested. Value will be updated with time for
 *                         energy detection procedure on the current antenna.
 */
void nrf_802154_sl_ant_div_energy_detection_requested_notify(uint32_t * p_ed_time);

/**
 * @brief Notification to be called when energy detection procedure is finished.
 * This notification checks whether the procedure should be repeated on the second antenna.
 *
 * @retval true  Energy detection should be repeated, antenna diversity module will switch the antenna when it is started.
 * @retval false Energy detection is finished, the result can be reported as normally.
 */
bool nrf_802154_sl_ant_div_energy_detection_finished_notify(void);

/**
 * @brief Notification to be called when energy detection procedure is aborted.
 */
void nrf_802154_sl_ant_div_energy_detection_aborted_notify(void);

/**
 * @brief Notification to be called when txack operation is requested.
 */
void nrf_802154_sl_ant_div_txack_notify(void);

#endif // NRF_802154_SL_ANT_DIV_H
