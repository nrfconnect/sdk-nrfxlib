/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_nrf2220_protocol_api.h
 *
 * @defgroup mpsl_fem_nrf2220_protocol_api MPSL nRF2220 Front End Module Protocol API Extension.
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_NRF2220_PROTOCOL_API_H__
#define MPSL_FEM_NRF2220_PROTOCOL_API_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Bit mask of @c mpsl_fem_pa_power_control_t for transmission involving transition to "transmit" state.
 * If set the "transmit" state is used during transmission.
 * If cleared the "bypass" state is used during transmission.
 */
#define MPSL_FEM_NRF2220_PA_POWER_CONTROL_USE_TRANSMIT      0x01U

/** @brief Notifies the nRF2220 software module about the temperature change of the nRF2220 device.
 *
 * This function performs necessary calculation of registers to be written into nRF2220 device
 * based on passed temperature. If it turns out as a result of the calculation that no register
 * of nRF2220 needs an update, then this function returns @c false and no further processing
 * is needed. If it turns out as a result of the calculation that some registers need a update,
 * then this function reurns @c true. In this case you should call either
 * @ref mpsl_fem_nrf2220_temperature_changed_update_request or @ref mpsl_fem_nrf2220_temperature_changed_update_now
 * to transfer calculated registers into nrf2220. Which one to choose depends on
 * protocol in use.
 * You should call the function @ref mpsl_fem_nrf2220_temperature_changed_update_request function
 * if the protocol in use relies on the MPSL scheduler. 
 * You should call the @ref mpsl_fem_nrf2220_temperature_changed_update_now function
 * if the protocol in use does not rely on the MPSL scheduler.
 *
 * @note The function is not re-entrant. Should be called from one context only.
 *
 * @param[in] temperature       Temperature of the nRF2220 device in degrees Celsius.
 *
 * @retval false    Temperature change processed, no followup operation required.
 * @retval true     Temperature change processed, required following call to
 *                  @ref mpsl_fem_nrf2220_temperature_changed_update_request
 *                  or @ref mpsl_fem_nrf2220_temperature_changed_update_now .
 */
bool mpsl_fem_nrf2220_temperature_changed(int8_t temperature);

/** @brief Type of callback function called as a result of @ref mpsl_fem_nrf2220_temperature_update_request
 *
 * @param res  The result of the operation of updating temperature-related registers.
 *             @c 0 in case of success. Other value indicate an error.
 */
typedef void (*mpsl_fem_nrf2220_temperature_update_callback_t)(int32_t res);

/** @brief Requests an update of temperature-related registers of nRF2220.
 *
 * This function is to be called when @ref mpsl_fem_nrf2220_temperature_changed returned @c true
 * and you use a protocol that relies on the MPSL scheduler.
 * This function uses asynchronous TWI writes provided through @ref mpsl_fem_twi_if_t interface.
 *
 * Prior calling this function you must:
 * - Acquire an exclusive access to the TWI peripheral.
 * - Ensure that the TWI IRQ priority is high enough so that any interrupts from the TWI do not
 *   get delayed. Consider boosting the TWI IRQ priority for the time of the operation.
 *
 * The function starts a procedure consisting of:
 * - request an MPSL timeslot
 * - when an MPSL timeslot is granted, it switches the nRF2220 into bypass state
 * - perform I2C writes to the nRF2220 to registers calculated by the recent
 *   call to @ref mpsl_fem_nrf2220_temperature_changed
 * - switch the nRF2220 back into standby state
 * - release the MPSL timeslot
 *
 * The end of the operation is notified by a call to the callback function
 * provided by @p p_callback pointer. The callback is called from low priority signalling context.
 * 
 * After calling this function with non-success result or after the @p p_callback is called you must:
 * - Restore previous TWI IRQ priority if has been boosted for the time of the operation.
 * - Release the exclusive access to the TWI peripheral.
 *
 * @sa @ref mpsl_fem_nrf2220_temperature_changed_update_now, @ref mpsl_fem_nrf2220_temperature_changed
 * 
 * @param[in] p_callback  The callback function that will be called to signal the end of the operation.
 *                        The callback is called from low priority signalling context.
 *
 * @retval 0             In case of success, the @p p_callback. The @p p_callback will be called.
 * @retval -NRF_EINVAL   Nothing to write, the @p p_callback will not be called.
 */
int32_t mpsl_fem_nrf2220_temperature_changed_update_request(mpsl_fem_nrf2220_temperature_update_callback_t p_callback);

/** @brief Performs an immediate update of temperature-related registers of the nRF2220.
 *
 * This function is to be called when the @ref mpsl_fem_nrf2220_temperature_changed returned @c true
 * and you use a protocol that does not rely on the MPSL scheduler.
 *
 * The call is synchronous.
 */
int32_t mpsl_fem_nrf2220_temperature_changed_update_now(void);


#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_NRF2220_PROTOCOL_API_H__

/**@} */
