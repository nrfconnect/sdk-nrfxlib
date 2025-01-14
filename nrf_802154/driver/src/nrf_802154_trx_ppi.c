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

#ifdef NRF52_SERIES

#include "nrf_802154_trx_ppi_api.h"

#include <stdbool.h>

#include "nrf_802154_debug_log.h"
#include "nrf_802154_peripherals.h"

#include "protocol/mpsl_fem_protocol_api.h"

#include "hal/nrf_egu.h"
#include "hal/nrf_ppi.h"
#include "hal/nrf_radio.h"
#include "hal/nrf_timer.h"

#define EGU_EVENT                  NRF_802154_EGU_RAMP_UP_EVENT
#define EGU_TASK                   NRF_802154_EGU_RAMP_UP_TASK

#define PPI_CHGRP_RAMP_UP          NRF_802154_PPI_CORE_GROUP                  ///< PPI group used to disable self-disabling PPIs
#define PPI_CHGRP_RAMP_UP_DIS_TASK NRF_PPI_TASK_CHG0_DIS                      ///< PPI task used to disable self-disabling PPIs

#define PPI_CHGRP_ABORT            NRF_802154_PPI_ABORT_GROUP                 ///< PPI group used to disable PPIs when async event aborting radio operation is propagated through the system

#define PPI_DISABLED_EGU           NRF_802154_PPI_RADIO_DISABLED_TO_EGU       ///< PPI that connects RADIO DISABLED event with EGU task
#define PPI_RAMP_UP_TRG_EGU        NRF_802154_PPI_RADIO_RAMP_UP_TRIGG         ///< PPI that connects ramp up trigger event with EGU task
#define PPI_EGU_RAMP_UP            NRF_802154_PPI_EGU_TO_RADIO_RAMP_UP        ///< PPI that connects EGU event with RADIO TXEN or RXEN task
#define PPI_EGU_TIMER_START        NRF_802154_PPI_EGU_TO_TIMER_START          ///< PPI that connects EGU event with TIMER START task
#define PPI_TIMER_TX_ACK           NRF_802154_PPI_TIMER_COMPARE_TO_RADIO_TXEN ///< PPI that connects TIMER COMPARE event with RADIO TXEN task
#define PPI_RADIO_SYNC_EGU_SYNC    NRF_802154_PPI_RADIO_SYNC_TO_EGU_SYNC      ///< PPI that connects RADIO SYNC event with EGU task for SYNC channel

void nrf_802154_trx_ppi_for_enable(void)
{
    // Intentionally empty.
}

void nrf_802154_trx_ppi_for_disable(void)
{
    // Intentionally empty.
}

uint8_t nrf_802154_trx_ppi_for_ramp_up_channel_id_get(void)
{
    return PPI_RAMP_UP_TRG_EGU;
}

void nrf_802154_trx_ppi_for_ramp_up_set(nrf_radio_task_t                      ramp_up_task,
                                        nrf_802154_trx_ramp_up_trigger_mode_t trigg_mode,
                                        bool                                  start_timer)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    // Clr event EGU (needed for nrf_802154_trx_ppi_for_ramp_up_was_triggered)
    nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_EVENT);

    uint32_t ppi_mask = (1UL << PPI_EGU_RAMP_UP) | (1UL << PPI_RAMP_UP_TRG_EGU);

    nrf_ppi_channel_and_fork_endpoint_setup(NRF_PPI,
                                            PPI_EGU_RAMP_UP,
                                            nrf_egu_event_address_get(
                                                NRF_802154_EGU_INSTANCE,
                                                EGU_EVENT),
                                            nrf_radio_task_address_get(NRF_RADIO, ramp_up_task),
                                            nrf_ppi_task_address_get(NRF_PPI,
                                                                     PPI_CHGRP_RAMP_UP_DIS_TASK));

    if (start_timer)
    {
        nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                       PPI_EGU_TIMER_START,
                                       nrf_egu_event_address_get(NRF_802154_EGU_INSTANCE,
                                                                 EGU_EVENT),
                                       nrf_timer_task_address_get(NRF_802154_TIMER_INSTANCE,
                                                                  NRF_TIMER_TASK_START));

        ppi_mask |= (1UL << PPI_EGU_TIMER_START);
    }

    if (trigg_mode == TRX_RAMP_UP_SW_TRIGGER)
    {
        nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                       PPI_RAMP_UP_TRG_EGU,
                                       nrf_radio_event_address_get(NRF_RADIO,
                                                                   NRF_RADIO_EVENT_DISABLED),
                                       nrf_egu_task_address_get(NRF_802154_EGU_INSTANCE, EGU_TASK));
    }
    else
    {
        nrf_ppi_task_endpoint_setup(NRF_PPI,
                                    PPI_RAMP_UP_TRG_EGU,
                                    nrf_egu_task_address_get(NRF_802154_EGU_INSTANCE, EGU_TASK));
    }

    nrf_ppi_channel_include_in_group(NRF_PPI, PPI_EGU_RAMP_UP, PPI_CHGRP_RAMP_UP);

    nrf_ppi_channels_enable(NRF_PPI, ppi_mask);

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
#error NRF_802154_CCAIDLE_TO_TXEN_EXTRA_TIME_US is unsupported for the selected SoC
#endif

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_extra_cca_attempts_set(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART,
                                   nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_CCABUSY),
                                   nrf_radio_task_address_get(NRF_RADIO, NRF_RADIO_TASK_CCASTART));

    nrf_ppi_channels_enable(NRF_PPI, 1UL << NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ramp_up_reconfigure(void)
{
    nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, EGU_EVENT);

    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   PPI_DISABLED_EGU,
                                   nrf_radio_event_address_get(NRF_RADIO,
                                                               NRF_RADIO_EVENT_DISABLED),
                                   nrf_egu_task_address_get(NRF_802154_EGU_INSTANCE, EGU_TASK));
}

void nrf_802154_trx_ppi_for_ramp_up_clear(nrf_radio_task_t ramp_up_task, bool start_timer)
{
    (void)ramp_up_task;
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    uint32_t ppi_mask = (1UL << PPI_EGU_RAMP_UP) |
                        (1UL << PPI_RAMP_UP_TRG_EGU);

    if (start_timer)
    {
        ppi_mask |= (1UL << PPI_EGU_TIMER_START);
    }

    nrf_ppi_channels_disable(NRF_PPI, ppi_mask);
    nrf_ppi_channel_and_fork_endpoint_setup(NRF_PPI, PPI_EGU_RAMP_UP, 0, 0, 0);
    nrf_ppi_channel_endpoint_setup(NRF_PPI, PPI_RAMP_UP_TRG_EGU, 0, 0);

    if (start_timer)
    {
        nrf_ppi_channel_endpoint_setup(NRF_PPI, PPI_EGU_TIMER_START, 0, 0);
    }

    nrf_ppi_channel_remove_from_group(NRF_PPI, PPI_EGU_RAMP_UP, PPI_CHGRP_RAMP_UP);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_txframe_ramp_up_clear(bool cca)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_802154_trx_ppi_for_ramp_up_clear(cca ? NRF_RADIO_TASK_RXEN : NRF_RADIO_TASK_TXEN, false);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_extra_cca_attempts_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_ppi_channel_disable(NRF_PPI, NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART);
    nrf_ppi_channel_endpoint_setup(NRF_PPI, NRF_802154_PPI_RADIO_CCABUSY_TO_RADIO_CCASTART, 0, 0);

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

    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   PPI_TIMER_TX_ACK,
                                   nrf_timer_event_address_get(NRF_802154_TIMER_INSTANCE,
                                                               NRF_TIMER_EVENT_COMPARE1),
                                   nrf_radio_task_address_get(NRF_RADIO,
                                                              NRF_RADIO_TASK_TXEN));
    nrf_ppi_channel_enable(NRF_PPI, PPI_TIMER_TX_ACK);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_ack_tx_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_ppi_channel_disable(NRF_PPI, PPI_TIMER_TX_ACK);
    nrf_ppi_channel_endpoint_setup(NRF_PPI, PPI_TIMER_TX_ACK, 0, 0);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_fem_set(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    uint32_t event_addr = nrf_egu_event_address_get(NRF_802154_EGU_INSTANCE,
                                                    EGU_EVENT);
    uint32_t task_addr = nrf_timer_task_address_get(NRF_802154_TIMER_INSTANCE,
                                                    NRF_TIMER_TASK_START);

    nrf_ppi_channel_endpoint_setup(NRF_PPI, PPI_EGU_TIMER_START, event_addr, task_addr);
    nrf_ppi_channel_enable(NRF_PPI, PPI_EGU_TIMER_START);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_fem_clear(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_ppi_channel_disable(NRF_PPI, PPI_EGU_TIMER_START);
    nrf_ppi_channel_endpoint_setup(NRF_PPI, PPI_EGU_TIMER_START, 0, 0);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

uint32_t nrf_802154_trx_ppi_group_for_abort_get(void)
{
    return (uint32_t)PPI_CHGRP_ABORT;
}

#if defined(RADIO_INTENSET_SYNC_Msk)
void nrf_802154_trx_ppi_for_radio_sync_set(nrf_egu_task_t task)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   PPI_RADIO_SYNC_EGU_SYNC,
                                   nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_SYNC),
                                   nrf_egu_task_address_get(NRF_802154_EGU_INSTANCE, task));
    nrf_ppi_channel_enable(NRF_PPI, PPI_RADIO_SYNC_EGU_SYNC);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

void nrf_802154_trx_ppi_for_radio_sync_clear(nrf_egu_task_t task)
{
    (void)task;
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_HIGH);

    nrf_ppi_channel_disable(NRF_PPI, PPI_RADIO_SYNC_EGU_SYNC);
    nrf_ppi_channel_endpoint_setup(NRF_PPI, PPI_RADIO_SYNC_EGU_SYNC, 0, 0);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_HIGH);
}

#endif

#endif // NRF52_SERIES
