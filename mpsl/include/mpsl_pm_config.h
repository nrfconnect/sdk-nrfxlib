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

/** @brief Initialize MPSL Power Management. */
void mpsl_pm_init(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_PM_CONFIG_H__

/**@} */
