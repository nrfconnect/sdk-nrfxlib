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
/*  PURPOSE: SERVER: Smart Energy Price Cluster
*/

#define ZB_TRACE_FILE_ID 3999

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_PRICE) || defined DOXYGEN

#include "zcl/zb_zcl_price.h"

zb_uint8_t gs_price_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_PRICE_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_price_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_PRICE_SERVER_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_price_server_cmd_list =
{
  sizeof(gs_price_server_received_commands), gs_price_server_received_commands,
  sizeof(gs_price_server_generated_commands), gs_price_server_generated_commands
};

typedef zb_uint8_t *(*zb_price_put_payload_cb_t)(zb_uint8_t *, const void *);

zb_bool_t zb_zcl_process_s_price_specific_commands(zb_uint8_t param);

static zb_ret_t check_value_price(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_price_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_PRICE,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)check_value_price,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_s_price_specific_commands);
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
/* convert payload into raw data */
static zb_uint8_t *zb_zcl_price_publish_price_put_payload(
  zb_uint8_t *data, const void *_payload)
{
  const zb_zcl_price_publish_price_payload_t *payload = _payload;

  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->provider_id);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->rate_label[0]);
  ZB_ZCL_PACKET_PUT_DATA_N(data, &payload->rate_label[1],
                           payload->rate_label[0]);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->current_time);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->unit_of_measure);
  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->currency);
  ZB_ZCL_PACKET_PUT_DATA8 (data,
                           ZB_UINT4x2_TO_UINT8(payload->price_trailing_digit,
                                               payload->price_tier));
  ZB_ZCL_PACKET_PUT_DATA8 (data,
                           ZB_UINT4x2_TO_UINT8(payload->number_of_price_tiers,
                                               payload->register_tier));
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->start_time);
  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->duration_in_minutes);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->price);

  /* NK:CR Remove optional fields. */
  /* IA:CR: These fields must be presented in the packet.
   * See SE spec, D.4.2.4.1.1:
   *   "Note: M = Mandatory field, O = Optional field.
   *   All fields must be present in the payload.
   *   Optional fields will be marked with specific values to
   *   indicate they are not being used."
   */

  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->price_ratio);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->generation_price);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->generation_price_ratio);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->alternate_cost_delivered);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->alternate_cost_unit);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->alternate_cost_trailing_digit);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->number_of_block_thresholds);

  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->price_control);

  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->number_of_generation_tiers);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->generation_tier);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->extended_number_of_price_tiers);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->extended_price_tier);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->extended_register_tier);

  return data;
}

/* convert payload into raw data */
static zb_uint8_t *zb_zcl_price_ack_put_payload(
  zb_uint8_t *data, const void *_payload)
{
  const zb_zcl_price_ack_payload_t *payload = _payload;

  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->provider_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->current_time);
  ZB_ZCL_PACKET_PUT_DATA8 (data, payload->price_control);
  return data;
}

static inline const zb_uint8_t *zb_zcl_get_scheduled_prices_get_payload_from_data(
  zb_zcl_price_get_scheduled_prices_payload_t *payload, const zb_uint8_t *data)
{
  ZB_ZCL_PACKET_GET_DATA32(&payload->start_time, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->number_of_events, data);

  return data;
}


static inline const zb_uint8_t *zb_zcl_get_tier_labels_get_payload_from_data(
                                                  zb_zcl_price_get_tier_labels_payload_t *payload,
                                                  const zb_uint8_t *data)
{
  ZB_ZCL_PACKET_GET_DATA32(&payload->issuer_tariff_id, data);

  return data;
}


static inline const zb_uint8_t *zb_zcl_price_ack_get_payload_from_data(
  zb_zcl_price_ack_payload_t *payload, const zb_uint8_t *data)
{
  ZB_ZCL_PACKET_GET_DATA32(&payload->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&payload->issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&payload->current_time, data);
  ZB_ZCL_PACKET_GET_DATA8 (&payload->price_control, data);
  return data;
}

void zb_zcl_price_send_cmd_price_ack(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_uint8_t src_ep,
  const zb_zcl_price_ack_payload_t *payload,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param,
    dst_addr,
    dst_addr_mode,
    dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    payload,
    sizeof(*payload),
    zb_zcl_price_ack_put_payload,
    ZB_ZCL_CLUSTER_ID_PRICE,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_PRICE_CLI_CMD_PRICE_ACK,
    cb
  );
}

void zb_zcl_price_send_cmd_get_current_price(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_uint8_t src_ep,
  zb_uint8_t rx_on_when_idle,
  zb_callback_t cb
)
{
  zb_zcl_price_get_current_price_payload_t payload = ZB_ZCL_PRICE_GET_CURRENT_PRICE_PAYLOAD_INIT;

  if (rx_on_when_idle)
  {
    ZB_ZCL_PRICE_SET_REQUESTOR_RX_ON_WHEN_IDLE(&payload.command_options);
  }

  zb_zcl_send_cmd(param,
    dst_addr,
    dst_addr_mode,
    dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    &payload,
    sizeof(zb_zcl_price_get_current_price_payload_t),
    NULL,
    ZB_ZCL_CLUSTER_ID_PRICE,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_PRICE_CLI_CMD_GET_CURRENT_PRICE,
    cb
  );
}

void zb_zcl_price_send_cmd_get_scheduled_prices(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  const zb_zcl_price_get_scheduled_prices_payload_t *payload,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param,
    dst_addr,
    dst_addr_mode,
    dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    payload,
    sizeof(*payload),
    NULL,
    ZB_ZCL_CLUSTER_ID_PRICE,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_PRICE_CLI_CMD_GET_SCHEDULED_PRICES,
    cb
  );
}

void zb_zcl_price_send_cmd_publish_price(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_uint8_t src_ep,
  const zb_zcl_price_publish_price_payload_t *payload,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param,
    dst_addr,
    dst_addr_mode,
    dst_ep,
    ZB_ZCL_FRAME_DIRECTION_TO_CLI,
    src_ep,
    payload,
    sizeof(*payload),
    zb_zcl_price_publish_price_put_payload,
    ZB_ZCL_CLUSTER_ID_PRICE,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_PRICE_SRV_CMD_PUBLISH_PRICE,
    cb
  );
}


static zb_uint8_t *zb_zcl_price_put_publish_tier_labels(zb_uint8_t *data, const void *pl_arg)
{
  const zb_zcl_price_publish_tier_labels_payload_t *pl = pl_arg;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_put_publish_tier_labels", (FMT__0));

  ZB_ASSERT(pl_arg);
  ZB_ASSERT(data);

  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->provider_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->issuer_tariff_id);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->command_index);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->total_number_of_commands);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->number_of_labels);

  for (i = 0; i < pl->number_of_labels; i++)
  {
    ZB_ZCL_PACKET_PUT_DATA8(data, (pl->tier_labels + i)->tier_id);
    ZB_ZCL_PACKET_PUT_DATA_N(data, (pl->tier_labels + i)->tier_label, 1 + (pl->tier_labels + i)->tier_label[0]);
  }


  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_put_publish_tier_labels", (FMT__0));
  return data;
}


static zb_uint8_t zb_zcl_price_calculate_publish_tier_labels_payload_size(zb_zcl_price_publish_tier_labels_payload_t *pl)
{
  zb_uint8_t pl_size = 0;
  zb_uint8_t i;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_price_calculate_publish_tier_labels_payload_size", (FMT__0));

  pl_size += ZB_OFFSETOF(zb_zcl_price_publish_tier_labels_payload_t, tier_labels);

  for (i = 0; i <= pl->number_of_labels - 1; i++)
  {
    pl_size += ZB_OFFSETOF(zb_zcl_price_publish_tier_labels_sub_payload_t, tier_label);
    pl_size += 1 + (pl->tier_labels + i)->tier_label[0]; /* the first byte + string length */
  }


  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_price_calculate_publish_tier_labels_payload_size", (FMT__0));

  return pl_size;
}


void zb_zcl_price_send_cmd_publish_tier_labels(zb_uint8_t param, zb_addr_u *dst_addr,
                                                    zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                    zb_uint8_t src_ep,
                                                    zb_zcl_price_publish_tier_labels_payload_t *payload,
                                                    zb_callback_t cb)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_send_cmd_publish_tier_labels", (FMT__0));

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode,
                  dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                  src_ep, payload,
                  zb_zcl_price_calculate_publish_tier_labels_payload_size(payload),
                  zb_zcl_price_put_publish_tier_labels,
                  ZB_ZCL_CLUSTER_ID_PRICE,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_PRICE_SRV_CMD_PUBLISH_TIER_LABELS,
                  cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_send_cmd_publish_tier_labels", (FMT__0));
}

static zb_bool_t zb_price_publish_price_payload_is_valid(
  zb_zcl_price_publish_price_payload_t *payload)
{
  /* NOTE: SE spec, D.4.2.4.1.1:
   *    "Rate Label (mandatory): A ZCL Octet String field capable of
   *    storing a 12 character string (the first Octet indicates length) ..."
   */
  if (payload->rate_label[0] >= sizeof(payload->rate_label))
  {
    TRACE_MSG(TRACE_ZCL1, "ERROR: "
            "Invalid length of PublishPrice.RateLabel string"
            " payload %hd size %hd",
            (FMT__H_H, payload->rate_label[0], sizeof(payload->rate_label)));
    return ZB_FALSE;
  }

  return ZB_TRUE;
}

/* Notify user about PriceAck command. */
static zb_bool_t zb_zcl_price_process_price_ack(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_price_ack_payload_t  pl_in = ZB_ZCL_PRICE_ACK_PAYLOAD_INIT;
  zb_bool_t                   processed = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_process_price_ack", (FMT__0));

  if (!ZB_ZCL_PRICE_ACK_PAYLOAD_SIZE_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  (void) zb_zcl_price_ack_get_payload_from_data(&pl_in, zb_buf_begin(param));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_PRICE_PRICE_ACK_CB_ID, RET_ERROR, cmd_info, &pl_in, NULL);

  if (ZCL_CTX().device_cb)
  {
    TRACE_MSG(TRACE_ZCL1, "Attempt to get price param from user app", (FMT__0));

    (ZCL_CTX().device_cb)(param);

    processed = (zb_bool_t) (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK);

    if (!processed)
    {
      TRACE_MSG(TRACE_ZCL1, "ERROR: "
                "Unable to get price_ack params from user app, err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "User app callback isn't defined.", (FMT__0));
  }

  if (processed)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_process_price_ack", (FMT__0));
  return processed;
}


static zb_bool_t zb_zcl_price_process_get_scheduled_prices(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_price_get_scheduled_prices_payload_t pl_in = ZB_ZCL_PRICE_GET_SCHEDULED_PRICES_PAYLOAD_INIT;
  zb_bool_t                                   processed = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_process_get_scheduled_prices", (FMT__0));

  if (!ZB_ZCL_PRICE_GET_SCHEDULED_PRICES_PAYLOAD_SIZE_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  (void) zb_zcl_get_scheduled_prices_get_payload_from_data(&pl_in, zb_buf_begin(param));

  /* value  0x00000000 will be replaced with the current time stamp. */
  if (pl_in.start_time == 0)
  {
    pl_in.start_time = zb_get_utc_time();
  }

  TRACE_MSG(TRACE_ZCL1, "Got GetScheduledPrices packet with %hd events, %d time stamp.",
            (FMT__H_D, pl_in.number_of_events, pl_in.start_time));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_PRICE_GET_SCHEDULED_PRICES_CB_ID,
                                    RET_NOT_FOUND, cmd_info, &pl_in, NULL);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
    processed = (zb_bool_t)(ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK);
  }

  if (!processed)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
  }
  else
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_process_get_scheduled_prices", (FMT__0));
  return ZB_TRUE;
}


static zb_bool_t zb_zcl_price_process_get_tier_labels(zb_uint8_t param,
                                                      const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_price_get_tier_labels_payload_t pl_in;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_process_get_tier_labels", (FMT__0));

  if (!ZB_ZCL_PRICE_GET_TIER_LABELS_PAYLOAD_SIZE_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (!zb_zcl_get_tier_labels_get_payload_from_data(&pl_in, zb_buf_begin(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_PRICE_GET_TIER_LABELS_CB_ID,
                                    RET_NOT_FOUND, cmd_info, &pl_in, NULL);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      /*We don't send a PublishTierLabels command. Application must send reply
       * for this command.
       */
      zb_buf_free(param);
      return ZB_TRUE;
    case RET_NOT_FOUND:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
      break;
    case RET_ERROR:
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_process_get_tier_labels", (FMT__0));
  return ZB_TRUE;
}


/* Get payload information from user and send PublishPrice to the client. */
static void zb_zcl_price_process_get_current_price(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t                                price_data_is_available = ZB_FALSE;
  zb_zcl_price_get_current_price_payload_t pl_in                   = ZB_ZCL_PRICE_GET_CURRENT_PRICE_PAYLOAD_INIT;
  zb_zcl_price_publish_price_payload_t     pl_out                  = ZB_ZCL_PRICE_PUBLISH_PRICE_PAYLOAD_INIT;
  zb_uint8_t                              *data                    = zb_buf_begin(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_price_process_get_current_price", (FMT__0));

  if (!ZB_ZCL_PRICE_GET_CURRENT_PRICE_PAYLOAD_SIZE_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  /* Input payload: get Command Options (see D.4.2.3.2.1.1).*/
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.command_options, data);

  /* 0. Allocate params and fill it with default values */
  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_PRICE_GET_CURRENT_PRICE_CB_ID, RET_NOT_FOUND, cmd_info, &pl_in, &pl_out);
  ZB_ZCL_DEVICE_CMD_PARAM_OUT_GET(param, zb_zcl_price_publish_price_payload_t)->current_time = zb_get_utc_time();

  /* 1. Get params from user */
  if (ZCL_CTX().device_cb)
  {
    TRACE_MSG(TRACE_ZCL1, "Attempt to get price param from user app", (FMT__0));

    (ZCL_CTX().device_cb)(param);

    price_data_is_available = (zb_bool_t)(ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK);

    if (!price_data_is_available)
    {
      TRACE_MSG(TRACE_ZCL1, "ERROR: "
                "Unable to get publish_price params from user app, err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "User app callback isn't defined.", (FMT__0));
  }

  /* 2. Validate payload */
  if (price_data_is_available)
  {
    price_data_is_available = zb_price_publish_price_payload_is_valid(&pl_out);
  }

  /* 3. Send result */
  if (price_data_is_available)
  {
    ZB_ZCL_PRICE_SEND_CMD_PUBLISH_PRICE(param,
     (zb_addr_u *) &ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
     ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
     ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
     ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
     &pl_out);
  }
  else
  {
    /* If the server don't have any information about price,
     * it should send default response with NOT_FOUND status to the client.
     * See SE spec, subclause D.4.2.3.1.
     */
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_price_process_get_current_price", (FMT__0));
}

static zb_bool_t zb_zcl_process_price_server_commands(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t       processed = ZB_FALSE;

  switch ((zb_zcl_price_client_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_PRICE_CLI_CMD_GET_CURRENT_PRICE:
      zb_zcl_price_process_get_current_price(param, cmd_info);
      processed = ZB_TRUE;
      break;
    case ZB_ZCL_PRICE_CLI_CMD_GET_SCHEDULED_PRICES:
      processed = zb_zcl_price_process_get_scheduled_prices(param, cmd_info);
      break;
    case ZB_ZCL_PRICE_CLI_CMD_PRICE_ACK:
      processed = zb_zcl_price_process_price_ack(param, cmd_info);
      break;
    case ZB_ZCL_PRICE_CLI_CMD_GET_TIER_LABELS:
      processed = zb_zcl_price_process_get_tier_labels(param, cmd_info);
      break;
    default:
      break;
  }

  return processed;
}

zb_bool_t zb_zcl_process_s_price_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t           processed = ZB_FALSE;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_price_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_s_price_specific_commands, "
            "param=%hd, cmd_id=%hd", (FMT__H_H, param, cmd_info.cmd_id));

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_PRICE);

  if (ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction)
  {
    processed = zb_zcl_process_price_server_commands(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_s_price_specific_commands (ret=%hd)",
            (FMT__H, processed));

  return processed;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_PRICE || defined DOXYGEN */
