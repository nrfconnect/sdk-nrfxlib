/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file nrf_modem_os.h
 * @brief OS specific definitions.
 *
 * @defgroup nrf_modem_os Modem library OS glue
 * @{
 */
#ifndef NRF_MODEM_OS_H__
#define NRF_MODEM_OS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Zero time-out. */
#define NRF_MODEM_OS_NO_WAIT 0
/** Infinite time-out. */
#define NRF_MODEM_OS_FOREVER -1

void nrf_modem_os_init(void);

/* @brief Put a thread to a sleep for a specific time or until an event occurs.
 *
 * @param[in]      context   A unique identifier assigned by the library to identify the context.
 * @param[in, out] p_timeout A pointer to the time-out value, in milliseconds. -1 for infinite
 *                           time-out. Contains the time-out value as input, remainig time to sleep
 *                           as output.
 *
 * @retval 0             If the procedure succeeded - it was interrupted by the RPC.
 * @retval NRF_ETIMEDOUT If a time-out condition occured.
 * @retval Other         Some other, OS specific error took place. The error code shall
 *                       belong to the nrf_errno error space.
 */
int32_t nrf_modem_os_timedwait(uint32_t context, int32_t *p_timeout);

void nrf_modem_os_errno_set(int errno_val);

void nrf_modem_os_application_irq_clear(void);

void nrf_modem_os_application_irq_set(void);

void nrf_modem_os_trace_irq_set(void);

void nrf_modem_os_trace_irq_clear(void);

int32_t nrf_modem_os_trace_put(const uint8_t *const p_buffer, uint32_t buf_len);

/* Need extern to not generate mock in testing. */
extern void nrf_modem_os_application_irq_handler(void);

/* Need extern to not generate mock in testing. */
extern void nrf_modem_os_trace_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_OS_H__ */

/** @} */
