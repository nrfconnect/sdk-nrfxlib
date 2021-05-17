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
#include "nrf_802154_notification.h"
#include "nrf_802154_utils_byteorder.h"

#include <assert.h>
#include <stddef.h>

#if NRF_802154_SECURITY_WRITER_ENABLED

/**
 * @brief Populates the key ID structure with key ID mode and source.
 *
 * @param[in]   p_sec_hdr   Pointer to the parsed security header from which key ID data is to
 *                          be extracted.
 * @param[out]  p_key_id    Pointer to the @ref nrf_802154_key_id_t structure to be populated.
 */
static void key_id_prepare(nrf_802154_frame_parser_aux_sec_hdr_t * p_sec_hdr,
                           nrf_802154_key_id_t                   * p_key_id)
{
    p_key_id->mode = p_sec_hdr->key_id_mode;

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
            p_key_id->p_key_id = (uint8_t *)p_sec_hdr->p_key_id;
            break;

        default:
            /* Key Identifier Mode is encoded on 2 bits and has 4 possible values, all handled in
             * the above cases. Ending up in the default case indicates the frame parser is bugged.
             */
            assert(false);
    }
}

/**
 * @brief Injects frame counter value into given address with security enabled.
 *
 * @param[in]   p_sec_hdr   Pointer to the parsed security header containing frame counter address
 *                          where the retrieved frame counter value is to be stored.
 * @param[in]   p_key_id    Pointer to the key identifying which counter shall be retrieved.
 *
 * @retval      NRF_802154_SECURITY_ERROR_NONE  Frame counter was injected successfully.
 *
 * @returns     nrf_802154_security_error_t     The security error code that occurred during
 *                                              the frame counter injection.
 */
static nrf_802154_security_error_t frame_counter_inject(
    nrf_802154_frame_parser_aux_sec_hdr_t * p_sec_hdr,
    nrf_802154_key_id_t                   * p_key_id)
{
    uint32_t                    frame_counter;
    nrf_802154_security_error_t err;

    err = nrf_802154_security_pib_frame_counter_get_next(&frame_counter, p_key_id);

    switch (err)
    {
        case NRF_802154_SECURITY_ERROR_NONE:
            if (p_sec_hdr->p_frame_counter != NULL)
            {
                /* Frame counter suppression is off. Inject the frame counter. */
                host_32_to_little(frame_counter, (uint8_t *)p_sec_hdr->p_frame_counter);
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
            assert(false);
    }

    return err;
}

/**
 * @brief Checks if security is enabled for the given Auxiliary Security Header.
 *
 * @param[in]   p_sec_hdr   Parsed frame's Auxiliary Security Header to be checked.
 *
 * @retval      true        Security is enabled.
 * @retbal      false       Security is disabled.
 */
static bool security_is_enabled(nrf_802154_frame_parser_aux_sec_hdr_t * p_sec_hdr)
{
    return ((NULL != p_sec_hdr->p_sec_ctrl) && (0 != p_sec_hdr->security_lvl));
}

bool nrf_802154_security_writer_pretransmission(
    const uint8_t                           * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function)
{
    if (p_params->is_retransmission)
    {
        // Pass.
        return true;
    }

    nrf_802154_frame_parser_aux_sec_hdr_t sec_hdr;
    nrf_802154_key_id_t                   key_id;
    bool result = false;

    /* Prepare Auxiliary Security Header for processing. */
    nrf_802154_frame_parser_aux_sec_hdr_parse(p_frame, &sec_hdr);

    do
    {
        if (!security_is_enabled(&sec_hdr))
        {
            /* Security is not enabled. Pass. */
            result = true;
            break;
        }

        /* Prepare key ID for key validation. */
        key_id_prepare(&sec_hdr, &key_id);

        nrf_802154_security_error_t err = frame_counter_inject(&sec_hdr, &key_id);

        switch (err)
        {
            case NRF_802154_SECURITY_ERROR_NONE:
                result = true;
                break;

            case NRF_802154_SECURITY_ERROR_KEY_NOT_FOUND:
                notify_function(p_frame, NRF_802154_TX_ERROR_KEY_ID_INVALID);
                break;

            case NRF_802154_SECURITY_ERROR_FRAME_COUNTER_OVERFLOW:
                notify_function(p_frame, NRF_802154_TX_ERROR_FRAME_COUNTER_ERROR);
                break;

            default:
                /* frame_counter_inject function shall not return other error codes than those
                 * handled in the above cases. If it does then it is a bug.
                 */
                assert(false);
        }
    }
    while (0);

    return result;
}

#endif // NRF_802154_SECURITY_WRITER_ENABLED
