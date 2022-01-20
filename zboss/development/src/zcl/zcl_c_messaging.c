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
/* PURPOSE: CLIENT: Messaging cluster handlers
*/

#define ZB_TRACE_FILE_ID 92

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_MESSAGING) || defined DOXYGEN

#include "zboss_api.h"
#include "zcl/zb_zcl_messaging.h"

zb_uint8_t gs_messaging_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_MESSAGING_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_messaging_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_MESSAGING_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_messaging_client_cmd_list =
{
  sizeof(gs_messaging_client_received_commands), gs_messaging_client_received_commands,
  sizeof(gs_messaging_client_generated_commands), gs_messaging_client_generated_commands
};



typedef zb_uint8_t *(zb_zcl_messaging_put_payload_cb_t)(zb_uint8_t *, const void *);


static zb_uint8_t *zb_zcl_messaging_msg_confirmation_put_payload(
  zb_uint8_t *data, const void *_payload)
{
  const zb_zcl_messaging_message_confirm_payload_t *payload = _payload;
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->message_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->confirmation_time);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->message_confirmation_control);
  ZB_ZCL_PACKET_PUT_STRING(data, payload->message_confirmation_response);
  return data;
}

void zb_zcl_messaging_send_get_last_msg(zb_uint8_t param,
                                             const zb_addr_u *dst_addr,
                                             zb_aps_addr_mode_t dst_addr_mode,
                                             zb_uint8_t dst_ep,
                                             zb_uint8_t src_ep,
                                             zb_callback_t cb)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_send_get_last_msg", (FMT__0));

  zb_zcl_send_cmd(param,
    dst_addr, dst_addr_mode, dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    NULL, 0, NULL,
    ZB_ZCL_CLUSTER_ID_MESSAGING,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_MESSAGING_CLI_CMD_GET_LAST_MESSAGE,
    cb
  );


  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_send_get_last_msg", (FMT__0));
}

void zb_zcl_messaging_send_msg_confirmation(zb_uint8_t param,
                                            const zb_addr_u *dst_addr,
                                            zb_aps_addr_mode_t dst_addr_mode,
                                            zb_uint8_t dst_ep,
                                            zb_uint8_t src_ep,
                                            const zb_zcl_messaging_message_confirm_payload_t *payload,
                                            zb_callback_t cb)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_send_msg_confirmation", (FMT__0));

  zb_zcl_send_cmd(param,
    dst_addr, dst_addr_mode, dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    payload, sizeof(*payload), zb_zcl_messaging_msg_confirmation_put_payload,
    ZB_ZCL_CLUSTER_ID_MESSAGING,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_MESSAGING_CLI_CMD_MESSAGE_CONFIRMATION,
    cb
  );

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_send_msg_confirmation", (FMT__0));
}


static void zb_zcl_messaging_client_handle_cancel_message(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_messaging_cancel_message_payload_t  pl = ZB_ZCL_MESSAGING_CANCEL_MSG_PAYLOAD_INIT;
  zb_uint8_t                            *data = zb_buf_begin(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_client_handle_cancel_message", (FMT__0));

  if (!ZB_ZCL_MESSAGING_MSG_CANCEL_MESSAGE_SIZE_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "malformed packet (small), prevented buffer overflow (%hd)",
              (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl.message_id, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl.message_control, data);

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_MESSAGING_CANCEL_MSG_CB_ID, RET_OK, cmd_info, &pl, NULL);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_client_handle_cancel_message", (FMT__0));

  zb_zcl_send_default_handler(param, cmd_info,
      (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);
}


static void zb_zcl_messaging_client_handle_display_message(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_messaging_display_message_payload_t pl = ZB_ZCL_MESSAGING_DISPLAY_MSG_PAYLOAD_INIT;
  zb_uint8_t                            *data = zb_buf_begin(param);
  zb_int16_t                            pkt_len = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_client_handle_display_message", (FMT__0));

  if (!ZB_ZCL_MESSAGING_DISPLAY_MSG_PAYLOAD_SIZE_IS_VALID(pkt_len))
  {
    TRACE_MSG(TRACE_ZCL1, "malformed packet (small), prevented buffer overflow (%hd)",
              (FMT__H, pkt_len));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl.message_id, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl.message_control, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl.start_time, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl.duration_in_minutes, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl.message_len, data);
  data+=pl.message_len;
  pkt_len-=pl.message_len+12;

  if (ZB_ZCL_MESSAGING_INTER_PAN_TRANSMISSION_ONLY == ZB_ZCL_MESSAGING_MESSAGE_CONTROL_GET_TRANSMISSION_MECHANISM(pl.message_control))
  {
    TRACE_MSG(TRACE_ZCL1, "Use of the Inter-PAN transmission mechanism within the Messaging cluster is now deprecated.",
    (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  if (pkt_len>0)
  {
    ZB_ZCL_PACKET_GET_DATA8(&pl.extended_message_control, data);
    TRACE_MSG(TRACE_ZCL3, "Extended message control field present: %hd.",(FMT__H,pl.extended_message_control));
  }
  else
  {
    pl.extended_message_control = 1;
  }

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_MESSAGING_DISPLAY_MSG_CB_ID, RET_OK, cmd_info, &pl, NULL);

/*NOTE: AEV: this done after ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH, because of this function can move buffer*/
  pl.message = (zb_uint8_t*)zb_buf_begin(param)+ZB_OFFSETOF(zb_zcl_messaging_display_message_payload_t, message);

  DUMP_TRAF("msg:", (zb_uint8_t *)pl.message, pl.message_len, 0);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
      (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_client_handle_display_message", (FMT__0));
}

static zb_bool_t zb_zcl_messaging_client_side_process_commands(zb_uint8_t param,
                const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch ((zb_zcl_messaging_srv_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_MESSAGING_SRV_CMD_CANCEL_MESSAGE:
      zb_zcl_messaging_client_handle_cancel_message(param, cmd_info);
      processed = ZB_TRUE;
      break;
    case ZB_ZCL_MESSAGING_SRV_CMD_DISPLAY_MESSAGE:
      zb_zcl_messaging_client_handle_display_message(param, cmd_info);
      processed = ZB_TRUE;
      break;
    default:
      break;
  }

  return processed;
}


zb_bool_t zb_zcl_process_c_messaging_specific_command(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t          res = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_c_messaging_specific_command param %hd", (FMT__H, param));

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_messaging_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_MESSAGING);

  if (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction)
  {
    /* process commands sent by servers */
    res = zb_zcl_messaging_client_side_process_commands(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_c_messaging_specific_command", (FMT__0));
  return res;
}

void zb_zcl_messaging_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_MESSAGING,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_c_messaging_specific_command);
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_MESSAGING || defined DOXYGEN */
