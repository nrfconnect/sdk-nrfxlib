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
/*  PURPOSE: CLIENT: Smart Energy Price Cluster
*/

#define ZB_TRACE_FILE_ID 3998

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_PRICE) || defined DOXYGEN

zb_uint8_t gs_price_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_PRICE_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_price_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_PRICE_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_price_client_cmd_list =
{
  sizeof(gs_price_client_received_commands), gs_price_client_received_commands,
  sizeof(gs_price_client_generated_commands), gs_price_client_generated_commands
};

typedef zb_uint8_t *(*zb_price_put_payload_cb_t)(zb_uint8_t *, const void *);

zb_bool_t zb_zcl_process_c_price_specific_commands(zb_uint8_t param);


static zb_ret_t check_value_price(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_price_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_PRICE,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)check_value_price,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_c_price_specific_commands);
}


static zb_ret_t check_value_price(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}


/* convert raw data into payload */
static ZB_INLINE const zb_uint8_t *zb_zcl_price_publish_price_get_payload_from_data(
  zb_zcl_price_publish_price_payload_t *payload, const zb_uint8_t param)
{
  zb_uint8_t  nibble_buf = 0;
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);
  zb_uint8_t exp_p_size = 0;

  if (!ZB_ZCL_PRICE_PUBLISH_PRICE_PAYLOAD_SIZE_IS_VALID(data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Received invalid PublishPrice command, "
              "payload_len = %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&payload->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->rate_label[0], data);

  if (payload->rate_label[0] >= sizeof(payload->rate_label))
  {
    TRACE_MSG(TRACE_ZCL1, "Received invalid Price Cluster command, "
              "rate_label[0]=%d", (FMT__D, payload->rate_label[0]));
    return NULL;
  }

  exp_p_size = ZB_ZCL_PRICE_PUBLISH_PRICE_PAYLOAD_EXPECTED_SIZE(payload);

  if (exp_p_size != data_size)
  {
    TRACE_MSG(TRACE_ZCL1, "Received invalid PublishPrice command, "
              "expected size = %hd, actual size = %hd", (FMT__H_H, exp_p_size, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA_N(&payload->rate_label[1], data,
                             payload->rate_label[0]);
  ZB_ZCL_PACKET_GET_DATA32(&payload->issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&payload->current_time, data);
  ZB_ZCL_PACKET_GET_DATA8(&payload->unit_of_measure, data);
  ZB_ZCL_PACKET_GET_DATA16(&payload->currency, data);
  ZB_ZCL_PACKET_GET_DATA8(&nibble_buf, data);
  payload->price_trailing_digit = ZB_UINT8_MSB_NIBBLE(nibble_buf);
  payload->price_tier = ZB_UINT8_LSB_NIBBLE(nibble_buf);
  ZB_ZCL_PACKET_GET_DATA8(&nibble_buf, data);
  payload->number_of_price_tiers = ZB_UINT8_MSB_NIBBLE(nibble_buf);
  payload->register_tier = ZB_UINT8_LSB_NIBBLE(nibble_buf);
  ZB_ZCL_PACKET_GET_DATA32(&payload->start_time, data);
  ZB_ZCL_PACKET_GET_DATA16(&payload->duration_in_minutes, data);
  ZB_ZCL_PACKET_GET_DATA32(&payload->price, data);

  ZB_ZCL_PACKET_GET_DATA8 (&payload->price_ratio, data);
  ZB_ZCL_PACKET_GET_DATA32(&payload->generation_price, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->generation_price_ratio, data);
  ZB_ZCL_PACKET_GET_DATA32(&payload->alternate_cost_delivered, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->alternate_cost_unit, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->alternate_cost_trailing_digit, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->number_of_block_thresholds, data);

  ZB_ZCL_PACKET_GET_DATA8(&payload->price_control, data);

  ZB_ZCL_PACKET_GET_DATA8 (&payload->number_of_generation_tiers, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->generation_tier, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->extended_number_of_price_tiers, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->extended_price_tier, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->extended_register_tier, data);

  return data;
}

void zb_zcl_price_send_cmd_get_tier_labels(zb_uint8_t param, zb_addr_u *dst_addr,
                                                zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                zb_uint8_t src_ep,
                                                zb_zcl_price_get_tier_labels_payload_t *payload,
                                                zb_callback_t cb)
{
  zb_zcl_price_get_tier_labels_payload_t pl;
  zb_uint8_t *data = (zb_uint8_t *) &pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_server_send_get_tier_labels", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));

  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->issuer_tariff_id);

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode,
                  dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                  src_ep, &pl, sizeof(pl), NULL,
                  ZB_ZCL_CLUSTER_ID_PRICE,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_PRICE_CLI_CMD_GET_TIER_LABELS,
                  cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_server_send_get_tier_labels", (FMT__0));
}


static void zb_zcl_price_process_publish_price(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_price_publish_price_payload_t   pl_in = ZB_ZCL_PRICE_PUBLISH_PRICE_PAYLOAD_INIT;
  zb_zcl_price_ack_payload_t       pl_out = ZB_ZCL_PRICE_ACK_PAYLOAD_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_process_publish_price", (FMT__0));

  if (!ZB_ZCL_PRICE_PUBLISH_PRICE_PAYLOAD_SIZE_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  if (!zb_zcl_price_publish_price_get_payload_from_data(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet payload.", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  pl_out = ZB_ZCL_PRICE_ACK_PAYLOAD_INIT_FROM_PUBLISH_PRICE_PAYLOAD(&pl_in);

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_PRICE_PUBLISH_PRICE_CB_ID, RET_OK, cmd_info, &pl_in, &pl_out);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  if (pl_in.price_control & ZB_ZCL_PRICE_CONTROL_PRICE_ACKNOWLEDGEMENT_REQUIRED)
  {
    ZB_ZCL_PRICE_SEND_CMD_PRICE_ACK(param,
     (zb_addr_u *) &ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
     ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
     ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
     ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
     &pl_out);
  }
  else
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_process_publish_price", (FMT__0));
}


static const zb_uint8_t *zb_zcl_price_publish_tier_labels_parse_payload(
                                                    zb_zcl_price_publish_tier_labels_payload_t *pl,
                                                    zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);
  zb_int16_t remaining_len = 0;
  zb_uint8_t number_of_entries_in_this_command = 0;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_price_publish_tier_labels_parse_payload, data_size = %d", (FMT__D, data_size));

  ZB_ZCL_PACKET_GET_DATA32(&pl->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->issuer_tariff_id, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->command_index, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->total_number_of_commands, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->number_of_labels, data);

  remaining_len = data_size - ZB_OFFSETOF(zb_zcl_price_publish_tier_labels_payload_t, tier_labels);
  pl->tier_labels = (zb_zcl_price_publish_tier_labels_sub_payload_t *)data;
  while (remaining_len > 0)
  {
    remaining_len--;  /* size of Tier ID field */
    data++;

    if ((remaining_len < *(data)) && (*(data) < sizeof(((zb_zcl_price_publish_tier_labels_sub_payload_t *)0)->tier_label)))
    {
      TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));  /* Can't parse tier labels entries */
      return NULL;
    }

    remaining_len -= *(data) + 1;  /* subtract the length of octet string */
    data += *(data) + 1;  /* move to next entry */
    number_of_entries_in_this_command++;
  }

  if (number_of_entries_in_this_command != pl->number_of_labels)
  {
    TRACE_MSG(TRACE_ZCL1, "Malformed packet: number_of_entries_in_this_command != number_of_labels", (FMT__0));
    TRACE_MSG(TRACE_ZCL1, "Malformed packet: number_of_entries_in_this_command = %d", (FMT__D, number_of_entries_in_this_command));
    TRACE_MSG(TRACE_ZCL1, "Malformed packet: number_of_labels = %d", (FMT__D, pl->number_of_labels));
    return NULL;
  }


  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_price_publish_tier_labels_parse_payload", (FMT__0));

  return data;
}


static void zb_zcl_price_process_publish_tier_labels(zb_uint8_t param,
                                                          const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_price_publish_tier_labels_payload_t pl_in;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_process_publish_tier_labels", (FMT__0));

  /* NOTE: this command can move memory and we must invoke it before payload parsing */
  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_PRICE_PUBLISH_TIER_LABELS_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);

  if ((!ZB_ZCL_PRICE_PUBLISH_TIER_LABELS_PAYLOAD_SIZE_IS_VALID(zb_buf_len(param)))
      || (!zb_zcl_price_publish_tier_labels_parse_payload(&pl_in, param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet payload.", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
                              (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_process_publish_tier_labels", (FMT__0));
}


static zb_bool_t zb_zcl_process_price_client_commands(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch ((zb_zcl_price_server_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_PRICE_SRV_CMD_PUBLISH_PRICE:
/* #ifdef ZB_ENABLE_SE */
#if defined(ZB_SE_ENABLE_STEADY_STATE_PROCESSING)
      if (zb_se_steady_state_block_zcl_cmd(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr, ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint, cmd_info->seq_number))
      {
        (void) zb_se_steady_publish_price_handle(param);
      }
      else
#endif
      {
        zb_zcl_price_process_publish_price(param, cmd_info);
      }
      processed = ZB_TRUE;
      break;
    case ZB_ZCL_PRICE_SRV_CMD_PUBLISH_TIER_LABELS:
      zb_zcl_price_process_publish_tier_labels(param, cmd_info);
      processed = ZB_TRUE;
      break;
    default:
      break;
  }

  return processed;
}

zb_bool_t zb_zcl_process_c_price_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t           processed = ZB_FALSE;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_price_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_c_price_specific_commands, "
            "param=%hd, cmd_id=%hd", (FMT__H_H, param, cmd_info.cmd_id));

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_PRICE);

  if (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction)
  {
    processed = zb_zcl_process_price_client_commands(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_c_price_specific_commands (ret=%hd)",
            (FMT__H, processed));

  return processed;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_PRICE || defined DOXYGEN */
