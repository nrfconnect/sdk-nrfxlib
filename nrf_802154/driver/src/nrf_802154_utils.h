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

#ifndef NRF_802154_UTILS_H__
#define NRF_802154_UTILS_H__

#include "nrf_802154_assert.h"
#include <stdint.h>
#include "nrfx.h"
#include <lib/nrfx_coredep.h>

#ifdef __STATIC_INLINE__
#undef __STATIC_INLINE__
#endif

#ifdef NRF_802154_UTILS_DECLARE_ONLY
#define __STATIC_INLINE__
#else
#define __STATIC_INLINE__ __STATIC_INLINE
#endif

/**
 * @defgroup nrf_802154_utils Utils definitions used in the 802.15.4 driver
 * @{
 * @ingroup nrf_802154
 * @brief Definitions of utils used in the 802.15.4 driver.
 */

/**@brief RTC clock frequency. */
#define NRF_802154_RTC_FREQUENCY               32768UL

/**@brief Defines the number of microseconds in one second. */
#define NRF_802154_US_PER_S                    1000000ULL

/**@brief Number of microseconds in one RTC tick (rounded up). */
#define NRF_802154_US_PER_TICK                 NRF_802154_RTC_TICKS_TO_US(1)

/**@brief Number of bits to shift RTC_FREQUENCY and US_PER_S to achieve the division by greatest common divisor. */
#define NRF_802154_FREQUENCY_US_PER_S_GCD_BITS 6

/**@brief Ceil division helper. */
#define NRF_802154_DIVIDE_AND_CEIL(A, B)       (((A) + (B)-1) / (B))

/**@brief Defines the conversion of RTC ticks to microseconds (us). */
#define NRF_802154_RTC_TICKS_TO_US(ticks)                                          \
    NRF_802154_DIVIDE_AND_CEIL(                                                    \
        (ticks) * (NRF_802154_US_PER_S >> NRF_802154_FREQUENCY_US_PER_S_GCD_BITS), \
        (NRF_802154_RTC_FREQUENCY >> NRF_802154_FREQUENCY_US_PER_S_GCD_BITS))

/**@brief Macro to get the number of elements in an array.
 *
 * @param[in] X   Array.
 */
#define NUMELTS(X) (sizeof((X)) / sizeof(X[0]))

/**@brief Type holding MCU critical section state.
 *
 * Variable of this type is required to hold state saved by @ref nrf_802154_mcu_critical_enter
 * and restored by @ref nrf_802154_mcu_critical_exit.
 */
typedef uint32_t nrf_802154_mcu_critical_state_t;

/**@brief Active waiting for given number of microseconds.
 *
 * It is guaranteed that execution of this function will take at least @c time_in_us
 * number of microseconds.
 */
__STATIC_INLINE__ void nrf_802154_delay_us(uint32_t time_in_us);

/**@brief Enters critical section on MCU level.
 *
 * Use @ref nrf_802154_mcu_critical_exit complementary. Consider following code:
 * @code
 * nrf_802154_mcu_critical_state_t mcu_cs;
 * mcu_cs = nrf_802154_mcu_critical_enter();
 * // do your critical stuff as fast as possible
 * nrf_802154_mcu_critical_exit(mcu_cs);
 * @endcode
 *
 * @returns Value to be passed to complementary call to @ref nrf_802154_mcu_critical_exit.
 */
__STATIC_INLINE__ nrf_802154_mcu_critical_state_t nrf_802154_mcu_critical_enter(void);

/**@brief Exits critical section on MCU level.
 *
 * This shall be used complementary to @ref nrf_802154_mcu_critical_enter.
 *
 * @param mcu_cs Value returned by @ref nrf_802154_mcu_critical_enter.
 */
__STATIC_INLINE__ void nrf_802154_mcu_critical_exit(nrf_802154_mcu_critical_state_t mcu_cs);

/**@brief Convert microseconds to RTC ticks.
 *
 * @param[in]  time  Time in microseconds.
 *
 * @return Time in RTC ticks.
 */
__STATIC_INLINE__ uint64_t NRF_802154_US_TO_RTC_TICKS(uint64_t time);

#ifndef NRF_802154_UTILS_DECLARE_ONLY

__STATIC_INLINE__ void nrf_802154_delay_us(uint32_t time_in_us)
{
    nrfx_coredep_delay_us(time_in_us);
}

__STATIC_INLINE__ nrf_802154_mcu_critical_state_t nrf_802154_mcu_critical_enter(void)
{
    nrf_802154_mcu_critical_state_t mcu_cs;

    mcu_cs = __get_PRIMASK();
    __disable_irq();

    return mcu_cs;
}

__STATIC_INLINE__ void nrf_802154_mcu_critical_exit(nrf_802154_mcu_critical_state_t mcu_cs)
{
    __set_PRIMASK(mcu_cs);
}

__STATIC_INLINE__ uint64_t NRF_802154_US_TO_RTC_TICKS(uint64_t time)
{
    uint64_t t1, u1;
    uint64_t result;

    /* The required range for time is [0..315360000000000], and the calculation below is
       verified to work within a broader range [0...2^49 ~ 17 years].

       This first step in the calculation is to find out how many units
       of 15625 us there are in the input_us, because 512 RTC units
       correspond _exactly_ to 15625 us. The desired calculation is therefore
       t1 = time / 15625, but the division is slow and therefore let's calculate
       t1 = time * k instead. The constant k is 1/15625, shifted up by as many bits
       as possible without causing overflow during the calculation.

       49 bits are needed to store the maximum value that time can have, and the
       lowest 13 bits in that value can be shifted away because a minimum of 14 bits
       are needed to store the divisor.

       This means that the time can be reduced to 49 - 13 = 36 bits to make space
       for k.

       The most suitable number of shift for the value 1/15625 = 0.000064
       (binary 0.00000000000001000011000110111101111...) is 41, because that results
       in a 28-bit number that does not cause overflow in the multiplication.

       ((2^41)/15625) is equal to 0x8637bd0, and is written in a hexadecimal representation
       to show the bit width of the number. Shifting is limited to 41 bits because:
         1  The time uses up to 49 bits;
         2) The time can only be shifted down 13 bits to avoid shifting away
            a full unit of 15625 microseconds;
         3) The maximum value of the calculation would otherwise overflow (that is,
            (315360000000000 >> 13) * 0x8637bd0 = 0x4b300bfcd0aefde0 would no longer be less than
            0Xffffffffffffffff).

       There is a possible loss of precision, so that t1 will be up to 93*15625 _smaller_
       than the accurate number. This is taken into account in the next step.
     */

    t1     = ((time >> 13) * 0x8637bd0) >> 28; // ((time >> 13) * (2^41 / 15625)) >> (41 - 13)
    result = t1 * 512;
    t1     = time - t1 * 15625;

    /* This second step of the calculation is to find out how many RTC units there are
       still left in the remaining microseconds.

       ((2^56)/15625) is equal to 0x431bde82d7b, and is written in a hexadecimal representation
       to show the bit width of the number. Shifting 56 bits is determined by the worst
       case value of t1. The constant is selected by using the same methodology as in the
       first step of the calculation above.

       The possible loss of precision in the calculation above can make t1 93*15625 lower
       than it should have been here. The worst case found is that t1 can be 1453125, and
       therefore there is no overflow in the calculation
       1453125 * 0x431bde82d7b = 0x5cfffffffff76627 (that is, it is less than 0xffffffffffffffff).

       15625 below is the binary representation of 30.51757813 (11110.100001001)
       scaled up by 2^9, and the calculation below are therefore using that scale.

       Rounding up to the nearest RTC tick is done by adding the value of the least
       significant bits of the fraction (that is, adding the value of bits 1..47 of the scaled
       up timer unit size (2^47)) to the calculated value before scaling the final
       value down to RTC ticks.*/

    // ceil((time * (2^56 / 15625)) >> (56 - 9))
    NRF_802154_ASSERT(t1 <= 1453125);
    u1   = (t1 * 0x431bde82d7b); // (time * (2^56 / 15625))
    u1  += 0x7fffffffffff;       // round up
    u1 >>= 47;                   // ceil(u1 >> (56 - 9))

    result += u1;

    return result;
}

#endif /* NRF_802154_UTILS_DECLARE_ONLY */

/**
 *@}
 **/

#endif // NRF_802154_UTILS_H__
