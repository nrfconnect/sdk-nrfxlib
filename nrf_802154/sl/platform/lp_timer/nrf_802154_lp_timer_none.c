/*
 * Copyright (c) 2020 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 *   This file implements the nrf 802.15.4 timer abstraction in case timer is not used.
 *
 * This timer abstraction should be used only when none of driver features that use timer is enabled.
 *
 */

#include "platform/nrf_802154_lp_timer.h"

void nrf_802154_lp_timer_init(void)
{
    // Intentionally empty
}

void nrf_802154_lp_timer_deinit(void)
{
    // Intentionally empty
}

void nrf_802154_lp_timer_critical_section_enter(void)
{
    // Intentionally empty
}

void nrf_802154_lp_timer_critical_section_exit(void)
{
    // Intentionally empty
}

// Other functions from LP timer API are intentionally not implemented to detect build configuration
// problems compile-time.
