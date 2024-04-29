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

/**
 * @brief Functions used by the SoftDevice Controller to obtain random numbers.
 *
 * These functions are used for security-related purposes, so they must conform to:
 * BLUETOOTH CORE SPECIFICATION Version 5.2 | Vol 2, Part H, Section 2.
 */
typedef struct
{
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
