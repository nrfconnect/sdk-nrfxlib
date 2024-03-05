/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Control4 Network Cluster cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2100

#include "zb_common.h"

#if defined ZB_CONTROL4_NETWORK_SUPPORT

#include "zb_zcl.h"
#include "zcl/zb_zcl_control4_networking.h"
#include "zb_zdo.h"
#include "zb_aps.h"

/** Global variable stored the cluster current state */
static zb_zcl_control4_network_state_t g_c4_network_cluster_state = ZB_ZCL_CONTROL4_NETWORK_STATE_IDLE;

/** Get Control4 Network Cluster state */
#define ZB_ZCL_CONTROL4_NETWORK_GET_STATE()         (g_c4_network_cluster_state)

/** Get Control4 Network Cluster state */
#define ZB_ZCL_CONTROL4_NETWORK_SET_STATE(state)    (g_c4_network_cluster_state = state)

/** Check if Control4 Network Cluster is stopped or not */
#define ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED()        (g_c4_network_cluster_state == ZB_ZCL_CONTROL4_NETWORK_STATE_IDLE || \
                                                     g_c4_network_cluster_state == ZB_ZCL_CONTROL4_NETWORK_STATE_STOPPED)

/** Check if Control4 Network Cluster is running or not */
#define ZB_ZCL_CONTROL4_NETWORK_IS_RUNNING()        (g_c4_network_cluster_state == ZB_ZCL_CONTROL4_NETWORK_STATE_RUNNING)

/** Check if Control4 Network Cluster is searching for ZAP or not */
#define ZB_ZCL_CONTROL4_NETWORK_IS_SEARCHING_ZAP()  (g_c4_network_cluster_state == ZB_ZCL_CONTROL4_NETWORK_STATE_SEARCHING_ZAP)

/** Check if Control4 Network is permitted or not */
#ifdef ZB_ED_FUNC
#define ZB_ZCL_CONTROL4_NETWORK_IS_PERMITTED()      (zb_control4_network_permitted())
#else
#define ZB_ZCL_CONTROL4_NETWORK_IS_PERMITTED()      (0)
#endif /* ZB_ED_FUNC */

/* Read ZAP Info timeout (in sec) */
#define ZB_ZCL_CONTROL4_NETWORK_READ_ZAP_INFO_TIMEOUT  30

zb_ret_t check_value_control4_networking_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_ret_t check_value_control4_networking_client(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_control4_networking_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);
void zb_zcl_control4_networking_write_attr_hook_client(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value);

zb_bool_t zb_zcl_process_control4_networking_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_control4_networking_specific_commands_cli(zb_uint8_t param);

static void zb_zcl_control4_network_cluster_identify(zb_bufid_t buffer);
static void zb_zcl_control4_network_cluster_announce(zb_bufid_t buffer);
static void zb_zcl_control4_network_cluster_quiry_zap_info(zb_bufid_t buffer);

static const zb_uint16_t control4_network_report_attr[] =
{
  /* TODO: add optional attributes to reporting */
  ZB_ZCL_ATTR_CONTROL4_NETWORKING_DEVICE_TYPE_ID,
  ZB_ZCL_ATTR_CONTROL4_NETWORKING_PRODUCT_STRING_ID,
  ZB_ZCL_ATTR_CONTROL4_NETWORKING_FIRMWARE_VERSION_ID,
  ZB_ZCL_ATTR_CONTROL4_NETWORKING_REFLASH_VERSION_ID,
  ZB_ZCL_ATTR_CONTROL4_NETWORKING_BOOT_COUNT_ID,
  ZB_ZCL_ATTR_CONTROL4_NETWORKING_MESH_CHANNEL_ID,
};

zb_uint8_t gs_control4_networking_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_control4_networking_client_cmd_list =
{
  0, NULL,
  sizeof(gs_control4_networking_server_received_commands), gs_control4_networking_server_received_commands
};

zb_discover_cmd_list_t gs_control4_networking_server_cmd_list =
{
  sizeof(gs_control4_networking_server_received_commands), gs_control4_networking_server_received_commands,
  0, NULL
};

void zb_zcl_control4_networking_init_server()
{
  zb_zcl_add_control4_cluster_handlers(ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       check_value_control4_networking_server,
                                       zb_zcl_control4_networking_write_attr_hook_server,
                                       zb_zcl_process_control4_networking_specific_commands_srv);
}

void zb_zcl_control4_networking_init_client()
{
  zb_zcl_add_control4_cluster_handlers(ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                       NULL, NULL,
                                       zb_zcl_process_control4_networking_specific_commands_cli);
}

zb_ret_t check_value_control4_networking_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);
  switch(attr_id)
  {
    case ZB_ZCL_ATTR_CONTROL4_NETWORKING_ANNOUNCE_WINDOW_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) < ZB_ZCL_CONTROL4_NETWORKING_ANNOUNCE_WINDOW_MIN_VALUE) ? RET_ERROR : RET_OK;
      break;
    case ZB_ZCL_ATTR_CONTROL4_NETWORKING_MTORR_PERIOD_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) < ZB_ZCL_CONTROL4_NETWORKING_MTORR_PERIOD_MIN_VALUE) ? RET_ERROR : RET_OK;
      break;
    case ZB_ZCL_ATTR_CONTROL4_NETWORKING_MESH_CHANNEL_ID:
      ret = (*value < ZB_ZCL_CONTROL4_NETWORKING_MESH_CHANNEL_MIN_VALUE ||
             *value > ZB_ZCL_CONTROL4_NETWORKING_MESH_CHANNEL_MAX_VALUE) ? RET_ERROR : RET_OK;
      break;
  }

  return ret;
}

/**
  Notify user application of ZAP (Zigbee Access Point) discovery event

  Possible events defined in zb_zcl_control4_zap_discover_event_t:
    ZB_ZCL_CONTROL4_NETWORK_ZAP_UPDATED - ZAP details are updated
    ZB_ZCL_CONTROL4_NETWORK_ZAP_DISCOVER_RETRYING - Retrying during searching ZAP state
    ZB_ZCL_CONTROL4_NETWORK_ZAP_DISCOVER_FAILED - ZAP Discover failed (Retrying attempts expired)

   @param buffer - buffer id
   @param event - ZAP discovery event (zb_zcl_control4_zap_discover_event_t)
 */

static void zb_zcl_control4_networking_zap_info_notify_user_app(zb_bufid_t buffer, zb_uint16_t event)
{
  static zb_bool_t is_scheduled = ZB_FALSE;

  if (!buffer)
  {
    if (!is_scheduled)
    {
      is_scheduled = ZB_TRUE;
      zb_buf_get_out_delayed_ext(zb_zcl_control4_networking_zap_info_notify_user_app, event, 0);
    }
  }
  else
  {
    if (ZCL_CTX().device_cb)
    {
      zb_zcl_control4_zap_info_notify_t notification = {.event = (zb_zcl_control4_zap_discover_event_t )event};

      TRACE_MSG(TRACE_ZCL1, "zb_zcl_control4_networking_zap_info_notify_user_app event=%hd", (FMT__H, event));

      ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(buffer, ZB_ZCL_CONTROL4_NETWORK_ZAP_INFO_CB_ID,
                                        RET_OK, NULL, &notification, NULL);

      (ZCL_CTX().device_cb)(buffer);
    }

    is_scheduled = ZB_FALSE;
    zb_buf_free(buffer);
  }
}

void zb_zcl_control4_networking_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  zb_zcl_attr_t* attr_desc;

  ZVUNUSED(endpoint);
  ZVUNUSED(attr_id);
  ZVUNUSED(new_value);
  ZVUNUSED(manuf_code);

  /* TODO: handle other writable attributes */

  if (attr_id == ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_ID)
  {
    attr_desc = zb_zcl_get_attr_desc_a(ZB_CONTROL4_NETWORK_ENDPOINT,
                                       ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       attr_id);
    if (attr_desc)
    {
      /* Check if ZAP Node ID changed */
      if (ZB_ZCL_ATTR_GET16(new_value) != ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) &&
          ZB_ZCL_ATTR_GET16(new_value) != ZB_ZCL_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_DEF_VALUE)
      {
        /* invoke user app */
        zb_zcl_control4_networking_zap_info_notify_user_app(0, ZB_ZCL_CONTROL4_NETWORK_ZAP_UPDATED);
      }
    }
  }
  else if (attr_id == ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_LONG_ID_ID)
  {
    zb_ieee_addr_t default_ap_long_id = ZB_ZCL_CONTROL4_NETWORKING_ACCESS_POINT_LONG_ID_DEF_VALUE;

    attr_desc = zb_zcl_get_attr_desc_a(ZB_CONTROL4_NETWORK_ENDPOINT,
                                       ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       attr_id);
    if (attr_desc)
    {
      /* Check if ZAP Long ID changed */
      if (!ZB_IEEE_ADDR_CMP(new_value, attr_desc->data_p) &&
          !ZB_IEEE_ADDR_CMP(new_value, default_ap_long_id))
      {
        /* invoke user app */
        zb_zcl_control4_networking_zap_info_notify_user_app(0, ZB_ZCL_CONTROL4_NETWORK_ZAP_UPDATED);
      }
    }
  }
}

static zb_zcl_status_t zb_zcl_control4_networking_map_ret_code_to_zcl_status(zb_ret_t ret_code)
{
  zb_zcl_status_t status;

  ZB_ASSERT(ret_code != RET_BUSY);

  switch (ret_code)
  {
    case RET_OK:
      status = ZB_ZCL_STATUS_SUCCESS;
      break;
    case RET_INVALID_PARAMETER_1:
      status = ZB_ZCL_STATUS_INVALID_FIELD;
      break;
    case RET_INVALID_PARAMETER:
      status = ZB_ZCL_STATUS_INVALID_VALUE;
      break;
    case RET_ERROR:
      status = ZB_ZCL_STATUS_FAIL;
      break;
    default:
      status = ZB_ZCL_STATUS_FAIL;
      break;
  }

  return status;
}

static zb_ret_t zb_zcl_process_control4_networking_immediate_announce_handler(zb_uint8_t param)
{
  zb_uint8_t *ptr;
  zb_uint8_t length, i;
  zb_uint16_t addr;
  zb_ret_t ret = RET_ERROR;

  if (!ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    if (zb_buf_len(param) == 0)
    {
      /* Command received without payload */
      zb_zcl_control4_network_cluster_announce(param);
      param = 0;
    }
    else
    {
      /* Command received with payload */
      ptr = (zb_uint8_t *)zb_buf_begin(param);
      length = *ptr++;

      if (zb_buf_len(param) == (length * sizeof(zb_uint16_t) + sizeof(zb_uint8_t)))
      {
        for (i = 0; i < length; ++i)
        {
          ZB_HTOLE16(&addr, ptr);
          ptr += sizeof(zb_uint16_t);

          if (addr == zb_get_short_address())
          {
            /* Found self address in the command payload */
            zb_zcl_control4_network_cluster_announce(param);
            param = 0;
            break;
          }
        }
      }
    }

    if (param)
    {
      zb_buf_free(param);
    }

    /* Don't send Default response */
    ret = RET_BUSY;
  }

  return ret;
}

zb_bool_t zb_zcl_process_control4_networking_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_process_control4_networking_specific_commands: param %hd", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch( cmd_info.cmd_id )
  {
    case ZB_ZCL_CMD_CONTROL4_NETWORKING_IMMEDIATE_ANNOUNCE_ID:
      ret = zb_zcl_process_control4_networking_immediate_announce_handler(param);
      break;
    default:
      processed = ZB_FALSE;
      break;
  }

  if (processed && ret != RET_BUSY)
  {
    zb_zcl_send_default_handler(param, &cmd_info,
                                zb_zcl_control4_networking_map_ret_code_to_zcl_status(ret));
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_process_control4_networking_specific_commands: processed %hd", (FMT__H, processed));

  return processed;
}

zb_bool_t zb_zcl_process_control4_networking_specific_commands_srv(zb_uint8_t param)
{
  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_control4_networking_server_cmd_list;
    return ZB_TRUE;
  }

  return zb_zcl_process_control4_networking_specific_commands(param);
}

zb_bool_t zb_zcl_process_control4_networking_specific_commands_cli(zb_uint8_t param)
{
  if (ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param)
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_control4_networking_client_cmd_list;
    return ZB_TRUE;
  }

  return ZB_FALSE;
}

static void zb_zcl_control4_network_cluster_send_attr_report(zb_bufid_t buffer, zb_uint16_t short_addr)
{
  zb_uint8_t *cmd_data;
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t bytes_avail;
  zb_uint8_t attr_size;
  zb_uint32_t i;
  /* Keep track of the index of latest attribute reported in case all atributes do not fit in one packet */
  static zb_uint8_t attr_index = 0;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_control4_network_cluster_send_attr_report: addr 0x%x buffer %hd", (FMT__D_H, short_addr, buffer));

  cmd_data = ZB_ZCL_START_PACKET(buffer);
  ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_REQ_FRAME_CONTROL_A(cmd_data, ZB_ZCL_FRAME_DIRECTION_TO_CLI,
                                                       ZB_FALSE, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(cmd_data, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_REPORT_ATTRIB);

  for (i = attr_index; i < ZB_ARRAY_SIZE(control4_network_report_attr); ++i)
  {
    attr_desc = zb_zcl_get_attr_desc_a(ZB_CONTROL4_NETWORK_ENDPOINT,
                                       ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       control4_network_report_attr[i]);
    if (attr_desc)
    {
      bytes_avail = ZB_ZCL_GET_BYTES_AVAILABLE(buffer,
                                               cmd_data,
                                               ZB_AF_CONTROL4_PROFILE_ID,
                                               ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING);
      attr_size = zb_zcl_get_attribute_size(attr_desc->type, attr_desc->data_p);

      /* Decrement read attr response by sizeof(zb_uint8_t), because attr value size is
       * calculated separately by attr_size  */
      if (bytes_avail >= (sizeof(zb_zcl_report_attr_req_t) - sizeof(zb_uint8_t) + attr_size))
      {
        ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_data, attr_desc->id);
        ZB_ZCL_PACKET_PUT_DATA8(cmd_data, attr_desc->type);
        cmd_data = zb_zcl_put_value_to_packet(cmd_data, attr_desc->type, attr_desc->data_p);
        attr_index = 0;
      }
      else
      {
        attr_index = i;
        TRACE_MSG(TRACE_ZCL1, "Attribute reporting will use more than one packet.", (FMT__0));
        break;
      }
    }
  }

  ZB_ZCL_FINISH_N_SEND_PACKET(buffer,
      cmd_data, short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_CONTROL4_NETWORK_ENDPOINT,
      ZB_CONTROL4_NETWORK_ENDPOINT,
      ZB_AF_CONTROL4_PROFILE_ID,
      ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
      NULL);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_control4_network_cluster_send_attr_report", (FMT__0));
}

static void zb_zcl_control4_network_cluster_send_announce(zb_bufid_t buffer)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t short_addr;

  if (!ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    attr_desc = zb_zcl_get_attr_desc_a(ZB_CONTROL4_NETWORK_ENDPOINT,
                                       ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_ID);
    ZB_ASSERT(attr_desc);

    /* Send Announce to ZAP */
    short_addr = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    if (short_addr == ZB_ZCL_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_DEF_VALUE)
    {
      /* Send Identify instead of Announce if we don't know ZAP Node ID */
      zb_zcl_control4_network_cluster_identify(buffer);
    }
    else
    {
      /* Send Announce */
      zb_zcl_control4_network_cluster_send_attr_report(buffer, short_addr);
    }
  }
  else
  {
    /* Control4 Network Cluster is not running */
    zb_buf_free(buffer);
  }
}

static void zb_zcl_control4_network_cluster_schedule_announce(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t announce_window;

  /* Cancel scheduled alarms for Announce */
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_announce, 0);

  if (ZB_ZCL_CONTROL4_NETWORK_IS_RUNNING())
  {
    TRACE_MSG(TRACE_ZCL2, "zb_zcl_control4_network_cluster_schedule_announce", (FMT__0));

    /* Schedule the next Announce using ANNOUNCE_WINDOW attribute value */
    attr_desc = zb_zcl_get_attr_desc_a(ZB_CONTROL4_NETWORK_ENDPOINT,
                                       ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_CONTROL4_NETWORKING_ANNOUNCE_WINDOW_ID);

    /* Calculate timeout value */
    announce_window = (attr_desc ? ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) : ZB_ZCL_CONTROL4_NETWORKING_ANNOUNCE_WINDOW_DEF_VALUE);
    announce_window -= ZB_ZCL_CONTROL4_NETWORKING_MTORR_PERIOD_MIN_VALUE;
    announce_window = ZB_ZCL_CONTROL4_NETWORKING_MTORR_PERIOD_MIN_VALUE + ZB_RANDOM_VALUE(announce_window);

    ZB_SCHEDULE_ALARM(zb_zcl_control4_network_cluster_announce, 0,
                      announce_window * ZB_TIME_ONE_SECOND);
  }
}

static void zb_zcl_control4_network_cluster_announce(zb_bufid_t buffer)
{
  if (!buffer)
  {
    zb_buf_get_out_delayed(zb_zcl_control4_network_cluster_announce);
  }
  else
  {
    zb_zcl_control4_network_cluster_send_announce(buffer);
    zb_zcl_control4_network_cluster_schedule_announce();
  }
}

static void zb_zcl_control4_network_cluster_identify(zb_bufid_t buffer)
{
  if (!ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    zb_zcl_control4_network_cluster_send_attr_report(buffer, ZB_NWK_BROADCAST_ROUTER_COORDINATOR);
  }
  else
  {
    /* Control4 Network Cluster is stopped */
    zb_buf_free(buffer);
  }
}

static void zb_zcl_control4_network_cluster_read_zap_info_status(zb_uint8_t status)
{
  static zb_uint8_t attempts = 0;

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_control4_network_cluster_read_zap_info_status status=%d", (FMT__D, status));

  if (status == ZB_ZCL_STATUS_SUCCESS)
  {
    attempts = 0;
  }
  else if (status == ZB_ZCL_STATUS_FAIL)
  {
    if (ZB_ZCL_CONTROL4_NETWORK_IS_SEARCHING_ZAP())
    {
      if (attempts < ZB_ZCL_CONTROL4_NETWORK_SEARCHING_ZAP_ATTEMPTS)
      {
        /* Try again obtain ZAP info and send Identify again */
        zb_buf_get_out_delayed(zb_zcl_control4_network_cluster_quiry_zap_info);
        zb_buf_get_out_delayed(zb_zcl_control4_network_cluster_identify);
        attempts++;
      }
      else
      {
        /* Can't obtain ZAP information. Stop the cluster logic */
        attempts = 0;
        ZB_ZCL_CONTROL4_NETWORK_SET_STATE(ZB_ZCL_CONTROL4_NETWORK_STATE_STOPPED);
        zb_zcl_control4_networking_zap_info_notify_user_app(0, ZB_ZCL_CONTROL4_NETWORK_ZAP_DISCOVER_FAILED);
      }
    }
  }
}

static void zb_zcl_control4_network_cluster_read_zap_info(zb_bufid_t buffer)
{
  zb_uint8_t *cmd_ptr;
  zb_uint8_t  ap_node_id;
  zb_uint8_t  ap_long_id;
  zb_uint8_t  ap_cost;
  zb_uint16_t short_addr;

  if (!ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    TRACE_MSG(TRACE_ZCL1, "zb_zcl_control4_network_cluster_read_zap_info: buffer %hd ", (FMT__H, buffer));

    /* Fill attributes */
    ap_node_id = ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_ID;
    ap_long_id = ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_LONG_ID_ID;
    ap_cost    = ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_COST_ID;

    /* Send the request to parent */
    short_addr = zb_nwk_get_parent();
    ZB_ASSERT(short_addr != 0xFFFF);

    ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ((buffer), cmd_ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, (ap_node_id));
    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, (ap_long_id));
    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, (ap_cost));
    ZB_ZCL_GENERAL_SEND_READ_ATTR_REQ((buffer), cmd_ptr, short_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                      ZB_CONTROL4_NETWORK_ENDPOINT, ZB_CONTROL4_NETWORK_ENDPOINT,
                                      ZB_AF_CONTROL4_PROFILE_ID, ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                      NULL);

    ZB_SCHEDULE_ALARM(zb_zcl_control4_network_cluster_read_zap_info_status, ZB_ZCL_STATUS_FAIL,
                      ZB_ZCL_CONTROL4_NETWORK_READ_ZAP_INFO_TIMEOUT * ZB_TIME_ONE_SECOND);
  }
  else
  {
    /* Control4 Network Cluster is stopped */
    zb_buf_free(buffer);
  }
}

static void zb_zcl_control4_network_cluster_schedule_quiry_zap_info(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t mtorr_period;

  /* Cancel scheduled alarms for Quiry ZAP Info */
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_quiry_zap_info, 0);

  if (ZB_ZCL_CONTROL4_NETWORK_IS_RUNNING())
  {
    TRACE_MSG(TRACE_ZCL2, "zb_zcl_control4_network_cluster_schedule_quiry_zap_info", (FMT__0));

    /* Schedule the next Quiry ZAP Info using MTORR_PERIOD attribute value */
    attr_desc = zb_zcl_get_attr_desc_a(ZB_CONTROL4_NETWORK_ENDPOINT,
                                       ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_CONTROL4_NETWORKING_MTORR_PERIOD_ID);

    /* Calculate timeout value */
    mtorr_period = (attr_desc ? ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) : ZB_ZCL_CONTROL4_NETWORKING_MTORR_PERIOD_DEF_VALUE);

    ZB_SCHEDULE_ALARM(zb_zcl_control4_network_cluster_quiry_zap_info, 0,
                      mtorr_period * ZB_TIME_ONE_SECOND);
  }
}

static void zb_zcl_control4_network_cluster_quiry_zap_info(zb_bufid_t buffer)
{
  if (!buffer)
  {
    zb_buf_get_out_delayed(zb_zcl_control4_network_cluster_quiry_zap_info);
  }
  else
  {
    zb_zcl_control4_network_cluster_read_zap_info(buffer);
    zb_zcl_control4_network_cluster_schedule_quiry_zap_info();
  }
}

void zb_zcl_control4_network_cluster_read_attr_resp_handler(zb_bufid_t param)
{
  zb_zcl_read_attr_res_t *read_attr_resp = NULL;
  zb_bool_t zap_info_received = ZB_FALSE;

  /* Cancel scheduled alarms for Read ZAP Info Fail */
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_read_zap_info_status, ZB_ALARM_ANY_PARAM);

  if (!ZB_ZCL_CONTROL4_NETWORK_IS_PERMITTED() ||
      ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    /* Control4 Network is not permitted */
    zb_buf_free(param);
    return;
  }

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_control4_network_cluster_read_attr_resp_handler buffer %hd", (FMT__H, param));

  do
  {
    ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, read_attr_resp);

    if (read_attr_resp && read_attr_resp->status == ZB_ZCL_STATUS_SUCCESS)
    {
      if (read_attr_resp->attr_id == ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_ID)
      {
        zap_info_received = ZB_TRUE;
        ZB_ZCL_SET_ATTRIBUTE(ZB_CONTROL4_NETWORK_ENDPOINT,
                             ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_NODE_ID_ID,
                             (zb_uint8_t* )&read_attr_resp->attr_value, ZB_FALSE);
      }
      else if (read_attr_resp->attr_id == ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_LONG_ID_ID)
      {
        zap_info_received = ZB_TRUE;
        ZB_ZCL_SET_ATTRIBUTE(ZB_CONTROL4_NETWORK_ENDPOINT,
                             ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_LONG_ID_ID,
                             (zb_uint8_t* )&read_attr_resp->attr_value, ZB_FALSE);
      }
      else if (read_attr_resp->attr_id == ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_COST_ID)
      {
        ZB_ZCL_SET_ATTRIBUTE(ZB_CONTROL4_NETWORK_ENDPOINT,
                             ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_CONTROL4_NETWORKING_ACCESS_POINT_COST_ID,
                             (zb_uint8_t* )&read_attr_resp->attr_value, ZB_FALSE);
      }
    }
  } while (read_attr_resp != NULL);

  if (zap_info_received)
  {
    zb_zcl_control4_network_cluster_read_zap_info_status(ZB_ZCL_STATUS_SUCCESS);

    if (ZB_ZCL_CONTROL4_NETWORK_IS_SEARCHING_ZAP())
    {
      TRACE_MSG(TRACE_ZCL1, "Received ZAP info. Now is running", (FMT__0));
      /* Run the main cluster logic */
      ZB_ZCL_CONTROL4_NETWORK_SET_STATE(ZB_ZCL_CONTROL4_NETWORK_STATE_RUNNING);
      ZB_SCHEDULE_ALARM(zb_zcl_control4_network_cluster_announce, 0,
                        ZB_ZCL_CONTROL4_NETWORKING_ANNOUNCE_WINDOW_MIN_VALUE * ZB_TIME_ONE_SECOND);
      zb_zcl_control4_network_cluster_schedule_quiry_zap_info();
    }
  }

  zb_buf_free(param);
}

static void zb_zcl_control4_network_cluster_start_delayed(zb_bufid_t buffer)
{
  ZVUNUSED(buffer);

  /* Start identification process */
  zb_buf_get_out_delayed(zb_zcl_control4_network_cluster_quiry_zap_info);
  zb_buf_get_out_delayed(zb_zcl_control4_network_cluster_identify);
}

zb_ret_t zb_zcl_control4_network_cluster_start(void)
{
  if (!ZB_ZCL_CONTROL4_NETWORK_IS_PERMITTED())
  {
    /* Joining Control4 Network is not permitted */
    return RET_UNAUTHORIZED;
  }

  if (!ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    /* Already started */
    return RET_OK;
  }

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_control4_network_cluster_start", (FMT__0));

  /* Start Control4 Network cluster with random jitter delay */
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_start_delayed, 0);
  ZB_SCHEDULE_ALARM(zb_zcl_control4_network_cluster_start_delayed, 0, ZB_RANDOM_VALUE(ZB_TIME_ONE_SECOND));
  ZB_ZCL_CONTROL4_NETWORK_SET_STATE(ZB_ZCL_CONTROL4_NETWORK_STATE_SEARCHING_ZAP);

  return RET_OK;
}

void zb_zcl_control4_network_cluster_stop(void)
{
  if (!ZB_ZCL_CONTROL4_NETWORK_IS_STOPPED())
  {
    TRACE_MSG(TRACE_ZCL1, "zb_zcl_control4_network_cluster_stop", (FMT__0));

    /* Stop all cluster logic */
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_start_delayed, 0);
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_announce, 0);
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_control4_network_cluster_quiry_zap_info, 0);
    ZB_ZCL_CONTROL4_NETWORK_SET_STATE(ZB_ZCL_CONTROL4_NETWORK_STATE_STOPPED);
  }
}

zb_zcl_control4_network_state_t zb_zcl_control4_network_cluster_get_state(void)
{
  return ZB_ZCL_CONTROL4_NETWORK_GET_STATE();
}

#endif /* ZB_SUPPORT_CLUSTER_CONTROL4_NETWORKING */
