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
 * @brief Module that contains an acknowledgement generator for the 802.15.4 radio driver.
 *
 */

#ifndef NRF_802154_ACK_GENERATOR_H
#define NRF_802154_ACK_GENERATOR_H

#include <stdint.h>

#include "mac_features/nrf_802154_frame_parser.h"

/** Initializes the ACK generator module. */
void nrf_802154_ack_generator_init(void);

/** @brief Resets the ACK generator module.
 *
 * @note This function should be called for every received frame to be acknowledged before
 *       @ref nrf_802154_ack_generator_create is called for that frame.
 */
void nrf_802154_ack_generator_reset(void);

/** @brief Creates an ACK in response to the provided frame and inserts it into a radio buffer.
 *
 * @note Only those contents of the frame being acknowledged marked by @p p_frame_data as valid
 * are used for ACK generation. If any data necessary to generate an ACK is missing or marked as
 * invalid by @p p_frame_data, this function returns NULL. Once more data becomes available and valid,
 * this function can be called again and the generation will be continued. That allows for
 * generating ACK iteratively as data to be acknowledged is being received.
 *
 * @param [in]  p_frame_data  Pointer to the parser data of the frame for which an Ack
 *                            will be generated.
 *
 * @returns  Either pointer to a constant buffer that contains PHR and PSDU
 *           of the created ACK frame, or NULL when the response cannot be created.
 */
uint8_t * nrf_802154_ack_generator_create(
    const nrf_802154_frame_parser_data_t * p_frame_data);

#endif // NRF_802154_ACK_GENERATOR_H
