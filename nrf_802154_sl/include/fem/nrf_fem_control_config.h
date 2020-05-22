/*
 * Copyright (c) 2020 Nordic Semiconductor ASA. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor ASA.
 * The use, copying, transfer or disclosure of such information is prohibited except by
 * express written agreement with Nordic Semiconductor ASA.
 */

#ifndef NRF_FEM_CONTROL_CONFIG_H_
#define NRF_FEM_CONTROL_CONFIG_H_

#include "nrf_fem_config.h"

#include <stdint.h>
#include <stdbool.h>

#include "drivers/nrfx_common.h"
#include "drivers/nrfx_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @section Configuration
 */

/**
 * @brief Configures the PA and LNA device interface.
 *
 * This function sets device interface parameters for the PA/LNA module.
 * The module can then be used to control a power amplifier or a low noise amplifier (or both) through the given interface and resources.
 *
 * The function also sets the PPI and GPIOTE channels to be configured for the PA/LNA interface.
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device.
 *
 * @retval   ::NRFX_SUCCESS                 PA/LNA control successfully configured.
 * @retval   ::NRFX_ERROR_NOT_SUPPORTED     PA/LNA is not available.
 *
 */
int32_t nrf_fem_interface_configuration_set(nrf_fem_interface_config_t const * const p_config);

/**
 * @brief Retrieves the configuration of PA and LNA device interface.
 *
 * This function gets device interface parameters for the PA/LNA module.
 * The module can then be used to control a power amplifier or a low noise amplifier (or both) through the given interface and resources.
 *
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device to be populated.
 *
 * @retval   ::NRFX_SUCCESS                 PA/LNA control successfully configured.
 * @retval   ::NRFX_ERROR_NOT_SUPPORTED     PA/LNA is not available.
 *
 */
int32_t nrf_fem_interface_configuration_get(nrf_fem_interface_config_t * p_config);

#ifdef __cplusplus
}
#endif

#endif /* NRF_FEM_CONTROL_CONFIG_H_ */
