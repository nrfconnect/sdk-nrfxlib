/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_SEMMC_CONFIG_H__
#define NRF_SEMMC_CONFIG_H__

/**
 * @brief NRF_SEMMC_ENABLED
 *
 * Boolean. Accepted values: 0 and 1.
 */
#ifndef NRF_SEMMC_ENABLED
#define NRF_SEMMC_ENABLED 0
#endif

/**
 * @brief NRF_SEMMC_RESPONSE_SIZE
 *
 * Integer value. Word size of eMMC response
 */
#ifndef NRF_SEMMC_RESPONSE_SIZE
#define NRF_SEMMC_RESPONSE_SIZE 4
#endif // NRF_SEMMC_T_SIZE

/**
 * @brief NRF_SEMMC_TRANSFERS_PER_REQUEST - Maximal number of transfers that can be passed in an array for a single request. It is the maximal value in the cmd_count parameter.
 *
 * Integer value.
 */
#ifndef NRF_SEMMC_TRANSFERS_PER_REQUEST
#define NRF_SEMMC_TRANSFERS_PER_REQUEST 1
#endif

#endif // NRF_SEMMC_CONFIG_H__
