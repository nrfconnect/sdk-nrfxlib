/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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
 * @brief Module that contains frame utilities for the 802.15.4 radio driver.
 *
 */

#ifndef NRF_802154_FRAME_H
#define NRF_802154_FRAME_H

#include "nrfx.h"
#include "nrf_802154_const.h"
#include "nrf_802154_utils_byteorder.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __STATIC_INLINE__
#undef __STATIC_INLINE__
#endif

#ifdef NRF_802154_FRAME_DECLARE_ONLY
#define __STATIC_INLINE__
#else
#define __STATIC_INLINE__               __STATIC_INLINE
#endif

#define NRF_802154_FRAME_INVALID_OFFSET 0xff

typedef enum
{
    /**
     * @brief Parse level at which no parsing has been performed on the frame.
     *
     * All frame fields and offsets should be considered as incomplete and invalid at this parse level.
     */
    PARSE_LEVEL_NONE,

    /**
     * @brief Parse level at which the offsets that can be determined based on the FCF can be considered valid.
     *
     * The following fields can be considered valid and complete:
     *    - FCF field
     *
     * The following offsets can be considered valid by the user:
     *    - DSN field offset
     *    - Addressing offsets
     * The data stored under the offsets should be considered as incomplete.
     */
    PARSE_LEVEL_FCF_OFFSETS,

    /**
     * @brief Parse level at which the destination address data can be considered complete.
     *
     * The following fields can be considered valid and complete:
     *    - DSN field
     *    - Destination addressing fields
     * The completeness doesn't indicate that the fields are actually present in the frame.
     */
    PARSE_LEVEL_DST_ADDRESSING_END,

    /**
     * @brief Parse level at which the data under offsets determined by the FCF can be considered complete.
     *
     * The following fields can be considered valid and complete:
     *    - DSN field
     *    - Addressing fields
     * The completeness doesn't indicate that the fields are actually present in the frame.
     */
    PARSE_LEVEL_ADDRESSING_END,

    /**
     * @brief Parse level at which the offsets that can be determined based on Security Control Field can be considered valid.
     *
     * The following offsets can be considered valid by the user:
     *    - All offsets before the Auxiliary Security Header
     *    - All Auxiliary Security Header field offsets and field sizes
     * The data stored under the offsets should be considered as incomplete.
     */
    PARSE_LEVEL_SEC_CTRL_OFFSETS,

    /**
     * @brief Parse level at which the data under offsets determined by the Security Control Field can be considered complete.
     *
     * The following fields can be considered valid and complete:
     *    - All fields before the Auxiliary Security Header
     *    - All Auxiliary Security Header fields
     * The completeness doesn't indicate that the fields are actually present in the frame.
     */
    PARSE_LEVEL_AUX_SEC_HDR_END,

    /**
     * @brief Parse level for fully parsed frame. All fields can be considered valid and complete.
     *
     * The following fields can be considered valid and complete:
     *    - All fields in the frame
     * The completeness doesn't indicate that the fields are actually present in the frame.
     *
     * This is the only parse level at which the PSDU length is taken into account and is expected to be correct.
     */
    PARSE_LEVEL_FULL,

    PARSE_LEVEL_MAX = PARSE_LEVEL_FULL
} nrf_802154_frame_parser_level_t;

typedef struct
{
    uint8_t                       * p_frame;        ///< Pointer to a buffer containing the frame bytes.
    nrf_802154_frame_parser_level_t parse_level;    ///< Current frame parse level.
    uint8_t                         valid_data_len; ///< Number of valid bytes in the frame.

    struct
    {
        struct
        {
            uint8_t panid_offset; ///< Destination PAN ID offset.
            uint8_t addr_offset;  ///< Destination address offset.
        } dst;

        struct
        {
            uint8_t panid_offset; ///< Source PAN ID offset.
            uint8_t addr_offset;  ///< Source address offset.
        } src;

        struct
        {
            uint8_t sec_ctrl_offset;      ///< Security Control Field offset.
            uint8_t frame_counter_offset; ///< Frame Counter field offset.
            uint8_t key_id_offset;        ///< Key ID offset.
            uint8_t key_src_offset;       ///< Key Source offset.
            uint8_t key_idx_offset;       ///< Key Index offset.
        }       aux_sec_hdr;

        uint8_t header_ie_offset;         ///< Header IE offset.
    } mhr;

    struct
    {
        uint8_t mac_payload_offset; ///< MAC payload offset.
    } mac_payload;

    struct
    {
        uint8_t dst_addr_size;             ///< Destination address size;
        uint8_t src_addr_size;             ///< Source address size;
        uint8_t dst_addressing_end_offset; ///< Destination addressing end offset.
        uint8_t addressing_end_offset;     ///< Addressing end offset.
        uint8_t aux_sec_hdr_end_offset;    ///< Auxiliary Security Header offset.
        uint8_t key_src_size;              ///< Key Source size.
        uint8_t mic_size;                  ///< Message Integrity Code size.
    } helper;
} nrf_802154_frame_t;

/**
 * @brief Gets current parse level of the provided parser data.
 *
 * @param[in] p_parser_data  Pointer to a parser data to check.
 *
 * @returns Current parse level of @p p_parser_data.
 */
__STATIC_INLINE__ nrf_802154_frame_parser_level_t nrf_802154_frame_parse_level_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the PSDU length of the frame.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  PSDU length of the frame.
 *
 * @note This function assumes that the PHR is correct.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_length_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the PSDU start offset of the frame.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  PSDU start offset of the frame.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_psdu_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the PSDU start address of the frame.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  PSDU start address of the frame.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_psdu_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the MAC frame version value from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Masked MAC frame version value.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_version_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the DSN suppress bit from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  DSN suppress bit is set.
 * @retval  false DSN suppress bit is not set.
 */
__STATIC_INLINE__ bool nrf_802154_frame_dsn_suppress_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the MAC frame destination address type from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Masked MAC frame destination address type.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addr_type_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Determines if the destination address is extended.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  The destination address is extended.
 * @retval  false The destination address is not extended.
 */
__STATIC_INLINE__ bool nrf_802154_frame_dst_addr_is_extended(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the MAC frame source address type from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Masked MAC frame source address type.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_src_addr_type_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Determines if the source address is extended.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  The destination address is extended.
 * @retval  false The destination address is not extended.
 */
__STATIC_INLINE__ bool nrf_802154_frame_src_addr_is_extended(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Determines if the source address is short.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  The destination address is short.
 * @retval  false The destination address is not short.
 */
__STATIC_INLINE__ bool nrf_802154_frame_src_addr_is_short(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the Security Enabled bit from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  The security is enabled.
 * @retval  false The security is not enabled.
 */
__STATIC_INLINE__ bool nrf_802154_frame_security_enabled_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the IE present bit from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  The Information Elements are present.
 * @retval  false The Information Elements are not present.
 */
__STATIC_INLINE__ bool nrf_802154_frame_ie_present_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

__STATIC_INLINE__ bool nrf_802154_frame_pending_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the frame type field from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Masked value of the frame type.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_type_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the PAN ID compressions bit from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  PAN ID compressions is set.
 * @retval  false PAN ID compressions is not set.
 */
__STATIC_INLINE__ bool nrf_802154_frame_panid_compression_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the AR bit from the Frame Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  AR bit is set.
 * @retval  false AR bit is not set.
 */
__STATIC_INLINE__ bool nrf_802154_frame_ar_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the DSN field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  The value of the DSN field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dsn_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the Destination PAN ID field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the Destination PAN ID field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_panid_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Destination PAN field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Destination PAN field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dst_panid_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of frame Destination Address.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the frame Destination Address field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addr_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Destination Address field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Destination Address field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dst_addr_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of frame Source PAN ID.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the frame Source PAN ID field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_src_panid_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Source PAN ID field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Source PAN ID field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_src_panid_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of frame Source Address.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the frame Source Address field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_src_addr_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Source Address field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Source Address field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_src_addr_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the size of the source address.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Source address size in bytes.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_src_addr_size_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the size of the destination address.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Destination address size in bytes.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addr_size_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the Security Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the Security Control Field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_sec_ctrl_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the Security Level field from the Security Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Masked Security Level field value.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_sec_ctrl_sec_lvl_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the Frame Counter Suppress bit from the Security Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  FC suppress bit is set.
 * @retval  false FC suppress bit is not set.
 */
__STATIC_INLINE__ bool nrf_802154_frame_sec_ctrl_fc_suppress_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the Key ID Mode field from the Security Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Masked Key ID Mode field value.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_sec_ctrl_key_id_mode_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the value of the ASN in Nonce bit from the Security Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @retval  true  ASN in Nonce bit is set.
 * @retval  false ASN in Nonce bit is not set.
 */
__STATIC_INLINE__ bool nrf_802154_frame_sec_ctrl_asn_in_nonce_bit_is_set(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Security Control Field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Security Control Field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_sec_ctrl_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the Frame Counter field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the Frame Counter field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_counter_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Frame Counter field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Frame Counter field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_counter_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the Key ID field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the Key ID field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_key_id_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Key ID field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Key ID field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_key_id_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the Key Source field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the Key Source field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_key_source_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Key Source field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Key Source field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_key_source_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the length of the Key Source field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Length of the Key Source field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_key_source_length_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the Key Index field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the Key Index field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_key_index_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the Key Index field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the Key Index field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_key_index_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the IE header.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the IE header.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_header_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the IE header.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the IE header.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_header_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the MAC payload.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the MAC payload.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_payload_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the MAC payload.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the MAC payload.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_mac_payload_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the MAC Command ID field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the MAC Command ID field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_command_id_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the MAC Command ID field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the MAC Command ID field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_mac_command_id_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the offset of the MFR field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Offset of the MFR field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_mfr_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the address of the MFR field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Address of the MFR field.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_mfr_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the length of the MIC field.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Length of the MIC field.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_mic_size_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the length of the MAC payload.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Length of the MAC payload.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_payload_length_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the length of the MAC header.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  Length of the MAC header.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_header_length_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the end offset of the destination addressing section.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  End offset of the destination addressing section.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addressing_end_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the end address of the destination addressing section.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  End address of the destination addressing section.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dst_addressing_end_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the end offset of the addressing section.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  End offset of the addressing section.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_addressing_end_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the end address of the addressing section.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  End address of the addressing section.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_addressing_end_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Gets the end offset of the security header section.
 *
 * @param[in]   p_parser_data   Pointer to a frame parser data.
 *
 * @returns  End offset of the security header section.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_aux_sec_hdr_end_offset_get(
    const nrf_802154_frame_t * p_parser_data);

/**
 * @brief Initializes the IE iterator with given IE header address.
 *
 * @param[in]   p_ie_header  Pointer to an IE header.
 *
 * @returns  Information element iterator.
 *
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_header_ie_iterator_begin(
    const uint8_t * p_ie_header);

/**
 * @brief Gets payload address of currently iterated IE.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 *
 * @returns  Current IE payload address.
 *
 * @note  The user must ensure that the payload is properly bounded.
 *
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_content_address_get(
    const uint8_t * p_ie_iterator);

/**
 * @brief Gets length of currently iterated IE.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 *
 * @returns  Length of currently iterated information element.
 *
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_length_get(const uint8_t * p_ie_iterator);

/**
 * @brief Gets next information element iterator.
 *
 * @param[in]   p_ie_iterator  Current information element iterator.
 *
 * @returns  Next information element iterator.
 *
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_iterator_next(
    const uint8_t * p_ie_iterator);

/**
 * @brief Gets identifier of currently iterated IE.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 *
 * @returns  Identifier of currently iterated information element.
 *
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_id_get(const uint8_t * p_ie_iterator);

/**
 * @brief Checks if the current IE is a terminator.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 * @param[in]   p_end_addr     Address past which the iteration shall stop.
 *
 * @retval  true   The IE is a terminator or iteration has passed the end address.
 * @retval  false  The IE is not a terminator and iteration has not passed the end address.
 *
 */
__STATIC_INLINE__ bool nrf_802154_frame_ie_iterator_end(const uint8_t * p_ie_iterator,
                                                        const uint8_t * p_end_addr);

/**
 * @brief Gets vendor-specific OUI (organizationally unique identifier) of currently iterated IE.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 *
 * @note This field is only present in vendor-specific IE. The caller of this function should ensure
 * that @p p_ie_iterator points to an IE with:
 *  - ID: @ref IE_VENDOR_ID ID.
 *  - IE length: at least @ref IE_VENDOR_SIZE_MIN
 * Otherwise, returned value is not OUI.
 * See @ref nrf_802154_frame_ie_id_get.
 *
 * @returns  Vendor-specific OUI.
 */
__STATIC_INLINE__ uint32_t nrf_802154_frame_ie_vendor_oui_get(const uint8_t * p_ie_iterator);

/**
 * @brief Gets subtype of Thread vendor-specific IE.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 *
 * @note This field is only present in Thread vendor-specific IE. The caller of this function should ensure
 * that @p p_ie_iterator points to an IE with:
 *  - ID: @ref IE_VENDOR_ID,
 *  - OUI: @ref IE_VENDOR_THREAD_OUI.
 *  - IE length: at least @ref IE_VENDOR_THREAD_SIZE_MIN
 * Otherwise, returned value is not Thread IE subtype.
 *  - @ref nrf_802154_frame_ie_id_get,
 *  - @ref nrf_802154_frame_ie_vendor_oui_get.
 *
 * @returns subtype of Thread vendor-specific IE.
 */
__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_vendor_thread_subtype_get(
    const uint8_t * p_ie_iterator);

/**
 * @brief Gets vendor-specific IE thread data address of currently iterated IE.
 *
 * @param[in]   p_ie_iterator  Information element iterator.
 *
 * @note This field is only present in Thread vendor-specific IE. The caller of this function should ensure
 * that @p p_ie_iterator points to an IE with:
 *  - ID: @ref IE_VENDOR_ID,
 *  - OUI: @ref IE_VENDOR_THREAD_OUI,
 *  - subtype: @ref IE_VENDOR_THREAD_ACK_PROBING_ID.
 *  - IE length: at least @ref IE_VENDOR_THREAD_ACK_SIZE_MIN
 * Otherwise, returned pointer does not point to thread vendor-specific IE data.
 * See also:
 *  - @ref nrf_802154_frame_ie_id_get,
 *  - @ref nrf_802154_frame_ie_vendor_oui_get,
 *  - @ref nrf_802154_frame_ie_vendor_thread_subtype_get.
 *
 * @returns  Current vendor-specific IE thread data address.
 */
__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_vendor_thread_data_addr_get(
    const uint8_t * p_ie_iterator);

#ifndef NRF_802154_FRAME_DECLARE_ONLY

__STATIC_INLINE__ nrf_802154_frame_parser_level_t nrf_802154_frame_parse_level_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->parse_level;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_length_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->p_frame[PHR_OFFSET];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_psdu_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return PSDU_OFFSET;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_psdu_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return &p_parser_data->p_frame[PSDU_OFFSET];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_version_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->p_frame[FRAME_VERSION_OFFSET] & FRAME_VERSION_MASK;
}

__STATIC_INLINE__ bool nrf_802154_frame_dsn_suppress_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[DSN_SUPPRESS_OFFSET] & DSN_SUPPRESS_BIT) ? true : false;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addr_type_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->p_frame[DEST_ADDR_TYPE_OFFSET] & DEST_ADDR_TYPE_MASK;
}

__STATIC_INLINE__ bool nrf_802154_frame_dst_addr_is_extended(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[DEST_ADDR_TYPE_OFFSET] & DEST_ADDR_TYPE_MASK) ==
           DEST_ADDR_TYPE_EXTENDED;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_src_addr_type_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->p_frame[SRC_ADDR_TYPE_OFFSET] & SRC_ADDR_TYPE_MASK;
}

__STATIC_INLINE__ bool nrf_802154_frame_src_addr_is_extended(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[SRC_ADDR_TYPE_OFFSET] & SRC_ADDR_TYPE_MASK) ==
           SRC_ADDR_TYPE_EXTENDED;
}

__STATIC_INLINE__ bool nrf_802154_frame_src_addr_is_short(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[SRC_ADDR_TYPE_OFFSET] & SRC_ADDR_TYPE_MASK) ==
           SRC_ADDR_TYPE_SHORT;
}

__STATIC_INLINE__ bool nrf_802154_frame_security_enabled_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[SECURITY_ENABLED_OFFSET] & SECURITY_ENABLED_BIT) ? true : false;
}

__STATIC_INLINE__ bool nrf_802154_frame_ie_present_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[IE_PRESENT_OFFSET] & IE_PRESENT_BIT) ? true : false;
}

__STATIC_INLINE__ bool nrf_802154_frame_pending_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[FRAME_PENDING_OFFSET] & FRAME_PENDING_BIT) != 0;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_type_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->p_frame[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK;
}

__STATIC_INLINE__ bool nrf_802154_frame_panid_compression_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    return (p_parser_data->p_frame[PAN_ID_COMPR_OFFSET] & PAN_ID_COMPR_MASK) ? true : false;
}

__STATIC_INLINE__ bool nrf_802154_frame_ar_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    if ((p_parser_data->p_frame[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) == FRAME_TYPE_MULTIPURPOSE)
    {
        return false;
    }
    else
    {
        return (p_parser_data->p_frame[ACK_REQUEST_OFFSET] & ACK_REQUEST_BIT) ? true : false;
    }
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dsn_get(
    const nrf_802154_frame_t * p_parser_data)
{
    if (nrf_802154_frame_dsn_suppress_bit_is_set(p_parser_data))
    {
        return NULL;
    }

    return &p_parser_data->p_frame[DSN_OFFSET];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_panid_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.dst.panid_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dst_panid_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_dst_panid_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addr_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.dst.addr_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dst_addr_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_dst_addr_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_src_panid_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.src.panid_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_src_panid_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_src_panid_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_src_addr_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.src.addr_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_src_addr_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_src_addr_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_src_addr_size_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.src_addr_size;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addr_size_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.dst_addr_size;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_sec_ctrl_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.aux_sec_hdr.sec_ctrl_offset;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_sec_ctrl_sec_lvl_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_sec_ctrl_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return 0;
    }

    return p_parser_data->p_frame[offset] & SECURITY_LEVEL_MASK;
}

__STATIC_INLINE__ bool nrf_802154_frame_sec_ctrl_fc_suppress_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_sec_ctrl_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return false;
    }

    return p_parser_data->p_frame[offset] & FRAME_COUNTER_SUPPRESS_BIT;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_sec_ctrl_key_id_mode_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_sec_ctrl_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return 0;
    }

    return (p_parser_data->p_frame[offset] & KEY_ID_MODE_MASK) >> KEY_ID_MODE_BIT_OFFSET;
}

__STATIC_INLINE__ bool nrf_802154_frame_sec_ctrl_asn_in_nonce_bit_is_set(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_sec_ctrl_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return false;
    }

    return p_parser_data->p_frame[offset] & ASN_IN_NONCE_BIT;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_sec_ctrl_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_sec_ctrl_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_counter_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.aux_sec_hdr.frame_counter_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_counter_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_counter_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_key_id_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.aux_sec_hdr.key_id_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_key_id_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_key_id_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_key_source_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.aux_sec_hdr.key_src_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_key_source_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_key_source_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_key_source_length_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.key_src_size;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_key_index_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.aux_sec_hdr.key_idx_offset;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_key_index_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_key_index_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return 0;
    }

    return p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_header_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mhr.header_ie_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_header_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_ie_header_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_payload_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->mac_payload.mac_payload_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_mac_payload_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_mac_payload_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_command_id_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    if (nrf_802154_frame_type_get(p_parser_data) != FRAME_TYPE_COMMAND)
    {
        return NRF_802154_FRAME_INVALID_OFFSET;
    }

    // MAC command frames containing Payload Information Elements are not supported.
    return nrf_802154_frame_mac_payload_offset_get(p_parser_data);
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_mac_command_id_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_mac_command_id_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_mfr_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return nrf_802154_frame_length_get(p_parser_data) - FCS_SIZE + PHR_SIZE;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_mfr_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_mfr_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_mic_size_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.mic_size;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_payload_length_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t mic_len       = nrf_802154_frame_mic_size_get(p_parser_data);
    uint8_t payload_start = nrf_802154_frame_mac_payload_offset_get(p_parser_data);
    uint8_t payload_end   = nrf_802154_frame_mfr_offset_get(p_parser_data) - mic_len;

    return payload_end - payload_start;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_mac_header_length_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t mhr_start = nrf_802154_frame_psdu_offset_get(p_parser_data);
    uint8_t mhr_end   = nrf_802154_frame_mac_payload_offset_get(p_parser_data);

    if (mhr_end == NRF_802154_FRAME_INVALID_OFFSET)
    {
        mhr_end = nrf_802154_frame_mfr_offset_get(p_parser_data);
    }

    return mhr_end - mhr_start;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_dst_addressing_end_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.dst_addressing_end_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_dst_addressing_end_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_dst_addressing_end_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_addressing_end_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.addressing_end_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_addressing_end_get(
    const nrf_802154_frame_t * p_parser_data)
{
    uint8_t offset = nrf_802154_frame_addressing_end_offset_get(p_parser_data);

    if (offset == NRF_802154_FRAME_INVALID_OFFSET)
    {
        return NULL;
    }

    return &p_parser_data->p_frame[offset];
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_aux_sec_hdr_end_offset_get(
    const nrf_802154_frame_t * p_parser_data)
{
    return p_parser_data->helper.aux_sec_hdr_end_offset;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_header_ie_iterator_begin(
    const uint8_t * p_ie_header)
{
    return p_ie_header;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_content_address_get(
    const uint8_t * p_ie_iterator)
{
    return p_ie_iterator + IE_DATA_OFFSET;
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_length_get(const uint8_t * p_ie_iterator)
{
    return p_ie_iterator[IE_LENGTH_OFFSET] & IE_LENGTH_MASK;
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_iterator_next(
    const uint8_t * p_ie_iterator)
{
    return nrf_802154_frame_ie_content_address_get(p_ie_iterator)
           + nrf_802154_frame_ie_length_get(p_ie_iterator);
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_id_get(const uint8_t * p_ie_iterator)
{
    return (p_ie_iterator[IE_ID_OFFSET_0] >> 7) | (p_ie_iterator[IE_ID_OFFSET_1] << 1);
}

__STATIC_INLINE__ bool nrf_802154_frame_ie_iterator_end(const uint8_t * p_ie_iterator,
                                                        const uint8_t * p_end_addr)
{
    uint8_t ie_id = nrf_802154_frame_ie_id_get(p_ie_iterator);

    return ((nrf_802154_frame_ie_length_get(p_ie_iterator) == 0) &&
            ((ie_id == IE_HT1) || (ie_id == IE_HT2)))
           || (p_ie_iterator >= p_end_addr);
}

__STATIC_INLINE__ uint32_t nrf_802154_frame_ie_vendor_oui_get(const uint8_t * p_ie_iterator)
{
    return little_24_to_host((uint8_t *)&p_ie_iterator[IE_DATA_OFFSET + IE_VENDOR_OUI_OFFSET]);
}

__STATIC_INLINE__ uint8_t nrf_802154_frame_ie_vendor_thread_subtype_get(
    const uint8_t * p_ie_iterator)
{
    return p_ie_iterator[IE_DATA_OFFSET + IE_VENDOR_THREAD_SUBTYPE_OFFSET];
}

__STATIC_INLINE__ const uint8_t * nrf_802154_frame_ie_vendor_thread_data_addr_get(
    const uint8_t * p_ie_iterator)
{
    return nrf_802154_frame_ie_content_address_get(p_ie_iterator) +
           IE_VENDOR_THREAD_DATA_OFFSET;
}

#endif /* NRF_802154_FRAME_DECLARE_ONLY */

#endif // NRF_802154_FRAME_H
