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
 * @brief Module that contains debug helpers for the 802.15.4 radio driver for the nRF SoC devices.
 *
 */

#ifndef NRF_802154_DEBUG_H_
#define NRF_802154_DEBUG_H_

#include <stdint.h>

#include "nrf_802154_config.h"
#include "nrf_802154_sl_log.h"
#include "nrf_802154_debug_log.h"
#include "nrf_802154_debug_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_DBG_RADIO_EVT_END           11
#define PIN_DBG_RADIO_EVT_DISABLED      12
#define PIN_DBG_RADIO_EVT_READY         13
#define PIN_DBG_RADIO_EVT_FRAMESTART    14
#define PIN_DBG_RADIO_EVT_EDEND         25
#define PIN_DBG_RADIO_EVT_PHYEND        24

#define PPI_DBG_RADIO_EVT_END           0
#define PPI_DBG_RADIO_EVT_DISABLED      1
#define PPI_DBG_RADIO_EVT_READY         2
#define PPI_DBG_RADIO_EVT_FRAMESTART    3
#define PPI_DBG_RADIO_EVT_EDEND         4
#define PPI_DBG_RADIO_EVT_PHYEND        5

#define GPIOTE_DBG_RADIO_EVT_END        0
#define GPIOTE_DBG_RADIO_EVT_DISABLED   1
#define GPIOTE_DBG_RADIO_EVT_READY      2
#define GPIOTE_DBG_RADIO_EVT_FRAMESTART 3
#define GPIOTE_DBG_RADIO_EVT_EDEND      4
#define GPIOTE_DBG_RADIO_EVT_PHYEND     5

#if ENABLE_DEBUG_GPIO

#define NRF_802154_DEBUG_PINS_USED_MASK            ((1 << PIN_DBG_RADIO_EVT_END) |        \
                                                    (1 << PIN_DBG_RADIO_EVT_DISABLED) |   \
                                                    (1 << PIN_DBG_RADIO_EVT_READY) |      \
                                                    (1 << PIN_DBG_RADIO_EVT_FRAMESTART) | \
                                                    (1 << PIN_DBG_RADIO_EVT_EDEND) |      \
                                                    (1 << PIN_DBG_RADIO_EVT_PHYEND))

#define NRF_802154_DEBUG_PPI_CHANNELS_USED_MASK    ((1 << PPI_DBG_RADIO_EVT_END) |        \
                                                    (1 << PPI_DBG_RADIO_EVT_DISABLED) |   \
                                                    (1 << PPI_DBG_RADIO_EVT_READY) |      \
                                                    (1 << PPI_DBG_RADIO_EVT_FRAMESTART) | \
                                                    (1 << PPI_DBG_RADIO_EVT_EDEND) |      \
                                                    (1 << PPI_DBG_RADIO_EVT_PHYEND))

#define NRF_802154_DEBUG_GPIOTE_CHANNELS_USED_MASK ((1 << GPIOTE_DBG_RADIO_EVT_END) |        \
                                                    (1 << GPIOTE_DBG_RADIO_EVT_DISABLED) |   \
                                                    (1 << GPIOTE_DBG_RADIO_EVT_READY) |      \
                                                    (1 << GPIOTE_DBG_RADIO_EVT_FRAMESTART) | \
                                                    (1 << GPIOTE_DBG_RADIO_EVT_EDEND) |      \
                                                    (1 << GPIOTE_DBG_RADIO_EVT_PHYEND))

#else // ENABLE_DEBUG_GPIO

#define NRF_802154_DEBUG_PINS_USED_MASK            0
#define NRF_802154_DEBUG_PPI_CHANNELS_USED_MASK    0
#define NRF_802154_DEBUG_GPIOTE_CHANNELS_USED_MASK 0

#endif // ENABLE_DEBUG_GPIO

/**
 * @brief Initializes debug helpers for the nRF 802.15.4 driver.
 */
void nrf_802154_debug_init(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_DEBUG_H_ */
