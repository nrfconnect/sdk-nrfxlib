/*
 * Copyright (c) 2019, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @brief Module implementing simple FIFO queue.
 */

#ifndef NRF_802154_QUEUE_H__
#define NRF_802154_QUEUE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**@brief Type representing a FIFO queue. */
typedef struct
{
    /**@brief Pointer to items memory of the queue.
     * @details Memory pointed by this pointer has size @c item_size * @c capacity. */
    void           * p_memory;

    /**@brief Size of an item in the queue. */
    uint8_t          item_size;

    /**@brief Maximum number of items that can be stored in the memory of the queue */
    uint8_t          capacity;

    /**@brief Index in the items memory of the queue where next item is written. */
    volatile uint8_t wridx;

    /**@brief Index in the items memory of the queue where next item is read. */
    volatile uint8_t rdidx;
} nrf_802154_queue_t;

/**@brief Initializes a queue.
 *
 * @param[in] p_queue       Pointer to the queue instance to be initialized. Must not be NULL.
 * @param[in] p_memory      Pointer to a memory that will be used to store items of the queue.
 *                          Must not be NULL.
 * @param[in] memory_size   Size of the memory pointed by @p p_memory.
 *                          This parameter must be no less than 2 * @p item_size
 * @param[in] item_size     Size of an item of the queue. Must not be 0.
 */
void nrf_802154_queue_init(nrf_802154_queue_t * p_queue,
                           void               * p_memory,
                           size_t               memory_size,
                           size_t               item_size);

/**@brief Returns pointer to the next item to be written to the queue.
 *
 * This function is to be used when writing data to the queue directly (no copy).
 * Returned pointer is valid when the queue is not full (@ref nrf_802154_queue_is_full returned false).
 * To write an item to the queue perform following.
 * @code
 * if (!nrf_802154_queue_is_full(&queue))
 * {
 *     my_item_t * p_item = (my_item_t *)nrf_802154_queue_push_begin(&queue);
 *     ... p_item is now direct pointer into memory of the queue, fill all data at p_item pointer
 *     p_item->some_field = some_value;
 *     ... fill all data at p_item pointer
 *     nrf_802154_queue_push_commit(&queue)
 * }
 * @endcode
 *
 * To ensure thread-safety external locking is required.
 *
 * @param[in] p_queue        Pointer to the queue instance.
 *
 * @return Pointer to the next item to be written.
 */
void * nrf_802154_queue_push_begin(const nrf_802154_queue_t * p_queue);

/**@brief Increments write pointer of the queue.
 *
 * @param[in] p_queue       Pointer to the queue instance.
 */
void nrf_802154_queue_push_commit(nrf_802154_queue_t * p_queue);

/**@brief Returns pointer to the next item to be read from the queue.
 *
 * This function is to be used when reading data from the queue directly (no copy).
 * Returned pointer is valid when the queue is not empty (@ref nrf_802154_queue_is_empty returned false).
 * To ensure thread-safety external locking is required.
 *
 * To read an item from the queue perform following.
 * @code
 * if (!nrf_802154_queue_is_empty(&queue))
 * {
 *     my_item_t * p_item = (my_item_t *)nrf_802154_queue_pop_begin(&queue);
 *     ... read & process data pointed by p_item
 *     nrf_802154_queue_pop_commit(&queue)
 * }
 * @endcode
 *
 *
 * @param[in] p_queue        Pointer to the queue instance.
 *
 * @return Pointer to the next item to be written.
 */
void * nrf_802154_queue_pop_begin(const nrf_802154_queue_t * p_queue);

/**@brief Increments read pointer of the queue.
 *
 * @param[in] p_queue       Pointer to the queue instance.
 */
void nrf_802154_queue_pop_commit(nrf_802154_queue_t * p_queue);

/**@brief Checks if the queue is empty.
 *
 * @param[in] p_queue       Pointer to the queue instance.
 *
 * @retval true             When the queue is empty
 * @retval false            When the queue is not empty.
 */
static inline bool nrf_802154_queue_is_empty(const nrf_802154_queue_t * p_queue)
{
    return (p_queue->wridx == p_queue->rdidx);
}

/**@brief Checks if the queue is full.
 *
 * @param[in] p_queue       Pointer to the queue instance.
 *
 * @retval true             When the queue is full.
 * @retval false            When the queue is not full.
 */
bool nrf_802154_queue_is_full(const nrf_802154_queue_t * p_queue);

#endif /* NRF_802154_QUEUE_H__ */
