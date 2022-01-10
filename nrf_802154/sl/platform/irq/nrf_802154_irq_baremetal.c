/*
 * Copyright (c) 2020 - 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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

    NVIC_SetPriority((IRQn_Type)irqn, prio);
    NVIC_ClearPendingIRQ((IRQn_Type)irqn);
}

void nrf_802154_irq_enable(uint32_t irqn)
{
    NVIC_EnableIRQ((IRQn_Type)irqn);
}

void nrf_802154_irq_disable(uint32_t irqn)
{
    NVIC_DisableIRQ((IRQn_Type)irqn);
    __DSB();
    __ISB();
}

void nrf_802154_irq_set_pending(uint32_t irqn)
{
    NVIC_SetPendingIRQ((IRQn_Type)irqn);
}

void nrf_802154_irq_clear_pending(uint32_t irqn)
{
    NVIC_ClearPendingIRQ((IRQn_Type)irqn);
}

bool nrf_802154_irq_is_enabled(uint32_t irqn)
{
    // The following calculation results in zero value if the interrupt is disabled
    // and non-zero value otherwise.
    return (bool)(NVIC->ISER[irqn >> 5UL] & (1UL << (irqn & 0x1FUL)));
}

uint32_t nrf_802154_irq_priority_get(uint32_t irqn)
{
    // coverity[mixed_enum_type]
    return NVIC_GetPriority((IRQn_Type)irqn);
}
