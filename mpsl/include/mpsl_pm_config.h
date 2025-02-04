/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_pm_config.h
 *
 * @defgroup mpsl_pm MPSL API for configuration of a System Power Manager external to the MPSL.
 * @ingroup  mpsl
 *
 * @{
 */

#ifndef MPSL_PM_CONFIG_H__
#define MPSL_PM_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Initialize MPSL external Power Management integration.
 *
 * Enables integration with external power management system. After calling
 * this function, the external power management system must service requests
 * for low latency and event registration according to the API defined
 * in @ref mpsl_pm.h file.
 */
void mpsl_pm_init(void);

/** @brief Uninitialize MPSL external Power Management integration.
 *
 * Disables integration with external power management system.
 */
void mpsl_pm_uninit(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_PM_CONFIG_H__

/**@} */
