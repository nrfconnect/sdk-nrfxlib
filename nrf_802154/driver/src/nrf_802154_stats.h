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

#ifndef NRF_802154_STATS_H_
#define NRF_802154_STATS_H_

#include "nrf_802154_types.h"
#include "nrf_802154_utils.h"

#if !defined(UNIT_TEST)
// Don't use directly. Use provided nrf_802154_stat_xxxx API macros.
extern volatile nrf_802154_stats_t g_nrf_802154_stats;

extern volatile nrf_802154_stat_totals_t g_nrf_802154_stat_totals;

/**@brief Increment one of the @ref nrf_802154_stat_counters_t fields.
 *
 * @param field_name    Identifier of struct member to increment
 */
#define nrf_802154_stat_counter_increment(field_name) \
    do                                                \
    {                                                 \
        nrf_802154_mcu_critical_state_t mcu_cs;       \
                                                      \
        nrf_802154_mcu_critical_enter(mcu_cs);        \
        (g_nrf_802154_stats.counters.field_name)++;   \
        nrf_802154_mcu_critical_exit(mcu_cs);         \
    }                                                 \
    while (0)

/**@brief Write one of the @ref nrf_802154_stat_timestamps_t fields.
 *
 * @param field_name    Identifier of struct member to write
 * @param value         Value to write
 */
#define nrf_802154_stat_timestamp_write(field_name, value)    \
    do                                                        \
    {                                                         \
        (g_nrf_802154_stats.timestamps.field_name) = (value); \
    }                                                         \
    while (0)

/**@brief Read one of the @ref nrf_802154_stat_timestamps_t fields. */
#define nrf_802154_stat_timestamp_read(field_name) \
    (g_nrf_802154_stats.timestamps.field_name)

#define nrf_802154_stat_totals_increment(field_name, value) \
    do                                                      \
    {                                                       \
        nrf_802154_mcu_critical_state_t mcu_cs;             \
                                                            \
        nrf_802154_mcu_critical_enter(mcu_cs);              \
        (g_nrf_802154_stat_totals.field_name) += (value);   \
        nrf_802154_mcu_critical_exit(mcu_cs);               \
    }                                                       \
    while (0)

extern void nrf_802154_stat_totals_get_notify(void);

#else // !defined(UNIT_TEST)

#define nrf_802154_stat_counter_increment(field_name) \
    nrf_802154_stat_counter_increment_func(offsetof(nrf_802154_stat_counters_t, field_name))

#define nrf_802154_stat_timestamp_write(field_name, value)                                   \
    nrf_802154_stat_timestamp_write_func(offsetof(nrf_802154_stat_timestamps_t, field_name), \
                                         (value))

#define nrf_802154_stat_timestamp_read(field_name) \
    nrf_802154_stat_timestamp_read_func(offsetof(nrf_802154_stat_timestamps_t, field_name))

// Functions for which mocks are generated.
void nrf_802154_stat_counter_increment_func(size_t field_offset);
void nrf_802154_stat_timestamp_write_func(size_t field_offset, uint32_t value);
uint32_t nrf_802154_stat_timestamp_read_func(size_t field_offset);

#endif // !defined(UNIT_TEST)

#endif /* NRF_802154_STATS_H_ */
