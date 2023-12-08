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

#include "nrf_802154_sl_utils.h"

#include <stdlib.h>
#include <stdint.h>

#include <zephyr/kernel.h>

#include "nrf_802154_sl_timer.h"

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

void nrf_802154_sl_timer_module_init(void)
{
    // Intentionally empty
}

void nrf_802154_sl_timer_module_uninit(void)
{
    // Intentionally empty
}

uint64_t nrf_802154_sl_timer_current_time_get(void)
{
    int64_t ticks = k_uptime_ticks();

    return k_ticks_to_us_ceil64(ticks);
}

void nrf_802154_sl_timer_init(nrf_802154_sl_timer_t * p_timer)
{
    // Intentionally empty
}

void nrf_802154_sl_timer_deinit(nrf_802154_sl_timer_t * p_timer)
{
    // Intentionally empty
}

nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_add(nrf_802154_sl_timer_t * p_timer)
{
    uint64_t now    = nrf_802154_sl_timer_current_time_get();
    int32_t  target = p_timer->trigger_time - now;

    target = MAX(target, 1);

    k_timer_user_data_set(&timer, p_timer); // Passing arguments is not supported.
    k_timer_start(&timer, K_USEC(target), K_NO_WAIT);

    return NRF_802154_SL_TIMER_RET_SUCCESS;
}

nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_remove(nrf_802154_sl_timer_t * p_timer)
{
    nrf_802154_sl_timer_ret_t ret;

    if (k_timer_status_get(&timer) > 0)
    {
        /* Timer has expired. */
        ret = NRF_802154_SL_TIMER_RET_INACTIVE;
    }
    else if (k_timer_remaining_get(&timer) == 0)
    {
        /* Timer was stopped (by someone else) before expiring. */
        ret = NRF_802154_SL_TIMER_RET_INACTIVE;
    }
    else
    {
        /* Timer is still running. */
        k_timer_stop(&timer);

        ret = NRF_802154_SL_TIMER_RET_SUCCESS;
    }

    return ret;
}

static void timeout_handler(struct k_timer * timer_id)
{
    nrf_802154_sl_timer_t * p_timer =
        (nrf_802154_sl_timer_t *)k_timer_user_data_get(timer_id);

    p_timer->action.callback.callback(p_timer);
}

void nrf_802154_platform_sl_lp_timer_init(void)
{
    // Intentionally empty
}

void nrf_802154_platform_sl_lp_timer_deinit(void)
{
    // Intentionally empty
}

void nrf_802154_platform_sl_lptimer_critical_section_enter(void)
{
    // Intentionally empty
}

void nrf_802154_platform_sl_lptimer_critical_section_exit(void)
{
    // Intentionally empty
}
