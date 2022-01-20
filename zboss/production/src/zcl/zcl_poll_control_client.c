/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: poll control cluster implementation, client role
*/

#define ZB_TRACE_FILE_ID 2077

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL)

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_poll_control.h"

zb_uint8_t gs_poll_control_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_poll_control_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_POLL_CONTROL_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_poll_control_client_cmd_list =
{
  sizeof(gs_poll_control_client_received_commands), gs_poll_control_client_received_commands,
  sizeof(gs_poll_control_client_generated_commands), gs_poll_control_client_generated_commands
};

void zb_zcl_set_fast_poll_timeout(zb_uint8_t ep, zb_uint16_t fast_poll_timeout)
{
  zb_zcl_attr_t *attr_desc;
  zb_zcl_poll_control_client_status_t *client_data;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_set_fast_poll_timeout ep %hd, timeout %d",
            (FMT__H_D, ep, fast_poll_timeout));

  attr_desc = zb_zcl_get_attr_desc_a(
    ep, ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_STATUS_DATA_ID);
  ZB_ASSERT(attr_desc);
  client_data = (zb_zcl_poll_control_client_status_t*)attr_desc->data_p;

  client_data->fast_poll_timeout = fast_poll_timeout;

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_set_fast_poll_timeout", (FMT__0));
}

/** @brief Stop fast poll and schedule next Check-in command */
static void zb_zcl_poll_control_stop_fast_poll_client(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc;
  zb_zcl_poll_control_client_status_t *client_data;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_poll_control_stop_fast_poll_client %hx", (FMT__H, endpoint));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_STATUS_DATA_ID);
  ZB_ASSERT(attr_desc);
  client_data = (zb_zcl_poll_control_client_status_t*)attr_desc->data_p;

  client_data->is_poll_mode = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_poll_control_stop_fast_poll_client", (FMT__0));
}

/** @brief Check-in command */
static zb_ret_t check_in_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;
  zb_zcl_poll_control_client_status_t *client_data;
  zb_zcl_attr_t *attr_desc;
  zb_bool_t   start_fast_poll;
  zb_uint16_t timeout;

  TRACE_MSG(TRACE_ZCL1, "> check_in_handler param %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_POLL_CONTROL_STATUS_DATA_ID);
  ZB_ASSERT(attr_desc);
  client_data = (zb_zcl_poll_control_client_status_t*)attr_desc->data_p;

  /* If client in poll mode we won't start fast poll, else - ask user app */
  start_fast_poll = ZB_FALSE;
  timeout = 0;

  if (!client_data->is_poll_mode)
  {
    zb_ret_t    cb_result;

    ZB_ZCL_POLL_CONTROL_CLI_CALL_USER_APP(
      param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      client_data->fast_poll_timeout,
      cb_result);

    if (cb_result == RET_OK)
    {
      start_fast_poll = ZB_TRUE;
      timeout = client_data->fast_poll_timeout;
    }
  }

  ZB_ZCL_POLL_CONTROL_SEND_CHECK_IN_RES(
    param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
        cmd_info.profile_id,
        ZB_FALSE,
        NULL /*zb_zcl_poll_control_fast_pool_stop_scheduler*/,
    start_fast_poll,
    timeout);

  client_data->is_poll_mode = start_fast_poll;

  if (start_fast_poll)
  {
    /* Schedule fast poll stop */
    ZB_SCHEDULE_ALARM(zb_zcl_poll_control_stop_fast_poll_client, endpoint,
        ZB_QUARTERECONDS_TO_BEACON_INTERVAL(client_data->fast_poll_timeout));
  }

  ret = RET_BUSY;   /* not need send answer yet */

  TRACE_MSG(TRACE_ZCL1, "< check_in_handler %hx", (FMT__H, ret));
  return ret;
}

/* specific commands handling - client side */
zb_bool_t zb_zcl_process_poll_control_specific_commands_cli(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_poll_control_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_poll_control_specific_commands_cli: param %hd, cmd %hd",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_POLL_CONTROL == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);

  switch( cmd_info.cmd_id )
  {
  case ZB_ZCL_CMD_POLL_CONTROL_CHECK_IN_ID:
    status = check_in_handler(param);
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
          ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
          cmd_info.seq_number,
          cmd_info.cmd_id,
          status==RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_FIELD);
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_poll_control_specific_commands_cli: processed %d",
             (FMT__D, processed));
  return processed;
}

void zb_zcl_poll_control_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_POLL_CONTROL,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_poll_control_specific_commands_cli);
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_POLL_CONTROL*/
