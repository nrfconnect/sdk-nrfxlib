/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_trace.h
 *
 * @defgroup nrf_modem_trace Modem library trace interface
 * @{
 * @brief API for handling modem traces.
 */
#ifndef NRF_MODEM_TRACE_H__
#define NRF_MODEM_TRACE_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Trace data
 *  Structure to store trace data pointers and their respective sizes.
 */
struct nrf_modem_trace_data {
	const void *data;
	size_t len;
};

/**
 * @brief Get trace data
 *
 * The application may call this function to receive trace data from the modem.
 * The trace fragments must be processed in order from the start of the array.
 * Processed data must be freed by calling @c nrf_modem_trace_processed.
 *
 * @param[out] frags On success, @c *frags is an array of trace data fragments.
 * @param[out] n_frags On success, @c *n_frags is the number of fragments in the @c *frags array.
 * @param[in] timeout Time to wait in milliseconds for trace data to become available or one of the
 *            special values NRF_MODEM_OS_FOREVER or NRF_MODEM_OS_NO_WAIT.
 *
 * @retval 0 on success.
 * @retval -NRF_EFAULT If @p frags or @p nfrags is NULL.
 * @retval -NRF_EINPROGRESS Trace is already being processed by the application.
 * @retval -NRF_ENODATA No more trace data is available until the modem is restarted.
 * @retval -NRF_ESHUTDOWN Modem was shut down.
 * @retval -NRF_EAGAIN If the request timed out.
 * @retval -NRF_ENOTSUP Modem is not configured for trace output.
 */
int nrf_modem_trace_get(struct nrf_modem_trace_data **frags, size_t *n_frags, int timeout);

/**
 * @brief Notify the Modem library that the application has completed processing of trace data.
 *
 * The application must call this function to let the Modem library free the trace memory.
 * It is the application's responsibility to call this function with the total size of all trace
 * fragments received. If the data is processed in chunks, the function can be called with the size
 * that is processed since last call.
 *
 * @param len Length of trace data that is processed.
 * @retval Zero on success.
 * @retval -NRF_EINVAL @p len is too large to be a valid trace length.
 */
int nrf_modem_trace_processed(size_t len);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_TRACE_H__ */
/** @} */
