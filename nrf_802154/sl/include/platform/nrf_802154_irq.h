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
 * @brief This module defines the Interrupt Abstraction Layer for the 802.15.4 driver.
 *
 * Interrupt Abstraction Layer can be used by other modules to configure, enable and disable
 * interrupts controlled by the 802.15.4 driver.
 */

#ifndef NRF_802154_IRQ_H__
#define NRF_802154_IRQ_H__

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Function pointer used for IRQ handling.
 *
 * This type intentionally does not specify any parameters of the function.
 * In contrast to C++, where unspecified function's parameter would be equivalent
 * to void parameter, in C such syntax indicates that the function in question
 * accepts parameters of any type. Therefore, it's convenient for compatibility reasons,
 * as the following functions:
 *
 * @code
 * void foo(void);
 * void bar(void * p_parameter);
 * @endcode
 *
 * are both of type nrf_802154_isr_t and can both be passed to functions expecting a parameter
 * of this type.
 */
typedef void (* nrf_802154_isr_t)();

/**
 * @brief Initializes a provided interrupt line.
 *
 * @note This function does not enable the interrupt. In order to enable it,  additional call
 *       to @ref nrf_802154_irq_enable is necessary.
 *
 * @param[in] irqn  IRQ line number.
 * @param[in] prio  Priority of the IRQ.
 * @param[in] isr   Pointer to ISR.
 *
 * @note Interpretation of the value represented by @p prio is platform-dependent and defined by
 *       the implementation.
 */
void nrf_802154_irq_init(uint32_t irqn, int32_t prio, nrf_802154_isr_t isr);

/**
 * @brief Enables an interrupt.
 *
 * @param[in] irqn  IRQ line number.
 */
void nrf_802154_irq_enable(uint32_t irqn);

/**
 * @brief Disables an interrupt.
 *
 * @param[in] irqn  IRQ line number.
 */
void nrf_802154_irq_disable(uint32_t irqn);

/**
 * @brief Sets an interrupt pending.
 *
 * @param[in] irqn  IRQ line number.
 */
void nrf_802154_irq_set_pending(uint32_t irqn);

/**
 * @brief Clears a pending interrupt.
 *
 * @param[in] irqn  IRQ line number.
 */
void nrf_802154_irq_clear_pending(uint32_t irqn);

/**
 * @brief Checks if an interrupt is enabled.
 *
 * @param[in] irqn  IRQ line number.
 *
 * @retval true   IRQ is enabled.
 * @retval false  Otherwise.
 */
bool nrf_802154_irq_is_enabled(uint32_t irqn);

/**
 * @brief Gets priority of an interrupt.
 *
 * This function returns the actual value of the IRQ priority present in the IRQ configuration
 * register. It might not be equal to the IRQ priority specified in @ref nrf_802154_irq_init.
 * In order to reliably compare IRQ priorities, do not rely on the values of IRQ priorities
 * passed to @ref nrf_802154_irq_init, but rather always use this function instead.
 *
 * @param[in] irqn  IRQ line number.
 *
 * @return Priority of the provided interrupt.
 */
uint32_t nrf_802154_irq_priority_get(uint32_t irqn);

#endif // NRF_802154_IRQ_H__
