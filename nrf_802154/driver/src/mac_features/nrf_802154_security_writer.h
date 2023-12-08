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

#ifndef NRF_802154_SECURITY_WRITER_H__
#define NRF_802154_SECURITY_WRITER_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_802154_types_internal.h"

/**
 * @brief Transmission setup hook for the security writer module.
 *
 * This hook parses the frame to check availability of the key identified by the frame's security
 * header and injects a frame counter associated with the key. If the frame's security level is
 * zero, then no processing occurs.
 *
 * If this function detects a malformed frame, the higher layer is notified of transmission
 * failure. A frame is considered malformed if requested key is invalid.
 *
 * @param[in]  p_frame          Pointer to the buffer that contains the PHR and PSDU
 *                              of the transmitted frame.
 * @param[in]  p_params         Pointer to the transmission parameters.
 * @param[in]  notify_function  Function to be called to notify transmission failure.
 *
 * @retval  false        Frame security header was not processed successfully.
 * @retval  true         Frame security header was processed successfully.
 */
bool nrf_802154_security_writer_tx_setup(
    uint8_t                                 * p_frame,
    nrf_802154_transmit_params_t            * p_params,
    nrf_802154_transmit_failed_notification_t notify_function);

/**
 * @brief TX started hook for the security writer module.
 *
 * Depending on whether @ref nrf_802154_security_writer_tx_setup has injected a frame counter
 * into the transmitted frame, this hook marks the frame counter as updated.
 *
 * @param[in]  p_frame  Pointer to the buffer that contains the PHR and PSDU of the transmitted frame.
 *
 * @retval  true  Always succeeds.
 */
bool nrf_802154_security_writer_tx_started_hook(uint8_t * p_frame);

#endif // NRF_802154_SECURITY_WRITER_H__
