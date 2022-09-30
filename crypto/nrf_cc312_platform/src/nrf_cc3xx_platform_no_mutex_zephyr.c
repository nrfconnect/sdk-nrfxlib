/**
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <zephyr/kernel.h>

BUILD_ASSERT(!IS_ENABLED(CONFIG_MULTITHREADING),
	"This file is intended for single-threading, but multi-threading is enabled. "
	"Please check your config build configuration!");

/** @brief Function to initialize the nrf_cc3xx_platform mutex APIs
 */
void nrf_cc3xx_platform_mutex_init(void)
{
	// No thread-safe mutexes are required
}
