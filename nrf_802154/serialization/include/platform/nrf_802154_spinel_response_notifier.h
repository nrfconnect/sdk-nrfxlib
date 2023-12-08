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
 * @defgroup nrf_802154_spinel_serialization_response_notifier
 * 802.15.4 radio driver spinel serialization response notifier
 * @{
 *
 */

#ifndef NRF_802154_RESPONSE_NOTIFIER_H_
#define NRF_802154_RESPONSE_NOTIFIER_H_

#include <stdint.h>
#include <stddef.h>

#include "../spinel_base/spinel.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Infinite timeout for @ref nrf_802154_spinel_response_notifier_property_await.
 */
#define SPINEL_RESPONSE_NOTIFIER_INF_TIMEOUT UINT32_MAX

/**
 * @brief Data buffer type used by response notifier module.
 */
typedef struct
{
    /**
     * @brief Buffer for notification data storage.
     */
    uint8_t data[SPINEL_FRAME_MAX_COMMAND_PAYLOAD_SIZE];
    /**
     * @brief Size of data stored in @ref data.
     */
    size_t data_len;
} nrf_802154_spinel_notify_buff_t;

/**
 * @brief Initialize module
 */
void nrf_802154_spinel_response_notifier_init(void);

/**
 * @brief Lock access to response notifier before request is sent.
 *
 * It is possible that while one thread is waiting for response, another thread
 * sends other request. Currently matching responses to requests is not
 * implemented. Because of that before a new request is sent, the previous
 * response must be received.
 *
 * This function shall be used before sending a request that will require awaiting for a response.
 *
 * An awaited property is an identificatior of a response that is to be returned for a serialized
 * call that takes place after the notifier lock. The property shall be awaited with
 * @ref nrf_802154_spinel_response_notifier_property_await.
 *
 * @param[in]  property  Awaited property.
 *
 */
void nrf_802154_spinel_response_notifier_lock_before_request(spinel_prop_key_t property);

/**
 * @brief Wait with timeout for property to be notified.
 *
 * @param[in]  timeout   Timeout in us.
 *
 * @returns  pointer to @ref nrf_802154_spinel_notify_buff_t with notified property data
 *           or NULL in case of timeout.
 *
 */
nrf_802154_spinel_notify_buff_t * nrf_802154_spinel_response_notifier_property_await(
    uint32_t timeout);

/**
 * @brief Free data bufer returned by @ref nrf_802154_spinel_response_notifier_property_await.
 *
 * @param[in]  p_notify  Data buffer to be freed
 *
 */
void nrf_802154_spinel_response_notifier_free(nrf_802154_spinel_notify_buff_t * p_notify);

/**
 * @brief Notify that spinel property that may be awaited was received.
 *
 * @param[in]  property  Property to be notified.
 * @param[in]  p_data    Pointer to a buffer that contains data associated with @ref property.
 * @param[in]  data_len  Size of the @ref p_data buffer.
 *
 */
void nrf_802154_spinel_response_notifier_property_notify(spinel_prop_key_t property,
                                                         const void      * p_data,
                                                         size_t            data_len);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_RESPONSE_NOTIFIER_H_ */

/** @} */
