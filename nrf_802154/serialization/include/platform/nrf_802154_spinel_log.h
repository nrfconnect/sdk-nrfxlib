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
 * @defgroup nrf_802154_spinel_serialization_logging
 * 802.15.4 radio driver spinel serialization logging
 * @{
 *
 */

#ifndef NRF_802154_SPINEL_LOG_H_
#define NRF_802154_SPINEL_LOG_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_NRF_802154_SER_LOG

#define NRF_802154_SPINEL_LOG_RAW(...)                   nrf_802154_spinel_log(__VA_ARGS__)
#define NRF_802154_SPINEL_BUFF_LOG_RAW(p_buff, buff_len) nrf_802154_spinel_buff_log(p_buff, \
                                                                                    buff_len)

#else // CONFIG_NRF_802154_SER_LOG

#define NRF_802154_SPINEL_LOG_RAW(...)      ((void)(0))
#define NRF_802154_SPINEL_BUFF_LOG_RAW(...) ((void)(0))

#endif // CONFIG_NRF_802154_SER_LOG

#define NRF_802154_SPINEL_LOG_BANNER_CALLING()               \
    do                                                       \
    {                                                        \
        NRF_802154_SPINEL_LOG_RAW("Calling %s\n", __func__); \
    }                                                        \
    while (0)

#define NRF_802154_SPINEL_LOG_BANNER_RESPONSE()                            \
    do                                                                     \
    {                                                                      \
        NRF_802154_SPINEL_LOG_RAW("Received response for %s\n", __func__); \
    }                                                                      \
    while (0)

#define NRF_802154_SPINEL_LOG_VAR_NAMED(fmt, var, name)         \
    do                                                          \
    {                                                           \
        NRF_802154_SPINEL_LOG_RAW("\t%s: "fmt "\n", name, var); \
    }                                                           \
    while (0)

#define NRF_802154_SPINEL_LOG_VAR(fmt, var) \
    NRF_802154_SPINEL_LOG_VAR_NAMED(fmt, var, #var)

#define NRF_802154_SPINEL_LOG_BUFF_NAMED(p_buff, buff_len, name)             \
    do                                                                       \
    {                                                                        \
        NRF_802154_SPINEL_LOG_RAW("\t%s: ", name);                           \
        NRF_802154_SPINEL_BUFF_LOG_RAW(p_buff, buff_len);                    \
        NRF_802154_SPINEL_LOG_RAW("\n\t%s_address: 0x%08x\n", name, p_buff); \
    }                                                                        \
    while (0)

#define NRF_802154_SPINEL_LOG_BUFF(p_buff, buff_len) \
    NRF_802154_SPINEL_LOG_BUFF_NAMED(p_buff, buff_len, #p_buff)

/**
 * @brief Printf-like function for logging from spinel serialization module.
 *
 * @param[in]  p_fmt  Pointer to a format string.
 * @param[in]  ...    Data to be printed according to @ref p_fmt format string.
 *
 */
void nrf_802154_spinel_log(const char * p_fmt, ...);

/**
 * @brief Log contents of the buffer.
 *
 * @param[in]  p_buff    Pointer to a buffer to be printed.
 * @param[in]  buff_len  Size of the @ref p_buff buffer.
 *
 */
void nrf_802154_spinel_buff_log(const uint8_t * p_buff, size_t buff_len);

#ifdef __cplusplus
}
#endif

#endif /* SPINEL_H_ */

/** @} */
