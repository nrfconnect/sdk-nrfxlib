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
/* PURPOSE: WWAH ZDO Level selector for ZCL WWAH Stubs for all func.
*/

#ifndef ZDO_WWAH_STUBS_H
#define ZDO_WWAH_STUBS_H 1

#include "zdo_hubs.h"

#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
#include "zcl/zb_zcl_wwah.h"

#define ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR() zb_is_wwah_server() /* used in zcl */
#define ZB_ZDO_CHECK_NEW_CHANNEL(new_channel_mask) zb_zcl_wwah_check_new_channel(new_channel_mask)
#define ZB_ZDO_CHECK_NEW_PANID(new_panid) zb_zcl_wwah_check_new_panid(new_panid)
#define ZB_ZDO_GET_MAX_PARENT_THRESHOLD_RETRY() ZDO_CTX().max_parent_threshold_retry * (1U + zb_zcl_wwah_mac_poll_cca_retry_count())

 /* Common logic when we have problems with parent polling:
  * 1) Data Request + 3 MAC Retries
  * 2) Wait MACPollFailureWaitTime
  * 3) Repeat steps 1-2 wwah_retry_cnt times
  * 4) Wait work_poll_interval
  * 5) Repeat steps 1-4 ZDO_CTX().max_parent_threshold_retry times
  */

 /* Note:
  *   1) if current retry count(crc) equals zero it means that last poll was success
  *   2) crc >= maximum_retry_count leads to polling stop
  *   3) crc % (wwah_retry_cnt + 1) shows current iteration of additional polling,
  *      i.e. loop steps 1-2 above. If remainder is zero - wait for standart timeout,
  *      or additional timeout otherwise.
  */

#define ZB_ZDO_GET_CURENT_POLL_ITERATION_INTERVAL(wait_time)                              \
   if (zb_zcl_wwah_mac_poll_cca_retry_count() &&                                          \
    ZDO_CTX().parent_threshold_retry % (1 + zb_zcl_wwah_mac_poll_cca_retry_count()))      \
   {                                                                                      \
     (wait_time) = zb_zcl_wwah_mac_poll_failure_wait_time();                              \
   }                                                                                      \
   else                                                                                   \
   {                                                                                      \
     (wait_time) = ZDO_CTX().pim.work_poll_interval;                                      \
   }

void zb_zcl_wwah_update_tc_connection(zb_uint16_t short_addr);

#define ZB_ZDO_CHECK_NWK_KEY_COMMANDS_BROADCAST_ALLOWED() zb_wwah_check_nwk_key_commands_broadcast_allowed() /* moved */
#define ZB_ZDO_CHECK_ZDO_COMMAND(di) zb_wwah_check_zdo_command(di) /* moved */
#define ZB_ZDO_CHECK_IF_APS_ACK_NEEDED(cluster_id) zb_zcl_wwah_check_if_aps_ack_needed(cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_IF_APS_LINK_KEY_AUTHORIZATION_NEEDED(cluster_id) zb_zcl_wwah_check_if_aps_link_key_authorization_needed(cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_IF_FORCED_TO_USE_TC(cluster_id) zb_zcl_wwah_check_if_forced_to_use_tc(cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_CLUSTER_PERMISSION(nwk_addr, cluster_id) zb_zcl_wwah_check_cluster_permission(nwk_addr, cluster_id) /* used in zcl */
#define ZB_ZDO_CHECK_IF_LEAVE_WITHOUT_REJOIN_ALLOWED() zb_wwah_check_if_leave_without_rejoin_allowed() /* moved */
#define ZB_ZDO_CHECK_IF_WWAH_REJOIN_ENABLED() zb_zcl_wwah_check_if_wwah_rejoin_enabled() /* not used */
#define ZB_ZDO_SEND_POWER_DESCRIPTOR_CHANGE() zb_zcl_wwah_schedule_send_power_descriptor_change() /* AF to ZCL signal - keep */
#define ZB_ZDO_CHECK_IF_INTERPAN_SUPPORTED() zb_zcl_wwah_check_if_interpan_supported() /* moved */
#define ZB_ZDO_CHECK_IF_WWAH_POOLING_MECHANISM() zb_zcl_wwah_check_if_periodic_router_check_ins_enabled() /* not used */
#define ZB_ZDO_UPDATE_TC_CONNECTION(short_addr) zb_zcl_wwah_update_tc_connection(short_addr) /* call from nwk to zcl */
#define ZB_ZDO_CHECK_SRC_ADDRESS_FOR_CLUSTER(short_addr, cluster_id) zb_zcl_wwah_check_src_addr_for_cluster(short_addr, cluster_id) /* used in zcl */
#else
#define ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR() ZB_FALSE
#define ZB_ZDO_CHECK_NEW_CHANNEL(new_channel_mask) ZB_TRUE
#define ZB_ZDO_CHECK_NEW_PANID(new_panid) ZB_TRUE
#define ZB_ZDO_GET_MAX_PARENT_THRESHOLD_RETRY() ZDO_CTX().max_parent_threshold_retry
#define ZB_ZDO_GET_CURENT_POLL_ITERATION_INTERVAL(wait_time) (wait_time) = ZDO_CTX().pim.work_poll_interval
#define ZB_ZDO_CHECK_NWK_KEY_COMMANDS_BROADCAST_ALLOWED() ZB_TRUE
#define ZB_ZDO_CHECK_ZDO_COMMAND(di) ZB_TRUE
#define ZB_ZDO_CHECK_IF_APS_ACK_NEEDED(cluster_id) ZB_TRUE
#define ZB_ZDO_CHECK_IF_APS_LINK_KEY_AUTHORIZATION_NEEDED(cluster_id) ZB_FALSE
#define ZB_ZDO_CHECK_IF_FORCED_TO_USE_TC(cluster_id) ZB_FALSE
#define ZB_ZDO_CHECK_CLUSTER_PERMISSION(nwk_addr, cluster_id) ZB_TRUE
#ifdef ZB_ZDO_DENY_LEAVE_CONFIG
#define ZB_ZDO_CHECK_IF_LEAVE_WITHOUT_REJOIN_ALLOWED() ZG->zdo.leave_req_allowed
#else
#define ZB_ZDO_CHECK_IF_LEAVE_WITHOUT_REJOIN_ALLOWED() ZB_TRUE
#endif
#define ZB_ZDO_CHECK_IF_WWAH_REJOIN_ENABLED() ZB_FALSE
#define ZB_ZDO_SEND_POWER_DESCRIPTOR_CHANGE()
#define ZB_ZDO_CHECK_IF_INTERPAN_SUPPORTED() ZB_TRUE
#define ZB_ZDO_CHECK_IF_WWAH_POOLING_MECHANISM() ZB_FALSE
#define ZB_ZDO_UPDATE_TC_CONNECTION(short_addr)
#define ZB_ZDO_CHECK_SRC_ADDRESS_FOR_CLUSTER(short_addr, cluster_id) ZB_TRUE
#endif /* (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER) */

#if defined ZB_PARENT_CLASSIFICATION && defined ZB_ROUTER_ROLE
void zdo_schedule_set_long_uptime(zb_uint8_t param);
void zdo_schedule_cancel_long_uptime(zb_uint8_t param);

#define ZB_ZDO_SCHEDULE_UPDATE_LONG_UPTIME() zdo_schedule_set_long_uptime(0)
#define ZB_ZDO_CANCEL_UPDATE_LONG_UPTIME() zdo_schedule_cancel_long_uptime(0)

#else /* ZB_PARENT_CLASSIFICATION */
#define ZB_ZDO_SCHEDULE_UPDATE_LONG_UPTIME()
#define ZB_ZDO_CANCEL_UPDATE_LONG_UPTIME()

#endif /* ZB_PARENT_CLASSIFICATION */

#endif /* ZDO_WWAH_STUBS_H */
