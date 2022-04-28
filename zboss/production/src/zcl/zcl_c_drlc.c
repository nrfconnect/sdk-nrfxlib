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
/* PURPOSE: CLIENT: Demand Response and Load Control cluster defintions

*/

#define ZB_TRACE_FILE_ID 24548

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_DRLC) || defined DOXYGEN

#include "zcl/zb_zcl_drlc.h"

/* Init 'zb_addr_u' from 'zb_zcl_parsed_hdr_t.src_addr' field */
#define ZB_ADDR_INIT_FROM_CMD_INFO(_cmd_info) \
  { \
    .addr_short = ZB_ZCL_PARSED_HDR_SHORT_DATA(_cmd_info).source.u.short_addr \
  }

zb_uint8_t gs_drlc_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DRLC_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_drlc_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_DRLC_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_drlc_client_cmd_list =
{
  sizeof(gs_drlc_client_received_commands), gs_drlc_client_received_commands,
  sizeof(gs_drlc_client_generated_commands), gs_drlc_client_generated_commands
};

zb_bool_t zb_zcl_process_c_drlc_specific_commands(zb_uint8_t param);

static zb_ret_t check_value_drlc(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_drlc_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DRLC,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)check_value_drlc,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_c_drlc_specific_commands);
}

static zb_ret_t check_value_drlc(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID:
      if( ZB_ZCL_ATTR_GET16(value) > ZB_ZCL_DRLC_CLUSTER_REVISION_MAX )
      {
        ret = RET_ERROR;
      }
      break;
    default:
      ret = RET_OK;
      break;
  }
  
  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return ret;
}
/**
 * Client
 *
 * No Attributes
 *
 * 2 mandatory commands generated
 * 3 mandatory commands handled
 *
 */

void zb_drlc_client_send_report_event_status(zb_uint8_t param,
  zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
  zb_uint8_t src_ep, zb_zcl_drlc_report_event_status_payload_t *payload,
  zb_callback_t cb)
{
  zb_zcl_drlc_report_event_status_payload_t pl = ZB_ZCL_DRLC_REPORT_EVENT_STATUS_PAYLOAD_INIT;
  zb_uint8_t *data = (zb_uint8_t *)&pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_drlc_client_send_report_event_status", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));

  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->issuer_event_id);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->event_status);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->event_status_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->criticality_level_applied);
  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->cooling_temperature_set_point_applied);
  ZB_ZCL_PACKET_PUT_DATA16(data, &payload->heating_temperature_set_point_applied);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->average_load_adjustment_percentage_applied);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->duty_cycle_applied);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->event_control);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->signature_type);
  ZB_ZCL_PACKET_PUT_DATA_N(data, &payload->signature, sizeof(pl.signature));

  zb_zcl_send_cmd(
    param, dst_addr, dst_addr_mode,
    dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    &pl, sizeof(pl), NULL,
    ZB_ZCL_CLUSTER_ID_DRLC,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_DRLC_CLI_CMD_REPORT_EVENT_STATUS,
    cb
  );

  TRACE_MSG(TRACE_ZCL1, "<< zb_drlc_client_send_report_event_status", (FMT__0));
}

void zb_drlc_client_send_get_scheduled_events(zb_uint8_t param,
  zb_addr_u *dst_addr, zb_aps_addr_mode_t dst_addr_mode, zb_uint8_t dst_ep,
  zb_uint8_t src_ep, zb_zcl_drlc_get_scheduled_events_payload_t *payload,
  zb_callback_t cb)
{
  zb_zcl_drlc_get_scheduled_events_payload_t pl = ZB_ZCL_DRLC_CMD_GET_SCHEDULED_EVENTS_PAYLOAD_INIT;
  zb_uint8_t *data = (zb_uint8_t *)&pl;

  TRACE_MSG(TRACE_ZCL1, ">> zb_drlc_client_send_get_scheduled_events", (FMT__0));

  ZB_BZERO(&pl, sizeof(pl));

  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->start_time);
  ZB_ZCL_PACKET_PUT_DATA8(data, payload->number_of_events);
  ZB_ZCL_PACKET_PUT_DATA32(data, &payload->issuer_event_id);

  zb_zcl_send_cmd(
    param, dst_addr, dst_addr_mode,
    dst_ep, ZB_ZCL_FRAME_DIRECTION_TO_SRV,
    src_ep,
    &pl, sizeof(pl), NULL,
    ZB_ZCL_CLUSTER_ID_DRLC,
    ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
    ZB_ZCL_DRLC_CLI_CMD_GET_SCHEDULED_EVENTS,
    cb
  );

  TRACE_MSG(TRACE_ZCL1, "<< zb_drlc_client_send_get_scheduled_events", (FMT__0));
}

zb_ret_t zb_drlc_client_handle_load_control_event(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_drlc_lce_payload_t                  pl_in = ZB_ZCL_DRLC_LCE_PAYLOAD_INIT;
  zb_zcl_drlc_report_event_status_payload_t  pl_out = ZB_ZCL_DRLC_REPORT_EVENT_STATUS_PAYLOAD_INIT;
  zb_addr_u                                  dst_addr = ZB_ADDR_INIT_FROM_CMD_INFO(cmd_info);
  zb_uint8_t                                *data = zb_buf_begin(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_drlc_client_handle_load_control_event", (FMT__0));

  if (!ZB_ZCL_DRLC_SRV_CMD_LOAD_CONTROL_EVENT_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return RET_INVALID_FORMAT;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl_in.issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl_in.device_class, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.utility_enrollment_group, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl_in.start_time, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl_in.duration_in_minutes, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.criticality_level, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.cooling_temperature_offset, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.heating_temperature_offset, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl_in.cooling_temperature_set_point, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl_in.heating_temperature_set_point, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.average_load_adjustment_percentage, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.duty_cycle, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.event_control, data);

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_DRLC_LOAD_CONTROL_EVENT_CB_ID, RET_OK, cmd_info, &pl_in, &pl_out);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    zb_drlc_client_send_report_event_status(param,
      &dst_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      &pl_out, NULL);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "<< error in user cb call:%d", (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
    zb_buf_free(param);
    return RET_ERROR;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_drlc_client_handle_load_control_event", (FMT__0));

  return RET_OK;
}

zb_ret_t zb_drlc_client_handle_cancel_load_control_event(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_drlc_cancel_lce_payload_t              pl_in = ZB_ZCL_DRLC_CANCEL_LCE_PAYLOAD_INIT;
  zb_zcl_drlc_report_event_status_payload_t     pl_out = ZB_ZCL_DRLC_REPORT_EVENT_STATUS_PAYLOAD_INIT;
  zb_addr_u                                     dst_addr = ZB_ADDR_INIT_FROM_CMD_INFO(cmd_info);
  zb_uint8_t                                   *data = zb_buf_begin(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_drlc_client_handle_cancel_load_control_event", (FMT__0));

  if (!ZB_ZCL_DRLC_SRV_CMD_CANCEL_LOAD_CONTROL_EVENT_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return RET_INVALID_FORMAT;
  }

  ZB_ZCL_PACKET_GET_DATA32(&pl_in.issuer_event_id, data);
  ZB_ZCL_PACKET_GET_DATA16(&pl_in.device_class, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.utility_enrollment_group, data);
  ZB_ZCL_PACKET_GET_DATA8(&pl_in.cancel_control, data);
  ZB_ZCL_PACKET_GET_DATA32(&pl_in.effective_time, data);

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_DRLC_CANCEL_LOAD_CONTROL_EVENT_CB_ID, RET_ERROR, cmd_info, &pl_in, &pl_out);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_drlc_client_handle_cancel_load_control_event", (FMT__0));

/*Note: If the Cancel Load Control Event command is received after the event has ended, the
device shall reply using the "Report Event Status Command" with an Event Status of "Rejected
- Invalid Cancel Command (Undefined Event)"*/
/**
 * We do not send replies, this is application's task since it is responsible
 * for calling send_report_event for each registered cancelled event
 */

  if (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param) == RET_OK)
  {
    zb_drlc_client_send_report_event_status(param,
      &dst_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      &pl_out,
      NULL);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "<< error in user cb call:%d", (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
    zb_buf_free(param);
    return RET_ERROR;
  }

  return RET_OK;
}

zb_ret_t zb_drlc_client_handle_cancel_all_load_control_events(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_drlc_cancel_alce_payload_t  pl_in = ZB_ZCL_DRLC_CANCEL_ALCE_PAYLOAD_INIT;
  zb_uint8_t *data = zb_buf_begin(param);

  TRACE_MSG(TRACE_ZCL1, ">> zb_drlc_client_handle_cancel_all_load_control_events", (FMT__0));

  if (!ZB_ZCL_DRLC_SRV_CMD_CANCEL_ALL_LOAD_CONTROL_EVENTS_IS_VALID(zb_buf_len(param)))
  {
    TRACE_MSG(TRACE_ZCL1, "Invalid packet len (%hd).", (FMT__H, zb_buf_len(param)));
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return RET_INVALID_FORMAT;
  }

  ZB_ZCL_PACKET_GET_DATA8(&pl_in.cancel_control, data);
  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_DRLC_CANCEL_ALL_LOAD_CONTROL_EVENTS_CB_ID, RET_OK, cmd_info, &pl_in, NULL);

  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_drlc_client_handle_cancel_all_load_control_events", (FMT__0));

  /* SE spec:
   * The Cancel All Load Control Events command is processed by the device as if individual
   * Cancel Load Control Event commands were received for all of the currently stored events in the device.
   * The device will respond with a "Report Event Status Command" for each individual load control event canceled.*/

  /* NOTE: We do not send replies, this is application's task since it is responsible
   * for calling send_report_event for each registered cancelled event
   */

  zb_buf_free(param);

  return RET_OK;
}

static zb_bool_t zb_zcl_process_drlc_client_commands(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_uint8_t processed = ZB_FALSE;
  zb_ret_t result = RET_ERROR;
  /* ZCL8: CCB 2477: use UNSUP_COMMAND instead of Unsupported cluster command status */
  zb_uint8_t status = ZB_ZCL_STATUS_UNSUP_CMD;

  switch ((zb_zcl_drlc_srv_cmd_t) cmd_info->cmd_id)
  {
    case ZB_ZCL_DRLC_SRV_CMD_LOAD_CONTROL_EVENT:
      result = zb_drlc_client_handle_load_control_event(param, cmd_info);
      status = (RET_OK == result) ? ZB_ZCL_STATUS_SUCCESS: 
               ((RET_INVALID_FORMAT == result) ? ZB_ZCL_STATUS_INVALID_FIELD : ZB_ZCL_STATUS_FAIL);
      processed = ZB_TRUE;
      break;
    case ZB_ZCL_DRLC_SRV_CMD_CANCEL_LOAD_CONTROL_EVENT:
      result = zb_drlc_client_handle_cancel_load_control_event(param, cmd_info);
      status = (RET_OK == result) ? ZB_ZCL_STATUS_SUCCESS: 
               ((RET_INVALID_FORMAT == result) ? ZB_ZCL_STATUS_INVALID_FIELD : ZB_ZCL_STATUS_FAIL);
      processed = ZB_TRUE;
      break;
    case ZB_ZCL_DRLC_SRV_CMD_CANCEL_ALL_LOAD_CONTROL_EVENTS:
      result = zb_drlc_client_handle_cancel_all_load_control_events(param, cmd_info);
      status = (RET_OK == result) ? ZB_ZCL_STATUS_SUCCESS: 
               ((RET_INVALID_FORMAT == result) ? ZB_ZCL_STATUS_INVALID_FIELD : ZB_ZCL_STATUS_FAIL);
      processed = ZB_TRUE;
      break;
    default:
      break;
  }

  if (!processed)
  {
    if (!cmd_info->disable_default_response)
    {
      ZB_ZCL_SEND_DEFAULT_RESP(param,
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
  }
  return ZB_TRUE;
}


/**
 *
 */

zb_bool_t zb_zcl_process_c_drlc_specific_commands(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_drlc_specific_commands", (FMT__0));

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_drlc_client_cmd_list;
    return ZB_TRUE;
  }

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  /* ZB_ASSERT(cmd_info.profile_id == ZB_AF_SE_PROFILE_ID); */
  ZB_ASSERT(cmd_info.cluster_id == ZB_ZCL_CLUSTER_ID_DRLC);

  if(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction)
  {
      return zb_zcl_process_drlc_client_commands(param, &cmd_info);
  }

  return ZB_FALSE;
}

#endif /* ZB_ZCL_SUPPORT_CLUSTER_DRLC || defined DOXYGEN */
