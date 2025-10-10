/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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

#include "nrf_802154_config.h"

#if NRF_802154_CSMA_CA_ENABLED
#include "nrf_802154_compiler.h"
#include "platform/nrf_802154_random.h"
#if NRF_802154_TEST_MODES_ENABLED
#include "nrf_802154_pib.h"
#endif
#include "nrf_802154_csma_ca_backoff.h"

static inline uint8_t backoff_periods_get_random(uint8_t be)
{
    return (uint8_t)(nrf_802154_random_get() % (1U << be));
}

uint8_t nrf_802154_csma_ca_backoff_periods_get(uint8_t be)
{
    uint8_t result;

#if NRF_802154_TEST_MODES_ENABLED

    switch (nrf_802154_pib_test_mode_csmaca_backoff_get())
    {
        case NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MAX:
            result = (1U << be) - 1U;
            break;

        case NRF_802154_TEST_MODE_CSMACA_BACKOFF_ALWAYS_MIN:
            result = 0U;
            break;

        case NRF_802154_TEST_MODE_CSMACA_BACKOFF_RANDOM:
            SWITCH_CASE_FALLTHROUGH;

        default:
            result = backoff_periods_get_random(be);
            break;
    }
#else
    result = backoff_periods_get_random(be);
#endif

    return result;
}

#endif // NRF_802154_CSMA_CA_ENABLED
