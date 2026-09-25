/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MPSL_LOG_MSG_H__
#define MPSL_LOG_MSG_H__

#include <stdint.h>

struct mpsl_log_msg_entry {
	uint16_t id;
	uint8_t level;
	const char *fmt;
};

static const struct mpsl_log_msg_entry mpsl_log_msgs[] = {
#if MPSL_LOG_PRINT_LEVEL >= 3
	{ 0x0001u, 3, "MPSL initialized" },
#endif
#if MPSL_LOG_PRINT_LEVEL >= 3
	{ 0x0002u, 3, "MPSL de-initialized" },
#endif
	{ 0xffffu, 0, (const char *)0 },
};

#endif /* MPSL_LOG_MSG_H__ */
