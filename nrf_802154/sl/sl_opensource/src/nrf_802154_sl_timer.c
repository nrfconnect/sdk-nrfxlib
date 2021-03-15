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

#include "nrf_802154_sl_utils.h"

#include <stdlib.h>
#include <stdint.h>

#include <kernel.h>
#include <assert.h>

#include "timer/nrf_802154_timer_coord.h"
#include "timer/nrf_802154_timer_sched.h"

static void timeout_handler(struct k_timer * timer_id);

K_TIMER_DEFINE(timer, timeout_handler, NULL);

void nrf_802154_timer_coord_init(void)
{
    // Intentionally empty
}

void nrf_802154_timer_coord_uninit(void)
{
    // Intentionally empty
}

void nrf_802154_timer_coord_start(void)
{
    // Intentionally empty
}

void nrf_802154_timer_coord_stop(void)
{
    // Intentionally empty
}

void nrf_802154_timer_sched_init(void)
{
    BUILD_ASSERT(CONFIG_SYS_CLOCK_TICKS_PER_SEC == NRF_802154_SL_RTC_FREQUENCY);
}

void nrf_802154_timer_sched_deinit(void)
{
    // Intentionally empty
}

uint32_t nrf_802154_timer_sched_time_get(void)
{
    return NRF_802154_SL_RTC_TICKS_TO_US(k_uptime_ticks());
}

void nrf_802154_timer_sched_add(nrf_802154_timer_t * p_timer, bool round_up)
{
    (void)round_up;
    assert(!(p_timer->p_next)); // Only one timer is allowed to run at a time.

    uint32_t now    = nrf_802154_timer_sched_time_get();
    uint32_t target = p_timer->t0 + p_timer->dt - now;

    k_timer_user_data_set(&timer, (void *)p_timer->callback); // Passing arguments is not supported.
    k_timer_start(&timer, K_USEC(target), K_NO_WAIT);
}

void nrf_802154_timer_sched_remove(nrf_802154_timer_t * p_timer, bool * p_was_running)
{
    if (k_timer_status_get(&timer) > 0)
    {
        /* Timer has expired. */
        if (p_was_running)
        {
            *p_was_running = false;
        }
    }
    else if (k_timer_remaining_get(&timer) == 0)
    {
        /* Timer was stopped (by someone else) before expiring. */
        if (p_was_running)
        {
            *p_was_running = false;
        }
    }
    else
    {
        /* Timer is still running. */
        k_timer_stop(&timer);
        if (p_was_running)
        {
            *p_was_running = true;
        }
    }
}

bool nrf_802154_timer_sched_time_is_in_future(uint32_t now, uint32_t t0, uint32_t dt)
{
    (void)now;
    (void)t0;
    (void)dt;
    return false;
}

static void timeout_handler(struct k_timer * timer_id)
{
    nrf_802154_timer_callback_t callback =
        (nrf_802154_timer_callback_t)k_timer_user_data_get(timer_id);

    callback(NULL); // Passing arguments is not supported.
}

bool nrf_802154_timer_sched_is_running(nrf_802154_timer_t * p_timer)
{
    (void)p_timer;
    return k_timer_status_get(&timer) < 0;
}

void nrf_802154_lp_timer_init(void)
{
    // Intentionally empty
}

void nrf_802154_lp_timer_deinit(void)
{
    // Intentionally empty
}

void nrf_802154_lp_timer_critical_section_enter(void)
{
    // Intentionally empty
}

void nrf_802154_lp_timer_critical_section_exit(void)
{
    // Intentionally empty
}
