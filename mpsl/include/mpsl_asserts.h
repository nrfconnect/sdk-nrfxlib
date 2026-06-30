/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_asserts.h
 *
 * @defgroup mpsl_asserts Assert messages for MPSL library
 *
 * @{
 */

#ifndef MPSL_ASSERTS_H__
#define MPSL_ASSERTS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Struct to store MPSL assert messages */
typedef struct {
	uint32_t file_id;
	uint32_t line;
	const char *assert_msg;
} mpsl_assert_entry_t;

/**@brief Array of documented MPSL asserts. */
static const mpsl_assert_entry_t mpsl_assert_messages[] = {
	{ 0, 0x4b7a, "Clock ctrl release" },
	{ 0, 0x54ad, "HFXO failure" },
	{ 0, 0x8ad8, "HFXO failure" },
	{ 0, 0xdc71, "HFXO failure" },
	{ 0, 0xde70, "Clock ctrl request" },
	{ 0, 0xe1d8, "HFXO failure" },
};

#ifdef __cplusplus
}
#endif

#endif // MPSL_ASSERTS_H__

/**@} */
