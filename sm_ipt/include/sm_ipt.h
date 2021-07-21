/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#ifndef SM_IPT_H_
#define SM_IPT_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "sm_ipt_os.h"

/**
 * @defgroup sm_ipt SM IPT (Shared Memory Inter-Processor Transport) module.
 * @{
 * @brief Module to facilitate communication using shared memory.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Callback to data receiving function.
 *
 * This callback will be called when packet is received.
 *
 * @param packet Received data.
 * @param len Length of received data.
 */
typedef void (*sm_ipt_receive_handler_t)(const uint8_t *packet, size_t len);

/** @brief Single shared memory queue.
 *
 * Used by @ref sm_ipt_ctx to store queue information.
 *
 * Fields of this structre are used internally and should not
 * be used by the user.
 */
struct sm_ipt_queue {
	/** Maximum allocable shared memory size. */
	size_t allocable_size;

	/** Size of a single allocable block. */
	size_t block_size;

	/** Queue placed in shared memory. */
	struct sm_ipt_shm_queue *shm_queue;
};

/** @brief SM IPT context.
 *
 * Used to store current context data.
 *
 * Fields of this structre are used internally and should not
 * be used by the user.
 */
struct sm_ipt_ctx {
	/** Input queue. */
	struct sm_ipt_queue in;

	/** Output queue. */
	struct sm_ipt_queue out;

	/** Output semaphore for allocation. */
	sm_ipt_os_sem_t out_sem;

	/** Output mutex for sending. */
	sm_ipt_os_mutex_t out_mutex;

	/** Allocation bitmap. */
	sm_ipt_os_atomic_t free_mask[IS_ENABLED(CONFIG_SM_IPT_NUM_BLOCKS_64) ? 2 : 1];

	/** Receive handler. */
	sm_ipt_receive_handler_t receive_handler;

	/** OS Context - to be implemented by OS layer. */
	struct sm_ipt_os_ctx os_ctx;
};

/** @brief Initialize the SM IPT
 *
 * This function initializes shared memory and context data.
 *
 * @param ctx         SM IPT Context.
 * @param callback    Callback to receive handler.
 *
 * @return            0 on success or negative error code.
 */
int sm_ipt_init(struct sm_ipt_ctx *ctx, sm_ipt_receive_handler_t callback);

/** @brief Free SM IPT rx buffer
 *
 * This function frees shared memory buffer for receiving data.
 *
 * @param ctx         SM IPT Context.
 * @param packet      Selected buffer.
 */
void sm_ipt_free_rx_buf(struct sm_ipt_ctx *ctx, const uint8_t *buf);

/** @brief Allocate SM IPT tx buffer
 *
 * This function allocates shared memory buffer for trasmitting data.
 *
 * @param ctx         SM IPT Context.
 * @param packet      Pointer to selected buffer.
 * @param len         Length of allocation.
 */
void sm_ipt_alloc_tx_buf(struct sm_ipt_ctx *ctx, uint8_t **buf, size_t len);

/** @brief Free SM IPT tx buffer
 *
 * This function frees shared memory buffer for trasmitting data.
 *
 * @param ctx         SM IPT Context.
 * @param packet      Pointer to selected buffer.
 */
void sm_ipt_free_tx_buf(struct sm_ipt_ctx *ctx, uint8_t *buf);

/** @brief Send message through SM IPT
 *
 * This function sends data stored at buf in shared memory.
 *
 * @param ctx         SM IPT Context.
 * @param packet      Selected buffer.
 * @param len         Length of message.
 *
 * @return            0 on success or negative error code.
 */
int sm_ipt_send(struct sm_ipt_ctx *ctx, uint8_t *buf, size_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* SM_IPT_H_ */
