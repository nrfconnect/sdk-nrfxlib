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
 * @brief 802.15.4 antenna diversity module.
 */

#include "nrf_802154_sl_ant_div.h"

#ifdef __cplusplus
extern "C" {
#endif

void nrf_802154_sl_ant_div_cfg_set(const nrf_802154_sl_ant_div_cfg_t * p_cfg)
{
    (void)p_cfg;
}

bool nrf_802154_sl_ant_div_cfg_get(nrf_802154_sl_ant_div_cfg_t * p_cfg)
{
    (void)p_cfg;

    return false;
}

bool nrf_802154_sl_ant_div_cfg_mode_set(nrf_802154_sl_ant_div_op_t   op,
                                        nrf_802154_sl_ant_div_mode_t mode)
{
    (void)op;
    (void)mode;

    return false;
}

nrf_802154_sl_ant_div_mode_t nrf_802154_sl_ant_div_cfg_mode_get(nrf_802154_sl_ant_div_op_t op)
{
    (void)op;

    return NRF_802154_SL_ANT_DIV_MODE_DISABLED;
}

bool nrf_802154_sl_ant_div_cfg_antenna_set(nrf_802154_sl_ant_div_op_t      op,
                                           nrf_802154_sl_ant_div_antenna_t antenna)
{
    (void)op;
    (void)antenna;

    return false;
}

nrf_802154_sl_ant_div_antenna_t nrf_802154_sl_ant_div_cfg_antenna_get(nrf_802154_sl_ant_div_op_t op)
{
    (void)op;

    return NRF_802154_SL_ANT_DIV_ANTENNA_NONE;
}

bool nrf_802154_sl_ant_div_init(void)
{
    return false;
}

bool nrf_802154_sl_ant_div_antenna_set(nrf_802154_sl_ant_div_antenna_t antenna)
{
    (void)antenna;

    return false;
}

nrf_802154_sl_ant_div_antenna_t nrf_802154_sl_ant_div_antenna_get(void)
{
    return NRF_802154_SL_ANT_DIV_ANTENNA_NONE;
}

nrf_802154_sl_ant_div_antenna_t nrf_802154_sl_ant_div_last_rx_best_antenna_get(void)
{
    return NRF_802154_SL_ANT_DIV_ANTENNA_NONE;
}

bool nrf_802154_sl_ant_div_rx_frame_started_notify(void)
{
    return false;
}

void nrf_802154_sl_ant_div_rx_frame_received_notify(void)
{
    // Intenionally empty
}

void nrf_802154_sl_ant_div_rx_aborted_notify(void)
{
    // Intentionally empty
}

void nrf_802154_sl_ant_div_rx_preamble_timeout_notify(void)
{
    // Intentionally empty
}

void nrf_802154_sl_ant_div_rx_preamble_detected_notify(void)
{
    // Intentionally empty
}

void nrf_802154_sl_ant_div_energy_detection_requested_notify(uint32_t * p_ed_time)
{
    (void)p_ed_time;
}

void nrf_802154_sl_ant_div_energy_detection_aborted_notify(void)
{
    // Intentionally empty
}

bool nrf_802154_sl_ant_div_energy_detection_finished_notify(void)
{
    return false;
}

void nrf_802154_sl_ant_div_rx_started_notify(void)
{
    // Intentionally empty
}

void nrf_802154_sl_ant_div_txack_notify(void)
{
    // Intentionally empty
}

#ifdef __cplusplus
}
#endif
