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
 * @brief Module that contains frame parsing utilities for the 802.15.4 radio driver.
 *
 */

#ifndef NRF_802154_FRAME_PARSER_H
#define NRF_802154_FRAME_PARSER_H

#include "nrf_802154_const.h"
#include "nrf_802154_utils_byteorder.h"
#include "nrf_802154_frame.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Clears the frame parser data.
 */
static inline void nrf_802154_frame_parser_data_clear(nrf_802154_frame_t * p_frame)
{
    p_frame->p_frame     = NULL;
    p_frame->parse_level = PARSE_LEVEL_NONE;
}

/**
 * @brief Initializes the frame parse data and parses the frame.
 *
 * This function initializes and parses the given frame to the requested level.
 * The parsing is performed to the extent that is achievable with the amount
 * of valid data. The resulting parse level may be higher that the requested one,
 * if it can be achieved without additional parsing.
 *
 * @param[in]    p_frame               Pointer to a frame.
 * @param[in]    valid_data_len        Number of valid bytes in the frame.
 * @param[in]    requested_parse_level Requested parse level.
 * @param[out]   p_parser_data         Pointer to a parser data that will be initialized.
 *
 * @retval  true  The parsing succeeded and requested parse level was achieved.
 * @retval  false The parsing failed or requested parse level could not be achieved.
 */
bool nrf_802154_frame_parser_data_init(uint8_t                       * p_frame,
                                       uint8_t                         valid_data_len,
                                       nrf_802154_frame_parser_level_t requested_parse_level,
                                       nrf_802154_frame_t            * p_parser_data);

/**
 * @brief Extends the valid data and parses the frame.
 *
 * This function extends the valid data and parses the given frame to the requested level.
 * The parsing is performed to the extent that is achievable with the amount
 * of valid data. The resulting parse level may be higher that the requested one,
 * if it can be achieved without additional parsing.
 *
 * @param[inout] p_parser_data         Pointer to a parser data to extend.
 * @param[in]    valid_data_len        Number of valid bytes in the frame.
 * @param[in]    requested_parse_level Requested parse level.
 *
 * @retval  true  The parsing succeeded and requested parse level was achieved.
 * @retval  false The parsing failed or requested parse level could not be achieved.
 */
bool nrf_802154_frame_parser_valid_data_extend(
    nrf_802154_frame_t            * p_parser_data,
    uint8_t                         valid_data_len,
    nrf_802154_frame_parser_level_t requested_parse_level);

#endif // NRF_802154_FRAME_PARSER_H
