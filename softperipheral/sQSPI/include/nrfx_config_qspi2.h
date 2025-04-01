/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRFX_CONFIG_QSPI2_H__
#define NRFX_CONFIG_QSPI2_H__

/**
 * @brief NRFX_QSPI2_ENABLED
 *
 * Boolean. Accepted values: 0 and 1.
 */
#ifndef NRFX_QSPI2_ENABLED
#define NRFX_QSPI2_ENABLED 0
#endif

/**
 * @brief NRFX_QSPI2_MAX_NUM_DATA_LINES - Maximal number of data lines used by the QSPI peripheral.
 *
 * Integer value. Minimum: 1. Maximum: 8.
 */
#ifndef NRFX_QSPI2_MAX_NUM_DATA_LINES
#define NRFX_QSPI2_MAX_NUM_DATA_LINES 2
#endif

/**
 * @brief NRFX_QSPI2_TRANSFERS_PER_REQUEST - Maximal number of transfers that can be passed in an array for a single request. It is the maximal value in the xfer_count parameter.
 *
 * Integer value.
 */
#ifndef NRFX_QSPI2_TRANSFERS_PER_REQUEST
#define NRFX_QSPI2_TRANSFERS_PER_REQUEST 1
#endif

/**
 * @brief NRFX_QSPI2_DEFAULT_CONFIG_IRQ_PRIORITY
 *
 * Integer value. Minimum: 0. Maximum: 7.
 */
#ifndef NRFX_QSPI2_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_QSPI2_DEFAULT_CONFIG_IRQ_PRIORITY NRFX_DEFAULT_IRQ_PRIORITY
#endif

/**
 * @brief NRFX_QSPI2_CONFIG_LOG_ENABLED
 *
 * Boolean. Accepted values: 0 and 1.
 */
#ifndef NRFX_QSPI2_CONFIG_LOG_ENABLED
#define NRFX_QSPI2_CONFIG_LOG_ENABLED 0
#endif

/**
 * @brief NRFX_QSPI2_CONFIG_LOG_LEVEL
 *
 * Integer value.
 * Supported values:
 * - Off     = 0
 * - Error   = 1
 * - Warning = 2
 * - Info    = 3
 * - Debug   = 4
 */
#ifndef NRFX_QSPI2_CONFIG_LOG_LEVEL
#define NRFX_QSPI2_CONFIG_LOG_LEVEL 3
#endif

#endif // NRFX_CONFIG_QSPI2_H__
