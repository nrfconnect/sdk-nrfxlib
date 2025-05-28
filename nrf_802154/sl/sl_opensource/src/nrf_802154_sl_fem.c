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

#include <nrfx.h>
#include "nrf_errno.h"
#include <stdbool.h>
#include <stdint.h>

#include "nrf_802154_fal.h"
#include "nrf_802154_types.h"

#include "mpsl_tx_power.h"
#include "protocol/mpsl_fem_protocol_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Macro to get the number of elements in an array.
 *
 * @param[in] X   Array.
 */
#define NUMELTS(X) (sizeof((X)) / sizeof(X[0]))

typedef struct
{
    nrf_radio_txpower_t reg;
    int8_t              dbm;
} radio_tx_power_t;

/**
 * Converts TX power integer values to RADIO TX power allowed values.
 *
 * @param[in]  integer_tx_power  TX power integer value.
 *
 * @retval     RADIO TX power allowed value.
 */
static radio_tx_power_t to_radio_tx_power_convert(int8_t integer_tx_power)
{
    static const radio_tx_power_t allowed_values[] =
    {
#if defined(RADIO_TXPOWER_TXPOWER_Neg40dBm)
        { NRF_RADIO_TXPOWER_NEG40DBM, -40 }, /**< -40 dBm. */
#endif
        { NRF_RADIO_TXPOWER_NEG20DBM, -20 }, /**< -20 dBm. */
        { NRF_RADIO_TXPOWER_NEG16DBM, -16 }, /**< -16 dBm. */
        { NRF_RADIO_TXPOWER_NEG12DBM, -12 }, /**< -12 dBm. */
        { NRF_RADIO_TXPOWER_NEG8DBM, -8 },   /**< -8 dBm. */
        { NRF_RADIO_TXPOWER_NEG4DBM, -4 },   /**< -4 dBm. */
        { NRF_RADIO_TXPOWER_0DBM, 0 },       /**< 0 dBm. */
#if defined(NRF53_SERIES)
        { NRF_RADIO_TXPOWER_NEG2DBM, 1 },    /**< 1 dBm. */
        { NRF_RADIO_TXPOWER_NEG1DBM, 2 },    /**< 2 dBm. */
        { NRF_RADIO_TXPOWER_0DBM, 3 },       /**< 3 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos2dBm)
        { NRF_RADIO_TXPOWER_POS2DBM, 2 },    /**< 2 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos3dBm)
        { NRF_RADIO_TXPOWER_POS3DBM, 3 },    /**< 3 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos4dBm)
        { NRF_RADIO_TXPOWER_POS4DBM, 4 },    /**< 4 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos5dBm)
        { NRF_RADIO_TXPOWER_POS5DBM, 5 },    /**< 5 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos6dBm)
        { NRF_RADIO_TXPOWER_POS6DBM, 6 },    /**< 6 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos7dBm)
        { NRF_RADIO_TXPOWER_POS7DBM, 7 },    /**< 7 dBm. */
#endif
#if defined(RADIO_TXPOWER_TXPOWER_Pos8dBm)
        { NRF_RADIO_TXPOWER_POS8DBM, 8 },    /**< 8 dBm. */
#endif
    };

    for (uint32_t i = NUMELTS(allowed_values) - 1; i > 0; i--)
    {
        if (integer_tx_power >= allowed_values[i].dbm)
        {
            return allowed_values[i];
        }
    }

    return allowed_values[0];
}

void mpsl_fem_caps_get(mpsl_fem_caps_t * p_caps)
{
    *p_caps = (mpsl_fem_caps_t){};
}

void mpsl_fem_enable(void)
{
    // Intentionally empty
}

int32_t mpsl_fem_disable(void)
{
    return 0;
}

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

int32_t mpsl_fem_abort_set(mpsl_subscribable_hw_event_t event, uint32_t group)
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

int8_t mpsl_fem_tx_power_split(const mpsl_tx_power_t         power,
                               mpsl_tx_power_split_t * const p_tx_power_split,
                               uint16_t                      freq_mhz,
                               bool                          tx_power_ceiling)
{
    (void)freq_mhz;
    (void)tx_power_ceiling;

    p_tx_power_split->radio_tx_power       = to_radio_tx_power_convert(power).dbm;
    p_tx_power_split->fem_pa_power_control = 0;

    return p_tx_power_split->radio_tx_power;
}

int32_t mpsl_fem_pa_power_control_set(mpsl_fem_pa_power_control_t pa_power_control)
{
    (void)pa_power_control;

    return 0;
}

bool mpsl_fem_device_config_254_apply_get(void)
{
    return false;
}

int8_t nrf_802154_fal_tx_power_split(const uint8_t                           channel,
                                     const int8_t                            power,
                                     nrf_802154_fal_tx_power_split_t * const p_tx_power_split)
{
    (void)channel;

    p_tx_power_split->radio_tx_power       = to_radio_tx_power_convert(power).dbm;
    p_tx_power_split->fem_pa_power_control = 0;

    return p_tx_power_split->radio_tx_power;
}

uint32_t mpsl_tx_power_dbm_to_radio_register_convert(mpsl_tx_power_t req_radio_power)
{
    return to_radio_tx_power_convert(req_radio_power).reg;
}

void mpsl_fem_lna_is_configured(int8_t * const p_gain)
{
    *p_gain = 0;
}

#ifdef __cplusplus
}
#endif
