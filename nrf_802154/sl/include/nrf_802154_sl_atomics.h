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

#ifndef NRF_802154_SL_ATOMICS_H__
#define NRF_802154_SL_ATOMICS_H__

#include <stdbool.h>
#include <stdint.h>
#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Type representing a mutex */
typedef volatile uint32_t nrf_802154_sl_mutex_t;

/**@brief Initializes a mutex
 *
 * @param p_mutex  Pointer to a mutex to initialize, must not be NULL.
 */
void nrf_802154_sl_mutex_init(nrf_802154_sl_mutex_t * p_mutex);

/** @brief Tries to take a non-blocking mutex.
 *
 *  @param p_mutex  Pointer to a mutex to acquire, must not be NULL.
 *
 *  @retval  true   Mutex was acquired.
 *  @retval  false  Mutex could not be acquired, because it was already acquired.
 *                  This will cause the nearest call to @ref nrf_802154_sl_mutex_release to return
 *                  `true`
 */
bool nrf_802154_sl_mutex_try_acquire(nrf_802154_sl_mutex_t * p_mutex);

/**@brief Releases a non-blocking mutex.
 *
 * @note Attempt to release a non-acquired mutex is an undefined behavior.
 *
 * @param p_mutex  Pointer to a mutex to release, must not be NULL.
 *
 * @retval false   No other context tried to acquire mutex while it was taken.
 * @retval true    Some other context tried to acquire the mutex while it was already taken.
 */
bool nrf_802154_sl_mutex_release(nrf_802154_sl_mutex_t * p_mutex);

/**@brief Type representing an atomic uint8_t */
typedef volatile uint8_t nrf_802154_sl_atomic_uint8_t;

/**@brief Atomic increase of uint8_t value.
 *
 * Performs following code in an atomic way:
 * @code
 * *(p_value)++;
 * @endcode
 *
 * @param p_value   Pointer to a value to be incremented.
 */
void nrf_802154_sl_atomic_uint8_inc(nrf_802154_sl_atomic_uint8_t * p_value);

/**@brief Atomic strong compare-and-swap operation (word variant).
 *
 * Performs compare-and-swap operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj       Atomic variable object.
 * @param[in] p_expected  Pointer to expected value.
 *                        In case of CAS failure the argument will be updated with current obj value.
 * @param[in] desired     Desired object value.
 *
 * @retval  true   The object was assigned new value.
 * @retval  false  The object was not modified.
 */
static inline bool nrf_802154_sl_atomic_cas_u32(uint32_t * p_obj,
                                                uint32_t * p_expected,
                                                uint32_t   desired)
{
    __DMB();

    do
    {
        uint32_t old_val = __LDREXW((volatile uint32_t *)p_obj);

        if ( old_val != *p_expected)
        {
            *p_expected = old_val;
            __CLREX();
            return false;
        }
    }
    while (__STREXW(desired, (volatile uint32_t *)p_obj));

    __DMB();

    return true;
}

/**@brief Atomic strong compare-and-swap operation (half-word variant).
 *
 * Performs compare-and-swap operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj       Atomic variable object.
 * @param[in] p_expected  Pointer to expected value.
 *                        In case of CAS failure the argument will be updated with current obj value.
 * @param[in] desired     Desired object value.
 *
 * @retval  true   The object was assigned new value.
 * @retval  false  The object was not modified.
 */
static inline bool nrf_802154_sl_atomic_cas_u16(uint16_t * p_obj,
                                                uint16_t * p_expected,
                                                uint16_t   desired)
{
    __DMB();

    do
    {
        uint16_t old_val = __LDREXH((volatile uint16_t *)p_obj);

        if ( old_val != *p_expected)
        {
            *p_expected = old_val;
            __CLREX();
            return false;
        }
    }
    while (__STREXH(desired, (volatile uint16_t *)p_obj));

    __DMB();

    return true;
}

/**@brief Atomic strong compare-and-swap operation (byte variant).
 *
 * Performs compare-and-swap operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj       Atomic variable object.
 * @param[in] p_expected  Pointer to expected value.
 *                        In case of CAS failure the argument will be updated with current obj value.
 * @param[in] desired     Desired object value.
 *
 * @retval  true   The object was assigned new value.
 * @retval  false  The object was not modified.
 */
static inline bool nrf_802154_sl_atomic_cas_u8(uint8_t * p_obj,
                                               uint8_t * p_expected,
                                               uint8_t   desired)
{
    __DMB();

    do
    {
        uint8_t old_val = __LDREXB((volatile uint8_t *)p_obj);

        if ( old_val != *p_expected)
        {
            *p_expected = old_val;
            __CLREX();
            return false;
        }
    }
    while (__STREXB(desired, (volatile uint8_t *)p_obj));

    __DMB();

    return true;
}

/**@brief Atomic store operation (word variant).
 *
 * Performs store operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj     Atomic variable object.
 * @param[in] value     New object value.
 */
static inline void nrf_802154_sl_atomic_store_u32(uint32_t * p_obj, uint32_t value)
{
    __DMB();
    *p_obj = value;
    __DMB();
}

/**@brief Atomic store operation (half-word variant).
 *
 * Performs store operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj     Atomic variable object.
 * @param[in] value     New object value.
 */
static inline void nrf_802154_sl_atomic_store_u16(uint16_t * p_obj, uint16_t value)
{
    __DMB();
    *p_obj = value;
    __DMB();
}

/**@brief Atomic store operation (byte variant).
 *
 * Performs store operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj     Atomic variable object.
 * @param[in] value     New object value.
 */
static inline void nrf_802154_sl_atomic_store_u8(uint8_t * p_obj, uint8_t value)
{
    __DMB();
    *p_obj = value;
    __DMB();
}

/**@brief Atomic load operation (word variant).
 *
 * Performs load operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj       Atomic variable object.
 */
static inline uint32_t nrf_802154_sl_atomic_load_u32(uint32_t * p_obj)
{
    uint32_t value;

    __DMB();
    value = *p_obj;
    __DMB();

    return value;
}

/**@brief Atomic load operation (half-word variant).
 *
 * Performs load operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj       Atomic variable object.
 */
static inline uint16_t nrf_802154_sl_atomic_load_u16(uint16_t * p_obj)
{
    uint16_t value;

    __DMB();
    value = *p_obj;
    __DMB();

    return value;
}

/**@brief Atomic load operation (byte variant).
 *
 * Performs load operation with sequentially consistent memory ordering.
 *
 * @param[in] p_obj       Atomic variable object.
 */
static inline uint8_t nrf_802154_sl_atomic_load_u8(uint8_t * p_obj)
{
    uint8_t value;

    __DMB();
    value = *p_obj;
    __DMB();

    return value;
}

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_SL_ATOMICS_H__
