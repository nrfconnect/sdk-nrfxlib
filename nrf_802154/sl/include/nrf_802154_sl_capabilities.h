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

/**
 * @file This file implements the nRF 802.15.4 Service Layer capabilities API.
 *
 */
#ifndef NRF_802154_SL_CAPABILITIES_H__
#define NRF_802154_SL_CAPABILITIES_H__

#include <stdint.h>

/**
 * @brief Capabilities of nrf 802.15.4 service layer
 *
 * Possible values:
 * - @ref NRF_802154_SL_CAPABILITY_CSMA,
 * - @ref NRF_802154_SL_CAPABILITY_DELAYED_TX,
 * - @ref NRF_802154_SL_CAPABILITY_DELAYED_RX,
 * - @ref NRF_802154_SL_CAPABILITY_ANT_DIVERSITY,
 * - @ref NRF_802154_SL_CAPABILITY_MULTITIMER,
 * - @ref NRF_802154_SL_CAPABILITY_TIMESTAMP
 *
 */
typedef uint32_t nrf_802154_sl_capabilities_t;

#define NRF_802154_SL_CAPABILITY_CSMA          (1UL << 0UL) // !< CSMA-CA supported
#define NRF_802154_SL_CAPABILITY_DELAYED_TX    (1UL << 1UL) // !< TX at specified time supported
#define NRF_802154_SL_CAPABILITY_DELAYED_RX    (1UL << 2UL) // !< RX at specified time supported
#define NRF_802154_SL_CAPABILITY_ANT_DIVERSITY (1UL << 3UL) // !< Antenna diversity supported
#define NRF_802154_SL_CAPABILITY_MULTITIMER    (1UL << 4UL) // !< Scheduling multiple timers supported
#define NRF_802154_SL_CAPABILITY_TIMESTAMP     (1UL << 5UL) // !< Frame timestamping supported

/**
 * @brief Get capabilities of current nRF 802.15.4 SL implementation.
 *
 * @return Mask of current SL capabilities
 */
nrf_802154_sl_capabilities_t nrf_802154_sl_capabilities_get(void);

#endif // NRF_802154_SL_CAPABILITIES_H__
