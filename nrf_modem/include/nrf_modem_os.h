/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Zero time-out. */
#define NRF_MODEM_OS_NO_WAIT 0
/** Infinite time-out. */
#define NRF_MODEM_OS_FOREVER -1
/** Number of OS semaphores required. */
#define NRF_MODEM_OS_NUM_SEM_REQUIRED 3

enum log_level {
	NRF_MODEM_LOG_LEVEL_NONE,
	NRF_MODEM_LOG_LEVEL_ERR,
	NRF_MODEM_LOG_LEVEL_WRN,
	NRF_MODEM_LOG_LEVEL_INF,
	NRF_MODEM_LOG_LEVEL_DBG,
};

/**
 * @brief Initialize the glue layer.
 */
void nrf_modem_os_init(void);

/**
 * @brief Allocate a buffer on the TX area of shared memory.
 *
 * @param bytes Buffer size.
 * @return void*
 */
void *nrf_modem_os_shm_tx_alloc(size_t bytes);

/**
 * @brief Free a shared memory buffer in the TX area.
 *
 * @param mem Buffer to free.
 */
void nrf_modem_os_shm_tx_free(void *mem);

/**
 * @brief Allocate a buffer on the library heap.
 *
 * @param bytes Buffer size.
 * @return void*
 */
void *nrf_modem_os_alloc(size_t bytes);

/**
 * @brief Free a memory buffer in the library heap.
 *
 * @param mem Buffer to free.
 */
void nrf_modem_os_free(void *mem);

/**
 * @brief Busy wait.
 *
 * @param usec Microseconds to busy wait for.
 */
void nrf_modem_os_busywait(int32_t usec);

/* @brief Put a thread to a sleep for a specific time or until an event occurs.
 *
 * @param[in]      context   A unique identifier assigned by the library
 *                           to identify the context.
 * @param[in, out] timeout   Timeout in millisec or -1 for infinite timeout.
 *                           Contains the timeout value as input and the
 *                           remainig time to sleep as output.
 *
 * @return Zero if the thread is woken before the timeout expired,
 *         or NRF_ETIMEDOUT otherwise.
 */
int32_t nrf_modem_os_timedwait(uint32_t context, int32_t *timeout);

/**
 * @brief Set errno.
 *
 * @param errno_val errno to set.
 */
void nrf_modem_os_errno_set(int errno_val);

/**
 * @brief Check if executing in interrupt context.
 *
 * @return true If in interrupt context.
 * @return false If not in interrupt context.
 */
bool nrf_modem_os_is_in_isr(void);

/*
 * @brief Initialize a semaphore.
 *
 * The function shall allocate and initialize a semaphore and return its address
 * as an output. If an address of an already allocated semaphore is provided as
 * an input, the allocation part is skipped and the semaphore is only reinitialized.
 *
 * @param[inout] sem The address of the semaphore.
 * @param initial_count Initial semaphore count.
 * @param limit Maximum semaphore count.
 *
 * @return int Zero on success, or a negative value otherwise.
 */
int nrf_modem_os_sem_init(void **sem,
			  unsigned int initial_count,
			  unsigned int limit);

/**
 * @brief Give a semaphore.
 *
 * @param sem The semaphore.
 *
 * @note Can be called from an ISR.
 */
void nrf_modem_os_sem_give(void *sem);

/**
 * @brief Take a semaphore.
 *
 * @param sem The semaphore.
 * @param timeout Timeout in milliseconds.
 *		  @c NRF_MODEM_OS_FOREVER indicates infinite timeout.
 *		  @c NRF_MODEM_OS_NO_WAIT indicates no timeout.
 *
 * @note @a timeout shall be set to NRF_MODEM_OS_NO_WAIT if called from ISR.
 *
 * @return int Zero on success or NRF_ETIMEDOUT if the timeout expired.
 */
int nrf_modem_os_sem_take(void *sem, int timeout);

/**
 * @brief Set the application IRQ, @c NRF_MODEM_APPLICATION_IRQ.
 */
void nrf_modem_os_application_irq_set(void);

/**
 * @brief Clear the application IRQ, @c NRF_MODEM_APPLICATION_IRQ.
 */
void nrf_modem_os_application_irq_clear(void);

/**
 * @brief Set the Trace IRQ.
 */
void nrf_modem_os_trace_irq_set(void);

/**
 * @brief Clear the Trace IRQ.
 */
void nrf_modem_os_trace_irq_clear(void);

/**
 * @brief Receive trace data from the modem.
 *
 * The modem library calls this function to forward trace data to the application.
 *
 * The memory pointed to by @p data is not freed until
 * @ref nrf_modem_trace_processed_callback is called.
 * The application may thus defer the processing trace data as necessary.
 *
 * @param data Memory buffer containing the output trace data.
 * @param len  Memory buffer length.
 * @return int32_t 0 on success, an errno otherwise.
 */
int32_t nrf_modem_os_trace_put(const uint8_t *data, uint32_t len);

/**
 * @brief Prepare to log a transient string.
 *
 * The modem library calls this function on each string that it logs
 * that does not reside in read-only memory, to accommodate for
 * any eventual copy that the logging function may need
 * due to, for example, deferred logging.
 *
 * @param str The string to be logged.
 * @return const char* The pointer to the string to be passed to the logging functions.
 */
const char *nrf_modem_os_log_strdup(const char *str);

/**
 * @brief Generic logging procedure.
 *
 * @param level Log level.
 * @param fmt Format string.
 * @param ... Varargs.
 */
void nrf_modem_os_log(int level, const char *fmt, ...);

/**
 * @brief Logging procedure for dumping hex representation of object.
 *
 * @param level Log level.
 * @param str String to print in the log.
 * @param data Data whose hex representation we want to log.
 * @param len Length of the data to hex dump.
 */
void nrf_modem_os_logdump(int level, const char *str, const void *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_OS_H__ */

/** @} */
