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

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_TRX_PPI

#include "nrf_802154_trx_ppi_api.h"

#include "nrf_802154_debug_log.h"
#include "nrf_802154_peripherals.h"

#include "hal/nrf_dppi.h"
#include "hal/nrf_egu.h"
#include "hal/nrf_radio.h"
#include "hal/nrf_timer.h"

#define EGU_EVENT                   NRF_EGU_EVENT_TRIGGERED15
#define EGU_TASK                    NRF_EGU_TASK_TRIGGER15

#define DPPI_CHGRP_RAMP_UP          NRF_DPPI_CHANNEL_GROUP0 ///< PPI group used to disable self-disabling PPIs
#define DPPI_CHGRP_RAMP_UP_DIS_TASK NRF_DPPI_TASK_CHG0_DIS  ///< PPI task used to disable self-disabling PPIs

#define PPI_DISABLED_EGU            NRF_802154_DPPI_RADIO_DISABLED_TO_EGU
#define PPI_EGU_RAMP_UP             NRF_802154_DPPI_EGU_TO_RADIO_RAMP_UP
#define PPI_TIMER_TX_ACK            NRF_802154_DPPI_TIMER_COMPARE_TO_RADIO_TXEN
#define PPI_RADIO_SYNC_EGU_SYNC     NRF_802154_DPPI_RADIO_SYNC_TO_EGU_SYNC

void nrf_802154_trx_ppi_for_ramp_up_set(nrf_radio_task_t ramp_up_task, bool start_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Clr event EGU (needed for nrf_802154_trx_ppi_for_ramp_up_was_triggered)
    nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_EVENT);

    nrf_dppi_channels_include_in_group(NRF_DPPIC, 1UL << PPI_EGU_RAMP_UP, DPPI_CHGRP_RAMP_UP);
    nrf_egu_publish_set(NRF_802154_EGU_INSTANCE, EGU_EVENT, PPI_EGU_RAMP_UP);
    nrf_radio_subscribe_set(NRF_RADIO, ramp_up_task, PPI_EGU_RAMP_UP);
    nrf_dppi_subscribe_set(NRF_DPPIC, DPPI_CHGRP_RAMP_UP_DIS_TASK, PPI_EGU_RAMP_UP);

    if (start_timer)
    {
        nrf_timer_subscribe_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START, PPI_DISABLED_EGU);
    }

    nrf_egu_subscribe_set(NRF_802154_EGU_INSTANCE, EGU_TASK, PPI_DISABLED_EGU);
    nrf_radio_publish_set(NRF_RADIO, NRF_RADIO_EVENT_DISABLED, PPI_DISABLED_EGU);

    nrf_dppi_channels_enable(NRF_DPPIC,
                             (1UL << PPI_EGU_RAMP_UP) |
                             (1UL << PPI_DISABLED_EGU));

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ramp_up_clear(nrf_radio_task_t ramp_up_task, bool start_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_dppi_channels_disable(NRF_DPPIC,
                              (1UL << PPI_EGU_RAMP_UP) |
                              (1UL << PPI_DISABLED_EGU));

    nrf_egu_publish_clear(NRF_802154_EGU_INSTANCE, EGU_EVENT);
    nrf_radio_subscribe_clear(NRF_RADIO, ramp_up_task);
    nrf_dppi_subscribe_clear(NRF_DPPIC, DPPI_CHGRP_RAMP_UP_DIS_TASK);
    nrf_dppi_channels_remove_from_group(NRF_DPPIC, 1UL << PPI_EGU_RAMP_UP, DPPI_CHGRP_RAMP_UP);

    if (start_timer)
    {
        nrf_timer_subscribe_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START);
    }

    nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE, EGU_TASK);
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);

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

    nrf_dppi_channels_enable(NRF_DPPIC, (1UL << PPI_TIMER_TX_ACK));

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ack_tx_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_dppi_channels_disable(NRF_DPPIC, (1UL << PPI_TIMER_TX_ACK));

    nrf_radio_subscribe_clear(NRF_RADIO, NRF_RADIO_TASK_TXEN);
    nrf_timer_publish_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_EVENT_COMPARE1);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_fem_set(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_timer_subscribe_set(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START, PPI_DISABLED_EGU);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_fem_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_timer_subscribe_clear(NRF_802154_TIMER_INSTANCE, NRF_TIMER_TASK_START);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

bool nrf_802154_trx_ppi_for_fem_powerdown_set(NRF_TIMER_Type * p_instance,
                                              uint32_t         compare_channel)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // TODO: Implement this function when FEM API supports nRF53

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
    return false;
}

void nrf_802154_trx_ppi_for_fem_powerdown_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // TODO: Implement this function when FEM API supports nRF53

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
    nrf_dppi_channels_enable(NRF_DPPIC, (1UL << PPI_RADIO_SYNC_EGU_SYNC));

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_radio_sync_clear(nrf_egu_task_t task)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_dppi_channels_disable(NRF_DPPIC, (1UL << PPI_RADIO_SYNC_EGU_SYNC));
    nrf_radio_publish_clear(NRF_RADIO, NRF_RADIO_EVENT_SYNC);
    nrf_egu_subscribe_clear(NRF_802154_EGU_INSTANCE, task);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#endif
