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
/* PURPOSE: ZCL Identify cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 1636

#include "zb_common.h"

#if defined(ZB_ZCL_SUPPORT_CLUSTER_IDENTIFY)

#include "zb_zcl.h"
#include "zcl/zb_zcl_identify.h"
#include "zb_bdb_internal.h"
#include "zb_zdo.h"
#include "zb_aps.h"

static zb_uint8_t gs_identify_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IDENTIFY_CLIENT_ROLE_RECEIVED_CMD_LIST
};

static zb_uint8_t gs_identify_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IDENTIFY_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_identify_server_cmd_list =
{
  sizeof(gs_identify_server_received_commands), gs_identify_server_received_commands,
  sizeof(gs_identify_client_received_commands), gs_identify_client_received_commands
};

zb_discover_cmd_list_t gs_identify_client_cmd_list =
{
  sizeof(gs_identify_client_received_commands), gs_identify_client_received_commands,
  sizeof(gs_identify_server_received_commands), gs_identify_server_received_commands
};

void zb_zcl_identify_time_handler(zb_uint8_t param);

#define GET_IDENTIFY_HANDLER(endpoint)                    \
  (zb_af_get_endpoint_desc((endpoint))->identify_handler)

zb_ret_t check_value_identify_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_identify_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_identify_specific_commands_cli(zb_uint8_t param);

void zb_zcl_identify_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);

void zb_zcl_identify_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IDENTIFY,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_identify_server,
                              zb_zcl_identify_write_attr_hook_server,
                              zb_zcl_process_identify_specific_commands_srv);
}

void zb_zcl_identify_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IDENTIFY,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_identify_specific_commands_cli);
}

zb_ret_t check_value_identify_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}
/* Invoke User App for "Identify effect" command
 *
 * Invoke User App with effect parameters : EffectId and EffectVariant
 * if invoke result RET_OK then schedule invoke User App with attribute Identify
 * else send responce command with error
 */
void zb_zcl_identify_effect_invoke_user_app(zb_uint8_t param)
{
  zb_zcl_identify_effect_user_app_schedule_t* invoke_data = ZB_BUF_GET_PARAM(param, zb_zcl_identify_effect_user_app_schedule_t);
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_invoke_effect_invoke_user_app param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, &(invoke_data->cmd_info), sizeof(zb_zcl_parsed_hdr_t));

  if (ZCL_CTX().device_cb!=NULL)
  {
    zb_zcl_device_callback_param_t *user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    ZB_MEMMOVE(&(user_app_data->cb_param.identify_effect_value_param),
        &(invoke_data->param),
        sizeof(zb_zcl_identify_effect_value_param_t));
    user_app_data->device_cb_id = ZB_ZCL_IDENTIFY_EFFECT_CB_ID;
    user_app_data->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
    result = user_app_data->status;
    /* TODO: check - free buffer after a call? */
  }
  else
  {
    result = RET_NOT_IMPLEMENTED;
  }

  ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, zb_zcl_get_zcl_status_from_ret(result));

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_on_off_effect_invoke_user_app param", (FMT__0));
}

zb_bool_t zb_zcl_process_identify_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t processed = ZB_TRUE;
  zb_uint8_t status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_identify_req_t  payload;
  zb_zcl_parse_status_t st;
  zb_zcl_identify_effect_req_t payload_effect;
  zb_zcl_attr_t * attr_desc;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_process_identify_specific_commands: param %d, cmd %d",
      (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_IDENTIFY == cmd_info.cluster_id);

  switch (cmd_info.cmd_id)
  {
    /* NOTE: pay attention that following case handles two commands Identify and
     * IdentifyQueryResponse. This commands have the same ids values (ZB_ZCL_CMD_IDENTIFY_IDENTIFY_ID
     * and ZB_ZCL_CMD_IDENTIFY_IDENTIFY_QUERY_RSP_ID) but differ by direction.
     */
    case ZB_ZCL_CMD_IDENTIFY_IDENTIFY_ID:
      if (cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV)
      {
        ZB_ZCL_IDENTIFY_GET_IDENTIFY_REQ(&payload, param, st);
        if (st != ZB_ZCL_PARSE_STATUS_SUCCESS)
        {
          TRACE_MSG(TRACE_ERROR, "Got erroneous payload length %hd != 2",
            (FMT__H, zb_buf_len(param)));
          status = ZB_ZCL_STATUS_MALFORMED_CMD;
        }
        else
        {
          zb_zcl_start_identifying(
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, payload.timeout);
        }

        ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, status);
      }
#if defined ZB_BDB_ENABLE_FINDING_BINDING
      else if (ZCL_SELECTOR().process_identify_query_res != NULL)
/* direction = from server to client - identify query response */
      {
        /* ZB_ZCL_CMD_IDENTIFY_IDENTIFY_QUERY_RSP_ID */
        ZCL_SELECTOR().process_identify_query_res(param);
      }
#endif /* defined ZB_BDB_ENABLE_FINDING_BINDING */
      break;
    case ZB_ZCL_CMD_IDENTIFY_IDENTIFY_QUERY_ID:
      TRACE_MSG(TRACE_ERROR, "Received identify query request", (FMT__0));
      if (zb_zcl_is_identifying(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
      {
        attr_desc = zb_zcl_get_attr_desc_a(
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          ZB_ZCL_CLUSTER_ID_IDENTIFY,
          ZB_ZCL_CLUSTER_SERVER_ROLE,
          ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID);
        ZB_ZCL_IDENTIFY_SEND_IDENTIFY_QUERY_RES(
          param,
          *(zb_uint16_t*)attr_desc->data_p,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.profile_id,
          cmd_info.seq_number,
          (zb_bool_t)ZB_APS_FC_GET_SECURITY(cmd_info.addr_data.common_data.fc));
      }
      else
      {
          status = ZB_ZCL_STATUS_SUCCESS;
          ZB_ZCL_PROCESS_COMMAND_FINISH_NEW(param, &cmd_info, status);
      }
      break;

    case ZB_ZCL_CMD_IDENTIFY_TRIGGER_EFFECT_ID:
      ZB_ZCL_IDENTIFY_GET_TRIGGER_VARIANT_REQ(&payload_effect, param, status);
      if (status!=ZB_ZCL_PARSE_STATUS_SUCCESS)
      {
        TRACE_MSG(TRACE_ERROR, "Got erroneous payload length %hd != 2",
            (FMT__H, zb_buf_len(param)));
        ZB_ZCL_SEND_DEFAULT_RESP(
            param,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
            ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
            cmd_info.profile_id,
            ZB_ZCL_CLUSTER_ID_IDENTIFY,
            cmd_info.seq_number,
            ZB_ZCL_CMD_IDENTIFY_TRIGGER_EFFECT_ID,
            ZB_ZCL_STATUS_MALFORMED_CMD);
      }
      else
      {
          ZB_ZCL_IDENTIFY_EFFECT_SCHEDULE_USER_APP(param, &cmd_info,
              payload_effect.effect_id, payload_effect.effect_variant);
      }
      break;

    default:
      processed = ZB_FALSE;
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_identify_specific_commands: processed %d",
      (FMT__D, processed));
  return processed;
}

zb_bool_t zb_zcl_process_identify_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_identify_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_identify_specific_commands(param);
}
zb_bool_t zb_zcl_process_identify_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_identify_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_identify_specific_commands(param);
}

/* Assumes param contains an endpoint number */
void zb_zcl_identify_time_handler(zb_uint8_t param)
{
  zb_zcl_attr_t *attr_desc;
  zb_callback_t identify_handler;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_identify_time_handler %hd", (FMT__H, param));
  attr_desc = zb_zcl_get_attr_desc_a(
        param,
        ZB_ZCL_CLUSTER_ID_IDENTIFY,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID);
  if (attr_desc && attr_desc->data_p)
  {
    zb_uint16_t * value_ptr = (zb_uint16_t*)attr_desc->data_p;
    if (*value_ptr)
    {
      (*value_ptr)--;
      ZB_SCHEDULE_ALARM(zb_zcl_identify_time_handler, param, ZB_TIME_ONE_SECOND);
      TRACE_MSG(TRACE_ZCL3, "new value is: %d", (FMT__D, *value_ptr));
    }
    else
    {
      identify_handler = GET_IDENTIFY_HANDLER(param);
      TRACE_MSG(TRACE_ZCL2, "identify timeout, handler %p", (FMT__P, identify_handler));
      if (identify_handler)
      {
        ZB_SCHEDULE_CALLBACK(identify_handler, ZB_FALSE);
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_identify_time_handler", (FMT__0));
}

zb_uint8_t zb_zcl_start_identifying(zb_uint8_t endpoint, zb_uint16_t timeout)
{
  zb_uint8_t status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_attr_t * attr_desc;
  zb_uint16_t old_timeout;
  zb_callback_t identify_handler;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_start_identifying endpoint %hd timeout %ds", (FMT__D_H, endpoint, timeout));
  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint,
    ZB_ZCL_CLUSTER_ID_IDENTIFY,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID);
  if (!attr_desc || !attr_desc->data_p)
  {
    status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
  }
  else
  {
    /* TODO read attribute call? */
    old_timeout = *(zb_uint16_t*)attr_desc->data_p;
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, timeout);
    if (*(zb_uint16_t*)attr_desc->data_p)
    {
      if (!old_timeout)
      {
        ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_identify_time_handler, endpoint);
        ZB_SCHEDULE_ALARM(zb_zcl_identify_time_handler, endpoint, ZB_TIME_ONE_SECOND);
        identify_handler = GET_IDENTIFY_HANDLER(endpoint);
        if (identify_handler)
        {
          ZB_SCHEDULE_CALLBACK(identify_handler, ZB_TRUE);
        }
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_start_identifying status %hd", (FMT__H, status));
  return status;
}

zb_uint8_t zb_zcl_is_identifying(zb_uint8_t endpoint)
{
  zb_uint8_t result = ZB_FALSE;
  zb_zcl_attr_t * attr_desc;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_is_identifying: endpoint %hd", (FMT__H, endpoint));

  if (endpoint)
  {
    attr_desc = zb_zcl_get_attr_desc_a(
        endpoint,
        ZB_ZCL_CLUSTER_ID_IDENTIFY,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID);
    if (attr_desc)
    {
      result = (0 != (*(zb_uint16_t*)attr_desc->data_p));
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "Attribute not found", (FMT__0));
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_is_identifying: result %hd", (FMT__H, result));
  return result;
}

void zb_zcl_stop_identifying(zb_uint8_t endpoint)
{
  zb_zcl_attr_t * attr_desc;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_stop_identifying: endpoint %hd", (FMT__H, endpoint));

  attr_desc = zb_zcl_get_attr_desc_a(
                endpoint,
                ZB_ZCL_CLUSTER_ID_IDENTIFY,
                ZB_ZCL_CLUSTER_SERVER_ROLE,
                ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID);

  if (attr_desc)
  {
    zb_callback_t   identify_handler;

    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_identify_time_handler, endpoint);

    /* Write 0 to identify time attribute. */
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, 0);

    /* Notify application that identify is finished. */
    identify_handler = GET_IDENTIFY_HANDLER(endpoint);
    TRACE_MSG(TRACE_ZCL2, "identify manually stopped, handler %p", (FMT__P, identify_handler));
    if (identify_handler)
    {
      ZB_SCHEDULE_CALLBACK(identify_handler, ZB_FALSE);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "error: Attribute not found", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_stop_identifying", (FMT__0));
}

/** @brief Hook on Write attribute
 * Start Identifying on IdentifyTime attribute set */
void zb_zcl_identify_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  zb_uint16_t val = 0;
  (void)endpoint;
  (void)attr_id;
  ZB_ASSIGN_UINT16(&val, new_value);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_identify_write_attr_hook endpoint %hx attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D, endpoint, attr_id, manuf_code));

  ZVUNUSED(manuf_code);

  /* ZCL8, 3.5.2.2.1 IdentifyTime attribute:
  * If this attribute is set to a value other than 0x0000 then the device SHALL enter its
  * identification procedure, in order to indicate to an observer which of several devices it is. It
  * is recommended that this procedure consists of flashing a light with a period of 0.5
  * seconds. The IdentifyTime attribute SHALL be decremented every second. */
  if (attr_id == ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID)
  {
    if (val != 0)
    {
      TRACE_MSG(TRACE_ZCL1, "start identifying", (FMT__0));
      zb_zcl_start_identifying(endpoint, val);
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "stop identifying", (FMT__0));
      zb_zcl_stop_identifying(endpoint);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_identify_write_attr_hook", (FMT__0));
}

#endif /* defined(ZB_ZCL_SUPPORT_CLUSTER_IDENTIFY) */
