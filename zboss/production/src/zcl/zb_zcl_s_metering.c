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
/*  PURPOSE: SERVER: Metering Cluster
*/

#define ZB_TRACE_FILE_ID 4001

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_METERING) || defined DOXYGEN

#include "zcl/zb_zcl_metering.h"

zb_bool_t zb_zcl_process_s_metering_specific_commands(zb_uint8_t param);

void zb_zcl_metering_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_METERING,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_s_metering_specific_commands);
}

/* TODO: Turn off correctly */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_METERING_OPTIONAL
#define ZB_ZCL_METERING_SEND_CMD_HELPER(_fn, _param, _cmd_info, _payload, _payload_size, _cb)  \
  _fn((_param), \
     (zb_addr_u *) &ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).source.u.short_addr, \
     ZB_APS_ADDR_MODE_16_ENDP_PRESENT, \
     ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).src_endpoint, \
     ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).dst_endpoint, \
     (_payload), (_payload_size), (_cb))


static zb_uint8_t *zb_zcl_metering_put_get_profile_response(zb_uint8_t *data, const void *pl_arg)
{

  const zb_zcl_metering_get_profile_response_payload_t *pl = pl_arg;

  ZB_ASSERT(pl_arg);
  ZB_ASSERT(data);

  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->end_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->status);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->profile_interval_period);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->number_of_periods_delivered);

  for (zb_int_t i = 0; i <= pl->number_of_periods_delivered - 1; i++)
  {
    ZB_ZCL_PACKET_PUT_DATA24(data, (pl->intervals + i));
  }

  return data;
}


static zb_uint8_t *zb_zcl_metering_put_get_sampled_data_response(zb_uint8_t *data, const void *pl_arg)
{

  const zb_zcl_metering_get_sampled_data_response_payload_t *pl = pl_arg;

  ZB_ASSERT(pl_arg);
  ZB_ASSERT(data);

  ZB_ZCL_PACKET_PUT_DATA16(data, &pl->sample_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->sample_start_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->sample_type);
  ZB_ZCL_PACKET_PUT_DATA16(data, &pl->sample_request_interval);
  ZB_ZCL_PACKET_PUT_DATA16(data, &pl->number_of_samples);

  for (zb_uint8_t i = 0; i <= pl->number_of_samples - 1; i++)
  {
    ZB_ZCL_PACKET_PUT_DATA24(data, (pl->samples + i));
  }

  return data;
}


static zb_uint8_t *zb_zcl_metering_put_publish_snapshot(zb_uint8_t *data, const void *pl_arg)
{

  const zb_zcl_metering_publish_snapshot_payload_t *pl = pl_arg;

  ZB_ASSERT(pl_arg);
  ZB_ASSERT(data);

  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->snapshot_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->snapshot_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->total_snapshots_found);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->command_index);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->total_number_of_commands);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->snapshot_cause);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->snapshot_payload_type);

  switch (pl->snapshot_payload_type)
  {
    case ZB_ZCL_METERING_TOU_DELIVERED_REGISTERS:
    {
      const zb_zcl_metering_tou_delivered_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_delivered);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_DELIVERED_REGISTERS", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_time_stamp_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_time_stamp_delivered);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->bill_delivered_trailing_digit);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_TOU_RECEIVED_REGISTERS:
    {
      const zb_zcl_metering_tou_received_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_received);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_RECEIVED_REGISTERS", (FMT__0));



      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_time_stamp_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_time_stamp_received);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->bill_received_trailing_digit);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_DELIVERED:
    {
      const zb_zcl_metering_block_delivered_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_delivered);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_DELIVERED", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_time_stamp_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_delivered);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_time_stamp_delivered);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->bill_delivered_trailing_digit);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }

      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_and_block_thresholds_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_and_block_thresholds_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_block_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_RECEIVED:
    {
      const zb_zcl_metering_block_received_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_received);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_RECEIVED", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->bill_to_date_time_stamp_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_received);
      ZB_ZCL_PACKET_PUT_DATA32(data, &sub_payload->projected_bill_time_stamp_received);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->bill_received_trailing_digit);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);


      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }

      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_and_block_thresholds_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_and_block_thresholds_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_block_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_TOU_DELIVERED_NO_BILLING:
    {
      const zb_zcl_metering_tou_delivered_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_delivered_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_DELIVERED_NO_BILLING", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_delivered);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_TOU_RECEIVED_NO_BILLING:
    {
      const zb_zcl_metering_tou_received_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_received_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_RECEIVED_NO_BILLING", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_received);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_DELIVERED_NO_BILLING:
    {
      const zb_zcl_metering_block_tier_delivered_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_tier_delivered_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_DELIVERED_NO_BILLING", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_delivered);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }

      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_and_block_thresholds_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_and_block_thresholds_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_block_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_RECEIVED_NO_BILLING:
    {
      const zb_zcl_metering_block_tier_received_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_tier_received_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_RECEIVED_NO_BILLING", (FMT__0));

      ZB_ZCL_PACKET_PUT_DATA48(data, &sub_payload->current_summation_received);
      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_summation + i));
      }

      ZB_ZCL_PACKET_PUT_DATA8(data, sub_payload->number_of_tiers_and_block_thresholds_in_use);

      for (zb_uint8_t i = 0; i <= sub_payload->number_of_tiers_and_block_thresholds_in_use - 1; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA48(data, (sub_payload->tier_block_summation + i));
      }
    }
      break;
    case ZB_ZCL_METERING_DATA_UNAVAILABLE:
      /*NOTE: from SE spec: The data for this snapshot is currently unavailable;
       * if used, there is currently no subsequent snapshot data.
       */
      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_DATA_UNAVAILABLE", (FMT__0));
      break;
    default:
      TRACE_MSG(TRACE_ZCL1, "Malformed packet: unsupported snapshot payload type", (FMT__0));
      break;
  }

  return data;
}


static zb_int16_t zb_zcl_metering_calculate_publish_snapshot_payload_size(const zb_zcl_metering_publish_snapshot_payload_t *pl)
{

  zb_int16_t pl_size = 0;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_calculate_publish_snapshot_payload_size", (FMT__0));

  pl_size += ZB_OFFSETOF(zb_zcl_metering_publish_snapshot_payload_t, snapshot_sub_payload);

  switch (pl->snapshot_payload_type)
  {
    case ZB_ZCL_METERING_TOU_DELIVERED_REGISTERS:
    {
      const zb_zcl_metering_tou_delivered_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_delivered);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_DELIVERED_REGISTERS", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_tou_delivered_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_TOU_RECEIVED_REGISTERS:
    {
      const zb_zcl_metering_tou_received_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_received);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_RECEIVED_REGISTERS", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_tou_received_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_DELIVERED:
    {
      const zb_zcl_metering_block_delivered_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_delivered);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_DELIVERED", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_block_delivered_payload_t, tier_summation);
      pl_size += (sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE);
      pl_size += 1;  /* 1 byte for number_of_tiers_and_block_thresholds_in_use */
      pl_size += sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_RECEIVED:
    {
      const zb_zcl_metering_block_received_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_received);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_RECEIVED", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_block_received_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
      pl_size += 1; /* 1 byte for number_of_tiers_and_block_thresholds_in_use */
      pl_size += sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_TOU_DELIVERED_NO_BILLING:
    {
      const zb_zcl_metering_tou_delivered_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_delivered_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_DELIVERED_NO_BILLING", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_tou_delivered_no_billing_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_TOU_RECEIVED_NO_BILLING:
    {
      const zb_zcl_metering_tou_received_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.tou_received_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_TOU_RECEIVED_NO_BILLING", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_tou_received_no_billing_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_DELIVERED_NO_BILLING:
    {
      const zb_zcl_metering_block_tier_delivered_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_tier_delivered_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_DELIVERED_NO_BILLING", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_block_tier_delivered_no_billing_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
      pl_size += 1; /*1 byte for number_of_tiers_and_block_thresholds_in_use*/
      pl_size += sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_BLOCK_TIER_RECEIVED_NO_BILLING:
    {
      const zb_zcl_metering_block_tier_received_no_billing_payload_t *sub_payload = &(pl->snapshot_sub_payload.block_tier_received_no_billing);

      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_BLOCK_TIER_RECEIVED_NO_BILLING", (FMT__0));

      pl_size += ZB_OFFSETOF(zb_zcl_metering_block_tier_received_no_billing_payload_t, tier_summation);
      pl_size += sub_payload->number_of_tiers_in_use * ZB_48BIT_SIZE;
      pl_size += 1; /*1 byte for number_of_tiers_and_block_thresholds_in_use*/
      pl_size += sub_payload->number_of_tiers_and_block_thresholds_in_use * ZB_48BIT_SIZE;
    }
      break;
    case ZB_ZCL_METERING_DATA_UNAVAILABLE:
      /*NOTE: from SE spec: The data for this snapshot is currently unavailable;
       * if used, there is currently no subsequent snapshot data.
       */
      TRACE_MSG(TRACE_ZCL1, "Snapshot payload type is ZB_ZCL_METERING_DATA_UNAVAILABLE", (FMT__0));
      break;
    default:
      TRACE_MSG(TRACE_ZCL1, "Malformed packet: unsupported snapshot payload type", (FMT__0));
      pl_size = -1;
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_calculate_publish_snapshot_payload_size", (FMT__0));

  return pl_size;
}


void zb_zcl_metering_send_cmd_publish_snapshot(zb_uint8_t param, const zb_addr_u *dst_addr,
                                                    zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                    zb_uint8_t src_ep,
                                                    const zb_zcl_metering_publish_snapshot_payload_t *pl,
                                                    zb_callback_t cb)
{

  zb_int16_t pl_size;

  pl_size = zb_zcl_metering_calculate_publish_snapshot_payload_size(pl);

  if ((pl_size <= 255)&& (pl_size > 0))
  {
    zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                    ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                    src_ep, pl,
                    (zb_uint8_t)pl_size,
                    zb_zcl_metering_put_publish_snapshot,
                    ZB_ZCL_CLUSTER_ID_METERING,
                    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                    ZB_ZCL_METERING_SRV_CMD_PUBLISH_SNAPSHOT,
                    cb);
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "zb_zcl_metering_send_cmd_publish_snapshot: invalid payload", (FMT__0));
    ZB_ZCL_SCHEDULE_STATUS_ABORT(param, (zb_addr_u *)dst_addr, dst_addr_mode, dst_ep, src_ep, cb);
  }
}


void zb_zcl_metering_send_cmd_request_fast_poll_mode_response(zb_uint8_t param, const zb_addr_u *dst_addr,
                                                                   zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                   zb_uint8_t src_ep,
                                                                   const zb_zcl_metering_request_fast_poll_mode_response_payload_t *pl,
                                                                   zb_uint8_t pl_size,
                                                                   zb_callback_t cb)
{

  zb_zcl_metering_request_fast_poll_mode_response_payload_t payload;
  zb_uint8_t *data = (zb_uint8_t *) &payload;

  ZB_BZERO(&payload, sizeof(payload));

  ZB_ZCL_PACKET_PUT_DATA8(data, pl->applied_update_period_in_seconds);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->fast_poll_mode_end_time);

  if (0 == pl_size)
  {
    pl_size = sizeof(payload);
  }

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                  ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                  src_ep, &payload, pl_size,
                  NULL,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_SRV_CMD_REQUEST_FAST_POLL_MODE_RESPONSE,
                  cb);

}


void zb_zcl_metering_send_cmd_get_profile_response(zb_uint8_t param, const zb_addr_u *dst_addr,
                                                        zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t src_ep, const zb_zcl_metering_get_profile_response_payload_t *pl,
                                                        zb_uint8_t pl_size, zb_callback_t cb)
{
  if (0 == pl_size)
  {
    pl_size = sizeof(zb_zcl_metering_get_profile_response_payload_t) - sizeof(void *) + ZB_24BIT_SIZE * pl->number_of_periods_delivered;
    TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_send_cmd_get_profile_response: "
                          "passed 0, calculated size = %d", (FMT__D, pl_size));
  }

  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                  ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                  src_ep, pl, pl_size,
                  zb_zcl_metering_put_get_profile_response,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_SRV_CMD_GET_PROFILE_RESPONSE,
                  cb);

}


void zb_zcl_metering_send_cmd_get_sampled_data_response(zb_uint8_t param, const zb_addr_u *dst_addr,
                                                             zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
                                                             zb_uint8_t src_ep,
                                                             const zb_zcl_metering_get_sampled_data_response_payload_t *pl,
                                                             zb_uint8_t pl_size, zb_callback_t cb)
{
  if (0 == pl_size)
  {
    pl_size = sizeof(zb_zcl_metering_get_sampled_data_response_payload_t) - sizeof(void *) + ZB_24BIT_SIZE * pl->number_of_samples;
    TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_send_cmd_get_sampled_data_response: "
                          "passed 0, calculated size = %d", (FMT__D, pl_size));
  }


  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                  ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                  src_ep, pl, pl_size,
                  zb_zcl_metering_put_get_sampled_data_response,
                  ZB_ZCL_CLUSTER_ID_METERING,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_METERING_SRV_CMD_GET_PROFILE_RESPONSE,
                  cb);

}


static const zb_uint8_t *zb_zcl_metering_get_snapshot_data_parse_payload(zb_zcl_metering_get_snapshot_payload_t *pl,
                                                                         zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_get_snapshot_data_parse_payload", (FMT__0));

  if (data_size != sizeof(zb_zcl_metering_get_snapshot_payload_t))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl->earliest_start_time, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->latest_end_time, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->snapshot_offset, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->snapshot_cause, data);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_get_snapshot_data_parse_payload", (FMT__0));

  return data;
}


static const zb_uint8_t *zb_zcl_metering_request_fast_poll_mode_parse_payload(
                                                zb_zcl_metering_request_fast_poll_mode_payload_t *pl,
                                                zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_request_fast_poll_mode_parse_payload", (FMT__0));

  if (data_size != sizeof(zb_zcl_metering_request_fast_poll_mode_payload_t))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA8(&pl->fast_poll_update_period, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->duration_in_minutes, data);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_request_fast_poll_mode_parse_payload", (FMT__0));

  return data;
}


static const zb_uint8_t *zb_zcl_metering_get_sampled_data_parse_payload(zb_zcl_metering_get_sampled_data_payload_t *pl,
                                                                        zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_get_sampled_data_parse_payload", (FMT__0));

  if (data_size != sizeof(zb_zcl_metering_get_sampled_data_payload_t))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA16(&pl->sample_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->earliest_sample_time, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->sample_type, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl->number_of_samples, data);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_get_sampled_data_parse_payload", (FMT__0));

  return data;
}


static const zb_uint8_t *zb_zcl_metering_get_profile_parse_payload(zb_zcl_metering_get_profile_payload_t *pl,
                                                                   zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_metering_get_profile_parse_payload", (FMT__0));

  /* NOTE: User must check whether the number_of_periods is greater than MaxNumberOfPeriodsDelivered. */
  if (!ZB_ZCL_METERING_GET_PROFILE_PAYLOAD_SIZE_IS_VALID(data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA8(&pl->interval_channel, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->end_time, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->number_of_periods, data);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_metering_get_profile_parse_payload", (FMT__0));

  return data;
}


static zb_bool_t zb_zcl_metering_process_request_fast_poll_mode(zb_uint8_t param,
                                                                const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_request_fast_poll_mode_payload_t pl_in ;
  zb_zcl_metering_request_fast_poll_mode_response_payload_t pl_out;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_request_fast_poll_mode", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_REQUEST_FAST_POLL_MODE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, &pl_out);

  if (!zb_zcl_metering_request_fast_poll_mode_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid request_fast_poll_mode", (FMT__0));
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
      ZB_ZCL_METERING_SEND_CMD_HELPER(zb_zcl_metering_send_cmd_request_fast_poll_mode_response,
                                      param, cmd_info, &pl_out, 0, NULL);
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

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_request_fast_poll_mode", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_metering_process_get_snapshot(zb_uint8_t param,
                                                      const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_get_snapshot_payload_t pl_in ;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_get_snapshot", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_GET_SNAPSHOT_CB_ID,
                                    RET_NOT_FOUND, cmd_info, &pl_in, NULL);

  if (!zb_zcl_metering_get_snapshot_data_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid get_profile", (FMT__0));
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
      /*We don't send a PublishSnapshot command. Application must send reply
       * for this command and free buffer.
       */
      break;
    case RET_NOT_FOUND:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
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

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_get_snapshot", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_metering_process_get_sampled_data(zb_uint8_t param,
                                                          const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_get_sampled_data_payload_t pl_in ;
  zb_zcl_metering_get_sampled_data_response_payload_t pl_out;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_get_sampled_data", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_GET_SAMPLED_DATA_CB_ID,
                                    RET_NOT_FOUND, cmd_info, &pl_in, &pl_out);

  if (!zb_zcl_metering_get_sampled_data_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid get_profile", (FMT__0));
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
      ZB_ZCL_METERING_SEND_CMD_HELPER(zb_zcl_metering_send_cmd_get_sampled_data_response,
                                      param, cmd_info, &pl_out, 0, NULL);
      break;
    case RET_NOT_FOUND:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
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

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_get_sampled_data", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_metering_process_get_profile(zb_uint8_t param,
                                                     const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_metering_get_profile_payload_t pl_in ;
  zb_zcl_metering_get_profile_response_payload_t pl_out;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_metering_process_get_profile", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_METERING_GET_PROFILE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, &pl_out);

  if (!zb_zcl_metering_get_profile_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid get_profile", (FMT__0));
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
      ZB_ZCL_METERING_SEND_CMD_HELPER(zb_zcl_metering_send_cmd_get_profile_response,
                                      param, cmd_info, &pl_out, 0, NULL);
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

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_metering_process_get_profile", (FMT__0));

  return ZB_TRUE;
}


static zb_bool_t zb_zcl_process_metering_server_commands(zb_uint8_t param,
                                                         const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch ((zb_zcl_metering_cli_cmd_t)cmd_info->cmd_id)
  {
    case ZB_ZCL_METERING_CLI_CMD_GET_PROFILE:
      processed = zb_zcl_metering_process_get_profile(param, cmd_info);
      break;
    case ZB_ZCL_METERING_CLI_CMD_REQUEST_FAST_POLL_MODE:
      processed = zb_zcl_metering_process_request_fast_poll_mode(param, cmd_info);
      break;
    case ZB_ZCL_METERING_CLI_CMD_GET_SNAPSHOT:
      processed = zb_zcl_metering_process_get_snapshot(param, cmd_info);
      break;
    case ZB_ZCL_METERING_CLI_CMD_GET_SAMPLED_DATA:
      processed = zb_zcl_metering_process_get_sampled_data(param, cmd_info);
      break;
    default:
      break;
  }

  return processed;
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_METERING_OPTIONAL */


zb_bool_t zb_zcl_process_s_metering_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t           processed = ZB_FALSE;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_s_metering_specific_commands, "
            "param=%hd, cmd_id=%hd", (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_METERING);

#ifdef ZB_ZCL_SUPPORT_CLUSTER_METERING_OPTIONAL
  if (ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction)
  {
    processed = zb_zcl_process_metering_server_commands(param, &cmd_info);
  }
#endif

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_s_metering_specific_commands (ret=%hd)",
            (FMT__H, processed));

  return processed;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_METERING || defined DOXYGEN */
