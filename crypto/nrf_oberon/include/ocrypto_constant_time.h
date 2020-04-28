/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * @defgroup nrf_oberon Oberon cryptographic library
 * @{
 * @brief Highly optimized cryptographic algorithm implementation for Cortex-M0, Cortex-M4,
 * and Cortex-M33. Created by Oberon, under distribution license with Nordic Semiconductor ASA.
 * @}
 *
 * @defgroup nrf_oberon_constant_time Constant time APIs
 * @ingroup nrf_oberon
 * @{
 * @brief Timing-invariant functions to use with cryptography.
 *
 * Collection of timing-invariant implementations of basic functions.
 */

#ifndef OCRYPTO_CONSTANT_TIME_H
#define OCRYPTO_CONSTANT_TIME_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Variable length comparison.
 *
 * @param x      Memory region to compare with @p y.
 * @param y      Memory region to compare with @p x.
 * @param length Number of bytes to compare, @p length > 0.
 *
 * @retval 1 If @p x and @p y point to equal memory regions.
 * @retval 0 Otherwise.
 */
int ocrypto_constant_time_equal(const void *x, const void *y, size_t length);

/**
 * Variable length compare to zero.
 *
 * @param x      Pointer to memory region that will be compared.
 * @param length Number of bytes to compare, @p length > 0.
 *
 * @retval 1 If @p x is equal to a zero memory region.
 * @retval 0 Otherwise.
 */
int ocrypto_constant_time_is_zero(const void *x, size_t length);

/**
 * Variable length copy.
 *
 * @param x      Pointer to memory region to copy @p y to.
 * @param y      Pointer to memory region to copy to @p x.
 * @param length Number of bytes to copy, @p length > 0.
 */
void ocrypto_constant_time_copy(void *x, const void *y, size_t length);

/**
 * Variable length fill with zero.
 *
 * @param x      Pointer to memory region to be filled with zero.
 * @param length Number of bytes to fill, @p length > 0.
 */
void ocrypto_constant_time_fill_zero(void *x, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OCRYPTO_CONSTANT_TIME_H */

/** @} */
