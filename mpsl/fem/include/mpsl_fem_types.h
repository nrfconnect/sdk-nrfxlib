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

/**@brief Support deprecated parts of MPSL Power Model API. */
#define _MPSL_FEM_POWER_MODEL_FEM_GAIN_API_DEPRECATED

/** Type for PA power control to be applied to Front-End Module, depending on its type.
 * 
 *  The meaning of this type is FEM type-specific. Applicable macros are defined within
 *  FEM type-specific header files found in @c protocol/mpsl_fem_[type]_protocol_api.h .
 *  Macros are named according to pattern @c MPSL_FEM_[TYPE]_PA_POWER_CONTROL_* .
 *  When these macros define just a subset of available bits, treat other bits as
 *  reserved and set them to 0.
 */
typedef uint8_t mpsl_fem_pa_power_control_t;

/**
 * @brief Structure that provides transmit control to be applied to a Front-End Module.
 * 
 * When this structure is filled as an outcome of @ref mpsl_fem_tx_power_split (recommended) 
 * you don't need to know what is inside the structure.
 * 
 * When you need to construct this struct on your own (not recommended), please note that
 * the content of this structure depends fully on the Front-End Module type in use.
 * Please refer to FEM type-specific header files found in @c protocol/mpsl_fem_[type]_protocol_api.h .
 */
typedef struct
{
    /** Gain in dB. */
    int8_t  gain_db;

    /** Control parameter to be applied to Front-End Module, depending on its type.
     * 
     *  The meaning of this field depends on FEM type. Applicable macros are defined within
     *  FEM type-specific header files found in @c protocol/mpsl_fem_[type]_protocol_api.h .
     *  Macros are named according to pattern @c MPSL_FEM_[TYPE]_PA_POWER_CONTROL_* .
     *  When these macros define just a subset of available bits, treat other bits as
     *  reserved and set them to 0.
     */
    uint8_t private_setting;
} mpsl_fem_gain_t;

/** Type binding gain of the FEM with PA power control value for calibration purposes. */
typedef struct
{
    /** Gain in dB. */
    int8_t gain_db;

#ifdef _MPSL_FEM_POWER_MODEL_FEM_GAIN_API_DEPRECATED
    /** Equivalent of mpsl_fem_gain_t::private_setting. 
     * 
     *  This field is deprecated and will be removed along with the macro 
     *  @ref _MPSL_FEM_POWER_MODEL_FEM_GAIN_API_DEPRECATED. Please use the field
     *  @c fem_pa_power_control instead. It has exactly the same content and meaning.
     */
    uint8_t private_setting;
#endif

    /** Control parameter to be applied to Front-End Module, depending on its type.
     * 
     *  The meaning of this field depends on FEM type. Applicable macros are defined within
     *  FEM type-specific header files found in @c protocol/mpsl_fem_[type]_protocol_api.h .
     *  Macros are named according to pattern @c MPSL_FEM_[TYPE]_PA_POWER_CONTROL_* .
     *  When these macros define just a subset of available bits, treat other bits as
     *  reserved and set them to 0.
     */
    mpsl_fem_pa_power_control_t fem_pa_power_control;
} mpsl_fem_calibration_point_t;

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
        mpsl_fem_calibration_point_t pouta; ///< Gain in dB and relevant MODE pin state for precalibrated POUTA setting.
        mpsl_fem_calibration_point_t poutb; ///< Gain in dB and relevant MODE pin state for precalibrated POUTB setting.
    } nrf21540_gpio;

    /** Calibration data for nRF21540 GPIO/SPI. */
    struct
    {
        mpsl_fem_calibration_point_t pouta; ///< Gain in dB and relevant register value for precalibrated POUTA setting.
        mpsl_fem_calibration_point_t poutb; ///< Gain in dB and relevant register value for precalibrated POUTB setting.
    } nrf21540_gpio_spi;

} mpsl_fem_calibration_data_t;

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_TYPES_H__

/**@} */
