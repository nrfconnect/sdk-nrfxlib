/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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
 * @file nrf_802154_buffer_allocator.c
 * @brief Buffer allocation for 802.15.4 receptions and transmissions.
 */

#include "nrf_802154_buffer_allocator.h"

#include "nrf_802154_serialization_crit_sect.h"

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>

static uint8_t * buffer_alloc(nrf_802154_buffer_t * p_buffer_pool, size_t buffer_pool_len)
{
    nrf_802154_buffer_t * p_buffer  = NULL;
    bool                  success   = false;
    bool                  retry     = false;
    uint32_t              crit_sect = 0UL;

    do
    {
        retry = false;

        // Iterate over the buffer pool to search for a free buffer
        for (uint32_t i = 0; i < buffer_pool_len; i++)
        {
            p_buffer = &p_buffer_pool[i];

            if (!p_buffer->taken)
            {
                // Free buffer detected. Enter critical section to take it
                nrf_802154_serialization_crit_sect_enter(&crit_sect);

                if (p_buffer->taken)
                {
                    // The allocation was preempted and the buffer was taken by higher priority
                    // Reiterate over the buffer pool and search for a free buffer again
                    retry = true;
                }
                else
                {
                    // The allocation can be performed safely
                    p_buffer->taken = true;
                    success         = true;
                }

                nrf_802154_serialization_crit_sect_exit(crit_sect);

                break;
            }
        }
    }
    while (retry);

    return success ? p_buffer->data : NULL;
}

static void buffer_free(nrf_802154_buffer_t * p_buffer_to_free,
                        nrf_802154_buffer_t * p_buffer_pool,
                        size_t                buffer_pool_len)
{
    uint32_t crit_sect = 0UL;
    size_t   idx       =
        ((uintptr_t)p_buffer_to_free - (uintptr_t)p_buffer_pool) / sizeof(nrf_802154_buffer_t);

    NRF_802154_ASSERT(idx < buffer_pool_len);

    nrf_802154_serialization_crit_sect_enter(&crit_sect);

    p_buffer_pool[idx].taken = false;

    nrf_802154_serialization_crit_sect_exit(crit_sect);
}

void nrf_802154_buffer_allocator_init(nrf_802154_buffer_allocator_t * p_obj,
                                      void                          * p_memory,
                                      size_t                          memsize)
{
    size_t capacity = memsize / sizeof(nrf_802154_buffer_t);

    NRF_802154_ASSERT((capacity == 0U) || ((capacity != 0U) && (p_memory != NULL)));

    p_obj->p_memory = p_memory;
    p_obj->capacity = capacity;

    nrf_802154_buffer_t * p_buffer = (nrf_802154_buffer_t *)p_obj->p_memory;

    for (size_t i = 0; i < p_obj->capacity; i++)
    {
        p_buffer[i].taken = false;
    }
}

void * nrf_802154_buffer_allocator_alloc(const nrf_802154_buffer_allocator_t * p_obj)
{
    return buffer_alloc((nrf_802154_buffer_t *)p_obj->p_memory, p_obj->capacity);
}

void nrf_802154_buffer_allocator_free(const nrf_802154_buffer_allocator_t * p_obj,
                                      void                                * p_buffer)
{
    buffer_free(p_buffer, (nrf_802154_buffer_t *)p_obj->p_memory, p_obj->capacity);
}
