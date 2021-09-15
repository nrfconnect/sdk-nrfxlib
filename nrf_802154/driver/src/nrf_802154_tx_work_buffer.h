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
 * @brief Module that provides work buffer for transmissions.
 *
 * This module performs the following functions:
 *  - stores properties of the frame to be transmitted
 *  - optionally provides an additional work buffer to modify frame data without affecting
 *    the original frame's contents
 *
 * By default, the using of work buffer is turned off. If desired, it can be turned on with
 * @ref nrf_802154_tx_work_buffer_enable_for.
 *
 */

#ifndef NRF_802154_TX_WORK_BUFFER_H_
#define NRF_802154_TX_WORK_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#include "nrf_802154_types.h"

/**
 * @brief Resets work buffer.
 *
 * Internal state of the work buffer is completely reset after this call.
 * If @p p_frame_props is not NULL, the work buffer properties are set according to the contents
 * of the given structure.
 *
 * @param[in]  p_frame_props  Pointer to a structure containing the initial properties that will
 *                            be set for the work buffer.
 */
void nrf_802154_tx_work_buffer_reset(const nrf_802154_transmitted_frame_props_t * p_frame_props);

/**
 * @brief Enables work buffer for provided frame.
 *
 * In order to avoid performing potentially hazardous operations on the original buffer containing
 * data to be transmitted, a work buffer can be allocated to the original frame. After all necessary
 * modifications are performed on the work buffer, its contents can be copied back to the original
 * buffer so that its state is always well-defined.
 *
 * @pre Before enabling the work buffer must be initialized with @ref nrf_802154_tx_work_buffer_reset
 *
 * @param[in]  p_original_frame  Pointer to the original frame to be transmitted.
 *
 * @returns  Pointer to a work buffer bound to @p p_original_frame.
 */
uint8_t * nrf_802154_tx_work_buffer_enable_for(uint8_t * p_original_frame);

/**
 * @brief Gets buffer to be transmitted.
 *
 * Depending on whether @p p_original_frame has a work buffer bound, this function either returns
 * that work buffer or the original frame, if no such binding exists.
 *
 * @param[in]  p_original_frame  Pointer to the original frame to be transmitted.
 *
 * @returns  Pointer to a buffer to be transmitted.
 */
const uint8_t * nrf_802154_tx_work_buffer_get(const uint8_t * p_original_frame);

/**
 * @brief Updates the original buffer with its bound work buffer contents.
 *
 * Processing performed on the work buffer might require copying its contents back to the original
 * buffer. This function performs all necessary updates of the original buffer in place.
 *
 * @param[inout]    p_original_frame    Pointer to the original frame to be transmitted.
 * @param[out]      p_frame_props       Pointer to a structure to which properties of the frame
 *                                      to be transmitted are stored.
 */
void nrf_802154_tx_work_buffer_original_frame_update(
    uint8_t                              * p_original_frame,
    nrf_802154_transmitted_frame_props_t * p_frame_props);

/**
 * @brief Marks a work buffer as secured.
 */
void nrf_802154_tx_work_buffer_is_secured_set(void);

/**
 * @brief Marks a work buffer as containing updated dynamic data.
 */
void nrf_802154_tx_work_buffer_is_dynamic_data_updated_set(void);

/**
 * @brief Sets offset of encryption plain text for the work buffer.
 *
 * @param[in]  offset  Offset of encryption plain text to be set.
 */
void nrf_802154_tx_work_buffer_plain_text_offset_set(uint8_t offset);

#endif // NRF_802154_TX_WORK_BUFFER_H_
