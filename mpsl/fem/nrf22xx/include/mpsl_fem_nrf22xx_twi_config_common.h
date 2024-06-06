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
#include <nrf_errno.h>

#include <mpsl_fem_config_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Interface to a TWI driver. */
typedef struct
{
    /** Determines if TWI interface is enabled.
     *  
     *  If false, then TWI interface is not used, other fields of the struct are ignored.
     *  If true, then TWI interface is used.
     */
    bool    enabled;

    /** TWI slave address of the FEM device. */
    uint8_t slave_address;

    /** Opaque pointer to the TWI object instance.
     * 
     *  This pointer is passed to all function calls to the functions pointed by @ref mpsl_fem_twi_if_t
     *  interface. The meaning of this pointer is determined by implementation of functions assigned
     *  to function pointers in this interface.
     */
    void  * p_instance;

    /** Pointer to a function performing read transfer from TWI device.
     *
     *  @param p_instance       Pointer to object representing TWI instance.
     *                          The value of @c p_instance field is put here.
     *                          The meaning is fully up to the implementation of the function.
     *  @param slave_address    Address of the slave device on TWI bus.
     *  @param internal_address Internal address within TWI device at which the read starts.
     *  @param p_data           Pointer to a buffer where read data will be stored.
     *  @param data_length      Length of buffer pointed by p_data. The number of bytes to read.
     * 
     *  @retval 0           In case of success.
     *  @retval -NRF_EIO    In case of NACK on TWI bus.
     *  @retval other       Other error.
     * 
     */
    int32_t (*p_xfer_read)(void * p_instance, uint8_t slave_address, uint8_t internal_address, uint8_t * p_data, uint8_t data_length);

    /** Pointer to a function performing write transfer to TWI device.
     *
     *  @param p_instance       Pointer to object representing TWI instance.
     *                          The value of @c p_instance field is put here.
     *                          The meaning is fully up to the implementation of the function.
     *  @param slave_address    Address of the slave device on TWI bus.
     *  @param internal_address Internal address within TWI device at which write starts.
     *  @param p_data           Pointer to a buffer containing values to be written.
     *  @param data_length      Length of buffer pointed by p_data. The number of bytes to write.
     * 
     *  @retval 0           In case of success.
     *  @retval -NRF_EIO    In case of NACK on TWI bus.
     *  @retval other       Other error.
    */
    int32_t (*p_xfer_write)(void * p_instance, uint8_t slave_address, uint8_t internal_address, const uint8_t * p_data, uint8_t data_length);
} mpsl_fem_twi_if_t;

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
