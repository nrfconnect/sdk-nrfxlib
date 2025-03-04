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

#include "nrf_802154_encrypt.h"

#include "nrf_802154_assert.h"
#include <string.h>

#include "nrf_802154_aes_ccm.h"
#include "nrf_802154_const.h"
#include "nrf_802154_pib.h"
#include "nrf_802154_types_internal.h"
#include "mac_features/nrf_802154_frame_parser.h"
#include "mac_features/nrf_802154_security_pib.h"

/**
 * @brief Copies memory in reversed byte order.
 *
 * @note Source and destination buffers must not be NULL.
 *
 * @param[out] p_dst   Pointer to the destination buffer.
 * @param[in]  p_src   Pointer to the source buffer.
 * @param[in]  n       The number of bytes to copy.
 */
static inline void memcpy_rev(void * p_dst, const void * p_src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        *((uint8_t *)p_dst + i) = *((uint8_t *)p_src + n - 1 - i);
    }
}

static bool frame_version_is_2015_or_above(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    switch (nrf_802154_frame_parser_frame_version_get(p_frame_data))
    {
        case FRAME_VERSION_0:
        case FRAME_VERSION_1:
            return false;

        case FRAME_VERSION_2:
            return true;

        default:
            return true;
    }
}

static uint8_t mic_level_from_security_level_get(uint8_t security_level)
{
    // According to the specification, two least significant bits of the security level
    // indicate provided level of data authenticity
    return security_level & SECURITY_LEVEL_MIC_LEVEL_MASK;
}

static bool a_data_and_m_data_prepare(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    nrf_802154_aes_ccm_data_t            * p_aes_ccm_data)
{
    bool result = true;

    uint8_t open_payload_len = 0;

    if ((nrf_802154_frame_parser_frame_type_get(p_frame_data) == FRAME_TYPE_COMMAND) &&
        (!frame_version_is_2015_or_above(p_frame_data)))
    {
        open_payload_len = MAC_CMD_COMMAND_ID_SIZE;
    }

    // It is assumed that the provided frame has a placeholder of appropriate length for MIC
    // at the end. The algorithm inputs should only contain MAC payload, so the MIC placeholder
    // of the below length should be removed
    uint8_t * p_mac_hdr   = (uint8_t *)nrf_802154_frame_parser_psdu_get(p_frame_data);
    uint8_t   mac_hdr_len =
        nrf_802154_frame_parser_mac_header_length_get(p_frame_data) + open_payload_len;
    uint8_t * p_mac_payload =
        (uint8_t *)(nrf_802154_frame_parser_mac_payload_get(p_frame_data) + open_payload_len);
    uint8_t mac_payload_len =
        nrf_802154_frame_parser_mac_payload_length_get(p_frame_data) - open_payload_len;

    switch (nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_frame_data))
    {
        case SECURITY_LEVEL_NONE:
            // No data authenticity nor data confidentiality
            p_aes_ccm_data->auth_data           = NULL;
            p_aes_ccm_data->auth_data_len       = 0;
            p_aes_ccm_data->plain_text_data     = NULL;
            p_aes_ccm_data->plain_text_data_len = 0;
            break;

        case SECURITY_LEVEL_MIC_32:
        case SECURITY_LEVEL_MIC_64:
        case SECURITY_LEVEL_MIC_128:
            // Data authenticity without data confidentiality
            p_aes_ccm_data->auth_data           = p_mac_hdr;
            p_aes_ccm_data->auth_data_len       = mac_hdr_len + mac_payload_len;
            p_aes_ccm_data->plain_text_data     = NULL;
            p_aes_ccm_data->plain_text_data_len = 0;
            break;

        case SECURITY_LEVEL_ENC_MIC_32:
        case SECURITY_LEVEL_ENC_MIC_64:
        case SECURITY_LEVEL_ENC_MIC_128:
            // Data authenticity and data confidentiality
            p_aes_ccm_data->auth_data           = p_mac_hdr;
            p_aes_ccm_data->auth_data_len       = mac_hdr_len;
            p_aes_ccm_data->plain_text_data     = p_mac_payload;
            p_aes_ccm_data->plain_text_data_len = mac_payload_len;
            break;

        default:
            result = false;
            break;
    }

    return result;
}

/**
 * @brief Retrieves key to be used for the AES CCM transformation.
 *
 * @param[in]   p_frame_data     Pointer to the frame parser data.
 * @param[out]  p_aes_ccm_data   Pointer to AES CCM transformation data to be filled.
 */
bool aes_ccm_data_key_prepare(const nrf_802154_frame_parser_data_t * p_frame_data,
                              nrf_802154_aes_ccm_data_t            * p_aes_ccm_data)
{
    nrf_802154_key_id_t key_id =
    {
        .mode     = nrf_802154_frame_parser_sec_ctrl_key_id_mode_get(p_frame_data),
        .p_key_id = (uint8_t *)nrf_802154_frame_parser_key_id_get(p_frame_data),
    };

    return NRF_802154_SECURITY_ERROR_NONE ==
           nrf_802154_security_pib_key_use(&key_id, p_aes_ccm_data->key);
}

/**
 * @brief Generates a CCM nonce.
 *
 * @param[in]  p_frame_data   Pointer to the frame parser data.
 * @param[out] p_nonce        Pointer to the buffer to be filled with generated nonce.
 *
 * @retval  true   Nonce was generated successfully.
 * @retval  false  Nonce could not be generated.
 */
bool aes_ccm_nonce_generate(const nrf_802154_frame_parser_data_t * p_frame_data,
                            uint8_t                              * p_nonce)
{
    if ((p_frame_data == NULL) || (p_nonce == NULL))
    {
        return false;
    }

    const uint8_t * p_src_addr = nrf_802154_pib_extended_address_get();
    uint8_t         offset     = 0;

    memcpy_rev(p_nonce, p_src_addr, EXTENDED_ADDRESS_SIZE);
    offset += EXTENDED_ADDRESS_SIZE;

    // Byte order for Frame Counter gets reversed as defined
    // in 802.15.4-2015 Std Chapters 9.3.1 and Annex B.2
    memcpy_rev((p_nonce + offset),
               nrf_802154_frame_parser_frame_counter_get(p_frame_data),
               FRAME_COUNTER_SIZE);
    offset += FRAME_COUNTER_SIZE;

    p_nonce[offset] = nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_frame_data);

    return true;
}

/**
 * @brief Prepares _a_ data and _m_ data strings as defined in IEEE 802.15.4-2015 9.3.4.2.
 *
 * @param[in]   p_frame_data      Pointer to the frame parser data.
 * @param[in]   p_aux_sec_hdr     Pointer to Auxiliary Security Header data.
 * @param[out]  p_aes_ccm_data    Pointer to AES CCM transformation data to be filled.
 *
 * @retval  true   Data was prepared successfully.
 * @retval  false  Data could not be prepared.
 */
bool aes_ccm_data_a_data_and_m_data_prepare(
    const nrf_802154_frame_parser_data_t * p_frame_data,
    nrf_802154_aes_ccm_data_t            * p_aes_ccm_data)
{
    bool result;

    switch (nrf_802154_frame_parser_frame_type_get(p_frame_data))
    {
        case FRAME_TYPE_ACK:
        // Fallthrough
        case FRAME_TYPE_DATA:
        // Fallthrough
        case FRAME_TYPE_COMMAND:
            result = a_data_and_m_data_prepare(p_frame_data, p_aes_ccm_data);
            break;

        case FRAME_TYPE_BEACON:
        // Fallthrough
        case FRAME_TYPE_EXTENDED:
        // Fallthrough
        case FRAME_TYPE_FRAGMENT:
        // Fallthrough
        case FRAME_TYPE_MULTIPURPOSE:
            // This frame type is currently unsupported.
            result = false;
            break;

        default:
            // No more frame types exist.
            result = false;
            NRF_802154_ASSERT(false);
            break;
    }

    return result;
}

/**
 * @brief Prepares data for AES CCM transformation.
 *
 * @param[in]   p_frame_data     Pointer to the frame parser data.
 * @param[out]  p_aes_ccm_data   Pointer to AES CCM transformation data to be filled.
 *
 * @retval  true    AES CCM transformation data was prepared successfully.
 * @retval  false   AES CCM transformation could not be prepared.
 */
static bool aes_ccm_data_content_prepare(const nrf_802154_frame_parser_data_t * p_frame_data,
                                         nrf_802154_aes_ccm_data_t            * p_aes_ccm_data)
{
    bool retval = false;

    do
    {
        if (!aes_ccm_data_key_prepare(p_frame_data, p_aes_ccm_data))
        {
            // Return immediately if specified key could not be found
            break;
        }

        if (!aes_ccm_nonce_generate(p_frame_data, p_aes_ccm_data->nonce))
        {
            // Return immediately if nonce could not be generated
            break;
        }

        // Fill _a_ data (authenticity) and _m_ data (confidentiality)
        if (!aes_ccm_data_a_data_and_m_data_prepare(p_frame_data, p_aes_ccm_data))
        {
            // Return immediately if transformation data could not be generated
            break;
        }

        // Fill in the remaining data
        p_aes_ccm_data->mic_level = mic_level_from_security_level_get(
            nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_frame_data));
        p_aes_ccm_data->raw_frame = (uint8_t *)p_frame_data->p_frame;

        retval = true;
    }
    while (0);

    return retval;
}

bool nrf_802154_encrypt_ack_prepare(const nrf_802154_frame_parser_data_t * p_ack_data)
{
    nrf_802154_aes_ccm_data_t aes_ccm_data;
    bool                      success = false;

    if (!nrf_802154_frame_parser_security_enabled_bit_is_set(p_ack_data) ||
        (nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_ack_data) == SECURITY_LEVEL_NONE))
    {
        success = true;
    }
    else if (aes_ccm_data_content_prepare(p_ack_data, &aes_ccm_data))
    {
        // Algorithm's inputs prepared. Schedule transformation
        success = nrf_802154_aes_ccm_transform_prepare(&aes_ccm_data);
    }
    else
    {
        // Intentionally empty
    }

    return success;
}

void nrf_802154_encrypt_ack_reset(void)
{
    nrf_802154_aes_ccm_transform_reset();
}

bool nrf_802154_encrypt_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function)
{
    nrf_802154_aes_ccm_transform_reset();

    if (p_params->frame_props.is_secured)
    {
        // The frame is already secured. Pass.
        return true;
    }

    if ((p_frame[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) == FRAME_TYPE_MULTIPURPOSE)
    {
        // Multipurpose frame parsing is not implemented, so skip encryption.
        return true;
    }

    nrf_802154_frame_parser_data_t frame_data;
    nrf_802154_aes_ccm_data_t      aes_ccm_data;
    bool                           success = false;

    success = nrf_802154_frame_parser_data_init(p_frame,
                                                p_frame[PHR_OFFSET] + PHR_SIZE,
                                                PARSE_LEVEL_FULL,
                                                &frame_data);
    NRF_802154_ASSERT(success);
    (void)success;

    if (!nrf_802154_frame_parser_security_enabled_bit_is_set(&frame_data) ||
        (nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(&frame_data) == SECURITY_LEVEL_NONE))
    {
        success = true;
    }
    else if (aes_ccm_data_content_prepare(&frame_data, &aes_ccm_data))
    {
        // Algorithm's inputs prepared. Schedule transformation
        success = nrf_802154_aes_ccm_transform_prepare(&aes_ccm_data);
    }
    else
    {
        success = false;
    }

    if (!success)
    {
        nrf_802154_transmit_done_metadata_t metadata = {};

        metadata.frame_props = p_params->frame_props;
        notify_function(p_frame, NRF_802154_TX_ERROR_KEY_ID_INVALID, &metadata);
    }

    return success;
}

bool nrf_802154_encrypt_tx_started_hook(uint8_t * p_frame)
{
    // The provided pointer is the original buffer. It doesn't need to be changed,
    // because the AES-CCM* module is aware of two separate buffers (original vs work buffer)
    nrf_802154_aes_ccm_transform_start(p_frame);

    return true;
}

void nrf_802154_encrypt_tx_ack_started_hook(uint8_t * p_ack)
{
    nrf_802154_aes_ccm_transform_start(p_ack);
}

bool nrf_802154_encrypt_tx_failed_hook(uint8_t * p_frame, nrf_802154_tx_error_t error)
{
    (void)error;

    nrf_802154_aes_ccm_transform_abort(p_frame);

    return true;
}

void nrf_802154_encrypt_tx_ack_failed_hook(uint8_t * p_ack, nrf_802154_tx_error_t error)
{
    (void)error;

    nrf_802154_aes_ccm_transform_abort(p_ack);
}
