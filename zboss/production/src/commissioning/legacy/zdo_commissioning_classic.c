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
/*  PURPOSE: Classic ZB commissioning: defined in Core specification and HA
*/


#define ZB_TRACE_FILE_ID 67
#include "zb_common.h"

#ifdef ZB_COMMISSIONING_CLASSIC_SUPPORT
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_nwk_nib.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zb_secur.h"
#include "zb_secur_api.h"
#include "zb_nvram.h"
#include "zb_bdb_internal.h"


static void zdo_classic_commissioning_force_link(void);

#ifdef ZB_FORMATION
static void zdo_classic_formation_force_link(void);
#endif /* ZB_FORMATION */

#ifdef ZB_JOIN_CLIENT
static void zdo_restart_association(zb_uint8_t param);
#endif /* ZB_JOIN_CLIENT */


void zdo_classic_initiate_commissioning(zb_uint8_t param)
{
  zb_ext_pan_id_t use_ext_pan_id;

  TRACE_MSG(TRACE_ZDO1, "zdo_classic_initiate_commissioning param %hd device type %d",
            (FMT__H_D_D, param, zb_get_device_type()));

  ZB_ASSERT(zb_get_device_type() != ZB_NWK_DEVICE_TYPE_NONE);

  zb_get_use_extended_pan_id(use_ext_pan_id);

  /* a. Maybe Formation */
  /*cstat -MISRAC2012-Rule-2.1_b*/
  if (ZB_IS_DEVICE_ZC()
#if defined ZB_ROUTER_ROLE && defined ZB_DISTRIBUTED_SECURITY_ON
      /* Not joined ZR with TC addr -1 means: start Distributed formation.*/
      || (zb_tc_is_distributed()
          && ZB_IS_DEVICE_ZR()
          && !zb_zdo_joined())
#endif
    )
  {
    /** @mdr{00012,44} */
    /*cstat +MISRAC2012-Rule-2.1_b*/
#if defined ZB_FORMATION
    ZB_ASSERT(FORMATION_SELECTOR().start_formation);
    ZB_SCHEDULE_CALLBACK(FORMATION_SELECTOR().start_formation, param);
#else
    TRACE_MSG(TRACE_MAC1, "Coordinator role is not supported", (FMT__0));
    zb_buf_free(param);
#endif
  }
  /* Rejoin after reboot? */
#ifdef ZB_JOIN_CLIENT
  else if (!ZB_EXTPANID_IS_ZERO(ZB_NIB_EXT_PAN_ID()))
  {
    /* try to rejoin the current network:
       use apsChannelMask as channel mask and do unsecure rejoin
       if we are not authenticated.
    */
    (void)zdo_initiate_rejoin(param, ZB_NIB_EXT_PAN_ID(),
                              ZB_AIB().aps_channel_mask_list,
                              zb_zdo_authenticated());
  }
  /* Rejoin to the net defined by aps_use_extended_pan_id */
  else if (!ZB_EXTPANID_IS_ZERO(use_ext_pan_id))
  {
    /* If the device is not the designated coordinator and apsUseExtendedPANID
       has a non-zero value, the device should attempt to rejoin the network
       specified in apsUseExtendedPANID. To do this, it should use NLME-JOIN.request
       with the ExtendedPANID parameter equal to the value of apsUseExtendedPANID, the
       ScanChannels parameter of the primitive equal to the value of the apsChannelMask
       configuration attribute. The RejoinNetwork parameter of the NLME-JOIN.request
       primitive should have a value of 0x02 indicating rejoin. */
    (void)zdo_initiate_rejoin(param, use_ext_pan_id,
                              ZB_AIB().aps_channel_mask_list,
                              ZB_FALSE);
  }
  else
  {
    /* ZR: discovery, then join */
    zb_nlme_network_discovery_request_t *req = ZB_BUF_GET_PARAM(param, zb_nlme_network_discovery_request_t);

    zb_channel_page_list_copy(req->scan_channels_list, ZB_AIB().aps_channel_mask_list);
    req->scan_duration = ZB_DEFAULT_SCAN_DURATION; /* TODO: configure it somehow? */
    TRACE_MSG(TRACE_APS1, "discovery, then join by association, scan attempts %hd", (FMT__H, COMM_CTX().discovery_ctx.nwk_scan_attempts));

    COMM_CTX().discovery_ctx.disc_count = COMM_CTX().discovery_ctx.nwk_scan_attempts;
    ZB_SCHEDULE_CALLBACK(zb_nlme_network_discovery_request, param);
  }
#endif  /* ZB_JOIN_CLIENT */
}

#ifdef ZB_JOIN_CLIENT
static void zdo_restart_association(zb_uint8_t param)
{
  zb_nlme_network_discovery_request_t *req = ZB_BUF_GET_PARAM(param, zb_nlme_network_discovery_request_t);
  TRACE_MSG(TRACE_ERROR, "rejoin failed - try association", (FMT__0));

  ZB_ASSERT(zb_get_device_type() != ZB_NWK_DEVICE_TYPE_NONE);

  TRACE_MSG(TRACE_ZDO1, "Dev type %hd", (FMT__H, zb_get_device_type()));
  ZG->zdo.handle.rejoin = ZB_FALSE;

  zb_channel_page_list_copy(req->scan_channels_list, ZB_AIB().aps_channel_mask_list);
  req->scan_duration = ZB_DEFAULT_SCAN_DURATION; /* TODO: configure it somehow? */
  COMM_CTX().discovery_ctx.disc_count = COMM_CTX().discovery_ctx.nwk_scan_attempts;
  ZB_SCHEDULE_CALLBACK(zb_nlme_network_discovery_request, param);
}


static void zdo_classic_handle_nwk_disc_failed_signal(zb_bufid_t param)
{
  TRACE_MSG(TRACE_ZDO2, "Calling zb_zdo_startup_complete_int to indicate nwk discovery failure",
            (FMT__0));
  (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, (zb_int16_t)ZB_NWK_STATUS_NOT_PERMITTED, 0U);
  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
}


static void zdo_classic_handle_join_failed_signal(zb_bufid_t param)
{
  if (ZB_U2B(ZG->zdo.handle.rejoin) && ZB_U2B(ZB_AIB().aps_insecure_join) && !ZB_U2B(ZB_AIB().always_rejoin))
  {
    /* FIXME: Do we really need to mix rejoin - Association - rejoin - Association - ... ?
       Also - if we dropped to association, looks like we need to erase network data before. It
       is really strange case when we do unsuccessful association attempt (no more children
       available), and then successful rejoin attempt to the same network.
    */
    TRACE_MSG(TRACE_ZDO1, "rejoin failed - try nwk search & association", (FMT__0));
    ZB_SCHEDULE_CALLBACK(zdo_restart_association, param);
  }
  else
  {
    if (ZB_U2B(COMM_CTX().discovery_ctx.scanlist_ref))
    {
      zb_buf_free(param);
      TRACE_MSG(TRACE_ZDO1, "{re}join failed", (FMT__0));
      ZB_SCHEDULE_CALLBACK(zdo_retry_joining, 0);
    }
    else
    {
      TRACE_MSG(TRACE_ZDO1, "{re}join totally failed - inform user", (FMT__0));
      (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, (zb_int16_t)ZB_NWK_STATUS_NOT_PERMITTED, 0U);
      ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
    }
  }
}


static void zdo_classic_handle_authentication_failed_signal(zb_bufid_t param)
{
/*
  2.1 Standard Security Mode

  If the joining device
  did not receive the key via the APSME-TRANSPORT-KEY.indication within the
  apsSecurityTimeOutPeriod since receiving the NLME-JOIN.confirm primitive, it
  shall reset and may choose to start the joining procedure again.

  In

  11.29 TP/SEC/BV-29 -I Security NWK Remove II-ZR
  Objective: DUT as ZR removes
  (DUT ZED2 performs silent leave).

  So it is unclear: do we need to just leave or join again?
  Let's leave to satisfy TC.
*/
/* NK: If we are in rejoin procedure, do "silent" rejoin, else - do "silent" leave */
  if (ZG->zdo.handle.rejoin
#ifdef ZB_CERTIFICATION_HACKS
      && !ZB_CERT_HACKS().disable_cyclic_tc_rejoin
#endif
    )
  {
#ifdef ZB_REJOIN_BACKOFF
    if (zb_zdo_rejoin_backoff_is_running())
    {
      /* Go to new rejoin backoff iteration */
      zb_zdo_rejoin_backoff_iteration_done();
      ZB_SCHEDULE_CALLBACK(zb_zdo_rejoin_backoff_continue, 0);
      zb_buf_free(param);
    }
    else
#endif
    {
      zb_ext_pan_id_t ext_pan_id;
      zb_get_extended_pan_id(ext_pan_id);
      /* force unsecure (AKA Trust Center) rejoin to current pan */
      (void)zdo_initiate_rejoin(param, ext_pan_id,
                                ZB_AIB().aps_channel_mask_list, ZB_FALSE);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "zdo_authentication_failed - Leave", (FMT__0));
#ifndef ZB_MULTILEAVE_HACK
    zdo_commissioning_leave(param, ZB_FALSE, ZB_FALSE);
    ZB_ZDO_SET_UNAUTH();      /* Call unauth_delayed after leave */
#else
    ZB_SET_JOINED_STATUS(ZB_FALSE);
    zb_buf_free(param);
    (void)zb_buf_get_out_delayed_ext(zb_zdo_startup_complete_int_delayed, ZB_NWK_STATUS_NO_KEY);
#endif
  }
}


static void zdo_classic_handle_initiate_rejoin_signal(zb_bufid_t param)
{
  zb_ext_pan_id_t ext_pan_id;

#ifdef ZB_NWK_BLACKLIST
  zb_nwk_blacklist_reset();
#endif
  zb_get_extended_pan_id(ext_pan_id);
  (void)zdo_initiate_rejoin(param, ext_pan_id,
                            ZB_AIB().aps_channel_mask_list, zb_zdo_authenticated());
}


static void zdo_classic_handle_dev_annce_sent_signal(zb_bufid_t param)
{
  zb_aps_device_key_pair_set_t *key_pair =
    zb_secur_get_link_key_by_address(ZB_AIB().trust_center_address, ZB_SECUR_ANY_KEY_ATTR);

  /* TODO: Implement me correctly!!! */
  if ( (ZB_NIB_SECURITY_LEVEL() == 0U) ||
       ((key_pair != NULL)
        && ZB_U2B(ZG->zdo.handle.rejoin)))
  {
    /* When joins to unsecured network and after secured rejoin - indicate startup immediately;
     * suppose that after secured rejoin we have VERIFIED key (otherwise device will try to obtain new link key with TC). */

    /* TODO: This reboot is also not so well defined  */
    (void)zb_app_signal_pack(param, ZB_SIGNAL_DEVICE_REBOOT, (zb_int16_t)zb_buf_get_status(param), 0U);
    ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
  }
  else
  {
    zb_buf_free(param);
  }
}


static void zdo_classic_handle_router_started_signal(zb_bufid_t param)
{
  zb_bool_t is_waiting = zdo_secur_waiting_for_tclk_update();

  if (ZB_NIB_SECURITY_LEVEL() == 0U
      || !is_waiting
      || ZB_IS_DEVICE_ZC())
  {
    TRACE_MSG(TRACE_ZDO1, "calling zdo_startup_complete", (FMT__0));
    (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, (zb_int16_t)zb_buf_get_status(param), 0U);
    /* Router start is last step in join. */
    ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
  }
  else
  {
    zb_buf_free(param);
  }
}


static void zdo_classic_handle_tclk_update_complete_signal(zb_bufid_t param)
{
  (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, (zb_int16_t)RET_OK, 0U);
  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
}


static void zdo_classic_handle_leave_done_signal(zb_bufid_t param)
{
  if (ZB_U2B(ZB_ZDO_GET_UNAUTH()))
  {
    ZB_ZDO_CLEAR_UNAUTH();
    if (param != ZB_BUF_INVALID)
    {
      ZB_SCHEDULE_CALLBACK2(zb_zdo_startup_complete_int_delayed, param, ZB_NWK_STATUS_NO_KEY);
    }
    else
    {
      (void)zb_buf_get_out_delayed_ext(zb_zdo_startup_complete_int_delayed, ZB_NWK_STATUS_NO_KEY, 0);
    }
  }
  else
  {
    if (param != ZB_BUF_INVALID)
    {
      zb_buf_free(param);
    }
  }
}


static void zdo_classic_handle_auth_ok_signal(zb_bufid_t param)
{
  if (!IS_DISTRIBUTED_SECURITY() && ZB_TCPOL().update_trust_center_link_keys_required)
  {
    /* Quite unsure, but... maybe, use same logic as for NCP and do not re-establish TCLK? */
    TRACE_MSG(TRACE_SECUR3,
              "!BDB & !distributed & update_trust_center_link_keys_required - get TCLK over APS",
              (FMT__0));
    zdo_initiate_tclk_gen_over_aps(0);
  }
  else if (ZB_IS_DEVICE_ZED())
  {
    /* If not bdb and I am ZED (no need to start router), I am is started. */
    TRACE_MSG(TRACE_SECUR3, "!BDB ZED - complete start", (FMT__0));
    (void)zb_buf_get_out_delayed_ext(zb_zdo_startup_complete_int_delayed, RET_OK, 0);
  }
  else
  {
    /* MISRA rule 15.7 requires empty 'else' branch. */
  }

  if (param != ZB_BUF_INVALID)
  {
    zb_buf_free(param);
  }
}


static void zdo_classic_handle_secur_failed_signal(zb_bufid_t param)
{
  zb_apsme_confirm_key_ind_t *ind = ZB_BUF_GET_PARAM(param, zb_apsme_confirm_key_ind_t);

  TRACE_MSG(TRACE_ZDO1, "report error status %hd, zb_app_signal_pack", (FMT__H, ind->status));
  (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, (zb_int16_t)ind->status, 0U);
  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
}


static void zdo_classic_handle_rejoin_after_secur_failed_signal(zb_bufid_t param)
{
  zb_ext_pan_id_t ext_pan_id;
  /* rejoin to current pan */
  zb_get_extended_pan_id(ext_pan_id);
  (void)zdo_initiate_rejoin(param, ext_pan_id, ZB_AIB().aps_channel_mask_list, ZB_FALSE);
}


static void zdo_classic_handle_leave_with_rejoin_signal(zb_bufid_t param)
{
#ifdef ZB_USE_NVRAM
  /* If we fail, trace is given and assertion is triggered */
  (void)zb_nvram_write_dataset(ZB_NVRAM_COMMON_DATA);
#endif

#ifdef ZB_NWK_BLACKLIST
  zb_nwk_blacklist_reset();
#endif

  zdo_inform_app_leave(ZB_NWK_LEAVE_TYPE_REJOIN);

#ifdef ZB_NSNG
  /* A hack because of unperfect NSNG CSMA/CA mechanism, hack for CCB2255 */
  ZB_SCHEDULE_ALARM(zb_nwk_do_rejoin_after_leave, param, ZB_MILLISECONDS_TO_BEACON_INTERVAL(500));
#else
  zb_nwk_do_rejoin_after_leave(param);
#endif
}

#endif /* ZB_JOIN_CLIENT */

#ifdef ZB_COORDINATOR_ROLE

static void zdo_classic_handle_secured_rejoin_signal(zb_bufid_t param)
{
  zb_apsme_transport_key_req_t *req = ZB_BUF_GET_PARAM(param, zb_apsme_transport_key_req_t);
  zb_aps_device_key_pair_set_t *key_pair = zb_secur_get_link_key_by_address(req->dest_address.addr_long, ZB_SECUR_VERIFIED_KEY);
  /* r21 doesn't use empty keys
   * In r21 setup alarm waiting for unique TCLK request.
   * Note: param must hold transport key request even while we do not
   * actually send it there!
   */
  /*
    It IS possible to work not in BDB but require TCLK update (use case: certification tests).
    But update_trust_center_link_keys_required flag is always presend and initialized to 1 during BDB init.
  */
  /* Note: is somewhere in time we put update_trust_center_link_keys_required under ifdef, need to check for ZB_IN_BDB() here.
     Remember ZB_IN_BDB() can be just 1 (BDB-only build). */
  if (ZB_TCPOL().update_trust_center_link_keys_required
      && !IS_DISTRIBUTED_SECURITY()
      && (key_pair == NULL))
  {
    ZB_SCHEDULE_ALARM(bdb_link_key_refresh_alarm, param, ZB_TCPOL().trust_center_node_join_timeout);
  }
  else
  {
    zb_buf_free(param);
  }
}


static void zdo_classic_handle_tclk_verified_remote_signal(zb_address_ieee_ref_t param)
{
  ZVUNUSED(param);
}


static void zdo_classic_handle_device_left_signal(zb_address_ieee_ref_t param)
{
  ZVUNUSED(param);
}

#endif /* ZB_COORDINATOR_ROLE */

#ifdef ZB_FORMATION
static void zdo_classic_handle_formation_done_signal(zb_bufid_t param)
{
  (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, zb_buf_get_status(param), 0);
  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
}


static void zdo_classic_handle_formation_failed_signal(zb_bufid_t param)
{
  (void)zb_app_signal_pack(param, ZB_ZDO_SIGNAL_DEFAULT_START, zb_buf_get_status(param), 0);
  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
}
#endif /* ZB_FORMATION */

static void zdo_classic_handle_comm_signal(zb_commissioning_signal_t signal, zb_bufid_t param)
{
  TRACE_MSG(TRACE_ZDO1, ">> zdo_classic_handle_comm_signal, signal %d, param %d",
            (FMT__D_D, signal, param));

  switch(signal)
  {
    case ZB_COMM_SIGNAL_START:
      zdo_classic_initiate_commissioning(param);
      break;
#ifdef ZB_JOIN_CLIENT
    case ZB_COMM_SIGNAL_NWK_DISC_FAILED:
      zdo_classic_handle_nwk_disc_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_JOIN_FAILED:
      zdo_classic_handle_join_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_AUTH_FAILED:
      zdo_classic_handle_authentication_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_INITIATE_REJOIN:
      zdo_classic_handle_initiate_rejoin_signal(param);
      break;
    case ZB_COMM_SIGNAL_DEV_ANNCE_SENT:
      zdo_classic_handle_dev_annce_sent_signal(param);
      break;
    case ZB_COMM_SIGNAL_ROUTER_STARTED:
      zdo_classic_handle_router_started_signal(param);
      break;
    case ZB_COMM_SIGNAL_TCLK_UPDATE_COMPLETE:
      zdo_classic_handle_tclk_update_complete_signal(param);
      break;
    case ZB_COMM_SIGNAL_TCLK_UPDATE_FAILED:
      zdo_authentication_failed(param);
      break;
    case ZB_COMM_SIGNAL_LEAVE_DONE:
      zdo_classic_handle_leave_done_signal(param);
      break;
    case ZB_COMM_SIGNAL_AUTH_OK:
      zdo_classic_handle_auth_ok_signal(param);
      break;
    case ZB_COMM_SIGNAL_SECUR_FAILED:
      zdo_classic_handle_secur_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_REJOIN_AFTER_SECUR_FAILED:
      zdo_classic_handle_rejoin_after_secur_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_LEAVE_WITH_REJOIN:
      zdo_classic_handle_leave_with_rejoin_signal(param);
      break;
#endif /* ZB_JOIN_CLIENT */

#ifdef ZB_COORDINATOR_ROLE
    case ZB_COMM_SIGNAL_SECURED_REJOIN:
      zdo_classic_handle_secured_rejoin_signal(param);
      break;
    case ZB_COMM_SIGNAL_TCLK_VERIFIED_REMOTE:
      zdo_classic_handle_tclk_verified_remote_signal(param);
      break;
    case ZB_COMM_SIGNAL_DEVICE_LEFT:
      zdo_classic_handle_device_left_signal(param);
      break;
#endif /* ZB_COORDINATOR_ROLE */

#ifdef ZB_FORMATION
    case ZB_COMM_SIGNAL_FORMATION_DONE:
      zdo_classic_handle_formation_done_signal(param);
      break;
    case ZB_COMM_SIGNAL_FORMATION_FAILED:
      zdo_classic_handle_formation_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_AUTHENTICATE_REMOTE:
      zb_buf_free(param);
      break;
#endif /* ZB_FORMATION */

    default:
      TRACE_MSG(TRACE_ERROR, "unknown commissioning signal: %d", (FMT__D, signal));
      ZB_ASSERT(0);
      if (param != ZB_BUF_INVALID)
      {
        zb_buf_free(param);
      }
      break;
  }

  TRACE_MSG(TRACE_ZDO1, "<< zdo_classic_handle_comm_signal", (FMT__0));
}


#ifdef ZB_ROUTER_ROLE

static zb_uint8_t zdo_classic_get_permit_join_duration(void)
{
  zb_uint8_t duration;

  if (ZB_NIB().max_children != 0U)
  {
    duration = ZDO_CTX().conf_attr.permit_join_duration;
  }
  else
  {
    duration = 0U;
  }

  return duration;
}

#endif  /* #ifdef ZB_ROUTER_ROLE */

static zb_bool_t zdo_classic_must_use_installcode(zb_bool_t is_client)
{
  ZVUNUSED(is_client);
#ifdef ZB_SECURITY_INSTALLCODES
  return (zb_bool_t)ZB_TCPOL().require_installcodes;
#else
  return ZB_FALSE;
#endif /* ZB_SECURITY_INSTALLCODES */
}


static void zdo_classic_init(void)
{
  ZB_TCPOL().update_trust_center_link_keys_required = ZB_TRUE;
}


static void zdo_classic_commissioning_force_link(void)
{
  zdo_classic_init();

  COMM_SELECTOR().signal = zdo_classic_handle_comm_signal;

#ifdef ZB_ROUTER_ROLE
  COMM_SELECTOR().get_permit_join_duration = zdo_classic_get_permit_join_duration;
#endif /* ZB_ROUTER_ROLE */

  COMM_SELECTOR().must_use_install_code = zdo_classic_must_use_installcode;
}


#ifdef ZB_FORMATION

/* It is possible to move these functions to a separate object module if needed
   for size optimization */


static void zdo_classic_commissioning_formation_channels_mask(zb_channel_list_t list)
{
  zb_channel_page_list_copy(list, ZB_AIB().aps_channel_mask_list);
}


static void zdo_classic_formation_force_link(void)
{
  zdo_formation_force_link();

  FORMATION_SELECTOR().start_formation = zdo_start_formation;
  FORMATION_SELECTOR().get_formation_channels_mask = zdo_classic_commissioning_formation_channels_mask;
}

#endif /* ZB_FORMATION */


#ifdef ZB_COORDINATOR_ROLE

void zb_set_network_coordinator_role_legacy(zb_uint32_t channel_mask)
{
  zdo_classic_commissioning_force_link();
#ifdef ZB_FORMATION
  /* [VK]: it's impossible to explain to MISRA that ZB_FORMATION is defined with ZB_COORDINATOR_ROLE always */
  zdo_classic_formation_force_link();
#endif /* ZB_FORMATION */
  zb_set_network_coordinator_role_with_mode(channel_mask, ZB_COMMISSIONING_CLASSIC);
}


void zb_set_nwk_coordinator_role_legacy_ext(zb_channel_list_t channel_list)
{
  zdo_classic_commissioning_force_link();
#ifdef ZB_FORMATION
  /* [VK]: it's impossible to explain to MISRA that ZB_FORMATION is defined with ZB_COORDINATOR_ROLE always */
  zdo_classic_formation_force_link();
#endif /* ZB_FORMATION */
  zb_set_nwk_role_mode_common_ext(ZB_NWK_DEVICE_TYPE_COORDINATOR,
                                  channel_list,
                                  ZB_COMMISSIONING_CLASSIC);
}

#endif /* ZB_COORDINATOR_ROLE */


#ifdef ZB_ROUTER_ROLE

void zb_set_network_router_role_legacy(zb_uint32_t channel_mask)
{
  zdo_classic_commissioning_force_link();
#if defined ZB_FORMATION && defined ZB_DISTRIBUTED_SECURITY_ON
  /* enable distributed security, not optimal, but no need to optimize legacy commissioning */
  /* [VK]: it's impossible to explain to MISRA that ZB_FORMATION is defined with ZB_ROUTER_ROLE always */
  zdo_classic_formation_force_link();
#endif /* ZB_FORMATION && ZB_DISTRIBUTED_SECURITY_ON */
  zb_set_network_router_role_with_mode(channel_mask, ZB_COMMISSIONING_CLASSIC);
}


void zb_set_nwk_router_role_legacy_ext(zb_channel_list_t channel_list)
{
  zdo_classic_commissioning_force_link();
#if defined ZB_FORMATION && defined ZB_DISTRIBUTED_SECURITY_ON
  /* enable distributed security, not optimal, but no need to optimize legacy commissioning */
  /* [VK]: it's impossible to explain to MISRA that ZB_FORMATION is defined with ZB_ROUTER_ROLE always */
  zdo_classic_formation_force_link();
#endif /* ZB_FORMATION && ZB_DISTRIBUTED_SECURITY_ON */
  zb_set_nwk_role_mode_common_ext(ZB_NWK_DEVICE_TYPE_ROUTER,
                                  channel_list,
                                  ZB_COMMISSIONING_CLASSIC);
}

#endif /* ZB_ROUTER_ROLE */


#ifdef ZB_ED_FUNC

void zb_set_network_ed_role_legacy(zb_uint32_t channel_mask)
{
  zdo_classic_commissioning_force_link();

  zb_set_network_ed_role_with_mode(channel_mask, ZB_COMMISSIONING_CLASSIC);
}

void zb_set_nwk_ed_role_legacy_ext(zb_channel_list_t channel_list)
{
  zdo_classic_commissioning_force_link();

  zb_set_nwk_role_mode_common_ext(ZB_NWK_DEVICE_TYPE_ED,
                                  channel_list,
                                  ZB_COMMISSIONING_CLASSIC);
}

#endif /* ZB_ED_FUNC */
#endif /* ZB_COMMISSIONING_CLASSIC_SUPPORT */
