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
 *   This file implements incoming frame filtering according to 3 and 4 levels of filtering.
 *
 * Filtering details are specified in 802.15.4-2015: 6.7.2.
 * 1st and 2nd filtering level is performed by FSM module depending on promiscuous mode, when FCS is
 * received.
 */

#include "nrf_802154_filter.h"

#include "nrf_802154_assert.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf_802154_const.h"
#include "nrf_802154_frame_parser.h"
#include "nrf_802154_pib.h"

#define FCF_CHECK_OFFSET           (PHR_SIZE + FCF_SIZE)
#define PANID_CHECK_OFFSET         (DEST_ADDR_OFFSET)
#define SHORT_ADDR_CHECK_OFFSET    (DEST_ADDR_OFFSET + SHORT_ADDRESS_SIZE)
#define EXTENDED_ADDR_CHECK_OFFSET (DEST_ADDR_OFFSET + EXTENDED_ADDRESS_SIZE)

/**
 * @brief Check if given frame version is allowed for given frame type.
 *
 * @param[in] frame_type     Type of incoming frame.
 * @param[in] frame_version  Version of incoming frame.
 *
 * @retval true   Given frame version is allowed for given frame type.
 * @retval false  Given frame version is not allowed for given frame type.
 */
static bool frame_type_and_version_filter(uint8_t frame_type, uint8_t frame_version)
{
    bool result;

    switch (frame_type)
    {
        case FRAME_TYPE_BEACON:
        case FRAME_TYPE_DATA:
        case FRAME_TYPE_ACK:
        case FRAME_TYPE_COMMAND:
            result = (frame_version != FRAME_VERSION_3);
            break;

        case FRAME_TYPE_MULTIPURPOSE:
            result = (frame_version == FRAME_VERSION_0);
            break;

        case FRAME_TYPE_FRAGMENT:
        case FRAME_TYPE_EXTENDED:
            result = true;
            break;

        default:
            result = false;
    }

    return result;
}

/**
 * @brief Check if given frame type may include destination address fields.
 *
 * @note Actual presence of destination address fields in the frame is indicated by FCF.
 *
 * @param[in] frame_type  Type of incoming frame.
 *
 * @retval true   Given frame type may include addressing fields.
 * @retval false  Given frame type may not include addressing fields.
 */
static bool dst_addressing_may_be_present(uint8_t frame_type)
{
    bool result;

    switch (frame_type)
    {
        case FRAME_TYPE_BEACON:
        case FRAME_TYPE_DATA:
        case FRAME_TYPE_ACK:
        case FRAME_TYPE_COMMAND:
        case FRAME_TYPE_MULTIPURPOSE:
            result = true;
            break;

        case FRAME_TYPE_FRAGMENT:
        case FRAME_TYPE_EXTENDED:
            result = false;
            break;

        default:
            result = false;
    }

    return result;
}

/**
 * @brief Verify if destination addressing bits in the FCF field are correct assuming its version is 2006.
 *
 * If given frame contains errors that prevent getting offset, this function returns
 * NRF_802154_RX_ERROR_INVALID_FRAME. If there are no destination address fields in given frame,
 * this function verifies if the driver is configured as a PAN coordinator or if the frame is a
 * beacon.
 * If both of those conditions are not met, the function returns
 * NRF_802154_RX_ERROR_INVALID_DEST_ADDR.
 * Otherwise, this function returns NRF_802154_RX_ERROR_NONE.
 *
 * @param[in]  p_frame_data Pointer to the frame parser data.
 * @param[in]  frame_type   Type of incoming frame.
 *
 * @retval NRF_802154_RX_ERROR_NONE               No errors in given frame were detected - it may be
 *                                                further processed.
 * @retval NRF_802154_RX_ERROR_INVALID_DEST_ADDR  The frame is valid but addressed to another node.
 * @retval NRF_802154_RX_ERROR_INVALID_FRAME      Detected an error in given frame - it should be
 *                                                discarded.
 */
static nrf_802154_rx_error_t dst_addressing_fcf_check_2006(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    uint8_t                                frame_type)
{
    nrf_802154_rx_error_t result;

    switch (nrf_802154_frame_parser_dst_addr_type_get(p_frame_data))
    {
        case DEST_ADDR_TYPE_SHORT:
            result = NRF_802154_RX_ERROR_NONE;
            break;

        case DEST_ADDR_TYPE_EXTENDED:
            result = NRF_802154_RX_ERROR_NONE;
            break;

        case DEST_ADDR_TYPE_NONE:
            if (nrf_802154_pib_pan_coord_get() || (frame_type == FRAME_TYPE_BEACON))
            {
                switch (nrf_802154_frame_parser_src_addr_type_get(p_frame_data))
                {
                    case SRC_ADDR_TYPE_SHORT:
                    case SRC_ADDR_TYPE_EXTENDED:
                        result = NRF_802154_RX_ERROR_NONE;
                        break;

                    default:
                        result = NRF_802154_RX_ERROR_INVALID_FRAME;
                }
            }
            else
            {
                result = NRF_802154_RX_ERROR_INVALID_DEST_ADDR;
            }

            break;

        default:
            result = NRF_802154_RX_ERROR_INVALID_FRAME;
    }

    return result;
}

/**
 * @brief Verify if destination addressing bits in the FCF field are correct assuming its version is 2015.
 *
 * If given frame contains errors that prevent getting offset, this function returns
 * NRF_802154_RX_ERROR_INVALID_FRAME.
 * Otherwise, this function returns NRF_802154_RX_ERROR_NONE.
 *
 * @param[in]  p_frame_data Pointer to the frame parser data.
 * @param[in]  frame_type   Type of incoming frame.
 *
 * @retval NRF_802154_RX_ERROR_NONE               No errors in given frame were detected - it may be
 *                                                further processed.
 * @retval NRF_802154_RX_ERROR_INVALID_FRAME      Detected an error in given frame - it should be
 *                                                discarded.
 */
static nrf_802154_rx_error_t dst_addressing_fcf_check_2015(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    uint8_t                                frame_type)
{
    nrf_802154_rx_error_t result;

    switch (frame_type)
    {
        case FRAME_TYPE_BEACON:
        case FRAME_TYPE_DATA:
        case FRAME_TYPE_ACK:
        case FRAME_TYPE_COMMAND:
        {
            uint8_t end_offset =
                nrf_802154_frame_parser_dst_addressing_end_offset_get(p_frame_data);

            if (end_offset == NRF_802154_FRAME_PARSER_INVALID_OFFSET)
            {
                result = NRF_802154_RX_ERROR_INVALID_FRAME;
            }
            else
            {
                result = NRF_802154_RX_ERROR_NONE;
            }
        }
        break;

        case FRAME_TYPE_MULTIPURPOSE:
            // TODO: Implement dst addressing filtering according to 2015 spec
            result = NRF_802154_RX_ERROR_INVALID_FRAME;
            break;

        case FRAME_TYPE_FRAGMENT:
        case FRAME_TYPE_EXTENDED:
            // No addressing data
            result = NRF_802154_RX_ERROR_NONE;
            break;

        default:
            result = NRF_802154_RX_ERROR_INVALID_FRAME;
    }

    return result;
}

/**
 * @brief Verify if destination addressing bits in the FCF field are correct.
 *
 * This function relays its arguments to either @ref dst_addressing_end_offset_get_2006
 * or @ref dst_addressing_end_offset_get_2015 depending on the frame version.
 *
 * @param[in]  p_frame_data  Pointer to the frame parser data.
 * @param[in]  frame_type    Type of incoming frame.
 * @param[in]  frame_version Version of the incoming frame.
 *
 * @retval NRF_802154_RX_ERROR_NONE               No errors in given frame were detected - it may be
 *                                                further processed.
 * @retval NRF_802154_RX_ERROR_INVALID_DEST_ADDR  The frame is valid but addressed to another node.
 * @retval NRF_802154_RX_ERROR_INVALID_FRAME      Detected an error in given frame - it should be
 *                                                discarded.
 */
static nrf_802154_rx_error_t dst_addressing_fcf_check(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    uint8_t                                frame_type,
    uint8_t                                frame_version)
{
    nrf_802154_rx_error_t result;

    switch (frame_version)
    {
        case FRAME_VERSION_0:
        case FRAME_VERSION_1:
            result = dst_addressing_fcf_check_2006(p_frame_data, frame_type);
            break;

        case FRAME_VERSION_2:
            result = dst_addressing_fcf_check_2015(p_frame_data, frame_type);
            break;

        default:
            result = NRF_802154_RX_ERROR_INVALID_FRAME;
    }

    return result;
}

/**
 * Verify if destination PAN Id of incoming frame allows processing by this node.
 *
 * @param[in] p_panid     Pointer of PAN ID of incoming frame.
 * @param[in] frame_type  Type of the frame being filtered.
 *
 * @retval true   PAN Id of incoming frame allows further processing of the frame.
 * @retval false  PAN Id of incoming frame does not allow further processing.
 */
static bool dst_pan_id_check(const uint8_t * p_panid, uint8_t frame_type)
{
    bool result;

    if ((0 == memcmp(p_panid, nrf_802154_pib_pan_id_get(), PAN_ID_SIZE)) ||
        (0 == memcmp(p_panid, BROADCAST_ADDRESS, PAN_ID_SIZE)))
    {
        result = true;
    }
    else if ((FRAME_TYPE_BEACON == frame_type) &&
             (0 == memcmp(nrf_802154_pib_pan_id_get(), BROADCAST_ADDRESS, PAN_ID_SIZE)))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

/**
 * Verify if destination short address of incoming frame allows processing by this node.
 *
 * @param[in] p_dst_addr  Pointer of destination address of incoming frame.
 *
 * @retval true   Destination address of incoming frame allows further processing of the frame.
 * @retval false  Destination address of incoming frame does not allow further processing.
 */
static bool dst_short_addr_check(const uint8_t * p_dst_addr)
{
    if ((0 == memcmp(p_dst_addr, nrf_802154_pib_short_address_get(), SHORT_ADDRESS_SIZE)) ||
        (0 == memcmp(p_dst_addr, BROADCAST_ADDRESS, SHORT_ADDRESS_SIZE)))
    {
        return true;
    }

    const uint8_t * alternate_address = nrf_802154_pib_alternate_short_address_get();

    if (alternate_address && (0 == memcmp(p_dst_addr, alternate_address, SHORT_ADDRESS_SIZE)))
    {
        return true;
    }

    return false;
}

/**
 * Verify if destination extended address of incoming frame allows processing by this node.
 *
 * @param[in] p_dst_addr  Pointer of destination address of incoming frame.
 *
 * @retval true   Destination address of incoming frame allows further processing of the frame.
 * @retval false  Destination address of incoming frame does not allow further processing.
 */
static bool dst_extended_addr_check(const uint8_t * p_dst_addr)
{
    bool result;

    if (0 == memcmp(p_dst_addr, nrf_802154_pib_extended_address_get(), EXTENDED_ADDRESS_SIZE))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

/**
 * Verify if destination addressing of incoming frame allows processing by this node.
 * This function checks addressing according to IEEE 802.15.4-2015.
 *
 * @param[in]  p_frame_data Pointer to the frame parser data.
 *
 * @retval NRF_802154_RX_ERROR_NONE               Destination address of incoming frame allows further processing of the frame.
 * @retval NRF_802154_RX_ERROR_INVALID_FRAME      Received frame is invalid.
 * @retval NRF_802154_RX_ERROR_INVALID_DEST_ADDR  Destination address of incoming frame does not allow further processing.
 */
static nrf_802154_rx_error_t dst_addr_check(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    const uint8_t * p_dst_panid = nrf_802154_frame_parser_dst_panid_get(p_frame_data);
    const uint8_t * p_dst_addr  = nrf_802154_frame_parser_dst_addr_get(p_frame_data);
    uint8_t         frame_type  = nrf_802154_frame_parser_frame_type_get(p_frame_data);

    if (p_dst_panid != NULL)
    {
        if (!dst_pan_id_check(p_dst_panid, frame_type))
        {
            return NRF_802154_RX_ERROR_INVALID_DEST_ADDR;
        }
    }

    uint8_t dst_addr_size =
        p_dst_addr ? nrf_802154_frame_parser_dst_addr_size_get(p_frame_data) : 0U;

    switch (dst_addr_size)
    {
        case SHORT_ADDRESS_SIZE:
            return dst_short_addr_check(p_dst_addr) ? NRF_802154_RX_ERROR_NONE :
                   NRF_802154_RX_ERROR_INVALID_DEST_ADDR;

        case EXTENDED_ADDRESS_SIZE:
            return dst_extended_addr_check(p_dst_addr) ? NRF_802154_RX_ERROR_NONE :
                   NRF_802154_RX_ERROR_INVALID_DEST_ADDR;

        case 0:
            // Allow frames destined to the Pan Coordinator without destination address or
            // beacon frames without destination address
            return (nrf_802154_pib_pan_coord_get() ||
                    (frame_type ==
                     FRAME_TYPE_BEACON)) ? NRF_802154_RX_ERROR_NONE :
                   NRF_802154_RX_ERROR_INVALID_DEST_ADDR;

        default:
            NRF_802154_ASSERT(false);
    }

    return NRF_802154_RX_ERROR_INVALID_FRAME;
}

nrf_802154_rx_error_t nrf_802154_filter_frame_part(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    nrf_802154_filter_mode_t               filter_mode)
{
    nrf_802154_rx_error_t result     = NRF_802154_RX_ERROR_NONE;
    uint8_t               frame_type = nrf_802154_frame_parser_frame_type_get(
        p_frame_data);
    uint8_t frame_version = nrf_802154_frame_parser_frame_version_get(
        p_frame_data);
    uint8_t psdu_length = nrf_802154_frame_parser_frame_length_get(
        p_frame_data);

    if (filter_mode & NRF_802154_FILTER_MODE_FCF)
    {
        NRF_802154_ASSERT(nrf_802154_frame_parser_parse_level_get(
                              p_frame_data) >= PARSE_LEVEL_FCF_OFFSETS);

        if ((psdu_length < IMM_ACK_LENGTH) || (psdu_length > MAX_PACKET_SIZE))
        {
            return NRF_802154_RX_ERROR_INVALID_LENGTH;
        }

        if (!frame_type_and_version_filter(frame_type, frame_version))
        {
            return NRF_802154_RX_ERROR_INVALID_FRAME;
        }

        if (dst_addressing_may_be_present(frame_type))
        {
            result = dst_addressing_fcf_check(p_frame_data,
                                              frame_type,
                                              frame_version);
        }
    }

    if (result != NRF_802154_RX_ERROR_NONE)
    {
        return result;
    }

    if (filter_mode & NRF_802154_FILTER_MODE_DST_ADDR)
    {
        NRF_802154_ASSERT(nrf_802154_frame_parser_parse_level_get(
                              p_frame_data) >= PARSE_LEVEL_DST_ADDRESSING_END);

        result = dst_addr_check(p_frame_data);
    }

    return result;
}
