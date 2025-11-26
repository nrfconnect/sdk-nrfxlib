/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_CONFIG_SQSPI_H__
#define NRF_CONFIG_SQSPI_H__

/**
 * @brief NRF_SQSPI_ENABLED
 *
 * Boolean. Accepted values: 0 and 1.
 */
#ifndef NRF_SQSPI_ENABLED
#define NRF_SQSPI_ENABLED 0
#endif

/**
 * @brief NRF_SQSPI_MAX_NUM_DATA_LINES - Maximal number of data lines used by the QSPI peripheral.
 *
 * Integer value. Minimum: 1. Maximum: 8.
 */
#ifndef NRF_SQSPI_MAX_NUM_DATA_LINES
#define NRF_SQSPI_MAX_NUM_DATA_LINES 2
#endif

/**
 * @brief NRF_SQSPI_TRANSFERS_PER_REQUEST - Maximal number of transfers that can be passed in an array for a single request. It is the maximal value in the xfer_count parameter.
 *
 * Integer value.
 */
#ifndef NRF_SQSPI_TRANSFERS_PER_REQUEST
#define NRF_SQSPI_TRANSFERS_PER_REQUEST 1
#endif

/**
 * @brief NRF_SQSPI_DEFAULT_CONFIG_IRQ_PRIORITY
 *
 * Integer value. Minimum: 0. Maximum: 7.
 */
#ifndef NRF_SQSPI_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRF_SQSPI_DEFAULT_CONFIG_IRQ_PRIORITY NRFX_DEFAULT_IRQ_PRIORITY
#endif

/**
 * @brief NRF_SQSPI_CONFIG_LOG_ENABLED
 *
 * Boolean. Accepted values: 0 and 1.
 */
#ifndef NRF_SQSPI_CONFIG_LOG_ENABLED
#define NRF_SQSPI_CONFIG_LOG_ENABLED 0
#endif

/**
 * @brief NRF_SQSPI_CONFIG_LOG_LEVEL
 *
 * Integer value.
 * Supported values:
 * - Off     = 0
 * - Error   = 1
 * - Warning = 2
 * - Info    = 3
 * - Debug   = 4
 */
#ifndef NRF_SQSPI_CONFIG_LOG_LEVEL
#define NRF_SQSPI_CONFIG_LOG_LEVEL 3
#endif

#endif // NRF_CONFIG_SQSPI_H__
