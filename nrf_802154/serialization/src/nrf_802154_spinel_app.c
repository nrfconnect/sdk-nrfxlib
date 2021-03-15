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

/**
 * @file nrf_802154_spinel_app.c
 *
 * @brief Implementation of nRF 802.15.4 Radio Driver API for application
 *        core talking to network core through spinel
 *
 * This file is responsible for providing implementation of API calls
 * defined by nrf_802154.h. Application is responsible for providing
 * appropriate callouts according to nRF 802.15.4 API.
 */

#include <stdint.h>

#include "nrf_802154_const.h"

#include "../spinel_base/spinel.h"
#include "nrf_802154_serialization.h"
#include "nrf_802154_spinel.h"
#include "nrf_802154_spinel_datatypes.h"
#include "nrf_802154_spinel_enc_app.h"
#include "nrf_802154_spinel_dec_app.h"
#include "nrf_802154_spinel_log.h"
#include "nrf_802154_spinel_response_notifier.h"
#include "nrf_802154_serialization_error.h"
#include "nrf_802154_serialization_error_helper.h"
#include "nrf_802154_buffer_mgr_dst.h"
#include "nrf_802154_buffer_mgr_src.h"

#include "nrf_802154.h"
#include "nrf_802154_types.h"

/**
 * @brief Wait with timeout for SPINEL_STATUS_OK to be received.
 *
 * @param[in]  timeout   Timeout in us.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t status_ok_await(uint32_t timeout)
{
    spinel_status_t                   status = SPINEL_STATUS_FAILURE;
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_last_status(p_notify_data->data,
                                                    p_notify_data->data_len,
                                                    &status);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", spinel_status_to_cstr(status), "status");

    SERIALIZATION_ERROR_IF(status != SPINEL_STATUS_OK,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_INVALID,
                           error,
                           bail);

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

/**
 * @brief Wait with timeout for some single bool property to be received.
 *
 * @param[out] p_net_response   Pointer to the bool variable which needs to be populated.
 * @param[in]  property_id      Awaited property identifier.
 * @param[in]  timeout          Timeout in us.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t net_generic_bool_response_await(
    bool   * p_net_response,
    uint32_t timeout)
{
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_generic_bool(p_notify_data->data,
                                                     p_notify_data->data_len,
                                                     p_net_response);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (*p_net_response) ? "true" : "false", "net response");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

/**
 * @brief Wait with timeout for channel property to be received.
 *
 * @param[in]  timeout   Timeout in us.
 * @param[out] p_channel Pointer to the channel variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t channel_await(uint32_t timeout, uint8_t * p_channel)
{
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(
        timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_channel(p_notify_data->data,
                                                p_notify_data->data_len,
                                                p_channel);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", *p_channel, "channel");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

/**
 * @brief Wait with timeout for TX Power property to be received.
 *
 * @param[in]  timeout   Timeout in us.
 * @param[out] p_power   Pointer to the power variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t tx_power_await(uint32_t timeout, int8_t * p_power)
{
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(
        timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_nrf_802154_tx_power_get_ret(p_notify_data->data,
                                                                    p_notify_data->data_len,
                                                                    p_power);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%d", *p_power, "Tx power");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

/**
 * @brief Wait with timeout for capabilities property to be received.
 *
 * @param[in]  timeout          Timeout in us.
 * @param[out] p_capabilities   Pointer to the capabilities variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t capabilities_await(uint32_t                    timeout,
                                               nrf_802154_capabilities_t * p_capabilities)
{
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(
        timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_nrf_802154_capabilities_get_ret(p_notify_data->data,
                                                                        p_notify_data->data_len,
                                                                        p_capabilities);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", *p_capabilities, "Capabilities");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

void nrf_802154_init(void)
{
    nrf_802154_serialization_init();
}

bool nrf_802154_sleep(void)
{
    nrf_802154_ser_err_t res;
    bool                 sleep_remote_resp = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP);

    res = nrf_802154_spinel_send_cmd_prop_value_set(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP,
                                                    SPINEL_DATATYPE_NRF_802154_SLEEP,
                                                    NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(&sleep_remote_resp,
                                          CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return sleep_remote_resp;
}

bool nrf_802154_receive(void)
{
    nrf_802154_ser_err_t res;
    bool                 receive_remote_resp = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE);

    res = nrf_802154_spinel_send_cmd_prop_value_set(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE,
                                                    SPINEL_DATATYPE_NRF_802154_RECEIVE,
                                                    NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(&receive_remote_resp,
                                          CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return receive_remote_resp;
}

void nrf_802154_pan_id_set(const uint8_t * p_pan_id)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_pan_id, PAN_ID_SIZE);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_ID_SET,
                                                    SPINEL_DATATYPE_NRF_802154_PAN_ID_SET,
                                                    p_pan_id,
                                                    PAN_ID_SIZE);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_short_address_set(const uint8_t * p_short_address)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_short_address, SHORT_ADDRESS_SIZE);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SHORT_ADDRESS_SET,
        SPINEL_DATATYPE_NRF_802154_SHORT_ADDRESS_SET,
        p_short_address,
        SHORT_ADDRESS_SIZE);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_extended_address_set(const uint8_t * p_extended_address)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_extended_address, EXTENDED_ADDRESS_SIZE);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_EXTENDED_ADDRESS_SET,
        SPINEL_DATATYPE_NRF_802154_EXTENDED_ADDRESS_SET,
        p_extended_address,
        EXTENDED_ADDRESS_SIZE);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_pan_coord_set(bool enabled)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", enabled ? "true" : "false", "enabled");

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_SET,
        SPINEL_DATATYPE_NRF_802154_PAN_COORD_SET,
        enabled);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_promiscuous_set(bool enabled)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", enabled ? "true" : "false", "enabled");

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PROMISCUOUS_SET,
        SPINEL_DATATYPE_NRF_802154_PROMISCUOUS_SET,
        enabled);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);
}

void nrf_802154_src_addr_matching_method_set(nrf_802154_src_addr_match_t match_method)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", match_method);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET,
        SPINEL_DATATYPE_NRF_802154_SRC_ADDR_MATCHING_METHOD_SET,
        match_method);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);
}

void nrf_802154_auto_pending_bit_set(bool enabled)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (enabled ? "true" : "false"), "enabled");

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_AUTO_PENDING_BIT_SET,
        SPINEL_DATATYPE_NRF_802154_AUTO_PENDING_BIT_SET,
        enabled);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);
}

bool nrf_802154_pending_bit_for_addr_set(const uint8_t * p_addr, bool extended)
{
    nrf_802154_ser_err_t res;
    bool                 addr_set_res = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_addr, extended ? 8 : 2);
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET,
        p_addr,
        extended ? 8 : 2);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(&addr_set_res,
                                          CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return addr_set_res;
}

bool nrf_802154_pending_bit_for_addr_clear(const uint8_t * p_addr, bool extended)
{
    nrf_802154_ser_err_t res;
    bool                 addr_clr_res = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_addr, extended ? 8 : 2);
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR,
        p_addr,
        extended ? 8 : 2);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(&addr_clr_res,
                                          CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return addr_clr_res;
}

void nrf_802154_pending_bit_for_addr_reset(bool extended)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_RESET,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_RESET,
        extended);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);
    return;
}

void nrf_802154_channel_set(uint8_t channel)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", channel);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_SET,
        SPINEL_DATATYPE_NRF_802154_CHANNEL_SET,
        channel);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

uint8_t nrf_802154_channel_get(void)
{
    nrf_802154_ser_err_t res;
    uint8_t              channel = 0;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CHANNEL_GET,
        SPINEL_DATATYPE_NRF_802154_CHANNEL_GET,
        &channel);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = channel_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, &channel);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return channel;
}

bool nrf_802154_cca(void)
{
    nrf_802154_ser_err_t res;
    bool                 cca_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA,
        SPINEL_DATATYPE_NRF_802154_CCA,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(&cca_result,
                                          CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return cca_result;
}

bool nrf_802154_energy_detection(uint32_t time_us)
{
    nrf_802154_ser_err_t res;
    bool                 ed_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ENERGY_DETECTION,
        SPINEL_DATATYPE_NRF_802154_ENERGY_DETECTION,
        time_us);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(
        &ed_result,
        CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return ed_result;
}

void nrf_802154_transmit_csma_ca_raw(const uint8_t * p_data)
{
    nrf_802154_ser_err_t res;
    uint32_t             data_handle;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_data, p_data[0]);

    bool handle_added = nrf_802154_buffer_mgr_src_add(nrf_802154_spinel_src_buffer_mgr_get(),
                                                      p_data,
                                                      &data_handle);

    SERIALIZATION_ERROR_IF(!handle_added, NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        NRF_802154_HDATA_ENCODE(data_handle, p_data, p_data[0]));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

bool nrf_802154_transmit_raw(const uint8_t * p_data, bool cca)
{
    nrf_802154_ser_err_t res;
    uint32_t             data_handle;
    bool                 transmit_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_data, p_data[0]);

    bool handle_added = nrf_802154_buffer_mgr_src_add(nrf_802154_spinel_src_buffer_mgr_get(),
                                                      p_data,
                                                      &data_handle);

    SERIALIZATION_ERROR_IF(!handle_added, NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW,
        NRF_802154_HDATA_ENCODE(data_handle, p_data, p_data[0]),
        cca);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(&transmit_result,
                                          CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    return transmit_result;

bail:
    if (handle_added)
    {
        /* Rollback what we did until an error to avoid memory leak. */
        nrf_802154_buffer_mgr_src_remove_by_buffer_handle(
            nrf_802154_spinel_src_buffer_mgr_get(),
            data_handle);
    }

    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return transmit_result;
}

void nrf_802154_buffer_free_raw(uint8_t * p_data)
{
    nrf_802154_ser_err_t res;
    uint32_t             data_handle;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_data, p_data[0]);

    bool handle_found = nrf_802154_buffer_mgr_dst_search_by_local_pointer(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        p_data,
        &data_handle);

    SERIALIZATION_ERROR_IF(!handle_found,
                           NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER,
                           error,
                           bail);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_BUFFER_FREE_RAW,
        SPINEL_DATATYPE_NRF_802154_BUFFER_FREE_RAW,
        data_handle);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

    (void)nrf_802154_buffer_mgr_dst_remove_by_local_pointer(nrf_802154_spinel_dst_buffer_mgr_get(),
                                                            p_data);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_tx_power_set(int8_t power)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_SET,
        SPINEL_DATATYPE_NRF_802154_TX_POWER_SET,
        power);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

int8_t nrf_802154_tx_power_get(void)
{
    int32_t res;
    int8_t  power = 0;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TX_POWER_GET,
        SPINEL_DATATYPE_NRF_802154_TX_POWER_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = tx_power_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, &power);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return power;
}

nrf_802154_capabilities_t nrf_802154_capabilities_get(void)
{
    int32_t                   res;
    nrf_802154_capabilities_t caps = 0UL;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CAPABILITIES_GET,
        SPINEL_DATATYPE_NRF_802154_CAPABILITIES_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = capabilities_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, &caps);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return caps;
}

int8_t nrf_802154_dbm_from_energy_level_calculate(uint8_t energy_level)
{
    return ED_MIN_DBM + (energy_level / ED_RESULT_FACTOR);
}
