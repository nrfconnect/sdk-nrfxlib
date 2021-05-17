/*
 * Copyright (c) 2017 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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
