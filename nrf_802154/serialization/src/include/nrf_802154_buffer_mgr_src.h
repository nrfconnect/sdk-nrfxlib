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
 * @file nrf_802154_buffer_mgr_src.h
 * @brief Buffer management on source side of a buffer of a nRF 802.15.4 serialization.
 *
 * Sending a buffer to a remote peer and then receiving asynchronous
 * notification about that buffer from the remote peer is a common scenario in
 * serialization. This module is intended for a local peer who needs to send
 * a buffer to a remote peer. The existence of the buffer is out of scope of
 * this module. Function @ref nrf_802154_buffer_mgr_src_add allows creation of
 * a @c buffer_handle which can be sent with a buffer content to a remote peer.
 * The remote peer can then reference the buffer in the local peer by providing
 * the @c buffer_handle. The local peer can map a received @c buffer_handle to
 * the original buffer pointer by a call to
 * @ref nrf_802154_buffer_mgr_src_search_by_buffer_handle.
 * When the local peer is done with given buffer it should call
 * @ref nrf_802154_buffer_mgr_src_remove_by_buffer_handle.
 *
 * The source peer manages buffer existence by its own discretion. The buffer
 * shall exist as long as @c buffer_handle to pointer mapping exists.
 */

#ifndef NRF_802154_BUFFER_MGR_SRC_H__
#define NRF_802154_BUFFER_MGR_SRC_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_kvmap.h"

/**@brief Type of a buffer manager object for a source peer. */
typedef struct
{
    /**@brief Map for holding source peer local buffer pointers. */
    nrf_802154_kvmap_t map;
} nrf_802154_buffer_mgr_src_t;

/**@brief Calculates number of bytes needed to store map.
 * @param buffers_count     Number of buffers to be tracked.
 */
#define NRF_802154_BUFFER_MGR_SRC_MAP_MEMSIZE(buffers_count) \
    NRF_802154_KVMAP_MEMORY_SIZE((buffers_count), sizeof(uint32_t), 0U)

/**@brief Defines instance of @ref nrf_802154_buffer_mgr_src_t with all necessary accompanying
 *        variables.
 *
 * Instance defined by this macro still needs
 * to be initialized. See an example below:
 * @code
 * NRF_802154_BUFFER_MGR_SRC_INST_DECL(m_buffer_mgr_src, 10U);
 *
 * void foo(void)
 * {
 *     NRF_802154_BUFFER_MGR_SRC_INIT(m_buffer_mgr_src)
 * }
 * @endcode
 *
 * @param name          Identifier of an object of type @ref nrf_802154_buffer_mgr_src_t
 * @param buffers_count Number of buffers the @c name object will be able to track.
 */
#define NRF_802154_BUFFER_MGR_SRC_INST_DECL(name, buffers_count) \
    nrf_802154_buffer_mgr_src_t name;                            \
    uint8_t name ## _map_mem[                                    \
        NRF_802154_BUFFER_MGR_SRC_MAP_MEMSIZE(buffers_count)]    \
    __attribute__((aligned(4)));                                 \
    const size_t name ## _buffers_count = (buffers_count);

/**@brief Provides extern complementary to @ref NRF_802154_BUFFER_MGR_SRC_INST_DECL.
 *
 * @param name          Identifier of an object of type @ref nrf_802154_buffer_mgr_src_t
 */
#define NRF_802154_BUFFER_MGR_SRC_INST_DECL_EXTERN(name) \
    extern nrf_802154_buffer_mgr_src_t name

/**@brief @ref NRF_802154_BUFFER_MGR_SRC_INST_DECL but with static storage */
#define NRF_802154_BUFFER_MGR_SRC_INST_DECL_STATIC(name, buffers_count) \
    static nrf_802154_buffer_mgr_src_t name;                            \
    static uint8_t name ## _map_mem[                                    \
        NRF_802154_BUFFER_MGR_SRC_MAP_MEMSIZE(buffers_count)]           \
    __attribute__((aligned(4)));                                        \
    static const size_t name ## _buffers_count = (buffers_count);

/**@brief Calls @ref nrf_802154_buffer_mgr_src_init for an object instantiated by
 *        @ref NRF_802154_BUFFER_MGR_SRC_INST_DECL.
 * @param name Name of object instance introduced by a NRF_802154_BUFFER_MGR_SRC_INST_DECL
 *             (note: not a pointer)
 */
#define NRF_802154_BUFFER_MGR_SRC_INIT(name) \
    nrf_802154_buffer_mgr_src_init(          \
        &name,                               \
        name ## _map_mem,                    \
        name ## _buffers_count)

/**@brief Initializes a source peer buffer manager.
 *
 * @param[out] p_obj         Pointer to an object instance to initialize
 * @param[in]  p_map_memory  Pointer to a memory buffer of size equal to
 *                           @ref NRF_802154_BUFFER_MGR_SRC_MAP_MEMSIZE(@c buffers_count)
 * @param[in]  buffers_count Number of buffers the object @p p_obj will be able to track
 *
 * @sa @ref NRF_802154_BUFFER_MGR_SRC_INSTANCE_DECL, @ref NRF_802154_BUFFER_MGR_SRC_INIT
 */
void nrf_802154_buffer_mgr_src_init(
    nrf_802154_buffer_mgr_src_t * p_obj,
    void                        * p_map_memory,
    size_t                        buffers_count);

/**@brief Adds a local memory buffer pointer to a buffer manager.
 *
 * @param[in,out] p_obj           Pointer to a buffer manager object.
 * @param[in]     p_buffer        Pointer to a buffer to track
 * @param[out]    p_buffer_handle A handle that can be used to reference the buffer pointed by
 *                                @ref p_buffer in a remote side.
 *
 * @retval true   Buffer added to tracking
 * @retval false  Out of memory
 */
bool nrf_802154_buffer_mgr_src_add(
    nrf_802154_buffer_mgr_src_t * p_obj,
    const void                  * p_buffer,
    uint32_t                    * p_buffer_handle);

/**@brief Searches a local memory buffer pointer by a buffer handle.
 *
 * @param[in,out] p_obj           Pointer to a buffer manager object.
 * @param[in]     buffer_handle   Buffer handle obtained from @ref nrf_802154_buffer_mgr_src_add
 * @param[out]    pp_buffer       Pointer to a local buffer pointer associated with buffer_handle
 *
 * @retval true     Given @p buffer_handle has been found. The pointer to an associated buffer
 *                  is available behind @p pp_buffer
 * @retval false    Given @p buffer_handle has not been found.
 */
bool nrf_802154_buffer_mgr_src_search_by_buffer_handle(
    nrf_802154_buffer_mgr_src_t * p_obj,
    uint32_t                      buffer_handle,
    void                       ** pp_buffer);

/**@brief Removes a local memory buffer pointer from a buffer manager.
 *
 * @param[in,out] p_obj         Pointer to a buffer manager object.
 * @param[in]     buffer_handle Buffer handle obtained from @ref nrf_802154_buffer_mgr_src_add
 *
 * @retval true   Pointer has been removed.
 * @retval false  Pointer has not been found (but it does not exist in the
 *                buffer manager anyway).
 */
bool nrf_802154_buffer_mgr_src_remove_by_buffer_handle(
    nrf_802154_buffer_mgr_src_t * p_obj,
    uint32_t                      buffer_handle);

#endif // NRF_802154_BUFFER_MGR_SRC_H__
