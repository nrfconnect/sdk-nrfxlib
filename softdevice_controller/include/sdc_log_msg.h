/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SDC_LOG_MSG_H__
#define SDC_LOG_MSG_H__

#include <stdint.h>

struct sdc_log_msg_entry {
	uint16_t id;
	uint8_t level;
	const char *fmt;
};

static const struct sdc_log_msg_entry sdc_log_msgs[] = {
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0001u, 1, "Fault handler is NULL" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0002u, 1, "MPSL is not initialized" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0003u, 1, "LFCLK is not accurate enough" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 3
	{ 0x0004u, 3, "SDC initialized" },
#endif
	{ 0xffffu, 0, (const char *)0 },
};

#endif /* SDC_LOG_MSG_H__ */
