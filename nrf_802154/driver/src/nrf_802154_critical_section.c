/*
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
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
 *   This file implements critical sections used with requests by 802.15.4 driver.
 *
 */

#define NRF_802154_MODULE_ID NRF_802154_DRV_MODULE_ID_CRITICAL_SECTION

#include "nrf_802154_critical_section.h"

#include <assert.h>
#include <stdint.h>

#include "nrf_802154_config.h"
#include "nrf_802154_debug.h"
#include "nrf_802154_utils.h"
#include "rsch/nrf_802154_rsch.h"
#include "platform/nrf_802154_lp_timer.h"
#include "platform/nrf_802154_irq.h"

#include <nrf.h>

#define CMSIS_IRQ_NUM_VECTACTIVE_DIFF                 16

#define NESTED_CRITICAL_SECTION_ALLOWED_PRIORITY_NONE (-1)

static volatile uint8_t m_nested_critical_section_counter;          ///< Counter of nested critical sections
static volatile int8_t  m_nested_critical_section_allowed_priority; ///< Indicator if nested critical sections are currently allowed

/***************************************************************************************************
 * @section Critical sections management
 **************************************************************************************************/

/** @brief Enter critical section for RADIO peripheral
 *
 * @note RADIO peripheral registers (and NVIC) are modified only when timeslot is granted for the
 *       802.15.4 driver.
 */
static void radio_critical_section_enter(void)
{
    if (nrf_802154_rsch_prec_is_approved(RSCH_PREC_RAAL, RSCH_PRIO_MIN_APPROVED))
    {
        nrf_802154_irq_disable(RADIO_IRQn);
    }
}

/** @brief Exit critical section for RADIO peripheral
 *
 * @note RADIO peripheral registers (and NVIC) are modified only when timeslot is granted for the
 *       802.15.4 driver.
 */
static void radio_critical_section_exit(void)
{
    if (nrf_802154_rsch_prec_is_approved(RSCH_PREC_RAAL, RSCH_PRIO_MIN_APPROVED))
    {
        nrf_802154_irq_enable(RADIO_IRQn);
    }
}

/** @brief Convert active priority value to int8_t type.
 *
 * @param[in]  active_priority  Active priority in uint32_t format
 *
 * @return Active_priority value in int8_t format.
 */
static int8_t active_priority_convert(uint32_t active_priority)
{
    return active_priority == UINT32_MAX ? INT8_MAX : (int8_t)active_priority;
}

/**@brief Returns priority of the currently executing context */
static int8_t active_vector_priority_get(void)
{
    return active_priority_convert(nrf_802154_critical_section_active_vector_priority_get());
}

static bool critical_section_enter(bool forced)
{
    bool                            result = false;
    int8_t                          active_vector_priority;
    nrf_802154_mcu_critical_state_t mcu_cs;

    /* We assume that preempting interrupts won't change the priority
     * of the currently executing one.
     */
    active_vector_priority = active_vector_priority_get();

    nrf_802154_mcu_critical_enter(mcu_cs);

    if (forced ||
        (m_nested_critical_section_counter == 0) ||
        (m_nested_critical_section_allowed_priority == active_vector_priority))
    {
        uint8_t cnt = m_nested_critical_section_counter;

        ++cnt;
        m_nested_critical_section_counter = cnt;

        if (cnt == 1U)
        {
            nrf_802154_lp_timer_critical_section_enter();
            radio_critical_section_enter();
        }

        result = true;
    }

    nrf_802154_mcu_critical_exit(mcu_cs);

    return result;
}

static void critical_section_exit(void)
{
    bool succeed = false;

    do
    {
        uint8_t                         cnt;
        nrf_802154_mcu_critical_state_t mcu_cs;

        nrf_802154_mcu_critical_enter(mcu_cs);

        cnt = m_nested_critical_section_counter;

        assert(cnt > 0);

        --cnt;

        if (cnt == 0U)
        {
            if (nrf_802154_critical_section_rsch_event_is_pending())
            {
                nrf_802154_mcu_critical_exit(mcu_cs);

                nrf_802154_critical_section_rsch_process_pending();
                continue;
            }

            radio_critical_section_exit();
            nrf_802154_lp_timer_critical_section_exit();
        }

        m_nested_critical_section_counter = cnt;

        nrf_802154_mcu_critical_exit(mcu_cs);

        succeed = true;
    }
    while (!succeed);

}

/***************************************************************************************************
 * @section API functions
 **************************************************************************************************/

void nrf_802154_critical_section_init(void)
{
    m_nested_critical_section_counter          = 0;
    m_nested_critical_section_allowed_priority = NESTED_CRITICAL_SECTION_ALLOWED_PRIORITY_NONE;
}

bool nrf_802154_critical_section_enter(void)
{
    bool result;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    result = critical_section_enter(false);

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);

    return result;
}

void nrf_802154_critical_section_forcefully_enter(void)
{
    bool critical_section_entered;

    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    critical_section_entered = critical_section_enter(true);
    assert(critical_section_entered);
    (void)critical_section_entered;

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_critical_section_exit(void)
{
    nrf_802154_log_function_enter(NRF_802154_LOG_VERBOSITY_LOW);

    critical_section_exit();

    nrf_802154_log_function_exit(NRF_802154_LOG_VERBOSITY_LOW);
}

void nrf_802154_critical_section_nesting_allow(void)
{
    assert(m_nested_critical_section_allowed_priority ==
           NESTED_CRITICAL_SECTION_ALLOWED_PRIORITY_NONE);
    assert(m_nested_critical_section_counter >= 1);

    m_nested_critical_section_allowed_priority = active_vector_priority_get();
}

void nrf_802154_critical_section_nesting_deny(void)
{
    assert(m_nested_critical_section_allowed_priority >= 0);
    assert(m_nested_critical_section_counter >= 1);

    m_nested_critical_section_allowed_priority = NESTED_CRITICAL_SECTION_ALLOWED_PRIORITY_NONE;
}

bool nrf_802154_critical_section_is_nested(void)
{
    return m_nested_critical_section_counter > 1;
}

uint32_t nrf_802154_critical_section_active_vector_priority_get(void)
{
    uint32_t  active_vector_id = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos;
    IRQn_Type irq_number;
    uint32_t  active_priority;

    // Check if this function is called from main thread.
    if (active_vector_id == 0)
    {
        return UINT32_MAX;
    }

    assert(active_vector_id >= CMSIS_IRQ_NUM_VECTACTIVE_DIFF);

    irq_number      = (IRQn_Type)(active_vector_id - CMSIS_IRQ_NUM_VECTACTIVE_DIFF);
    active_priority = nrf_802154_irq_priority_get(irq_number);

    return active_priority;
}
