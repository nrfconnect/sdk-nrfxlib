/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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

#ifndef NRF_802154_BSIM_UTILS_H__
#define NRF_802154_BSIM_UTILS_H__

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)

#include "nrf_802154_types.h"
#include "nrf_802154_utils.h"

typedef struct
{
    struct
    {
        uint32_t time_to_radio_address_us; ///< Time until RADIO->ADDRESS event in microseconds.
    } tx_started;

    struct
    {
        uint32_t time_to_radio_address_us; ///< Time until RADIO->ADDRESS event in microseconds.
    } tx_ack_started;

} nrf_802154_bsim_utils_core_hooks_adjustments_t;

/**
 * @brief Get adjustments for Tx started hook.
 */
void nrf_802154_bsim_utils_core_hooks_adjustments_get(
    nrf_802154_bsim_utils_core_hooks_adjustments_t * p_obj);

/**
 * @brief Set adjustments for Tx started hook.
 */
void nrf_802154_bsim_utils_core_hooks_adjustments_set(
    const nrf_802154_bsim_utils_core_hooks_adjustments_t * p_obj);

#endif /* defined(CONFIG_SOC_SERIES_BSIM_NRFXX) */

#endif /* NRF_802154_BSIM_UTILS_H__ */
