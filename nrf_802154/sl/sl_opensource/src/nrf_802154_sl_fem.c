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

#include <stdint.h>
#include <nrf.h>
#include "nrf_errno.h"

#include "mpsl_fem_protocol_api.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t mpsl_fem_pa_configuration_set(const mpsl_fem_event_t * const p_activate_event,
                                      const mpsl_fem_event_t * const p_deactivate_event)
{
    (void)p_activate_event;
    (void)p_deactivate_event;

    return -NRF_EPERM;
}

int32_t mpsl_fem_pa_configuration_clear(void)
{
    return -NRF_EPERM;
}

int32_t mpsl_fem_lna_configuration_set(const mpsl_fem_event_t * const p_activate_event,
                                       const mpsl_fem_event_t * const p_deactivate_event)
{
    (void)p_activate_event;
    (void)p_deactivate_event;

    return -NRF_EPERM;
}

int32_t mpsl_fem_lna_configuration_clear(void)
{
    return -NRF_EPERM;
}

void mpsl_fem_deactivate_now(mpsl_fem_functionality_t type)
{
    (void)type;
}

int32_t mpsl_fem_abort_set(uint32_t event, uint32_t group)
{
    return -NRF_EPERM;
}

int32_t mpsl_fem_abort_extend(uint32_t channel_to_add, uint32_t group)
{
    (void)channel_to_add;
    (void)group;
    return 0;
}

int32_t mpsl_fem_abort_reduce(uint32_t channel_to_remove, uint32_t group)
{
    (void)channel_to_remove;
    (void)group;
    return 0;
}

int32_t mpsl_fem_abort_clear(void)
{
    return -NRF_EPERM;
}

void mpsl_fem_cleanup(void)
{
    // Intentionally empty
}

void mpsl_fem_pa_is_configured(int8_t * const p_gain)
{
    (void)p_gain;
}

bool mpsl_fem_prepare_powerdown(NRF_TIMER_Type * p_instance,
                                uint32_t         compare_channel,
                                uint32_t         ppi_id,
                                uint32_t         event_addr)
{
    (void)p_instance;
    (void)compare_channel;
    (void)ppi_id;

    return false;
}

int8_t nrf_802154_fal_tx_power_get(const uint8_t channel, const int8_t power)
{
    (void)channel;
    return power;
}

#ifdef __cplusplus
}
#endif
