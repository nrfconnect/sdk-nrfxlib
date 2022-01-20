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
/*  PURPOSE: CLIENT: Metering Cluster
*/

#define ZB_TRACE_FILE_ID 4000

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_METERING) || defined DOXYGEN

#include "zcl/zb_zcl_metering.h"

zb_uint8_t gs_metering_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_METERING_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_metering_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_METERING_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_metering_client_cmd_list =
{
  sizeof(gs_metering_client_received_commands), gs_metering_client_received_commands,
  sizeof(gs_metering_client_generated_commands), gs_metering_client_generated_commands
};

zb_bool_t zb_zcl_process_c_metering_specific_commands(zb_uint8_t param);

void zb_zcl_metering_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_METERING,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)zb_zcl_process_c_metering_specific_commands);
}

void zb_zcl_metering_send_cmd_get_snapshot(zb_uint8_t param, zb_addr_u *dst_addr,
                                                zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                zb_uint8_t src_ep,
                                                zb_zcl_metering_get_snapshot_payload_t *payload,
                                                zb_callback_t cb)
{
  zb_zcl_metering_get_snapshot_payload_t pl;
  zb_uint8_t *data = (zb_uint8_t *) &pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_server_send_get_snapshot", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));


  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->earliest_start_time);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->latest_end_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->snapshot_offset);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->snapshot_cause);

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode,
                  dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                  src_ep, &pl, sizeof(pl), NULL,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_CLI_CMD_GET_SNAPSHOT,
                  cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_server_send_get_snapshot", (FMT__0));
}


void zb_zcl_metering_send_cmd_get_sampled_data(zb_uint8_t param, zb_addr_u *dst_addr,
                                                    zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                    zb_uint8_t src_ep,
                                                    zb_zcl_metering_get_sampled_data_payload_t *payload,
                                                    zb_callback_t cb)
{
  zb_zcl_metering_get_sampled_data_payload_t pl;
  zb_uint8_t *data = (zb_uint8_t *) &pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_server_send_get_sampled_data", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));

  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->sample_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->earliest_sample_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->sample_type);
  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->number_of_samples);

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode,
                  dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                  src_ep, &pl, sizeof(pl), NULL,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_CLI_CMD_GET_SAMPLED_DATA,
                  cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_server_send_get_sampled_data", (FMT__0));
}


void zb_zcl_metering_send_cmd_request_fast_poll_mode(zb_uint8_t param, zb_addr_u *dst_addr,
                                                          zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                          zb_uint8_t src_ep,
                                                          zb_zcl_metering_request_fast_poll_mode_payload_t *payload,
                                                          zb_callback_t cb)
{
  zb_zcl_metering_request_fast_poll_mode_payload_t pl;
  zb_uint8_t *data = (zb_uint8_t *) &pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_server_send_request_fast_poll_mode", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));

  ZB_ZCL_PACKET_PUT_DATA8(data, payload->fast_poll_update_period);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->duration_in_minutes);

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode,
                  dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                  src_ep, &pl, sizeof(pl), NULL,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_CLI_CMD_REQUEST_FAST_POLL_MODE,
                  cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_server_send_request_fast_poll_mode", (FMT__0));
}


void zb_zcl_metering_send_cmd_get_profile(zb_uint8_t param, zb_addr_u *dst_addr,
                                               zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                               zb_uint8_t src_ep, zb_zcl_metering_get_profile_payload_t *payload,
                                               zb_callback_t cb)
{
  zb_zcl_metering_get_profile_payload_t pl;
  zb_uint8_t *data = (zb_uint8_t *) &pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_server_send_get_profile", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));

  ZB_ZCL_PACKET_PUT_DATA8(data, payload->interval_channel);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->end_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->number_of_periods);

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode,
                  dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                  src_ep, &pl, sizeof(pl), NULL,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_CLI_CMD_GET_PROFILE,
                  cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_server_send_get_profile", (FMT__0));
}


static const zb_uint8_t *zb_zcl_metering_request_fast_poll_mode_response_parse_payload(
                                                          zb_zcl_metering_request_fast_poll_mode_response_payload_t *pl,
                                                          zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_metering_request_fast_poll_mode_response_parse_payload", (FMT__0));

  if (data_size < sizeof(zb_zcl_metering_request_fast_poll_mode_response_payload_t))
  {
    TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA8(&pl->applied_update_period_in_seconds, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->fast_poll_mode_end_time, data);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_metering_request_fast_poll_mode_response_parse_payload", (FMT__0));

  return data;
}


static const zb_uint8_t *zb_zcl_metering_get_sampled_data_response_parse_payload(zb_zcl_metering_get_sampled_data_response_payload_t *pl,
                                                                                 zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_get_sampled_data_response_parse_payload", (FMT__0));

  if (data_size >= (ZB_OFFSETOF(zb_zcl_metering_get_sampled_data_response_payload_t, samples)))
  {
    ZB_ZCL_PACKET_GET_DATA16(&pl->sample_id, data);
    ZB_ZCL_PACKET_GET_DATA32(&pl->sample_start_time, data);
    ZB_ZCL_PACKET_GET_DATA8(&pl->sample_type, data);
    ZB_ZCL_PACKET_GET_DATA16(&pl->sample_request_interval, data);
    ZB_ZCL_PACKET_GET_DATA16(&pl->number_of_samples, data);

    data_size -= ZB_OFFSETOF(zb_zcl_metering_get_sampled_data_response_payload_t, samples);

    if (data_size == ZB_24BIT_SIZE * pl->number_of_samples)
    {
      pl->samples = (zb_uint24_t *)data;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Malformed packet, remaining size:%d, expected remaining size:%d",
                (FMT__D_D, data_size, ZB_24BIT_SIZE * pl->number_of_samples));
      data = NULL;
    }

  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
    data = NULL;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_get_sampled_data_response_parse_payload", (FMT__0));

  return data;
}


static const zb_uint8_t *zb_zcl_metering_publish_snapshot_parse_payload(zb_zcl_metering_publish_snapshot_payload_t *pl,
                                                                        zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_publish_snapshot_parse_payload", (FMT__0));

  if (data_size < ZB_OFFSETOF(zb_zcl_metering_publish_snapshot_payload_t, snapshot_sub_payload))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size", (FMT__0));
    return NULL;
  }

  data_size -= ZB_OFFSETOF(zb_zcl_metering_publish_snapshot_payload_t, snapshot_sub_payload);

  ZB_ZCL_PACKET_GET_DATA32(&pl->snapshot_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->snapshot_time, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->total_snapshots_found, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->command_index, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->total_number_of_commands, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->snapshot_cause, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->snapshot_payload_type, data);

  switch (pl->snapshot_payload_type)
  {
    case ZB_ZCL_METERING_TOU_DELIVERED_REGISTERS:
    {
      zb_zcl_metering_tou_delivered_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_delivered);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_DELIVERED_REGISTERS", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_tou_delivered_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_tou_delivered_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_time_stamp_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_time_stamp_delivered, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->bill_delivered_trailing_digit, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size != (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_TOU_RECEIVED_REGISTERS:
    {
      zb_zcl_metering_tou_received_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_received);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_RECEIVED_REGISTERS", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_tou_received_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_tou_received_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_time_stamp_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_time_stamp_received, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->bill_received_trailing_digit, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size != (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_DELIVERED:
    {
      zb_zcl_metering_block_delivered_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_delivered);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_DELIVERED", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_block_delivered_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_block_delivered_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_time_stamp_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_delivered, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_time_stamp_delivered, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->bill_delivered_trailing_digit, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size < (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;

      /*subtract tier_summation size*/
      data_size -= sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;

      data += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE; /* move pointer to number_of_tiers_and_block_thresholds_in_use */

      if (0 == data_size)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse after tier_summation", (FMT__0));
        return NULL;
      }

      /*subtract 1 byte for number_of_tiers_and_block_thresholds_in_use*/
      data_size -= 1;

      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_and_block_thresholds_in_use, data);

      if (data_size != sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse tier_block_summation", (FMT__0));
        return NULL;
      }

      sub_payload->tier_block_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_RECEIVED:
    {
      zb_zcl_metering_block_received_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_received);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_RECEIVED", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_block_received_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_block_received_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->bill_to_date_time_stamp_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_received, data);
      ZB_ZCL_PACKET_GET_DATA32(&sub_payload->projected_bill_time_stamp_received, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->bill_received_trailing_digit, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size < (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;

      /*subtract tier_summation size*/
      data_size -= sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;

      data += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE; /* move pointer to number_of_tiers_and_block_thresholds_in_use */

      if (0 == data_size)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse after tier_summation", (FMT__0));
        return NULL;
      }

      /*subtract 1 byte for number_of_tiers_and_block_thresholds_in_use*/
      data_size -= 1;

      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_and_block_thresholds_in_use, data);

      if (data_size != sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse tier_block_summation", (FMT__0));
        return NULL;
      }

      sub_payload->tier_block_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_TOU_DELIVERED_NO_BILLING:
    {
      zb_zcl_metering_tou_delivered_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_delivered_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_DELIVERED_NO_BILLING", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_tou_delivered_no_billing_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_tou_delivered_no_billing_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_delivered, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size < (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_TOU_RECEIVED_NO_BILLING:
    {
      zb_zcl_metering_tou_received_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_received_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_RECEIVED_NO_BILLING", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_tou_received_no_billing_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_tou_received_no_billing_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_received, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size < (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_DELIVERED_NO_BILLING:
    {
      zb_zcl_metering_block_tier_delivered_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_tier_delivered_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_DELIVERED_NO_BILLING", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_block_tier_delivered_no_billing_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_block_tier_delivered_no_billing_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_delivered, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size < (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;

      /*subtract tier_summation size*/
      data_size -= sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;

      data += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE; /* move pointer to number_of_tiers_and_block_thresholds_in_use */

      if (0 == data_size)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse after tier_summation", (FMT__0));
        return NULL;
      }

      /*subtract 1 byte for number_of_tiers_and_block_thresholds_in_use*/
      data_size -= 1;

      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_and_block_thresholds_in_use, data);

      if (data_size != sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse tier_block_summation", (FMT__0));
        return NULL;
      }

      sub_payload->tier_block_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_RECEIVED_NO_BILLING:
    {
      zb_zcl_metering_block_tier_received_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_tier_received_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_RECEIVED_NO_BILLING", (FMT__0));

      if (data_size < ZB_OFFSETOF(zb_zcl_metering_block_tier_received_no_billing_payload_t, tier_summation))
      {
        TRACE_MSG(TRACE_ZCL1, "Invalid sub-payload size", (FMT__0));
        return NULL;
      }

      data_size -= ZB_OFFSETOF(zb_zcl_metering_block_tier_received_no_billing_payload_t, tier_summation);

      ZB_ZCL_PACKET_GET_DATA48(&sub_payload->current_summation_received, data);
      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_in_use, data);

      if (data_size < (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE))
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }

      sub_payload->tier_summation = (zb_uint48_t *)data;

      /*subtract tier_summation size*/
      data_size -= sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;

      data += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE; /* move pointer to number_of_tiers_and_block_thresholds_in_use */

      if (0 == data_size)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse after tier_summation", (FMT__0));
        return NULL;
      }

      /*subtract 1 byte for number_of_tiers_and_block_thresholds_in_use*/
      data_size -= 1;

      ZB_ZCL_PACKET_GET_DATA8(&sub_payload->number_of_tiers_and_block_thresholds_in_use, data);

      if (data_size != sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet: can't parse tier_block_summation", (FMT__0));
        return NULL;
      }

      sub_payload->tier_block_summation = (zb_uint48_t *)data;
    }
      break;
    case ZB_ZCL_METERING_DATA_UNAVAILABLE:
      /*NOTE: from SE spec: The data for this snapshot is currently unavailable;
       * if used, there is currently no subsequent snapshot data.
       */
      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_DATA_UNAVAILABLE", (FMT__0));
      if (data_size != 0)
      {
        TRACE_MSG(TRACE_ZCL1, "Malformed packet", (FMT__0));
        return NULL;
      }
      break;
    default:
      TRACE_MSG(TRACE_ZCL1, "Malformed packet: unsupported snapshot payload type", (FMT__0));
      return NULL;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_publish_snapshot_parse_payload", (FMT__0));

  return data;
}

static const zb_uint8_t *zb_zcl_metering_get_profile_response_parse_payload(zb_zcl_metering_get_profile_response_payload_t *pl,
                                                                            zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_metering_get_profile_response_parse_payload", (FMT__0));

  if (data_size >= (ZB_OFFSETOF(zb_zcl_metering_get_profile_response_payload_t, intervals)))
  {
    ZB_ZCL_PACKET_GET_DATA32(&pl->end_time, data);
    ZB_ZCL_PACKET_GET_DATA8(&pl->status, data);
    ZB_ZCL_PACKET_GET_DATA8(&pl->profile_interval_period, data);
    ZB_ZCL_PACKET_GET_DATA8(&pl->number_of_periods_delivered, data);

    data_size -= ZB_OFFSETOF(zb_zcl_metering_get_profile_response_payload_t, intervals);

    if (data_size == ZB_24BIT_SIZE * pl->number_of_periods_delivered)
    {
      pl->intervals = (zb_uint24_t *)data;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Malformed packet, remaining size:%d, expected remaining size:%d",
                (FMT__D_D, data_size, ZB_24BIT_SIZE * pl->number_of_periods_delivered));
      data = NULL;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Malformed packet, data_size:%d", (FMT__D, data_size));
    data = NULL;
  }

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_metering_get_profile_response_parse_payload", (FMT__0));

  return data;
}


static zb_bool_t zb_zcl_metering_process_request_fast_poll_mode_response(zb_uint8_t param,
                                                              const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_request_fast_poll_mode_response_payload_t pl_in ;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_request_fast_poll_mode_response", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_REQUEST_FAST_POLL_MODE_RESPONSE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);

  if (!zb_zcl_metering_request_fast_poll_mode_response_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid request_fast_poll_mode_response command payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "User app callback isn't defined.", (FMT__0));
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
      break;
    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_request_fast_poll_mode_response", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_metering_process_publish_snapshot(zb_uint8_t param,
                                                          const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_publish_snapshot_payload_t pl_in ;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_publish_snapshot", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_PUBLISH_SNAPSHOT_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);

  if (!zb_zcl_metering_publish_snapshot_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid publish_snapshot command payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "User app callback isn't defined.", (FMT__0));
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
      break;
    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_publish_snapshot", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_metering_process_get_sampled_data_response(zb_uint8_t param,
                                                                   const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_get_sampled_data_response_payload_t pl_in ;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_get_sampled_data_response", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_GET_SAMPLED_DATA_RESPONSE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);

  if (!zb_zcl_metering_get_sampled_data_response_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid get_sampled_data_response command payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "User app callback isn't defined.", (FMT__0));
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
      break;
    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_get_sampled_data_response", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_metering_process_get_profile_response(zb_uint8_t param,
                                                              const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_get_profile_response_payload_t pl_in ;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_get_profile_response", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_GET_PROFILE_RESPONSE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);

  if (!zb_zcl_metering_get_profile_response_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid get_profile_response command payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "User app callback isn't defined.", (FMT__0));
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_SUCCESS);
      break;
    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_get_profile_response", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_process_metering_client_commands(zb_uint8_t param,
                                                         const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch ((zb_zcl_metering_cli_cmd_t)cmd_info->cmd_id)
  {
    case ZB_ZCL_METERING_SRV_CMD_GET_PROFILE_RESPONSE:
      processed = zb_zcl_metering_process_get_profile_response(param, cmd_info);
      break;
    case ZB_ZCL_METERING_SRV_CMD_REQUEST_FAST_POLL_MODE_RESPONSE:
      processed = zb_zcl_metering_process_request_fast_poll_mode_response(param, cmd_info);
      break;
    case ZB_ZCL_METERING_SRV_CMD_PUBLISH_SNAPSHOT:
      processed = zb_zcl_metering_process_publish_snapshot(param, cmd_info);
      break;
    case ZB_ZCL_METERING_SRV_CMD_GET_SAMPLED_DATA_RESPONSE:
      processed = zb_zcl_metering_process_get_sampled_data_response(param, cmd_info);
      break;
    default:
      break;
  }

  return processed;
}


zb_bool_t zb_zcl_process_c_metering_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t           processed = ZB_FALSE;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_metering_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_c_metering_specific_commands, "
            "param=%hd, cmd_id=%hd", (FMT__H_H, param, cmd_info.cmd_id));

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_METERING);

  if (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction)
  {
    processed = zb_zcl_process_metering_client_commands(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_c_metering_specific_commands (ret=%hd)",
            (FMT__H, processed));

  return processed;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_METERING || defined DOXYGEN */
