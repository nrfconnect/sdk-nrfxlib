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
#ifdef CONFIG_ZIGBEE_ERROR_TO_STRING_ENABLED
#include "zb_error_to_string.h"

/** Counts number of elements inside the array
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

/**
 * @brief Macro for adding an entity to the description array.
 *
 * Macro that helps to create a single entity in the description array.
 */
#define ZB_ERROR_STRING(mnemonic) {.code = -mnemonic, .name = #mnemonic}

/**
 * @brief Array entity element that describes an error.
 */
typedef struct
{
	zb_ret_t     code; /**< Error code. */
	char const * name; /**< Descriptive name (the same as the internal error mnemonic). */
} zb_strerror_desc_t;

/**
 * @brief Unknown error code.
 *
 * The constant string used by @ref zb_strerror_get when the error description was not found.
 */
static char const m_unknown_str[] = "Unknown error code";

/**
 * @brief Array with error codes.
 *
 * Array that describes error codes.
 *
 * @note It is required for this array to have error codes placed in ascending order.
 *       This condition is checked in automatic unit test before the release.
 */
static zb_strerror_desc_t const zb_strerror_array[] =
{
	ZB_ERROR_STRING(RET_OK),
	ZB_ERROR_STRING(RET_ERROR),
	ZB_ERROR_STRING(RET_BLOCKED),
	ZB_ERROR_STRING(RET_EXIT),
	ZB_ERROR_STRING(RET_BUSY),
	ZB_ERROR_STRING(RET_EOF),
	ZB_ERROR_STRING(RET_OUT_OF_RANGE),
	ZB_ERROR_STRING(RET_EMPTY),
	ZB_ERROR_STRING(RET_CANCELLED),

	ZB_ERROR_STRING(RET_INVALID_PARAMETER_1),
	ZB_ERROR_STRING(RET_INVALID_PARAMETER_2),
	ZB_ERROR_STRING(RET_INVALID_PARAMETER_3),
	ZB_ERROR_STRING(RET_INVALID_PARAMETER_4),
	ZB_ERROR_STRING(RET_INVALID_PARAMETER_6),
	ZB_ERROR_STRING(RET_PENDING),
	ZB_ERROR_STRING(RET_NO_MEMORY),
	ZB_ERROR_STRING(RET_INVALID_PARAMETER),
	ZB_ERROR_STRING(RET_OPERATION_FAILED),
	ZB_ERROR_STRING(RET_BUFFER_TOO_SMALL),
	ZB_ERROR_STRING(RET_END_OF_LIST),
	ZB_ERROR_STRING(RET_ALREADY_EXISTS),
	ZB_ERROR_STRING(RET_NOT_FOUND),
	ZB_ERROR_STRING(RET_OVERFLOW),
	ZB_ERROR_STRING(RET_TIMEOUT),
	ZB_ERROR_STRING(RET_NOT_IMPLEMENTED),
	ZB_ERROR_STRING(RET_NO_RESOURCES),
	ZB_ERROR_STRING(RET_UNINITIALIZED),
	ZB_ERROR_STRING(RET_INVALID_STATE),
	ZB_ERROR_STRING(RET_CONNECTION_FAILED),
	ZB_ERROR_STRING(RET_CONNECTION_LOST),
	ZB_ERROR_STRING(RET_UNAUTHORIZED),
	ZB_ERROR_STRING(RET_CONFLICT),
	ZB_ERROR_STRING(RET_INVALID_FORMAT),
	ZB_ERROR_STRING(RET_NO_MATCH),
	ZB_ERROR_STRING(RET_PROTOCOL_ERROR),
	ZB_ERROR_STRING(RET_VERSION),
	ZB_ERROR_STRING(RET_FILE_NOT_FOUND),
	ZB_ERROR_STRING(RET_CONVERSION_ERROR),
	ZB_ERROR_STRING(RET_FILE_CORRUPTED),
	ZB_ERROR_STRING(RET_PAGE_NOT_FOUND),
	ZB_ERROR_STRING(RET_ILLEGAL_REQUEST),
	ZB_ERROR_STRING(RET_INVALID_GROUP),
	ZB_ERROR_STRING(RET_TABLE_FULL),
	ZB_ERROR_STRING(RET_IGNORE),
	ZB_ERROR_STRING(RET_AGAIN),
	ZB_ERROR_STRING(RET_DEVICE_NOT_FOUND),
	ZB_ERROR_STRING(RET_OBSOLETE)
};

char const * zb_error_to_string_get(zb_ret_t code)
{
	char const * p_ret = zb_error_to_string_find(code);
	return (p_ret == NULL) ? m_unknown_str : p_ret;
}

char const * zb_error_to_string_find(zb_ret_t code)
{
	zb_strerror_desc_t const * p_start;
	zb_strerror_desc_t const * p_end;
	p_start = zb_strerror_array;
	p_end   = zb_strerror_array + ARRAY_SIZE(zb_strerror_array);

	while (p_start < p_end)	{
		zb_strerror_desc_t const * p_mid = p_start + ((p_end - p_start) / 2);
		zb_ret_t mid_c = p_mid->code;
		if (mid_c > code) {
			p_end = p_mid;
		} else if (mid_c < code) {
			p_start = p_mid + 1;
		} else {
			return p_mid->name;
		}
	}
	return NULL;
}

#endif /* CONFIG_ZIGBEE_ERROR_TO_STRING_ENABLED */

/** @} */
