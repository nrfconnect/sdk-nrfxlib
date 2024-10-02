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

#ifndef NRF_802154_IE_WRITER_H__
#define NRF_802154_IE_WRITER_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_802154_types_internal.h"

/**
 * @defgroup nrf_802154_ie_writer Radio driver Information Element data injection feature.
 * @{
 * @ingroup nrf_802154
 * @brief Information element writer module.
 */

/**
 * @brief Resets the IE writer module to pristine state.
 */

void nrf_802154_ie_writer_reset(void);

/**
 * @brief Prepares to write Information Element data to all elements recognized by the module.
 *
 * Calls to this function put the IE writer module into an armed state.
 * In the armed state the addresses of recognized Information Element values
 * are latched. The IE writer module shall perform necessary calculations and write
 * the results to the latched addresses during the execution of either
 * nrf_802154_ie_writer_tx_ack_started_hook or nrf_802154_ie_writer_tx_started_hook hooks.
 *
 * It is expected, that the layout of the frame does not change while the IE writer is in
 * armed state.
 *
 * If this function detects a malformed header IE, the module state shall be reset to the
 * unarmed state. There is no guarantee that all malformed header IEs will be detected, but
 * it is guaranteed that the writer module shall never modify memory past the p_end_addr argument.
 *
 * @param[in]  p_ie_header  Address of the IE header.
 * @param[in]  p_end_addr   First invalid address after p_ie_header.
 *
 */
void nrf_802154_ie_writer_prepare(uint8_t * p_ie_header, const uint8_t * p_end_addr);

/**
 * @brief Transmission setup hook for the IE writer module.
 *
 * This hook parses the frame in search of header Information Elements and arms
 * the IE writer module. The IE writer module shall write data to recognized
 * Information Elements during the execution of nrf_802154_ie_writer_tx_ack_started_hook
 * and nrf_802154_ie_writer_tx_started_hook hooks.
 *
 * If this function detects a malformed frame, the module state shall be reset to the
 * unarmed state. There is no guarantee that all malformed frames will be detected.
 *
 * @param[in]  p_frame          Pointer to the buffer that contains the PHR and PSDU
 *                              of the transmitted frame.
 * @param[in]  p_params         Pointer to the transmission parameters.
 * @param[in]  notify_function  Function to be called to notify transmission failure.
 *
 * @retval  true         Always succeeds.
 */
bool nrf_802154_ie_writer_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function);

/**
 * @brief ACK TX started hook for the IE writer module.
 *
 * This hook executes during the EVENT_ADDRESS radio event. If the IE writer is armed,
 * the IEs recognized by the writer module shall be modified. If IE writer is not armed
 * no action shall be performed.
 *
 * The function writes to memory addresses that were found suitable by the call to
 * nrf_802154_ie_writer_prepare function. No parsing of the frame pointed by the p_ack
 * argument is performed and the argument itself is ignored due to timing requirements
 * that this hook must not exceed.
 *
 * @param[in]  p_ack    Pointer to the buffer that contains the PHR and PSDU of the ACK frame.
 *
 */
void nrf_802154_ie_writer_tx_ack_started_hook(uint8_t * p_ack);

/**
 * @brief TX started hook for the IE writer module.
 *
 * This hook executes during the EVENT_ADDRESS radio event. If the IE writer is armed,
 * the IEs recognized by the writer module shall be modified. If IE writer is not armed
 * no action shall be performed.
 *
 * The function writes to memory addresses that were found suitable by the call to
 * nrf_802154_ie_writer_prepare function. No parsing of the frame pointed by the p_frame
 * argument is performed and the argument itself is ignored due to timing requirements
 * that this hook must not exceed.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains the PHR and PSDU of the transmitted frame.
 *
 * @retval  true  Always succeeds.
 */
bool nrf_802154_ie_writer_tx_started_hook(uint8_t * p_frame);

/**
 * @brief Sets the value of CSL period to inject into the CSL information element.
 *
 * @param[in]  period  CSL period value.
 */
void nrf_802154_ie_writer_csl_period_set(uint16_t period);

/**
 * @brief Sets the anchor time based on which the next CSL window time and the CSL phase is calculated.
 *
 * @param[in]  anchor_time  Anchor time value.
 */
void nrf_802154_ie_writer_csl_anchor_time_set(uint64_t anchor_time);

/**
 * @brief Sets the value of CST period to inject into the CST information element.
 *
 * @param[in]  period  CST period value.
 */
void nrf_802154_ie_writer_cst_period_set(uint16_t period);

/**
 * @brief Sets the anchor time based on which the next CST window time and the CST phase is calculated.
 *
 * @param[in]  anchor_time Anchor time value.
 */
void nrf_802154_ie_writer_cst_anchor_time_set(uint64_t anchor_time);

/**
 *@}
 **/

#endif // NRF_802154_IE_WRITER_H__
