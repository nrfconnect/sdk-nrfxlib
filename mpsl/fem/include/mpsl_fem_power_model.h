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

#ifndef MPSL_FEM_POWER_MODEL_H__
#define MPSL_FEM_POWER_MODEL_H__

#include <stdint.h>
#include <stdbool.h>

#include "mpsl_fem_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Decomposition of a single value of output power on the antenna.
 */
typedef struct
{
    /** SoC power in dBm. When returned by a model, this field is expected to contain value
     *  supported by the SoC that can safely be applied directly to RADIO registers. */
    int8_t          soc_pwr;

    /** PA power control to be applied to the FEM.
     *
     * This value is FEM type-dependent. The produced value is to be passed to the
     * call to @ref mpsl_fem_pa_power_control_set.
     */
    mpsl_fem_pa_power_control_t fem_pa_power_control;

    /** Achieved power on the antenna in dBm. */
    int8_t          achieved_pwr;
} mpsl_fem_power_model_output_t;

/**
 * @brief Pointer to function to be used by MPSL to retrieve the model's output.
 *
 * @warning This function must execute in maximum 2 microseconds in the worst case scenario.
 *          Failing to meet this requirement will lead to undefined behavior of the protocol stacks.
 *
 * @param[in]   requested_power   Requested power on the antenna in dBm.
 * @param[in]   freq_mhz          Frequency in MHz.
 * @param[out]  p_output          Output of the model. For details, refer to
 *                                @ref mpsl_fem_power_model_output_t.
 * @param[in]   tx_power_ceiling  Flag to get ceiling or floor of requested RADIO TX power level.
 */
typedef void (*mpsl_fem_power_model_output_fetch_t)(
    int8_t                          requested_power,
    uint16_t                        freq_mhz,
    mpsl_fem_power_model_output_t * p_output,
    bool                            tx_power_ceiling);

/**
 * @brief Pointer to function to be used by MPSL to initialize the model.
 *
 * When MPSL configures interface of the Front-End Module selected by the application, it retrieves
 * and calculates data that might be useful to the power model. When it finishes, MPSL calls this
 * function. As a result, this function can be used to initialize the power model and to utilize
 * the data acquired by MPSL.
 *
 * @note This function is called before the main application starts.
 *
 * @param[in]  p_calibration_data  Pointer to calibration data relevant to the Front-End Module
 *                                 selected by the application. Lifetime of the object behind this
 *                                 pointer is limited to the duration of this function.
 */
typedef void (*mpsl_fem_power_model_init_t)(const mpsl_fem_calibration_data_t * p_calibration_data);

/**
 * @brief Front-End Module power model.
 *
 * This structure contains data that the application must pass to MPSL in case
 * it provides a custom model for power decomposition between SoC and nRF21540.
 */
typedef struct
{
    /** Pointer to function to be used to retrieve the model's output.
     *  This function can be called from high priority interrupts. It can preempt update of model
     *  parameters, which is expected to be executed in lower priority contexts. */
    mpsl_fem_power_model_output_fetch_t fetch;
    /** Pointer to function to be used to initialize the model.
     *  If NULL, no function will be called when the Front-End Module driver is initialized.
     *  It is assumed that it is safe to call the fetch method without any additional actions. */
    mpsl_fem_power_model_init_t        init;
} mpsl_fem_power_model_t;

/**
 * @brief External conditions that affect Front-End Module's power model.
 */
typedef struct
{
    /** Temperature in degrees Celsius. */
    int8_t  temperature;
    /** Voltage in units of 0.1 Volt. */
    uint8_t voltage;
} mpsl_fem_external_conditions_t;

/**
 * @brief Sets Front-End Module power model.
 *
 * This function sets up the control flow for handling the power model.
 *
 * If nRF21540 Front-End Module is used, the value returned by
 * @ref mpsl_fem_nrf21540_power_model_builtin_get can be used as @p p_model. In that case,
 * the built-in model provided by MPSL is used and the application is responsible for triggering
 * the model's recalculation through @ref mpsl_fem_nrf21540_power_model_builtin_update.
 * If the model is not updated as external conditions change, it will output inaccurate data.
 *
 * In all other cases, if @p p_model is not NULL, custom model provided by the application
 * is used. In that case, the application is responsible for maintaining the model out of
 * scope of MPSL. Calling @ref mpsl_fem_nrf21540_power_model_builtin_update is then pointless.
 *
 * If @p p_model equal NULL is passed to the function, the call is treated as a request to not use
 * any model.
 *
 * @note This function must be called before @ref mpsl_fem_nrf21540_gpio_spi_interface_config_set,
 *       @ref mpsl_fem_nrf21540_gpio_interface_config_set or @ref mpsl_fem_simple_gpio_interface_config_set,
 *       depending on which one is called to select appropriate Front-End Module driver.
 *
 * @param[in]  p_model  Pointer to the model to be used by MPSL.
 *
 * @retval -NRF_EINVAL  Provided pointer is invalid.
 * @retval 0            The model has been initialized successfully.
 */
int32_t mpsl_fem_power_model_set(const mpsl_fem_power_model_t * p_model);

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_POWER_MODEL_H__

/**@} */
