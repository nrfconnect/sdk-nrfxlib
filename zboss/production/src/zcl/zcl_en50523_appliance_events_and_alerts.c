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
/* PURPOSE: ZCL Identify cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2071

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_EN50523_APPLIANCE_EVENTS_AND_ALERTS)

#include "zb_aps.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_en50523_appliance_events_and_alerts.h"

static zb_uint8_t gs_appl_ev_and_alerts_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_APPL_EV_AND_ALERTS_SERVER_ROLE_GENERATED_CMD_LIST
};

static zb_uint8_t gs_appl_ev_and_alerts_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_APPL_EV_AND_ALERTS_SERVER_ROLE_RECEIVED_CMD_LIST
};

static zb_discover_cmd_list_t gs_appl_ev_and_alerts_client_cmd_list =
{
  sizeof(gs_appl_ev_and_alerts_server_generated_commands), gs_appl_ev_and_alerts_server_generated_commands,
  sizeof(gs_appl_ev_and_alerts_server_received_commands), gs_appl_ev_and_alerts_server_received_commands
};

static zb_discover_cmd_list_t gs_appl_ev_and_alerts_server_cmd_list =
{
  sizeof(gs_appl_ev_and_alerts_server_received_commands), gs_appl_ev_and_alerts_server_received_commands,
  sizeof(gs_appl_ev_and_alerts_server_generated_commands), gs_appl_ev_and_alerts_server_generated_commands
};


zb_bool_t zb_zcl_process_en50523_appliance_events_and_alerts_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_en50523_appliance_events_and_alerts_cli(zb_uint8_t param);

void zb_zcl_en50523_appliance_events_and_alerts_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_en50523_appliance_events_and_alerts_srv);
}

void zb_zcl_en50523_appliance_events_and_alerts_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_en50523_appliance_events_and_alerts_cli);
}

static zb_zcl_status_t zb_zcl_en50523_appliance_events_and_alerts_map_ret_code_to_zcl_status(zb_ret_t ret_code)
{
  ZB_ASSERT(ret_code != RET_BUSY);

  return zb_zcl_get_zcl_status_from_ret(ret_code);
}

static zb_ret_t zb_zcl_en50523_appliance_events_and_alerts_get_alerts_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t    ret = RET_OK;
  zb_uint8_t *ptr;
  zb_uint16_t i;
  zb_zcl_en50523_appl_ev_and_alerts_get_alerts_resp_t pl_out;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_get_alerts_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_out, sizeof(pl_out));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_CB_ID, RET_OK, cmd_info, NULL, &pl_out);

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_SEND_GET_ALERTS_RESP_START(param,
      cmd_info->seq_number, pl_out.alerts_count, ptr);

    for (i = 0; i < ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_ALERTS_COUNT_GET_NUM(pl_out.alerts_count); i++)
    {
      ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_SEND_GET_ALERTS_RESP_ADD(ptr, &pl_out.alert_struct[i]);
    }

    ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_SEND_GET_ALERTS_RESP_END(ptr, param,
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

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_en50523_appliance_events_and_alerts_get_alerts_handler ret %hx", (FMT__H, ret));

  return ret;
}

static zb_ret_t zb_zcl_en50523_appliance_events_and_alerts_get_alerts_resp_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_en50523_appl_ev_and_alerts_get_alerts_resp_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_get_alerts_resp_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_GET_ALERTS_RESP(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_GET_ALERTS_RESP", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_RESP_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_get_alerts_resp_handler ret %hx", (FMT__H, ret));

  return ret;
}

static zb_ret_t zb_zcl_en50523_appliance_events_and_alerts_alerts_notificaton_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_en50523_appl_ev_and_alerts_alerts_notif_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_alerts_notificaton_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_NOTIF(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_NOTIF", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_ALERTS_NOTIFICATION_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_alerts_notificaton_handler ret %hx", (FMT__H, ret));

  return ret;
}

static zb_ret_t zb_zcl_en50523_appliance_events_and_alerts_event_notificaton_handler(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_en50523_appl_ev_and_alerts_event_notif_t pl_in;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_event_notificaton_handler %hx", (FMT__H, param));

  ZB_BZERO(&pl_in, sizeof(pl_in));

  ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_EVENT_NOTIF(&pl_in, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_GET_EVENT_NOTIF", (FMT__0));

    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
      ZB_ZCL_EN50523_APPL_EV_AND_ALERTS_EVENT_NOTIFICATION_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

    if (ZCL_CTX().device_cb != NULL)
    {
      (ZCL_CTX().device_cb)(param);
    }

    ret = (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK ? RET_OK : RET_ERROR);
  }

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_en50523_appliance_events_and_alerts_event_notificaton_handler ret %hx", (FMT__H, ret));

  return ret;
}

zb_bool_t zb_zcl_process_en50523_appliance_events_and_alerts_srv(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;

  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_appl_ev_and_alerts_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1,
      "> zb_zcl_process_en50523_appliance_events_and_alerts_srv: param %hd", (FMT__H, param));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_ID:
    ret = zb_zcl_en50523_appliance_events_and_alerts_get_alerts_handler(param, &cmd_info);
    break;

  default:
    processed = ZB_FALSE;
    break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_en50523_appliance_events_and_alerts_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_en50523_appliance_events_and_alerts_srv: processed %d",
      (FMT__D, processed));

  return processed;
}

zb_bool_t zb_zcl_process_en50523_appliance_events_and_alerts_cli(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;

  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_appl_ev_and_alerts_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1,
      "> zb_zcl_process_en50523_appliance_events_and_alerts_cli: param %hd", (FMT__H, param));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);

  switch (cmd_info.cmd_id)
  {
  case ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_GET_ALERTS_RESP_ID:
    ret = zb_zcl_en50523_appliance_events_and_alerts_get_alerts_resp_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_ALERTS_NOTIFICATION_ID:
    ret = zb_zcl_en50523_appliance_events_and_alerts_alerts_notificaton_handler(param, &cmd_info);
    break;
  case ZB_ZCL_CMD_EN50523_APPL_EV_AND_ALERTS_EVENT_NOTIFICATION_ID:
    ret = zb_zcl_en50523_appliance_events_and_alerts_event_notificaton_handler(param, &cmd_info);
    break;

  default:
    processed = ZB_FALSE;
    break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_en50523_appliance_events_and_alerts_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_en50523_appliance_events_and_alerts_cli: processed %d",
      (FMT__D, processed));

  return processed;
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_EN50523_APPLIANCE_EVENTS_AND_ALERTS */
