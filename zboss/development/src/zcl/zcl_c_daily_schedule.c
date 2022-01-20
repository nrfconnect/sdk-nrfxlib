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
/* PURPOSE: CLIENT: Daily Schedule Cluster
*/

#define ZB_TRACE_FILE_ID 12

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_DAILY_SCHEDULE) || defined DOXYGEN

#include "zcl/zb_zcl_daily_schedule.h"

/******************************************************************************/
/* Common definitions */

zb_uint8_t gs_daily_schedule_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_daily_schedule_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_daily_schedule_client_cmd_list =
{
  sizeof(gs_daily_schedule_client_received_commands), gs_daily_schedule_client_received_commands,
  sizeof(gs_daily_schedule_client_generated_commands), gs_daily_schedule_client_generated_commands
};


zb_bool_t zb_zcl_process_c_daily_schedule_specific_commands(zb_uint8_t param);

static zb_ret_t check_value_daily_schedule(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_daily_schedule_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)check_value_daily_schedule,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_c_daily_schedule_specific_commands);
}


static zb_ret_t check_value_daily_schedule(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}

#define PUT_PL_FUN(type) type ## _put_payload
#define PUT_PL_START(type, pl_var) \
static zb_uint8_t *PUT_PL_FUN(type)(zb_uint8_t *data, const void *pl_arg) \
{ \
  const type *pl_var = pl_arg; \
  ZB_ASSERT(pl_arg); \
  ZB_ASSERT(data);

#define PUT_PL_END() return data; }

PUT_PL_START(zb_zcl_daily_schedule_get_schedule_payload_t, pl)
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->provider_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->earliest_start_time);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->min_issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->number_of_schedules);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->schedule_type);

PUT_PL_END()

PUT_PL_START(zb_zcl_daily_schedule_get_day_profile_payload_t, pl)
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->provider_id);
  ZB_ZCL_PACKET_PUT_DATA16(data, &pl->day_id);
PUT_PL_END()

#undef PUT_PL_START
#undef PUT_PL_END

void zb_zcl_daily_schedule_send_cmd_get_schedule(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  const zb_zcl_daily_schedule_get_schedule_payload_t *pl,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                       ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                       src_ep, pl, sizeof(*pl),
                       PUT_PL_FUN(zb_zcl_daily_schedule_get_schedule_payload_t),
                       ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                       ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                       ZB_ZCL_DAILY_SCHEDULE_CLI_CMD_GET_SCHEDULE,
                       cb);
}


void zb_zcl_daily_schedule_send_cmd_get_day_profile(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  const zb_zcl_daily_schedule_get_day_profile_payload_t *pl,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                       ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                       src_ep, pl, sizeof(*pl),
                       PUT_PL_FUN(zb_zcl_daily_schedule_get_day_profile_payload_t),
                       ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                       ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                       ZB_ZCL_DAILY_SCHEDULE_CLI_CMD_GET_DAY_PROFILE,
                       cb);
}


void zb_zcl_daily_schedule_send_cmd_get_schedule_cancellation(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                  ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                  src_ep, NULL, 0, NULL,
                  ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_DAILY_SCHEDULE_CLI_CMD_GET_SCHEDULE_CANCELLATION,
                  cb);
}


/******************************************************************************/
/* Client definitions */

//+
static const zb_uint8_t *zb_zcl_daily_schedule_publish_schedule_parse_payload(
  zb_zcl_daily_schedule_publish_schedule_payload_t *pl, zb_uint8_t bufid)
{
  zb_uint8_t *data = zb_buf_begin(bufid);
  zb_uint8_t data_size = zb_buf_len(bufid);

  if (!ZB_ZCL_DAILY_SCHEDULE_PUBLISH_SCHEDULE_PL_SIZE_IS_VALID(data, data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd, expected: %hd", (FMT__H_H,
      data_size, ZB_ZCL_DAILY_SCHEDULE_PUBLISH_SCHEDULE_PL_EXPECTED_SIZE(pl)));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->schedule_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->start_time, data);
  ZB_ZCL_PACKET_GET_DATA8 (&pl->schedule_type, data);
  ZB_ZCL_PACKET_GET_DATA8 (&pl->schedule_time_reference, data);

  ZB_ZCL_PACKET_GET_STATIC_STRING(pl->schedule_name, data);

  if (!data)
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid string len = %hd",
              (FMT__H, ZB_ZCL_GET_STRING_LENGTH(pl->schedule_name)));
    return NULL;
  }

  return data;
}


static const zb_uint8_t *zb_zcl_daily_schedule_publish_day_profile_parse_payload(
  zb_zcl_daily_schedule_publish_day_profile_payload_t *pl,
  zb_uint8_t bufid)
{
  zb_uint8_t *data = zb_buf_begin(bufid);
  zb_uint8_t data_size = zb_buf_len(bufid);
  zb_uint8_t data_len = 0;
  zb_uint8_t entry_count = 0, i;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_daily_schedule_publish_day_profile_parse_payload", (FMT__0));

  if (!ZB_ZCL_DAILY_SCHEDULE_PUBLISH_DAY_PROFILE_PL_SIZE_IS_VALID(data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->schedule_id, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->total_number_of_schedule_entries, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->command_index, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->total_number_of_commands, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->schedule_type, data);
  pl->day_schedule_entries = (zb_zcl_daily_schedule_entry_t *)data;

  data_len = data_size - (sizeof(zb_zcl_daily_schedule_publish_day_profile_payload_t) - sizeof(void *) - sizeof(zb_uint8_t));

  switch (pl->schedule_type)
  {
    case ZB_ZCL_DAILY_SCHEDULE_TYPE_LINKY_SCHEDULE:
      entry_count = data_len / sizeof(zb_zcl_daily_schedule_linky_schedule_entry_t);

      /* Restore byte order */
      for (i = 0; i < entry_count; i++)
      {
        ZB_LETOH16_ONPLACE(pl->day_schedule_entries[i].linky_schedule_entry.start_time);
      }

      break;
  }

  pl->number_of_entries_in_this_command = entry_count;

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_daily_schedule_publish_day_profile_parse_payload", (FMT__0));

  return data;
}


static const zb_uint8_t *zb_zcl_daily_schedule_cancel_schedule_parse_payload(
                                            zb_zcl_daily_schedule_cancel_schedule_payload_t *pl,
                                            zb_uint8_t bufid)
{
  zb_uint8_t *data = zb_buf_begin(bufid);
  zb_uint8_t data_size = zb_buf_len(bufid);

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_daily_schedule_cancel_schedule_parse_payload", (FMT__0));

  if (!ZB_ZCL_DAILY_SCHEDULE_CANCEL_SCHEDULE_PL_SIZE_IS_VALID(data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->schedule_id, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->schedule_type, data);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_daily_schedule_cancel_schedule_parse_payload", (FMT__0));

  return data;
}

static zb_bool_t zb_zcl_daily_schedule_process_publish_schedule(zb_uint8_t param,
                                                     const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_daily_schedule_publish_schedule_payload_t  pl_in = ZB_ZCL_DAILY_SCHEDULE_PUBLISH_SCHEDULE_PL_INIT;

  TRACE_MSG(TRACE_ZCL3, ">> zb_zcl_daily_schedule_process_publish_schedule", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_DAILY_SCHEDULE_PUBLISH_SCHEDULE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);

  if (!zb_zcl_daily_schedule_publish_schedule_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
    (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL3, "<< zb_zcl_daily_schedule_process_publish_schedule", (FMT__0));

  return ZB_TRUE;
}

static zb_bool_t zb_zcl_daily_schedule_process_publish_day_profile(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_daily_schedule_publish_day_profile_payload_t pl_in = ZB_ZCL_DAILY_SCHEDULE_PUBLISH_DAY_PROFILE_PL_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_daily_schedule_process_publish_day_profile", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_DAILY_SCHEDULE_PUBLISH_DAY_PROFILE_CB_ID,
                                    RET_ERROR, cmd_info, &pl_in, NULL);
  if (!zb_zcl_daily_schedule_publish_day_profile_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid publish day profile payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
    (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_daily_schedule_process_publish_day_profile", (FMT__0));
  return ZB_TRUE;
}

static zb_bool_t zb_zcl_daily_schedule_process_cancel_schedule(zb_uint8_t param,
                                                    const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_daily_schedule_cancel_schedule_payload_t pl_in = ZB_ZCL_DAILY_SCHEDULE_CANCEL_SCHEDULE_PL_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_daily_schedule_process_cancel_schedule", (FMT__0));

  if (!zb_zcl_daily_schedule_cancel_schedule_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid cancel schedule payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_DAILY_SCHEDULE_CANCEL_SCHEDULE_CB_ID,
                                    RET_NOT_FOUND, cmd_info, &pl_in, NULL);
  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
    (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_daily_schedule_process_cancel_schedule", (FMT__0));

  return ZB_TRUE;
}

static zb_bool_t zb_zcl_daily_schedule_process_cancel_all_schedules(zb_uint8_t param,
                                                    const zb_zcl_parsed_hdr_t *cmd_info)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_daily_schedule_process_cancel_all_schedules", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_DAILY_SCHEDULE_CANCEL_ALL_SCHEDULES_CB_ID,
                                    RET_NOT_FOUND, cmd_info, NULL, NULL);
  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  zb_zcl_send_default_handler(param, cmd_info,
    (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_daily_schedule_process_cancel_all_schedules", (FMT__0));

  return ZB_TRUE;
}

static zb_bool_t zb_zcl_process_daily_schedule_cli_cmd(zb_uint8_t param,
                                            const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch( (zb_zcl_daily_schedule_srv_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_PUBLISH_SCHEDULE:
      processed = zb_zcl_daily_schedule_process_publish_schedule(param, cmd_info);
      break;
    case ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_PUBLISH_DAY_PROFILE:
      processed = zb_zcl_daily_schedule_process_publish_day_profile(param, cmd_info);
      break;
    case ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_CANCEL_SCHEDULE:
      processed = zb_zcl_daily_schedule_process_cancel_schedule(param, cmd_info);
      break;
    case ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_CANCEL_ALL_SCHEDULES:
      processed = zb_zcl_daily_schedule_process_cancel_all_schedules(param, cmd_info);
      break;
    /* FIXME: add default case */
#ifdef WIP
    default:
      break;
#endif
  }

  return processed;
}


/******************************************************************************/
/* Handler definition */
zb_bool_t zb_zcl_process_c_daily_schedule_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t           processed = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_c_daily_schedule_specific_commands, "
            "param=%hd", (FMT__H, param ));
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_daily_schedule_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE);

  if (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction)
  {
    processed = zb_zcl_process_daily_schedule_cli_cmd(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_c_daily_schedule_specific_commands (ret=%hd, cmd_id=%hd)",
            (FMT__H_H, processed, cmd_info.cmd_id));

  return processed;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_DAILY_SCHEDULE || defined DOXYGEN */
