/*
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
 *   This file implements frame parsing utilities for 802.15.4 radio driver.
 *
 * @note This module is based on following assumptions:
 *       a. All received frames contain both source and destination address.
 *       b. All received frames contain destination PAN ID field.
 *       Frames that don't meet these assumptions are dropped.
 */

#include "nrf_802154_frame_parser.h"

#include "nrf_802154_const.h"
#include "nrf_802154_utils_byteorder.h"

/***************************************************************************************************
 * @section Helper functions
 **************************************************************************************************/

// Addressing

static bool src_addr_is_present(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    return nrf_802154_frame_parser_src_addr_type_get(p_parser_data) != SRC_ADDR_TYPE_NONE;
}

static uint8_t src_addr_size_get(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    uint8_t addr_type = nrf_802154_frame_parser_src_addr_type_get(p_parser_data);

    switch (addr_type)
    {
        case SRC_ADDR_TYPE_EXTENDED:
            return EXTENDED_ADDRESS_SIZE;

        case SRC_ADDR_TYPE_SHORT:
            return SHORT_ADDRESS_SIZE;

        case SRC_ADDR_TYPE_NONE:
            return 0;

        default:
            return NRF_802154_FRAME_PARSER_INVALID_OFFSET;
    }
}

static bool dst_addr_is_present(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    return nrf_802154_frame_parser_dst_addr_type_get(p_parser_data) != DEST_ADDR_TYPE_NONE;
}

static uint8_t dst_addr_size_get(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    uint8_t addr_type = nrf_802154_frame_parser_dst_addr_type_get(p_parser_data);

    switch (addr_type)
    {
        case DEST_ADDR_TYPE_EXTENDED:
            return EXTENDED_ADDRESS_SIZE;

        case DEST_ADDR_TYPE_SHORT:
            return SHORT_ADDRESS_SIZE;

        case DEST_ADDR_TYPE_NONE:
            return 0;

        default:
            return NRF_802154_FRAME_PARSER_INVALID_OFFSET;
    }
}

// PAN ID
static bool dst_panid_is_present(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    bool panid_compression = nrf_802154_frame_parser_panid_compression_is_set(p_parser_data);

    switch (nrf_802154_frame_parser_frame_version_get(p_parser_data))
    {
        case FRAME_VERSION_0:
        case FRAME_VERSION_1:
            if (!dst_addr_is_present(p_parser_data))
            {
                return false;
            }

            return true;

        case FRAME_VERSION_2:
        default:
            if (nrf_802154_frame_parser_dst_addr_is_extended(p_parser_data) &&
                nrf_802154_frame_parser_src_addr_is_extended(p_parser_data))
            {
                return panid_compression ? false : true;
            }

            if (src_addr_is_present(p_parser_data) && dst_addr_is_present(p_parser_data))
            {
                return true;
            }

            if (src_addr_is_present(p_parser_data))
            {
                return false;
            }

            if (dst_addr_is_present(p_parser_data))
            {
                return panid_compression ? false : true;
            }

            return panid_compression ? true : false;
    }
}

static bool src_panid_is_present(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    bool panid_compression = nrf_802154_frame_parser_panid_compression_is_set(p_parser_data);

    switch (nrf_802154_frame_parser_frame_version_get(p_parser_data))
    {
        case FRAME_VERSION_0:
        case FRAME_VERSION_1:
            if (!src_addr_is_present(p_parser_data))
            {
                return false;
            }

            return panid_compression ? false : true;

        case FRAME_VERSION_2:
        default:
            if (nrf_802154_frame_parser_dst_addr_is_extended(p_parser_data) &&
                nrf_802154_frame_parser_src_addr_is_extended(p_parser_data))
            {
                return false;
            }

            if (src_addr_is_present(p_parser_data) && dst_addr_is_present(p_parser_data))
            {
                return panid_compression ? false : true;
            }

            if (src_addr_is_present(p_parser_data))
            {
                return panid_compression ? false : true;
            }

            return false;
    }
}

// Security

uint8_t key_source_size_get(uint8_t key_id_mode)
{
    switch (key_id_mode)
    {
        case KEY_ID_MODE_0:
            return KEY_SRC_KEY_ID_MODE_0_SIZE;

        case KEY_ID_MODE_1:
            return KEY_SRC_KEY_ID_MODE_1_SIZE;

        case KEY_ID_MODE_2:
            return KEY_SRC_KEY_ID_MODE_2_SIZE;

        case KEY_ID_MODE_3:
            return KEY_SRC_KEY_ID_MODE_3_SIZE;

        default:
            // Unknown key identifier mode
            return 0;
    }
}

static uint8_t mic_size_get(const nrf_802154_frame_parser_data_t * p_parser_data)
{
    switch (nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_parser_data))
    {
        case SECURITY_LEVEL_MIC_32:
        case SECURITY_LEVEL_ENC_MIC_32:
            return MIC_32_SIZE;

        case SECURITY_LEVEL_MIC_64:
        case SECURITY_LEVEL_ENC_MIC_64:
            return MIC_64_SIZE;

        case SECURITY_LEVEL_MIC_128:
        case SECURITY_LEVEL_ENC_MIC_128:
            return MIC_128_SIZE;

        default:
            return 0;
    }
}

/***************************************************************************************************
 * @section Parsing functions
 **************************************************************************************************/

static bool fcf_parse(nrf_802154_frame_parser_data_t * p_parser_data)
{
    uint8_t offset = PHR_SIZE + FCF_SIZE;
    uint8_t addr_size;

    if (offset > p_parser_data->valid_data_len)
    {
        // Not enough valid data to parse the FCF
        return false;
    }

    if (nrf_802154_frame_parser_dsn_suppress_bit_is_set(p_parser_data) == false)
    {
        offset += DSN_SIZE;
    }

    if (dst_panid_is_present(p_parser_data))
    {
        p_parser_data->mhr.dst.panid_offset = offset;
        offset                             += PAN_ID_SIZE;
    }

    if (dst_addr_is_present(p_parser_data))
    {
        p_parser_data->mhr.dst.addr_offset = offset;
    }

    addr_size = dst_addr_size_get(p_parser_data);

    if (addr_size == NRF_802154_FRAME_PARSER_INVALID_OFFSET)
    {
        return false;
    }

    p_parser_data->helper.dst_addr_size             = addr_size;
    offset                                         += addr_size;
    p_parser_data->helper.dst_addressing_end_offset = offset;

    if (src_panid_is_present(p_parser_data))
    {
        p_parser_data->mhr.src.panid_offset = offset;
        offset                             += PAN_ID_SIZE;
    }

    if (src_addr_is_present(p_parser_data))
    {
        p_parser_data->mhr.src.addr_offset = offset;
    }

    addr_size = src_addr_size_get(p_parser_data);

    if (addr_size == NRF_802154_FRAME_PARSER_INVALID_OFFSET)
    {
        return false;
    }

    p_parser_data->helper.src_addr_size = addr_size;
    offset                             += addr_size;

    p_parser_data->helper.addressing_end_offset = offset;

    return true;
}

static bool sec_ctrl_parse(nrf_802154_frame_parser_data_t * p_parser_data)
{
    uint8_t offset = p_parser_data->helper.addressing_end_offset;
    uint8_t key_id_mode;
    uint8_t key_src_size;

    if (nrf_802154_frame_parser_security_enabled_bit_is_set(p_parser_data) == false)
    {
        p_parser_data->helper.aux_sec_hdr_end_offset = offset;
        p_parser_data->helper.mic_size               = 0;
        return true;
    }

    if ((offset + SECURITY_CONTROL_SIZE) > p_parser_data->valid_data_len)
    {
        return false;
    }

    p_parser_data->mhr.aux_sec_hdr.sec_ctrl_offset = offset;
    offset += SECURITY_CONTROL_SIZE;

    if (nrf_802154_frame_parser_sec_ctrl_fc_suppress_bit_is_set(p_parser_data) == false)
    {
        p_parser_data->mhr.aux_sec_hdr.frame_counter_offset = offset;
        offset += FRAME_COUNTER_SIZE;
    }

    key_id_mode  = nrf_802154_frame_parser_sec_ctrl_key_id_mode_get(p_parser_data);
    key_src_size = key_source_size_get(key_id_mode);

    if (key_id_mode != KEY_ID_MODE_0)
    {
        p_parser_data->mhr.aux_sec_hdr.key_id_offset = offset;

        if (key_src_size > 0)
        {
            p_parser_data->mhr.aux_sec_hdr.key_src_offset = offset;
            offset += key_src_size;
        }

        p_parser_data->helper.key_src_size            = key_src_size;
        p_parser_data->mhr.aux_sec_hdr.key_idx_offset = offset;
        offset += KEY_IDX_SIZE;
    }

    p_parser_data->helper.mic_size               = mic_size_get(p_parser_data);
    p_parser_data->helper.aux_sec_hdr_end_offset = offset;

    return true;
}

static bool full_parse(nrf_802154_frame_parser_data_t * p_parser_data)
{
    uint8_t         offset      = p_parser_data->helper.aux_sec_hdr_end_offset;
    uint8_t         psdu_length = nrf_802154_frame_parser_frame_length_get(p_parser_data);
    const uint8_t * p_ie_header;
    const uint8_t * p_end_addr;
    const uint8_t * p_iterator;

    if (((psdu_length + PHR_SIZE) != p_parser_data->valid_data_len) ||
        (psdu_length > MAX_PACKET_SIZE))
    {
        return false;
    }

    if (nrf_802154_frame_parser_ie_present_bit_is_set(p_parser_data))
    {
        p_parser_data->mhr.header_ie_offset = offset;

        p_ie_header = &p_parser_data->p_frame[offset];
        p_end_addr  = nrf_802154_frame_parser_mfr_get(p_parser_data) - mic_size_get(p_parser_data);
        p_iterator  = nrf_802154_frame_parser_header_ie_iterator_begin(p_ie_header);

        while (!nrf_802154_frame_parser_ie_iterator_end(p_iterator, p_end_addr))
        {
            p_iterator = nrf_802154_frame_parser_ie_iterator_next(p_iterator);

            if (p_iterator > p_end_addr)
            {
                // Boundary check failed
                return false;
            }
            else if (p_iterator == p_end_addr)
            {
                // End of frame; IE header has no termination.
                offset = p_iterator - p_parser_data->p_frame;
                break;
            }
            else if (nrf_802154_frame_parser_ie_iterator_end(p_iterator, p_end_addr))
            {
                // End of IE header; termination reached.
                offset = nrf_802154_frame_parser_ie_content_address_get(p_iterator) -
                         p_parser_data->p_frame;
                break;
            }
            else
            {
                // Intentionally empty
            }
        }
    }

    if (offset != nrf_802154_frame_parser_mfr_offset_get(p_parser_data))
    {
        p_parser_data->mac_payload.mac_payload_offset = offset;
    }

    return true;
}

static bool level_is_elevated(nrf_802154_frame_parser_data_t * p_parser_data,
                              nrf_802154_frame_parser_level_t  requested_parse_level)
{
    return requested_parse_level > p_parser_data->parse_level;
}

static bool parse_state_advance(nrf_802154_frame_parser_data_t * p_parser_data,
                                nrf_802154_frame_parser_level_t  requested_parse_level)
{
    bool                            result;
    nrf_802154_frame_parser_level_t next_level;

    do
    {
        result = false;

        switch (p_parser_data->parse_level)
        {
            case PARSE_LEVEL_NONE:
                if (level_is_elevated(p_parser_data, requested_parse_level))
                {
                    result     = fcf_parse(p_parser_data);
                    next_level = PARSE_LEVEL_FCF_OFFSETS;
                }
                break;

            case PARSE_LEVEL_FCF_OFFSETS:
                if (p_parser_data->valid_data_len >=
                    p_parser_data->helper.dst_addressing_end_offset)
                {
                    result     = true;
                    next_level = PARSE_LEVEL_DST_ADDRESSING_END;
                }
                break;

            case PARSE_LEVEL_DST_ADDRESSING_END:
                if (p_parser_data->valid_data_len >= p_parser_data->helper.addressing_end_offset)
                {
                    result     = true;
                    next_level = PARSE_LEVEL_ADDRESSING_END;
                }
                break;

            case PARSE_LEVEL_ADDRESSING_END:
                if (level_is_elevated(p_parser_data, requested_parse_level))
                {
                    result     = sec_ctrl_parse(p_parser_data);
                    next_level = PARSE_LEVEL_SEC_CTRL_OFFSETS;
                }
                break;

            case PARSE_LEVEL_SEC_CTRL_OFFSETS:
                if (p_parser_data->valid_data_len >= p_parser_data->helper.aux_sec_hdr_end_offset)
                {
                    result     = true;
                    next_level = PARSE_LEVEL_AUX_SEC_HDR_END;
                }
                break;

            case PARSE_LEVEL_AUX_SEC_HDR_END:
                if (level_is_elevated(p_parser_data, requested_parse_level))
                {
                    result     = full_parse(p_parser_data);
                    next_level = PARSE_LEVEL_FULL;
                }
                break;

            case PARSE_LEVEL_FULL:
                return true;

            default:
                NRF_802154_ASSERT(false);
                return false;
        }

        if (result)
        {
            p_parser_data->parse_level = next_level;
        }
    }
    while (result);

    return p_parser_data->parse_level >= requested_parse_level;
}

bool nrf_802154_frame_parser_data_init(const uint8_t                  * p_frame,
                                       uint8_t                          valid_data_len,
                                       nrf_802154_frame_parser_level_t  requested_parse_level,
                                       nrf_802154_frame_parser_data_t * p_parser_data)
{
    if (p_frame == NULL)
    {
        return false;
    }

    p_parser_data->p_frame        = p_frame;
    p_parser_data->valid_data_len = valid_data_len;
    p_parser_data->parse_level    = PARSE_LEVEL_NONE;

    memset(&p_parser_data->mhr, NRF_802154_FRAME_PARSER_INVALID_OFFSET, sizeof(p_parser_data->mhr));
    memset(&p_parser_data->mac_payload,
           NRF_802154_FRAME_PARSER_INVALID_OFFSET,
           sizeof(p_parser_data->mac_payload));
    memset(&p_parser_data->helper,
           NRF_802154_FRAME_PARSER_INVALID_OFFSET,
           sizeof(p_parser_data->helper));

    return parse_state_advance(p_parser_data, requested_parse_level);
}

bool nrf_802154_frame_parser_valid_data_extend(
    nrf_802154_frame_parser_data_t * p_parser_data,
    uint8_t                          valid_data_len,
    nrf_802154_frame_parser_level_t  requested_parse_level)
{
    if (valid_data_len > p_parser_data->valid_data_len)
    {
        p_parser_data->valid_data_len = valid_data_len;
    }

    return parse_state_advance(p_parser_data, requested_parse_level);
}
