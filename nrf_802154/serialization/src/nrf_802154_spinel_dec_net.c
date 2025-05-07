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
#include "nrf_802154_config.h"

#if NRF_802154_DELAYED_TRX_ENABLED
static uint8_t * mp_transmit_at_frame; ///< Pointer to the frame that was requested to delay-transmit

#endif

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
 * @brief Deal with SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP_IF_IDLE request and send response.
 *
 * @param[in]  p_property_data    Pointer to a buffer - unused here (no additional data to decode).
 * @param[in]  property_data_len  Size of the @ref p_data buffer - unused here.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_sleep_if_idle(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_sleep_error_t sleep_response = nrf_802154_sleep_if_idle();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP_IF_IDLE,
        SPINEL_DATATYPE_NRF_802154_SLEEP_IF_IDLE_RET,
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

#if NRF_802154_DELAYED_TRX_ENABLED
/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_receive_at(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint64_t       rx_time;
    uint32_t       timeout;
    uint8_t        channel;
    uint32_t       id;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_RECEIVE_AT,
                                 &rx_time,
                                 &timeout,
                                 &channel,
                                 &id);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    bool result = nrf_802154_receive_at(rx_time, timeout, channel, id);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_CANCEL.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_receive_at_cancel(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t       id;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_CANCEL,
                                 &id);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    bool result = nrf_802154_receive_at_cancel(id);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_CANCEL,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_CANCEL_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_receive_at_scheduled_cancel(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t       id;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL,
                                 &id);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    bool result = nrf_802154_receive_at_scheduled_cancel(id);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL_RET,
        result);
}

#endif // NRF_802154_DELAYED_TRX_ENABLED

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_channel_get(const void * p_property_data,
                                                                      size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint8_t channel = nrf_802154_channel_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET,
        SPINEL_DATATYPE_NRF_802154_CHANNEL_GET_RET,
        channel);
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_channel_set(const void * p_property_data,
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_ID_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_alternate_short_address_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    const void   * p_short_address;
    size_t         short_address_len;
    spinel_ssize_t siz;
    bool           data_valid;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET,
                                 &data_valid,
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

    if (!data_valid)
    {
        p_short_address = NULL;
    }

    nrf_802154_alternate_short_address_set((uint8_t *)p_short_address);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_EXTENDED_ADDRESS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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

#if NRF_802154_PAN_COORD_GET_ENABLED

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_pan_coord_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    bool result = nrf_802154_pan_coord_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_GET,
        SPINEL_DATATYPE_NRF_802154_PAN_COORD_GET_RET,
        result);
}

#endif // NRF_802154_PAN_COORD_GET_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PROMISCUOUS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RX_ON_WHEN_IDLE_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_rx_on_when_idle_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool           enabled;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_RX_ON_WHEN_IDLE_SET,
                                 &enabled);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_rx_on_when_idle_set(enabled);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CONTINUOUS_CARRIER.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_continuous_carrier(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    bool result = nrf_802154_continuous_carrier();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CONTINUOUS_CARRIER,
        SPINEL_DATATYPE_NRF_802154_CONTINUOUS_CARRIER_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_MODULATED_CARRIER.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_modulated_carrier(
    const void * p_property_data,
    size_t       property_data_len)
{
    const void * p_buffer;
    size_t       p_buffer_len;

    spinel_ssize_t siz = spinel_datatype_unpack(
        p_property_data,
        property_data_len,
        SPINEL_DATATYPE_NRF_802154_MODULATED_CARRIER,
        &p_buffer,
        &p_buffer_len);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (p_buffer_len != RAW_PAYLOAD_OFFSET + ((uint8_t *)p_buffer)[0])
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    bool result = nrf_802154_modulated_carrier(p_buffer);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_MODULATED_CARRIER,
        SPINEL_DATATYPE_NRF_802154_MODULATED_CARRIER_RET,
        result);
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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

    if (addr_len == EXTENDED_ADDRESS_SIZE)
    {
        extended = true;
    }
    else if (addr_len == SHORT_ADDRESS_SIZE)
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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

    if (addr_len == EXTENDED_ADDRESS_SIZE)
    {
        extended = true;
    }
    else if (addr_len == SHORT_ADDRESS_SIZE)
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ack_data_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    void                * p_addr;
    size_t                addr_len;
    void                * p_data;
    size_t                length;
    nrf_802154_ack_data_t data_type;
    spinel_ssize_t        siz;
    bool                  extended;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_ACK_DATA_SET,
                                 &p_addr,
                                 &addr_len,
                                 &p_data,
                                 &length,
                                 &data_type);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (addr_len == EXTENDED_ADDRESS_SIZE)
    {
        extended = true;
    }
    else if (addr_len == SHORT_ADDRESS_SIZE)
    {
        extended = false;
    }
    else
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    bool ack_data_set_res = nrf_802154_ack_data_set(
        (const uint8_t *)p_addr,
        extended,
        (const void *)p_data,
        (uint16_t)length,
        data_type);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_SET,
        SPINEL_DATATYPE_NRF_802154_ACK_DATA_SET_RET,
        ack_data_set_res);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_CLEAR.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ack_data_clear(
    const void * p_property_data,
    size_t       property_data_len)
{
    const uint8_t       * p_addr;
    size_t                addr_len;
    bool                  extended;
    nrf_802154_ack_data_t data_type;
    spinel_ssize_t        siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_ACK_DATA_CLEAR,
                                 &p_addr,
                                 &addr_len,
                                 &data_type);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    if (addr_len == EXTENDED_ADDRESS_SIZE)
    {
        extended = true;
    }
    else if (addr_len == SHORT_ADDRESS_SIZE)
    {
        extended = false;
    }
    else
    {
        return NRF_802154_SERIALIZATION_ERROR_REQUEST_INVALID;
    }

    bool ack_data_clear_res = nrf_802154_ack_data_clear(p_addr, extended, data_type);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_CLEAR,
        SPINEL_DATATYPE_NRF_802154_ACK_DATA_CLEAR_RET,
        ack_data_clear_res);
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ack_data_remove_all(
    const void * p_property_data,
    size_t       property_data_len)
{
    bool                  extended;
    nrf_802154_ack_data_t data_type;
    spinel_ssize_t        siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_ACK_DATA_REMOVE_ALL,
                                 &extended,
                                 &data_type);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_ack_data_remove_all(extended, data_type);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

#if NRF_802154_CSMA_CA_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_csma_ca_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t                               remote_frame_handle;
    const void                           * p_frame;
    size_t                                 frame_hdata_len;
    void                                 * p_local_frame_ptr;
    nrf_802154_transmit_csma_ca_metadata_t tx_metadata;

    spinel_ssize_t siz = spinel_datatype_unpack(
        p_property_data,
        property_data_len,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        NRF_802154_TRANSMIT_CSMA_CA_METADATA_DECODE(tx_metadata),
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
    bool result = nrf_802154_transmit_csma_ca_raw(p_local_frame_ptr, &tx_metadata);

    if (!result)
    {
        nrf_802154_buffer_mgr_dst_remove_by_local_pointer(nrf_802154_spinel_dst_buffer_mgr_get(),
                                                          p_local_frame_ptr);
    }

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csma_ca_min_be_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint8_t        min_be;
    spinel_ssize_t siz;
    bool           result;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_SET,
                                 &min_be);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    result = nrf_802154_csma_ca_min_be_set(min_be);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_SET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_SET_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_802154_CSMA_CA_MIN_BE_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csma_ca_min_be_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint8_t min_be = nrf_802154_csma_ca_min_be_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_GET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_GET_RET,
        min_be);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csma_ca_max_be_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint8_t        max_be;
    spinel_ssize_t siz;
    bool           result;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_SET,
                                 &max_be);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    result = nrf_802154_csma_ca_max_be_set(max_be);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_SET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_SET_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csma_ca_max_be_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint8_t max_be = nrf_802154_csma_ca_max_be_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_GET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_GET_RET,
        max_be);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csma_ca_max_backoffs_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint8_t        max_backoffs;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET,
                                 &max_backoffs);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_csma_ca_max_backoffs_set(max_backoffs);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to backoffs decoded.
 * @param[in]  property_data_len  Size of the @ref p_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csma_ca_max_backoffs_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint8_t max_backoffs = nrf_802154_csma_ca_max_backoffs_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET_RET,
        max_backoffs);
}

#endif // NRF_802154_CSMA_CA_ENABLED

#if NRF_802154_TEST_MODES_ENABLED
/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_test_mode_csmaca_backoff_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    nrf_802154_test_mode_csmaca_backoff_t value;
    spinel_ssize_t                        siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET,
                                 &value);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_test_mode_csmaca_backoff_set(value);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_test_mode_csmaca_backoff_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_test_mode_csmaca_backoff_t value = nrf_802154_test_mode_csmaca_backoff_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET,
        SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET_RET,
        value);
}

#endif // NRF_802154_TEST_MODES_ENABLED

#if NRF_802154_IFS_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_SET
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ifs_mode_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    nrf_802154_ifs_mode_t value;
    spinel_ssize_t        siz;
    bool                  result;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_IFS_MODE_SET,
                                 &value);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    result = nrf_802154_ifs_mode_set(value);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_SET,
        SPINEL_DATATYPE_NRF_802154_IFS_MODE_SET_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_GET
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ifs_mode_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_ifs_mode_t value = nrf_802154_ifs_mode_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_GET,
        SPINEL_DATATYPE_NRF_802154_IFS_MODE_GET_RET,
        value);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_SET
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ifs_min_sifs_period_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint16_t       value;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_SET,
                                 &value);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_ifs_min_sifs_period_set(value);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_GET
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ifs_min_sifs_period_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint16_t value = nrf_802154_ifs_min_sifs_period_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_GET,
        SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_GET_RET,
        value);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_SET
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ifs_min_lifs_period_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint16_t       value;
    spinel_ssize_t siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_SET,
                                 &value);

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_ifs_min_lifs_period_set(value);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_GET
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_ifs_min_lifs_period_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint16_t value = nrf_802154_ifs_min_lifs_period_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_GET,
        SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_GET_RET,
        value);
}

#endif // NRF_802154_IFS_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_raw(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t                       remote_frame_handle;
    const void                   * p_frame;
    size_t                         frame_hdata_len;
    void                         * p_local_frame_ptr;
    nrf_802154_transmit_metadata_t tx_metadata;

    spinel_ssize_t siz = spinel_datatype_unpack(
        p_property_data,
        property_data_len,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW,
        NRF_802154_TRANSMIT_METADATA_DECODE(tx_metadata),
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

    bool result = nrf_802154_transmit_raw(p_local_frame_ptr, &tx_metadata);

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

#if NRF_802154_DELAYED_TRX_ENABLED
/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW_AT.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_raw_at(
    const void * p_property_data,
    size_t       property_data_len)
{
    uint32_t                          remote_frame_handle;
    const void                      * p_frame;
    size_t                            frame_hdata_len;
    void                            * p_local_frame_ptr;
    nrf_802154_transmit_at_metadata_t tx_metadata;
    uint64_t                          tx_time;

    spinel_ssize_t siz = spinel_datatype_unpack(
        p_property_data,
        property_data_len,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_AT,
        NRF_802154_TRANSMIT_AT_METADATA_DECODE(tx_metadata),
        &tx_time,
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

    // tx_metadata.channel set to 0 means that NULL was passed as metadata on app core
    // and the channel should be set to the current channel from PIB
    if (0 == tx_metadata.channel)
    {
        tx_metadata.channel = nrf_802154_channel_get();
    }

    bool result = nrf_802154_transmit_raw_at(p_local_frame_ptr, tx_time, &tx_metadata);

    if (!result)
    {
        nrf_802154_buffer_mgr_dst_remove_by_local_pointer(nrf_802154_spinel_dst_buffer_mgr_get(),
                                                          p_local_frame_ptr);
    }
    else
    {
        // Latch the local pointer in case the transmission is cancelled
        mp_transmit_at_frame = p_local_frame_ptr;
    }

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW_AT,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_AT_RET,
        result);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_AT_CANCEL.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_transmit_at_cancel(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    bool result = nrf_802154_transmit_at_cancel();

    if (result)
    {
        // The transmission was cancelled successfully
        if (!mp_transmit_at_frame)
        {
            // This should never happen
            return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
        }

        // Free the local frame pointer
        bool removed = nrf_802154_buffer_mgr_dst_remove_by_local_pointer(
            nrf_802154_spinel_dst_buffer_mgr_get(),
            mp_transmit_at_frame);

        if (!removed)
        {
            return NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER;
        }
    }

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_AT_CANCEL,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_CANCEL_RET,
        result);
}

#endif // NRF_802154_DELAYED_TRX_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_BUFFER_FREE_RAW.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_tx_power_set(
    const void * p_property_data,
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_tx_power_get(
    const void * p_property_data,
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
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
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

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_time_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    uint64_t time;

    time = nrf_802154_time_get();

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET,
        SPINEL_DATATYPE_NRF_802154_TIME_GET_RET,
        time);
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_cca_cfg_get(const void * p_property_data,
                                                                      size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_cca_cfg_t cfg;

    nrf_802154_cca_cfg_get(&cfg);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_GET,
        SPINEL_DATATYPE_NRF_802154_CCA_CFG_GET_RET,
        NRF_802154_CCA_CFG_ENCODE(cfg));
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_stat_timestamps_get(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_stat_timestamps_t t;

    nrf_802154_stat_timestamps_get(&t);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_STAT_TIMESTAMPS_GET,
        SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_GET_RET,
        NRF_802154_STAT_TIMESTAMPS_ENCODE(t));
}

static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_cca_cfg_set(const void * p_property_data,
                                                                      size_t       property_data_len)
{
    nrf_802154_cca_cfg_t cfg;
    spinel_ssize_t       siz;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CCA_CFG_SET,
                                 NRF_802154_CCA_CFG_DECODE(cfg));

    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_cca_cfg_set(&cfg);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_security_global_frame_counter_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    spinel_ssize_t siz;
    uint32_t       frame_counter;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET,
                                 &frame_counter);
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_security_global_frame_counter_set(frame_counter);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET_IF_LARGER.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t
spinel_decode_prop_nrf_802154_security_global_frame_counter_set_if_larger(
    const void * p_property_data,
    size_t       property_data_len)
{
    spinel_ssize_t siz;
    uint32_t       frame_counter;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET,
                                 &frame_counter);
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_security_global_frame_counter_set_if_larger(frame_counter);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_STORE.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_security_key_store(
    const void * p_property_data,
    size_t       property_data_len)
{
    spinel_ssize_t              siz;
    nrf_802154_key_t            key = {0};
    uint32_t                    key_size;
    uint32_t                    key_id_size;
    nrf_802154_security_error_t err;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_STORE,
                                 NRF_802154_SECURITY_KEY_STORE_DECODE(key, key_size, key_id_size));
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    err = nrf_802154_security_key_store(&key);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_STORE,
        SPINEL_DATATYPE_NRF_802154_SECURITY_ERROR_RET,
        err);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_security_key_remove(
    const void * p_property_data,
    size_t       property_data_len)
{
    spinel_ssize_t              siz;
    uint8_t                     key_id_data[KEY_ID_MODE_3_SIZE];
    nrf_802154_key_id_t         key_id = {.p_key_id = key_id_data};
    uint32_t                    key_id_size;
    nrf_802154_security_error_t err;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_REMOVE,
                                 NRF_802154_SECURITY_KEY_REMOVE_DECODE(key_id, key_id_size));
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    err = nrf_802154_security_key_remove(&key_id);

    return nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE,
        SPINEL_DATATYPE_NRF_802154_SECURITY_ERROR_RET,
        err);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE_ALL.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_security_key_remove_all(
    const void * p_property_data,
    size_t       property_data_len)
{
    (void)p_property_data;
    (void)property_data_len;

    nrf_802154_security_key_remove_all();

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

#if NRF_802154_DELAYED_TRX_ENABLED && NRF_802154_IE_WRITER_ENABLED

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_PERIOD_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csl_writer_period_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    spinel_ssize_t siz;
    uint16_t       csl_period;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CSL_WRITER_PERIOD_SET,
                                 &csl_period);
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_csl_writer_period_set(csl_period);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

/**
 * @brief Decode and dispatch SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET.
 *
 * @param[in]  p_property_data    Pointer to a buffer that contains data to be decoded.
 * @param[in]  property_data_len  Size of the @ref p_property_data buffer.
 *
 */
static nrf_802154_ser_err_t spinel_decode_prop_nrf_802154_csl_writer_anchor_time_set(
    const void * p_property_data,
    size_t       property_data_len)
{
    spinel_ssize_t siz;
    uint64_t       csl_anchor_time;

    siz = spinel_datatype_unpack(p_property_data,
                                 property_data_len,
                                 SPINEL_DATATYPE_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET,
                                 &csl_anchor_time);
    if (siz < 0)
    {
        return NRF_802154_SERIALIZATION_ERROR_DECODING_FAILURE;
    }

    nrf_802154_csl_writer_anchor_time_set(csl_anchor_time);

    return nrf_802154_spinel_send_prop_last_status_is(SPINEL_STATUS_OK);
}

#endif // NRF_802154_DELAYED_TRX_ENABLED && NRF_802154_IE_WRITER_ENABLED

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

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP_IF_IDLE:
            return spinel_decode_prop_nrf_802154_sleep_if_idle(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE:
            return spinel_decode_prop_nrf_802154_receive(p_property_data, property_data_len);

#if NRF_802154_DELAYED_TRX_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT:
            return spinel_decode_prop_nrf_802154_receive_at(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_CANCEL:
            return spinel_decode_prop_nrf_802154_receive_at_cancel(p_property_data,
                                                                   property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL:
            return spinel_decode_prop_nrf_802154_receive_at_scheduled_cancel(p_property_data,
                                                                             property_data_len);

#endif // NRF_802154_DELAYED_TRX_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET:
            return spinel_decode_prop_nrf_802154_channel_get(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_SET:
            return spinel_decode_prop_nrf_802154_channel_set(p_property_data, property_data_len);

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

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET:
            return spinel_decode_prop_nrf_802154_alternate_short_address_set(p_property_data,
                                                                             property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_EXTENDED_ADDRESS_SET:
            return spinel_decode_prop_nrf_802154_extended_address_set(p_property_data,
                                                                      property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_SET:
            return spinel_decode_prop_nrf_802154_pan_coord_set(p_property_data, property_data_len);
#if NRF_802154_PAN_COORD_GET_ENABLED

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_GET:
            return spinel_decode_prop_nrf_802154_pan_coord_get(p_property_data, property_data_len);

#endif // NRF_802154_PAN_COORD_GET_ENABLED

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PROMISCUOUS_SET:
            return spinel_decode_prop_nrf_802154_promiscuous_set(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RX_ON_WHEN_IDLE_SET:
            return spinel_decode_prop_nrf_802154_rx_on_when_idle_set(p_property_data,
                                                                     property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA:
            return spinel_decode_prop_nrf_802154_cca(p_property_data, property_data_len);

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CONTINUOUS_CARRIER:
            return spinel_decode_prop_nrf_802154_continuous_carrier(p_property_data,
                                                                    property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_MODULATED_CARRIER:
            return spinel_decode_prop_nrf_802154_modulated_carrier(p_property_data,
                                                                   property_data_len);

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION:
            return spinel_decode_prop_nrf_802154_energy_detection(p_property_data,
                                                                  property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_SET:
            return spinel_decode_prop_nrf_802154_tx_power_set(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET:
            return spinel_decode_prop_nrf_802154_tx_power_get(p_property_data, property_data_len);

#if NRF_802154_CSMA_CA_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW:
            return spinel_decode_prop_nrf_802154_transmit_csma_ca_raw(p_property_data,
                                                                      property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_SET:
            return spinel_decode_prop_nrf_802154_csma_ca_min_be_set(p_property_data,
                                                                    property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_GET:
            return spinel_decode_prop_nrf_802154_csma_ca_min_be_get(p_property_data,
                                                                    property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_SET:
            return spinel_decode_prop_nrf_802154_csma_ca_max_be_set(p_property_data,
                                                                    property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_GET:
            return spinel_decode_prop_nrf_802154_csma_ca_max_be_get(p_property_data,
                                                                    property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET:
            return spinel_decode_prop_nrf_802154_csma_ca_max_backoffs_set(p_property_data,
                                                                          property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET:
            return spinel_decode_prop_nrf_802154_csma_ca_max_backoffs_get(p_property_data,
                                                                          property_data_len);
#endif // NRF_802154_CSMA_CA_ENABLED

#if NRF_802154_TEST_MODES_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET:
            return spinel_decode_prop_nrf_802154_test_mode_csmaca_backoff_set(p_property_data,
                                                                              property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET:
            return spinel_decode_prop_nrf_802154_test_mode_csmaca_backoff_get(p_property_data,
                                                                              property_data_len);
#endif // NRF_802154_TEST_MODES_ENABLED

#if NRF_802154_IFS_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_SET:
            return spinel_decode_prop_nrf_802154_ifs_mode_set(p_property_data,
                                                              property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_GET:
            return spinel_decode_prop_nrf_802154_ifs_mode_get(p_property_data,
                                                              property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_SET:
            return spinel_decode_prop_nrf_802154_ifs_min_sifs_period_set(p_property_data,
                                                                         property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_GET:
            return spinel_decode_prop_nrf_802154_ifs_min_sifs_period_get(p_property_data,
                                                                         property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_SET:
            return spinel_decode_prop_nrf_802154_ifs_min_lifs_period_set(p_property_data,
                                                                         property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_GET:
            return spinel_decode_prop_nrf_802154_ifs_min_lifs_period_get(p_property_data,
                                                                         property_data_len);

#endif // NRF_802154_IFS_ENABLED

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW:
            return spinel_decode_prop_nrf_802154_transmit_raw(p_property_data, property_data_len);

#if NRF_802154_DELAYED_TRX_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW_AT:
            return spinel_decode_prop_nrf_802154_transmit_raw_at(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_AT_CANCEL:
            return spinel_decode_prop_nrf_802154_transmit_at_cancel(p_property_data,
                                                                    property_data_len);

#endif // NRF_802154_DELAYED_TRX_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_BUFFER_FREE_RAW:
            return spinel_decode_prop_nrf_802154_buffer_free_raw(p_property_data,
                                                                 property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET:
            return spinel_decode_prop_nrf_802154_capabilities_get(p_property_data,
                                                                  property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET:
            return spinel_decode_prop_nrf_802154_time_get(p_property_data,
                                                          property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_GET:
            return spinel_decode_prop_nrf_802154_cca_cfg_get(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_SET:
            return spinel_decode_prop_nrf_802154_cca_cfg_set(p_property_data, property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_SET:
            return spinel_decode_prop_nrf_802154_ack_data_set(p_property_data,
                                                              property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_CLEAR:
            return spinel_decode_prop_nrf_802154_ack_data_clear(p_property_data,
                                                                property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_REMOVE_ALL:
            return spinel_decode_prop_nrf_802154_ack_data_remove_all(p_property_data,
                                                                     property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET:
            return spinel_decode_prop_nrf_802154_security_global_frame_counter_set(p_property_data,
                                                                                   property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_STORE:
            return spinel_decode_prop_nrf_802154_security_key_store(p_property_data,
                                                                    property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE:
            return spinel_decode_prop_nrf_802154_security_key_remove(p_property_data,
                                                                     property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE_ALL:
            return spinel_decode_prop_nrf_802154_security_key_remove_all(p_property_data,
                                                                         property_data_len);

#if NRF_802154_DELAYED_TRX_ENABLED && NRF_802154_IE_WRITER_ENABLED
        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_PERIOD_SET:
            return spinel_decode_prop_nrf_802154_csl_writer_period_set(p_property_data,
                                                                       property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET:
            return spinel_decode_prop_nrf_802154_csl_writer_anchor_time_set(p_property_data,
                                                                            property_data_len);
#endif // NRF_802154_DELAYED_TRX_ENABLED && NRF_802154_IE_WRITER_ENABLED

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_STAT_TIMESTAMPS_GET:
            return spinel_decode_prop_nrf_802154_stat_timestamps_get(p_property_data,
                                                                     property_data_len);

        case SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET_IF_LARGER:
            return spinel_decode_prop_nrf_802154_security_global_frame_counter_set_if_larger(
                p_property_data,
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
