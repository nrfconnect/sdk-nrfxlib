/*
 * Copyright (c) 2011 - 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @defgroup mpsl_temp_api MPSL Library Temp API
 * @{
 *
 * @brief APIs for the MPSL Temp module.
 *
 */

#ifndef MPSL_TEMP_H__
#define MPSL_TEMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Get the temperature measured on the chip
 *
 * This function will block until the temperature measurement is done.
 * It takes around 50 us from call to return.
 *
 * @retval Result of temperature measurement. Die temperature in 0.25 degrees Celsius.
 */
int32_t mpsl_temperature_get(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_TEMP_H__

/**@} */
