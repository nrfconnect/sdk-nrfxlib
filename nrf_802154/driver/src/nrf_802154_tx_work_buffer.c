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

#include "nrf_802154_assert.h"
#include <stddef.h>
#include <string.h>

#include "nrf_802154_const.h"
#include "nrf_802154_tx_work_buffer.h"

static uint8_t   m_work_buffer[MAX_PACKET_SIZE + PHR_SIZE]; ///< Work buffer.
static uint8_t * mp_original_frame;                         ///< Pointer to the original frame the work buffer is currently bound to.
static uint8_t   m_plain_text_offset;                       ///< Offset of encryption plain text.
static bool      m_is_secured;                              ///< Flag that indicates if work buffer has been successfully secured.
static bool      m_is_dynamic_data_updated;                 ///< Flag that indicates if work buffer has had dynamic data successfully updated.

void nrf_802154_tx_work_buffer_reset(const nrf_802154_transmitted_frame_props_t * p_frame_props)
{
    mp_original_frame   = NULL;
    m_plain_text_offset = 0;

    if (p_frame_props == NULL)
    {
        m_is_secured              = false;
        m_is_dynamic_data_updated = false;
    }
    else
    {
        m_is_secured              = p_frame_props->is_secured;
        m_is_dynamic_data_updated = p_frame_props->dynamic_data_is_set;
    }
}

uint8_t * nrf_802154_tx_work_buffer_enable_for(uint8_t * p_original_frame)
{
    mp_original_frame = p_original_frame;
    return m_work_buffer;
}

const uint8_t * nrf_802154_tx_work_buffer_get(const uint8_t * p_original_frame)
{
    return mp_original_frame ? m_work_buffer : p_original_frame;
}

void nrf_802154_tx_work_buffer_original_frame_update(
    uint8_t                              * p_original_frame,
    nrf_802154_transmitted_frame_props_t * p_frame_props)
{
    NRF_802154_ASSERT(p_frame_props != NULL);

    p_frame_props->is_secured          = m_is_secured;
    p_frame_props->dynamic_data_is_set = m_is_dynamic_data_updated;

    if (mp_original_frame == NULL)
    {
        return;
    }

    uint8_t work_buffer_len = m_work_buffer[PHR_OFFSET] + PHR_SIZE;

    if (m_is_dynamic_data_updated && m_is_secured)
    {
        memcpy(p_original_frame, m_work_buffer, work_buffer_len);
    }
    else if (m_is_dynamic_data_updated)
    {
        memcpy(p_original_frame, m_work_buffer, m_plain_text_offset);
    }
    else if (m_is_secured)
    {
        memcpy(p_original_frame, m_work_buffer, work_buffer_len - m_plain_text_offset);
    }
    else
    {
        // Intentionally empty.
    }
}

void nrf_802154_tx_work_buffer_is_secured_set(void)
{
    m_is_secured = true;
}

void nrf_802154_tx_work_buffer_is_dynamic_data_updated_set(void)
{
    m_is_dynamic_data_updated = true;
}

void nrf_802154_tx_work_buffer_plain_text_offset_set(uint8_t offset)
{
    m_plain_text_offset = offset;
}
