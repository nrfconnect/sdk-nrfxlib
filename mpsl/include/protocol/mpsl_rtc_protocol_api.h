/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef MPSL_RTC_PROTOCOL_API_H__
#define MPSL_RTC_PROTOCOL_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/* @brief Enable monotonic mode.
 *
 * @note Only applicable to platforms with RTC as GRTC is inherently monotonic.
 *
 * @param[in] ppi_channel user-provided ppi channel.
 */
void mpsl_rtc_monotonic_mode_enable(uint8_t ppi_channel);

/* @brief Whether MPSL is running in monotic mode. */
bool mpsl_rtc_monotonic_mode_is_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /* MPSL_RTC_PROTOCOL_API_H__ */
