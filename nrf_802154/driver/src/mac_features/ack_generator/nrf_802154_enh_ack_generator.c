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
 *   This file implements an enhanced acknowledgement (Enh-Ack) generator for 802.15.4 radio driver.
 *
 */

#include "nrf_802154_enh_ack_generator.h"

#include "nrf_802154_assert.h"
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

typedef enum
{
    ACK_STATE_RESET,
    ACK_STATE_INVALID,
    ACK_STATE_PROCESSING,
    ACK_STATE_COMPLETE,
} ack_state_t;

static ack_state_t m_ack_state;

static uint8_t                        m_ack[ENH_ACK_MAX_SIZE + PHR_SIZE];
static nrf_802154_frame_parser_data_t m_ack_data;
static const uint8_t                * mp_ie_data;
static uint8_t                        m_ie_data_len;

static void ack_state_set(ack_state_t state_to_set)
{
    m_ack_state = state_to_set;
}

static inline ack_state_t ack_state_get(void)
{
    return m_ack_state;
}

static uint8_t sequence_number_set(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    const uint8_t * p_frame_dsn = nrf_802154_frame_parser_dsn_get(p_frame_data);

    if (p_frame_dsn != NULL)
    {
        m_ack[DSN_OFFSET] = *p_frame_dsn;

        return DSN_SIZE;
    }

    return 0U;
}

/***************************************************************************************************
 * @section Frame control field functions
 **************************************************************************************************/

static void fcf_frame_type_set(void)
{
    m_ack[FRAME_TYPE_OFFSET] |= FRAME_TYPE_ACK;
}

static void fcf_security_enabled_set(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    if (nrf_802154_frame_parser_security_enabled_bit_is_set(p_frame_data))
    {
        m_ack[SECURITY_ENABLED_OFFSET] |= SECURITY_ENABLED_BIT;
    }
}

static void fcf_frame_pending_set(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    if (nrf_802154_ack_data_pending_bit_should_be_set(p_frame_data))
    {
        m_ack[FRAME_PENDING_OFFSET] |= FRAME_PENDING_BIT;
    }
}

static void fcf_panid_compression_set(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    if (nrf_802154_frame_parser_panid_compression_is_set(p_frame_data))
    {
        m_ack[PAN_ID_COMPR_OFFSET] |= PAN_ID_COMPR_MASK;
    }
}

static void fcf_sequence_number_suppression_set(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    if (nrf_802154_frame_parser_dsn_suppress_bit_is_set(p_frame_data))
    {
        m_ack[DSN_SUPPRESS_OFFSET] |= DSN_SUPPRESS_BIT;
    }
}

static void fcf_ie_present_set(bool ie_present)
{
    if (ie_present)
    {
        m_ack[IE_PRESENT_OFFSET] |= IE_PRESENT_BIT;
    }
}

static void fcf_dst_addressing_mode_set(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    if (nrf_802154_frame_parser_src_addr_is_extended(p_frame_data))
    {
        m_ack[DEST_ADDR_TYPE_OFFSET] |= DEST_ADDR_TYPE_EXTENDED;
    }
    else if (nrf_802154_frame_parser_src_addr_is_short(p_frame_data))
    {
        m_ack[DEST_ADDR_TYPE_OFFSET] |= DEST_ADDR_TYPE_SHORT;
    }
    else
    {
        m_ack[DEST_ADDR_TYPE_OFFSET] |= DEST_ADDR_TYPE_NONE;
    }
}

static void fcf_src_addressing_mode_set(void)
{
    m_ack[SRC_ADDR_TYPE_OFFSET] |= SRC_ADDR_TYPE_NONE;
}

static void fcf_frame_version_set(void)
{
    m_ack[FRAME_VERSION_OFFSET] |= FRAME_VERSION_2;
}

static uint8_t frame_control_set(const nrf_802154_frame_parser_data_t * p_frame_data,
                                 bool                                   ie_present)
{
    nrf_802154_frame_parser_level_t level = nrf_802154_frame_parser_parse_level_get(p_frame_data);

    if (level >= PARSE_LEVEL_FCF_OFFSETS)
    {
        fcf_frame_type_set();
        fcf_security_enabled_set(p_frame_data);
        fcf_panid_compression_set(p_frame_data);
        fcf_sequence_number_suppression_set(p_frame_data);
        fcf_ie_present_set(ie_present);
        fcf_dst_addressing_mode_set(p_frame_data);
        fcf_frame_version_set();
        fcf_src_addressing_mode_set();
    }

    if (level >= PARSE_LEVEL_FULL)
    {
        // As some frame pending bit setting algorithms depend on MAC payload,
        // the entire frame must be known to set this field.
        fcf_frame_pending_set(p_frame_data);
    }

    return FCF_SIZE;
}

/***************************************************************************************************
 * @section Addressing fields functions
 **************************************************************************************************/

static uint8_t destination_set(const nrf_802154_frame_parser_data_t * p_frame_data,
                               nrf_802154_frame_parser_data_t       * p_ack_data)
{
    uint8_t   bytes_written   = 0U;
    uint8_t * p_ack_dst_panid = (uint8_t *)nrf_802154_frame_parser_dst_panid_get(p_ack_data);
    uint8_t * p_ack_dst_addr  = (uint8_t *)nrf_802154_frame_parser_dst_addr_get(p_ack_data);

    const uint8_t * p_frame_src_panid = nrf_802154_frame_parser_src_panid_get(p_frame_data);
    const uint8_t * p_frame_dst_panid = nrf_802154_frame_parser_dst_panid_get(p_frame_data);
    const uint8_t * p_frame_src_addr  = nrf_802154_frame_parser_src_addr_get(p_frame_data);

    uint8_t src_addr_size = nrf_802154_frame_parser_src_addr_size_get(p_frame_data);

    // Fill the Ack destination PAN ID field.
    if (p_ack_dst_panid != NULL)
    {
        const uint8_t * p_dst_panid;

        if (p_frame_src_panid != NULL)
        {
            p_dst_panid = p_frame_src_panid;
        }
        else if (p_frame_dst_panid != NULL)
        {
            p_dst_panid = p_frame_dst_panid;
        }
        else
        {
            p_dst_panid = nrf_802154_pib_pan_id_get();
        }

        memcpy(p_ack_dst_panid, p_dst_panid, PAN_ID_SIZE);
        bytes_written += PAN_ID_SIZE;
    }

    // Fill the Ack destination address field.
    if ((p_ack_dst_addr != NULL) && (p_frame_src_addr != NULL))
    {
        NRF_802154_ASSERT(nrf_802154_frame_parser_dst_addr_is_extended(p_ack_data) ==
                          nrf_802154_frame_parser_src_addr_is_extended(p_frame_data));

        memcpy(p_ack_dst_addr, p_frame_src_addr, src_addr_size);
        bytes_written += src_addr_size;
    }

    return bytes_written;
}

static void source_set(void)
{
    // Intentionally empty: source address type is None.
}

/***************************************************************************************************
 * @section Auxiliary security header functions
 **************************************************************************************************/

static uint8_t security_header_size(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    uint8_t sec_ctrl_offset = nrf_802154_frame_parser_sec_ctrl_offset_get(p_frame_data);
    uint8_t aux_sec_hdr_end = nrf_802154_frame_parser_aux_sec_hdr_end_offset_get(p_frame_data);

    return aux_sec_hdr_end - sec_ctrl_offset;
}

static uint8_t key_id_size_get(uint8_t key_id_mode)
{
    switch (key_id_mode)
    {
        case KEY_ID_MODE_1:
            return KEY_ID_MODE_1_SIZE;

        case KEY_ID_MODE_2:
            return KEY_ID_MODE_2_SIZE;

        case KEY_ID_MODE_3:
            return KEY_ID_MODE_3_SIZE;

        default:
            return 0U;
    }
}

static uint8_t security_key_id_set(const nrf_802154_frame_parser_data_t * p_frame_data,
                                   nrf_802154_frame_parser_data_t       * p_ack_data)
{
    const uint8_t * p_frame_key_id = nrf_802154_frame_parser_key_id_get(p_frame_data);
    uint8_t       * p_ack_key_id   = (uint8_t *)nrf_802154_frame_parser_key_id_get(p_ack_data);
    uint8_t         key_id_size    =
        key_id_size_get(nrf_802154_frame_parser_sec_ctrl_key_id_mode_get(p_ack_data));

    if ((p_ack_key_id != NULL) && (p_frame_key_id != NULL))
    {
        memcpy(p_ack_key_id, p_frame_key_id, key_id_size);
    }

    return key_id_size;
}

static bool frame_counter_set(nrf_802154_frame_parser_data_t * p_ack_data,
                              uint8_t                        * p_bytes_written)
{
    nrf_802154_key_id_t ack_key_id;
    uint32_t            new_fc_value;
    uint8_t           * p_frame_counter = (uint8_t *)nrf_802154_frame_parser_frame_counter_get(
        p_ack_data);

    if (p_frame_counter == NULL)
    {
        // The frame counter is suppressed
        *p_bytes_written = 0;
        return true;
    }

    ack_key_id.mode     = nrf_802154_frame_parser_sec_ctrl_key_id_mode_get(p_ack_data);
    ack_key_id.p_key_id = (uint8_t *)nrf_802154_frame_parser_key_id_get(p_ack_data);

    if (NRF_802154_SECURITY_ERROR_NONE !=
        nrf_802154_security_pib_frame_counter_get_next(&new_fc_value, &ack_key_id))
    {
        *p_bytes_written = 0;
        return false;
    }

    // Set the frame counter value in security header of the ACK frame
    host_32_to_little(new_fc_value, p_frame_counter);
    *p_bytes_written = FRAME_COUNTER_SIZE;

    return true;
}

static bool security_header_set(const nrf_802154_frame_parser_data_t * p_frame_data,
                                nrf_802154_frame_parser_data_t       * p_ack_data,
                                uint8_t                              * p_bytes_written)
{
    bool security_header_prepared;
    bool result;

    uint8_t bytes_written       = 0U;
    uint8_t fc_bytes_written    = 0U;
    uint8_t ack_sec_ctrl_offset = nrf_802154_frame_parser_addressing_end_offset_get(
        p_ack_data);
    uint8_t * ack_sec_ctrl = (uint8_t *)nrf_802154_frame_parser_addressing_end_get(
        p_ack_data);
    const uint8_t * frame_sec_ctrl = nrf_802154_frame_parser_sec_ctrl_get(p_frame_data);

    if ((ack_sec_ctrl == NULL) || (frame_sec_ctrl == NULL))
    {
        *p_bytes_written = bytes_written;
        return true;
    }

    // All the bits in the security control byte can be copied.
    *ack_sec_ctrl  = *frame_sec_ctrl;
    bytes_written += SECURITY_CONTROL_SIZE;

    // Security control field is now ready. The parsing of the frame can advance.
    result = nrf_802154_frame_parser_valid_data_extend(p_ack_data,
                                                       ack_sec_ctrl_offset + PHR_SIZE,
                                                       PARSE_LEVEL_SEC_CTRL_OFFSETS);
    NRF_802154_ASSERT(result);
    (void)result;

    if (nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_frame_data) == SECURITY_LEVEL_NONE)
    {
        // The security level value is zero, therefore no auxiliary security header processing
        // is performed according to 802.15.4 specification. This also applies to the frame counter,
        // the value of which is left as it is in the message to which the ACK responds.
        // The entire auxiliary security header content is simply copied to ACK.
        uint8_t sec_hdr_size = security_header_size(p_frame_data) - SECURITY_CONTROL_SIZE;

        memcpy(ack_sec_ctrl + SECURITY_CONTROL_SIZE,
               frame_sec_ctrl + SECURITY_CONTROL_SIZE,
               sec_hdr_size);
        bytes_written           += sec_hdr_size;
        security_header_prepared = true;
    }
    else
    {
        bytes_written           += security_key_id_set(p_frame_data, p_ack_data);
        security_header_prepared = frame_counter_set(p_ack_data, &fc_bytes_written);
        bytes_written           += fc_bytes_written;
    }

    bytes_written   += nrf_802154_frame_parser_mic_size_get(p_ack_data);
    *p_bytes_written = bytes_written;

    return security_header_prepared;
}

/***************************************************************************************************
 * @section Information Elements
 **************************************************************************************************/

static void ie_header_set(const uint8_t                  * p_ie_data,
                          uint8_t                          ie_data_len,
                          nrf_802154_frame_parser_data_t * p_ack_data)
{
    uint8_t   ie_offset = p_ack_data->helper.aux_sec_hdr_end_offset;
    uint8_t * p_ack_ie;

    p_ack_ie = (uint8_t *)p_ack_data->p_frame + ie_offset;

    if (p_ie_data == NULL)
    {
        return;
    }

    NRF_802154_ASSERT(p_ack_ie != NULL);

    memcpy(p_ack_ie, p_ie_data, ie_data_len);

#if NRF_802154_IE_WRITER_ENABLED
    nrf_802154_ie_writer_prepare(p_ack_ie, p_ack_ie + ie_data_len);
#endif
}

/***************************************************************************************************
 * @section Authentication and encryption transformation
 **************************************************************************************************/

static bool encryption_prepare(const nrf_802154_frame_parser_data_t * p_ack_data)
{
#if NRF_802154_ENCRYPTION_ENABLED
    nrf_802154_encrypt_ack_reset();

    if (nrf_802154_frame_parser_security_enabled_bit_is_set(p_ack_data) == false)
    {
        return true;
    }

    if (nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_ack_data) == SECURITY_LEVEL_NONE)
    {
        return true;
    }

    return nrf_802154_encrypt_ack_prepare(p_ack_data);
#else // NRF_802154_ENCRYPTION_ENABLED
    return true;
#endif  // NRF_802154_ENCRYPTION_ENABLED
}

/***************************************************************************************************
 * @section Enhanced ACK generation
 **************************************************************************************************/

static void fcf_process(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    // Set Frame Control field bits.
    // Some of them might require correction when higher parse level is available
    m_ack[PHR_OFFSET] += frame_control_set(p_frame_data, false);

    bool result = nrf_802154_frame_parser_valid_data_extend(&m_ack_data,
                                                            m_ack[PHR_OFFSET] + PHR_SIZE,
                                                            PARSE_LEVEL_FCF_OFFSETS);

    NRF_802154_ASSERT(result);
    (void)result;
}

static void addr_end_process(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    // Set valid sequence number in ACK frame.
    m_ack[PHR_OFFSET] += sequence_number_set(p_frame_data);
    // Set destination address and PAN ID.
    m_ack[PHR_OFFSET] += destination_set(p_frame_data, &m_ack_data);

    // Set source address and PAN ID.
    source_set();

    // Having the frame's source address, presence of IEs can be determined.
    // coverity[unchecked_value]
    mp_ie_data = nrf_802154_ack_data_ie_get(
        nrf_802154_frame_parser_src_addr_get(p_frame_data),
        nrf_802154_frame_parser_src_addr_is_extended(p_frame_data),
        &m_ie_data_len);

    // Update the IE present bit in Frame Control field knowing if IEs should be present.
    fcf_ie_present_set(mp_ie_data != NULL);

    bool result = nrf_802154_frame_parser_valid_data_extend(&m_ack_data,
                                                            m_ack[PHR_OFFSET] + PHR_SIZE,
                                                            PARSE_LEVEL_ADDRESSING_END);

    NRF_802154_ASSERT(result);
    (void)result;
}

static bool aux_sec_hdr_process(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    uint8_t bytes_written;

    if (security_header_set(p_frame_data, &m_ack_data, &bytes_written) == false)
    {
        return false;
    }

    m_ack[PHR_OFFSET] += bytes_written;

    bool result = nrf_802154_frame_parser_valid_data_extend(&m_ack_data,
                                                            m_ack[PHR_OFFSET] + PHR_SIZE,
                                                            PARSE_LEVEL_AUX_SEC_HDR_END);

    NRF_802154_ASSERT(result);
    (void)result;

    return true;
}

static void ie_process(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    // Set IE header.
    ie_header_set(mp_ie_data, m_ie_data_len, &m_ack_data);
    m_ack[PHR_OFFSET] += m_ie_data_len;

    // Add space for the FCS field.
    m_ack[PHR_OFFSET] += FCS_SIZE;

    bool result = nrf_802154_frame_parser_valid_data_extend(&m_ack_data,
                                                            m_ack[PHR_OFFSET] + PHR_SIZE,
                                                            PARSE_LEVEL_FULL);

    NRF_802154_ASSERT(result);
    (void)result;
}

static bool encryption_process(void)
{
    return encryption_prepare(&m_ack_data);
}

static uint8_t * ack_process(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    bool                                 * p_processing_done)
{
    nrf_802154_frame_parser_level_t frame_parse_level = nrf_802154_frame_parser_parse_level_get(
        p_frame_data);
    nrf_802154_frame_parser_level_t ack_parse_level = nrf_802154_frame_parser_parse_level_get(
        &m_ack_data);

    *p_processing_done = false;

    if ((frame_parse_level >= PARSE_LEVEL_FCF_OFFSETS) &&
        (ack_parse_level < PARSE_LEVEL_FCF_OFFSETS))
    {
        fcf_process(p_frame_data);
    }

    if ((frame_parse_level >= PARSE_LEVEL_ADDRESSING_END) &&
        (ack_parse_level < PARSE_LEVEL_ADDRESSING_END))
    {
        addr_end_process(p_frame_data);
    }

    if ((frame_parse_level >= PARSE_LEVEL_AUX_SEC_HDR_END) &&
        (ack_parse_level < PARSE_LEVEL_AUX_SEC_HDR_END))
    {
        if (!aux_sec_hdr_process(p_frame_data))
        {
            // Failure to set auxiliary security header, the ACK cannot be created. Exit immediately
            *p_processing_done = true;
            return NULL;
        }

        ie_process(p_frame_data);
    }

    if (frame_parse_level == PARSE_LEVEL_FULL)
    {
        // With the entire frame validated update the Frame Pending bit in Frame Control field
        fcf_frame_pending_set(p_frame_data);

        if (encryption_process())
        {
            // Success. Processing completed
            *p_processing_done = true;
            return m_ack;
        }
        else
        {
            // Failure to prepare encryption even though it's required, the ACK cannot be created.
            // Exit immediately
            *p_processing_done = true;
            return NULL;
        }
    }

    return NULL;
}

/***************************************************************************************************
 * @section Public API implementation
 **************************************************************************************************/

void nrf_802154_enh_ack_generator_init(void)
{
    // Intentionally empty.
}

void nrf_802154_enh_ack_generator_reset(void)
{
    memset(m_ack, 0U, sizeof(m_ack));
    (void)nrf_802154_frame_parser_data_init(m_ack, 0U, PARSE_LEVEL_NONE, &m_ack_data);
    mp_ie_data    = 0U;
    m_ie_data_len = 0U;
    m_ack_state   = ACK_STATE_RESET;
}

uint8_t * nrf_802154_enh_ack_generator_create(
    const nrf_802154_frame_parser_data_t * p_frame_data)
{
    switch (ack_state_get())
    {
        case ACK_STATE_RESET:
        #if NRF_802154_IE_WRITER_ENABLED
            // The IE writer module can be in the IE_WRITER_PREPARE state if
            // the previous transmission failed at an early stage.
            // Reset it, to avoid data corruption in case this ACK
            // does not contain information elements. Otherwise, the
            // IE writer would commit data in nrf_802154_ie_writer_tx_ack_started_hook
            // regardless if writing of IE elements is needed or not.
            nrf_802154_ie_writer_reset();
        #endif

            ack_state_set(ACK_STATE_PROCESSING);
        // Fallthrough

        case ACK_STATE_PROCESSING:
        {
            bool      processing_done;
            uint8_t * p_ack = ack_process(p_frame_data, &processing_done);

            if (processing_done)
            {
                ack_state_set(p_ack ? ACK_STATE_COMPLETE : ACK_STATE_INVALID);
            }

            return processing_done ? p_ack : NULL;
        }

        case ACK_STATE_INVALID:
            return NULL;

        case ACK_STATE_COMPLETE:
            return m_ack;

        default:
            NRF_802154_ASSERT(false);
            return NULL;
    }
}
