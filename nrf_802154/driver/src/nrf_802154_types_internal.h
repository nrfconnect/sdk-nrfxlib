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

#ifndef NRF_802154_TYPES_INTERNAL_H__
#define NRF_802154_TYPES_INTERNAL_H__

#include "nrf_802154_types.h"
#include "nrf_802154_fal.h"

typedef struct
{
    nrf_802154_transmitted_frame_props_t frame_props;        // !< Properties of the frame to be transmitted.
    nrf_802154_fal_tx_power_split_t      tx_power;           // !< Power to be used when transmitting the frame, split into components to be applied on each stage on transmit path.
    uint8_t                              channel;            // !< Transmission channel
    bool                                 cca;                // !< If the driver is to perform CCA procedure before transmission.
    bool                                 immediate;          // !< If true, the driver schedules transmission immediately or never. If false, the transmission may be postponed
                                                             // until its preconditions are met.
    uint8_t                              extra_cca_attempts; // !< Maximum number of additional CCA attempts that can be performed if the first attempt returns busy channel. Ignored if @ref cca equals @c false.
} nrf_802154_transmit_params_t;

typedef struct
{
    uint8_t * p_frame; // !< Pointer to the frame for which the timeout of ack reception is notified.
} nrf_802154_ack_timeout_handle_params_t;

#endif  // NRF_802154_TYPES_INTERNAL_H__
