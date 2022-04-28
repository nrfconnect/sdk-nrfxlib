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

#define ZB_TRACE_FILE_ID 78

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_DOOR_LOCK)

#include "zcl/zb_zcl_door_lock.h"
#include "zb_aps.h"

static zb_uint8_t gs_door_lock_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DOOR_LOCK_CLIENT_ROLE_RECEIVED_CMD_LIST
};

static zb_uint8_t gs_door_lock_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DOOR_LOCK_SERVER_ROLE_RECEIVED_CMD_LIST
};

static zb_discover_cmd_list_t gs_door_lock_client_cmd_list =
{
  sizeof(gs_door_lock_client_received_commands), gs_door_lock_client_received_commands,
  sizeof(gs_door_lock_server_received_commands), gs_door_lock_server_received_commands
};

static zb_discover_cmd_list_t gs_door_lock_server_cmd_list =
{
  sizeof(gs_door_lock_server_received_commands), gs_door_lock_server_received_commands,
  sizeof(gs_door_lock_client_received_commands), gs_door_lock_client_received_commands
};


zb_ret_t check_value_door_lock_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_door_lock_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_door_lock_specific_commands_cli(zb_uint8_t param);

void zb_zcl_door_lock_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DOOR_LOCK,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_door_lock_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_door_lock_specific_commands_srv);
}

void zb_zcl_door_lock_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DOOR_LOCK,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_door_lock_specific_commands_cli);
}

zb_ret_t check_value_door_lock_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(endpoint);

  switch( attr_id )
  {
  case ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_ID:

    if( *value > ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_UNLOCKED &&  *value < ZB_ZCL_ATTR_DOOR_LOCK_LOCK_STATE_UNDEFINED )
    {
      ret = RET_ERROR;
    }
    break;

  case ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_ID:
    if( *value > ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_OTHER &&  *value < ZB_ZCL_ATTR_DOOR_LOCK_LOCK_TYPE_UNDEFINED )
    {
      ret = RET_ERROR;
    }
    break;

  case ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ID:
      if(*value != ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_DISABLED &&
         *value != ZB_ZCL_ATTR_DOOR_LOCK_ACTUATOR_ENABLED_ENABLED)
      {
        ret = RET_ERROR;
      }
      break;

  default:
    break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_door_lock ret %hd", (FMT__H, ret));
  return ret;
}

static zb_zcl_status_t zb_zcl_door_lock_map_ret_code_to_zcl_status(zb_ret_t ret_code)
{
  ZB_ASSERT(ret_code != RET_BUSY);

  return zb_zcl_get_zcl_status_from_ret(ret_code);
}

/** @brief Lock Door command */
static zb_ret_t zb_zcl_door_lock_lock_door_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_door_lock_read_lock_door_res_payload_t pl_out;
  zb_bool_t security = (zb_bool_t)ZB_APS_FC_GET_SECURITY(cmd_info->addr_data.common_data.fc);
  

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_door_lock_lock_door_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_DOOR_LOCK_LOCK_DOOR_CB_ID, RET_OK, cmd_info, NULL, &pl_out);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    ZB_ZCL_DOOR_LOCK_SEND_LOCK_DOOR_RES(param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->seq_number,
      pl_out.status,
      security);
    ret = RET_BUSY;
  }
  else
  {
    ret = RET_ERROR;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_door_lock_lock_door_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Unlock Door command */
static zb_ret_t zb_zcl_door_lock_unlock_door_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_door_lock_read_unlock_door_res_payload_t pl_out;
  zb_bool_t security = (zb_bool_t)ZB_APS_FC_GET_SECURITY(cmd_info->addr_data.common_data.fc);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_door_lock_unlock_door_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_DOOR_LOCK_UNLOCK_DOOR_CB_ID, RET_OK, cmd_info, NULL, &pl_out);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    ZB_ZCL_DOOR_LOCK_SEND_UNLOCK_DOOR_RES(param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->seq_number,
      pl_out.status,
      security);
    ret = RET_BUSY;
  }
  else
  {
    ret = RET_ERROR;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_door_lock_unlock_door_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Lock Door Response command */
static zb_ret_t zb_zcl_door_lock_lock_door_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_door_lock_read_lock_door_res_payload_t *pl_in = NULL;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_door_lock_lock_door_resp_handler %hx", (FMT__H, param));

  pl_in = ZB_ZCL_DOOR_LOCK_READ_LOCK_DOOR_RES(param);

  if (!pl_in)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_DOOR_LOCK_READ_LOCK_DOOR_RES", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_DOOR_LOCK_LOCK_DOOR_RESP_CB_ID, RET_OK, cmd_info, pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_door_lock_lock_door_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Unlock Door Response command */
static zb_ret_t zb_zcl_door_lock_unlock_door_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_door_lock_read_unlock_door_res_payload_t *pl_in = NULL;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_door_lock_unlock_door_resp_handler %hx", (FMT__H, param));

  pl_in = ZB_ZCL_DOOR_LOCK_READ_UNLOCK_DOOR_RES(param);

  if (!pl_in)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_DOOR_LOCK_READ_UNLOCK_DOOR_RES", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_DOOR_LOCK_UNLOCK_DOOR_RESP_CB_ID, RET_OK, cmd_info, pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_door_lock_unlock_door_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

zb_bool_t zb_zcl_process_door_lock_specific_commands_srv(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;

  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_door_lock_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1,
            "> zb_zcl_process_door_lock_specific_commands_srv: param %d, cmd %d",
            (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_DOOR_LOCK == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_DOOR_LOCK_LOCK_DOOR:
    ret = zb_zcl_door_lock_lock_door_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_DOOR_LOCK_UNLOCK_DOOR:
    ret = zb_zcl_door_lock_unlock_door_handler(param, &cmd_info);
    break;
  default:
    processed = ZB_FALSE;
    break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_door_lock_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_door_lock_specific_commands_srv: processed %d",
            (FMT__D, processed));

  return processed;
}

zb_bool_t zb_zcl_process_door_lock_specific_commands_cli(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;

  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_door_lock_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1,
            "> zb_zcl_process_door_lock_specific_commands_cli: param %d, cmd %d",
            (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_DOOR_LOCK == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_DOOR_LOCK_LOCK_DOOR_RES:
    ret = zb_zcl_door_lock_lock_door_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_DOOR_LOCK_UNLOCK_DOOR_RES:
    ret = zb_zcl_door_lock_unlock_door_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_DOOR_LOCK_OPERATION_EVENT_NOTIFICATION_ID:
    break;
  default:
    processed = ZB_FALSE;
    break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_door_lock_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_door_lock_specific_commands_cli: processed %d",
            (FMT__D, processed));

  return processed;
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_DOOR_LOCK */
