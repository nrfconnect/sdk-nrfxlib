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
 * @defgroup nrf_802154_serialization_error
 * 802.15.4 radio driver serialization error handler
 * @{
 *
 */

#ifndef NRF_802154_SERIALIZATION_ERROR_H_
#define NRF_802154_SERIALIZATION_ERROR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief No error.
 */
#define NRF_802154_SERIALIZATION_ERROR_OK               (0)

/**
 * @brief Error was reported by backend.
 */
#define NRF_802154_SERIALIZATION_ERROR_BACKEND_FAILURE  (-1)

/**
 * @brief Error was reported by spinel data packing layer.
 */
#define NRF_802154_SERIALIZATION_ERROR_ENCODING_FAILURE (-2)

/**
 * @brief Error was reported by spinel data unpacking layer.
 */
#define NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE (-3)

/**
 * @brief Invalid command response was received.
 */
#define NRF_802154_SERIALIZATION_ERROR_RESPONSE_INVALID (-4)

/**
 * @brief Timeout occured when waiting for command response.
 */
#define NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT (-5)

/**
 * @brief Initialization of spinel serialization failed.
 */
#define NRF_802154_SERIALIZATION_ERROR_INIT_FAILED      (-6)

/**
 * @brief Received command with invalid arguments.
 */
#define NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID  (-7)

/**
 * @brief Frame buffering reported an error.
 */
#define NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER   (-8)

/**
 * @brief Insufficient amount of memory reported by frame buffering.
 */
#define NRF_802154_SERIALIZATION_ERROR_NO_MEMORY        (-9)

/**
 * @brief Datatype for nrf_802154 serialization error reason.
 */
typedef int32_t nrf_802154_ser_err_t;

/**
 * @brief Struct with nrf_802154 serialization error description.
 */
typedef struct
{
    /**
     * @brief Reason of the reported error.
     */
    nrf_802154_ser_err_t reason;
} nrf_802154_ser_err_data_t;

/**
 * @brief Notifies any serialization related errors to the next higher layer.
 *
 * @param[in]  p_err  Pointer to a @ref nrf_802154_ser_err_data_t with error description.
 *
 */
extern void nrf_802154_serialization_error(const nrf_802154_ser_err_data_t * p_err);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SERIALIZATION_ERROR_H_ */

/** @} */
