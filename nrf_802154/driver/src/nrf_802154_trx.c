/*
 * Copyright (c) 2019, Nordic Semiconductor ASA
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

#include "nrf_802154_assert.h"
#include <string.h>

#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_peripherals.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_rssi.h"
#include "nrf_802154_trx_ppi_api.h"
#include "nrf_802154_utils.h"

#include <nrf_erratas.h>
#include "hal/nrf_egu.h"
#include "hal/nrf_radio.h"
#include "hal/nrf_timer.h"
#if defined(NRF53_SERIES)
#include "hal/nrf_vreqctrl.h"
#endif

#include "nrf_802154_procedures_duration.h"
#include "nrf_802154_critical_section.h"
#include "mpsl_fem_config_common.h"
#include "mpsl_tx_power.h"
#include "platform/nrf_802154_irq.h"
#include "protocol/mpsl_fem_protocol_api.h"

#include "nrf_802154_sl_ant_div.h"

#ifdef NRF_802154_USE_INTERNAL_INCLUDES
#include "nrf_802154_trx_internal.h"
#endif

#define EGU_SYNC_EVENT        NRFX_CONCAT_2(NRF_EGU_EVENT_TRIGGERED, NRF_802154_EGU_SYNC_CHANNEL_NO)
#define EGU_SYNC_TASK         NRFX_CONCAT_2(NRF_EGU_TASK_TRIGGER, NRF_802154_EGU_SYNC_CHANNEL_NO)
#define EGU_SYNC_INTMASK      NRFX_CONCAT_2(NRF_EGU_INT_TRIGGERED, NRF_802154_EGU_SYNC_CHANNEL_NO)

#define RADIO_BASE            ((uintptr_t)NRF_RADIO)

#if defined(DPPI_PRESENT)
#define FICR_BASE             NRF_FICR_NS_BASE
#else
#define PPI_CCAIDLE_FEM       NRF_802154_PPI_RADIO_CCAIDLE_TO_FEM_GPIOTE ///< PPI that connects RADIO CCAIDLE event with GPIOTE tasks used by FEM
#define PPI_CHGRP_ABORT       NRF_802154_PPI_ABORT_GROUP                 ///< PPI group used to disable PPIs when async event aborting radio operation is propagated through the system
#endif

#define SHORT_ADDRESS_BCSTART NRF_RADIO_SHORT_ADDRESS_BCSTART_MASK

/// Value set to SHORTS register when no shorts should be enabled.
#define SHORTS_IDLE           0

/// Value set to SHORTS register for RX operation.
#if !defined(NRF52_SERIES)
#define SHORTS_RX             (NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK | \
                               NRF_RADIO_SHORT_PHYEND_DISABLE_MASK |    \
                               SHORT_ADDRESS_BCSTART)
#else
#define SHORTS_RX             (NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK | \
                               NRF_RADIO_SHORT_END_DISABLE_MASK |       \
                               SHORT_ADDRESS_BCSTART)
#endif

#define SHORTS_RX_FREE_BUFFER (NRF_RADIO_SHORT_RXREADY_START_MASK)

#define SHORTS_TX_ACK         (NRF_RADIO_SHORT_TXREADY_START_MASK | \
                               NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
/* The short CCAIDLE_TXEN is not used */
#define SHORTS_CCAIDLE_TXEN 0U
#else
#define SHORTS_CCAIDLE_TXEN NRF_RADIO_SHORT_CCAIDLE_TXEN_MASK
#endif

#define SHORTS_MULTI_CCA_TX (NRF_RADIO_SHORT_RXREADY_CCASTART_MASK | \
                             SHORTS_CCAIDLE_TXEN |                   \
                             NRF_RADIO_SHORT_TXREADY_START_MASK |    \
                             NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#define SHORTS_CCA_TX       (NRF_RADIO_SHORT_RXREADY_CCASTART_MASK | \
                             NRF_RADIO_SHORT_CCABUSY_DISABLE_MASK |  \
                             SHORTS_CCAIDLE_TXEN |                   \
                             NRF_RADIO_SHORT_TXREADY_START_MASK |    \
                             NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#define SHORTS_TX           (NRF_RADIO_SHORT_TXREADY_START_MASK | \
                             NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)

#if !defined(NRF52_SERIES)
#define SHORTS_RX_ACK       (NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK | \
                             NRF_RADIO_SHORT_PHYEND_DISABLE_MASK)
#else
#define SHORTS_RX_ACK       (NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK | \
                             NRF_RADIO_SHORT_END_DISABLE_MASK)

#endif

#define SHORTS_MOD_CARRIER  (NRF_RADIO_SHORT_TXREADY_START_MASK | \
                             NRF_RADIO_SHORT_PHYEND_START_MASK)

#define SHORTS_ED           (NRF_RADIO_SHORT_READY_EDSTART_MASK)

#define SHORTS_CCA          (NRF_RADIO_SHORT_RXREADY_CCASTART_MASK | \
                             NRF_RADIO_SHORT_CCABUSY_DISABLE_MASK)

#define CRC_LENGTH          2                                    ///< Length of CRC in 802.15.4 frames [bytes]
#define CRC_POLYNOMIAL      0x011021                             ///< Polynomial used for CRC calculation in 802.15.4 frames

#if !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
#define MAX_RAMPDOWN_CYCLES (50 * (SystemCoreClock / 1000000UL)) ///< Maximum number of busy wait loop cycles that radio ramp-down is allowed to take
#else
#define MAX_RAMPDOWN_CYCLES 10
#endif

#if NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
void nrf_802154_radio_irq_handler(void); ///< Prototype required by internal RADIO IRQ handler
#endif  // NRF_802154_INTERNAL_RADIO_IRQ_HANDLING

#ifndef NRF_802154_TRX_ENABLE_INTERNAL
#define NRF_802154_TRX_ENABLE_INTERNAL() \
    do                                   \
    {                                    \
    }                                    \
    while (0)
#endif

#ifndef NRF_802154_TRX_RADIO_RESET_INTERNAL
#define NRF_802154_TRX_RADIO_RESET_INTERNAL() \
    do                                        \
    {                                         \
    }                                         \
    while (0)
#endif

#ifndef NRF_802154_TRX_TEST_MODE_ALLOW_LATE_TX_ACK
#define NRF_802154_TRX_TEST_MODE_ALLOW_LATE_TX_ACK 0
#endif

#if !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
/// System Clock Frequency (Core Clock) provided by nrfx.
extern uint32_t SystemCoreClock;

#endif

/// Common parameters for the FEM handling.
static const mpsl_fem_event_t m_activate_rx_cc0 =
{
    .type        = MPSL_FEM_EVENT_TYPE_TIMER,
    .event.timer =
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
    .type        = MPSL_FEM_EVENT_TYPE_TIMER,
    .event.timer =
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
    .type = MPSL_FEM_EVENT_TYPE_GENERIC,
#if defined(DPPI_PRESENT)
    .event.generic.event = NRF_802154_DPPI_RADIO_CCAIDLE
#else
    .override_ppi        = true,
    .ppi_ch_id           = PPI_CCAIDLE_FEM,
    .event.generic.event = ((uint32_t)RADIO_BASE + (uint32_t)NRF_RADIO_EVENT_CCAIDLE)
#endif
};

/**@brief Fal event used by @ref nrf_802154_trx_transmit_ack and @ref txack_finish */
static mpsl_fem_event_t m_activate_tx_cc0_timeshifted;

static volatile trx_state_t m_trx_state;

typedef struct
{
    bool          psdu_being_received;    ///< If PSDU is currently being received.
    bool          missing_receive_buffer; ///!< If trx entered receive state without receive buffer
    bool          rxstarted_notif_en;
    bool          ccastarted_notif_en;
    bool          tx_started;             ///< If the requested transmission has started.
    bool          rssi_started;
    volatile bool rssi_settled;
} nrf_802154_flags_t;

static nrf_802154_flags_t m_flags; ///< Flags used to store the current driver state.

/** @brief Value of TIMER internal counter from which the counting is resumed on RADIO.EVENTS_END event. */
static volatile uint32_t m_timer_value_on_radio_end_event;
static volatile bool     m_transmit_with_cca;
static volatile uint8_t  m_remaining_cca_attempts;

static void timer_frequency_set_1mhz(void);

static void rxframe_finish_disable_ppis(void);
static void rxack_finish_disable_ppis(void);
static void txframe_finish_disable_ppis(bool cca);

static void go_idle_abort(void);
static void receive_frame_abort(void);
static void receive_ack_abort(void);
static void transmit_frame_abort(void);
static void transmit_ack_abort(void);
static void standalone_cca_abort(void);

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
static void continuous_carrier_abort(void);
static void modulated_carrier_abort(void);

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
static void energy_detection_abort(void);

/** Clear flags describing frame being received. */
static void rx_flags_clear(void)
{
    m_flags.missing_receive_buffer = false;
    m_flags.psdu_being_received    = false;
}

static void * volatile mp_receive_buffer;

/** Force the TIMER to be stopped and count from 0. */
static inline void timer_stop_and_clear(void)
{
#ifdef NRF52_SERIES
    // Anomaly 78: use SHUTDOWN instead of STOP.
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_SHUTDOWN);
#else
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_STOP);
    nrf_timer_task_trigger(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_CLEAR);
#endif
}

static void txpower_set(int8_t txpower)
{
#ifdef NRF53_SERIES
    bool radio_high_voltage_enable = false;

    if (txpower > 0)
    {
        /* To get higher than 0dBm raise operating voltage of the radio, giving 3dBm power boost */
        radio_high_voltage_enable = true;
    }
    nrf_vreqctrl_radio_high_voltage_set(NRF_VREQCTRL_NS, radio_high_voltage_enable);
#endif
    uint32_t reg = mpsl_tx_power_dbm_to_radio_register_convert(txpower);

    nrf_radio_txpower_set(NRF_RADIO, reg);
}

/** Initialize TIMER peripheral used by the driver. */
static void nrf_timer_init(void)
{
    timer_stop_and_clear();
    nrf_timer_mode_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_MODE_TIMER);
    nrf_timer_bit_width_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_BIT_WIDTH_32);
    timer_frequency_set_1mhz();
}

#if defined(NRF53_SERIES)
/** Implement the YOPAN-158 workaround. */
static void yopan_158_workaround(void)
{
#define RADIO_ADDRESS_MASK        0xFFFFF000UL
#define FICR_TRIM_REGISTERS_COUNT 32UL
    /* This is a workaround for an issue reported in YOPAN-158.
     *
     * After RADIO peripheral reset with RADIO.POWER register the trim-values, loaded from FICR at
     * network core boot time by MDK, are lost. The trim-values are not preserved and re-applied by
     * hardware.
     *
     * Only selected trim-values are restored, those that apply to RADIO peripheral. The check
     * is done based on destination address.
     */

    /* Copy all the trimming values from FICR into the target addresses. Trim until one ADDR
       is not initialized. */
    for (uint32_t index = 0; index < FICR_TRIM_REGISTERS_COUNT; index++)
    {
        if (((volatile uint32_t *)((volatile uintptr_t)NRF_FICR_NS->TRIMCNF[index].ADDR &
                                   (uintptr_t)RADIO_ADDRESS_MASK) == (uint32_t *)NRF_RADIO))
        {
            *((volatile uint32_t *)NRF_FICR_NS->TRIMCNF[index].ADDR) =
                NRF_FICR_NS->TRIMCNF[index].DATA;
        }
    }
}

#endif /* NRF53_SERIES */

/** Sets the frequency of 1 MHz for NRF_802154_TIMER_INSTANCE. */
static void timer_frequency_set_1mhz(void)
{
    uint32_t base_frequency = NRF_TIMER_BASE_FREQUENCY_GET(NRF_802154_TIMER_INSTANCE);
    uint32_t prescaler      = NRF_TIMER_PRESCALER_CALCULATE(base_frequency, 1000000);

    nrf_timer_prescaler_set(NRF_802154_TIMER_INSTANCE, prescaler);
}

static inline void wait_until_radio_is_disabled(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    bool radio_is_disabled = false;

    /* RADIO should enter DISABLED state after no longer than RX ramp-down time or TX ramp-down
     * time, depending on its initial state before TASK_DISABLE was triggered. The loop below busy
     * waits for the state transition to complete. To prevent the CPU from spinning in an endless
     * loop, the maximum allowed number of loop cycles is limited. The limit's intention is not to
     * approximate the expected maximum time the transition might actually take, which is generally
     * very short, but to act as a safeguard against obviously incorrect and unexpected behaviors.
     * In practice, in most cases the radio will have already changed state to DISABLED before this
     * function starts. In the remaining cases several cycles of the loop should be sufficient for
     * the transition to complete.
     */
    for (uint32_t i = 0; i < MAX_RAMPDOWN_CYCLES; i++)
    {
        if (nrf_radio_state_get(NRF_RADIO) == NRF_RADIO_STATE_DISABLED)
        {
            radio_is_disabled = true;
            break;
        }
#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
        nrf_802154_delay_us(1);
        /* In this simulated board, and in general in the POSIX ARCH,
         * code takes 0 simulated time to execute.
         * Let's hold for 1 microsecond to allow the RADIO HW to clear the state
         */
#endif
    }

    NRF_802154_ASSERT(radio_is_disabled);
    (void)radio_is_disabled;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#if !defined(RADIO_POWER_POWER_Msk)
static inline void radio_reset_without_power_reg(void)
{
    /* SUBSCRIBE registers */
    NRF_RADIO->SUBSCRIBE_TXEN      = 0;
    NRF_RADIO->SUBSCRIBE_RXEN      = 0;
    NRF_RADIO->SUBSCRIBE_START     = 0;
    NRF_RADIO->SUBSCRIBE_STOP      = 0;
    NRF_RADIO->SUBSCRIBE_DISABLE   = 0;
    NRF_RADIO->SUBSCRIBE_RSSISTART = 0;
    NRF_RADIO->SUBSCRIBE_BCSTART   = 0;
    NRF_RADIO->SUBSCRIBE_BCSTOP    = 0;
    NRF_RADIO->SUBSCRIBE_EDSTART   = 0;
    NRF_RADIO->SUBSCRIBE_EDSTOP    = 0;
    NRF_RADIO->SUBSCRIBE_CCASTART  = 0;
    NRF_RADIO->SUBSCRIBE_CCASTOP   = 0;

    /* EVENT registers */
    NRF_RADIO->EVENTS_READY      = 0;
    NRF_RADIO->EVENTS_ADDRESS    = 0;
    NRF_RADIO->EVENTS_PAYLOAD    = 0;
    NRF_RADIO->EVENTS_END        = 0;
    NRF_RADIO->EVENTS_DISABLED   = 0;
    NRF_RADIO->EVENTS_DEVMATCH   = 0;
    NRF_RADIO->EVENTS_DEVMISS    = 0;
    NRF_RADIO->EVENTS_BCMATCH    = 0;
    NRF_RADIO->EVENTS_CRCOK      = 0;
    NRF_RADIO->EVENTS_CRCERROR   = 0;
    NRF_RADIO->EVENTS_FRAMESTART = 0;
    NRF_RADIO->EVENTS_EDEND      = 0;
    NRF_RADIO->EVENTS_EDSTOPPED  = 0;
    NRF_RADIO->EVENTS_CCAIDLE    = 0;
    NRF_RADIO->EVENTS_CCABUSY    = 0;
    NRF_RADIO->EVENTS_CCASTOPPED = 0;
    NRF_RADIO->EVENTS_RATEBOOST  = 0;
    NRF_RADIO->EVENTS_TXREADY    = 0;
    NRF_RADIO->EVENTS_RXREADY    = 0;
    NRF_RADIO->EVENTS_MHRMATCH   = 0;
    NRF_RADIO->EVENTS_PHYEND     = 0;
    NRF_RADIO->EVENTS_CTEPRESENT = 0;

    /* PUBLISH registers */
    NRF_RADIO->PUBLISH_READY      = 0;
    NRF_RADIO->PUBLISH_ADDRESS    = 0;
    NRF_RADIO->PUBLISH_PAYLOAD    = 0;
    NRF_RADIO->PUBLISH_END        = 0;
    NRF_RADIO->PUBLISH_DISABLED   = 0;
    NRF_RADIO->PUBLISH_DEVMATCH   = 0;
    NRF_RADIO->PUBLISH_DEVMISS    = 0;
    NRF_RADIO->PUBLISH_BCMATCH    = 0;
    NRF_RADIO->PUBLISH_CRCOK      = 0;
    NRF_RADIO->PUBLISH_CRCERROR   = 0;
    NRF_RADIO->PUBLISH_FRAMESTART = 0;
    NRF_RADIO->PUBLISH_EDEND      = 0;
    NRF_RADIO->PUBLISH_EDSTOPPED  = 0;
    NRF_RADIO->PUBLISH_CCAIDLE    = 0;
    NRF_RADIO->PUBLISH_CCABUSY    = 0;
    NRF_RADIO->PUBLISH_CCASTOPPED = 0;
    NRF_RADIO->PUBLISH_RATEBOOST  = 0;
    NRF_RADIO->PUBLISH_TXREADY    = 0;
    NRF_RADIO->PUBLISH_RXREADY    = 0;
    NRF_RADIO->PUBLISH_MHRMATCH   = 0;
    NRF_RADIO->PUBLISH_PHYEND     = 0;
    NRF_RADIO->PUBLISH_CTEPRESENT = 0;

    /* INTEN registers */
    NRF_RADIO->INTENSET00 = 0;
    NRF_RADIO->INTENCLR00 = 0xffffffff;

#if !defined(NRF54H20_ENGA_XXAA)
    NRF_RADIO->TASKS_SOFTRESET = 1;
#else /* defined(NRF54H20_ENGA_XXAA) */
    NRF_RADIO->TASKS_TXEN       = 0;
    NRF_RADIO->TASKS_RXEN       = 0;
    NRF_RADIO->TASKS_START      = 0;
    NRF_RADIO->TASKS_STOP       = 0;
    NRF_RADIO->TASKS_DISABLE    = 0;
    NRF_RADIO->TASKS_RSSISTART  = 0;
    NRF_RADIO->TASKS_BCSTART    = 0;
    NRF_RADIO->TASKS_BCSTOP     = 0;
    NRF_RADIO->TASKS_EDSTART    = 0;
    NRF_RADIO->TASKS_EDSTOP     = 0;
    NRF_RADIO->TASKS_CCASTART   = 0;
    NRF_RADIO->TASKS_CCASTOP    = 0;
    NRF_RADIO->SHORTS           = 0;
    NRF_RADIO->PACKETPTR        = 0;
    NRF_RADIO->FREQUENCY        = 0;
    NRF_RADIO->TXPOWER          = 0;
    NRF_RADIO->MODE             = 0;
    NRF_RADIO->PCNF0            = 0;
    NRF_RADIO->PCNF1            = 0;
    NRF_RADIO->BASE0            = 0;
    NRF_RADIO->BASE1            = 0;
    NRF_RADIO->PREFIX0          = 0;
    NRF_RADIO->PREFIX1          = 0;
    NRF_RADIO->TXADDRESS        = 0;
    NRF_RADIO->RXADDRESSES      = 0;
    NRF_RADIO->CRCCNF           = 0;
    NRF_RADIO->CRCPOLY          = 0;
    NRF_RADIO->CRCINIT          = 0;
    NRF_RADIO->TIFS             = 0;
    NRF_RADIO->DATAWHITEIV      = 0;
    NRF_RADIO->BCC              = 0;
    NRF_RADIO->DACNF            = 0;
    NRF_RADIO->MHRMATCHCONF     = 0;
    NRF_RADIO->MHRMATCHMASK     = 0;
    NRF_RADIO->SFD              = 0xA7;
    NRF_RADIO->EDCTRL           = RADIO_EDCTRL_ResetValue;
    NRF_RADIO->CCACTRL          = 0x52D0000;
    NRF_RADIO->DFEMODE          = 0;
    NRF_RADIO->CTEINLINECONF    = 0x2800;
    NRF_RADIO->DFECTRL1         = 0x23282;
    NRF_RADIO->DFECTRL2         = 0;
    NRF_RADIO->SWITCHPATTERN    = 0;
    NRF_RADIO->CLEARPATTERN     = 0;
    NRF_RADIO->DFEPACKET.PTR    = 0;
    NRF_RADIO->DFEPACKET.MAXCNT = 0x1000;

    for (uint8_t i = 0; i < 8; i++)
    {
        NRF_RADIO->DAB[i]          = 0;
        NRF_RADIO->DAP[i]          = 0;
        NRF_RADIO->PSEL.DFEGPIO[i] = 0xFFFFFFFF;
    }
#endif /* !defined(NRF54H20_ENGA_XXAA) */
}

#endif /* !defined(RADIO_POWER_POWER_Msk) */

/** Reset radio peripheral. */
static void nrf_radio_reset(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

#if defined(RADIO_POWER_POWER_Msk)
    nrf_radio_power_set(NRF_RADIO, false);
    nrf_radio_power_set(NRF_RADIO, true);
#else
    radio_reset_without_power_reg();
#endif

    NRF_802154_TRX_RADIO_RESET_INTERNAL();

#if defined(NRF53_SERIES)
    yopan_158_workaround();
#endif /* NRF53_SERIES */

    nrf_802154_log_global_event(NRF_802154_LOG_VERBOSITY_LOW,
                                NRF_802154_LOG_GLOBAL_EVENT_ID_RADIO_RESET,
                                0U);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

/** Robustly disable the radio peripheral. */
static void radio_robust_disable(void)
{
    nrf_radio_state_t radio_state = nrf_radio_state_get(NRF_RADIO);

    if ((radio_state == NRF_RADIO_STATE_RXDISABLE) || (radio_state == NRF_RADIO_STATE_TXDISABLE))
    {
        /* RADIO is in an unstable state that should resolve to DISABLED. Do nothing. */
    }
    else
    {
#if !defined(RADIO_POWER_POWER_Msk)
        /* Disable shorts to ensure no event will be triggered after disabling. */
        nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);
#endif
        /* RADIO is in a stable state and needs to be transitioned to DISABLED manually.
         * It cannot be disabled if EVENT_DISABLED is set. Clear it first. */
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
        nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);
    }
}

static void channel_set(uint8_t channel)
{
    NRF_802154_ASSERT(channel >= 11U && channel <= 26U);

    nrf_radio_frequency_set(NRF_RADIO, 2405U + 5U * (channel - 11U));
}

static void cca_configuration_update(void)
{
    nrf_802154_cca_cfg_t cca_cfg;
    uint8_t              threshold_hw;
    int8_t               lna_gain_db = 0;

    nrf_802154_pib_cca_cfg_get(&cca_cfg);
    mpsl_fem_lna_is_configured(&lna_gain_db);

    threshold_hw = nrf_802154_rssi_dbm_to_hw(cca_cfg.ed_threshold + lna_gain_db);

    nrf_radio_cca_configure(NRF_RADIO,
                            cca_cfg.mode,
                            nrf_802154_rssi_cca_ed_threshold_corrected_get(threshold_hw),
                            cca_cfg.corr_threshold,
                            cca_cfg.corr_limit);
}

/** Initialize interrupts for radio peripheral. */
static void irq_init(void)
{
#if NRF_802154_INTERNAL_RADIO_IRQ_HANDLING
    nrf_802154_irq_init(nrfx_get_irq_number(NRF_RADIO),
                        NRF_802154_IRQ_PRIORITY,
                        nrf_802154_radio_irq_handler);
#endif
    nrf_802154_irq_enable(nrfx_get_irq_number(NRF_RADIO));
}

static void trigger_disable_to_start_rampup(void)
{
    if (!nrf_802154_trx_ppi_for_ramp_up_was_triggered())
    {
        nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
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
    timer_stop_and_clear();
    nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE, NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
    nrf_802154_trx_ppi_for_fem_clear();
    /* There is no need to explicitly deactivate LNA pin during reset as mpsl_fem_abort_set is used
     * to provide a deactivation mechanism on DISABLED event.
     */
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

/** Configure FEM to set PA at appropriate time.
 *
 * @note This function must be called before ramp up PPIs are configured.
 */
static void fem_for_pa_set(mpsl_fem_pa_power_control_t pa_power_control)
{
    (void)mpsl_fem_pa_power_control_set(pa_power_control);
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
    timer_stop_and_clear();
    nrf_802154_trx_ppi_for_fem_clear();
    mpsl_fem_deactivate_now(MPSL_FEM_PA);
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

/** Configure FEM for TX procedure.
 *
 * @note This function must be called before ramp up PPIs are configured.
 */
static void fem_for_tx_set(bool cca, mpsl_fem_pa_power_control_t pa_power_control)
{
    bool success;

    (void)mpsl_fem_pa_power_control_set(pa_power_control);

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
    else
    {
        nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE,
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK);
        nrf_timer_cc_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL0, 2);
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
    timer_stop_and_clear();
}

#if defined(NRF52840_XXAA) || \
    defined(NRF52833_XXAA)
/** @brief Applies DEVICE-CONFIG-254.
 *
 * Shall be called after every RADIO peripheral reset.
 */
static void device_config_254_apply_tx(void)
{
    uint32_t ficr_reg1 = *(volatile uint32_t *)0x10000330UL;
    uint32_t ficr_reg2 = *(volatile uint32_t *)0x10000334UL;
    uint32_t ficr_reg3 = *(volatile uint32_t *)0x10000338UL;

    /* Check if the device is fixed by testing every FICR register's value separately. */
    if (ficr_reg1 != 0xffffffffUL)
    {
        volatile uint32_t * p_radio_reg1 = (volatile uint32_t *)0x4000174cUL;

        *p_radio_reg1 = ficr_reg1;
    }

    if (ficr_reg2 != 0xffffffffUL)
    {
        volatile uint32_t * p_radio_reg2 = (volatile uint32_t *)0x40001584UL;

        *p_radio_reg2 = ficr_reg2;
    }

    if (ficr_reg3 != 0xffffffffUL)
    {
        volatile uint32_t * p_radio_reg3 = (volatile uint32_t *)0x40001588UL;

        *p_radio_reg3 = ficr_reg3;
    }
}

#endif

#if defined(NRF5340_XXAA) && !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)

/** @brief Applies ERRATA-117
 *
 * Shall be called after setting RADIO mode to NRF_RADIO_MODE_IEEE802154_250KBIT.
 */
static void errata_117_apply(void)
{
    /* Register at 0x01FF0084. */
    uint32_t ficr_reg = *(volatile uint32_t *)(FICR_BASE + 0x84UL);
    /* Register at 0x41008588. */
    volatile uint32_t * p_radio_reg = (volatile uint32_t *)(RADIO_BASE + 0x588UL);

    *p_radio_reg = ficr_reg;
}

#endif /* defined(NRF5340_XXAA) && !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)*/

/** @brief Applies modulation fix when PA is used.
 *
 * Shall be called after setting RADIO mode to NRF_RADIO_MODE_IEEE802154_250KBIT.
 *
 * @param[in]  enable   @c true if the appropriate register should be modified
 *                      @c false if it should return to its previous, latched value
 */
static void pa_modulation_fix_apply(bool enable)
{
#if !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
#if (defined(NRF5340_XXAA) || NRF54L_CONFIGURATION_56_ENABLE)
    static uint32_t     m_pa_mod_filter_latched    = 0;
    static bool         m_pa_mod_filter_is_latched = false;
    volatile uint32_t * p_radio_reg;

#if defined(NRF5340_XXAA)
    p_radio_reg = (volatile uint32_t *)(RADIO_BASE + 0x584UL);
#elif NRF54L_CONFIGURATION_56_ENABLE
    p_radio_reg = (volatile uint32_t *)(RADIO_BASE + 0x8C4UL);
#else
    #error Unknown SoC
#endif

    if (enable)
    {
        mpsl_fem_caps_t fem_caps = {};

        mpsl_fem_caps_get(&fem_caps);

        if ((fem_caps.flags & MPSL_FEM_CAPS_FLAG_PA_SETUP_REQUIRED) != 0)
        {
#if defined(NRF5340_XXAA)
            m_pa_mod_filter_latched    = *(p_radio_reg);
            m_pa_mod_filter_is_latched = true;
            *(p_radio_reg)             = 0x40081B08;
#elif NRF54L_CONFIGURATION_56_ENABLE
            // MLTPAN-56
            m_pa_mod_filter_latched    = *(p_radio_reg);
            m_pa_mod_filter_is_latched = true;
            *(p_radio_reg)             = 0x01280001ul;
#endif
        }
    }
    else if (m_pa_mod_filter_is_latched)
    {
        *(p_radio_reg)             = m_pa_mod_filter_latched;
        m_pa_mod_filter_is_latched = false;
    }
#endif /* (defined(NRF5340_XXAA) || NRF54L_CONFIGURATION_56_ENABLE) */
#else /* !defined(CONFIG_SOC_SERIES_BSIM_NRFXX) */
    (void)enable;
#endif /* !defined(CONFIG_SOC_SERIES_BSIM_NRFXX) */
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

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if defined(NRF54H_SERIES)
static void radio_trims_apply(void)
{
#if defined(NRF54H20_XXAA) && !defined(TEST)
    /* HMPAN-103 */
    if ((*(volatile uint32_t *)0x5302C8A0 == 0x80000000) ||
        (*(volatile uint32_t *)0x5302C8A0 == 0x0058120E))
    {
        *(volatile uint32_t *)0x5302C8A0 = 0x0058090E;
    }

    *(volatile uint32_t *)0x5302C8A4 = 0x00F8AA5F;
    *(volatile uint32_t *)0x5302C7AC = 0x8672827A;
    *(volatile uint32_t *)0x5302C7B0 = 0x7E768672;
    *(volatile uint32_t *)0x5302C7B4 = 0x0406007E;
#endif

    nrf_radio_fast_ramp_up_enable_set(NRF_RADIO, true);
}

#endif /* defined(NRF54H_SERIES) */

void nrf_802154_trx_enable(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_DISABLED);

    nrf_timer_init();
    nrf_radio_reset();

#if defined(NRF52840_XXAA) || \
    defined(NRF52833_XXAA)
    // Apply DEVICE-CONFIG-254 if needed.
    if (mpsl_fem_device_config_254_apply_get())
    {
        device_config_254_apply_tx();
    }
#endif

    nrf_radio_packet_conf_t packet_conf;

    nrf_radio_mode_set(NRF_RADIO, NRF_RADIO_MODE_IEEE802154_250KBIT);

#if defined(NRF54L_SERIES) && !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    // Apply MLTPAN-6
    *(volatile uint32_t *)(RADIO_BASE + 0x810UL) = 2;
#endif

#if defined(NRF5340_XXAA) && !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    // Apply ERRATA-117 after setting RADIO mode to NRF_RADIO_MODE_IEEE802154_250KBIT.
    errata_117_apply();
#endif

    pa_modulation_fix_apply(true);

    memset(&packet_conf, 0, sizeof(packet_conf));
    packet_conf.lflen  = 8;
    packet_conf.plen   = NRF_RADIO_PREAMBLE_LENGTH_32BIT_ZERO;
    packet_conf.crcinc = true;
    packet_conf.maxlen = MAX_PACKET_SIZE;
    nrf_radio_packet_configure(NRF_RADIO, &packet_conf);

#if defined(NRF54H_SERIES)
    radio_trims_apply();
#endif

    NRF_802154_TRX_ENABLE_INTERNAL();

#if defined(RADIO_MODECNF0_RU_Msk)
    nrf_radio_modecnf0_set(NRF_RADIO, true, 0);
#endif

    // Configure CRC
    nrf_radio_crc_configure(NRF_RADIO, CRC_LENGTH, NRF_RADIO_CRC_ADDR_IEEE802154, CRC_POLYNOMIAL);

    nrf_802154_trx_ppi_for_enable();

    // Configure CCA
    cca_configuration_update();

    // Set channel
    channel_set(nrf_802154_pib_channel_get());

    // Custom initialization operations
    nrf_802154_custom_part_of_radio_init();

    irq_init();

    NRF_802154_ASSERT(nrf_radio_shorts_get(NRF_RADIO) == SHORTS_IDLE);

    mpsl_fem_enable();
#if defined(DPPI_PRESENT)
    mpsl_fem_abort_set(NRF_802154_DPPI_RADIO_DISABLED,
                       0U); /* The group parameter is ignored by FEM for SoCs with DPPIs */
#else
    mpsl_fem_abort_set(nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_DISABLED),
                       PPI_CHGRP_ABORT);
#endif

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

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case TRX_STATE_CONTINUOUS_CARRIER:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        case TRX_STATE_MODULATED_CARRIER:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

        case TRX_STATE_ENERGY_DETECTION:
            nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_RXEN, false);
            nrf_802154_trx_ppi_for_fem_clear();
            break;

        default:
            NRF_802154_ASSERT(false);
    }
    nrf_802154_trx_ppi_for_disable();
}

static void fem_power_down_now(void)
{
    mpsl_fem_deactivate_now(MPSL_FEM_ALL);
    mpsl_fem_disable();
}

void nrf_802154_trx_disable(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    if (m_trx_state != TRX_STATE_DISABLED)
    {
        pa_modulation_fix_apply(false);

#if defined(RADIO_POWER_POWER_Msk)
        nrf_radio_power_set(NRF_RADIO, false);
#endif

        /* While the RADIO is powered off deconfigure any PPIs used directly by trx module */
        ppi_all_clear();

#if !defined(RADIO_POWER_POWER_Msk)
        radio_robust_disable();
        wait_until_radio_is_disabled();
        nrf_radio_reset();
#endif

        nrf_802154_irq_clear_pending(nrfx_get_irq_number(NRF_RADIO));

#if defined(RADIO_INTENSET_SYNC_Msk)
        nrf_egu_int_disable(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK);
        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_SYNC_EVENT);
#endif

        /* Stop & deconfigure timer */
        nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                                 NRF_TIMER_SHORT_COMPARE0_STOP_MASK |
                                 NRF_TIMER_SHORT_COMPARE1_STOP_MASK);
        timer_stop_and_clear();

#if defined(RADIO_POWER_POWER_Msk)
        nrf_radio_power_set(NRF_RADIO, true);
#endif

        mpsl_fem_lna_configuration_clear();
        mpsl_fem_pa_configuration_clear();
        mpsl_fem_abort_clear();

        if (m_trx_state != TRX_STATE_IDLE)
        {
            fem_power_down_now();
        }

        m_flags.psdu_being_received    = false;
        m_flags.missing_receive_buffer = false;
        m_flags.rssi_started           = false;
        m_flags.tx_started             = false;

        m_trx_state = TRX_STATE_DISABLED;

        nrf_802154_log_global_event(NRF_802154_LOG_VERBOSITY_LOW,
                                    NRF_802154_LOG_GLOBAL_EVENT_ID_RADIO_RESET,
                                    0U);
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
            NRF_802154_ASSERT(false);
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
            NRF_802154_ASSERT(false);
            break;
    }

    NRF_802154_ASSERT(result);
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
            NRF_802154_ASSERT(false);
            break;
    }

    if (!result)
    {
        NRF_802154_ASSERT(false);
    }
}

void nrf_802154_trx_antenna_update(void)
{
    NRF_802154_ASSERT(m_trx_state != TRX_STATE_DISABLED);

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
#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case TRX_STATE_CONTINUOUS_CARRIER:
        case TRX_STATE_MODULATED_CARRIER:
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED
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
    return m_flags.psdu_being_received;
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
            NRF_802154_ASSERT(!m_flags.missing_receive_buffer);
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
            NRF_802154_ASSERT(false);
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

void nrf_802154_trx_receive_frame(uint8_t                                 bcc,
                                  nrf_802154_trx_ramp_up_trigger_mode_t   rampup_trigg_mode,
                                  nrf_802154_trx_receive_notifications_t  notifications_mask,
                                  const nrf_802154_fal_tx_power_split_t * p_ack_tx_power)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint32_t ints_to_enable = 0U;
    uint32_t shorts         = SHORTS_RX;

    timer_stop_and_clear();

    m_trx_state = TRX_STATE_RXFRAME;

    // Clear filtering flag
    rx_flags_clear();

    m_flags.rxstarted_notif_en = (notifications_mask & TRX_RECEIVE_NOTIFICATION_STARTED) != 0U;

    // Clear the RSSI measurement flag.
    m_flags.rssi_started = false;

    m_flags.rssi_settled = false;

    txpower_set(p_ack_tx_power->radio_tx_power);

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
    NRF_802154_ASSERT(bcc != 0U);
    nrf_radio_bcc_set(NRF_RADIO, bcc * 8U);

    // Enable IRQs
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);
    ints_to_enable |= NRF_RADIO_INT_CRCERROR_MASK;
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH);
    ints_to_enable |= NRF_RADIO_INT_BCMATCH_MASK;
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCOK);
    ints_to_enable |= NRF_RADIO_INT_CRCOK_MASK;
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
    ints_to_enable |= NRF_RADIO_INT_ADDRESS_MASK;

    if (rampup_trigg_mode == TRX_RAMP_UP_HW_TRIGGER)
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);
#if !defined(NRF53_SERIES)
        ints_to_enable |= (NRF_RADIO_INT_READY_MASK | NRF_RADIO_INT_DISABLED_MASK);
#else
        ints_to_enable |= NRF_RADIO_INT_READY_MASK;
#endif
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
#if !defined(RADIO_INTENSET_SYNC_Msk)
        NRF_802154_ASSERT(false);
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
#if !defined(NRF54L_SERIES)
        delta_time = 1;
#else
        // Avoid value 1 due to MLTPAN-22 and MLTPAN-24
        delta_time = 2;
#endif
        nrf_timer_cc_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_CC_CHANNEL0, delta_time);
    }

    // Set FEM PA gain for ACK transmission
    mpsl_fem_pa_power_control_set(p_ack_tx_power->fem_pa_power_control);

    m_timer_value_on_radio_end_event = delta_time;

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Let the TIMER stop on last event required by a FEM
    nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE,
                            NRF_TIMER_SHORT_COMPARE0_STOP_MASK);

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, rampup_trigg_mode, true);

    if (rampup_trigg_mode == TRX_RAMP_UP_SW_TRIGGER)
    {
        trigger_disable_to_start_rampup();
    }

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
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, TRX_RAMP_UP_SW_TRIGGER, false);

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

#if defined(RADIO_EVENTS_RSSIEND_EVENTS_RSSIEND_Msk)
            nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_RSSIEND);
#endif
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
    int8_t  lna_gain_db                     = 0;
    uint8_t rssi_sample_minus_dbm           = nrf_radio_rssi_sample_get(NRF_RADIO);
    uint8_t rssi_sample_corrected_minus_dbm =
        nrf_802154_rssi_sample_corrected_get(rssi_sample_minus_dbm);
    int8_t rssi_sample_corrected_dbm = -((int8_t)rssi_sample_corrected_minus_dbm);

    mpsl_fem_lna_is_configured(&lna_gain_db);

    return rssi_sample_corrected_dbm - lna_gain_db;
}

bool nrf_802154_trx_rssi_sample_is_available(void)
{
#if defined(RADIO_EVENTS_RSSIEND_EVENTS_RSSIEND_Msk)
    return nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_RSSIEND);
#else
    return true;
#endif
}

void nrf_802154_trx_transmit_frame(const void                            * p_transmit_buffer,
                                   nrf_802154_trx_ramp_up_trigger_mode_t   rampup_trigg_mode,
                                   uint8_t                                 cca_attempts,
                                   const nrf_802154_fal_tx_power_split_t * p_tx_power,
                                   nrf_802154_trx_transmit_notifications_t notifications_mask)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint32_t ints_to_enable = 0U;
    bool     cca            = cca_attempts > 0;

    timer_stop_and_clear();

    m_trx_state              = TRX_STATE_TXFRAME;
    m_transmit_with_cca      = cca;
    m_remaining_cca_attempts = cca_attempts;

    m_flags.ccastarted_notif_en = false;

    txpower_set(p_tx_power->radio_tx_power);

    nrf_radio_packetptr_set(NRF_RADIO, p_transmit_buffer);

    // Set shorts
    if (cca_attempts > 1)
    {
        nrf_radio_shorts_set(NRF_RADIO, SHORTS_MULTI_CCA_TX);
        nrf_802154_trx_ppi_for_extra_cca_attempts_set();
    }
    else if (cca_attempts == 1)
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

    if (rampup_trigg_mode == TRX_RAMP_UP_HW_TRIGGER)
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);
#if !defined(NRF53_SERIES)
        ints_to_enable |= (NRF_RADIO_INT_READY_MASK | NRF_RADIO_INT_DISABLED_MASK);
#else
        ints_to_enable |= NRF_RADIO_INT_READY_MASK;
#endif
    }

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
            ints_to_enable             |= NRF_RADIO_INT_READY_MASK;
            m_flags.ccastarted_notif_en = true;
        }
    }

    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
    ints_to_enable    |= NRF_RADIO_INT_ADDRESS_MASK;
    m_flags.tx_started = false;

    nrf_radio_int_enable(NRF_RADIO, ints_to_enable);

    fem_for_tx_set(cca, p_tx_power->fem_pa_power_control);
    nrf_802154_trx_antenna_update();
    nrf_802154_trx_ppi_for_txframe_ramp_up_set(cca, rampup_trigg_mode);

    if (rampup_trigg_mode == TRX_RAMP_UP_SW_TRIGGER)
    {
        trigger_disable_to_start_rampup();
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

bool nrf_802154_trx_transmit_ack(const void * p_transmit_buffer, uint32_t delay_us)
{
    /* Assumptions on peripherals
     * TIMER is running, is counting from value saved in m_timer_value_on_radio_end_event,
     * which trigered on END event, which happened RX_PHYEND_EVENT_LATENCY_US us after frame
     * on air receive was finished.
     * RADIO is DISABLED
     * PPIs are DISABLED
     */

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    bool result = false;

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_RXFRAME_FINISHED);
    NRF_802154_ASSERT(p_transmit_buffer != NULL);

    m_trx_state = TRX_STATE_TXACK;

    // Set TIMER's CC to the moment when ramp-up should occur.
    if (delay_us <= TX_RAMP_UP_TIME + RX_PHYEND_EVENT_LATENCY_US)
    {
        timer_stop_and_clear();
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
        return result;
    }

    uint32_t timer_cc_ramp_up_start = m_timer_value_on_radio_end_event + delay_us -
                                      TX_RAMP_UP_TIME -
                                      RX_PHYEND_EVENT_LATENCY_US;

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
    // Assumption here is that FEM activation takes no more than TX_RAMP_UP_TIME.
    m_activate_tx_cc0_timeshifted = m_activate_tx_cc0;

    // Set the moment for FEM at which real transmission starts.
    m_activate_tx_cc0_timeshifted.event.timer.counter_period.end = timer_cc_ramp_up_start +
                                                                   TX_RAMP_UP_TIME;

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
    nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);

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
#if !defined(NRF53_SERIES)
        uint32_t ints_to_enable = NRF_RADIO_INT_PHYEND_MASK |
                                  NRF_RADIO_INT_ADDRESS_MASK |
                                  NRF_RADIO_INT_DISABLED_MASK;

#else
        uint32_t ints_to_enable = NRF_RADIO_INT_PHYEND_MASK |
                                  NRF_RADIO_INT_ADDRESS_MASK;

#endif

        nrf_radio_int_enable(NRF_RADIO, ints_to_enable);
    }
    else
    {
#if !NRF_802154_TRX_TEST_MODE_ALLOW_LATE_TX_ACK
        /* We were to late with setting up PPI_TIMER_ACK, ack transmission was not triggered and
         * will not be triggered in future.
         */
        nrf_802154_trx_ppi_for_ack_tx_clear();

        /* As the timer was running during operation, it is possible we were able to configure
         * FEM thus it may trigger in future or may started PA activation.
         */
        mpsl_fem_pa_configuration_clear();
        mpsl_fem_deactivate_now(MPSL_FEM_PA);

        timer_stop_and_clear();

        /* No callbacks will be called */
#else // !NRF_802154_TRX_TEST_MODE_ALLOW_LATE_TX_ACK
        uint32_t ints_to_enable = NRF_RADIO_INT_PHYEND_MASK | NRF_RADIO_INT_ADDRESS_MASK;

        nrf_radio_int_enable(NRF_RADIO, ints_to_enable);

        nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_TXEN);

        result = true;
#endif // !NRF_802154_TRX_TEST_MODE_ALLOW_LATE_TX_ACK
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
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

    uint32_t ints_to_disable = NRF_RADIO_INT_READY_MASK |
                               NRF_RADIO_INT_ADDRESS_MASK |
                               NRF_RADIO_INT_CRCOK_MASK |
                               NRF_RADIO_INT_DISABLED_MASK;

    ints_to_disable |= NRF_RADIO_INT_CRCERROR_MASK;
    ints_to_disable |= NRF_RADIO_INT_BCMATCH_MASK;
    nrf_radio_int_disable(NRF_RADIO, ints_to_disable);

#if defined(RADIO_INTENSET_SYNC_Msk)
    nrf_egu_int_disable(NRF_802154_EGU_INSTANCE, EGU_SYNC_INTMASK);
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void rxframe_finish_psdu_is_not_being_received(void)
{
    m_flags.psdu_being_received = false;
}

static void rxframe_finish(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    /* Note that CRCOK/CRCERROR event is generated several cycles before END event.
     *
     * Below shown what is happening in the hardware
     *
     * TIMER is started by path:
     * RADIO.SHORT_PHYEND_DISABLE -> RADIO.TASKS_DISABLE -> RADIO.EVENTS_DISABLED ->
     *    PPI_DISABLED_EGU -> EGU.TASKS_TRIGGER -> EGU.EVENTS_TRIGGERED -> PPI_EGU_TIMER_START -> TIMER.TASKS_START
     *
     * FEM's LNA mode is disabled by path:
     * RADIO.SHORT_PHYEND_DISABLE -> RADIO.TASKS_DISABLE -> RADIO.EVENTS_DISABLED -> (FEM's PPI triggering disable LNA
     *                                                                                operation, see mpsl_fem_abort_set() )
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
            timer_stop_and_clear();
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

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED
        case TRX_STATE_CONTINUOUS_CARRIER:
            continuous_carrier_abort();
            break;

        case TRX_STATE_MODULATED_CARRIER:
            modulated_carrier_abort();
            break;
#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

        case TRX_STATE_ENERGY_DETECTION:
            energy_detection_abort();
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

trx_state_t nrf_802154_trx_state_get(void)
{
    return m_trx_state;
}

uint32_t nrf_802154_trx_ramp_up_ppi_channel_get(void)
{
    return nrf_802154_trx_ppi_for_ramp_up_channel_id_get();
}

static void go_idle_from_state_finished(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    m_trx_state = TRX_STATE_GOING_IDLE;

    radio_robust_disable();

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
            NRF_802154_ASSERT(false);
            break;

        case TRX_STATE_IDLE:
            /* There will be no callout */
            break;

        case TRX_STATE_GOING_IDLE:
            /* There will be callout */
            result = true;
            break;

        case TRX_STATE_RXFRAME_FINISHED:
            timer_stop_and_clear();
        /* Fallthrough */

        case TRX_STATE_FINISHED:
            go_idle_from_state_finished();
            result = true;
            break;

        default:
            NRF_802154_ASSERT(false);
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
    radio_robust_disable();
    timer_stop_and_clear();

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
    timer_stop_and_clear();
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

    radio_robust_disable();
    timer_stop_and_clear();

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_standalone_cca(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT((m_trx_state == TRX_STATE_IDLE) || (m_trx_state == TRX_STATE_FINISHED));

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
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, TRX_RAMP_UP_SW_TRIGGER, false);

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
    radio_robust_disable();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void standalone_cca_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    standalone_cca_finish();

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

void nrf_802154_trx_continuous_carrier(const nrf_802154_fal_tx_power_split_t * p_tx_power)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT((m_trx_state == TRX_STATE_IDLE) || (m_trx_state == TRX_STATE_FINISHED));

    m_trx_state = TRX_STATE_CONTINUOUS_CARRIER;

    // Set Tx Power
    txpower_set(p_tx_power->radio_tx_power);

    // Set FEM
    fem_for_pa_set(p_tx_power->fem_pa_power_control);

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_TXEN, TRX_RAMP_UP_SW_TRIGGER, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_continuous_carrier_restart(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_CONTINUOUS_CARRIER);

    // Continuous carrier PPIs are configured without self-disabling
    // Triggering RADIO.TASK_DISABLE causes ramp-down -> RADIO.EVENTS_DISABLED -> EGU.TASK -> EGU.EVENT ->
    // RADIO.TASK_TXEN -> ramp_up -> new continous carrier
    radio_robust_disable();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void continuous_carrier_abort(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);

    fem_for_pa_reset();

    radio_robust_disable();

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_modulated_carrier(const void                            * p_transmit_buffer,
                                      const nrf_802154_fal_tx_power_split_t * p_tx_power)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT((m_trx_state == TRX_STATE_IDLE) || (m_trx_state == TRX_STATE_FINISHED));
    NRF_802154_ASSERT(p_transmit_buffer != NULL);

    m_trx_state = TRX_STATE_MODULATED_CARRIER;

    // Set Tx Power
    txpower_set(p_tx_power->radio_tx_power);

    // Set Tx buffer
    nrf_radio_packetptr_set(NRF_RADIO, p_transmit_buffer);

    // Set shorts
    nrf_radio_shorts_set(NRF_RADIO, SHORTS_MOD_CARRIER);

    // Set FEM
    fem_for_pa_set(p_tx_power->fem_pa_power_control);

    // Select antenna
    nrf_802154_trx_antenna_update();

    // Set PPIs
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_TXEN, TRX_RAMP_UP_SW_TRIGGER, false);

    trigger_disable_to_start_rampup();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_trx_modulated_carrier_restart(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_MODULATED_CARRIER);

    // Modulated carrier PPIs are configured without self-disabling
    // Triggering RADIO.TASK_DISABLE causes ramp-down -> RADIO.EVENTS_DISABLED -> EGU.TASK -> EGU.EVENT ->
    // RADIO.TASK_TXEN -> ramp_up -> new modulated carrier
    radio_robust_disable();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void modulated_carrier_abort()
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(NRF_RADIO_TASK_TXEN, false);

    nrf_radio_shorts_set(NRF_RADIO, SHORTS_IDLE);

    fem_for_pa_reset();

    radio_robust_disable();

    m_trx_state = TRX_STATE_FINISHED;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

void nrf_802154_trx_energy_detection(uint32_t ed_count)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT((m_trx_state == TRX_STATE_FINISHED) || (m_trx_state == TRX_STATE_IDLE));

    m_trx_state = TRX_STATE_ENERGY_DETECTION;

    ed_count--;
    /* Check that vd_count will fit into defined bits of register */
#if defined(RADIO_EDCNT_EDCNT_Msk)
    NRF_802154_ASSERT( (ed_count & (~RADIO_EDCNT_EDCNT_Msk)) == 0U);
#endif

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
    nrf_802154_trx_ppi_for_ramp_up_set(NRF_RADIO_TASK_RXEN, TRX_RAMP_UP_SW_TRIGGER, false);

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
    radio_robust_disable();

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

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_READY_MASK);

    nrf_802154_trx_ppi_for_ramp_up_reconfigure();

    switch (m_trx_state)
    {
        case TRX_STATE_TXFRAME:
            if (m_flags.ccastarted_notif_en)
            {
                nrf_802154_trx_transmit_frame_ccastarted();
            }
            break;

        case TRX_STATE_RXFRAME:
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_address(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    // At this point we can be certain that no more DISABLED events
    // comming from the time before the transmission/reception started
    // will arrive. We can safely disable the DISABLED interrupt.
    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_DISABLED_MASK);

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
            if (m_flags.rxstarted_notif_en)
            {
                nrf_802154_trx_receive_frame_started();
            }
            break;

        case TRX_STATE_RXACK:
            m_flags.rssi_started = true;
            nrf_802154_trx_receive_ack_started();
            break;

        case TRX_STATE_TXFRAME:
            nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_ADDRESS_MASK);
            m_flags.tx_started = true;
            nrf_802154_trx_transmit_frame_started();
            break;

        case TRX_STATE_TXACK:
            nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_ADDRESS_MASK);
            nrf_802154_trx_transmit_ack_started();
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_bcmatch(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    uint8_t current_bcc;
    uint8_t next_bcc;

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_RXFRAME);

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

static void irq_handler_crcerror(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_RXFRAME:
            rxframe_finish();
            /* On crc error TIMER is not needed, no ACK may be sent */
            timer_stop_and_clear();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_receive_frame_crcerror();
            break;

        case TRX_STATE_RXACK:
            rxack_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_receive_ack_crcerror();
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

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
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void txframe_finish_disable_ppis(bool cca)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_txframe_ramp_up_clear(cca);
    nrf_802154_trx_ppi_for_extra_cca_attempts_clear(); // fine to call unconditionally

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

static void txframe_finish_disable_ints(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_int_disable(NRF_RADIO,
                          NRF_RADIO_INT_PHYEND_MASK |
                          NRF_RADIO_INT_CCAIDLE_MASK |
                          NRF_RADIO_INT_CCABUSY_MASK |
                          NRF_RADIO_INT_ADDRESS_MASK |
                          NRF_RADIO_INT_READY_MASK |
                          NRF_RADIO_INT_DISABLED_MASK);

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

    m_flags.tx_started             = false;
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

    m_flags.tx_started             = false;
    m_flags.missing_receive_buffer = false;

    nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_CCASTOP);
    radio_robust_disable();

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

    timer_stop_and_clear();

    nrf_radio_int_disable(NRF_RADIO,
                          NRF_RADIO_INT_PHYEND_MASK | NRF_RADIO_INT_ADDRESS_MASK |
                          NRF_RADIO_INT_DISABLED_MASK);

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

    timer_stop_and_clear();

    nrf_radio_int_disable(NRF_RADIO, NRF_RADIO_INT_PHYEND_MASK | NRF_RADIO_INT_ADDRESS_MASK);

    radio_robust_disable();

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
            NRF_802154_ASSERT(false);
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

        case TRX_STATE_TXFRAME:
        case TRX_STATE_TXACK:
        case TRX_STATE_RXFRAME:
            // Robust radio ramp-down requires that RADIO.DISABLE is cleared. If the ramp-up was
            // triggered by software, the event was cleared already immediately after triggering
            // RADIO.DISABLE task. If the ramp-up was triggered by (D)PPI, the event would need
            // to be cleared. The IRQ handler does that on entry to irq_handler_disabled.
            // The interrupt must not be disabled here though. It is possible that this handler is executed
            // before the radio ramp-up finishes as a result of a ramp-down triggered earlier. This is more
            // probable on platforms where code execution is fast and the critical section that configures
            // radio ramp-up is exited before that ramp-up finishes. To allow for this interrupt to fire again
            // when the ramp-up finishes and for the DISABLED event to be cleared once more, do not disable
            // the interrupt now. It's disabled in ADDRESS event handler.
            break;

        default:
            NRF_802154_ASSERT(false);
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
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_ccabusy(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    switch (m_trx_state)
    {
        case TRX_STATE_TXFRAME:
            NRF_802154_ASSERT(m_transmit_with_cca);

            if (m_remaining_cca_attempts > 1)
            {
                /* There are still remaining CCA attempts to be performed.
                 * Start of next CCA has already been triggered by (D)PPI.
                 *
                 * Assumptions:
                 * - RADIO is in RXIDLE and will transition to TX on CCAIDLE
                 * - TIMER is stopped
                 * - FEM has LNA ramped up and will ramp-up PA on CCAIDLE
                 */
                m_remaining_cca_attempts--;
                if (m_remaining_cca_attempts == 1)
                {
                    /* The last CCA attempt was just triggered through a (D)PPI that connects
                     * RADIO->EVENTS_CCABUSY to RADIO->TASKS_CCASTART. Clear that (D)PPI so that
                     * there's no next CCA attempt. Also, configure an additional short that will
                     * disable the RADIO in case busy channel is detected so that consistency with
                     * single CCA approach is maintained. The RADIO is expected to be disabled
                     * when CCA concludes with result busy.
                     */
                    nrf_802154_trx_ppi_for_extra_cca_attempts_clear();
                    nrf_radio_shorts_set(
                        NRF_RADIO,
                        SHORTS_MULTI_CCA_TX | NRF_RADIO_SHORT_CCABUSY_DISABLE_MASK);

                    /* If the handler was delayed for the duration of a single CCA procedure, a race
                     * condition between the lines above and the end of CCA might occur. If that CCA
                     * detects busy channel, there are three cases how the CCABUSY might be placed in
                     * time relatively to the lines above:
                     * - before (D)PPI is disconnected and the shorts are reconfigured;
                     * - after (D)PPI is disconnected and before the shorts are reconfigured;
                     * - after (D)PPI id disconnected and the shorts are reconfigured;
                     *
                     * The recovery procedure for the last case is to manually trigger
                     * RADIO->TASKS_CCASTOP and RADIO->TASKS_DISABLE. For the second case it would
                     * be enough to only trigger RADIO->TASKS_DISABLE, but the cases cannot be
                     * distinguished in code and RADIO->TASKS_CCASTOP is harmless in the second case,
                     * so the same recovery procedure fixes both the second and the third case.
                     * For the first case such procedure will cause one fewer CCA attempt than
                     * requested. That's a low price to pay to ensure the RADIO always ends
                     * up in the expected, well-defined state, especially that this race should
                     * happen extremely rarely.
                     */
                    if (nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY))
                    {
                        /* RADIO->EVENTS_CCABUSY occurred during IRQ handler execution. Manually
                         * drive RADIO to state that should have been achieved through hardware and
                         * finish the procedure.
                         */
                        nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_CCASTOP);
                        radio_robust_disable();
                        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);

                        txframe_finish();
                        m_trx_state = TRX_STATE_FINISHED;
                        nrf_802154_trx_transmit_frame_ccabusy();
                    }
                }
            }
            else
            {
                txframe_finish();
                m_trx_state = TRX_STATE_FINISHED;
                nrf_802154_trx_transmit_frame_ccabusy();
            }
            break;

        case TRX_STATE_STANDALONE_CCA:
            standalone_cca_finish();
            m_trx_state = TRX_STATE_FINISHED;
            nrf_802154_trx_standalone_cca_finished(false);
            break;

        default:
            NRF_802154_ASSERT(false);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

static void irq_handler_edend(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_ENERGY_DETECTION);

    int8_t  lna_gain_db   = 0;
    uint8_t ed_sample_hw  = nrf_radio_ed_sample_get(NRF_RADIO);
    int8_t  ed_sample_dbm = nrf_802154_rssi_ed_sample_to_dbm_convert(ed_sample_hw);

    energy_detection_finish();
    m_trx_state = TRX_STATE_FINISHED;

    mpsl_fem_lna_is_configured(&lna_gain_db);

    nrf_802154_trx_energy_detection_finished(ed_sample_dbm - lna_gain_db);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

#if defined(RADIO_INTENSET_SYNC_Msk)
static void irq_handler_sync(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    NRF_802154_ASSERT(m_trx_state == TRX_STATE_RXFRAME);

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

    // Check MAC frame header.
    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_BCMATCH_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_BCMATCH);

        irq_handler_bcmatch();
    }

    if (nrf_radio_int_enable_check(NRF_RADIO, NRF_RADIO_INT_CRCERROR_MASK) &&
        nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR))
    {
        nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_CRCERROR);

        irq_handler_crcerror();
    }

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

        nrf_802154_irq_set_pending(nrfx_get_irq_number(NRF_RADIO));
    }

    nrf_802154_mcu_critical_exit(mcu_crit_state);
}

#endif

const nrf_802154_sl_event_handle_t * nrf_802154_trx_radio_end_event_handle_get(void)
{
    static const nrf_802154_sl_event_handle_t r = {
#if defined(DPPI_PRESENT)
        .event_addr = NRF_802154_DPPI_RADIO_END,
        .shared     = true
#else
        .event_addr = (uint32_t)&NRF_RADIO->EVENTS_END
#endif
    };

    return &r;
}

const nrf_802154_sl_event_handle_t * nrf_802154_trx_radio_ready_event_handle_get(void)
{
    static const nrf_802154_sl_event_handle_t r = {
#if defined(DPPI_PRESENT)
        .event_addr = NRF_802154_DPPI_RADIO_READY,
        .shared     = true
#else
        .event_addr = (uint32_t)&NRF_RADIO->EVENTS_READY
#endif
    };

    return &r;
}

const nrf_802154_sl_event_handle_t * nrf_802154_trx_radio_crcok_event_handle_get(void)
{
    static const nrf_802154_sl_event_handle_t r = {
        .event_addr = (uint32_t)&NRF_RADIO->EVENTS_CRCOK,
#if defined(DPPI_PRESENT)
        .shared = false
#endif
    };

    return &r;
}

const nrf_802154_sl_event_handle_t * nrf_802154_trx_radio_phyend_event_handle_get(void)
{
    static const nrf_802154_sl_event_handle_t r = {
#if defined(DPPI_PRESENT)
        .event_addr = NRF_802154_DPPI_RADIO_PHYEND,
        .shared     = true
#else
        .event_addr = (uint32_t)&NRF_RADIO->EVENTS_PHYEND
#endif
    };

    return &r;
}
