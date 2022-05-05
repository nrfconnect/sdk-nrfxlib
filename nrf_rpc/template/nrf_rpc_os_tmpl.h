/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef NRF_RPC_OS_H_
#define NRF_RPC_OS_H_

/*
 * THIS IS A TEMPLATE FILE.
 * It should be copied to a suitable location within the host environment into
 * which Remote Procedure serialization is integrated, and the following macros
 * should be provided with appropriate implementations.
 */

/**
 * @defgroup nrf_rpc_os OS-dependent functionality for nRF PRC
 * @{
 * @ingroup nrf_rpc
 *
 * @brief OS-dependent functionality for nRF PRC
 */

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_RPC_OS_WAIT_FOREVER -1
#define NRF_RPC_OS_NO_WAIT 0

/** @brief Structure to pass events between threads. */
struct nrf_rpc_os_event;

/** @brief Structure to pass messages between threads. */
struct nrf_rpc_os_msg;

/** @brief Work callback that will be called from thread pool.
 *
 * @param data Data passed from @ref nrf_rpc_os_thread_pool_send.
 * @param len  Data length.
 */
typedef void (*nrf_rpc_os_work_t)(const uint8_t *data, size_t len);

/** @brief nRF RPC OS-dependent initialization.
 *
 * @param callback Work callback that will be called when something was send
 *                 to a thread pool.
 *
 * @return         0 on success or negative error code.
 */
int nrf_rpc_os_init(nrf_rpc_os_work_t callback);

/** @brief Send work to a thread pool.
 *
 * Function reserves a thread from a thread pool and executes callback provided
 * in @ref nrf_rpc_os_init with `data` and `len` paramteres. If there is no
 * thread available in the thread pool then this function waits.
 *
 * @param data Data pointer to pass. Data is passed as a pointer, no copying is
 *             done.
 * @param len  Length of the `data`.
 */
void nrf_rpc_os_thread_pool_send(const uint8_t *data, size_t len);

/** @brief Initialize event passing structure.
 *
 * @param event Event structure to initialize.
 *
 * @return      0 on success or negative error code.
 */
int nrf_rpc_os_event_init(struct nrf_rpc_os_event *event);

/** @brief Set an event.
 *
 * If some thread is waiting of the event then it will be waken up. If there is
 * no thread waiting next call to @ref nrf_rpc_os_event_wait will return
 * immediately.
 *
 * Event behavior is the same as a binary semaphore.
 *
 * @param event Event to set.
 */
void nrf_rpc_os_event_set(struct nrf_rpc_os_event *event);

/** @brief Wait for an event.
 *
 * @param event   Event to wait for.
 * @param timeout Timeout in milliseconds.
 *		  @c NRF_RPC_OS_WAIT_FOREVER indicates infinite timeout.
 *		  @c NRF_RPC_OS_NO_WAIT indicates no timeout.
 *
 * @return        0 on success or negative error code.
 */
int nrf_rpc_os_event_wait(struct nrf_rpc_os_event *event, int32_t timeout);

/** @brief Initialize message passing structure.
 *
 * @param msg Structure to initialize.
 *
 * @return    0 on success or negative error code.
 */
int nrf_rpc_os_msg_init(struct nrf_rpc_os_msg *msg);

/** @brief Pass a message to a different therad.
 *
 * nRF RPC is passing one message at a time, so there is no need to do
 * FIFO here.
 *
 * @param msg  Message passing structure.
 * @param data Data pointer to pass. Data is passed as a pointer, so no copying
 *             is done.
 * @param len  Length of the `data`.
 */
void nrf_rpc_os_msg_set(struct nrf_rpc_os_msg *msg, const uint8_t *data,
			size_t len);

/** @brief Get a message.
 *
 * If message was not set yet then this function waits.
 *
 * @param[in]  msg  Message passing structure.
 * @param[out] data Received data pointer. Data is passed as a pointer, so no
 *                  copying is done.
 * @param[out] len  Length of the `data`.
 */
void nrf_rpc_os_msg_get(struct nrf_rpc_os_msg *msg, const uint8_t **data,
			size_t *len);

/** @brief Get TLS (Thread Local Storage) for nRF RPC.
 *
 * nRF PRC need one pointer to associate with a thread.
 *
 * @return Pointer stored on TLS or NULL if pointer was not set for this thread.
 */
void* nrf_rpc_os_tls_get(void);

/** @brief Set TLS (Thread Local Storage) for nRF RPC.
 *
 * @param data Pointer to store on TLS.
 */
void nrf_rpc_os_tls_set(void *data);

/** @brief Reserve one context from command context pool.
 *
 * If there is no available context then this function waits for it.
 *
 * @return Context index between 0 and
 *         @kconfig{CONFIG_NRF_RPC_CMD_CTX_POOL_SIZE} - 1.
 */
uint32_t nrf_rpc_os_ctx_pool_reserve();

/** @brief Release context from context pool.
 *
 * @param index Context index that was previously reserved.
 */
void nrf_rpc_os_ctx_pool_release(uint32_t index);

/** @brief Set number of remote threads.
 *
 * Number of remote threads that can be reserved by
 * @ref nrf_rpc_os_remote_reserve is limited by `count` parameter.
 * After initialization `count` is assumed to be zero.
 *
 * @param count Number of remote threads.
 */
void nrf_rpc_os_remote_count(int count);

/** @brief Reserve one thread from a remote thread pool.
 *
 * If there are no more threads available or @ref nrf_rpc_os_remote_count was
 * not called yet then this function waits.
 *
 * Remote thread reserving and releasing can be implemented using a semaphore.
 */
void nrf_rpc_os_remote_reserve();

/** @brief Release one thread from a remote thread pool.
 */
void nrf_rpc_os_remote_release();

#ifdef __cplusplus
}
#endif

/**
 *@}
 */

#endif /* NRF_RPC_OS_H_ */
