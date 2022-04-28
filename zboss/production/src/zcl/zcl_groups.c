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
/* PURPOSE: ZCL Groups cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2067

#include "zb_common.h"

#if defined ZB_ENABLE_ZCL

#include "zb_zcl.h"
#include "zb_zdo.h"
#include "zb_aps.h"

#if defined ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING \
    || defined ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING_CLIENT
zb_uint8_t gs_groups_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_GROUPS_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_groups_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_GROUPS_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_groups_client_cmd_list =
{
  sizeof(gs_groups_client_received_commands), gs_groups_client_received_commands,
  sizeof(gs_groups_server_received_commands), gs_groups_server_received_commands
};

zb_discover_cmd_list_t gs_groups_server_cmd_list =
{
  sizeof(gs_groups_server_received_commands), gs_groups_server_received_commands,
  sizeof(gs_groups_client_received_commands), gs_groups_client_received_commands
};
#endif  /* defined ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING
         * || defined ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING_CLIENT */

#if defined (ZB_ZCL_SUPPORT_CLUSTER_GROUPS)

#ifdef ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING

static void add_group_handler(zb_uint8_t param, zb_bool_t check_identifying);
static void view_group_handler(zb_uint8_t param);
static void get_group_membership_handler(zb_uint8_t param);
static void remove_group_handler(zb_uint8_t param);
static void remove_all_groups_handler(zb_uint8_t param);

static zb_ret_t aps_status_to_zcl_status(zb_ret_t aps_status);
static void dummy_handler(zb_uint8_t unused);

zb_ret_t check_value_groups_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_groups_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_groups_commands_cli(zb_uint8_t param);

void zb_zcl_groups_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_GROUPS,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_groups_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_groups_commands_srv);
}

void zb_zcl_groups_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_GROUPS,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_groups_commands_cli);
}

zb_ret_t check_value_groups_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}
/*
zb_bool_t zb_zcl_process_groups_commands_mixed(zb_uint8_t param)
{
  zb_bool_t processed = ZB_FALSE;
  processed = zb_zcl_process_groups_commands_srv(param);
#ifdef ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING_CLIENT
  if (!processed)
  {
    processed = zb_zcl_process_groups_commands_cli(param);
  }
#endif
  return processed;
}
*/
zb_bool_t zb_zcl_process_groups_commands_srv(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t *cmd_info;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_groups_server_cmd_list;
    return ZB_TRUE;
  }

  cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_groups_commands_srv: param %hd",(FMT__H, param));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_GROUPS == cmd_info->cluster_id);

  TRACE_MSG(TRACE_ZCL2, "direction %hd, cmd_id %hd",
            (FMT__H_H, cmd_info->cmd_direction, cmd_info->cmd_id));
  if (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV)
  {
    switch (cmd_info->cmd_id)
    {
      case ZB_ZCL_CMD_GROUPS_ADD_GROUP:
        add_group_handler(param, ZB_FALSE);
        break;

      case ZB_ZCL_CMD_GROUPS_VIEW_GROUP:
        view_group_handler(param);
        break;

      case ZB_ZCL_CMD_GROUPS_GET_GROUP_MEMBERSHIP:
        get_group_membership_handler(param);
        break;

      case ZB_ZCL_CMD_GROUPS_REMOVE_GROUP:
        remove_group_handler(param);
        break;

      case ZB_ZCL_CMD_GROUPS_REMOVE_ALL_GROUPS:
        remove_all_groups_handler(param);
        break;

      case ZB_ZCL_CMD_GROUPS_ADD_GROUP_IF_IDENTIFYING:
        add_group_handler(param, ZB_TRUE);
        break;

      default:
        TRACE_MSG(TRACE_ZCL2, "cmd not supported", (FMT__0));
        processed = ZB_FALSE;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "direction - to client, skip cmd", (FMT__0));
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_groups_commands_srv: ret %hd", (FMT__H, processed));
  return processed;
}

static void add_group_send_default_resp(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info, zb_ret_t status)
{
  TRACE_MSG(TRACE_ZCL1, ">> add_group_send_default_resp", (FMT__0));

  TRACE_MSG(TRACE_ZCL2, "disable_default_response %hd", (FMT__H, cmd_info->disable_default_response));

  if (ZB_ZCL_CHECK_IF_SEND_DEFAULT_RESP(*cmd_info, status))
  {
    ZB_ZCL_SEND_DEFAULT_RESP(
      param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      cmd_info->seq_number,
      cmd_info->cmd_id,
      status);
  }
  else
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< add_group_send_default_resp", (FMT__0));
}

static void add_group_cb_send_default_resp(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_apsme_add_group_conf_t *conf_param;
  zb_ret_t status;

  TRACE_MSG(TRACE_ZCL1, ">> add_group_cb_send_default_resp, param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, zb_buf_begin(param), sizeof(zb_zcl_parsed_hdr_t));
  conf_param = ZB_BUF_GET_PARAM(param, zb_apsme_add_group_conf_t);
  status = aps_status_to_zcl_status(conf_param->status);

  /* Send default response, if enabled */
  add_group_send_default_resp(param, &cmd_info, status);

  TRACE_MSG(TRACE_ZCL1, "<< add_group_cb_send_default_resp", (FMT__0));
}

static void add_group_send_add_group_resp(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info,
  zb_ret_t status, zb_uint16_t group_id)
{
  zb_uint8_t *resp_data;

  TRACE_MSG(TRACE_ZCL1, ">> add_group_send_add_group_resp", (FMT__0));

  /* Construct response packet header */
  resp_data = ZB_ZCL_START_PACKET(param);

  /* NOTE: currently, manufacturer specific is not supported */
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(resp_data);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(resp_data, cmd_info->seq_number, ZB_ZCL_CMD_GROUPS_ADD_GROUP_RES);

  /* Add group response format, ZCL8 spec 3.6.2.4.1 */
  /* | status 1b | group id 2b | */

  ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, group_id);

  ZB_ZCL_FINISH_N_SEND_PACKET(param, resp_data,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
    cmd_info->profile_id,
    cmd_info->cluster_id,
    NULL);

  TRACE_MSG(TRACE_ZCL1, "<< add_group_send_add_group_resp", (FMT__0));
}

static void add_group_cb_send_add_group_resp(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_apsme_add_group_conf_t *conf_param;
  zb_ret_t status;

  TRACE_MSG(TRACE_ZCL1, ">> add_group_cb_send_add_group_resp, param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, zb_buf_begin(param), sizeof(zb_zcl_parsed_hdr_t));

  conf_param = ZB_BUF_GET_PARAM(param, zb_apsme_add_group_conf_t);
  status = aps_status_to_zcl_status(conf_param->status);

  add_group_send_add_group_resp(param, &cmd_info, status, conf_param->group_address);

  TRACE_MSG(TRACE_ZCL1, "<< add_group_cb_send_add_group_resp", (FMT__0));
}

static void add_group_handler(zb_uint8_t param, zb_bool_t check_identifying)
{
  zb_zcl_groups_add_group_req_t add_group_req;
  zb_apsme_add_group_req_t *aps_req;
  zb_ret_t status = ZB_ZCL_STATUS_SUCCESS;
  zb_bool_t add_group = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_parsed_hdr_t *resp_cmd_info = NULL;
  zb_bool_t respond_with_default_resp = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> add_group_handler param %hd, check_identifying %hd",
            (FMT__H_H, param, check_identifying));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  /* ZCL8 spec. 3.6.2.3.7.2 Effect on Receipt (for Add Group If Identifying Command)
   * 1. The device verifies that it is currently identifying itself...
   */
  if (check_identifying)
  {
    add_group = zb_zcl_is_identifying(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint)
      ? ZB_TRUE : ZB_FALSE;
    TRACE_MSG(TRACE_ZCL2, "add_group %hd", (FMT__H, add_group));
  }

  if (add_group)
  {
    ZB_ZCL_GROUPS_GET_ADD_GROUP_REQ(param, add_group_req);

    TRACE_MSG(
        TRACE_ZCL2,
        "group_id %d, ep %hd",
        (FMT__D_H, add_group_req.group_id, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint));

    /* ZCL8 spec. 3.6.2.3.2.2, 3.6.2.3.7.2: The device verifies that the Group ID field contains a valid group identifier
     * in the range 0x0001 – 0xfff7. If the Group ID field contains a group identifier outside this range,
     * the status SHALL be INVALID_VALUE and the device continues from step 5.*/
    if (add_group_req.group_id >= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MIN_VALUE &&
        add_group_req.group_id <= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MAX_VALUE)
    {
      zb_buf_reuse(param);

      aps_req = ZB_BUF_GET_PARAM(param, zb_apsme_add_group_req_t);
      aps_req->group_address = add_group_req.group_id;
      aps_req->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

      /* According to ZB spec, we should use appropriate ZDO functions to
       * call APS. In this case command result is returned via
       * confirmation - call to callback.
       * To simplify current implementation, call APS directly:
       * result will be received immediately.
       * To improve source code and to use ZDO, need to implement hash to
       * store cmd_info - it is need to send response to peer. */
      /* zb_apsme_add_group_request calls ZDO_RUN_CALLBACK_BY_INDEX that
         kills buffer if callback not found; register dummy handler
         to avoid this situation. */

      aps_req->confirm_cb = NULL;

      if (!check_identifying)
      {
        if (ZB_APS_FC_GET_DELIVERY_MODE(cmd_info.addr_data.common_data.fc) == ZB_APS_DELIVERY_UNICAST)
        {
          aps_req->confirm_cb = add_group_cb_send_add_group_resp;
        }
      }
      else
      {
        aps_req->confirm_cb = add_group_cb_send_default_resp;
      }

      resp_cmd_info = zb_buf_initial_alloc(param, sizeof(zb_zcl_parsed_hdr_t));
      ZB_MEMCPY(resp_cmd_info, &cmd_info, sizeof(zb_zcl_parsed_hdr_t));

      zb_zdo_add_group_req(param);
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "Error, invalid group ID for add group request", (FMT__0));
      status = ZB_ZCL_STATUS_INVALID_VALUE;

      if (!check_identifying)
      {
        if (ZB_APS_FC_GET_DELIVERY_MODE(cmd_info.addr_data.common_data.fc) == ZB_APS_DELIVERY_UNICAST)
        {
          add_group_send_add_group_resp(param, &cmd_info, status, add_group_req.group_id);
        }
        else
        {
          /* Broadcast or Groupcast Add Group - drop packet */
          zb_buf_free(param);
        }
      }
      else
      {
        respond_with_default_resp = ZB_TRUE;
      }
    }
  }
  else
  /* ZCL8 spec. 3.6.2.3.7.2 Effect on Receipt (for Add Group If Identifying Command)
   * 1. ... If the device it not currently identifying itself ...
   */
  {
    respond_with_default_resp = ZB_TRUE;
  } /* id add_group */

  if (respond_with_default_resp)
  {
    if (!add_group)
    {
      TRACE_MSG(TRACE_ZCL2, "Can't add group - no identifying", (FMT__0));
      /* ZCL8 spec. 3.6.2.3.7.2 Effect on Receipt (for Add Group If Identifying Command)
       * 1. ...If the device it not currently identifying
       * itself, the Add Group If Identifying command was received as unicast and a default response is
       * requested, the device SHALL generate a Default Response command with the Status field set to
       * SUCCESS and SHALL transmit it back to the originator of the Add Group If Identifying command...
       * Test spec, G-TC-02S - status should be SUCCESS.
       */
      status = ZB_ZCL_STATUS_SUCCESS;
    }

    add_group_send_default_resp(param, &cmd_info, status);
  }

  TRACE_MSG(TRACE_ZCL1, "<< add_group_handler", (FMT__0));
}


static void view_group_handler(zb_uint8_t param)
{
  zb_zcl_groups_view_group_req_t view_group_req;
  zb_uint8_t *resp_data;
  zb_uint8_t status = ZB_ZCL_STATUS_SUCCESS;
  zb_apsme_get_group_membership_req_t *aps_req;
  zb_apsme_get_group_membership_conf_t *conf;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, ">> view_group_handler %hd", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  ZB_ZCL_GROUPS_GET_VIEW_GROUP_REQ(param, view_group_req);

  if (view_group_req.group_id >= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MIN_VALUE &&
      view_group_req.group_id <= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MAX_VALUE)
  {
    zb_buf_reuse(param);
    aps_req = zb_buf_initial_alloc(param, sizeof(zb_apsme_get_group_membership_req_t));

    aps_req->n_groups = 1;
    aps_req->groups[0] = view_group_req.group_id;

    /* TODO: do not rely on function synchronicity and send response from confirm_cb */
    /* See comment to zb_apsme_add_group_request() call */
    aps_req->confirm_cb = dummy_handler;
    zb_apsme_get_group_membership_request(param);
    conf = ZB_BUF_GET_PARAM(param, zb_apsme_get_group_membership_conf_t);

    TRACE_MSG(TRACE_ZCL2, "n_groups %hd", (FMT__H, conf->n_groups));
    if (!conf->n_groups)
    {
      status = ZB_ZCL_STATUS_NOT_FOUND;
    }
    TRACE_MSG(TRACE_ZCL2, "zcl status %hx", (FMT__H, status));
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Error, invalid group ID for view group request", (FMT__0));
    status = ZB_ZCL_STATUS_INVALID_VALUE;
  }

  /** [ZB_ZCL_CONSTRUCT_FRAME_HEADER] */
  /* Construct response packet header */
  resp_data = ZB_ZCL_START_PACKET(param);

  /* NOTE: currently, manufacturer specific is not supported */
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(resp_data);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(
      resp_data,
      cmd_info.seq_number,
      ZB_ZCL_CMD_GROUPS_VIEW_GROUP_RES);
  /** [ZB_ZCL_CONSTRUCT_FRAME_HEADER] */

  /* View group response format, ZCL8 spec 3.6.2.4.2.1 */
  /* | status 1b | group id 2b | Group name XXb| */

  ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, view_group_req.group_id);
  /* Group name currently is not supported */
  ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_NULL_STRING);

  ZB_ZCL_FINISH_N_SEND_PACKET(param, resp_data,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      cmd_info.profile_id,
      cmd_info.cluster_id,
      NULL);

  TRACE_MSG(TRACE_ZCL1, "<< view_group_handler", (FMT__0));
}


static void get_group_membership_handler(zb_uint8_t param)
{
  zb_zcl_groups_get_group_membership_req_t *get_member_req;
  zb_uint8_t *resp_data;
  zb_apsme_get_group_membership_req_t *aps_req;
  zb_apsme_get_group_membership_conf_t *conf;
  zb_ushort_t i;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t recv_pkt_is_valid = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> get_group_membership_handler", (FMT__0));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  ZB_ZCL_GROUPS_GET_GROUP_MEMBERSHIP_REQ(param, get_member_req);
  TRACE_MSG(TRACE_ZCL2, "get_member_req %p", (FMT__P, get_member_req));

  recv_pkt_is_valid = (get_member_req) ? ZB_TRUE : ZB_FALSE;

  if (recv_pkt_is_valid)
  {
    TRACE_MSG(TRACE_ZCL2, "group count %hd", (FMT__H, get_member_req->group_count));

    /* use runtime buffer - to simplify passing parameters, just copy
     * it to the beginning of this buffer */
    zb_buf_reuse(ZCL_CTX().runtime_buf);
    aps_req = zb_buf_initial_alloc(
        ZCL_CTX().runtime_buf,
        (   sizeof(zb_apsme_get_group_membership_req_t)
            + (get_member_req->group_count - 1) * sizeof(zb_uint16_t)));

    aps_req->n_groups = get_member_req->group_count;
    ZB_MEMCPY(aps_req->groups, get_member_req->group_id,
              get_member_req->group_count * sizeof(zb_uint16_t));

    /* TODO: do not rely on function synchronicity and send response from confirm_cb */
    aps_req->confirm_cb = dummy_handler;
    zb_apsme_get_group_membership_request(ZCL_CTX().runtime_buf);

    TRACE_MSG(TRACE_ZCL2, "get conf...", (FMT__0));
    conf = ZB_BUF_GET_PARAM(ZCL_CTX().runtime_buf, zb_apsme_get_group_membership_conf_t);
  }

  /* Construct response packet header */
  resp_data = ZB_ZCL_START_PACKET(param);

  /* NOTE: currently, manufacturer specific is not supported */
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(resp_data);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(
      resp_data,
      cmd_info.seq_number,
      ZB_ZCL_CMD_GROUPS_GET_GROUP_MEMBERSHIP_RES);

  if (recv_pkt_is_valid)
  {
    TRACE_MSG(
        TRACE_ZCL2,
        "capacity %hd, n_groups %hd",
        (FMT__H_H, conf->capacity, conf->n_groups));

    /* Get group membership response format, ZCL8 spec 3.6.2.4.3.1 */
    /* | capacity 1b | group count 1b | Group id 2b x NN | */

    ZB_ZCL_PACKET_PUT_DATA8(resp_data, conf->capacity);
    ZB_ZCL_PACKET_PUT_DATA8(resp_data, conf->n_groups);
    for (i = 0; i < conf->n_groups; i++)
    {
      ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, conf->groups[i]);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Error in received packet, send empty response", (FMT__0));
    ZB_ZCL_PACKET_PUT_DATA8(resp_data, 0);
    ZB_ZCL_PACKET_PUT_DATA8(resp_data, 0);
  }

  ZB_ZCL_FINISH_N_SEND_PACKET(param, resp_data,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      cmd_info.profile_id,
      cmd_info.cluster_id,
      NULL);

  TRACE_MSG(TRACE_ZCL1, "<< view_group_handler", (FMT__0));
}


static void remove_group_send_remove_group_resp(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info,
  zb_ret_t status, zb_uint16_t group_id)
{
  zb_uint8_t *resp_data;

  TRACE_MSG(TRACE_ZCL1, ">> remove_group_send_remove_group_resp", (FMT__0));

  /* Construct response packet header */
  resp_data = ZB_ZCL_START_PACKET(param);

  /* NOTE: currently, manufacturer specific is not supported */
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(resp_data);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(
      resp_data,
      cmd_info->seq_number,
      ZB_ZCL_CMD_GROUPS_REMOVE_GROUP_RES);

  /* Remove group response format, ZCL8 spec 3.6.2.4.4.1 */
  /* | status 1b | group id 2b | */

  ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, group_id);

  ZB_ZCL_FINISH_N_SEND_PACKET(param, resp_data,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      NULL);

  TRACE_MSG(TRACE_ZCL1, "<< remove_group_send_remove_group_resp", (FMT__0));
}

static void remove_group_cb_send_remove_group_resp(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_apsme_remove_group_conf_t *conf_param;
  zb_ret_t status;

  TRACE_MSG(TRACE_ZCL1, ">> remove_group_cb_send_remove_group_resp, param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, zb_buf_begin(param), sizeof(zb_zcl_parsed_hdr_t));

  conf_param = ZB_BUF_GET_PARAM(param, zb_apsme_remove_group_conf_t);
  status = aps_status_to_zcl_status(conf_param->status);

  TRACE_MSG(TRACE_ZCL2, "zcl status %hx", (FMT__H, status));

  remove_group_send_remove_group_resp(param, &cmd_info, status, conf_param->group_address);

  TRACE_MSG(TRACE_ZCL1, "<< remove_group_cb_send_remove_group_resp", (FMT__0));
}


static void remove_group_handler(zb_uint8_t param)
{
  zb_zcl_groups_remove_group_req_t rem_group_req;
  zb_apsme_remove_group_req_t *aps_req;
  zb_ret_t status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_parsed_hdr_t *resp_cmd_info;

  TRACE_MSG(TRACE_ZCL1, ">> remove_group_handler", (FMT__0));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  ZB_ZCL_GROUPS_GET_REMOVE_GROUP_REQ(param, rem_group_req);
    TRACE_MSG(TRACE_ZCL2, "group_id %d", (FMT__D, rem_group_req.group_id));

  zb_buf_reuse(param);

  resp_cmd_info = zb_buf_initial_alloc(param, sizeof(zb_zcl_parsed_hdr_t));
  ZB_MEMCPY(resp_cmd_info, &cmd_info, sizeof(zb_zcl_parsed_hdr_t));

  if (rem_group_req.group_id >= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MIN_VALUE &&
      rem_group_req.group_id <= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MAX_VALUE)
  {
    aps_req = ZB_BUF_GET_PARAM(param, zb_apsme_remove_group_req_t);
    aps_req->group_address = rem_group_req.group_id;
    aps_req->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

#ifdef ZB_ZCL_SUPPORT_CLUSTER_SCENES
    /* Remove associated scenes */
    zb_buf_get_out_delayed_ext(zb_zcl_scenes_remove_all_scenes_in_all_endpoints_by_group_id,
                               rem_group_req.group_id, 0);
#endif

    /* See comment for zb_apsme_add_group_request() call */
    aps_req->confirm_cb = remove_group_cb_send_remove_group_resp;
    zb_zdo_remove_group_req(param);
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Error, invalid group ID for remove group request", (FMT__0));
    status = ZB_ZCL_STATUS_INVALID_VALUE;

    remove_group_send_remove_group_resp(param, &cmd_info, status, rem_group_req.group_id);
  }

  TRACE_MSG(TRACE_ZCL1, "<< remove_group_handler", (FMT__0));
}


static void remove_all_groups_cb_send_default_resp(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_apsme_remove_all_groups_conf_t *conf_param;
  zb_uint8_t *resp_data;
  zb_ret_t status;

  TRACE_MSG(TRACE_ZCL1, ">> remove_all_groups_cb_send_default_resp, param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, zb_buf_begin(param), sizeof(zb_zcl_parsed_hdr_t));

  conf_param = ZB_BUF_GET_PARAM(param, zb_apsme_remove_all_groups_conf_t);

  /* Map APS status to ZCL status */
  status = aps_status_to_zcl_status(conf_param->status);

  /* If no groups founded answer with success status. See ZCL8 Spec  3.6.2.3.6.2 */
  if (status == ZB_ZCL_STATUS_NOT_FOUND)
  {
    status = ZB_ZCL_STATUS_SUCCESS;
  }

  TRACE_MSG(TRACE_ZCL2, "zcl status %hx", (FMT__H, status));

  /* Construct response packet header */
  resp_data = ZB_ZCL_START_PACKET(param);

  /* NOTE: currently, manufacturer specific is not supported */
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_RES_FRAME_CONTROL(resp_data);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(
      resp_data,
      cmd_info.seq_number,
      ZB_ZCL_CMD_GROUPS_REMOVE_GROUP_RES);

  /* Send default response, if enabled */
  TRACE_MSG(TRACE_ZCL2, "disable_default_response %hd", (FMT__H, cmd_info.disable_default_response));
  if (!cmd_info.disable_default_response)
  {
    ZB_ZCL_SEND_DEFAULT_RESP(
      param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      cmd_info.profile_id,
      cmd_info.cluster_id,
      cmd_info.seq_number,
      cmd_info.cmd_id,
      status);
  }
  else
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< remove_all_groups_cb_send_default_resp", (FMT__0));
}


static void remove_all_groups_handler(zb_uint8_t param)
{
  zb_apsme_remove_all_groups_req_t *aps_req;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_parsed_hdr_t *resp_cmd_info;

  TRACE_MSG(TRACE_ZCL1, ">> remove_all_groups_handler", (FMT__0));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  zb_buf_reuse(param);

  resp_cmd_info = zb_buf_initial_alloc(param, sizeof(zb_zcl_parsed_hdr_t));
  ZB_MEMCPY(resp_cmd_info, &cmd_info, sizeof(zb_zcl_parsed_hdr_t));

  aps_req = ZB_BUF_GET_PARAM(param, zb_apsme_remove_all_groups_req_t);
  aps_req->endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

#ifdef ZB_ZCL_SUPPORT_CLUSTER_SCENES
  /* Remove scenes associated with every group in every endpoint */
  zb_buf_get_out_delayed(zb_zcl_scenes_remove_all_scenes_in_all_endpoints);
#endif

  /* See comment for zb_apsme_add_group_request() call */
  aps_req->confirm_cb = remove_all_groups_cb_send_default_resp;
  zb_zdo_remove_all_groups_req(param);

  TRACE_MSG(TRACE_ZCL1, "<< remove_all_groups_handler", (FMT__0));
}


static zb_ret_t aps_status_to_zcl_status(zb_ret_t aps_status)
{
  zb_ret_t zcl_status;

  switch (aps_status)
  {
    case ERROR_CODE(ERROR_CATEGORY_APS, ZB_APS_STATUS_TABLE_FULL):
      zcl_status = ZB_ZCL_STATUS_INSUFF_SPACE;
      break;

    case ERROR_CODE(ERROR_CATEGORY_APS, ZB_APS_STATUS_INVALID_GROUP):
      zcl_status = ZB_ZCL_STATUS_NOT_FOUND;
      break;

    case ERROR_CODE(ERROR_CATEGORY_APS, ZB_APS_STATUS_SUCCESS):
      zcl_status = ZB_ZCL_STATUS_SUCCESS;
      break;

    default:
      zcl_status = ZB_ZCL_STATUS_FAIL;
      break;
  }

  return zcl_status;
}


/* Just do nothing, used as gag */
static void dummy_handler(zb_uint8_t unused)
{
  ZVUNUSED(unused);
}

#endif /* ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING */


#ifdef ZB_COMPILE_ZCL_SAMPLE
void send_add_group_cmd(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t group_id, zb_uint16_t dst_addr,
                             zb_uint8_t dst_ep, zb_uint8_t src_ep)
{

  ZB_ZCL_GROUPS_SEND_ADD_GROUP_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                               dst_ep, src_ep, prof_id, ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                               NULL, group_id);

}

void parse_add_group_res(zb_bufid_t buf)
{
  zb_zcl_groups_add_group_res_t *add_group_res;

  ZB_ZCL_GROUPS_GET_ADD_GROUP_RES(buf, add_group_res);

  TRACE_MSG(TRACE_ZCL1, "Response status %hd, group_id %d",
            (FMT__H_D, add_group_res->status, add_group_res->group_id));
}

void send_view_group_cmd(zb_bufid_t data_buf,zb_uint16_t prof_id, zb_uint16_t group_id, zb_uint16_t dst_addr,
                              zb_uint8_t dst_ep, zb_uint8_t src_ep)
{

  ZB_ZCL_GROUPS_SEND_VIEW_GROUP_REQ(
      data_buf,
      dst_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      dst_ep,
      src_ep,
      prof_id,
      ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
      NULL,
      group_id);

}

void parse_view_group_res(zb_bufid_t buf)
{
  zb_zcl_groups_view_group_res_t *view_group_res;

  ZB_ZCL_GROUPS_GET_VIEW_GROUP_RES(buf, view_group_res);

  TRACE_MSG(TRACE_ZCL1, "Response status %hd, group_id %d",
            (FMT__H_D, view_group_res->status, view_group_res->group_id));
}


void send_get_group_membership_cmd(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t dst_addr,
                                        zb_uint8_t dst_ep, zb_uint8_t src_ep)
{
  zb_uint8_t *ptr;

#define TEST_GROUP_COUNT 3
#define TEST_GROUP_ID1 12
#define TEST_GROUP_ID2 13
#define TEST_GROUP_ID3 14

  ZB_ZCL_GROUPS_INIT_GET_GROUP_MEMBERSHIP_REQ(
      data_buf,
      ptr,
      ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
      TEST_GROUP_COUNT);

  ZB_ZCL_GROUPS_ADD_ID_GET_GROUP_MEMBERSHIP_REQ(ptr, TEST_GROUP_ID1);
  ZB_ZCL_GROUPS_ADD_ID_GET_GROUP_MEMBERSHIP_REQ(ptr, TEST_GROUP_ID2);
  ZB_ZCL_GROUPS_ADD_ID_GET_GROUP_MEMBERSHIP_REQ(ptr, TEST_GROUP_ID3);

  ZB_ZCL_GROUPS_SEND_GET_GROUP_MEMBERSHIP_REQ(
    data_buf, ptr, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    dst_ep, src_ep, prof_id, NULL);

}

void send_remove_group_cmd(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t group_id, zb_uint16_t dst_addr,
                                zb_uint8_t dst_ep, zb_uint8_t src_ep)
{

  ZB_ZCL_GROUPS_SEND_REMOVE_GROUP_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    dst_ep, src_ep, prof_id, ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    NULL, group_id);

}

void parse_remove_group_res(zb_bufid_t buf)
{
  zb_zcl_groups_remove_group_res_t *rem_group_res;

  ZB_ZCL_GROUPS_GET_REMOVE_GROUP_RES(buf, rem_group_res);

  TRACE_MSG(TRACE_ZCL1, "Response status %hd, group_id %d",
            (FMT__H_D, rem_group_res->status, rem_group_res->group_id));
}

void send_remove_all_groups_cmd(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t dst_addr,
                                     zb_uint8_t dst_ep, zb_uint8_t src_ep)
{

  ZB_ZCL_GROUPS_SEND_REMOVE_ALL_GROUPS_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    dst_ep, src_ep, prof_id, ZB_ZCL_ENABLE_DEFAULT_RESPONSE, NULL);

}

void send_add_group_if_ident_cmd(
    zb_bufid_t data_buf,
    zb_uint16_t prof_id,
    zb_uint16_t group_id,
    zb_uint16_t dst_addr,
    zb_uint8_t dst_ep,
    zb_uint8_t src_ep)
{

  ZB_ZCL_GROUPS_SEND_ADD_GROUP_IF_IDENT_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                               dst_ep, src_ep, prof_id, ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                               NULL, group_id);

}


#endif /* ZB_COMPILE_ZCL_SAMPLE */

#endif /* ZB_ZCL_SUPPORT_CLUSTER_GROUPS */

#ifdef ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING_CLIENT

zb_bool_t zb_zcl_process_groups_commands_cli(zb_uint8_t param)
{
  zb_bool_t            processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t  cmd_info;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_groups_commands_cli: param %hd",(FMT__H, param));

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_groups_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_GROUPS);

  TRACE_MSG(TRACE_ZCL2, "direction %hd, cmd_id %hd",
            (FMT__H_H, cmd_info.cmd_direction, cmd_info.cmd_id));
  if (cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI)
  {
    switch (cmd_info.cmd_id)
    {
      case ZB_ZCL_CMD_GROUPS_ADD_GROUP_RES:
      case ZB_ZCL_CMD_GROUPS_VIEW_GROUP_RES:
      case ZB_ZCL_CMD_GROUPS_GET_GROUP_MEMBERSHIP:
      case ZB_ZCL_CMD_GROUPS_REMOVE_GROUP_RES:
        /* Do nothing, just send default response if necessary */
        if (!cmd_info.disable_default_response)
        {
          ZB_ZCL_SEND_DEFAULT_RESP(
            param,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
            ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
            cmd_info.profile_id,
            cmd_info.cluster_id,
            cmd_info.seq_number,
            cmd_info.cmd_id,
            ZB_ZCL_STATUS_SUCCESS);
        }
        else
        {
          zb_buf_free(param);
        }
        processed = ZB_TRUE;
        break;

      default:
        TRACE_MSG(TRACE_ZCL2, "cmd not supported", (FMT__0));
        processed = ZB_FALSE;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "direction - to server, skip cmd", (FMT__0));
    processed = ZB_FALSE;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_groups_commands_cli: ret %hd", (FMT__H, processed));
  return processed;
}

#endif /* ZB_ZCL_ENABLE_DEFAULT_GROUPS_PROCESSING_CLIENT */

#endif /* ZB_ENABLE_ZCL */
