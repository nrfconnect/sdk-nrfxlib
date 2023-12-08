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

#include <stddef.h>

#include "nrf_802154.h"
#include "nrf_802154_stats.h"

#define NUMBER_OF_STAT_COUNTERS (sizeof(nrf_802154_stat_counters_t) / sizeof(uint32_t))

/**@brief Structure holding statistics about the Radio Driver behavior. */
volatile nrf_802154_stats_t g_nrf_802154_stats;

void nrf_802154_stats_get(nrf_802154_stats_t * p_stats)
{
    *p_stats = g_nrf_802154_stats;
}

void nrf_802154_stat_counters_get(nrf_802154_stat_counters_t * p_stat_counters)
{
    *p_stat_counters = g_nrf_802154_stats.counters;
}

void nrf_802154_stat_counters_subtract(const nrf_802154_stat_counters_t * p_stat_counters)
{
    volatile uint32_t * p_dst = (volatile uint32_t *)(&g_nrf_802154_stats.counters);
    const uint32_t    * p_src = (const uint32_t *)p_stat_counters;

    for (size_t i = 0; i < NUMBER_OF_STAT_COUNTERS; ++i)
    {
        nrf_802154_mcu_critical_state_t mcu_cs;

        nrf_802154_mcu_critical_enter(mcu_cs);
        *p_dst -= *p_src;
        nrf_802154_mcu_critical_exit(mcu_cs);

        p_dst++;
        p_src++;
    }
}

void nrf_802154_stat_timestamps_get(nrf_802154_stat_timestamps_t * p_stat_timestamps)
{
    *p_stat_timestamps = g_nrf_802154_stats.timestamps;
}

void nrf_802154_stat_counters_reset(void)
{
    volatile uint32_t * p_dst = (volatile uint32_t *)(&g_nrf_802154_stats.counters);

    for (size_t i = 0; i < NUMBER_OF_STAT_COUNTERS; ++i)
    {
        *(p_dst++) = 0U;
    }
}
