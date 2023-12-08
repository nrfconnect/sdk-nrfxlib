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

/**@file nrf_802154_kvmap.c
 * @brief Simple key-value map.
 */

#include "nrf_802154_kvmap.h"

#include "nrf_802154_serialization_crit_sect.h"

#include <stdint.h>
#include <string.h>

#define NRF_802154_KVMAP_ITEMSIZE(key_size, val_size) ((key_size) + (val_size))

static inline uint8_t * item_ptr_by_idx_get(const nrf_802154_kvmap_t * p_kvmap, size_t idx)
{
    return ((uint8_t *)(p_kvmap->p_memory)) +
           (idx * NRF_802154_KVMAP_ITEMSIZE(p_kvmap->key_size, p_kvmap->val_size));
}

static void item_value_write(const nrf_802154_kvmap_t * p_kvmap,
                             uint8_t                  * p_item,
                             const void               * p_value)
{
    if (p_kvmap->val_size != 0U)
    {
        memcpy(p_item + p_kvmap->key_size, p_value, p_kvmap->val_size);
    }
}

static size_t item_idx_by_key_search(const nrf_802154_kvmap_t * p_kvmap, const void * p_key)
{
    size_t    item_size = NRF_802154_KVMAP_ITEMSIZE(p_kvmap->key_size, p_kvmap->val_size);
    uint8_t * p_item    = p_kvmap->p_memory;
    size_t    idx;

    /* Linear search */
    for (idx = 0U; idx < p_kvmap->count; ++idx, p_item += item_size)
    {
        if (memcmp(p_item, p_key, p_kvmap->key_size) == 0)
        {
            /* Hit! */
            break;
        }
    }

    return idx;
}

void nrf_802154_kvmap_init(nrf_802154_kvmap_t * p_kvmap,
                           void               * p_memory,
                           size_t               memsize,
                           size_t               key_size,
                           size_t               val_size)
{
    p_kvmap->p_memory = p_memory;
    p_kvmap->capacity = memsize / NRF_802154_KVMAP_ITEMSIZE(key_size, val_size);
    p_kvmap->key_size = key_size;
    p_kvmap->val_size = val_size;
    p_kvmap->count    = 0U;
}

bool nrf_802154_kvmap_add(nrf_802154_kvmap_t * p_kvmap, const void * p_key, const void * p_value)
{
    uint32_t crit_sect = 0UL;
    size_t   idx;
    bool     success = true;

    nrf_802154_serialization_crit_sect_enter(&crit_sect);

    idx = item_idx_by_key_search(p_kvmap, p_key);
    if (idx < p_kvmap->count)
    {
        /* Item already present */
        uint8_t * p_item = item_ptr_by_idx_get(p_kvmap, idx);

        item_value_write(p_kvmap, p_item, p_value);
    }
    else if (p_kvmap->count >= p_kvmap->capacity)
    {
        /* Item not found, but the map is at full capacity. Don't add the item */
        success = false;
    }
    else
    {
        /* Not found, try to add next at p_kvmap->count */
        uint8_t * p_item = item_ptr_by_idx_get(p_kvmap, p_kvmap->count);

        memcpy(p_item, p_key, p_kvmap->key_size);
        item_value_write(p_kvmap, p_item, p_value);

        p_kvmap->count++;
    }

    nrf_802154_serialization_crit_sect_exit(crit_sect);

    return success;
}

bool nrf_802154_kvmap_remove(nrf_802154_kvmap_t * p_kvmap, const void * p_key)
{
    uint32_t crit_sect = 0UL;
    size_t   idx;
    bool     success = true;

    nrf_802154_serialization_crit_sect_enter(&crit_sect);

    idx = item_idx_by_key_search(p_kvmap, p_key);
    if (idx >= p_kvmap->count)
    {
        /* Key not found */
        success = false;
    }
    else
    {
        p_kvmap->count--;
        if (idx < p_kvmap->count)
        {
            const uint8_t * p_last_item = item_ptr_by_idx_get(p_kvmap, p_kvmap->count);
            uint8_t       * p_item      = item_ptr_by_idx_get(p_kvmap, idx);

            memcpy(p_item,
                   p_last_item,
                   NRF_802154_KVMAP_ITEMSIZE(p_kvmap->key_size, p_kvmap->val_size));
        }
        else
        {
            /* We hit last item, no item move necessary */
        }
    }

    nrf_802154_serialization_crit_sect_exit(crit_sect);

    return success;
}

bool nrf_802154_kvmap_search(const nrf_802154_kvmap_t * p_kvmap,
                             const void               * p_key,
                             void                     * p_value)
{
    uint32_t crit_sect = 0UL;
    size_t   idx;
    bool     success = true;

    nrf_802154_serialization_crit_sect_enter(&crit_sect);

    idx = item_idx_by_key_search(p_kvmap, p_key);
    if (idx >= p_kvmap->count)
    {
        /* Key not found */
        success = false;
    }
    else
    {
        const uint8_t * p_item = item_ptr_by_idx_get(p_kvmap, idx);

        /* Copy value associated with the key if requested and values are present */
        if ((p_value != NULL) && (p_kvmap->val_size != 0U))
        {
            memcpy(p_value, p_item + p_kvmap->key_size, p_kvmap->val_size);
        }
    }

    nrf_802154_serialization_crit_sect_exit(crit_sect);

    return success;
}
