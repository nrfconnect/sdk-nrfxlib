/*
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
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
 * @file
 *   This file implements the nrf 802.15.4 HF Clock abstraction with nrfx driver.
 *
 * This implementation uses clock driver implementation from nrfx.
 */

#include "platform/nrf_802154_clock.h"

#include <stddef.h>

#include <compiler_abstraction.h>
#include <nrfx_clock.h>

static void clock_event_handler(nrfx_clock_evt_type_t event)
{
    if (event == NRFX_CLOCK_EVT_HFCLK_STARTED)
    {
        nrf_802154_clock_hfclk_ready();
    }

    if (event == NRFX_CLOCK_EVT_LFCLK_STARTED)
    {
        nrf_802154_clock_lfclk_ready();
    }
}

void nrf_802154_clock_init(void)
{
    nrfx_clock_init(&clock_event_handler);
    nrfx_clock_enable();
}

void nrf_802154_clock_deinit(void)
{
    nrfx_clock_disable();
    nrfx_clock_uninit();
}

void nrf_802154_clock_hfclk_start(void)
{
    nrfx_clock_start(NRF_CLOCK_DOMAIN_HFCLK);
}

void nrf_802154_clock_hfclk_stop(void)
{
    nrfx_clock_stop(NRF_CLOCK_DOMAIN_HFCLK);
}

bool nrf_802154_clock_hfclk_is_running(void)
{
    return nrfx_clock_is_running(NRF_CLOCK_DOMAIN_HFCLK, NULL);
}

void nrf_802154_clock_lfclk_start(void)
{
    nrfx_clock_start(NRF_CLOCK_DOMAIN_LFCLK);
}

void nrf_802154_clock_lfclk_stop(void)
{
    nrfx_clock_stop(NRF_CLOCK_DOMAIN_LFCLK);
}

bool nrf_802154_clock_lfclk_is_running(void)
{
    return nrfx_clock_is_running(NRF_CLOCK_DOMAIN_LFCLK, NULL);
}

__WEAK void nrf_802154_clock_hfclk_ready(void)
{
    // Intentionally empty.
}

__WEAK void nrf_802154_clock_lfclk_ready(void)
{
    // Intentionally empty.
}
