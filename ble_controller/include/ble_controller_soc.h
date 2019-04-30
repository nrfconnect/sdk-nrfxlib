/*
 * Copyright (c) 2018 - 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */


/**
 * @file ble_controller_soc.h
 * @defgroup ble_controller_soc BLE Controller SoC Interface
 * @ingroup ble_controller
 *
 * The BLE Controller SoC interface provides APIs for flash access, random number generation,
 * and block encryption. While the BLE Controller is enabled, the application should only use
 * the provided APIs to access NRF_NVMC, NRF_RNG, or NRF_ECB.
 * Not doing so will lead to undefined behavior.
 * @{
 */


#ifndef BLE_CONTROLLER_SOC_H__
#define BLE_CONTROLLER_SOC_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include "nrf_errno.h"


/**@brief Flash command status. */
enum BLE_CONTROLLER_FLASH_CMD_STATUS
{
    BLE_CONTROLLER_FLASH_CMD_STATUS_SUCCESS = 0,
    BLE_CONTROLLER_FLASH_CMD_STATUS_TIMEOUT = 1,
};


/**@brief Flash command callback.
 *
 * The flash command callback will be called when a flash operation is completed.
 * It will be executed in the same execution priority as @ref ble_controller_low_prio_tasks_process.
 *
 * @param[in] status The status of the flash operation. @sa BLE_CONTROLLER_FLASH_CMD_STATUS.
 */
typedef void (*ble_controller_flash_callback_t)(uint32_t status);


/**@brief High frequency clock callback.
 *
 * This callback will be called when a the high frequency clock is started.
 * It will be executed in the same execution priority as @ref ble_controller_low_prio_tasks_process.
 */
typedef void (*ble_controller_hf_clock_callback_t)(void);


/**@brief Write data to flash
 *
 * This asynchronous API will ensure that the flash operation will not interfere with radio activity.
 * The completion will be communicated to the application through the provided callback function.
 *
 * @note The data in the p_src buffer should not be modified before the completion callback has
 *       been executed.
 *
 * @param[in]  addr        Flash location to be written.
 * @param[in]  p_src       Pointer to buffer with data to be written.
 * @param[in]  size        Number of 32-bit words to write. Maximum size is the number of words in one
 *                         flash page. See the device's Product Specification for details.
 * @param[in]  on_complete Callback to be called when flash is written.
 *                         The callback will be executed in the context as
 *                         @ref ble_controller_low_prio_tasks_process.
 *
 * @retval 0                   Success
 * @retval - ::NRF_EINVAL      Either:
 *                                - Tried to write to a non existing flash address
 *                                - addr or p_src was not word aligned
 *                                - Size was 0, or higher than the maximum allowed size
 * @retval - ::NRF_EINPROGRESS Previous flash operation is not yet completed
 */
int32_t ble_controller_flash_write(uint32_t addr,
                                   const void * p_src,
                                   uint32_t size,
                                   ble_controller_flash_callback_t on_complete);


/**@brief Erase a flash page
 *
 * This asynchronous API will ensure that the flash operation will not interfere with radio activity.
 * The completion will be communicated to the application through the provided callback function.
 *
 * @param[in]  addr        Start address of the flash page to be erased.
 *                         If the address is not aligned with the start of flash page,
 *                         the page containing this address will be erased.
 * @param[in]  on_complete Function to be called when page is erased.
 *                         The callback will be executed in the context as
 *                         @ref ble_controller_low_prio_tasks_process.
 *
 * @retval 0                   Success
 * @retval - ::NRF_EINVAL      Tried to erase a non existing flash page.
 * @retval - ::NRF_EINPROGRESS Previous flash operation is not yet completed
 */
int32_t ble_controller_flash_page_erase(uint32_t addr, ble_controller_flash_callback_t on_complete);


/**@brief Get random bytes from the random pool.
 *
 * The BLE Controller will use NRF_RNG to obtain the numbers.
 * The function can be called from ISR context.
 *
 * @note If the function is repeatedly called from an execution priority higher
 * than the RNG IRQ priority, no new bytes will be returned.
 *
 * @param[out]  p_dst   Pointer to a buffer in RAM for storing the bytes.
 * @param[in]   length  Number of random bytes to retrieve.
 *
 * @retval Number of bytes obtained
 */
uint32_t ble_controller_rand_vector_get(uint8_t * p_dst, uint16_t length);


/**@brief Get random bytes from the random pool synchronously
 *
 * The BLE Controller will use NRF_RNG to obtain the numbers.
 * The function can be called from ISR context.
 *
 * @note  This function works like @ref ble_controller_rand_vector_get, except that
 *        instead of failing in the event of not enough numbers being available, it
 *        will block until it has filled the specified length with random number
 *        bytes. The time until completion will vary.
 *
 * @param[out]  p_dst   Pointer to a buffer in RAM for storing the bytes.
 * @param[in]   length  Number of random bytes to retrieve.
 */
void ble_controller_rand_vector_get_blocking(uint8_t * p_dst, uint16_t length);


/**@brief Encrypts a block according to the specified parameters.
 *
 * The BLE Controller will use NRF_ECB encrypt the block. The encryption type is 128-bit AES.
 *
 * @note:
 *    - The application may set the SEVONPEND bit in the SCR to 1 to make the BLE Controller sleep
 *      while the ECB is running. The SEVONPEND bit should only be cleared from the current execution
 *      context or a lower priority execution context.
 *
 * @param[in]  key        Encryption key
 * @param[in]  cleartext  Cleartext data
 * @param[out] ciphertext Encrypted data
 *
 * @retval 0 Success
 */
int32_t ble_controller_ecb_block_encrypt(const uint8_t key[16],
                                         const uint8_t cleartext[16],
                                         uint8_t ciphertext[16]);


/**@brief Request the high frequency crystal oscillator.
 *
 * This API will start the high frequency crystal oscillator if it is not already running.
 * The high frequency clock will be kept running until @ref ble_controller_hf_clock_release is called.
 *
 * @param[in] on_started Function to be called when the high frequency clock is started.
 *                       The callback will be executed in the context as
 *                       @ref ble_controller_low_prio_tasks_process.
 *
 * @retval 0 Success
 */
int32_t ble_controller_hf_clock_request(ble_controller_hf_clock_callback_t on_started);


/**@brief Releases the high frequency crystal oscillator.
 *
 * This API will stop the high frequency crystal oscillator. The BLE Controller may continue to
 * use the high frequency clock for BLE activity. However, the BLE Controller will automatically
 * turn it off when it is no longer needed.
 *
 * @retval 0 Success
 */
int32_t ble_controller_hf_clock_release(void);


/**@brief Checks if the high frequency crystal oscillator is running.
 *
 * @param[out] p_is_running Set to true if the high frequency crystal oscillator is running.
 *
 * @retval 0 Success
 */
int32_t ble_controller_hf_clock_is_running(bool * p_is_running);


/**@brief Get the temperature measured on the chip
 *
 * This function will block until the temperature measurement is done.
 *
 * @retval Result of temperature measurement. Die temperature in 0.25 degrees Celsius.
 */
int32_t ble_controller_temp_get(void);


#ifdef __cplusplus
}
#endif

/** @} end of ble_controller_soc */

#endif /* BLE_CONTROLLER_SOC_H__ */
