/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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
 * @file Common types provided by the nRF 802.15.4 Service Layer.
 *
 */

#ifndef NRF_802154_SL_TYPES_H__
#define NRF_802154_SL_TYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <nrfx.h>

/**
 * @brief Handle of a hardware event.
 *
 * This handle can be used to trigger peripheral tasks through hardware.
 */
typedef struct
{
    /**
     * @brief Actual handle to use when configuring hardware.
     *
     * On nRF52 series, this field represents an address of a hardware event
     * that can be written to a PPI register.
     *
     * On nRF53 series, this field depending on @c shared might either represent
     * an address of a hardware event that should publish to a DPPI channel,
     * or a number of a DPPI channel that the hardware event of interest
     * is already publishing to.
     */
    uint32_t event_addr;

#if defined(DPPI_PRESENT)
    /**
     * @brief Indicates if the event is already publishing to a DPPI channel.
     */
    bool shared;

#endif
} nrf_802154_sl_event_handle_t;

#endif // NRF_802154_SL_TYPES_H__
