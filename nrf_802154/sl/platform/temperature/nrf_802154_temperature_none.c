/*
 * Copyright (c) 2017 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 *   This file implements the thermometer abstraction stub that does not use thermometer.
 *
 * This thermometer abstraction will cause RSSI, LQI, ED, CCA threshold errors up to 3 dBm when
 * temperature of device differs more than 5 C from 20 C.
 *
 */

#include "platform/nrf_802154_temperature.h"

#include <stdint.h>

#define DEFAULT_TEMPERATURE 20 ///< Default temperature reported by this driver stub [C].

void nrf_802154_temperature_init(void)
{
    // Intentionally empty
}

void nrf_802154_temperature_deinit(void)
{
    // Intentionally empty
}

int8_t nrf_802154_temperature_get(void)
{
    return DEFAULT_TEMPERATURE;
}
