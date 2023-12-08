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
 * @defgroup nrf_802154_serialization_error_helper
 * 802.15.4 radio driver serialization error helpers
 * @{
 *
 */

#ifndef NRF_802154_SERIALIZATION_ERROR_HELPER_H_
#define NRF_802154_SERIALIZATION_ERROR_HELPER_H_

#include "nrf_802154_serialization_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SERIALIZATION_ERROR_INIT(name) \
    nrf_802154_ser_err_t name = NRF_802154_SERIALIZATION_ERROR_OK;

/**
 * Throw serialization error e.
 */
#define SERIALIZATION_ERROR_RAISE_IF_FAILED(e)             \
    do                                                     \
    {                                                      \
        if (e != NRF_802154_SERIALIZATION_ERROR_OK)        \
        {                                                  \
            nrf_802154_ser_err_data_t err = {.reason = e}; \
            nrf_802154_serialization_error(&err);          \
        }                                                  \
    }                                                      \
    while (0)

/**
 * Update error e with reason r and go to label l.
 */
#define SERIALIZATION_ERROR(r, e, l) \
    do                               \
    {                                \
        e = r;                       \
        goto l;                      \
    }                                \
    while (0)

/**
 * Update error e with reason r if condition c is met and go to label l.
 */
#define SERIALIZATION_ERROR_IF(c, r, e, l) \
    do                                     \
    {                                      \
        if (c)                             \
        {                                  \
            SERIALIZATION_ERROR(r, e, l);  \
        }                                  \
    }                                      \
    while (0)

/**
 * Update error e if reason r indicates error condition and go to label l.
 */
#define SERIALIZATION_ERROR_CHECK(r, e, l) \
    SERIALIZATION_ERROR_IF(r < 0, r, e, l)

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SERIALIZATION_ERROR_HELPER_H_ */

/** @} */
