/*
 * Copyright (c) 2026, Nordic Semiconductor ASA
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

#include "nrf_802154_bsmap.h"

#include "nrf_802154_assert.h"
#include <string.h>

/* Indicates an index that does not point to any entry in the BSMAP. */
#define INDEX_INVALID UINT16_MAX

void nrf_802154_bsmap_init(nrf_802154_bsmap_t * p_bsmap,
                           size_t               key_size,
                           size_t               value_size,
                           size_t               capacity,
                           void               * p_memory)
{
    NRF_802154_ASSERT(p_bsmap != NULL);
    NRF_802154_ASSERT((key_size != 0U) && (key_size <= UINT16_MAX));
    NRF_802154_ASSERT(value_size <= UINT16_MAX);
    NRF_802154_ASSERT((capacity <= UINT16_MAX) && (capacity <= INDEX_INVALID));
    uint16_t entry_size = (uint16_t)NRF_802154_BSMAP_ENTRY_SIZE(key_size, value_size);

    NRF_802154_ASSERT((entry_size >= key_size) && (entry_size >= value_size));
    NRF_802154_ASSERT(p_memory != NULL);

    *p_bsmap = (nrf_802154_bsmap_t) {
        .key_size         = (uint16_t)key_size,
        .value_size       = (uint16_t)value_size,
        .entry_size       = entry_size,
        .capacity         = (uint16_t)capacity,
        .keys_count       = 0U,
        .p_memory         = p_memory,
        .entry_idx_in_aux = INDEX_INVALID,
        .p_aux_memory     = NULL,
        .p_iter           = NULL,
    };
}

static inline void * entry_ptr_from_memory_get(const nrf_802154_bsmap_t * p_bsmap, uint16_t index)
{
    size_t offset = index * p_bsmap->entry_size;

    return (uint8_t *)p_bsmap->p_memory + offset;
}

static inline uint16_t entry_idx_in_aux_get(const nrf_802154_bsmap_t * p_bsmap)
{
    return (uint16_t)(*(const volatile nrfx_atomic_t *)&p_bsmap->entry_idx_in_aux);
}

static inline void entry_idx_in_aux_set(nrf_802154_bsmap_t * p_bsmap, uint16_t val)
{
    (void)NRFX_ATOMIC_FETCH_STORE(&p_bsmap->entry_idx_in_aux, val);
}

static void * entry_ptr_from_whatever_get(const nrf_802154_bsmap_t * p_bsmap, uint16_t index)
{
    if (index == entry_idx_in_aux_get(p_bsmap))
    {
        /* The index is currently copied to the auxiliary memory. */
        return p_bsmap->p_aux_memory;
    }

    return entry_ptr_from_memory_get(p_bsmap, index);
}

static inline int keys_compare(const nrf_802154_bsmap_t * p_bsmap,
                               const void               * p_key1,
                               const void               * p_key2)
{
    return memcmp(p_key1, p_key2, p_bsmap->key_size);
}

static bool binary_search(const nrf_802154_bsmap_t * p_bsmap,
                          const void               * p_key,
                          uint16_t                 * p_index)
{
    uint16_t a = 0U;
    uint16_t b = p_bsmap->keys_count;
    uint16_t c = 0U;

    while (a < b)
    {
        c = (uint16_t)(a + (b - a) / 2U);

        const void * p_key_c    = entry_ptr_from_whatever_get(p_bsmap, c);
        int          cmp_result = keys_compare(p_bsmap, p_key, p_key_c);

        if (cmp_result == 0)
        {
            *p_index = c;
            return true;
        }

        if (cmp_result < 0)
        {
            b = c;
        }
        else
        {
            a = c + 1;
            c = a;
        }
    }

    /* Not found, c is the index where the key should be inserted. */
    *p_index = c;

    return false;
}

bool nrf_802154_bsmap_rec_get(const nrf_802154_bsmap_t * p_bsmap,
                              const void               * p_key,
                              void                     * p_value)
{
    uint16_t index  = 0U;
    bool     result = binary_search(p_bsmap, p_key, &index);

    if (result && (p_bsmap->value_size != 0U) && (p_value != NULL))
    {
        const uint8_t * p_entry = (const uint8_t *)entry_ptr_from_whatever_get(p_bsmap, index);

        memcpy(p_value, p_entry + p_bsmap->key_size, p_bsmap->value_size);
    }

    return result;
}

static inline void entry_to_aux_memory_swap(nrf_802154_bsmap_t * p_bsmap, uint16_t index)
{
    memcpy(p_bsmap->p_aux_memory, entry_ptr_from_memory_get(p_bsmap, index), p_bsmap->entry_size);
    entry_idx_in_aux_set(p_bsmap, index);
}

static inline void entry_to_aux_memory_swap_end(nrf_802154_bsmap_t * p_bsmap)
{
    entry_idx_in_aux_set(p_bsmap, INDEX_INVALID);
}

static void entry_write_content(const nrf_802154_bsmap_t * p_bsmap, uint16_t index,
                                const void * p_content)
{
    memcpy(entry_ptr_from_memory_get(p_bsmap, index), p_content, p_bsmap->entry_size);
}

static void entry_value_update(nrf_802154_bsmap_t * p_bsmap, uint16_t index, const void * p_value)
{
    entry_to_aux_memory_swap(p_bsmap, index);
    memcpy((uint8_t *)entry_ptr_from_memory_get(p_bsmap, index) + p_bsmap->key_size,
           p_value,
           p_bsmap->value_size);
    entry_to_aux_memory_swap_end(p_bsmap);
}

static void entry_from_key_val_direct_write(const nrf_802154_bsmap_t * p_bsmap,
                                            uint16_t                   index,
                                            const void               * p_key,
                                            const void               * p_value)
{
    uint8_t * p_dst = (uint8_t *)entry_ptr_from_memory_get(p_bsmap, index);

    memcpy(p_dst, p_key, p_bsmap->key_size);
    if (p_bsmap->value_size != 0U)
    {
        memcpy(p_dst + p_bsmap->key_size, p_value, p_bsmap->value_size);
    }
}

static void entries_shift_up(nrf_802154_bsmap_t * p_bsmap, uint16_t from_index)
{
    uint16_t i = p_bsmap->keys_count;

    /* For the first entry we are shifting there is no need to use
     * p_aux_memory, since the destination index i is not yet reachable
     * by binary_search yet.
     */
    entry_write_content(p_bsmap, i, entry_ptr_from_memory_get(p_bsmap, i - 1U));

    p_bsmap->keys_count++;

    while (--i > from_index)
    {
        entry_to_aux_memory_swap(p_bsmap, i);
        entry_write_content(p_bsmap, i, entry_ptr_from_memory_get(p_bsmap, i - 1U));
        entry_to_aux_memory_swap_end(p_bsmap);
    }
}

bool nrf_802154_bsmap_rec_write(nrf_802154_bsmap_t * p_bsmap,
                                const void         * p_key,
                                const void         * p_value,
                                void               * p_aux_memory)
{
    uint16_t index  = 0U;
    bool     result = true;

    p_bsmap->p_aux_memory = p_aux_memory;

    if (binary_search(p_bsmap, p_key, &index))
    {
        /* The key exists update in place. */
        if (p_bsmap->value_size != 0U)
        {
            entry_value_update(p_bsmap, index, p_value);
        }
        else
        {
            /* There is no value for the BSMAP. The goal of the function is reached,
             * that is the key exists. Nothing to do.
             */
        }
    }
    else if (p_bsmap->keys_count < p_bsmap->capacity)
    {
        /* The key does not exist in the BSMAP, but there is room for it. */
        if (index >= p_bsmap->keys_count)
        {
            /* No shift needed. index is not yet reachable by binary_search,
             * no need to use p_aux_memory.
             */
            entry_from_key_val_direct_write(p_bsmap, index, p_key, p_value);

            p_bsmap->keys_count++;
        }
        else
        {
            /* We need to make room at index. Let's shift all entries from index
             * to index+1.
             */
            entries_shift_up(p_bsmap, index);

            /* Note: p_bsmap->keys_count is already increased. */

            entry_to_aux_memory_swap(p_bsmap, index);
            entry_from_key_val_direct_write(p_bsmap, index, p_key, p_value);
            entry_to_aux_memory_swap_end(p_bsmap);
        }
    }
    else
    {
        /* The key does not exist and there is no room for a new entry. */
        result = false;
    }

    /* p_aux_memory is no more needed. */
    p_bsmap->p_aux_memory = NULL;

    return result;
}

static void entries_shift_down(nrf_802154_bsmap_t * p_bsmap, uint16_t to_index)
{
    for (uint16_t i = to_index + 1U; i < p_bsmap->keys_count; i++)
    {
        entry_to_aux_memory_swap(p_bsmap, i - 1U);
        entry_write_content(p_bsmap, i - 1U, entry_ptr_from_memory_get(p_bsmap, i));
        entry_to_aux_memory_swap_end(p_bsmap);
    }
}

static void entry_delete(nrf_802154_bsmap_t * p_bsmap,
                         uint16_t             index,
                         void               * p_aux_memory)
{
    p_bsmap->p_aux_memory = p_aux_memory;

    entries_shift_down(p_bsmap, index);
    p_bsmap->keys_count--;

    /* p_aux_memory is no more needed. */
    p_bsmap->p_aux_memory = NULL;
}

bool nrf_802154_bsmap_rec_delete(nrf_802154_bsmap_t * p_bsmap,
                                 const void         * p_key,
                                 void               * p_aux_memory)
{
    uint16_t index = 0U;

    if (!binary_search(p_bsmap, p_key, &index))
    {
        return false;
    }

    entry_delete(p_bsmap, index, p_aux_memory);

    return true;
}

void nrf_802154_bsmap_clear(nrf_802154_bsmap_t * p_bsmap)
{
    p_bsmap->keys_count = 0U;
}

void nrf_802154_bsmap_iterator_begin(nrf_802154_bsmap_t          * p_bsmap,
                                     nrf_802154_bsmap_iterator_t * p_iter,
                                     bool                          forward)
{
    /* Implementation limits to at most one active iterator of the BSMAP. */
    NRF_802154_ASSERT(p_bsmap->p_iter == NULL);
    NRF_802154_ASSERT(p_iter != NULL);

    p_bsmap->p_iter = p_iter;

    p_iter->index   = forward ? 0 : (int32_t)p_bsmap->keys_count - 1;
    p_iter->forward = forward;
    p_iter->deleted = false;
    p_iter->p_bsmap = p_bsmap;
}

bool nrf_802154_bsmap_iterator_is_valid(const nrf_802154_bsmap_iterator_t * p_iter)
{
    if (p_iter->p_bsmap == NULL)
    {
        return false;
    }

    return (p_iter->index >= 0) &&
           (p_iter->index < (int32_t)p_iter->p_bsmap->keys_count);
}

void nrf_802154_bsmap_iterator_rec_key_get(const nrf_802154_bsmap_iterator_t * p_iter,
                                           void                              * p_key)
{
    const nrf_802154_bsmap_t * p_bsmap = p_iter->p_bsmap;
    uint16_t                   index   = (uint16_t)((uint32_t)(p_iter->index));
    const uint8_t            * p_entry = (const uint8_t *)entry_ptr_from_whatever_get(p_bsmap,
                                                                                      index);

    memcpy(p_key, p_entry, p_bsmap->key_size);
}

void nrf_802154_bsmap_iterator_rec_value_get(const nrf_802154_bsmap_iterator_t * p_iter,
                                             void                              * p_value)
{
    const nrf_802154_bsmap_t * p_bsmap = p_iter->p_bsmap;

    if (p_bsmap->value_size != 0U)
    {
        uint16_t index = (uint16_t)((uint32_t)(p_iter->index));

        const uint8_t * p_entry = (const uint8_t *)entry_ptr_from_whatever_get(p_bsmap, index);

        memcpy(p_value, p_entry + p_bsmap->key_size, p_bsmap->value_size);
    }
}

void nrf_802154_bsmap_iterator_rec_value_write(
    const nrf_802154_bsmap_iterator_t * p_iter,
    const void                        * p_value,
    void                              * p_aux_memory)
{
    nrf_802154_bsmap_t * p_bsmap = p_iter->p_bsmap;

    if (p_bsmap->value_size != 0U)
    {
        uint16_t index = (uint16_t)((uint32_t)(p_iter->index));

        p_bsmap->p_aux_memory = p_aux_memory;

        entry_value_update(p_bsmap, index, p_value);

        /* p_aux_memory is no more needed. */
        p_bsmap->p_aux_memory = NULL;
    }
}

void nrf_802154_bsmap_iterator_rec_delete(
    nrf_802154_bsmap_iterator_t * p_iter,
    void                        * p_aux_memory)
{
    nrf_802154_bsmap_t * p_bsmap = p_iter->p_bsmap;
    uint16_t             index   = (uint16_t)((uint32_t)(p_iter->index));

    entry_delete(p_bsmap, index, p_aux_memory);

    p_iter->deleted = true;
}

void nrf_802154_bsmap_iterator_next(nrf_802154_bsmap_iterator_t * p_iter)
{
    if (p_iter->forward)
    {
        if (!p_iter->deleted)
        {
            p_iter->index += 1;
            /* The p_iter->index can become keys_count,
             * nrf_802154_bsmap_iterator_is_valid checks for that.
             */
        }
    }
    else
    {
        p_iter->index -= 1;
        /* The p_iter->index can become negative.
         * nrf_802154_bsmap_iterator_is_valid checks for that.
         */
    }

    p_iter->deleted = false;
}

void nrf_802154_bsmap_iterator_finish(nrf_802154_bsmap_iterator_t * p_iter)
{
    if (p_iter->p_bsmap != NULL)
    {
        p_iter->p_bsmap->p_iter = NULL;
        p_iter->p_bsmap         = NULL;
    }
}
