/*
 * Copyright (c) 2020 - 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 *   This file contains implementation of the nRF 802.15.4 GPIOTE abstraction.
 *
 * This implementation is intended to be used in bare-metal applications. Developer of such application can:
 * - use this implementation unmodified when GPIOTE peripheral is to be used only by the nRF 802.15.4 Radio Driver
 * - use this implementation as a reference for developing own platform port. In this case additional code can be put in GPIOTE_IRQHandler
 *   allowing handling interrupts coming from sources not related to nRF 802.15.4 driver.
 *
 */

#include "platform/nrf_802154_gpiote.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "hal/nrf_gpiote.h"
#include "nrf_802154_sl_coex.h"
#include "platform/nrf_802154_irq.h"

#define GPIOTE_IRQ_PRIORITY 4

static uint32_t m_gpio_pin;     ///< Number of GPIO pin associated with the Grant line.
static uint32_t m_gpiote_ch_id; ///< Number of GPIOTE channel associated with the Grant line.

void nrf_802154_gpiote_init(void)
{
    switch (nrf_802154_wifi_coex_interface_type_id_get())
    {
        case NRF_802154_WIFI_COEX_IF_NONE:
            return;

        case NRF_802154_WIFI_COEX_IF_3WIRE:
        {
            nrf_802154_wifi_coex_3wire_if_config_t cfg;

            nrf_802154_wifi_coex_cfg_3wire_get(&cfg);

            m_gpio_pin     = cfg.grant_cfg.gpio_pin;
            m_gpiote_ch_id = cfg.grant_cfg.gpiote_ch_id;
            assert(m_gpio_pin != COEX_GPIO_PIN_INVALID);

            NRF_GPIOTE->CONFIG[m_gpiote_ch_id] =
                ((GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos)
                 | (m_gpio_pin << GPIOTE_CONFIG_PSEL_Pos)
                 | (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos));
            NRF_GPIOTE->INTENSET = (1 << m_gpiote_ch_id);

            nrf_802154_irq_init(GPIOTE_IRQn,
                                GPIOTE_IRQ_PRIORITY,
                                nrf_802154_wifi_coex_gpiote_irqhandler);
            nrf_802154_irq_enable(GPIOTE_IRQn);
            break;
        }

        default:
            assert(false);
    }
}

void nrf_802154_gpiote_deinit(void)
{
    nrf_802154_irq_clear_pending(GPIOTE_IRQn);
    nrf_802154_irq_disable(GPIOTE_IRQn);
}

void GPIOTE_IRQHandler(void)
{
    if (NRF_GPIOTE->EVENTS_IN[m_gpiote_ch_id])
    {
        NRF_GPIOTE->EVENTS_IN[m_gpiote_ch_id] = 0;
        nrf_802154_wifi_coex_gpiote_irqhandler();
    }
}
