/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_nrf21540_gpio_protocol_api.h
 *
 * @defgroup mpsl_fem_nrf21540_gpio_protocol_api MPSL nRF21540 GPIO Front End Module Protocol API Extension.
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_NRF21540_GPIO_PROTOCOL_API_H__
#define MPSL_FEM_NRF21540_GPIO_PROTOCOL_API_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Bit number of @c mpsl_fem_pa_power_control_t holding MODE pin state. */
#define MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_BITNO    (0)

/** @brief Bit mask of @c mpsl_fem_pa_power_control_t for holding MODE pin state. */
#define MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_MSK      (1U << MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_BITNO)

/** @brief Bit field value of @c mpsl_fem_pa_power_control_t for controlling MODE pin state to use POUTA power setting. */
#define MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_POUTA    (0U << MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_BITNO)

/** @brief Bit field value of @c mpsl_fem_pa_power_control_t for controlling MODE pin state to use POUTB power setting. */
#define MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_POUTB    (1U << MPSL_FEM_NRF21540_GPIO_PA_POWER_CONTROL_MODE_BITNO)

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_NRF21540_GPIO_PROTOCOL_API_H__

/**@} */
