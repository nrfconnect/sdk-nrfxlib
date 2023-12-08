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

/**@file nrf_802154_buffer_mgr_src.c
 * @brief Buffer management on source side of a buffer of a nRF 802.15.4 serialization.
 *
 * @note Implementation valid for 32-bit architectures only
 */

#include "nrf_802154_buffer_mgr_src.h"

#include "nrf_802154_assert.h"

void nrf_802154_buffer_mgr_src_init(
    nrf_802154_buffer_mgr_src_t * p_obj,
    void                        * p_map_memory,
    size_t                        buffers_count)
{
    /* Implementation is for 32-bit architectures only
     * When this is not true we need mapping uint32->void*
     * When this is true only presence of buffer handle can be checked */
    NRF_802154_ASSERT(sizeof(void *) == sizeof(uint32_t) );

    nrf_802154_kvmap_init(&p_obj->map,
                          p_map_memory,
                          NRF_802154_BUFFER_MGR_SRC_MAP_MEMSIZE(buffers_count),
                          sizeof(uint32_t),
                          0U);
}

bool nrf_802154_buffer_mgr_src_add(
    nrf_802154_buffer_mgr_src_t * p_obj,
    const void                  * p_buffer,
    uint32_t                    * p_buffer_handle)
{
    bool     result;
    uint32_t buffer_handle = (uintptr_t)p_buffer;

    result = nrf_802154_kvmap_add(&p_obj->map, &buffer_handle, NULL);
    if (result)
    {
        *p_buffer_handle = buffer_handle;
    }

    return result;
}

bool nrf_802154_buffer_mgr_src_search_by_buffer_handle(
    nrf_802154_buffer_mgr_src_t * p_obj,
    uint32_t                      buffer_handle,
    void                       ** pp_buffer)
{
    bool result = false;

    if (nrf_802154_kvmap_search(&p_obj->map, &buffer_handle, NULL))
    {
        *pp_buffer = (void *)buffer_handle;
        result     = true;
    }
    return result;
}

bool nrf_802154_buffer_mgr_src_remove_by_buffer_handle(
    nrf_802154_buffer_mgr_src_t * p_obj,
    uint32_t                      buffer_handle)
{
    return nrf_802154_kvmap_remove(&p_obj->map, &buffer_handle);
}
