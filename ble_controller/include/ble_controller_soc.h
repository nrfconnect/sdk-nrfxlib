/*
 * Copyright (c) Nordic Semiconductor ASA
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


/** @brief Flash command status. */
enum BLE_CONTROLLER_FLASH_CMD_STATUS
{
    BLE_CONTROLLER_FLASH_CMD_STATUS_SUCCESS = 0,
    BLE_CONTROLLER_FLASH_CMD_STATUS_TIMEOUT = 1,
};


/** @brief Flash command callback.
 *
 * The flash command callback will be called when a flash operation is completed.
 * It will be executed in the same execution priority as mpsl_low_priority_process.
 *
 * @param[in] status The status of the flash operation. @sa BLE_CONTROLLER_FLASH_CMD_STATUS.
 */
typedef void (*ble_controller_flash_callback_t)(uint32_t status);


/** @brief Write data to flash.
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
 *                         mpsl_low_priority_process.
 *
 * @retval 0                   Success
 * @retval -NRF_EINVAL         Either:
 *                                - Tried to write to a non existing flash address
 *                                - addr or p_src was not word aligned
 *                                - Size was 0, or higher than the maximum allowed size
 * @retval -NRF_EINPROGRESS    Previous flash operation is not yet completed
 */
int32_t ble_controller_flash_write(uint32_t addr,
                                   const void * p_src,
                                   uint32_t size,
                                   ble_controller_flash_callback_t on_complete);


/** @brief Erase a flash page.
 *
 * This asynchronous API will ensure that the flash operation will not interfere with radio activity.
 * The completion will be communicated to the application through the provided callback function.
 *
 * @param[in]  addr        Start address of the flash page to be erased.
 *                         If the address is not aligned with the start of flash page,
 *                         the page containing this address will be erased.
 * @param[in]  on_complete Function to be called when page is erased.
 *                         The callback will be executed in the context as
 *                         mpsl_low_priority_process.
 *
 * @retval 0                   Success
 * @retval -NRF_EINVAL         Tried to erase a non existing flash page.
 * @retval -NRF_EINPROGRESS    Previous flash operation is not yet completed
 */
int32_t ble_controller_flash_page_erase(uint32_t addr, ble_controller_flash_callback_t on_complete);


/** @brief Get random bytes from the random pool.
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


/** @brief Get random bytes from the random pool synchronously
 *
 * The BLE Controller will use NRF_RNG to obtain the numbers.
 * The function can be called from ISR context.
 *
 * @note This function works like @ref ble_controller_rand_vector_get(), except
 *       that instead of failing in the event of not enough numbers being
 *       available, it will block until it has filled the specified length with
 *       random number bytes. The time until completion will vary.
 *
 * @param[out]  p_dst   Pointer to a buffer in RAM for storing the bytes.
 * @param[in]   length  Number of random bytes to retrieve.
 */
void ble_controller_rand_vector_get_blocking(uint8_t * p_dst, uint16_t length);


/** @brief Encrypt a block according to the specified parameters.
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



#ifdef __cplusplus
}
#endif

/** @} end of ble_controller_soc */

#endif /* BLE_CONTROLLER_SOC_H__ */
