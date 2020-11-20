/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRF_FEM_ABSTRACT_INTERFACE_H_
#define NRF_FEM_ABSTRACT_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>

#include "nrf_fem_protocol_api.h"

/**
 * @brief Set of pointers to functions that enables run-time choice of FEM implementation.
 */
typedef struct
{
    uint32_t fem_type_id;

    int32_t (* p_pa_configuration_set)(const nrf_802154_fal_event_t * const p_activate_event,
                                       const nrf_802154_fal_event_t * const p_deactivate_event);
    int32_t (* p_lna_configuration_set)(const nrf_802154_fal_event_t * const p_activate_event,
                                        const nrf_802154_fal_event_t * const p_deactivate_event);
    int32_t (* p_pa_configuration_clear)(void);
    int32_t (* p_lna_configuration_clear)(void);
    void (* p_deactivate_now)(nrf_fal_functionality_t type);
    void (* p_cleanup)(void);
    bool (* p_prepare_powerdown)(NRF_TIMER_Type  * p_instance,
                                 uint32_t          compare_channel,
                                 nrf_ppi_channel_t ppi_id);
    void (* p_pa_is_configured)(int8_t * const p_gain);
    int32_t (* p_abort_set)(uint32_t event, nrf_ppi_channel_group_t group);
    int32_t (* p_abort_extend)(nrf_ppi_channel_t channel_to_add, nrf_ppi_channel_group_t group);
    int32_t (* p_abort_reduce)(nrf_ppi_channel_t channel_to_remove, nrf_ppi_channel_group_t group);
    int32_t (* p_abort_clear)(void);
} nrf_802154_fal_interface_t;

/**
 * @brief Connects to one of FEM implementation
 *
 * @param[in] p_methods   Pointer to structure of pointers. This will be used in case of future calls
 *                        to specific FEM implementation methods.
 *
 * @note This must be used by FEM implementation "constructor" and this is the only case
 *       in which it can be used.
 *
 * @retval   ::NRFX_SUCCESS               The "link" was successfuly created.
 * @retval   ::NRFX_ERROR_FORBIDDEN       Some error occured (e.q. null pointer was passed)
 */
int32_t nrf_802154_fal_interface_set(const nrf_802154_fal_interface_t * p_methods);

#endif // NRF_FEM_ABSTRACT_INTERFACE_
