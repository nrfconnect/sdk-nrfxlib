/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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
 *   This file implements the nrf 802.15.4 HF Clock abstraction with MPSL API.
 *
 * This implementation uses clock driver implementation from MPSL.
 */

#include "platform/nrf_802154_clock.h"

#include <assert.h>
#include <stdint.h>

#include "mpsl.h"
#include "mpsl_clock.h"

static bool m_clock_requested = false;

void nrf_802154_clock_init(void)
{
    // Intentionally empty. Clock initialization is performed when MPSL is initialized.
}

void nrf_802154_clock_deinit(void)
{
    if (m_clock_requested)
    {
        nrf_802154_clock_hfclk_stop();
    }
}

void nrf_802154_clock_hfclk_start(void)
{
    (void)mpsl_clock_hfclk_request(nrf_802154_clock_hfclk_ready);

    m_clock_requested = true;
}

void nrf_802154_clock_hfclk_stop(void)
{
    (void)mpsl_clock_hfclk_release();

    m_clock_requested = false;
}

bool nrf_802154_clock_hfclk_is_running(void)
{
    uint32_t hfclk_is_running = mpsl_clock_hfclk_is_running(&hfclk_is_running);

    return (bool)hfclk_is_running;
}

void nrf_802154_clock_lfclk_start(void)
{
    // Low frequency clock is started when MPSL is initialized.
    bool is_initialized = mpsl_is_initialized();

    assert(is_initialized);

    (void)is_initialized;

    nrf_802154_clock_lfclk_ready();
}

void nrf_802154_clock_lfclk_stop(void)
{
    // Intentionally empty. MPSL does not support stopping low frequency clock.
}

bool nrf_802154_clock_lfclk_is_running(void)
{
    // Low frequency clock is started when MPSL is initialized and it is kept running as long as MPSL itself.
    return mpsl_is_initialized();
}

__WEAK void nrf_802154_clock_hfclk_ready(void)
{
    // Intentionally empty.
}

__WEAK void nrf_802154_clock_lfclk_ready(void)
{
    // Intentionally empty.
}
