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

/**
 * @file Definitions of utils used in the nRF 802.15.4 Service Layer.
 *
 */

#ifndef NRF_802154_SL_UTILS_H__
#define NRF_802154_SL_UTILS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include "nrfx.h"

/**@brief RTC clock frequency. */
#define NRF_802154_SL_RTC_FREQUENCY               32768UL

/**@brief Defines the number of microseconds in one second. */
#define NRF_802154_SL_US_PER_S                    1000000ULL

/**@brief Number of microseconds in one RTC tick (rounded up). */
#define NRF_802154_SL_US_PER_TICK                 NRF_802154_SL_RTC_TICKS_TO_US(1)

/**@brief Number of bits to shift RTC_FREQUENCY and US_PER_S to achieve the division by greatest common divisor. */
#define NRF_802154_SL_FREQUENCY_US_PER_S_GCD_BITS 6

/**@brief Ceil division helper. */
#define NRF_802154_SL_DIVIDE_AND_CEIL(A, B)       (((A) + (B)-1) / (B))

/**@brief Defines the conversion of RTC ticks to microseconds (us). */
#define NRF_802154_SL_RTC_TICKS_TO_US(ticks)                                             \
    NRF_802154_SL_DIVIDE_AND_CEIL(                                                       \
        (ticks) * (NRF_802154_SL_US_PER_S >> NRF_802154_SL_FREQUENCY_US_PER_S_GCD_BITS), \
        (NRF_802154_SL_RTC_FREQUENCY >> NRF_802154_SL_FREQUENCY_US_PER_S_GCD_BITS))

/**@brief Maximum of two values. */
#define max(a, b) ((a) > (b) ? (a) : (b))

/**@brief Minimum of two values. */
#define min(a, b) ((a) < (b) ? (a) : (b))

/**@brief Converts microseconds (us) to RTC ticks.
 *
 * @param time      Time is us to be converted to RTC ticks.
 * @param round_up  Round up resulting value, otherwise the result is rounded down.
 *
 * @return Time converted to RTC ticks.
 */
uint64_t NRF_802154_SL_US_TO_RTC_TICKS(uint64_t time, bool round_up);

/**@brief Type holding MCU critical section state.
 *
 * Variable of this type is required to hold state saved by @ref nrf_802154_sl_mcu_critical_enter
 * and restored by @ref nrf_802154_sl_mcu_critical_exit.
 */
typedef uint32_t nrf_802154_sl_mcu_critical_state_t;

/**@brief Enters critical section on MCU level.
 *
 * Use @ref nrf_802154_sl_mcu_critical_exit complementary. Consider following code:
 * @code
 * nrf_802154_sl_mcu_critical_state_t mcu_cs;
 * nrf_802154_sl_mcu_critical_enter(mcu_cs);
 * // do your critical stuff as fast as possible
 * nrf_802154_sl_mcu_critical_exit(mcu_cs);
 * @endcode
 *
 * @param mcu_critical_state    Variable of @ref nrf_802154_sl_mcu_critical_state_t where current
 *                              state of MCU level critical section will be stored.
 */
#define nrf_802154_sl_mcu_critical_enter(mcu_critical_state) \
    do                                                       \
    {                                                        \
        (mcu_critical_state) = __get_PRIMASK();              \
        __disable_irq();                                     \
    }                                                        \
    while (0)

/**@brief Exits critical section on MCU level.
 *
 * This shall be used complementary to @ref nrf_802154_sl_mcu_critical_enter.
 *
 * @param mcu_critical_state    Variable of @ref nrf_802154_sl_mcu_critical_state_t where
 *                              state of MCU level critical section is stored by
 *                              former call to @ref nrf_802154_sl_mcu_critical_enter
 */
#define nrf_802154_sl_mcu_critical_exit(mcu_critical_state) \
    do                                                      \
    {                                                       \
        __set_PRIMASK(mcu_critical_state);                  \
    }                                                       \
    while (0)

/**
 * @brief Checks if the given time is in the future.
 *
 * The value of @p now is assumed to be based on a 64-bit time source that never overflows.
 * These assumptions are met, for example, by @ref nrf_802154_sl_timer_current_time_get().
 *
 * @param[in]  now  Current time.
 * @param[in]  t0   The time value to be checked.
 *
 * @retval true   Given time @p t0 is in future (compared to given @p now).
 * @retval false  Given time @p t0 is not in future (compared to given @p now).
 */
static inline bool nrf_802154_sl_time64_is_in_future(uint64_t now, uint64_t t0)
{
    return now < t0;
}

#endif // NRF_802154_SL_UTILS_H__
