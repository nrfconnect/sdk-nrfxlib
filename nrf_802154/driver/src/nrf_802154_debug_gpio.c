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
 * @file
 *   This file implements debug helpers for the nRF 802.15.4 radio driver.
 *
 */

#include "nrf_802154_debug.h"

#include <stdint.h>

#include "nrfx.h"
#include "hal/nrf_gpio.h"
#include "hal/nrf_gpiote.h"
#include "hal/nrf_ppi.h"

/**
 * @brief Initialize PPI to toggle GPIO pins on radio events.
 */
static void radio_event_gpio_toggle_init(void)
{
    nrf_gpio_cfg_output(PIN_DBG_RADIO_EVT_END);
    nrf_gpio_cfg_output(PIN_DBG_RADIO_EVT_DISABLED);
    nrf_gpio_cfg_output(PIN_DBG_RADIO_EVT_READY);

    nrf_gpiote_task_configure(NRF_GPIOTE,
                              GPIOTE_DBG_RADIO_EVT_END,
                              PIN_DBG_RADIO_EVT_END,
                              NRF_GPIOTE_POLARITY_TOGGLE,
                              NRF_GPIOTE_INITIAL_VALUE_HIGH);
    nrf_gpiote_task_configure(NRF_GPIOTE,
                              GPIOTE_DBG_RADIO_EVT_DISABLED,
                              PIN_DBG_RADIO_EVT_DISABLED,
                              NRF_GPIOTE_POLARITY_TOGGLE,
                              NRF_GPIOTE_INITIAL_VALUE_HIGH);
    nrf_gpiote_task_configure(NRF_GPIOTE,
                              GPIOTE_DBG_RADIO_EVT_READY,
                              PIN_DBG_RADIO_EVT_READY,
                              NRF_GPIOTE_POLARITY_TOGGLE,
                              NRF_GPIOTE_INITIAL_VALUE_HIGH);

    nrf_gpiote_task_enable(NRF_GPIOTE, GPIOTE_DBG_RADIO_EVT_END);
    nrf_gpiote_task_enable(NRF_GPIOTE, GPIOTE_DBG_RADIO_EVT_DISABLED);
    nrf_gpiote_task_enable(NRF_GPIOTE, GPIOTE_DBG_RADIO_EVT_READY);

    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   (nrf_ppi_channel_t)PPI_DBG_RADIO_EVT_END,
                                   nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_END),
                                   nrf_gpiote_task_address_get(NRF_GPIOTE, NRF_GPIOTE_TASK_OUT_0));
    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   (nrf_ppi_channel_t)PPI_DBG_RADIO_EVT_DISABLED,
                                   nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_DISABLED),
                                   nrf_gpiote_task_address_get(NRF_GPIOTE, NRF_GPIOTE_TASK_OUT_1));
    nrf_ppi_channel_endpoint_setup(NRF_PPI,
                                   (nrf_ppi_channel_t)PPI_DBG_RADIO_EVT_READY,
                                   nrf_radio_event_address_get(NRF_RADIO, NRF_RADIO_EVENT_RXREADY),
                                   nrf_gpiote_task_address_get(NRF_GPIOTE, NRF_GPIOTE_TASK_OUT_2));

    nrf_ppi_channel_enable(NRF_PPI, (nrf_ppi_channel_t)PPI_DBG_RADIO_EVT_END);
    nrf_ppi_channel_enable(NRF_PPI, (nrf_ppi_channel_t)PPI_DBG_RADIO_EVT_DISABLED);
    nrf_ppi_channel_enable(NRF_PPI, (nrf_ppi_channel_t)PPI_DBG_RADIO_EVT_READY);
}

/**
 * @brief Initialize GPIO to set it simulated arbiter events.
 */
static void raal_simulator_gpio_init(void)
{
#if RAAL_SIMULATOR
    nrf_gpio_cfg_output(PIN_DBG_TIMESLOT_ACTIVE);
    nrf_gpio_cfg_output(PIN_DBG_RAAL_CRITICAL_SECTION);
#endif
}

void nrf_802154_debug_gpio_init(void)
{
    radio_event_gpio_toggle_init();
    raal_simulator_gpio_init();
}
