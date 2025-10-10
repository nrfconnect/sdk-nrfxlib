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

#ifndef NRF_802154_CSMA_CA_BACKOFF_H__
#define NRF_802154_CSMA_CA_BACKOFF_H__

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup nrf_802154_csma_ca_backoff 802.15.4 driver CSMA-CA support
 * @{
 * @ingroup nrf_802154_csma_ca
 * @brief CSMA-CA procedure.
 */

/** @brief Gets the number of backoff periods to wait before the next CCA attempt of CSMA/CA.
 *
 * When test modes are not enabled, the number of backoff periods is a random value in the range
 * from 0 to (2^BE - 1), where BE is the current backoff exponent, as defined in IEEE Std. 802.15.4.
 *
 * When test modes are enabled the returned value depends on the value returned by
 * @c nrf_802154_pib_test_mode_csmaca_backoff_get .
 *
 * @note Consecutive calls to this function may return different values.
 *
 * @param[in] be Backoff Exponent. Allowed range 0...8.
 *
 * @return Number of backoff periods to wait before the next CCA attempt
 */
uint8_t nrf_802154_csma_ca_backoff_periods_get(uint8_t be);

/**
 *@}
 **/

#endif // NRF_802154_CSMA_CA_BACKOFF_H__
