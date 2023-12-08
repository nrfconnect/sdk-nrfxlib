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
 * @defgroup nrf_802154_spinel_serialization
 * 802.15.4 radio driver spinel serialization
 * @{
 *
 */

#ifndef NRF_802154_SPINEL_H_
#define NRF_802154_SPINEL_H_

#include "nrf_802154_serialization_error.h"
#include "nrf_802154_buffer_mgr_dst.h"
#include "nrf_802154_buffer_mgr_src.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT
/**
 * @brief Default response timeout in us for response to serialized request.
 */
#define CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT 500
#endif /* CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT */

/**
 * @brief Maximal size of a Spinel frame in 802.15.4 serialization.
 */
#define NRF_802154_SPINEL_FRAME_MAX_SIZE    296

/**
 * @brief Buffer size for Spinel frame in 802.15.4 serialization.
 *
 * This macro can used as a replacement for @ref SPINEL_FRAME_BUFFER_SIZE in 802.15.4 serialization
 * to reduce memory required for Spinel frame processing.
 */
#define NRF_802154_SPINEL_FRAME_BUFFER_SIZE (NRF_802154_SPINEL_FRAME_MAX_SIZE + \
                                             SPINEL_ENCRYPTER_EXTRA_DATA_SIZE)

/**
 * @brief Serializes data according to format string and sends it over spinel backend.
 *
 * @param[in]  p_fmt  Pointer to a format string describing data types to be serialized.
 *                    Format string should conform to spinel specification.
 * @param[in]  ...    Data to be serialized and sent according to @ref p_fmt format string.
 *
 * @returns  number of bytes sent or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_send(const char * p_fmt, ...);

/**
 * @brief Gets buffer manager for transactions originated by the remote serialization peer.
 *
 * @returns  Pointer to the destination frame buffer manager.
 */
nrf_802154_buffer_mgr_dst_t * nrf_802154_spinel_dst_buffer_mgr_get(void);

/**
 * @brief Gets buffer manager for transactions originated by the local serialization peer.
 *
 * @returns Pointer to the source frame buffer manager.
 */
nrf_802154_buffer_mgr_src_t * nrf_802154_spinel_src_buffer_mgr_get(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SPINEL_H_ */

/** @} */
