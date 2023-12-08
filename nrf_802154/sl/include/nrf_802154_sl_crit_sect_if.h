/*
 * Copyright (c) 2017, Nordic Semiconductor ASA
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

#ifndef NRF_802154_SL_CRIT_SECT_IF_H__
#define NRF_802154_SL_CRIT_SECT_IF_H__

#include <stdbool.h>

/**
 * @defgroup nrf_802154_sl_crit_sect_if Interface of the critical section.
 * @{
 */

/**
 * @brief Pointer to function used to enter critical section.
 */
typedef bool (* nrf_802154_sl_crit_sect_enter_t)(void);

/**
 * @brief Pointer to function used to exit critical section.
 */
typedef void (* nrf_802154_sl_crit_sect_exit_t)(void);

/**
 * @brief Structure that holds interface of Radio Driver critical sections.
 */
typedef struct
{
    nrf_802154_sl_crit_sect_enter_t enter;
    nrf_802154_sl_crit_sect_exit_t  exit;
} nrf_802154_sl_crit_sect_interface_t;

/**
 * @brief Initializes the critical section interface.
 *
 * After the call SL library can use @ref nrf_802154_sl_crit_sect_enter and
 * @ref nrf_802154_sl_crit_sect_exit to use critical section provided by the Radio Driver.
 *
 * @param[in] p_crit_sect_interface     Pointer to a critical section interface of the Radio Driver.
 *                                      It must point to static data for lifetime of Radio Driver.
 */
void nrf_802154_sl_crit_sect_init(
    const nrf_802154_sl_crit_sect_interface_t * p_crit_sect_interface);

/**
 * @brief Enters into the critical section.
 *
 * @retval true    Code entered into the critical section.
 * @retval false   Critical section was already taken, could not enter into critical section.
 */
static inline bool nrf_802154_sl_crit_sect_enter(void)
{
    extern const nrf_802154_sl_crit_sect_interface_t * gp_nrf_802154_sl_crit_sect_interface;

    return gp_nrf_802154_sl_crit_sect_interface->enter();
}

/**
 * @brief Exits out of the critical section.
 */
static inline void nrf_802154_sl_crit_sect_exit(void)
{
    extern const nrf_802154_sl_crit_sect_interface_t * gp_nrf_802154_sl_crit_sect_interface;

    return gp_nrf_802154_sl_crit_sect_interface->exit();
}

/**
 *@}
 **/

#endif // NRF_802154_SL_CRIT_SECT_IF_H__
