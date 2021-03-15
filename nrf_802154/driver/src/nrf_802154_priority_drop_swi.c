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
 *   This file implements procedures that should be called with lower priority than caller in
 *   the nrf 802.15.4 radio driver.
 *
 */

#include <assert.h>

#include "nrf_802154_config.h"
#include "nrf_802154_peripherals.h"
#include "rsch/nrf_802154_rsch_prio_drop.h"

#include "nrf_802154_swi.h"
#include "hal/nrf_egu.h"
#include "platform/nrf_802154_clock.h"

#define HFCLK_STOP_INT   NRF_EGU_INT_TRIGGERED1   ///< Label of HFClk stop interrupt.
#define HFCLK_STOP_TASK  NRF_EGU_TASK_TRIGGER1    ///< Label of HFClk stop task.
#define HFCLK_STOP_EVENT NRF_EGU_EVENT_TRIGGERED1 ///< Label of HFClk stop event.

/**
 * @brief Requests a stop of the HF clock.
 *
 * The notification is triggered from the SWI priority level.
 *
 * @note This function is to be called through notification module to prevent calling it from
 *       the arbiter context.
 */
static void swi_hfclk_stop(void)
{
    assert(!nrf_egu_event_check(NRF_802154_EGU_INSTANCE, HFCLK_STOP_EVENT));

    nrf_egu_task_trigger(NRF_802154_EGU_INSTANCE, HFCLK_STOP_TASK);
}

/**
 * @brief Terminates the stopping of the HF clock.
 *
 * @note This function terminates the stopping of the HF clock only if it has not been performed
 * yet.
 */
static void swi_hfclk_stop_terminate(void)
{
    nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, HFCLK_STOP_EVENT);
}

void nrf_802154_rsch_prio_drop_init(void)
{
    nrf_egu_int_enable(NRF_802154_EGU_INSTANCE, HFCLK_STOP_INT);

    nrf_802154_swi_init();
}

void nrf_802154_rsch_prio_drop_hfclk_stop(void)
{
    swi_hfclk_stop();
}

void nrf_802154_rsch_prio_drop_hfclk_stop_terminate(void)
{
    swi_hfclk_stop_terminate();
}

void nrf_802154_rsch_prio_drop_swi_irq_handler(void)
{
    if (nrf_egu_event_check(NRF_802154_EGU_INSTANCE, HFCLK_STOP_EVENT))
    {
        nrf_802154_clock_hfclk_stop();

        nrf_egu_event_clear(NRF_802154_EGU_INSTANCE, HFCLK_STOP_EVENT);
    }
}
