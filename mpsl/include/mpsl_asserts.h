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
	{ 0, 19322, "Clock ctrl release" },
	{ 0, 21677, "HFXO failure" },
	{ 0, 35544, "HFXO failure" },
	{ 0, 56433, "HFXO failure" },
	{ 0, 56944, "Clock ctrl request" },
	{ 0, 57816, "HFXO failure" },
};

#ifdef __cplusplus
}
#endif

#endif // MPSL_ASSERTS_H__

/**@} */
