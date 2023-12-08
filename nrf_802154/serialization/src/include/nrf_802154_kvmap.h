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

/**@file nrf_802154_kvmap.h
 * @brief Simple key-value map.
 */

#ifndef NRF_802154_KVMAP_H_INCLUDED__
#define NRF_802154_KVMAP_H_INCLUDED__

#include <stdbool.h>
#include <stddef.h>

/**@brief Structure representing a key-value map */
typedef struct
{
    /**@brief Pointer to a memory used to store items */
    void * p_memory;
    /**@brief Maximum number of items the key-value map instance is able to store */
    size_t capacity;
    /**@brief Number of currently stored items */
    size_t count;
    /**@brief Size of a key in bytes */
    size_t key_size;
    /**@brief Size of a value in bytes */
    size_t val_size;
} nrf_802154_kvmap_t;

/**@brief Calculates capacity of memory required to store a key-value map.
 *
 * Example:
 * @code
 * static uint8_t m_map_memory[NRF_802154_KVMAP_MEMORY_SIZE(10, 7, 6)];
 * static nrf_802154_kvmap_t m_map;
 *
 * nrf_802154_kvmap_init(&m_map, m_map_memory, sizeof(m_map_memory),
 *                       7, 6);
 * @endcode
 */
#define NRF_802154_KVMAP_MEMORY_SIZE(capacity, key_size, val_size) \
    ((capacity) * ((key_size) + (val_size)))

/**@brief Initializes a key-value map instance.
 *
 * @param[out] p_kvmap  Pointer to an object to initialize.
 *                      The pointed object should persist as long as the map is in use.
 *                      Cannot be NULL.
 * @param[in] p_memory  Pointer to a memory to be used as items storage.
 *                      Memory pointed by this pointer should persist as long as
 *                      the map pointed by @p p_kvmap is in use.
 *                      Cannot be NULL (with exception, when memsize is 0)
 * @param[in] memsize   Size of the memory pointed by @p p_memory. When defining
 *                      storage you can use @ref NRF_802154_KVMAP_MEMORY_SIZE helper macro.
 * @param[in] key_size  Size of the key in bytes within the map. This must be greater than 0.
 *                      Please note that keys are compared by @ref memcmp. If a key is a
 *                      struct beware of padding in key structs.
 * @param[in] val_size  Size of the value in bytes within the map. This can be 0 if the map
 *                      is used to store keys only.
 */
void nrf_802154_kvmap_init(nrf_802154_kvmap_t * p_kvmap,
                           void               * p_memory,
                           size_t               memsize,
                           size_t               key_size,
                           size_t               val_size);

/**@brief Returns total number of items a key-value map can store
 *
 * @param[in] p_kvmap   Pointer to a key-value map to examine
 *
 * @return              Number of items a key-value map can store
 */
static inline size_t nrf_802154_kvmap_capacity(const nrf_802154_kvmap_t * p_kvmap)
{
    return p_kvmap->capacity;
}

/**@brief Returns current number of items a key-value actually holds.
 *
 * @param[in] p_kvmap   Pointer to a key-value map to examine
 *
 * @return              Number of items a key-value map can store
 */
static inline size_t nrf_802154_kvmap_count(const nrf_802154_kvmap_t * p_kvmap)
{
    return p_kvmap->count;
}

/**@brief Adds a key-value pair to a map.
 *
 * @param[in,out] p_kvmap   Pointer to a key-value map to modify
 * @param[in]     p_key     Pointer to a key to add. Must not be NULL. The size must
 *                          correspond to @c key_size passed to recent
 *                          @ref nrf_802154_kvmap_init
 * @param[in]     p_value   Pointer to a value associated with the key to add. The size must
 *                          correspond to @c val_size passed to recent
 *                          @ref nrf_802154_kvmap_init. This pointer can be NULL when
 *                          the @val_size used to initialize was 0.
 *
 * @retval true     The key-value pair was successfully added to the map.
 *                  This value is returned also when the key was already present in
 *                  the map. In this case just the value is updated.
 * @retval false    Maximum capacity of the map has been reached and new item
 *                  could not be added.
 */
bool nrf_802154_kvmap_add(nrf_802154_kvmap_t * p_kvmap,
                          const void         * p_key,
                          const void         * p_value);

/**@brief Removes a key-value pair from a map.
 *
 * @param[in,out] p_kvmap   Pointer to a key-value map to modify
 * @param[in]     p_key     Pointer to a key to remove. Must not be NULL.
 *                          The size of a memory pointed by @p p_key must correspond to
 *                          @c key_size passed to recent call to @ref nrf_802154_kvmap_init.
 *
 * @retval true     The key was present in the map and has been removed.
 * @retval false    The key was not found in the map. The map in unmodified.
 */
bool nrf_802154_kvmap_remove(nrf_802154_kvmap_t * p_kvmap, const void * p_key);

/**@brief Searches for a key in a key-value map.
 *
 * @param[in]  p_kvmap  Pointer to a key-value map to search.
 * @param[in]  p_key    Pointer to a key to search. Must not be NULL.
 *                      The size of a memory pointed by @p p_key must correspond to
 *                      @c key_size passed to recent call @ref nrf_802154_kvmap_init.
 * @param[out] p_value  If NULL. No value associated with potentially found key is retrieved.
 *                      If not NULL the size of a memory pointed by @p p_value must
 *                      correspond to @c val_size passed to recent @ref nrf_802154_kvmap_init.
 *                      When the key is found, the value associated with the key is stored
 *                      behind @p p_value. When the key is not found memory behind this
 *                      pointer remains unmodified.
 *
 * @retval true     The key has been found. Value associated with the key is available
 *                  behind @p p_value pointer if @p p_value passed was not NULL.
 * @retval false    The key has not been found. Memory pointed by @p p_value
 *                  has been not modified.
 */
bool nrf_802154_kvmap_search(const nrf_802154_kvmap_t * p_kvmap,
                             const void               * p_key,
                             void                     * p_value);

#endif /* NRF_802154_KVMAP_H_INCLUDED__ */
