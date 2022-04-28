/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZCL IAS WD cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2069

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_IAS_WD)

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zb_zdo.h"
#include "zb_aps.h"
#include "zcl/zb_zcl_ias_wd.h"
#include "zcl/zb_zcl_ias_zone.h"
/** @internal
    @{
*/

zb_uint8_t gs_ias_wd_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IAS_WD_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_ias_wd_client_cmd_list =
{
  0, NULL,
  sizeof(gs_ias_wd_server_received_commands), gs_ias_wd_server_received_commands
};

zb_discover_cmd_list_t gs_ias_wd_server_cmd_list =
{
  sizeof(gs_ias_wd_server_received_commands), gs_ias_wd_server_received_commands,
  0, NULL
};

zb_ret_t check_value_ias_wd_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

zb_bool_t zb_zcl_process_ias_wd_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_ias_wd_specific_commands_cli(zb_uint8_t param);

void zb_zcl_ias_wd_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IAS_WD,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_ias_wd_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ias_wd_specific_commands_srv);
}

void zb_zcl_ias_wd_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IAS_WD,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ias_wd_specific_commands_cli);
}

zb_ret_t check_value_ias_wd_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_IAS_WD_MAX_DURATION_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_ATTR_IAS_WD_MAX_DURATION_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_ias_wd ret %hd", (FMT__H, ret));
  return ret;
}
/* Invoke User App for "Start Warning" command
 */
void zb_zcl_ias_wd_start_warning_invoke_user_app(zb_uint8_t param)
{
  zb_zcl_ias_wd_start_warning_user_app_schedule_t* invoke_data =
      ZB_BUF_GET_PARAM(param, zb_zcl_ias_wd_start_warning_user_app_schedule_t);
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_wd_start_warning_invoke_user_app param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, &(invoke_data->cmd_info), sizeof(zb_zcl_parsed_hdr_t));

  if (ZCL_CTX().device_cb!=NULL)
  {
    zb_zcl_device_callback_param_t *user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    ZB_MEMMOVE(&(user_app_data->cb_param.start_warning_value_param),
        &(invoke_data->param),
        sizeof(zb_zcl_ias_wd_start_warning_value_param_t));
    user_app_data->device_cb_id = ZB_ZCL_IAS_WD_START_WARNING_VALUE_CB_ID;
    user_app_data->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
    result = user_app_data->status;
  }
  else
  {
    result = RET_NOT_IMPLEMENTED;
  }

  ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, zb_zcl_get_zcl_status_from_ret(result));

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_wd_start_warning_invoke_user_app", (FMT__0));
}

/** @brief Start Warning command */
static zb_ret_t start_warning_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_wd_start_warning_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> start_warning_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_IAS_WD_GET_START_WARNING_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_WD_GET_START_WARNING_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_zcl_attr_t *attr_desc;
    zb_uint16_t max_dur;
    attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
            ZB_ZCL_CLUSTER_ID_IAS_WD, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_WD_MAX_DURATION_ID);
    ZB_ASSERT(attr_desc);

    max_dur = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    ZB_ZCL_IAS_WD_START_WARNING_SCHEDULE_USER_APP(param, &cmd_info,
      ZB_ZCL_IAS_WD_GET_WARNING_MODE(payload.status),
      ZB_ZCL_IAS_WD_GET_STROBE(payload.status),
      ZB_ZCL_IAS_WD_GET_SIREN_LEVEL(payload.status),
      (payload.duration < max_dur ? payload.duration : max_dur),
      (payload.strobe_duty_cycle < ZB_ZCL_IAS_WD_STROBE_DUTY_CYCLE_MAX_VALUE ?
       payload.strobe_duty_cycle : ZB_ZCL_IAS_WD_STROBE_DUTY_CYCLE_MAX_VALUE),
      payload.strobe_level);

    ret = RET_BUSY;
  }

  TRACE_MSG(TRACE_ZCL1, "< start_warning_handler ret %hx", (FMT__H, ret));
  return ret;
}

/* Invoke User App for "Squawk" command
 */
void zb_zcl_ias_wd_squawk_invoke_user_app(zb_uint8_t param)
{
  zb_zcl_ias_wd_squawk_user_app_schedule_t* invoke_data =
      ZB_BUF_GET_PARAM(param, zb_zcl_ias_wd_squawk_user_app_schedule_t);
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_wd_squawk_invoke_user_app param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, &(invoke_data->cmd_info), sizeof(zb_zcl_parsed_hdr_t));

  if (ZCL_CTX().device_cb!=NULL)
  {
    zb_zcl_device_callback_param_t *user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    ZB_MEMMOVE(&(user_app_data->cb_param.squawk_value_param),
        &(invoke_data->param),
        sizeof(zb_zcl_ias_wd_squawk_value_param_t));
    user_app_data->device_cb_id = ZB_ZCL_IAS_WD_SQUAWK_VALUE_CB_ID;
    user_app_data->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
    result = user_app_data->status;
  }
  else
  {
    result = RET_NOT_IMPLEMENTED;
  }

  ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, zb_zcl_get_zcl_status_from_ret(result));

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_wd_squawk_invoke_user_app", (FMT__0));
}

/** @brief Squawk command */
static zb_ret_t squawk_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_wd_squawk_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> squawk_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_IAS_WD_GET_SQUAWK_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_WD_GET_SQUAWK_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_IAS_WD_SQUAWK_SCHEDULE_USER_APP(param, &cmd_info,
      ZB_ZCL_IAS_WD_GET_SQUAWK_MODE(payload.status),
      ZB_ZCL_IAS_WD_GET_SQUAWK_STROBE(payload.status),
      ZB_ZCL_IAS_WD_GET_SQUAWK_LEVEL(payload.status));

    ret = RET_BUSY;
  }

  TRACE_MSG(TRACE_ZCL1, "< squawk_handler ret %hx", (FMT__H, ret));
  return ret;
}

zb_bool_t zb_zcl_process_ias_wd_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_ias_wd_specific_commands: param %d, cmd %d",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_IAS_WD == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch( cmd_info.cmd_id )
  {
  case ZB_ZCL_CMD_IAS_WD_START_WARNING_ID:
    status = start_warning_handler(param);
    break;

  case ZB_ZCL_CMD_IAS_WD_SQUAWK_ID:
    status = squawk_handler(param);
    break;

  default:
    processed = ZB_FALSE;
    break;
  }

  if( processed )
  {
    if( cmd_info.disable_default_response && status == RET_OK)
    {
      TRACE_MSG( TRACE_ZCL3,
                 "Default response disabled",
                 (FMT__0));
      zb_buf_free(param);
    }
    else if (status != RET_BUSY)
    {
      ZB_ZCL_SEND_DEFAULT_RESP( param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.profile_id,
          ZB_ZCL_CLUSTER_ID_IAS_WD,
          cmd_info.seq_number,
          cmd_info.cmd_id,
          status==RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_FIELD);
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_ias_wd_specific_commands: processed %d",
             (FMT__D, processed));
  return processed;
}

zb_bool_t zb_zcl_process_ias_wd_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ias_wd_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_ias_wd_specific_commands(param);
}
zb_bool_t zb_zcl_process_ias_wd_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ias_wd_client_cmd_list;
    return ZB_TRUE;
  }
  return ZB_FALSE;
}

/**
 *  @} internal
*/
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_IAS_WD */
