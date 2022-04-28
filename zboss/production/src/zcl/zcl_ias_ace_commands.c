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
/* PURPOSE: ZCL IAS ACE cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2068

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_IAS_ACE)

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_ias_ace.h"
#include "zcl/zb_zcl_ias_zone.h"
/** @internal
    @{
*/

zb_uint8_t gs_ias_ace_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IAS_ACE_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_ias_ace_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IAS_ACE_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_ias_ace_client_cmd_list =
{
  sizeof(gs_ias_ace_client_received_commands), gs_ias_ace_client_received_commands,
  sizeof(gs_ias_ace_server_received_commands), gs_ias_ace_server_received_commands
};

zb_discover_cmd_list_t gs_ias_ace_server_cmd_list =
{
  sizeof(gs_ias_ace_server_received_commands), gs_ias_ace_server_received_commands,
  sizeof(gs_ias_ace_client_received_commands), gs_ias_ace_client_received_commands
};

zb_bool_t zb_zcl_process_ias_ace_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_ias_ace_specific_commands_cli(zb_uint8_t param);

void zb_zcl_ias_ace_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IAS_ACE,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ias_ace_specific_commands_srv);
}

void zb_zcl_ias_ace_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IAS_ACE,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ias_ace_specific_commands_cli);
}

static zb_zcl_status_t zb_zcl_ias_ace_map_ret_code_to_zcl_status(zb_ret_t ret_code)
{
  ZB_ASSERT(ret_code != RET_BUSY);

  return zb_zcl_get_zcl_status_from_ret(ret_code);
}

static zb_uint8_t zb_zcl_ias_ace_get_zone_table_length(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc;

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ACE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_LENGTH_ID);
  ZB_ASSERT(attr_desc);

  return ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
}

static zb_zcl_ias_ace_zone_table_t* zb_zcl_ias_ace_get_zone_table(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc;

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ACE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ACE_ZONE_TABLE_ID);
  ZB_ASSERT(attr_desc);

  return (zb_zcl_ias_ace_zone_table_t *)attr_desc->data_p;
}

/** @brief Arm command */
static zb_ret_t zb_zcl_ias_ace_arm_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_arm_t pl_in;
  zb_zcl_ias_ace_arm_resp_t pl_out;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_arm_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));
  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_IAS_ACE_GET_ARM_REQ(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_ARM_REQ", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    if (pl_in.arm_mode > ZB_ZCL_IAS_ACE_ARM_MODE_ALL ||
        pl_in.zone_id > ZB_ZCL_IAS_ACE_ZONE_ID_MAX_VALUE)
    {
      ret = RET_INVALID_PARAMETER;
    }
    else
    {
      ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
        ZB_ZCL_IAS_ACE_ARM_CB_ID, RET_OK, cmd_info, &pl_in, &pl_out);

      if (ZCL_CTX().device_cb != NULL)
      {
        (ZCL_CTX().device_cb)(param);
      }

      if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
      {
        ZB_ZCL_IAS_ACE_SEND_ARM_RESP(param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
          cmd_info->profile_id,
          cmd_info->seq_number,
          pl_out.arm_notification);

        ret = RET_BUSY;
      }
      else
      {
        ret = RET_ERROR;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_arm_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Bypass command */
static zb_ret_t zb_zcl_ias_ace_bypass_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_bypass_t pl_in;
  zb_zcl_ias_ace_bypass_resp_t pl_out;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_bypass_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));
  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_IAS_ACE_GET_BYPASS_REQ(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_BYPASS_REQ", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t *ptr;
    zb_uint16_t i;

    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_BYPASS_CB_ID, RET_OK, cmd_info, &pl_in, &pl_out);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
    {
      ZB_ZCL_IAS_ACE_SEND_BYPASS_RESP_START(param,
        cmd_info->seq_number, pl_out.length, ptr);

      for (i = 0; i < pl_out.length; i++)
      {
        ZB_ZCL_IAS_ACE_SEND_BYPASS_RESP_ADD(ptr, pl_out.bypass_result[i]);
      }

      ZB_ZCL_IAS_ACE_SEND_BYPASS_RESP_END(ptr, param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id);

      ret = RET_BUSY;
    }
    else
    {
      ret = RET_ERROR;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_bypass_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Emergency command */
static zb_ret_t zb_zcl_ias_ace_emergency_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_emergency_handler %hx", (FMT__H, param));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_IAS_ACE_EMERGENCY_CB_ID, RET_OK, cmd_info, NULL, NULL);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_emergency_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Fire command */
static zb_ret_t zb_zcl_ias_ace_fire_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_fire_handler %hx", (FMT__H, param));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_IAS_ACE_FIRE_CB_ID, RET_OK, cmd_info, NULL, NULL);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_fire_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Panic command */
static zb_ret_t zb_zcl_ias_ace_panic_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_panic_handler %hx", (FMT__H, param));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_IAS_ACE_PANIC_CB_ID, RET_OK, cmd_info, NULL, NULL);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_panic_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Zone ID Map command */
static zb_ret_t zb_zcl_ias_ace_get_zone_id_map_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_ias_ace_get_zone_id_map_resp_t zone_id_map_resp;
  zb_zcl_ias_ace_zone_table_t *table;
  zb_uint16_t length;
  zb_uint16_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_zone_id_map_handler %hx", (FMT__H, param));

  ZB_BZERO(&zone_id_map_resp, sizeof(zone_id_map_resp));
  length = zb_zcl_ias_ace_get_zone_table_length(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  table = zb_zcl_ias_ace_get_zone_table(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);

  for (i = 0; i < length; i++)
  {
    if (table[i].zone_type != ZB_ZCL_IAS_ZONE_ZONETYPE_INVALID)
    {
      zone_id_map_resp.zone_id_map[table[i].zone_id / 16] |= (1 << (table[i].zone_id % 16));
    }
  }

  ZB_ZCL_IAS_ACE_SEND_GET_ZONE_ID_MAP_RESP(param,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
    cmd_info->profile_id,
    cmd_info->seq_number,
    zone_id_map_resp.zone_id_map);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_zone_id_map_handler", (FMT__0));

  return RET_BUSY;
}

/** @brief Get Zone Information command */
static zb_ret_t zb_zcl_ias_ace_get_zone_info_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_zone_info_t payload;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_zone_info_handler %hx", (FMT__H, param));

  ZB_BZERO(&payload, sizeof(payload));

  ZB_ZCL_IAS_ACE_GET_GET_ZONE_INFO_REQ(&payload, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_GET_ZONE_INFO_REQ", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    if (payload.zone_id > ZB_ZCL_IAS_ACE_ZONE_ID_MAX_VALUE)
    {
      ret = RET_INVALID_PARAMETER;
    }
    else
    {
      zb_ieee_addr_t def_address;
      zb_char_t def_zone_label = ZB_ZCL_NULL_STRING;
      zb_zcl_ias_ace_zone_table_t *table;
      zb_uint16_t length;
      zb_uint16_t i;

      ZB_ZCL_IAS_ACE_SET_ZONE_ADDRESS_DEFAULT_VALUE(def_address);

      length = zb_zcl_ias_ace_get_zone_table_length(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
      table = zb_zcl_ias_ace_get_zone_table(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);

      for (i = 0; i < length; i++)
      {
        if (table[i].zone_id == payload.zone_id)
        {
          break;
        }
      }

      ZB_ZCL_IAS_ACE_SEND_GET_ZONE_INFO_RESP(param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id,
        cmd_info->seq_number,
        payload.zone_id,
        (i < length ? table[i].zone_type : ZB_ZCL_IAS_ZONE_ZONETYPE_INVALID),
        (i < length ? table[i].zone_address : def_address),
        (i < length ? (table[i].zone_label ? table[i].zone_label : &def_zone_label) : &def_zone_label));

      ret = RET_BUSY;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_zone_info_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Panel Status command */
static zb_ret_t zb_zcl_ias_ace_get_panel_status_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_panel_status_resp_t pl_out;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_panel_status_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_IAS_ACE_GET_PANEL_STATUS_CB_ID, RET_OK, cmd_info, NULL, &pl_out);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    ZB_ZCL_IAS_ACE_SEND_GET_PANEL_STATUS_RESP(param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id, cmd_info->seq_number,
        pl_out.panel_status, pl_out.seconds_remaining,
        pl_out.aud_notification, pl_out.alarm_status);

    ret = RET_BUSY;
  }
  else
  {
    ret = RET_ERROR;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_panel_status_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Bypassed Zone List command */
static zb_ret_t zb_zcl_ias_ace_get_bypassed_zone_list_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_uint8_t *ptr;
  zb_uint16_t i;
  zb_zcl_ias_ace_set_bypassed_zone_list_t pl_out;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_bypassed_zone_list_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_IAS_ACE_GET_BYPASSED_ZONE_LIST_CB_ID, RET_OK, cmd_info, NULL, &pl_out);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    ZB_ZCL_IAS_ACE_SEND_SET_BYPASSED_ZONE_LIST_START_RESP(param,
      cmd_info->seq_number, pl_out.length, ptr);

    for (i = 0; i < pl_out.length; i++)
    {
      ZB_ZCL_IAS_ACE_SEND_SET_BYPASSED_ZONE_LIST_ADD(ptr, pl_out.zone_id[i]);
    }

    ZB_ZCL_IAS_ACE_SEND_SET_BYPASSED_ZONE_LIST_END(ptr, param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id, NULL);

    ret = RET_BUSY;
  }
  else
  {
    ret = RET_ERROR;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_bypassed_zone_list_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Zone Status command */
static zb_ret_t zb_zcl_ias_ace_get_zone_status_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_zone_status_t pl_in;
  zb_zcl_ias_ace_get_zone_status_resp_t pl_out;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_zone_status_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));
  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_IAS_ACE_GET_GET_ZONE_STATUS_REQ(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_GET_ZONE_STATUS_REQ", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    if (pl_in.starting_zone_id + pl_in.max_num_zone_ids > ZB_ZCL_IAS_ACE_ZONE_TABLE_LENGTH)
    {
      ret = RET_INVALID_PARAMETER;
    }
    else
    {
      zb_uint8_t *ptr;
      zb_uint16_t i;

      ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
        ZB_ZCL_IAS_ACE_GET_ZONE_STATUS_CB_ID, RET_OK, cmd_info, &pl_in, &pl_out);

      if (ZCL_CTX().device_cb != NULL)
      {
        (ZCL_CTX().device_cb)(param);
      }

      if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
      {
        ZB_ZCL_IAS_ACE_SEND_GET_ZONE_STATUS_RESP_START(param,
          cmd_info->seq_number, pl_out.zone_status_complete, pl_out.length, ptr);

        for (i = 0; i < pl_out.length; i++)
        {
          ZB_ZCL_IAS_ACE_SEND_GET_ZONE_STATUS_RESP_ADD(ptr,
            pl_out.zone_id_status[i].zone_id, pl_out.zone_id_status[i].zone_status);
        }

        ZB_ZCL_IAS_ACE_SEND_GET_ZONE_STATUS_RESP_END(ptr, param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
          cmd_info->profile_id);

        ret = RET_BUSY;
      }
      else
      {
        ret = RET_ERROR;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_zone_status_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Arm Response command */
static zb_ret_t zb_zcl_ias_ace_arm_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_arm_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_arm_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_ARM_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_ARM_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_ARM_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_arm_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Zone ID Map Response command */
static zb_ret_t zb_zcl_ias_ace_get_zone_id_map_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_zone_id_map_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_zone_id_map_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_GET_ZONE_ID_MAP_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_GET_ZONE_ID_MAP_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_GET_ZONE_ID_MAP_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_zone_id_map_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Zone Info Response command */
static zb_ret_t zb_zcl_ias_ace_get_zone_info_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_zone_info_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_zone_info_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_GET_ZONE_INFO_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_GET_ZONE_INFO_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_GET_ZONE_INFO_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_zone_info_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Zone Status Changed command */
static zb_ret_t zb_zcl_ias_ace_zone_status_changed_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_zone_status_changed_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_zone_status_changed_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_ZONE_STATUS_CHANGED_REQ(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_ZONE_STATUS_CHANGED_REQ", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_ZONE_STATUS_CHANGED_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_zone_status_changed_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Panel Status Changed command */
static zb_ret_t zb_zcl_ias_ace_panel_status_changed_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_panel_status_changed_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_panel_status_changed_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_PANEL_STATUS_CHANGED_REQ(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_PANEL_STATUS_CHANGED_REQ", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_PANEL_STATUS_CHANGED_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_panel_status_changed_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Panel Status Response command */
static zb_ret_t zb_zcl_ias_ace_get_panel_status_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_panel_status_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_panel_status_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_GET_PANEL_STATUS_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_GET_PANEL_STATUS_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_GET_PANEL_STATUS_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_panel_status_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Set Bypassed Zone List command */
static zb_ret_t zb_zcl_ias_ace_set_bypassed_zone_list_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_set_bypassed_zone_list_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_set_bypassed_zone_list_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_SET_BYPASSED_ZONE_LIST(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)

  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_SET_BYPASSED_ZONE_LIST", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_SET_BYPASSED_ZONE_LIST_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_set_bypassed_zone_list_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Bypass Response command */
static zb_ret_t zb_zcl_ias_ace_bypass_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_bypass_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_bypass_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_BYPASS_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_BYPASS_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_BYPASS_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_bypass_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

/** @brief Get Zone Status Response command */
static zb_ret_t zb_zcl_ias_ace_get_zone_status_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_ace_get_zone_status_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_ace_get_zone_status_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_IAS_ACE_GET_GET_ZONE_STATUS_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ACE_GET_GET_ZONE_STATUS_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_IAS_ACE_GET_ZONE_STATUS_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_ace_get_zone_status_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

zb_bool_t zb_zcl_process_ias_ace_specific_commands_srv(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;

  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ias_ace_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1,
            "> zb_zcl_process_ias_ace_specific_commands_srv: param %d, cmd %d",
            (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_IAS_ACE == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_IAS_ACE_ARM_ID:
    ret = zb_zcl_ias_ace_arm_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_BYPASS_ID:
    ret = zb_zcl_ias_ace_bypass_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_EMERGENCY_ID:
    ret = zb_zcl_ias_ace_emergency_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_FIRE_ID:
    ret = zb_zcl_ias_ace_fire_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_PANIC_ID:
    ret = zb_zcl_ias_ace_panic_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_ZONE_ID_MAP_ID:
    ret = zb_zcl_ias_ace_get_zone_id_map_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_ZONE_INFO_ID:
    ret = zb_zcl_ias_ace_get_zone_info_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_PANEL_STATUS_ID:
    ret = zb_zcl_ias_ace_get_panel_status_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_BYPASSED_ZONE_LIST_ID:
    ret = zb_zcl_ias_ace_get_bypassed_zone_list_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_ZONE_STATUS_ID:
    ret = zb_zcl_ias_ace_get_zone_status_handler(param, &cmd_info);
    break;
  default:
    processed = ZB_FALSE;
    break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_ias_ace_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_ias_ace_specific_commands_srv: processed %d",
            (FMT__D, processed));

  return processed;
}

zb_bool_t zb_zcl_process_ias_ace_specific_commands_cli(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;

  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ias_ace_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1,
            "> zb_zcl_process_ias_ace_specific_commands_cli: param %d, cmd %d",
            (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_IAS_ACE == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_IAS_ACE_ARM_RESP_ID:
    ret = zb_zcl_ias_ace_arm_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_ZONE_ID_MAP_RESP_ID:
    ret = zb_zcl_ias_ace_get_zone_id_map_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_ZONE_INFO_RESP_ID:
    ret = zb_zcl_ias_ace_get_zone_info_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_ZONE_STATUS_CHANGED_ID:
    ret = zb_zcl_ias_ace_zone_status_changed_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_PANEL_STATUS_CHANGED_ID:
    ret = zb_zcl_ias_ace_panel_status_changed_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_PANEL_STATUS_RESPONSE_ID:
    ret = zb_zcl_ias_ace_get_panel_status_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_SET_BYPASSED_ZONE_LIST_ID:
    ret = zb_zcl_ias_ace_set_bypassed_zone_list_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_BYPASS_RESPONSE_ID:
    ret = zb_zcl_ias_ace_bypass_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_IAS_ACE_GET_ZONE_STATUS_RESPONSE_ID:
    ret = zb_zcl_ias_ace_get_zone_status_resp_handler(param, &cmd_info);
    break;

  default:
    processed = ZB_FALSE;
    break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_ias_ace_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_ias_ace_specific_commands_cli: processed %d",
            (FMT__D, processed));

  return processed;
}

/**
 *  @} internal
*/
#endif /* ZB_ZCL_SUPPORT_CLUSTER_IAS_ACE */
