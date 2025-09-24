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

#ifndef NRF_802154_STATS_H_
#define NRF_802154_STATS_H_

#include "nrfx.h"
#include "nrf_802154_types.h"
#include "nrf_802154_sl_atomics.h"

#ifdef __STATIC_INLINE__
#undef __STATIC_INLINE__
#endif

#ifdef NRF_802154_STATS_DECLARE_ONLY
#define __STATIC_INLINE__
#else
#define __STATIC_INLINE__ __STATIC_INLINE
#endif

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_cca_failed_attempts(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_received_frames(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_received_energy_events(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_received_preambles(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_requests(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_granted_requests(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_denied_requests(void);
__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_unsolicited_grants(void);

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_csmaca_start_timestamp(uint64_t value);
__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_cca_start_timestamp(uint64_t value);
__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_cca_idle_timestamp(uint64_t value);
__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_tx_end_timestamp(uint64_t value);
__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_ack_end_timestamp(uint64_t value);
__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_rx_end_timestamp(uint64_t value);

__STATIC_INLINE__ uint64_t nrf_802154_stat_timestamp_read_last_rx_end_timestamp(void);
__STATIC_INLINE__ uint64_t nrf_802154_stat_timestamp_read_last_ack_end_timestamp(void);

#ifndef NRF_802154_STATS_DECLARE_ONLY

#define __NRF_802154_STAT_COUNTER_INC_IMPL(_field_name)                           \
    do                                                                            \
    {                                                                             \
        extern nrf_802154_stats_t g_nrf_802154_stats;                             \
        nrf_802154_sl_atomic_add_u32(&g_nrf_802154_stats.counters._field_name, 1);\
    }                                                                             \
    while (0)

#define __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(_field_name, _value)                           \
    do                                                                                       \
    {                                                                                        \
        extern nrf_802154_stats_t g_nrf_802154_stats;                                        \
        nrf_802154_sl_atomic_store_u64(&g_nrf_802154_stats.timestamps._field_name, (_value));\
    }                                                                                        \
    while (0)

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_cca_failed_attempts(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(cca_failed_attempts);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_received_frames(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(received_frames);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_received_energy_events(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(received_energy_events);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_received_preambles(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(received_preambles);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_requests(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(coex_requests);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_granted_requests(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(coex_granted_requests);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_denied_requests(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(coex_denied_requests);
}

__STATIC_INLINE__ void nrf_802154_stat_counter_increment_coex_unsolicited_grants(void)
{
    __NRF_802154_STAT_COUNTER_INC_IMPL(coex_unsolicited_grants);
}

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_csmaca_start_timestamp(uint64_t value)
{
    __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(last_csmaca_start_timestamp, value);
}

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_cca_start_timestamp(uint64_t value)
{
    __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(last_cca_start_timestamp, value);
}

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_cca_idle_timestamp(uint64_t value)
{
    __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(last_cca_idle_timestamp, value);
}

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_tx_end_timestamp(uint64_t value)
{
    __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(last_tx_end_timestamp, value);
}

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_ack_end_timestamp(uint64_t value)
{
    __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(last_ack_end_timestamp, value);
}

__STATIC_INLINE__ void nrf_802154_stat_timestamp_write_last_rx_end_timestamp(uint64_t value)
{
    __NRF_80214_STAT_TIMESTAMP_WRITE_IMPL(last_rx_end_timestamp, value);
}

__STATIC_INLINE__ uint64_t nrf_802154_stat_timestamp_read_last_rx_end_timestamp(void)
{
    extern nrf_802154_stats_t g_nrf_802154_stats;

    return nrf_802154_sl_atomic_load_u64(&g_nrf_802154_stats.timestamps.last_rx_end_timestamp);
}

__STATIC_INLINE__ uint64_t nrf_802154_stat_timestamp_read_last_ack_end_timestamp(void)
{
    extern nrf_802154_stats_t g_nrf_802154_stats;

    return nrf_802154_sl_atomic_load_u64(&g_nrf_802154_stats.timestamps.last_ack_end_timestamp);
}

#endif /* NRF_802154_STATS_DECLARE_ONLY */

#endif /* NRF_802154_STATS_H_ */
