/*
 * Copyright (c) 2020 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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
