/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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
 *   This file contains system-agnostic implementation of the nRF 802.15.4 GPIOTE
 *   critical section abstraction.
 */

#include "platform/nrf_802154_gpiote.h"

#include "hal/nrf_gpio.h"
#include "nrf_802154_sl_utils.h"
#include "platform/nrf_802154_irq.h"

static volatile bool     m_gpiote_irq_enabled;      ///< Whether the GPIOTE interrupt was turned on before entering the first critical section.
static volatile uint32_t m_gpiote_irq_disabled_cnt; ///< Counter of how many times GPIOTE interrupt was disabled while entering critical section.

void nrf_802154_gpiote_critical_section_enter(void)
{
    nrf_802154_sl_mcu_critical_state_t mcu_cs;
    uint32_t                           cnt;

    nrf_802154_sl_mcu_critical_enter(mcu_cs);
    cnt = m_gpiote_irq_disabled_cnt;

    if (cnt == 0U)
    {
        m_gpiote_irq_enabled = nrf_802154_irq_is_enabled(GPIOTE_IRQn);
        nrf_802154_irq_disable(GPIOTE_IRQn);
    }

    cnt++;
    m_gpiote_irq_disabled_cnt = cnt;

    nrf_802154_sl_mcu_critical_exit(mcu_cs);
}

void nrf_802154_gpiote_critical_section_exit(void)
{
    nrf_802154_sl_mcu_critical_state_t mcu_cs;
    uint32_t                           cnt;

    nrf_802154_sl_mcu_critical_enter(mcu_cs);

    cnt = m_gpiote_irq_disabled_cnt;
    cnt--;

    if (cnt == 0U)
    {
        if (m_gpiote_irq_enabled)
        {
            nrf_802154_irq_enable(GPIOTE_IRQn);
        }
    }

    m_gpiote_irq_disabled_cnt = cnt;

    nrf_802154_sl_mcu_critical_exit(mcu_cs);
}
