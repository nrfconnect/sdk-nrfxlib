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
/* PURPOSE: SERVER: Messaging cluster handlers
*/

#define ZB_TRACE_FILE_ID 68

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_MESSAGING) || defined DOXYGEN

#include "zboss_api.h"
#include "zcl/zb_zcl_messaging.h"

zb_uint8_t gs_messaging_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_MESSAGING_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_messaging_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_MESSAGING_SERVER_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_messaging_server_cmd_list =
{
  sizeof(gs_messaging_server_received_commands), gs_messaging_server_received_commands,
  sizeof(gs_messaging_server_generated_commands), gs_messaging_server_generated_commands
};


typedef zb_uint8_t *(zb_zcl_messaging_put_payload_cb_t)(zb_uint8_t *, const void *);


static zb_uint8_t *zb_zcl_messaging_display_msg_put_payload(
  zb_uint8_t *data, const void *_payload)
{
  const zb_zcl_messaging_display_message_payload_t *payload = _payload;
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->message_id);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->message_control);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->start_time);
  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->duration_in_minutes);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->message_len);
  ZB_MEMCPY(data, payload->message, payload->message_len);
  data+=payload->message_len;
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->extended_message_control);

  return data;
}

static zb_uint8_t *zb_zcl_messaging_cancel_msg_put_payload(
  zb_uint8_t *data, const void *_payload)
{
  const zb_zcl_messaging_cancel_message_payload_t *payload = _payload;
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->message_id);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->message_control);
  return data;
}

void zb_zcl_messaging_send_cancel_msg(zb_uint8_t param,
                                           const zb_addr_u *dst_addr,
                                           zb_aps_addr_mode_t dst_addr_mode,
                                           zb_uint8_t dst_ep,
                                           zb_uint8_t src_ep,
                                           const zb_zcl_messaging_cancel_message_payload_t *payload,
                                           zb_callback_t cb)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_send_cancel_msg", (FMT__0));

  zb_zcl_send_cmd(param,
    dst_addr, dst_addr_mode, dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_CLI,
    src_ep,
    payload, sizeof(*payload), zb_zcl_messaging_cancel_msg_put_payload,
    ZB_ZCL_CLUSTER_ID_MESSAGING,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_MESSAGING_SRV_CMD_CANCEL_MESSAGE,
    cb
  );

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_send_cancel_msg", (FMT__0));
}

/*
Any message that needs truncation shall truncate on a UTF-8 character boundary.
The SE secure payload is 59 bytes for the Message field in a non-fragmented, non-source routed
Display Message packet (11 bytes for other Display Message fields).
Devices using fragmentation can send a message larger than this.
Reserving bytes for source route will reduce this.
*/
void zb_zcl_messaging_send_display_msg(zb_uint8_t param,
                                            const zb_addr_u *dst_addr,
                                            zb_aps_addr_mode_t dst_addr_mode,
                                            zb_uint8_t dst_ep,
                                            zb_uint8_t src_ep,
                                            const zb_zcl_messaging_display_message_payload_t *payload,
                                            zb_callback_t cb)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_send_display_msg", (FMT__0));

  zb_zcl_send_cmd(param,
    dst_addr, dst_addr_mode, dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_CLI,
    src_ep,
    payload, 0, zb_zcl_messaging_display_msg_put_payload,
    ZB_ZCL_CLUSTER_ID_MESSAGING,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_MESSAGING_SRV_CMD_DISPLAY_MESSAGE,
    cb
  );

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_send_display_msg", (FMT__0));
}


static void zb_zcl_messaging_server_handle_message_confirmation(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_int32_t data_remaining = 0;
  zb_zcl_messaging_message_confirm_payload_t pl = ZB_ZCL_MESSAGING_MSG_CONFIRM_PAYLOAD_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_server_handle_message_confirmation", (FMT__0));

  data_remaining = zb_buf_len(param);

  if (!ZB_ZCL_MESSAGING_MSG_CONFIRM_PAYLOAD_SIZE_IS_VALID((zb_uint8_t)data_remaining))
  {
    TRACE_MSG(TRACE_ZCL1, "zb_zcl_messaging_server_handle_message_confirmation: "
              "ERROR: invalid payload len (%d)",
              (FMT__D, data_remaining));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl.message_id, data); data_remaining -= 4;
  ZB_ZCL_PACKET_GET_DATA32(&pl.confirmation_time, data); data_remaining -= 4;
  if (data_remaining>0)
  {
	ZB_ZCL_PACKET_GET_DATA8(&pl.message_confirmation_control, data); data_remaining -= 1;
  }
  if (data_remaining>0)
  {
    ZB_ZCL_PACKET_GET_STATIC_STRING(pl.message_confirmation_response, data);
	data_remaining -= pl.message_confirmation_response[0];
  }

  if (data_remaining<0)
  {
    /* Send INVALID_FIELD status if message has invalid length. */
    TRACE_MSG(TRACE_ZCL1, "zb_zcl_messaging_server_handle_message_confirmation: "
              "ERROR: invalid msg confirmation response field length.",
              (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_MESSAGING_MSG_CONFIRMATION_CB_ID, RET_OK, cmd_info, &pl, NULL);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
      (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_server_handle_message_confirmation", (FMT__0));
}

static void zb_zcl_messaging_server_handle_get_last_message(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_messaging_get_last_message_response_t resp = ZB_ZCL_MESSAGING_GET_LAST_MESSAGE_RESPONSE_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_messaging_server_handle_get_last_message", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_MESSAGING_GET_LAST_MSG_CB_ID, RET_OK, cmd_info, NULL, &resp);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  /*check ret status of application callback. In protected case we send not_found default response. */

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      switch (resp.resp_type)
      {
        case ZB_ZCL_MESSAGING_RESPONSE_TYPE_NORMAL:
          zb_zcl_messaging_send_display_msg(param,
            (zb_addr_u *) &(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr),
             ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
             &resp.resp.display_message,
             NULL);
          break;
        case ZB_ZCL_MESSAGING_RESPONSE_TYPE_PROTECTED:
            /*NOTE: DisplayProtectedMessage command is unsupported(optional)*/
        default:
          zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
          break;
      }
      break;
    case RET_NOT_FOUND:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
      break;
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;

  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_messaging_server_handle_get_last_message", (FMT__0));
}


static zb_bool_t zb_zcl_messaging_server_side_process_commands(zb_uint8_t param,
                const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch ((zb_zcl_messaging_cli_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_MESSAGING_CLI_CMD_GET_LAST_MESSAGE:
      (void) zb_zcl_messaging_server_handle_get_last_message(param, cmd_info);
      processed = ZB_TRUE;
      break;
    case ZB_ZCL_MESSAGING_CLI_CMD_MESSAGE_CONFIRMATION:
      (void) zb_zcl_messaging_server_handle_message_confirmation(param, cmd_info);
      processed = ZB_TRUE;
      break;
    default:
      break;
  }

  return processed;
}


zb_bool_t zb_zcl_process_s_messaging_specific_command(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t          res = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_s_messaging_specific_command param %hd", (FMT__H, param));

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_messaging_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_MESSAGING);

  if (ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction)
  {
    /* process commands sent by clients */
    res = zb_zcl_messaging_server_side_process_commands(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_s_messaging_specific_command", (FMT__0));
  return res;
}

void zb_zcl_messaging_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_MESSAGING,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_s_messaging_specific_command);
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_MESSAGING || defined DOXYGEN */
