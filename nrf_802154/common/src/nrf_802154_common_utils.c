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

#include "nrf_802154_common_utils.h"
#include "nrf_802154_const.h"
#include "nrf_802154_nrfx_addons.h"

#if NRF_802154_GFSK_2MBPS_PHY_ENABLED
#define PHY_ARG phy
#else /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
#define PHY_ARG NRF_802154_PHY_OQPSK_250KBPS
#endif /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */

uint8_t nrf_802154_energy_level_from_dbm_calculate(int8_t ed_dbm)
{
    return nrf_802154_addons_energy_level_from_dbm_calculate(ed_dbm);
}

uint8_t nrf_802154_ccaedthres_from_dbm_calculate(int8_t dbm)
{
    return dbm - ED_RSSIOFFS;
}

static uint16_t phy_shr_us(nrf_802154_phy_t phy)
{
    if (phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return PHY_GFSK_SHR_SYMBOLS * PHY_GFSK_US_PER_OCTET / PHY_GFSK_SYMBOLS_PER_OCTET;
    }
    else
    {
        return PHY_OQPSK_SHR_SYMBOLS * PHY_OQPSK_US_PER_SYMBOL;
    }
}

static uint16_t phy_octets_to_us(uint16_t octets, nrf_802154_phy_t phy)
{
    if (phy == NRF_802154_PHY_EXP1_GFSK_2MBPS)
    {
        return octets * PHY_GFSK_US_PER_OCTET;
    }
    else
    {
        return octets * PHY_OQPSK_SYMBOLS_PER_OCTET * PHY_OQPSK_US_PER_SYMBOL;
    }
}

#if NRF_802154_GFSK_2MBPS_PHY_ENABLED
uint64_t nrf_802154_timestamp_end_to_phr_convert(uint64_t end_timestamp, uint8_t psdu_length, nrf_802154_phy_t phy)
#else /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
uint64_t nrf_802154_timestamp_end_to_phr_convert(uint64_t end_timestamp, uint8_t psdu_length)
#endif /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
{
    return end_timestamp - (uint64_t)phy_octets_to_us((uint16_t)PHR_SIZE + psdu_length, PHY_ARG);
}

#if NRF_802154_GFSK_2MBPS_PHY_ENABLED
uint64_t nrf_802154_timestamp_phr_to_shr_convert(uint64_t phr_timestamp, nrf_802154_phy_t phy)
#else /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
uint64_t nrf_802154_timestamp_phr_to_shr_convert(uint64_t phr_timestamp)
#endif /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
{
    return phr_timestamp - (uint64_t)phy_shr_us(PHY_ARG);
}

#if NRF_802154_GFSK_2MBPS_PHY_ENABLED
uint64_t nrf_802154_timestamp_phr_to_mhr_convert(uint64_t phr_timestamp, nrf_802154_phy_t phy)
#else /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
uint64_t nrf_802154_timestamp_phr_to_mhr_convert(uint64_t phr_timestamp)
#endif /* NRF_802154_GFSK_2MBPS_PHY_ENABLED */
{
    return phr_timestamp + (uint64_t)phy_octets_to_us(PHR_SIZE, PHY_ARG);
}
