/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_simple_gpio.h
 *
 * @defgroup mpsl_fem_simple_gpio MPSL Simple GPIO Front End Module Configuration
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_CONFIG_SIMPLE_GPIO_H__
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_H__

#include <stdint.h>
#include <stdbool.h>
#include "mpsl_fem_config_common.h"
#include "nrfx.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NRF52_SERIES)
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_PPI_CHANNELS_COUNT  2
#else
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_DPPI_CHANNELS_COUNT 3
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_EGU_CHANNELS_COUNT  3
#endif

/** @brief Configuration parameters for the Front End Module Simple GPIO variant.
 *
 *  A Simple GPIO Front End Module may be used with all Front End Modules
 *  which use one wire for a Power Amplifier (PA) and one wire for a Linear
 *  Noise Amplifier (LNA).
 */
typedef struct
{
    /** Configration structure of the Simple GPIO Front End Module. */
    struct
    {
        /** Time between the activation of the PA pin and the start of the radio transmission.
         *  Should be no bigger than Radio Ramp-Up time. */
        uint32_t pa_time_gap_us;
        /** Time between the activation of the LNA pin and the start of the radio reception.
         *  Should be no bigger than Radio Ramp-Up time. */
        uint32_t lna_time_gap_us;
        /** Configurable PA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   pa_gain_db;
        /** Configurable LNA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   lna_gain_db;
    } fem_config;

    /** Power Amplifier pin configuration. */
    mpsl_fem_gpiote_pin_config_t pa_pin_config;
    /** Low Noise Amplifier pin configuration. */
    mpsl_fem_gpiote_pin_config_t lna_pin_config;

#if defined(NRF52_SERIES)
    /** Array of PPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      ppi_channels[MPSL_FEM_CONFIG_SIMPLE_GPIO_PPI_CHANNELS_COUNT];
#else
    /** Array of DPPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      dppi_channels[MPSL_FEM_CONFIG_SIMPLE_GPIO_DPPI_CHANNELS_COUNT];
    /** Number of EGU instance for which @c egu_channels apply. */
    uint8_t                      egu_instance_no;
    /** Array of EGU channels (belonging to EGU instance number @c egu_instance_no) which
     *  need to be provided to Front End Module to operate. */
    uint8_t                      egu_channels[MPSL_FEM_CONFIG_SIMPLE_GPIO_EGU_CHANNELS_COUNT];
#endif

} mpsl_fem_simple_gpio_interface_config_t;

/** @brief Configures the PA and LNA device interface.
 *
 * This function sets device interface parameters for the PA/LNA module.
 * The module can then be used to control PA or LNA (or both) through the given interface and resources.
 *
 * The function also sets the PPI and GPIOTE channels to be configured for the PA/LNA interface.
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device.
 *
 * @retval   0             PA/LNA control successfully configured.
 * @retval   -NRF_EPERM    PA/LNA is not available.
 *
 */
int32_t mpsl_fem_simple_gpio_interface_config_set(mpsl_fem_simple_gpio_interface_config_t const * const p_config);

/**
 * @brief Simple GPIO Front End Module Timings
 *
 * A Simple GPIO Front End Module may be used with all Front End Modules which
 * use one wire for PA and one wire for LNA.
 * The timing restrictions should be obtained from its corresponding datasheet.
 */

/** Time in microseconds when PA GPIO is activated before the radio is ready for transmission. */
#define MPSL_FEM_SIMPLE_GPIO_DEFAULT_PA_TIME_IN_ADVANCE_US  23

/** Time in microseconds when LNA GPIO is activated before the radio is ready for reception. */
#define MPSL_FEM_SIMPLE_GPIO_DEFAULT_LNA_TIME_IN_ADVANCE_US 5

/**
 * @brief Simple GPIO Front End Module Gains
 *
 * A Simple GPIO Front End Module may be used with all Front End Modules which
 * use one wire for PA and one wire for LNA.
 * The gains should be obtained from its corresponding datasheet.
 */

/** Gain of the PA in dB. */
#define MPSL_FEM_SIMPLE_GPIO_PA_DEFAULT_GAIN_DB   22

/** Gain of the LNA in dB. */
#define MPSL_FEM_SIMPLE_GPIO_LNA_DEFAULT_GAIN_DB  11

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_SIMPLE_GPIO_H__

/**@} */
