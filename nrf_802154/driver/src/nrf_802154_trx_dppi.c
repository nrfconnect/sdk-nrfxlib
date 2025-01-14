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

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_TRX_PPI

#include "nrfx.h"

#if defined(DPPI_PRESENT)

#include "nrf_802154_trx_ppi_api.h"

#include "nrf_802154_debug_log.h"
#include "nrf_802154_peripherals.h"

#include "hal/nrf_dppi.h"
#include "hal/nrf_egu.h"
#include "hal/nrf_radio.h"
#include "hal/nrf_timer.h"

#define EGU_EVENT                   NRF_802154_EGU_RAMP_UP_EVENT
#define EGU_TASK                    NRF_802154_EGU_RAMP_UP_TASK

#define DPPI_CHGRP_RAMP_UP          NRF_DPPI_CHANNEL_GROUP0 ///< PPI group used to disable self-disabling PPIs
#define DPPI_CHGRP_RAMP_UP_DIS_TASK NRF_DPPI_TASK_CHG0_DIS  ///< PPI task used to disable self-disabling PPIs

#define PPI_DISABLED_EGU            NRF_802154_DPPI_RADIO_DISABLED
#define PPI_EGU_RAMP_UP             NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP
#define PPI_TIMER_TX_ACK            NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN
#define PPI_RADIO_SYNC_EGU_SYNC     NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
#define EGU_TIMER_START_EVENT       NRFX_CONCAT_2(NRF_EGU_EVENT_TRIGGERED, \
                                                  NRF_802154_EGU_TIMER_START_CHANNEL_NO)
#define EGU_TIMER_START_TASK        NRFX_CONCAT_2(NRF_EGU_TASK_TRIGGER, \
                                                  NRF_802154_EGU_TIMER_START_CHANNEL_NO)

#define EGU_TIMER_START2_EVENT      NRFX_CONCAT_2(NRF_EGU_EVENT_TRIGGERED, \
                                                  NRF_802154_EGU_TIMER_START2_CHANNEL_NO)
#define EGU_TIMER_START2_TASK       NRFX_CONCAT_2(NRF_EGU_TASK_TRIGGER, \
                                                  NRF_802154_EGU_TIMER_START2_CHANNEL_NO)
#endif

void nrf_802154_trx_ppi_for_enable(void)
{
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_DISABLED, PPI_DISABLED_EGU);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_READY, NRF_802154_DPPI_RADIO_READY);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS, NRF_802154_DPPI_RADIO_ADDRESS);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_END, NRF_802154_DPPI_RADIO_END);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_PHYEND, NRF_802154_DPPI_RADIO_PHYEND);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE, NRF_802154_DPPI_RADIO_CCAIDLE);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY, NRF_802154_DPPI_RADIO_CCABUSY);

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
    nrf_egu_publish_set(NRF_802154_EGU_INSTANCE,
                        EGU_TIMER_START_EVENT,
                        NRF_802154_DPPI_TIMER_START);
    nrf_timer_subscribe_set(NRF_802154_TIMER_INSTANCE,
                            NRF_TIMER_TASK_START,
                            NRF_802154_DPPI_TIMER_START);
#endif

    nrf_dppi_channels_enable(NRF_802154_DPPIC_INSTANCE,
                             (1UL << NRF_802154_DPPI_RADIO_CCABUSY) |
                             (1UL << PPI_DISABLED_EGU) |
                             (1UL << NRF_802154_DPPI_RADIO_READY) |
                             (1UL << NRF_802154_DPPI_RADIO_ADDRESS) |
                             (1UL << NRF_802154_DPPI_RADIO_END) |
                             (1UL << NRF_802154_DPPI_RADIO_PHYEND) |
                             (1UL << NRF_802154_DPPI_RADIO_CCAIDLE) |
#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
                             (1UL << NRF_802154_DPPI_TIMER_START) |
#endif
                             (1UL << NRF_802154_DPPI_RADIO_HW_TRIGGER));
}

void nrf_802154_trx_ppi_for_disable(void)
{
    nrf_dppi_channels_disable(NRF_802154_DPPIC_INSTANCE,
                              (1UL << NRF_802154_DPPI_RADIO_CCABUSY) |
                              (1UL << PPI_DISABLED_EGU) |
                              (1UL << NRF_802154_DPPI_RADIO_READY) |
                              (1UL << NRF_802154_DPPI_RADIO_ADDRESS) |
                              (1UL << NRF_802154_DPPI_RADIO_END) |
                              (1UL << NRF_802154_DPPI_RADIO_PHYEND) |
                              (1UL << NRF_802154_DPPI_RADIO_CCAIDLE) |
#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
                              (1UL << NRF_802154_DPPI_TIMER_START) |
#endif
                              (1UL << NRF_802154_DPPI_RADIO_HW_TRIGGER));

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
    nrf_timer_subscribe_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START);
    nrf_egu_publish_clear(NRF_802154_EGU_INSTANCE, EGU_TIMER_START_EVENT);
#endif

#if NRF_802154_TEST_MODES_ENABLED
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY);
#endif // NRF_802154_TEST_MODES_ENABLED
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_CCAIDLE);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_PHYEND);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_END);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_ADDRESS);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_READY);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
}

uint8_t nrf_802154_trx_ppi_for_ramp_up_channel_id_get(void)
{
    return NRF_802154_DPPI_RADIO_HW_TRIGGER;
}

void nrf_802154_trx_ppi_for_ramp_up_set(nrf_radio_task_t                      ramp_up_task,
                                        nrf_802154_trx_ramp_up_trigger_mode_t trigg_mode,
                                        bool                                  start_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Clr event EGU (needed for nrf_802154_trx_ppi_for_ramp_up_was_triggered)
    nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_EVENT);

    nrf_dppi_channels_include_in_group(NRF_802154_DPPIC_INSTANCE,
                                       1UL << PPI_EGU_RAMP_UP,
                                       DPPI_CHGRP_RAMP_UP);

    nrf_radio_subscribe_set(NRF_RADIO, ramp_up_task, PPI_EGU_RAMP_UP);
    nrf_dppi_subscribe_set(NRF_802154_DPPIC_INSTANCE, DPPI_CHGRP_RAMP_UP_DIS_TASK, PPI_EGU_RAMP_UP);

    if (start_timer)
    {
#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
        nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE,
                              EGU_TIMER_START_TASK,
                              PPI_DISABLED_EGU);
#else
        nrf_timer_subscribe_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START, PPI_DISABLED_EGU);
#endif
    }

    nrf_egu_publish_set(NRF_802154_EGU_INSTANCE, EGU_EVENT, PPI_EGU_RAMP_UP);

    nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE, EGU_TASK, PPI_DISABLED_EGU);

    nrf_dppi_channels_enable(NRF_802154_DPPIC_INSTANCE,
                             (1UL << PPI_EGU_RAMP_UP));

    if (trigg_mode == TRX_RAMP_UP_HW_TRIGGER)
    {
        nrf_radio_subscribe_set(NRF_RADIO,
                                NRF_RADIO_TASK_DISABLE,
                                NRF_802154_DPPI_RADIO_HW_TRIGGER);
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_txframe_ramp_up_set(
    bool                                  cca,
    nrf_802154_trx_ramp_up_trigger_mode_t trigg_mode)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_set(
        cca ? NRF_RADIO_TASK_RXEN : NRF_RADIO_TASK_TXEN,
        trigg_mode,
        false);

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
    if (cca)
    {
        nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE,
                              EGU_TIMER_START_TASK,
                              PPI_EGU_RAMP_UP);

        // Let the TIMER be started by RADIO.EVENTS_CCAIDLE
        // Connections marked with (*) is already done by nrf_802154_trx_ppi_for_enable.
        //
        // EGU.EGU_TIMER_START_EVENT ---(*)-+--> NRF_802154_DPPI_TIMER_START  <--(*)-- TIMER.TASKS_START
        // EGU.EGU_TIMER_START2_EVENT ------/
        //
        // RADIO.EVENTS_CCAIDLE --(*)-> NRF_802154_DPPI_RADIO_CCAIDLE <------ EGU.EGU_TIMER_START2_TASK

        nrf_egu_publish_set(NRF_802154_EGU_INSTANCE,
                            EGU_TIMER_START2_EVENT,
                            NRF_802154_DPPI_TIMER_START);
        nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE,
                              EGU_TIMER_START2_TASK,
                              NRF_802154_DPPI_RADIO_CCAIDLE);

        // Setup TIMER.CC[1] to a value when the RADIO.TASKS_TXEN should be triggered.
        // The TIMER.CC[0] holds value at which the TIMER stopped after the ramp-up for CCA operation.
        // The timer will continue counting from this value.
        uint32_t timer_val_at_ccaidle = nrf_timer_cc_get(NRF_802154_TIMER_INSTANCE,
                                                         NRF_TIMER_CC_CHANNEL0);

        nrf_timer_cc_set(NRF_802154_TIMER_INSTANCE,
                         NRF_TIMER_CC_CHANNEL1,
                         timer_val_at_ccaidle + NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US);
        nrf_timer_shorts_enable(NRF_802154_TIMER_INSTANCE,
                                NRF_TIMER_SHORT_COMPARE1_STOP_MASK);

        // Let the TIMER'.CC[1] trigger the RADIO.TASKS_TXEN
        //
        // TIMER.CC1 ----> NRF_802154_DPPI_RADIO_TXEN <----- RADIO.TASKS_TXEN
        nrf_timer_publish_set(NRF_802154_TIMER_INSTANCE,
                              NRF_TIMER_EVENT_COMPARE1,
                              NRF_802154_DPPI_RADIO_TXEN);

        nrf_radio_subscribe_set(NRF_RADIO,
                                NRF_RADIO_TASK_TXEN,
                                NRF_802154_DPPI_RADIO_TXEN);

        nrf_dppi_channels_enable(NRF_802154_DPPIC_INSTANCE,
                                 (1UL << NRF_802154_DPPI_RADIO_TXEN));
    }
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_extra_cca_attempts_set(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Assume that NRF_802154_DPPI_RADIO_CCABUSY is enabled and NRF_RADIO_EVENT_CCABUSY publishes to it
    nrf_radio_subscribe_set(NRF_RADIO, NRF_RADIO_TASK_CCASTART, NRF_802154_DPPI_RADIO_CCABUSY);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ramp_up_reconfigure(void)
{
    // Intentionally empty
}

void nrf_802154_trx_ppi_for_ramp_up_clear(nrf_radio_task_t ramp_up_task, bool start_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_dppi_channels_disable(NRF_802154_DPPIC_INSTANCE,
                              (1UL << PPI_EGU_RAMP_UP));

    nrf_egu_publish_clear(NRF_802154_EGU_INSTANCE, EGU_EVENT);
    nrf_radio_subscribe_clear(NRF_RADIO, ramp_up_task);
    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_DISABLE);

    nrf_dppi_subscribe_clear(NRF_802154_DPPIC_INSTANCE, DPPI_CHGRP_RAMP_UP_DIS_TASK);
    nrf_dppi_channels_remove_from_group(NRF_802154_DPPIC_INSTANCE,
                                        1UL << PPI_EGU_RAMP_UP,
                                        DPPI_CHGRP_RAMP_UP);

    if (start_timer)
    {
#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
        nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE, EGU_TIMER_START_TASK);
#else
        nrf_timer_subscribe_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START);
#endif
    }

    nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE, EGU_TASK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_txframe_ramp_up_clear(bool cca)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
    if (cca)
    {
        nrf_dppi_channels_disable(NRF_802154_DPPIC_INSTANCE,
                                  (1UL << NRF_802154_DPPI_RADIO_TXEN));
        nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_TXEN);
        nrf_timer_publish_clear(NRF_802154_TIMER_INSTANCE,
                                NRF_TIMER_EVENT_COMPARE1);
        nrf_timer_shorts_disable(NRF_802154_TIMER_INSTANCE,
                                 NRF_TIMER_SHORT_COMPARE1_STOP_MASK);

        nrf_egu_publish_clear(NRF_802154_EGU_INSTANCE,
                              EGU_TIMER_START2_EVENT);
        nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE,
                                EGU_TIMER_START2_TASK);

        nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE,
                                EGU_TIMER_START_TASK);
    }
#endif

    nrf_802154_trx_ppi_for_ramp_up_clear(cca ? NRF_RADIO_TASK_RXEN : NRF_RADIO_TASK_TXEN, false);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_extra_cca_attempts_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_CCASTART);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ramp_up_propagation_delay_wait(void)
{
    __ASM("nop");
    __ASM("nop");
    __ASM("nop");
    __ASM("nop");
    __ASM("nop");
    __ASM("nop");
}

bool nrf_802154_trx_ppi_for_ramp_up_was_triggered(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    if (nrf_radio_state_get(NRF_RADIO) != NRF_RADIO_STATE_DISABLED)
    {
        // If RADIO state is not DISABLED, it means that RADIO is still ramping down or already
        // started ramping up.
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
        return true;
    }

    // Wait for PPIs
    nrf_802154_trx_ppi_for_ramp_up_propagation_delay_wait();

    if (nrf_egu_event_check(NRF_802154_EGU_INSTANCE, EGU_EVENT))
    {
        // If EGU event is set, procedure is running.
        nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
        return true;
    }

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
    return false;
}

void nrf_802154_trx_ppi_for_ack_tx_set(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // TIMER_COMPARE1 ----> RADIO_TXEN
    nrf_radio_subscribe_set(NRF_RADIO, NRF_RADIO_TASK_TXEN, PPI_TIMER_TX_ACK);
    nrf_timer_publish_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_EVENT_COMPARE1, PPI_TIMER_TX_ACK);

    nrf_dppi_channels_enable(NRF_802154_DPPIC_INSTANCE, (1UL << PPI_TIMER_TX_ACK));

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ack_tx_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_dppi_channels_disable(NRF_802154_DPPIC_INSTANCE, (1UL << PPI_TIMER_TX_ACK));

    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_TXEN);
    nrf_timer_publish_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_EVENT_COMPARE1);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_fem_set(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
    nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE,
                          EGU_TIMER_START_TASK,
                          PPI_DISABLED_EGU);
#else
    nrf_timer_subscribe_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START, PPI_DISABLED_EGU);
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_fem_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

#if (NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US != 0)
    nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE, EGU_TIMER_START_TASK);
#else
    nrf_timer_subscribe_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START);
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

uint32_t nrf_802154_trx_ppi_group_for_abort_get(void)
{
    // TODO: Implement it when external event (like coex) can abort radio operation
    return 0;
}

#if defined(RADIO_INTENSET_SYNC_Msk)
void nrf_802154_trx_ppi_for_radio_sync_set(nrf_egu_task_t task)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_SYNC, PPI_RADIO_SYNC_EGU_SYNC);
    nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE, task, PPI_RADIO_SYNC_EGU_SYNC);
    nrf_dppi_channels_enable(NRF_802154_DPPIC_INSTANCE, (1UL << PPI_RADIO_SYNC_EGU_SYNC));

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_radio_sync_clear(nrf_egu_task_t task)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_dppi_channels_disable(NRF_802154_DPPIC_INSTANCE, (1UL << PPI_RADIO_SYNC_EGU_SYNC));
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_SYNC);
    nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE, task);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#endif

#endif // defined(DPPI_PRESENT)
