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
/* PURPOSE: ZCL OTA Upgrade cluster specific commands handling - Client role
*/

#define ZB_TRACE_FILE_ID 2076

#include "zb_common.h"

/** @internal
    @{
*/

/*
  To exclude OTA code linking when no OTA cluster declared in the app, OTA code divided into 4 files:

  zcl_ota_upgrade_commands.c (this file) - keep client code here. Linked if zb_zcl_ota_upgrade_init_cli is called from ZB_ZCL_CLUSTER_DESC()
  zcl_ota_upgrade_srv_commands.c - server code
  zcl_ota_upgrade_common.c - code common for client and server
  zcl_ota_upgrade_minimal.c - functions called from ZBOSS ZCL kernel unconditionally.

  Also modified calls to zcl_ota_abort_and_set_tc() and zb_zcl_process_ota_upgrade_default_response_commands(().
  First called via small adapter using non-standard command ZB_ZCL_CMD_OTA_UPGRADE_INTERNAL_ABORT_ID.
  Second called from zb_zcl_handle_default_response_commands()->zb_zcl_handle_specific_commands().
  For both the idea is: if no cluster, does not call.
 */

#if defined (ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE) || defined DOXYGEN

#if defined ZB_HA_ENABLE_OTA_UPGRADE_CLIENT || defined DOXYGEN

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_ota_upgrade.h"
#include "zb_zdo.h"
#include "zb_aps.h"
#include "zdo_wwah_stubs.h"

static zb_uint8_t gs_ota_upgrade_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_OTA_UPGRADE_CLIENT_ROLE_RECEIVED_CMD_LIST
};

static zb_uint8_t gs_ota_upgrade_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_OTA_UPGRADE_CLIENT_ROLE_GENERATED_CMD_LIST
};

static zb_discover_cmd_list_t gs_ota_upgrade_client_cmd_list =
{
  sizeof(gs_ota_upgrade_client_received_commands), gs_ota_upgrade_client_received_commands,
  sizeof(gs_ota_upgrade_client_generated_commands), gs_ota_upgrade_client_generated_commands
};

static zb_bool_t zb_zcl_process_ota_upgrade_specific_commands_cli(zb_uint8_t param);
static zb_bool_t zb_zcl_process_ota_cli_upgrade_specific_commands(zb_uint8_t param);

void zb_zcl_ota_upgrade_init_cli(void)
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              zb_zcl_check_value_ota_upgrade,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ota_upgrade_specific_commands_cli);
}


static void schedule_resend_buffer(zb_uint8_t endpoint);

static zb_zcl_ota_upgrade_client_variable_t * get_upgrade_client_variables(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_CLIENT_DATA_ID);
  ZB_ASSERT(attr_desc);
  return (zb_zcl_ota_upgrade_client_variable_t*)attr_desc->data_p;
}

/* public API */
void zb_zcl_ota_set_file_size(zb_uint8_t endpoint, zb_uint32_t size)
{
  get_upgrade_client_variables(endpoint)->download_file_size = size;
}


static void zb_zcl_ota_upgrade_block_req_cb(zb_uint8_t param)
{
  zb_zcl_command_send_status_t *cmd_send_status = ZB_BUF_GET_PARAM(param, zb_zcl_command_send_status_t);
  zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(cmd_send_status->src_endpoint);

  client_data->img_block_req_sent = 0;

  if (client_data->pending_img_block_resp)
  {
    if (cmd_send_status->status == RET_OK)
    {
      /* Block_req is acknowledged, can process pending resp if exists */
      zb_zcl_process_ota_upgrade_specific_commands_cli(client_data->pending_img_block_resp);
    }
    else
    {
      /* Block_req is not acknowledged, drop pending img_block_resp */
      zb_buf_free(client_data->pending_img_block_resp);
    }
  }

  client_data->pending_img_block_resp = 0;

  zb_buf_free(param);
}

/* Do not allow to do OTA upgrade too fast (even if it is configured by ZCL attr) - dups from OTA
 * server should be filtered. */
#define OTA_MIN_BLOCK_REQ_DELAY ZB_TIME_BEACON_INTERVAL_TO_MSEC(  \
    ZB_N_APS_ACK_WAIT_DURATION_FROM_NON_SLEEPY * (ZB_N_APS_MAX_FRAME_RETRIES - 1) / ZB_APS_DUPS_TABLE_SIZE )
#define OTA_BLOCK_REQ_DELAY(_delay) (((_delay > OTA_MIN_BLOCK_REQ_DELAY)) ? (_delay) : OTA_MIN_BLOCK_REQ_DELAY)

static void zb_zcl_ota_upgrade_send_block_request(zb_uint8_t param, zb_uint64_t current_delay)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ieee_addr_t our_long_address;
  zb_uint8_t endpoint;
  zb_uint16_t manufacturer;
  zb_uint16_t image_type;
  zb_uint32_t file_version;
  zb_uint32_t current_offset;
  zb_zcl_ota_upgrade_client_variable_t *client_data;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_send_block_request param %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  manufacturer = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID);
  image_type = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID);
  file_version = zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);
  current_offset = zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
  client_data = get_upgrade_client_variables(endpoint);

  TRACE_MSG(TRACE_ZCL2, "src ep %hd dst ep %hd", (FMT__H_H,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint));

  /* Do not request block if offset is default (image downloading is not started) */
  ZB_ASSERT(current_offset != ZB_ZCL_OTA_UPGRADE_FILE_OFFSET_DEF_VALUE);

  zb_get_long_address(our_long_address);
  ZB_ZCL_OTA_UPGRADE_SEND_IMAGE_BLOCK_REQ(param,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                                          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                                          endpoint,
                                          ZB_AF_HA_PROFILE_ID, ZB_FALSE, zb_zcl_ota_upgrade_block_req_cb,
                                          ZB_ZCL_OTA_UPGRADE_QUERY_IMAGE_BLOCK_IEEE_PRESENT |
                                          ZB_ZCL_OTA_UPGRADE_QUERY_IMAGE_BLOCK_DELAY_PRESENT,
                                          manufacturer,
                                          image_type,
                                          file_version,
                                          current_offset,
                                          client_data->max_data_size,
                                          our_long_address,
  /* We are using attribute value directly as answer to server's request to slow down,
   *  but calculating real delay using OTA_BLOCK_DELAY, because we MAY behave this way according to spec:
   * - ZCL 11.5.3 Rate Limiting
   *   The MinimumBlockPeriod attribute is a minimum delay. The client MAY request data slower than what the
   *   server specifies (i.e. with a longer delay). Sleeping end devices MAY do this normally to conserve battery
   *   power.
   * - ZCL 11.13.6.2.8 MinimumBlockPeriod (optional)
   *   This attribute does not necessarily reflect the actual delay applied by the client between Image Block Re-
   *   quests, only the value set by the server on the client. */
                                          zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID),
                                          OTA_BLOCK_REQ_DELAY(current_delay));
  client_data->img_block_req_sent = 1;

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_send_block_request", (FMT__0));
}

/****************** public function and its helper ****************/

static void zb_zcl_ota_upgrade_request_server_send(zb_uint8_t param, zb_uint16_t endpoint16)
{
  zb_uint8_t endpoint;
  zb_uint16_t manufacturer;
  zb_uint16_t image_type;
  zb_uint32_t file_version;
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t addr;
  zb_uint8_t dst_endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_request_server_send param %hx ep %d", (FMT__H_D,
      param, endpoint16));

  endpoint = (zb_uint8_t)endpoint16;

  /* zb_zcl_ota_upgrade_request_server_send is called by delayed buffer alloc,
   * so check for scheduling more than 1 zb_zcl_ota_upgrade_request_server_send
   * when ZBOSS is in OOM state. */
  if (zb_zcl_ota_upgrade_get_ota_status(endpoint) == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
  {
    manufacturer = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID);
    image_type = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID);
    file_version = zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID );

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                       ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ADDR_ID);
    ZB_ASSERT(attr_desc);
    addr = *(zb_uint16_t *)attr_desc->data_p;

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                       ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ENDPOINT_ID);
    ZB_ASSERT(attr_desc);
    dst_endpoint = *(zb_uint8_t *)attr_desc->data_p;

    ZB_ZCL_OTA_UPGRADE_SEND_QUERY_NEXT_IMAGE_REQ(param,
                                                 addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                                 dst_endpoint, endpoint, ZB_AF_HA_PROFILE_ID, ZB_FALSE, NULL,
                                                 0,
                                                 manufacturer,
                                                 image_type,
                                                 file_version,
                                                 0,
                                                 ZB_FALSE);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_request_server_send", (FMT__0));
}


static void zb_zcl_ota_upgrade_request_server(zb_uint8_t endpoint)
{
  zb_zcl_ota_upgrade_client_variable_t *client_data;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_request_server endpoint %d", (FMT__H, endpoint));

  client_data = get_upgrade_client_variables(endpoint);

  (client_data->timer_counter)--;

  TRACE_MSG(TRACE_ZCL2, "timer_counter %d timer_query %d", (FMT__D_D,
      client_data->timer_counter, client_data->timer_query));

  if(client_data->timer_counter==0)
  {
    if (zb_zcl_ota_upgrade_get_ota_status(endpoint) == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
    {
      zb_buf_get_out_delayed_ext(zb_zcl_ota_upgrade_request_server_send, endpoint, 0);
    }
#ifdef ZB_ZCL_OTA_INCREMENTAL_QUERY_INTERVAL
/* TODO: implement correctly - handling timer_query, now it is
 * re-calculated but not used as timeout */
    if ( (client_data->timer_query*=2) > ZB_ZCL_OTA_UPGRADE_MAX_INTERVAL )
    {
      client_data->timer_query = ZB_ZCL_OTA_UPGRADE_MAX_INTERVAL;
    }
#endif
    client_data->timer_counter = client_data->timer_query;
  }

  /* TODO: fix it - integer overflow for ZB_ZCL_OTA_UPGRADE_QUERY_TIMER_INTERVAL */
  ZB_SCHEDULE_ALARM(zb_zcl_ota_upgrade_request_server, endpoint,
                    ZB_ZCL_OTA_UPGRADE_QUERY_TIMER_INTERVAL);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_request_server", (FMT__0));
}

static void zb_zcl_ota_server_discovery_callback(zb_uint8_t param)
{
  zb_uint8_t *zdp_cmd = zb_buf_begin(param);

  zb_zdo_match_desc_resp_t *resp = (zb_zdo_match_desc_resp_t*)zdp_cmd;
  zb_uint8_t *match_list = (zb_uint8_t*)(resp + 1);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_server_discovery_callback %hd", (FMT__H, resp->status));
  if (resp->status == ZB_ZDP_STATUS_SUCCESS &&
      /* See Use Trust Center for Cluster Command */
      ZB_ZDO_CHECK_CLUSTER_PERMISSION(resp->nwk_addr, ZB_ZCL_CLUSTER_ID_OTA_UPGRADE))
  {
    /* The client shall use the first response received6. 6 CCB 1314. */
    if (!resp->match_len ||
        zb_zcl_ota_upgrade_start_client((*match_list), resp->nwk_addr) != RET_OK)
    {
      /* Notify an application that OTA Upgrade server not found */
      ZB_ZCL_OTA_UPGRADE_SERVER_NOT_FOUND_USER_APP(param);
    }
  }
  else
  {
    /* Notify an application that OTA Upgrade server not found */
    ZB_ZCL_OTA_UPGRADE_SERVER_NOT_FOUND_USER_APP(param);
  }

  zb_buf_free(param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_server_discovery_callback", (FMT__0));
}

/* public API */
void zb_zcl_ota_upgrade_init_client(zb_uint8_t param)
{
  zb_zdo_match_desc_param_t *req;
  zb_uint16_t tc_addr;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_init_client %hd", (FMT__H, param));

  // start_ota_server_discovery
  req = zb_buf_initial_alloc(param, sizeof(zb_zdo_match_desc_param_t) + sizeof(zb_uint16_t));

  tc_addr = zb_aib_get_trust_center_short_address();;
  /* If device is forced to use TC and we know TC addr then send request to TC or
   * send broadcast request otherwise */
  req->nwk_addr = !IS_DISTRIBUTED_SECURITY() &&
                  ZB_ZDO_CHECK_IF_FORCED_TO_USE_TC(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE) &&
                  tc_addr != ZB_UNKNOWN_SHORT_ADDR ?
                        tc_addr : ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE;
  req->addr_of_interest = req->nwk_addr;
  req->profile_id = ZB_AF_HA_PROFILE_ID;
  req->num_in_clusters = 1;
  req->num_out_clusters = 0;
  req->cluster_list[0] = ZB_ZCL_CLUSTER_ID_OTA_UPGRADE;

  /* NK: Do not set initial value: upgrade could be started before Match Desc Request.
     Silently fill variables in this case (zb_zcl_ota_upgrade_request_server() func already checks
     if upgrade is started). */

  if (zb_zdo_match_desc_req(param, zb_zcl_ota_server_discovery_callback) == ZB_ZDO_INVALID_TSN)
  {
    zb_zdo_match_desc_resp_t *resp;

    resp = zb_buf_initial_alloc(param, sizeof(zb_zdo_match_desc_resp_t));
    resp->tsn = ZB_ZDO_INVALID_TSN;
    resp->status = ZB_ZDP_STATUS_INSUFFICIENT_SPACE;
    TRACE_MSG(TRACE_ERROR, "no mem space for zb_zdo_match_desc_resp_t", (FMT__0));
    ZB_SCHEDULE_CALLBACK(zb_zcl_ota_server_discovery_callback, param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_init_client", (FMT__0));
}


/* public API */
zb_ret_t zb_zcl_ota_upgrade_start_client(zb_uint8_t server_ep, zb_uint16_t server_addr)
{
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc;
  zb_ret_t ret = RET_ERROR;

  if (server_addr != ZB_ZCL_OTA_UPGRADE_SERVER_ADDR_DEF_VALUE &&
      server_ep != ZB_ZCL_OTA_UPGRADE_SERVER_ENDPOINT_DEF_VALUE)
  {
    endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE);

    /* update attribute */
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ID);
    ZB_ASSERT(attr_desc);

    zb_address_ieee_by_short(server_addr, (zb_uint8_t*)(attr_desc->data_p));
    TRACE_MSG(TRACE_ZCL2, "Server addr "TRACE_FORMAT_64" ", (FMT__A, TRACE_ARG_64((zb_uint8_t*)attr_desc->data_p)));

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ADDR_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, server_addr);

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
           ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ENDPOINT_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, server_ep);

    /* Set timer_counter to 1 - to do first request after 1 min (minimal interval). */
    {
      zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(endpoint);
      client_data->timer_counter = 1;
    }

    /* start timer for Query Upgrade Server */
    /* Do delayed start - to do not upgrade during commissioning */
    ZB_SCHEDULE_ALARM(zb_zcl_ota_upgrade_request_server, endpoint,
                      ZB_ZCL_OTA_UPGRADE_QUERY_TIMER_INTERVAL);

    /* OTA Upgrade client is started */
    ret = RET_OK;
  }

  return ret;
}


/* public API */
void zb_zcl_ota_upgrade_stop_client(void)
{
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_stop_client", (FMT__0));

  /* Set OTA status to Normal - initial value for OTA */
  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE);
  if (endpoint)
  {
    zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL);
  }

  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_ota_upgrade_request_server, endpoint);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_stop_client", (FMT__0));
}


/* public API */
void zb_zcl_ota_upgrade_set_query_interval(zb_uint8_t endpoint, zb_uint16_t interval)
{

  zb_zcl_ota_upgrade_client_variable_t *client_data;

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_set_query_interval ep %hd interval %d min",
      (FMT__H_D, endpoint, interval));

  client_data = get_upgrade_client_variables(endpoint);

  client_data->timer_query = interval;
  if(client_data->timer_counter > interval)
  {
    client_data->timer_counter = interval;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_set_query_interval", (FMT__0));
}


/* public API */
void zb_zcl_ota_upgrade_file_upgraded(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG( TRACE_ZCL1, "> zb_zcl_ota_upgrade_file_upgraded %hx", (FMT__H, endpoint));

  // set attribute
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
  ZB_ASSERT(attr_desc);
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, 0xffffff);

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);
  ZB_ASSERT(attr_desc);
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ZB_ZCL_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_DEF_VALUE);

  /* Restore attribute of request period from backup:
   * 11.10.10  MinimumBlockPeriod Attribute
   * This attribute SHALL reflect the minimum delay between Image Block Request commands generated by the
   * client in milliseconds. The value of this attribute SHALL be updated when the rate is changed by the server,
   * but SHOULD reflect the client default when an upgrade is not in progress or a server does not support this
   * feature. */
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);
  ZB_ASSERT(attr_desc);
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ZCL_CTX().ota_cli.ota_period_backup);

  get_upgrade_client_variables(endpoint)->download_file_size = 0;

  zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL);

  TRACE_MSG( TRACE_ZCL1, "< zb_zcl_ota_upgrade_file_upgraded", (FMT__0));
}

 /*************************** Client handlers *************************/

/** @brief Image Notify command */
static zb_ret_t image_notify_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  /* Compilers may complain here about maybe-uninitialized without {0} when optimization is enabled */
  zb_zcl_ota_upgrade_image_notify_t payload = {0};
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> image_notify_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_IMAGE_NOTIFY_REQ(&payload, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL2, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_IMAGE_NOTIFY_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    zb_bool_t is_agree_file = ZB_TRUE;
#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
    if (zb_zcl_wwah_check_if_forced_to_use_tc(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE))
    {
      is_agree_file = (cmd_info.addr_data.common_data.source.u.short_addr == 
                       zb_aib_get_trust_center_short_address());
    }
#endif 
    TRACE_MSG(TRACE_ZCL2, "dst_addr 0x%x", (FMT__D, cmd_info.addr_data.common_data.dst_addr));
    if( ZB_NWK_IS_ADDRESS_BROADCAST(cmd_info.addr_data.common_data.dst_addr) )
    {
      // note: no break this switch
      //        each value payload_type mean ALL low tests must be
      switch(payload.payload_type)
      {
      case ZB_ZCL_OTA_UPGRADE_IMAGE_NOTIFY_PAYLOAD_JITTER_CODE_IMAGE_VER:
        is_agree_file = is_agree_file && 
                        ZB_ZCL_OTA_UPGRADE_VERSION_CMP(payload.file_version,
                        zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID));
        /* FALLTHROUGH */
      case ZB_ZCL_OTA_UPGRADE_IMAGE_NOTIFY_PAYLOAD_JITTER_CODE_IMAGE:
        is_agree_file = is_agree_file &&
            (payload.image_type==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID));
        /* FALLTHROUGH */
      case ZB_ZCL_OTA_UPGRADE_IMAGE_NOTIFY_PAYLOAD_JITTER_CODE:
        is_agree_file = is_agree_file &&
        (payload.manufacturer==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID));
      }
      if(is_agree_file)
      {
        zb_uint8_t my_jitter_rnd = ZB_RANDOM_JTR(ZB_ZCL_OTA_UPGRADE_QUERY_JITTER_MAX_VALUE) + 1;
#ifdef ZB_STACK_REGRESSION_TESTING_API
        if (ZB_REGRESSION_TESTS_API().zcl_ota_custom_query_jitter)
        {
          my_jitter_rnd = ZB_REGRESSION_TESTS_API().zcl_ota_custom_query_jitter;
        }
#endif /* ZB_STACK_REGRESSION_TESTING_API */
        TRACE_MSG(TRACE_ZCL2, "my_jitter_rnd %d", (FMT__H, my_jitter_rnd));
        is_agree_file = is_agree_file && (my_jitter_rnd <= payload.query_jitter);
      }
    }
    else
    {
      is_agree_file = is_agree_file &&
                      (cmd_info.addr_data.common_data.dst_addr == zb_get_short_address());
    }

    if (zb_zcl_ota_upgrade_get_ota_status(endpoint) != ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
    {
      /* NK:FIXME: WHY? It is not by spec!
         Zigbee Document 095264r22, Zigbee Over-the-Air Upgrading Cluster, Revision 22, Version 1.1

         6.10.3.3:
         However, payload type value of 0x03 has a slightly different effect.  If  the client device has all the
         information matching those included in the command including the new file version, the device shall
         then ignore the command.  This indicates that the device has already gone through the upgrade process.
         This is to prevent the device from downloading the same image version multiple times.  This is only
         true if the command is sent as broadcast/multicast.

         6.10.3.4:
         On receipt of a unicast Image Notify command, the device shall always send a Query Next Image
         request back to the upgrade server.  This provides a way for the server to force reinstallation of image
         on the device.
         ...
         On receipt of a broadcast or multicast Image Notify command, the device shall keep examining each
         field included in the payload with its own value.  For each field, if the value matches its own, it shall
         proceed to examine the next field.  If values in all three fields (naming manufacturer code, image type
         and new file version) match its own values, then it shall discard the command.  The new file version in
         the payload shall match either the devices current running file version or the downloaded file version
         (on the additional memory space).
         ...
         For payload type value of 0x03, if
         both manufacturer code and image type match the devices own values but the new file version is not a
         match, the device shall proceed.  In this case, the (new) file version may be lower or higher than the
         devices file version to indicate a downgrade or an upgrade of the firmware respectively.

         TODO: Modify this behaviour:
         <--- Drop it only if it is ZB_ZCL_OTA_UPGRADE_IMAGE_NOTIFY_PAYLOAD_JITTER_CODE_IMAGE_VER,
         all checks (including file version) are ok and command is broadcast/multicast. If command
         is unicast, proceed it even if all fields are equal (even if file version is the same) -
         abort current download and begin new.
      */
      TRACE_MSG(TRACE_ZCL2, "upgrade is in progress - skip image notify", (FMT__0));
      is_agree_file = ZB_FALSE;
    }

    TRACE_MSG(TRACE_ZCL2, "is_agree_file %d", (FMT__H, is_agree_file));
    if(is_agree_file)
    {
      zb_uint16_t hw_ver = get_upgrade_client_variables(endpoint)->hw_version;

     /* For the case we didn't got it from the notify command */
      payload.image_type=zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID);
      payload.manufacturer=zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID);
      ZB_ZCL_OTA_UPGRADE_SEND_QUERY_NEXT_IMAGE_REQ(param,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
            ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
            ZB_AF_HA_PROFILE_ID, ZB_FALSE, NULL,
            ZB_ZCL_OTA_UPGRADE_QUERY_NEXT_IMAGE_HW_VERSION,
            payload.manufacturer,
            payload.image_type,
            zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID),
            hw_ver,
            (zb_bool_t)ZB_APS_FC_GET_SECURITY(cmd_info.addr_data.common_data.fc));

      ret = RET_BUSY;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< image_notify_handler ret %hx", (FMT__H, ret));
  return ret;
}

/** @brief Query Next Image Request command */
static zb_ret_t query_next_image_resp_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_query_next_image_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> query_next_image_resp_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_QUERY_NEXT_IMAGE_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_QUERY_NEXT_IMAGE_RES",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    TRACE_MSG(TRACE_ZCL2, "payload.status %d", (FMT__H, payload.status));

    switch(payload.status)
    {
      /* Server returned success if FW must be upgraded basing on file_version
       * hw_version. It is up to application to decide which sub-element must be
       * upgraded (see below)
       */
    case ZB_ZCL_STATUS_SUCCESS:
      {
        zb_zcl_attr_t *attr_desc;
        zb_uint16_t delay = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);
        zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(endpoint);
        zb_uint8_t app_status = ZB_ZCL_STATUS_SUCCESS;

        TRACE_MSG(TRACE_ZCL2, "SUCCESS found file", (FMT__0));

        if (payload.file_version ==
            zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID))
        {
          /* ZCL8, 11.13.5.4 Effect on receipt:
             If the file version contained in the Query Next Image Response is the same as the
             CurrentFileVersion attribute (the current running version of software) or the
             DownloadedFileVersion attribute for the specified Image Type, then the message SHOULD
             be discarded and no further processing SHOULD be done. */
          TRACE_MSG(TRACE_ZCL2, "file version is equal to current running version, deny", (FMT__0));
          app_status = ZB_ZCL_OTA_UPGRADE_STATUS_ERROR;
        }
        else
        {
          /* Call user's callback to indicate upgrade process start. */
          ZB_ZCL_OTA_UPGRADE_START_USER_APP(param, payload.manufacturer, payload.image_type,
                                            payload.file_version, payload.image_size, app_status);
        }

        if (app_status == ZB_ZCL_OTA_UPGRADE_STATUS_OK)
        {
        // set attribute
        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
        ZB_ASSERT(attr_desc);
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, 0);

        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);
        ZB_ASSERT(attr_desc);
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, payload.file_version);

        client_data->download_file_size = payload.image_size;

        /* Backup period attribute */
        ZCL_CTX().ota_cli.ota_period_backup = delay;

        zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADING);

          //send query block: offset 0
          ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
          /* In any case we need OTA file header. Then there can be variants (see
           * multi-fw). Variants are to be implemented in the application. */
          zb_zcl_ota_upgrade_send_block_request(param, delay);

        ret = RET_BUSY;
        /* Fill data for resend_buffer. FIXME: rewrite it */
        ZB_MEMCPY(&ZCL_CTX().ota_cli.cmd_info_2, &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
          ZCL_CTX().ota_cli.payload_2.response.success.manufacturer = payload.manufacturer;
          ZCL_CTX().ota_cli.payload_2.response.success.image_type = payload.image_type;
          ZCL_CTX().ota_cli.payload_2.response.success.file_version = payload.file_version;
          schedule_resend_buffer(endpoint);
        }
      }
      break;
    case ZB_ZCL_STATUS_NO_IMAGE_AVAILABLE:
    default:
      break;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< query_next_image_resp_handler ret %hx", (FMT__H, ret));
  return ret;
}


static void resend_buffer(zb_uint8_t param);


static void cancel_resend_buffer()
{
  ZCL_CTX().ota_cli.resend_retries = 0;
  ZB_SCHEDULE_ALARM_CANCEL(resend_buffer, 0);
}


static void schedule_resend_buffer(zb_uint8_t endpoint)
{
  zb_uint16_t delay = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);

  ZCL_CTX().ota_cli.resend_retries = 0;
  ZB_SCHEDULE_ALARM_CANCEL(resend_buffer, 0);
  /* Extend resend inteval to exclude situation when we request new block and retransmit APS packet
   * with old request. */
  ZB_SCHEDULE_ALARM(resend_buffer, 0, ZB_ZCL_OTA_UPGRADE_RESEND_BUFFER_DELAY + ZB_MILLISECONDS_TO_BEACON_INTERVAL(delay));
}


/* public API */
void zcl_ota_abort(zb_uint8_t endpoint, zb_uint8_t param)
{
  zb_uint8_t status = ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL;
  zb_zcl_attr_t *attr_desc;

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                     ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
  if (attr_desc)
  {
    status = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  }
  // set attribute
  zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL);
  /* cancel resend */
  cancel_resend_buffer();

  // set attribute
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
  ZB_ASSERT(attr_desc);
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ZB_ZCL_OTA_UPGRADE_FILE_OFFSET_DEF_VALUE);

  // reset downloaded file version attribute
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);
  ZB_ASSERT(attr_desc);
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, ZB_ZCL_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_DEF_VALUE);

  /* Restore attribute of request period from backup*/
  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);
  ZB_ASSERT(attr_desc);
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ZCL_CTX().ota_cli.ota_period_backup);

  {
    zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(endpoint);

    client_data->img_block_req_sent = 0;
    if (client_data->pending_img_block_resp)
    {
      zb_buf_free((client_data->pending_img_block_resp));
      client_data->pending_img_block_resp = 0;
    }
  }

  /* call User App */
  if (status != ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
  {
    if ((param > 0) && (param != ZB_UNDEFINED_BUFFER))
    {
      ZB_ZCL_OTA_UPGRADE_ABORT_USER_APP(param);
    }
    else
    {
      zb_bufid_t send_buf = zb_buf_get_out();

      ZB_ASSERT(send_buf);
      ZB_ZCL_OTA_UPGRADE_ABORT_USER_APP(send_buf);
      zb_buf_free(send_buf);
    }
  }
}

/* FIXME: link problem. */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
void schedule_init_client(zb_uint8_t param)
{
  TRACE_MSG(TRACE_APP1, "> schedule_init_client", (FMT__0));
  ZB_SCHEDULE_ALARM(zb_zcl_ota_upgrade_init_client, param, 2*ZB_TIME_ONE_SECOND);
  TRACE_MSG(TRACE_APP1, "< schedule_init_client", (FMT__0));
}

/* This function performs:
 * 1) Cancel any download that is in progress if OTA server is not a TC;
 * 2) Set the value of the UpgradeServerID attribute to the apsTrustCenterAddress.
 *    This new UpgradeServerID value SHALL be persisted across reboots.
 */
#ifndef ZB_COORDINATOR_ONLY
static void zcl_ota_abort_and_set_tc_cli()
{
  zb_ieee_addr_t tc_long_address;
  zb_uint8_t endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE);
  zb_zcl_attr_t *attr_desc;
  if (endpoint)
  {
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ID);
    ZB_ASSERT(attr_desc);
    zb_aib_get_trust_center_address(tc_long_address);
    if (!IS_DISTRIBUTED_SECURITY() &&
        !ZB_IEEE_ADDR_IS_ZERO(tc_long_address) &&
        (ZB_MEMCMP(attr_desc->data_p, tc_long_address, sizeof(zb_ieee_addr_t)) != 0))
    {
      if (zb_zcl_ota_upgrade_get_ota_status(endpoint) != ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
      {
        zcl_ota_abort(endpoint, 0);
      }
      zb_zcl_set_attr_val(endpoint,
                          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                          ZB_ZCL_CLUSTER_CLIENT_ROLE,
                          ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ID,
                          tc_long_address,
                          ZB_FALSE);
      zb_buf_get_out_delayed(schedule_init_client);
    }
  }
}
#endif  /* ZB_COORDINATOR_ONLY */
#endif

/* Helper routine to finish OTA Upgrade */
static void zb_zcl_ota_upgrade_end(zb_uint8_t param,
                                   zb_uint8_t status,
                                   zb_zcl_parsed_hdr_t                  *cmd_info,
                                   zb_zcl_ota_upgrade_image_block_res_t *payload)
{
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;

  TRACE_MSG(TRACE_OTA2, "OTA: send upgrade end request, status %hd", (FMT__H, status));

  if (status != ZB_ZCL_STATUS_SUCCESS)
  {
    zcl_ota_abort(endpoint, param);
  }

  /* send upgrade end request */
  ZB_ZCL_OTA_UPGRADE_SEND_UPGRADE_END_REQ(param,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
                                          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                          ZB_AF_HA_PROFILE_ID, ZB_FALSE, NULL,
                                          status,
                                          payload->response.success.manufacturer,
                                          payload->response.success.image_type,
                                          payload->response.success.file_version);
}


/* Note: resend_buffer is used by r20 ZBOSS apps. Seems now no need to call it from outside, so tet make it static. */
// try resend req
static void resend_buffer(zb_uint8_t param)
{
  zb_ieee_addr_t our_long_address = {0};
  zb_bufid_t send_buf = 0;
  zb_uint32_t current_offset;
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t endpoint;
  zb_uint16_t delay;
  zb_zcl_ota_upgrade_client_variable_t *client_data;

  ZCL_CTX().ota_cli.ota_restart_after_rejoin = 0;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL3, "resend_buffer", (FMT__0));
  send_buf = zb_buf_get_out();
  if (send_buf)
  {
    zb_buf_reuse(send_buf);
    endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).dst_endpoint;
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                       ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
    if (attr_desc
        && ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADING == ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc))
    {
      ZCL_CTX().ota_cli.resend_retries++;
      if (ZCL_CTX().ota_cli.resend_retries < ZCL_OTA_MAX_RESEND_RETRIES)
      {
        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                           ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
        ZB_ASSERT(attr_desc);
        current_offset=ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
        delay = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);
        client_data = get_upgrade_client_variables(endpoint);

        TRACE_MSG(TRACE_ZCL2, "OTA: re-sending UPGRADE_SEND_IMAGE_BLOCK_REQ delay %d",
                  (FMT__D, delay));
        zb_get_long_address(our_long_address);
        ZB_ZCL_OTA_UPGRADE_SEND_IMAGE_BLOCK_REQ(send_buf,
                                                ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).source.u.short_addr,
                                                ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                                ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).src_endpoint,
                                                ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).dst_endpoint,
                                                ZB_AF_HA_PROFILE_ID, ZB_FALSE, zb_zcl_ota_upgrade_block_req_cb,
                                                (ZB_ZCL_OTA_UPGRADE_QUERY_IMAGE_BLOCK_IEEE_PRESENT |
                                                 ZB_ZCL_OTA_UPGRADE_QUERY_IMAGE_BLOCK_DELAY_PRESENT),
                                                ZCL_CTX().ota_cli.payload_2.response.success.manufacturer,
                                                ZCL_CTX().ota_cli.payload_2.response.success.image_type,
                                                ZCL_CTX().ota_cli.payload_2.response.success.file_version,
                                                current_offset,
                                                client_data->max_data_size,
                                                our_long_address,
        /* We are using attribute value directly as answer to server's request to slow down,
        *  but calculating real delay using OTA_BLOCK_DELAY, because we MAY behave this way according to spec:
        * - ZCL 11.5.3 Rate Limiting
        *   The MinimumBlockPeriod attribute is a minimum delay. The client MAY request data slower than what the
        *   server specifies (i.e. with a longer delay). Sleeping end devices MAY do this normally to conserve battery
        *   power.
        * - ZCL 11.13.6.2.8 MinimumBlockPeriod (optional)
        *   This attribute does not necessarily reflect the actual delay applied by the client between Image Block Re-
        *   quests, only the value set by the server on the client. */
                                                delay, OTA_BLOCK_REQ_DELAY(delay));
        client_data->img_block_req_sent = 1;
        ZB_SCHEDULE_ALARM_CANCEL(resend_buffer, 0);
        /* Extend resend inteval to exclude situation when we request new block and retransmit APS packet
         * with old request. */
        ZB_SCHEDULE_ALARM(resend_buffer, 0, ZB_ZCL_OTA_UPGRADE_RESEND_BUFFER_DELAY + ZB_MILLISECONDS_TO_BEACON_INTERVAL(delay));
      }
      else
      {
        TRACE_MSG(TRACE_ZCL1, "OTA: out if IMAGE_BLOCK_REQ retries, abort upgrade",
                  (FMT__0));

        zb_zcl_ota_upgrade_end(send_buf,
                               ZB_ZCL_STATUS_ABORT,
                               &(ZCL_CTX().ota_cli.cmd_info_2),
                               &(ZCL_CTX().ota_cli.payload_2));

        /* zb_buf_free(send_buf); */
      }
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "OTA: resend_buffer while in the wrong state",
                (FMT__0));
      zb_buf_free(send_buf);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "OTA: no memory for resend_buffer. Retry via scheduler.",
              (FMT__0));
    ZB_SCHEDULE_CALLBACK(resend_buffer, 0);
  }
}

/* Helper routine to get next image block */
static void zb_zcl_ota_upgrade_get_next_image_block(zb_uint8_t param,
                                                    zb_zcl_parsed_hdr_t                  *cmd_info,
                                                    zb_zcl_ota_upgrade_image_block_res_t *payload)
{
  zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;

  TRACE_MSG(TRACE_OTA2, "OTA: send imageBlockReq", (FMT__0));

  zb_zcl_ota_upgrade_send_block_request(param, zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID));

  /* Schedule resend buffer if we don't get response */
  ZB_MEMCPY(&ZCL_CTX().ota_cli.cmd_info_2, cmd_info, sizeof(zb_zcl_parsed_hdr_t));
  ZB_MEMCPY(&ZCL_CTX().ota_cli.payload_2, payload, sizeof(zb_zcl_ota_upgrade_image_block_res_t));
  schedule_resend_buffer(endpoint);
}

/* Helper routine to process downloaded image */
static void zb_zcl_ota_upgrade_process_downloaded_image(zb_uint8_t param,
                                                        zb_zcl_parsed_hdr_t                  *cmd_info,
                                                        zb_zcl_ota_upgrade_image_block_res_t *payload)
{
  zb_uint8_t endpoint;
  zb_uint8_t upgrade_status;

  TRACE_MSG(TRACE_OTA2, "OTA: process downloaded image", (FMT__0));

  /* reserve space for user callback parameters, it will be used later while
   * calling this callback. When space is reserved, buffer data is moved to the left -
   * it is better to do it here before any actions with pointers to data */
  (void)ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;

  cancel_resend_buffer();
  zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADED);

  ZB_ZCL_OTA_UPGRADE_CHECK_USER_APP(param, upgrade_status);

  if (upgrade_status != ZB_ZCL_OTA_UPGRADE_STATUS_BUSY)
  {
    /* Finish OTA process */
    zb_zcl_ota_upgrade_end(param,
                           (upgrade_status == ZB_ZCL_OTA_UPGRADE_STATUS_OK ?
                            ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_IMAGE),
                           cmd_info, payload);
  }
  else
  {
    /* Application is busy, restore parameters and wait the signal to resume */
    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), cmd_info, sizeof(zb_zcl_parsed_hdr_t));
  }
}

/* public API. Never called */
/* Resume OTA Upgrade signal from application */
void zb_zcl_ota_upgrade_resume_client(zb_uint8_t param, zb_uint8_t upgrade_status)
{
  zb_uint8_t endpoint;
  zb_uint8_t ota_status;
  zb_zcl_ota_upgrade_image_block_res_t payload = {0};
  zb_zcl_parsed_hdr_t                  cmd_info;
  zb_zcl_parse_status_t                status = ZB_ZCL_PARSE_STATUS_FAILURE;

  TRACE_MSG(TRACE_OTA2, "zb_zcl_ota_upgrade_resume_client param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_OTA1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES", (FMT__0));
    zb_buf_free(param);
    return;
  }

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
  ota_status = zb_zcl_ota_upgrade_get_ota_status(endpoint);

  if (ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADING ||
      ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_WAIT_FOR_MORE)
  {
    if (upgrade_status != ZB_ZCL_OTA_UPGRADE_STATUS_OK)
    {
      zb_zcl_ota_upgrade_end(param, ZB_ZCL_STATUS_INVALID_IMAGE, &cmd_info, &payload);
    }
    else
    {
      zb_uint32_t file_size;
      zb_uint32_t current_offset;
      zb_zcl_ota_upgrade_client_variable_t *client_data;
      zb_zcl_attr_t                        *attr_desc = NULL;

      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                         ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                                         ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                         ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
      ZB_ASSERT(attr_desc);

      client_data = get_upgrade_client_variables(endpoint);
      file_size = client_data->download_file_size;
      current_offset = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);

      if (file_size > current_offset)
      {
        zb_zcl_ota_upgrade_get_next_image_block(param, &cmd_info, &payload);
      }
      else
      {
        zb_zcl_ota_upgrade_process_downloaded_image(param, &cmd_info, &payload);
      }
    }
  }
  else if (ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADED)
  {
    zb_zcl_ota_upgrade_end(param,
                           (upgrade_status == ZB_ZCL_OTA_UPGRADE_STATUS_OK ?
                            ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_IMAGE),
                           &cmd_info, &payload);
  }
  else
  {
    TRACE_MSG(TRACE_OTA2, "Nothing to resume! Free buffer", (FMT__0));
    zb_buf_free(param);
  }
}

/* public API */
void zb_zcl_ota_restart_after_rejoin(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                                    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
  TRACE_MSG(TRACE_OTA1, "zb_zcl_ota_restart_after_rejoin endpoint %hd", (FMT__H, endpoint));
  if ((attr_desc != NULL)
      && ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADING == ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc))
  {
    TRACE_MSG(TRACE_OTA1, "yes, restart OTA after getting sever descriptor", (FMT__0));
    ZCL_CTX().ota_cli.ota_restart_after_rejoin = 1;

    /* [MM]: Moved from zb_zcl_ota_server_discovery_callback */
    TRACE_MSG(TRACE_OTA1, "Restart after rejoin - schedule OTA buffer req resend", (FMT__0));
    /* Fill data for resend_buffer.  */
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).dst_endpoint = endpoint; /*    src ep (yes, src) */
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).source.u.short_addr = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ADDR_ID);
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&ZCL_CTX().ota_cli.cmd_info_2).src_endpoint = zb_zcl_ota_upgrade_get8(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_ENDPOINT_ID); /*    dst ep (yes, dst) */
    ZCL_CTX().ota_cli.payload_2.response.success.manufacturer = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID);
    ZCL_CTX().ota_cli.payload_2.response.success.image_type = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID);;
    ZCL_CTX().ota_cli.payload_2.response.success.file_version = zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);

    schedule_resend_buffer(endpoint);
  }
  else
  {
    if (attr_desc != NULL)
    {
      TRACE_MSG(TRACE_OTA1, "Upgrade status %hd - do not restart OTA", (FMT__H, ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc)));
    }
    else
    {
      TRACE_MSG(TRACE_ERROR, "Invalid Pointer attr_desc", (FMT__0));
    }
  }
}


static void zb_zcl_ota_upgrade_finish_upgrade(zb_uint8_t param)
{
  zb_uint8_t user_ret;
  zb_uint8_t endpoint = *ZB_BUF_GET_PARAM(param, zb_uint8_t);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_finish_upgrade param %hd ep %hd", (FMT__H_H,
      param, endpoint));

  ZB_ZCL_OTA_UPGRADE_FINISH_USER_APP(param, user_ret);
  ZVUNUSED(user_ret);

  zb_buf_free(param);

  zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_finish_upgrade param %hd ep %hd", (FMT__H_H,
      param, endpoint));
}

/** @brief Image Block Response command */
static zb_ret_t image_block_resp_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_image_block_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> image_block_resp_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES",
      (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(endpoint);
    zb_zcl_attr_t *attr_desc;
    zb_uint8_t ota_status = zb_zcl_ota_upgrade_get_ota_status(endpoint);

    /* If we are not downloading the image, do nothing */
    if (ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL)
    {
      ret = RET_ERROR;
    }
    /* If fw is already downloaded, do not allow new Image Block Responses */
    else if (ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADED ||
             ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_WAITING_UPGRADE ||
             ota_status == ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_COUNT_DOWN)
    {
      /* Do not abort here - simply drop the packet (it may be retransmit etc). */
      TRACE_MSG(TRACE_ZCL2, "OTA: ImgBlockResp when already downloaded, drop pkt", (FMT__0));
      /* zcl_ota_abort(endpoint, param); */

      /* ZB_ZCL_OTA_UPGRADE_SEND_UPGRADE_END_REQ(param, */
      /*                                         ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr, */
      /*                                         ZB_APS_ADDR_MODE_16_ENDP_PRESENT, */
      /*                                         ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint, */
      /*                                         ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, */
      /*                                         ZB_AF_HA_PROFILE_ID, ZB_FALSE, NULL, */
      /*                                         ZB_ZCL_STATUS_INVALID_IMAGE, */
      /*                                         zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID), */
      /*                                         zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID), */
      /*                                         zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID)); */
      zb_buf_free(param);
      ret = RET_BUSY;
    }
    else
    /* According to ota_status, download is in progress. */
    {
      switch(payload.status)
      {
        case ZB_ZCL_STATUS_SUCCESS:
          attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                             ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);
          ZB_ASSERT(attr_desc);

          /* clear delay before use
             ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, 0);*/

          if(payload.response.success.manufacturer==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID) &&
             payload.response.success.image_type==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID) &&
             payload.response.success.file_version==zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID) )
          {
            zb_uint32_t file_size = client_data->download_file_size;
            zb_uint32_t current_offset;
            zb_bool_t get_next_block = ZB_FALSE;
            zb_uint_t upgrade_status  = ZB_ZCL_STATUS_SUCCESS;

            attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                               ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
            ZB_ASSERT(attr_desc);

            current_offset=ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);

            if (payload.response.success.file_offset==current_offset)
            {
              /* reserve space for user callback parameters, it will be used later while
               * calling this callback. When space is reserved, buffer data is moved to the left -
               * it is better to do it here before any actions with pointers to data */
              (void)ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);

              /* Make sure that the image payload pointer is correct after making the reservation. */
              ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES(&payload, param, status);


              /* Block with expected offset is received, cancel resending. */
              cancel_resend_buffer();
              // call User App
              // NOTE file data place`s in buffer, payload saves pointer to data only!

              /* Move offset before user's callback in case user's callback move it */
              current_offset += payload.response.success.data_size;
              ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, current_offset);

              ZB_ZCL_OTA_UPGRADE_RECEIVE_USER_APP(param, payload.response.success.file_offset,
                                                  payload.response.success.data_size, payload.response.success.image_data,
                                                  upgrade_status);

              if (upgrade_status == ZB_ZCL_OTA_UPGRADE_STATUS_OK)
              {
                /* User's callback can change current_offset, so re-read it */
                current_offset = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
                get_next_block = ZB_TRUE;
                TRACE_MSG(TRACE_ZCL2, "OTA: recv offset == current offset %ld, send get next req", (FMT__L, current_offset));
              }
              else if (upgrade_status == ZB_ZCL_OTA_UPGRADE_STATUS_BUSY)
              {
                TRACE_MSG(TRACE_ZCL2, "OTA: client is busy, suspend OTA and wait for the signal to resume", (FMT__0));
                /* restore parameters */
                ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
                ret = RET_BUSY;
                break;
              }
              else
              {
                TRACE_MSG(TRACE_ZCL2, "User's OTA callback set upgrade status %d - sending upgrade end", (FMT__D, upgrade_status));

                zb_zcl_ota_upgrade_end(param, ZB_ZCL_STATUS_INVALID_IMAGE, &cmd_info, &payload);
                ret = RET_BUSY;
                /* Prevent sending Upgrade End request twice when we downloaded entire file with some error. */
                break;
              }
            }
            else
            {
              if (ZCL_CTX().ota_cli.ota_restart_after_rejoin)
              {
                /* ZC thinks we are doing OTA. If so, let's start just now */
                TRACE_MSG(TRACE_ZCL2, "This is OTA after reboot - start download immediately", (FMT__0));
                ZCL_CTX().ota_cli.ota_restart_after_rejoin = 0;
                get_next_block = ZB_TRUE;
              }
            }
            TRACE_MSG(TRACE_ZCL2, "file_size %ld current_offset %ld", (FMT__L_L, file_size, current_offset));
            if (file_size > current_offset)
            {
              TRACE_MSG(TRACE_ZCL2, "OTA: get_next_block %hd", (FMT__H, get_next_block));

              if (get_next_block)
              {
                /* restore parameters */
                ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
                zb_zcl_ota_upgrade_get_next_image_block(param, &cmd_info, &payload);

                /* Mark buffer as BUSY - it will not be release */
                ret = RET_BUSY;
              }
              else
              {
                /* mark buffer is NOT BUSY - it will be released after returning from the func */
                TRACE_MSG(TRACE_ZCL2, "OTA: skip sending, free buf %hd", (FMT__H, param));
              }
            }
            else
            {
              zb_zcl_ota_upgrade_process_downloaded_image(param, &cmd_info, &payload);

              /* Mark buffer as BUSY - it will not be release */
              ret = RET_BUSY;
            }
          }
          else
          {

            TRACE_MSG(TRACE_ZCL1, "Error invalid command parameter: manufacturer %d/%d image type %d/%d file version %lx/%lx",
                      (FMT__D_D_D_D_L_L,
                       payload.response.success.manufacturer, zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID),
                       payload.response.success.image_type, zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID),
                       payload.response.success.file_version, zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID)));
            ret = RET_INVALID_PARAMETER_1;
          }
          break;
        case ZB_ZCL_STATUS_WAIT_FOR_DATA:
        {
          /*
            zb_uint16_t manufacturer = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID);
            zb_uint16_t image_type = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID);
            zb_uint32_t file_version = zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);
            zb_uint32_t current_offset = zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
            zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(endpoint);
          */
          zb_uint32_t delay32;
          zb_uint64_t delay64;

          TRACE_MSG(TRACE_ZCL2, "set delay attr %d",
                    (FMT__D, payload.response.wait_for_data.delay));

          /* TODO: implement attribute value check in zb_zcl_check_attr_value (0 - 0x258)
             NOTE: it seems error in the spec, too low upper limit;
             possibly it should be 6000? */

          /* update attribute from server command:
           * 11.10.10  MinimumBlockPeriod Attribute
           * This attribute SHALL reflect the minimum delay between Image Block Request commands generated by the
           * client in milliseconds. The value of this attribute SHALL be updated when the rate is changed by the server,
           * but SHOULD reflect the client default when an upgrade is not in progress or a server does not support this
           * feature. */
          ret = ((ZB_ZCL_STATUS_SUCCESS == zb_zcl_set_attr_val(endpoint,
                                                              ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                                                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                                              ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID,
                                                              (zb_uint8_t*)&payload.response.wait_for_data.delay, ZB_FALSE))
                 ? RET_OK
                 : RET_ERROR);

          if (ret == RET_OK)
          {
            /* OTA spec 6.10.8.4
               if (request_time - current_time) == 0, use BlockRequestDelay value to wait */

            delay32 = ZB_TIME32_SUBTRACT(payload.response.wait_for_data.request_time,
                                         payload.response.wait_for_data.current_time);

            /* request/current time is sent in UTC (seconds), translate it to ms */
            delay64 = 1000ull * (delay32);

            if (delay64 == 0)
            {
              /* if time delta is zero, use BlockRequestDelay value */
              delay64 = payload.response.wait_for_data.delay;
            }

            TRACE_MSG(TRACE_ZCL2, "OTA: delay32 %ds", (FMT__D, (zb_uint16_t)delay32));

            /* re-send query next block */
            /* TODO: ImageBlockResp may also be received as a response
             * to ImagePageReq, in this case ImagePageReq should be resent */
            ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
            zb_zcl_ota_upgrade_send_block_request(param, delay64);

            ret = RET_BUSY;
            /* cancel resend */
            cancel_resend_buffer();
          }
        }
        break;
        case ZB_ZCL_STATUS_ABORT:
          zcl_ota_abort(endpoint, param);
          break;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< image_block_resp_handler ret %hx", (FMT__H, ret));
  return ret;
}


/* public API */
void zb_zcl_ota_upgrade_send_upgrade_end_req(zb_uint8_t param, zb_uint8_t upgrade_status)
{
  zb_zcl_ota_upgrade_image_block_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  ZB_BZERO(&payload, sizeof(payload));

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_send_upgrade_end_req param %hd, status %hd",
            (FMT__H_H, param, upgrade_status));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_RES",
        (FMT__0));
    zb_buf_free(param);
  }
  else
  {
    /* Finish OTA process */
    zb_zcl_ota_upgrade_end(param,
                      (upgrade_status == ZB_ZCL_OTA_UPGRADE_STATUS_OK ?
                          ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_IMAGE),
                           &cmd_info, &payload);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_send_upgrade_end_req", (FMT__0));
}

/** @brief Upgrade End Response command */
static zb_ret_t upgrade_end_resp_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_upgrade_end_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t user_ret;

  TRACE_MSG(TRACE_ZCL1, "> upgrade_end_resp_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_UPGRADE_END_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_UPGRADE_END_RES",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    TRACE_MSG(TRACE_ZCL2, "payload parameter %ld %d %d", (FMT__L_D_D,
        payload.file_version, payload.manufacturer, payload.image_type));
    TRACE_MSG(TRACE_ZCL2, "saved parameter %ld %d %d", (FMT__L_D_D,
        zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID),
        zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID)));
    TRACE_MSG(TRACE_ZCL2, "time parameter %ld %ld", (FMT__L_L,
        payload.current_time, payload.upgrade_time));

       /* OTA spec 6.10.10.4: examine the manufacturer code, image
          type and file version to verify that they match its own. If
          the received values do not match its own values or they are
          not wild card values, then it shall discard the command and
          no further processing shall continue.

          TODO: make correct check - take into account wildcard values
       */
    if(payload.manufacturer==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID) &&
       payload.image_type==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID) &&
       payload.file_version==zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID) &&
       (payload.current_time <= payload.upgrade_time || ZB_ZCL_OTA_UPGRADE_UPGRADE_TIME_DEF_VALUE==payload.upgrade_time))
    {
      if(payload.upgrade_time == ZB_ZCL_OTA_UPGRADE_UPGRADE_TIME_DEF_VALUE)
      {
        zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_WAITING_UPGRADE);
      }
      else
      {
         // call User App
        ZB_ZCL_OTA_UPGRADE_APPLY_USER_APP(param, user_ret);

        switch (user_ret)
        {
        case ZB_ZCL_OTA_UPGRADE_STATUS_REQUIRE_MORE_IMAGE:
          zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_WAIT_FOR_MORE);

          // send
          ZB_ZCL_OTA_UPGRADE_SEND_UPGRADE_END_REQ(param,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
            ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
            ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
            ZB_AF_HA_PROFILE_ID, ZB_FALSE, NULL,
            ZB_ZCL_STATUS_REQUIRE_MORE_IMAGE,
            payload.manufacturer,
            payload.image_type,
            payload.file_version);

          ret = RET_BUSY;
          break;

        case ZB_ZCL_OTA_UPGRADE_STATUS_OK:
          zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_COUNT_DOWN);
          if(ZB_ZCL_OTA_UPGRADE_UPGRADE_TIME_DEF_VALUE==payload.upgrade_time)
          {
            ZB_ZCL_OTA_UPGRADE_FINISH_USER_APP(param, user_ret);
          }
          else
          {
            *ZB_BUF_GET_PARAM(param, zb_uint8_t) = endpoint;

            ZB_SCHEDULE_ALARM(zb_zcl_ota_upgrade_finish_upgrade, param,
               (payload.upgrade_time - payload.current_time)*ZB_TIME_ONE_SECOND);
            ret = RET_BUSY;
          }
          break;

        case ZB_ZCL_OTA_UPGRADE_STATUS_ERROR:
        default:
          zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL);
          break;
        }
      }
    }
    else
    {
      ret = RET_INVALID_PARAMETER_2;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< upgrade_end_resp_handler ret %hx", (FMT__H, ret));
  return ret;
}

/** @brief Query Specific File Response command */
static zb_ret_t query_specific_file_resp_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_query_specific_file_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> query_specific_file_resp_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_QUERY_SPECIFIC_FILE_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_QUERY_SPECIFIC_FILE_RES",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    switch(payload.status)
    {
    case ZB_ZCL_STATUS_SUCCESS:
      if(payload.manufacturer==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MANUFACTURE_ID) &&
         payload.image_type==zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_TYPE_ID) &&
         ZB_ZCL_OTA_UPGRADE_VERSION_CMP(payload.file_version,
              zb_zcl_ota_upgrade_get32(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_VERSION_ID)) )
      {
        zb_zcl_attr_t *attr_desc;
        zb_uint16_t delay = zb_zcl_ota_upgrade_get16(endpoint, ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID);
        zb_zcl_ota_upgrade_client_variable_t *client_data = get_upgrade_client_variables(endpoint);

        // set attribute
        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_FILE_OFFSET_ID);
        ZB_ASSERT(attr_desc);
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, 0);

        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_DOWNLOADED_FILE_VERSION_ID);
        ZB_ASSERT(attr_desc);
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, payload.file_version);

        client_data->download_file_size = payload.image_size;

        /* Backup period attribute */
        ZCL_CTX().ota_cli.ota_period_backup = delay;

        zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_DOWNLOADING);

        //send query block: offset 0
        ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
        zb_zcl_ota_upgrade_send_block_request(param, delay);

        ret = RET_BUSY;
      }
      break;
    case ZB_ZCL_STATUS_NO_IMAGE_AVAILABLE:
      break;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< query_specific_file_resp_handler ret %hx", (FMT__H, ret));
  return ret;
}


static zb_bool_t zb_zcl_process_ota_cli_upgrade_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_ota_upgrade_specific_commands: param %d, cmd %d",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE == cmd_info.cluster_id);

  switch( cmd_info.cmd_id )
  {
    case ZB_ZCL_CMD_OTA_UPGRADE_IMAGE_NOTIFY_ID:
      ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);
      status = image_notify_handler(param);
      break;

    case ZB_ZCL_CMD_OTA_UPGRADE_QUERY_NEXT_IMAGE_RESP_ID:
      ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);
      status = query_next_image_resp_handler(param);
      break;

    case ZB_ZCL_CMD_OTA_UPGRADE_IMAGE_BLOCK_RESP_ID:
    {
      zb_zcl_ota_upgrade_client_variable_t *client_data =
        get_upgrade_client_variables(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint);

      ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);
      if (client_data->img_block_req_sent)
      {
        /* We have pending request, do not handle new block immediately */
        if (client_data->pending_img_block_resp)
        {
          /* Yes, we will skip this block. Another block is already pending. */
          zb_buf_free(param);
        }
        else
        {
          client_data->pending_img_block_resp = param;
        }
        return ZB_TRUE;           /* Notify ZCL that command is processed */
      }
      else
      {
        status = image_block_resp_handler(param);
      }
    }
    break;

    case ZB_ZCL_CMD_OTA_UPGRADE_UPGRADE_END_RESP_ID:
      ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);
      status = upgrade_end_resp_handler(param);
      break;

    case ZB_ZCL_CMD_OTA_UPGRADE_QUERY_SPECIFIC_FILE_RESP_ID:
      ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction);
      status = query_specific_file_resp_handler(param);
      break;

#if defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && !defined ZB_COORDINATOR_ONLY
    case ZB_ZCL_CMD_OTA_UPGRADE_INTERNAL_ABORT_ID:
      zcl_ota_abort_and_set_tc_cli();
      break;
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && !defined ZB_COORDINATOR_ONLY */

    case ZB_ZCL_CMD_DEFAULT_RESP:
      /* This is not a normall call to specific commands processing.
         This is a replacement of direct call from zb_zcl_handle_default_response_commands().
         It made to exclude OTA code linking if OTA cluster is not declared by the app.
       */
      return (zb_bool_t)zb_zcl_process_ota_upgrade_default_response_commands(param);
      break;

    default:
      processed = ZB_FALSE;
      break;
  }

  if( processed )
  {
    if( (ZB_NWK_IS_ADDRESS_BROADCAST(cmd_info.addr_data.common_data.dst_addr) ||
         (cmd_info.disable_default_response)) && status == RET_OK)
    {
      TRACE_MSG( TRACE_ZCL3, "Default response disabled", (FMT__0));
      zb_buf_free(param);
    }
    /* Fixed according to CCB 2519, malformed command should be ignored, default response should
     * not be sent, see ZCL8 spec subclause 11.13.3.5.1 */
    else if ((cmd_info.cmd_id == ZB_ZCL_CMD_OTA_UPGRADE_IMAGE_NOTIFY_ID) && (ZB_NWK_IS_ADDRESS_BROADCAST(cmd_info.addr_data.common_data.dst_addr)) && (status == RET_INVALID_PARAMETER_1 || status == RET_INVALID_PARAMETER_2))
    {
      TRACE_MSG(TRACE_ZCL3, "Got malformed packet, no further processing should be done", (FMT__0));
      zb_buf_free(param);
    }
    else if (status != RET_BUSY)
    {
      ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION( param,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                                          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                                          cmd_info.profile_id,
                                          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                                          cmd_info.seq_number,
                                          cmd_info.cmd_id,
                                          (status==RET_OK ? ZB_ZCL_STATUS_SUCCESS :
                                           (status==RET_INVALID_PARAMETER_2 ? ZB_ZCL_STATUS_MALFORMED_CMD :
                                            ZB_ZCL_STATUS_INVALID_FIELD)),
                                          (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction ?
                                           ZB_ZCL_FRAME_DIRECTION_TO_SRV :
                                           ZB_ZCL_FRAME_DIRECTION_TO_CLI));
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_ota_upgrade_specific_commands: processed %d",
             (FMT__D, processed));
  return processed;
}

static zb_bool_t zb_zcl_process_ota_upgrade_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ota_upgrade_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_ota_cli_upgrade_specific_commands(param);
}

/**
 *  @} internal
*/

#endif /* defined ZB_HA_ENABLE_OTA_UPGRADE_CLIENT || defined DOXYGEN */

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE || defined DOXYGEN */
