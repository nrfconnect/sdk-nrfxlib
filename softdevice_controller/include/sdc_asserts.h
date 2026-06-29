/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file sdc_asserts.h
 *
 * @defgroup sdc_asserts Assert messages for SoftDevice Controller
 *
 * @{
 */

#ifndef SDC_ASSERTS_H__
#define SDC_ASSERTS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Struct to store SDC assert messages */
typedef struct {
	uint32_t file_id;
	uint32_t line;
	const char *assert_msg;
} sdc_assert_entry_t;

/**@brief Array of documented SDC asserts. */
static const sdc_assert_entry_t sdc_assert_messages[] = {
	{ 0, 0x8057, "CS results encoding failed" },
};

#ifdef __cplusplus
}
#endif

#endif // SDC_ASSERTS_H__

/**@} */
