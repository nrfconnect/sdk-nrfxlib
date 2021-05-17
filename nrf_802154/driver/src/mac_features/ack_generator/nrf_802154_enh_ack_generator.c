/*
 * Copyright (c) 2018 - 2021, Nordic Semiconductor ASA
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
 *   This file implements an enhanced acknowledgement (Enh-Ack) generator for 802.15.4 radio driver.
 *
 */

#include "nrf_802154_enh_ack_generator.h"

#include <assert.h>
#include <string.h>

#include "mac_features/nrf_802154_frame_parser.h"
#include "mac_features/nrf_802154_ie_writer.h"
#include "mac_features/nrf_802154_security_pib.h"
#include "nrf_802154_ack_data.h"
#include "nrf_802154_encrypt.h"
#include "nrf_802154_const.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_utils_byteorder.h"

#define ENH_ACK_MAX_SIZE MAX_PACKET_SIZE

static uint8_t m_ack_data[ENH_ACK_MAX_SIZE + PHR_SIZE];

static void ack_buffer_clear(void)
{
    memset(m_ack_data, 0, FCF_SIZE + PHR_SIZE);
}

static void sequence_number_set(const uint8_t * p_frame)
{
    if (!nrf_802154_frame_parser_dsn_suppress_bit_is_set(p_frame))
    {
        m_ack_data[DSN_OFFSET] = p_frame[DSN_OFFSET];
    }
}

/***************************************************************************************************
 * @section Frame control field functions
 **************************************************************************************************/

static void fcf_frame_type_set(void)
{
    m_ack_data[FRAME_TYPE_OFFSET] |= FRAME_TYPE_ACK;
}

static void fcf_security_enabled_set(const uint8_t * p_frame)
{
    m_ack_data[SECURITY_ENABLED_OFFSET] |=
        (p_frame[SECURITY_ENABLED_OFFSET] & SECURITY_ENABLED_BIT);
}

static void fcf_frame_pending_set(const uint8_t * p_frame)
{
    if (nrf_802154_ack_data_pending_bit_should_be_set(p_frame))
    {
        m_ack_data[FRAME_PENDING_OFFSET] |= FRAME_PENDING_BIT;
    }
}

static void fcf_panid_compression_set(const uint8_t * p_frame)
{
    if (p_frame[PAN_ID_COMPR_OFFSET] & PAN_ID_COMPR_MASK)
    {
        m_ack_data[PAN_ID_COMPR_OFFSET] |= PAN_ID_COMPR_MASK;
    }
}

static void fcf_sequence_number_suppression_set(const uint8_t * p_frame)
{
    if (nrf_802154_frame_parser_dsn_suppress_bit_is_set(p_frame))
    {
        m_ack_data[DSN_SUPPRESS_OFFSET] |= DSN_SUPPRESS_BIT;
    }
}

static void fcf_ie_present_set(const uint8_t * p_ie_data)
{
    if (p_ie_data != NULL)
    {
        m_ack_data[IE_PRESENT_OFFSET] |= IE_PRESENT_BIT;
    }
}

static void fcf_dst_addressing_mode_set(const uint8_t * p_frame)
{
    if (nrf_802154_frame_parser_src_addr_is_extended(p_frame))
    {
        m_ack_data[DEST_ADDR_TYPE_OFFSET] |= DEST_ADDR_TYPE_EXTENDED;
    }
    else if (nrf_802154_frame_parser_src_addr_is_short(p_frame))
    {
        m_ack_data[DEST_ADDR_TYPE_OFFSET] |= DEST_ADDR_TYPE_SHORT;
    }
    else
    {
        m_ack_data[DEST_ADDR_TYPE_OFFSET] |= DEST_ADDR_TYPE_NONE;
    }
}

static void fcf_src_addressing_mode_set(void)
{
    m_ack_data[SRC_ADDR_TYPE_OFFSET] |= SRC_ADDR_TYPE_NONE;
}

static void fcf_frame_version_set(void)
{
    m_ack_data[FRAME_VERSION_OFFSET] |= FRAME_VERSION_2;
}

static void frame_control_set(const uint8_t                      * p_frame,
                              const uint8_t                      * p_ie_data,
                              nrf_802154_frame_parser_mhr_data_t * p_ack_offsets)
{
    bool parse_results;

    fcf_frame_type_set();
    fcf_security_enabled_set(p_frame);
    fcf_frame_pending_set(p_frame);
    fcf_panid_compression_set(p_frame);
    fcf_sequence_number_suppression_set(p_frame);
    fcf_ie_present_set(p_ie_data);
    fcf_dst_addressing_mode_set(p_frame);
    fcf_frame_version_set();
    fcf_src_addressing_mode_set();

    parse_results = nrf_802154_frame_parser_mhr_parse(m_ack_data, p_ack_offsets);
    assert(parse_results);
    (void)parse_results;

    m_ack_data[PHR_OFFSET] = p_ack_offsets->addressing_end_offset - PHR_SIZE + FCS_SIZE;
}

/***************************************************************************************************
 * @section Addressing fields functions
 **************************************************************************************************/

static void destination_set(const nrf_802154_frame_parser_mhr_data_t * p_frame,
                            const nrf_802154_frame_parser_mhr_data_t * p_ack)
{
    // Fill the Ack destination PAN ID field.
    if (p_ack->p_dst_panid != NULL)
    {
        const uint8_t * p_dst_panid;

        if (p_frame->p_src_panid != NULL)
        {
            p_dst_panid = p_frame->p_src_panid;
        }
        else if (p_frame->p_dst_panid != NULL)
        {
            p_dst_panid = p_frame->p_dst_panid;
        }
        else
        {
            p_dst_panid = nrf_802154_pib_pan_id_get();
        }

        memcpy((uint8_t *)p_ack->p_dst_panid, p_dst_panid, PAN_ID_SIZE);
    }

    // Fill the Ack destination address field.
    if (p_frame->p_src_addr != NULL)
    {
        assert(p_ack->p_dst_addr != NULL);
        assert(p_ack->dst_addr_size == p_frame->src_addr_size);

        memcpy((uint8_t *)p_ack->p_dst_addr, p_frame->p_src_addr, p_frame->src_addr_size);
    }
}

static void source_set(void)
{
    // Intentionally empty: source address type is None.
}

/***************************************************************************************************
 * @section Auxiliary security header functions
 **************************************************************************************************/

static uint8_t security_header_size(const nrf_802154_frame_parser_mhr_data_t * p_frame)
{
    uint8_t sec_hdr_size = SECURITY_CONTROL_SIZE;

    if (((*p_frame->p_sec_ctrl) & FRAME_COUNTER_SUPPRESS_BIT) == 0)
    {
        sec_hdr_size += FRAME_COUNTER_SIZE;
    }

    switch ((*p_frame->p_sec_ctrl) & KEY_ID_MODE_MASK)
    {
        case KEY_ID_MODE_1_MASK:
            sec_hdr_size += KEY_ID_MODE_1_SIZE;
            break;

        case KEY_ID_MODE_2_MASK:
            sec_hdr_size += KEY_ID_MODE_2_SIZE;
            break;

        case KEY_ID_MODE_3_MASK:
            sec_hdr_size += KEY_ID_MODE_3_SIZE;
            break;

        default:
            break;
    }

    return sec_hdr_size;
}

static void security_control_set(const nrf_802154_frame_parser_mhr_data_t * p_frame,
                                 const nrf_802154_frame_parser_mhr_data_t * p_ack)
{
    assert(p_frame->p_sec_ctrl != NULL);

    // All the bits in the security control byte can be copied.
    *(uint8_t *)p_ack->p_sec_ctrl = *p_frame->p_sec_ctrl;

    m_ack_data[PHR_OFFSET] += SECURITY_CONTROL_SIZE;
}

static const uint8_t * security_key_id_set(const nrf_802154_frame_parser_mhr_data_t * p_frame,
                                           const nrf_802154_frame_parser_mhr_data_t * p_ack,
                                           bool                                       fc_suppressed,
                                           const uint8_t                           ** p_sec_end)
{
    const uint8_t * p_frame_key_id;
    const uint8_t * p_ack_key_id;
    uint8_t         key_id_size = 0;

    p_frame_key_id = p_frame->p_sec_ctrl + SECURITY_CONTROL_SIZE;
    p_ack_key_id   = p_ack->p_sec_ctrl + SECURITY_CONTROL_SIZE;

    if (!fc_suppressed)
    {
        p_frame_key_id += FRAME_COUNTER_SIZE;
        p_ack_key_id   += FRAME_COUNTER_SIZE;
    }

    switch ((*p_ack->p_sec_ctrl) & KEY_ID_MODE_MASK)
    {
        case KEY_ID_MODE_1_MASK:
            key_id_size = KEY_ID_MODE_1_SIZE;
            break;

        case KEY_ID_MODE_2_MASK:
            key_id_size = KEY_ID_MODE_2_SIZE;
            break;

        case KEY_ID_MODE_3_MASK:
            key_id_size = KEY_ID_MODE_3_SIZE;
            break;

        default:
            break;
    }

    if (0 != key_id_size)
    {
        memcpy((uint8_t *)p_ack_key_id, p_frame_key_id, key_id_size);
        m_ack_data[PHR_OFFSET] += key_id_size;
    }

    switch (*(p_ack->p_sec_ctrl) & SECURITY_LEVEL_MASK)
    {
        case SECURITY_LEVEL_MIC_32:
        case SECURITY_LEVEL_ENC_MIC_32:
            m_ack_data[PHR_OFFSET] += MIC_32_SIZE;
            break;

        case SECURITY_LEVEL_MIC_64:
        case SECURITY_LEVEL_ENC_MIC_64:
            m_ack_data[PHR_OFFSET] += MIC_64_SIZE;
            break;

        case SECURITY_LEVEL_MIC_128:
        case SECURITY_LEVEL_ENC_MIC_128:
            m_ack_data[PHR_OFFSET] += MIC_128_SIZE;
            break;

        default:
            break;
    }

    *p_sec_end = p_ack_key_id + key_id_size;

    return (0 == key_id_size) ? NULL : p_ack_key_id;
}

static bool frame_counter_set(const nrf_802154_frame_parser_mhr_data_t * p_ack_pdata,
                              const uint8_t                            * p_ack_key_id)
{
    nrf_802154_key_id_t ack_key_id;
    uint32_t            new_fc_value;

    ack_key_id.mode     = ((*p_ack_pdata->p_sec_ctrl) & KEY_ID_MODE_MASK) >> KEY_ID_MODE_BIT_OFFSET;
    ack_key_id.p_key_id = (uint8_t *)p_ack_key_id;

    if (NRF_802154_SECURITY_ERROR_NONE !=
        nrf_802154_security_pib_frame_counter_get_next(&new_fc_value, &ack_key_id))
    {
        return false;
    }

    // Set the frame counter value in security header of the ACK frame
    host_32_to_little(new_fc_value,
                      (uint8_t *)(p_ack_pdata->p_sec_ctrl + SECURITY_CONTROL_SIZE));

    m_ack_data[PHR_OFFSET] += FRAME_COUNTER_SIZE;

    return true;
}

static bool security_header_set(const nrf_802154_frame_parser_mhr_data_t * p_frame,
                                const nrf_802154_frame_parser_mhr_data_t * p_ack,
                                const uint8_t                           ** p_sec_end)
{
    bool security_header_prepared;

    if (((*p_frame->p_sec_ctrl) & SECURITY_LEVEL_MASK) == 0)
    {
        // The security level value is zero, therefore no auxiliary security header processing
        // is performed according to 802.15.4 specification. This also applies to the frame counter,
        // the value of which is left as it is in the message to which the ACK responds.
        // The entire auxiliary security header content is simply copied to ACK.
        uint8_t sec_hdr_size = security_header_size(p_frame);

        memcpy((uint8_t *)p_ack->p_sec_ctrl, p_frame->p_sec_ctrl, sec_hdr_size);
        m_ack_data[PHR_OFFSET]  += sec_hdr_size;
        security_header_prepared = true;
    }
    else
    {
        bool            fc_suppressed;
        const uint8_t * p_ack_key_id;

        security_control_set(p_frame, p_ack);

        // Frame counter is set by MAC layer when the frame is encrypted.
        fc_suppressed = ((*p_ack->p_sec_ctrl) & FRAME_COUNTER_SUPPRESS_BIT);

        p_ack_key_id = security_key_id_set(p_frame, p_ack, fc_suppressed, p_sec_end);

        if (fc_suppressed)
        {
            // There is no frame counter filed, so all done here.
            security_header_prepared = true;
        }
        else
        {
            security_header_prepared = frame_counter_set(p_ack, p_ack_key_id);
        }
    }

    return security_header_prepared;
}

/***************************************************************************************************
 * @section Information Elements
 **************************************************************************************************/

static void ie_header_set(const uint8_t * p_ie_data, uint8_t ie_data_len, const uint8_t * p_sec_end)
{
    uint8_t * p_ack_ie = (uint8_t *)p_sec_end;

    if (p_ie_data == NULL)
    {
        return;
    }

    assert(p_ack_ie != NULL);

    memcpy(p_ack_ie, p_ie_data, ie_data_len);
    m_ack_data[PHR_OFFSET] += ie_data_len;

#if NRF_802154_IE_WRITER_ENABLED
    nrf_802154_ie_writer_prepare(p_ack_ie, p_ack_ie + ie_data_len);
#endif
}

static void ie_header_terminate(const nrf_802154_frame_parser_mhr_data_t * p_ack,
                                const uint8_t                            * p_sec_end,
                                const uint8_t                            * p_ie_data,
                                uint8_t                                    ie_data_len)
{
    if (p_ie_data == NULL)
    {
        // No IEs to terminate.
        return;
    }

    if ((p_ack->p_sec_ctrl == NULL) || ((*p_ack->p_sec_ctrl & SECURITY_LEVEL_MASK) == 0))
    {
        // This code assumes that neither regular frame payload nor Payload IEs can be set by the
        // driver. Therefore without security, the Ack has no payload, so termination is not necessary.
        return;
    }

    uint8_t * p_ack_ie = (uint8_t *)p_sec_end;
    uint8_t   ie_hdr_term[IE_HEADER_SIZE];

    assert(p_ack_ie != NULL);

    host_16_to_little((IE_HT2) << IE_HEADER_ELEMENT_ID_OFFSET, ie_hdr_term);

    memcpy(p_ack_ie + ie_data_len, ie_hdr_term, sizeof(ie_hdr_term));
    m_ack_data[PHR_OFFSET] += sizeof(ie_hdr_term);
}

/***************************************************************************************************
 * @section Authentication and encryption transformation
 **************************************************************************************************/

static bool encryption_prepare(const nrf_802154_frame_parser_mhr_data_t * p_ack)
{
#if NRF_802154_ENCRYPTION_ENABLED
    if (p_ack->p_sec_ctrl == NULL)
    {
        return true;
    }

    if ((*(p_ack->p_sec_ctrl) & SECURITY_LEVEL_MASK) == 0)
    {
        return true;
    }

    return nrf_802154_encrypt_ack_prepare(m_ack_data);
#else // NRF_802154_ENCRYPTION_ENABLED
    return true;
#endif  // NRF_802154_ENCRYPTION_ENABLED
}

/***************************************************************************************************
 * @section Public API implementation
 **************************************************************************************************/

void nrf_802154_enh_ack_generator_init(void)
{
    // Intentionally empty.
}

const uint8_t * nrf_802154_enh_ack_generator_create(const uint8_t * p_frame)
{
    nrf_802154_frame_parser_mhr_data_t frame_offsets;
    nrf_802154_frame_parser_mhr_data_t ack_offsets;
    const uint8_t                    * p_sec_end    = NULL;
    bool                               parse_result = nrf_802154_frame_parser_mhr_parse(p_frame,
                                                                                        &frame_offsets);

    if (!parse_result)
    {
        return NULL;
    }

    uint8_t         ie_data_len = 0;
    const uint8_t * p_ie_data   = nrf_802154_ack_data_ie_get(
        frame_offsets.p_src_addr,
        frame_offsets.src_addr_size == EXTENDED_ADDRESS_SIZE,
        &ie_data_len);

    // Clear previously created ACK.
    ack_buffer_clear();

    // Set Frame Control field bits.
    frame_control_set(p_frame, p_ie_data, &ack_offsets);

    // Set valid sequence number in ACK frame.
    sequence_number_set(p_frame);

    // Set destination address and PAN ID.
    destination_set(&frame_offsets, &ack_offsets);

    // Set source address and PAN ID.
    source_set();

    // Attempt to set auxiliary security header.
    if (ack_offsets.p_sec_ctrl == NULL)
    {
        // There is no auxiliary security header.
        p_sec_end = &m_ack_data[ack_offsets.addressing_end_offset];
    }
    else if (security_header_set(&frame_offsets, &ack_offsets, &p_sec_end))
    {
        // Security header has been set successfully: intentionally empty.
    }
    else
    {
        // Failure to set auxiliary security header: The ACK cannot be created.
        ack_buffer_clear();
        return NULL;
    }

    // Set IE header.
    ie_header_set(p_ie_data, ie_data_len, p_sec_end);

    // Terminate the IE header if needed.
    ie_header_terminate(&ack_offsets, p_sec_end, p_ie_data, ie_data_len);

    // Prepare encryption.
    if (!encryption_prepare(&ack_offsets))
    {
        // Failure to prepare encryption even though it's required. The ACK cannot be created.
        ack_buffer_clear();
        return NULL;
    }

    return m_ack_data;
}

#ifdef TEST
void nrf_802154_enh_ack_generator_module_reset(void)
{
    memset(m_ack_data, 0, sizeof(m_ack_data));
}

#endif // TEST
