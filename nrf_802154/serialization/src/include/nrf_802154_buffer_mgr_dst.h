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
 * @file nrf_802154_buffer_mgr_dst.h
 * @brief Buffer management for destination peer of a nRF 802.15.4 serialization.
 *
 * Receiving a buffer from a remote peer and then sending notification about
 * that buffer is a common scenario in serialization.
 *
 * This module is intended for a local peer who receives a buffer content
 * referenced by @c buffer_handle from a remote peer.
 * Function @ref nrf_802154_buffer_mgr_dst_add allows creation of a copy of
 * received content and provides a native local pointer to the copy of received
 * data. When local peer needs to notify a remote peer about some events related
 * to the buffer @ref nrf_802154_buffer_mgr_dst_search_by_local_pointer function
 * can be used to map a local pointer to @c buffer_handle the buffer has been
 * received with. When local peer is done with given buffer
 * @ref nrf_802154_buffer_mgr_dst_remove_by_local_pointer should be called to
 * remove mapping and free any resources used by the mapping.
 */

#ifndef NRF_802154_BUFFER_MGR_DST_H__
#define NRF_802154_BUFFER_MGR_DST_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_kvmap.h"
#include "nrf_802154_buffer_allocator.h"

/**@brief Type of a buffer manager for destination peer of serialization. */
typedef struct
{
    /**@brief Map for matching local pointers to remote buffer handle.
     *
     * Key is a local pointer (@c void*)
     * Value is a remote buffer handle (@c uint32_t)
     */
    nrf_802154_kvmap_t            map;

    /**@brief Allocator providing storage for local buffers. */
    nrf_802154_buffer_allocator_t allocator;
} nrf_802154_buffer_mgr_dst_t;

/**@brief Calculates number of bytes needed to store map.
 * @param buffers_count     Number of buffers to be tracked.
 */
#define NRF_802154_BUFFER_MGR_DST_MAP_MEMSIZE(buffers_count) \
    NRF_802154_KVMAP_MEMORY_SIZE((buffers_count), sizeof(void *), sizeof(uint32_t))

/**@brief Defines instance of @ref rf_802154_buffer_mgr_dst_t with all necessary accompanying
 *        variables.
 *
 * Instance defined by this macro needs to be initialized. See an example below:
 * @code
 * static NRF_802154_BUFFER_MGR_DST_INST_DECL(m_buffer_mgr_host, 10U);
 *
 * void foo(void)
 * {
 *     NRF_802154_BUFFER_MGR_DST_INIT(m_buffer_mgr_host)
 * }
 * @endcode
 *
 * @param name          Identifier of an object of type @ref nrf_802154_buffer_mgr_dst_t
 * @param buffers_count Number of buffers the @c name object will be able to track.
 */
#define NRF_802154_BUFFER_MGR_DST_INST_DECL(name, buffers_count) \
    nrf_802154_buffer_mgr_dst_t name;                            \
    uint8_t name ## _map_mem[                                    \
        NRF_802154_BUFFER_MGR_DST_MAP_MEMSIZE(buffers_count)]    \
    __attribute__((aligned(4)));                                 \
    uint8_t name ## _allocator_mem[                              \
        NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE(buffers_count)]  \
    __attribute__((aligned(4)));                                 \
    const size_t name ## _buffers_count = (buffers_count);

/**@brief Provides extern complementary to @ref NRF_802154_BUFFER_MGR_DST_INST_DECL.
 *
 * @param name Identifier of an object of type @ref nrf_802154_buffer_mgr_dst_t
 */
#define NRF_802154_BUFFER_MGR_DST_INST_DECL_EXTERN(name) \
    extern nrf_802154_buffer_mgr_dst_t name

/**@brief As @ref NRF_802154_BUFFER_MGR_DST_INST_DECL but with static storage. */
#define NRF_802154_BUFFER_MGR_DST_INST_DECL_STATIC(name, buffers_count) \
    static nrf_802154_buffer_mgr_dst_t name;                            \
    static uint8_t name ## _map_mem[                                    \
        NRF_802154_BUFFER_MGR_DST_MAP_MEMSIZE(buffers_count)]           \
    __attribute__((aligned(4)));                                        \
    static uint8_t name ## _allocator_mem[                              \
        NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE(buffers_count)]         \
    __attribute__((aligned(4)));                                        \
    static const size_t name ## _buffers_count = (buffers_count);

/**@brief Calls @ref nrf_802154_buffer_mgr_dst_init for an object instantiated by
 *        @ref NRF_802154_BUFFER_MGR_DST_INST_DECL.
 * @param name Name of object instance introduced by a NRF_802154_BUFFER_MGR_DST_INST_DECL
 *             (note: not a pointer)
 */
#define NRF_802154_BUFFER_MGR_DST_INIT(name) \
    nrf_802154_buffer_mgr_dst_init(          \
        &name,                               \
        name ## _map_mem,                    \
        name ## _allocator_mem,              \
        name ## _buffers_count)

/**@brief Initializes a buffer manager.
 *
 * @param[out] p_obj         Pointer to an object instance to initialize
 * @param[in]  p_map_memory  Pointer to a memory buffer of size equal to
 *                           @ref NRF_802154_BUFFER_MGR_DST_MAP_MEMSIZE(@c buffers_count)
 * @param[in]  p_allocator_memory Pointer to a memory buffer of size equal to
 *                           @ref NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE(@c buffers_count)
 * @param[in]  buffers_count Number of buffers the object @p p_obj will be able to track
 *
 * @sa @ref NRF_802154_BUFFER_MGR_DST_INSTANCE_DECL, @ref NRF_802154_BUFFER_DST_MGR_INIT
 */
void nrf_802154_buffer_mgr_dst_init(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    void                        * p_map_memory,
    void                        * p_allocator_memory,
    size_t                        buffers_count);

/**@brief Adds a remote buffer handle to a buffer manager obtaining a local pointer.
 *
 * This causes an allocation of a buffer and copy of data into it.
 *
 * @param[in,out] p_obj           Pointer to a buffer manager object.
 * @param[in]     buffer_handle   Handle of a remote buffer.
 * @param[in]     p_data          Points to a data the buffer returned by @p pp_local_pointer
 *                                should contain.
 * @param[in]     data_size       Length of data pointed by @p p_data
 * @param[out]    p_local_pointer A pointer to an buffer where a copy of data pointed by
 *                                @ref p_data has been made
 *
 * @retval true   Buffer added to tracking
 * @retval false  Out of memory, @c *pp_local_pointer is undefined
 */
bool nrf_802154_buffer_mgr_dst_add(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    uint32_t                      buffer_handle,
    const void                  * p_data,
    size_t                        data_size,
    void                       ** pp_local_pointer);

/**@brief Searches remote buffer handle by a local buffer pointer.
 *
 * @param[in,out] p_obj           Pointer to an host buffer manager object.
 * @param[in]     p_local_pointer Local pointer buffer obtained from call to
 *                                @ref nrf_802154_buffer_mgr_dst_add
 * @param[out]    p_buffer_handle Pointer to a remote buffer handle assocciated with
 *                                local pointer
 *
 * @retval true     Given @p p_local_pointer has been found. The handle associated with
 *                  the pointer is available at @c *p_buffer_handle.
 * @retval false    Given @p p_local_pointer has not been found.
 */
bool nrf_802154_buffer_mgr_dst_search_by_local_pointer(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    void                        * p_local_pointer,
    uint32_t                    * p_buffer_handle);

/**@brief Removes a local pointer to remote buffer handle association from a buffer manager.
 *
 * This function frees buffer pointed by a @p p_local_pointer if it exists in buffer manager.
 *
 * @param[in,out] p_obj           Pointer to a buffer manager object.
 * @param[in]     p_local_pointer Local pointer to be removed from buffer manager.
 *
 * @retval true   Association has been found and has been removed. Storage pointed by
 *                @p p_local_pointer has been deallocated. Data behind the @ref p_local_pointer
 *                is undefined.
 * @retval false  Association has not been found (but it does not exist in the
 *                buffer manager anyway). Buffer manager has been not modified.
 */
bool nrf_802154_buffer_mgr_dst_remove_by_local_pointer(
    nrf_802154_buffer_mgr_dst_t * p_obj,
    void                        * p_local_pointer);

#endif // NRF_802154_BUFFER_MGR_DST_H__
