/*
 * Copyright (c) 2019 - 2021, Nordic Semiconductor ASA
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

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_TRX

#include "nrf_802154_trx.h"

#include <assert.h>
#include <string.h>

#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_nrfx_addons.h"
#include "nrf_802154_types.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_rssi.h"
#include "nrf_802154_swi.h"
#include "nrf_802154_trx_ppi_api.h"
#include "nrf_802154_utils.h"

#include "hal/nrf_egu.h"
#include "hal/nrf_radio.h"
#include "hal/nrf_timer.h"

#include "nrf_802154_procedures_duration.h"
#include "nrf_802154_critical_section.h"
#include "mpsl_fem_protocol_api.h"
#include "platform/nrf_802154_irq.h"

#include "nrf_802154_sl_ant_div.h"

#define EGU_SYNC_EVENT   NRF_EGU_EVENT_TRIGGERED3
#define EGU_SYNC_TASK    NRF_EGU_TASK_TRIGGER3
#define EGU_SYNC_INTMASK NRF_EGU_INT_TRIGGERED3

#if defined(NRF52840_XXAA) || \
    defined(NRF52833_XXAA) || \
    defined(NRF52820_XXAA) || \
    defined(NRF52811_XXAA)
#define PPI_CCAIDLE_FEM  NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE ///< PPI that connects RADIO CCAIDLE event with GPIOTE tasks used by FEM
#define PPI_CHGRP_ABORT  NRF_802154_PPI_ABORT_GROUP                 ///< PPI group used to disable PPIs when async event aborting radio operation is propagated through the system
#define RADIO_BASE       NRF_RADIO_BASE
#elif defined(NRF5340_XXAA)
#define PPI_CCAIDLE_FEM  0
#define RADIO_BASE       NRF_RADIO_NS_BASE
#endif

#if NRF_802154_DISABLE_BCC_MATCHING
#define SHORT_ADDRESS_BCSTART 0UL
#else // NRF_802154_DISABLE_BCC_MATCHING
#define SHORT_ADDRESS_BCSTART NRF_RADIO_SHORT_ADDRESS_BCSTART_MASK
#endif  // NRF_802154_DISABLE_BCC_MATCHING

/// Value set to SHORTS register when no shorts should be enabled.
#define SHORTS_IDLE           0

/// Value set to SHORTS register for RX operation.
#define SHORTS_RX             (NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK | \
                               NRF_RADIO_SHORT_END_DISABLE_MASK |       \
                               SHORT_ADDRESS_BCSTART)

#define SHORTS_RX_FREE_BUFFER (NRF_RADIO_SHORT_RXREADY_START_MASK)

#define SHORTS_TX_ACK         (NRF_RADIO_SHORT_TXREADY_START_MASK | \
                               NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#define SHORTS_CCA_TX         (NRF_RADIO_SHORT_RXREADY_CCASTART_MASK | \
                               NRF_RADIO_SHORT_CCABUSY_DISABLE_MASK |  \
                               NRF_RADIO_SHORT_CCAIDLE_TXEN_MASK |     \
                               NRF_RADIO_SHORT_TXREADY_START_MASK |    \
                               NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#define SHORTS_TX             (NRF_RADIO_SHORT_TXREADY_START_MASK | \
                               NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#define SHORTS_RX_ACK         (NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK | \
                               NRF_RADIO_SHORT_END_DISABLE_MASK)

#define SHORTS_MOD_CARRIER    (NRF_RADIO_SHORT_TXREADY_START_MASK | \
                               NRF_RADIO_SHORT_PHYEND_START_MASK)

#define SHORTS_ED             (NRF_RADIO_SHORT_READY_EDSTART_MASK)

#define SHORTS_CCA            (NRF_RADIO_SHORT_RXREADY_CCASTART_MASK | \
                               NRF_RADIO_SHORT_CCABUSY_DISABLE_MASK)

#define CRC_LENGTH            2          ///< Length of CRC in 802.15.4 frames [bytes]
#define CRC_POLYNOMIAL        0x011021   ///< Polynomial used for CRC calculation in 802.15.4 frames

#define TXRU_TIME             40         ///< Transmitter ramp up time [us]
#define EVENT_LAT             23         ///< END event latency [us]
#define MAX_RXRAMPDOWN_CYCLES 32         ///< Maximum number of cycles that RX ramp-down might take

#define RSSI_SETTLE_TIME_US   15         ///< Time required for RSSI measurements to become valid after signal level change.

#if NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
void nrf_802154_radio_irq_handler(void); ///< Prototype required by internal RADIO IRQ handler
#endif  // NRF_802154_INTERNAL_RADIO_IRQ_HANDLING

/// Common parameters for the FEM handling.
static const mpsl_fem_event_t m_activate_rx_cc0 =
{
    .type         = MPSL_FEM_EVENT_TYPE_TIMER,
    .override_ppi = false,
    .event.timer  =
    {
        .p_timer_instance     = NRF_802154_TIMER_INSTANCE,
        .compare_channel_mask = ((1 << NRF_TIMER_CC_CHANNEL0) | (1 << NRF_TIMER_CC_CHANNEL2)),
        .counter_period       = {
            .end = RX_RAMP_UP_TIME
        },
    },
};

static const mpsl_fem_event_t m_activate_tx_cc0 =
{
    .type         = MPSL_FEM_EVENT_TYPE_TIMER,
    .override_ppi = false,
    .event.timer  =
    {
        .p_timer_instance     = NRF_802154_TIMER_INSTANCE,
        .compare_channel_mask = ((1 << NRF_TIMER_CC_CHANNEL0) | (1 << NRF_TIMER_CC_CHANNEL2)),
        .counter_period       = {
            .end = TX_RAMP_UP_TIME
        },
    },
};

static const mpsl_fem_event_t m_ccaidle =
{
    .type                           = MPSL_FEM_EVENT_TYPE_GENERIC,
    .override_ppi                   = true,
    .ppi_ch_id                      = PPI_CCAIDLE_FEM,
    .event.generic.register_address = ((uint32_t)RADIO_BASE + (uint32_t)NRF_RADIO_EVENT_CCAIDLE)
};

/**@brief Fal event used by @ref nrf_802154_trx_transmit_ack and @ref txack_finish */
static mpsl_fem_event_t m_activate_tx_cc0_timeshifted;

static volatile trx_state_t m_trx_state;

typedef struct
{
#if !NRF_802154_DISABLE_BCC_MATCHING
    bool psdu_being_received; ///< If PSDU is currently being received.

#endif

    bool missing_receive_buffer; ///!< If trx entered receive state without receive buffer

#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
    bool tx_started;             ///< If the requested transmission has started.

#endif  // NRF_802154_TX_STARTED_NOTIFY_ENABLED

    bool          rssi_started;

    volatile bool rssi_settled;
} nrf_802154_flags_t;

static nrf_802154_flags_t m_flags; ///< Flags used to store the current driver state.

/**@brief Value of TIMER internal counter from which the counting is resumed on RADIO.EVENTS_END event. */
static volatile uint32_t m_timer_value_on_radio_end_event;
static volatile bool     m_transmit_with_cca;

static void rxframe_finish_disable_ppis(void);
static void rxack_finish_disable_ppis(void);
static void txframe_finish_disable_ppis(bool cca);

static void go_idle_abort(void);
static void receive_frame_abort(void);
static void receive_ack_abort(void);
static void transmit_frame_abort(void);
static void transmit_ack_abort(void);
static void standalone_cca_abort(void);
static void continuous_carrier_abort(void);
static void modulated_carrier_abort(void);
static void energy_detection_abort(void);

/** Clear flags describing frame being received. */
void rx_flags_clear(void)
{
    m_flags.missing_receive_buffer = false;
#if !NRF_802154_DISABLE_BCC_MATCHING
    m_flags.psdu_being_received = false;
#endif // !NRF_802154_DISABLE_BCC_MATCHING
}

static void * volatile mp_receive_buffer;

/** Initialize TIMER peripheral used by the driver. */
void nrf_timer_init(void)
{
    nrf_timer_mode_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_MODE_TIMER);
    nrf_timer_bit_width_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_frequency_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_FREQ_1MHz);

#if NRF_802154_DISABLE_BCC_MATCHING
    // Setup timer for detecting PSDU reception.
    nrf_timer_mode_set(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_MODE_COUNTER);
    nrf_timer_bit_width_set(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_BIT_WIDTH_8);
#endif
}

/** Reset radio peripheral. */
static void nrf_radio_reset(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_radio_power_set(NRF_RADIO, false);
    nrf_radio_power_set(NRF_RADIO, true);

    nrf_802154_log_global_event(NRF_802154_LOG_VERBOSITY_LOW,
                                NRF_802154_LOG_GLOBAL_EVENT_ID_RADIO_RESET, 0U);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void channel_set(uint8_t channel)
{
    assert(channel >= 11U && channel <= 26U);

    nrf_radio_frequency_set(NRF_RADIO, 2405U + 5U * (channel - 11U));
}

static void cca_configuration_update(void)
{
    nrf_802154_cca_cfg_t cca_cfg;

    nrf_802154_pib_cca_cfg_get(&cca_cfg);
    nrf_radio_cca_configure(NRF_RADIO,
                            cca_cfg.mode,
                            nrf_802154_rssi_cca_ed_threshold_corrected_get(cca_cfg.ed_threshold),
                            cca_cfg.corr_threshold,
                            cca_cfg.corr_limit);
}

/** Initialize interrupts for radio peripheral. */
static void irq_init(void)
{
#if !NRF_802154_IRQ_PRIORITY_ALLOWED(NRF_802154_IRQ_PRIORITY)
#error NRF_802154_IRQ_PRIORITY value out of the allowed range.
#endif

#if NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
    nrf_802154_irq_init(RADIO_IRQn, NRF_802154_IRQ_PRIORITY, nrf_802154_radio_irq_handler);
#endif
    nrf_802154_irq_enable(RADIO_IRQn);
}

static void trigger_disable_to_start_rampup(void)
{
    if (!nrf_802154_trx_ppi_for_ramp_up_was_triggered())
    {
        nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);
    }
}

/** Configure FEM to set LNA at appropriate time. */
static void fem_for_lna_set(void)
{
    if (mpsl_fem_lna_configuration_set(&m_activate_rx_cc0, NULL) == 0)
    {
        nrf_timer_shorts_enable(m_activate_rx_cc0.event.timer.p_timer_instance,
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK);

        nrf_802154_trx_ppi_for_fem_set();
    }
}

/** Reset FEM configuration for LNA. */
static void fem_for_lna_reset(void)
{
    mpsl_fem_lna_configuration_clear();
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
    nrf_802154_trx_ppi_for_fem_clear();
}

/** Configure FEM to set PA at appropriate time.
 *
 * @note This function must be called before ramp up PPIs are configured.
 */
static void fem_for_pa_set(void)
{
    if (mpsl_fem_pa_configuration_set(&m_activate_tx_cc0, NULL) == 0)
    {
        nrf_timer_shorts_enable(m_activate_tx_cc0.event.timer.p_timer_instance,
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
        nrf_802154_trx_ppi_for_fem_set();
    }
}

/** Reset FEM configuration for PA.
 *
 * @note This function must be called before ramp up PPIs are configured.
 */
static void fem_for_pa_reset(void)
{
    mpsl_fem_pa_configuration_clear();
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
    nrf_802154_trx_ppi_for_fem_clear();
    mpsl_fem_deactivate_now(MPSL_FEM_PA);
}

/** Configure FEM for TX procedure.
 *
 * @note This function must be called before ramp up PPIs are configured.
 */
static void fem_for_tx_set(bool cca)
{
    bool success;

    if (cca)
    {
        bool pa_set  = false;
        bool lna_set = false;

        if (mpsl_fem_lna_configuration_set(&m_activate_rx_cc0, &m_ccaidle) == 0)
        {
            lna_set = true;
        }

        if (mpsl_fem_pa_configuration_set(&m_ccaidle, NULL) == 0)
        {
            pa_set = true;
        }

        success = pa_set || lna_set;

    }
    else
    {
        success = (mpsl_fem_pa_configuration_set(&m_activate_tx_cc0, NULL) == 0);
    }

    if (success)
    {
        nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
        nrf_802154_trx_ppi_for_fem_set();
    }
}

static void fem_for_tx_reset(bool cca)
{
    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK);

    if (cca)
    {
        mpsl_fem_lna_configuration_clear();
        mpsl_fem_pa_configuration_clear();
    }
    else
    {
        mpsl_fem_pa_configuration_clear();
    }

    nrf_802154_trx_ppi_for_fem_clear();
    nrf_802154_trx_ppi_for_ramp_up_propagation_delay_wait();
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
}

void nrf_802154_trx_module_reset(void)
{
    m_trx_state                      = TRX_STATE_DISABLED;
    m_timer_value_on_radio_end_event = 0;
    m_transmit_with_cca              = false;
    mp_receive_buffer                = NULL;

    memset(&m_flags, 0, sizeof(m_flags));
}

void nrf_802154_trx_init(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    nrf_802154_trx_module_reset();

    nrf_timer_init();
#if defined(RADIO_INTENSET_SYNC_Msk)
    nrf_802154_swi_init();
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_enable(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_trx_state == TRX_STATE_DISABLED);

    nrf_radio_reset();

    nrf_radio_packet_conf_t packet_conf;

    nrf_radio_mode_set(NRF_RADIO, NRF_RADIO_MODE_IEEE802154_250KBIT);

    memset(&packet_conf, 0, sizeof(packet_conf));
    packet_conf.lflen  = 8;
    packet_conf.plen   = NRF_RADIO_PREAMBLE_LENGTH_32BIT_ZERO;
    packet_conf.crcinc = true;
    packet_conf.maxlen = MAX_PACKET_SIZE;
    nrf_radio_packet_configure(NRF_RADIO, &packet_conf);

    nrf_radio_modecnf0_set(NRF_RADIO, true, 0);

    // Configure CRC
    nrf_radio_crc_configure(NRF_RADIO, CRC_LENGTH, NRF_RADIO_CRC_ADDR_IEEE802154, CRC_POLYNOMIAL);

    // Configure CCA
    cca_configuration_update();

    // Set channel
    channel_set(nrf_802154_pib_channel_get());

    // Custom initialization operations
    nrf_802154_custom_part_of_radio_init();

    irq_init();

    assert(nrf_radio_shorts_get(NRF_RADIO) == SHORTS_IDLE);

#if defined(NRF52840_XXAA) || \
    defined(NRF52833_XXAA) || \
    defined(NRF52820_XXAA) || \
    defined(NRF52811_XXAA)
    mpsl_fem_abort_set(nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_DISABLED),
                       PPI_CHGRP_ABORT);
#endif

    mpsl_fem_deactivate_now(MPSL_FEM_ALL);

    m_trx_state = TRX_STATE_IDLE;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void ppi_all_clear(void)
{
    switch (m_trx_state)
    {
        case TRX_STATE_IDLE:
        case TRX_STATE_GOING_IDLE:
        case TRX_STATE_RXFRAME_FINISHED:
        case TRX_STATE_FINISHED:
            // Intentionally empty. PPIs are not configured in this state.
            break;

        case TRX_STATE_RXFRAME:
            rxframe_finish_disable_ppis();
            break;

        case TRX_STATE_RXACK:
            rxack_finish_disable_ppis();
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        case TRX_STATE_TXFRAME:
            txframe_finish_disable_ppis(m_transmit_with_cca);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        case TRX_STATE_TXACK:
            nrf_802154_trx_ppi_for_ack_tx_clear();
            // FEM PPIs are not configured for this state. TIMER was started in TRX_STATE_RXFRAME
            // and PPIs starting timer were cleared when exiting TRX_STATE_RXFRAME.
            break;

        case TRX_STATE_STANDALONE_CCA:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        case TRX_STATE_CONTINUOUS_CARRIER:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        case TRX_STATE_MODULATED_CARRIER:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        case TRX_STATE_ENERGY_DETECTION:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        default:
            assert(false);
    }
}

static void fem_power_down_now(void)
{
    mpsl_fem_deactivate_now(MPSL_FEM_ALL);

    if (nrf_802154_trx_ppi_for_fem_powerdown_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL0))
    {
        // FEM requires timer to run to perform powering down operation
        nrf_timer_event_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_EVENT_COMPARE0);

        nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START);

        while (!nrf_timer_event_check(NRF_802154_TIMER_INSTANCE, NRF_TIMER_EVENT_COMPARE0))
        {
            // Wait until the event is set.
            // The waiting takes several microseconds
        }

        nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
        nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
        nrf_802154_trx_ppi_for_fem_powerdown_clear();
    }
}

void nrf_802154_trx_disable(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (m_trx_state != TRX_STATE_DISABLED)
    {
        nrf_radio_power_set(NRF_RADIO, false);
        nrf_802154_irq_clear_pending(RADIO_IRQn);

        /* While the RADIO is powered off deconfigure any PPIs used directly by trx module */
        ppi_all_clear();

#if !NRF_802154_DISABLE_BCC_MATCHING && defined(RADIO_INTENSET_SYNC_Msk)
        nrf_egu_int_disable(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK);
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_SYNC_EVENT);
#endif

        /* Stop & deconfigure timer */
        nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                                 NRF_TIMER_SHORT_COMPARE0_STOP_MASK |
                                 NRF_TIMER_SHORT_COMPARE1_STOP_MASK);
        nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

        nrf_radio_power_set(NRF_RADIO, true);

#if defined(NRF52840_XXAA) ||     \
        defined(NRF52833_XXAA) || \
        defined(NRF52820_XXAA) || \
        defined(NRF52811_XXAA)
        mpsl_fem_abort_clear();
#endif

        // TODO: Deconfigure FAL PA and LNA here?
        mpsl_fem_deactivate_now(MPSL_FEM_ALL);

        if (m_trx_state != TRX_STATE_IDLE)
        {
            fem_power_down_now();
        }

#if NRF_802154_DISABLE_BCC_MATCHING
        // Anomaly 78: use SHUTDOWN instead of STOP and CLEAR.
        nrf_timer_task_trigger(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
        nrf_timer_shorts_disable(NRF_802154_COUNTER_TIMER_INSTANCE,
                                 NRF_TIMER_SHORT_COMPARE1_STOP_MASK);
#else
        m_flags.psdu_being_received = false;
#endif
        m_flags.missing_receive_buffer = false;
        m_flags.rssi_started           = false;
#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
        m_flags.tx_started = false;
#endif

        m_trx_state = TRX_STATE_DISABLED;

        nrf_802154_log_global_event(NRF_802154_LOG_VERBOSITY_LOW,
                                    NRF_802154_LOG_GLOBAL_EVENT_ID_RADIO_RESET, 0U);
    }
    else
    {
        // Intentionally empty
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void rx_automatic_antenna_handle(void)
{
    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
        case TRX_STATE_RXFRAME_FINISHED:
            nrf_802154_sl_ant_div_rx_started_notify();
            break;

        case TRX_STATE_ENERGY_DETECTION:
            // Intentionally empty - notification is called from core when requesting energy detection.
            // This is done due to possibility of nrf_802154_trx_energy_detection being called multiple times
            // during one energy detection from the point of view of application, if the entire procedure does not
            // fit in single timeslot.
            break;

        case TRX_STATE_TXACK:
            nrf_802154_sl_ant_div_txack_notify();
            break;

        default:
            assert(false);
            break;
    }
}

/**
 * Updates the antenna for reception, according to antenna diversity configuration.
 */
static void rx_antenna_update(void)
{
    bool                         result = true;
    nrf_802154_sl_ant_div_mode_t mode   = nrf_802154_sl_ant_div_cfg_mode_get(
        NRF_802154_SL_ANT_DIV_OP_RX);

    switch (mode)
    {
        case NRF_802154_SL_ANT_DIV_MODE_DISABLED:
            break;

        case NRF_802154_SL_ANT_DIV_MODE_MANUAL:
            result = nrf_802154_sl_ant_div_antenna_set(
                nrf_802154_sl_ant_div_cfg_antenna_get(NRF_802154_SL_ANT_DIV_OP_RX));
            break;

        case NRF_802154_SL_ANT_DIV_MODE_AUTO:
            rx_automatic_antenna_handle();
            break;

        default:
            assert(false);
            break;
    }

    assert(result);
    (void)result;
}

/**
 * Updates the antenna for transmission, according to antenna diversity configuration.
 *
 * Antenna diversity for tx is not currently supported. If antenna diversity is not
 * in disabled state, default antenna will always be used for transmission.
 */
static void tx_antenna_update(void)
{
    bool                         result = true;
    nrf_802154_sl_ant_div_mode_t mode   = nrf_802154_sl_ant_div_cfg_mode_get(
        NRF_802154_SL_ANT_DIV_OP_TX);

    switch (mode)
    {
        case NRF_802154_SL_ANT_DIV_MODE_DISABLED:
            /* Intentionally empty. */
            break;

        case NRF_802154_SL_ANT_DIV_MODE_MANUAL:
            result = nrf_802154_sl_ant_div_antenna_set(
                nrf_802154_sl_ant_div_cfg_antenna_get(NRF_802154_SL_ANT_DIV_OP_TX));
            break;

        case NRF_802154_SL_ANT_DIV_MODE_AUTO:
        default:
            assert(false);
            break;
    }

    if (!result)
    {
        assert(false);
    }
}

void nrf_802154_trx_antenna_update(void)
{
    assert(m_trx_state != TRX_STATE_DISABLED);

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
        case TRX_STATE_RXFRAME_FINISHED:
        case TRX_STATE_ENERGY_DETECTION:
        case TRX_STATE_TXACK:
            rx_antenna_update();
            break;

        case TRX_STATE_STANDALONE_CCA:
        case TRX_STATE_RXACK:
        case TRX_STATE_TXFRAME:
        case TRX_STATE_CONTINUOUS_CARRIER:
        case TRX_STATE_MODULATED_CARRIER:
            tx_antenna_update();
            break;

        default:
            /* Intentionally empty */
            break;
    }
}

void nrf_802154_trx_channel_set(uint8_t channel)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    channel_set(channel);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_cca_configuration_update(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    cca_configuration_update();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/** Check if PSDU is currently being received.
 *
 * @returns True if radio is receiving PSDU, false otherwise.
 */
bool nrf_802154_trx_psdu_is_being_received(void)
{
#if NRF_802154_DISABLE_BCC_MATCHING
    nrf_timer_task_trigger(NRF_802154_COUNTER_TIMER_INSTANCE,
                           nrf_timer_capture_task_get(NRF_TIMER_CC_CHANNEL0));
    uint32_t counter = nrf_timer_cc_get(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL0);

    assert(counter <= 1);

    return counter > 0;
#else // NRF_802154_DISABLE_BCC_MATCHING
    return m_flags.psdu_being_received;
#endif  // NRF_802154_DISABLE_BCC_MATCHING
}

bool nrf_802154_trx_receive_is_buffer_missing(void)
{
    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
        /* no break */
        case TRX_STATE_RXACK:
            return m_flags.missing_receive_buffer;

        default:
            assert(!m_flags.missing_receive_buffer);
            return false;
    }
}

static void receive_buffer_missing_buffer_set(void * p_receive_buffer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint32_t shorts = SHORTS_IDLE;

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
            shorts = SHORTS_RX | SHORTS_RX_FREE_BUFFER;
            break;

        case TRX_STATE_RXACK:
            shorts = SHORTS_RX_ACK | SHORTS_RX_FREE_BUFFER;
            break;

        default:
            assert(false);
    }

    m_flags.missing_receive_buffer = false;

    nrf_radio_packetptr_set(NRF_RADIO, p_receive_buffer);

    nrf_radio_shorts_set(NRF_RADIO, shorts);

    if (nrf_radio_state_get(NRF_RADIO) == NRF_RADIO_STATE_RXIDLE)
    {
        nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_START);
    }

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);
}

bool nrf_802154_trx_receive_buffer_set(void * p_receive_buffer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool result = false;

    mp_receive_buffer = p_receive_buffer;

    if ((p_receive_buffer != NULL) && m_flags.missing_receive_buffer)
    {
        receive_buffer_missing_buffer_set(p_receive_buffer);
        result = true;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);

    return result;
}

void nrf_802154_trx_receive_frame(uint8_t                                bcc,
                                  nrf_802154_trx_receive_notifications_t notifications_mask)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint32_t ints_to_enable = 0U;
    uint32_t shorts         = SHORTS_RX;

    // Force the TIMER to be stopped and count from 0.
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

    m_trx_state = TRX_STATE_RXFRAME;

    // Clear filtering flag
    rx_flags_clear();
    // Clear the RSSI measurement flag.
    m_flags.rssi_started = false;

    m_flags.rssi_settled = false;

    nrf_radio_txpower_set(NRF_RADIO, nrf_802154_pib_tx_power_get());

    if (mp_receive_buffer != NULL)
    {
        m_flags.missing_receive_buffer = false;
        nrf_radio_packetptr_set(NRF_RADIO, mp_receive_buffer);
        shorts |= SHORTS_RX_FREE_BUFFER;
    }
    else
    {
        m_flags.missing_receive_buffer = true;
    }

    // Set shorts
    nrf_radio_shorts_set(NRF_RADIO, shorts);

    // Set BCC
#if !NRF_802154_DISABLE_BCC_MATCHING
    assert(bcc != 0U);
    nrf_radio_bcc_set(NRF_RADIO, bcc * 8U);
#else
    assert(bcc == 0U);
    (void)bcc;
#endif // !NRF_802154_DISABLE_BCC_MATCHING

    // Enable IRQs
#if !NRF_802154_DISABLE_BCC_MATCHING || NRF_802154_NOTIFY_CRCERROR
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);
    ints_to_enable |= NRF_RADIO_INT_CRCERROR_MASK;
#endif // !NRF_802154_DISABLE_BCC_MATCHING ||NRF_802154_NOTIFY_CRCERROR
#if !NRF_802154_DISABLE_BCC_MATCHING
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH);
    ints_to_enable |= NRF_RADIO_INT_BCMATCH_MASK;
#endif // !NRF_802154_DISABLE_BCC_MATCHING
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCOK);
    ints_to_enable |= NRF_RADIO_INT_CRCOK_MASK;

    if ((notifications_mask & TRX_RECEIVE_NOTIFICATION_STARTED) != 0U)
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
        ints_to_enable |= NRF_RADIO_INT_ADDRESS_MASK;
    }

    bool allow_sync_swi = false;

#ifdef RADIO_INTENSET_SYNC_Msk
    if (((notifications_mask & TRX_RECEIVE_NOTIFICATION_PRESTARTED) != 0U) ||
        (NRF_802154_SL_ANT_DIV_MODE_DISABLED !=
         nrf_802154_sl_ant_div_cfg_mode_get(NRF_802154_SL_ANT_DIV_OP_RX)))
    {
        allow_sync_swi = true;
    }
#endif

    if (allow_sync_swi)
    {
#if (NRF_802154_DISABLE_BCC_MATCHING || !defined(RADIO_INTENSET_SYNC_Msk))
        assert(false);
#else
        // The RADIO can't generate interrupt on EVENT_SYNC. Path to generate interrupt:
        // RADIO.EVENT_SYNC -> PPI_RADIO_SYNC_EGU_SYNC -> EGU.TASK_SYNC -> EGU.EVENT_SYNC ->
        // SWI_IRQHandler (in nrf_802154_swi.c), calls nrf_802154_trx_swi_irq_handler
        nrf_802154_trx_ppi_for_radio_sync_set(EGU_SYNC_TASK);

        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_SYNC);
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_SYNC_EVENT);
        nrf_egu_int_enable(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK);
#endif
    }

    nrf_radio_int_enable(NRF_RADIO, ints_to_enable);

    // Set FEM
    uint32_t delta_time;

    if (mpsl_fem_lna_configuration_set(&m_activate_rx_cc0, NULL) == 0)
    {
        delta_time = nrf_timer_cc_get(NRF_802154_TIMER_INSTANCE,
                                      NRF_TIMER_CC_CHANNEL0);
    }
    else
    {
        delta_time = 1;
        nrf_timer_cc_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL0, delta_time);
    }

    m_timer_value_on_radio_end_event = delta_time;

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Let the TIMER stop on last event required by a FEM
    nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE,
                            NRF_TIMER_SHORT_COMPARE0_STOP_MASK);

#if NRF_802154_DISABLE_BCC_MATCHING
    nrf_timer_shorts_enable(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE1_STOP_MASK);
    nrf_timer_cc_set(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL1, 1);
#endif // NRF_802154_DISABLE_BCC_MATCHING

    // Set PPIs
#if NRF_802154_DISABLE_BCC_MATCHING
#error Configuration unsupported anymore
#endif // NRF_802154_DISABLE_BCC_MATCHING
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, true);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_receive_ack(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint32_t shorts         = SHORTS_RX_ACK;
    uint32_t ints_to_enable = 0U;

    m_trx_state = TRX_STATE_RXACK;

    if (mp_receive_buffer != NULL)
    {
        m_flags.missing_receive_buffer = false;
        nrf_radio_packetptr_set(NRF_RADIO, mp_receive_buffer);
        shorts |= SHORTS_RX_FREE_BUFFER;
    }
    else
    {
        m_flags.missing_receive_buffer = true;
    }

    nrf_radio_shorts_set(NRF_RADIO, shorts);

    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
    ints_to_enable |= NRF_RADIO_INT_ADDRESS_MASK;
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCOK);
    ints_to_enable |= NRF_RADIO_INT_CRCOK_MASK;
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);
    ints_to_enable |= NRF_RADIO_INT_CRCERROR_MASK;

    nrf_radio_int_enable(NRF_RADIO, ints_to_enable);

    fem_for_lna_set();
    nrf_802154_trx_antenna_update();
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

bool nrf_802154_trx_rssi_measure(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = false;

    if (m_trx_state == TRX_STATE_RXFRAME)
    {
        // When TRX is in RXFRAME the RADIO may be also ramping down after previous operation or still ramping up
        nrf_radio_state_t radio_state = nrf_radio_state_get(NRF_RADIO);

        if ((radio_state == RADIO_STATE_STATE_RxIdle) || (radio_state == RADIO_STATE_STATE_Rx))
        {
            if (!m_flags.rssi_settled)
            {
                // This operation is requested first time after nrf_802154_trx_receive_frame has been called
                // Radio is ramped up, but we need to wait RSSISETTLE time.
                // Precisely, we need to wait NRF_RADIO_EVENT_RSSIEND between READY->START short worked
                // and RSSI_START task is triggered. Due to limited resources we assume worst case and
                // wait whole time when rssi_measure is requested first time after nrf_802154_trx_receive_frame.
                nrf_802154_delay_us(RSSI_SETTLE_TIME_US);
                m_flags.rssi_settled = true;
            }

            nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_RSSIEND);
            nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_RSSISTART);
            m_flags.rssi_started = true;

            result = true;
        }
        else
        {
            // The RADIO may be:
            // - still ramping down after transmit
            // - ramping up for receive
            // - ramping down after frame is received (shorts)
        }
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

bool nrf_802154_trx_rssi_measure_is_started(void)
{
    return m_flags.rssi_started;
}

uint8_t nrf_802154_trx_rssi_last_sample_get(void)
{
    return nrf_radio_rssi_sample_get(NRF_RADIO);
}

bool nrf_802154_trx_rssi_sample_is_available(void)
{
    return nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_RSSIEND);
}

void nrf_802154_trx_transmit_frame(const void                            * p_transmit_buffer,
                                   bool                                    cca,
                                   nrf_802154_trx_transmit_notifications_t notifications_mask)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint32_t ints_to_enable = 0U;

    // Force the TIMER to be stopped and count from 0.
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

    m_trx_state         = TRX_STATE_TXFRAME;
    m_transmit_with_cca = cca;

    nrf_radio_txpower_set(NRF_RADIO, nrf_802154_pib_tx_power_get());
    nrf_radio_packetptr_set(NRF_RADIO, p_transmit_buffer);

    // Set shorts
    if (cca)
    {
        nrf_radio_shorts_set(NRF_RADIO, SHORTS_CCA_TX);
    }
    else
    {
        nrf_radio_shorts_set(NRF_RADIO, SHORTS_TX);
    }

    // Enable IRQs
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_PHYEND);
    ints_to_enable |= NRF_RADIO_INT_PHYEND_MASK;

    if (cca)
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);

        ints_to_enable |= NRF_RADIO_INT_CCABUSY_MASK;

        if ((notifications_mask & TRX_TRANSMIT_NOTIFICATION_CCAIDLE) != 0U)
        {
            nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE);
            ints_to_enable |= NRF_RADIO_INT_CCAIDLE_MASK;
        }

        if ((notifications_mask & TRX_TRANSMIT_NOTIFICATION_CCASTARTED) != 0U)
        {
            nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);
            ints_to_enable |= NRF_RADIO_INT_READY_MASK;
        }
    }

    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
    ints_to_enable    |= NRF_RADIO_INT_ADDRESS_MASK;
    m_flags.tx_started = false;
#endif // NRF_802154_TX_STARTED_NOTIFY_ENABLED

    nrf_radio_int_enable(NRF_RADIO, ints_to_enable);

    fem_for_tx_set(cca);
    nrf_802154_trx_antenna_update();
    nrf_802154_trx_ppi_for_ramp_up_set(cca ? NRF_RADIO_TASK_RXEN : NRF_RADIO_TASK_TXEN, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

bool nrf_802154_trx_transmit_ack(const void * p_transmit_buffer, uint32_t delay_us)
{
    /* Assumptions on peripherals
     * TIMER is running, is counting from value saved in m_timer_value_on_radio_end_event,
     * which trigered on END event, which happened EVENT_LAT us after frame on air receive was finished.
     * RADIO is DISABLED
     * PPIs are DISABLED
     */

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = false;

    assert(m_trx_state == TRX_STATE_RXFRAME_FINISHED);
    assert(p_transmit_buffer != NULL);

    m_trx_state = TRX_STATE_TXACK;

    // Set TIMER's CC to the moment when ramp-up should occur.
    if (delay_us <= TXRU_TIME + EVENT_LAT)
    {
        nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
        return result;
    }

    uint32_t timer_cc_ramp_up_start = m_timer_value_on_radio_end_event + delay_us - TXRU_TIME -
                                      EVENT_LAT;

    nrf_timer_cc_set(NRF_802154_TIMER_INSTANCE,
                     NRF_TIMER_CC_CHANNEL1,
                     timer_cc_ramp_up_start);

    nrf_radio_packetptr_set(NRF_RADIO, p_transmit_buffer);

    // Set shorts
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_TX_ACK);

    // Clear TXREADY event to detect if PPI worked
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_TXREADY);

    // Set FEM
    // Note: the TIMER is running, ramp up will start in timer_cc_ramp_up_start tick
    // Assumption here is that FEM activation takes no more than TXRU_TIME.
    m_activate_tx_cc0_timeshifted = m_activate_tx_cc0;

    // Set the moment for FEM at which real transmission starts.
    m_activate_tx_cc0_timeshifted.event.timer.counter_period.end = timer_cc_ramp_up_start +
                                                                   TXRU_TIME;

    if (mpsl_fem_pa_configuration_set(&m_activate_tx_cc0_timeshifted, NULL) == 0)
    {
        // FEM scheduled its operations on timer, so the timer must be running until last
        // operation scheduled by the FEM (TIMER's CC0), which is later than radio ramp up
        nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
    }
    else
    {
        // FEM didn't schedule anything on timer, so the timer may be stopped when radio ramp-up
        // is triggered
        nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE1_STOP_MASK);
    }

    // Select antenna
    nrf_802154_trx_antenna_update();

    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_PHYEND);
#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
#endif

    // Set PPIs
    nrf_802154_trx_ppi_for_ack_tx_set();

    // Since this point the transmission is armed on TIMER's CC1

    // Detect if PPI will work in future or has just worked.
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_CAPTURE3);
    uint32_t timer_cc_now = nrf_timer_cc_get(NRF_802154_TIMER_INSTANCE,
                                             NRF_TIMER_CC_CHANNEL3);
    uint32_t timer_cc_fem_start = nrf_timer_cc_get(NRF_802154_TIMER_INSTANCE,
                                                   NRF_TIMER_CC_CHANNEL0);

    // When external PA uses a timer, it should be configured to a time later than ramp up time. In
    // such case, the timer stops with shorts on PA timer. But if external PA does not use a timer,
    // FEM time is set to a value in the past that was used by LNA. After the timer overflows,
    // the timer stops with a short on the past value used by LNA. We have to detect if the current
    // timer value is after the overflow.
    if ((timer_cc_now < timer_cc_ramp_up_start) &&
        ((timer_cc_fem_start >= timer_cc_ramp_up_start) || (timer_cc_now > timer_cc_fem_start)))
    {
        result = true;
    }
    else
    {
        nrf_802154_trx_ppi_for_ramp_up_propagation_delay_wait();

        if (nrf_radio_state_get(NRF_RADIO) == NRF_RADIO_STATE_TXRU)
        {
            result = true;
        }
        else if (nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_TXREADY))
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }

    if (result)
    {
        uint32_t ints_to_enable = NRF_RADIO_INT_PHYEND_MASK;

#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
        ints_to_enable |= NRF_RADIO_INT_ADDRESS_MASK;
#endif // NRF_802154_TX_STARTED_NOTIFY_ENABLED

        nrf_radio_int_enable(NRF_RADIO, ints_to_enable);
    }
    else
    {
        /* We were to late with setting up PPI_TIMER_ACK, ack transmission was not triggered and
         * will not be triggered in future.
         */
        nrf_802154_trx_ppi_for_ack_tx_clear();

        /* As the timer was running during operation, it is possible we were able to configure
         * FEM thus it may trigger in future or may started PA activation.
         */
        mpsl_fem_pa_configuration_clear();
        mpsl_fem_deactivate_now(MPSL_FEM_PA);

        nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

        /* No callbacks will be called */
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

static inline void wait_until_radio_is_disabled(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool radio_is_disabled = false;

    // RADIO should enter DISABLED state after no longer than RX ramp-down time, which is equal
    // approximately 0.5us. Taking a bold assumption that a single iteration of the loop takes
    // one cycle to complete, 32 iterations would amount to exactly 0.5 us of execution time.
    // Please note that this approach ignores software latency completely, i.e. RADIO should
    // have changed state already before entering this function due to ISR processing delays.
    for (uint32_t i = 0; i < MAX_RXRAMPDOWN_CYCLES; i++)
    {
        if (nrf_radio_state_get(NRF_RADIO) == NRF_RADIO_STATE_DISABLED)
        {
            radio_is_disabled = true;
            break;
        }
    }

    assert(radio_is_disabled);
    (void)radio_is_disabled;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxframe_finish_disable_ppis(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, true);
#if defined(RADIO_INTENSET_SYNC_Msk)
    nrf_802154_trx_ppi_for_radio_sync_clear(EGU_SYNC_TASK);
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxframe_finish_disable_fem_activation(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Disable LNA activation
    mpsl_fem_lna_configuration_clear();
    // Disable short used by LNA activation
    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                             NRF_TIMER_SHORT_COMPARE0_STOP_MASK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxframe_finish_disable_ints(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    uint32_t ints_to_disable = NRF_RADIO_INT_CRCOK_MASK | NRF_RADIO_INT_ADDRESS_MASK;

#if !NRF_802154_DISABLE_BCC_MATCHING || NRF_802154_NOTIFY_CRCERROR
    ints_to_disable |= NRF_RADIO_INT_CRCERROR_MASK;
#endif // !NRF_802154_DISABLE_BCC_MATCHING || NRF_802154_NOTIFY_CRCERROR
#if !NRF_802154_DISABLE_BCC_MATCHING
    ints_to_disable |= NRF_RADIO_INT_BCMATCH_MASK;
#endif // !NRF_802154_DISABLE_BCC_MATCHING
    nrf_radio_int_disable(NRF_RADIO, ints_to_disable);

#if !NRF_802154_DISABLE_BCC_MATCHING && defined(RADIO_INTENSET_SYNC_Msk)
    nrf_egu_int_disable(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK);
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxframe_finish_psdu_is_not_being_received(void)
{
#if NRF_802154_DISABLE_BCC_MATCHING
    // Anomaly 78: use SHUTDOWN instead of STOP and CLEAR.
    nrf_timer_task_trigger(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
    nrf_timer_shorts_disable(NRF_802154_COUNTER_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE1_STOP_MASK);
#else
    m_flags.psdu_being_received = false;
#endif // NRF_802154_DISABLE_BCC_MATCHING
}

static void rxframe_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    /* Note that CRCOK/CRCERROR event is generated several cycles before END event.
     *
     * Below shown what is happening in the hardware
     *
     * TIMER is started by path:
     * RADIO.SHORT_END_DISABLE -> RADIO.TASKS_DISABLE -> RADIO.EVENTS_DISABLED ->
     *    PPI_DISABLED_EGU -> EGU.TASKS_TRIGGER -> EGU.EVENTS_TRIGGERED -> PPI_EGU_TIMER_START -> TIMER.TASKS_START
     *
     * FEM's LNA mode is disabled by path:
     * RADIO.SHORT_END_DISABLE -> RADIO.TASKS_DISABLE -> RADIO.EVENTS_DISABLED -> (FEM's PPI triggering disable LNA operation,
     *                                                                             see mpsl_fem_abort_set() )
     *
     * RADIO will not ramp up, as PPI_EGU_RAMP_UP channel is self-disabling, and
     * it was disabled when receive ramp-up was started.
     */
    wait_until_radio_is_disabled(); // This includes waiting since CRCOK/CRCERROR (several cycles) event until END
                                    // and then during RXDISABLE state (0.5us)

    nrf_802154_trx_ppi_for_ramp_up_propagation_delay_wait();

    /* Now it is guaranteed, that:
     * - FEM operation to disable LNA mode is triggered through FEM's PPIs
     * - TIMER is started again allowing operation of nrf_802154_trx_transmit_ack
     */

    rxframe_finish_disable_ppis();
    rxframe_finish_disable_fem_activation();
    rxframe_finish_psdu_is_not_being_received();
    rxframe_finish_disable_ints();
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);
    m_flags.missing_receive_buffer = false;

    /* Current state of peripherals:
     * RADIO is DISABLED
     * FEM is powered but LNA mode has just been turned off
     * TIMER is running, counting from the value stored in m_timer_value_on_radio_end_event
     * All PPIs used by receive operation are disabled, forks are cleared, PPI groups that were used are cleared
     * RADIO.SHORTS are cleared
     */

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_DISABLED:
        case TRX_STATE_IDLE:
        case TRX_STATE_FINISHED:
            /* Nothing to do, intentionally empty */
            break;

        case TRX_STATE_GOING_IDLE:
            go_idle_abort();
            break;

        case TRX_STATE_RXFRAME:
            receive_frame_abort();
            break;

        case TRX_STATE_RXFRAME_FINISHED:
            nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
            m_trx_state = TRX_STATE_FINISHED;
            break;

        case TRX_STATE_RXACK:
            receive_ack_abort();
            break;

        case TRX_STATE_TXFRAME:
            transmit_frame_abort();
            break;

        case TRX_STATE_TXACK:
            transmit_ack_abort();
            break;

        case TRX_STATE_STANDALONE_CCA:
            standalone_cca_abort();
            break;

        case TRX_STATE_CONTINUOUS_CARRIER:
            continuous_carrier_abort();
            break;

        case TRX_STATE_MODULATED_CARRIER:
            modulated_carrier_abort();
            break;

        case TRX_STATE_ENERGY_DETECTION:
            energy_detection_abort();
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

trx_state_t nrf_802154_trx_state_get(void)
{
    return m_trx_state;
}

static void go_idle_from_state_finished(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    m_trx_state = TRX_STATE_GOING_IDLE;

    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_radio_int_enable(NRF_RADIO, NRF_RADIO_INT_DISABLED_MASK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

bool nrf_802154_trx_go_idle(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = false;

    switch (m_trx_state)
    {
        case TRX_STATE_DISABLED:
            assert(false);
            break;

        case TRX_STATE_IDLE:
            /* There will be no callout */
            break;

        case TRX_STATE_GOING_IDLE:
            /* There will be callout */
            result = true;
            break;

        case TRX_STATE_RXFRAME_FINISHED:
            nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
        /* Fallthrough */

        case TRX_STATE_FINISHED:
            go_idle_from_state_finished();
            result = true;
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

static void go_idle_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_DISABLED_MASK);
    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void receive_frame_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    rxframe_finish_disable_ppis();
    rxframe_finish_disable_fem_activation();
    rxframe_finish_psdu_is_not_being_received();
    rxframe_finish_disable_ints();
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    m_flags.missing_receive_buffer = false;
    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxack_finish_disable_ppis(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, false);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxack_finish_disable_ints(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_int_disable(NRF_RADIO,
                          NRF_RADIO_INT_ADDRESS_MASK | NRF_RADIO_INT_CRCERROR_MASK |
                          NRF_RADIO_INT_CRCOK_MASK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxack_finish_disable_fem_activation(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Disable LNA activation
    mpsl_fem_lna_configuration_clear();

    // Clear LNA PPIs
    nrf_802154_trx_ppi_for_fem_clear();

    // Disable short used by LNA activation
    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                             NRF_TIMER_SHORT_COMPARE0_STOP_MASK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxack_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    rxack_finish_disable_ppis();
    rxack_finish_disable_ints();
    rxack_finish_disable_fem_activation();
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);
    m_flags.missing_receive_buffer = false;

    /* Current state of peripherals
     * RADIO is DISABLED
     * FEM is powered but LNA mode has just been turned off
     * TIMER is shutdown
     * PPIs used by receive operation are disabled, forks are cleared, PPI groups used are cleared
     * RADIO.SHORTS are cleared
     */

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void receive_ack_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    rxack_finish_disable_ppis();
    rxack_finish_disable_ints();
    rxack_finish_disable_fem_activation();
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);
    m_flags.missing_receive_buffer = false;

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_standalone_cca(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert((m_trx_state == TRX_STATE_IDLE) || (m_trx_state == TRX_STATE_FINISHED));

    m_trx_state = TRX_STATE_STANDALONE_CCA;

    // Set shorts
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_CCA);

    // Enable IRQs
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE);
    nrf_radio_int_enable(NRF_RADIO, NRF_RADIO_INT_CCABUSY_MASK | NRF_RADIO_INT_CCAIDLE_MASK);

    // Set FEM
    fem_for_lna_set();

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void standalone_cca_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, false);

    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    fem_for_lna_reset();

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_CCABUSY_MASK | NRF_RADIO_INT_CCAIDLE_MASK);

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_CCASTOP);
    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void standalone_cca_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    standalone_cca_finish();

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_continuous_carrier(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert((m_trx_state == TRX_STATE_IDLE) || (m_trx_state == TRX_STATE_FINISHED));

    m_trx_state = TRX_STATE_CONTINUOUS_CARRIER;

    // Set Tx Power
    nrf_radio_txpower_set(NRF_RADIO, nrf_802154_pib_tx_power_get());

    // Set FEM
    fem_for_pa_set();

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_TXEN, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_continuous_carrier_restart(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_trx_state == TRX_STATE_CONTINUOUS_CARRIER);

    // Continuous carrier PPIs are configured without self-disabling
    // Triggering RADIO.TASK_DISABLE causes ramp-down -> RADIO.EVENTS_DISABLED -> EGU.TASK -> EGU.EVENT ->
    // RADIO.TASK_TXEN -> ramp_up -> new continous carrier

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void continuous_carrier_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);

    fem_for_pa_reset();

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_modulated_carrier(const void * p_transmit_buffer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert((m_trx_state == TRX_STATE_IDLE) || (m_trx_state == TRX_STATE_FINISHED));
    assert(p_transmit_buffer != NULL);

    m_trx_state = TRX_STATE_MODULATED_CARRIER;

    // Set Tx Power
    nrf_radio_txpower_set(NRF_RADIO, nrf_802154_pib_tx_power_get());

    // Set Tx buffer
    nrf_radio_packetptr_set(NRF_RADIO, p_transmit_buffer);

    // Set shorts
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_MOD_CARRIER);

    // Set FEM
    fem_for_pa_set();

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_TXEN, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_modulated_carrier_restart(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_trx_state == TRX_STATE_MODULATED_CARRIER);

    // Modulated carrier PPIs are configured without self-disabling
    // Triggering RADIO.TASK_DISABLE causes ramp-down -> RADIO.EVENTS_DISABLED -> EGU.TASK -> EGU.EVENT ->
    // RADIO.TASK_TXEN -> ramp_up -> new modulated carrier

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void modulated_carrier_abort()
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);

    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    fem_for_pa_reset();

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_energy_detection(uint32_t ed_count)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert((m_trx_state == TRX_STATE_FINISHED) || (m_trx_state == TRX_STATE_IDLE));

    m_trx_state = TRX_STATE_ENERGY_DETECTION;

    ed_count--;
    /* Check that vd_count will fit into defined bits of register */
    assert( (ed_count & (~RADIO_EDCNT_EDCNT_Msk)) == 0U);

    nrf_radio_ed_loop_count_set(NRF_RADIO, ed_count);

    // Set shorts
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_ED);

    // Enable IRQs
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_EDEND);
    nrf_radio_int_enable(NRF_RADIO, NRF_RADIO_INT_EDEND_MASK);

    // Set FEM
    fem_for_lna_set();

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void energy_detection_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, false);
    fem_for_lna_reset();

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_EDEND_MASK);
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_EDSTOP);
    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void energy_detection_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    energy_detection_finish();
    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void irq_handler_ready(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_trx_state == TRX_STATE_TXFRAME);

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_READY_MASK);

    nrf_802154_trx_transmit_frame_ccastarted();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_address(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
            nrf_802154_trx_receive_frame_started();
            break;

        case TRX_STATE_RXACK:
            m_flags.rssi_started = true;
            nrf_802154_trx_receive_ack_started();
            break;

#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
        case TRX_STATE_TXFRAME:
            nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_ADDRESS_MASK);
            m_flags.tx_started = true;
            nrf_802154_trx_transmit_frame_started();
            break;
#endif // NRF_802154_TX_STARTED_NOTIFY_ENABLED

#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
        case TRX_STATE_TXACK:
            nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_ADDRESS_MASK);
            nrf_802154_trx_transmit_ack_started();
            break;
#endif

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if !NRF_802154_DISABLE_BCC_MATCHING
static void irq_handler_bcmatch(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint8_t current_bcc;
    uint8_t next_bcc;

    assert(m_trx_state == TRX_STATE_RXFRAME);

    m_flags.psdu_being_received = true;

    // If CRCERROR event is set, it means that events are handled out of order due to software
    // latency. Just skip this handler in this case - frame will be dropped.
    if (nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR))
    {
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
        return;
    }

    current_bcc = nrf_radio_bcc_get(NRF_RADIO) / 8U;

    next_bcc = nrf_802154_trx_receive_frame_bcmatched(current_bcc);

    if (next_bcc > current_bcc)
    {
        /* Note: If we don't make it before given octet is received by RADIO bcmatch will not be triggered.
         * The fact that filtering may be not completed at the call to nrf_802154_trx_receive_received handler
         * should be handled by the handler.
         */
        nrf_radio_bcc_set(NRF_RADIO, next_bcc * 8);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#endif

#if !NRF_802154_DISABLE_BCC_MATCHING || NRF_802154_NOTIFY_CRCERROR
static void irq_handler_crcerror(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
#if NRF_802154_DISABLE_BCC_MATCHING
            /* The hardware restarts receive at the moment.
             * The TIMER is being shutdown and restarted by the hardware see
             * PPI_CRCERROR_CLEAR in nrf_802154_trx_receive_frame
             */
#else
            rxframe_finish();
            /* On crc error TIMER is not needed, no ACK may be sent */
            nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
            m_trx_state = TRX_STATE_FINISHED;
#endif
            nrf_802154_trx_receive_frame_crcerror();
            break;

        case TRX_STATE_RXACK:
            rxack_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_receive_ack_crcerror();
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#endif

static void irq_handler_crcok(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
            m_flags.rssi_started = true;
            rxframe_finish();
            m_trx_state = TRX_STATE_RXFRAME_FINISHED;
            nrf_802154_trx_receive_frame_received();
            break;

        case TRX_STATE_RXACK:
            rxack_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_receive_ack_received();
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void txframe_finish_disable_ppis(bool cca)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(cca ? NRF_RADIO_TASK_RXEN : NRF_RADIO_TASK_TXEN, false);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void txframe_finish_disable_ints(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_PHYEND_MASK |
                          NRF_RADIO_INT_CCAIDLE_MASK |
                          NRF_RADIO_INT_CCABUSY_MASK |
                          NRF_RADIO_INT_ADDRESS_MASK |
                          NRF_RADIO_INT_READY_MASK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void txframe_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    /* Below shown what is happening in the hardware
     *
     * Due to short RADIO.SHORT_PHYEND_DISABLE the RADIO is in TXDISABLE (currently ramping down for 21us) state or
     * has already reached DISABLED state (if we entered into ISR with greater latency)
     *
     * Even if RADIO reaches state DISABLED (event DISABLED was triggered by short), no transmission will be triggered
     * as PPI_EGU_RAMP_UP is self-disabling PPI channel.
     *
     * If FEM is in use the PPI_EGU_TIMER_START might be triggered if radio reached DISABLED state,
     * so the TIMER may start counting from the value on which FEM activation finished. The TIMER's CC registers
     * are set in the past so even if TIMER started no spurious FEM PA activation will occur.
     * We need to disable PPI_EGU_TIMER_START and then shutdown TIMER as it is not used.
     */
    txframe_finish_disable_ppis(m_transmit_with_cca);

    fem_for_tx_reset(m_transmit_with_cca);

    txframe_finish_disable_ints();

    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);
#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
    m_flags.tx_started = false;
#endif
    m_flags.missing_receive_buffer = false;

    /* Current state of peripherals
     * RADIO is either in TXDISABLE or DISABLED
     * FEM is powered but PA mode will be turned off on entry into DISABLED state or is already turned off
     * TIMER is shutdown
     * All PPIs that were used are disabled (forks are cleared if used)
     * RADIO.SHORTS are cleared
     */

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void transmit_frame_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    txframe_finish_disable_ppis(m_transmit_with_cca);
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    fem_for_tx_reset(m_transmit_with_cca);

    txframe_finish_disable_ints();
#if NRF_802154_TX_STARTED_NOTIFY_ENABLED
    m_flags.tx_started = false;
#endif
    m_flags.missing_receive_buffer = false;

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_CCASTOP);
    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void txack_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    /* Below shown what is happening in the hardware
     *
     * Due to short RADIO.SHORT_PHYEND_DISABLE the RADIO is in TXDISABLE (currently ramping down for 21us) state or
     * has already reached DISABLED state (if we entered into ISR with greater latency)
     *
     * Even if RADIO reaches state DISABLED (event DISABLED was triggered by short), no transmission will be triggered
     * as only PPI_TIMER_TX_ACK was enabled.
     *
     * FEM will disable PA mode on RADIO.DISABLED event
     *
     * The TIMER was counting to trigger RADIO ramp up and FEM (if required). The TIMER is configured
     * to trigger STOP task on one of these events (whichever is later). As we finished the TIMER is
     * stopped now, and there is no PPIs starting it automatically by the hardware.
     */
    nrf_802154_trx_ppi_for_ack_tx_clear();

    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    mpsl_fem_pa_configuration_clear();

    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                             NRF_TIMER_SHORT_COMPARE0_STOP_MASK |
                             NRF_TIMER_SHORT_COMPARE1_STOP_MASK);

    // Anomaly 78: use SHUTDOWN instead of STOP and CLEAR.
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_PHYEND_MASK | NRF_RADIO_INT_ADDRESS_MASK);

    /* Current state of peripherals
     * RADIO is either in TXDISABLE or DISABLED
     * FEM is powered but PA mode will be turned off on entry into DISABLED state or is already turned off
     * TIMER is shutdown
     * All PPIs that were used are disabled (forks are cleared if used)
     * RADIO.SHORTS are cleared
     */

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void transmit_ack_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ack_tx_clear();

    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    mpsl_fem_pa_configuration_clear();

    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                             NRF_TIMER_SHORT_COMPARE0_STOP_MASK |
                             NRF_TIMER_SHORT_COMPARE1_STOP_MASK);

    // Anomaly 78: use SHUTDOWN instead of STOP and CLEAR.
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_PHYEND_MASK | NRF_RADIO_INT_ADDRESS_MASK);

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void irq_handler_phyend(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_TXFRAME:
            txframe_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_transmit_frame_transmitted();
            break;

        case TRX_STATE_TXACK:
            txack_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_transmit_ack_transmitted();
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void go_idle_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_DISABLED_MASK);

    fem_power_down_now();

    m_trx_state = TRX_STATE_IDLE;

    nrf_802154_trx_go_idle_finished();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void irq_handler_disabled(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_GOING_IDLE:
            go_idle_finish();
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_ccaidle(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_STANDALONE_CCA:
            standalone_cca_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_standalone_cca_finished(true);
            break;

        case TRX_STATE_TXFRAME:
            nrf_802154_trx_transmit_frame_ccaidle();
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_ccabusy(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_TXFRAME:
            assert(m_transmit_with_cca);
            txframe_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_transmit_frame_ccabusy();
            break;

        case TRX_STATE_STANDALONE_CCA:
            standalone_cca_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_standalone_cca_finished(false);
            break;

        default:
            assert(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_edend(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_trx_state == TRX_STATE_ENERGY_DETECTION);

    uint8_t ed_sample = nrf_radio_ed_sample_get(NRF_RADIO);

    energy_detection_finish();
    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_trx_energy_detection_finished(ed_sample);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if defined(RADIO_INTENSET_SYNC_Msk)
static void irq_handler_sync(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    assert(m_trx_state == TRX_STATE_RXFRAME);

    nrf_802154_trx_receive_frame_prestarted();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#endif

void nrf_802154_radio_irq_handler(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // Prevent interrupting of this handler by requests from higher priority code.
    nrf_802154_critical_section_forcefully_enter();

#if defined(RADIO_INTENSET_SYNC_Msk)
    // Note: For NRF_RADIO_EVENT_SYNC we enable interrupt through EGU.
    // That's why we check here EGU's EGU_SYNC_INTMASK.
    // The RADIO does not have interrupt from SYNC event.
    if (nrf_egu_int_enable_check(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_SYNC))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_SYNC);
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_SYNC_EVENT);

        irq_handler_sync();
    }
#endif

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_READY_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_READY))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);

        irq_handler_ready();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_ADDRESS_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);

        irq_handler_address();
    }

#if !NRF_802154_DISABLE_BCC_MATCHING
    // Check MAC frame header.
    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_BCMATCH_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH);

        irq_handler_bcmatch();
    }

#endif // !NRF_802154_DISABLE_BCC_MATCHING

#if !NRF_802154_DISABLE_BCC_MATCHING || NRF_802154_NOTIFY_CRCERROR
    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_CRCERROR_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);

        irq_handler_crcerror();
    }
#endif // !NRF_802154_DISABLE_BCC_MATCHING || NRF_802154_NOTIFY_CRCERROR

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_CRCOK_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CRCOK))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCOK);

        irq_handler_crcok();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_PHYEND_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_PHYEND))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_PHYEND);

        irq_handler_phyend();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_DISABLED_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_DISABLED))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);

        irq_handler_disabled();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_CCAIDLE_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE);

        irq_handler_ccaidle();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_CCABUSY_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);

        irq_handler_ccabusy();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_EDEND_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_EDEND))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_EDEND);

        irq_handler_edend();
    }

    nrf_802154_critical_section_exit();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
void RADIO_IRQHandler(void)
{
    nrf_802154_radio_irq_handler();
}

#endif // NRF_802154_INTERNAL_RADIO_IRQ_HANDLING

#if defined(RADIO_INTENSET_SYNC_Msk)
void nrf_802154_trx_swi_irq_handler(void)
{
    // If this handler is preempted by MARGIN, RADIO IRQ might be set to pending
    // and executed after MARGIN processing is finished, i.e. after the end of a timeslot.
    // To prevent that from happening, the handler is executed with disabled interrupts.
    nrf_802154_mcu_critical_state_t mcu_crit_state;

    nrf_802154_mcu_critical_enter(mcu_crit_state);

    if (nrf_egu_int_enable_check(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK) &&
        nrf_egu_event_check(NRF_802154_EGU_INSTANCE, EGU_SYNC_EVENT))
    {
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_SYNC_EVENT);

        // We are in SWI_IRQHandler, which priority is usually lower than RADIO_IRQHandler.
        // To avoid problems with critical sections, trigger RADIO_IRQ manually.
        // - If we are not in critical section, RADIO_IRQ will start shortly (calling
        // nrf_802154_radio_irq_handler) preempting current SWI_IRQHandler. From
        // nrf_802154_radio_irq_handler we acquire critical section and
        // process sync event.
        // If we are in critical section, the RADIO_IRQ is disabled on NVIC.
        // Following will make it pending, and processing of RADIO_IRQ will start
        // when critical section is left.

        nrf_802154_irq_set_pending(RADIO_IRQn);
    }

    nrf_802154_mcu_critical_exit(mcu_crit_state);
}

#endif
