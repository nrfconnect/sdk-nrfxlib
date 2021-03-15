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

#include "nrf_802154_const.h"

#include "../spinel_base/spinel.h"
#include "nrf_802154_spinel_datatypes.h"
#include "nrf_802154_spinel_dec.h"
#include "nrf_802154_spinel_enc_net.h"
#include "nrf_802154_spinel_log.h"
#include "nrf_802154_serialization_error.h"
#include "nrf_802154_serialization_error_helper.h"
#include "nrf_802154_buffer_mgr_dst.h"
#include "nrf_802154_buffer_mgr_src.h"

#include "nrf_802154.h"

/**
 * @brief Deal with SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP request and send response.
 *
 * @param[in]  p_property_data    Pointer to a buffer - unused here (no additional data to decode).
 * @param[in]  property_data_len  Size of the @ref p_data buffer - unused here.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_sleep(const void * p_property_data,
                                                                size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    bool sleep_response;

    sleep_response = nrf_802154_sleep();

    return nrf_802154_spinel_send_cmd_prop_value_is(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP,
                                                    SPINEL_DATATYPE_NRF_802154_SLEEP_RET,
                                                    sleep_response);
}

/**
 * @brief Deal with SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE request and send response.
 *
 * @param[in]  p_property_data    Pointer to a buffer - unused here (no additional data to decode).
 * @param[in]  property_data_len  Size of the @ref p_data buffer - unused here.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_receive(const void * p_property_data,
                                                                  size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    bool receive_response;

    receive_response = nrf_802154_receive();

    return nrf_802154_spinel_send_cmd_prop_value_is(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE,
                                                    SPINEL_DATATYPE_NRF_802154_RECEIVE_RET,
                                                    receive_response);
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802514_channel_get(const void * p_property_data,
                                                                      size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET,
        SPINEL_DATATYPE_NRF_802154_CHANNEL_GET_RET,
        nrf_802154_channel_get());
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802514_channel_set(const void * p_property_data,
                                                                      size_t       property_data_len)
{
    uint8_t        channel;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CHANNEL_SET,
                                 &channel);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_channel_set(channel);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_PAN_ID_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_pan_id_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    const void   * p_pan_id;
    size_t         pan_id_len;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_PAN_ID_SET,
                                 &p_pan_id,
                                 &pan_id_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (pan_id_len != PAN_ID_SIZE)
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    nrf_802154_pan_id_set((uint8_t *)p_pan_id);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SHORT_ADDRESS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_short_address_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    const void   * p_short_address;
    size_t         short_address_len;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_SHORT_ADDRESS_SET,
                                 &p_short_address,
                                 &short_address_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (short_address_len != SHORT_ADDRESS_SIZE)
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    nrf_802154_short_address_set((uint8_t *)p_short_address);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_EXTENDED_ADDRESS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_extended_address_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    const void   * p_extended_address;
    size_t         extended_address_len;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_EXTENDED_ADDRESS_SET,
                                 &p_extended_address,
                                 &extended_address_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (extended_address_len != EXTENDED_ADDRESS_SIZE)
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    nrf_802154_extended_address_set((uint8_t *)p_extended_address);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_pan_coord_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool           enabled;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_PAN_COORD_SET,
                                 &enabled);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_pan_coord_set(enabled);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PROMISCUOUS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_promiscuous_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool           enabled;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_PROMISCUOUS_SET,
                                 &enabled);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_promiscuous_set(enabled);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_cca(const void * p_property_data,
                                                              size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    bool result = nrf_802154_cca();

    return nrf_802154_spinel_send_cmd_prop_value_is(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA,
                                                    SPINEL_DATATYPE_NRF_802154_CCA_RET,
                                                    result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_energy_detection(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t       time_us;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION,
                                 &time_us);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    bool result = nrf_802154_energy_detection(time_us);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION,
        SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_AUTO_PENDING_BIT_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_auto_pending_bit_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool           enabled;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_AUTO_PENDING_BIT_SET,
                                 &enabled);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_auto_pending_bit_set(enabled);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_pending_bit_for_addr_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    const uint8_t * p_addr;
    size_t          addr_len;
    bool            extended;
    bool            result = false;
    spinel_ssize_t  siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET,
                                 &p_addr,
                                 &addr_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (addr_len == 8)
    {
        extended = true;
    }
    else if (addr_len == 2)
    {
        extended = false;
    }
    else
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    result = nrf_802154_pending_bit_for_addr_set(p_addr, extended);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_pending_bit_for_addr_clear(
    const void * p_property_data,
    size_t       property_data_len)
{
    const uint8_t * p_addr;
    size_t          addr_len;
    bool            extended;
    bool            result = false;
    spinel_ssize_t  siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR,
                                 &p_addr,
                                 &addr_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (addr_len == 8)
    {
        extended = true;
    }
    else if (addr_len == 2)
    {
        extended = false;
    }
    else
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    result = nrf_802154_pending_bit_for_addr_clear(p_addr, extended);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_RESET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_pending_bit_for_addr_reset(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool           extended;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_RESET,
                                 &extended);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_pending_bit_for_addr_reset(extended);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_src_addr_matching_method_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    nrf_802154_src_addr_match_t match_method;
    spinel_ssize_t              siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET,
                                 &match_method);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    switch (match_method)
    {
        case NRF_802154_SRC_ADDR_MATCH_THREAD:
        case NRF_802154_SRC_ADDR_MATCH_ZIGBEE:
        case NRF_802154_SRC_ADDR_MATCH_ALWAYS_1:
            // Intentionally empty
            break;

        default:
            return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_INVALID_ARGUMENT);
    }

    nrf_802154_src_addr_matching_method_set(match_method);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_csma_ca_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t     remote_frame_handle;
    const void * p_frame;
    size_t       frame_hdata_len;
    void       * p_local_frame_ptr;

    spinel_ssize_t siz = spinel_datatype_unpack(
        p_property_data,
        property_data_len,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        NRF_802154_HDATA_DECODE(remote_frame_handle, p_frame, frame_hdata_len));

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    // Map the remote handle to locally accessible pointer and copy the buffer content there
    bool frame_added = nrf_802154_buffer_mgr_dst_add(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        remote_frame_handle,
        p_frame,
        NRF_802154_DATA_LEN_FROM_HDATA_LEN(frame_hdata_len),
        &p_local_frame_ptr);

    if (!frame_added)
    {
        return NRF_802154_SERIALIZATION_ERROR_NO_MEMORY;
    }

    // Transmit the content under the locally accessible pointer
    nrf_802154_transmit_csma_ca_raw(p_local_frame_ptr);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t     remote_frame_handle;
    const void * p_frame;
    size_t       frame_hdata_len;
    bool         cca;
    void       * p_local_frame_ptr;

    spinel_ssize_t siz = spinel_datatype_unpack(
        p_property_data,
        property_data_len,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW,
        NRF_802154_HDATA_DECODE(remote_frame_handle, p_frame, frame_hdata_len),
        &cca);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    // Map the remote handle to locally accessible pointer and copy the buffer content there
    bool frame_added = nrf_802154_buffer_mgr_dst_add(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        remote_frame_handle,
        p_frame,
        NRF_802154_DATA_LEN_FROM_HDATA_LEN(frame_hdata_len),
        &p_local_frame_ptr);

    if (!frame_added)
    {
        return NRF_802154_SERIALIZATION_ERROR_NO_MEMORY;
    }

    bool result = nrf_802154_transmit_raw(p_local_frame_ptr, cca);

    if (!result)
    {
        nrf_802154_buffer_mgr_dst_remove_by_local_pointer(nrf_802154_spinel_dst_buffer_mgr_get(),
                                                          p_local_frame_ptr);
    }

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_BUFFER_FREE_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_buffer_free_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t local_frame_handle;
    void   * p_local_ptr;

    spinel_ssize_t siz = spinel_datatype_unpack(p_property_data,
                                                property_data_len,
                                                SPINEL_DATATYPE_NRF_802154_BUFFER_FREE_RAW,
                                                &local_frame_handle);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    // Search for a locally accessible pointer to be freed based on the local handle
    bool ptr_found = nrf_802154_buffer_mgr_src_search_by_buffer_handle(
        nrf_802154_spinel_src_buffer_mgr_get(),
        local_frame_handle,
        &p_local_ptr);

    if (ptr_found)
    {
        // Free the buffer
        nrf_802154_buffer_free_raw(p_local_ptr);

        // Remove the mapping associated with the provided handle
        bool ptr_removed = nrf_802154_buffer_mgr_src_remove_by_buffer_handle(
            nrf_802154_spinel_src_buffer_mgr_get(),
            local_frame_handle);

        if (!ptr_removed)
        {
            return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
        }
    }

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_TX_POWER_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_tx_power_set(const void * p_property_data,
                                                                       size_t       property_data_len)
{
    spinel_ssize_t siz;
    int8_t         power;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_TX_POWER_SET,
                                 &power);
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_tx_power_set(power);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_TX_POWER_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_tx_power_get(const void * p_property_data,
                                                                       size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    int8_t power;

    power = nrf_802154_tx_power_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET,
        SPINEL_DATATYPE_NRF_802154_TX_POWER_GET_RET,
        power);
}

/**
 * @brief Decode and dispatch SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_capabilities_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_capabilities_t caps;

    caps = nrf_802154_capabilities_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET,
        SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET_RET,
        caps);
}

nrf_802154_ser_err_t nrf_802154_spinel_decode_cmd_prop_value_set(const void * p_cmd_data,
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
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP:
            return spinel_decode_prop_nrf_802154_sleep(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE:
            return spinel_decode_prop_nrf_802154_receive(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET:
            return spinel_decode_prop_nrf_802514_channel_get(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_SET:
            return spinel_decode_prop_nrf_802514_channel_set(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_AUTO_PENDING_BIT_SET:
            return spinel_decode_prop_nrf_802154_auto_pending_bit_set(p_property_data,
                                                                      property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET:
            return spinel_decode_prop_nrf_802154_pending_bit_for_addr_set(p_property_data,
                                                                          property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR:
            return spinel_decode_prop_nrf_802154_pending_bit_for_addr_clear(p_property_data,
                                                                            property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_RESET:
            return spinel_decode_prop_nrf_802154_pending_bit_for_addr_reset(p_property_data,
                                                                            property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET:
            return spinel_decode_prop_nrf_802154_src_addr_matching_method_set(p_property_data,
                                                                              property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_ID_SET:
            return spinel_decode_prop_nrf_802154_pan_id_set(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SHORT_ADDRESS_SET:
            return spinel_decode_prop_nrf_802154_short_address_set(p_property_data,
                                                                   property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_EXTENDED_ADDRESS_SET:
            return spinel_decode_prop_nrf_802154_extended_address_set(p_property_data,
                                                                      property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_SET:
            return spinel_decode_prop_nrf_802154_pan_coord_set(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PROMISCUOUS_SET:
            return spinel_decode_prop_nrf_802154_promiscuous_set(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA:
            return spinel_decode_prop_nrf_802154_cca(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION:
            return spinel_decode_prop_nrf_802154_energy_detection(p_property_data,
                                                                  property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_SET:
            return spinel_decode_prop_nrf_802154_tx_power_set(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET:
            return spinel_decode_prop_nrf_802154_tx_power_get(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW:
            return spinel_decode_prop_nrf_802154_transmit_csma_ca_raw(p_property_data,
                                                                      property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW:
            return spinel_decode_prop_nrf_802154_transmit_raw(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_BUFFER_FREE_RAW:
            return spinel_decode_prop_nrf_802154_buffer_free_raw(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET:
            return spinel_decode_prop_nrf_802154_capabilities_get(p_property_data,
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
        case SPINEL_CMD_PROP_VALUE_SET:
            return nrf_802154_spinel_decode_cmd_prop_value_set(p_cmd_data, cmd_data_len);

        default:
            NRF_802154_SPINEL_LOG_RAW("Unsupported command: %s(%u)\n",
                                      spinel_command_to_cstr(cmd),
                                      cmd);
            return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }
}
