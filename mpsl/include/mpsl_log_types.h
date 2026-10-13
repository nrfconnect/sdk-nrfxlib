/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_log_types.h
 *
 * @defgroup mpsl_log_types MPSL log types
 * @ingroup  mpsl
 *
 * Types shared by the generated log message tables of MPSL and the libraries
 * built on top of it.
 * @{
 */

#ifndef MPSL_LOG_TYPES_H__
#define MPSL_LOG_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @brief Entry of a library's generated log message table. */
typedef struct mpsl_log_msg_entry
{
  uint16_t id;     /**< Message id, unique within the library. */
  uint8_t level;   /**< Log level, matching Zephyr's LOG_LEVEL_* values. */
  const char *fmt; /**< printf-style format string, NULL for the end marker. */
} mpsl_log_msg_entry_t;

#ifdef __cplusplus
}
#endif

#endif // MPSL_LOG_TYPES_H__

/**@} */
