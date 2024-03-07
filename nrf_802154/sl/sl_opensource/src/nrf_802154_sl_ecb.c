/*
 * Copyright (c) 2024, Nordic Semiconductor ASA
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

#include "nrf_802154_sl_ecb.h"

#include <nrfx.h>

#if defined(CONFIG_SOC_COMPATIBLE_NRF52X) || defined(CONFIG_SOC_COMPATIBLE_NRF53X)

#include "hal/nrf_ecb.h"

#define ECB_INST NRF_ECB

static inline void sleep_wfe(void)
{
#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
    void z_impl_k_busy_wait();
    z_impl_k_busy_wait(10);
#elif defined(CONFIG_SOC_COMPATIBLE_NRF52X)
    __WFE();
#else
    /* Do-nothing. This includes nRF5340 series due multiple sleep-related anomalies (160, 165, 168) */
#endif
}

static void wait_for_ecb_end(void)
{
    while (!nrf_ecb_event_check(ECB_INST, NRF_ECB_EVENT_ENDECB) &&
           !nrf_ecb_event_check(ECB_INST, NRF_ECB_EVENT_ERRORECB))
    {
#if !defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
        if ((SCB->SCR & SCB_SCR_SEVONPEND_Msk) == SCB_SCR_SEVONPEND_Msk)
#endif
        {
            NVIC_ClearPendingIRQ(ECB_IRQn);

            uint32_t irq_was_masked = __get_PRIMASK();

            __disable_irq();

            nrf_ecb_int_enable(ECB_INST, NRF_ECB_INT_ENDECB_MASK | NRF_ECB_INT_ERRORECB_MASK);
            if (!nrf_ecb_event_check(ECB_INST, NRF_ECB_EVENT_ENDECB) &&
                !nrf_ecb_event_check(ECB_INST, NRF_ECB_EVENT_ERRORECB))
            {
                sleep_wfe();
            }

            if (!irq_was_masked)
            {
                __enable_irq();
            }
        }
    }
}

void nrf_802154_sl_ecb_block_encrypt(nrf_802154_sl_ecb_data_t * p_ecb_data)
{
    nrf_ecb_int_disable(ECB_INST, NRF_ECB_INT_ENDECB_MASK | NRF_ECB_INT_ERRORECB_MASK);

    do
    {
        nrf_ecb_task_trigger(ECB_INST, NRF_ECB_TASK_STOPECB);
        nrf_ecb_event_clear(ECB_INST, NRF_ECB_EVENT_ENDECB);
        nrf_ecb_event_clear(ECB_INST, NRF_ECB_EVENT_ERRORECB);
        nrf_ecb_data_pointer_set(ECB_INST, p_ecb_data);

        nrf_ecb_task_trigger(ECB_INST, NRF_ECB_TASK_STARTECB);
        wait_for_ecb_end();
    }
    while (nrf_ecb_event_check(ECB_INST, NRF_ECB_EVENT_ERRORECB));

    nrf_ecb_int_disable(ECB_INST, NRF_ECB_INT_ENDECB_MASK | NRF_ECB_INT_ERRORECB_MASK);
    nrf_ecb_event_clear(ECB_INST, NRF_ECB_EVENT_ERRORECB);
    nrf_ecb_event_clear(ECB_INST, NRF_ECB_EVENT_ENDECB);
    NVIC_ClearPendingIRQ(ECB_IRQn);
}

#endif /* defined(CONFIG_SOC_COMPATIBLE_NRF52X) || defined(CONFIG_SOC_COMPATIBLE_NRF53X) */
