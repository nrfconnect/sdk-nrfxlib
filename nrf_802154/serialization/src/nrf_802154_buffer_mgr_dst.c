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

/**@file nrf_802154_buffer_mgr_dst.c
 * @brief Buffer management for destination peer of a nRF 802.15.4 serialization.
 */

#include "nrf_802154_buffer_mgr_dst.h"

#include "nrf_802154_assert.h"
#include <string.h>

void nrf_802154_buffer_mgr_dst_init(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    void                        * p_map_memory,
    void                        * p_allocator_memory,
    size_t                        buffers_count)
{
    nrf_802154_kvmap_init(&p_obj->map,
                          p_map_memory,
                          NRF_802154_BUFFER_MGR_DST_MAP_MEMSIZE(buffers_count),
                          sizeof(void *),
                          sizeof(uint32_t));

    nrf_802154_buffer_allocator_init(&p_obj->allocator,
                                     p_allocator_memory,
                                     NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE(buffers_count));
}

bool nrf_802154_buffer_mgr_dst_add(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    uint32_t                      buffer_handle,
    const void                  * p_data,
    size_t                        data_size,
    void                       ** pp_local_pointer)
{
    bool result = false;

    *pp_local_pointer = nrf_802154_buffer_allocator_alloc(&p_obj->allocator);

    if (*pp_local_pointer != NULL)
    {
        memcpy(*pp_local_pointer, p_data, data_size);
        result = nrf_802154_kvmap_add(&p_obj->map, pp_local_pointer, &buffer_handle);

        /* Allocator is a pool allocator of fixed size.
         * If allocator managed to allocate, there must be place in the map.
         */
        NRF_802154_ASSERT(result);
    }

    return result;
}

bool nrf_802154_buffer_mgr_dst_search_by_local_pointer(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    void                        * p_local_pointer,
    uint32_t                    * p_buffer_handle)
{
    return nrf_802154_kvmap_search(&p_obj->map, &p_local_pointer, p_buffer_handle);
}

bool nrf_802154_buffer_mgr_dst_remove_by_local_pointer(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    void                        * p_local_pointer)
{
    bool result;

    result = nrf_802154_kvmap_remove(&p_obj->map, &p_local_pointer);
    if (result)
    {
        nrf_802154_buffer_allocator_free(&p_obj->allocator, p_local_pointer);
    }

    return result;
}
