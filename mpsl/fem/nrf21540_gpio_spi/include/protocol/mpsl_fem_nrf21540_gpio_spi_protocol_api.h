/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_nrf21540_gpio_spi_protocol_api.h
 *
 * @defgroup mpsl_fem_nrf21540_gpio_spi_protocol_api MPSL nRF21540 GPIO/SPI Front End Module Protocol API Extension.
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_NRF21540_GPIO_SPI_PROTOCOL_API_H__
#define MPSL_FEM_NRF21540_GPIO_SPI_PROTOCOL_API_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Bit number of @ref mpsl_fem_pa_power_control_t::private_setting starting a field holding TX_GAIN of CONFREG0 register. */
#define MPSL_FEM_NRF21540_GPIO_SPI_PA_POWER_CONTROL_CONFREG0_TXGAIN_BITNO    (0)

/** @brief Bit mask of @ref mpsl_fem_pa_power_control_t::private_setting for holding TX_GAIN of CONFREG0 register. */
#define MPSL_FEM_NRF21540_GPIO_SPI_PA_POWER_CONTROL_CONFREG0_TXGAIN_MSK      (0x1FU << MPSL_FEM_NRF21540_GPIO_SPI_PA_POWER_CONTROL_CONFREG0_TXGAIN_BITNO)

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_NRF21540_GPIO_SPI_PROTOCOL_API_H__

/**@} */
