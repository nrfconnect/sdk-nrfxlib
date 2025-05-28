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
#include "nrf_802154_config.h"
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
 * @param[in]  timeout          Timeout in us.
 * @param[out] p_net_response   Pointer to the bool variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t net_generic_bool_response_await(uint32_t timeout,
                                                            bool   * p_net_response)
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
 * @brief Wait with timeout for some single uint8_t property to be received.
 *
 * @param[in]  timeout          Timeout in us.
 * @param[out] p_net_response   Pointer to the uint8_t variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t net_generic_uint8_response_await(uint32_t  timeout,
                                                             uint8_t * p_net_response)
{
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_generic_uint8(p_notify_data->data,
                                                      p_notify_data->data_len,
                                                      p_net_response);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", *p_net_response, "net response");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

#if NRF_802154_IFS_ENABLED
/**
 * @brief Wait with timeout for some single uint16_t property to be received.
 *
 * @param[in]  timeout          Timeout in us.
 * @param[out] p_net_response   Pointer to the uint16_t variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t net_generic_uint16_response_await(uint32_t   timeout,
                                                              uint16_t * p_net_response)
{
    nrf_802154_ser_err_t              res;
    nrf_802154_spinel_notify_buff_t * p_notify_data = NULL;

    SERIALIZATION_ERROR_INIT(error);

    p_notify_data = nrf_802154_spinel_response_notifier_property_await(timeout);

    SERIALIZATION_ERROR_IF(p_notify_data == NULL,
                           NRF_802154_SERIALIZATION_ERROR_RESPONSE_TIMEOUT,
                           error,
                           bail);

    res = nrf_802154_spinel_decode_prop_generic_uint16(p_notify_data->data,
                                                       p_notify_data->data_len,
                                                       p_net_response);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", *p_net_response, "net response");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

#endif

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

/**
 * @brief Wait with timeout for time property to be received.
 *
 * @param[in]  timeout  Timeout in us.
 * @param[out] p_time   Pointer to the time variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t time_await(uint32_t   timeout,
                                       uint64_t * p_time)
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

    res = nrf_802154_spinel_decode_prop_nrf_802154_time_get_ret(p_notify_data->data,
                                                                p_notify_data->data_len,
                                                                p_time);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%llu", *p_time, "Time");

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

/**
 * @brief Wait with timeout for CCA configuration property to be received.
 *
 * @param[in]  timeout  Timeout in us.
 * @param[out] p_cfg    Pointer to the CCA configuration variable which needs to be populated.
 *
 * @returns  zero on success or negative error value on failure.
 *
 */
static nrf_802154_ser_err_t cca_cfg_await(uint32_t               timeout,
                                          nrf_802154_cca_cfg_t * p_cfg)
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

    res = nrf_802154_spinel_decode_prop_nrf_802154_cca_cfg_get_ret(p_notify_data->data,
                                                                   p_notify_data->data_len,
                                                                   p_cfg);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", p_cfg->mode, "Mode");
    NRF_802154_SPINEL_LOG_VAR_NAMED("%d", p_cfg->ed_threshold, "ED threshold");
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", p_cfg->corr_threshold, "Corr threshold");
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", p_cfg->corr_limit, "Corr limit");

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

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &sleep_remote_resp);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return sleep_remote_resp;
}

nrf_802154_sleep_error_t nrf_802154_sleep_if_idle(void)
{
    nrf_802154_ser_err_t     res;
    nrf_802154_sleep_error_t sleep_remote_resp = NRF_802154_SLEEP_ERROR_NONE;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP_IF_IDLE);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SLEEP_IF_IDLE,
        SPINEL_DATATYPE_NRF_802154_SLEEP_IF_IDLE,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &sleep_remote_resp);

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

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &receive_remote_resp);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return receive_remote_resp;
}

bool nrf_802154_receive_at(uint64_t rx_time,
                           uint32_t timeout,
                           uint8_t  channel,
                           uint32_t id)
{
    nrf_802154_ser_err_t res;
    bool                 rx_at_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_AT,
        rx_time,
        timeout,
        channel,
        id);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &rx_at_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return rx_at_result;

}

bool nrf_802154_receive_at_cancel(uint32_t id)
{
    nrf_802154_ser_err_t res;
    bool                 cancel_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_CANCEL);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_CANCEL,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_CANCEL,
        id);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &cancel_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return cancel_result;
}

bool nrf_802154_receive_at_scheduled_cancel(uint32_t id)
{
    nrf_802154_ser_err_t res;
    bool                 cancel_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL,
        SPINEL_DATATYPE_NRF_802154_RECEIVE_AT_SCHEDULED_CANCEL,
        id);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &cancel_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return cancel_result;
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

void nrf_802154_alternate_short_address_set(const uint8_t * p_short_address)
{
    nrf_802154_ser_err_t res;

    bool    data_valid      = p_short_address != NULL;
    uint8_t invalid_addr[2] = {0xff, 0xff};

    if (!data_valid)
    {
        p_short_address = invalid_addr;
    }

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_short_address, SHORT_ADDRESS_SIZE);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET,
        SPINEL_DATATYPE_NRF_802154_ALTERNATE_SHORT_ADDRESS_SET,
        data_valid,
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

#if NRF_802154_PAN_COORD_GET_ENABLED

bool nrf_802154_pan_coord_get(void)
{
    nrf_802154_ser_err_t res;
    bool                 result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PAN_COORD_GET,
        SPINEL_DATATYPE_NRF_802154_PAN_COORD_GET,
        &result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return result;
}

#endif // NRF_802154_PAN_COORD_GET_ENABLED

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

void nrf_802154_rx_on_when_idle_set(bool enabled)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", enabled ? "true" : "false", "enabled");

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_RX_ON_WHEN_IDLE_SET,
        SPINEL_DATATYPE_NRF_802154_RX_ON_WHEN_IDLE_SET,
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

bool nrf_802154_ack_data_set(const uint8_t       * p_addr,
                             bool                  extended,
                             const void          * p_data,
                             uint16_t              length,
                             nrf_802154_ack_data_t data_type)
{
    nrf_802154_ser_err_t res;
    bool                 ack_data_set_res = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_addr, extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE);
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_SET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_SET,
        SPINEL_DATATYPE_NRF_802154_ACK_DATA_SET,
        p_addr,
        extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE,
        p_data,
        length,
        data_type);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &ack_data_set_res);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return ack_data_set_res;
}

bool nrf_802154_ack_data_clear(const uint8_t       * p_addr,
                               bool                  extended,
                               nrf_802154_ack_data_t data_type)
{
    nrf_802154_ser_err_t res;
    bool                 ack_data_clear_res = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_addr, extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE);
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_CLEAR);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_CLEAR,
        SPINEL_DATATYPE_NRF_802154_ACK_DATA_CLEAR,
        p_addr,
        extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE,
        data_type);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &ack_data_clear_res);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return ack_data_clear_res;
}

void nrf_802154_ack_data_remove_all(bool extended, nrf_802154_ack_data_t data_type)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_REMOVE_ALL);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_ACK_DATA_REMOVE_ALL,
        SPINEL_DATATYPE_NRF_802154_ACK_DATA_REMOVE_ALL,
        extended,
        data_type);

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
    NRF_802154_SPINEL_LOG_BUFF(p_addr, extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE);
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_SET,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_SET,
        p_addr,
        extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &addr_set_res);

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
    NRF_802154_SPINEL_LOG_BUFF(p_addr, extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE);
    NRF_802154_SPINEL_LOG_VAR_NAMED("%s", (extended ? "true" : "false"), "extended");

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR,
        SPINEL_DATATYPE_NRF_802154_PENDING_BIT_FOR_ADDR_CLEAR,
        p_addr,
        extended ? EXTENDED_ADDRESS_SIZE : SHORT_ADDRESS_SIZE);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &addr_clr_res);

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

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &channel);
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

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA,
        SPINEL_DATATYPE_NRF_802154_CCA,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &cca_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return cca_result;
}

#if NRF_802154_CARRIER_FUNCTIONS_ENABLED

bool nrf_802154_continuous_carrier(void)
{
    nrf_802154_ser_err_t res;
    bool                 continuous_carrier_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CONTINUOUS_CARRIER);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CONTINUOUS_CARRIER,
        SPINEL_DATATYPE_NRF_802154_CONTINUOUS_CARRIER,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &continuous_carrier_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return continuous_carrier_result;
}

bool nrf_802154_modulated_carrier(const uint8_t * p_data)
{
    nrf_802154_ser_err_t res;
    bool                 modulated_carrier_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_BUFF(p_data, RAW_PAYLOAD_OFFSET + p_data[0]);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_MODULATED_CARRIER);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_MODULATED_CARRIER,
        SPINEL_DATATYPE_NRF_802154_MODULATED_CARRIER,
        p_data,
        RAW_PAYLOAD_OFFSET + p_data[0]);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &modulated_carrier_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return modulated_carrier_result;
}

#endif // NRF_802154_CARRIER_FUNCTIONS_ENABLED

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

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &ed_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return ed_result;
}

#if NRF_802154_CSMA_CA_ENABLED

bool nrf_802154_transmit_csma_ca_raw(uint8_t                                      * p_data,
                                     const nrf_802154_transmit_csma_ca_metadata_t * p_metadata)
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

    if (p_metadata == NULL)
    {
        static const nrf_802154_transmit_csma_ca_metadata_t metadata_default =
        {
            .frame_props = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
            .tx_power    = {.use_metadata_value = false},
            .tx_channel  = {.use_metadata_value = false}
        };

        p_metadata = &metadata_default;
    }

    SERIALIZATION_ERROR_IF(!handle_added, NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_CSMA_CA_RAW,
        NRF_802154_TRANSMIT_CSMA_CA_METADATA_ENCODE(*p_metadata),
        NRF_802154_HDATA_ENCODE(data_handle, p_data, p_data[0]));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &transmit_result);

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

bool nrf_802154_csma_ca_min_be_set(uint8_t min_be)
{
    nrf_802154_ser_err_t res;
    bool                 result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", min_be);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_SET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_SET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_SET,
        min_be);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &result);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return result;
}

uint8_t nrf_802154_csma_ca_min_be_get(void)
{
    nrf_802154_ser_err_t res;
    uint8_t              min_be = 0;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MIN_BE_GET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MIN_BE_GET,
        &min_be);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &min_be);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return min_be;
}

bool nrf_802154_csma_ca_max_be_set(uint8_t max_be)
{
    nrf_802154_ser_err_t res;
    bool                 result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", max_be);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_SET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_SET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_SET,
        max_be);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &result);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return result;
}

uint8_t nrf_802154_csma_ca_max_be_get(void)
{
    nrf_802154_ser_err_t res;
    uint8_t              max_be = 0;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BE_GET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BE_GET,
        &max_be);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &max_be);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return max_be;
}

void nrf_802154_csma_ca_max_backoffs_set(uint8_t max_backoffs)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", max_backoffs);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_SET,
        max_backoffs);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

uint8_t nrf_802154_csma_ca_max_backoffs_get(void)
{
    nrf_802154_ser_err_t res;
    uint8_t              max_backoffs = 0;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET,
        SPINEL_DATATYPE_NRF_802154_CSMA_CA_MAX_BACKOFFS_GET,
        &max_backoffs);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &max_backoffs);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return max_backoffs;
}

#endif // NRF_802154_CSMA_CA_ENABLED

#if NRF_802154_TEST_MODES_ENABLED
nrf_802154_test_mode_csmaca_backoff_t nrf_802154_test_mode_csmaca_backoff_get(void)
{
    nrf_802154_ser_err_t                  res;
    nrf_802154_test_mode_csmaca_backoff_t value = 0;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET,
        SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_GET,
        &value);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &value);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return value;
}

void nrf_802154_test_mode_csmaca_backoff_set(nrf_802154_test_mode_csmaca_backoff_t value)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", value);

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET,
        SPINEL_DATATYPE_NRF_802154_TEST_MODE_CSMACA_BACKOFF_SET,
        value);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

#endif // NRF_802154_TEST_MODES_ENABLED

bool nrf_802154_transmit_raw(uint8_t                              * p_data,
                             const nrf_802154_transmit_metadata_t * p_metadata)
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

    if (p_metadata == NULL)
    {
        static const nrf_802154_transmit_metadata_t metadata_default =
        {
            .frame_props = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
            .cca         = true,
            .tx_power    = {.use_metadata_value = false},
            .tx_channel  = {.use_metadata_value = false}
        };

        p_metadata = &metadata_default;
    }

    SERIALIZATION_ERROR_IF(!handle_added, NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW,
        NRF_802154_TRANSMIT_METADATA_ENCODE(*p_metadata),
        NRF_802154_HDATA_ENCODE(data_handle, p_data, p_data[0]));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &transmit_result);

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

bool nrf_802154_transmit_raw_at(uint8_t                                 * p_data,
                                uint64_t                                  tx_time,
                                const nrf_802154_transmit_at_metadata_t * p_metadata)
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

    if (p_metadata == NULL)
    {
        static const nrf_802154_transmit_at_metadata_t metadata_default =
        {
            .frame_props = NRF_802154_TRANSMITTED_FRAME_PROPS_DEFAULT_INIT,
            .cca         = true,
            // channel set to 0 will be replaced on net core by the current channel from PIB
            .channel            = 0,
            .tx_power           = {.use_metadata_value = false},
            .extra_cca_attempts = 0U,
        };

        p_metadata = &metadata_default;
    }

    SERIALIZATION_ERROR_IF(!handle_added, NRF_802154_SERIALIZATION_ERROR_NO_MEMORY, error, bail);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW_AT);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_RAW_AT,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_RAW_AT,
        NRF_802154_TRANSMIT_AT_METADATA_ENCODE(*p_metadata),
        tx_time,
        NRF_802154_HDATA_ENCODE(data_handle, p_data, p_data[0]));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &transmit_result);

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

bool nrf_802154_transmit_at_cancel(void)
{
    nrf_802154_ser_err_t res;
    bool                 cancel_result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_AT_CANCEL);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TRANSMIT_AT_CANCEL,
        SPINEL_DATATYPE_NRF_802154_TRANSMIT_AT_CANCEL,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &cancel_result);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return cancel_result;
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

    bool removed = nrf_802154_buffer_mgr_dst_remove_by_local_pointer(
        nrf_802154_spinel_dst_buffer_mgr_get(),
        p_data);

    SERIALIZATION_ERROR_IF(!removed,
                           NRF_802154_SERIALIZATION_ERROR_INVALID_BUFFER,
                           error,
                           bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

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

uint64_t nrf_802154_time_get(void)
{
    int32_t  res;
    uint64_t time = 0UL;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_TIME_GET,
        SPINEL_DATATYPE_NRF_802154_TIME_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = time_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, &time);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return time;
}

void nrf_802154_cca_cfg_set(const nrf_802154_cca_cfg_t * p_cfg)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", p_cfg->mode, "Mode");
    NRF_802154_SPINEL_LOG_VAR_NAMED("%d", p_cfg->ed_threshold, "ED threshold");
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", p_cfg->corr_threshold, "Corr threshold");
    NRF_802154_SPINEL_LOG_VAR_NAMED("%u", p_cfg->corr_limit, "Corr limit");

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_SET,
        SPINEL_DATATYPE_NRF_802154_CCA_CFG_SET,
        NRF_802154_CCA_CFG_ENCODE(*p_cfg));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_cca_cfg_get(nrf_802154_cca_cfg_t * p_cfg)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CCA_CFG_GET,
        SPINEL_DATATYPE_NRF_802154_CCA_CFG_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = cca_cfg_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, p_cfg);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);
}

void nrf_802154_security_global_frame_counter_set(uint32_t frame_counter)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET,
        SPINEL_DATATYPE_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET,
        frame_counter);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_security_global_frame_counter_set_if_larger(uint32_t frame_counter)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET_IF_LARGER,
        SPINEL_DATATYPE_NRF_802154_SECURITY_GLOBAL_FRAME_COUNTER_SET,
        frame_counter);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

nrf_802154_security_error_t nrf_802154_security_key_store(nrf_802154_key_t * p_key)
{
    nrf_802154_ser_err_t        res;
    nrf_802154_security_error_t err = NRF_802154_SECURITY_ERROR_NONE;;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_STORE);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_STORE,
        SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_STORE,
        NRF_802154_SECURITY_KEY_STORE_ENCODE(*p_key, AES_CCM_KEY_SIZE, KEY_ID_MODE_3_SIZE));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, &err);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return err;
}

nrf_802154_security_error_t nrf_802154_security_key_remove(nrf_802154_key_id_t * p_id)
{
    nrf_802154_ser_err_t        res;
    nrf_802154_security_error_t err = NRF_802154_SECURITY_ERROR_NONE;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE,
        SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_REMOVE,
        NRF_802154_SECURITY_KEY_REMOVE_ENCODE(*p_id, KEY_ID_MODE_3_SIZE));

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT, &err);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return err;
}

void nrf_802154_security_key_remove_all(void)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_SECURITY_KEY_REMOVE_ALL,
        SPINEL_DATATYPE_NRF_802154_SECURITY_KEY_REMOVE_ALL,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_csl_writer_period_set(uint16_t period)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_PERIOD_SET,
        SPINEL_DATATYPE_NRF_802154_CSL_WRITER_PERIOD_SET,
        period);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

void nrf_802154_csl_writer_anchor_time_set(uint64_t anchor_time)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET,
        SPINEL_DATATYPE_NRF_802154_CSL_WRITER_ANCHOR_TIME_SET,
        anchor_time);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

static nrf_802154_ser_err_t stat_timestamps_get_ret_await(
    uint32_t                       timeout,
    nrf_802154_stat_timestamps_t * p_stat_timestamps)
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

    res = nrf_802154_spinel_decode_prop_nrf_802154_stat_timestamps_get_ret(
        p_notify_data->data,
        p_notify_data->data_len,
        p_stat_timestamps);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    NRF_802154_SPINEL_LOG_BANNER_RESPONSE();

bail:
    if (p_notify_data != NULL)
    {
        nrf_802154_spinel_response_notifier_free(p_notify_data);
    }

    return error;
}

void nrf_802154_stat_timestamps_get(nrf_802154_stat_timestamps_t * p_stat_timestamps)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_STAT_TIMESTAMPS_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_STAT_TIMESTAMPS_GET,
        SPINEL_DATATYPE_NRF_802154_STAT_TIMESTAMPS_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = stat_timestamps_get_ret_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                        p_stat_timestamps);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);
}

#if NRF_802154_IFS_ENABLED

nrf_802154_ifs_mode_t nrf_802154_ifs_mode_get(void)
{
    nrf_802154_ser_err_t  res;
    nrf_802154_ifs_mode_t mode = NRF_802154_IFS_MODE_DISABLED;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_GET,
        SPINEL_DATATYPE_NRF_802154_IFS_MODE_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint8_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                           &mode);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return mode;
}

bool nrf_802154_ifs_mode_set(nrf_802154_ifs_mode_t mode)
{
    nrf_802154_ser_err_t res;
    bool                 result = false;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();
    NRF_802154_SPINEL_LOG_VAR("%u", mode);

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_SET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MODE_SET,
        SPINEL_DATATYPE_NRF_802154_IFS_MODE_SET,
        mode);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_bool_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                          &result);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return result;
}

uint16_t nrf_802154_ifs_min_sifs_period_get(void)
{
    nrf_802154_ser_err_t res;
    uint16_t             period = 0U;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_GET,
        SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint16_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                            &period);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return period;
}

void nrf_802154_ifs_min_sifs_period_set(uint16_t period)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_SIFS_PERIOD_SET,
        SPINEL_DATATYPE_NRF_802154_IFS_MIN_SIFS_PERIOD_SET,
        period);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

uint16_t nrf_802154_ifs_min_lifs_period_get(void)
{
    nrf_802154_ser_err_t res;
    uint16_t             period = 0U;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_GET);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_GET,
        SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_GET,
        NULL);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = net_generic_uint16_response_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT,
                                            &period);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return period;
}

void nrf_802154_ifs_min_lifs_period_set(uint16_t period)
{
    nrf_802154_ser_err_t res;

    SERIALIZATION_ERROR_INIT(error);

    NRF_802154_SPINEL_LOG_BANNER_CALLING();

    nrf_802154_spinel_response_notifier_lock_before_request(SPINEL_PROP_LAST_STATUS);

    res = nrf_802154_spinel_send_cmd_prop_value_set(
        SPINEL_PROP_VENDOR_NORDIC_NRF_802154_IFS_MIN_LIFS_PERIOD_SET,
        SPINEL_DATATYPE_NRF_802154_IFS_MIN_LIFS_PERIOD_SET,
        period);

    SERIALIZATION_ERROR_CHECK(res, error, bail);

    res = status_ok_await(CONFIG_NRF_802154_SER_DEFAULT_RESPONSE_TIMEOUT);
    SERIALIZATION_ERROR_CHECK(res, error, bail);

bail:
    SERIALIZATION_ERROR_RAISE_IF_FAILED(error);

    return;
}

#endif // NRF_802154_IFS_ENABLED
