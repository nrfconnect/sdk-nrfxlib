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
 *   This file implements Finite State Machine of nRF 802.15.4 radio driver.
 *
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_CORE

#include "nrf_802154_core.h"

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf_802154.h"
#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_critical_section.h"
#include "nrf_802154_debug.h"
#include "nrf_802154_notification.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_procedures_duration.h"
#include "nrf_802154_rssi.h"
#include "nrf_802154_rx_buffer.h"
#include "nrf_802154_sl_atomics.h"
#include "nrf_802154_sl_timer.h"
#include "nrf_802154_sl_utils.h"
#include "nrf_802154_stats.h"
#include "nrf_802154_utils.h"
#include "nrf_802154_trx.h"
#include "nrf_802154_tx_work_buffer.h"
#include "nrf_802154_tx_power.h"
#include "nrf_802154_types_internal.h"
#include "nrf_802154_utils.h"
#include "nrf_802154_nrfx_addons.h"
#include "drivers/nrfx_errors.h"
#include "hal/nrf_radio.h"
#include "mac_features/nrf_802154_filter.h"
#include "mac_features/nrf_802154_frame_parser.h"
#include "mac_features/ack_generator/nrf_802154_ack_generator.h"
#include "rsch/nrf_802154_rsch.h"
#include "rsch/nrf_802154_rsch_crit_sect.h"
#include "timer/nrf_802154_timer_coord.h"
#include "platform/nrf_802154_irq.h"
#include "protocol/mpsl_fem_protocol_api.h"

#include "nrf_802154_core_hooks.h"
#include "nrf_802154_sl_ant_div.h"

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
#include "nrf_802154_bsim_utils.h"
#include "NRF_AES_ECB.h"
#endif

/// Delay before first check of received frame: 24 bits is PHY header and MAC Frame Control field.
#define BCC_INIT                    (3 * 8)

/// Duration of single iteration of Energy Detection procedure
#define ED_ITER_DURATION            128U
/// Overhead of hardware preparation for ED procedure (aTurnaroundTime) [number of iterations]
#define ED_ITERS_OVERHEAD           2U

#define MAX_CRIT_SECT_TIME          60           ///< Maximal time that the driver spends in single critical section.

#define LQI_VALUE_FACTOR            ED_RSSISCALE ///< Factor needed to calculate LQI value based on data from RADIO peripheral
#define LQI_MAX                     0xff         ///< Maximal LQI value

/** Get LQI of given received packet. If CRC is calculated by hardware LQI is included instead of CRC
 *  in the frame. Length is stored in byte with index 0; CRC is 2 last bytes.
 */
#define RX_FRAME_LQI(data)          ((data)[(data)[0] - 1])

/** Timeout value when waiting for ntf_802154_trx_receive_frame_started after ntf_802154_trx_receive_frame_prestarted.
 *  Timeout value in microseconds. The time equals to whole synchronization header (SHR) duration of 802.15.4 frame.
 */
#define PRESTARTED_TIMER_TIMEOUT_US (160U)

#if NRF_802154_RX_BUFFERS > 1
/// Pointer to currently used receive buffer.
static rx_buffer_t * mp_current_rx_buffer;

#else
/// If there is only one buffer use const pointer to the receive buffer.
static rx_buffer_t * const mp_current_rx_buffer = &nrf_802154_rx_buffers[0];

#endif

static uint8_t                       * mp_ack;                  ///< Pointer to Ack frame buffer.
static uint8_t                       * mp_tx_data;              ///< Pointer to the data to transmit.
static uint32_t                        m_ed_time_left;          ///< Remaining time of the current energy detection procedure [us].
static int8_t                          m_ed_result;             ///< Result of the current energy detection procedure.
static uint8_t                         m_last_lqi;              ///< LQI of the last received non-ACK frame, corrected for the temperature.
static nrf_802154_fal_tx_power_split_t m_tx_power;              ///< Power to be used to transmit the current frame split into components.
static uint8_t                         m_tx_channel;            ///< Channel to be used to transmit the current frame.
static int8_t                          m_last_rssi;             ///< RSSI of the last received non-ACK frame, corrected for the temperature.
static uint8_t                         m_no_rx_buffer_notified; ///< Set when NRF_802154_RX_ERROR_NO_BUFFER has been notified.

static nrf_802154_frame_parser_data_t m_current_rx_frame_data;  ///< RX frame parser data.

static volatile radio_state_t m_state;                          ///< State of the radio driver.

typedef struct
{
    bool frame_filtered        : 1; ///< If frame being received passed filtering operation.
    bool frame_parsed          : 1; ///< If frame being received has been parsed
    bool rx_timeslot_requested : 1; ///< If timeslot for the frame being received is already requested.
    bool tx_with_cca           : 1; ///< If currently transmitted frame is transmitted with cca.
    bool tx_diminished_prio    : 1; ///< If priority of the current transmission should be diminished.

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    bool tx_started_notify     : 1; ///< If higher layer should be notified that transmission started.

#endif
} nrf_802154_flags_t;

static nrf_802154_flags_t m_flags;                            ///< Flags used to store the current driver state.

static volatile bool        m_rsch_timeslot_is_granted;       ///< State of the RSCH timeslot.
static volatile rsch_prio_t m_rsch_priority = RSCH_PRIO_IDLE; ///< Last notified RSCH priority.

/** @brief Value of argument @c notifications_mask to @ref nrf_802154_trx_receive_frame */
static nrf_802154_trx_receive_notifications_t m_trx_receive_frame_notifications_mask;
/** @brief Value of argument @c notifications_mask to @ref nrf_802154_trx_transmit_frame */
static nrf_802154_trx_transmit_notifications_t m_trx_transmit_frame_notifications_mask;

static volatile uint8_t      m_rx_prestarted_trig_count;
static nrf_802154_sl_timer_t m_rx_prestarted_timer;

/** @brief Value of Coex TX Request mode */
static nrf_802154_coex_tx_request_mode_t m_coex_tx_request_mode;

/** @brief Identifier of currently active reception window. */
static uint32_t m_rx_window_id;

static const nrf_802154_transmitted_frame_props_t m_default_frame_props =
    NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT;

/***************************************************************************************************
 * @section Common core operations
 **************************************************************************************************/

static rsch_prio_t min_required_rsch_prio(radio_state_t state);

static void request_preconditions_for_state(radio_state_t state)
{
    nrf_802154_rsch_crit_sect_prio_request(min_required_rsch_prio(state));
}

/** Set driver state.
 *
 * @param[in]  state  Driver state to set.
 */
static void state_set(radio_state_t state)
{
    m_state = state;

    nrf_802154_log_local_event(NRF_802154_LOG_VERBOSITY_LOW,
                               NRF_802154_LOG_LOCAL_EVENT_ID_CORE__SET_STATE,
                               (uint32_t)state);

    request_preconditions_for_state(state);
}

/** Specifies what ramp up trigger mode to use when handling RX or TX operation request.
 *
 * It is assumed that the DELAYED_TRX module always requests HW mode both RX and TX,
 * while in all other cases SW mode is required.
 *
 * @param[in]  request_orig  Module that originates the request.
 */
static nrf_802154_trx_ramp_up_trigger_mode_t ramp_up_mode_choose(req_originator_t request_orig)
{
    return (request_orig == REQ_ORIG_DELAYED_TRX) ?
           TRX_RAMP_UP_HW_TRIGGER : TRX_RAMP_UP_SW_TRIGGER;
}

/** Clear RX frame data. */
static void rx_data_clear(void)
{
    (void)nrf_802154_frame_parser_data_init(mp_current_rx_buffer->data,
                                            0U,
                                            PARSE_LEVEL_NONE,
                                            &m_current_rx_frame_data);
}

/** Clear flags describing frame being received. */
static void rx_flags_clear(void)
{
    m_flags.frame_filtered        = false;
    m_flags.rx_timeslot_requested = false;
    m_flags.frame_parsed          = false;
}

/** Wait for the RSSI measurement. */
static void rssi_measurement_wait(void)
{
    while (!nrf_802154_trx_rssi_sample_is_available())
    {
        // Intentionally empty: This function is called from a critical section.
        // WFE would not be waken up by a RADIO event.
    }
}

/** Get the result of the last RSSI measurement.
 *
 * @returns  Result of the last RSSI measurement in dBm.
 */
static int8_t rssi_last_measurement_get(void)
{
    return nrf_802154_trx_rssi_last_sample_get();
}

/** Get LQI of a received frame.
 *
 * @param[in]  p_data  Pointer to buffer containing PHR and PSDU of received frame
 *
 * @returns  LQI of given frame.
 */
static uint8_t lqi_get(const uint8_t * p_data)
{
    uint32_t lqi = RX_FRAME_LQI(p_data);

    lqi  = nrf_802154_rssi_lqi_corrected_get(lqi);
    lqi *= LQI_VALUE_FACTOR;

    if (lqi > LQI_MAX)
    {
        lqi = LQI_MAX;
    }

    return (uint8_t)lqi;
}

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
/**
 * @brief Get timestamp made by timer coordinator.
 *
 * @note This function increments the returned value by 1 us if the timestamp is equal to the
 *       @ref NRF_802154_NO_TIMESTAMP value to indicate that the timestamp is available.
 *
 * @returns Timestamp [us] of the last event captured by timer coordinator frame or
 *          @ref NRF_802154_NO_TIMESTAMP if the timestamp is inaccurate.
 */
static uint64_t timer_coord_timestamp_get(void)
{
    uint64_t timestamp          = NRF_802154_NO_TIMESTAMP;
    bool     timestamp_received = nrf_802154_timer_coord_timestamp_get(&timestamp);

    if (!timestamp_received)
    {
        timestamp = NRF_802154_NO_TIMESTAMP;
    }
    else if (timestamp == NRF_802154_NO_TIMESTAMP)
    {
        timestamp++;
    }
    else
    {
        // Return timestamp without correction
    }

    return timestamp;
}

#endif

static void received_frame_notify(uint8_t * p_data)
{
    nrf_802154_notify_received(p_data, m_last_rssi, m_last_lqi);
}

/** Allow nesting critical sections and notify MAC layer that a frame was received. */
static void received_frame_notify_and_nesting_allow(uint8_t * p_data)
{
    nrf_802154_critical_section_nesting_allow();

    received_frame_notify(p_data);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that receive procedure failed. */
static void receive_failed_notify(nrf_802154_rx_error_t error)
{
    nrf_802154_critical_section_nesting_allow();

    // Don't care about the result - if the notification cannot be performed
    // no impact on the device's operation is expected
    (void)nrf_802154_notify_receive_failed(error, m_rx_window_id, true);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that transmission of requested frame has started. */
static void transmit_started_notify(void)
{
#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    /**
     * TX started hooks were executed before transmission started. Use latched result
     */
#else
    (void)nrf_802154_core_hooks_tx_started(mp_tx_data);
#endif
}

/** Notify MAC layer that transmission of ACK frame has started. */
static void transmit_ack_started_notify()
{
#if !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    nrf_802154_core_hooks_tx_ack_started(mp_ack);
#else
    /**
     * Otherwise this was already called immediately after setting up the transmission.
     */
#endif
    nrf_802154_tx_ack_started(mp_ack);
}

/** Notify that reception of a frame has started. */
static void receive_started_notify(void)
{
    const uint8_t * p_frame = mp_current_rx_buffer->data;

    nrf_802154_core_hooks_rx_started(p_frame);
}

/** Notify MAC layer that a frame was transmitted. */
static void transmitted_frame_notify(uint8_t * p_ack, int8_t power, uint8_t lqi)
{
    uint8_t                           * p_frame  = mp_tx_data;
    nrf_802154_transmit_done_metadata_t metadata = {0};

    metadata.data.transmitted.p_ack = p_ack;
    metadata.data.transmitted.power = power;
    metadata.data.transmitted.lqi   = lqi;

    if (p_ack == NULL)
    {
        metadata.data.transmitted.time = NRF_802154_NO_TIMESTAMP;
    }
    else
    {
        metadata.data.transmitted.length = p_ack[0];
        nrf_802154_stat_timestamp_read(&metadata.data.transmitted.time, last_ack_end_timestamp);
    }

    nrf_802154_critical_section_nesting_allow();

    nrf_802154_core_hooks_transmitted(p_frame);

    nrf_802154_notify_transmitted(p_frame, &metadata);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that transmission procedure failed. */
static void transmit_failed_notify(uint8_t                                   * p_frame,
                                   nrf_802154_tx_error_t                       error,
                                   const nrf_802154_transmit_done_metadata_t * p_meta)
{
    if (nrf_802154_core_hooks_tx_failed(p_frame, error))
    {
        nrf_802154_notify_transmit_failed(p_frame, error, p_meta);
    }
}

/** Allow nesting critical sections and notify MAC layer that transmission procedure failed. */
static void transmit_failed_notify_and_nesting_allow(
    nrf_802154_tx_error_t                       error,
    const nrf_802154_transmit_done_metadata_t * p_meta)
{
    nrf_802154_critical_section_nesting_allow();

    transmit_failed_notify(mp_tx_data, error, p_meta);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that energy detection procedure ended. */
static void energy_detected_notify(const nrf_802154_energy_detected_t * p_result)
{
    nrf_802154_critical_section_nesting_allow();

    nrf_802154_notify_energy_detected(p_result);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that CCA procedure ended. */
static void cca_notify(bool result)
{
    nrf_802154_critical_section_nesting_allow();

    nrf_802154_notify_cca(result);

    nrf_802154_critical_section_nesting_deny();
}

/** Check if timeslot is currently granted.
 *
 * @retval true   The timeslot is granted.
 * @retval false  The timeslot is not granted.
 */
static bool timeslot_is_granted(void)
{
    return m_rsch_timeslot_is_granted;
}

static bool antenna_diversity_is_enabled(void)
{
    return (NRF_802154_SL_ANT_DIV_MODE_DISABLED !=
            nrf_802154_sl_ant_div_cfg_mode_get(NRF_802154_SL_ANT_DIV_OP_RX));
}

/***************************************************************************************************
 * @section RX buffer management
 **************************************************************************************************/

/** Set currently used rx buffer to given address.
 *
 * @param[in]  p_rx_buffer  Pointer to receive buffer that should be used now.
 */
static void rx_buffer_in_use_set(rx_buffer_t * p_rx_buffer)
{
#if NRF_802154_RX_BUFFERS > 1
    mp_current_rx_buffer = p_rx_buffer;
#else
    (void)p_rx_buffer;
#endif
}

/** Check if currently there is available rx buffer.
 *
 * @retval true   There is available rx buffer.
 * @retval false  Currently there is no available rx buffer.
 */
static bool rx_buffer_is_available(void)
{
    return (mp_current_rx_buffer != NULL) && (mp_current_rx_buffer->free);
}

/** Get pointer to available rx buffer.
 *
 * @returns Pointer to available rx buffer or NULL if rx buffer is not available.
 */
static uint8_t * rx_buffer_get(void)
{
    return rx_buffer_is_available() ? mp_current_rx_buffer->data : NULL;
}

/***************************************************************************************************
 * @section ACK transmission management
 **************************************************************************************************/

/** Check if ACK is requested in given frame.
 *
 * @param[in]  p_frame  Pointer to a frame to check.
 *
 * @retval  true   ACK is requested in given frame.
 * @retval  false  ACK is not requested in given frame.
 */
static bool ack_is_requested(const uint8_t * p_frame)
{
    nrf_802154_frame_parser_data_t frame_data;

    if ((p_frame[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) == FRAME_TYPE_MULTIPURPOSE)
    {
        // Multipurpose frame parsing is not implemented, so assume AR is not set.
        return false;
    }

    bool result = nrf_802154_frame_parser_data_init(p_frame,
                                                    p_frame[PHR_OFFSET] + PHR_SIZE,
                                                    PARSE_LEVEL_FCF_OFFSETS,
                                                    &frame_data);

    return result && nrf_802154_frame_parser_ar_bit_is_set(&frame_data);
}

/***************************************************************************************************
 * @section Energy detection management
 **************************************************************************************************/

/** Setup next iteration of energy detection procedure.
 *
 *  Energy detection procedure is performed in iterations to make sure it is performed for requested
 *  time regardless radio arbitration.
 *
 *  @param[inout] p_requested_ed_time_us  Remaining time of energy detection procedure [us]. Value will be updated
 *                                        with time remaining for the next attempt of energy detection.
 *  @param[out]   p_next_trx_ed_count     Number of trx energy detection iterations to perform.
 *
 *  @retval  true   Next iteration of energy detection procedure will be performed now.
 *  @retval  false  Next iteration of energy detection procedure will not be performed now due to
 *                  ending timeslot.
 */
static bool ed_iter_setup(uint32_t * p_requested_ed_time_us, uint32_t * p_next_trx_ed_count)
{
    uint32_t iters_left_in_timeslot = nrf_802154_rsch_timeslot_us_left_get() / ED_ITER_DURATION;

    if (iters_left_in_timeslot > ED_ITERS_OVERHEAD)
    {
        /* Note that in single phy iters_left_in_timeslot will always be very big thus we will get here. */
        iters_left_in_timeslot -= ED_ITERS_OVERHEAD;

        uint32_t requested_iters = *p_requested_ed_time_us / ED_ITER_DURATION;

        if (requested_iters < iters_left_in_timeslot)
        {
            /* We will finish all iterations before timeslot end, thus no time is left */
            *p_requested_ed_time_us = 0U;
        }
        else
        {
            *p_requested_ed_time_us = *p_requested_ed_time_us -
                                      (iters_left_in_timeslot * ED_ITER_DURATION);
            requested_iters = iters_left_in_timeslot;
        }

        *p_next_trx_ed_count = requested_iters;

        return true;
    }

    return false;
}

/***************************************************************************************************
 * @section FSM transition request sub-procedures
 **************************************************************************************************/

static rsch_prio_t min_required_rsch_prio(radio_state_t state)
{
    switch (state)
    {
        case RADIO_STATE_SLEEP:
            return RSCH_PRIO_IDLE;

        case RADIO_STATE_FALLING_ASLEEP:
        case RADIO_STATE_RX:
            return RSCH_PRIO_IDLE_LISTENING;

        case RADIO_STATE_RX_ACK:
        case RADIO_STATE_ED:
        case RADIO_STATE_CCA:
            return RSCH_PRIO_RX;

        case RADIO_STATE_TX:
        case RADIO_STATE_TX_ACK:
#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
            return RSCH_PRIO_TX;

        case RADIO_STATE_CCA_TX:
            if (m_flags.tx_diminished_prio)
            {
                return RSCH_PRIO_IDLE_LISTENING;
            }
            else
            {
                return RSCH_PRIO_TX;
            }

        default:
            NRF_802154_ASSERT(false);
            return RSCH_PRIO_IDLE;
    }
}

static bool is_state_allowed_for_prio(rsch_prio_t prio, radio_state_t state)
{
    return (min_required_rsch_prio(state) <= prio);
}

static bool are_preconditions_met(void)
{
    rsch_prio_t   current_prio;
    radio_state_t current_state;

    current_prio  = m_rsch_priority;
    current_state = m_state;

    return is_state_allowed_for_prio(current_prio, current_state);
}

static int_fast8_t action_needed(rsch_prio_t old_prio, rsch_prio_t new_prio, radio_state_t state)
{
    bool old_prio_allows = is_state_allowed_for_prio(old_prio, state);
    bool new_prio_allows = is_state_allowed_for_prio(new_prio, state);

    int_fast8_t result = 0;

    if (old_prio_allows && !new_prio_allows)
    {
        result = -1;
    }
    else if (!old_prio_allows && new_prio_allows)
    {
        result = 1;
    }

    return result;
}

/** Check if time remaining in the timeslot is long enough to process whole critical section. */
static bool remaining_timeslot_time_is_enough_for_crit_sect(void)
{
    return nrf_802154_rsch_timeslot_us_left_get() >= MAX_CRIT_SECT_TIME;
}

/** Check if critical section can be processed at the moment.
 *
 * @note This function returns valid result only inside critical section.
 *
 * @retval true   There is enough time in current timeslot or timeslot is denied at the moment.
 * @retval false  Current timeslot ends too shortly to process critical section inside.
 */
static bool critical_section_can_be_processed_now(void)
{
    return !timeslot_is_granted() || remaining_timeslot_time_is_enough_for_crit_sect();
}

/** Enter critical section and verify if there is enough time to complete operations within. */
static bool critical_section_enter_and_verify_timeslot_length(void)
{
    bool result = nrf_802154_critical_section_enter();

    if (result)
    {
        if (!critical_section_can_be_processed_now())
        {
            result = false;

            nrf_802154_critical_section_exit();
        }
    }

    return result;
}

static bool can_terminate_current_operation(radio_state_t     state,
                                            nrf_802154_term_t term_lvl,
                                            bool              receiving_psdu_now)
{
    bool result = false;

    switch (state)
    {
        case RADIO_STATE_SLEEP:
        case RADIO_STATE_FALLING_ASLEEP:
#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
            result = true;
            break;

        case RADIO_STATE_RX:
            result = (term_lvl >= NRF_802154_TERM_802154) || !receiving_psdu_now;
            break;

        case RADIO_STATE_TX_ACK:
        case RADIO_STATE_CCA_TX:
        case RADIO_STATE_TX:
        case RADIO_STATE_RX_ACK:
        case RADIO_STATE_ED:
        case RADIO_STATE_CCA:
            result = (term_lvl >= NRF_802154_TERM_802154);
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    return result;
}

static void operation_terminated_notify(radio_state_t state, bool receiving_psdu_now)
{
    switch (state)
    {
        case RADIO_STATE_SLEEP:
        case RADIO_STATE_FALLING_ASLEEP:
#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
            break;

        case RADIO_STATE_RX:
            if (receiving_psdu_now)
            {
                // Don't care about the result - if the notification cannot be performed
                // no impact on the device's operation is expected
                (void)nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_ABORTED,
                                                       m_rx_window_id,
                                                       true);
            }

            break;

        case RADIO_STATE_TX_ACK:
            mp_current_rx_buffer->free = false;
            nrf_802154_core_hooks_tx_ack_failed(mp_ack, NRF_802154_TX_ERROR_ABORTED);
            received_frame_notify(mp_current_rx_buffer->data);
            break;

        case RADIO_STATE_CCA_TX:
        case RADIO_STATE_TX:
        {
            nrf_802154_transmit_done_metadata_t metadata = {};

            nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);
            transmit_failed_notify(mp_tx_data, NRF_802154_TX_ERROR_ABORTED, &metadata);
        }
        break;

        case RADIO_STATE_RX_ACK:
        {
            nrf_802154_transmit_done_metadata_t metadata = {};

            nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);
            transmit_failed_notify(mp_tx_data, NRF_802154_TX_ERROR_NO_ACK, &metadata);
        }
        break;

        case RADIO_STATE_ED:
            nrf_802154_notify_energy_detection_failed(NRF_802154_ED_ERROR_ABORTED);
            break;

        case RADIO_STATE_CCA:
            nrf_802154_notify_cca_failed(NRF_802154_CCA_ERROR_ABORTED);
            break;

        default:
            NRF_802154_ASSERT(false);
    }
}

/** Terminate ongoing operation.
 *
 * This function is called when MAC layer requests transition to another operation.
 *
 * After calling this function RADIO should enter DISABLED state and Radio Scheduler
 * should be in continuous mode.
 *
 * @param[in]  term_lvl      Termination level of this request. Selects procedures to abort.
 * @param[in]  req_orig      Module that originates termination request.
 * @param[in]  notify_abort  If Termination of current operation shall be notified.
 *
 * @retval true   Terminated ongoing operation.
 * @retval false  Ongoing operation was not terminated.
 */
static bool current_operation_terminate(nrf_802154_term_t term_lvl,
                                        req_originator_t  req_orig,
                                        bool              notify)
{
    bool result = nrf_802154_core_hooks_terminate(term_lvl, req_orig);

    if (result)
    {
        bool receiving_psdu_now = false;

        if (m_state == RADIO_STATE_RX)
        {
            receiving_psdu_now = nrf_802154_trx_psdu_is_being_received();
        }

        result = can_terminate_current_operation(m_state, term_lvl, receiving_psdu_now);

        if (result)
        {
            nrf_802154_trx_abort();

            if (m_state == RADIO_STATE_RX)
            {
                /* When in rx mode, nrf_802154_trx_receive_frame_prestarted handler might
                 * have already been called. We need to stop counting timeout. */
                m_rx_prestarted_trig_count = 0;
                (void)nrf_802154_sl_timer_remove(&m_rx_prestarted_timer);

                /* Notify antenna diversity module that RX has been aborted. */
                nrf_802154_sl_ant_div_rx_aborted_notify();

                /* We might have boosted preconditions (to support coex) above level
                 * normally requested for current state by request_preconditions_for_state(m_state).
                 * When current operation is terminated we request preconditions back
                 * thus ceasing to request to coex. */
                request_preconditions_for_state(m_state);
            }

            if (m_state == RADIO_STATE_ED)
            {
                nrf_802154_sl_ant_div_energy_detection_aborted_notify();
            }

            if (notify)
            {
                operation_terminated_notify(m_state, receiving_psdu_now);
            }
        }

    }

    return result;
}

/** Initialize Falling Asleep operation. */
static void falling_asleep_init(void)
{
    if (nrf_802154_trx_go_idle())
    {
        // There will be nrf_802154_trx_in_idle call, where we will continue processing
    }
    else
    {
        state_set(RADIO_STATE_SLEEP);
    }
}

/**@brief Makes value to be passed to @ref nrf_802154_trx_receive_frame as @c notifications_mask parameter */
static nrf_802154_trx_receive_notifications_t make_trx_frame_receive_notification_mask(void)
{
    nrf_802154_trx_receive_notifications_t result = TRX_RECEIVE_NOTIFICATION_NONE;

#if (NRF_802154_STATS_COUNT_ENERGY_DETECTED_EVENTS)
    result |= TRX_RECEIVE_NOTIFICATION_PRESTARTED;
#endif
#if (NRF_802154_STATS_COUNT_RECEIVED_PREAMBLES)
    result |= TRX_RECEIVE_NOTIFICATION_STARTED;
#endif

    if (nrf_802154_wifi_coex_is_enabled())
    {
        switch (nrf_802154_pib_coex_rx_request_mode_get())
        {
            case NRF_802154_COEX_RX_REQUEST_MODE_DESTINED:
                /* Coex requesting handled through nrf_802154_trx_receive_frame_bcmatched handler.
                 * No additional notifications required. */
                break;

            case NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION:
                result |= TRX_RECEIVE_NOTIFICATION_PRESTARTED | TRX_RECEIVE_NOTIFICATION_STARTED;
                // Note: TRX_RECEIVE_NOTIFICATION_STARTED is required for stopping counting timeout for
                // activity triggered by nrf_802154_trx_receive_frame_prestarted.
                break;

            case NRF_802154_COEX_RX_REQUEST_MODE_PREAMBLE:
                result |= TRX_RECEIVE_NOTIFICATION_STARTED;
                break;

            default:
                NRF_802154_ASSERT(false);
        }
    }

    return result;
}

/**@brief Makes value to be passed to @ref nrf_802154_trx_transmit_frame as @c notifications_mask parameter
 *
 * @param[in] cca   Pass true, if cca operation it to be performed before transmit.
 *                  Pass false otherwise.
 */
static nrf_802154_trx_transmit_notifications_t make_trx_frame_transmit_notification_mask(bool cca)
{
    nrf_802154_trx_transmit_notifications_t result = TRX_TRANSMIT_NOTIFICATION_NONE;

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    if (cca)
    {
        result |= TRX_TRANSMIT_NOTIFICATION_CCAIDLE;
    }
#endif

    if (nrf_802154_wifi_coex_is_enabled())
    {
        switch (nrf_802154_pib_coex_tx_request_mode_get())
        {
            case NRF_802154_COEX_TX_REQUEST_MODE_FRAME_READY:
            case NRF_802154_COEX_TX_REQUEST_MODE_CCA_START:
            case NRF_802154_COEX_TX_REQUEST_MODE_ON_CCA_TOGGLE:
                /* No additional notifications required. */
                break;

            case NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE:
                result |= TRX_TRANSMIT_NOTIFICATION_CCAIDLE;
                break;

            default:
                NRF_802154_ASSERT(false);
        }
    }

    return result;
}

static void notify_no_rx_buffer(void)
{
    uint8_t old_value = 0U;

    if (nrf_802154_sl_atomic_cas_u8(&m_no_rx_buffer_notified, &old_value, 1U))
    {
        receive_failed_notify(NRF_802154_RX_ERROR_NO_BUFFER);
    }
}

static void rx_init_free_buffer_find_and_update(bool free_buffer)
{
    if (!free_buffer)
    {
        // If no buffer was available, then find a new one.
        rx_buffer_in_use_set(nrf_802154_rx_buffer_free_find());

        uint8_t * rx_buffer = rx_buffer_get();

        nrf_802154_trx_receive_buffer_set(rx_buffer);

        if (NULL == rx_buffer)
        {
            notify_no_rx_buffer();
        }
    }
}

/**
 * @brief Initializes RX operation
 *
 * @param[in]  ru_tr_mode            Desired trigger mode for radio ramp up.
 * @param[out] p_abort_shall_follow  It is set to `true` when initialization fails and the trx
 *                                   module needs to be reset. When obtaining `true` here, the user
 *                                   is then obliged to call @ref nrf_802154_trx_abort. Such a case
 *                                   can only happen when @p ru_tr_mode = TRX_RAMP_UP_HW_TRIGGER.
 *                                   In all other cases, the value is not modified at all, so it
 *                                   should be initialized to `false` before calling the function.
 *                                   Can be NULL if @p ru_tr_mode is not TRX_RAMP_UP_HW_TRIGGER.
 */
static void rx_init(nrf_802154_trx_ramp_up_trigger_mode_t ru_tr_mode, bool * p_abort_shall_follow)
{
    bool free_buffer;

    if (!timeslot_is_granted())
    {
        return;
    }

    if (!are_preconditions_met())
    {
        return;
    }

    // Clear filtering flag
    rx_flags_clear();

    // Find available RX buffer
    free_buffer = rx_buffer_is_available();

    nrf_802154_trx_receive_buffer_set(rx_buffer_get());

    nrf_802154_fal_tx_power_split_t split_power = {0};

    (void)nrf_802154_tx_power_split_pib_power_get(&split_power);

    nrf_802154_trx_channel_set(nrf_802154_pib_channel_get());

    nrf_802154_trx_receive_frame(BCC_INIT / 8U,
                                 ru_tr_mode,
                                 m_trx_receive_frame_notifications_mask,
                                 &split_power);

    if (ru_tr_mode == TRX_RAMP_UP_HW_TRIGGER)
    {
        uint32_t ppi_ch = nrf_802154_trx_ramp_up_ppi_channel_get();

        if (!nrf_802154_rsch_delayed_timeslot_ppi_update(ppi_ch))
        {
            /**
             * The trigger has occurred. This has happened too early so there is a high risk
             * that the radio will not ramp up. It is necessary to abort the operation.
             */
            NRF_802154_ASSERT(p_abort_shall_follow);
            *p_abort_shall_follow = true;
        }
    }

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    // Configure the timer coordinator to get a timestamp of the CRCOK event.
    nrf_802154_timer_coord_timestamp_prepare(nrf_802154_trx_radio_crcok_event_handle_get());
#endif

    rx_init_free_buffer_find_and_update(free_buffer);

    rx_data_clear();

    mp_ack = NULL;
}

/** Initialize TX operation. */
static bool tx_init(const uint8_t                       * p_data,
                    nrf_802154_trx_ramp_up_trigger_mode_t rampup_trigg_mode,
                    uint8_t                               cca_attempts)
{
    bool cca = cca_attempts > 0;

    if (!timeslot_is_granted() || !nrf_802154_rsch_timeslot_request(
            nrf_802154_tx_duration_get(p_data[0], cca, ack_is_requested(p_data))))
    {
        return false;
    }

    if (!are_preconditions_met())
    {
        return false;
    }

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    if (cca)
    {
        // Configure the timer coordinator to get a time stamp of the READY event.
        // Note: This event triggers CCASTART, so the time stamp of READY event
        // is the time stamp when CCA started.
        nrf_802154_timer_coord_timestamp_prepare(nrf_802154_trx_radio_ready_event_handle_get());
    }
    else
    {
        // Configure the timer coordinator to get a time stamp of the PHYEND event.
        nrf_802154_timer_coord_timestamp_prepare(nrf_802154_trx_radio_phyend_event_handle_get());
    }
#endif

    nrf_802154_trx_channel_set(m_tx_channel);
    m_flags.tx_with_cca = cca;
    nrf_802154_trx_transmit_frame(nrf_802154_tx_work_buffer_get(p_data),
                                  rampup_trigg_mode,
                                  cca_attempts,
                                  &m_tx_power,
                                  m_trx_transmit_frame_notifications_mask);
#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    /**
     * In simulation the frame contents are latched when the first bit of the preamble is
     * "transmitted" by the simulated radio. Any modifications performed after that (for example in
     * handler of RADIO.ADDRESS event) have no effect on the frame received by other devices in the
     * simulation.
     *
     * Because of these limitations of the simulation, modification of the transmitted frame buffer
     * is performed immediately after the radio ramp-up start. Also, since in simulation no time
     * passes while the CPU is executing code and the radio ramp-up is shorter than the time needed
     * by the ECB peripheral to encrypt the longest possible 802.15.4 frame, the ECB is artificially
     * sped up to guarantee that encryption completes before the transmission starts.
     */
    nrf_802154_bsim_utils_core_hooks_adjustments_t adjustments;

    adjustments.tx_started.time_to_radio_address_us =
        nrf_802154_rsch_delayed_timeslot_time_to_hw_trigger_get();

    if (cca)
    {
        // Assume that the first CCA succeeds
        adjustments.tx_started.time_to_radio_address_us +=
            RX_RAMP_UP_TIME + CCA_TIME + RX_TX_TURNAROUND_TIME;
    }
    else
    {
        adjustments.tx_started.time_to_radio_address_us += TX_RAMP_UP_TIME;
    }

    adjustments.tx_started.time_to_radio_address_us += PHY_US_TIME_FROM_SYMBOLS(PHY_SHR_SYMBOLS);

    nrf_802154_bsim_utils_core_hooks_adjustments_set(&adjustments);

    m_flags.tx_started_notify = nrf_802154_core_hooks_tx_started(mp_tx_data);

    nrf_802154_bsim_utils_core_hooks_adjustments_t zeroes = {0};

    nrf_802154_bsim_utils_core_hooks_adjustments_set(&zeroes);
#endif

    if (rampup_trigg_mode == TRX_RAMP_UP_HW_TRIGGER)
    {
        uint32_t ppi_ch = nrf_802154_trx_ramp_up_ppi_channel_get();

        if (!nrf_802154_rsch_delayed_timeslot_ppi_update(ppi_ch))
        {
            /**
             * The trigger has occurred. This has happened too early so there is a high risk
             * that the radio will not ramp up. It is necessary to abort the operation.
             */
            nrf_802154_trx_abort();
            return false;
        }
    }

    return true;
}

/** Initialize ED operation */
static void ed_init(void)
{
    if (!timeslot_is_granted())
    {
        return;
    }

    if (!are_preconditions_met())
    {
        return;
    }

    uint32_t trx_ed_count = 0U;

    // Notify antenna diversity about energy detection request. Antenna diversity state
    // will be updated, and m_ed_time_left reduced accordingly.
    nrf_802154_sl_ant_div_energy_detection_requested_notify(&m_ed_time_left);

    if (!ed_iter_setup(&m_ed_time_left, &trx_ed_count))
    {
        // Just wait for next timeslot if there is not enough time in this one.
        return;
    }

    nrf_802154_trx_energy_detection(trx_ed_count);
}

/** Initialize CCA operation. */
static void cca_init(void)
{
    if (!timeslot_is_granted() || !nrf_802154_rsch_timeslot_request(nrf_802154_cca_duration_get()))
    {
        return;
    }

    if (!are_preconditions_met())
    {
        return;
    }

    nrf_802154_trx_standalone_cca();
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

/** Initialize Continuous Carrier operation. */
static void continuous_carrier_init(void)
{
    if (!timeslot_is_granted())
    {
        return;
    }

    if (!are_preconditions_met())
    {
        return;
    }
    nrf_802154_fal_tx_power_split_t split_power = {0};

    (void)nrf_802154_tx_power_split_pib_power_get(&split_power);

    nrf_802154_trx_continuous_carrier(&split_power);
}

/** Initialize Modulated Carrier operation. */
static void modulated_carrier_init(const uint8_t * p_data)
{
    if (!timeslot_is_granted())
    {
        return;
    }

    if (!are_preconditions_met())
    {
        return;
    }

    nrf_802154_fal_tx_power_split_t split_power = {0};

    (void)nrf_802154_tx_power_split_pib_power_get(&split_power);

    nrf_802154_trx_modulated_carrier(p_data, &split_power);
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

/** Switches to idle state: rx or sleep depending on RxOnWhenIdle mode */
static void switch_to_idle(void)
{
    if (!nrf_802154_pib_rx_on_when_idle_get())
    {
        state_set(RADIO_STATE_SLEEP);
    }
    else
    {
        state_set(RADIO_STATE_RX);
        rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);
    }
}

/***************************************************************************************************
 * @section Radio Scheduler notification handlers
 **************************************************************************************************/
static void on_timeslot_ended(void)
{
    bool result;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (timeslot_is_granted())
    {
        m_rsch_timeslot_is_granted = false;

        bool receiving_psdu_now = false;

        if (m_state == RADIO_STATE_RX)
        {
            receiving_psdu_now = nrf_802154_trx_psdu_is_being_received();
        }

        nrf_802154_trx_disable();

        nrf_802154_timer_coord_stop();

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
        /**
         * In simulation no time passes while the CPU is executing code and the radio ramp-up is
         * shorter than the time needed by the ECB peripheral to encrypt the longest possible
         * 802.15.4 frame. The ECB is artificially sped up to guarantee that encryption completes
         * before transmission starts. Reset its settings to not confuse other users of this
         * peripheral outside of 802.15.4 timeslot.
         */
        nrf_aes_ecb_cheat_reset_t_ecb();
#endif

        nrf_802154_rsch_continuous_ended();

        result = nrf_802154_core_hooks_terminate(NRF_802154_TERM_802154, REQ_ORIG_RSCH);
        NRF_802154_ASSERT(result);
        (void)result;

        switch (m_state)
        {
            case RADIO_STATE_FALLING_ASLEEP:
                state_set(RADIO_STATE_SLEEP);
                break;

            case RADIO_STATE_RX:
                if (receiving_psdu_now)
                {
                    receive_failed_notify(NRF_802154_RX_ERROR_TIMESLOT_ENDED);
                }

                break;

            case RADIO_STATE_TX_ACK:
                state_set(RADIO_STATE_RX);
                mp_current_rx_buffer->free = false;
                nrf_802154_core_hooks_tx_ack_failed(mp_ack, NRF_802154_TX_ERROR_TIMESLOT_ENDED);
                received_frame_notify_and_nesting_allow(mp_current_rx_buffer->data);
                break;

            case RADIO_STATE_CCA_TX:
            case RADIO_STATE_TX:
            {
                state_set(RADIO_STATE_RX);
                nrf_802154_transmit_done_metadata_t metadata = {};

                nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);
                transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_TIMESLOT_ENDED,
                                                         &metadata);
            }
            break;

            case RADIO_STATE_RX_ACK:
            {
                state_set(RADIO_STATE_RX);
                nrf_802154_transmit_done_metadata_t metadata = {};

                nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);
                transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_NO_ACK,
                                                         &metadata);
            }
            break;

            case RADIO_STATE_ED:
            case RADIO_STATE_CCA:
#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
            case RADIO_STATE_CONTINUOUS_CARRIER:
            case RADIO_STATE_MODULATED_CARRIER:
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
            case RADIO_STATE_SLEEP:
                // Intentionally empty.
                break;

            default:
                NRF_802154_ASSERT(false);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_preconditions_denied(radio_state_t state)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result;

    result = nrf_802154_core_hooks_terminate(NRF_802154_TERM_802154, REQ_ORIG_RSCH);
    NRF_802154_ASSERT(result);
    (void)result;

    bool receiving_psdu_now = false;

    if (state == RADIO_STATE_RX)
    {
        receiving_psdu_now = nrf_802154_trx_psdu_is_being_received();
    }

    nrf_802154_trx_abort();

    switch (state)
    {
        case RADIO_STATE_FALLING_ASLEEP:
            // There should be on_timeslot_ended event
            break;

        case RADIO_STATE_RX:
            if (receiving_psdu_now)
            {
                receive_failed_notify(NRF_802154_RX_ERROR_ABORTED);
            }

            break;

        case RADIO_STATE_TX_ACK:
            state_set(RADIO_STATE_RX);
            break;

        case RADIO_STATE_CCA_TX:
            m_flags.tx_diminished_prio = false;
        // Fallthrough

        case RADIO_STATE_TX:
        case RADIO_STATE_RX_ACK:
            state_set(RADIO_STATE_RX);
            break;

        case RADIO_STATE_ED:
        case RADIO_STATE_CCA:
#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case RADIO_STATE_SLEEP:
            // Intentionally empty.
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    operation_terminated_notify(state, receiving_psdu_now);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_preconditions_approved(radio_state_t state)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_trx_abort();

    switch (state)
    {
        case RADIO_STATE_SLEEP:
            // Intentionally empty. Appropriate action will be performed on state change.
            break;

        case RADIO_STATE_RX:
            rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);
            break;

        case RADIO_STATE_CCA_TX:
            (void)tx_init(mp_tx_data, TRX_RAMP_UP_SW_TRIGGER, true);
            break;

        case RADIO_STATE_TX:
            (void)tx_init(mp_tx_data, TRX_RAMP_UP_SW_TRIGGER, false);
            break;

        case RADIO_STATE_ED:
            ed_init();
            break;

        case RADIO_STATE_CCA:
            cca_init();
            break;

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case RADIO_STATE_CONTINUOUS_CARRIER:
            continuous_carrier_init();
            break;

        case RADIO_STATE_MODULATED_CARRIER:
            modulated_carrier_init(mp_tx_data);
            break;
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

        default:
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_timeslot_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_trx_enable();

    m_rsch_timeslot_is_granted = true;

    nrf_802154_timer_coord_start();

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    /**
     * In simulation no time passes while the CPU is executing code and the radio ramp-up is shorter
     * than the time needed by the ECB peripheral to encrypt the longest possible 802.15.4 frame.
     * Speed up the ECB to guarantee that encryption completes before transmission starts.
     */
    nrf_aes_ecb_cheat_set_t_ecb(1);
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static bool preconditions_approved_should_be_ignored(rsch_prio_t previously_approved_prio,
                                                     rsch_prio_t currently_approved_prio)
{
    // Approved preconditions should only be ignored only all the following conditions are met:
    // * all preconditions apart from Coex had already been approved;
    // * the call is a result of Coex becoming approved at the highest priority;
    // * currently performed operation is transmission with CCA;
    // * Coex for transmission is requested after CCA reports idle channel
    bool only_coex_was_unapproved       = (previously_approved_prio == RSCH_PRIO_RX);
    bool all_preconditions_are_approved = (currently_approved_prio == RSCH_PRIO_MAX);
    bool current_state_is_cca_tx        = (m_state == RADIO_STATE_CCA_TX);
    bool coex_tx_request_mode_allows    = (m_coex_tx_request_mode ==
                                           NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE);

    return (only_coex_was_unapproved &&
            all_preconditions_are_approved &&
            current_state_is_cca_tx &&
            coex_tx_request_mode_allows);
}

void nrf_802154_rsch_crit_sect_prio_changed(rsch_prio_t prio)
{
    rsch_prio_t old_prio = m_rsch_priority;

    m_rsch_priority = prio;

    if ((old_prio == RSCH_PRIO_IDLE) && (prio != RSCH_PRIO_IDLE))
    {
        // We have just got a timeslot.
        on_timeslot_started();
    }
    else if ((old_prio != RSCH_PRIO_IDLE) && (prio == RSCH_PRIO_IDLE))
    {
        // We are giving back timeslot.
        on_timeslot_ended();
        return;
    }
    else if (prio == RSCH_PRIO_IDLE)
    {
        // It might happen that even though IDLE has already been notified, this function is called
        // again as a result of preemptions caused by unexpected timeslot change (e.g. the application
        // requested transition to sleep while out of timeslot and RAAL notified timeslot start
        // in the middle of that sleep request). The following block makes RAAL finish its processing.
        nrf_802154_rsch_continuous_ended();
    }
    else
    {
        // Intentionally empty
    }

    int_fast8_t transition = action_needed(old_prio, prio, m_state);

    if (transition == 0)
    {
        return;
    }
    else if (transition < 0)
    {
        on_preconditions_denied(m_state);

        // After denying preconditions, TRX is disabled. However, it is possible that the existing
        // preconditions are enough for the new state (entered due to denied preconditions) and TRX
        // could be enabled for the new state. If this is the case, on_preconditions_approved() is
        // called to fully switch to the new state.
        radio_state_t new_state = m_state;

        if (is_state_allowed_for_prio(prio, new_state))
        {
            on_preconditions_approved(new_state);
        }
    }
    else
    {
        if (!preconditions_approved_should_be_ignored(old_prio, prio))
        {
            on_preconditions_approved(m_state);
        }
    }
}

/***************************************************************************************************
 * @section RADIO interrupt handler
 **************************************************************************************************/

void nrf_802154_trx_receive_ack_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_RX_ACK);
    nrf_802154_core_hooks_rx_ack_started();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_rx_prestarted_timeout(nrf_802154_sl_timer_t * p_timer)
{
    (void)p_timer;

    /* If we were in critical section this handler would not be called.
     * If we were outside of critical section this handler could be called,
     * and nrf_802154_critical_section_enter must succeed.
     * Justification:
     * - If higher priority interrupt preempts this handler before it takes critical section, that
     * interrupt may enter/exit critical section, but when it returns to this handler
     * entering critical section will succeed.
     * - If we entered critical section here the higher priority interrupt from radio
     * will not occur.
     * - The only related interrupt that can preempt this handler while it owns critical section
     * is from raal timeslot margin, which will fail to enter critical section and schedule
     * priority change to be called by nrf_802154_critical_section_exit.
     *
     * Critical section is entered forcefully here nonetheless, due to a rare issue with
     * nrf_802154_critical_section_exit being preempted before the nested critical section counter
     * could be decremented. Allowing for critical section nesting here resolves the problem.
     * TODO: After the bug is fixed, change to nrf_802154_critical_section_enter and check if
     * critical section was successfully entered.
     */

    nrf_802154_critical_section_forcefully_enter();

    nrf_802154_sl_ant_div_rx_preamble_timeout_notify();

    do
    {
        /**
         * Multiple, rapidly consecutive reception start events without later moving to the next
         * step are possible. There is a special handling for this case here.
         */
        if (m_rx_prestarted_trig_count > 1)
        {
            uint64_t now = nrf_802154_sl_timer_current_time_get();

            /**
             * If the value of trigger_time field is a moment in the future, it means that timer
             * handling has been preempted by HELPER1 radio event after removing the timer from
             * scheduler, but before handling this callback. In that case, process the timeout
             * as usual, but notify  antenna diversity module that another preamble was detected
             * in order to repeat RSSI measurements.
             */
            if (nrf_802154_sl_time64_is_in_future(now, m_rx_prestarted_timer.trigger_time))
            {
                m_rx_prestarted_trig_count = 1;
                nrf_802154_sl_ant_div_rx_preamble_detected_notify();
                break;
            }
        }

        m_rx_prestarted_trig_count = 0;
    }
    while (0);

    /* If nrf_802154_trx_receive_frame_prestarted boosted preconditions beyond those normally
     * required by current state, they need to be restored now.
     */
    if (nrf_802154_pib_coex_rx_request_mode_get() ==
        NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION)
    {
        request_preconditions_for_state(m_state);
    }
    nrf_802154_critical_section_exit();
}

void nrf_802154_trx_receive_frame_prestarted(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (!antenna_diversity_is_enabled())
    {
        // Only assert if notifications mask would not allow for calling this function.
        NRF_802154_ASSERT((m_trx_receive_frame_notifications_mask &
                           TRX_RECEIVE_NOTIFICATION_PRESTARTED) !=
                          0U);
    }
    else
    {
        // Antenna diversity uses this function for detecting possible preamble on air.
    }

    NRF_802154_ASSERT(m_state == RADIO_STATE_RX);

#if (NRF_802154_STATS_COUNT_ENERGY_DETECTED_EVENTS)
    nrf_802154_stat_counter_increment(received_energy_events);
#endif

    nrf_802154_sl_ant_div_rx_preamble_detected_notify();

    // Antenna diversity module should be notified if framestart doesn't come.
    bool rx_timeout_should_be_started = antenna_diversity_is_enabled();

    if (nrf_802154_pib_coex_rx_request_mode_get() ==
        NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION)
    {
        // Request boosted preconditions for receive
        nrf_802154_rsch_crit_sect_prio_request(RSCH_PRIO_RX);
        // Boosted preconditions should be reverted if the framestart doesn't come.
        rx_timeout_should_be_started = true;
    }

    /*
     * This handler might not be followed by nrf_802154_trx_receive_frame_started. The timer
     * below is used for timing out if the framestart doesn't come.
     * There are two reasons for that: reverting boosted preconditions and notifying antenna diversity
     * module.
     */
    if (rx_timeout_should_be_started)
    {

        uint64_t now = nrf_802154_sl_timer_current_time_get();

        (void)nrf_802154_sl_timer_remove(&m_rx_prestarted_timer);

        m_rx_prestarted_timer.trigger_time             = now + PRESTARTED_TIMER_TIMEOUT_US;
        m_rx_prestarted_timer.action_type              = NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK;
        m_rx_prestarted_timer.action.callback.callback = on_rx_prestarted_timeout;

        nrf_802154_sl_timer_ret_t ret;

        ret = nrf_802154_sl_timer_add(&m_rx_prestarted_timer);
        NRF_802154_ASSERT(ret == NRF_802154_SL_TIMER_RET_SUCCESS);
        (void)ret;

        m_rx_prestarted_trig_count += 1;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_frame_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_RX);
    NRF_802154_ASSERT(
        (m_trx_receive_frame_notifications_mask & TRX_RECEIVE_NOTIFICATION_STARTED) != 0U);

#if (NRF_802154_STATS_COUNT_RECEIVED_PREAMBLES)
    nrf_802154_stat_counter_increment(received_preambles);
#endif

    switch (nrf_802154_pib_coex_rx_request_mode_get())
    {
        case NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION:
            m_rx_prestarted_trig_count = 0;
            (void)nrf_802154_sl_timer_remove(&m_rx_prestarted_timer);
        /* Fallthrough */

        case NRF_802154_COEX_RX_REQUEST_MODE_PREAMBLE:
            /* Request boosted preconditions */
            nrf_802154_rsch_crit_sect_prio_request(RSCH_PRIO_RX);
            break;

        default:
            break;
    }

    if (antenna_diversity_is_enabled())
    {
        // If antenna diversity is enabled, rx_prestarted_timer would be started even
        // in different coex rx request modes than NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION
        m_rx_prestarted_trig_count = 0;
        (void)nrf_802154_sl_timer_remove(&m_rx_prestarted_timer);
        nrf_802154_sl_ant_div_rx_frame_started_notify();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

uint8_t nrf_802154_trx_receive_frame_bcmatched(uint8_t bcc)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint8_t                         next_bcc;
    nrf_802154_filter_mode_t        filter_mode;
    bool                            parse_result;
    bool                            should_filter = false;
    nrf_802154_rx_error_t           filter_result = NRF_802154_RX_ERROR_NONE;
    nrf_802154_frame_parser_level_t parse_level   = PARSE_LEVEL_NONE;
    nrf_802154_frame_parser_level_t prev_level    =
        nrf_802154_frame_parser_parse_level_get(&m_current_rx_frame_data);

    NRF_802154_ASSERT(m_state == RADIO_STATE_RX);

    switch (prev_level)
    {
        case PARSE_LEVEL_NONE:
            parse_level   = PARSE_LEVEL_FCF_OFFSETS;
            filter_mode   = NRF_802154_FILTER_MODE_FCF;
            should_filter = true;
            break;

        case PARSE_LEVEL_FCF_OFFSETS:
            parse_level   = PARSE_LEVEL_DST_ADDRESSING_END;
            filter_mode   = NRF_802154_FILTER_MODE_DST_ADDR;
            should_filter = true;
            break;

        case PARSE_LEVEL_DST_ADDRESSING_END:
            parse_level = PARSE_LEVEL_SEC_CTRL_OFFSETS;
            break;

        case PARSE_LEVEL_SEC_CTRL_OFFSETS:
            parse_level = PARSE_LEVEL_AUX_SEC_HDR_END;
            break;

        default:
            /* Parsing completed. Nothing more to be done. */
            nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
            return 0;
    }

    if (nrf_802154_frame_parser_frame_type_get(&m_current_rx_frame_data) == FRAME_TYPE_MULTIPURPOSE)
    {
        m_flags.frame_filtered = true;
        return 0;
    }

    parse_result = nrf_802154_frame_parser_valid_data_extend(&m_current_rx_frame_data,
                                                             bcc,
                                                             parse_level);

    if (!parse_result)
    {
        should_filter = false;
        filter_result = NRF_802154_RX_ERROR_INVALID_FRAME;
    }

    if (should_filter)
    {
        filter_result = nrf_802154_filter_frame_part(&m_current_rx_frame_data, filter_mode);

        if ((filter_result == NRF_802154_RX_ERROR_NONE) &&
            (filter_mode == NRF_802154_FILTER_MODE_DST_ADDR))
        {
            m_flags.frame_filtered = true;

            nrf_802154_rsch_crit_sect_prio_request(RSCH_PRIO_RX);
            nrf_802154_ack_generator_reset();
        }
    }

    if (nrf_802154_pib_promiscuous_get())
    {
        /*
         * In promiscuous mode all filtering should be ignored unless the frame has
         * length 0 or above maximum frame length.
         */
        uint8_t psdu_length = nrf_802154_frame_parser_frame_length_get(&m_current_rx_frame_data);

        if (psdu_length > 0 && psdu_length <= MAX_PACKET_SIZE)
        {
            filter_result = NRF_802154_RX_ERROR_NONE;
        }
    }

    if (filter_result != NRF_802154_RX_ERROR_NONE)
    {
        nrf_802154_trx_abort();
        rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);

        /* Release boosted preconditions */
        request_preconditions_for_state(m_state);

        if (nrf_802154_frame_parser_frame_type_get(&m_current_rx_frame_data) != FRAME_TYPE_ACK)
        {
            receive_failed_notify(filter_result);
        }

        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
        return 0;
    }

    switch (parse_level)
    {
        case PARSE_LEVEL_FCF_OFFSETS:
            next_bcc = PHR_SIZE + nrf_802154_frame_parser_dst_addressing_end_offset_get(
                &m_current_rx_frame_data);
            break;

        case PARSE_LEVEL_DST_ADDRESSING_END:
            next_bcc = PHR_SIZE + nrf_802154_frame_parser_addressing_end_offset_get(
                &m_current_rx_frame_data) + SECURITY_CONTROL_SIZE;
            break;

        case PARSE_LEVEL_SEC_CTRL_OFFSETS:
            next_bcc = PHR_SIZE + nrf_802154_frame_parser_aux_sec_hdr_end_offset_get(
                &m_current_rx_frame_data);
            break;

        default:
            next_bcc = 0;
            break;
    }

    if (!m_flags.rx_timeslot_requested)
    {
        uint16_t duration = nrf_802154_rx_duration_get(
            mp_current_rx_buffer->data[0],
            nrf_802154_frame_parser_ar_bit_is_set(&m_current_rx_frame_data));

        if (nrf_802154_rsch_timeslot_request(duration))
        {
            m_flags.rx_timeslot_requested = true;

            receive_started_notify();
        }
        else
        {
            // Disable receiver and wait for a new timeslot.
            nrf_802154_trx_abort();

            // We should not leave trx in temporary state, let's receive then.
            // We avoid hard reset of radio during TX ACK phase due to timeslot end,
            // which could result in spurious RF emission.
            rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);

            // Don't care about the result - if the notification cannot be performed
            // no impact on the device's operation is expected
            (void)nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_TIMESLOT_ENDED,
                                                   m_rx_window_id,
                                                   true);
        }
    }

    if (m_flags.frame_filtered &&
        nrf_802154_frame_parser_ar_bit_is_set(&m_current_rx_frame_data) &&
        nrf_802154_pib_auto_ack_get())
    {
        mp_ack = nrf_802154_ack_generator_create(&m_current_rx_frame_data);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return next_bcc;
}

void nrf_802154_trx_go_idle_finished(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    state_set(RADIO_STATE_SLEEP);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_bad_ack(void);

void nrf_802154_trx_receive_frame_crcerror(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_RX);
    rx_flags_clear();
    rx_data_clear();

    // We don't change receive buffer, receive will go to the same that was already used
    request_preconditions_for_state(m_state);

    nrf_802154_fal_tx_power_split_t split_power = {0};

    (void)nrf_802154_tx_power_split_pib_power_get(&split_power);

    nrf_802154_trx_receive_frame(BCC_INIT / 8U,
                                 TRX_RAMP_UP_SW_TRIGGER,
                                 m_trx_receive_frame_notifications_mask,
                                 &split_power);

#if NRF_802154_NOTIFY_CRCERROR
    receive_failed_notify(NRF_802154_RX_ERROR_INVALID_FCS);
#endif // NRF_802154_NOTIFY_CRCERROR

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_ack_crcerror(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_RX_ACK);

    on_bad_ack();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_frame_received(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint8_t             * p_received_data = mp_current_rx_buffer->data;
    nrf_802154_rx_error_t filter_result   = NRF_802154_RX_ERROR_RUNTIME;

    // Latch RSSI and LQI values before sending ACK
    m_last_rssi = rssi_last_measurement_get();
    m_last_lqi  = lqi_get(p_received_data);

    bool parse_result = nrf_802154_frame_parser_valid_data_extend(
        &m_current_rx_frame_data,
        PHR_SIZE + nrf_802154_frame_parser_frame_length_get(&m_current_rx_frame_data),
        PARSE_LEVEL_FULL);

    if (parse_result && !m_flags.frame_filtered)
    {
        filter_result = nrf_802154_filter_frame_part(&m_current_rx_frame_data,
                                                     NRF_802154_FILTER_MODE_ALL);

        if (filter_result == NRF_802154_RX_ERROR_NONE)
        {
            uint16_t duration = nrf_802154_ack_duration_with_turnaround_get();

            if (nrf_802154_rsch_timeslot_request(duration))
            {
                m_flags.frame_filtered        = true;
                m_flags.rx_timeslot_requested = true;

                nrf_802154_ack_generator_reset();
                receive_started_notify();
            }
            else
            {
                filter_result = NRF_802154_RX_ERROR_TIMESLOT_ENDED;
            }
        }
    }

    if (m_flags.frame_filtered || nrf_802154_pib_promiscuous_get())
    {
        nrf_802154_stat_counter_increment(received_frames);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
        uint64_t ts = timer_coord_timestamp_get();

        if (ts != NRF_802154_NO_TIMESTAMP)
        {
            ts -= RX_PHYEND_EVENT_LATENCY_US;
        }

        nrf_802154_stat_timestamp_write(last_rx_end_timestamp, ts);
#endif

        nrf_802154_sl_ant_div_rx_frame_received_notify();

        bool send_ack = false;

        if (m_flags.frame_filtered &&
            parse_result &&
            nrf_802154_frame_parser_ar_bit_is_set(&m_current_rx_frame_data) &&
            nrf_802154_pib_auto_ack_get())
        {
            nrf_802154_tx_work_buffer_reset(&m_default_frame_props);
            mp_ack   = nrf_802154_ack_generator_create(&m_current_rx_frame_data);
            send_ack = (mp_ack != NULL);
        }

        if (send_ack)
        {
            state_set(RADIO_STATE_TX_ACK);

            if (is_state_allowed_for_prio(m_rsch_priority, RADIO_STATE_TX_ACK))
            {
                if (nrf_802154_trx_transmit_ack(nrf_802154_tx_work_buffer_get(mp_ack), ACK_IFS))
                {
                    // Intentionally empty: transmitting ack, because we can
#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
                    /**
                     * In simulation the frame contents are latched when the first bit of the
                     * preamble is "transmitted" by the simulated radio. Any modifications performed
                     * after that (for example in handler of RADIO.ADDRESS event) have no effect on
                     * the frame received by other devices in the simulation.
                     *
                     * Because of these limitations of the simulation, modification of the transmitted
                     * frame buffer is performed immediately after the radio ramp-up start. Also,
                     * since in simulation no time passes while the CPU is executing code and the
                     * radio ramp-up is shorter than the time needed by the ECB peripheral to encrypt
                     * the longest possible 802.15.4 frame, the ECB is artificially sped up to
                     * guarantee that encryption completes before the transmission starts.
                     */
                    nrf_802154_bsim_utils_core_hooks_adjustments_t adjustments;

                    adjustments.tx_ack_started.time_to_radio_address_us =
                        ACK_IFS + TX_RAMP_UP_TIME + PHY_US_TIME_FROM_SYMBOLS(PHY_SHR_SYMBOLS);

                    nrf_802154_bsim_utils_core_hooks_adjustments_set(&adjustments);

                    nrf_802154_core_hooks_tx_ack_started(mp_ack);

                    nrf_802154_bsim_utils_core_hooks_adjustments_t zeroes = {0};

                    nrf_802154_bsim_utils_core_hooks_adjustments_set(&zeroes);
#endif /* defined(CONFIG_SOC_SERIES_BSIM_NRFXX) */
                }
                else
                {
                    mp_current_rx_buffer->free = false;

                    switch_to_idle();

                    received_frame_notify_and_nesting_allow(p_received_data);
                }
            }
            else
            {
                if (!nrf_802154_rsch_prec_is_approved(RSCH_PREC_COEX,
                                                      min_required_rsch_prio(RADIO_STATE_TX_ACK)))
                {
                    nrf_802154_stat_counter_increment(coex_denied_requests);
                }

                mp_current_rx_buffer->free = false;

                switch_to_idle();

                received_frame_notify_and_nesting_allow(p_received_data);
            }
        }
        else
        {
            request_preconditions_for_state(m_state);
            // Filter out received ACK frame if promiscuous mode is disabled.
            if (((p_received_data[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) != FRAME_TYPE_ACK) ||
                nrf_802154_pib_promiscuous_get())
            {
                // Current buffer will be passed to the application
                mp_current_rx_buffer->free = false;

                switch_to_idle();

                received_frame_notify_and_nesting_allow(p_received_data);
            }
            else
            {
                // Receive to the same buffer
                rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);
            }
        }
    }
    else
    {
        // CRC is OK, but filtering operation did not end - it is invalid frame with valid CRC
        // or problem due to software latency (i.e. handled BCMATCH, CRCERROR, CRCOK from two
        // consecutively received frames).
        request_preconditions_for_state(m_state);
        rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);

        receive_failed_notify(filter_result);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static bool fcf_is_security_enabled(const uint8_t * p_frame)
{
    return p_frame[SECURITY_ENABLED_OFFSET] & SECURITY_ENABLED_BIT;
}

static inline bool tx_started_core_hooks_will_fit_within_timeslot(const uint8_t * p_frame)
{
    if (!fcf_is_security_enabled(p_frame))
    {
        return true;
    }

    uint32_t estimated_max_hook_time = nrf_802154_frame_duration_get(p_frame[0], false, true) / 2U;

    return nrf_802154_rsch_timeslot_us_left_get() >= estimated_max_hook_time;
}

void nrf_802154_trx_transmit_frame_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT((m_state == RADIO_STATE_TX) || (m_state == RADIO_STATE_CCA_TX));
    if (tx_started_core_hooks_will_fit_within_timeslot(mp_tx_data))
    {
        transmit_started_notify();
    }
    else
    {
        nrf_802154_trx_abort();
        switch_to_idle();

        nrf_802154_transmit_done_metadata_t metadata = {};

        nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);

        transmit_failed_notify(mp_tx_data, NRF_802154_TX_ERROR_TIMESLOT_ENDED, &metadata);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_ack_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_TX_ACK);
    if (tx_started_core_hooks_will_fit_within_timeslot(mp_ack))
    {
        transmit_ack_started_notify();
    }
    else
    {
        uint8_t * p_received_data = mp_current_rx_buffer->data;

        nrf_802154_trx_abort();
        mp_current_rx_buffer->free = false;

        nrf_802154_core_hooks_tx_ack_failed(mp_ack, NRF_802154_RX_ERROR_TIMESLOT_ENDED);
        switch_to_idle();
        received_frame_notify_and_nesting_allow(p_received_data);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_ack_transmitted(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_TX_ACK);

    uint8_t * p_received_data = mp_current_rx_buffer->data;

    // Current buffer used for receive operation will be passed to the application
    mp_current_rx_buffer->free = false;

    switch_to_idle();

    received_frame_notify_and_nesting_allow(p_received_data);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_frame_transmitted(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    uint64_t ts = timer_coord_timestamp_get();

    // ts holds now timestamp of the PHYEND event
    nrf_802154_stat_timestamp_write(last_tx_end_timestamp, ts);

    if (m_flags.tx_with_cca)
    {
        m_flags.tx_diminished_prio = false;

        // We calculate the timestamp when ccaidle must happened.
        ts -= nrf_802154_frame_duration_get(mp_tx_data[0], true, true) + RX_TX_TURNAROUND_TIME;

        nrf_802154_stat_timestamp_write(last_cca_idle_timestamp, ts);
    }

#endif

    if (ack_is_requested(mp_tx_data))
    {
        state_set(RADIO_STATE_RX_ACK);

        bool rx_buffer_free = rx_buffer_is_available();

        nrf_802154_trx_receive_buffer_set(rx_buffer_get());

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
        // Configure the timer coordinator to get a timestamp of the CRCOK event.
        nrf_802154_timer_coord_timestamp_prepare(nrf_802154_trx_radio_crcok_event_handle_get());
#endif

        nrf_802154_trx_receive_ack();

        rx_init_free_buffer_find_and_update(rx_buffer_free);
    }
    else
    {
        switch_to_idle();

        transmitted_frame_notify(NULL, 0, 0);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static bool ack_match_check_version_not_2(const uint8_t * p_tx_data, const uint8_t * p_ack_data)
{
    // Frame Version != 2

    // Check: Phy length
    if (p_ack_data[PHR_OFFSET] != IMM_ACK_LENGTH)
    {
        return false;
    }

    // Check if Frame version is 0 or 1.
    switch (p_ack_data[FRAME_VERSION_OFFSET] & FRAME_VERSION_MASK)
    {
        case FRAME_VERSION_0:
        case FRAME_VERSION_1:
            break;

        default:
            return false;
    }

    // Check: Sequence number match
    if (p_ack_data[DSN_OFFSET] != p_tx_data[DSN_OFFSET])
    {
        return false;
    }

    return true;
}

static bool ack_match_check_version_2(const uint8_t * p_tx_frame, const uint8_t * p_ack_frame)
{
    nrf_802154_frame_parser_data_t tx_data;
    nrf_802154_frame_parser_data_t ack_data;
    bool                           parse_result;

    parse_result = nrf_802154_frame_parser_data_init(p_tx_frame,
                                                     p_tx_frame[PHR_OFFSET] + PHR_SIZE,
                                                     PARSE_LEVEL_ADDRESSING_END,
                                                     &tx_data);
    if (!parse_result)
    {
        return false;
    }

    parse_result = nrf_802154_frame_parser_data_init(p_ack_frame,
                                                     p_ack_frame[PHR_OFFSET] + PHR_SIZE,
                                                     PARSE_LEVEL_ADDRESSING_END,
                                                     &ack_data);

    if (nrf_802154_frame_parser_frame_version_get(&ack_data) != FRAME_VERSION_2)
    {
        return false;
    }

    // Transmitted frame was Version 2
    // For frame version 2 sequence number bit may be suppressed and its check fails.
    // Verify ACK frame using its destination address.

    const uint8_t * p_tx_src_addr     = nrf_802154_frame_parser_src_addr_get(&tx_data);
    const uint8_t * p_ack_dst_addr    = nrf_802154_frame_parser_dst_addr_get(&ack_data);
    uint8_t         tx_src_addr_size  = nrf_802154_frame_parser_src_addr_size_get(&tx_data);
    uint8_t         ack_dst_addr_size = nrf_802154_frame_parser_dst_addr_size_get(&ack_data);

    if (!parse_result ||
        (p_tx_src_addr == NULL) ||
        (p_ack_dst_addr == NULL) ||
        (tx_src_addr_size != ack_dst_addr_size) ||
        (0 != memcmp(p_tx_src_addr,
                     p_ack_dst_addr,
                     tx_src_addr_size)))
    {
        // Mismatch
        return false;
    }

    return true;
}

static bool ack_match_check(const uint8_t * p_tx_data, const uint8_t * p_ack_data)
{
    if ((p_tx_data == NULL) || (p_ack_data == NULL))
    {
        return false;
    }

    // Check: Frame Control Field -> Frame type
    if ((p_ack_data[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) != FRAME_TYPE_ACK)
    {
        return false; // This is not an ACK frame
    }

    // Check: Frame Control Field -> Frame version
    if ((p_tx_data[FRAME_VERSION_OFFSET] & FRAME_VERSION_MASK) == FRAME_VERSION_2)
    {
        return ack_match_check_version_2(p_tx_data, p_ack_data);
    }

    return ack_match_check_version_not_2(p_tx_data, p_ack_data);
}

static void on_bad_ack(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // We received either a frame with incorrect CRC or not an ACK frame or not matching ACK
    switch_to_idle();

    nrf_802154_transmit_done_metadata_t metadata = {};

    nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);
    transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_INVALID_ACK, &metadata);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_ack_received(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // CRC of received frame is correct
    uint8_t * p_ack_data = mp_current_rx_buffer->data;

    if (ack_match_check(mp_tx_data, p_ack_data))
    {
#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
        uint64_t ts = timer_coord_timestamp_get();

        nrf_802154_stat_timestamp_write(last_ack_end_timestamp, ts);
#endif

        rx_buffer_t * p_ack_buffer = mp_current_rx_buffer;

        mp_current_rx_buffer->free = false;

        // Detect Frame Pending field set to one on Ack frame received after a Data Request Command
        bool                           should_receive = false;
        nrf_802154_frame_parser_data_t frame_data;

        bool parse_result = nrf_802154_frame_parser_data_init(mp_tx_data,
                                                              mp_tx_data[PHR_OFFSET] + PHR_SIZE,
                                                              PARSE_LEVEL_FULL,
                                                              &frame_data);

        if (parse_result &&
            (nrf_802154_frame_parser_frame_type_get(&frame_data) == FRAME_TYPE_COMMAND))
        {
            const uint8_t * p_cmd = nrf_802154_frame_parser_mac_command_id_get(&frame_data);

            if ((p_cmd != NULL) && (*p_cmd == MAC_CMD_DATA_REQ))
            {
                if (p_ack_data[FRAME_PENDING_OFFSET] & FRAME_PENDING_BIT)
                {
                    should_receive = true;
                }
            }
        }

        if (should_receive)
        {
            state_set(RADIO_STATE_RX);
            rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);
        }
        else
        {
            switch_to_idle();
        }

        transmitted_frame_notify(p_ack_buffer->data,           // phr + psdu
                                 rssi_last_measurement_get(),  // rssi
                                 lqi_get(p_ack_buffer->data)); // lqi;
    }
    else
    {
        on_bad_ack();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_standalone_cca_finished(bool channel_was_idle)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch_to_idle();

    cca_notify(channel_was_idle);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_frame_ccastarted(void)
{
    // This handler provided by trx is never called because parameter notifications_mask
    // of the nrf_802154_trx_transmit_frame does not contain TRX_TRANSMIT_NOTIFICATION_CCASTARTED.
    NRF_802154_ASSERT(false);
}

void nrf_802154_trx_transmit_frame_ccaidle(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_state == RADIO_STATE_CCA_TX);
    NRF_802154_ASSERT(m_trx_transmit_frame_notifications_mask & TRX_TRANSMIT_NOTIFICATION_CCAIDLE);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    uint64_t ts = timer_coord_timestamp_get();

    // Configure the timer coordinator to get a timestamp of the PHYEND event.
    nrf_802154_timer_coord_timestamp_prepare(nrf_802154_trx_radio_phyend_event_handle_get());

    // Update stat timestamp of CCASTART event
    nrf_802154_stat_timestamp_write(last_cca_start_timestamp, ts);
#endif

    if (m_coex_tx_request_mode == NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE)
    {
        nrf_802154_rsch_crit_sect_prio_request(RSCH_PRIO_TX);
        m_flags.tx_diminished_prio = false;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_frame_ccabusy(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_stat_counter_increment(cca_failed_attempts);

    switch_to_idle();

    nrf_802154_transmit_done_metadata_t metadata = {};

    nrf_802154_tx_work_buffer_original_frame_update(mp_tx_data, &metadata.frame_props);
    transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_BUSY_CHANNEL, &metadata);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_energy_detection_finished(int8_t ed_sample_dbm)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    m_ed_result = MAX(m_ed_result, ed_sample_dbm);

    if (m_ed_time_left >= ED_ITER_DURATION)
    {
        uint32_t trx_ed_count = 0U;

        if (ed_iter_setup(&m_ed_time_left, &trx_ed_count))
        {
            nrf_802154_trx_energy_detection(trx_ed_count);
        }
        else
        {
            /* There is too little time in current timeslot, just wait for timeslot end.
             * Operation will be resumed in next timeslot */
        }
    }
    else if (nrf_802154_sl_ant_div_energy_detection_finished_notify())
    {
        ed_init();
    }
    else
    {
        nrf_802154_trx_channel_set(nrf_802154_pib_channel_get());

        switch_to_idle();

        nrf_802154_energy_detected_t ed_result = {};

        ed_result.ed_dbm = m_ed_result;

        energy_detected_notify(&ed_result);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/***************************************************************************************************
 * @section API functions
 **************************************************************************************************/

void nrf_802154_core_init(void)
{
    m_state                    = RADIO_STATE_SLEEP;
    m_rsch_timeslot_is_granted = false;
    m_rx_prestarted_trig_count = 0;
    m_no_rx_buffer_notified    = 0U;

    nrf_802154_sl_timer_init(&m_rx_prestarted_timer);

    nrf_802154_trx_init();
    nrf_802154_ack_generator_init();
}

void nrf_802154_core_deinit(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (timeslot_is_granted())
    {
        nrf_802154_trx_disable();
    }

    mpsl_fem_cleanup();

    nrf_802154_irq_disable(nrfx_get_irq_number(NRF_RADIO));
    nrf_802154_irq_clear_pending(nrfx_get_irq_number(NRF_RADIO));

    nrf_802154_sl_timer_deinit(&m_rx_prestarted_timer);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

radio_state_t nrf_802154_core_state_get(void)
{
    return m_state;
}

static bool core_sleep(nrf_802154_term_t term_lvl, req_originator_t req_orig, bool notify_abort)
{
    bool result = current_operation_terminate(term_lvl, req_orig, notify_abort);

    if (result)
    {
        // The order of calls in the following blocks is inverted to avoid RAAL races.
        if (timeslot_is_granted())
        {
            state_set(RADIO_STATE_FALLING_ASLEEP);
            falling_asleep_init();
        }
        else
        {
            state_set(RADIO_STATE_SLEEP);
        }
    }

    return result;
}

bool nrf_802154_core_sleep(nrf_802154_term_t term_lvl)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = nrf_802154_critical_section_enter();

    if (result)
    {
        if ((m_state != RADIO_STATE_SLEEP) && (m_state != RADIO_STATE_FALLING_ASLEEP))
        {
            result = core_sleep(term_lvl, REQ_ORIG_CORE, true);
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

static bool core_receive(nrf_802154_term_t term_lvl,
                         req_originator_t  req_orig,
                         bool              notify_abort,
                         uint32_t          id)
{
    bool result = current_operation_terminate(term_lvl, req_orig, notify_abort);

    if (result)
    {
        m_trx_receive_frame_notifications_mask =
            make_trx_frame_receive_notification_mask();

        m_rx_window_id = id;
        state_set(RADIO_STATE_RX);

        bool abort_shall_follow = false;

        rx_init(ramp_up_mode_choose(req_orig), &abort_shall_follow);

        if (abort_shall_follow)
        {
            nrf_802154_trx_abort();

            // HW triggering failed, fallback is SW trigger.
            // (fallback immunizes against the rare case of spurious lptimer firing)
            rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);
        }
    }

    return result;
}

bool nrf_802154_core_receive(nrf_802154_term_t              term_lvl,
                             req_originator_t               req_orig,
                             nrf_802154_notification_func_t notify_function,
                             bool                           notify_abort,
                             uint32_t                       id)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = nrf_802154_critical_section_enter();

    if (result)
    {
        if ((m_state != RADIO_STATE_RX) && (m_state != RADIO_STATE_TX_ACK))
        {
            if (critical_section_can_be_processed_now())
            {
                result = core_receive(term_lvl, req_orig, notify_abort, id);
            }
            else
            {
                result = false;
            }
        }

        if (notify_function != NULL)
        {
            notify_function(result);
        }

        nrf_802154_critical_section_exit();
    }
    else
    {
        if (notify_function != NULL)
        {
            notify_function(false);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_transmit(nrf_802154_term_t              term_lvl,
                              req_originator_t               req_orig,
                              uint8_t                      * p_data,
                              nrf_802154_transmit_params_t * p_params,
                              nrf_802154_notification_func_t notify_function)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        if (nrf_802154_core_hooks_pre_transmission(p_data, p_params, &transmit_failed_notify))
        {
            result = current_operation_terminate(term_lvl, req_orig, true);

            if (result)
            {
                nrf_802154_tx_work_buffer_reset(&p_params->frame_props);
                result = nrf_802154_core_hooks_tx_setup(p_data, p_params, &transmit_failed_notify);

                if (!result)
                {
                    switch_to_idle();
                }
            }

            if (result)
            {
                m_coex_tx_request_mode                  = nrf_802154_pib_coex_tx_request_mode_get();
                m_trx_transmit_frame_notifications_mask =
                    make_trx_frame_transmit_notification_mask(p_params->cca);
                m_flags.tx_diminished_prio =
                    m_coex_tx_request_mode == NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE;

                state_set(p_params->cca ? RADIO_STATE_CCA_TX : RADIO_STATE_TX);
                mp_tx_data   = p_data;
                m_tx_power   = p_params->tx_power;
                m_tx_channel = p_params->channel;

                uint8_t cca_attempts = p_params->cca ? (1 + p_params->extra_cca_attempts) : 0;

                // coverity[check_return]
                result = tx_init(p_data, ramp_up_mode_choose(req_orig), cca_attempts);

                if (p_params->immediate)
                {
                    if (!result)
                    {
                        switch_to_idle();
                    }
                }
                else
                {
                    result = true;
                }
            }
        }

        if (notify_function != NULL)
        {
            notify_function(result);
        }

        nrf_802154_critical_section_exit();
    }
    else
    {
        if (notify_function != NULL)
        {
            notify_function(false);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_ack_timeout_handle(const nrf_802154_ack_timeout_handle_params_t * p_param)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        if ((m_state == RADIO_STATE_RX_ACK) && (p_param->p_frame == mp_tx_data))
        {
            bool r;

            if (!nrf_802154_pib_rx_on_when_idle_get())
            {
                r = core_sleep(NRF_802154_TERM_802154, REQ_ORIG_ACK_TIMEOUT, false);
            }
            else
            {
                r = core_receive(NRF_802154_TERM_802154,
                                 REQ_ORIG_ACK_TIMEOUT,
                                 false,
                                 NRF_802154_RESERVED_IMM_RX_WINDOW_ID);

            }
            NRF_802154_ASSERT(r);
            (void)r;

            nrf_802154_transmit_done_metadata_t metadata = {0};

            nrf_802154_tx_work_buffer_original_frame_update(p_param->p_frame,
                                                            &metadata.frame_props);
            nrf_802154_notify_transmit_failed(p_param->p_frame,
                                              NRF_802154_TX_ERROR_NO_ACK,
                                              &metadata);
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_energy_detection(nrf_802154_term_t term_lvl, uint32_t time_us)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        result = current_operation_terminate(term_lvl, REQ_ORIG_CORE, true);

        if (result)
        {
            if (time_us < ED_ITER_DURATION)
            {
                time_us = ED_ITER_DURATION;
            }

            m_ed_time_left = time_us;
            m_ed_result    = ED_RSSIOFFS;

            state_set(RADIO_STATE_ED);
            ed_init();
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_cca(nrf_802154_term_t term_lvl)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        result = current_operation_terminate(term_lvl, REQ_ORIG_CORE, true);

        if (result)
        {
            state_set(RADIO_STATE_CCA);
            cca_init();
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

bool nrf_802154_core_continuous_carrier(nrf_802154_term_t term_lvl)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        result = current_operation_terminate(term_lvl, REQ_ORIG_CORE, true);

        if (result)
        {
            state_set(RADIO_STATE_CONTINUOUS_CARRIER);
            continuous_carrier_init();
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_modulated_carrier(nrf_802154_term_t term_lvl,
                                       const uint8_t   * p_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        result = current_operation_terminate(term_lvl, REQ_ORIG_CORE, true);

        if (result)
        {
            state_set(RADIO_STATE_MODULATED_CARRIER);
            mp_tx_data = (uint8_t *)p_data;
            modulated_carrier_init(p_data);
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

bool nrf_802154_core_notify_buffer_free(uint8_t * p_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    rx_buffer_t * p_buffer     = (rx_buffer_t *)p_data;
    bool          in_crit_sect = critical_section_enter_and_verify_timeslot_length();

    p_buffer->free = true;
    nrf_802154_sl_atomic_store_u8(&m_no_rx_buffer_notified, 0U);

    if (in_crit_sect)
    {
        if (timeslot_is_granted())
        {
            if (nrf_802154_trx_receive_is_buffer_missing())
            {
                rx_buffer_in_use_set(p_buffer);
                nrf_802154_trx_receive_buffer_set(rx_buffer_get());
            }
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return true;
}

bool nrf_802154_core_channel_update(req_originator_t req_orig)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        if (timeslot_is_granted())
        {
            nrf_802154_trx_channel_set(nrf_802154_pib_channel_get());
        }

        switch (m_state)
        {
            case RADIO_STATE_RX:
                if (current_operation_terminate(NRF_802154_TERM_802154, req_orig, true))
                {
                    rx_init(TRX_RAMP_UP_SW_TRIGGER, NULL);
                }
                break;

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
            case RADIO_STATE_CONTINUOUS_CARRIER:
                if (timeslot_is_granted())
                {
                    nrf_802154_trx_continuous_carrier_restart();
                }
                break;

            case RADIO_STATE_MODULATED_CARRIER:
                if (timeslot_is_granted())
                {
                    nrf_802154_trx_modulated_carrier_restart();
                }
                break;

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
            default:
                // Don't perform any additional action in any other state.
                break;
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_cca_cfg_update(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        if (timeslot_is_granted())
        {
            nrf_802154_trx_cca_configuration_update();
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_rssi_measure(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        if (timeslot_is_granted() && (m_state == RADIO_STATE_RX))
        {
            result = nrf_802154_trx_rssi_measure();
        }
        else
        {
            result = false;
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_last_rssi_measurement_get(int8_t * p_rssi)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result       = false;
    bool in_crit_sect = false;
    bool rssi_started = nrf_802154_trx_rssi_measure_is_started();

    if (rssi_started)
    {
        in_crit_sect = critical_section_enter_and_verify_timeslot_length();
    }

    if (rssi_started && in_crit_sect)
    {
        // Checking if a timeslot is granted is valid only in a critical section
        if (timeslot_is_granted())
        {
            rssi_started = nrf_802154_trx_rssi_measure_is_started();
            if (rssi_started)
            {
                rssi_measurement_wait();
                *p_rssi = rssi_last_measurement_get();
                result  = true;
            }
        }
    }

    if (in_crit_sect)
    {
        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

int8_t  nrf_802154_core_last_frame_rssi_get(void)
{
    return m_last_rssi;
}

uint8_t nrf_802154_core_last_frame_lqi_get(void)
{
    return m_last_lqi;
}

bool nrf_802154_core_antenna_update(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        if (timeslot_is_granted())
        {
            nrf_802154_trx_antenna_update();
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

int8_t nrf_802154_sl_ant_div_rssi_measure_get(void)
{
    int8_t result = NRF_802154_RSSI_INVALID;

    // This function is supposed to be called after detecting frame prestarted event, but before
    // detecting valid frame address. This means that we're currently in critical section, but the
    // timeslot is not yet extended due to detecting valid frame. To avoid invalid timeslot extension
    // due to blocking rssi measurements, antenna check can be aborted here if timeslot is about to end.
    // Antenna switching takes 200 ns (250 ns with safety margin), while rssi measurement - 250,
    // which gives total time of 750 ns.
    // 750 ns is less than safety margin, so timeslot us left different than 0 is sufficient.
    if (!nrf_802154_rsch_timeslot_us_left_get())
    {
        return result;
    }

    nrf_802154_trx_rssi_measure();

    if (nrf_802154_trx_rssi_measure_is_started())
    {
        while (!nrf_802154_trx_rssi_sample_is_available())
        {
            // Intentionally empty: This function is called from a critical section.
            // WFE would not be waken up by a RADIO event.
        }

        uint8_t rssi_sample = nrf_802154_trx_rssi_last_sample_get();

        rssi_sample = nrf_802154_rssi_sample_corrected_get(rssi_sample);

        result = -((int8_t)rssi_sample);
    }

    return result;
}
