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
 * @defgroup nrf_802154_spinel_serialization_backend
 * 802.15.4 radio driver spinel serialization backend
 * @{
 *
 */

#ifndef NRF_802154_SPINEL_BACKEND_H_
#define NRF_802154_SPINEL_BACKEND_H_

#include <stddef.h>

#include "nrf_802154_serialization_error.h"
#include "nrf_802154_spinel_backend_callouts.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sends a spinel frame over spinel backend.
 *
 * @param[in]  p_data    Pointer to a buffer that contains spinel encoded frame.
 * @param[in]  data_len  Size of the @ref p_data buffer.
 *
 * @returns  number of bytes sent or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_encoded_packet_send(const void * p_data,
                                                           size_t       data_len);

/**
 * @brief Initializes spinel backend.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_backend_init(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SPINEL_BACKEND_H_ */

/** @} */
