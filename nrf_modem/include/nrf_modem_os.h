/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_os.h
 *
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
#define NRF_MODEM_OS_NUM_SEM_REQUIRED 7
/** Number of OS mutexes required. */
#define NRF_MODEM_OS_NUM_MUTEX_REQUIRED 1

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
 * @brief Deinitialize the glue layer.
 *
 * When shutdown is called, all pending calls to @c nrf_modem_os_timedwait exit
 * and return -NRF_ESHUTDOWN.
 */
void nrf_modem_os_shutdown(void);

/**
 * @brief Allocate a buffer on the TX area of shared memory.
 *
 * @param bytes Buffer size.
 * @returns A pointer to the allocated memory buffer or @c NULL if allocation failed.
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
 * @returns A pointer to the allocated memory buffer or @c NULL if allocation failed.
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

/**
 * @brief Put a thread to sleep for a specific time or until an event occurs.
 *
 * The thread is woken if the timeout has expired and by a @c nrf_modem_event_notify call if
 * * The @p context equals the event context.
 * * The @p context is zero (thread wakes on all calls to event notify).
 * * The event context is zero (wake all sleeping threads).
 *
 * The @p timeout parameter is updated to contain the amount of time left to sleep.
 *
 * @param context Library context.
 * @param[in, out] timeout Timeout in milliseconds, or @c NRF_MODEM_OS_FOREVER for no timeout.
 *			   Specifies the timeout value on input and the remaining
 *			   time to sleep on output.
 *
 * @retval 0 The thread is woken before the timeout expired.
 * @retval -NRF_EAGAIN The timeout expired.
 * @retval -NRF_ESHUTDOWN Modem is not initialized, or was shut down.
 */
int32_t nrf_modem_os_timedwait(uint32_t context, int32_t *timeout);

/**
 * @brief Notify the application that an event has occurred.
 *
 * This function wakes up a thread sleeping in @c nrf_modem_os_timedwait if:
 * * The thread context equals @p context.
 * * The thread context is zero (thread wakes on all calls to event notify).
 * * The @p context is zero (wake all sleeping threads).
 *
 * @param context Library context.
 */
void nrf_modem_os_event_notify(uint32_t context);

/**
 * @brief Put a thread to sleep for a specific amount of time.
 *
 * @param timeout Timeout in millisec.
 * @returns 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_sleep(uint32_t timeout);

/**
 * @brief Set errno.
 *
 * @param errno_val errno to set.
 */
void nrf_modem_os_errno_set(int errno_val);

/**
 * @brief Check if executing in interrupt context.
 *
 * @retval true If in interrupt context.
 * @retval false If not in interrupt context.
 */
bool nrf_modem_os_is_in_isr(void);

/**
 * @brief Initialize a semaphore.
 *
 * The function initializes a semaphore and returns its address as an output.
 * If an address of an already allocated semaphore is provided as
 * an input, the allocation part is skipped and the semaphore is only reinitialized.
 *
 * @param[in, out] sem The address of the semaphore.
 * @param initial_count Initial semaphore count.
 * @param limit Maximum semaphore count.
 *
 * @returns 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_sem_init(void **sem, unsigned int initial_count, unsigned int limit);

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
 * @note @a timeout must be set to NRF_MODEM_OS_NO_WAIT if called from ISR.
 *
 * @retval 0 on success.
 * @retval -NRF_EAGAIN If the semaphore could not be taken.
 */
int nrf_modem_os_sem_take(void *sem, int timeout);

/**
 * @brief Get a semaphore's count.
 *
 * @param sem The semaphore.
 * @returns Current semaphore count.
 */
unsigned int nrf_modem_os_sem_count_get(void *sem);

/**
 * @brief Initialize a mutex.
 *
 * The function shall allocate and initialize a mutex and return its address
 * as an output. If an address of an already allocated mutex is provided as
 * an input, the allocation part is skipped and the mutex is only reinitialized.
 *
 * @param[in, out] mutex The address of the mutex.
 *
 * @returns 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_mutex_init(void **mutex);

/**
 * @brief Lock a mutex.
 *
 * @param mutex The mutex.
 * @param timeout Timeout in milliseconds.
 *		  @c NRF_MODEM_OS_FOREVER indicates infinite timeout.
 *		  @c NRF_MODEM_OS_NO_WAIT indicates no timeout.
 *
 * @retval 0 on success.
 * @retval -NRF_EAGAIN If the mutex could not be taken.
 */
int nrf_modem_os_mutex_lock(void *mutex, int timeout);

/**
 * @brief Unlock a mutex.
 *
 * @param mutex The mutex.
 *
 * @retval 0 on success.
 * @retval -NRF_EPERM if the current thread does not own the mutex.
 * @retval -NRF_EINVAL if the mutex is not locked.
 */
int nrf_modem_os_mutex_unlock(void *mutex);

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
