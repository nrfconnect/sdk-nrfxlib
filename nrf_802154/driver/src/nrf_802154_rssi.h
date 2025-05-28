/*
 * Copyright (c) 2018, Nordic Semiconductor ASA
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

#ifndef NRF_802154_RSSI_H__
#define NRF_802154_RSSI_H__

#include <stdint.h>

/**
 * @defgroup nrf_802154_rssi RSSI measurement function
 * @{
 * @ingroup nrf_802154
 * @brief RSSI calculations used internally in the 802.15.4 driver.
 */

/**
 * @brief Adjusts the given RSSISAMPLE value by a temperature correction factor.
 *
 * @param[in]  rssi_sample  Value read from the RSSISAMPLE register.
 *
 * @returns RSSISAMPLE corrected by a temperature factor.
 */
uint8_t nrf_802154_rssi_sample_corrected_get(uint8_t rssi_sample);

/**
 * @brief Adjusts the reported LQI value using a temperature correction factor.
 *
 * @param[in]  lqi   Value read from the LQI byte.
 *
 * @returns LQI byte value corrected by a temperature factor.
 */
uint8_t nrf_802154_rssi_lqi_corrected_get(uint8_t lqi);

/**
 * @brief Adjusts the EDSAMPLE value using a temperature correction factor.
 *
 * @param[in]  ed    Value read from the EDSAMPLE register.
 *
 * @returns EDSAMPLE value corrected by a temperature factor.
 */
int16_t nrf_802154_rssi_ed_corrected_get(int16_t ed);

/**
 * @brief Adjusts the CCA ED threshold value using a temperature correction factor.
 *
 * @param[in]  cca_ed  Value representing the CCA ED threshold to be corrected.
 *
 * @returns CCA ED threshold value corrected by a temperature factor.
 */
uint8_t nrf_802154_rssi_cca_ed_threshold_corrected_get(uint8_t cca_ed);

/**
 * @brief Convert the hardware reported energy detection value to a value compliant with the 802.15.4 specification.
 *
 * @param[in]  ed_sample  The hardware reported value
 *
 * @returns 802.15.4 compliant energy detection value.
 */
uint8_t nrf_802154_rssi_ed_sample_convert(uint8_t ed_sample);

/**
 * @brief  Converts the energy level received during the energy detection procedure to a dBm value.
 *
 * @param[in]  energy_level  Energy level passed by @ref nrf_802154_energy_detected.
 *
 * @return  Result of the energy detection procedure in dBm.
 */
int8_t nrf_802154_rssi_dbm_from_energy_level_calculate(uint8_t energy_level);

/**
 * @brief  Converts the EDSAMPLE value to a dBm value.
 *
 * @note Performs temperature correction internally.
 *
 * @param[in]  ed_sample  The hardware reported value.
 *
 * @return  Result of the energy detection procedure in dBm.
 */
int8_t nrf_802154_rssi_ed_sample_to_dbm_convert(uint8_t ed_sample);

/**
 * @brief  Converts the a dBm value to radio hardware value.
 *
 * @param[in]  dbm  The dBm value.
 *
 * @return  Value in hardware units.
 */
uint8_t nrf_802154_rssi_dbm_to_hw(int8_t dbm);

/**
 * @brief  Converts the a radio hardware value to dBm value.
 *
 * @param[in]  hwval  The radio hardware value.
 *
 * @return  Value in dBm.
 */
int8_t nrf_802154_rssi_hw_to_dbm(uint8_t hwval);

/**
 *@}
 **/

#endif // NRF_802154_RSSI_H__
