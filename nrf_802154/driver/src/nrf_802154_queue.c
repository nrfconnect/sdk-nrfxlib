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

#include "nrf_802154_assert.h"

#include "nrf_802154_queue.h"

static inline uint8_t increment_modulo(uint8_t v, uint8_t wrap_at_value)
{
    v++;

    if (v >= wrap_at_value)
    {
        v = 0U;
    }

    return v;
}

static inline void * idx2ptr(const nrf_802154_queue_t * p_queue, size_t idx)
{
    return ((uint8_t *)(p_queue->p_memory)) + idx * p_queue->item_size;
}

void nrf_802154_queue_init(nrf_802154_queue_t * p_queue,
                           void               * p_memory,
                           size_t               memory_size,
                           size_t               item_size)
{
    NRF_802154_ASSERT(p_queue != NULL);
    NRF_802154_ASSERT(p_memory != NULL);
    NRF_802154_ASSERT(item_size != 0U);

    /* Due uint8_t type of nrf_802154_queue_t::item_size */
    NRF_802154_ASSERT(item_size <= UINT8_MAX);

    size_t capacity = memory_size / item_size;

    /* Due simplified design, one entry in p_memory array is lost,
     * see nrf_802154_queue_is_empty and nrf_802154_queue_is_full */
    NRF_802154_ASSERT(capacity >= 2U);

    /* Due uint8_t type of nrf_802154_queue_t::capacity */
    NRF_802154_ASSERT(capacity <= UINT8_MAX);

    p_queue->p_memory  = p_memory;
    p_queue->capacity  = capacity;
    p_queue->item_size = item_size;
    p_queue->wridx     = 0U;
    p_queue->rdidx     = 0U;
}

void * nrf_802154_queue_push_begin(const nrf_802154_queue_t * p_queue)
{
    return idx2ptr(p_queue, p_queue->wridx);
}

void nrf_802154_queue_push_commit(nrf_802154_queue_t * p_queue)
{
    p_queue->wridx = increment_modulo(p_queue->wridx, p_queue->capacity);
}

void * nrf_802154_queue_pop_begin(const nrf_802154_queue_t * p_queue)
{
    return idx2ptr(p_queue, p_queue->rdidx);
}

void nrf_802154_queue_pop_commit(nrf_802154_queue_t * p_queue)
{
    p_queue->rdidx = increment_modulo(p_queue->rdidx, p_queue->capacity);
}

bool nrf_802154_queue_is_full(const nrf_802154_queue_t * p_queue)
{
    size_t wridx;

    wridx = increment_modulo(p_queue->wridx, p_queue->capacity);

    return (p_queue->rdidx == wridx);
}
