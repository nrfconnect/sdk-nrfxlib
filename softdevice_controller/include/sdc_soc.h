/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


/**
 * @file sdc_soc.h
 * @defgroup sdc_soc SoftDevice Controller SoC Interface
 * @ingroup sdc
 *
 * The SoftDevice Controller SoC interface provides APIs for flash access and block encryption
 * While the SoftDevice Controller is enabled, the application should only use
 * the provided APIs to access NRF_NVMC or NRF_ECB.
 * Not doing so will lead to undefined behavior.
 * @{
 */


#ifndef SDC_SOC_H__
#define SDC_SOC_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include "nrf_errno.h"


/** @brief Flash command status. */
enum sdc_soc_flash_cmd_status
{
    SDC_SOC_FLASH_CMD_STATUS_SUCCESS = 0,
    SDC_SOC_FLASH_CMD_STATUS_TIMEOUT = 1,
};


/** @brief Flash command callback.
 *
 * The flash command callback will be called when a flash operation is completed.
 * It will be executed in the same execution priority as @ref mpsl_low_priority_process.
 *
 * @param[in] status The status of the flash operation. @sa SDC_SOC_FLASH_CMD_STATUS.
 */
typedef void (*sdc_soc_flash_callback_t)(uint32_t status);


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
 *                         @ref mpsl_low_priority_process.
 *
 * @retval 0                   Success
 * @retval -NRF_EINVAL         Either:
 *                                - Tried to write to a non existing flash address
 *                                - addr or p_src was not word aligned
 *                                - Size was 0, or higher than the maximum allowed size
 * @retval -NRF_EINPROGRESS    Previous flash operation is not yet completed
 */
int32_t sdc_soc_flash_write_async(uint32_t addr,
                                  const void * p_src,
                                  uint32_t size,
                                  sdc_soc_flash_callback_t on_complete);


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
 *                         @ref mpsl_low_priority_process.
 *
 * @retval 0                   Success
 * @retval -NRF_EINVAL         Tried to erase a non existing flash page.
 * @retval -NRF_EINPROGRESS    Previous flash operation is not yet completed
 */
int32_t sdc_soc_flash_page_erase_async(uint32_t addr, sdc_soc_flash_callback_t on_complete);

/** @brief Encrypt a block according to the specified parameters.
 *
 * The SoftDevice Controller will use NRF_ECB encrypt the block. The encryption type is 128-bit AES.
 *
 * @note:
 *    - The application may set the SEVONPEND bit in the SCR to 1 to make the SoftDevice Controller sleep
 *      while the ECB is running. The SEVONPEND bit should only be cleared from the current execution
 *      context or a lower priority execution context.
 *
 * @param[in]  key        Encryption key
 * @param[in]  cleartext  Cleartext data
 * @param[out] ciphertext Encrypted data
 *
 * @retval 0 Success
 */
int32_t sdc_soc_ecb_block_encrypt(const uint8_t key[16],
                                  const uint8_t cleartext[16],
                                  uint8_t ciphertext[16]);

/**
 * @brief Functions used by the SoftDevice Controller to obtain random numbers.
 *
 * These functions are used for several different purposes,
 * including private address generation so they are expected to conform to:
 * BLUETOOTH CORE SPECIFICATION Version 5.2 | Vol 2, Part H, Section 2.
 */
typedef struct
{
    /** @brief Function used by the controller to obtain random numbers in a low priority context.
     *  The function will be executed in the same context as mpsl_low_priority_process and should not block.
     *
     *  @param[out]  p_buff  The destination buffer for the random numbers
     *  @param[in]   length  The requested number of random bytes and the maximum length of the destination buffer
     *  @retval              The number of bytes written to p_buff
     */
    uint8_t (*rand_prio_low_get)(uint8_t *p_buff, uint8_t length);

    /** @brief Function used by the controller to obtain random numbers in a high priority context.
     *  This function will be called in an ISR context and should not block.
     *
     *  @param[out]  p_buff  The destination buffer for the random numbers
     *  @param[in]   length  The requested number of random bytes and the maximum length of the destination buffer
     *  @retval              The number of bytes written to p_buff
     */
    uint8_t (*rand_prio_high_get)(uint8_t *p_buff, uint8_t length);

    /** @brief Function used by the controller to obtain random numbers.
     *  This function must block until length bytes of random numbers were written to p_buff.
     *  The function will be executed in the same context as mpsl_low_priority_process.
     *
     *  @param[out]  p_buff  The destination buffer for the random numbers
     *  @param[in]   length  The requested number of random bytes and the maximum length of the destination buffer
     */
    void (*rand_poll)(uint8_t *p_buff, uint8_t length);
} sdc_rand_source_t;

/** @brief Pass a source of randomness to the SoftDevice Controller
 *
 * The SoftDevice Controller will use the function pointers provided in this function call to get random numbers.
 *
 * @param[in] rand_source  A table of function pointers to obtain random numbers
 *
 * @retval 0 Success
 */
int32_t sdc_rand_source_register(const sdc_rand_source_t *rand_source);


#ifdef __cplusplus
}
#endif

/** @} end of sdc_soc */

#endif /* SDC_SOC_H__ */
