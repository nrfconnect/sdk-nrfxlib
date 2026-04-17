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
 * The SoftDevice Controller SoC interface provides APIs for block encryption and random numbers.
 * While the SoftDevice Controller is enabled, the application should only use the provided APIs to
 * access NRF_ECB. Not doing so will lead to undefined behavior.
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

/** @brief Encrypt a block according to the specified parameters.
 *
 * The SoftDevice Controller will use NRF_ECB encrypt the block. The encryption type is 128-bit AES.
 *
 * @note The application may set the SEVONPEND bit in the SCR to 1 to make the SoftDevice Controller sleep
 *       while the ECB is running. The SEVONPEND bit must not be cleared (set to 0) from a function
 *       running in an interrupt priority level higher (lower numerical value) than the execution priority
 *       level this function was called from.
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
