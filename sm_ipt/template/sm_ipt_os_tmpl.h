/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SM_IPT_OS_H_
#define SM_IPT_OS_H_

/*
 * THIS IS A TEMPLATE FILE.
 * It should be copied to a suitable location within the host environment into
 * which Shared memory inter-processor transport is integrated, and the following
 * macros and structures should be provided with appropriate implementations.
 */

/**
 * @defgroup sm_ipt_os OS-dependent functionality for SM IPT
 * @{
 * @ingroup sm_ipt_os
 *
 * @brief OS-dependent functionality for SM IPT
 */

#ifdef __cplusplus
extern "C" {
#endif


/* --------------- Shared memory information --------------- */

/** @brief This struct holds all the data required by
 *	os layer to function.
 */
struct sm_ipt_os_ctx {
	/** @brief Pointer to out share memory. */
	void *out_shmem_ptr;

	/** @brief Pointer to in share memory. */
	void *in_shmem_ptr;

	/** @brief Size of out share memory. */
	uint32_t out_total_size;

	/** @brief Size of in share memory. */
	uint32_t in_total_size;

	/** @brief Any other data needed by OS layer */
	some_type other_os_context_data;
};

/** @brief Assertion function.
 */
#define SM_IPT_ASSERT(_expr) __ASSERT_FUNCTION(_expr)

/** @brief Full memory barier.
 */
#define SM_IPT_OS_MEMORY_BARRIER() __sync_synchronize()

/** @brief Returns pointer to the container struct.
 */
#define SM_IPT_OS_GET_CONTAINTER(ptr, type, field) SOME_CONTAINER_MACRO(ptr, type, field)

/* --------------- Inter-core signaling --------------- */

/** @brief Signal other core that new data is waiting.
 */
void nrf_rpc_os_signal(struct sm_ipt_os_ctx *os_ctx);

/** @brief Sets callback that will be called when the other core
 *  has signaled incoming data.
 */
void nrf_rpc_os_signal_handler(struct sm_ipt_os_ctx *os_ctx,
				void (*handler)(struct sm_ipt_os_ctx *));

/** @brief Initializes shared memory and returns all pointers
 *	used by sm_ipt.
 */
int sm_ipt_os_init(struct sm_ipt_os_ctx *os_ctx);

/* --------------- Atomics --------------- */

/* This atomic type must be 32 bits in size.
 */
typedef some_type nrf_rpc_os_atomic_t;

/* Perform the operation suggested by the name, and return the value that
 * had previously been in *atomic.
 */
uint32_t nrf_rpc_os_atomic_or(nrf_rpc_os_atomic_t *atomic, uint32_t value);
uint32_t nrf_rpc_os_atomic_and(nrf_rpc_os_atomic_t *atomic, uint32_t value);
uint32_t nrf_rpc_os_atomic_get(nrf_rpc_os_atomic_t *atomic);

/* --------------- Mutexes --------------- */

typedef some_type nrf_rpc_os_mutex_t;
void nrf_rpc_os_mutex_init(nrf_rpc_os_mutex_t *mutex);
void nrf_rpc_os_lock(nrf_rpc_os_mutex_t *mutex);
void nrf_rpc_os_unlock(nrf_rpc_os_mutex_t *mutex);

/* --------------- Semaphores --------------- */

typedef some_type nrf_rpc_os_sem_t;
void nrf_rpc_os_sem_init(nrf_rpc_os_sem_t *sem);
void nrf_rpc_os_take(nrf_rpc_os_sem_t *sem);
void nrf_rpc_os_give(nrf_rpc_os_sem_t *sem);

/* --------------- Other OS functionality --------------- */

void nrf_rpc_os_yield(void);
void nrf_rpc_os_fatal(void);
int nrf_rpc_os_clz64(uint64_t value);
int nrf_rpc_os_clz32(uint32_t value);

#ifdef __cplusplus
}
#endif

/**
 *@}
 */

#endif /* SM_IPT_OS_H_ */
