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
 *   This file implements the nrf 802.15.4 Interrupt Abstraction Layer with direct NVIC calls.
 */

#include "platform/nrf_802154_irq.h"

#include "nrf.h"

void nrf_802154_irq_init(uint32_t irqn, uint32_t prio, nrf_802154_isr_t isr)
{
    /* This implementation assumes that:
     *   - a vector table entry related to IRQ line number irqn is provided by the caller
     *     of this function;
     *   - that vector table entry calls the provided handler @p isr, e.g.
     *
     * @code
     * void nrf_802154_radio_irq_handler(void * p_context)
     * {
     *    (void)p_context;
     *    // Handle RADIO IRQ
     * }
     *
     * void RADIO_IRQHandler(void)
     * {
     *     nrf_802154_radio_irq_handler(NULL);
     * }
     *
     * void radio_irq_init(uint32_t prio)
     * {
     *     nrf_802154_irq_init(RADIO_IRQn, prio, nrf_802154_radio_irq_handler);
     * }
     * @endcode
     */
    (void)isr;

    NVIC_SetPriority(irqn, prio);
    NVIC_ClearPendingIRQ(irqn);
}

void nrf_802154_irq_enable(uint32_t irqn)
{
    NVIC_EnableIRQ(irqn);
}

void nrf_802154_irq_disable(uint32_t irqn)
{
    NVIC_DisableIRQ(irqn);
    __DSB();
    __ISB();
}

void nrf_802154_irq_set_pending(uint32_t irqn)
{
    NVIC_SetPendingIRQ(irqn);
}

void nrf_802154_irq_clear_pending(uint32_t irqn)
{
    NVIC_ClearPendingIRQ(irqn);
}

bool nrf_802154_irq_is_enabled(uint32_t irqn)
{
    // The following calculation results in zero value if the interrupt is disabled
    // and non-zero value otherwise.
    return (bool)(NVIC->ISER[irqn >> 5UL] & (1UL << (irqn & 0x1FUL)));
}

uint32_t nrf_802154_irq_priority_get(uint32_t irqn)
{
    return NVIC_GetPriority(irqn);
}
