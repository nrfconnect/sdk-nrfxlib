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
/* PURPOSE: SERVER: Daily Schedule Cluster
*/

#define ZB_TRACE_FILE_ID 13

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_DAILY_SCHEDULE) || defined DOXYGEN

#include "zcl/zb_zcl_daily_schedule.h"
/******************************************************************************/
/* Common definitions */

zb_uint8_t gs_daily_schedule_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_daily_schedule_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE_SERVER_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_daily_schedule_server_cmd_list =
{
  sizeof(gs_daily_schedule_server_received_commands), gs_daily_schedule_server_received_commands,
  sizeof(gs_daily_schedule_server_generated_commands), gs_daily_schedule_server_generated_commands
};

/* FIXME: Some commands are sent via follow macros. But we usually use ZB_APS_ADDR_MODE_64_ENDP_PRESENT address mode*/
/** Helper for sending cluster commands
 * @param _fn - sending cluster command function
 * @param _param - user parameter
 * @param _cmd_info - command info
 * @param _payload - sending payload. Corresponds to sending command.
 * @param _cb - Callback which should be called when the ZCL stack receives
 * APS ack.
 */
#define ZB_ZCL_DAILY_SCHEDULE_SEND_CMD_HELPER(_fn, _param, _cmd_info, _payload, _cb)  \
  _fn((_param), \
     (zb_addr_u *) &ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).source, \
     ZB_APS_ADDR_MODE_16_ENDP_PRESENT, \
     ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).src_endpoint, \
     ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).dst_endpoint, \
     (_payload), (_cb) \
  )

zb_bool_t zb_zcl_process_s_daily_schedule_specific_commands(zb_uint8_t param);

static zb_ret_t check_value_daily_schedule(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);


void zb_zcl_daily_schedule_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)check_value_daily_schedule,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_s_daily_schedule_specific_commands);
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

PUT_PL_START(zb_zcl_daily_schedule_publish_schedule_payload_t, pl)
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->provider_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->schedule_id);
  ZB_ZCL_PACKET_PUT_DATA16(data, &pl->day_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->start_time);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->schedule_type);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->schedule_time_reference);
  TRACE_MSG(TRACE_ZCL1, "schedule_name size %hd", (FMT__H, pl->schedule_name[0]));
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->schedule_name[0]);
  ZB_ZCL_PACKET_PUT_DATA_N(data, pl->schedule_name+1, pl->schedule_name[0]);
PUT_PL_END()

PUT_PL_START(zb_zcl_daily_schedule_cancel_schedule_payload_t, pl)
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->provider_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->schedule_id);
  ZB_ZCL_PACKET_PUT_DATA8(data, pl->schedule_type);
PUT_PL_END()

PUT_PL_START(zb_zcl_daily_schedule_publish_day_profile_payload_t, pl)
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->provider_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA32(data, &pl->schedule_id);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->total_number_of_schedule_entries);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->command_index);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->total_number_of_commands);
  ZB_ZCL_PACKET_PUT_DATA8 (data, pl->schedule_type);
  switch (pl->schedule_type)
  {
    case ZB_ZCL_DAILY_SCHEDULE_TYPE_LINKY_SCHEDULE:
    {
      zb_uint8_t i;
      for (i = 0; i < pl->number_of_entries_in_this_command; i++)
      {
        ZB_ZCL_PACKET_PUT_DATA16(data, &pl->day_schedule_entries[i].linky_schedule_entry.start_time);
        ZB_ZCL_PACKET_PUT_DATA8(data, pl->day_schedule_entries[i].linky_schedule_entry.price_tier);
        ZB_ZCL_PACKET_PUT_DATA8(data, pl->day_schedule_entries[i].linky_schedule_entry.auxiliary_load_switch_state);
      }

      break;
    }
    default:
      /* none */
      break;
  }
PUT_PL_END()

#undef PUT_PL_START
#undef PUT_PL_END

void zb_zcl_daily_schedule_send_cmd_publish_schedule(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  const zb_zcl_daily_schedule_publish_schedule_payload_t *pl,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                       ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                       src_ep, pl, sizeof(*pl),
                       PUT_PL_FUN(zb_zcl_daily_schedule_publish_schedule_payload_t),
                       ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                       ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                       ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_PUBLISH_SCHEDULE,
                       cb);
}


void zb_zcl_daily_schedule_send_cmd_publish_day_profile(zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  const zb_zcl_daily_schedule_publish_day_profile_payload_t *pl,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                  ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                  src_ep, pl, sizeof(*pl),
                  PUT_PL_FUN(zb_zcl_daily_schedule_publish_day_profile_payload_t),
                  ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                  ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                  ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_PUBLISH_DAY_PROFILE,
                  cb);
}

void zb_zcl_daily_schedule_send_cmd_cancel_schedule(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  const zb_zcl_daily_schedule_cancel_schedule_payload_t *pl,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                       ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                       src_ep, pl, sizeof(*pl),
                       PUT_PL_FUN(zb_zcl_daily_schedule_cancel_schedule_payload_t),
                       ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                       ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                       ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_CANCEL_SCHEDULE,
                       cb);
}

void zb_zcl_daily_schedule_send_cmd_cancel_all_schedules(zb_uint8_t param,
  const zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep, zb_uint8_t src_ep,
  zb_callback_t cb
)
{
  zb_zcl_send_cmd(param, dst_addr, dst_addr_mode, dst_ep,
                       ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                       src_ep, NULL, 0,
                       NULL,
                       ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,
                       ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                       ZB_ZCL_DAILY_SCHEDULE_SRV_CMD_CANCEL_ALL_SCHEDULES,
                       cb);
}
/******************************************************************************/
/* Server definitions */

static ZB_INLINE const zb_uint8_t *zb_zcl_daily_schedule_get_schedule_parse_payload(
  zb_zcl_daily_schedule_get_schedule_payload_t *pl, zb_uint8_t param)
{
  zb_uint8_t *data = (zb_uint8_t *)zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  ZB_ASSERT(data);

  if (!ZB_ZCL_DAILY_SCHEDULE_GET_SCHEDULE_PL_SIZE_IS_VALID(data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->earliest_start_time, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl->min_issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->number_of_schedules, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl->schedule_type, data);

  return data;
}

static const zb_uint8_t *zb_zcl_daily_schedule_get_day_profile_parse_payload(
  zb_zcl_daily_schedule_get_day_profile_payload_t *pl, zb_uint8_t param)
{
  zb_uint8_t *data = zb_buf_begin(param);
  zb_uint8_t data_size = zb_buf_len(param);

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_daily_schedule_get_day_profile_parse_payload", (FMT__0));

  if (!ZB_ZCL_DAILY_SCHEDULE_GET_DAY_PROFILE_PL_SIZE_IS_VALID(data_size))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload size %hd", (FMT__H, data_size));
    return NULL;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl->provider_id, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl->day_id, data);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_daily_schedule_get_day_profile_parse_payload", (FMT__0));

  return data;
}


static zb_bool_t zb_zcl_daily_schedule_process_get_schedule(zb_uint8_t param,
                                                 const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_daily_schedule_get_schedule_payload_t pl_in = ZB_ZCL_DAILY_SCHEDULE_GET_SCHEDULE_PL_INIT;
  zb_zcl_daily_schedule_publish_schedule_payload_t pl_out = ZB_ZCL_DAILY_SCHEDULE_PUBLISH_SCHEDULE_PL_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_daily_schedule_process_get_schedule", (FMT__0));

  if (!zb_zcl_daily_schedule_get_schedule_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_DAILY_SCHEDULE_GET_SCHEDULE_CB_ID, RET_NOT_FOUND, cmd_info, &pl_in, &pl_out);
  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    zb_buf_free(param);
  }
  else
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_daily_schedule_process_get_schedule", (FMT__0));
  return ZB_TRUE;
}


static zb_bool_t zb_zcl_daily_schedule_process_get_day_profile(zb_uint8_t param,
                                                 const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_daily_schedule_get_day_profile_payload_t pl_in = ZB_ZCL_DAILY_SCHEDULE_GET_DAY_PROFILE_PL_INIT;
  zb_zcl_daily_schedule_publish_day_profile_payload_t pl_out = ZB_ZCL_DAILY_SCHEDULE_PUBLISH_DAY_PROFILE_PL_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_daily_schedule_process_get_day_profile", (FMT__0));

  if (!zb_zcl_daily_schedule_get_day_profile_parse_payload(&pl_in, param))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid payload", (FMT__0));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return ZB_TRUE;
  }

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
                                    ZB_ZCL_DAILY_SCHEDULE_GET_DAY_PROFILE_CB_ID,
                                    RET_NOT_FOUND, cmd_info, &pl_in, &pl_out);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    ZB_ZCL_DAILY_SCHEDULE_SEND_CMD_HELPER(zb_zcl_daily_schedule_send_cmd_publish_day_profile, param, cmd_info, &pl_out, NULL);
  }
  else
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_daily_schedule_process_get_day_profile", (FMT__0));
  return ZB_TRUE;
}


static zb_bool_t zb_zcl_daily_schedule_process_get_schedule_cancellation(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_daily_schedule_cancel_schedule_payload_t pl_out = ZB_ZCL_DAILY_SCHEDULE_CANCEL_SCHEDULE_PL_INIT;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_daily_schedule_process_get_schedule_cancellation", (FMT__0));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
                                    ZB_ZCL_DAILY_SCHEDULE_GET_SCHEDULE_CANCELLATION_CB_ID,
                                    RET_NOT_FOUND, cmd_info, NULL, &pl_out);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
#ifdef ZB_ZCL_DAILY_SCHEDULE_CANCEL_PARTICULAR_SCHEDULE
    ZB_ZCL_DAILY_SCHEDULE_SEND_CMD_HELPER(zb_zcl_daily_schedule_send_cmd_cancel_schedule, param, cmd_info, &pl_out, NULL);
#else
    zb_zcl_daily_schedule_send_cmd_cancel_all_schedules(param,
                                                        (zb_addr_u *) &ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source,
                                                        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                                        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
                                                        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                                        NULL);
#endif
  }
  else
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_NOT_FOUND);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_daily_schedule_process_get_schedule_cancellation", (FMT__0));
  return ZB_TRUE;
}


static zb_bool_t zb_zcl_process_daily_schedule_srv_cmd(zb_uint8_t param,
                                            const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_bool_t processed = ZB_FALSE;

  switch((zb_zcl_daily_schedule_cli_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_DAILY_SCHEDULE_CLI_CMD_GET_SCHEDULE:
      processed = zb_zcl_daily_schedule_process_get_schedule(param, cmd_info);
      break;
    case ZB_ZCL_DAILY_SCHEDULE_CLI_CMD_GET_DAY_PROFILE:
      processed = zb_zcl_daily_schedule_process_get_day_profile(param, cmd_info);
      break;
    case ZB_ZCL_DAILY_SCHEDULE_CLI_CMD_GET_SCHEDULE_CANCELLATION:
      processed = zb_zcl_daily_schedule_process_get_schedule_cancellation(param, cmd_info);
      break;
#ifdef WIP
    default:
      break;
#endif
    /* FIXME: add default case */
  }

  return processed;
}

/******************************************************************************/
/* Handler definition */
zb_bool_t zb_zcl_process_s_daily_schedule_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t           processed = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_s_daily_schedule_specific_commands, "
            "param=%hd", (FMT__H, param));

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_daily_schedule_server_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE);

  if (ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction)
  {
    processed = zb_zcl_process_daily_schedule_srv_cmd(param, &cmd_info);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_s_daily_schedule_specific_commands (cmd_id=%hd, ret=%hd)",
            (FMT__H_H, cmd_info.cmd_id, processed));

  return processed;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_DAILY_SCHEDULE || defined DOXYGEN */
