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
 * @file nrf_802154_spinel_net.c
 *
 * @brief Client of nRF 802.15.4 Radio Driver for the network core
 *        responsible for serializing callouts through spinel to application core.
 *
 * This file is responsible for providing implementation callouts defined by
 * defined by nrf_802154.h as required by nRF 802.15.4 API. The implementation
 * of callouts send commands through spinel to application core effectively
 * causing calling of corresponding callouts in the application core.
 */

#include <stdint.h>

#include "../spinel_base/spinel.h"
#include "nrf_802154_spinel.h"
#include "nrf_802154_spinel_datatypes.h"
#include "nrf_802154_spinel_enc_net.h"
#include "nrf_802154_spinel_log.h"
#include "nrf_802154_spinel_response_notifier.h"
#include "nrf_802154_serialization_error.h"
#include "nrf_802154_serialization_error_helper.h"
#include "nrf_802154_buffer_mgr_dst.h"
#include "nrf_802154_buffer_mgr_src.h"

#include "nrf_802154.h"

/**@brief A pointer to the last transmitted ACK frame. */
static const uint8_t * volatile mp_last_tx_ack;

static void local_transmitted_frame_ptr_free(void * p_frame)
{
    SERIALIZATION_ERROR_INIT(error);

    bool frame_found = nrf_802154_buffer_mgr_dst_remove_by_local_pointer(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        p_frame);

    SERIALIZATION_ERROR_IF(!frame_found,
                           NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER,
                           error,
                           bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_cca_done(bool channel_free)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", channel_free ? "true" : "false", "channel_free");

    res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_DONE,
        SPINEL_DATATYPE_NRF_802154_CCA_DONE,
        channel_free);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_cca_failed(nrf_802154_cca_error_t err)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", err);

    res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_FAILED,
        SPINEL_DATATYPE_NRF_802154_CCA_FAILED,
        err);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_energy_detected(const nrf_802154_energy_detected_t * p_result)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", p_result->ed_dbm);

    res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTED,
        SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTED,
        p_result->ed_dbm);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_energy_detection_failed(nrf_802154_ed_error_t err)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", err);

    res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION_FAILED,
        SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION_FAILED,
        err);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_tx_ack_started(const uint8_t * p_data)
{
    /* Due to timing restrictions this function cannot be serialized directly.
     * Instead parameter is memorized and serialization is triggered by
     * later call to last_tx_ack_started_send
     */
    mp_last_tx_ack = p_data;
}

static nrf_802154_ser_err_t last_tx_ack_started_send(void)
{
    nrf_802154_ser_err_t res           = NRF_802154_SERIALIZATION_ERROR_OK;
    const uint8_t      * p_last_tx_ack = mp_last_tx_ack;

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    if (p_last_tx_ack != NULL)
    {
        mp_last_tx_ack = NULL;

        res = nrf_802154_spinel_send_cmd_prop_value_is(
            SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_ACK_STARTED,
            SPINEL_DATATYPE_NRF_802154_TX_ACK_STARTED,
            p_last_tx_ack,
            (size_t)(p_last_tx_ack[0]));
    }

    return res;
}

void nrf_802154_received_timestamp_raw(uint8_t * p_data,
                                       int8_t    power,
                                       uint8_t   lqi,
                                       uint64_t  time)
{
    nrf_802154_ser_err_t res;
    uint32_t             local_data_handle;

    SERIALIZATION_ERROR_INIT(error);

    res = last_tx_ack_started_send();
    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_data, p_data[0]);

    // Create a handle to the original frame buffer
    bool handle_added = nrf_802154_buffer_mgr_src_add(nrf_802154_spinel_src_buffer_mgr_get(),
                                                      (void *)p_data,
                                                      &local_data_handle);

    if (!handle_added)
    {
        // Handle could not be created. Drop the frame and throw an error
        nrf_802154_buffer_free_raw(p_data);
        SERIALIZATION_ERROR(NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);
    }

    // Serialize the call
    res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVED_TIMESTAMP_RAW,
        SPINEL_DATATYPE_NRF_802154_RECEIVED_TIMESTAMP_RAW,
        NRF_802154_HDATA_ENCODE(local_data_handle, p_data, p_data[0]),
        power,
        lqi,
        time);

    if (res < 0)
    {
        // Serialization failed. Drop the frame, clean up and throw an error
        nrf_802154_buffer_mgr_src_remove_by_buffer_handle(nrf_802154_spinel_src_buffer_mgr_get(),
                                                          local_data_handle);

        nrf_802154_buffer_free_raw(p_data);

        SERIALIZATION_ERROR(res, error, bail);
    }

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_receive_failed(nrf_802154_rx_error_t error, uint32_t id)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(ser_error);

    res = last_tx_ack_started_send();
    SERIALIZATION_ERROR_CHECK(res, ser_error, bail);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", error);

    // Serialize the call
    res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_FAILED,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_FAILED,
        error,
        id);

    SERIALIZATION_ERROR_CHECK(res, ser_error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(ser_error);
}

void nrf_802154_transmitted_raw(uint8_t                                   * p_frame,
                                const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    uint32_t  remote_frame_handle;
    uint32_t  ack_handle = 0;
    uint8_t * p_ack      = p_metadata->data.transmitted.p_ack;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_frame, p_frame[0]);

    // Search for the handle to the original frame buffer based on the local pointer
    bool frame_found = nrf_802154_buffer_mgr_dst_search_by_local_pointer(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        (void *)p_frame,
        &remote_frame_handle);

    // The handle is expected to be found, throw an error if it was not found
    SERIALIZATION_ERROR_IF(!frame_found,
                           NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER,
                           error,
                           bail);

    if (p_ack != NULL)
    {
        // Create a handle to the original Ack buffer
        bool ack_handle_added = nrf_802154_buffer_mgr_src_add(
            nrf_802154_spinel_src_buffer_mgr_get(),
            (void *)p_ack,
            &ack_handle);

        if (!ack_handle_added)
        {
            // Drop the transmitted frame and throw an error if Ack could not be stored
            local_transmitted_frame_ptr_free((void *)p_frame);
            SERIALIZATION_ERROR(NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);
        }
    }

    // Serialize the call
    nrf_802154_ser_err_t res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMITTED_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMITTED_RAW,
        NRF_802154_TRANSMITTED_RAW_ENCODE(remote_frame_handle, p_frame, *p_metadata, ack_handle));

    // Free the local frame pointer no matter the result of serialization
    local_transmitted_frame_ptr_free((void *)p_frame);

    // Throw an error if serialization failed
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_transmit_failed(uint8_t                                   * p_frame,
                                nrf_802154_tx_error_t                       tx_error,
                                const nrf_802154_transmit_done_metadata_t * p_metadata)
{
    uint32_t remote_frame_handle;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_frame, p_frame[0]);

    // Search for the handle to the original frame buffer based on the local pointer
    bool frame_found = nrf_802154_buffer_mgr_dst_search_by_local_pointer(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        (void *)p_frame,
        &remote_frame_handle);

    // The handle is expected to be found, throw an error if it was not found
    SERIALIZATION_ERROR_IF(!frame_found,
                           NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER,
                           error,
                           bail);

    // Serialize the call
    nrf_802154_ser_err_t res = nrf_802154_spinel_send_cmd_prop_value_is(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_FAILED,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_FAILED,
        NRF_802154_TRANSMIT_FAILED_ENCODE(remote_frame_handle, p_frame, tx_error, *p_metadata));

    // Free the local frame pointer no matter the result of serialization
    local_transmitted_frame_ptr_free((void *)p_frame);

    // Throw an error if serialization failed
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

#ifdef TEST
/**@brief Unit test facility */
void nrf_802154_spinel_net_module_reset(void)
{
    mp_last_tx_ack = NULL;
}

#endif
