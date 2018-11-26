/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**@file
 * Collection of timing-invariant implementations of basic functions.
 */

#ifndef OCC_CONSTANT_TIME_H
#define OCC_CONSTANT_TIME_H

#include <stddef.h>


/**
 * Variable length comparison.
 *
 * **Example**
 * @include occ_constant_time_equal.c
 *
 * @param x      Memory region to compare with @p y.
 * @param y      Memory region to compare with @p x.
 * @param length Number of bytes to compare, @p length > 0.
 *
 * @returns 1 If @p x and @p y point to equal memory regions.
 * @returns 0 Otherwise.
 */
int occ_constant_time_equal(const void *x, const void *y, size_t length);

/**
 * Variable length compare to zero.
 *
 * **Example**
 * @include occ_constant_time_is_zero.c
 *
 * @param x      Pointer to memory region that will be compared.
 * @param length Number of bytes to compare, @p length > 0.
 *
 * @returns 1 If @p x is equal to a zero memory regions.
 * @returns 0 Otherwise.
 */
int occ_constant_time_is_zero(const void *x, size_t length);

#endif
