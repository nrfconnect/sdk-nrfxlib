/*
 * Copyright (c) 2020 - 2022, Nordic Semiconductor ASA
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

#ifndef NRF_802154_NRFX_ADDONS_H__
#define NRF_802154_NRFX_ADDONS_H__

#include "nrf.h"
#include "nrf_802154_const.h"

/* The usage of ED_RSSISCALE is described inprecisely in the nRF product specifications. The meaning of
   this constant is the following: If we calculate ed_scaled =  EDSAMPLE *  ED_RSSISCALE, then
   it is guaranteed that in the range 0-255 ed_scaled maps linearly to the ED power in dBm. This means,
   that the maximum value in EDSAMPLE which can be reported in compliance with the 802.15.4 specification is
   255/ED_RSSISCALE. */

#if defined (NRF52840_XXAA)
#define ED_RSSIOFFS  (-92) ///< dBm value corresponding to value 0 in the EDSAMPLE register.
#define ED_RSSISCALE 4     ///< Factor needed to calculate the ED result based on the data from the RADIO peripheral.
#elif defined (NRF52833_XXAA) || defined(NRF5340_XXAA)
#define ED_RSSIOFFS  (-93) ///< dBm value corresponding to value 0 in the EDSAMPLE register.
#define ED_RSSISCALE 5     ///< Factor needed to calculate the ED result based on the data from the RADIO peripheral.
#else
#error "Selected chip is not supported."
#endif

#define EDSAMPLE_MIN_REPORTED_VALUE (PHY_MIN_RECEIVER_SENSITIVITY - ED_RSSIOFFS + 10) ///< Minimal reported EDSAMPLE value (reported as 0)
#define EDSAMPLE_MAX_REPORTED_VALUE (ED_RESULT_MAX / ED_RSSISCALE)                    ///< Maximal reported EDSAMPLE value (reported as 255)

/**
 * @brief  Converts the energy level received during the energy detection procedure to a dBm value.
 *
 * @param[in]  energy_level Energy level value compliant with the 802.15.4 specification (0-255)
 *
 * @return  Result of the energy detection procedure in dBm.
 */
static inline int8_t nrf_802154_addons_dbm_from_energy_level_calculate(uint8_t energy_level)
{
    return ((int16_t)(EDSAMPLE_MAX_REPORTED_VALUE - EDSAMPLE_MIN_REPORTED_VALUE) *
            ((int16_t)energy_level)) /
           ED_RESULT_MAX + EDSAMPLE_MIN_REPORTED_VALUE + ED_RSSIOFFS;
}

#endif // NRF_802154_NRFX_ADDONS_H__
