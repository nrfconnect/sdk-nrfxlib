/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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

#include <assert.h>
#include <stddef.h>

#ifndef TEST
#include <nrf.h>
#endif

#include "../spinel_base/spinel.h"
#include "nrf_802154_spinel.h"
#include "nrf_802154_spinel_datatypes.h"
#include "nrf_802154_spinel_dec.h"
#include "nrf_802154_spinel_response_notifier.h"
#include "nrf_802154_spinel_log.h"
#include "nrf_802154_serialization_error.h"
#include "nrf_802154_buffer_mgr_dst.h"
#include "nrf_802154_buffer_mgr_src.h"

#include "nrf_802154.h"

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_DONE.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_cca_done(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool channel_free = false;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_CCA_DONE,
                                                &channel_free);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_cca_done(channel_free);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_FAILED.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_cca_failed(
    const void * p_property_data,
    size_t       property_data_len)
{
    nrf_802154_cca_error_t err = NRF_802154_CCA_ERROR_ABORTED;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_CCA_FAILED,
                                                &err);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_cca_failed(err);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTED.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_energy_detected(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint8_t result;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTED,
                                                &result);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_energy_detected(result);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION_FAILED.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_energy_detection_failed(
    const void * p_property_data,
    size_t       property_data_len)
{
    nrf_802154_ed_error_t err = NRF_802154_CCA_ERROR_ABORTED;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION_FAILED,
                                                &err);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_energy_detection_failed(err);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVED_TIMESTAMP_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_received_timestamp_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t remote_frame_handle;
    void   * p_frame;
    size_t   frame_hdata_len;
    int8_t   power;
    uint8_t  lqi;
    uint32_t timestamp;
    void   * p_local_ptr;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_RECEIVED_TIMESTAMP_RAW,
                                                NRF_802154_HDATA_DECODE(remote_frame_handle,
                                                                        p_frame,
                                                                        frame_hdata_len),
                                                &power,
                                                &lqi,
                                                &timestamp);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    // Associate the remote frame handle with a local pointer
    // and copy the buffer content there
    bool frame_added = nrf_802154_buffer_mgr_dst_add(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        remote_frame_handle,
        p_frame,
        NRF_802154_DATA_LEN_FROM_HDATA_LEN(frame_hdata_len),
        &p_local_ptr);

    if (!frame_added)
    {
        return NRF_802154_SERIALIZATION_ERROR_NO_MEMORY;
    }

    nrf_802154_received_timestamp_raw(p_local_ptr, power, lqi, timestamp);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_FAILED
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_receive_failed(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint8_t error;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_RECEIVE_FAILED,
                                                &error);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_receive_failed(error);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_ACK_STARTED
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_tx_ack_started(
    const void * p_property_data,
    size_t       property_data_len)
{
    const uint8_t * p_tx_ack;
    size_t          tx_ack_len;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_TX_ACK_STARTED,
                                                &p_tx_ack,
                                                &tx_ack_len);

    if ((siz < 0) || (p_tx_ack == NULL) || (p_tx_ack[0] != tx_ack_len))
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_tx_ack_started(p_tx_ack);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMITTED_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmitted_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t frame_handle;
    uint32_t remote_ack_handle;
    void   * p_ack;
    size_t   ack_hdata_len;
    int8_t   power;
    uint8_t  lqi;
    void   * p_frame;
    void   * p_ack_local_ptr = NULL;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_TRANSMITTED_RAW,
                                                &frame_handle,
                                                NRF_802154_HDATA_DECODE(remote_ack_handle,
                                                                        p_ack,
                                                                        ack_hdata_len),
                                                &power,
                                                &lqi);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    // Search for the original frame buffer based on the provided handle
    bool frame_found = nrf_802154_buffer_mgr_src_search_by_buffer_handle(
        nrf_802154_spinel_src_buffer_mgr_get(),
        frame_handle,
        &p_frame);

    if (!frame_found)
    {
        // The frame is expected to be found, because it's the application that requested
        // to transmit it in the first place. Return error if it has not been found
        return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
    }

    // Handle the Ack
    if (NRF_802154_DATA_LEN_FROM_HDATA_LEN(ack_hdata_len) == 0)
    {
        // Length equal to 0 indicates that p_ack was originally a NULL pointer
        // Do not handle the Ack
    }
    else
    {
        // Associate the remote Ack handle with a local pointer and copy the buffer content there
        bool ack_added = nrf_802154_buffer_mgr_dst_add(
            nrf_802154_spinel_dst_buffer_mgr_get(),
            remote_ack_handle,
            p_ack,
            NRF_802154_DATA_LEN_FROM_HDATA_LEN(ack_hdata_len),
            &p_ack_local_ptr);

        if (!ack_added)
        {
            return NRF_802154_SERIALIZATION_ERROR_NO_MEMORY;
        }
    }

    frame_found = nrf_802154_buffer_mgr_src_remove_by_buffer_handle(
        nrf_802154_spinel_src_buffer_mgr_get(),
        frame_handle);

    if (!frame_found)
    {
        return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
    }

    nrf_802154_transmitted_raw(p_frame, p_ack_local_ptr, power, lqi);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_failed(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t              frame_handle;
    nrf_802154_tx_error_t tx_error;
    void                * p_frame;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED,
                                                &frame_handle,
                                                &tx_error);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    // Search for the original frame buffer based on the provided handle
    bool frame_found = nrf_802154_buffer_mgr_src_search_by_buffer_handle(
        nrf_802154_spinel_src_buffer_mgr_get(),
        frame_handle,
        &p_frame);

    if (!frame_found)
    {
        // The frame is expected to be found, because it's the application that requested
        // to transmit it in the first place. Return error if it has not been found
        return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
    }

    frame_found = nrf_802154_buffer_mgr_src_remove_by_buffer_handle(
        nrf_802154_spinel_src_buffer_mgr_get(),
        frame_handle);

    if (!frame_found)
    {
        return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
    }

    nrf_802154_transmit_failed(p_frame, tx_error);

    return NRF_802154_SERIALIZATION_ERROR_OK;
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_last_status(
    const void      * p_property_data,
    size_t            property_data_len,
    spinel_status_t * status)
{
    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_SPINEL_PROP_LAST_STATUS,
                                                status);

    return ((siz) < 0 ? NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE :
            NRF_802154_SERIALIZATION_ERROR_OK);
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_generic_bool(
    const void * p_property_data,
    size_t       property_data_len,
    bool       * p_bool_response)
{
    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_BOOL_S,
                                                p_bool_response);

    return ((siz) < 0 ? NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE :
            NRF_802154_SERIALIZATION_ERROR_OK);
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_tx_power_get_ret(
    const void * p_property_data,
    size_t       property_data_len,
    int8_t     * p_power)
{
    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_TX_POWER_GET_RET,
                                                p_power);

    return ((siz) < 0 ? NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE :
            NRF_802154_SERIALIZATION_ERROR_OK);
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_channel(const void * p_property_data,
                                                           size_t       property_data_len,
                                                           uint8_t    * p_channel)
{
    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_CHANNEL_GET_RET,
                                                p_channel);

    return ((siz) < 0 ? NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE :
            NRF_802154_SERIALIZATION_ERROR_OK);
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_prop_nrf_802154_capabilities_get_ret(
    const void                * p_property_data,
    size_t                      property_data_len,
    nrf_802154_capabilities_t * p_capabilities)
{
    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET_RET,
                                                p_capabilities);

    return ((siz) < 0 ? NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE :
            NRF_802154_SERIALIZATION_ERROR_OK);
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_cmd_prop_value_is(
    const void * p_cmd_data,
    size_t       cmd_data_len)
{
    spinel_prop_key_t property;
    const void      * p_property_data;
    size_t            property_data_len;
    spinel_ssize_t    siz;

    siz = spinel_datatype_unpack(p_cmd_data,
                                 cmd_data_len,
                                 SPINEL_DATATYPE_UINT_PACKED_S SPINEL_DATATYPE_DATA_S,
                                 &property,
                                 &p_property_data,
                                 &property_data_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    switch (property)
    {
        case SPINEL_PROP_LAST_STATUS:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR:
        // fall through
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW:
            nrf_802154_spinel_response_notifier_property_notify(property,
                                                                p_property_data,
                                                                property_data_len);
            return NRF_802154_SERIALIZATION_ERROR_OK;

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_DONE:
            return spinel_decode_prop_nrf_802154_cca_done(p_property_data,
                                                          property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_FAILED:
            return spinel_decode_prop_nrf_802154_cca_failed(p_property_data,
                                                            property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTED:
            return spinel_decode_prop_nrf_802154_energy_detected(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION_FAILED:
            return spinel_decode_prop_nrf_802154_energy_detection_failed(p_property_data,
                                                                         property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVED_TIMESTAMP_RAW:
            return spinel_decode_prop_nrf_802154_received_timestamp_raw(p_property_data,
                                                                        property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMITTED_RAW:
            return spinel_decode_prop_nrf_802154_transmitted_raw(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_FAILED:
            return spinel_decode_prop_nrf_802154_transmit_failed(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_FAILED:
            return spinel_decode_prop_nrf_802154_receive_failed(p_property_data,
                                                                property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_ACK_STARTED:
            return spinel_decode_prop_nrf_802154_tx_ack_started(p_property_data,
                                                                property_data_len);

        default:
            NRF_802154_SPINEL_LOG_RAW("Unsupported property: %s(%u)\n",
                                      spinel_prop_key_to_cstr(property),
                                      property);
            return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }
}

nrf_802154_ser_err_t nrf_802154_spinel_dispatch_cmd(spinel_command_t cmd,
                                                    const void     * p_cmd_data,
                                                    size_t           cmd_data_len)
{
    switch (cmd)
    {
        case SPINEL_CMD_PROP_VALUE_IS:
            return nrf_802154_spinel_decode_cmd_prop_value_is(p_cmd_data, cmd_data_len);

        default:
            NRF_802154_SPINEL_LOG_RAW("Unsupported command: %s(%u)\n",
                                      spinel_command_to_cstr(cmd),
                                      cmd);
            return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }
}

#ifndef TEST
__WEAK void nrf_802154_received_timestamp_raw(uint8_t * p_data,
                                              int8_t    rssi,
                                              uint8_t   lqi,
                                              uint32_t  timestamp)
{
    (void)p_data;
    (void)rssi;
    (void)lqi;
    (void)timestamp;
    // Intentionally empty
}

__WEAK void nrf_802154_receive_failed(nrf_802154_rx_error_t error)
{
    (void)error;
    // Intentionally empty
}

__WEAK void nrf_802154_transmitted_raw(const uint8_t * p_data,
                                       uint8_t       * p_ack,
                                       int8_t          rssi,
                                       uint8_t         lqi)
{
    (void)p_data;
    (void)p_ack;
    (void)rssi;
    (void)lqi;
    // Intentionally empty
}

__WEAK void nrf_802154_transmit_failed(const uint8_t       * p_data,
                                       nrf_802154_tx_error_t error)
{
    (void)error;
    // Intentionally empty
}

__WEAK void nrf_802154_tx_ack_started(const uint8_t * p_data)
{
    (void)p_data;
    // Intentionally empty
}

__WEAK void nrf_802154_cca_done(bool channel_free)
{
    (void)channel_free;
    // Intentionally empty
}

__WEAK void nrf_802154_cca_failed(nrf_802154_cca_error_t error)
{
    (void)error;
    // Intentionally empty
}

__WEAK void nrf_802154_energy_detected(uint8_t ed_level)
{
    (void)ed_level;
    // Intentionally empty
}

__WEAK void nrf_802154_energy_detection_failed(nrf_802154_ed_error_t error)
{
    (void)error;
    // Intentionally empty
}

#endif // TEST
