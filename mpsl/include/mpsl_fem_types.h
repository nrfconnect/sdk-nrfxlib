/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_types.h
 *
 * @defgroup mpsl_fem_types MPSL datatypes for handling Front-End Modules
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_TYPES_H__
#define MPSL_FEM_TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure that provides complete information about Front-End Module gain.
 */
typedef struct
{
    /** Gain in dB. */
    int8_t  gain_db;
    /** Private setting to be applied to Front-End Module, depending on its type.
     *  For nRF21540 GPIO/SPI, this is a register value.
     *  For nRF21540 GPIO, this is MODE pin value. */
    uint8_t private_setting;
} mpsl_fem_gain_t;

/**
 * @brief Union that provides data for calibrating Front-End Module gain.
 */
typedef union
{
    /** Calibration data for simple GPIO. */
    struct
    {
        int8_t gain; ///< PA gain in dB.
    } simple_gpio;

    /** Calibration data for nRF21540 GPIO. */
    struct
    {
        mpsl_fem_gain_t pouta; ///< Gain in dB and relevant MODE pin state for precalibrated POUTA setting.
        mpsl_fem_gain_t poutb; ///< Gain in dB and relevant MODE pin state for precalibrated POUTB setting.
    } nrf21540_gpio;

    /** Calibration data for nRF21540 GPIO/SPI. */
    struct
    {
        mpsl_fem_gain_t pouta; ///< Gain in dB and relevant register value for precalibrated POUTA setting.
        mpsl_fem_gain_t poutb; ///< Gain in dB and relevant register value for precalibrated POUTB setting.
    } nrf21540_gpio_spi;

} mpsl_fem_calibration_data_t;

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_TYPES_H__

/**@} */
