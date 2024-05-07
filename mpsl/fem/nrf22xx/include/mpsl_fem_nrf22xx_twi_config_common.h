/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_nrf22xx_twi_config_common.h
 *
 * @defgroup mpsl_fem_twi_config_common MPSL Front End Module common TWI configuration
 * @ingroup  mpsl_fem
 *
 * The MPSL Front End Module common TWI configuration defines structures common for every Front End
 * Module that features a TWI interface.
 * @{
 */

#ifndef MPSL_FEM_TWI_CONFIG_COMMON_H__
#define MPSL_FEM_TWI_CONFIG_COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include <nrf.h>

#include <mpsl_fem_config_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Configuration of the TWI interface that can be used to communicate with Front End Module.
 */
typedef struct
{
    /** Pointer to the TWI peripheral instance to be used. */
    NRF_TWIM_Type * p_twim;
    /** SCL pin. */
    mpsl_fem_pin_t  scl;
    /** SDA pin. */
    mpsl_fem_pin_t  sda;
    /** Clock frequency in Hz. */
    uint32_t        freq_hz;
    /** TWI slave address. */
    uint8_t         address;
} mpsl_fem_twi_config_t;

/** @brief Parameters of a TWI register to be written.
 */
typedef struct
{
    uint8_t addr;    /**< Internal register address. */
    uint8_t val;     /**< Value to be written.*/
} mpsl_fem_twi_reg_val_t;

/** @brief Map of TWI registers to be written.
 */
typedef struct
{
    mpsl_fem_twi_reg_val_t * p_regs;   /**< Pointer to array that holds TWI registers. */
    uint16_t                 nb_regs;  /**< Number of entries in the array. */
} mpsl_fem_twi_reg_map_t;

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_TWI_CONFIG_COMMON_H__

/**@} */
