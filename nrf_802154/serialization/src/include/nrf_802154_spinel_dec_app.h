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
 * @defgroup nrf_802154_spinel_serialization_decoder_app
 * 802.15.4 radio driver spinel serialization decoder for APP core
 * @{
 *
 */

#ifndef NRF_802154_SPINEL_DEC_APP_H_
#define NRF_802154_SPINEL_DEC_APP_H_

#include <stddef.h>

#include "../spinel_base/spinel.h"
#include "nrf_802154_serialization_error.h"

#include "nrf_802154.h"
#include "nrf_802154_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Decode SPINEL_PROP_LAST_STATUS.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] status             Decoded spinel status.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_last_status(const void      * p_property_data,
                                                               size_t            property_data_len,
                                                               spinel_status_t * status);

/**
 * @brief Decode SPINEL_DATATYPE_BOOL_S.
 *
 * @note This is used to decode `bool` responses for several kinds of requests in 802.15.4 radio driver.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_bool_response    Pointer to decoded response value.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_generic_bool(
    const void * p_property_data,
    size_t       property_data_len,
    bool       * p_bool_response);

/**
 * @brief Decode SPINEL_DATATYPE_UINT8_S.
 *
 * @note This is used to decode `uint8_t` responses for several kinds of requests in 802.15.4 radio driver.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_uint8_response   Pointer to decoded response value.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_generic_uint8(
    const void * p_property_data,
    size_t       property_data_len,
    uint8_t    * p_uint8_response);

/**
 * @brief Decode SPINEL_DATATYPE_UINT16_S.
 *
 * @note This is used to decode `uint16_t` responses for several kinds of requests in 802.15.4 radio driver.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_uint8_response   Pointer to decoded response value.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_generic_uint16(
    const void * p_property_data,
    size_t       property_data_len,
    uint16_t   * p_uint16_response);

/**
 * @brief Decode SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_power            Decoded Tx Power.
 *
 * @returns zero on success or negative error value on failure.
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_tx_power_get_ret(
    const void * p_property_data,
    size_t       property_data_len,
    int8_t     * p_power);

/**
 * @brief Decode SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_channel          Decoded channel.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_channel(const void * p_property_data,
                                                           size_t       property_data_len,
                                                           uint8_t    * p_channel);

/**
 * @brief Decode SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_capabilities     Decoded capabilities.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_capabilities_get_ret(
    const void                * p_property_data,
    size_t                      property_data_len,
    nrf_802154_capabilities_t * p_capabilities);

/**
 * @brief Decode SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_time             Decoded time.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_time_get_ret(
    const void * p_property_data,
    size_t       property_data_len,
    uint64_t   * p_time);

/**
 * @brief Decode SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_cfg              Decoded CCA configuration.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_cca_cfg_get_ret(
    const void           * p_property_data,
    size_t                 property_data_len,
    nrf_802154_cca_cfg_t * p_cfg);

/**
 * @brief Decode SPINEL_PROP_VENDOR_NORDIC_NRF_802154_STAT_TIMESTAMPS_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 * @param[out] p_stat_timestamps  Decoded stat timestamps
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_stat_timestamps_get_ret(
    const void                   * p_property_data,
    size_t                         property_data_len,
    nrf_802154_stat_timestamps_t * p_stat_timestamps);

/**
 * @brief Decode and dispatch SPINEL_CMD_PROP_VALUE_IS.
 *
 * @param[in]  p_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  data_len  Size of the @ref p_data buffer.
 *
 * @returns zero on success or negative error value on failure.
 *
 */
nrf_802154_ser_err_t nrf_802154_spinel_decode_cmd_prop_value_is(const void * cmd_data,
                                                                size_t       cmd_data_len);

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SPINEL_DEC_APP_H_ */

/** @} */
