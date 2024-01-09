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

#ifndef NRF_802154_COMMON_UTILS_H__
#define NRF_802154_COMMON_UTILS_H__

#include <stdint.h>
#include "nrf_802154_config.h"

/**
 * @defgroup nrf_802154_data Functions to calculate data given by the driver
 * @{
 */

/**
 * @brief  Converts energy level received during the energy detection procedure to IEEE Std. 802.15.4-2015 compliant value.
 *
 * @param ed_dbm  Energy level in dBm
 *
 * @return uint8_t  Energy level in units compliant to IEEE Std. 802.15.4-2015 chapter 10.2.5.
 */
uint8_t nrf_802154_energy_level_from_dbm_calculate(int8_t ed_dbm);

/**
 * @brief  Converts a given dBm level to a CCA energy detection threshold value.
 *
 * @param[in]  dbm  Energy level in dBm used to calculate the CCAEDTHRES value.
 *
 * @return  Energy level value corresponding to the given dBm level that is to be written to
 *          the CCACTRL register.
 */
uint8_t nrf_802154_ccaedthres_from_dbm_calculate(int8_t dbm);

/**
 * @brief  Calculates the timestamp of the first symbol of the preamble in a received frame.
 *
 * @deprecated This function is deprecated. Use @ref nrf_802154_timestamp_end_to_phr_convert
 * instead and adjust the code that calls this function to rely on the timestamp of the first symbol
 * of the PHR, not the timestamp of the first symbol of the frame.
 *
 * @param[in]  end_timestamp  Timestamp of the end of the last symbol in the frame,
 *                            in microseconds.
 * @param[in]  psdu_length    Number of bytes in the frame PSDU.
 *
 * @return  Timestamp of the beginning of the first preamble symbol of a given frame,
 *          in microseconds.
 */
uint64_t nrf_802154_first_symbol_timestamp_get(uint64_t end_timestamp, uint8_t psdu_length);

/**
 * @brief  Calculates the timestamp of the MAC Header in a received frame.
 *
 * @deprecated This function is deprecated. Use @ref nrf_802154_timestamp_end_to_phr_convert
 * instead and adjust the code that calls this function to rely on the timestamp of the first symbol
 * of the PHR, not the MAC Header timestamp.
 *
 * @param[in]  end_timestamp  Timestamp of the end of the last symbol in the frame,
 *                            in microseconds.
 * @param[in]  psdu_length    Number of bytes in the frame PSDU.
 *
 * @return  Timestamp of the MHR of a given frame, in microseconds.
 */
uint64_t nrf_802154_mhr_timestamp_get(uint64_t end_timestamp, uint8_t psdu_length);

/**
 * @brief  Converts the timestamp of the frame's end to the timestamp of the start of its PHR.
 *
 * This function calculates the time when the first symbol of the PHR is at the local antenna. Note
 * that this time is equivalent to: the end of the frame's SFD and RMARKER as defined in'
 * IEEE 802.15.4-2020, Section 6.9.1.
 *
 * @param[in]  end_timestamp  Timestamp of the end of the last symbol in the frame,
 *                            in microseconds.
 * @param[in]  psdu_length    Number of bytes in the frame PSDU.
 *
 * @return  Timestamp of the start of the PHR of a given frame, in microseconds.
 */
uint64_t nrf_802154_timestamp_end_to_phr_convert(uint64_t end_timestamp, uint8_t psdu_length);

/**
 * @brief  Converts the timestamp of the frame's PHR to the timestamp of the start of its SHR.
 *
 * This function converts the time when the first symbol of the frame's PHR is at the local antenna
 * to the timestamp of the start of the frame's SHR.
 *
 * @param[in]  phr_timestamp  Timestamp of the frame's PHR.
 *
 * @return  Timestamp of the start of the SHR of a given frame, in microseconds.
 */
uint64_t nrf_802154_timestamp_phr_to_shr_convert(uint64_t phr_timestamp);

/**
 * @brief  Converts the timestamp of the frame's PHR to the timestamp of the start of its MHR.
 *
 * This function converts the time when the first symbol of the frame's PHR is at the local antenna
 * to the timestamp of the start of the frame's MHR.
 *
 * @param[in]  phr_timestamp  Timestamp of the frame's PHR.
 *
 * @return  Timestamp of the start of the MHR of a given frame, in microseconds.
 */
uint64_t nrf_802154_timestamp_phr_to_mhr_convert(uint64_t phr_timestamp);

/**
 * @}
 */

#endif // NRF_802154_COMMON_UTILS_H__
