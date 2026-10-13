/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_temp.h
 *
 * @defgroup mpsl_temp MPSL Temperature Sensor interface
 * @ingroup  mpsl
 *
 * The MPSL temperature sensor interface provides APIs for measuring the chip temperature.
 * @{
 */

#ifndef MPSL_TEMP_H__
#define MPSL_TEMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Invalid / not-yet-sampled temperature (@ref mpsl_temperature_last_get). */
#define MPSL_TEMPERATURE_INVALID INT32_MIN

/** @brief Get the temperature measured on the chip
 *
 * This function will block until the temperature measurement is done.
 * It takes around 50 us from call to return.
 *
 * The result is stored for @ref mpsl_temperature_last_get.
 *
 * @note This function must be executed in the same execution priority as @ref mpsl_low_priority_process.
 *
 * @return Result of temperature measurement. Die temperature in 0.25 degrees Celsius.
 */
int32_t mpsl_temperature_get(void);

/** @brief Get the die temperature from the last @ref mpsl_temperature_get call.
 *
 * @return Last sample in 0.25 °C units, or @ref MPSL_TEMPERATURE_INVALID if none yet.
 */
int32_t mpsl_temperature_last_get(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_TEMP_H__

/**@} */
