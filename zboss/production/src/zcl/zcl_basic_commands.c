/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZCL Basic cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2061

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_BASIC)

#include "zb_aps.h"
#include "zb_zcl.h"
#include "zb_zdo.h"
#include "zcl/zb_zcl_basic.h"

static zb_uint8_t gs_basic_commands[] = { ZB_ZCL_CLUSTER_ID_BASIC_SERVER_ROLE_RECEIVED_CMD_LIST };

static zb_discover_cmd_list_t gs_basic_server_cmd_list =
{
  sizeof(gs_basic_commands), gs_basic_commands,
  0, NULL
};

static zb_discover_cmd_list_t gs_basic_client_cmd_list =
{
  0, NULL,
  sizeof(gs_basic_commands), gs_basic_commands
};

zb_ret_t check_value_basic_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_basic_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_basic_specific_commands_cli(zb_uint8_t param);

void zb_zcl_basic_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_BASIC,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_basic_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_basic_specific_commands_srv);
}

void zb_zcl_basic_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_BASIC,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_basic_specific_commands_cli);
}

zb_ret_t check_value_basic_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}
/* Invoke User App for "Reset to Factory Defaults" command
 */
static void zb_zcl_basic_reset_invoke_user_app(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_basic_reset_invoke_user_app param %hd", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  if ( ZCL_CTX().set_default_attr_cb != NULL )
  {
    (ZCL_CTX().set_default_attr_cb)(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint);
  }
  else
  {
    ZB_ZCL_RESET_TO_FACTORY_DEFAULTS_USER_APP(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, result);
  }

  ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, result==RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_HW_FAIL);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_basic_reset_invoke_user_app", (FMT__0));
}

zb_bool_t zb_zcl_process_basic_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_basic_client_cmd_list;
    return ZB_TRUE;
  }
  return ZB_FALSE;
}

zb_bool_t zb_zcl_process_basic_specific_commands_srv(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_basic_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_basic_specific_commands_srv: param %hd, cmd %hd",
             (FMT__H_H, param, cmd_info.cmd_id));

  switch (cmd_info.cmd_id)
  {
    case ZB_ZCL_CMD_BASIC_RESET_ID:
      TRACE_MSG(TRACE_ZCL3, "Processed RESET command", (FMT__0));
      ZB_SCHEDULE_CALLBACK(zb_zcl_basic_reset_invoke_user_app, param);
      status = RET_BUSY;
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
      ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

      ZB_ZCL_SEND_DEFAULT_RESP( param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.profile_id,
          ZB_ZCL_CLUSTER_ID_BASIC,
          cmd_info.seq_number,
          cmd_info.cmd_id,
          status==RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_FIELD);
    }
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_basic_specific_commands|_srv: processed %d",
            (FMT__D, processed));

  return processed;
}
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_BASIC*/
