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

/** @brief Type of an asynchronous write transfer callback function.
 *
 *  @param p_instance Pointer to object representing TWI instance.
 *                    This is a value of @c p_instance parameter passed to
 *                    @c p_xfer_write_async of @ref mpsl_fem_twi_if_t .
 *  @param res        Result of the TWI operation.
 *                    @c 0        - operation performed successfully.
 *                    @c -NRF_EIO - write error, slave device did not ACK.
 *                    other value - other error.
 *  @param p_context  Opaque context pointer passed to a function that started the transfer.
 */
typedef void (*mpsl_fem_twi_async_xfer_write_cb_t)(void * p_instance, int32_t res, void * p_context);

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

    /** Pointer to a function starting an asynchronous write transfer on a TWI bus.
     *
     *  This function may be called only when exclusive access to the TWI bus is granted.
     *  This must be ensured externally.
     *
     *  The function pointed by this pointer may be called from any interrupt priority, including
     *  those related to MPSL timeslot signalling.
     *
     *  @param p_instance       Pointer to object representing TWI instance.
     *                          The value of @c p_instance field is put here.
     *                          The meaning is fully up to the implementation of the function.
     *  @param slave_address    Address of the slave device on TWI bus.
     *  @param p_data           Pointer to a buffer containing data to be written.
     *                          In case of successful call the caller is responsible for holding
     *                          the buffer pointed by @p p_data until @p p_callback is called.
     *  @param data_length      Length of buffer pointed by p_data. The number of bytes to write.
     *  @param p_callback       Pointer to a callback function that will be called
     *                          when the TWI transfer is finished. This callback informs about the
     *                          result of the operation. It is called from an ISR of the TWI peripheral.
     *  @param p_context        Pointer to a context that the @p p_callback will be notified with.
     * 
     *  @retval 0           In case of success. The TWI transfer is being performed.
     *                      It will end with a call to @p p_callback
     *  @retval other       Failure of TWI transfer initialization.
     *                      The @p p_callback will not be called.
     */
    int32_t (*p_xfer_write_async)(void                               * p_instance,
                                  uint8_t                              slave_address,
                                  const uint8_t                      * p_data,
                                  uint8_t                              data_length,
                                  mpsl_fem_twi_async_xfer_write_cb_t   p_callback,
                                  void                               * p_context);

    /** Returns the time in microseconds needed to perform asynchronous TWI write transfer.
     *
     *  This function allows to get the worst case time needed to perform @c p_xfer_write_async until
     *  the completion of th operation. The time returned is measured since the call to function
     *  pointed by @ref p_xfer_write_async until the callback provided to that function is called.
     *
     *  @note The implementation of @c p_xfer_write_async_time_get takes into account the current
     *  TWI bus speed and any software overhead required by the TWI driver. The implementation
     *  may return a bit bigger value taking into account aby safety margin, but must not return
     *  too small value.
     *
     *  @param p_instance   Pointer to object representing TWI instance.
     *                      The value of @c p_instance field is put here.
     *                      The meaning is fully up to the implementation of the function.
     *  @param data_length  Number of bytes to write through the TWI bus. Does not include
     *                      the slave address field which is always present.
     *  @return Time that is needed to perform corresponding write transfer invoked by @c p_xfer_write_async .
     */
    uint32_t (*p_xfer_write_async_time_get)(void    * p_instance,
                                            uint8_t   data_length);
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
