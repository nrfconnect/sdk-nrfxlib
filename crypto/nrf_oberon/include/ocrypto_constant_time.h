/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * Collection of timing-invariant implementations of basic functions.
 */

#ifndef OCRYPTO_CONSTANT_TIME_H
#define OCRYPTO_CONSTANT_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>


/**
 * Variable length comparison.
 *
 * **Example**
 * @include ocrypto_constant_time_equal.c
 *
 * @param x      Memory region to compare with @p y.
 * @param y      Memory region to compare with @p x.
 * @param length Number of bytes to compare, @p length > 0.
 *
 * @returns 1 If @p x and @p y point to equal memory regions.
 * @returns 0 Otherwise.
 */
int ocrypto_constant_time_equal(const void *x, const void *y, size_t length);

/**
 * Variable length compare to zero.
 *
 * **Example**
 * @include ocrypto_constant_time_is_zero.c
 *
 * @param x      Pointer to memory region that will be compared.
 * @param length Number of bytes to compare, @p length > 0.
 *
 * @returns 1 If @p x is equal to a zero memory region.
 * @returns 0 Otherwise.
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
 * @param x      Pointer to memory region that will filled with zero.
 * @param length Number of bytes to fill, @p length > 0.
 */
void ocrypto_constant_time_fill_zero(void *x, size_t length);

#ifdef __cplusplus
}
#endif

#endif
