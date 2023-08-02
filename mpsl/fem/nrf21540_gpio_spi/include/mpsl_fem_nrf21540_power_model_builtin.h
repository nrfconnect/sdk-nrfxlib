/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_power_model.h
 *
 * The model describes how Front-End Module gain and SoC output power can be combined
 * to achieve requested output power on the antenna, given the current temperature,
 * voltage and operating frequency.
 *
 * @defgroup mpsl_fem_power_model MPSL Front-End Module power model APIs
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_NRF21540_POWER_MODEL_BUILTIN_H__
#define MPSL_FEM_NRF21540_POWER_MODEL_BUILTIN_H__

#include "mpsl_fem_power_model.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Update the built-in nRF21540 model using provided parameters.
 *
 * For given values of @p p_environment, this function populates the contents
 * of an internally stored, built-in MPSL nRF21540 power model.
 *
 * If the value of parameter passed to @ref mpsl_fem_power_model_set was different than the return
 * value of @ref mpsl_fem_nrf21540_power_model_builtin_get, this function does nothing.
 *
 * @note This function takes a significant amount of time to execute (approximately 5ms). Calling it
 *       from high priority contexts might lead to degraded performance or starving
 *       other contexts of the application.
 *
 * @param[in]  p_environment  Pointer to a structure that holds current external conditions
 *                            to recalculate the model for.
 */
void mpsl_fem_nrf21540_power_model_builtin_update(const mpsl_fem_external_conditions_t * p_environment);

/**
 * @brief Get the pointer to the built-in nRF21540 model.
 *
 * The pointer returned by this function can be used as a parameter to
 * @ref mpsl_fem_power_model_set that results in the built-in model being selected.
 *
 * @return  Pointer to the internal, built-in nRF21540 model.
 */
const mpsl_fem_power_model_t * mpsl_fem_nrf21540_power_model_builtin_get(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_POWER_MODEL_H__

/**@} */
