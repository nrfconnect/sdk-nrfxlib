/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_nrf21540_gpio.h
 *
 * @defgroup mpsl_fem_nrf21540_gpio MPSL nRF21540 GPIO Front End Module Configuration
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_CONFIG_NRF21540_GPIO_H__
#define MPSL_FEM_CONFIG_NRF21540_GPIO_H__

#include <stdint.h>
#include <stdbool.h>
#include "mpsl_fem_config_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Configuration parameters for the Power Amplifier (PA) and Low Noise
 *  Amplifier (LNA) interface in the nRF21540 GPIO variant.
 */
typedef struct
{
    /** Configuration structure of the nRF21540 GPIO Front End Module. */
    struct
    {
        /** Time between the activation of the PA pin and the start of the radio transmission.
         *  Should be no bigger than Radio Ramp-Up time. */
        uint32_t pa_time_gap_us;
        /** Time between the activation of the LNA pin and the start of the radio reception.
         *  Should be no bigger than Radio Ramp-Up time. */
        uint32_t lna_time_gap_us;
        /** The time between activating the PDN pin and activating the PA/LNA pin. */
        uint32_t pdn_settle_us;
        /** The time between deactivating the PA/LNA pin and deactivating the PDN pin. */
        uint32_t trx_hold_us;
        /** Configurable PA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   pa_gain_db;
        /** Configurable LNA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   lna_gain_db;
    } fem_config;

    /** PA pin configuration. */
    mpsl_fem_gpiote_pin_config_t pa_pin_config;
    /** LNA pin configuration. */
    mpsl_fem_gpiote_pin_config_t lna_pin_config;
    /** PDN pin configuration. */
    mpsl_fem_gpiote_pin_config_t pdn_pin_config;

#if defined(NRF52_SERIES)
    /** Array of PPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      ppi_channels[3];
#else
    /** Array of DPPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      dppi_channels[4];
    /** Number of EGU instance for which @c egu_channels apply. */
    uint8_t                      egu_instance_no;
    /** Array of EGU channels (belonging to EGU instance number @c egu_instance_no) which
     *  need to be provided to Front End Module to operate. */
    uint8_t                      egu_channels[3];
#endif

} mpsl_fem_nrf21540_gpio_interface_config_t;

/** @brief Configures the PA and LNA device interface.
 *
 * This function sets device interface parameters for the PA/LNA module.
 * The module can then be used to control PA or LNA (or both) through the given interface and resources.
 *
 * The function also sets the PPI and GPIOTE channels to be configured for the PA/LNA interface.
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device.
 *
 * @retval   0              PA/LNA control successfully configured.
 * @retval   -NRF_EPERM     PA/LNA is not available.
 *
 */
int32_t mpsl_fem_nrf21540_gpio_interface_config_set(mpsl_fem_nrf21540_gpio_interface_config_t const * const p_config);

/**
 * @brief nRF21540 GPIO Front End Module Timings
 */

/** Time in microseconds when PA GPIO is activated before the radio is ready for transmission. */
#define MPSL_FEM_NRF21540_GPIO_DEFAULT_PA_TIME_IN_ADVANCE_US  13

/** Time in microseconds when LNA GPIO is activated before the radio is ready for reception. */
#define MPSL_FEM_NRF21540_GPIO_DEFAULT_LNA_TIME_IN_ADVANCE_US 13

/** The time between activating the PDN and asserting the RX_EN/TX_EN. */
#define MPSL_FEM_NRF21540_GPIO_DEFAULT_PDN_SETTLE_US          18

/** The time between deasserting the RX_EN/TX_EN and deactivating PDN. */
#define MPSL_FEM_NRF21540_GPIO_DEFAULT_TRX_HOLD_US            5

/**
 * @brief nRF21540 GPIO Front End Module Gains
 *
 * The provided gains are the default gains. The actual gain may depend on the
 * temperature and the configuration of the Front End Module.
 * See the Objective Product Specification for more details.
 */

/** Gain of the PA in dB. */
#define MPSL_FEM_NRF21540_GPIO_PA_DEFAULT_GAIN_DB   20

/** Gain of the LNA in dB. */
#define MPSL_FEM_NRF21540_GPIO_LNA_DEFAULT_GAIN_DB  20

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_NRF21540_GPIO_H__

/**@} */
