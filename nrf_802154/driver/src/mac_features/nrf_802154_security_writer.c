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

/**
 * @file
 *   This file implements the security header data injector for the 802.15.4 driver.
 *
 */

#include "mac_features/nrf_802154_security_writer.h"

#include "mac_features/nrf_802154_frame_parser.h"
#include "mac_features/nrf_802154_security_pib.h"
#include "nrf_802154_config.h"
#include "nrf_802154_const.h"
#include "nrf_802154_tx_work_buffer.h"
#include "nrf_802154_utils_byteorder.h"

#include "nrf_802154_assert.h"
#include <stddef.h>

#if NRF_802154_SECURITY_WRITER_ENABLED

static bool m_frame_counter_injected; ///< Flag that indicates if frame counter was injected.

/**
 * @brief Populates the key ID structure with key ID mode and source.
 *
 * @param[in]   p_frame_data Pointer to the frame parser data.
 * @param[out]  p_key_id     Pointer to the @ref nrf_802154_key_id_t structure to be populated.
 */
static void key_id_prepare(const nrf_802154_frame_parser_data_t * p_frame_data,
                           nrf_802154_key_id_t                  * p_key_id)
{
    p_key_id->mode = nrf_802154_frame_parser_sec_ctrl_key_id_mode_get(p_frame_data);

    switch (p_key_id->mode)
    {
        case KEY_ID_MODE_0:
            /* There is no need to retrieve the key ID as only one key ID with mode 0 is
             * supported. Therefore mode 0 unambiguously identifies the key.
             */
            break;

        case KEY_ID_MODE_1:
        /* Fallthrough */
        case KEY_ID_MODE_2:
        /* Fallthrough */
        case KEY_ID_MODE_3:
            p_key_id->p_key_id = (uint8_t *)nrf_802154_frame_parser_key_id_get(p_frame_data);
            break;

        default:
            /* Key Identifier Mode is encoded on 2 bits and has 4 possible values, all handled in
             * the above cases. Ending up in the default case indicates the frame parser is bugged.
             */
            NRF_802154_ASSERT(false);
    }
}

/**
 * @brief Injects frame counter value into given address with security enabled.
 *
 * @param[in]   p_frame_data Pointer to the frame parser data.
 * @param[in]   p_key_id     Pointer to the key identifying which counter shall be retrieved.
 *
 * @retval      NRF_802154_SECURITY_ERROR_NONE  Frame counter was injected successfully.
 *
 * @returns     nrf_802154_security_error_t     The security error code that occurred during
 *                                              the frame counter injection.
 */
static nrf_802154_security_error_t frame_counter_inject(
    nrf_802154_frame_parser_data_t * p_frame_data,
    nrf_802154_key_id_t            * p_key_id)
{
    uint32_t  frame_counter;
    uint8_t * p_frame_counter =
        (uint8_t *)nrf_802154_frame_parser_frame_counter_get(p_frame_data);
    nrf_802154_security_error_t err;

    err = nrf_802154_security_pib_frame_counter_get_next(&frame_counter, p_key_id);

    switch (err)
    {
        case NRF_802154_SECURITY_ERROR_NONE:
            if (p_frame_counter != NULL)
            {
                /* Frame counter suppression is off. Inject the frame counter. */
                host_32_to_little(frame_counter, p_frame_counter);
            }
            break;

        case NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND:
        /* Fallthrough */
        case NRF_802154_SECURITY_ERROR_FRAME_COUNTER_OVERFLOW:
            break;

        default:
            /* nrf_802154_security_pib_frame_counter_get_next function shall not return other
             * error codes than those handled in the above cases. If it does then it is a bug.
             */
            NRF_802154_ASSERT(false);
    }

    return err;
}

/**
 * @brief Checks if security is enabled for the given Auxiliary Security Header.
 *
 * @param[in]   p_frame_data Pointer to the frame parser data to be checked.
 *
 * @retval      true        Security is enabled.
 * @retbal      false       Security is disabled.
 */
static bool security_is_enabled(const nrf_802154_frame_parser_data_t * p_frame_data)
{
    return (NULL != nrf_802154_frame_parser_sec_ctrl_get(p_frame_data)) &&
           (SECURITY_LEVEL_NONE != nrf_802154_frame_parser_sec_ctrl_sec_lvl_get(p_frame_data));
}

bool nrf_802154_security_writer_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function)
{
    nrf_802154_frame_parser_data_t frame_data;
    nrf_802154_key_id_t            key_id;
    bool                           result = false;

    key_id.p_key_id          = NULL;
    m_frame_counter_injected = false;

    if (p_params->frame_props.dynamic_data_is_set)
    {
        // The frame has a frame counter field already set. Pass.
        return true;
    }

    if ((p_frame[FRAME_TYPE_OFFSET] & FRAME_TYPE_MASK) == FRAME_TYPE_MULTIPURPOSE)
    {
        // Multipurpose frame parsing is not implemented, so skip security.
        return true;
    }

    result = nrf_802154_frame_parser_data_init(p_frame,
                                               p_frame[PHR_OFFSET] + PHR_SIZE,
                                               PARSE_LEVEL_AUX_SEC_HDR_END,
                                               &frame_data);
    NRF_802154_ASSERT(result);
    (void)result;

    do
    {
        if (!security_is_enabled(&frame_data))
        {
            /* Security is not enabled. Pass. */
            result = true;
            break;
        }

        /* Prepare key ID for key validation. */
        key_id_prepare(&frame_data, &key_id);

        nrf_802154_security_error_t err = frame_counter_inject(&frame_data, &key_id);

        switch (err)
        {
            case NRF_802154_SECURITY_ERROR_NONE:
                result                   = true;
                m_frame_counter_injected = true;
                break;

            case NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND:
            {
                nrf_802154_transmit_done_metadata_t metadata = {};

                metadata.frame_props = p_params->frame_props;
                notify_function(p_frame, NRF_802154_TX_ERROR_KEY_ID_INVALID, &metadata);
                result = false;
            }
            break;

            case NRF_802154_SECURITY_ERROR_FRAME_COUNTER_OVERFLOW:
            {
                nrf_802154_transmit_done_metadata_t metadata = {};

                metadata.frame_props = p_params->frame_props;
                notify_function(p_frame, NRF_802154_TX_ERROR_FRAME_COUNTER_ERROR, &metadata);
                result = false;
            }
            break;

            default:
                /* frame_counter_inject function shall not return other error codes than those
                 * handled in the above cases. If it does then it is a bug.
                 */
                NRF_802154_ASSERT(false);
                result = false;
        }
    }
    while (0);

    return result;
}

bool nrf_802154_security_writer_tx_started_hook(uint8_t * p_frame)
{
    if (m_frame_counter_injected)
    {
        /* Mark dynamic data updated in the work buffer. */
        nrf_802154_tx_work_buffer_is_dynamic_data_updated_set();
    }

    return true;
}

#endif // NRF_802154_SECURITY_WRITER_ENABLED
