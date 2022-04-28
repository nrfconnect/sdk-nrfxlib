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
/* PURPOSE:
*/

#define ZB_TRACE_FILE_ID 81

#include "zb_common.h"

/* 2018/08/10 CR:MINOR (rev. 39551): Probably remove this ifdef? Really, why we need to compile empty ZCL
 * library that way? This may be done with one string in Options file instead of bunch of ifdef-s
 * in every cluster.
 * Need to discuss with EE. */
#if defined (ZB_ZCL_SUPPORT_CLUSTER_WINDOW_COVERING)

#include "zb_zcl.h"
#include "zb_aps.h"

zb_uint8_t gs_window_covering_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_WINDOW_COVERING_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_window_covering_client_cmd_list =
{
  0, NULL,
  sizeof(gs_window_covering_server_received_commands), gs_window_covering_server_received_commands
};

zb_discover_cmd_list_t gs_window_covering_server_cmd_list =
{
  sizeof(gs_window_covering_server_received_commands), gs_window_covering_server_received_commands,
  0, NULL
};

static zb_ret_t check_value_window_covering_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
static zb_bool_t zb_zcl_process_window_covering_specific_commands_srv(zb_uint8_t param);
static zb_bool_t zb_zcl_process_window_covering_specific_commands_cli(zb_uint8_t param);

void zb_zcl_window_covering_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_window_covering_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_window_covering_specific_commands_srv);
}

void zb_zcl_window_covering_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_WINDOW_COVERING,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_window_covering_specific_commands_cli);
}

static zb_ret_t check_value_window_covering_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_WINDOW_COVERING_WINDOW_COVERING_TYPE_ID:
      if( *value > ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_PROJECTOR_SCREEN )
      {
        ret = RET_ERROR;
      }
      break;

    case ZB_ZCL_ATTR_WINDOW_COVERING_CONFIG_STATUS_ID:
      if( *value >= 0x80 )
      {
        ret = RET_ERROR;
      }
      break;

    case ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID:
      if( *value > ZB_ZCL_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_MAX_VALUE )
      {
        ret = RET_ERROR;
      }
      break;

    case ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_ID:
      if( *value > ZB_ZCL_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_MAX_VALUE )
      {
        ret = RET_ERROR;
      }
      break;
    case ZB_ZCL_ATTR_WINDOW_COVERING_MODE_ID:
      if( !(*value & 0x1f) )
      {
        ret = RET_ERROR;
      }
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_window_covering ret %hd", (FMT__H, ret));

  return ret;
}

static zb_zcl_status_t zb_zcl_window_covering_map_ret_code_to_zcl_status(zb_ret_t ret_code)
{
  ZB_ASSERT(ret_code != RET_BUSY);

  return zb_zcl_get_zcl_status_from_ret(ret_code);
}

static zb_ret_t zb_zcl_window_covering_invoke_user_app(zb_uint8_t param,
                                                       const zb_zcl_parsed_hdr_t *cmd_info,
                                                       zb_zcl_device_callback_id_t user_cb_id,
                                                       void *payload)
      {
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_window_covering_invoke_user_app cb_id %d", (FMT__D, user_cb_id));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, user_cb_id, RET_OK, cmd_info, payload, NULL);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
      }

  ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_window_covering_invoke_user_app ret %d", (FMT__D, ret));

  return ret;
}

static zb_ret_t zb_zcl_process_window_covering_up_open_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "Window Covering Up/Open command", (FMT__0));

  ret = zb_zcl_window_covering_invoke_user_app(param,
                                               cmd_info,
                                               ZB_ZCL_WINDOW_COVERING_UP_OPEN_CB_ID,
                                               NULL);

  return ret;
}

static zb_ret_t zb_zcl_process_window_covering_down_close_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "Window Covering Down/Close command", (FMT__0));

  ret = zb_zcl_window_covering_invoke_user_app(param,
                                               cmd_info,
                                               ZB_ZCL_WINDOW_COVERING_DOWN_CLOSE_CB_ID,
                                               NULL);

  return ret;
}

static zb_ret_t zb_zcl_process_window_covering_stop_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "Window Covering Stop command", (FMT__0));

  ret = zb_zcl_window_covering_invoke_user_app(param,
                                               cmd_info,
                                               ZB_ZCL_WINDOW_COVERING_STOP_CB_ID,
                                               NULL);

  return ret;
}

static zb_ret_t zb_zcl_process_window_covering_go_to_lift_percentage_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_parse_status_t status;
  zb_zcl_go_to_lift_percentage_req_t lift_percentage;

  TRACE_MSG(TRACE_ZCL1, "Window Covering Get Go to Lift percentage command", (FMT__0));

  ZB_ZCL_WINDOW_COVERING_GET_GO_TO_LIFT_PERCENTAGE_REQ(param,
                                                       &lift_percentage,
                                                       status);
  if (status == ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    if (zb_zcl_check_attr_value(ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_LIFT_PERCENTAGE_ID,
        &(lift_percentage.percentage_lift_value)) == RET_OK)
    {
      TRACE_MSG(TRACE_ZCL1, "Value = %hd, going...",
                (FMT__H, lift_percentage.percentage_lift_value));

      ret = zb_zcl_window_covering_invoke_user_app(param,
                                                   cmd_info,
                                                   ZB_ZCL_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE_CB_ID,
                                                   (void *)&lift_percentage);
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "Invalid value = %hd",
                (FMT__H, lift_percentage.percentage_lift_value));

      ret = RET_INVALID_PARAMETER;
    }
  }
  else
  {
    ret = RET_INVALID_PARAMETER_1;
  }

  return ret;
}

static zb_ret_t zb_zcl_process_window_covering_go_to_tilt_percentage_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_parse_status_t status;
  zb_zcl_go_to_tilt_percentage_req_t tilt_percentage;

  TRACE_MSG(TRACE_ZCL1, "Window Covering Get Go to Tilt percentage command", (FMT__0));

  ZB_ZCL_WINDOW_COVERING_GET_GO_TO_TILT_PERCENTAGE_REQ(param,
                                                       &tilt_percentage,
                                                       status);
  if (status == ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    if (zb_zcl_check_attr_value(ZB_ZCL_CLUSTER_ID_WINDOW_COVERING, ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        ZB_ZCL_ATTR_WINDOW_COVERING_CURRENT_POSITION_TILT_PERCENTAGE_ID,
        &(tilt_percentage.percentage_tilt_value)) == RET_OK)
    {
      TRACE_MSG(TRACE_ZCL1, "Value = %hd, going...",
                (FMT__H, tilt_percentage.percentage_tilt_value));

      ret = zb_zcl_window_covering_invoke_user_app(param,
                                                   cmd_info,
                                                   ZB_ZCL_WINDOW_COVERING_GO_TO_TILT_PERCENTAGE_CB_ID,
                                                   (void *)&tilt_percentage);
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "Invalid value = %hd",
                (FMT__H, tilt_percentage.percentage_tilt_value));

      ret = RET_INVALID_PARAMETER;
    }
  }
  else
  {
    ret = RET_INVALID_PARAMETER_1;
  }

  return ret;
}

static zb_bool_t zb_zcl_process_window_covering_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_ret_t  ret       = RET_OK;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1,
            "> zb_zcl_process_window_covering_specific_commands: param %hd",
            (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_WINDOW_COVERING == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_WINDOW_COVERING_UP_OPEN:
    ret = zb_zcl_process_window_covering_up_open_handler(param, &cmd_info);
      break;
  case ZB_ZCL_CMD_WINDOW_COVERING_DOWN_CLOSE:
    ret = zb_zcl_process_window_covering_down_close_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_WINDOW_COVERING_STOP:
    ret = zb_zcl_process_window_covering_stop_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE:
    ret = zb_zcl_process_window_covering_go_to_lift_percentage_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_WINDOW_COVERING_GO_TO_TILT_PERCENTAGE:
    ret = zb_zcl_process_window_covering_go_to_tilt_percentage_handler(param, &cmd_info);
    break;

    default:
    processed = ZB_FALSE;
      break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_window_covering_map_ret_code_to_zcl_status(ret));
}

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_window_covering_specific_commands: processed %d",
            (FMT__D, processed));

  return processed;
}

static zb_bool_t zb_zcl_process_window_covering_specific_commands_srv(zb_uint8_t param)
{
  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_window_covering_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_window_covering_specific_commands(param);
}

static zb_bool_t zb_zcl_process_window_covering_specific_commands_cli(zb_uint8_t param)
{
  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_window_covering_client_cmd_list;
    return ZB_TRUE;
  }
  return ZB_FALSE;
}
#endif  /* ZB_ZCL_SUPPORT_CLUSTER_WINDOW_COVERING */
