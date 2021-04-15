/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @addtogroup zb_error
 *
 * @brief Module for converting ZBOSS error code into a printable string.
 * @{
 */
#ifndef ZB_ERRPR_TO_STRING_H__
#define ZB_ERRPR_TO_STRING_H__

#include "zboss_api.h"
#include "zb_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function for getting a printable error string.
 *
 * @param code Error code to convert.
 *
 * @note This function cannot fail.
 *       For the function that may fail with error translation,
 *       see @ref zb_error_to_string_find.
 *
 * @return Pointer to the printable string.
 *         If the string is not found,
 *         it returns a simple string that says that the error is unknown.
 */
char const * zb_error_to_string_get(zb_ret_t code);

/**
 * @brief Function for finding a printable error string.
 *
 * This function gets the error string in the same way as @ref zb_error_to_string_get,
 * but if the string is not found, it returns NULL.
 *
 * @param code  Error code to convert.
 * @return      Pointer to the printable string.
 *              If the string is not found, NULL is returned.
 */
char const * zb_error_to_string_find(zb_ret_t code);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* ZB_STRERROR_H__ */
