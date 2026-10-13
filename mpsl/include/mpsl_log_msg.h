/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MPSL_LOG_MSG_H__
#define MPSL_LOG_MSG_H__

#include "mpsl_log_types.h"

#define MPSL_LOG_MSGS_LIB_ID   0u
#define MPSL_LOG_MSGS_LIB_NAME "MPSL"

static const mpsl_log_msg_entry_t mpsl_log_msgs[] = {
#if MPSL_LOG_PRINT_LEVEL >= 3
	{ 0x0001u, 3, "MPSL initialized" },
#endif
#if MPSL_LOG_PRINT_LEVEL >= 3
	{ 0x0002u, 3, "MPSL de-initialized" },
#endif
	{ 0xffffu, 0, (const char *)0 },
};

#endif /* MPSL_LOG_MSG_H__ */
