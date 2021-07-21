/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "sm_ipt.h"
#include "sm_ipt_log.h"
#include "sm_ipt_errno.h"


#define FLAG_RELEASE 0x80

#define WORD_SIZE sizeof(uint32_t)

#if defined(CONFIG_SM_IPT_NUM_BLOCKS_32)
typedef uint32_t mask_t;
typedef int32_t smask_t;
#define NUM_BLOCKS 32
#define mask_clz sm_ipt_os_clz32
#elif defined(CONFIG_SM_IPT_NUM_BLOCKS_64)
typedef uint64_t mask_t;
typedef int64_t smask_t;
#define NUM_BLOCKS 64
#define mask_clz sm_ipt_os_clz64
#else
#error Number of shared dynamic memory blocks is not configured.
#endif

#define QUEUE_INDEX_MASK (NUM_BLOCKS - 1)

/* Worst case scenario: NUM_BLOCKS count of messages waiting to be sent and NUM_BLOCKS count of
 * buffers waiting to be freed + 1 item that must always be free in circular buffer
 */
#define QUEUE_ITEMS_CNT (2 * NUM_BLOCKS + 1)


struct sm_ipt_shm_queue {
	uint32_t queue_tx;
	uint32_t queue_rx;
	uint8_t queue[QUEUE_ITEMS_CNT];
	uint8_t handshake;
	uint16_t _padding; /* Care should be taken to align allocable to word boundary */
	uint8_t allocable[];
};

struct sm_ipt_message {
	uint32_t size;
	uint8_t buf[];
};


static void memory_corrupted_error(void)
{
	SM_IPT_ERR("Shared memory corrupted");
	sm_ipt_os_fatal();
	SM_IPT_ASSERT(0)
}

static void free_mask_set(struct sm_ipt_ctx *ctx, mask_t mask)
{
	sm_ipt_os_atomic_or(&ctx->free_mask[0], mask);

	if (NUM_BLOCKS > 32) {
		sm_ipt_os_atomic_or(&ctx->free_mask[1], (uint64_t)mask >> 32);
	}

	SM_IPT_OS_MEMORY_BARRIER();
}

static bool free_mask_unset(struct sm_ipt_ctx *ctx, mask_t mask)
{
	uint32_t old;
	uint32_t mask_part;

	mask_part = (uint32_t)mask;
	old = sm_ipt_os_atomic_and(&ctx->free_mask[0], ~mask_part);
	if ((old & mask_part) != mask_part) {
		sm_ipt_os_atomic_or(&ctx->free_mask[0], old & mask_part);

		return false;
	}

	if (NUM_BLOCKS > 32) {
		mask_part = (uint32_t)((uint64_t)mask >> 32);
		old = sm_ipt_os_atomic_and(&ctx->free_mask[1], ~mask_part);

		if ((old & mask_part) != mask_part) {
			sm_ipt_os_atomic_or(&ctx->free_mask[1], old & mask_part);
			sm_ipt_os_atomic_or(&ctx->free_mask[0], (uint32_t)mask);
			return false;
		}
	}

	SM_IPT_OS_MEMORY_BARRIER();

	return true;
}

static mask_t free_mask_get(struct sm_ipt_ctx *ctx)
{
	mask_t mask;

	mask = sm_ipt_os_atomic_get(&ctx->free_mask[0]);

	if (NUM_BLOCKS > 32) {
		mask = (uint64_t)sm_ipt_os_atomic_get(&ctx->free_mask[1]) << 32;
	}

	return mask;
}

static void free_mask_init(struct sm_ipt_ctx *ctx)
{
	free_mask_set(ctx, ~(mask_t)0);
}

static mask_t calc_mask(size_t blocks, size_t index)
{
	mask_t mask;

	mask = (mask_t)1 << (NUM_BLOCKS - 1);        /* 100000000... */
	mask = (smask_t)mask >> (blocks - 1);        /* 111000000... (e.g. blocks = 3) */
	mask = mask >> index;                        /* 000011100... (e.g. index = 4) */

	return mask;
}

void sm_ipt_alloc_tx_buf(struct sm_ipt_ctx *ctx, uint8_t **buf, size_t len)
{
	/* Actual allocated memory: | 32-bit size | data | padding | */
	size_t blocks = ((sizeof(struct sm_ipt_message) + len) + (ctx->out.block_size - 1))
							/ ctx->out.block_size;
	bool sem_taken = false;
	mask_t cur_mask;
	mask_t sh_mask;
	bool unset_success;
	mask_t mask;
	size_t free_index;

	SM_IPT_DBG("Trying to allocate %d bytes", len);

	if ((blocks > NUM_BLOCKS) || (blocks == 0)) {
		SM_IPT_ERR("Requested %d bytes, maximum is %d", len,
				ctx->out.allocable_size - sizeof(struct sm_ipt_message));
		*buf = NULL;
		return;
	}

	do {
		do {
			/* create shifted mask with bits set where `blocks` can be allocated */
			cur_mask = free_mask_get(ctx);
			sh_mask = cur_mask;
			for (size_t i = 1; i < blocks; i++) {
				sh_mask &= (sh_mask << 1);
			}

			 /* if no memory */
			if (sh_mask == 0) {
				/* wait for any block to be empty */
				SM_IPT_DBG("Not enough free shared memory. Waiting.");
				sm_ipt_os_take(&ctx->out_sem);
				sem_taken = true;
			}

		} while (sh_mask == 0);

		/* get first available blocks */
		free_index = mask_clz(sh_mask);
		/* create bit mask with blocks that will be used */
		mask = calc_mask(blocks, free_index);
		/* update masks */
		unset_success = free_mask_unset(ctx, mask);

		/* there is a small probability that unset will be unsuccessful */
		if (!unset_success) {
			/* give semaphore, because free_mask_unset may cause other thread
			 * waiting before it reverted the changes
			 */
			sm_ipt_os_give(&ctx->out_sem);
			sem_taken = false;
		}
	} while (!unset_success);

	/* Give semaphore back, because there may be some other thread waiting */
	if (sem_taken && ((cur_mask & ~mask) != 0)) {
		sm_ipt_os_give(&ctx->out_sem);
	}

	struct sm_ipt_message *msg = (struct sm_ipt_message *)&ctx->out.shm_queue->allocable[
								ctx->out.block_size * free_index];

	msg->size = blocks * ctx->out.block_size;

	*buf = msg->buf;
	SM_IPT_DBG("Allocated memory at 0x%x", (uint32_t)*buf);
}

void sm_ipt_free_tx_buf(struct sm_ipt_ctx *ctx, uint8_t *buf)
{
	struct sm_ipt_message *msg = SM_IPT_OS_GET_CONTAINTER(buf, struct sm_ipt_message, buf);
	uint32_t offset = (uint8_t *)msg - (uint8_t *)ctx->out.shm_queue->allocable;
	uint32_t block_index = offset / ctx->out.block_size;
	uint32_t allocated_blocks;

	SM_IPT_ASSERT(block_index < NUM_BLOCKS);
	SM_IPT_ASSERT(buf == &ctx->out.shm_queue->allocable[block_index * ctx->out.block_size]);

	allocated_blocks = msg->size / ctx->out.block_size;

	SM_IPT_ASSERT(allocated_blocks % ctx->out.block_size == 0);
	SM_IPT_ASSERT(msg->size <= ctx->out.allocable_size);
	SM_IPT_ASSERT(offset + msg->size <= ctx->out.allocable_size);

	free_mask_set(ctx, calc_mask(allocated_blocks, block_index));
	sm_ipt_os_give(&ctx->out_sem);
	SM_IPT_DBG("Dealocated TX block at 0x%x", (uint32_t)msg->buf);
}


static void queue_send(struct sm_ipt_ctx *ctx, uint8_t data)
{
	uint32_t tx;
	uint32_t dst;

	sm_ipt_os_lock(&ctx->out_mutex);

	tx = ctx->out.shm_queue->queue_tx;
	dst = tx;

	if (dst >= QUEUE_ITEMS_CNT) {
		sm_ipt_os_unlock(&ctx->out_mutex);
		memory_corrupted_error();
		return;
	}

	tx++;

	if (tx >= QUEUE_ITEMS_CNT) {
		tx = 0;
	}

	ctx->out.shm_queue->queue[dst] = data;

	SM_IPT_OS_MEMORY_BARRIER();

	ctx->out.shm_queue->queue_tx = tx;

	sm_ipt_os_unlock(&ctx->out_mutex);

	sm_ipt_os_signal(&ctx->os_ctx);
}


int sm_ipt_send(struct sm_ipt_ctx *ctx, uint8_t *buf, size_t len)
{
	struct sm_ipt_message *msg = SM_IPT_OS_GET_CONTAINTER(buf, struct sm_ipt_message, buf);
	uint32_t offset = (uint8_t *)msg - (uint8_t *)ctx->out.shm_queue->allocable;
	uint32_t block_index = offset / ctx->out.block_size;
	uint32_t allocated_blocks;
	uint32_t blocks = (len + ctx->out.block_size - 1) / ctx->out.block_size;
	uint32_t total_len = len + sizeof(struct sm_ipt_message);

	SM_IPT_DBG("Trying to send message of: %d bytes, at address: 0x%x", len,
									(uint32_t)msg->buf);

	SM_IPT_ASSERT(block_index < NUM_BLOCKS);
	SM_IPT_ASSERT((uint8_t *)msg ==
			(&ctx->out.shm_queue->allocable[block_index * ctx->out.block_size]));

	allocated_blocks = msg->size / ctx->out.block_size;

	SM_IPT_ASSERT(msg->size % ctx->out.block_size == 0);
	SM_IPT_ASSERT(msg->size <= ctx->out.allocable_size);
	SM_IPT_ASSERT(offset + msg->size <= ctx->out.allocable_size);
	SM_IPT_ASSERT(total_len <= msg->size);

	if (blocks < allocated_blocks) {
		free_mask_set(ctx, calc_mask(allocated_blocks - blocks, block_index + blocks));
		sm_ipt_os_give(&ctx->out_sem);
	}

	msg->size = total_len;

	queue_send(ctx, block_index);

	return 0;
}


static int queue_recv(struct sm_ipt_ctx *ctx)
{
	uint32_t tx = ctx->in.shm_queue->queue_tx;
	uint32_t rx = ctx->in.shm_queue->queue_rx;
	uint8_t data;

	tx = ctx->in.shm_queue->queue_tx;
	rx = ctx->in.shm_queue->queue_rx;

	if (rx >= QUEUE_ITEMS_CNT) {
		memory_corrupted_error();
		return -NRF_EIO;
	}

	if (tx == rx) {
		return -NRF_EBADMSG;
	}

	SM_IPT_OS_MEMORY_BARRIER();

	data = ctx->in.shm_queue->queue[rx];

	rx++;

	if (rx >= QUEUE_ITEMS_CNT) {
		rx = 0;
	}

	ctx->in.shm_queue->queue_rx = rx;

	return data;
}


static void signal_received(struct sm_ipt_os_ctx *os_ctx)
{
	int block;
	int release;
	struct sm_ipt_message *msg;
	uint32_t blocks;
	struct sm_ipt_ctx *ctx;

	ctx = SM_IPT_OS_GET_CONTAINTER(os_ctx, struct sm_ipt_ctx, os_ctx);

	while (true) {
		block = queue_recv(ctx);
		if (block < 0) {
			break;
		}

		release = (block & FLAG_RELEASE);
		block &= ~FLAG_RELEASE;

		if (block >= NUM_BLOCKS) {
			continue;
		}

		if (release) {
			msg = (struct sm_ipt_message *)(&ctx->out.shm_queue->allocable[
								block * ctx->out.block_size]);
			blocks = (msg->size + ctx->out.block_size - 1) / ctx->out.block_size;
			free_mask_set(ctx, calc_mask(blocks, block));
			sm_ipt_os_give(&ctx->out_sem);
		} else {
			msg = (struct sm_ipt_message *)(&ctx->in.shm_queue->allocable[
								block * ctx->in.block_size]);

			if ((msg->size >= (&ctx->in.shm_queue->allocable[ctx->in.allocable_size] -
				(uint8_t *)msg)) || (msg->size < sizeof(struct sm_ipt_message))) {
				memory_corrupted_error();
				break;
			}

			SM_IPT_DBG("Message received at: 0x%x, length: %d", (uint32_t)msg->buf,
						(msg->size - sizeof(struct sm_ipt_message)));

			ctx->receive_handler(msg->buf, msg->size - sizeof(struct sm_ipt_message));
		}
	}
}

void sm_ipt_free_rx_buf(struct sm_ipt_ctx *ctx, const uint8_t *buf)
{
	struct sm_ipt_message *msg = SM_IPT_OS_GET_CONTAINTER(buf, struct sm_ipt_message, buf);
	uint32_t offset = (uint8_t *)msg - (uint8_t *)ctx->in.shm_queue->allocable;
	uint32_t block_index = offset / ctx->in.block_size;

	SM_IPT_ASSERT(block_index < NUM_BLOCKS);

	queue_send(ctx, (block_index | FLAG_RELEASE));
	SM_IPT_DBG("Dealocated RX block at 0x%x", (uint32_t)msg->buf);
}

static void handshake_step(struct sm_ipt_ctx *ctx, uint8_t this_value, uint8_t next_value)
{
	SM_IPT_DBG("Setting out handshake to: %d", this_value);

	ctx->out.shm_queue->handshake = this_value;

	SM_IPT_OS_MEMORY_BARRIER();

	SM_IPT_DBG("Waiting for handshake next value: %d", next_value);

	while ((ctx->in.shm_queue->handshake != this_value) &&
		(ctx->in.shm_queue->handshake != next_value)) {
		sm_ipt_os_yield();
		SM_IPT_OS_MEMORY_BARRIER();
	}
}

int sm_ipt_init(struct sm_ipt_ctx *ctx, sm_ipt_receive_handler_t callback)
{
	int err;

	ctx->receive_handler = callback;

	err = sm_ipt_os_init(&ctx->os_ctx);
	if (err < 0) {
		return err;
	}

	free_mask_init(ctx);
	sm_ipt_os_sem_init(&ctx->out_sem);
	sm_ipt_os_mutex_init(&ctx->out_mutex);

	ctx->out.shm_queue = (struct sm_ipt_shm_queue *) ctx->os_ctx.out_shmem_ptr;
	ctx->in.shm_queue = (struct sm_ipt_shm_queue *) ctx->os_ctx.in_shmem_ptr;

	SM_IPT_ASSERT((uint32_t)ctx->out.shm_queue.allocable % WORD_SIZE == 0);
	SM_IPT_ASSERT((uint32_t)ctx->in.shm_queue.allocable % WORD_SIZE == 0);

	ctx->out.block_size = (((ctx->os_ctx.out_total_size - sizeof(struct sm_ipt_queue))
							/ NUM_BLOCKS) / WORD_SIZE) * WORD_SIZE;
	ctx->in.block_size = (((ctx->os_ctx.in_total_size - sizeof(struct sm_ipt_queue))
							/ NUM_BLOCKS) / WORD_SIZE) * WORD_SIZE;
	ctx->out.allocable_size = ctx->out.block_size * NUM_BLOCKS;
	ctx->in.allocable_size = ctx->in.block_size * NUM_BLOCKS;

	handshake_step(ctx, 0x32, 0x43);
	handshake_step(ctx, 0x43, 0xF6);

	ctx->out.shm_queue->queue_tx = 0;
	ctx->out.shm_queue->queue_rx = 0;
	ctx->in.shm_queue->queue_tx = 0;
	ctx->in.shm_queue->queue_rx = 0;

	sm_ipt_os_signal_handler(&ctx->os_ctx, signal_received);

	handshake_step(ctx, 0xF6, 0xA8);
	handshake_step(ctx, 0xA8, 0xA8);

	SM_IPT_INF("Initialized successfully");
	SM_IPT_INF("Max TX size: %d", ctx->out.allocable_size);
	SM_IPT_INF("Max RX size: %d", ctx->in.allocable_size);

	return 0;
}
