/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
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

#include "nrf_802154_security_pib.h"

#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_sl_atomics.h"

#include <string.h>
#include <stdbool.h>
#include "nrf_802154_assert.h"

typedef struct
{
    uint8_t                  key[AES_CCM_KEY_SIZE];
    uint8_t                  id[KEY_ID_MODE_3_SIZE];
    nrf_802154_key_id_mode_t mode;
    uint32_t                 frame_counter;
    bool                     use_global_frame_counter;
    bool                     taken;
} table_entry_t;

static table_entry_t m_key_storage[NRF_802154_SECURITY_KEY_STORAGE_SIZE];
static uint32_t      m_global_frame_counter;

static bool mode_is_valid(nrf_802154_key_id_mode_t mode)
{
    switch (mode)
    {
        case KEY_ID_MODE_0:
        case KEY_ID_MODE_1:
        case KEY_ID_MODE_2:
        case KEY_ID_MODE_3:
            return true;

        default:
            return false;
    }
}

static int id_length_get(nrf_802154_key_id_mode_t mode)
{
    switch (mode)
    {
        case 1:
            return KEY_ID_MODE_1_SIZE;

        case 2:
            return KEY_ID_MODE_2_SIZE;

        case 3:
            return KEY_ID_MODE_3_SIZE;

        default:
            return 0;
    }
}

static bool key_matches(table_entry_t * p_key, nrf_802154_key_id_t * p_id)
{
    if (!p_key->taken)
    {
        return false;
    }

    if (p_key->mode != p_id->mode)
    {
        return false;
    }

    if (p_id->mode == KEY_ID_MODE_0)
    {
        return true;
    }
    else if ((p_id->p_key_id == NULL) ||
             (memcmp(p_id->p_key_id, p_key->id, id_length_get(p_id->mode)) != 0))
    {
        return false;
    }
    else
    {
        return true;
    }
}

static bool key_is_present(nrf_802154_key_id_t * p_id)
{
    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        if (key_matches(&m_key_storage[i], p_id))
        {
            return true;
        }
    }

    return false;
}

nrf_802154_security_error_t nrf_802154_security_pib_init(void)
{
    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        m_key_storage[i].taken = false;
    }

    return NRF_802154_SECURITY_ERROR_NONE;
}

nrf_802154_security_error_t nrf_802154_security_pib_deinit(void)
{
    return NRF_802154_SECURITY_ERROR_NONE;
}

nrf_802154_security_error_t nrf_802154_security_pib_key_store(nrf_802154_key_t * p_key)
{
    NRF_802154_ASSERT(p_key != NULL);

    if (p_key->type != NRF_802154_KEY_CLEARTEXT)
    {
        return NRF_802154_SECURITY_ERROR_TYPE_NOT_SUPPORTED;
    }

    if (!mode_is_valid(p_key->id.mode))
    {
        return NRF_802154_SECURITY_ERROR_MODE_NOT_SUPPORTED;
    }

    if (key_is_present(&p_key->id))
    {
        return NRF_802154_SECURITY_ERROR_ALREADY_PRESENT;
    }

    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        if (m_key_storage[i].taken == false)
        {
            memcpy(m_key_storage[i].key,
                   p_key->value.p_cleartext_key,
                   sizeof(m_key_storage[i].key));
            m_key_storage[i].mode = p_key->id.mode;
            memcpy(m_key_storage[i].id, p_key->id.p_key_id, id_length_get(p_key->id.mode));
            m_key_storage[i].frame_counter            = p_key->frame_counter;
            m_key_storage[i].use_global_frame_counter = p_key->use_global_frame_counter;

            __DMB();

            m_key_storage[i].taken = true;
            return NRF_802154_SECURITY_ERROR_NONE;
        }
    }

    return NRF_802154_SECURITY_ERROR_STORAGE_FULL;
}

nrf_802154_security_error_t nrf_802154_security_pib_key_remove(nrf_802154_key_id_t * p_id)
{
    NRF_802154_ASSERT(p_id != NULL);

    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        if (key_matches(&m_key_storage[i], p_id))
        {
            m_key_storage[i].taken = false;
            return NRF_802154_SECURITY_ERROR_NONE;
        }
    }

    return NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND;
}

void nrf_802154_security_pib_key_remove_all(void)
{
    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        m_key_storage[i].taken = false;
    }
}

nrf_802154_security_error_t nrf_802154_security_pib_key_use(nrf_802154_key_id_t * p_id,
                                                            void                * destination)
{
    NRF_802154_ASSERT(destination != NULL);
    NRF_802154_ASSERT(p_id != NULL);

    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        if (key_matches(&m_key_storage[i], p_id))
        {
            memcpy((uint8_t *)destination, m_key_storage[i].key, sizeof(m_key_storage[i].key));
            return NRF_802154_SECURITY_ERROR_NONE;
        }
    }

    return NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND;
}

void nrf_802154_security_pib_global_frame_counter_set(uint32_t frame_counter)
{
    m_global_frame_counter = frame_counter;
}

void nrf_802154_security_pib_global_frame_counter_set_if_larger(uint32_t frame_counter)
{
    uint32_t fc;

    do
    {
        fc = m_global_frame_counter;

        if (fc >= frame_counter)
        {
            break;
        }

    }
    while (!nrf_802154_sl_atomic_cas_u32(&m_global_frame_counter, &fc, frame_counter));
}

nrf_802154_security_error_t nrf_802154_security_pib_frame_counter_get_next(
    uint32_t            * p_frame_counter,
    nrf_802154_key_id_t * p_id)
{
    NRF_802154_ASSERT(p_frame_counter != NULL);
    NRF_802154_ASSERT(p_id != NULL);

    uint32_t * p_frame_counter_to_use = NULL;
    uint32_t   fc;

    for (uint32_t i = 0; i < NRF_802154_SECURITY_KEY_STORAGE_SIZE; i++)
    {
        if (key_matches(&m_key_storage[i], p_id))
        {
            if (m_key_storage[i].use_global_frame_counter)
            {
                p_frame_counter_to_use = &m_global_frame_counter;
            }
            else
            {
                p_frame_counter_to_use = &m_key_storage[i].frame_counter;
            }
            break;
        }
    }

    if (p_frame_counter_to_use == NULL)
    {
        /* No proper key found. */
        return NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND;
    }

    do
    {
        fc = __LDREXW(p_frame_counter_to_use);

        if (fc == UINT32_MAX)
        {
            __CLREX();
            return NRF_802154_SECURITY_ERROR_FRAME_COUNTER_OVERFLOW;
        }
    }
    while (__STREXW(fc + 1, p_frame_counter_to_use));

    *p_frame_counter = *p_frame_counter_to_use - 1;

    return NRF_802154_SECURITY_ERROR_NONE;
}
