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

uint8_t nrf_802154_energy_level_from_dbm_calculate(int8_t ed_dbm)
{
    return nrf_802154_addons_energy_level_from_dbm_calculate(ed_dbm);
}

uint8_t nrf_802154_ccaedthres_from_dbm_calculate(int8_t dbm)
{
    return dbm - ED_RSSIOFFS;
}

uint64_t nrf_802154_first_symbol_timestamp_get(uint64_t end_timestamp, uint8_t psdu_length)
{
    uint32_t frame_symbols = PHY_SHR_SYMBOLS;

    frame_symbols += (PHR_SIZE + psdu_length) * PHY_SYMBOLS_PER_OCTET;

    return end_timestamp - (frame_symbols * PHY_US_PER_SYMBOL);
}

uint64_t nrf_802154_mhr_timestamp_get(uint64_t end_timestamp, uint8_t psdu_length)
{
    return end_timestamp - (psdu_length * PHY_SYMBOLS_PER_OCTET * PHY_US_PER_SYMBOL);
}

uint64_t nrf_802154_timestamp_end_to_phr_convert(uint64_t end_timestamp, uint8_t psdu_length)
{
    uint32_t frame_symbols = (PHR_SIZE + psdu_length) * PHY_SYMBOLS_PER_OCTET;

    return end_timestamp - (frame_symbols * PHY_US_PER_SYMBOL);
}

uint64_t nrf_802154_timestamp_phr_to_shr_convert(uint64_t phr_timestamp)
{
    return phr_timestamp - (PHY_SHR_SYMBOLS * PHY_US_PER_SYMBOL);
}

uint64_t nrf_802154_timestamp_phr_to_mhr_convert(uint64_t phr_timestamp)
{
    return phr_timestamp + (PHR_SIZE * PHY_SYMBOLS_PER_OCTET * PHY_US_PER_SYMBOL);
}
