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
/*  PURPOSE: Base Device Finding & Binding functionality. Initially copy-paste from
 ha_ez_mode_commissioning.c.
*/

#define ZB_TRACE_FILE_ID 859

#include "zb_common.h"

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)

#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zb_aps.h"
#include "zb_nwk.h"
#include "zcl/zb_zcl_common.h"
#include "zb_bdb_internal.h"
#include "zb_bdb_internal.h"

void zb_bdb_finding_binding_initiator_alarm(zb_uint8_t param);
void zb_bdb_finding_binding_target_alarm(zb_uint8_t param);
void zb_bdb_error_handle_buf(zb_int16_t status, zb_uint8_t param);
zb_bool_t zb_bdb_find_cluster_match(zb_uint16_t cluster_id, zb_uint8_t role);
void zb_bdb_active_eps_check_cb(zb_uint8_t param);
void zb_bdb_send_identify_query_next_req(zb_uint8_t param);
void zb_bdb_identify_query_sent_cb(zb_uint8_t param);
void zb_bdb_fb_send_active_ep_req(zb_uint8_t param, zb_uint16_t src_addr);
void zb_bdb_process_ext_addr_resp(zb_uint8_t param);

static zb_ret_t zb_bdb_find_respondent(zb_uint16_t nwk_addr, zb_uint8_t *resp_index);
static void zb_bdb_finding_binding_initiator_stop(  zb_uint8_t param, zb_uint16_t status);
static zb_ret_t zb_bdb_finding_binding_target_func(zb_uint8_t endpoint, zb_uint16_t commissioning_time_secs);

/*
  Initialization of EZ-Mode context
*/
void zb_bdb_finding_binding_init_ctx()
{
  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_finding_binding_init_ctx", (FMT__0));
  ZB_SCHEDULE_ALARM_CANCEL(zb_bdb_finding_binding_initiator_alarm, ZB_ALARM_ANY_PARAM);
  ZB_SCHEDULE_ALARM_CANCEL(zb_bdb_finding_binding_target_alarm, ZB_ALARM_ANY_PARAM);
  ZB_MEMSET((zb_uint8_t *)&BDB_COMM_CTX(), 0, ZB_OFFSETOF(zb_bdb_comm_ctx_t, FIRST_GENERAL_BDB_FIELD));
  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_finding_binding_init_ctx", (FMT__0));

  ZCL_SELECTOR().process_identify_query_res = zb_bdb_process_identify_query_res;
}


/*
****************************************
F & B Target
*/

/* Starts EZ-Mode Finding and binding mechanism at the target's endpoint internal fuction */
static zb_ret_t zb_bdb_finding_binding_target_func(zb_uint8_t endpoint, zb_uint16_t commissioning_time_secs)
{
  zb_ret_t ret = RET_OK;
  zb_bool_t bdb_finding_binding_started;

  TRACE_MSG(TRACE_ZCL1, "> bdb_finding_binding_target_func endpoint %hd", (FMT__H, endpoint));

  bdb_finding_binding_started =
	  (zb_bool_t)((zb_bdb_commissioning_mode_mask_t)ZB_BDB().bdb_commissioning_step != ZB_BDB_COMMISSIONING_STOP);

#ifdef ZB_STACK_REGRESSION_TESTING_API
  if (ZB_REGRESSION_TESTS_API().bdb_allow_multiple_fb_targets)
  {
    bdb_finding_binding_started = ZB_FALSE;
  }
#endif

  if (ret == RET_OK
      && (!zb_zdo_joined() || bdb_finding_binding_started))
  {
    ret = RET_INVALID_STATE;
  }

  if (ret == RET_OK)
  {
    /* Clear BDB Comissioning Mode state after identifying has finished */
    ZB_SCHEDULE_ALARM(zb_bdb_finding_binding_target_alarm, 0,
                      ZB_MILLISECONDS_TO_BEACON_INTERVAL(commissioning_time_secs * 1000));
    ret = ( zb_zcl_start_identifying(endpoint, commissioning_time_secs) == ZB_ZCL_STATUS_SUCCESS ?
            RET_OK : RET_ERROR );
  }
  if (ret == RET_OK)
  {
    ZB_BDB().bdb_commissioning_step = ZB_BDB_FINDING_N_BINDING;
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    BDB_COMM_CTX().state = ZB_BDB_COMM_FINDING_AND_BINDING_TARGET;
    BDB_COMM_CTX().ep = endpoint;
  }

  TRACE_MSG(TRACE_ZCL1, "< bdb_finding_binding_target_func ret %hd", (FMT__H, ret));

  return ret;
}


/* Starts EZ-Mode Finding and binding mechanism at the target's endpoint with default time */
zb_ret_t zb_bdb_finding_binding_target(zb_uint8_t endpoint)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> bdb_finding_binding_target endpoint %hd", (FMT__H, endpoint));

  /* Check arguments passed */
  if (!ZB_AF_IS_EP_REGISTERED(endpoint))
  {
    ret = RET_INVALID_PARAMETER_1;
  }

  if (ret == RET_OK)
  {
    ret = zb_bdb_finding_binding_target_func(endpoint, (zb_uint16_t)ZB_BDB().bdb_commissioning_time);
  }

  TRACE_MSG(TRACE_ZCL1, "< bdb_finding_binding_target ret %hd", (FMT__H, ret));

  return ret;
}

/* Starts EZ-Mode Finding and binding mechanism at the target's endpoint with time in parameter */
zb_ret_t zb_bdb_finding_binding_target_ext(zb_uint8_t endpoint, zb_uint16_t commissioning_time_secs)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> bdb_finding_binding_target_ext endpoint %hd commissioning_time_secs %d", (FMT__H_D, endpoint, commissioning_time_secs));

  /* Check arguments passed */
  if (!ZB_AF_IS_EP_REGISTERED(endpoint))
  {
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (commissioning_time_secs < ZB_BDBC_MIN_COMMISSIONING_TIME_S)
  {
    ret = RET_INVALID_PARAMETER_2;
  }

  if (ret == RET_OK)
  {
    ret = zb_bdb_finding_binding_target_func(endpoint, commissioning_time_secs);
  }

  TRACE_MSG(TRACE_ZCL1, "< bdb_finding_binding_target_ext ret 0x%lx", (FMT__L, ret));

  return ret;
}


void zb_bdb_finding_binding_target_cancel()
{
  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_finding_binding_target_cancel", (FMT__0));

  ZB_SCHEDULE_ALARM_CANCEL(zb_bdb_finding_binding_target_alarm, 0);
  zb_zcl_stop_identifying(BDB_COMM_CTX().ep);
  BDB_COMM_CTX().state = ZB_BDB_COMM_IDLE;
  ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
  TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
  ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;

  TRACE_MSG(TRACE_ZCL1, "< zb_bdb_finding_binding_target_cancel", (FMT__0));
}


void zb_bdb_finding_binding_target_alarm(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_finding_binding_target_alarm %hd", (FMT__H, param));
  ZVUNUSED(param);

  if (param == 0)
  {
    zb_buf_get_out_delayed(zb_bdb_finding_binding_target_alarm);
  }
  else
  {
    zb_bdb_finding_binding_target_cancel();

    TRACE_MSG(TRACE_ZDO1, "F&B target done calling zb_zdo_startup_complete", (FMT__0));
    zb_app_signal_pack(param, ZB_BDB_SIGNAL_FINDING_AND_BINDING_TARGET_FINISHED, RET_OK, 0);
    ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_finding_binding_target_alarm", (FMT__0));
}


/*
****************************************
F & B Initiator
*/

static void zb_bdb_finding_binding_initiator_stop(
  zb_uint8_t param, zb_uint16_t status)
{
  zb_zdo_signal_fb_initiator_finished_params_t *signal_params;

  BDB_COMM_CTX().state = ZB_BDB_COMM_IDLE;
  ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
  TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
  ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
  ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED;

  signal_params = (zb_zdo_signal_fb_initiator_finished_params_t *)zb_app_signal_pack(
    param,
    ZB_BDB_SIGNAL_FINDING_AND_BINDING_INITIATOR_FINISHED,
    (ZB_BDB_STATUS_SUCCESS == ZB_BDB().bdb_commissioning_status) ? RET_OK : RET_ERROR,
    0 /* data_size */);

  if (signal_params)
  {
    zb_buf_set_status(param, RET_OK);
    signal_params->status = (zb_zdo_fb_initiator_finished_status_t) status;
    ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
  }
}

void zb_bdb_finding_binding_initiator_cancel(void)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_finding_binding_initiator_cancel", (FMT__0));

  ZB_SCHEDULE_ALARM_CANCEL(zb_bdb_finding_binding_initiator_alarm, ZB_ALARM_ALL_CB);
  zb_buf_get_out_delayed_ext(zb_bdb_finding_binding_initiator_stop, ZB_ZDO_FB_INITIATOR_STATUS_CANCEL, 0);

  TRACE_MSG(TRACE_ZCL1, "< zb_bdb_finding_binding_initiator_cancel", (FMT__0));
}

/* Alarm when finding & binding is invoked but no identify query response got */
void zb_bdb_finding_binding_initiator_alarm(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_finding_binding_initiator_alarm %hd", (FMT__H, param));

  ZVUNUSED(param);

  if (ZB_ZDP_STATUS_SUCCESS == BDB_COMM_CTX().status)
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
    zb_buf_get_out_delayed_ext(zb_bdb_finding_binding_initiator_stop, ZB_ZDO_FB_INITIATOR_STATUS_SUCCESS, 0);
  }
  else if (ZB_ZDP_STATUS_TABLE_FULL == BDB_COMM_CTX().status)
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_BINDING_TABLE_FULL;
    zb_buf_get_out_delayed_ext(zb_bdb_finding_binding_initiator_stop, ZB_ZDO_FB_INITIATOR_STATUS_ERROR, 0);
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_IDENTIFY_QUERY_RESPONSE;
    zb_buf_get_out_delayed_ext(zb_bdb_finding_binding_initiator_stop, ZB_ZDO_FB_INITIATOR_STATUS_ALARM, 0);
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_finding_binding_initiator_alarm", (FMT__0));
}

/**
   Find record in the F&B respondents table.
   Return respondent or NULL if not found.
*/
static zb_ret_t zb_bdb_find_respondent(zb_uint16_t nwk_addr, zb_uint8_t *resp_index)
{
  zb_ret_t ret = RET_NOT_FOUND;
  zb_uindex_t i;
  zb_address_ieee_ref_t addr_ref = 0;

  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_find_respondent by addr %d", (FMT__D, nwk_addr));

  if (zb_address_by_short(nwk_addr, ZB_FALSE, ZB_FALSE, &addr_ref) == RET_OK)
  {
    for (i = 0 ; i < BDB_COMM_CTX().respondent_number ; i++)
    {
      if (BDB_COMM_CTX().respondent[i].addr_ref == addr_ref)
      {
        *resp_index = i;
        ret = RET_OK;
        TRACE_MSG(TRACE_ZCL2, "Found nwk_addr in the Respondents", (FMT__0));
        break;
      }
    }
  }
  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_find_respondent %d", (FMT__D, ret));
  return ret;
}

static zb_ret_t zb_bdb_find_respondent_by_bind_confirm_buf(zb_bufid_t confirm_buf,
                                                           zb_uint8_t *resp_index)
{
  zb_ret_t ret = RET_NOT_FOUND;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, ">> zb_bdb_find_respondent_by_bind_confirm_buf, buf %d",
            (FMT__D, confirm_buf));

  ZB_ASSERT(confirm_buf != ZB_BUF_INVALID);

  for (i = 0; i < BDB_COMM_CTX().respondent_number; i++)
  {
    if (BDB_COMM_CTX().respondent[i].curr_bind_req_buf == confirm_buf)
    {
      *resp_index = i;
      ret = RET_OK;
      TRACE_MSG(TRACE_ZCL2, "found confirm_buf in the respondents, index %d", (FMT__D, i));
      break;
    }
  }

  TRACE_MSG(TRACE_ZCL1, ">> zb_bdb_find_respondent_by_bind_confirm_buf, ret %d",
            (FMT__D, ret));
  return ret;
}

static void process_next_binding_for_respondent(zb_bdb_comm_respondent_info_t *respondent,
                                                zb_bufid_t param);


static void handle_bind_confirm(zb_uint8_t param)
{
  zb_apsme_binding_req_t *aps_bind_req = ZB_BUF_GET_PARAM(param, zb_apsme_binding_req_t);
  zb_bdb_comm_respondent_info_t *respondent = NULL;
  zb_uint8_t resp_idx;
  zb_ret_t status;

  TRACE_MSG(TRACE_ZCL2, ">> handle_bind_confirm, param %d", (FMT__D, param));

  if (zb_bdb_find_respondent_by_bind_confirm_buf(param, &resp_idx) == RET_OK)
  {
    respondent = &BDB_COMM_CTX().respondent[resp_idx];
  }

  ZB_ASSERT(respondent);

  if (!respondent)
  {
    TRACE_MSG(TRACE_ERROR, "got bind_confirm (param %d), but didn't send a request",
              (FMT__D, param));
  }
  else
  {
    respondent->curr_bind_req_buf = ZB_BUF_INVALID;

    status = zb_buf_get_status(param);

    if (status == RET_OK)
    {
      BDB_COMM_CTX().status = ZB_ZDP_STATUS_SUCCESS;
    }
    else
    {
      /* If status is not success set status ZB_ZDP_STATUS_TABLE_FULL even
       * for other retcodes and indicate with ZB_ZDO_FB_INITIATOR_STATUS_ERROR
       * by zb_bdb_finding_binding_initiator_alarm() */
      BDB_COMM_CTX().status = ZB_ZDP_STATUS_TABLE_FULL;
    }

    /* [AV]
       use binding progress cb to report about binding results;
       don't break and cry if only one bind request has failed - keep on processing */
    if (status == RET_OK)
    {
      TRACE_MSG(TRACE_ZCL2, "bind success", (FMT__0));
      BDB_COMM_CTX().was_bound = ZB_TRUE;
      status = ZB_BDB_COMM_BIND_SUCCESS;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL2, "bind fail status was %d", (FMT__D, status));
      status = ZB_BDB_COMM_BIND_FAIL;
    }

    {
      zb_ieee_addr_t resp_ext_addr;

      zb_address_ieee_by_ref(resp_ext_addr, respondent->addr_ref);
      if (BDB_COMM_CTX().finding_binding_progress_cb)
      {
        /* Report about binding results */
        BDB_COMM_CTX().finding_binding_progress_cb(
          status,
          resp_ext_addr,
          respondent->ep_list[respondent->eps_checked],
          aps_bind_req->clusterid);
      }
    }

    process_next_binding_for_respondent(respondent, param);
  }

  TRACE_MSG(TRACE_ZCL2, "<< handle_bind_confirm", (FMT__0));
}

static zb_ret_t bind_respondent_cluster(zb_bdb_comm_respondent_info_t *respondent,
                                    zb_uint8_t ep,
                                    zb_uint16_t cluster,
                                    zb_bufid_t param)
{
  zb_apsme_binding_req_t *aps_bind_req = ZB_BUF_GET_PARAM(param, zb_apsme_binding_req_t);
  zb_ret_t status;

  TRACE_MSG(TRACE_ZCL2, ">> bind_respondent_cluster, respondent %p, ep %d, cluster 0x%x, param %d",
            (FMT__P_D_D_D, respondent, ep, cluster, param));


  /* If finding_binding_progress_cb == NULL, we are Simple device and must
   * bound anything that stirs.
   * If we are still Simple device but have that collback, bind behavior is
   * under application control.
   */
  if (BDB_COMM_CTX().finding_binding_progress_cb != NULL)
  {
    zb_ieee_addr_t resp_ext_addr;
    TRACE_MSG(TRACE_ZCL2, "finding_binding_start_cb %p",
              (FMT__P, BDB_COMM_CTX().finding_binding_progress_cb));

    zb_address_ieee_by_ref(resp_ext_addr, respondent->addr_ref);
    /* Callback can return RET_IGNORE to skip bind to that cluster. */
    status = BDB_COMM_CTX().finding_binding_progress_cb(
      ZB_BDB_COMM_BIND_ASK_USER,
      resp_ext_addr,
      respondent->ep_list[respondent->eps_checked],
      cluster) == ZB_TRUE ? RET_OK : RET_IGNORE;
  }
  else
  {
    status = RET_OK;
  }

  /* [AV] Skip binding routines if bind was rejected by user */
  if (status == RET_IGNORE)
  {
    TRACE_MSG(TRACE_ZCL2, "bind rejected by the user", (FMT__0));
  }

  if (status == RET_OK)
  {
    ZB_BZERO(aps_bind_req, sizeof(zb_apsme_binding_req_t));

    zb_get_long_address(aps_bind_req->src_addr);
    aps_bind_req->src_endpoint = BDB_COMM_CTX().ep;
    aps_bind_req->clusterid = cluster;
    if (ZB_BDB().bdb_commissioning_group_id == 0xffff)
    {
      zb_address_ieee_by_ref(aps_bind_req->dst_addr.addr_long, respondent->addr_ref);
      aps_bind_req->addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
      aps_bind_req->dst_endpoint = respondent->ep_list[respondent->eps_checked];
      TRACE_MSG(TRACE_ZCL2, "F&B: Unicast bind src ep %hd cluster 0x%x dst " TRACE_FORMAT_64 " ep %hd",
                (FMT__H_D_A_H, aps_bind_req->src_endpoint, aps_bind_req->clusterid, TRACE_ARG_64(aps_bind_req->dst_addr.addr_long), aps_bind_req->dst_endpoint));
    }
    else
    {
      aps_bind_req->addr_mode = ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT;
      aps_bind_req->dst_addr.addr_short = ZB_BDB().bdb_commissioning_group_id;
      aps_bind_req->dst_endpoint = respondent->ep_list[respondent->eps_checked];
      TRACE_MSG(TRACE_ZCL2, "F&B: Group bind src ep %hd cluster 0x%x dst group %d",
                (FMT__H_D_D, aps_bind_req->src_endpoint, aps_bind_req->clusterid, aps_bind_req->dst_addr.addr_short));
    }

    respondent->curr_bind_req_buf = param;
    aps_bind_req->confirm_cb = handle_bind_confirm;
#ifdef SNCP_MODE
    aps_bind_req->remote_bind = ZB_B2U(ZB_TRUE);
#endif
    zb_apsme_bind_request(param);
  }

  TRACE_MSG(TRACE_ZCL2, "<< bind_respondent_cluster status %hd", (FMT__H, status));

  return status;
}

static void process_binding_for_respondent(zb_bdb_comm_respondent_info_t *respondent,
                                           zb_bufid_t param);

static void process_next_binding_for_respondent(zb_bdb_comm_respondent_info_t *respondent,
                                                zb_bufid_t param)
{
  respondent->curr_cluster_idx++;
  process_binding_for_respondent(respondent, param);
}

static void process_binding_for_respondent(zb_bdb_comm_respondent_info_t *respondent,
                                           zb_bufid_t param)
{
  zb_zdo_simple_desc_resp_t *resp = NULL;
  zb_uint8_t in_cluster_cnt;
  zb_uint8_t out_cluster_cnt;
  zb_ret_t bind_status = RET_OK;

  TRACE_MSG(TRACE_ZCL2, ">> process_binding_for_respondent, respondent %p, param %d",
            (FMT__P_D, respondent, param));

  ZB_ASSERT(respondent->simple_desc_resp_buf);
  resp = (zb_zdo_simple_desc_resp_t*)zb_buf_begin(respondent->simple_desc_resp_buf);
  in_cluster_cnt = resp->simple_desc.app_input_cluster_count;
  out_cluster_cnt = resp->simple_desc.app_output_cluster_count;
  TRACE_MSG(TRACE_ZCL3, "in cluster cnt %d out cluster cnt %d", (FMT__D_D, in_cluster_cnt, out_cluster_cnt));

  while (respondent->curr_cluster_idx < in_cluster_cnt + out_cluster_cnt)
  {
    zb_uint8_t role = (respondent->curr_cluster_idx >= in_cluster_cnt ?
           ZB_ZCL_CLUSTER_CLIENT_ROLE : ZB_ZCL_CLUSTER_SERVER_ROLE);

    if (zb_bdb_find_cluster_match(resp->simple_desc.app_cluster_list[respondent->curr_cluster_idx],
                                  role))
    {
      TRACE_MSG(TRACE_ZCL2, "found cluster match clusterid %d",
                (FMT__D, resp->simple_desc.app_cluster_list[respondent->curr_cluster_idx]));

      bind_status = bind_respondent_cluster(respondent,
                                            respondent->ep_list[respondent->eps_checked],
                                            resp->simple_desc.app_cluster_list[respondent->curr_cluster_idx],
                                            param);
      break;
    }
    else
    {
      respondent->curr_cluster_idx++;
    }
  }

  if (respondent->curr_cluster_idx == in_cluster_cnt + out_cluster_cnt)
  {
    TRACE_MSG(TRACE_ZCL3, "check simple desc finished", (FMT__0));
    if (BDB_COMM_CTX().was_bound
        && ZB_BDB().bdb_commissioning_group_id != 0xffff)
    {
      zb_uint16_t resp_short_addr;

      zb_address_short_by_ref(&resp_short_addr, respondent->addr_ref);
      TRACE_MSG(TRACE_ZCL3, "Sending ADD_GROUP_REQ", (FMT__0));
      ZB_ZCL_GROUPS_SEND_ADD_GROUP_REQ(
        param, resp_short_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        respondent->ep_list[respondent->eps_checked],
        BDB_COMM_CTX().ep, ZB_AF_HA_PROFILE_ID, ZB_ZCL_ENABLE_DEFAULT_RESPONSE, NULL,
        ZB_BDB().bdb_commissioning_group_id);
    }
    else
    {
      zb_buf_free(param);
    }

    respondent->eps_checked++;
    ZB_SCHEDULE_CALLBACK(zb_bdb_active_eps_check_cb, respondent->simple_desc_resp_buf);
    respondent->simple_desc_resp_buf = 0;
  }
  else
  {
    if (bind_status != RET_OK)
    {
      process_next_binding_for_respondent(respondent, param);
    }
  }

  TRACE_MSG(TRACE_ZCL2, "<< process_binding_for_respondent", (FMT__0));
}

/**
  Checks simple descriptor received from respondent and
  performs binding if cluster match is found
*/
void zb_bdb_simple_desc_check_cb(zb_uint8_t param, zb_uint16_t param2)
{
  zb_zdo_simple_desc_resp_t *resp = (zb_zdo_simple_desc_resp_t*)zb_buf_begin(param2);
  zb_bdb_comm_respondent_info_t *respondent = NULL;
  zb_uint8_t resp_idx;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_simple_desc_check_cb %hd %d", (FMT__H_D, param, param2));


  /*Find record in the F&B identify table */
  /* Check profile id matching for incoming simple_desc before processing it
   * (required by test FB-PRE-TC-03B)
   */
  if (zb_zcl_is_target_endpoint(BDB_COMM_CTX().ep_desc, resp->simple_desc.app_profile_id))
  {
    if (zb_bdb_find_respondent(resp->hdr.nwk_addr, &resp_idx) == RET_OK)
    {
      respondent = &BDB_COMM_CTX().respondent[resp_idx];
    }

    if (respondent)
    {
      respondent->curr_cluster_idx = 0;
      respondent->simple_desc_resp_buf = param2;
      param2 = 0;
      process_binding_for_respondent(respondent, param);
      param = 0;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL3, "Simple_desc ignored due to profile_id mismatch", (FMT__0));
    /* Try to skip this descriptor. */
    if (zb_bdb_find_respondent(resp->hdr.nwk_addr, &resp_idx) == RET_OK)
    {
      respondent = &BDB_COMM_CTX().respondent[resp_idx];
      respondent->eps_checked++;
      ZB_SCHEDULE_CALLBACK(zb_bdb_active_eps_check_cb, param2);
      param2 = 0;
    }
  }

  if (param)
  {
    zb_buf_free(param);
  }
  if (param2)
  {
    zb_buf_free(param2);
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_simple_desc_check_cb", (FMT__0));
}


/* Checks whether cluster matches on the initiator's endpoint */
zb_bool_t zb_bdb_find_cluster_match(zb_uint16_t cluster_id, zb_uint8_t role)
{
  zb_bool_t ret = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_find_cluster_match clusterId %d role %hd", (FMT__D_H, cluster_id, role));

  if (BDB_COMM_CTX().ep_desc!=NULL)
  {
    zb_uint8_t cluster_count = BDB_COMM_CTX().ep_desc->cluster_count;
    struct zb_zcl_cluster_desc_s *cluster_desc_list =
      BDB_COMM_CTX().ep_desc->cluster_desc_list;
    zb_int16_t i;

    TRACE_MSG(TRACE_ZCL3, "cluster_id %d local cluster count %hd", (FMT__D_H, cluster_id, cluster_count));

    for (i=0; i < cluster_count; i++)
    {
      TRACE_MSG(TRACE_ZCL3, "cluster_id[%d] %d role %hd role_mask %hd", (FMT__D_D_H_H, i, cluster_desc_list[i].cluster_id, role, cluster_desc_list[i].role_mask));
      if (cluster_id == cluster_desc_list[i].cluster_id)
      {
        if (role != cluster_desc_list[i].role_mask
            /* || role == ZB_ZCL_CLUSTER_MIXED_ROLE */
            /* || cluster_desc_list[i].role_mask == ZB_ZCL_CLUSTER_MIXED_ROLE */)
        {
          TRACE_MSG(TRACE_ZCL3, "Match!", (FMT__0));
          ret = ZB_TRUE;
          break;
        }
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_bdb_find_cluster_match ret %hd", (FMT__H, ret));
  return ret;
}

/**
  Processes simple descriptor response during EZ Mode commissioning
  (finding & binding)
*/
void zb_bdb_process_simple_desc_res(zb_uint8_t param)
{
  zb_int16_t status = RET_OK;

  zb_zdo_simple_desc_resp_t *resp = (zb_zdo_simple_desc_resp_t*)zb_buf_begin(param);
  zb_bdb_comm_respondent_info_t *respondent = NULL;
  zb_uint8_t resp_idx;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_process_simple_desc_res %hd", (FMT__H, param));

  status = ( (BDB_COMM_CTX().state == ZB_BDB_COMM_FINDING_AND_BINDING) ?
             RET_OK : RET_CANCELLED );

  if (status == RET_OK)
  {
    status = resp->hdr.status;
  }

  if (status == RET_OK)
  {
    zb_buf_get_out_delayed_ext(zb_bdb_simple_desc_check_cb, param, 0);
  }
  else
  {
    /* Error with this endpoint. Continue f&b process with further endpoints. */
    if (zb_bdb_find_respondent(resp->hdr.nwk_addr, &resp_idx) == RET_OK)
    {
      respondent = &BDB_COMM_CTX().respondent[resp_idx];
      respondent->eps_checked++;
      ZB_SCHEDULE_CALLBACK(zb_bdb_active_eps_check_cb, param);
    }
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_process_simple_desc_res", (FMT__0));
}

/*
  Sends SimpleDescriptor request for all the active endpoints
  of the respondent
*/
void zb_bdb_active_eps_check_cb(zb_uint8_t param)
{
  zb_bdb_comm_respondent_info_t *respondent = NULL;
  /* TRICKY HACK: in some cases there is zb_zdo_simple_desc_resp_t instead of zb_zdo_ep_resp_t.
     And code below works only because nwk_addr in zb_zdo_ep_resp_t has the same offset
     as hdr.nwk_addr in zb_zdo_simple_desc_resp_t... */
  zb_zdo_ep_resp_t *resp = (zb_zdo_ep_resp_t*)zb_buf_begin(param);
  zb_uint8_t resp_idx;
  zb_uindex_t respondents_to_move;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_process_active_ep_check_cb %hd", (FMT__H, param));

  /*Find record in the F&B identify table by address */
  if (zb_bdb_find_respondent(resp->nwk_addr, &resp_idx) == RET_OK)
  {
    respondent = &BDB_COMM_CTX().respondent[resp_idx];
  }

  if (respondent)
  {
    if (respondent->eps_checked < respondent->ep_cnt)
    {
      zb_zdo_simple_desc_req_t * req;
      zb_uint8_t ep = respondent->ep_list[respondent->eps_checked];
      zb_uint16_t resp_short_addr;

      TRACE_MSG(TRACE_ZCL3, "processing ep %hd", (FMT__H, ep));

      req = zb_buf_initial_alloc(param, sizeof(zb_zdo_simple_desc_req_t));
      ZB_BZERO(req, sizeof(zb_zdo_simple_desc_req_t));

      zb_address_short_by_ref(&resp_short_addr, respondent->addr_ref);
      req->nwk_addr = resp_short_addr;
      req->endpoint = ep;
      if (zb_zdo_simple_desc_req(param, zb_bdb_process_simple_desc_res) == ZB_ZDO_INVALID_TSN)
      {
        zb_bdb_error_handle_buf(ZB_ZDP_STATUS_INSUFFICIENT_SPACE, param);
      }

      param = 0;
    }
    else
    {
      respondent->addr_ref = 0xFF;

      if (resp_idx < ZB_ARRAY_SIZE(BDB_COMM_CTX().respondent) - 1)
      {
        respondents_to_move = ZB_ARRAY_SIZE(BDB_COMM_CTX().respondent) - resp_idx - 1;

        ZB_MEMMOVE(&BDB_COMM_CTX().respondent[resp_idx],
                   &BDB_COMM_CTX().respondent[resp_idx + 1],
                   sizeof(BDB_COMM_CTX().respondent[0]) * respondents_to_move);
      }

      BDB_COMM_CTX().respondent_number--;

      if (0 == BDB_COMM_CTX().respondent_number)
      {
        /* [AV] User already knows, whether binding was performed or not. Just report f&b finished OK
           Now it's the only point where EZ-Mode finishes successfully */
        TRACE_MSG(TRACE_ZCL1, "All endpoints are checked. Finding and binding finished", (FMT__0));

        if (BDB_COMM_CTX().status == ZB_ZDP_STATUS_TABLE_FULL)
        {
          TRACE_MSG(TRACE_ZCL1, "Binding table is full", (FMT__0));
          ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_BINDING_TABLE_FULL;
        }
        else
        {
          ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
        }
      }
    }
  } /* if (respondent) */

  if (param)
  {
    zb_buf_free(param);
  }


  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_process_active_ep_check_cb", (FMT__0));
}

/*
 add ep to respondent and go on
*/
static void zb_bdb_add_ep_to_respondent(zb_uint8_t param, zb_uint16_t nwk_addr, zb_uint8_t ep)
{
  zb_ret_t status = RET_OK;
  zb_zdo_ep_resp_t *resp;
  zb_bdb_comm_respondent_info_t *respondent=NULL;
  zb_uint8_t resp_idx = 0xff;
  zb_bool_t is_new_respondent = ZB_TRUE;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_add_ep_to_respondent addr %d ep %hd", (FMT__D_H, nwk_addr, ep));
  status = ( (BDB_COMM_CTX().state == ZB_BDB_COMM_FINDING_AND_BINDING) ?
  RET_OK : RET_CANCELLED );
  if(status== RET_OK)
  {
    if (zb_bdb_find_respondent(nwk_addr, &resp_idx) != RET_OK)
    {
      if (BDB_COMM_CTX().respondent_number < ZB_ARRAY_SIZE(BDB_COMM_CTX().respondent))
      {
        resp_idx = BDB_COMM_CTX().respondent_number;
        TRACE_MSG(TRACE_ZCL1, "add new respondent 0x%x (idx %hd)", (FMT__D_H, nwk_addr, resp_idx));
        BDB_COMM_CTX().respondent_number++;
      }
      else
      {
        TRACE_MSG(TRACE_ZCL1, "Out of space for Identify Query resp devices", (FMT__0));
      }
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "respondent 0x%x (idx %hd) is already in the list, continue...",
                (FMT__D_H, nwk_addr, resp_idx));
      is_new_respondent = ZB_FALSE;
    }
  }
  if (resp_idx != 0xff)
  {
    zb_address_ieee_ref_t addr_ref;

    respondent = &BDB_COMM_CTX().respondent[resp_idx];

    if (is_new_respondent)
    {
      status = zb_address_by_short(nwk_addr, ZB_FALSE, ZB_FALSE, &addr_ref);

      if (status == RET_OK)
      {
        respondent->addr_ref = addr_ref;
        respondent->ep_cnt = 0;
        respondent->eps_checked = 0;
      }
    }

    if (status == RET_OK)
    {
      if (respondent->ep_cnt < ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN)
      {
        respondent->ep_list[respondent->ep_cnt] = ep;
        ++(respondent->ep_cnt);
      }
      else
      {
        TRACE_MSG(TRACE_ERROR, "Too many active endpoints for EZ-Mode to serve %hd, drop this ep",
                  (FMT__H, respondent->ep_cnt));
      }
    }

    if (status == RET_OK && is_new_respondent)
    {
      resp = zb_buf_initial_alloc(param, sizeof(zb_zdo_ep_resp_t));
      ZB_BZERO(resp, sizeof(zb_zdo_ep_resp_t));
      resp->nwk_addr = nwk_addr;
      resp->status = 0;
      resp->ep_count = respondent->ep_cnt;
      TRACE_MSG(TRACE_ERROR, "start zb_bdb_active_eps_check_cb", (FMT__0));
      ZB_SCHEDULE_CALLBACK(zb_bdb_active_eps_check_cb, param);
    }
    else if (status == RET_OK)
    {
      /* Do not need to start anything or indicate the error, free the buf */
      zb_buf_free(param);
    }
    else
    {
      /*zb_bdb_error_handle_param(status, param);*/
      zb_bdb_error_handle_buf(status, param);
    }
  }
  else
  {
    /*zb_bdb_error_handle_param(status, param);*/
    zb_bdb_error_handle_buf(status, param);
  }
  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_add_ep_to_respondent", (FMT__0));
}

/*
  Processes active endpoints response during finding & binding
*/
void zb_bdb_process_active_eps_res(zb_uint8_t param)
{
  zb_int16_t status = RET_OK;
  zb_zdo_ep_resp_t *resp = (zb_zdo_ep_resp_t*)zb_buf_begin(param);
  zb_uint8_t *ep_list = (zb_uint8_t *)resp + sizeof(zb_zdo_ep_resp_t);
  zb_bdb_comm_respondent_info_t *respondent=NULL;
  zb_uint8_t resp_idx;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_process_active_eps_res from addr %d %hd", (FMT__D_H, resp->nwk_addr, param));


  status = ( (BDB_COMM_CTX().state == ZB_BDB_COMM_FINDING_AND_BINDING) ?
             RET_OK : RET_CANCELLED );

  /*Find record in the F&B identify table */
  if ((status == RET_OK) && (zb_bdb_find_respondent(resp->nwk_addr, &resp_idx) == RET_OK))
  {
    zb_uindex_t i = 0;
    zb_uint16_t resp_short_addr;
    respondent = &BDB_COMM_CTX().respondent[resp_idx];
    zb_address_short_by_ref(&resp_short_addr, respondent->addr_ref);
    TRACE_MSG(TRACE_ZCL3, "Active endpoints response got from addr %d ep_cnt %hd",
              (FMT__H_D, resp_short_addr, resp->ep_count));

    respondent->ep_cnt = resp->ep_count;
    respondent->eps_checked = 0;

    if (respondent->ep_cnt > ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN)
    {
      TRACE_MSG(TRACE_ERROR, "Too many active endpoints for EZ-Mode to serve %hd",
                (FMT__H, respondent->ep_cnt));
      TRACE_MSG(TRACE_ERROR, "Serve first %hd endpoints",
                (FMT__H, ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN));
      respondent->ep_cnt = ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN;
    }
    /* Save active endpoints list to context to reuse buffer */
    for (; i < respondent->ep_cnt; i++)
    {
      respondent->ep_list[i] = ep_list[i];
    }
    ZB_SCHEDULE_CALLBACK(zb_bdb_active_eps_check_cb, param);
  }
  else
  {
    /*zb_bdb_error_handle_param(status, param);*/
    zb_bdb_error_handle_buf(status, param);
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_process_active_eps_res", (FMT__0));
}

/*
  Processes identify query response during EZ Mode commissioning
  (finding & binding)
*/
void zb_bdb_process_identify_query_res(zb_uint8_t param)
{
  zb_int16_t status = RET_OK;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_process_identify_query_res %hd", (FMT__H, param));

  status = ( (BDB_COMM_CTX().state == ZB_BDB_COMM_FINDING_AND_BINDING) ?
             RET_OK : RET_CANCELLED );

  /* It is not data confirm, so seems like we should not check u.hdr.status.
     TODO: Check other cases - sometimes we are in data confirm, sometimes - in data indication! */

  if (status == RET_OK)
  {
    zb_ieee_addr_t ext_addr;

    if (BDB_COMM_CTX().status == RET_NOT_FOUND)
    {
      BDB_COMM_CTX().status = RET_OK;
    }
    ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);
    TRACE_MSG(TRACE_ZCL2, "Identify query response got from addr 0x%x",
              (FMT__D, cmd_info.addr_data.common_data.source.u.short_addr));

    if (zb_address_ieee_by_short(cmd_info.addr_data.common_data.source.u.short_addr, ext_addr) != RET_OK
#ifdef ZB_CERTIFICATION_HACKS
        /* We probably know ext address, but tests like FB-PRE-TC-01A require ieee_addr_req */
        || ZB_CERT_HACKS().force_ext_addr_req
#endif
      )
    {
      /* All we need in this buffer is respondent_addr which we know
       * already. So reuse it for addr_req.  */
      zb_zdo_ieee_addr_req_param_t *req;
      req = ZB_BUF_GET_PARAM(param, zb_zdo_ieee_addr_req_param_t);
      req->nwk_addr = cmd_info.addr_data.common_data.source.u.short_addr;
      req->dst_addr = req->nwk_addr;
      req->start_index = 0;
      req->request_type = 0; /* 0x00  Single device response  */
      TRACE_MSG(TRACE_ZCL2, "Have no ext address for short 0x%x. Sending ieee_addr_req.",
                (FMT__D, cmd_info.addr_data.common_data.source.u.short_addr));
      if (zb_zdo_ieee_addr_req(param, zb_bdb_process_ext_addr_resp) == ZB_ZDO_INVALID_TSN)
      {
        zb_bdb_error_handle_buf(ZB_ZDP_STATUS_INSUFFICIENT_SPACE, param);
      }
    }
    else
    {
#ifdef USE_ACTIVE_EP_REQ
      zb_bdb_fb_send_active_ep_req(param, cmd_info.addr_data.common_data.source.u.short_addr);
#else
      zb_bdb_add_ep_to_respondent(param, cmd_info.addr_data.common_data.source.u.short_addr, cmd_info.addr_data.common_data.src_endpoint);
#endif
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "Device is not in F&B state, so skip F&B resp, current_state 0x%hx", (FMT__H, BDB_COMM_CTX().state));

    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_process_identify_query_res", (FMT__0));
}


void zb_bdb_fb_send_active_ep_req(zb_uint8_t param, zb_uint16_t src_addr)
{
  zb_bdb_comm_respondent_info_t *respondent = NULL;
  zb_uint8_t resp_idx = 0xff;

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_fb_send_active_ep_req param %hd src_addr 0x%x", (FMT__H_D, param, src_addr));
  if (zb_bdb_find_respondent(src_addr, &resp_idx) != RET_OK)
  {
    if (BDB_COMM_CTX().respondent_number < ZB_ARRAY_SIZE(BDB_COMM_CTX().respondent))
    {
      resp_idx = BDB_COMM_CTX().respondent_number;
      BDB_COMM_CTX().respondent_number++;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Out of space for Identify Query resp devices", (FMT__0));
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "F&B is already busy dealing with remote %x. Drop this Identify Req Resp",
              (FMT__D, src_addr));
    resp_idx = 0xff;
  }
  if (resp_idx != 0xff)
  {
    zb_zdo_active_ep_req_t *req;
    zb_address_ieee_ref_t addr_ref = 0;
    respondent = &BDB_COMM_CTX().respondent[resp_idx];

    if (zb_address_by_short(src_addr, ZB_FALSE, ZB_FALSE, &addr_ref) == RET_OK)
    {
      respondent->addr_ref = addr_ref;
      req = zb_buf_initial_alloc(param, sizeof(zb_zdo_active_ep_req_t));
      ZB_BZERO(req, sizeof(zb_zdo_active_ep_req_t));
      req->nwk_addr = src_addr;
      TRACE_MSG(TRACE_ZCL3, "F&B sending active ep req to 0x%x", (FMT__D, src_addr));
      if (zb_zdo_active_ep_req(param, zb_bdb_process_active_eps_res) == ZB_ZDO_INVALID_TSN)
      {
        zb_bdb_error_handle_buf(ZB_ZDP_STATUS_INSUFFICIENT_SPACE, param);
      }
      param = 0;
    }
  }

  if (param)
  {
    zb_buf_free(param);
  }
  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_fb_send_active_ep_req", (FMT__0));
}


void zb_bdb_process_ext_addr_resp(zb_uint8_t param)
{
  zb_address_ieee_ref_t addr_ref;
  zb_zdo_nwk_addr_resp_head_t *resp = (zb_zdo_nwk_addr_resp_head_t*)zb_buf_begin(param);

  if (zb_buf_get_status(param) == RET_OK)
  {
    TRACE_MSG(TRACE_ZCL1, "Got long address " TRACE_FORMAT_64 " for 0x%x",
              (FMT__A_D, TRACE_ARG_64(resp->ieee_addr), resp->nwk_addr));
    zb_address_update(resp->ieee_addr, resp->nwk_addr, ZB_FALSE, &addr_ref);
    zb_bdb_fb_send_active_ep_req(param, resp->nwk_addr);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Failed to get long address for 0x%x",
              (FMT__D, resp->nwk_addr));
    zb_buf_free(param);
  }
}


/*
  Checks whether identify_query request
  was sent successfully
*/
void zb_bdb_identify_query_sent_cb(zb_uint8_t param)
{

  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_identify_query_sent_cb %hd", (FMT__H, param));

  if (zb_buf_get_status(param) == RET_OK)
  {
//[AM] Do not send Identify Query multiply times. 8.6 Finding & binding procedure for an initiator endpoint.
    zb_buf_free(param);
  }
  else
  {
    /* Report error - can't send command */
    zb_bdb_error_handle_buf(zb_buf_get_status(param), param);
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_identify_query_sent_cb", (FMT__0));
}

/**
Repeat Identify query
*/

/**
  Sends Identify Query request during
  EZ-Mode finding & binding initiator
*/
void zb_bdb_send_identify_query_req(zb_uint8_t param)
{
  zb_addr_u addr;
  TRACE_MSG(TRACE_ZCL2, "> zb_bdb_identify_query_req %hd", (FMT__H, param));

  addr.addr_short = ZB_NWK_BROADCAST_ALL_DEVICES;
  ZB_ZCL_IDENTIFY_SEND_IDENTIFY_QUERY_REQ
    (param, addr,
     ZB_APS_ADDR_MODE_16_ENDP_PRESENT/*ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT*/,
     ZB_ZCL_BROADCAST_ENDPOINT, BDB_COMM_CTX().ep, get_profile_id_by_endpoint(BDB_COMM_CTX().ep),
     ZB_ZCL_DISABLE_DEFAULT_RESPONSE, /* don't need ZCL_STATUS_FAIL in response */
     zb_bdb_identify_query_sent_cb);
  /*
    wait for response for ZB_NWK_BROADCAST_DELIVERY_TIME()
  */
  ZB_SCHEDULE_ALARM(zb_bdb_finding_binding_initiator_alarm, 0, ZB_NWK_EXPIRY_BROADCAST * ZB_TIME_ONE_SECOND);

  TRACE_MSG(TRACE_ZCL2, "< zb_bdb_identify_query_req", (FMT__0));
}


/**
  Starts EZ-Mode Finding and binding mechanism at the initiator's endpoint

  Summary: Finding and binding as initiator zb_bdb_finding_binding_initiator()
  returns RET_OK if procedure was started successfully, error code otherwise. To report procedure
  status, user callback is called. It may be called several times with Success status and only once
  with error status. If any error appears, finding and binding stops.

  Finding and binding algorithm:
  1. Check that operation may be started and tx command Identify Query zb_bdb_send_identify_query_req()
  Error status is reported with finding_binding_cb()
  2. After command is send, start 180 sec timer zb_bdb_finding_binding_initiator_alarm()
  3*. If timeout fires, call user finding_binding_cb with RET_TIMEOUT status
  4. If Identify query response is received, request active endpoints from the responder
  (zb_zdo_active_ep_req(), callback zb_bdb_process_active_eps_res()).
  Error status is reported with finding_binding_cb()
  Note: the 1st responder device is treated as "target device", others are ignored
  5. On receiving active EP list, store it and start this list analysis: zb_bdb_active_eps_check_cb()
  Error status is reported with finding_binding_cb()
  6*. Go through each EP and request simple descriptor for it. If no more EP - call user callback
  finding_binding_cb with RET_NO_BOUND_DEVICE status
  7. On receiving simple descriptor response, zb_bdb_process_simple_desc_res() calls
  zb_bdb_simple_desc_check_cb() to handle response.
  Error status is reported with finding_binding_cb()
  8. zb_bdb_simple_desc_check_cb() checks clusters and performs binding.
  Use user callback to ask if binding is needed or not.
  Status (error or success) is reported with finding_binding_cb()
  9. After binding is finished, go to p.6, call zb_bdb_active_eps_check_cb() to
  check another EP


 */
zb_ret_t zb_bdb_finding_binding_initiator(zb_uint8_t endpoint,
                                          zb_bdb_comm_binding_callback_t user_binding_cb)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_finding_binding_initiator endpoint %d", (FMT__D, endpoint));

  /* Check endpoint is registered */
  BDB_COMM_CTX().ep_desc = zb_af_get_endpoint_desc(endpoint);

  /* Check endpoint is registered */
  ret = ( BDB_COMM_CTX().ep_desc ? RET_OK : RET_INVALID_PARAMETER_1 );

  /* Check NWK steering complete */
  if (ret == RET_OK)
  {
    ret = ( zb_zdo_joined() ?
            RET_OK : RET_INVALID_STATE);
  }

  /* Check commissioning is not in progress */
  if (ret == RET_OK)
  {
    ret = ( (BDB_COMM_CTX().state == ZB_BDB_COMM_IDLE &&
             ZB_BDB().bdb_commissioning_step == ZB_BDB_COMMISSIONING_STOP) ?
            RET_OK : RET_BUSY );
  }

  if (ret == RET_OK)
  {
    ZB_BDB().bdb_commissioning_step = ZB_BDB_FINDING_N_BINDING;
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;

    BDB_COMM_CTX().state = ZB_BDB_COMM_FINDING_AND_BINDING;
    BDB_COMM_CTX().status = RET_NOT_FOUND;
    BDB_COMM_CTX().ep = endpoint;
    /* [AV]
        No need if f&b finished specific callback: store it in user_cb field */
    BDB_COMM_CTX().finding_binding_progress_cb = user_binding_cb;
    BDB_COMM_CTX().respondent_number = 0;
    ZB_BZERO(BDB_COMM_CTX().respondent, sizeof(zb_bdb_comm_respondent_info_t) * BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER);
    BDB_COMM_CTX().was_bound = ZB_FALSE;

    zb_buf_get_out_delayed(zb_bdb_send_identify_query_req);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_bdb_finding_binding_initiator ret %hd", (FMT__H, ret));

  return ret;
}


/* Pushes error message up via user callback in buffer's header */
void zb_bdb_error_handle_buf(zb_int16_t status, zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_bdb_error_handle_buf status %d, param %hd", (FMT__D_H, status, param));

  zb_buf_set_status(param, status);
  if (status == ZB_ZDP_STATUS_TABLE_FULL)
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_BINDING_TABLE_FULL;
  }
  else if (status == RET_OK)
  {
    TRACE_MSG(TRACE_ZCL1, "F&B finished ok", (FMT__0));
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_IDENTIFY_QUERY_RESPONSE;
  }

  if (ZB_BDB_COMM_FINDING_AND_BINDING_TARGET == BDB_COMM_CTX().state)
  {
    zb_app_signal_pack(param,
                       ZB_BDB_SIGNAL_FINDING_AND_BINDING_TARGET_FINISHED,
                       ZB_BDB().bdb_commissioning_status, 0);
  }

  BDB_COMM_CTX().state = ZB_BDB_COMM_IDLE;
  TRACE_MSG(TRACE_ZDO1, "F&B done, status %d, app signal %d. Calling zb_zdo_startup_complete_int status %hd",
            (FMT__D_D_H, ZB_BDB().bdb_commissioning_status, ZB_BDB().bdb_application_signal, zb_buf_get_status(param)));

  /* bdb commissioning is finished */
  ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
  TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));

  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);

  TRACE_MSG(TRACE_ZCL1, "< zb_bdb_error_handle_buf", (FMT__0));
}

#endif  /* defined(ZB_BDB_ENABLE_FINDING_BINDING) */
