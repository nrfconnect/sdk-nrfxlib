/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
 * www.dsr-zboss.com
 * www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: ZCL Poll Control cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2078

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL)

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zb_aps.h"
#include "zb_watchdog.h"
#include "zcl/zb_zcl_poll_control.h"
#include "zdo_wwah_stubs.h"
/** @internal
    @{
*/

zb_uint8_t gs_poll_control_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL_SERVER_ROLE_GENERATED_CMD_LIST
};

zb_uint8_t gs_poll_control_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_poll_control_server_cmd_list =
{
  sizeof(gs_poll_control_server_received_commands), gs_poll_control_server_received_commands,
  sizeof(gs_poll_control_server_generated_commands), gs_poll_control_server_generated_commands
};

zb_ret_t check_value_poll_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
static void zb_zcl_poll_control_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);

zb_bool_t zb_zcl_process_poll_control_specific_commands_srv(zb_uint8_t param);

void zb_zcl_poll_control_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_poll_control_server,
                              zb_zcl_poll_control_write_attr_hook_server,
                              zb_zcl_process_poll_control_specific_commands_srv);
}

zb_ret_t check_value_poll_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_bool_t ret = ZB_TRUE;
  zb_ret_t ret_val = RET_ERROR;
  zb_zcl_attr_t *attr_desc;
  zb_uint32_t value32 = 0;
  zb_uint16_t value16 = 0;
  ZB_ASSIGN_UINT32(&value32, value);
  ZB_ASSIGN_UINT16(&value16, value);

  TRACE_MSG(TRACE_ZCL1, "> check_value_poll_control_server", (FMT__0));

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID:
      if (ZB_ZCL_POLL_CONTROL_CHECKIN_INTERVAL_NO_CHECK_IN_VALUE == value32)
      {
        TRACE_MSG(TRACE_ZCL2, "no Check-in value", (FMT__0));
        ret = ZB_TRUE;
      }
      else
      {
        ret = (value32 <= ZB_ZCL_POLL_CONTROL_CHECKIN_INTERVAL_MAX_VALUE)
          ? ZB_TRUE : ZB_FALSE;

        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_MIN_CHECKIN_INTERVAL_ID);
        if ( attr_desc != NULL )
        {
          zb_uint32_t min_value = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
          TRACE_MSG(TRACE_ZCL2, "min val %ld", (FMT__L, min_value));
          ret &= value32 >= min_value;
        }

        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID);
        if ( attr_desc != NULL )
        {
          zb_uint32_t long_poll = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
          TRACE_MSG(TRACE_ZCL2, "long_poll %ld", (FMT__L, long_poll));
          ret &= value32 >= long_poll;
        }
      }
      break;

    case ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID:
      ret = (ZB_ZCL_POLL_CONTROL_LONG_POLL_INTERVAL_MIN_VALUE <= value32) &&
            (value32 <= ZB_ZCL_POLL_CONTROL_LONG_POLL_INTERVAL_MAX_VALUE)
              ? ZB_TRUE : ZB_FALSE;

      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_MIN_INTERVAL_ID);
      if ( attr_desc != NULL )
      {
        zb_uint32_t poll_min = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
        TRACE_MSG(TRACE_ZCL2, "min val %ld", (FMT__L, poll_min));
        ret &= poll_min <= value32;
      }

      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID);
      if ( attr_desc != NULL )
      {
        zb_uint32_t check_in_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
        TRACE_MSG(TRACE_ZCL2, "check_in_interval %ld", (FMT__L, check_in_interval));
        ret &= value32 <= check_in_interval;
      }

      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID);
      if ( attr_desc != NULL )
      {
        zb_uint16_t short_poll_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
        TRACE_MSG(TRACE_ZCL2, "short_poll_interval %ld", (FMT__L, short_poll_interval));
        ret &= short_poll_interval <= value32;
      }

      break;

    case ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID:
      ret = (ZB_ZCL_POLL_CONTROL_SHORT_POLL_INTERVAL_MIN_VALUE <= value16)
        ? ZB_TRUE : ZB_FALSE;

      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID);
      if ( attr_desc != NULL )
      {
        zb_uint32_t long_poll = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
        TRACE_MSG(TRACE_ZCL2, "long_poll %ld", (FMT__L, long_poll));
        ret &= value16 <= long_poll;
      }
      break;

    case ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_TIMEOUT_ID:
      ret = (ZB_ZCL_POLL_CONTROL_FAST_POLL_TIMEOUT_MIN_VALUE <= value16)
        ? ZB_TRUE : ZB_FALSE;
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_MAX_TIMEOUT_ID);
      if ( attr_desc != NULL )
      {
        zb_uint16_t poll_max = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
        TRACE_MSG(TRACE_ZCL2, "val %d min val %d max val %d", (FMT__D_D_D,
            value16, ZB_ZCL_POLL_CONTROL_FAST_POLL_TIMEOUT_MIN_VALUE, poll_max));
        /* According to CCB#1815: if FastPollTimeout > FastPollTimeoutMax server
         * should return INVALID_VALUE.
         * See url: old.Zigbee.org/en/members_area/CCB/list.asp?&NoLimit=true&NoLimit=true
         * for more details.
         */
        ret &= value16 <= poll_max;
      }
      TRACE_MSG(TRACE_ZCL2, "val %d", (FMT__D, value16));
      break;

    default:
      break;
  }

  if (ret)
  {
    ret_val = RET_OK;
  }
  TRACE_MSG(TRACE_ZCL1, "< check_value_poll_control_server ret %hd", (FMT__H, ret_val));
  return ret_val;
}

#if defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN
#define ZB_CHECK_IN_NO_RESPONSE_INTERVAL_MS 7680
#define ZB_CHECK_IN_NO_RESPONSE_INTERVAL    ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_CHECK_IN_NO_RESPONSE_INTERVAL_MS)

// for devices with Poll control cluster and without OTA cluster
#ifndef ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL
#define ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL            0x00
#endif


void zb_zcl_poll_control_check_in_non_response(zb_uint8_t endpoint);

static void poll_control_check_binding(zb_bufid_t param,
                                       zb_uint8_t src_endpoint,
                                       zb_callback_t cb);

void zb_zcl_poll_controll_register_cb(zb_callback_t cb)
{
  TRACE_MSG(TRACE_ZCL1, "zb_zcl_poll_controll_register_cb cb %p", (FMT__P, cb));

  ZCL_CTX().checkin_cb = cb;
}


/** @brief Restart check-in cycle with passed interval
 * @param param - buffer to be used, if 0, buffer will be acquired when needed
 * @param new_checkin_interval_qsec - new checkin interval (in quarterseconds) to be applied */
static void zb_zcl_poll_control_start_internal(zb_uint8_t param, zb_uint32_t new_checkin_interval_qsec)
{
  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_poll_control_start_internal param %hx new_interval %li",
      (FMT__H_L, param, new_checkin_interval_qsec));
#ifdef ZB_USE_CHECKIN_WATCHDOG
  {
    zb_uint32_t checkin_wd_interval = ZB_QUARTERECONDS_TO_BEACON_INTERVAL(new_checkin_interval_qsec)*3;
    if (checkin_wd_interval < ZB_WD_CHECKIN_MIN_TO)
    {
      checkin_wd_interval = ZB_WD_CHECKIN_MIN_TO;
    }
    zb_add_watchdog(ZB_WD_CHECKIN, checkin_wd_interval);
  }
#endif

  if (param != 0U)
  {
    zb_buf_free(param);
  }

  /* Param is not reused here to prevent buffer blocking for pretty long check-in interval */
  ZB_SCHEDULE_ALARM(zb_zcl_poll_control_start_check_in, 0, ZB_QUARTERECONDS_TO_BEACON_INTERVAL(new_checkin_interval_qsec));
  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_poll_control_start_internal", (FMT__0));
}


/** @brief Start check-ins cycle with interval taken from Poll Control Cluster
 * @param param - buffer to be used for check-in command
 * @param endpoint - endpoint on which check-ins should be started */
void zb_zcl_poll_control_start(zb_uint8_t param, zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_start param %hx endpoint %hx", (FMT__H_H, param, endpoint));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID);
  ZB_ASSERT(attr_desc);

  zb_zcl_poll_control_start_internal(param, ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc));
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_start", (FMT__0));
}


/** @brief Stop check-ins */
zb_uint8_t zb_zcl_poll_control_stop(void)
{
  zb_uint8_t canceled_param = 0;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_stop", (FMT__0));
  ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(
      zb_zcl_poll_control_start_check_in, ZB_ALARM_ANY_PARAM, &canceled_param);

#ifdef ZB_USE_CHECKIN_WATCHDOG
  zb_stop_watchdog(ZB_WD_CHECKIN);
#endif

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_stop", (FMT__0));
  return canceled_param;
}

/** @brief Hook on Write Check-in Interval attribute
 * send Check-in if change Check-in interval & Check-in remain time > new check-in interval */
void write_attr_check_in_interval_hook(zb_uint8_t endpoint, zb_uint8_t *new_value_ptr)
{
  zb_time_t time_alarm;
  zb_time_t new_interval;
  zb_uint32_t new_val;
  zb_uint8_t canceled_param = 0;
  zb_ret_t ret;

  ZVUNUSED(endpoint);

  TRACE_MSG(TRACE_ZCL1, "> write_attr_check_in_interval_hook endpoint %hx",
      (FMT__H, endpoint));

  ZB_MEMCPY(&new_val, new_value_ptr, sizeof(zb_uint32_t));
  new_interval = ZB_QUARTERECONDS_TO_BEACON_INTERVAL(new_val);
  ZVUNUSED(new_interval);

  ret = ZB_SCHEDULE_GET_ALARM_TIME(zb_zcl_poll_control_start_check_in, ZB_ALARM_ANY_PARAM, &time_alarm);

  if (ret == RET_OK)
  {
    /* HA specification changed: re-start check-in unconditionally */

    canceled_param = zb_zcl_poll_control_stop();
    TRACE_MSG(TRACE_APP1, "new check in value %ld", (FMT__L, new_val));
    /* If zero check-in interval is set - stop polling */
    if (new_val != ZB_ZCL_POLL_CONTROL_CHECKIN_INTERVAL_NO_CHECK_IN_VALUE)
    {
      /* re-use buffer specified by canceled_param */
      TRACE_MSG(TRACE_APP2, "call start_check_in with new interval", (FMT__0));
      zb_zcl_poll_control_start_internal(canceled_param, new_val);
    }
    else
    {
      /* free buffer - it is not re-used */
      TRACE_MSG(TRACE_APP2, "skip rescheduling check-in", (FMT__0));
      if (canceled_param)
      {
        TRACE_MSG(TRACE_APP2, "free canceled buffer %hd", (FMT__H, canceled_param));
        zb_buf_free(canceled_param);
      }
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "cant find alarm, skip", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "< write_attr_check_in_interval_hook", (FMT__0));
}


static void poll_control_send_default_response(zb_bufid_t param,
                                               zb_zcl_parsed_hdr_t *cmd_info,
                                               zb_zcl_status_t status)
{
  if (cmd_info->disable_default_response && status == ZB_ZCL_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL2, "Default response disabled", (FMT__0));
    zb_buf_free(param);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "Send DefaultResponse, status %xd", (FMT__D, status));
    ZB_ZCL_SEND_DEFAULT_RESP(param,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
                             ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                             cmd_info->profile_id,
                             ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                             cmd_info->seq_number,
                             cmd_info->cmd_id,
                             status);
  }
}


static zb_size_t get_aligned_size(zb_size_t size)
{
  return ((size + ZB_BUF_ALLOC_ALIGN - 1) / ZB_BUF_ALLOC_ALIGN) * ZB_BUF_ALLOC_ALIGN;
}

static void check_in_res_handler_pack_params(zb_bufid_t param,
                                             zb_zcl_poll_control_check_in_res_t *payload,
                                             zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_size_t payload_size_aligned = get_aligned_size(sizeof(*payload));
  zb_size_t params_size = payload_size_aligned + sizeof(*cmd_info);
  zb_uint8_t *ptr = NULL;

  ptr = zb_buf_initial_alloc(param, params_size);

  ZB_MEMCPY(ptr, payload, sizeof(*payload));
  ptr += payload_size_aligned;

  ZB_MEMCPY(ptr, cmd_info, sizeof(*cmd_info));
}


static void check_in_res_handler_unpack_params(zb_bufid_t param,
                                               zb_zcl_poll_control_check_in_res_t *payload,
                                               zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_size_t payload_size_aligned = get_aligned_size(sizeof(*payload));
  zb_uint8_t *ptr = NULL;

  ptr = zb_buf_begin(param);

  ZB_MEMCPY(payload, ptr, sizeof(*payload));
  ptr += payload_size_aligned;

  ZB_MEMCPY(cmd_info, ptr, sizeof(*cmd_info));
}


static void check_in_res_handler_check_binding_response_cb(zb_bufid_t param)
{
  zb_zcl_poll_control_check_in_res_t payload;
  zb_zcl_parsed_hdr_t cmd_info = { 0 };
  zb_aps_check_binding_resp_t *check_binding_resp = NULL;
  zb_zcl_status_t status = ZB_ZCL_STATUS_SUCCESS;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL2, ">> check_in_res_handler_check_binding_response_cb, param %d",
            (FMT__D, param));

  check_binding_resp = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_resp_t);

  /*
    ZCL8: If the Poll Control Server receives a Check-In Response from a client
    for which there is no binding (unbound), it SHOULD respond with a Default Response
    with a status value indicating FAILURE.
  */
  if (!check_binding_resp->exists)
  {
    status = (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                ZB_ZCL_STATUS_FAIL : ZB_ZCL_STATUS_ACTION_DENIED;
  }

  if (status == ZB_ZCL_STATUS_SUCCESS)
  {
    check_in_res_handler_unpack_params(param, &payload, &cmd_info);
    endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
  }

  /*
    ZCL8: If the Poll Control Server receives a Check-In Response from a bound client
    after temporary fast poll mode is completed it SHOULD respond with a Default Response
    with a status value indicating FAILURE.
   */
  if (status == ZB_ZCL_STATUS_SUCCESS)
  {
    zb_time_t tmp;
    zb_ret_t ret;

    ret = ZB_SCHEDULE_GET_ALARM_TIME(zb_zcl_poll_control_check_in_non_response, endpoint, &tmp);

    if (ret != RET_OK)
    {
      status = ZB_ZCL_STATUS_FAIL;
    }
  }

  if (status == ZB_ZCL_STATUS_SUCCESS)
  {
    /* Cancel default Check-in loop (7.68 sec) */
    /*
       [AV] This doesn't cancel default Check-in loop; next check-in stays scheduled!
       FIXME: cancel start_check_in here?
    */
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_poll_control_check_in_non_response, endpoint);

    if(payload.is_start)
    {
      zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
         ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_TIMEOUT_ID);
      zb_time_t new_interval = ZB_QUARTERECONDS_TO_MSEC(payload.timeout);

      if (!new_interval)
      {
         ZB_ASSERT(attr_desc);
         new_interval = ZB_QUARTERECONDS_TO_MSEC(ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc));
      }
      zb_zdo_pim_stop_fast_poll(0);
      zb_zdo_pim_set_fast_poll_timeout(new_interval);
      zb_zdo_pim_start_fast_poll(0);
    }
    else
    {
      zb_zdo_pim_stop_fast_poll(0);
    }

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
    zb_zcl_wwah_bad_parent_recovery_signal(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_OK);
#endif
  }

  poll_control_send_default_response(param, &cmd_info, status);

  TRACE_MSG(TRACE_ZCL2, "<< check_in_res_handler_check_binding_response_cb", (FMT__0));
}


/** @brief Check-in response command */
static zb_ret_t check_in_res_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_poll_control_check_in_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;
  /* zb_zcl_poll_control_server_status_t *server_data; */

  TRACE_MSG(TRACE_ZCL1, "> check_in_res_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_POLL_CONTROL_GET_CHECK_IN_RES(&payload, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_POLL_CONTROL_GET_CHECK_IN_RES",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }

  /*
    ZCL8: If the Poll Control Server receives a Check-In Response from a client
    for which there is no binding (unbound), it SHOULD respond with a Default Response
    with a status value indicating FAILURE.
  */
  if (ret == RET_OK)
  {
    check_in_res_handler_pack_params(param, &payload, &cmd_info);
    poll_control_check_binding(param,
                               endpoint,
                               check_in_res_handler_check_binding_response_cb);
    ret = RET_BUSY;
  }


  TRACE_MSG(TRACE_ZCL1, "< check_in_res_handler ret %hx", (FMT__H, ret));
  return ret;
}


static void fast_poll_stop_handler_send_default_response(zb_uint8_t param)
{
  /* ZCL8: Table 2-12. Enumerated Command Status Values:
   * ACTION_DENIED is DEPRECATED: use FAILURE
   */
  zb_zcl_status_t status = (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                ZB_ZCL_STATUS_FAIL : ZB_ZCL_STATUS_ACTION_DENIED;
  zb_zdo_pim_stop_fast_poll_extended_resp_t *resp = ZB_BUF_GET_PARAM(param, zb_zdo_pim_stop_fast_poll_extended_resp_t);
  zb_zcl_parsed_hdr_t *cmd_info = NULL;

  if (resp->stop_result == ZB_ZDO_PIM_STOP_FAST_POLL_RESULT_STOPPED)
  {
    status = ZB_ZCL_STATUS_SUCCESS;
  }

  cmd_info = (zb_zcl_parsed_hdr_t*)zb_buf_begin(param);
  poll_control_send_default_response(param, cmd_info, status);
}


static void fast_poll_stop_handler_check_binding_response_cb(zb_bufid_t param)
{
  zb_aps_check_binding_resp_t *check_binding_resp = NULL;

  TRACE_MSG(TRACE_ZCL2, ">> fast_poll_stop_handler_check_binding_response_cb, param %d",
            (FMT__D, param));

  check_binding_resp = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_resp_t);

  if (check_binding_resp->exists)
  {
    /*
      ZCL8: If the Poll Control Server is unable to stop fast
      polling due to the fact that there is another bound client
      which has requested that polling continue
      it SHOULD respond with a Default Response with a status value
      indicating FAILURE
    */
    /*
      So call stop_fast_poll anyway to decrement internal counter
      and then recheck if we are still in fast poll and send
      FAILURE in such case
    */

    zb_zdo_pim_stop_fast_poll_extended_req(param, fast_poll_stop_handler_send_default_response);
  }
  else
  {
    zb_zcl_parsed_hdr_t *cmd_info = NULL;

    cmd_info = (zb_zcl_parsed_hdr_t*)zb_buf_begin(param);
    /* ZCL8: Table 2-12. Enumerated Command Status Values:
     * ACTION_DENIED is DEPRECATED: use FAILURE
     */
    poll_control_send_default_response(param, cmd_info, (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                                       ZB_ZCL_STATUS_FAIL : ZB_ZCL_STATUS_ACTION_DENIED);
  }

  TRACE_MSG(TRACE_ZCL2, "<< fast_poll_stop_handler_check_binding_response_cb", (FMT__0));
}


/** @brief fast_poll_stop command */
static zb_ret_t fast_poll_stop_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = NULL;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> fast_poll_stop_handler %hx", (FMT__H, param));

  /* copy cmd_info so we can use it to send a response later */
  cmd_info = zb_buf_initial_alloc(param, sizeof(zb_zcl_parsed_hdr_t));
  ZB_MEMCPY(cmd_info,
            ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t),
            sizeof(zb_zcl_parsed_hdr_t));

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;

  TRACE_MSG(TRACE_ZCL2, "call poll_control_check_binding", (FMT__0));
  poll_control_check_binding(param, endpoint, fast_poll_stop_handler_check_binding_response_cb);

  TRACE_MSG(TRACE_ZCL1, "< fast_poll_stop_handler", (FMT__0));

  return RET_BUSY;
}

/** @brief Set Long Poll Interval command */
static zb_ret_t set_long_poll_interval_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_poll_control_set_long_poll_interval_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> set_long_poll_interval_handler param %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_POLL_CONTROL_GET_SET_LONG_POLL_INTERVAL_REQ(&payload, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_POLL_CONTROL_GET_SET_LONG_POLL_INTERVAL_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if( /*!*/zb_zcl_check_attr_value(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, endpoint,
      ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID, (zb_uint8_t*)(&payload.interval)) == RET_ERROR)
  {
    /* TODO: DO NOT send Default response here - do it in the
     * zb_zcl_process_poll_control_specific_commands() */
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
    TRACE_MSG(TRACE_ZCL1, "set RET_BUSY", (FMT__0));
  }
  else
  {
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    TRACE_MSG(TRACE_ZCL2, "payload.interval %hx", (FMT__L, payload.interval));
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, payload.interval);

    zb_zdo_pim_set_long_poll_interval(ZB_QUARTERECONDS_TO_MSEC(payload.interval));
#ifdef ZB_USE_NVRAM
    ZB_SCHEDULE_CALLBACK(zb_zcl_poll_control_save_nvram, 0);
#endif
  }

  TRACE_MSG(TRACE_ZCL1, "< set_long_poll_interval_handler ret %hx", (FMT__H, ret));
  return ret;
}

/** @brief Set Short Poll Interval command */
static zb_ret_t set_short_poll_interval_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_poll_control_set_short_poll_interval_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> set_short_poll_interval_handler param %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;


  ZB_ZCL_POLL_CONTROL_GET_SET_SHORT_POLL_INTERVAL_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_POLL_CONTROL_GET_SET_SHORT_POLL_INTERVAL_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if( /*!*/zb_zcl_check_attr_value(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, endpoint,
      ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID, (zb_uint8_t*)(&payload.interval)) == RET_ERROR)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID);
    ZB_ASSERT(attr_desc);
    TRACE_MSG(TRACE_ZCL2, "payload.interval %hx", (FMT__D, payload.interval));
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, payload.interval);

    zb_zdo_pim_set_fast_poll_interval(ZB_QUARTERECONDS_TO_MSEC(payload.interval));
#ifdef ZB_USE_NVRAM
    ZB_SCHEDULE_CALLBACK(zb_zcl_poll_control_save_nvram, 0);
#endif
  }

  TRACE_MSG(TRACE_ZCL1, "< set_short_poll_interval_handler ret %hx", (FMT__H, ret));
  return ret;
}

/* specific commands handling - server side */
zb_bool_t zb_zcl_process_poll_control_specific_commands_srv(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;
  zb_zcl_status_t resp_code;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_poll_control_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_poll_control_specific_commands_srv: param %hd, cmd %hd",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_POLL_CONTROL == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch( cmd_info.cmd_id )
  {
    case ZB_ZCL_CMD_POLL_CONTROL_CHECK_IN_RESPONSE_ID:
      status = check_in_res_handler(param);
      break;

    case ZB_ZCL_CMD_POLL_CONTROL_FAST_POLL_STOP_ID:
      status = fast_poll_stop_handler(param);
      break;

    case ZB_ZCL_CMD_POLL_CONTROL_SET_LONG_POLL_INTERVAL_ID:
      status = set_long_poll_interval_handler(param);
      break;

    case ZB_ZCL_CMD_POLL_CONTROL_SET_SHORT_POLL_INTERVAL_ID:
      status = set_short_poll_interval_handler(param);
      break;

    default:
      processed = ZB_FALSE;
      break;
  }

  if (processed && status != RET_BUSY)
  {
    switch (status)
    {
      case RET_OK:
        resp_code = ZB_ZCL_STATUS_SUCCESS;
        break;

      /* ZCL8: Table 2-12. Enumerated Command Status Values:
       * ACTION_DENIED is DEPRECATED: use FAILURE
       */
      case RET_INVALID_STATE:
        resp_code = (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                    ZB_ZCL_STATUS_FAIL : ZB_ZCL_STATUS_ACTION_DENIED;
        break;

      case RET_TIMEOUT:
        resp_code = ZB_ZCL_STATUS_TIMEOUT;
        break;

      default:
        resp_code = ZB_ZCL_STATUS_INVALID_FIELD;
        break;
    }

    poll_control_send_default_response(param, &cmd_info, resp_code);
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_poll_control_specific_commands_srv: processed %d",
             (FMT__D, processed));
  return processed;
}


/** @brief Run Check-in command if Check in Responce not receive
 * */
void zb_zcl_poll_control_check_in_non_response(zb_uint8_t endpoint)
{
  ZVUNUSED(endpoint);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_check_in_non_response endpoint %hd", (FMT__H, endpoint));

  /* MA: return to Long Poll */
  zb_zdo_pim_stop_fast_poll(0);

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
  zb_zcl_wwah_bad_parent_recovery_signal(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED);
#endif

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_check_in_non_response", (FMT__0));
}


void zcl_poll_control_check_in_send_cb(zb_uint8_t param)
{
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc;
  zb_zcl_poll_control_srv_cfg_data_t *addr_data;
  zb_zcl_command_send_status_t *cmd_send_status = ZB_BUF_GET_PARAM(param, zb_zcl_command_send_status_t);


  TRACE_MSG(TRACE_ZCL1, "> zcl_poll_control_check_in_send_cb", (FMT__0));

  TRACE_MSG(TRACE_ZCL1, "check-in cmd send status %hd, param %hd",
            (FMT__H_H, cmd_send_status->status, param));

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_ADDR_DATA_ID);
  ZB_ASSERT(attr_desc);
  addr_data = (zb_zcl_poll_control_srv_cfg_data_t*)attr_desc->data_p;

  if (cmd_send_status->status == 0)
  {
    /* Restart only in case of successful APS ACK reception */
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_poll_control_check_in_non_response, endpoint);
    /* Don't increment pkt number, it's been already done while
     * sending - only move deadline */
    TRACE_MSG(TRACE_ZDO3, "Call zb_zdo_pim_start_turbo_poll_packets 0", (FMT__0));
    zb_zdo_pim_start_turbo_poll_packets(0);
    ZB_SCHEDULE_ALARM(zb_zcl_poll_control_check_in_non_response, endpoint, ZB_CHECK_IN_NO_RESPONSE_INTERVAL);
  }

  /*
    sending_cmd cases:
    sending_cmd == 0: no command is sending now
    sending_cmd == 1: command sending is in progress
    sending_cmd  > 1: check-in timeout expired while trying to send
        check-in command or somehow we were trying to send the next
        check-in commang
  */
  TRACE_MSG(TRACE_ZCL1, "sending_cmd %hd", (FMT__H, addr_data->sending_cmd));
  if (addr_data->sending_cmd > 1)
  {
    ZB_SCHEDULE_CALLBACK(zb_zcl_poll_control_start_check_in, param);
  }
  else
  {
    zb_buf_free(param);
  }
  addr_data->sending_cmd = 0;

  TRACE_MSG(TRACE_ZCL1, "< zcl_poll_control_check_in_send_cb", (FMT__0));
}


static void check_in_handle_check_binding_confirm(zb_bufid_t param)
{
  zb_aps_check_binding_resp_t *check_binding_resp = NULL;
  zb_uint8_t endpoint;
  zb_uint32_t checkin_interval;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZDO2, ">> check_in_handle_check_binding_confirm, param %d", (FMT__D, param));

  check_binding_resp = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_resp_t);

  /* FIXME: potential problem in the case of multi EP device */
  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
  ZB_ASSERT(endpoint);

  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint, ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID);
  ZB_ASSERT(attr_desc);

  checkin_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
  TRACE_MSG(TRACE_ZCL2, "checkin_interval %hd", (FMT__H, checkin_interval));

  if (check_binding_resp->exists)
  {
    zb_zcl_poll_control_srv_cfg_data_t *addr_data;

    zb_uint16_t dst_addr = 0;
    zb_uint8_t dst_ep = 0;
    zb_uint8_t addr_mode;

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_POLL_CONTROL_ADDR_DATA_ID);
    ZB_ASSERT(attr_desc);

    addr_data = (zb_zcl_poll_control_srv_cfg_data_t*)attr_desc->data_p;
    TRACE_MSG(TRACE_ZCL2, "poll addr %d, poll ep %hd", (FMT__D_H, addr_data->poll_addr, addr_data->poll_ep));

    if (addr_data->poll_addr == ZB_ZCL_POLL_CTRL_INVALID_ADDR && addr_data->poll_ep == ZB_ZCL_POLL_INVALID_EP)
    {
      TRACE_MSG(TRACE_ZCL2, "sending using binding", (FMT__0));
      addr_mode = ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL2, "sending using short addr", (FMT__0));
      dst_addr = addr_data->poll_addr;
      dst_ep = addr_data->poll_ep;
      addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    }

    ZB_ZCL_POLL_CONTROL_SEND_CHECK_IN_REQ(
      param,
      dst_addr, addr_mode, dst_ep,
      endpoint, ZB_AF_HA_PROFILE_ID,
      zcl_poll_control_check_in_send_cb);

    /* Set a flag - we are sending checkin command */
    addr_data->sending_cmd = 1;

    ZB_SCHEDULE_ALARM(zb_zcl_poll_control_check_in_non_response, endpoint, ZB_CHECK_IN_NO_RESPONSE_INTERVAL);

    zb_zdo_pim_set_fast_poll_timeout(ZB_CHECK_IN_NO_RESPONSE_INTERVAL_MS);
    zb_zdo_pim_start_fast_poll(0);

#ifdef ZB_USE_CHECKIN_WATCHDOG
    ZB_KICK_WATCHDOG(ZB_WD_CHECKIN);
#endif

    /* Go fast polling until check-in response comes or timeout fires
     * ZB_CHECK_IN_NO_RESPONSE_INTERVAL */

    if (ZCL_CTX().checkin_cb != NULL)
    {
      /* Call user callback during checking-in */
      ZB_SCHEDULE_CALLBACK(ZCL_CTX().checkin_cb, 0);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "skip and reschedule check in - no binding", (FMT__0));

    /* param is not reused intentionally to prevent buffer blocking
     * during whole check-in alarm interval */
    zb_buf_free(param);
  }

  /* Schedule next Check-in command */
  ZB_SCHEDULE_ALARM(zb_zcl_poll_control_start_check_in, 0, ZB_QUARTERECONDS_TO_BEACON_INTERVAL(checkin_interval));

  TRACE_MSG(TRACE_ZDO2, "<< check_in_handle_check_binding_confirm", (FMT__0));
}


static void poll_control_check_binding(zb_bufid_t param,
                                       zb_uint8_t src_endpoint,
                                       zb_callback_t cb)
{
  zb_aps_check_binding_req_t *check_binding_req = NULL;
  TRACE_MSG(TRACE_ZCL2, ">> poll_control_check_binding, param %d, src ep %d, cb %p",
            (FMT__D_D_P, param, src_endpoint, cb));

  check_binding_req = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_req_t);
  ZB_BZERO(check_binding_req, sizeof(*check_binding_req));

  check_binding_req->src_endpoint = src_endpoint;
  check_binding_req->cluster_id = ZB_ZCL_CLUSTER_ID_POLL_CONTROL;
  check_binding_req->response_cb = cb;

  zb_aps_check_binding_request(param);

  TRACE_MSG(TRACE_ZCL2, "<< poll_control_check_binding", (FMT__0));
}


/** @brief Start Check-in command
 * Get buffer
 * Get endpoint for Poll Control Cluster
 * Send Check-in command
 * Schedule default next Check-in (7.68 sec)
 * */
void zb_zcl_poll_control_start_check_in(zb_uint8_t param)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t endpoint;
  zb_uint32_t checkin_interval;
  zb_zcl_poll_control_srv_cfg_data_t *addr_data;
#ifdef ZB_ZCL_NO_CHECKINS_DURING_OTA
  zb_uint8_t ota_status = ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL;
#endif
  zb_bool_t skip_check_in = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_start_check_in, param %hd", (FMT__H, param));

  /* FIXME: potential problem in the case of multi EP device */
  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
  ZB_ASSERT(endpoint);

  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint, ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID);
  ZB_ASSERT(attr_desc);

  checkin_interval = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
  TRACE_MSG(TRACE_ZCL1, "checkin_interval %hd", (FMT__H, checkin_interval));

  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_POLL_CONTROL_ADDR_DATA_ID);
  ZB_ASSERT(attr_desc);
  addr_data = (zb_zcl_poll_control_srv_cfg_data_t*)attr_desc->data_p;

#ifdef ZB_ZCL_NO_CHECKINS_DURING_OTA
#ifdef ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE
  {
    zb_uint8_t endpoint_ota;

    endpoint_ota = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE);
    if (endpoint_ota != 0)
    {
      /* OTA link problem! Forcing OTA code link always */
      ota_status = zb_zcl_ota_upgrade_get_ota_status(endpoint_ota);
    }
    else
    {
      ota_status = ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL;
    }
  }
#endif

  TRACE_MSG(TRACE_ZCL1, "ota_status %hd checkin_interval %hd",
            (FMT__H_H, ota_status, checkin_interval));
#endif

  if (checkin_interval == ZB_ZCL_POLL_CONTROL_CHECKIN_INTERVAL_NO_CHECK_IN_VALUE)
  {
    TRACE_MSG(TRACE_ZCL1, "skip check in - zero interval", (FMT__0));
    skip_check_in = ZB_TRUE;
  }
#ifdef ZB_ZCL_NO_CHECKINS_DURING_OTA
  else if (ota_status != ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
  {
    TRACE_MSG(TRACE_ZCL1, "skip check in - OTA is in progress", (FMT__0));
    skip_check_in = ZB_TRUE;
    /* reschedule the next check-in iteration, otherwise it will stop */
    ZB_SCHEDULE_ALARM(zb_zcl_poll_control_start_check_in, 0,
                      ZB_QUARTERECONDS_TO_BEACON_INTERVAL(checkin_interval));
  }
#endif
  else
  {
    TRACE_MSG(TRACE_ZCL1, "continue... sending_cmd %hd", (FMT__H, addr_data->sending_cmd));
    /* check flag, it is non-zero if check-in command is being sent now */
    if (addr_data->sending_cmd > 0)
    {
      TRACE_MSG(TRACE_ZCL1, "Currently sending Check-in cmd, skip", (FMT__0));
      if (addr_data->sending_cmd < 2)
      {
        addr_data->sending_cmd++;
      }
      skip_check_in = ZB_TRUE;
    }
  }

  if (skip_check_in == ZB_FALSE && param == 0)
  {
    TRACE_MSG(TRACE_ZCL2, "will call zb_get_out_buf_delayed", (FMT__0));
    zb_buf_get_out_delayed(zb_zcl_poll_control_start_check_in);
  }
  else if (skip_check_in == ZB_FALSE)
  {
    TRACE_MSG(TRACE_ZCL2, "call poll_control_check_binding", (FMT__0));
    poll_control_check_binding(param,
                               ZB_ZCL_BROADCAST_ENDPOINT,
                               check_in_handle_check_binding_confirm);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "skip check in...", (FMT__0));
    if (param != 0)
    {
      TRACE_MSG(TRACE_ZCL2, "free buf %hd", (FMT__H, param));
      zb_buf_free(param);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_start_check_in", (FMT__0));
}

#endif /* defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN */


/** @brief Save NVRAM by write attribute
 */
#ifdef ZB_USE_NVRAM
void zb_zcl_poll_control_save_nvram(zb_uint8_t param)
{
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_save_nvram", (FMT__0));
  /* If we fail, trace is given and assertion is triggered */
  (void)zb_nvram_write_dataset(ZB_NVRAM_HA_POLL_CONTROL_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_save_nvram", (FMT__0));
}
#endif /* ZB_USE_NVRAM */

/** @brief Hook on Write attribute
 * send Check-in if change Check-in interval & Check-in remain time > new check-in interval */
static void zb_zcl_poll_control_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  zb_uint32_t new_val = 0;
/* 07/08/2019 EE CR:MAJOR LE/BE conversion? Is there a problem of local/remote attr write? */
  ZB_MEMCPY(&new_val, new_value, sizeof(zb_uint32_t));

  (void)endpoint;
  (void)attr_id;
  (void)new_value;
  (void)manuf_code;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_write_attr_hook endpoint %hx attr_id %d",
      (FMT__H_D, endpoint, attr_id));

#if defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN
  if(attr_id==ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID)
  {
    write_attr_check_in_interval_hook(endpoint, new_value);
  }
  else if (attr_id==ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID)
  {
    zb_zdo_pim_set_long_poll_interval(ZB_QUARTERECONDS_TO_MSEC(new_val));
  }
  else if (attr_id==ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID)
  {
    zb_zdo_pim_set_fast_poll_interval(ZB_QUARTERECONDS_TO_MSEC(new_val));
  }
#ifdef ZB_USE_NVRAM
  TRACE_MSG(TRACE_ZCL1, "check if nvram save is needed", (FMT__0));
  if(attr_id==ZB_ZCL_ATTR_POLL_CONTROL_CHECKIN_INTERVAL_ID
     || attr_id==ZB_ZCL_ATTR_POLL_CONTROL_LONG_POLL_INTERVAL_ID
     || attr_id==ZB_ZCL_ATTR_POLL_CONTROL_SHORT_POLL_INTERVAL_ID
     || attr_id==ZB_ZCL_ATTR_POLL_CONTROL_FAST_POLL_TIMEOUT_ID)
  {
    TRACE_MSG(TRACE_ZCL1, "schedule nvram save", (FMT__0));
    ZB_SCHEDULE_CALLBACK(zb_zcl_poll_control_save_nvram, 0);
  }
#endif /* ZB_USE_NVRAM */
#endif /* defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN */

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_write_attr_hook", (FMT__0));
}


#if defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN
zb_ret_t zb_zcl_poll_control_set_client_addr(zb_uint8_t local_ep, zb_uint16_t addr, zb_uint8_t ep)
{
  zb_zcl_attr_t *attr_desc;
  zb_zcl_poll_control_srv_cfg_data_t *addr_data;
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_set_client_addr local_ep %hd, addr %d, endpoint %hd",
            (FMT__H_D_H, local_ep, addr, ep));
  /* See Use Trust Center for Cluster Command */
  if (ZB_ZDO_CHECK_CLUSTER_PERMISSION(addr, ZB_ZCL_CLUSTER_ID_POLL_CONTROL))
  {
    attr_desc = zb_zcl_get_attr_desc_a(
      local_ep,
      ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_ADDR_DATA_ID);
    ZB_ASSERT(attr_desc);
    addr_data = (zb_zcl_poll_control_srv_cfg_data_t*)attr_desc->data_p;

    addr_data->poll_addr = addr;
    addr_data->poll_ep = ep;
    ret = RET_OK;
  }
  else
  {
    ret = RET_ERROR;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_set_client_addr", (FMT__0));
  return ret;
}
#endif /* defined ZB_HA_ENABLE_POLL_CONTROL_SERVER || defined DOXYGEN */

/**
 *  @} internal
*/

#if defined ZB_ENABLE_HA
/*
static zb_uint8_t gs_poll_control_cmd_list_srv[] =
{
    ZB_ZCL_CMD_POLL_CONTROL_CHECK_IN_ID
};

static zb_uint8_t gs_poll_control_cmd_list_cli[] =
{
    ZB_ZCL_CMD_POLL_CONTROL_CHECK_IN_RESPONSE_ID,
    ZB_ZCL_CMD_POLL_CONTROL_FAST_POLL_STOP_ID,
    ZB_ZCL_CMD_POLL_CONTROL_SET_LONG_POLL_INTERVAL_ID,
    ZB_ZCL_CMD_POLL_CONTROL_SET_SHORT_POLL_INTERVAL_ID
};
*/
/* Returns supported commands list for receiving or generating */
/*
zb_uint8_t zb_zcl_get_cmd_list_poll_control(zb_bool_t is_client_generated, zb_uint8_t **cmd_list)
{
  zb_uint8_t ret = 0;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_get_cmd_list_poll_control is_client_generated %hd", (FMT__H, is_client_generated));

  if (is_client_generated)
  {
    *cmd_list = gs_poll_control_cmd_list_cli;
    ret = sizeof(gs_poll_control_cmd_list_cli);
  }
  else
  {
    *cmd_list = gs_poll_control_cmd_list_srv;
    ret = sizeof(gs_poll_control_cmd_list_srv);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_get_cmd_list_poll_control ret %hd", (FMT__H, ret));
  return ret;
}
*/
#endif /* defined ZB_ENABLE_HA */

#endif /* ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL */
