/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_nrf21540_gpio_spi.h
 *
 * @defgroup mpsl_fem_nrf21540_gpio_spi MPSL nRF21540 GPIO/SPI Front End Module Configuration
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_CONFIG_NRF21540_GPIO_SPI_H__
#define MPSL_FEM_CONFIG_NRF21540_GPIO_SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "mpsl_fem_config_common.h"
#include "mpsl_fem_config_nrf21540_common.h"
#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SPI interface. */
typedef struct
{
    NRF_SPIM_Type              * p_spim;        /**< Pointer to the SPI peripheral instance to be used. */
    mpsl_fem_pin_t               mosi_pin;      /**< MOSI pin. */
    mpsl_fem_pin_t               miso_pin;      /**< MISO pin. */
    mpsl_fem_pin_t               sck_pin;       /**< SCK pin. */
    mpsl_fem_gpiote_pin_config_t cs_pin_config; /**< CS pin. */
} mpsl_fem_spi_config_t;

/** @brief Configuration parameters for the Power Amplifier (PA), the Low Noise
 *  Amplifier (LNA) and the SPI interface in the nRF21540 GPIO/SPI variant.
 */
typedef struct
{
    /** nRF21540 parameters configuration. */
    mpsl_fem_nrf21540_config_t   fem_config;

    /** PA pin configuration. */
    mpsl_fem_gpiote_pin_config_t pa_pin_config;
    /** LNA pin configuration. */
    mpsl_fem_gpiote_pin_config_t lna_pin_config;
    /** PDN pin configuration. */
    mpsl_fem_gpiote_pin_config_t pdn_pin_config;
    /** MODE pin configuration. */
    mpsl_fem_gpio_pin_config_t   mode_pin_config;

    /** SPI interface configuration. */
    mpsl_fem_spi_config_t        spi_config;

#if defined(NRF52_SERIES)
    /** Array of PPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      ppi_channels[4];
#else
    /** Array of DPPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      dppi_channels[6];
    /** Number of EGU instance for which @c egu_channels apply. */
    uint8_t                      egu_instance_no;
    /** Array of EGU channels (belonging to EGU instance number @c egu_instance_no) which
     *  need to be provided to Front End Module to operate. */
    uint8_t                      egu_channels[5];
#endif

    /** Flag that indicates if PA gain should be changeable in runtime. If this flag is set,
     *  @c fem_config.pa_gains_db must contain correct values corresponding to POUTA and POUTB gains.
     *  Otherwise, PA gain is considered constant. In both cases there are SPI transfers performed
     *  to the nRF21540, so the SPI interface must be configured and operable. */
    bool                         pa_gain_runtime_control;

} mpsl_fem_nrf21540_gpio_spi_interface_config_t;

/** @brief Configures the PA and LNA device interface.
 *
 * This function sets device interface parameters for the PA/LNA module.
 * The module can then be used to control PA or LNA (or both) through the given interface and resources.
 *
 * The function also sets the PPI and GPIOTE channels to be configured for the PA/LNA interface as well
 * as SPI interface to be used.
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device.
 *
 * @retval   0              PA/LNA control successfully configured.
 * @retval   -NRF_EPERM     PA/LNA control configuration error.
 *
 */
int32_t mpsl_fem_nrf21540_gpio_spi_interface_config_set(mpsl_fem_nrf21540_gpio_spi_interface_config_t const * const p_config);

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_NRF21540_GPIO_SPI_H__

/**@} */
