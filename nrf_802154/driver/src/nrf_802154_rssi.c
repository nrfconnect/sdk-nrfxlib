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

/**
 * @file
 *   This file implements RSSI calculations for 802.15.4 driver.
 *
 */
#include "nrf_802154_rssi.h"
#include "nrf_802154_nrfx_addons.h"
#include "nrf_802154_const.h"

#include "nrfx.h"
#include <stdint.h>

#include "platform/nrf_802154_temperature.h"

#if defined(NRF52_SERIES)

/* Implementation for nRF52 family. */
static int8_t nrf_802154_rssi_sample_temp_corr_value_get(uint8_t rssi_sample)
{
    (void)rssi_sample;

    int8_t temp = nrf_802154_temperature_get();
    int8_t result;

#if defined(NRF52840_XXAA) || defined(NRF52833_XXAA)
    /* Implementation based on Errata 153 for nRF52840 SoC and Errata 225 for nRF52833 SoCs.. */
    if (temp <= -30)
    {
        result = 3;
    }
    else if (temp <= -10)
    {
        result = 2;
    }
    else if (temp <= 10)
    {
        result = 1;
    }
    else if (temp <= 30)
    {
        result = 0;
    }
    else if (temp <= 50)
    {
        result = -1;
    }
    else if (temp <= 70)
    {
        result = -2;
    }
    else if (temp <= 85)
    {
        result = -3;
    }
    else
    {
        // nRF52840 cannot work for a temperature above 85 degrees Celsius, so this part won't affect its operation,
        // even if it isn't present in its errata.
        result = -4;
    }
#else
    /* Implementation for other SoCs from nRF52 family */
    result = 0;
#endif
    return result;
}

#elif defined(NRF53_SERIES)

/** Macro for calculating x raised to the power of 2. */
#define POW_2(x)        ((x) * (x))
/** Macro for calculating x raised to the power of 3. */
#define POW_3(x)        ((x) * POW_2(x))

/**
 * @brief Polynomial coefficients of the correction equation.
 *
 *  Coefficients were calculated as round(x * RSSI_COEFF_BASE) based on Errata 87.
 */
#define RSSI_COEFF_A0   30198989L /* Initial value: 7.2           */
#define RSSI_COEFF_A1   6543114L  /* Initial value: 1.56          */
#define RSSI_COEFF_A2   41524L    /* Initial value: 9.9e-3        */
#define RSSI_COEFF_A3   205L      /* Initial value: 4.9e-5        */
#define RSSI_COEFF_TEMP 209715L   /* Initial value: 0.05   */
/** @brief Value used to increase precision of calculations. */
#define RSSI_COEFF_BASE (1UL << 22U)

/**
 * @brief Normalizes intermediate RSSI value by dividing and rounding.
 *
 * @param[in]   rssi_value  Intermediate RSSI value to be normalized.
 *
 * @returns normalized rssi_value.
 */
static int8_t normalize_rssi(int32_t rssi_value)
{
    uint32_t abs_rssi_value;

    abs_rssi_value = (rssi_value < 0) ? (-rssi_value) : rssi_value;
    abs_rssi_value = (abs_rssi_value + (RSSI_COEFF_BASE / 2)) / RSSI_COEFF_BASE;

    return (rssi_value < 0) ? (-(int8_t)abs_rssi_value) : ((int8_t)abs_rssi_value);
}

/* Implementation based on Errata 87 for nRF53 family. */
static int8_t nrf_802154_rssi_sample_temp_corr_value_get(uint8_t rssi_sample)
{
    int32_t temp;
    int32_t rssi_sample_i32;
    int8_t  compensated_rssi;

    temp            = (int32_t)nrf_802154_temperature_get();
    rssi_sample_i32 = (int32_t)rssi_sample;

    compensated_rssi = normalize_rssi((RSSI_COEFF_A1 * rssi_sample_i32)
                                      + (RSSI_COEFF_A3 * POW_3(rssi_sample_i32))
                                      - (RSSI_COEFF_A2 * POW_2(rssi_sample_i32))
                                      - (RSSI_COEFF_TEMP * temp) - RSSI_COEFF_A0);

    return compensated_rssi - (int8_t)rssi_sample;
}

#else

static int8_t nrf_802154_rssi_sample_temp_corr_value_get(uint8_t rssi_sample)
{
    return 0;
}

#endif

uint8_t nrf_802154_rssi_sample_corrected_get(uint8_t rssi_sample)
{
    return rssi_sample + nrf_802154_rssi_sample_temp_corr_value_get(rssi_sample);
}

uint8_t nrf_802154_rssi_lqi_corrected_get(uint8_t lqi)
{
    return lqi - nrf_802154_rssi_sample_temp_corr_value_get(lqi);
}

int16_t nrf_802154_rssi_ed_corrected_get(int16_t ed)
{
    return ed - nrf_802154_rssi_sample_temp_corr_value_get(ed);
}

uint8_t nrf_802154_rssi_cca_ed_threshold_corrected_get(uint8_t cca_ed)
{
    return cca_ed - nrf_802154_rssi_sample_temp_corr_value_get(cca_ed);
}

uint8_t nrf_802154_rssi_ed_sample_convert(uint8_t ed_sample)
{
    int16_t result;

    result = nrf_802154_rssi_ed_corrected_get(ed_sample);
    result = ED_RESULT_MAX * (result - EDSAMPLE_MIN_REPORTED_VALUE) /
             (EDSAMPLE_MAX_REPORTED_VALUE - EDSAMPLE_MIN_REPORTED_VALUE);

    if (result < 0)
    {
        result = 0;
    }

    if (result > ED_RESULT_MAX)
    {
        result = ED_RESULT_MAX;
    }

    return (uint8_t)result;
}

int8_t nrf_802154_rssi_dbm_from_energy_level_calculate(uint8_t energy_level)
{
    return ((int16_t)(EDSAMPLE_MAX_REPORTED_VALUE - EDSAMPLE_MIN_REPORTED_VALUE) *
            ((int16_t)energy_level)) /
           ED_RESULT_MAX + EDSAMPLE_MIN_REPORTED_VALUE + ED_RSSIOFFS;
}

int8_t nrf_802154_rssi_ed_sample_to_dbm_convert(uint8_t ed_sample)
{
    int16_t result;

    result = nrf_802154_rssi_ed_corrected_get(ed_sample);
    result = ED_RSSIOFFS + result;

    return result;
}

uint8_t nrf_802154_rssi_dbm_to_hw(int8_t dbm)
{
    return dbm - ED_RSSIOFFS;
}

int8_t nrf_802154_rssi_hw_to_dbm(uint8_t hwval)
{
    return hwval + ED_RSSIOFFS;
}
