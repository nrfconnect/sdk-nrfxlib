/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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
 * @file
 *   This file implements an immediate acknowledgement (Imm-Ack) generator for 802.15.4 radio driver.
 *
 */

#include "nrf_802154_imm_ack_generator.h"

#include <string.h>

#include "nrf_802154_ack_data.h"
#include "nrf_802154_const.h"
#if NRF_802154_IE_WRITER_ENABLED
#include "mac_features/nrf_802154_ie_writer.h"
#endif

#define IMM_ACK_INITIALIZER {IMM_ACK_LENGTH, ACK_HEADER_WITH_PENDING, 0x00, 0x00, 0x00, 0x00}

static uint8_t m_ack_data[IMM_ACK_LENGTH + PHR_SIZE];

void nrf_802154_imm_ack_generator_init(void)
{
    const uint8_t ack_data[] = IMM_ACK_INITIALIZER;

    memcpy(m_ack_data, ack_data, sizeof(ack_data));
}

void nrf_802154_imm_ack_generator_reset(void)
{
    // Intentionally empty
}

uint8_t * nrf_802154_imm_ack_generator_create(
    const nrf_802154_frame_parser_data_t * p_frame_data)
{
    #if NRF_802154_IE_WRITER_ENABLED
    // The IE writer module can be in the IE_WRITER_PREPARE state if
    // the previous transmission failed at an early stage.
    // Reset it, to avoid data corruption in when this ACK is transmitted.
    // Otherwise, the IE writer would commit data in nrf_802154_ie_writer_tx_ack_started_hook
    // regardless if writing of IE elements is needed or not.
    nrf_802154_ie_writer_reset();
    #endif

    if (nrf_802154_frame_parser_parse_level_get(p_frame_data) < PARSE_LEVEL_FULL)
    {
        // The entire frame being acknowledged is necessary to correctly generate Ack
        return NULL;
    }

    const uint8_t * frame_dsn = nrf_802154_frame_parser_dsn_get(p_frame_data);

    if (frame_dsn == NULL)
    {
        return NULL;
    }

    // Set valid sequence number in ACK frame.
    m_ack_data[DSN_OFFSET] = *frame_dsn;

    // Set pending bit in ACK frame.
    if (nrf_802154_ack_data_pending_bit_should_be_set(p_frame_data))
    {
        m_ack_data[FRAME_PENDING_OFFSET] = ACK_HEADER_WITH_PENDING;
    }
    else
    {
        m_ack_data[FRAME_PENDING_OFFSET] = ACK_HEADER_WITHOUT_PENDING;
    }

    return m_ack_data;
}
