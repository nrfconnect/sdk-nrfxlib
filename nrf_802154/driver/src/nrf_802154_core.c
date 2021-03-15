/*
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf_802154.h"
#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_critical_section.h"
#include "nrf_802154_debug.h"
#include "nrf_802154_notification.h"
#include "nrf_802154_nrfx_addons.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_procedures_duration.h"
#include "nrf_802154_rssi.h"
#include "nrf_802154_rx_buffer.h"
#include "nrf_802154_stats.h"
#include "nrf_802154_utils.h"
#include "nrf_802154_trx.h"
#include "nrf_802154_types.h"
#include "nrf_802154_utils.h"
#include "drivers/nrfx_errors.h"
#include "hal/nrf_radio.h"
#include "mpsl_fem_protocol_api.h"
#include "mac_features/nrf_802154_delayed_trx.h"
#include "mac_features/nrf_802154_filter.h"
#include "mac_features/nrf_802154_frame_parser.h"
#include "mac_features/ack_generator/nrf_802154_ack_data.h"
#include "mac_features/ack_generator/nrf_802154_ack_generator.h"
#include "rsch/nrf_802154_rsch.h"
#include "rsch/nrf_802154_rsch_crit_sect.h"
#include "timer/nrf_802154_timer_coord.h"
#include "timer/nrf_802154_timer_sched.h"
#include "platform/nrf_802154_hp_timer.h"
#include "platform/nrf_802154_irq.h"

#include "nrf_802154_core_hooks.h"
#include "nrf_802154_sl_ant_div.h"

/// Delay before first check of received frame: 24 bits is PHY header and MAC Frame Control field.
#define BCC_INIT                    (3 * 8)

/// Duration of single iteration of Energy Detection procedure
#define ED_ITER_DURATION            128U
/// Overhead of hardware preparation for ED procedure (aTurnaroundTime) [number of iterations]
#define ED_ITERS_OVERHEAD           2U

#define ACK_IFS                     TURNAROUND_TIME ///< Ack Inter Frame Spacing [us] - delay between last symbol of received frame and first symbol of transmitted Ack

#define MAX_CRIT_SECT_TIME          60              ///< Maximal time that the driver spends in single critical section.

#define LQI_VALUE_FACTOR            4               ///< Factor needed to calculate LQI value based on data from RADIO peripheral
#define LQI_MAX                     0xff            ///< Maximal LQI value

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

static const uint8_t * mp_ack;         ///< Pointer to Ack frame buffer.
static const uint8_t * mp_tx_data;     ///< Pointer to the data to transmit.
static uint32_t        m_ed_time_left; ///< Remaining time of the current energy detection procedure [us].
static uint8_t         m_ed_result;    ///< Result of the current energy detection procedure.

static volatile radio_state_t m_state; ///< State of the radio driver.

typedef struct
{
    bool frame_filtered        : 1;                           ///< If frame being received passed filtering operation.
    bool rx_timeslot_requested : 1;                           ///< If timeslot for the frame being received is already requested.
    bool tx_with_cca           : 1;                           ///< If currently transmitted frame is transmitted with cca.
    bool tx_diminished_prio    : 1;                           ///< If priority of the current transmission should be diminished.
} nrf_802154_flags_t;

static nrf_802154_flags_t m_flags;                            ///< Flags used to store the current driver state.

static volatile bool        m_rsch_timeslot_is_granted;       ///< State of the RSCH timeslot.
static volatile rsch_prio_t m_rsch_priority = RSCH_PRIO_IDLE; ///< Last notified RSCH priority.

/** @brief Value of argument @c notifications_mask to @ref nrf_802154_trx_receive_frame */
static nrf_802154_trx_receive_notifications_t m_trx_receive_frame_notifications_mask;
/** @brief Value of argument @c notifications_mask to @ref nrf_802154_trx_transmit_frame */
static nrf_802154_trx_transmit_notifications_t m_trx_transmit_frame_notifications_mask;

static nrf_802154_timer_t m_rx_prestarted_timer;

/** @brief Value of Coex TX Request mode */
static nrf_802154_coex_tx_request_mode_t m_coex_tx_request_mode;

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
#if !NRF_802154_FRAME_TIMESTAMP_ENABLED
#error NRF_802154_FRAME_TIMESTAMP_ENABLED == 0 when NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED != 0
#endif
#if NRF_802154_DISABLE_BCC_MATCHING
#error NRF_802154_DISABLE_BCC_MATCHING != 0 when NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED != 0
#endif
#endif // NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
static uint32_t m_listening_start_hp_timestamp;

#endif

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
                               NRF_802154_LOG_LOCAL_EVENT_ID_CORE__SET_STATE, (uint32_t)state);

    request_preconditions_for_state(state);
}

/** Clear flags describing frame being received. */
static void rx_flags_clear(void)
{
    m_flags.frame_filtered        = false;
    m_flags.rx_timeslot_requested = false;
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
    uint8_t rssi_sample = nrf_802154_trx_rssi_last_sample_get();

    rssi_sample = nrf_802154_rssi_sample_corrected_get(rssi_sample);

    return -((int8_t)rssi_sample);
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
static uint32_t timer_coord_timestamp_get(void)
{
    uint32_t timestamp          = NRF_802154_NO_TIMESTAMP;
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
    nrf_802154_notify_received(p_data,                      // data
                               rssi_last_measurement_get(), // rssi
                               lqi_get(p_data));            // lqi
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

    nrf_802154_notify_receive_failed(error);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that transmission of requested frame has started. */
static void transmit_started_notify(void)
{
    const uint8_t * p_frame = mp_tx_data;

    if (nrf_802154_core_hooks_tx_started(p_frame))
    {
        nrf_802154_tx_started(p_frame);
    }

}

#if !NRF_802154_DISABLE_BCC_MATCHING
/** Notify that reception of a frame has started. */
static void receive_started_notify(void)
{
    const uint8_t * p_frame = mp_current_rx_buffer->data;

    nrf_802154_core_hooks_rx_started(p_frame);
}

#endif

/** Notify MAC layer that a frame was transmitted. */
static void transmitted_frame_notify(uint8_t * p_ack, int8_t power, uint8_t lqi)
{
    const uint8_t * p_frame = mp_tx_data;

    nrf_802154_critical_section_nesting_allow();

    nrf_802154_core_hooks_transmitted(p_frame);
    nrf_802154_notify_transmitted(p_frame, p_ack, power, lqi);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that transmission procedure failed. */
static void transmit_failed_notify(nrf_802154_tx_error_t error)
{
    const uint8_t * p_frame = mp_tx_data;

    if (nrf_802154_core_hooks_tx_failed(p_frame, error))
    {
        nrf_802154_notify_transmit_failed(p_frame, error);
    }
}

/** Allow nesting critical sections and notify MAC layer that transmission procedure failed. */
static void transmit_failed_notify_and_nesting_allow(nrf_802154_tx_error_t error)
{
    nrf_802154_critical_section_nesting_allow();

    transmit_failed_notify(error);

    nrf_802154_critical_section_nesting_deny();
}

/** Notify MAC layer that energy detection procedure ended. */
static void energy_detected_notify(uint8_t result)
{
    nrf_802154_critical_section_nesting_allow();

    nrf_802154_notify_energy_detected(result);

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
    return nrf_802154_frame_parser_ar_bit_is_set(p_frame);
}

/***************************************************************************************************
 * @section Energy detection management
 **************************************************************************************************/

/** Get ED result value.
 *
 *  @param[in]  ed_sample   Energy Detection sample gathered from TRX module
 *  @returns ED result based on data collected during Energy Detection procedure.
 */
static uint8_t ed_result_get(uint8_t ed_sample)
{
    uint32_t result;

    result  = nrf_802154_rssi_ed_corrected_get(ed_sample);
    result *= ED_RESULT_FACTOR;

    if (result > ED_RESULT_MAX)
    {
        result = ED_RESULT_MAX;
    }

    return (uint8_t)result;
}

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
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
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
            assert(false);
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
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
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
            assert(false);
    }

    return result;
}

static void operation_terminated_notify(radio_state_t state, bool receiving_psdu_now)
{
    switch (state)
    {
        case RADIO_STATE_SLEEP:
        case RADIO_STATE_FALLING_ASLEEP:
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
            break;

        case RADIO_STATE_RX:
            if (receiving_psdu_now)
            {
                nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_ABORTED);
            }

            break;

        case RADIO_STATE_TX_ACK:
            mp_current_rx_buffer->free = false;
            received_frame_notify(mp_current_rx_buffer->data);
            break;

        case RADIO_STATE_CCA_TX:
        case RADIO_STATE_TX:
            transmit_failed_notify(NRF_802154_TX_ERROR_ABORTED);
            break;

        case RADIO_STATE_RX_ACK:
            transmit_failed_notify(NRF_802154_TX_ERROR_ABORTED);
            break;

        case RADIO_STATE_ED:
            nrf_802154_notify_energy_detection_failed(NRF_802154_ED_ERROR_ABORTED);
            break;

        case RADIO_STATE_CCA:
            nrf_802154_notify_cca_failed(NRF_802154_CCA_ERROR_ABORTED);
            break;

        default:
            assert(false);
    }
}

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
static void operation_terminated_update_total_times(trx_state_t trx_state, uint32_t timestamp)
{
    uint32_t t;

    switch (trx_state)
    {
        case TRX_STATE_RXFRAME:
        case TRX_STATE_RXACK:
            t = timestamp - m_listening_start_hp_timestamp;
            nrf_802154_stat_totals_increment(total_listening_time, t);
            break;

        default:
            break;
    }
}

static bool operation_terminated_update_total_times_is_required(trx_state_t trx_state)
{
    switch (trx_state)
    {
        case TRX_STATE_RXFRAME:
        case TRX_STATE_RXACK:
            /* These cases must be in-sync with implementation of
             * operation_terminated_update_total_times
             */
            return true;

        default:
            return false;
    }
}

#endif

static void trx_abort(void)
{
#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    trx_state_t trx_state       = nrf_802154_trx_state_get();
    bool        update_required = operation_terminated_update_total_times_is_required(trx_state);

#endif

    nrf_802154_trx_abort();

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    if (update_required)
    {
        uint32_t timestamp = nrf_802154_hp_timer_current_time_get();

        operation_terminated_update_total_times(trx_state, timestamp);
    }
#endif
}

static void trx_disable(void)
{
#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    trx_state_t trx_state       = nrf_802154_trx_state_get();
    bool        update_required = operation_terminated_update_total_times_is_required(trx_state);

#endif

    nrf_802154_trx_disable();

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    if (update_required)
    {
        uint32_t timestamp = nrf_802154_hp_timer_current_time_get();

        operation_terminated_update_total_times(trx_state, timestamp);
    }
#endif
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
            trx_abort();

            if (m_state == RADIO_STATE_RX)
            {
                /* When in rx mode, nrf_802154_trx_receive_frame_prestarted handler might
                 * have already been called. We need to stop counting timeout. */
                nrf_802154_timer_sched_remove(&m_rx_prestarted_timer, NULL);

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

/** Enter Sleep state. */
static void sleep_init(void)
{
    nrf_802154_timer_coord_stop();
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
        sleep_init();
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
                assert(false);
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
                /* No additional notifications required. */
                break;

            case NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE:
                result |= TRX_TRANSMIT_NOTIFICATION_CCAIDLE;
                break;

            default:
                assert(false);
        }
    }

    return result;
}

/** Initialize RX operation. */
static void rx_init(void)
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

    nrf_802154_trx_receive_frame(BCC_INIT / 8U, m_trx_receive_frame_notifications_mask);

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    m_listening_start_hp_timestamp = nrf_802154_hp_timer_current_time_get();
#endif

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    // Configure the timer coordinator to get a timestamp of the END event which
    // fires several cycles after CRCOK or CRCERROR events.
    nrf_802154_timer_coord_timestamp_prepare(
        nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_END));
#else
    // Configure the timer coordinator to get a timestamp of the CRCOK event.
    nrf_802154_timer_coord_timestamp_prepare(nrf_radio_event_address_get(NRF_RADIO,
                                                                         NRF_RADIO_EVENT_CRCOK));
#endif
#endif

    // Find RX buffer if none available
    if (!free_buffer)
    {
        rx_buffer_in_use_set(nrf_802154_rx_buffer_free_find());

        nrf_802154_trx_receive_buffer_set(rx_buffer_get());
    }
}

/** Initialize TX operation. */
static bool tx_init(const uint8_t * p_data, bool cca)
{
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
        nrf_802154_timer_coord_timestamp_prepare(nrf_radio_event_address_get(NRF_RADIO,
                                                                             NRF_RADIO_EVENT_READY));
    }
    else
    {
        // Configure the timer coordinator to get a time stamp of the PHYEND event.
        nrf_802154_timer_coord_timestamp_prepare(nrf_radio_event_address_get(NRF_RADIO,
                                                                             NRF_RADIO_EVENT_PHYEND));
    }
#endif

    m_flags.tx_with_cca = cca;
    nrf_802154_trx_transmit_frame(p_data,
                                  cca,
                                  m_trx_transmit_frame_notifications_mask);

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

    nrf_802154_trx_continuous_carrier();
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

    nrf_802154_trx_modulated_carrier((const void *)p_data);
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

        trx_disable();

        nrf_802154_timer_coord_stop();

        nrf_802154_rsch_continuous_ended();

        result = nrf_802154_core_hooks_terminate(NRF_802154_TERM_802154, REQ_ORIG_RSCH);
        assert(result);
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
                received_frame_notify_and_nesting_allow(mp_current_rx_buffer->data);
                break;

            case RADIO_STATE_CCA_TX:
            case RADIO_STATE_TX:
            case RADIO_STATE_RX_ACK:
                state_set(RADIO_STATE_RX);
                transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_TIMESLOT_ENDED);
                break;

            case RADIO_STATE_ED:
            case RADIO_STATE_CCA:
            case RADIO_STATE_CONTINUOUS_CARRIER:
            case RADIO_STATE_MODULATED_CARRIER:
            case RADIO_STATE_SLEEP:
                // Intentionally empty.
                break;

            default:
                assert(false);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_preconditions_denied(radio_state_t state)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result;

    result = nrf_802154_core_hooks_terminate(NRF_802154_TERM_802154, REQ_ORIG_CORE);
    assert(result);
    (void)result;

    bool receiving_psdu_now = false;

    if (state == RADIO_STATE_RX)
    {
        receiving_psdu_now = nrf_802154_trx_psdu_is_being_received();
    }

    trx_abort();

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
        case RADIO_STATE_CONTINUOUS_CARRIER:
        case RADIO_STATE_MODULATED_CARRIER:
        case RADIO_STATE_SLEEP:
            // Intentionally empty.
            break;

        default:
            assert(false);
    }

    operation_terminated_notify(state, receiving_psdu_now);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_preconditions_approved(radio_state_t state)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    trx_abort();

    switch (state)
    {
        case RADIO_STATE_SLEEP:
            // Intentionally empty. Appropriate action will be performed on state change.
            break;

        case RADIO_STATE_RX:
            rx_init();
            break;

        case RADIO_STATE_CCA_TX:
            (void)tx_init(mp_tx_data, true);
            break;

        case RADIO_STATE_TX:
            (void)tx_init(mp_tx_data, false);
            break;

        case RADIO_STATE_ED:
            ed_init();
            break;

        case RADIO_STATE_CCA:
            cca_init();
            break;

        case RADIO_STATE_CONTINUOUS_CARRIER:
            continuous_carrier_init();
            break;

        case RADIO_STATE_MODULATED_CARRIER:
            modulated_carrier_init(mp_tx_data);
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_timeslot_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_trx_enable();

    m_rsch_timeslot_is_granted = true;

    nrf_802154_timer_coord_start();

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

    assert(m_state == RADIO_STATE_RX_ACK);
    nrf_802154_core_hooks_rx_ack_started();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_rx_prestarted_timeout(void * p_context)
{
    (void)p_context;

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

    /**
     * If timer is still running here, it means that timer handling has been preempted by HELPER1
     * radio event after removing the timer from scheduler, but before handling this callback.
     * In that case, process the timeout as usual, but notify antenna diversity module that another
     * preamble was detected in order to repeat RSSI measurements.
     */
    if (nrf_802154_timer_sched_is_running(&m_rx_prestarted_timer))
    {
        nrf_802154_sl_ant_div_rx_preamble_detected_notify();
    }

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
        assert((m_trx_receive_frame_notifications_mask & TRX_RECEIVE_NOTIFICATION_PRESTARTED) !=
               0U);
    }
    else
    {
        // Antenna diversity uses this function for detecting possible preamble on air.
    }

    assert(m_state == RADIO_STATE_RX);

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

        uint32_t now = nrf_802154_timer_sched_time_get();

        nrf_802154_timer_sched_remove(&m_rx_prestarted_timer, NULL);

        m_rx_prestarted_timer.t0       = now;
        m_rx_prestarted_timer.dt       = PRESTARTED_TIMER_TIMEOUT_US;
        m_rx_prestarted_timer.callback = on_rx_prestarted_timeout;

        nrf_802154_timer_sched_add(&m_rx_prestarted_timer, true);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_frame_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_state == RADIO_STATE_RX);
    assert((m_trx_receive_frame_notifications_mask & TRX_RECEIVE_NOTIFICATION_STARTED) != 0U);

#if (NRF_802154_STATS_COUNT_RECEIVED_PREAMBLES)
    nrf_802154_stat_counter_increment(received_preambles);
#endif

    switch (nrf_802154_pib_coex_rx_request_mode_get())
    {
        case NRF_802154_COEX_RX_REQUEST_MODE_ENERGY_DETECTION:
            nrf_802154_timer_sched_remove(&m_rx_prestarted_timer, NULL);
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
        nrf_802154_timer_sched_remove(&m_rx_prestarted_timer, NULL);
        nrf_802154_sl_ant_div_rx_frame_started_notify();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if !NRF_802154_DISABLE_BCC_MATCHING
uint8_t nrf_802154_trx_receive_frame_bcmatched(uint8_t bcc)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint8_t               prev_num_data_bytes;
    uint8_t               num_data_bytes;
    nrf_802154_rx_error_t filter_result;
    bool                  frame_accepted = true;

    num_data_bytes      = bcc;
    prev_num_data_bytes = num_data_bytes;

    assert(num_data_bytes >= PHR_SIZE + FCF_SIZE);
    assert(m_state == RADIO_STATE_RX);

    if (!m_flags.frame_filtered)
    {
        filter_result = nrf_802154_filter_frame_part(mp_current_rx_buffer->data,
                                                     &num_data_bytes);

        if (filter_result == NRF_802154_RX_ERROR_NONE)
        {
            if (num_data_bytes != prev_num_data_bytes)
            {
                bcc = num_data_bytes;
            }
            else
            {
                m_flags.frame_filtered = true;

                /* Request boosted preconditions */
                nrf_802154_rsch_crit_sect_prio_request(RSCH_PRIO_RX);
            }
        }
        else if ((filter_result == NRF_802154_RX_ERROR_INVALID_LENGTH) ||
                 (!nrf_802154_pib_promiscuous_get()))
        {
            trx_abort();
            rx_init();

            frame_accepted = false;

            /* Release boosted preconditions */
            request_preconditions_for_state(m_state);

            if ((mp_current_rx_buffer->data[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) !=
                FRAME_TYPE_ACK)
            {
                receive_failed_notify(filter_result);
            }
        }
        else
        {
            // Promiscuous mode, allow incorrect frames. Nothing to do here.
        }
    }

    if ((!m_flags.rx_timeslot_requested) && (frame_accepted))
    {
        if (nrf_802154_rsch_timeslot_request(nrf_802154_rx_duration_get(
                                                 mp_current_rx_buffer->data[0],
                                                 ack_is_requested(mp_current_rx_buffer->data))))
        {
            m_flags.rx_timeslot_requested = true;

            receive_started_notify();
        }
        else
        {
            // Disable receiver and wait for a new timeslot.
            trx_abort();

            // We should not leave trx in temporary state, let's receive then.
            // We avoid hard reset of radio during TX ACK phase due to timeslot end,
            // which could result in spurious RF emission.
            rx_init();

            nrf_802154_notify_receive_failed(NRF_802154_RX_ERROR_TIMESLOT_ENDED);
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return bcc;
}

#endif

void nrf_802154_trx_go_idle_finished(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    sleep_init();
    state_set(RADIO_STATE_SLEEP);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void on_bad_ack(void);

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
static void update_total_times_on_receive_end(uint32_t listening_start_hp_timestamp,
                                              uint32_t receive_end_hp_timestamp, uint8_t phr)
{
    uint32_t t_listening;
    uint32_t t_frame;

    t_frame     = nrf_802154_frame_duration_get(phr, true, true);
    t_listening = receive_end_hp_timestamp - listening_start_hp_timestamp;

    if (t_frame > t_listening)
    {
        t_frame = t_listening;
    }

    t_listening -= t_frame;

    nrf_802154_stat_totals_increment(total_listening_time, t_listening);
    nrf_802154_stat_totals_increment(total_receive_time, t_frame);
}

#endif

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
void nrf_802154_stat_totals_get_notify(void)
{
    // Total times are going to be read, update stat_totals to hold
    // correct times until now.

    nrf_802154_mcu_critical_state_t mcu_cs;

    nrf_802154_mcu_critical_enter(mcu_cs);

    trx_state_t trx_state = nrf_802154_trx_state_get();

    if ((trx_state == TRX_STATE_RXFRAME) || (trx_state == TRX_STATE_RXACK))
    {
        uint32_t listening_end_timestamp = nrf_802154_hp_timer_current_time_get();

        if (listening_end_timestamp - m_listening_start_hp_timestamp >= MAX_PHY_FRAME_TIME_US)
        {
            /* m_listening_start_hp_timestamp ... now - MAX_PHY_FRAME_TIME_US must be listening.
             * Last MAX_PHY_FRAME_TIME_US is considered uncertain.
             */
            listening_end_timestamp -= MAX_PHY_FRAME_TIME_US;

            uint32_t t_listening = listening_end_timestamp - m_listening_start_hp_timestamp;

            m_listening_start_hp_timestamp = listening_end_timestamp;

            nrf_802154_stat_totals_increment(total_listening_time, t_listening);
        }
        else
        {
            /* Too little time passed since m_listening_start_hp_timestamp, we don't know
             * if frame is being received now until it is received. */
        }
    }

    nrf_802154_mcu_critical_exit(mcu_cs);

}

#endif

void nrf_802154_trx_receive_frame_crcerror(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    uint32_t receive_end_hp_timestamp     = nrf_802154_hp_timer_timestamp_get();
    uint32_t listening_start_hp_timestamp = m_listening_start_hp_timestamp;

#endif

    assert(m_state == RADIO_STATE_RX);
    rx_flags_clear();

    // We don't change receive buffer, receive will go to the same that was already used
#if !NRF_802154_DISABLE_BCC_MATCHING
    request_preconditions_for_state(m_state);
    nrf_802154_trx_receive_frame(BCC_INIT / 8U, m_trx_receive_frame_notifications_mask);

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    m_listening_start_hp_timestamp = nrf_802154_hp_timer_current_time_get();

    // Configure the timer coordinator to get a timestamp of the END event which
    // fires several cycles after CRCOK or CRCERROR events.
    nrf_802154_timer_coord_timestamp_prepare(
        nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_END));
#endif

#else
    // With BCC matching disabled trx module will re-arm automatically
#endif

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    update_total_times_on_receive_end(listening_start_hp_timestamp, receive_end_hp_timestamp,
                                      mp_current_rx_buffer->data[PHR_OFFSET]);
#endif

#if NRF_802154_NOTIFY_CRCERROR
    receive_failed_notify(NRF_802154_RX_ERROR_INVALID_FCS);
#endif // NRF_802154_NOTIFY_CRCERROR

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_ack_crcerror(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_state == RADIO_STATE_RX_ACK);

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    uint32_t receive_end_hp_timestamp     = nrf_802154_hp_timer_timestamp_get();
    uint32_t listening_start_hp_timestamp = m_listening_start_hp_timestamp;

    update_total_times_on_receive_end(listening_start_hp_timestamp, receive_end_hp_timestamp,
                                      mp_current_rx_buffer->data[PHR_OFFSET]);
#endif

    on_bad_ack();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_frame_received(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint8_t * p_received_data = mp_current_rx_buffer->data;

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    uint32_t receive_end_hp_timestamp     = nrf_802154_hp_timer_timestamp_get();
    uint32_t listening_start_hp_timestamp = m_listening_start_hp_timestamp;

    update_total_times_on_receive_end(listening_start_hp_timestamp, receive_end_hp_timestamp,
                                      mp_current_rx_buffer->data[PHR_OFFSET]);
#endif

#if NRF_802154_DISABLE_BCC_MATCHING
    uint8_t               num_data_bytes      = PHR_SIZE + FCF_SIZE;
    uint8_t               prev_num_data_bytes = 0;
    nrf_802154_rx_error_t filter_result;

    // Frame filtering
    while (num_data_bytes != prev_num_data_bytes)
    {
        prev_num_data_bytes = num_data_bytes;

        // Keep checking consecutive parts of the frame header.
        filter_result = nrf_802154_filter_frame_part(mp_current_rx_buffer->data, &num_data_bytes);

        if (filter_result == NRF_802154_RX_ERROR_NONE)
        {
            if (num_data_bytes == prev_num_data_bytes)
            {
                m_flags.frame_filtered = true;
            }
        }
        else
        {
            break;
        }
    }

    // Timeslot request
    if (m_flags.frame_filtered &&
        ack_is_requested(p_received_data) &&
        !nrf_802154_rsch_timeslot_request(nrf_802154_rx_duration_get(0, true)))
    {
        // Frame is destined to this node but there is no timeslot to transmit ACK.
        // Just disable receiver and wait for a new timeslot.
        nrf_802154_trx_abort();

        rx_flags_clear();

        // Filter out received ACK frame if promiscuous mode is disabled.
        if (((p_received_data[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) != FRAME_TYPE_ACK) ||
            nrf_802154_pib_promiscuous_get())
        {
            mp_current_rx_buffer->free = false;
            received_frame_notify_and_nesting_allow(p_received_data);
        }

        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
        return;
    }
#endif // NRF_802154_DISABLE_BCC_MATCHING

    if (m_flags.frame_filtered || nrf_802154_pib_promiscuous_get())
    {
        nrf_802154_stat_counter_increment(received_frames);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
        uint32_t ts = timer_coord_timestamp_get();

        nrf_802154_stat_timestamp_write(last_rx_end_timestamp, ts);
#endif

        nrf_802154_sl_ant_div_rx_frame_received_notify();

        bool send_ack = false;

        if (m_flags.frame_filtered &&
            ack_is_requested(mp_current_rx_buffer->data) &&
            nrf_802154_pib_auto_ack_get())
        {
            mp_ack = nrf_802154_ack_generator_create(mp_current_rx_buffer->data);
            if (NULL != mp_ack)
            {
                send_ack = true;
            }
        }

        if (send_ack)
        {
            state_set(RADIO_STATE_TX_ACK);

            if (is_state_allowed_for_prio(m_rsch_priority, RADIO_STATE_TX_ACK))
            {
                if (nrf_802154_trx_transmit_ack(mp_ack, ACK_IFS))
                {
                    // Intentionally empty: transmitting ack, because we can
                }
                else
                {
                    mp_current_rx_buffer->free = false;

                    state_set(RADIO_STATE_RX);
                    rx_init();

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

                state_set(RADIO_STATE_RX);
                rx_init();

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

                // Find new buffer
                rx_buffer_in_use_set(nrf_802154_rx_buffer_free_find());

                rx_init();

                received_frame_notify_and_nesting_allow(p_received_data);
            }
            else
            {
                // Receive to the same buffer
                rx_init();
            }
        }
    }
    else
    {
        // CRC is OK, but filtering operation did not end - it is invalid frame with valid CRC
        // or problem due to software latency (i.e. handled BCMATCH, CRCERROR, CRCOK from two
        // consecutively received frames).
        request_preconditions_for_state(m_state);
        rx_init();

#if NRF_802154_DISABLE_BCC_MATCHING
        if ((p_received_data[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) != FRAME_TYPE_ACK)
        {
            receive_failed_notify(filter_result);
        }
#else // NRF_802154_DISABLE_BCC_MATCHING
        receive_failed_notify(NRF_802154_RX_ERROR_RUNTIME);
#endif  // NRF_802154_DISABLE_BCC_MATCHING
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_frame_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert((m_state == RADIO_STATE_TX) || (m_state == RADIO_STATE_CCA_TX));
    transmit_started_notify();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_ack_started(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_state == RADIO_STATE_TX_ACK);
    nrf_802154_tx_ack_started(mp_ack);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_ack_transmitted(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_state == RADIO_STATE_TX_ACK);

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    uint32_t t_transmit = TX_RAMP_UP_TIME + nrf_802154_frame_duration_get(mp_ack[PHR_OFFSET],
                                                                          true,
                                                                          true);

    nrf_802154_stat_totals_increment(total_transmit_time, t_transmit);
#endif

    uint8_t * p_received_data = mp_current_rx_buffer->data;

    // Current buffer used for receive operation will be passed to the application
    mp_current_rx_buffer->free = false;

    state_set(RADIO_STATE_RX);

    rx_init();

    received_frame_notify_and_nesting_allow(p_received_data);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_frame_transmitted(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    uint32_t t_listening = 0U;
    uint32_t t_transmit  = 0U;

#endif

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    uint32_t ts = timer_coord_timestamp_get();

    // ts holds now timestamp of the PHYEND event
    nrf_802154_stat_timestamp_write(last_tx_end_timestamp, ts);

    if (m_flags.tx_with_cca)
    {
        m_flags.tx_diminished_prio = false;

        // We calculate the timestamp when ccaidle must happened.
        ts -= nrf_802154_frame_duration_get(mp_tx_data[0], true, true) + RX_TX_TURNAROUND_TIME;

        nrf_802154_stat_timestamp_write(last_cca_idle_timestamp, ts);

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
        t_listening += RX_RAMP_UP_TIME +
                       (ts - nrf_802154_stat_timestamp_read(last_cca_start_timestamp));
        t_transmit += RX_TX_TURNAROUND_TIME;
#endif
    }
    else
    {
#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
        t_transmit += TX_RAMP_UP_TIME;
#endif
    }

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    t_transmit += nrf_802154_frame_duration_get(mp_tx_data[PHR_OFFSET], true, true);

    nrf_802154_stat_totals_increment(total_listening_time, t_listening);
    nrf_802154_stat_totals_increment(total_transmit_time, t_transmit);
#endif
#endif

    if (ack_is_requested(mp_tx_data))
    {
        state_set(RADIO_STATE_RX_ACK);

        bool rx_buffer_free = rx_buffer_is_available();

        nrf_802154_trx_receive_buffer_set(rx_buffer_get());

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
        // Configure the timer coordinator to get a timestamp of the END event which
        // fires several cycles after CRCOK or CRCERROR events.
        nrf_802154_timer_coord_timestamp_prepare(
            nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_END));
#else
        // Configure the timer coordinator to get a timestamp of the CRCOK event.
        nrf_802154_timer_coord_timestamp_prepare(nrf_radio_event_address_get(NRF_RADIO,
                                                                             NRF_RADIO_EVENT_CRCOK));
#endif
#endif

        nrf_802154_trx_receive_ack();

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
        m_listening_start_hp_timestamp = nrf_802154_hp_timer_current_time_get();
#endif

        if (!rx_buffer_free)
        {
            rx_buffer_in_use_set(nrf_802154_rx_buffer_free_find());

            nrf_802154_trx_receive_buffer_set(rx_buffer_get());
        }
    }
    else
    {
        state_set(RADIO_STATE_RX);

        rx_init();

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

static bool ack_match_check_version_2(const uint8_t * p_tx_data, const uint8_t * p_ack_data)
{
    if ((p_ack_data[FRAME_VERSION_OFFSET] & FRAME_VERSION_MASK) != FRAME_VERSION_2)
    {
        return false;
    }

    // Transmitted frame was Version 2
    // For frame version 2 sequence number bit may be suppressed and its check fails.
    // Verify ACK frame using its destination address.
    nrf_802154_frame_parser_mhr_data_t tx_mhr_data;
    nrf_802154_frame_parser_mhr_data_t ack_mhr_data;
    bool                               parse_result;

    parse_result = nrf_802154_frame_parser_mhr_parse(p_tx_data, &tx_mhr_data);
    assert(parse_result);
    (void)parse_result;
    parse_result = nrf_802154_frame_parser_mhr_parse(p_ack_data, &ack_mhr_data);

    if (!parse_result ||
        (tx_mhr_data.p_src_addr == NULL) ||
        (ack_mhr_data.p_dst_addr == NULL) ||
        (tx_mhr_data.src_addr_size != ack_mhr_data.dst_addr_size) ||
        (0 != memcmp(tx_mhr_data.p_src_addr,
                     ack_mhr_data.p_dst_addr,
                     tx_mhr_data.src_addr_size)))
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
    state_set(RADIO_STATE_RX);

    rx_init();

    transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_INVALID_ACK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_ack_received(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // CRC of received frame is correct
    uint8_t * p_ack_data = mp_current_rx_buffer->data;

#if NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED
    uint32_t receive_end_hp_timestamp     = nrf_802154_hp_timer_timestamp_get();
    uint32_t listening_start_hp_timestamp = m_listening_start_hp_timestamp;

    update_total_times_on_receive_end(listening_start_hp_timestamp, receive_end_hp_timestamp,
                                      mp_current_rx_buffer->data[PHR_OFFSET]);
#endif

    if (ack_match_check(mp_tx_data, p_ack_data))
    {
#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
        uint32_t ts = timer_coord_timestamp_get();

        nrf_802154_stat_timestamp_write(last_ack_end_timestamp, ts);
#endif

        rx_buffer_t * p_ack_buffer = mp_current_rx_buffer;

        mp_current_rx_buffer->free = false;

        state_set(RADIO_STATE_RX);
        rx_init();

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

    state_set(RADIO_STATE_RX);
    rx_init();

    cca_notify(channel_was_idle);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_transmit_frame_ccastarted(void)
{
    // This handler provided by trx is never called because parameter notifications_mask
    // of the nrf_802154_trx_transmit_frame does not contain TRX_TRANSMIT_NOTIFICATION_CCASTARTED.
    assert(false);
}

void nrf_802154_trx_transmit_frame_ccaidle(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_state == RADIO_STATE_CCA_TX);
    assert(m_trx_transmit_frame_notifications_mask & TRX_TRANSMIT_NOTIFICATION_CCAIDLE);

#if (NRF_802154_FRAME_TIMESTAMP_ENABLED)
    uint32_t ts = timer_coord_timestamp_get();

    // Configure the timer coordinator to get a timestamp of the PHYEND event.
    nrf_802154_timer_coord_timestamp_prepare(nrf_radio_event_address_get(NRF_RADIO,
                                                                         NRF_RADIO_EVENT_PHYEND));

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

#if (NRF_802154_TOTAL_TIMES_MEASUREMENT_ENABLED)
    uint32_t t_listening = RX_RAMP_UP_TIME + PHY_US_TIME_FROM_SYMBOLS(A_CCA_DURATION_SYMBOLS);

    nrf_802154_stat_totals_increment(total_listening_time, t_listening);
#endif

    state_set(RADIO_STATE_RX);
    rx_init();

    transmit_failed_notify_and_nesting_allow(NRF_802154_TX_ERROR_BUSY_CHANNEL);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_energy_detection_finished(uint8_t ed_sample)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (m_ed_result < ed_sample)
    {
        // Collect maximum value of samples provided by trx
        m_ed_result = ed_sample;
    }

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

        state_set(RADIO_STATE_RX);
        rx_init();

        energy_detected_notify(ed_result_get(m_ed_result));

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

    nrf_802154_irq_disable(RADIO_IRQn);
    nrf_802154_irq_clear_pending(RADIO_IRQn);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

radio_state_t nrf_802154_core_state_get(void)
{
    return m_state;
}

bool nrf_802154_core_sleep(nrf_802154_term_t term_lvl)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = nrf_802154_critical_section_enter();

    if (result)
    {
        if ((m_state != RADIO_STATE_SLEEP) && (m_state != RADIO_STATE_FALLING_ASLEEP))
        {
            result = current_operation_terminate(term_lvl, REQ_ORIG_CORE, true);

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
                    sleep_init();
                    state_set(RADIO_STATE_SLEEP);
                }
            }
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_receive(nrf_802154_term_t              term_lvl,
                             req_originator_t               req_orig,
                             nrf_802154_notification_func_t notify_function,
                             bool                           notify_abort)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = nrf_802154_critical_section_enter();

    if (result)
    {
        if ((m_state != RADIO_STATE_RX) && (m_state != RADIO_STATE_TX_ACK))
        {
            if (critical_section_can_be_processed_now())
            {
                result = current_operation_terminate(term_lvl, req_orig, notify_abort);

                if (result)
                {
                    m_trx_receive_frame_notifications_mask =
                        make_trx_frame_receive_notification_mask();
                    state_set(RADIO_STATE_RX);
                    rx_init();
                }
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
                              const uint8_t                * p_data,
                              bool                           cca,
                              bool                           immediate,
                              nrf_802154_notification_func_t notify_function)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = critical_section_enter_and_verify_timeslot_length();

    if (result)
    {
        // Short-circuit evaluation in place.
        if ((immediate) || (nrf_802154_core_hooks_pre_transmission(p_data, cca)))
        {
            result = current_operation_terminate(term_lvl, req_orig, true);

            if (result)
            {
                m_coex_tx_request_mode                  = nrf_802154_pib_coex_tx_request_mode_get();
                m_trx_transmit_frame_notifications_mask =
                    make_trx_frame_transmit_notification_mask(cca);
                m_flags.tx_diminished_prio =
                    m_coex_tx_request_mode == NRF_802154_COEX_TX_REQUEST_MODE_CCA_DONE;

                state_set(cca ? RADIO_STATE_CCA_TX : RADIO_STATE_TX);
                mp_tx_data = p_data;

                // coverity[check_return]
                result = tx_init(p_data, cca);
                if (immediate)
                {
                    if (!result)
                    {
                        state_set(RADIO_STATE_RX);
                        rx_init();
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
            m_ed_result    = 0;

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
            mp_tx_data = p_data;
            modulated_carrier_init(p_data);
        }

        nrf_802154_critical_section_exit();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_core_notify_buffer_free(uint8_t * p_data)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    rx_buffer_t * p_buffer     = (rx_buffer_t *)p_data;
    bool          in_crit_sect = critical_section_enter_and_verify_timeslot_length();

    p_buffer->free = true;

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

bool nrf_802154_core_channel_update(void)
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
                if (current_operation_terminate(NRF_802154_TERM_NONE, REQ_ORIG_CORE, true))
                {
                    rx_init();
                }
                break;

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
