/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
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

#include <stdint.h>
#include <string.h>
#include "nrf_802154_assert.h"

#ifndef NRF_802154_UTILS_BYTEORDER_H
#define NRF_802154_UTILS_BYTEORDER_H

/**
 * @defgroup nrf_802154_utils_byteorder Utils definitions for byte ordering used in the 802.15.4 driver
 * @{
 * @ingroup nrf_802154
 * @brief Definitions of utils for byte ordering used in the 802.15.4 driver.
 */

#if !defined(__BYTE_ORDER__)

/* Couldn't determine endian-ness of the target machine. */
#error "Please define __BYTE_ORDER__!"

#elif defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

/**@brief Write 64-bit value from host byte-order to little-endian byte array.
 *
 * @param[in]  value     A 64-bit host-order value to write
 * @param[out] p_buffer  A little-endian byte array to be populated
 */
static inline void host_64_to_little(uint64_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);
    memcpy(p_buffer, &value, sizeof(uint64_t));
}

/**@brief Write 32-bit value from host byte-order to little-endian byte array.
 *
 * @param[in]  value     A 32-bit host-order value to write
 * @param[out] p_buffer  A little-endian byte array to be populated
 */
static inline void host_32_to_little(uint32_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);
    memcpy(p_buffer, &value, sizeof(uint32_t));
}

/**@brief Write 24-bit value from host byte-order to little-endian byte array.
 *
 * @param[in]  value     A 32-bit host-order value, whose lower 24 bits will be copied
 * @param[out] p_buffer  A little-endian byte array to be populated
 */
static inline void host_24_to_little(uint32_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);
    memcpy(p_buffer, &value, 3);
}

/**@brief Write 16-bit value from host byte-order to little-endian byte array.
 *
 * @param[in]  value     A 16-bit host-order value to write
 * @param[out] p_buffer  A little-endian byte array to be populated
 */
static inline void host_16_to_little(uint16_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);
    memcpy(p_buffer, &value, sizeof(uint16_t));
}

/**@brief Convert 64-bit value from little-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A little-endian byte array to be read
 *
 * @returns   A 64-bit host-order value
 */
static inline uint64_t little_64_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint64_t value = 0;

    memcpy(&value, p_buffer, sizeof(uint64_t));

    return value;
}

/**@brief Convert 32-bit value from little-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A little-endian byte array to be read
 *
 * @returns   A 32-bit host-order value
 */
static inline uint32_t little_32_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint32_t value = 0;

    memcpy(&value, p_buffer, sizeof(uint32_t));

    return value;
}

/**@brief Convert 24-bit value from little-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A little-endian byte array to be read
 *
 * @returns   A 24-bit host-order value
 */
static inline uint32_t little_24_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint32_t value = 0;

    memcpy(&value, p_buffer, 3);

    return value;
}

/**@brief Convert 16-bit value from little-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A little-endian byte array to be read
 *
 * @returns   A 16-bit host-order value
 */
static inline uint16_t little_16_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint16_t value = 0;

    memcpy(&value, p_buffer, sizeof(uint16_t));

    return value;
}

/**@brief Write 64-bit value from host byte-order to big-endian byte array.
 *
 * @param[in]  value     A 64-bit host-order value to write
 * @param[out] p_buffer  A big-endian byte array to be populated
 */
static inline void host_64_to_big(uint64_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint8_t shift = (sizeof(uint64_t) - 1) * 8;

    for (uint8_t i = 0; i < sizeof(uint64_t); i++, shift -= 8)
    {
        p_buffer[i] = (value >> shift) & 0xff;
    }
}

/**@brief Write 32-bit value from host byte-order to big-endian byte array.
 *
 * @param[in]  value     A 32-bit host-order value to write
 * @param[out] p_buffer  A big-endian byte array to be populated
 */
static inline void host_32_to_big(uint32_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint8_t shift = (sizeof(uint32_t) - 1) * 8;

    for (uint8_t i = 0; i < sizeof(uint32_t); i++, shift -= 8)
    {
        p_buffer[i] = (value >> shift) & 0xff;
    }
}

/**@brief Write 24-bit value from host byte-order to big-endian byte array.
 *
 * @param[in]  value     A 24-bit host-order value to write
 * @param[out] p_buffer  A big-endian byte array to be populated
 */
static inline void host_24_to_big(uint32_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint8_t shift = (sizeof(uint32_t) - 2) * 8;

    for (uint8_t i = 0; i < sizeof(uint32_t) - 1; i++, shift -= 8)
    {
        p_buffer[i] = (value >> shift) & 0xff;
    }
}

/**@brief Write 16-bit value from host byte-order to big-endian byte array.
 *
 * @param[in]  value     A 16-bit host-order value to write
 * @param[out] p_buffer  A big-endian byte array to be populated
 */
static inline void host_16_to_big(uint16_t value, uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint8_t shift = (sizeof(uint16_t) - 1) * 8;

    for (uint8_t i = 0; i < sizeof(uint16_t); i++, shift -= 8)
    {
        p_buffer[i] = (value >> shift) & 0xff;
    }
}

/**@brief Convert 64-bit value from big-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A big-endian byte array to be read
 *
 * @returns   A 64-bit host-order value
 */
static inline uint64_t big_64_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint64_t value = 0;

    for (uint8_t i = 0; i < sizeof(uint64_t); i++)
    {
        value = (value << 8) | p_buffer[i];
    }

    return value;
}

/**@brief Convert 32-bit value from big-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A big-endian byte array to be read
 *
 * @returns   A 32-bit host-order value
 */
static inline uint32_t big_32_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint32_t value = 0;

    for (uint8_t i = 0; i < sizeof(uint32_t); i++)
    {
        value = (value << 8) | p_buffer[i];
    }

    return value;
}

/**@brief Convert 24-bit value from big-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A big-endian byte array to be read
 *
 * @returns   A 24-bit host-order value
 */
static inline uint32_t big_24_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint32_t value = 0;

    for (uint8_t i = 0; i < sizeof(uint32_t) - 1; i++)
    {
        value = (value << 8) | p_buffer[i];
    }

    return value;
}

/**@brief Convert 16-bit value from big-endian byte array to host-order value.
 *
 * @param[in] p_buffer  A big-endian byte array to be read
 *
 * @returns   A 16-bit host-order value
 */
static inline uint16_t big_16_to_host(uint8_t * p_buffer)
{
    NRF_802154_ASSERT(p_buffer != NULL);

    uint16_t value = 0;

    for (uint8_t i = 0; i < sizeof(uint16_t); i++)
    {
        value = (value << 8) | p_buffer[i];
    }

    return value;
}

#else

/* Most likely the case of big-endian machine. */
#error "Unsupported endian-ness of the target machine"

#endif // __BYTE_ORDER__

/**
 *@}
 **/

#endif // NRF_802154_UTILS_BYTEORDER_H
