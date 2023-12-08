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
 * @file nrf_802154_buffer_allocator.h
 * @brief Buffer allocation for 802.15.4 receptions and transmissions.
 */

#ifndef NRF_802154_BUFFER_ALLOCATOR_H__
#define NRF_802154_BUFFER_ALLOCATOR_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Default length of a buffer allocated by the buffer allocation mechanism.
 *
 * The buffer should be able to store the longest possible 802.15.4 packet.
 */
#define NRF_802154_BUFFER_ALLOCATOR_DEFAULT_BUFFER_LEN 128

/**@brief Calculates byte size of memory required to store a buffer allocator.
 *
 * Example:
 * @code
 * static uint8_t m_memory[NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE(10)];
 * static nrf_802154_buffer_allocator_t m_buffer_allocator;
 *
 * nrf_802154_buffer_allocator_init(&m_buffer_allocator, m_memory, sizeof(m_memory));
 * @endcode
 */
#define NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE(capacity) \
    ((capacity) * (sizeof(nrf_802154_buffer_t)))

/** @brief Structure representing a buffer. */
typedef struct
{
    /** @brief Stored data. */
    uint8_t       data[NRF_802154_BUFFER_ALLOCATOR_DEFAULT_BUFFER_LEN];
    /** @brief Flag indicating if a buffer is currently in use. */
    volatile bool taken;
} nrf_802154_buffer_t;

/** @brief Structure representing a buffer allocator. */
typedef struct
{
    /** @brief Pointer to a memory used to store buffers. */
    void * p_memory;
    /** @brief Maximum number of buffers the buffer allocator instance is able to store. */
    size_t capacity;
} nrf_802154_buffer_allocator_t;

/**
 * @brief Initializes a buffer allocator instance.
 *
 * @param[out] p_obj  Pointer to an object to initialize.
 *                    The pointed object should persist as long as the buffer allocator
 *                    is in use. Cannot be NULL.
 * @param[in] p_memory  Pointer to a memory to be used as to store buffers.
 *                      Memory pointed by this pointer should persist as long as
 *                      the buffer allocator pointed by @p p_obj is in use.
 *                      Cannot be NULL (with exception, when memsize is 0)
 * @param[in] memsize   Size of the memory pointed by @p p_memory. When defining
 *                      storage you can use @ref NRF_802154_BUFFER_ALLOCATOR_MEMORY_SIZE
 *                      helper macro.
 */
void nrf_802154_buffer_allocator_init(nrf_802154_buffer_allocator_t * p_obj,
                                      void                          * p_memory,
                                      size_t                          memsize);

/**
 * @brief Allocates buffer for 802.15.4 reception or transmission.
 *
 * @param[in] p_obj  Pointer to a buffer allocator that stores the buffer pool to allocate from.
 *
 * @return Pointer to allocated buffer or NULL if no buffer could be allocated.
 */
void * nrf_802154_buffer_allocator_alloc(const nrf_802154_buffer_allocator_t * p_obj);

/**
 * @brief Frees buffer allocated for 802.15.4 reception or transmission.
 *
 * @param[in] p_obj     Pointer to a buffer allocator that stores the buffer pool to free from.
 * @param[in] p_buffer  Pointer to a buffer to free.
 *
 * @note This function should be used complementary to @ref nrf_802154_buffer_allocator_alloc.
 */
void nrf_802154_buffer_allocator_free(const nrf_802154_buffer_allocator_t * p_obj, void * p_buffer);

/**
 * @brief Gets total number of buffers a buffer allocator can store.
 *
 * @param[in] p_obj  Pointer to a buffer allocator to check.
 *
 * @return  Number of buffers a buffer allocator can store.
 */
static inline size_t nrf_802154_buffer_allocator_capacity(
    const nrf_802154_buffer_allocator_t * p_obj)
{
    return p_obj->capacity;
}

#endif // NRF_802154_BUFFER_ALLOCATOR_H__
