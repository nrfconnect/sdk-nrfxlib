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
/* PURPOSE: ZCL WWAH cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 12084

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_WWAH)

#include "zb_zcl.h"
#include "zcl/zb_zcl_wwah.h"
#include "zdo_wwah_survey_beacons.h"
#include "zdo_wwah_parent_classification.h"
#include "zb_bdb_internal.h"
#include "zdo_hubs.h"

/* #include "zb_mac_globals.h" */
/* #include "mac_internal.h" */
#include "zb_aps.h"
#include "zcl/zb_zcl_keep_alive.h"

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER

zb_zcl_wwah_attr_t wwah_attr;

void zb_zcl_wwah_periodic_checkin_countdown(zb_uint8_t param);
void zb_zcl_wwah_recounter_checkin(void);
void zb_zcl_wwah_periodic_checkin_timeout(zb_uint8_t param);

static const zb_uint8_t gs_wwah_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_WWAH_SERVER_ROLE_RECEIVED_CMD_LIST
};

static const zb_uint8_t gs_wwah_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_WWAH_SERVER_ROLE_GENERATED_CMD_LIST
};

zb_discover_cmd_list_t gs_wwah_server_cmd_list =
{
  sizeof(gs_wwah_server_received_commands), (zb_uint8_t *)gs_wwah_server_received_commands,
  sizeof(gs_wwah_server_generated_commands), (zb_uint8_t *)gs_wwah_server_generated_commands
};

zb_ret_t check_value_wwah(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

zb_bool_t zb_zcl_process_wwah_specific_commands_srv(zb_uint8_t param);

zb_uint8_t wwah_endpoint = 0;

void get_pib_max_frame_retries_confirm(zb_uint8_t param)
{
  zb_mlme_get_confirm_t *conf = (zb_mlme_get_confirm_t *)zb_buf_begin(param);
  zb_uint8_t *ptr = ((zb_uint8_t *)conf) + sizeof(zb_mlme_get_confirm_t);
  zb_uint8_t zero = 0;
  TRACE_MSG(TRACE_ZCL1, "get_pib_max_frame_confirm status %hd value %hd", (FMT__H_H, conf->status, *ptr));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_MAC_RETRY_COUNT_ID,
                       conf->status == MAC_SUCCESS ? ptr: &zero,
                       ZB_FALSE);
  /* If we fail, trace is given and assertion is triggered */
  (void)zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  zb_buf_free(param);
}

void get_pib_max_frame_retries(zb_uint8_t param)
{
  zb_mlme_get_request_t *req = NULL;

  req = zb_buf_initial_alloc(param, sizeof(zb_mlme_get_request_t));
  req->pib_attr = ZB_PIB_ATTRIBUTE_MAX_FRAME_RETRIES;
  req->confirm_cb_u.cb = get_pib_max_frame_retries_confirm;
  req->iface_id = ZB_PIBCACHE_PRIMARY_IFACE();

  zb_multimac_mlme_get_request_proxy(param);
}

static zb_bool_t wwah_handle_read_attr_resp(zb_bufid_t param)
{
  zb_bool_t processed = ZB_FALSE;
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  if (zb_zcl_wwah_periodic_checkin_block_zcl_cmd(cmd_info))
  {
    processed = zb_zcl_wwah_periodic_checkin_read_attr_handle(param);
  }

  return processed;
}

void zb_zcl_wwah_init_server(void)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_init_server", (FMT__0));
  if (zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_WWAH,
                                  ZB_ZCL_CLUSTER_SERVER_ROLE,
                                  check_value_wwah,
                                  (zb_zcl_cluster_write_attr_hook_t)NULL,
                                  zb_zcl_process_wwah_specific_commands_srv) == RET_OK
  /* If we are here - we show wwah behavior */
#if 0
       && WWAH_CTX().wwah_behavior
#endif
    )
  {
    wwah_endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);
    WWAH_CTX().periodic_checkins.poll_method = ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED;
    WWAH_CTX().periodic_checkins.tsn = ZB_ZDO_INVALID_TSN;

    WWAH_CTX().wwah_hub_endpoint = 0; /* Will set it after matching */
    WWAH_CTX().wwah_hub_address = 0x0000; /*zb_address_short_by_ieee(ZB_AIB().trust_center_address) */
    WWAH_CTX().time_server_addr = 0;
    WWAH_CTX().time_server_endpoint = 0;

    ZB_ASSERT(ZB_ZCL_WWAH_APS_ACK_EXEMPT_TABLE_SIZE > 0 &&
              ZB_ZCL_WWAH_APS_ACK_EXEMPT_TABLE_SIZE <= 0xFF);

    ZB_MEMSET(WWAH_CTX().aps_ack_exempt_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD,
              sizeof(WWAH_CTX().aps_ack_exempt_table));
    WWAH_CTX().aps_ack_exempt_table_cnt = 0;

    ZB_ASSERT(ZB_ZCL_WWAH_APS_LINK_KEY_AUTHORIZATION_TABLE_SIZE > 0 &&
              ZB_ZCL_WWAH_APS_LINK_KEY_AUTHORIZATION_TABLE_SIZE <= 0xFF);

    ZB_MEMSET(WWAH_CTX().aps_link_key_authorization_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD,
              sizeof(WWAH_CTX().aps_link_key_authorization_table));
    WWAH_CTX().aps_link_key_authorization_table_cnt = 0;

    WWAH_CTX().aps_link_key_enabled_by_default = ZB_FALSE;

    ZB_ASSERT(ZB_ZCL_WWAH_USE_TRUST_CENTER_FOR_CLUSTER_TABLE_SIZE == 4);

    ZB_MEMSET(WWAH_CTX().use_trust_center_for_cluster_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD, sizeof(WWAH_CTX().use_trust_center_for_cluster_table));
    WWAH_CTX().use_trust_center_for_cluster_table_cnt = 0;

    ZCL_SELECTOR().block_zcl_cmd = zb_zcl_wwah_periodic_checkin_block_zcl_cmd;
    ZCL_SELECTOR().read_attr_resp_handler = wwah_handle_read_attr_resp;

    /* TODO: Place f&b checking for endpoint*/
  }
}

void zb_zcl_wwah_init_server_attr(void)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_init_server_attr", (FMT__0));

  wwah_attr.cluster_revision = ZB_ZCL_WWAH_CLUSTER_REVISION_MIN_VALUE;
  wwah_attr.disable_ota_downgrades = ZB_FALSE;
  wwah_attr.mgmt_leave_without_rejoin_enabled = ZB_ZCL_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED_DEFAULT_VALUE;
  wwah_attr.nwk_retry_count = ZB_NWKC_UNICAST_RETRIES;
  zb_buf_get_out_delayed(get_pib_max_frame_retries); /* fetch and update */
  wwah_attr.router_check_in_enabled = ZB_ZCL_WWAH_ROUTER_CHECK_IN_ENABLED_DEFAULT_VALUE;
  wwah_attr.touchlink_interpan_enabled = ZB_TRUE;
  wwah_attr.wwah_parent_classification_enabled = ZB_ZCL_WWAH_WWAH_PARENT_CLASSIFICATION_ENABLED_DEFAULT_VALUE;
  wwah_attr.wwah_app_event_retry_enabled = ZB_ZCL_WWAH_WWAH_APP_EVENT_RETRY_ENABLED_DEFAULT_VALUE;
  wwah_attr.wwah_app_event_retry_queue_size = ZB_ZCL_WWAH_WWAH_APP_EVENT_RETRY_QUEUE_SIZE_DEFAULT_VALUE;
  wwah_attr.wwah_rejoin_enabled = ZB_ZCL_WWAH_WWAH_REJOIN_ENABLED_DEFAULT_VALUE;
  wwah_attr.mac_poll_failure_wait_time = ZB_ZCL_WWAH_MAC_POLL_FAILURE_WAIT_TIME_DEFAULT_VALUE;
  wwah_attr.configuration_mode_enabled = ZB_ZCL_WWAH_CONFIGURATION_MODE_ENABLED_DEFAULT_VALUE;
  wwah_attr.current_debug_report_id = 0;
  wwah_attr.tc_security_on_nwk_key_rotation_enabled = ZB_ZCL_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED_DEFAULT_VALUE;
  wwah_attr.wwah_bad_parent_recovery_enabled = ZB_ZCL_WWAH_WWAH_BAD_PARENT_RECOVERY_ENABLED_DEFAULT_VALUE;
  wwah_attr.pending_network_update_channel = ZB_ZCL_WWAH_PENDING_NETWORK_UPDATE_CHANNEL_DEFAULT_VALUE;
  wwah_attr.pending_network_update_panid = ZB_ZCL_WWAH_PENDING_NETWORK_UPDATE_PANID_DEFAULT_VALUE;
  wwah_attr.ota_max_offline_duration = 0;

  zb_zcl_wwah_stop_periodic_checkin();
  zb_zcl_wwah_stop_bad_parent_recovery();

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_init_server_attr", (FMT__0));
}

zb_ret_t check_value_wwah(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for attributes are allowed */

  return RET_OK;
}
/*
 * Invoke User App with WWAH command.
 */
void zb_zcl_wwah_invoke_user_app(zb_uint8_t param, zb_uint16_t endpoint16)
{

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_invoke_user_app %hx endpoint %d",
      (FMT__H_D, param, endpoint16));
  zb_buf_free(param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_invoke_user_app", (FMT__0));
}

/*
 * Check new channel in ZDO MGMT Network Update command
 * @return ZB_TRUE if new channel is allowed
 *         ZB_FALSE otherwise
 */

zb_bool_t zb_zcl_wwah_check_new_channel(zb_uint32_t new_channel_mask)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_TRUE;
  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_CHANNEL_ID);
    ZB_ASSERT(attr_desc);
    if ((zb_bool_t)(ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) != 0xFF))
    {
      ret = (zb_bool_t)(1u << ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == new_channel_mask);
    }
  }
  return ret;
}

/*
 * Check new PANID in NLME Network Update req command
 * @return ZB_TRUE if new PANID is allowed
 */

zb_bool_t zb_zcl_wwah_check_new_panid(zb_uint16_t new_panid)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_TRUE;
  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_PANID_ID);
    ZB_ASSERT(attr_desc);
    if ((zb_bool_t)(ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) != 0xFFFF))
    {
      ret = (zb_bool_t)(ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) == new_panid);
    };
  }
  return ret;
}

/*
 * Check if Zigbee non-Green Power Inter-pan messages supported
 * @return ZB_TRUE interpan message allowed
 *         ZB_FALSE otherwise
 */

zb_bool_t zb_zcl_wwah_check_if_interpan_supported(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_TRUE;

  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_TOUCHLINK_INTERPAN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ret = (zb_bool_t)ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  }
  return ret;
}


/*
 * Check if OTA Downgrade disabled
 * @return ZB_TRUE Disabled
 *         ZB_FALSE Enabled
 */

zb_bool_t zb_zcl_wwah_check_if_downgrade_disabled(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_FALSE;
  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_DISABLE_OTA_DOWNGRADES_ID);
    ZB_ASSERT(attr_desc);
    ret = (zb_bool_t)ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  }
  return ret;
}

/*
 * Check if specific WWAH Router Check-Ins mechanism is enabled.
 * @return ZB_TRUE mechanism enabled
 *         ZB_FALSE otherwise
 */

zb_bool_t zb_zcl_wwah_check_if_periodic_router_check_ins_enabled(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_TRUE;

  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ret = (zb_bool_t)ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  }
  return ret;
}

zb_bool_t zb_zcl_wwah_check_src_addr_for_cluster(zb_uint16_t short_addr, zb_uint16_t cluster_id)
{
  zb_bool_t ret = ZB_TRUE;

  if (zb_is_wwah_server())
  {
    /* 17-01067-015 WWAH ZCL Cluster definition, Server Dependencies
     * The device SHALL reject all packets regarding this cluster if they are not sourced from the
     * hub/coordinator and not encrypted by the TC Link Key with error code NOT_AUTHORIZED
     * (0x7e). To check if the command is from the coordinator, the device could check that the
     * source network address is 0x0000.
     */
    if (cluster_id == ZB_ZCL_CLUSTER_ID_WWAH && short_addr != 0)
    {
      ret = ZB_FALSE;
    }
  }
  return ret;
}

/*
 * Get additional retries count for mac poll
 * @return ZB_ZCL_WWAH_MAC_POLL_RETRY_COUNT if WWAH behavior is enabled
 * @return 0 otherwise
 */

zb_uint8_t zb_zcl_wwah_mac_poll_cca_retry_count(void)
{
  zb_uint8_t ret = 0;
  if (zb_is_wwah_server())
  {
    ret = ZB_ZCL_WWAH_MAC_POLL_RETRY_COUNT;
  }
  return ret;
}

/*
 * Get MACPollFailureWaitTime in beacon intervals
 * @return MACPollFailureWaitTime in beacon intervals
 */

zb_time_t zb_zcl_wwah_mac_poll_failure_wait_time(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_time_t ret = 0;
  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_MAC_POLL_FAILURE_WAIT_TIME_ID);
    ZB_ASSERT(attr_desc);
    ret = ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_SECONDS_TO_MILLISECONDS(ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc)));
  }
  return ret;
}

/*
 * Check if APS ACK requirement needed for given cluster.
 */
zb_bool_t zb_zcl_wwah_check_if_aps_ack_needed(zb_uint16_t cluster_id)
{
  zb_uindex_t i;
  zb_bool_t ret = ZB_TRUE;
  TRACE_MSG(TRACE_ZCL1, "check_if_aps_ack_needed (cnt %hd)", (FMT__H, WWAH_CTX().aps_ack_exempt_table_cnt));
  if (zb_is_wwah_server())
  {
    for (i = 0; i < WWAH_CTX().aps_ack_exempt_table_cnt; ++i)
    {
      if (cluster_id == WWAH_CTX().aps_ack_exempt_table[i])
      {
        ret = ZB_FALSE;
        break;
      }
    }
  }
  return ret;
}

/*
 * Check if requiring APS level security needed for given cluster.
 */
zb_bool_t zb_zcl_wwah_check_if_aps_link_key_authorization_needed(zb_uint16_t cluster_id)
{
  zb_uindex_t i;
  zb_bool_t ret = WWAH_CTX().aps_link_key_enabled_by_default;

  //return ZB_FALSE; /* Uncomment to temporary disable APS Security in WWAH.*/
  TRACE_MSG(TRACE_ZCL1, ">> check_if_aps_link_key_authorization_needed cluster_id 0x%x", (FMT__D, cluster_id));

  if (cluster_id == ZB_ZCL_CLUSTER_ID_WWAH)
  {
    ret = ZB_TRUE;
  }
  else if (zb_is_wwah_server())
  {
    for (i = 0; i < WWAH_CTX().aps_link_key_authorization_table_cnt; ++i)
    {
      if (cluster_id == WWAH_CTX().aps_link_key_authorization_table[i])
      {
        /* Inverted policy: see WWAH_CTX().aps_link_key_enabled_by_default description */
        ret = (zb_bool_t)!ret;
        break;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< check_if_aps_link_key_authorization_needed ret %hd", (FMT__H, ret));
  return ret;
}

/*
 * Check if given cluster forced to use the Trust Center.
 * @retval ZB_TRUE - cluster is forced to use only Trust Center
 * @retval ZB_FALSE - otherwise
 */
zb_bool_t zb_zcl_wwah_check_if_forced_to_use_tc(zb_uint16_t cluster_id)
{
  zb_uindex_t i;
  zb_bool_t ret = ZB_FALSE;
  if (zb_is_wwah_server())
  {
    for (i = 0; i < WWAH_CTX().use_trust_center_for_cluster_table_cnt; ++i)
    {
      if (cluster_id == WWAH_CTX().use_trust_center_for_cluster_table[i])
      {
        ret = ZB_TRUE;
        break;
      }
    }
  }
  return ret;
}

/*
 * This function is used to check cluster permissions based on "Use TC for Cluster" command
 * @param nwk_addr address of remote device
 * @param cluster_id given cluster
 * @retval ZB_TRUE frame is OK when: Given cluster is forced to use TC and nwk_addr belongs to TC
 *                 or WWAH behaviour is switched off
 *                 or Distributed security is switched on
 * @retval ZB_FALSE otherwise
 */
zb_bool_t zb_zcl_wwah_check_cluster_permission(zb_uint16_t nwk_addr, zb_uint16_t cluster_id)
{
  zb_bool_t ret = ZB_TRUE;
  if (!IS_DISTRIBUTED_SECURITY() && zb_zcl_wwah_check_if_forced_to_use_tc(cluster_id))
  {
    ret = (zb_bool_t)(nwk_addr == zb_aib_get_trust_center_short_address());
  }
  return ret;
}


zb_bool_t zb_zcl_wwah_check_if_wwah_rejoin_enabled(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_FALSE;

  if (zb_is_wwah_server())
  {
    attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_REJOIN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ret = (zb_bool_t)ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  }
  TRACE_MSG(TRACE_NWK1, "zb_zcl_wwah_check_if_wwah_rejoin_enabled: %hd", (FMT__H, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_enable_aps_link_key_authorization_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_wwah_cluster_list_t payload = { 0 };
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_aps_link_key_authorization_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_WWAH_GET_CLUSTER_LIST_CMD(param, payload, status);
  if (status == ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    if (payload.number_of_clusters > ZB_ZCL_WWAH_APS_LINK_KEY_AUTHORIZATION_TABLE_SIZE)
    {
      status = ZB_ZCL_PARSE_STATUS_FAILURE;
    }
  }
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_ENABLE_APS_LINK_KEY_AUTHORIZATION_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    /* This command enforces that all cluster commands for the cluster have APS level security. */
    ZB_MEMSET(WWAH_CTX().aps_link_key_authorization_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD,
              sizeof(WWAH_CTX().aps_link_key_authorization_table));
    WWAH_CTX().aps_link_key_authorization_table_cnt = 0;

    for(i = 0; i < payload.number_of_clusters; ++i)
    {
      if (payload.cluster_id[i] != ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD &&
      /* The device SHALL ignore any cluster IDs in the list for clusters not supported by the device. */
        is_cluster_present(payload.cluster_id[i]))
      {
        TRACE_MSG(TRACE_ZCL1, "add cluster %hd to aps_link_key_authorization_table (idx %hd)", (FMT__D_H, payload.cluster_id[i], WWAH_CTX().aps_link_key_authorization_table_cnt));
        WWAH_CTX().aps_link_key_authorization_table[WWAH_CTX().aps_link_key_authorization_table_cnt++] = payload.cluster_id[i];
      }
    }
    WWAH_CTX().aps_link_key_enabled_by_default = ZB_TRUE;
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_aps_link_key_authorization_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_aps_link_key_authorization_handler(zb_uint8_t param){
  zb_ret_t ret = RET_OK;
  zb_zcl_wwah_cluster_list_t payload = { 0 };
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_aps_link_key_authorization_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_WWAH_GET_CLUSTER_LIST_CMD(param, payload, status);
  if (status == ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    if (payload.number_of_clusters > ZB_ZCL_WWAH_APS_LINK_KEY_AUTHORIZATION_TABLE_SIZE)
    {
      status = ZB_ZCL_PARSE_STATUS_FAILURE;
    }
  }
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_DISABLE_APS_LINK_KEY_AUTHORIZATION_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    /* This command removes the enforcement of APS level security on cluster commands. */
    ZB_MEMSET(WWAH_CTX().aps_link_key_authorization_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD,
              sizeof(WWAH_CTX().aps_link_key_authorization_table));
    WWAH_CTX().aps_link_key_authorization_table_cnt = 0;
    for(i = 0; i < payload.number_of_clusters; ++i)
    {
      if (payload.cluster_id[i] != ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD &&
      /* The device SHALL ignore any cluster IDs in the list for clusters not supported by the device. */
        is_cluster_present(payload.cluster_id[i]))
      {
        WWAH_CTX().aps_link_key_authorization_table[WWAH_CTX().aps_link_key_authorization_table_cnt++] = payload.cluster_id[i];
      }
    }
    WWAH_CTX().aps_link_key_enabled_by_default = ZB_FALSE;
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_aps_link_key_authorization_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_aps_link_key_authorization_query_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_parse_status_t status;
  zb_ret_t ret;
  zb_uint16_t cluster_id;
  zb_bool_t allowed;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_aps_link_key_authorization_query_handler %hd", (FMT__H, param));
  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ZCL_WWAH_GET_APS_LINK_KEY_AUTHORIZATION_QUERY(param, cluster_id, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_APS_LINK_KEY_AUTHORIZATION_QUERY", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    /* The device SHALL respond with value of FALSE for any cluster IDs in the list for clusters
     * not supported by the device.*/
    allowed = is_cluster_present(cluster_id) ?
      zb_zcl_wwah_check_if_aps_link_key_authorization_needed(cluster_id) : ZB_FALSE;
    ZB_ZCL_WWAH_SEND_APS_LINK_KEY_AUTHORIZATION_QUERY_RESPONSE(param,
      cmd_info.seq_number,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      cmd_info.profile_id,
      NULL,
      &cluster_id,
      allowed);
    ret = RET_BUSY;
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_aps_link_key_authorization_query_handler", (FMT__0));
  return ret;
}

#ifdef ZB_JOIN_CLIENT
void zb_zcl_wwah_request_key(zb_uint8_t param)
{
  zb_apsme_request_key_req_t *req = ZB_BUF_GET_PARAM(param, zb_apsme_request_key_req_t);
  TRACE_MSG(TRACE_ZCL1, "Scheduling apsme request key", (FMT__0));
  zb_aib_get_trust_center_address(req->dest_address);
  /* 17-01067-015 WWAH ZCL cluster definition, p.14:
     On receipt of this command, a device SHALL request a new Trust Center Link Key from the
     Trust Center. */
  req->key_type = ZB_TC_LINK_KEY; //ZB_APP_LINK_KEY;
  ZB_SCHEDULE_CALLBACK(zb_secur_apsme_request_key, param);
}

zb_ret_t zb_zcl_wwah_request_new_aps_link_key_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_request_new_aps_link_key_handler %hd", (FMT__H, param));

  zb_buf_get_out_delayed(zb_zcl_wwah_request_key);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_request_new_aps_link_key_handler", (FMT__0));
  return ret;
}
zb_ret_t zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t enable_bool = ZB_TRUE;
  zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_handler %hx", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ZCL_WWAH_GET_ENABLE_WWAH_APP_EVENT_RETRY_ALGORITHM(param, payload, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_ENABLE_WWAH_APP_EVENT_RETRY_ALGORITHM_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
    if (!payload.first_backoff_time_in_seconds ||
        !payload.backoff_sequence_common_ratio ||
        payload.max_backoff_time_in_seconds <= payload.first_backoff_time_in_seconds ||
        payload.max_backoff_time_in_seconds > 86400 ||
        !payload.max_re_delivery_attempts)
    {
      TRACE_MSG(TRACE_ZCL1, "payload of ZB_ZCL_WWAH_ENABLE_WWAH_APP_EVENT_RETRY_ALGORITHM_HANDLER is out of range", (FMT__0));
      ret = RET_OUT_OF_RANGE;
    }
    else
    {
      ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                           ZB_ZCL_CLUSTER_ID_WWAH,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           ZB_ZCL_ATTR_WWAH_WWAH_APP_EVENT_RETRY_ENABLED_ID,
                           (zb_uint8_t *)&enable_bool,
                           ZB_FALSE);
    }
  if (ret == RET_OK)
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_WWAH_ENABLE_APP_EVENT_RETRY_ALGORITHM_CB_ID, RET_NOT_FOUND, &cmd_info, &payload, NULL);
    if (ZCL_CTX().device_cb)
    {
      ZCL_CTX().device_cb(param);
      ret = ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param);
    }
    else
    {
      ret = RET_NOT_FOUND;
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_wwah_app_event_retry_algorithm_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  zb_bool_t disable_bool = ZB_FALSE;
  zb_zcl_parsed_hdr_t cmd_info;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_wwah_app_event_retry_algorithm_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_WWAH_APP_EVENT_RETRY_ENABLED_ID,
                       (zb_uint8_t *)&disable_bool,
                       ZB_FALSE);
  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_WWAH_DISABLE_APP_EVENT_RETRY_ALGORITHM_CB_ID, RET_NOT_FOUND, &cmd_info, NULL, NULL);
  if (ZCL_CTX().device_cb)
  {
    ZCL_CTX().device_cb(param);
    ret = ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param);
  }
  else
  {
    ret = RET_NOT_FOUND;
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_wwah_app_event_retry_algorithm_handler", (FMT__0));
  return ret;
}

void zb_zcl_wwah_send_request_time(zb_uint8_t param)
{
  zb_uint8_t *cmd_ptr;
  zb_ret_t ret = WWAH_CTX().time_server_endpoint == 0 ? RET_ERROR : RET_OK;
  zb_uint8_t time_client_endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_TIME, ZB_ZCL_CLUSTER_CLIENT_ROLE);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_send_request_time param %hd", (FMT__H, param));
  if (ret == RET_OK)
  {
    if (!param)
    {
      ret = zb_buf_get_out_delayed(zb_zcl_wwah_send_request_time);
      if (ret != RET_OK)
      {
        TRACE_MSG(TRACE_ERROR, "Failed zb_buf_get_out_delayed [%d]", (FMT__D, ret));
      }
    }
    else
    {
      ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ(param, cmd_ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
      ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_TIME_TIME_STATUS_ID);
      ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_TIME_TIME_ID);
      ZB_ZCL_GENERAL_SEND_READ_ATTR_REQ(param,
                                    cmd_ptr,
                                    WWAH_CTX().time_server_addr,
                                    ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                    WWAH_CTX().time_server_endpoint,
                                    time_client_endpoint,
                                    ZB_AF_HA_PROFILE_ID,
                                    ZB_ZCL_CLUSTER_ID_TIME,
                                    NULL);
    }
  }
  else if (param)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_send_request_time", (FMT__0));
}

static void zb_zcl_wwah_match_desc_time_cb(zb_uint8_t param)
{
  zb_bufid_t buf = param;
  zb_uint8_t *zdp_cmd = zb_buf_begin(buf);
  zb_zdo_match_desc_resp_t *resp = (zb_zdo_match_desc_resp_t*)zdp_cmd;

  if (resp->status == ZB_ZDP_STATUS_SUCCESS && resp->match_len == 1)
  {
    WWAH_CTX().time_server_addr = resp->nwk_addr;
    WWAH_CTX().time_server_endpoint = *((zb_uint8_t*)(resp + 1));
    ZB_SCHEDULE_CALLBACK(zb_zcl_wwah_send_request_time, param);
  }
  else
  {
    zb_buf_free(buf);
  }
}

static void zb_zcl_wwah_match_desc_time(zb_uint8_t param)
{
  if (!param)
  {
    zb_buf_get_out_delayed(zb_zcl_wwah_match_desc_time);
  }
  else
  {
    zb_bufid_t buf = param;
    zb_zdo_match_desc_param_t *req;

    TRACE_MSG(TRACE_APP1, "send_wwah_match_desc", (FMT__0));

    req = zb_buf_initial_alloc(buf, sizeof(zb_zdo_match_desc_param_t) + 1 * sizeof(zb_uint16_t));

    req->nwk_addr = zb_aib_get_trust_center_short_address();
    req->addr_of_interest = req->nwk_addr;
    req->profile_id = ZB_AF_HA_PROFILE_ID;
    req->num_in_clusters = 1;
    req->num_out_clusters = 0;
    req->cluster_list[0] = ZB_ZCL_CLUSTER_ID_TIME;

    zb_zdo_match_desc_req(param, zb_zcl_wwah_match_desc_time_cb);

  }
}

zb_ret_t zb_zcl_wwah_update_time(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_update_time", (FMT__0));

  if (WWAH_CTX().time_server_endpoint)
  {
    ret = zb_schedule_alarm(zb_zcl_wwah_send_request_time, param, ZB_MILLISECONDS_TO_BEACON_INTERVAL(200));
  }
  else
  {
    /* If we do not have Time Server Endpoint, need to discover it first */
    zb_buf_get_out_delayed(zb_zcl_wwah_match_desc_time);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_update_time", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_request_time_handler(zb_uint8_t param)
{
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_request_time_handler %hd", (FMT__H, param));

  ret = zb_zcl_wwah_update_time(0);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_request_time_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_enable_wwah_rejoin_algorithm_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_wwah_enable_wwah_rejoin_algorithm_t payload;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_wwah_rejoin_algorithm_handler %hd", (FMT__H, param));

  ZB_ZCL_WWAH_GET_ENABLE_WWAH_REJOIN_ALGORITHM(param, payload, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_ENABLE_WWAH_REJOIN_ALGORITHM", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    zb_bool_t enable_bool = ZB_TRUE;

    ZB_MEMCPY(&WWAH_CTX().rejoin_alg, &payload, sizeof(zb_zcl_wwah_enable_wwah_rejoin_algorithm_t));
    ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                         ZB_ZCL_CLUSTER_ID_WWAH,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_WWAH_WWAH_REJOIN_ENABLED_ID,
                         (zb_uint8_t *)&enable_bool,
                         ZB_FALSE);
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
    if (RET_OK != ret)
    {
      TRACE_MSG(TRACE_ERROR, "failed to write into nvram", (FMT__0));
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_wwah_rejoin_algorithm_handler, ret %d", (FMT__D, ret));
  return ret;
}


zb_ret_t zb_zcl_wwah_disable_wwah_rejoin_algorithm_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t enable_bool = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_wwah_rejoin_algorithm_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_WWAH_REJOIN_ENABLED_ID,
                       (zb_uint8_t *)&enable_bool,
                       ZB_FALSE);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  if (RET_OK != ret)
  {
    TRACE_MSG(TRACE_ERROR, "failed to write into nvram", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_wwah_rejoin_algorithm_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_get_rejoin_tmo(zb_uint16_t attempt, zb_time_t *tmo)
{
  zb_ret_t ret = RET_OK;
  zb_uint16_t fast_rejoin_attempts = 1;
  zb_uint16_t is_fast_rejoin_attempt = 0;
  zb_uint16_t backoff_attempt;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_get_rejoin_tmo: attempt %d", (FMT__D, attempt));

  if (wwah_attr.wwah_rejoin_enabled)
  {
    ZB_ASSERT(WWAH_CTX().rejoin_alg.duration_between_each_rejoin_in_seconds);
    fast_rejoin_attempts = (WWAH_CTX().rejoin_alg.fast_rejoin_timeout_in_seconds / WWAH_CTX().rejoin_alg.duration_between_each_rejoin_in_seconds) + 1;
  }

  backoff_attempt = ((attempt - 1) / fast_rejoin_attempts);
  TRACE_MSG(TRACE_ZCL1, "backoff_attempt %d", (FMT__D, backoff_attempt));
  TRACE_MSG(TRACE_ZCL1, "fast_rejoin_attempts %d", (FMT__D, fast_rejoin_attempts));

  if (!wwah_attr.wwah_rejoin_enabled)
  {
    /* Wait for next Rejoin cycle */
    ret = RET_EXIT;
    *tmo = ZB_ZCL_WWAH_REJOIN_SLEEPY_ED_TIMEOUT;
  }
  else
  {
    is_fast_rejoin_attempt = (attempt % fast_rejoin_attempts);
    TRACE_MSG(TRACE_ZCL1, "is_fast_rejoin_attempt %d", (FMT__D, is_fast_rejoin_attempt));
    if (is_fast_rejoin_attempt)
    {
      *tmo = WWAH_CTX().rejoin_alg.duration_between_each_rejoin_in_seconds * ZB_TIME_ONE_SECOND;
    }
    else
    {
      /* A value of 0x00 means do not perform any Fast Rejoins after the Fast Rejoin Time In Seconds
       * (eg. no backoff/retry).
       * FIXME: Not very clear what does it mean. Let's wait for next Rejoin cycle in that case.
       */
      if (!WWAH_CTX().rejoin_alg.fast_rejoin_first_backoff_in_seconds)
      {
        /* Wait for next Rejoin cycle */
        ret = RET_EXIT;
        *tmo = ZB_ZCL_WWAH_REJOIN_SLEEPY_ED_TIMEOUT;
      }
      else
      {
        /* A value of 0x00 means do not reset the backoff duration */
        if (WWAH_CTX().rejoin_alg.max_backoff_iterations
            && backoff_attempt >= WWAH_CTX().rejoin_alg.max_backoff_iterations)
        {
          backoff_attempt %= WWAH_CTX().rejoin_alg.max_backoff_iterations;
        }

        *tmo = (WWAH_CTX().rejoin_alg.fast_rejoin_first_backoff_in_seconds * (1l << backoff_attempt));
        if (*tmo > WWAH_CTX().rejoin_alg.rejoin_max_backoff_time_in_seconds)
        {
          *tmo = WWAH_CTX().rejoin_alg.rejoin_max_backoff_time_in_seconds;
        }
        *tmo *= ZB_TIME_ONE_SECOND;
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_get_rejoin_tmo: ret %hd tmo %ld sec", (FMT__H_L, ret, *tmo / ZB_TIME_ONE_SECOND));
  return ret;
}

#endif  /* ZB_JOIN_CLIENT */

zb_ret_t zb_zcl_wwah_set_ias_zone_enrollment_method_handler(zb_uint8_t param)
{
  zb_uint8_t enrollment_method;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_set_ias_zone_enrollment_method_handler %hd", (FMT__H, param));
  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ZCL_WWAH_GET_SET_IAS_ZONE_ENROLLMENT_METHOD(param, enrollment_method, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    if (ZCL_CTX().device_cb)
    {
      ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD_CB_ID, RET_NOT_FOUND, &cmd_info, &enrollment_method, NULL);
      ZCL_CTX().device_cb(param);
      ret = ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param);
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_set_ias_zone_enrollment_method_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_clear_binding_table_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_clear_binding_table_handler %hd", (FMT__H, param));

  apsme_forget_device();

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_clear_binding_table_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_enable_periodic_router_check_ins_handler(zb_uint8_t param){
  zb_ret_t ret = RET_OK;
  zb_uint16_t payload;
  zb_zcl_parse_status_t status;
  zb_bool_t enable_bool = ZB_TRUE;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_periodic_router_check_ins_handler %hd", (FMT__H, param));
  ZB_ZCL_WWAH_GET_ENABLE_PERIODIC_ROUTER_CHECK_INS(param, payload, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS || payload == 0)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    /* On receipt of this command, a router or non-sleepy end device SHALL enable the Periodic
     * Router Check-in WWAH functionality  */
    if (!zb_get_rx_on_when_idle())
    {
      TRACE_MSG(TRACE_ZCL1, "RX-OFF: ignore command", (FMT__0));
      ret = RET_IGNORE;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "RX-ON: set attribute, start periodic checkins", (FMT__0));
      attr_desc = zb_zcl_get_attr_desc_a(wwah_endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID);
      ZB_ASSERT(attr_desc);
      /* Check it was not started before */
      if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == ZB_FALSE)
      {
        ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                             ZB_ZCL_CLUSTER_ID_WWAH,
                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                             ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID,
                             (zb_uint8_t *)&enable_bool,
                             ZB_FALSE);
        /* If we fail, trace is given and assertion is triggered */
        ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
        WWAH_CTX().periodic_checkins.keepalive_base = payload;
        WWAH_CTX().periodic_checkins.keepalive_jitter = 0;
        zb_zcl_wwah_start_periodic_checkin();
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_periodic_router_check_ins_handler ret %hd", (FMT__H, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_periodic_router_check_ins_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  zb_bool_t disable_bool = ZB_FALSE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_periodic_router_check_ins_handler %hd", (FMT__H, param));
  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID,
                       (zb_uint8_t *)&disable_bool,
                       ZB_FALSE);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  zb_zcl_wwah_stop_periodic_checkin();
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_periodic_router_check_ins_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_set_mac_poll_cca_wait_time_handler(zb_uint8_t param){
  zb_ret_t ret = RET_OK;
  zb_uint8_t payload;
  zb_zcl_parse_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_set_mac_poll_cca_wait_time_handler %hd", (FMT__H, param));
  ZB_ZCL_WWAH_GET_SET_MAC_POLL_CCA_WAIT_TIME(param, payload, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_SET_MAC_POLL_CCA_WAIT_TIME_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                         ZB_ZCL_CLUSTER_ID_WWAH,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_WWAH_MAC_POLL_FAILURE_WAIT_TIME_ID,
                         &payload,
                         ZB_FALSE);
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_set_mac_poll_cca_wait_time_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_set_pending_network_update_handler(zb_uint8_t param){
  zb_ret_t ret = RET_OK;
  zb_zcl_wwah_set_pending_network_update_t payload;
  zb_zcl_parse_status_t status;
  zb_uint8_t channel;
  zb_uint16_t pan_id;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_set_pending_network_update_handler %hd", (FMT__H, param));

  ZB_ZCL_WWAH_GET_SET_PENDING_NETWORK_UPDATE(param, payload, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_SET_PENDING_NETWORK_UPDATE_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    channel = payload.channel;
    pan_id = payload.pan_id;
    ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                         ZB_ZCL_CLUSTER_ID_WWAH,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_CHANNEL_ID,
                         &channel,
                         ZB_FALSE);
    ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                         ZB_ZCL_CLUSTER_ID_WWAH,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_PANID_ID,
                         (zb_uint8_t *)&pan_id,
                         ZB_FALSE);
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_set_pending_network_update_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_require_aps_acks_on_unicasts_handler(zb_uint8_t param){
  zb_ret_t ret = RET_OK;
  zb_zcl_wwah_cluster_list_t payload = { 0 };
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_require_aps_acks_on_unicasts_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_WWAH_GET_CLUSTER_LIST_CMD(param, payload, status);
  if (status == ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    if (payload.number_of_clusters > ZB_ZCL_WWAH_APS_ACK_EXEMPT_TABLE_SIZE)
    {
      status = ZB_ZCL_PARSE_STATUS_FAILURE;
    }
  }
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_REQUIRE_APS_ACKS_ON_UNICASTS_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    /* This command enforces that all unicast commands have APS ACKs enabled. */
    ZB_MEMSET(WWAH_CTX().aps_ack_exempt_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD,
              sizeof(WWAH_CTX().aps_ack_exempt_table));
    WWAH_CTX().aps_ack_exempt_table_cnt = 0;
    for(i = 0; i < payload.number_of_clusters; ++i)
    {
      if (payload.cluster_id[i] != ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD &&
      /* The device SHALL ignore any cluster IDs in the list for clusters not supported by the device.*/
        is_cluster_present(payload.cluster_id[i]))
      {
        WWAH_CTX().aps_ack_exempt_table[WWAH_CTX().aps_ack_exempt_table_cnt++] = payload.cluster_id[i];
      }
    }
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  }
  return ret;
}

zb_ret_t zb_zcl_wwah_remove_aps_acks_on_unicasts_requirement_handler(zb_uint8_t param)
{
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_remove_aps_acks_on_unicasts_requirement_handler %hd", (FMT__H, param));
  ZB_MEMSET(WWAH_CTX().aps_ack_exempt_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD,
            sizeof(WWAH_CTX().aps_ack_exempt_table));
  WWAH_CTX().aps_ack_exempt_table_cnt = 0;
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_remove_aps_acks_on_unicasts_requirement_handler", (FMT__0));
  return RET_OK;
}

zb_ret_t zb_zcl_wwah_aps_ack_requirement_query_handler(zb_uint8_t param)
{
  zb_uint8_t *resp_data;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_aps_ack_requirement_query_handler %hd", (FMT__H, param));
  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  ZB_ZCL_WWAH_SEND_APS_ACK_REQUIREMENT_QUERY_RESPONSE_START(param, cmd_info.seq_number, WWAH_CTX().aps_ack_exempt_table_cnt, resp_data);
  for(i = 0; i < WWAH_CTX().aps_ack_exempt_table_cnt; ++i)
  {
    ZB_ZCL_WWAH_SEND_APS_ACK_REQUIREMENT_QUERY_RESPONSE_ADD(resp_data,  &WWAH_CTX().aps_ack_exempt_table[i]);
  }
  ZB_ZCL_WWAH_SEND_APS_ACK_REQUIREMENT_QUERY_RESPONSE_END(
    resp_data,
    param,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
    cmd_info.profile_id);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_aps_ack_requirement_query_handler", (FMT__0));
  return RET_BUSY;
}

zb_ret_t zb_zcl_wwah_debug_report_query_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_uint8_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_wwah_debug_report_t *out;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_debug_report_query_handler %hd", (FMT__H, param));
  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ZCL_WWAH_GET_DEBUG_REPORT_QUERY(param, payload, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_GET_DEBUG_REPORT_QUERY", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param, ZB_ZCL_WWAH_DEBUG_REPORT_QUERY_CB_ID, RET_NOT_FOUND, &cmd_info, &payload, NULL);
    if (ZCL_CTX().device_cb)
    {
      ZCL_CTX().device_cb(param);
      ret = ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param);
      if (ret == RET_OK)
      {
        out = ZB_ZCL_DEVICE_CMD_PARAM_OUT_GET(param, zb_zcl_wwah_debug_report_t);
        ZB_ASSERT(out);
        ZB_ZCL_WWAH_SEND_DEBUG_REPORT_QUERY_RESPONSE(param,
        cmd_info.seq_number,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
        cmd_info.profile_id,
        NULL,
        out->report_id,
        out->report,
        out->report_size);
        ret = RET_BUSY;
      }
    }
    else
    {
      ret = RET_NOT_FOUND;
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_debug_report_query_handler", (FMT__0));
  return ret;
}


zb_ret_t zb_zcl_wwah_survey_beacons_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t need_standard_beacons;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_wwah_survey_beacons_handler", (FMT__0));

#if !(defined ZB_BEACON_SURVEY && defined ZB_ZCL_ENABLE_WWAH_SERVER)
  TRACE_MSG(TRACE_ZCL1, "beacon survey is disabled", (FMT__0));
  ret = RET_ERROR;
#else
  if (WWAH_CTX().survey_beacons_in_progress)
  {
    ret = RET_BUSY;
    zb_buf_free(param);
  }

  if (ZB_IS_DEVICE_ZR())
  {
    ret = RET_IGNORE;
  }

  if (RET_OK == ret)
  {
    ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

    ZB_ZCL_WWAH_GET_SURVEY_BEACONS(param, need_standard_beacons, status);

    if (ZB_ZCL_PARSE_STATUS_FAILURE == status)
    {
      TRACE_MSG(TRACE_ZCL1, "beacons survey parsing is failed", (FMT__0));
      ret = RET_ERROR;
    }
  }

  if (RET_OK == ret)
  {
    zb_zdo_beacon_survey_configuration_t *conf =
      ZB_BUF_GET_PARAM(param, zb_zdo_beacon_survey_configuration_t);

    conf->params.channel_page = zb_get_current_page();
    conf->params.channel_mask = (1l << zb_get_current_channel());
    conf->params.scan_type = (zb_uint8_t)(((zb_uint8_t)need_standard_beacons) ? ACTIVE_SCAN : ENHANCED_ACTIVE_SCAN);

    ret = zdo_wwah_start_survey_beacons(param);

    if (RET_OK == ret)
    {
      WWAH_CTX().survey_beacons_seq_number = cmd_info.seq_number;
      WWAH_CTX().survey_beacons_in_progress = 1;
      ret = RET_BUSY;
      TRACE_MSG(TRACE_ZDO1, "survey beacons started, seq_number %hd", (FMT__H, cmd_info.seq_number));
    }
  }
#endif  /* ZB_BEACON_SURVEY */

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_wwah_survey_beacons_handler", (FMT__0));

  return ret;
}

#if defined ZB_BEACON_SURVEY && defined ZB_ZCL_ENABLE_WWAH_SERVER
void zb_zcl_wwah_send_survey_beacons_response(zb_bufid_t obuf, zb_uint16_t buf)
{
  if (obuf == 0)
  {
    zb_buf_get_out_delayed_ext(zb_zcl_wwah_send_survey_beacons_response, buf, 0);
  }
  else
  {
    zb_uint8_t *ptr;
    zb_uint8_t src_endpoint =
      get_endpoint_by_cluster(
        ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);
    zb_zdo_beacon_survey_resp_params_t *resp_params =
      zb_buf_get_tail(
        buf,
        (sizeof(zb_zdo_beacon_survey_resp_params_t)
         + ZDO_WWAH_MAX_BEACON_SURVEY_BYTES()));

    ZB_ZCL_WWAH_SEND_SURVEY_BEACONS_RESPONSE_START(
      obuf, WWAH_CTX().survey_beacons_seq_number);

    ptr =
      (zb_uint8_t*)zb_buf_begin(obuf)
      + ZB_ZCL_GET_HEADER_SIZE(*(zb_uint8_t*)zb_buf_begin(obuf));

    TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_send_survey_beacons_response, buf %hd, obuf %hd, beacons_number %hd",
      (FMT__H_H_H, buf, obuf, resp_params->parents.count_potential_parents));

    ZB_ZCL_WWAH_SEND_SURVEY_BEACONS_RESPONSE_ADD_ALL(
      ptr,
      resp_params->parents.parents_info_ptr,
      resp_params->parents.count_potential_parents);

    ZB_ZCL_WWAH_SEND_SURVEY_BEACONS_RESPONSE_END(
      ptr,
      obuf,
      WWAH_CTX().wwah_hub_address,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      WWAH_CTX().wwah_hub_endpoint,
      src_endpoint,
      get_profile_id_by_endpoint(src_endpoint));

    WWAH_CTX().survey_beacons_in_progress = 0;

    TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_send_survey_beacons_response", (FMT__0));
  }
}
#endif

zb_ret_t zb_zcl_wwah_disable_ota_downgrades_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  zb_bool_t disable_bool = ZB_TRUE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_ota_downgrades_handler %hd", (FMT__H, param));
  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_DISABLE_OTA_DOWNGRADES_ID,
                       (zb_uint8_t *)&disable_bool,
                       ZB_FALSE);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_ota_downgrades_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_mgmt_leave_without_rejoin_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  zb_bool_t disable_bool = ZB_FALSE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_mgmt_leave_without_rejoin_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED_ID,
                       (zb_uint8_t *)&disable_bool,
                       ZB_FALSE);
  zb_wwah_set_leave_without_rejoin_allowed(disable_bool);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_mgmt_leave_without_rejoin_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_touchlink_interpan_message_support_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  zb_bool_t disable_bool = ZB_FALSE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_touchlink_interpan_message_support_handler %hd", (FMT__H, param));

  /* can be enabled but not disabled */
  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_TOUCHLINK_INTERPAN_ENABLED_ID,
                       (zb_uint8_t *)&disable_bool,
                       ZB_FALSE);
  zb_wwah_set_interpan_supported(disable_bool);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_touchlink_interpan_message_support_handler, ret %d", (FMT__D, ret));
  return ret;
}

/**
 *  @brief Set value for the 'WWAH Parent Classification is enabled' attribute
 *
 *  @param is_enabled - new attribute value
 */
static zb_ret_t zdo_wwah_set_parent_classification_state(zb_bool_t is_enabled)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t current_state;
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc_parent_classification = NULL;

  TRACE_MSG(TRACE_ZDO1, ">>zdo_wwah_set_parent_classification_state, is_enabled %d", (FMT__D, is_enabled));

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);

  if (0x00 == endpoint || 0xFF == endpoint)
  {
    ret = RET_ERROR;
    TRACE_MSG(TRACE_ZCL2, "invalid endpoint", (FMT__0));
  }

  if (RET_OK == ret)
  {
    attr_desc_parent_classification = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_WWAH,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_WWAH_WWAH_PARENT_CLASSIFICATION_ENABLED_ID);
  }

  if (!attr_desc_parent_classification)
  {
    ret = RET_ERROR;
    TRACE_MSG(TRACE_ZCL2, "invalid attribute descriptor", (FMT__0));
  }

  if (RET_OK == ret)
  {
    current_state = *(zb_bool_t*)attr_desc_parent_classification->data_p;

    TRACE_MSG(TRACE_ZCL2, "current state %d, new state %d", (FMT__D_D, current_state, is_enabled));

    if (current_state != is_enabled)
    {
      /* sync with ZDO Hubs ctx */
      zdo_wwah_parent_classification_set(is_enabled);
      ZB_ZCL_SET_ATTRIBUTE(endpoint,
                           ZB_ZCL_CLUSTER_ID_WWAH,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           ZB_ZCL_ATTR_WWAH_WWAH_PARENT_CLASSIFICATION_ENABLED_ID,
                           (zb_uint8_t*)&is_enabled,
                           ZB_FALSE);
      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
    }
  }

  TRACE_MSG(TRACE_ZDO1, "<<zdo_wwah_set_parent_classification_state, ret %d", (FMT__D, ret));

  return ret;
}

zb_ret_t zb_zcl_wwah_enable_wwah_parent_classification_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_wwah_parent_classification_handler %hd", (FMT__H, param));

  ret = zdo_wwah_set_parent_classification_state(ZB_TRUE);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_wwah_parent_classification_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_wwah_parent_classification_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_wwah_parent_classification_handler %hd", (FMT__H, param));

  ret = zdo_wwah_set_parent_classification_state(ZB_FALSE);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_wwah_parent_classification_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_enable_tc_security_on_nwk_key_rotation_handler(zb_uint8_t param)
{
  zb_ret_t ret;
  zb_bool_t enable_bool = ZB_TRUE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_tc_security_on_nwk_key_rotation_handler %hd", (FMT__H, param));
  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED_ID,
                       (zb_uint8_t *)&enable_bool,
                       ZB_FALSE);

  /* WWAH ZCL Cluster Definition:
   * If TCSecurityOnNwkKeyRotationEnabled Attribute is set to TRUE,
   * the node SHALL only process network key rotation commands,
   * specifically APS Transport Key (command identifier 0x05),
   * which are sent via unicast and are encrypted by the Trust Center Link Key. */
  zb_wwah_set_nwk_key_commands_broadcast_allowed(ZB_FALSE);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_tc_security_on_nwk_key_rotation_handler, ret %d", (FMT__D, ret));
  return ret;
}

/* ----------------------- BAD PARENT RECOVERY ----------------------- */

void zb_zcl_wwah_bad_parent_recovery_tmo(zb_zcl_wwah_bad_parent_recovery_signal_t sig);

void zb_zcl_wwah_restart_bad_parent_recovery(zb_zcl_wwah_bad_parent_recovery_signal_t sig)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_restart_bad_parent_recovery: sig %hd", (FMT__H, sig));

  if (WWAH_CTX().bad_parent_recovery.started)
  {
    if (sig == ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED)
    {
      WWAH_CTX().bad_parent_recovery.poll_control_checkin_failed_cnt = 0;
    }
    else
    {
      ZB_SCHEDULE_ALARM_CANCEL((zb_callback_t)zb_zcl_wwah_bad_parent_recovery_tmo, sig);
      ZB_SCHEDULE_ALARM((zb_callback_t)zb_zcl_wwah_bad_parent_recovery_tmo, sig, ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_TIMEOUT);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_restart_bad_parent_recovery", (FMT__0));
}

void zb_zcl_wwah_bad_parent_recovery_signal(zb_zcl_wwah_bad_parent_recovery_signal_t sig)
{
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_bad_parent_recovery_signal sig %hd", (FMT__H, sig));

  if (WWAH_CTX().bad_parent_recovery.started)
  {
    switch (sig)
    {
      case ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_RSSI_WITH_PARENT_OK:
        TRACE_MSG(TRACE_ZCL1, "ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_RSSI_WITH_PARENT_OK", (FMT__0));
        zb_zcl_wwah_restart_bad_parent_recovery(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_RSSI_WITH_PARENT_BAD);
        break;
      case ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_APS_ACK_OK:
        TRACE_MSG(TRACE_ZCL1, "ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_APS_ACK_OK", (FMT__0));
        zb_zcl_wwah_restart_bad_parent_recovery(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_APS_ACK_FAILED);
        break;
      case ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_OK:
        TRACE_MSG(TRACE_ZCL1, "ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_OK", (FMT__0));
        zb_zcl_wwah_restart_bad_parent_recovery(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED);
        break;
      case ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED:
        TRACE_MSG(TRACE_ZCL1, "ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED", (FMT__0));
        ++WWAH_CTX().bad_parent_recovery.poll_control_checkin_failed_cnt;
        if (WWAH_CTX().bad_parent_recovery.poll_control_checkin_failed_cnt == ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED_CNT)
        {
          zb_zcl_wwah_bad_parent_recovery_tmo(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED);
        }
        break;
      default:
        TRACE_MSG(TRACE_ZCL1, "sig %hd, ignore", (FMT__H, sig));
        break;
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_bad_parent_recovery_signal", (FMT__0));
}

void zb_zcl_wwah_stop_bad_parent_recovery(void)
{
  ZB_SCHEDULE_ALARM_CANCEL((zb_callback_t)zb_zcl_wwah_bad_parent_recovery_tmo, ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_RSSI_WITH_PARENT_BAD);
  ZB_SCHEDULE_ALARM_CANCEL((zb_callback_t)zb_zcl_wwah_bad_parent_recovery_tmo, ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_APS_ACK_FAILED);
  WWAH_CTX().bad_parent_recovery.poll_control_checkin_failed_cnt = 0;
  WWAH_CTX().bad_parent_recovery.started = ZB_FALSE;
}

void zb_zcl_wwah_start_bad_parent_recovery(void)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_start_bad_parent_recovery", (FMT__0));

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);
  if (endpoint)
  {
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_BAD_PARENT_RECOVERY_ENABLED_ID);
    ZB_ASSERT(attr_desc);

    if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == ZB_TRUE &&
        !ZB_IS_DEVICE_ZC())
    {
      WWAH_CTX().bad_parent_recovery.started = ZB_TRUE;

      zb_zcl_wwah_restart_bad_parent_recovery(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_RSSI_WITH_PARENT_BAD);
      zb_zcl_wwah_restart_bad_parent_recovery(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_APS_ACK_FAILED);
      zb_zcl_wwah_restart_bad_parent_recovery(ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_start_bad_parent_recovery", (FMT__0));
}

void zb_zcl_wwah_bad_parent_recovery_tmo(zb_zcl_wwah_bad_parent_recovery_signal_t sig)
{
  TRACE_MSG(TRACE_ZCL1, "zb_zcl_wwah_start_bad_parent_recovery_tmo: sig %hd", (FMT__H, sig));
  zb_zcl_wwah_stop_bad_parent_recovery();
  bdb_start_rejoin_recovery(
    0,
    (sig == ZB_ZCL_WWAH_BAD_PARENT_RECOVERY_POLL_CONTROL_CHECK_IN_FAILED) ?
    BDB_COMM_REJOIN_REASON_POLL_CONTROL_CHECK_IN :
    BDB_COMM_REJOIN_REASON_UNSPECIFIED);
}

zb_ret_t zb_zcl_wwah_enable_wwah_bad_parent_recovery_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t enable_bool = ZB_TRUE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_wwah_bad_parent_recovery_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_WWAH_BAD_PARENT_RECOVERY_ENABLED_ID,
                       (zb_uint8_t *)&enable_bool,
                       ZB_FALSE);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  zb_zcl_wwah_start_bad_parent_recovery();

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_wwah_bad_parent_recovery_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_wwah_bad_parent_recovery_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t enable_bool = ZB_FALSE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_wwah_bad_parent_recovery_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_WWAH_BAD_PARENT_RECOVERY_ENABLED_ID,
                       (zb_uint8_t *)&enable_bool,
                       ZB_FALSE);
  if (RET_OK != zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA))
  {
    TRACE_MSG(TRACE_ERROR, "failed to write into nvram", (FMT__0));
  }
  zb_zcl_wwah_stop_bad_parent_recovery();

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_wwah_bad_parent_recovery_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_enable_configuration_mode_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t enable_bool = ZB_TRUE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_enable_configuration_mode_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_CONFIGURATION_MODE_ENABLED_ID,
                       (zb_uint8_t *)&enable_bool,
                       ZB_FALSE);
  zb_wwah_set_configuration_mode(enable_bool);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_enable_configuration_mode_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_disable_configuration_mode_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t disable_bool = ZB_FALSE;
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_disable_configuration_mode_handler %hd", (FMT__H, param));

  ZB_ZCL_SET_ATTRIBUTE(wwah_endpoint,
                       ZB_ZCL_CLUSTER_ID_WWAH,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_WWAH_CONFIGURATION_MODE_ENABLED_ID,
                       (zb_uint8_t *)&disable_bool,
                       ZB_FALSE);
  zb_wwah_set_configuration_mode(disable_bool);
  /* If we fail, trace is given and assertion is triggered */
  ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_disable_configuration_mode_handler, ret %d", (FMT__D, ret));
  return ret;
}

zb_ret_t zb_zcl_wwah_use_trust_center_for_cluster_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_wwah_cluster_list_t payload = { 0 };
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uindex_t i;
  zb_uint16_t cluster_role;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_use_trust_center_for_cluster_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_WWAH_GET_CLUSTER_LIST_CMD(param, payload, status);
  if (status == ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    if (payload.number_of_clusters > ZB_ZCL_WWAH_USE_TRUST_CENTER_FOR_CLUSTER_TABLE_SIZE)
    {
      status = ZB_ZCL_PARSE_STATUS_FAILURE;
    }
    for(i = 0; i < payload.number_of_clusters; ++i)
    {
      /* The set of clusters that will be assigned in this fashion are the following: */
      if (!(payload.cluster_id[i] == ZB_ZCL_CLUSTER_ID_OTA_UPGRADE ||
          payload.cluster_id[i] == ZB_ZCL_CLUSTER_ID_TIME ||
          payload.cluster_id[i] == ZB_ZCL_CLUSTER_ID_POLL_CONTROL ||
          payload.cluster_id[i] == ZB_ZCL_CLUSTER_ID_KEEP_ALIVE))
      {
        status = ZB_ZCL_PARSE_STATUS_FAILURE;
      }
    }
  }
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS )
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_WWAH_USE_TRUST_CENTER_FOR_CLUSTER_HANDLER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    /* On receipt of this command, a device SHALL use the Trust Center for all clusters in the list given. */
    ZB_MEMSET(WWAH_CTX().use_trust_center_for_cluster_table, (zb_uint8_t)ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD, sizeof(WWAH_CTX().use_trust_center_for_cluster_table));
    WWAH_CTX().use_trust_center_for_cluster_table_cnt = 0;
    for(i = 0; i < payload.number_of_clusters; ++i)
    {
      cluster_role = payload.cluster_id[i] == ZB_ZCL_CLUSTER_ID_POLL_CONTROL? ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE;
      /* ignore any cluster IDs in the list for clusters not supported by the device. */
      if (get_endpoint_by_cluster(payload.cluster_id[i], cluster_role))
      {
        WWAH_CTX().use_trust_center_for_cluster_table[WWAH_CTX().use_trust_center_for_cluster_table_cnt++] = payload.cluster_id[i];
      }
      if (payload.cluster_id[i] == ZB_ZCL_CLUSTER_ID_OTA_UPGRADE)
      {
        /* OTA TODO: check if OTA client is active, else not call and not link */
        zb_buf_get_out_delayed(zcl_ota_abort_and_set_tc);
      }
    }
    /* If we fail, trace is given and assertion is triggered */
    ret = zb_nvram_write_dataset(ZB_NVRAM_ZCL_WWAH_DATA);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_use_trust_center_for_cluster_handler", (FMT__0));
  return ret;
}

zb_ret_t zb_zcl_wwah_trust_center_for_cluster_server_query_handler(zb_uint8_t param)
{
  zb_uint8_t *resp_data;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_trust_center_for_cluster_server_query_handler %hd", (FMT__H, param));
  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
  ZB_ZCL_WWAH_SEND_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_RESPONSE_START(param, cmd_info.seq_number, WWAH_CTX().use_trust_center_for_cluster_table_cnt, resp_data);
  for(i = 0; i < WWAH_CTX().use_trust_center_for_cluster_table_cnt; ++i)
  {
    ZB_ZCL_WWAH_SEND_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_RESPONSE_ADD(resp_data, &WWAH_CTX().use_trust_center_for_cluster_table[i]);
  }
  ZB_ZCL_WWAH_SEND_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_RESPONSE_END(
    resp_data,
    param,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
    cmd_info.profile_id);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_trust_center_for_cluster_server_query_handler", (FMT__0));
  return RET_BUSY;
}

void zb_zcl_wwah_send_power_descriptor_change(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_send_power_descriptor_change", (FMT__0));
  ZB_ZCL_WWAH_SEND_POWER_DESCRIPTOR_CHANGE(param,
                                           WWAH_CTX().wwah_hub_address,
                                           ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                           WWAH_CTX().wwah_hub_endpoint,
                                           wwah_endpoint,
                                           ZB_AF_HA_PROFILE_ID,
                                           NULL,
                                           ZB_ZDO_NODE_POWER_DESC());
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_send_power_descriptor_change", (FMT__0));
}

void zb_zcl_wwah_schedule_send_power_descriptor_change(void)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_wwah_schedule_send_power_descriptor_change", (FMT__0));
  if (zb_is_wwah_server())
  {
    zb_buf_get_out_delayed(zb_zcl_wwah_send_power_descriptor_change);
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_wwah_schedule_send_power_descriptor_change", (FMT__0));
}

void zb_zcl_wwah_match_desc_resp_handle(zb_uint8_t param)
{
  zb_zdo_match_desc_resp_t *resp = (zb_zdo_match_desc_resp_t*)zb_buf_begin(param);
  zb_uint8_t *match_ep;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_periodic_checkin_match_desc_resp_handle param %hd", (FMT__H, param));

  if (resp->status == ZB_ZDP_STATUS_SUCCESS
      && resp->tsn == WWAH_CTX().periodic_checkins.tsn
      && resp->match_len > 0)
  {
    /* Match EP list follows right after response header */
    match_ep = (zb_uint8_t*)(resp + 1);
    WWAH_CTX().periodic_checkins.endpoint = *match_ep;

    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wwah_periodic_checkin_match_desc_req_delayed, ZB_ALARM_ALL_CB);
    zb_zcl_wwah_recounter_checkin();
  }
  zb_buf_free(param);
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_periodic_checkin_match_desc_resp_handle", (FMT__0));
}

void zb_zcl_wwah_periodic_checkin_match_desc_req_delayed(zb_uint8_t param)
{
  ZVUNUSED(param);
  ++WWAH_CTX().periodic_checkins.poll_method;
  zb_buf_get_out_delayed(zb_zcl_wwah_periodic_checkin_match_desc_req);
}

void zb_zcl_wwah_periodic_checkin_match_desc_req(zb_uint8_t param)
{
  zb_zdo_match_desc_param_t *req;
  zb_af_endpoint_desc_t *ep_desc = NULL;
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_periodic_checkin_match_desc_req %hd", (FMT__H, param));

  if (WWAH_CTX().periodic_checkins.poll_method != ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED)
  {
    req = zb_buf_initial_alloc(param, sizeof(zb_zdo_match_desc_param_t) + (1) * sizeof(zb_uint16_t));

    req->nwk_addr = tc_short_addr;
    req->addr_of_interest = tc_short_addr;
    req->num_in_clusters = 1;
    req->num_out_clusters = 0;

   /* To implement WWAH Periodic Router Check-Ins device may either:
    * 1) attempt to read Cluster Revision Attribute
    * 2) leverage the Keepalive Server if supported.
    * As discussed second method has more priority. */

    switch (WWAH_CTX().periodic_checkins.poll_method)
    {
      case ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE:
        ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                                    ZB_ZCL_CLUSTER_CLIENT_ROLE);
        if (ep_desc)
        {
          req->cluster_list[0] = ZB_ZCL_CLUSTER_ID_KEEP_ALIVE;
          break;
        }
        ++WWAH_CTX().periodic_checkins.poll_method;
        /* FALLTHROUGH */
      case ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_CLUSTER_REVISION:
        ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_BASIC,
                                                    ZB_ZCL_CLUSTER_CLIENT_ROLE);
        if (ep_desc)
        {
          req->cluster_list[0] = ZB_ZCL_CLUSTER_ID_BASIC;
          break;
        }
        /* FALLTHROUGH */
      default:
        TRACE_MSG(TRACE_ERROR, "Keep-alive polling method not found.", (FMT__0));
        ZB_ASSERT(0);
        break;
    }

    req->profile_id = ep_desc->simple_desc->app_profile_id;

    TRACE_MSG(TRACE_ZCL1, "send match descr for cluster 0x%x", (FMT__D, req->cluster_list[0]));

    WWAH_CTX().periodic_checkins.tsn = zb_zdo_match_desc_req(param, zb_zcl_wwah_match_desc_resp_handle);

    if (WWAH_CTX().periodic_checkins.tsn != ZB_ZDO_INVALID_TSN)
    {
      ZB_SCHEDULE_ALARM(zb_zcl_wwah_periodic_checkin_match_desc_req_delayed, 0,
                        ZB_ZCL_WWAH_PERIODIC_CHECKINS_CLUSTER_MATCH_DESC_TIME);
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "tc poll finished: can not send match desc", (FMT__0));
      zb_zcl_wwah_stop_periodic_checkin();
      bdb_start_rejoin_recovery(param, BDB_COMM_REJOIN_REASON_UNSPECIFIED);
    }
  }
  else
  {
    zb_buf_free(param);
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_periodic_checkin_match_desc_req", (FMT__0));
}

void zb_zcl_wwah_start_periodic_checkin(void)
{
  /* Get trust center IEEE */
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_start_periodic_checkin", (FMT__0));

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);
  if (endpoint)
  {
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID);
    ZB_ASSERT(attr_desc);

    /* Check it was not started before */
    if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == ZB_TRUE &&
        !ZB_IS_DEVICE_ZC() &&
        zb_get_rx_on_when_idle() &&
        WWAH_CTX().periodic_checkins.endpoint == 0 &&
        WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED)
    {
      TRACE_MSG(TRACE_ZCL1, "zb_zcl_wwah_start_periodic_checkin: start", (FMT__0));
      /* Send Match Desc - Keep-Alive cluster */
      if (!zb_aib_trust_center_address_zero() &&
          tc_short_addr != ZB_UNKNOWN_SHORT_ADDR)
      {
        WWAH_CTX().periodic_checkins.poll_method = ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE;
        zb_buf_get_out_delayed(zb_zcl_wwah_periodic_checkin_match_desc_req);
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_start_periodic_checkin", (FMT__0));
}

void zb_zcl_wwah_periodic_checkin_countdown(zb_uint8_t param)
{
  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_periodic_checkin_countdown", (FMT__0));
  if (WWAH_CTX().periodic_checkins.countdown > 0)
  {
    ZB_SCHEDULE_ALARM(zb_zcl_wwah_periodic_checkin_countdown, 0, ZB_TIME_ONE_SECOND * WWAH_CTX().periodic_checkins.countdown);
    TRACE_MSG(TRACE_ZCL1, "cd move %d", (FMT__D, WWAH_CTX().periodic_checkins.countdown));
    WWAH_CTX().periodic_checkins.countdown = 0;
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "tc poll", (FMT__0));
    zb_buf_get_out_delayed(zb_zcl_wwah_periodic_checkin_tc_poll);
  }
}

void zb_zcl_wwah_stop_periodic_checkin(void)
{
  WWAH_CTX().periodic_checkins.countdown = 0;
  WWAH_CTX().periodic_checkins.endpoint = 0;
  WWAH_CTX().periodic_checkins.poll_method = ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED;
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wwah_periodic_checkin_countdown, ZB_ALARM_ALL_CB);

#if defined ZB_PARENT_CLASSIFICATION && defined ZB_ROUTER_ROLE
  nwk_set_tc_connectivity(0);
#endif
}

void zb_zcl_wwah_periodic_checkin_timeout(zb_uint8_t param)
{
  zb_bool_t is_fail = ZB_FALSE;

  ZVUNUSED(param);
  TRACE_MSG(TRACE_ZCL1, "zb_zcl_wwah_periodic_checkin_timeout param %hd", (FMT__H, param));
  if (WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE)
  {
    /* After 3 failed Keepalive attempts, the device SHALL perform the rejoin. */
    ++WWAH_CTX().periodic_checkins.failure_cnt;
    if (WWAH_CTX().periodic_checkins.failure_cnt >= ZB_ZCL_WWAH_PERIODIC_CHECKINS_MAX_FAILURE_CNT)
    {
      is_fail = ZB_TRUE;
    }
    else
    {
      zb_zcl_wwah_recounter_checkin();
    }
  }
  if (WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_CLUSTER_REVISION || is_fail)
  {
    zb_zcl_wwah_stop_periodic_checkin();
    bdb_start_rejoin_recovery(0, BDB_COMM_REJOIN_REASON_UNSPECIFIED);
  }
}

void zb_zcl_wwah_periodic_checkin_tc_poll(zb_uint8_t param)
{
  zb_ret_t ret = RET_BUSY;
  zb_uint8_t *cmd_ptr;
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();
  zb_af_endpoint_desc_t *ep_desc = NULL;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_periodic_checkin_tc_poll param %hd", (FMT__H, param));

  ZB_ASSERT(WWAH_CTX().periodic_checkins.endpoint &&
            WWAH_CTX().periodic_checkins.poll_method < ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED &&
            tc_short_addr != ZB_UNKNOWN_SHORT_ADDR);


  switch (WWAH_CTX().periodic_checkins.poll_method)
  {
    case ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE:
    {
      ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ(param, cmd_ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);

      ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID);
      ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID);

      ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                                  ZB_ZCL_CLUSTER_CLIENT_ROLE);
      ZB_ASSERT(ep_desc);

      ret = zb_zcl_finish_and_send_packet(param,
                                          cmd_ptr,
                                          (zb_addr_u *)(&tc_short_addr),
                                          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                          WWAH_CTX().periodic_checkins.endpoint,
                                          ep_desc->ep_id,
                                          get_profile_id_by_endpoint(ep_desc->ep_id),
                                          ZB_ZCL_CLUSTER_ID_KEEP_ALIVE,
                                          NULL);
    }
    break;

    case ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_CLUSTER_REVISION:
    {
      ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ_A(param,
                                          cmd_ptr,
                                          ZB_ZCL_FRAME_DIRECTION_TO_SRV,
                                          ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
      ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID);

      ep_desc = get_endpoint_by_cluster_with_role(ZB_ZCL_CLUSTER_ID_BASIC,
                                                  ZB_ZCL_CLUSTER_CLIENT_ROLE);
      ZB_ASSERT(ep_desc);

      ret = zb_zcl_finish_and_send_packet(param,
                                          cmd_ptr,
                                          (zb_addr_u *)(&tc_short_addr),
                                          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                          WWAH_CTX().periodic_checkins.endpoint,
                                          ep_desc->ep_id,
                                          get_profile_id_by_endpoint(ep_desc->ep_id),
                                          ZB_ZCL_CLUSTER_ID_BASIC,
                                          NULL);
    }
    break;

    default:
      ZB_ASSERT(0);
      break;
  }

  if (ret != RET_OK)
  {
    ++WWAH_CTX().periodic_checkins.failure_cnt;
  }
  else
  {
    WWAH_CTX().periodic_checkins.tsn = ZCL_CTX().seq_number - 1;
    /* Need to wait some time for the answer (unicast). Minumum border is
     * ZB_N_APS_ACK_WAIT_DURATION_FROM_NON_SLEEPY * ZB_N_APS_MAX_FRAME_RETRIES, but intermediate
     * hops may add some additional delay. */
    ZB_SCHEDULE_ALARM(zb_zcl_wwah_periodic_checkin_timeout, 0, ZB_N_APS_ACK_WAIT_DURATION_FROM_NON_SLEEPY * ZB_N_APS_MAX_FRAME_RETRIES);
  }

  if ((WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE &&
      WWAH_CTX().periodic_checkins.failure_cnt >= ZB_ZCL_WWAH_PERIODIC_CHECKINS_MAX_FAILURE_CNT) ||
      (WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_CLUSTER_REVISION &&
      (zb_bool_t)WWAH_CTX().periodic_checkins.failure_cnt))
  {
    zb_zcl_wwah_stop_periodic_checkin();
    bdb_start_rejoin_recovery((ret != RET_OK) ? param : 0, BDB_COMM_REJOIN_REASON_UNSPECIFIED);
  }
  else if (ret != RET_OK)
  {
    /* Reschedule next read attempt. */
    ZB_SCHEDULE_CALLBACK(zb_zcl_wwah_periodic_checkin_tc_poll, param);
  }
}

zb_bool_t zb_zcl_wwah_periodic_checkin_read_attr_handle(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_read_attr_res_t *resp = NULL;
  zb_uint8_t read_attr_resp_cnt = 0;
  zb_uint16_t cluster_id = cmd_info->cluster_id;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_wwah_periodic_checkin_read_attr_handle param %hd", (FMT__H, param));
  ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wwah_periodic_checkin_timeout, ZB_ALARM_ALL_CB);

  do
  {
    ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, resp);

    if (resp && resp->status == ZB_ZCL_STATUS_SUCCESS)
    {
      if (cluster_id == ZB_ZCL_CLUSTER_ID_KEEP_ALIVE)
      {
        if (resp->attr_id == ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_BASE_ID)
        {
          WWAH_CTX().periodic_checkins.keepalive_base = resp->attr_value[0] * 60;
          TRACE_MSG(TRACE_ZCL1, "keep-alive: base %hd minutes", (FMT__H, WWAH_CTX().periodic_checkins.keepalive_base));
        }
        else if (resp->attr_id == ZB_ZCL_ATTR_KEEP_ALIVE_TC_KEEP_ALIVE_JITTER_ID)
        {
          ZB_HTOLE16(&WWAH_CTX().periodic_checkins.keepalive_jitter, resp->attr_value);
          TRACE_MSG(TRACE_ZCL1, "keep-alive: jitter %hd seconds", (FMT__H, WWAH_CTX().periodic_checkins.keepalive_jitter));
        }
        ++read_attr_resp_cnt;
      }
      else if (cluster_id == ZB_ZCL_CLUSTER_ID_BASIC)
      {
        ++read_attr_resp_cnt;
      }
    }
  }
  while (resp);

  if (!read_attr_resp_cnt ||
      (WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE &&
       read_attr_resp_cnt != 2))
  {
    ++WWAH_CTX().periodic_checkins.failure_cnt;
  }

  if ((WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE &&
      WWAH_CTX().periodic_checkins.failure_cnt == ZB_ZCL_WWAH_PERIODIC_CHECKINS_MAX_FAILURE_CNT) ||
      (WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_CLUSTER_REVISION &&
      WWAH_CTX().periodic_checkins.failure_cnt))
  {
    zb_zcl_wwah_stop_periodic_checkin();
    bdb_start_rejoin_recovery(param, BDB_COMM_REJOIN_REASON_UNSPECIFIED);
    param = 0;
  }
  else
  {
    zb_zcl_wwah_recounter_checkin();
    /* restore hub connectivity */
#if defined ZB_PARENT_CLASSIFICATION && defined ZB_ROUTER_ROLE
    nwk_set_tc_connectivity(1);
#endif
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_wwah_periodic_checkin_read_attr_handle", (FMT__0));

  return (zb_bool_t)(!param);
}

zb_bool_t zb_zcl_wwah_periodic_checkin_block_zcl_cmd(zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_uint16_t tc_short_addr = zb_aib_get_trust_center_short_address();

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_wwah_periodic_checkin_block_zcl_cmd: addr %d ep %hd tsn %hd",
            (FMT__D_H_H,
             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
             ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
             cmd_info->seq_number));

  return (zb_bool_t)(!IS_DISTRIBUTED_SECURITY() &&
    tc_short_addr != ZB_UNKNOWN_SHORT_ADDR &&
    WWAH_CTX().periodic_checkins.endpoint == ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint &&
    WWAH_CTX().periodic_checkins.tsn == cmd_info->seq_number &&
    WWAH_CTX().periodic_checkins.countdown == 0 &&
    (cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_BASIC ||
    cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_KEEP_ALIVE) &&
    zb_zcl_wwah_check_if_periodic_router_check_ins_enabled());
}

void zb_zcl_wwah_recounter_checkin(void)
{
  if (WWAH_CTX().periodic_checkins.poll_method != ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED &&
      WWAH_CTX().periodic_checkins.endpoint != 0)
  {
    TRACE_MSG(TRACE_ZCL1, "recounter zb_zcl_wwah_update_tc_connection", (FMT__0));
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_wwah_periodic_checkin_countdown, ZB_ALARM_ALL_CB);
    WWAH_CTX().periodic_checkins.countdown = WWAH_CTX().periodic_checkins.keepalive_base + ZB_RANDOM_VALUE(WWAH_CTX().periodic_checkins.keepalive_jitter);
    zb_zcl_wwah_periodic_checkin_countdown(0);
  }
}

void zb_zcl_wwah_update_tc_connection(zb_uint16_t short_addr)
{
  zb_uint16_t tc_short_addr;
  if (zb_is_wwah_server() &&
      WWAH_CTX().periodic_checkins.poll_method != ZB_ZCL_WWAH_PERIODIC_CHECKINS_NOT_SUPPORTED)
  {
    tc_short_addr = zb_aib_get_trust_center_short_address();
    if (tc_short_addr != ZB_UNKNOWN_SHORT_ADDR && tc_short_addr == short_addr)
    {
      zb_zcl_wwah_recounter_checkin();
    }
  }
}

zb_bool_t zb_zcl_process_wwah_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t ret = RET_OK;

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_wwah_specific_commands: buf %hd",
             (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  if( cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV )
  {
    /* Update WWAH endpoint value. */
    WWAH_CTX().wwah_hub_endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint;

    switch (cmd_info.cmd_id)
    {
      case ZB_ZCL_CMD_WWAH_ENABLE_APS_LINK_KEY_AUTHORIZATION_ID:
        ret = zb_zcl_wwah_enable_aps_link_key_authorization_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable APS Link Key Authorization command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_APS_LINK_KEY_AUTHORIZATION_ID:
        ret = zb_zcl_wwah_disable_aps_link_key_authorization_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable APS Link Key Authorization command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_APS_LINK_KEY_AUTHORIZATION_QUERY_ID:
        ret = zb_zcl_wwah_aps_link_key_authorization_query_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed APS Link Key Authorization Query command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_REQUEST_NEW_APS_LINK_KEY_ID:
        ret = zb_zcl_wwah_request_new_aps_link_key_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Request New APS Link Key command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_WWAH_APP_EVENT_RETRY_ALGORITHM_ID:
        ret = zb_zcl_wwah_enable_wwah_app_event_retry_algorithm_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable WWAH App Event Retry Algorithm command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_WWAH_APP_EVENT_RETRY_ALGORITHM_ID:
        ret = zb_zcl_wwah_disable_wwah_app_event_retry_algorithm_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable WWAH App Event Retry Algorithm command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_REQUEST_TIME_ID:
        ret = zb_zcl_wwah_request_time_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Request Time command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_WWAH_REJOIN_ALGORITHM_ID:
        ret = zb_zcl_wwah_enable_wwah_rejoin_algorithm_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable WWAH Rejoin Algorithm command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_WWAH_REJOIN_ALGORITHM_ID:
        ret = zb_zcl_wwah_disable_wwah_rejoin_algorithm_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable WWAH Rejoin Algorithm command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_SET_IAS_ZONE_ENROLLMENT_METHOD_ID:
        ret = zb_zcl_wwah_set_ias_zone_enrollment_method_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Set IAS Zone Enrollment Method command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_CLEAR_BINDING_TABLE_ID:
        ret = zb_zcl_wwah_clear_binding_table_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Clear Binding Table command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_PERIODIC_ROUTER_CHECK_INS_ID:
        ret = zb_zcl_wwah_enable_periodic_router_check_ins_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable Periodic Router Check Ins command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_PERIODIC_ROUTER_CHECK_INS_ID:
        ret = zb_zcl_wwah_disable_periodic_router_check_ins_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable Periodic Router Check Ins command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_SET_MAC_POLL_CCA_WAIT_TIME_ID:
        ret = zb_zcl_wwah_set_mac_poll_cca_wait_time_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Set MAC Poll CCA Wait Time command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_SET_PENDING_NETWORK_UPDATE_ID:
        ret = zb_zcl_wwah_set_pending_network_update_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Set Pending Network Update command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_REQUIRE_APS_ACKS_ON_UNICASTS_ID:
        ret = zb_zcl_wwah_require_aps_acks_on_unicasts_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Require APS ACKs on Unicasts command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_REMOVE_APS_ACKS_ON_UNICASTS_REQUIREMENT_ID:
        ret = zb_zcl_wwah_remove_aps_acks_on_unicasts_requirement_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Remove APS ACKs on Unicasts Requirement command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_APS_ACK_REQUIREMENT_QUERY_ID:
        ret = zb_zcl_wwah_aps_ack_requirement_query_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed APS ACK Requirement Query command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DEBUG_REPORT_QUERY_ID:
        ret = zb_zcl_wwah_debug_report_query_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Debug Report Query command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_SURVEY_BEACONS_ID:
        ret = zb_zcl_wwah_survey_beacons_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Survey Beacons command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_OTA_DOWNGRADES_ID:
        ret = zb_zcl_wwah_disable_ota_downgrades_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable OTA Downgrades command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_MGMT_LEAVE_WITHOUT_REJOIN_ID:
        ret = zb_zcl_wwah_disable_mgmt_leave_without_rejoin_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable MGMT Leave Without Rejoin command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_TOUCHLINK_INTERPAN_MESSAGE_SUPPORT_ID:
        ret = zb_zcl_wwah_disable_touchlink_interpan_message_support_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable Touchlink Interpan Message Support command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_WWAH_PARENT_CLASSIFICATION_ID:
        ret = zb_zcl_wwah_enable_wwah_parent_classification_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable WWAH Parent Classification command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_WWAH_PARENT_CLASSIFICATION_ID:
        ret = zb_zcl_wwah_disable_wwah_parent_classification_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable WWAH Parent Classification command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_TC_SECURITY_ON_NWK_KEY_ROTATION_ID:
        ret = zb_zcl_wwah_enable_tc_security_on_nwk_key_rotation_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable TC Security On Nwk Key Rotation command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_WWAH_BAD_PARENT_RECOVERY_ID:
        ret = zb_zcl_wwah_enable_wwah_bad_parent_recovery_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable WWAH Bad Parent Recovery command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_WWAH_BAD_PARENT_RECOVERY_ID:
        ret = zb_zcl_wwah_disable_wwah_bad_parent_recovery_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable WWAH Bad Parent Recovery command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_ENABLE_CONFIGURATION_MODE_ID:
        ret = zb_zcl_wwah_enable_configuration_mode_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Enable Configuration Mode command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_DISABLE_CONFIGURATION_MODE_ID:
        ret = zb_zcl_wwah_disable_configuration_mode_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Disable Configuration Mode command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_USE_TRUST_CENTER_FOR_CLUSTER_ID:
        ret = zb_zcl_wwah_use_trust_center_for_cluster_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Use Trust Center for Cluster command", (FMT__0));
        break;

      case ZB_ZCL_CMD_WWAH_TRUST_CENTER_FOR_CLUSTER_SERVER_QUERY_ID:
        ret = zb_zcl_wwah_trust_center_for_cluster_server_query_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed Trust Center for Cluster Server Query command", (FMT__0));
        break;

      default:
        processed = ZB_FALSE;
        break;
    }
  }

  if (ret == RET_BUSY)
  {
    // do nothing: buf has reused, processed has set ZB_TRUE
  }
  else if(processed == ZB_FALSE)
  {
    // do nothing: base func will send  else if(processed == ZB_FALSE)
    /* TODO: Recheck handling of unknown WWAH command. Does server and client correctly respond? */
  }
  else if (ret != RET_OK)
  {
    zb_uint8_t status_code;
    switch(ret)
    {
    case RET_OUT_OF_RANGE:
      status_code = ZB_ZCL_STATUS_INVALID_VALUE;
      break;
    case RET_INVALID_PARAMETER:
      status_code = ZB_ZCL_STATUS_INVALID_FIELD;
      break;
    case RET_IGNORE:
      /* ZCL8: CCB 2477: use UNSUP_COMMAND instead of any other Unsupported command status */
      status_code = ZB_ZCL_STATUS_UNSUP_CMD;
      break;
    case RET_NOT_FOUND:
      status_code = ZB_ZCL_STATUS_NOT_FOUND;
      break;
    default:
      status_code = ZB_ZCL_STATUS_FAIL;
      break;
    }
    ZB_ZCL_SEND_DEFAULT_RESP_MANUF(param,
                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                                   ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                                   cmd_info.profile_id,
                                   ZB_ZCL_CLUSTER_ID_WWAH,
                                   cmd_info.seq_number,
                                   cmd_info.cmd_id,
                                   status_code,
                                   WWAH_MANUFACTURER_CODE,
                                   ZB_ZCL_FRAME_DIRECTION_TO_CLI);
  }
  else if (!(cmd_info.disable_default_response))
  {
    ZB_ZCL_SEND_DEFAULT_RESP_MANUF(param,
                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                                   ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                                   ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                                   cmd_info.profile_id,
                                   ZB_ZCL_CLUSTER_ID_WWAH,
                                   cmd_info.seq_number,
                                   cmd_info.cmd_id,
                                   processed == ZB_TRUE ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL,
                                   WWAH_MANUFACTURER_CODE,
                                   ZB_ZCL_FRAME_DIRECTION_TO_CLI);
  }
  else
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_wwah_specific_commands: processed %d",
            (FMT__D, processed));

  return ZB_TRUE;
}


zb_bool_t zb_zcl_process_wwah_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_wwah_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_wwah_specific_commands(param);
}

void zb_zcl_wwah_trace_ctx(zb_uint8_t param)
{
  zb_uint8_t i = 0;
  ZVUNUSED(param);

  TRACE_MSG(TRACE_ZCL3, ">> WWAH CTX", (FMT__0));
  TRACE_MSG(TRACE_ZCL3, "wwah_behavior %hd wwah_hub_endpoint %hd time_server_endpoint %hd", (FMT__H_H_H, WWAH_CTX().wwah_behavior, WWAH_CTX().wwah_hub_endpoint, WWAH_CTX().time_server_endpoint));
  TRACE_MSG(TRACE_ZCL3, "wwah_hub_address %hd aps_link_key_enabled_by_default %hd", (FMT__D_H, WWAH_CTX().wwah_hub_address, WWAH_CTX().aps_link_key_enabled_by_default));

  /* WWAH tables */
  TRACE_MSG(TRACE_ZCL3, "aps_ack_exempt_table: %hd elements", (FMT__H, WWAH_CTX().aps_ack_exempt_table_cnt));
  for(i = 0; i < WWAH_CTX().aps_ack_exempt_table_cnt; ++i)
  {
    TRACE_MSG(TRACE_ZCL3, "  [%hd]: cluster_id 0x%x", (FMT__H_D, i, WWAH_CTX().aps_ack_exempt_table[i]));
  }
  TRACE_MSG(TRACE_ZCL3, "aps_link_key_authorization_table: %hd elements", (FMT__H, WWAH_CTX().aps_link_key_authorization_table_cnt));
  for(i = 0; i < WWAH_CTX().aps_link_key_authorization_table_cnt; ++i)
  {
    TRACE_MSG(TRACE_ZCL3, "  [%hd]: cluster_id 0x%x", (FMT__H_D, i, WWAH_CTX().aps_link_key_authorization_table[i]));
  }
  TRACE_MSG(TRACE_ZCL3, "use_trust_center_for_cluster_table: %hd elements", (FMT__H, WWAH_CTX().use_trust_center_for_cluster_table_cnt));
  for(i = 0; i < WWAH_CTX().use_trust_center_for_cluster_table_cnt; ++i)
  {
    TRACE_MSG(TRACE_ZCL3, "  [%hd]: cluster_id 0x%x", (FMT__H_D, i, WWAH_CTX().use_trust_center_for_cluster_table[i]));
  }

  TRACE_MSG(TRACE_ZCL3, "<< WWAH CTX", (FMT__0));
}

/* NVRAM Read/Write API */

/* TRICKY: Need to write aligned by 4 - so if not aligned, simply write one more empty record.
   Macro return: how many 2-byte entries to write - to be sure overall result is aligned to 4. */
#define ALIGN_TABLE_CNT_TO_4(_cnt) ((((_cnt)*2 + 3)/4)*2)

zb_uint16_t zb_nvram_wwah_dataset_size(void)
{
  return (sizeof(zb_nvram_dataset_wwah_t) +
          (ALIGN_TABLE_CNT_TO_4(WWAH_CTX().aps_ack_exempt_table_cnt) +
           ALIGN_TABLE_CNT_TO_4(WWAH_CTX().aps_link_key_authorization_table_cnt) +
           ALIGN_TABLE_CNT_TO_4(WWAH_CTX().use_trust_center_for_cluster_table_cnt)) *
           sizeof(zb_uint16_t));
}


void zb_nvram_read_wwah_dataset(
  zb_uint8_t page, zb_uint32_t pos, zb_uint16_t len, zb_nvram_ver_t nvram_ver, zb_uint16_t ds_ver)

{
  zb_nvram_dataset_wwah_t ds;
  zb_ret_t ret;

  ZVUNUSED(len);
  ZVUNUSED(nvram_ver);
  ZVUNUSED(ds_ver);

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_read_wwah_dataset %d pos %ld len %d nvram_ver %d",
      (FMT__H_L_D_D, page, pos, len, nvram_ver));

  ret = zb_osif_nvram_read(page, pos, (zb_uint8_t*)&ds, sizeof(ds));

  if (ret == RET_OK)
  {
    zb_uint8_t endpoint;
    zb_zcl_attr_t *attr_desc;

    pos += sizeof(ds);

    /* Restore WWAH CTX */
    WWAH_CTX().wwah_behavior = ds.wwah_behavior;
    WWAH_CTX().aps_link_key_enabled_by_default = (zb_bool_t)ds.aps_link_key_enabled_by_default;

    ZB_MEMCPY((zb_uint8_t*)&WWAH_CTX().periodic_checkins,
              (zb_uint8_t*)&ds.periodic_checkins,
              sizeof(zb_zcl_wwah_periodic_checkins_data_t));

    WWAH_CTX().periodic_checkins.tsn = 0;
    WWAH_CTX().periodic_checkins.failure_cnt = 0;
    WWAH_CTX().periodic_checkins.countdown = 0;

    /* Restore WWAH attributes */
    endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);
    ZB_ASSERT(endpoint);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.cluster_revision);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_DISABLE_OTA_DOWNGRADES_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.disable_ota_downgrades);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.mgmt_leave_without_rejoin_enabled);
    zb_wwah_set_leave_without_rejoin_allowed(ds.mgmt_leave_without_rejoin_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_NWK_RETRY_COUNT_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.nwk_retry_count);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_MAC_RETRY_COUNT_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.mac_retry_count);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.router_check_in_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_TOUCHLINK_INTERPAN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.touchlink_interpan_enabled);
    zb_wwah_set_interpan_supported(ds.touchlink_interpan_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_PARENT_CLASSIFICATION_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.wwah_parent_classification_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_APP_EVENT_RETRY_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.wwah_app_event_retry_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_APP_EVENT_RETRY_QUEUE_SIZE_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.wwah_app_event_retry_queue_size);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_REJOIN_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.wwah_rejoin_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_MAC_POLL_FAILURE_WAIT_TIME_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.mac_poll_failure_wait_time);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_CONFIGURATION_MODE_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.configuration_mode_enabled);
    zb_wwah_set_configuration_mode(ds.configuration_mode_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_CURRENT_DEBUG_REPORT_ID_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.current_debug_report_id);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.tc_security_on_nwk_key_rotation_enabled);
    zb_wwah_set_nwk_key_commands_broadcast_allowed(ds.tc_security_on_nwk_key_rotation_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_WWAH_BAD_PARENT_RECOVERY_ENABLED_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.wwah_bad_parent_recovery_enabled);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_CHANNEL_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ds.pending_network_update_channel);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_PANID_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.pending_network_update_panid);
    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_WWAH_OTA_MAX_OFFLINE_DURATION_ID);
    ZB_ASSERT(attr_desc);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, ds.ota_max_offline_duration);

    /* Restore WWAH tables */
    {
      if (ds.aps_ack_exempt_table_cnt &&
          ZB_ARRAY_SIZE(WWAH_CTX().aps_ack_exempt_table) >= ds.aps_ack_exempt_table_cnt)
      {
        ret = zb_osif_nvram_read(page, pos,
                                 (zb_uint8_t*)&WWAH_CTX().aps_ack_exempt_table,
                                 ds.aps_ack_exempt_table_cnt * sizeof(zb_uint16_t));
        pos += ds.aps_ack_exempt_table_cnt * sizeof(zb_uint16_t);


        if (ret == RET_OK)
        {
          WWAH_CTX().aps_ack_exempt_table_cnt = ds.aps_ack_exempt_table_cnt;
          /* Fix alignment */
          if (WWAH_CTX().aps_ack_exempt_table[WWAH_CTX().aps_ack_exempt_table_cnt-1] ==
              ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD)
          {
            --WWAH_CTX().aps_ack_exempt_table_cnt;
          }
        }
      }

      if (ret == RET_OK &&
          ds.aps_link_key_authorization_table_cnt &&
          ZB_ARRAY_SIZE(WWAH_CTX().aps_link_key_authorization_table) >= ds.aps_link_key_authorization_table_cnt)
      {
        ret = zb_osif_nvram_read(page, pos,
                                 (zb_uint8_t*)&WWAH_CTX().aps_link_key_authorization_table,
                                 ds.aps_link_key_authorization_table_cnt * sizeof(zb_uint16_t));
        pos += ds.aps_link_key_authorization_table_cnt * sizeof(zb_uint16_t);

        if (ret == RET_OK)
        {
          WWAH_CTX().aps_link_key_authorization_table_cnt = ds.aps_link_key_authorization_table_cnt;
          /* Fix alignment */
          if (WWAH_CTX().aps_link_key_authorization_table[WWAH_CTX().aps_link_key_authorization_table_cnt-1] == ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD)
          {
            --WWAH_CTX().aps_link_key_authorization_table_cnt;
          }
        }
      }

      if (ret == RET_OK &&
          ds.use_trust_center_for_cluster_table_cnt &&
          ZB_ARRAY_SIZE(WWAH_CTX().use_trust_center_for_cluster_table) >= ds.use_trust_center_for_cluster_table_cnt)
      {
        ret = zb_osif_nvram_read(page, pos,
                                 (zb_uint8_t*)&WWAH_CTX().use_trust_center_for_cluster_table,
                                 ds.use_trust_center_for_cluster_table_cnt * sizeof(zb_uint16_t));

        if (ret == RET_OK)
        {
          WWAH_CTX().use_trust_center_for_cluster_table_cnt = ds.use_trust_center_for_cluster_table_cnt;
          /* Fix alignment */
          if (WWAH_CTX().use_trust_center_for_cluster_table[WWAH_CTX().use_trust_center_for_cluster_table_cnt-1] ==
              ZB_ZCL_WWAH_CLUSTER_ID_FREE_RECORD)
          {
            --WWAH_CTX().use_trust_center_for_cluster_table_cnt;
          }
        }
      }
    }
  }

  zb_zcl_wwah_trace_ctx(0);

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_read_wwah_dataset", (FMT__0));
}

zb_ret_t zb_nvram_write_wwah_dataset(zb_uint8_t page, zb_uint32_t pos)
{
  zb_nvram_dataset_wwah_t ds;
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc;
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "> zb_nvram_write_wwah_dataset page %d pos %ld",
      (FMT__H_L, page, pos));

  ZB_BZERO(&ds, sizeof(ds));

  /* Store WWAH CTX */
  ds.wwah_behavior = WWAH_CTX().wwah_behavior;
  ds.aps_link_key_enabled_by_default = WWAH_CTX().aps_link_key_enabled_by_default;

  ZB_MEMCPY((zb_uint8_t*)&ds.periodic_checkins,
            (zb_uint8_t*)&WWAH_CTX().periodic_checkins,
            sizeof(zb_zcl_wwah_periodic_checkins_data_t));

  /* Store WWAH attributes */
  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_WWAH, ZB_ZCL_CLUSTER_SERVER_ROLE);
  ZB_ASSERT(endpoint);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID);
  ZB_ASSERT(attr_desc);
  ds.cluster_revision = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_DISABLE_OTA_DOWNGRADES_ID);
  ZB_ASSERT(attr_desc);
  ds.disable_ota_downgrades = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_MGMT_LEAVE_WITHOUT_REJOIN_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.mgmt_leave_without_rejoin_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_NWK_RETRY_COUNT_ID);
  ZB_ASSERT(attr_desc);
  ds.nwk_retry_count = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_MAC_RETRY_COUNT_ID);
  ZB_ASSERT(attr_desc);
  ds.mac_retry_count = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_ROUTER_CHECK_IN_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.router_check_in_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_TOUCHLINK_INTERPAN_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.touchlink_interpan_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_WWAH_PARENT_CLASSIFICATION_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.wwah_parent_classification_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_WWAH_APP_EVENT_RETRY_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.wwah_app_event_retry_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_WWAH_APP_EVENT_RETRY_QUEUE_SIZE_ID);
  ZB_ASSERT(attr_desc);
  ds.wwah_app_event_retry_queue_size = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_WWAH_REJOIN_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.wwah_rejoin_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_MAC_POLL_FAILURE_WAIT_TIME_ID);
  ZB_ASSERT(attr_desc);
  ds.mac_poll_failure_wait_time = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_CONFIGURATION_MODE_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.configuration_mode_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_CURRENT_DEBUG_REPORT_ID_ID);
  ZB_ASSERT(attr_desc);
  ds.current_debug_report_id = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_TC_SECURITY_ON_NWK_KEY_ROTATION_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.tc_security_on_nwk_key_rotation_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_WWAH_BAD_PARENT_RECOVERY_ENABLED_ID);
  ZB_ASSERT(attr_desc);
  ds.wwah_bad_parent_recovery_enabled = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_CHANNEL_ID);
  ZB_ASSERT(attr_desc);
  ds.pending_network_update_channel = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_PENDING_NETWORK_UPDATE_PANID_ID);
  ZB_ASSERT(attr_desc);
  ds.pending_network_update_panid = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
  //
  attr_desc = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_WWAH,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_WWAH_OTA_MAX_OFFLINE_DURATION_ID);
  ZB_ASSERT(attr_desc);
  ds.ota_max_offline_duration = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

  ds.aps_ack_exempt_table_cnt = ALIGN_TABLE_CNT_TO_4(WWAH_CTX().aps_ack_exempt_table_cnt);
  ZB_ASSERT(ds.aps_ack_exempt_table_cnt <= ZB_ARRAY_SIZE(WWAH_CTX().aps_ack_exempt_table));
  ds.aps_link_key_authorization_table_cnt = ALIGN_TABLE_CNT_TO_4(WWAH_CTX().aps_link_key_authorization_table_cnt);
  ZB_ASSERT(ds.aps_link_key_authorization_table_cnt <= ZB_ARRAY_SIZE(WWAH_CTX().aps_link_key_authorization_table));
  ds.use_trust_center_for_cluster_table_cnt = ALIGN_TABLE_CNT_TO_4(WWAH_CTX().use_trust_center_for_cluster_table_cnt);
  ZB_ASSERT(ds.use_trust_center_for_cluster_table_cnt <= ZB_ARRAY_SIZE(WWAH_CTX().use_trust_center_for_cluster_table));

  ret = zb_nvram_write_data(page, pos, (zb_uint8_t*)&ds, sizeof(ds));
  pos += sizeof(ds);

  if (ret == RET_OK)
  {
    /* Store WWAH tables */
    if (ds.aps_ack_exempt_table_cnt)
    {
      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_write_data(page, pos,
                                (zb_uint8_t*)&WWAH_CTX().aps_ack_exempt_table,
                                ds.aps_ack_exempt_table_cnt * sizeof(zb_uint16_t));
      pos += ds.aps_ack_exempt_table_cnt * sizeof(zb_uint16_t);
    }

    if (ret == RET_OK && ds.aps_link_key_authorization_table_cnt)
    {
      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_write_data(page, pos,
                                (zb_uint8_t*)&WWAH_CTX().aps_link_key_authorization_table,
                                ds.aps_link_key_authorization_table_cnt * sizeof(zb_uint16_t));
      pos += ds.aps_link_key_authorization_table_cnt * sizeof(zb_uint16_t);
    }

    if (ret == RET_OK && ds.use_trust_center_for_cluster_table_cnt)
    {
      /* If we fail, trace is given and assertion is triggered */
      ret = zb_nvram_write_data(page, pos,
                                (zb_uint8_t*)&WWAH_CTX().use_trust_center_for_cluster_table,
                                ds.use_trust_center_for_cluster_table_cnt * sizeof(zb_uint16_t));
    }
  }

  zb_zcl_wwah_trace_ctx(0);

  TRACE_MSG(TRACE_ZCL1, "< zb_nvram_write_wwah_dataset ret %hd", (FMT__H, ret));
  return ret;
}


void wwah_post_commissioning_actions(void)
{
  if (zb_is_wwah_server())
  {
    zb_zcl_wwah_start_periodic_checkin();
    zb_zcl_wwah_start_bad_parent_recovery();
  }
}

#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */

#endif /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */
