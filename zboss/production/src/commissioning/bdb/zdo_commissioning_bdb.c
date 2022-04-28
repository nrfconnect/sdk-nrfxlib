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
/*  PURPOSE: BDD specific commissioning
*/

#define ZB_TRACE_FILE_ID 62

#include "zb_common.h"
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
#include "zdo_wwah_stubs.h"

#define BDB_COMMISSIONING_DEBUG

#if defined ZB_BDB_MODE && !defined BDB_OLD

zb_bdb_comm_ctx_t g_bdb_ctx;

void bdb_commissioning_machine(zb_uint8_t param);
static void bdb_initialization_machine(zb_uint8_t param);
static void bdb_touchlink_machine(zb_uint8_t param);
static void bdb_network_steering_machine(zb_uint8_t param);
static void bdb_network_formation_machine(zb_uint8_t param);
#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
static void bdb_finding_n_binding_machine(zb_uint8_t param);
#endif
void bdb_initialization_procedure(zb_uint8_t param);
static void bdb_precomm_rejoin_over_all_channels(zb_uint8_t param, zb_uint16_t secure);
void bdb_network_steering_on_network(zb_uint8_t param);
#ifdef ZB_JOIN_CLIENT
void bdb_network_steering_not_on_network(zb_uint8_t param);
#endif
void bdb_after_mgmt_permit_joining_cb(zb_uint8_t param);
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
static void bdb_rejoin_machine(zb_uint8_t param);
static void schedule_wwah_rejoin_backoff_attempt(zb_uint8_t param);
#endif
#ifndef NCP_MODE_HOST
static void nwk_cancel_network_discovery_response(zb_bufid_t buf);
#endif /* NCP_MODE_HOST */

zb_bool_t bdb_not_ever_joined()
{
  zb_ext_pan_id_t curr_ext_pan_id = {0};
  zb_get_extended_pan_id(curr_ext_pan_id);

  return (zb_bool_t)(ZB_EXTPANID_IS_ZERO(curr_ext_pan_id)
                     && !zb_zdo_authenticated()
                     && !zb_zdo_tclk_valid());
}


zb_bool_t bdb_is_in_tc_rejoin(void)
{
  return ZB_U2B(ZB_BDB().bdb_tc_rejoin_active);
}


#ifdef ZB_FORMATION

static zb_bool_t bdb_can_do_network_formation()
{
  return (zb_bool_t)(!zb_zdo_joined()
                     && (
#ifdef ZB_DISTRIBUTED_SECURITY_ON
                       (ZB_IS_DEVICE_ZR()
                        && FORMATION_SELECTOR().start_formation != NULL
                        /* ZR can do formation in Distributed only */
                        && zb_tc_is_distributed())
#else
                       0
#endif
                       /* Do Formation as ZC */
                       || ZB_IS_DEVICE_ZC())
    );
}

#endif /* ZB_FORMATION */

static void bdb_initiate_commissioning(zb_uint8_t param)
{
  /* ZB_BDB().bdb_commissioning_step = ZB_BDB_INITIALIZATION; */
  /* TRACE_MSG(TRACE_ZDO1, "bdb mode - initiate bdb commissioning", (FMT__0)); */
  /* ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param); */

#if 0//def ZB_BDB_TOUCHLINK
  if (ZB_BDB().bdb_commissioning_mode == ZB_BDB_TOUCHLINK_TARGET)
  {
    ZB_BDB().bdb_commissioning_step = ZB_BDB_TOUCHLINK_TARGET;
    ZB_SCHEDULE_CALLBACK(bdb_touchlink_machine, param);
  }
  else
#endif
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN;
    ZB_BDB().v_scan_channels = 0; /* Will be set from scan routines. */
    TRACE_MSG(TRACE_ZDO1, "Start BDB Initialization procedure", (FMT__0));
    ZB_SCHEDULE_CALLBACK(bdb_initialization_procedure, param);
  }
}

zb_bool_t bdb_start_top_level_commissioning(zb_uint8_t mode_mask)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_start_top_level_commissioning mode_mask 0x%hx", (FMT__H, mode_mask));
  if ((ZB_BDB().bdb_commissioning_step != ZB_BDB_INITIALIZATION &&
       ZB_BDB().bdb_commissioning_step != ZB_BDB_COMMISSIONING_STOP) ||
      ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_IN_PROGRESS)
  {
    TRACE_MSG(TRACE_ZDO1, "bdb_commissioning in progress (step=%hd), return ZB_FALSE",
                          (FMT__H,ZB_BDB().bdb_commissioning_step));
    return ZB_FALSE;
  }
  ZB_BDB().bdb_commissioning_mode = mode_mask;
  ZB_BDB().bdb_commissioning_step = ZB_BDB_INITIALIZATION;
  /* Do stack init and startup */
  if (!zb_zdo_joined())
  {
    zboss_start_continue();
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_BDB().bdb_commissioning_step = ZB_BDB_INITIALIZATION;
    bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_START, 0);
  }
  return ZB_TRUE;
}

void bdb_commissioning_signal(bdb_commissioning_signal_t sig, zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO3, "bdb_commissioning_signal sig %hd param %hd", (FMT__H_H, sig, param));
  BDB_COMM_CTX().signal = sig;

  if (param)
  {
    ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
  }
  else
  {
    zb_buf_get_out_delayed(bdb_commissioning_machine);
  }
}

#ifdef ZB_BDB_TOUCHLINK
void bdb_check_fn()
{
  if (!ZB_AIB().aps_use_nvram)
  {
    TRACE_MSG(TRACE_ZDO1, "No nvram - factory new", (FMT__0));
    ZB_ZLL_SET_FACTORY_NEW();
  }
  else
  {
    /* TODO: recheck this condition */
    if (bdb_not_ever_joined())
    {
      TRACE_MSG(TRACE_ZDO1, "Factory new", (FMT__0));
      ZB_ZLL_SET_FACTORY_NEW();
    }
    else
    {
      TRACE_MSG(TRACE_ZDO1, "Not factory new", (FMT__0));
      ZB_ZLL_CLEAR_FACTORY_NEW();
    }
  }
}
#endif


static void bdb_init(void)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_init", (FMT__0));
  ZB_BDB().bdb_commissioning_step = ZB_BDB_INITIALIZATION;
  ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
  ZB_BDB().bdb_commissioning_group_id = (zb_uint16_t)-1;
  /* ZBOSS "autostart" mode:
     - ZC, not on a network: Formation
     - ZC, on a network: Init + Steering
     - non-D ZR/ZED, not on a network: Steering (not on a network)
     - non-D ZR/ZED, on a network: Init + Steering (on a network)
  */
  ZB_BDB().bdb_commissioning_mode = ZB_BDB_NETWORK_STEERING | ZB_BDB_NETWORK_FORMATION | ZB_BDB_FINDING_N_BINDING;
  ZB_BDB().bdb_commissioning_time = ZB_BDBC_MIN_COMMISSIONING_TIME_S;
  ZB_BDB().bdb_scan_duration = ZB_DEFAULT_SCAN_DURATION;

  /* init to zero: channel sets will be set to correct values during commissioning start
     (see bdb_init_channel_sets()) or if the app sets them directly using
     zb_set_bdb_primary_channel_set() or zb_set_bdb_secondary_channel_set() */
  ZB_BDB().bdb_primary_channel_set = ZB_BDB().bdb_secondary_channel_set = 0u;

  zb_aib_tcpol_set_update_trust_center_link_keys_required(ZB_TRUE);

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  zb_bdb_finding_binding_init_ctx();
#endif
}

zb_bool_t bdb_joined(void)
{
  zb_bool_t joined;
  zb_ext_pan_id_t ext_pan_id = {0};

  zb_get_extended_pan_id(ext_pan_id);
  joined = (zb_bool_t)(!ZB_EXTPANID_IS_ZERO(ext_pan_id) &&
                       zb_zdo_authenticated());

  if (zb_aib_get_coordinator_version() >= 21)
  {
    joined = (zb_bool_t)(joined && zb_zdo_tclk_valid());
  }

  return joined;
}


static void bdb_init_channel_sets(void)
{
  zb_uint8_t used_page;

  TRACE_MSG(TRACE_ZDO1, ">> bdb_init_channel_sets", (FMT__0));

  TRACE_MSG(TRACE_ZDO1, "primary: 0x%x; secondary: 0x%x",
            (FMT__D_D, zb_get_bdb_primary_channel_set(), zb_get_bdb_secondary_channel_set()));
  /* DD: this logic is moved from zb_set_nwk_role_mode_common_ext() and
     zb_production_config_load_ver_2_0().
     Not sure it is correct for MULTIMAC case though. */

  used_page = zb_aib_channel_page_list_get_first_filled_page();

  /* if SubGHz support is disabled, the first filled page should be 0, 2.4GHz,
     so we don't need to check if we support SubGHz. */
  if (used_page != ZB_CHANNEL_PAGES_NUM)
  {
    if (zb_get_bdb_primary_channel_set() == 0u)
    {
      zb_set_bdb_primary_channel_set(zb_aib_channel_page_list_get_mask(used_page));
      TRACE_MSG(TRACE_ZDO1, "new primary channel set: 0x%x",
                (FMT__D, zb_get_bdb_primary_channel_set()));
    }
    if (zb_get_bdb_secondary_channel_set() == 0u)
    {
      /* TODO (ZBS-428): This needs to be fixed according to stack documentation, primary and
      secondary channel masks should follow the BDB spec default values (5.3 Attributes),
      and secondary channel set should not be changed if it has been manually set to 0 (5.3.12 bdbSecondaryChannelSet attribute) */
      TRACE_MSG(TRACE_ZDO1, "secondary channel set unused: 0x%x",
                (FMT__D, zb_get_bdb_secondary_channel_set()));
    }
  }

  TRACE_MSG(TRACE_ZDO1, "<< bdb_init_channel_sets", (FMT__0));
}


void bdb_preinit(void)
{
  zb_bool_t joined;
  zb_ext_pan_id_t ext_pan_id = {0};

  TRACE_MSG(TRACE_ZDO1, ">> bdb_preinit", (FMT__0));

#ifdef ZB_BDB_TOUCHLINK
  bdb_check_fn();
#endif

  ZB_ASSERT(zb_get_device_type() != ZB_NWK_DEVICE_TYPE_NONE);

  zb_get_extended_pan_id(ext_pan_id);
  joined = bdb_joined();

#if defined ZB_DISTRIBUTED_SECURITY_ON
  if (joined)
  {
    zb_sync_distributed();
  }
#endif

  if (joined)
  {
    TRACE_MSG(TRACE_ZDO1, "Not factory new", (FMT__0));
#ifdef ZB_BDB_TOUCHLINK
    /* just in case */
    ZB_ZLL_CLEAR_FACTORY_NEW();
#endif
  }

#ifdef ZB_BDB_TOUCHLINK
  /* 8.7 Touchlink procedure for an initiator:
     The touchlink procedure for an initiator can perform a "normal" channel scan or an "extended"
     channel scan; the latter is used if a reset to factory new is required (see sub-clause 9.2) or
     if the target could be operating on a channel other than those defined in
     bdbcTLPrimaryChannelSet.
 */
  ZB_BDB().bdb_ext_channel_scan = 1; /* ZB_ZLL_IS_FACTORY_NEW() */
#endif

  bdb_init_channel_sets();

  TRACE_MSG(TRACE_INFO1, "dev type %hd, joined %hd, ext_pan_id %hd, authenticated %hd, tclk_valid %hd",
            (FMT__H_H_H_H_H,
             zb_get_device_type(),
             joined,
             !ZB_EXTPANID_IS_ZERO(ext_pan_id),
             zb_zdo_authenticated(),
             zb_zdo_tclk_valid()));
}


/**
   BDB Initialization procedure according to 7.1 Initialization procedure
 */
void bdb_initialization_procedure(zb_uint8_t param)
{
  zb_ext_pan_id_t ext_pan_id = {0};
  zb_get_extended_pan_id(ext_pan_id);

  bdb_preinit();
/* Can do rejoin if ZED and if authenticated: only secured rejoin is
   * allowed by BDB.
   * Seems, ZR must continue its work without a rejoin (hmm?)
   */
  if (bdb_joined())
  {
#if defined ZB_COORDINATOR_ROLE
    if (ZB_IS_DEVICE_ZC())
    {
      TRACE_MSG(TRACE_ZDO2, "Start ZC without formation", (FMT__0));
      ZB_SCHEDULE_CALLBACK(zb_nwk_cont_without_formation, param);
      param = 0;
    }
    else
#endif
#if defined ZB_ROUTER_ROLE
    if (ZB_IS_DEVICE_ZR() && !ZB_BDB().bdb_force_router_rejoin)
    {
      TRACE_MSG(TRACE_ZDO2, "Initiate start router for ZR", (FMT__0));
      ZB_SCHEDULE_CALLBACK(zb_zdo_start_router, param);
      param = 0;
    }
    else
#endif
    {
      /* Perform secure rejoin */
      TRACE_MSG(TRACE_ZDO2, "Perform secure rejoin", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_SECURE_REJOIN, param);
      param = 0;
    }

    /* Reset steering to avoid network opening
     * (permit join request command) */
    ZB_BDB().bdb_commissioning_mode = ZB_BDB_NETWORK_FORMATION | ZB_BDB_FINDING_N_BINDING;

    /* We are NFN - indicate ZB_BDB_SIGNAL_DEVICE_REBOOT */
    ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_DEVICE_REBOOT;
  }
  else if (bdb_not_ever_joined())
  {
    /* Nothing to do, just continue */
    TRACE_MSG(TRACE_ERROR, "Nothing to do, just continue", (FMT__0));
    ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_DEVICE_FIRST_START;
  }
  else if (!ZB_EXTPANID_IS_ZERO(ext_pan_id)
           && ((zb_aib_get_coordinator_version() < 21) ||
               ((zb_aib_get_coordinator_version() >= 21) && zb_zdo_tclk_valid())))
  {
    /* Perform TC rejoin */
    TRACE_MSG(TRACE_ERROR, "Perform TC rejoin", (FMT__0));
    bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_TC_REJOIN, param);

    ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_DEVICE_REBOOT;
    param = 0;
  }
#ifndef ZB_COORDINATOR_ONLY
  else
  {
    TRACE_MSG(TRACE_ERROR, "Strange configuration, leave", (FMT__0));
    zdo_commissioning_leave(param, ZB_FALSE, ZB_FALSE);
    param = 0;
  }
#endif  /* #ifndef ZB_COORDINATOR_ONLY */

  if (param)
  {
    /* No additional actions are needed, finish initialization */
    bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
  }
}

static void bdb_post_commissioning_actions(void)
{
#if (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
  wwah_post_commissioning_actions();
#endif
}

static void bdb_send_tc_rejoin_done_after_reboot(zb_uint8_t param, zb_uint16_t status)
{
  TRACE_MSG(TRACE_ZDO1, ">> bdb_send_tc_rejoin_done_after_reboot, param %hd, status %d",
    (FMT__H_D, param, status));

  zb_app_signal_pack(param, ZB_BDB_SIGNAL_TC_REJOIN_DONE, -status, 0);
  ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);

  TRACE_MSG(TRACE_ZDO1, "<< bdb_send_tc_rejoin_done_after_reboot", (FMT__0));
}

void bdb_commissioning_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_commissioning_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));

  TRACE_MSG(TRACE_ZDO1, ">> bdb_commissioning_machine param %hd step 0x%x bdb_commissioning_mode 0x%x bdb_commissioning_status %d joined %d",
            (FMT__H_D_D_D_D, param, ZB_BDB().bdb_commissioning_step, ZB_BDB().bdb_commissioning_mode, ZB_BDB().bdb_commissioning_status, zb_zdo_joined()));

  /* Run machines specific to the current SE device state */
  switch (ZB_BDB().bdb_commissioning_step)
  {
    case ZB_BDB_INITIALIZATION:
      ZB_SCHEDULE_CALLBACK(bdb_initialization_machine, param);
      break;

    case ZB_BDB_TOUCHLINK_COMMISSIONING:
      /* FIXME: Do we really need separate step for target? Do not touch it for now, but need to
       * refactor during Touchlink commissioning refactoring/extension. */
    case ZB_BDB_TOUCHLINK_TARGET:
      ZB_SCHEDULE_CALLBACK(bdb_touchlink_machine, param);
      break;

    case ZB_BDB_NETWORK_STEERING:
      ZB_SCHEDULE_CALLBACK(bdb_network_steering_machine, param);
      break;

    case ZB_BDB_NETWORK_FORMATION:
      ZB_SCHEDULE_CALLBACK(bdb_network_formation_machine, param);
      break;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
    case ZB_BDB_FINDING_N_BINDING:
      ZB_SCHEDULE_CALLBACK(bdb_finding_n_binding_machine, param);
      break;
#endif

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
    case ZB_BDB_REJOIN:
      ZB_SCHEDULE_CALLBACK(bdb_rejoin_machine, param);
      break;
#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */

    case ZB_BDB_COMMISSIONING_STOP:
      if (BDB_COMM_CTX().signal == BDB_COMM_SIGNAL_FINISH)
      {
        /* commissioning done */
        if (ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_IN_PROGRESS)
        {
          ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
          /* Reset "force rejoin" flag */
          ZB_BDB().bdb_force_router_rejoin = ZB_FALSE;
        }

        TRACE_MSG(TRACE_INFO1, "COMMISSIONING_STOP: app signal %hd comm status %hd", (FMT__H_H, ZB_BDB().bdb_application_signal, ZB_BDB().bdb_commissioning_status));
        ZB_BDB().bdb_op_cancelled = ZB_FALSE;
        if (ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_CANCELLED)
        {
          zb_app_signal_pack_with_detailed_status(param,
                                                  ZB_BDB().bdb_application_signal,
                                                  RET_INTERRUPTED,
                                                  0);

        }
        else
        {
          zb_app_signal_pack(param, ZB_BDB().bdb_application_signal,
                             -ZB_BDB().bdb_commissioning_status, 0);
        }
        ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);

        if (ZB_BDB().bdb_tc_rejoin_after_reboot == ZB_TRUE &&
            ZB_BDB().bdb_application_signal == ZB_BDB_SIGNAL_DEVICE_REBOOT)
        {
          zb_buf_get_out_delayed_ext(bdb_send_tc_rejoin_done_after_reboot, ZB_BDB().bdb_commissioning_status, 0U);
          ZB_BDB().bdb_tc_rejoin_after_reboot = ZB_FALSE;
        }

        /* FIXME: Not sure when to start WWAH and other activities */
        if (ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_SUCCESS &&
            (ZB_BDB().bdb_application_signal == ZB_BDB_SIGNAL_DEVICE_FIRST_START ||
             ZB_BDB().bdb_application_signal == ZB_BDB_SIGNAL_DEVICE_REBOOT ||
             ZB_BDB().bdb_application_signal == ZB_BDB_SIGNAL_STEERING))
        {
          bdb_post_commissioning_actions();
        }
      }
      /* else ignore anything we can receive (for instance, LEAVE_DONE etc) */
      else
      {
        TRACE_MSG(TRACE_ZDO1, "commissioning is not in progress, ignore", (FMT__0));
        zb_buf_free(param);
      }
      break;

    default:
      TRACE_MSG(TRACE_ERROR, "Unknown commissioning state %d", (FMT__D, ZB_BDB().bdb_commissioning_mode));
#ifdef BDB_COMMISSIONING_DEBUG
      ZB_ASSERT(0);
#else
      if (param)
      {
        zb_buf_free(param);
      }
#endif
      break;
  }
}

static void bdb_initialization_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_initialization_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));
  switch (BDB_COMM_CTX().signal)
  {
    case BDB_COMM_SIGNAL_INIT_START:
    {
#ifdef ZB_BDB_TOUCHLINK
      if (ZB_BDB().bdb_commissioning_mode == ZB_BDB_TOUCHLINK_TARGET)
      {
        bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
      }
      else
#endif
      {
        if (bdb_joined() && zb_zdo_joined())
        {
          TRACE_MSG(TRACE_ZDO1, "Already inited, skip initialization", (FMT__0));
          bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
        }
        else
        {
          ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
          ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN;
          ZB_BDB().v_scan_channels = 0; /* Will be set from scan routines. */
          TRACE_MSG(TRACE_ZDO1, "Start BDB Initialization procedure", (FMT__0));
          ZB_SCHEDULE_CALLBACK(bdb_initialization_procedure, param);
        }
      }
    }
    break;

    case BDB_COMM_SIGNAL_INIT_SECURE_REJOIN:
      bdb_precomm_rejoin_over_all_channels(param, 1);
      break;

    case BDB_COMM_SIGNAL_INIT_TC_REJOIN:
      if (ZB_BDB().bdb_application_signal == ZB_BDB_SIGNAL_DEVICE_REBOOT)
      {
        ZB_BDB().bdb_tc_rejoin_after_reboot = ZB_TRUE_U;
      }

      ZB_BDB().bdb_tc_rejoin_active = ZB_TRUE;
      bdb_precomm_rejoin_over_all_channels(param, 0);
      break;

    case BDB_COMM_SIGNAL_NWK_AUTH_FAILED:
      /* ZG->aps.authenticated = ZB_FALSE; */
      /* FALLTHROUGH */
    case BDB_COMM_SIGNAL_NWK_JOIN_FAILED:
    {
      TRACE_MSG(TRACE_ZDO1, "Rejoin failed, secured %hd", (FMT__H, bdb_is_in_tc_rejoin()));

#if 0
      /* TC rejoin should be invoked only if we have unique TCLK or if it is directly allowed in
       * application to perform TC rejoin with legacy ZC.
       * To prevent infinite rejoins loop here, TC rejoin will be performed only once, but application
       * will receive ZB_BDB_SIGNAL_TC_REJOIN_DONE to choose a next action */
      if (!bdb_is_in_tc_rejoin() && ((zb_zdo_tclk_valid() && (zb_aib_get_coordinator_version() >= 21))
          || zb_aib_tcpol_get_allow_unsecure_tc_rejoins())
#ifdef ZB_REJOIN_BACKOFF
          && !zb_zdo_rejoin_backoff_is_running()
#endif
          )
      {
        /* Perform TC rejoin */
        TRACE_MSG(TRACE_ZDO1, "Perform TC rejoin", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_TC_REJOIN, param);
      }
      else
#endif
      {
        TRACE_MSG(TRACE_ZDO1, "Can't find network", (FMT__0));
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
        ZB_BDB().bdb_tc_rejoin_active = ZB_FALSE_U;

        bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
      }
    }
    break;

    case BDB_COMM_SIGNAL_NWK_JOIN_DONE:
      TRACE_MSG(TRACE_ZDO1, "Device is joined", (FMT__0));
#ifdef ZB_JOIN_CLIENT

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
      if (!ZB_PIBCACHE_RX_ON_WHEN_IDLE()
          && zb_zcl_wwah_check_if_wwah_rejoin_enabled()
          && !BDB_COMM_CTX().rejoin.rr_global_retries)
      {
        TRACE_MSG(TRACE_ZDO1, "Cancel any scheduled rejoin attempts", (FMT__0));
        ZB_SCHEDULE_ALARM_CANCEL(zdo_commissioning_initiate_rejoin, ZB_ALARM_ANY_PARAM);
      }
#endif

      if (!zdo_secur_waiting_for_tclk_update())
#endif
      {
        /* Join is finished - can reset the blacklist. */
#ifdef ZB_NWK_BLACKLIST
        zb_nwk_blacklist_reset();
#endif
      }

      ZB_BDB().bdb_tc_rejoin_active = ZB_FALSE_U;
      bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_NWK_FORMATION_OK:
      TRACE_MSG(TRACE_ZDO1, "Network is started OK", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF:
      TRACE_MSG(TRACE_ZDO1, "Network is started OK (router)", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_LEAVE_DONE:
      TRACE_MSG(TRACE_ZDO1, "Left from the network", (FMT__0));
      /* ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK; */
      /* We left from the network (probably because of wrong network configuration).
         Try to continue commissioning as FN device. */
      ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_DEVICE_FIRST_START;
      bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_INIT_FINISH:
      TRACE_MSG(TRACE_ZDO1, "Initialization finished, next step - Touchlink", (FMT__0));
      /* Go to next commissioning step - Touchlink procedure */
      ZB_BDB().bdb_commissioning_step = ZB_BDB_TOUCHLINK_COMMISSIONING;
      bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_START, param);
      break;

    case BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED:

      break;


    default:
      TRACE_MSG(TRACE_ERROR, "Unknown signal %d", (FMT__D, BDB_COMM_CTX().signal));
#ifdef BDB_COMMISSIONING_DEBUG
      ZB_ASSERT(0);
#else
      if (param)
      {
        zb_buf_free(param);
      }
#endif
      break;
  }
}

static void bdb_precomm_rejoin_over_all_channels(zb_uint8_t param, zb_uint16_t secure)
{
  if (!param)
  {
    zb_buf_get_out_delayed_ext(bdb_precomm_rejoin_over_all_channels, secure, 0);
  }
  else
  {
    zb_channel_list_t rejoin_ch_list;
    zb_ext_pan_id_t ext_pan_id;


    TRACE_MSG(TRACE_ZDO1, "bdb_precomm_rejoin_over_all_channels %hd", (FMT__H, param));
    zb_channel_list_init(rejoin_ch_list);
#if defined ZB_SUBGHZ_BAND_ENABLED
    zb_channel_page_list_copy(rejoin_ch_list, ZB_AIB().aps_channel_mask_list);
#else
    zb_channel_page_list_set_2_4GHz_mask(rejoin_ch_list,
                                         zb_aib_channel_page_list_get_2_4GHz_mask());
#endif /* !ZB_SUBGHZ_BAND_ENABLED */
    zb_get_extended_pan_id(ext_pan_id);
    zdo_initiate_rejoin(param, ext_pan_id,
                        rejoin_ch_list,
                        (zb_bool_t)secure);
  }
}

#ifdef ZB_BDB_TOUCHLINK
static void bdb_touchlink_target_timeout(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_touchlink_target_timeout: finish Touchlink Target", (FMT__0));
  ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK_TARGET_FINISHED;
  ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
  bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, param);
}
#endif

/* TODO: Split on Touchlink Target and Touchlink Initiator */
static void bdb_touchlink_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_touchlink_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));
  switch (BDB_COMM_CTX().signal)
  {
    case BDB_COMM_SIGNAL_TOUCHLINK_START:
    {
#ifdef ZB_BDB_TOUCHLINK
      if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_TOUCHLINK_TARGET)
      {
        /* TODO: Fix this correctly after discussion about signals. */
        if (ZB_BDB().bdb_commissioning_mode == ZB_BDB_TOUCHLINK_TARGET)
        {
          ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK_TARGET;
        }

        TRACE_MSG(TRACE_ZDO1, "Start Touchlink Target", (FMT__0));
        /* Now if touchlink compiled, force formation compile (need for distributed). */
        /* TODO: a) check for touchlink enabled; b) call touchlink indirectly */
#ifdef ZB_FORMATION
        zdo_formation_force_link();
#endif
        ZB_SCHEDULE_CALLBACK(bdb_touchlink_target_start, param);
      }
      else if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_TOUCHLINK_COMMISSIONING)
      {
        /* TODO: Fix this correctly after discussion about signals. */
        if (ZB_BDB().bdb_commissioning_mode == ZB_BDB_TOUCHLINK_COMMISSIONING)
        {
          ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TOUCHLINK;
        }

        TRACE_MSG(TRACE_ZDO1, "Start Touchlink initiator", (FMT__0));
        TRACE_MSG(TRACE_ZDO1, "Running BDB Touchlink initiator v_do_primary_scan %hd", (FMT__H, ZB_BDB().v_do_primary_scan));
        ZB_SCHEDULE_CALLBACK(bdb_touchlink_initiator, param);
      }
      else
#endif
      {
        TRACE_MSG(TRACE_ZDO1, "Skip Touchlink", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, param);
      }
    }
    break;

#ifdef ZB_BDB_TOUCHLINK
    case BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_DONE:
      TRACE_MSG(TRACE_ZDO1, "Touchlink initiator succeeded, continue", (FMT__0));
      ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_SUCCESS;
      bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_FAILED:
      TRACE_MSG(TRACE_ZDO1, "Touchlink initiator failed", (FMT__0));
      ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_TARGET_FAILURE;
      bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED:
      if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_TOUCHLINK_TARGET)
      {
        /* Touchlink Target: We are asked to start the network, no other networks discovered -
         * continue */
        /* TODO: Manipulating with status this way only because of bdb_touchlink implementation,
         * remove when it will be fixed! */
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
        ZB_SCHEDULE_CALLBACK(bdb_touchlink_target_start, param);
      }
      else
      {
        ZB_ASSERT(0);           /* Debug */
      }
      break;

    case BDB_COMM_SIGNAL_NWK_JOIN_FAILED:
      if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_TOUCHLINK_COMMISSIONING &&
          !(ZB_BDB().bdb_commissioning_mode & ZB_BDB_TOUCHLINK_TARGET))
      {
        TRACE_MSG(TRACE_ZDO1, "Touchlink Initiator: Failed to rejoin to the formed network", (FMT__0));
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
        bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, param);
      }
      else
      {
        ZB_ASSERT(0);           /* Debug */
      }
      break;

    case BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF:
    {
      TRACE_MSG(TRACE_ZDO1, "Touchlink Target: successfully formed the network", (FMT__0));

      /* Inform the application about Touchlink Target formation */
      zb_app_signal_pack(param, ZB_BDB_SIGNAL_TOUCHLINK_NWK,
                         ZB_BDB_STATUS_SUCCESS,
                         0);
      ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);

      ZB_SCHEDULE_ALARM_CANCEL(bdb_touchlink_target_timeout, ZB_ALARM_ANY_PARAM);
      bdb_touchlink_target_timeout(0);
    }
    break;

    case BDB_COMM_SIGNAL_NWK_JOIN_DONE:
      TRACE_MSG(TRACE_ZDO1, "Touchlink Initiator: successfully joined the network", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, 0);
      break;

    case BDB_COMM_SIGNAL_TOUCHLINK_NOTIFY_TASK_RESULT:
    {
      zb_ret_t status = zb_buf_get_status(param);
      if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_TOUCHLINK_TARGET)
      {
        if (status == ZB_ZLL_TASK_STATUS_OK)
        {
          /* Touchlink Target started */

          /* FIXME: BDB spec does not define the timeout for Touchlink Target. Let's assume device
           * will be in the Target mode for 1 min by default. */
          /* TODO: Stop Touchlink Target if device finishes Touchlink Target procedure before
           * timeout. */
          TRACE_MSG(TRACE_ZDO1, "Touchlink target started, wait for Touchlink Target timeout", (FMT__0));
          ZB_SCHEDULE_ALARM(bdb_touchlink_target_timeout, 0, ZB_TIME_ONE_SECOND * 60);

          /* Inform the application that Touchlink Target is started */
          zb_app_signal_pack(param, ZB_BDB_SIGNAL_TOUCHLINK_TARGET, ZB_BDB_STATUS_SUCCESS, 0);
          ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
        }
        else
        {
          TRACE_MSG(TRACE_ZDO1, "Touchlink target start failed", (FMT__0));
          ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_TARGET_FAILURE;
          bdb_commissioning_signal(BDB_COMM_SIGNAL_TOUCHLINK_FINISH, param);
        }
      }
      else /* Touchlink Initiator */
      {
        if (status == ZB_ZLL_TASK_STATUS_FINISHED)
        {
          TRACE_MSG(TRACE_ZDO1, "Some device discovered: continue with touchlink_initiator", (FMT__0));
          /* TODO: Refactor this function! Similar to bdb_commissioning_machine() there is a lot of
           * unclear manipulations with flags there etc. */
          ZB_SCHEDULE_CALLBACK(bdb_touchlink_initiator, param);
        }
        else
        {
          ZB_ASSERT(0);         /* debug */
        }
      }
    }
    break;
#endif /* ZB_BDB_TOUCHLINK */
    case BDB_COMM_SIGNAL_TOUCHLINK_FINISH:
      if (ZB_BDB().bdb_commissioning_status != ZB_BDB_STATUS_IN_PROGRESS)
      {
        ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
        TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_FINISH, param);
      }
      else
      {
        TRACE_MSG(TRACE_ZDO1, "Touchlink finished, next step - Network Steering", (FMT__0));
        /* Go to next commissioning step - Network Steering procedure */
        ZB_BDB().bdb_commissioning_step = ZB_BDB_NETWORK_STEERING;
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_START, param);
      }
      break;

    default:
      TRACE_MSG(TRACE_ERROR, "Unknown signal %d", (FMT__D, BDB_COMM_CTX().signal));
#ifdef BDB_COMMISSIONING_DEBUG
      ZB_ASSERT(0);
#else
      if (param)
      {
        zb_buf_free(param);
      }
#endif
      break;
  }
}

#ifndef ZB_COORDINATOR_ONLY
void bdb_network_steering_auth_failure(zb_uint8_t param)
{
  ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
#ifdef ZB_CERTIFICATION_HACKS
  if (ZB_CERT_HACKS().stay_on_network_after_auth_failure && zb_zdo_joined())
  {
    /* special hack for bdb tests - do not leave network if authentication failed */
    ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
    TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
    bdb_commissioning_signal(BDB_COMM_SIGNAL_FINISH, param);
  }
  else
#endif
  {
    TRACE_MSG(TRACE_ZDO1, "Authentication failure. Leave.", (FMT__0));
    zdo_commissioning_leave(param, ZB_FALSE, ZB_FALSE);
  }
}

void bdb_network_steering_tclk_ex_failure(zb_uint8_t param)
{
  ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_TCLK_EX_FAILURE;

  /* Joined but failed to update it's TC link key - leave network and complete steering
   * with status ZB_BDB_STATUS_TCLK_EX_FAILURE */

  TRACE_MSG(TRACE_ZDO1, "TC link key update failure. Leave.", (FMT__0));
  /* ZB_BDB().bdb_commissioning_step = ZB_BDB_LAST_COMMISSIONING_STEP; */

#ifdef ZB_CERTIFICATION_HACKS
  if (ZB_CERT_HACKS().stay_on_network_after_auth_failure)
  {
    /* special hack for bdb tests - do not leave network if tclk update failed */
    bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
  }
  else
#endif
  {
    zdo_commissioning_leave(param, ZB_FALSE, ZB_FALSE);
  }
}
#endif /* !ZB_COORDINATOR_ONLY */

void bdb_network_steering_start(zb_uint8_t param)
{
#ifdef ZB_JOIN_CLIENT
  if (zdo_secur_waiting_for_tclk_update())
  {
    TRACE_MSG(TRACE_ZDO1, "We are waiting for TCLK update. What to do? Lets free the buf at least. buf %hd", (FMT__H, param));
    zb_buf_free(param);
  }
  else
#endif
  {
    if (zb_zdo_joined())
    {
      /*
        The node then sets bdbCommissioningStatus to SUCCESS. If the node supports
        touchlink, it sets the values of the aplFreeNwkAddrRangeBegin,
        aplFreeNwkAddrRangeEnd, aplFreeGroupID-RangeBegin and
        aplFreeGroupIDRangeEnd attributes all to 0x0000 (indicating the node 1045 having
        joined the network using MAC association).
      */
      TRACE_MSG(TRACE_ZDO1, "Start BDB network steering when on network", (FMT__0));
      ZB_SCHEDULE_CALLBACK(bdb_network_steering_on_network, param);
    }
    else
    {
      /* If we are NFN and join failed - finish and pass it up to application. */
      if (!zb_bdb_is_factory_new())
      {
        TRACE_MSG(TRACE_ZDO1, "Not ZB_JOINED and NFN - finish BDB", (FMT__0));
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
      }
      else
      {
        TRACE_MSG(TRACE_ZDO1, "Start BDB network steering when NOT on network", (FMT__0));
#ifdef ZB_JOIN_CLIENT
        /* call directly to simplify cancellation logic */
        bdb_network_steering_not_on_network(param);
#endif
      }
    }
  }
}

#ifdef ZB_JOIN_CLIENT
void bdb_network_steering_start_scan(zb_uint8_t param)
{
  zb_nlme_network_discovery_request_t *req = ZB_BUF_GET_PARAM(param, zb_nlme_network_discovery_request_t);

  ZB_BZERO(req, sizeof(zb_nlme_network_discovery_request_t));

  TRACE_MSG(TRACE_ZDO3, "bdb_network_steering_start_scan param %hd", (FMT__H, param));
  /* MMDEVSTUBS */

#if defined ZB_SUB_GHZ_ZB30_SUPPORT
  {
    zb_uint8_t used_page;

    used_page = zb_aib_channel_page_list_get_first_filled_page();
    zb_channel_page_list_set_mask(req->scan_channels_list, used_page, ZB_BDB().v_scan_channels);
  }
#else
  zb_channel_page_list_set_2_4GHz_mask(req->scan_channels_list, ZB_BDB().v_scan_channels);
#endif /* !ZB_SUB_GHZ_ZB30_SUPPORT */

  req->scan_duration = ZB_BDB().bdb_scan_duration;
  COMM_CTX().discovery_ctx.disc_count = COMM_CTX().discovery_ctx.nwk_scan_attempts;

  /* That finishes in zdo_app.c zb_nlme_network_discovery_confirm */
  ZB_SCHEDULE_CALLBACK(zb_nlme_network_discovery_request, param);
}


void bdb_network_steering_not_on_network(zb_uint8_t param)
{
  if (ZB_BDB().v_do_primary_scan == ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN
      && ZB_BDB().bdb_primary_channel_set != 0
      && !ZB_IS_DEVICE_ZC())
  {
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START;
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;

#ifdef ZB_REJOIN_BACKOFF
    if (zb_zdo_rejoin_backoff_is_running() && (ZDO_CTX().zdo_rejoin_backoff.rjb_cnt == 1) && !ZB_BDB().v_scan_channels)
    {
      /* the first attempt - do secure rejoin using the current channel */
      ZB_BDB().v_scan_channels = (1l << ZB_PIBCACHE_CURRENT_CHANNEL());
      /* TRICKY: Set primary_scan again - we will do 2 PRIMARY_SCANs (first on current channel,
       * second on bdb_primary_channel_set), then SECONDARY_SCAN.  */
      ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN;
    }
    else
    {
      /* We done rejoin_backoff specific attempt, now reset v_scan_channels and use usual scheme
       * (bdb_primary_channel_set, then bdb_secondary_channel_set). */
      ZB_BDB().v_scan_channels = 0;

      /* the second and further attempts - do secure rejoin using ALL channels */

      /* 01/27/2017 EE Why having already 2 channel sets we introduce
         another one - rejoin backoff channels mask? Can't we go thru
         primary/secondary like we do for usual start? ok, after first attempt at
         the current channel.
         Do we need default_channel_mask parameter in zb_zdo_rejoin_backoff_start?
         Isn't product_cfg.aps_channel_mask == bdb_primary_channel_set?
      */
      /* NK: Seems like it is ok. Lets update bdb_channel_mask on rejoin_backoff start instead of
       * using rjb_chan_mask (in the case when application wants to use custom channel mask for rjb).
       *
       * Currently will not remove rjb_chan_mask because it is used in legacy (non-BDB)
       * variant. It is a hack because in this variant we overwrite original
       * ZB_AIB().aps_channel_mask on 1st rjb attempt, then restore it from rjb_chan_mask (in all known
       * cases production_mask == ZB_AIB().aps_channel_mask == rjb_chan_mask).
       * Not sure about supporting legacy (non-BDB) rejoin_backoff, for me it looks like it may be
       * completely removed.
       */

      /* Implement insecure rejoin via BDB if needed. */
    }
#endif

    if (!ZB_BDB().v_scan_channels)
    {
      /* 01/30/2017 EE  In case of rejoin backoff you substitute primary
      channel set by current channel, then use secondary channel set.
      Not sure this is what you want to do.
      Probably need to go in 2 steps: current, primary, secondary.
      As an alternative (simpler, but a bit further from BDB : current, primary | secondary.
      BTW I do not know why they ever invented primary and secondary...
      */
      ZB_BDB().v_scan_channels = ZB_BDB().bdb_primary_channel_set;
    }
    TRACE_MSG(TRACE_ZDO1, "Doing primary scan channel mask 0x%lx", (FMT__L, ZB_BDB().v_scan_channels));
    /* call directly to simplify cancellation logic */
    bdb_network_steering_start_scan(param);
  }
  else if (!ZB_IS_DEVICE_ZC()
           && ((ZB_BDB().v_do_primary_scan == ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START
                && ZB_BDB().bdb_secondary_channel_set != 0)
               ||
               (ZB_BDB().v_do_primary_scan == ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN
                && ZB_BDB().bdb_secondary_channel_set != 0
                && ZB_BDB().bdb_primary_channel_set == 0)))
  {
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_DONE;
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_BDB().v_scan_channels = ZB_BDB().bdb_secondary_channel_set;
    TRACE_MSG(TRACE_ZDO1, "Doing secondary scan channel mask 0x%lx", (FMT__L, ZB_BDB().v_scan_channels));
    /* call directly to simplify cancellation logic */
    bdb_network_steering_start_scan(param);
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
    TRACE_MSG(TRACE_ZDO1, "BDB: Could not join or authorize - run next bdb machine step", (FMT__0));
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN;
    bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
  }

/* TODO (ZBS-429): sync the channel mask values in ZDO that will be used for retries */
#ifndef NCP_MODE_HOST
  zb_channel_page_list_set_mask(ZB_AIB().aps_channel_mask_list, 0, ZB_BDB().v_scan_channels);
#endif /* NCP_MODE_HOST */

}
#endif  /* ZB_JOIN_CLIENT */

void bdb_network_steering_finish(zb_uint8_t param)
{
  bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
}

void bdb_network_steering_on_network(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO3, "bdb_network_steering_on_network param %hd", (FMT__H, param));
  /*
    Send Permit Joining.

    If I am ZR or ZC, open myself for connects (permit joining).
   */
  {
    /*
  Continues EZ-Mode Nwk steering after network has been formed or device has joined to the nwk:
  invokes permit_joining routines for ZC & ZR, pushes the results of Nwk steering up
  via user callback for ZED
*/
    /* Send mgmt permit joining broadcast */
    zb_zdo_mgmt_permit_joining_req_param_t *req_param = ZB_BUF_GET_PARAM(param, zb_zdo_mgmt_permit_joining_req_param_t);

    ZB_BZERO(req_param, sizeof(zb_zdo_mgmt_permit_joining_req_param_t));
    req_param->dest_addr = ZB_NWK_BROADCAST_ROUTER_COORDINATOR;
    req_param->permit_duration = ZB_BDBC_MIN_COMMISSIONING_TIME_S;
    req_param->tc_significance = ZB_TRUE;
    zb_zdo_mgmt_permit_joining_req(param, bdb_after_mgmt_permit_joining_cb);
  }
}

/*
  Called when broadcasting PermitJoining finished
  during EZ-Mode Nwk Steering for ZC or ZR:
  schedules local nlme.permit_joining request
*/
void bdb_after_mgmt_permit_joining_cb(zb_uint8_t param)
{

  TRACE_MSG(TRACE_ZDO2, "> bdb_after_mgmt_permit_joining_cb %hd", (FMT__H, param));

  if (zb_buf_get_status(param) == RET_OK)
  {
#ifdef ZB_ROUTER_ROLE
    if (ZB_IS_DEVICE_ZC_OR_ZR())
    {
      zb_zdo_mgmt_permit_joining_req_param_t *req = ZB_BUF_GET_PARAM(param,
                                                                           zb_zdo_mgmt_permit_joining_req_param_t);

      ZB_BZERO(req, sizeof(zb_zdo_mgmt_permit_joining_req_param_t));
      req->permit_duration = ZB_BDBC_MIN_COMMISSIONING_TIME_S;
      req->dest_addr = ZB_PIBCACHE_NETWORK_ADDRESS();

      zb_zdo_mgmt_permit_joining_req(param, bdb_network_steering_finish);
      param = 0;
    }
#endif
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NOT_PERMITTED;
  }

  if (param)
  {
    bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
  }

  TRACE_MSG(TRACE_ZCL2, "< bdb_after_mgmt_permit_joining_cb", (FMT__0));
}

static void bdb_network_steering_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_network_steering_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));
  switch (BDB_COMM_CTX().signal)
  {
    case BDB_COMM_SIGNAL_NETWORK_STEERING_START:
    {
      if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_NETWORK_STEERING)
      {
        /* TODO: Fix this correctly after discussion about signals. */
        if (ZB_BDB().bdb_commissioning_mode == ZB_BDB_NETWORK_STEERING)
        {
          ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_STEERING;
        }

        /* For ZC steering means "open net for join". Meaningful only after formation complete. */
        if (!ZB_IS_DEVICE_ZC() || zb_zdo_joined())
        {
          TRACE_MSG(TRACE_ZDO1, "Start Network Steering", (FMT__0));
          if (ZB_BDB().bdb_op_cancelled == ZB_TRUE)
          {
            TRACE_MSG(TRACE_ZDO1, "steering cancelled", (FMT__0));
            ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_CANCELLED;
            bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
          }
          else
          {
            /* call directly to simplify cancellation logic */
            bdb_network_steering_start(param);
          }
        }
        else
        {
          TRACE_MSG(TRACE_ZDO1, "Skip Network Steering: meaningless before formation", (FMT__0));
          bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
        }
      }
      else
      {
        TRACE_MSG(TRACE_ZDO1, "Skip Network Steering", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
      }
    }
    break;

    case BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED:
      if (zb_buf_get_status(param) == RET_INTERRUPTED)
      {
        TRACE_MSG(TRACE_ZDO1, "discovery was cancelled", (FMT__0));
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_CANCELLED;
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
        break;
      }

      if(ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_IN_PROGRESS)
      {
        /* If commisioning is in progress, steering state machine should handle NWK discovery failed signal,
        since a secondary channel scan is supposed to happen after a failed scan of primary channels */
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_START, param);
        break;
      }

      /* FALLTHROUGH */
    case BDB_COMM_SIGNAL_NWK_JOIN_FAILED:
      TRACE_MSG(TRACE_ZDO1, "No networks found", (FMT__0));
      ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_NO_NETWORK;
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
      break;

#ifndef ZB_COORDINATOR_ONLY
    case BDB_COMM_SIGNAL_NWK_AUTH_FAILED:
      ZB_SCHEDULE_CALLBACK(bdb_network_steering_auth_failure, param);
      break;

    case BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_EX_FAILURE:
      ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_TCLK_EX_FAILURE;
      ZB_SCHEDULE_CALLBACK(bdb_network_steering_tclk_ex_failure, param);
      break;
#endif /* !ZB_COORDINATOR_ONLY */

    case BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF:
    case BDB_COMM_SIGNAL_NWK_JOIN_DONE:
#ifdef ZB_JOIN_CLIENT
      if (!zdo_secur_waiting_for_tclk_update())
      {
        /* Join is finished - can reset the blacklist. */
        TRACE_MSG(TRACE_ZDO1, "Router started/ED join done, TCLK is not needed - continue", (FMT__0));
        ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
        /* Join is finished - can reset the blacklist. */
#ifdef ZB_NWK_BLACKLIST
        zb_nwk_blacklist_reset();
#endif
        /* TRICKY: Run steering one more time - to broadcast Permit Joining. This time it will be
         * steering for device on network. */
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_START, param);
      }
      else
#endif
      {
        TRACE_MSG(TRACE_ZDO1, "Router started/ED join done, wait for TCLK", (FMT__0));
        zb_buf_free(param);
      }
      break;

    case BDB_COMM_SIGNAL_LEAVE_DONE:
      if (ZB_BDB().bdb_commissioning_status != ZB_BDB_STATUS_NO_NETWORK)
      {
        /* Probably bdb device failed to update tclk exchange */
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH, param);
      }
      else
      {
        /* Will be here after bdb device failed authentication - lets join to
         * other networks */
#ifdef ZB_JOIN_CLIENT
        ZB_SCHEDULE_CALLBACK(zdo_retry_joining, 0);
#endif
      }
      break;

    case BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_DONE:
      TRACE_MSG(TRACE_ZDO1, "TCLK succeeded", (FMT__0));
      ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
      /* Join is finished - can reset the blacklist. */
#ifdef ZB_NWK_BLACKLIST
      zb_nwk_blacklist_reset();
#endif
      /* TRICKY: Run steering one more time - to broadcast Permit Joining. This time it will be
       * steering for device on network. */
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_START, param);
      break;

    case BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH:
      /* Do not stop even if steering is failed - may continue with Formation if it is allowed
       * (Distributed ZR case) */

      /* if (ZB_BDB().bdb_commissioning_status != ZB_BDB_STATUS_IN_PROGRESS) */
      /* { */
      /*   ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP; */
      /*   bdb_commissioning_signal(BDB_COMM_SIGNAL_FINISH, param); */
      /* } */
      /* else */
      {
        /* Go to next commissioning step - Network Steering procedure */
        TRACE_MSG(TRACE_ZDO1, "Network Steering finished, next step - Network Formation", (FMT__0));
        ZB_BDB().bdb_commissioning_step = ZB_BDB_NETWORK_FORMATION;
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_FORMATION_START, param);
      }
      break;

    default:
      TRACE_MSG(TRACE_ERROR, "Unknown signal %d", (FMT__D, BDB_COMM_CTX().signal));
#ifdef BDB_COMMISSIONING_DEBUG
      ZB_ASSERT(0);
#else
      if (param)
      {
        zb_buf_free(param);
      }
#endif
      break;
  }
}


static void bdb_network_formation_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_network_formation_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));
  switch (BDB_COMM_CTX().signal)
  {
    case BDB_COMM_SIGNAL_NETWORK_FORMATION_START:
    {
      if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_NETWORK_FORMATION)
      {
        /* TODO: Fix this correctly after discussion about signals. */
        if (ZB_BDB().bdb_commissioning_mode == ZB_BDB_NETWORK_FORMATION)
        {
          ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_FORMATION;
        }

#ifdef ZB_FORMATION
        if (bdb_can_do_network_formation())
        {
          TRACE_MSG(TRACE_ZDO1, "Start Network Formation", (FMT__0));
          TRACE_MSG(TRACE_ZDO1, "Start BDB formation for device type %d", (FMT__D, zb_get_device_type()));
          if (ZB_BDB().bdb_op_cancelled == ZB_TRUE)
          {
            TRACE_MSG(TRACE_ZDO1, "formation cancelled", (FMT__0));
            ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_CANCELLED;
            bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH, param);
          }
          else
          {
            ZB_ASSERT(FORMATION_SELECTOR().start_formation != NULL);
            /* call directly to simplify cancellation logic */
            FORMATION_SELECTOR().start_formation(param);
          }
        }
        else
#endif /* ZB_FORMATION */
        {
          TRACE_MSG(TRACE_ZDO1, "Can not do Network Formation, skip", (FMT__0));
          bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH, param);
        }
      }
      else
      {
        TRACE_MSG(TRACE_ZDO1, "Skip Network Formation", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH, param);
      }
    }
    break;

    case BDB_COMM_SIGNAL_NWK_FORMATION_OK:
      TRACE_MSG(TRACE_ZDO1, "Network is successfully formed", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH:
    default:
      if (ZB_BDB().bdb_commissioning_status != ZB_BDB_STATUS_IN_PROGRESS)
      {
        ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
        TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_FINISH, param);
      }
      else
      {
        TRACE_MSG(TRACE_ZDO1, "Network Formation finished, next step - Finding & Binding", (FMT__0));
        /* Go to next commissioning step - finish */
#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
        ZB_BDB().bdb_commissioning_step = ZB_BDB_FINDING_N_BINDING;
        bdb_commissioning_signal(BDB_COMM_SIGNAL_FINDING_N_BINDING_START, param);
#endif
      }
      break;
  }
}


#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
static void bdb_finding_n_binding_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_finding_n_binding_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));
  switch (BDB_COMM_CTX().signal)
  {
    case BDB_COMM_SIGNAL_FINDING_N_BINDING_START:
    {
      /* if (ZB_BDB().bdb_commissioning_mode & ZB_BDB_FINDING_N_BINDING) */
      /* { */
      /*   TRACE_MSG(TRACE_ZDO1, "TODO: Start Finding & Binding", (FMT__0)); */
      /* } */
      /* else */
      {
        TRACE_MSG(TRACE_ZDO1, "Skip Finding & Binding", (FMT__0));
        bdb_commissioning_signal(BDB_COMM_SIGNAL_FINDING_N_BINDING_FINISH, param);
      }
    }
    break;

    case BDB_COMM_SIGNAL_FINDING_N_BINDING_FINISH:
      TRACE_MSG(TRACE_ZDO1, "Finding & Binding finished, finish BDB commissioning", (FMT__0));
      /* Go to next commissioning step - finish */
      ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
      TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_LEAVE_DONE:
      TRACE_MSG(TRACE_ZDO1, "Leave done, abort BDB commissioning", (FMT__0));

      switch (BDB_COMM_CTX().state)
      {
        case ZB_BDB_COMM_IDLE:
          /* Do nothing in this case, BDB_COMM_CTX should be already reset */
          break;

        case ZB_BDB_COMM_FINDING_AND_BINDING:
          zb_bdb_finding_binding_initiator_cancel();
          break;

        case ZB_BDB_COMM_FINDING_AND_BINDING_TARGET:
          zb_bdb_finding_binding_target_cancel();
          break;

        default:
          /* NOTE: other COMM states are not currently used, so handle them when it will be needed */
          ZB_ASSERT(ZB_FALSE);
          break;
      }

      break;

    default:
      TRACE_MSG(TRACE_ERROR, "Unknown signal %d", (FMT__D, BDB_COMM_CTX().signal));

      TRACE_MSG(TRACE_ERROR, "Ignore (F&B implemented separately)", (FMT__0));
      if (param)
      {
        zb_buf_free(param);
      }
      break;
  }
}
#endif

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
static zb_bool_t bdb_ds_filter_cb(zb_uint16_t i)
{
  switch (i)
  {
    case ZB_NVRAM_COMMON_DATA:
      return ZB_TRUE;
    default:
      return ZB_FALSE;
  }
}

/* Savepoint logic for SE R&R and WWAH Rejoin could be unified and moved into a separate module.
   For NCP case it could be put at SoC side */
static void bdb_savepoint_before_rr()
{
  TRACE_MSG(TRACE_ZDO1, "bdb_savepoint_before_rr", (FMT__0));

  BDB_COMM_CTX().rejoin.rr_sv_authenticated = ZB_B2U(ZG->aps.authenticated);
  BDB_COMM_CTX().rejoin.rr_sv_device_type = zb_get_device_type();

  /* Save parent info. We do not want to reload it from nvram because neighbor table load supposes clear neighbor table. */
  BDB_COMM_CTX().rejoin.rr_sv_parent_short = 0xffff;
  if (ZG->nwk.handle.parent != (zb_uint8_t)-1)
  {
    zb_neighbor_tbl_ent_t *nent = NULL;
    zb_uint16_t addr = BDB_COMM_CTX().rejoin.rr_sv_parent_short;
    zb_address_by_ref(BDB_COMM_CTX().rejoin.rr_sv_parent_long, &addr, ZG->nwk.handle.parent);
    if (zb_nwk_neighbor_get(ZG->nwk.handle.parent, ZB_FALSE, &nent) == RET_OK)
    {
      BDB_COMM_CTX().rejoin.rr_sv_parent_nent = *nent;
    }
  }
  /* All other information is now in our nvram. We can restore it from there. */
}

static void bdb_restore_cont(zb_uint8_t param)
{
  /* Undo effect of zdo_rejoin_clear_prev_join(). */
  /* If ZED, start poll again */
  if (ZB_IS_DEVICE_ZED())
  {
    if (!ZB_U2B(ZB_PIBCACHE_RX_ON_WHEN_IDLE()))
    {
      zb_zdo_pim_start_poll(0);
    }
    zb_buf_free(param);
  }
  else
  {
    BDB_COMM_CTX().rejoin.rr_ignore_start_router_conf = 1;
    ZB_SCHEDULE_CALLBACK(zb_zdo_start_router, param);
  }
}

static void bdb_restore_saved_rr(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_restore_saved_rr param %hd", (FMT__H, param));

#ifdef ZB_USE_NVRAM
  /* get common dataset from nvram */
  ZB_NVRAM().ds_filter_cb = bdb_ds_filter_cb;
  zb_nvram_load();
  ZB_NVRAM().ds_filter_cb = NULL;
#endif

  if (BDB_COMM_CTX().rejoin.rr_sv_parent_short != 0xffff)
  {
    /* Create and lock parent */
    zb_address_update(BDB_COMM_CTX().rejoin.rr_sv_parent_long, BDB_COMM_CTX().rejoin.rr_sv_parent_short, ZB_FALSE, &ZG->nwk.handle.parent);
  }
  else
  {
    /* Restore ZG->nwk.handle.parent from BDB_COMM_CTX().rejoin.rr_sv_parent_long */
    zb_address_by_ieee(BDB_COMM_CTX().rejoin.rr_sv_parent_long, ZB_FALSE, ZB_FALSE, &ZG->nwk.handle.parent);
  }

  {
    zb_neighbor_tbl_ent_t *nent = NULL;

    /* TODO: [Multi-MAC] set iface_id */
    if (zb_nwk_neighbor_get(ZG->nwk.handle.parent, ZB_TRUE, &nent) == RET_OK)
    {
      nent->relationship = ZB_NWK_RELATIONSHIP_PARENT;
    }
  }

  {
    zb_address_ieee_ref_t ref;
    zb_address_update(ZB_PIBCACHE_EXTENDED_ADDRESS(),
                      ZB_PIBCACHE_NETWORK_ADDRESS(),
                      ZB_TRUE, &ref);
  }
  ZB_NIB().device_type = (zb_nwk_device_type_t)BDB_COMM_CTX().rejoin.rr_sv_device_type;
  ZG->aps.authenticated = ZB_U2B(BDB_COMM_CTX().rejoin.rr_sv_authenticated);
  ZB_SET_JOINED_STATUS(ZB_TRUE);
#ifdef ZB_USE_NVRAM
  zb_nvram_store_addr_n_nbt();
#endif

  /* Now pibcache is filled by right values, but values are not pushed to MAC. Switch MAC channel etc. */
  zb_nwk_sync_pibcache_with_mac(param, bdb_restore_cont);
}
#endif  /* #ifdef ZB_ZCL_ENABLE_WWAH_SERVER */

/* WWAH Rejoin procedure:
   1. Store existing network settings.
   [Continue if Sleepy ED and join was NOT caused by Poll Control Check-in failure, else go to step
   5]
   2. Secure Rejoin on the Current Channel.
   3. TC Rejoin on the Current Channel.
   4. (optional, now skip) TC Rejoin on preferred channel list
   [<<<]
   [Continue if device is using unique TCLK, else go to step 8]
   5. TC Rejoin on all channels
   6. Back to previous network settings and
      Wait for X seconds or until additional triggers (example: user button push)
      a. Non-sleepy ED: X seconds will be equal to the Keepalive cluster value or 24 hours if
      Keepalive cluster is not Implemented
      b. Sleepy ED: if WWAH rejoin algorithm is implemented then hte wait will be determined by the
      WWAH rejoin algorithm, else 15 minutes
   7. Non-sleepy ED or ZR - back to step 3.
   [These steps are only for Sleepy ED]
   8. Secure Rejoin on all channels.
   9. Back to previous network settings and
      Wait for X seconds or until additional triggers
   10. Go to step 1.

   *When performing a rejoin on multiple channels, the device shall first perform a beacon survey on
   the given set of channels, sort the resulting beacons using the WWAH prioritized parent
   algorithm and prefer beacons with a higher network update ID when more than one share the same
   WWAH parent priority.
 */
/* TODO: implement all rejoin triggers for ZR and ZED */
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
static void bdb_retry_rejoin_recovery(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_retry_rejoin_recovery param %hd", (FMT__H, param));
  if (!param)
  {
    zb_buf_get_out_delayed(bdb_retry_rejoin_recovery);
  }
  else
  {
    BDB_COMM_CTX().rejoin.waiting = 0;
    bdb_commissioning_signal(BDB_COMM_CTX().rejoin.next_rejoin_signal, param);
  }
}

static void bdb_rejoin_machine(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_rejoin_machine param %hd signal %hd state %hd",
            (FMT__H_H_H, param, BDB_COMM_CTX().signal, ZB_BDB().bdb_commissioning_step));

  switch (BDB_COMM_CTX().signal)
  {
    case BDB_COMM_SIGNAL_REJOIN_START:
    {
      TRACE_MSG(TRACE_ZDO1, "Start Rejoin", (FMT__0));

      /* Double-check this should be stopped during Rejoin */
      zb_zcl_wwah_stop_periodic_checkin();
      zb_zcl_wwah_stop_bad_parent_recovery();

      if (!BDB_COMM_CTX().rejoin.rr_skip_savepoint)
      {
        bdb_savepoint_before_rr();
      }

      BDB_COMM_CTX().rejoin.rr_have_unique_tclk = (zb_secur_get_link_key_by_address(ZB_PIBCACHE_EXTENDED_ADDRESS(), ZB_SECUR_VERIFIED_KEY) != NULL);

#ifdef ZB_NWK_BLACKLIST
      zb_nwk_blacklist_reset();
#endif

      if (!ZB_U2B(ZB_PIBCACHE_RX_ON_WHEN_IDLE()) &&
          !BDB_COMM_CTX().rejoin.rejoin_by_checkin_failure)
      {
        bdb_commissioning_signal(BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL, 0);
        BDB_COMM_CTX().rejoin.last_rejoin_signal = BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL;
      }
      else
      {
        bdb_commissioning_signal(BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS, 0);
        BDB_COMM_CTX().rejoin.last_rejoin_signal = BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS;
      }

      /* Inform application that rejoin is started */
      zb_app_signal_pack(param, ZB_BDB_SIGNAL_WWAH_REJOIN_STARTED,
                         RET_OK,
                         0);
      ZB_SCHEDULE_CALLBACK(zb_zdo_startup_complete_int, param);
    }
    break;

    case BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL:
    case BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL:
    case BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_ALL_CHANNELS:
    case BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS:
    {
      zb_channel_list_t channel_list;
      zb_bool_t current_channel =
        (zb_bool_t)(BDB_COMM_CTX().signal == BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL ||
         BDB_COMM_CTX().signal == BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL);
      zb_bool_t secure =
        (zb_bool_t)(BDB_COMM_CTX().signal == BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL ||
         BDB_COMM_CTX().signal == BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS);

      if (current_channel)
      {
        TRACE_MSG(TRACE_ZDO1, "Try rejoin over current page %hd channel %hd",
                  (FMT__H_H, ZB_PIBCACHE_CURRENT_PAGE(), ZB_PIBCACHE_CURRENT_CHANNEL()));
        zb_channel_list_init(channel_list);
        zb_channel_page_list_set_logical_channel(channel_list, ZB_PIBCACHE_CURRENT_PAGE(),
                                                 ZB_PIBCACHE_CURRENT_CHANNEL());
      }
      else
      {
        /* FIXME: do we need to use preferred channels here? */
        TRACE_MSG(TRACE_ZDO1, "Try rejoin over all channels", (FMT__0));
        zb_channel_page_list_copy(channel_list, ZB_AIB().aps_channel_mask_list);
      }

      if (secure)
      {
        ZG->aps.authenticated = ZB_TRUE;
      }

      TRACE_MSG(TRACE_ZDO1, "rejoin secure %d", (FMT__D, secure));
      BDB_COMM_CTX().rejoin.last_rejoin_signal = BDB_COMM_CTX().signal;

#if defined ZB_DISTRIBUTED_SECURITY_ON
      if (!secure && IS_DISTRIBUTED_SECURITY())
      {
        /* WWAH spec, 11.4 Parent and Hub Connectivity Recovery Procedure
           Please note that for Distributed security networks, TC Rejoin is not supported. As a
           result, for a device rejoining a Distributed security network, all steps involving TC
           Rejoin shall be skipped.
         */
        /* Skip rejoin by imitation that current step is failed  */
        bdb_commissioning_signal(BDB_COMM_SIGNAL_NWK_JOIN_FAILED, param);
      }
      else
#endif
      {
        zb_ext_pan_id_t ext_pan_id;
        zb_get_extended_pan_id(ext_pan_id);

        zdo_initiate_rejoin(param, ext_pan_id,
                              channel_list,
                              (zb_bool_t)secure);
      }
    }
    break;

    case BDB_COMM_SIGNAL_NWK_AUTH_FAILED:
      TRACE_MSG(TRACE_ZDO1, "Failed NWK authentication", (FMT__0));
      /* FALLTHROUGH */
    case BDB_COMM_SIGNAL_LEAVE_DONE: /* Is it possible? */
    case BDB_COMM_SIGNAL_NWK_JOIN_FAILED:
    {
      zb_bool_t need_to_wait = ZB_FALSE;

      TRACE_MSG(TRACE_ZDO1, "Failed rejoin", (FMT__0));

      switch (BDB_COMM_CTX().rejoin.last_rejoin_signal)
      {
        case BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL: /* Step 2 */
          /* -> Step 3 */
          BDB_COMM_CTX().rejoin.next_rejoin_signal =
            BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL;
          break;
        case BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL: /* Step 3 */
        {
          if (BDB_COMM_CTX().rejoin.rr_have_unique_tclk)
          {
            /* Have unique TCLK: -> Step 5 */
            BDB_COMM_CTX().rejoin.next_rejoin_signal =
              BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_ALL_CHANNELS;
          }
          else
          {
            /* Do not have unique TCLK */
            need_to_wait = ZB_TRUE;

            if (ZB_U2B(ZB_PIBCACHE_RX_ON_WHEN_IDLE()))
            {
              /* RX-ON: Wait, than back to step 3 */
              BDB_COMM_CTX().rejoin.next_rejoin_signal =
                BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL;
            }
            else
            {
              /* Non-sleepy ED: Continue to step 8 */
              BDB_COMM_CTX().rejoin.next_rejoin_signal =
                BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS;
            }
          }
        }
        break;
        case BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_ALL_CHANNELS:  /* Step 5 */
        {
          /* -> Step 6 */
          need_to_wait = ZB_TRUE;

          if (ZB_U2B(ZB_PIBCACHE_RX_ON_WHEN_IDLE()))
          {
            /* Non-sleepy ED: Back to step 3 */
            BDB_COMM_CTX().rejoin.next_rejoin_signal =
              BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL;
          }
          else
          {
            /* Non-sleepy ED: Continue to step 8 */
            BDB_COMM_CTX().rejoin.next_rejoin_signal =
              BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS;
          }
        }
        break;
        case BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS:  /* Step 8 */
        {
          /* -> Step 9-10  */
          need_to_wait = ZB_TRUE;
          BDB_COMM_CTX().rejoin.next_rejoin_signal =
            BDB_COMM_SIGNAL_REJOIN_START;
        }
        break;
        default:
          ZB_ASSERT(0);         /* debug */
          break;
      }

      if (need_to_wait)
      {
        zb_time_t time_to_wait = 0;

        TRACE_MSG(TRACE_ZDO1, "skip savepoint %hd", (FMT__H, !BDB_COMM_CTX().rejoin.rr_skip_savepoint));

        if (ZB_U2B(ZB_PIBCACHE_RX_ON_WHEN_IDLE()))
        {
          /* FIXME: TODO: remove direct access to WWAH ctx from commissioning level! */
          /* For Routers and Non-Sleepy End Devices, X seconds will be equal to the Keepalive
           * cluster value of 24 hours if Keepalive cluster is not implemented. */
          if (WWAH_CTX().periodic_checkins.poll_method == ZB_ZCL_WWAH_PERIODIC_CHECKINS_READ_KEEPALIVE)
          {
            time_to_wait = (WWAH_CTX().periodic_checkins.keepalive_base +
                            ZB_RANDOM_VALUE(WWAH_CTX().periodic_checkins.keepalive_jitter)) * ZB_TIME_ONE_SECOND;
          }
          else
          {
            time_to_wait = ZB_ZCL_WWAH_REJOIN_NON_SLEEPY_ED_ROUTER_TIMEOUT;
          }
        }
        else
        {
          ++BDB_COMM_CTX().rejoin.rr_global_retries;

#ifdef ZB_JOIN_CLIENT
          if (zb_zcl_wwah_get_rejoin_tmo(BDB_COMM_CTX().rejoin.rr_global_retries, &time_to_wait) == RET_EXIT)
          {
            /* Wait for next Full Rejoin cycle */
            BDB_COMM_CTX().rejoin.rr_global_retries = 0;
          }
#endif
        }

        if (!BDB_COMM_CTX().rejoin.rr_skip_savepoint)
        {
          ZB_SCHEDULE_CALLBACK(bdb_restore_saved_rr, param);
        }
        TRACE_MSG(TRACE_ZDO1, "retry rejoin, time_to_wait=%ld", (FMT__L, time_to_wait));
        BDB_COMM_CTX().rejoin.waiting = 1;
        ZB_SCHEDULE_ALARM(bdb_retry_rejoin_recovery, 0, time_to_wait);
      }
      else
      {
        ZB_SCHEDULE_CALLBACK(bdb_retry_rejoin_recovery, param);
      }
    }
    break;

    case BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF:
      if (BDB_COMM_CTX().rejoin.waiting)
      {
        /* Ignore START_ROUTER_CONF when it is triggered by bdb_restore_saved_rr(). */
        break;
      }
      /* FALLTHROUGH */
    case BDB_COMM_SIGNAL_NWK_JOIN_DONE:
      TRACE_MSG(TRACE_ZDO1, "Device is joined", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_REJOIN_FINISH, param);
      break;

    case BDB_COMM_SIGNAL_REJOIN_FINISH:
      TRACE_MSG(TRACE_ZDO1, "Rejoin finished, finish BDB commissioning", (FMT__0));
      /* Go to next commissioning step - finish */
      ZB_BDB().bdb_commissioning_step = ZB_BDB_COMMISSIONING_STOP;
      TRACE_MSG(TRACE_ZDO3, "ZB_BDB_COMMISSIONING_STOP", (FMT__0));
      bdb_commissioning_signal(BDB_COMM_SIGNAL_FINISH, param);
      break;

    default:
      TRACE_MSG(TRACE_ERROR, "Unknown signal %d", (FMT__D, BDB_COMM_CTX().signal));

      TRACE_MSG(TRACE_ERROR, "Ignore (F&B implemented separately)", (FMT__0));
      if (param)
      {
        zb_buf_free(param);
      }
      break;
  }
}

#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */

void bdb_start_rejoin_recovery(zb_uint8_t param, zb_uint16_t user_param)
{
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
  TRACE_MSG(TRACE_ZDO1, ">> bdb_start_rejoin_recovery: param %hd user_param %d", (FMT__H_D, param, user_param));
  if (!param)
  {
    zb_buf_get_out_delayed_ext(bdb_start_rejoin_recovery, user_param, 0);
  }
  else
  {
    zb_uint8_t rejoin_reason = ZB_GET_LOW_BYTE(user_param);

    BDB_COMM_CTX().rejoin.rejoin_by_checkin_failure =
      (rejoin_reason == BDB_COMM_REJOIN_REASON_POLL_CONTROL_CHECK_IN);

    BDB_COMM_CTX().rejoin.rr_global_retries = 0;

    ZB_BDB().bdb_commissioning_step = ZB_BDB_REJOIN;
    bdb_commissioning_signal(BDB_COMM_SIGNAL_REJOIN_START, param);
  }
  TRACE_MSG(TRACE_ZDO1, "<< bdb_start_rejoin_recovery", (FMT__0));
#else
  ZVUNUSED(user_param);
  TRACE_MSG(TRACE_ERROR, "bdb_start_rejoin_recovery: not supported!", (FMT__0));
  if (param)
  {
    zb_buf_free(param);
  }
#endif
}

#ifdef ZB_JOIN_CLIENT

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
void schedule_wwah_rejoin_backoff_attempt(zb_uint8_t param)
{
  if (!param)
  {
    zb_buf_get_out_delayed(schedule_wwah_rejoin_backoff_attempt);
  }
  else
  {
    zb_time_t backoff_interval = 0;
    zb_uint8_t *rejoin_reason = NULL;

    TRACE_MSG(TRACE_ZDO1, ">>schedule_wwah_rejoin_backoff_attempt", (FMT__0));

    TRACE_MSG(TRACE_ZDO1, "WWAH rejoin is enabled, scheduling next rejoin attempt", (FMT__0));

    ++BDB_COMM_CTX().rejoin.rr_global_retries;
    if (zb_zcl_wwah_get_rejoin_tmo(BDB_COMM_CTX().rejoin.rr_global_retries, &backoff_interval) == RET_EXIT)
    {
      BDB_COMM_CTX().rejoin.rr_global_retries = 0;
    }

    TRACE_MSG(TRACE_ZDO1, "Next rejoin attempt in %hd", (FMT__H_H, backoff_interval));
    rejoin_reason = ZB_BUF_GET_PARAM(param, zb_uint8_t);
    *rejoin_reason = ZB_REJOIN_REASON_BACKOFF_REJOIN;
    ZB_SCHEDULE_ALARM(zdo_commissioning_initiate_rejoin, param, backoff_interval);

    TRACE_MSG(TRACE_ZDO1, "<<schedule_wwah_rejoin_backoff_attempt", (FMT__0));
  }
}
#endif

static void bdb_handle_join_failed_signal(zb_bufid_t param)
{
  TRACE_MSG(TRACE_ZDO1, "{re}join failed - call BDB machine", (FMT__0));
  /* Note: there was a check for BDB * NFN and call to bdb machine in that case only.
     See original zb_nlme_join_confirm() code.
     TODO: in BDB machine implement rejoin re-attempt vs association */
  /* NK: Looks like this is already covered in BDB machine - if device is NFN, control will be
   * passed to application (signal with status NO_NETWORK). Not sure if we need to implement
   * some additional logic in BDB machine itself - application may decide if it wants to do
   * rejoin backoff (for example), or silently go to sleep, or reset to factory defaults
   * etc. */

  if (COMM_CTX().discovery_ctx.scanlist_ref)
  {
    zb_buf_free(param);
    ZB_SCHEDULE_CALLBACK(zdo_retry_joining, 0);
  }
  else
  {
    bdb_commissioning_signal(BDB_COMM_SIGNAL_NWK_JOIN_FAILED, param);
  }
}


static void bdb_handle_initiate_rejoin_signal(zb_bufid_t param)
{
  zb_buf_free(param);

  if (!zb_zdo_is_rejoin_active())
  {
    zdo_rejoin_clear_prev_join();
    ZB_BDB().bdb_force_router_rejoin = ZB_TRUE;
    bdb_start_top_level_commissioning(ZB_BDB_NETWORK_STEERING);

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
    if (!ZB_PIBCACHE_RX_ON_WHEN_IDLE()
        && zb_zcl_wwah_check_if_wwah_rejoin_enabled())
    {
      schedule_wwah_rejoin_backoff_attempt(0);
    }
#endif
  }
}


static void bdb_handle_leave_done_signal(zb_bufid_t param)
{
  /* In r21 all case changed to  bdb_commissioning_leave_done().
   * But it is not 100% equivalent.
   * There is no zdo_retry_joining() call but is zb_zdo_reset_inform_user_app() call.
   * Let's leave r22 behavior for now, but not sure it has no problems which fixed in r21. */

  /* VP: Moved from zdo (zb_nlme_reset_confirm) to prevent races between reset_confirm_cb
   * and bdb_commissioning_machine. */

  /* FIXME: not very good check, but anyway. We need to differentiate 2 cases:
   * 1st case: join is not complete
   * 2nd case: join is complete and control was already passed to application
   * If join is not complete (key exchange failed etc), schedule bdb iterations etc.
   * If join is already completed, application may already initiate new steering etc, so do not
   * touch bdb at all. */

  /* TODO: Send special ZDO signal (LEAVE_DONE or so) - it will allow to avoid manual join
   * restarting after leave (from application). */
  if (ZB_BDB().bdb_commissioning_step == ZB_BDB_COMMISSIONING_STOP)
  {
    TRACE_MSG(TRACE_NWK1, "BDB commissioning machine is already finished, do nothing",
              (FMT__0));
  }
  else
  {
    TRACE_MSG(TRACE_NWK1, "Scheduling BDB after leave step %d status %d",
              (FMT__D_D, ZB_BDB().bdb_commissioning_step, ZB_BDB().bdb_commissioning_status));
    if (ZB_BDB().bdb_commissioning_status != ZB_BDB_STATUS_NO_NETWORK)
    {
      /* Probably bdb device failed to update tclk exchange */
      bdb_commissioning_signal(BDB_COMM_SIGNAL_LEAVE_DONE, 0);
    }
    else
    {
      /* Will be here after bdb device failed authentication - lets join to
       * other networks */
      ZB_SCHEDULE_CALLBACK(zdo_retry_joining, 0);
    }
  }

  if (param != ZB_BUF_INVALID)
  {
    zb_buf_free(param);
  }
}


#ifndef NCP_MODE_HOST

static void bdb_handle_auth_ok_signal(zb_bufid_t param)
{
  /* TODO: Move this part to BDB commissioning machine: have some variations depending of the
   * state when this signal is generated (initialization/TC rejoin/steering/etc) */
  if (!IS_DISTRIBUTED_SECURITY()
      && ZB_TCPOL().update_trust_center_link_keys_required
      && !ZB_TCPOL().waiting_for_tclk_exchange) /* TCLK is not already in progress */
  {
    zb_uint16_t aps_key_idx = (zb_uint16_t)-1;

    if (!ZB_IEEE_ADDR_IS_ZERO(ZB_AIB().trust_center_address))
    {
      aps_key_idx = zb_aps_keypair_get_index_by_addr(ZB_AIB().trust_center_address,
                                                     ZB_SECUR_VERIFIED_KEY);
    }
    if (aps_key_idx == (zb_uint16_t)-1)
    {
      TRACE_MSG(TRACE_SECUR3, "BDB & !distributed - get TCLK over APS", (FMT__0));
      zdo_initiate_tclk_gen_over_aps(0);
    }
  }

  if (param != ZB_BUF_INVALID)
  {
    zb_buf_free(param);
  }
}


static void bdb_handle_rejoin_after_secur_failed_signal(zb_bufid_t param)
{
  zb_ext_pan_id_t ext_pan_id;
  /* rejoin to current pan */
  zb_get_extended_pan_id(ext_pan_id);
  zdo_initiate_rejoin(param, ext_pan_id, ZB_AIB().aps_channel_mask_list, ZB_FALSE);
}

#endif /* !NCP_MODE_HOST */


static void bdb_handle_leave_with_rejoin_signal(zb_bufid_t param)
{
#ifndef NCP_MODE_HOST
  zb_uint8_t *rejoin_reason = NULL;
#endif
  /* [MM]: 01/15/2018: Left this logic unchanged after
   * refactoring. However, explained above is tricky and is required to be
   * checked */

  /* VP: currently we have both bdb join/rejoin and old-style join/rejoin logic and they are mixed.
     In some case BDB device receives nwk leave (with rejoin in our case), then performing usual rejoin (without using BDB machine),
     after join failure/success stack uses BDB commissioning machine to complete join process. This leads to calling bdb_commissioning_machine
     without reset it's logic - then stack silently completes process without notifying application or further join attempts.
     Temporarily fix: use BDB logic for rejoin on receiving leave (locally or remotely).
  */
  zdo_inform_app_leave(ZB_NWK_LEAVE_TYPE_REJOIN);

#ifndef NCP_MODE_HOST
  rejoin_reason = ZB_BUF_GET_PARAM(param, zb_uint8_t);
  *rejoin_reason = ZB_REJOIN_REASON_LEAVE_WITH_REJOIN;
  ZB_SCHEDULE_CALLBACK(zdo_commissioning_initiate_rejoin, param);
#else
  zb_buf_free(param);
#endif

#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
  if (!ZB_PIBCACHE_RX_ON_WHEN_IDLE()
      && zb_zcl_wwah_check_if_wwah_rejoin_enabled())
  {
    BDB_COMM_CTX().rejoin.rr_global_retries = 0;
  }
#endif
}

#endif /* ZB_JOIN_CLIENT */


#ifndef NCP_MODE_HOST
#ifdef ZB_COORDINATOR_ROLE

static void bdb_handle_secured_rejoin_signal(zb_bufid_t param)
{
  zb_apsme_transport_key_req_t *req = ZB_BUF_GET_PARAM(param, zb_apsme_transport_key_req_t);
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
      && !zb_secur_get_link_key_by_address(req->dest_address.addr_long, ZB_SECUR_VERIFIED_KEY))
  {
    ZB_SCHEDULE_ALARM(bdb_link_key_refresh_alarm, param, ZB_TCPOL().trust_center_node_join_timeout);
  }
  else
  {
    zb_buf_free(param);
  }
}


static void bdb_handle_tclk_verified_remote_signal(zb_address_ieee_ref_t param)
{
  ZVUNUSED(param);
}


static void bdb_handle_device_left_signal(zb_address_ieee_ref_t param)
{
  ZVUNUSED(param);
}


#endif /* ZB_COORDINATOR_ROLE */

#endif /* !NCP_MODE_HOST */


#ifdef ZB_FORMATION

static void bdb_handle_formation_failed_signal(zb_bufid_t param)
{
  if (zb_buf_get_status(param) == RET_INTERRUPTED)
  {
    TRACE_MSG(TRACE_ZDO1, "formation was cancelled", (FMT__0));
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_CANCELLED;
    bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH, param);
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_FORMATION_FAILURE;
    ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
  }
}

#endif /* ZB_FORMATION */



static void bdb_handle_comm_signal(zb_commissioning_signal_t signal, zb_bufid_t param)
{
  TRACE_MSG(TRACE_ZDO1, ">> bdb_handle_comm_signal, signal %d, param %d",
            (FMT__D_D, signal, param));

  switch(signal)
  {
    case ZB_COMM_SIGNAL_START:
      bdb_initiate_commissioning(param);
      break;
#ifdef ZB_JOIN_CLIENT
    case ZB_COMM_SIGNAL_NWK_DISC_FAILED:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED, param);
      break;
    case ZB_COMM_SIGNAL_JOIN_FAILED:
      bdb_handle_join_failed_signal(param);
      break;
    case ZB_COMM_SIGNAL_AUTH_FAILED:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NWK_AUTH_FAILED, param);
      break;
    case ZB_COMM_SIGNAL_INITIATE_REJOIN:
      bdb_handle_initiate_rejoin_signal(param);
      break;
    case ZB_COMM_SIGNAL_DEV_ANNCE_SENT:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NWK_JOIN_DONE, param);
      break;
    case ZB_COMM_SIGNAL_ROUTER_STARTED:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF, param);
      break;
    case ZB_COMM_SIGNAL_TCLK_UPDATE_COMPLETE:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_DONE, param);
      break;
    case ZB_COMM_SIGNAL_TCLK_UPDATE_FAILED:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_EX_FAILURE, param);
      break;
    case ZB_COMM_SIGNAL_LEAVE_DONE:
      bdb_handle_leave_done_signal(param);
      break;

#ifndef NCP_MODE_HOST
    case ZB_COMM_SIGNAL_AUTH_OK:
      bdb_handle_auth_ok_signal(param);
      break;
    case ZB_COMM_SIGNAL_SECUR_FAILED:
      bdb_update_tclk_failed(param);
      break;
    case ZB_COMM_SIGNAL_REJOIN_AFTER_SECUR_FAILED:
      bdb_handle_rejoin_after_secur_failed_signal(param);
      break;
#endif /* !NCP_MODE_HOST */

    case ZB_COMM_SIGNAL_LEAVE_WITH_REJOIN:
      bdb_handle_leave_with_rejoin_signal(param);
      break;
#endif /* ZB_JOIN_CLIENT */

#ifndef NCP_MODE_HOST

#ifdef ZB_COORDINATOR_ROLE
    case ZB_COMM_SIGNAL_SECURED_REJOIN:
      bdb_handle_secured_rejoin_signal(param);
      break;
    case ZB_COMM_SIGNAL_TCLK_VERIFIED_REMOTE:
      bdb_handle_tclk_verified_remote_signal(param);
      break;
    case ZB_COMM_SIGNAL_DEVICE_LEFT:
      bdb_handle_device_left_signal(param);
      break;
#endif /* ZB_COORDINATOR_ROLE */

#ifdef ZB_FORMATION
    case ZB_COMM_SIGNAL_AUTHENTICATE_REMOTE:
      zb_buf_free(param);
      break;
#endif /* ZB_FORMATION */

#endif /* !NCP_MODE_HOST */

#ifdef ZB_FORMATION
    case ZB_COMM_SIGNAL_FORMATION_DONE:
      bdb_commissioning_signal(BDB_COMM_SIGNAL_NWK_FORMATION_OK, param);
      break;
    case ZB_COMM_SIGNAL_FORMATION_FAILED:
      bdb_handle_formation_failed_signal(param);
      break;
#endif /* ZB_FORMATION */


    default:
      TRACE_MSG(TRACE_ERROR, "unknown commissioning signal: %d", (FMT__D, signal));
      ZB_ASSERT(0);
      if (param != ZB_BUF_INVALID)
      {
        zb_buf_free(param);
      }
  }

  TRACE_MSG(TRACE_ZDO1, "<< bdb_handle_comm_signal", (FMT__0));
}

#ifdef ZB_ROUTER_ROLE

static zb_uint8_t bdb_get_permit_join_duration(void)
{
  /* The network is closed by default */
  return 0;
}

#if !defined ZB_COORDINATOR_ONLY && !defined NCP_MODE_HOST
static void bdb_handle_no_active_links_left_signal(zb_bufid_t param)
{
  if (ZB_BDB().bdb_commissioning_mode != ZB_BDB_TOUCHLINK_TARGET)
  {
    zb_send_no_active_links_left_signal(param);
  }
}
#endif /* !ZB_COORDINATOR_ONLY && !NCP_MODE_HOST */

#endif  /* #ifdef ZB_ROUTER_ROLE */


#ifndef NCP_MODE_HOST

static zb_bool_t bdb_must_use_installcode(zb_bool_t is_client)
{
  ZVUNUSED(is_client);
#ifdef ZB_SECURITY_INSTALLCODES
  return (zb_bool_t)ZB_TCPOL().require_installcodes;
#else
  return ZB_FALSE;
#endif /* ZB_SECURITY_INSTALLCODES */
}

#ifdef ZB_BDB_TOUCHLINK

static zb_bool_t bdb_should_accept_frame_before_join(zb_bufid_t param)
{
  zb_bool_t ret;

  ZVUNUSED(param);

  TRACE_MSG(TRACE_ZDO2, "bdb_should_accept_frame_before_join, param %d", (FMT__D, param));

  ret = ((ZB_BDB().bdb_commissioning_mode == ZB_BDB_TOUCHLINK_TARGET
          || ZB_BDB().bdb_commissioning_mode == ZB_BDB_TOUCHLINK_COMMISSIONING)
         && ZB_BDB().bdb_commissioning_status == ZB_BDB_STATUS_IN_PROGRESS);

  TRACE_MSG(TRACE_ZDO2, "<< bdb_should_accept_frame_before_join, ret %d", (FMT__D, ret));

  return ret;
}

#endif /* ZB_BDB_TOUCHLINK */

#endif /* !NCP_MODE_HOST */


zb_uint8_t bdb_get_scan_duration(void)
{
  return ZB_BDB().bdb_scan_duration;
}


void zb_bdb_initiate_tc_rejoin(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;

  if (param == ZB_UNDEFINED_BUFFER)
  {
    zb_buf_get_out_delayed(zb_bdb_initiate_tc_rejoin);
    return;
  }

  TRACE_MSG(TRACE_ZDO1, ">> zb_bdb_initiate_tc_rejoin, param %hd", (FMT__H, param));

  if ((!zb_zdo_tclk_valid() || (zb_aib_get_coordinator_version() < 21))
        && !zb_aib_tcpol_get_allow_unsecure_tc_rejoins())
  {
    TRACE_MSG(TRACE_ERROR, "TC rejoin is not allowed", (FMT__0));
    ret = RET_INVALID_STATE;
  }

  if (ret == RET_OK)
  {
    TRACE_MSG(TRACE_ZDO1, "Start TC rejoin", (FMT__0));

    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_BDB().bdb_commissioning_step = ZB_BDB_INITIALIZATION;
    ZB_BDB().bdb_application_signal = ZB_BDB_SIGNAL_TC_REJOIN_DONE;

    bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_TC_REJOIN, param);
  }
  else
  {
    (void)zb_app_signal_pack(param, ZB_BDB_SIGNAL_TC_REJOIN_DONE, ret, 0);
    ZB_SCHEDULE_CALLBACK(zboss_signal_handler, param);
  }

  TRACE_MSG(TRACE_ZDO1, "<< zb_bdb_initiate_tc_rejoin, ret %d", (FMT__D, ret));
}


zb_ret_t zb_bdb_start_secured_rejoin(void)
{
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZDO1, ">> zb_bdb_start_secured_rejoin, prev_comm_signal %hd", (FMT__H, BDB_COMM_CTX().signal));

  ret = BDB_COMM_CTX().signal == BDB_COMM_SIGNAL_NWK_JOIN_FAILED ? RET_OK : RET_INVALID_STATE;

  if (ret == RET_OK)
  {
    TRACE_MSG(TRACE_ZDO2, "Start secured rejoin", (FMT__0));
    bdb_commissioning_signal(BDB_COMM_SIGNAL_INIT_SECURE_REJOIN, ZB_BUF_INVALID);
  }

  TRACE_MSG(TRACE_ZDO1, "<< zb_bdb_start_secured_rejoin, ret %d", (FMT__D, ret));

  return ret;
}


void bdb_force_link(void)
{
  bdb_init();

#if defined ZB_BDB_TOUCHLINK && !defined NCP_MODE_HOST
  ZG->nwk.selector.should_accept_frame_before_join = bdb_should_accept_frame_before_join;
#endif /* ZB_BDB_TOUCHLINK && !NCP_MODE_HOST */
#ifndef NCP_MODE_HOST
  /* introduce some kind of setter? To be solved in the scope of ZBS-241 */
  ZG->nwk.selector.nwk_cancel_nwk_disc_resp = nwk_cancel_network_discovery_response;
#endif

  COMM_SELECTOR().signal = bdb_handle_comm_signal;
  COMM_SELECTOR().is_in_tc_rejoin = bdb_is_in_tc_rejoin;

#ifdef ZB_JOIN_CLIENT
  COMM_SELECTOR().get_scan_duration = bdb_get_scan_duration;
#endif

#ifdef ZB_ROUTER_ROLE
  COMM_SELECTOR().get_permit_join_duration = bdb_get_permit_join_duration;

#if !defined ZB_COORDINATOR_ONLY && !defined NCP_MODE_HOST
  ZG->nwk.selector.no_active_links_left_cb = bdb_handle_no_active_links_left_signal;
#endif /* !ZB_COORDINATOR_ONLY && !NCP_MODE_HOST */

#endif  /* #ifdef ZB_ROUTER_ROLE */

#ifndef NCP_MODE_HOST
  COMM_SELECTOR().must_use_install_code = bdb_must_use_installcode;
#endif /* !NCP_MODE_HOST */
}


#ifdef ZB_ROUTER_ROLE

void zb_set_network_router_role(zb_uint32_t channel_mask)
{
  bdb_force_link();

  zb_set_network_router_role_with_mode(channel_mask, ZB_COMMISSIONING_BDB);
}


void zb_set_network_router_role_ext(zb_channel_list_t channel_list)
{
  bdb_force_link();
  zb_set_nwk_role_mode_common_ext(ZB_NWK_DEVICE_TYPE_ROUTER,
                                  channel_list,
                                  ZB_COMMISSIONING_BDB);
}

#endif /* ZB_ROUTER_ROLE */


#ifdef ZB_ED_FUNC

void zb_set_network_ed_role(zb_uint32_t channel_mask)
{
  bdb_force_link();
  zb_set_network_ed_role_with_mode(channel_mask, ZB_COMMISSIONING_BDB);
}


void zb_set_network_ed_role_ext(zb_channel_list_t channel_list)
{
  bdb_force_link();
  zb_set_nwk_role_mode_common_ext(ZB_NWK_DEVICE_TYPE_ED,
                                  channel_list,
                                  ZB_COMMISSIONING_BDB);
}

#endif /* ZB_ED_FUNC */

/*! \addtogroup zboss_bdb_api */
/*! @{ */

void zb_set_bdb_primary_channel_set(zb_uint32_t channel_mask)
{
  ZB_BDB().bdb_primary_channel_set = channel_mask;
}

zb_uint32_t zb_get_bdb_primary_channel_set()
{
  return ZB_BDB().bdb_primary_channel_set;
}

void zb_set_bdb_secondary_channel_set(zb_uint32_t channel_mask)
{
  ZB_BDB().bdb_secondary_channel_set = channel_mask;
}

zb_uint32_t zb_get_bdb_secondary_channel_set()
{
  return ZB_BDB().bdb_secondary_channel_set;
}

void zb_bdb_set_legacy_device_support(zb_uint8_t enable)
{
  /* WARNING: Turn off link key exchange if legacy device support (<ZB3.0) is needed */
  if ((enable != 0) && (enable != 1))
  {
    ZB_ASSERT(0);
  }
#ifndef ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
  zb_aib_tcpol_set_update_trust_center_link_keys_required((zb_bool_t)!enable);
#endif
}

void zb_set_bdb_commissioning_mode(zb_uint8_t commissioning_mode)
{
  ZB_BDB().bdb_commissioning_mode = commissioning_mode;
}

zb_bool_t zb_bdb_is_factory_new()
{
  return (zb_bool_t)!bdb_joined();
}


#ifndef NCP_MODE_HOST
/* these functions are not supported for NCP now.
   Will be resolved in scope of ZBS-241 */

#ifdef ZB_JOIN_CLIENT

static void bdb_send_steering_cancelled_signal(zb_bufid_t buf, zb_ret_t status)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_send_steering_cancelled_signal, buf %d, status %d",
            (FMT__D_D, buf, status));

  (void)zb_app_signal_pack_with_detailed_status(buf, ZB_BDB_SIGNAL_STEERING_CANCELLED, status, 0);
  ZB_SCHEDULE_CALLBACK(zboss_signal_handler, buf);
}


static zb_bool_t try_to_cancel_nwk_discovery_alarm(void)
{
  zb_uint8_t cancelled_param;
  zb_bool_t ret = ZB_FALSE;

  ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(zb_nlme_network_discovery_request,
                                       ZB_ALARM_ANY_PARAM,
                                       &cancelled_param);
  if (cancelled_param != 0U)
  {
    zb_buf_set_status(cancelled_param, RET_INTERRUPTED);
    bdb_commissioning_signal(BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED, cancelled_param);
    ret = ZB_TRUE;
  }

  return ret;
}


void bdb_cancel_joining(zb_bufid_t buf)
{
  /* The states this function can be called:
     1. The device is a ZC, no matter what is the status of steering
        Conditions: ZB_IS_DEVICE_ZC() == ZB_TRUE
        Actions: return RET_ILLEGAL_REQUEST
     2. No steering (initiated by bdb_start_top_level_commissioning) is in progress
        Conditions: ZB_BDB().bdb_commissioning_mode != ZB_BDB_NETWORK_STEERING
        Actions: return RET_INVALID_STATE
     3. Device is joined, no matter what is the status of steering
        Conditions: zb_zdo_joined() == ZB_TRUE
        Actions: return RET_INVALID_STATE
     4. Steering is in progress, network discovery request is not sent yet
        Conditions: ZB_BDB().bdb_commissioning_step <= ZB_BDB_NETWORK_STEERING
        Actions: set ZB_BDB().bdb_op_cancelled flag
                 return RET_OK
     5. Steering is in progress, network discovery is scheduled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_STEERING
        Actions: call zb_nwk_cancel_network_discovery()
                 zb_nwk_cancel_network_discovery will return RET_OK
                 return RET_OK
     6. Steering is in progress, discovery is in progress
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_STEERING
        Actions: call zb_nwk_cancel_network_discovery()
                 zb_nwk_cancel_network_discovery will return RET_OK
                 return RET_OK
     7. Steering is in progress, discovery confirm is scheduled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_STEERING
        Actions: call zb_nwk_cancel_network_discovery()
                 zb_nwk_cancel_network_discovery will return RET_INVALID_STATE
                 return RET_PENDING
     8. Steering is in progress, discovery confirm is handled by ZDO layer,
        another discovery is scheduled by ZDO (see zdo_handle_nlme_network_discovery_confirm()
        and zdo_next_nwk_discovery_req())
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_STEERING
        Actions: cancel zb_nlme_network_discovery_request() alarm,
                 generate BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED
                 signal with status RET_INTERRUPTED and return RET_OK
     9. Steering is in progress, discovery confirm is already processed
        Conditions: ZB_BDB().bdb_commissioning_step >= ZB_BDB_NETWORK_STEERING
        Actions: return RET_PENDING, it is too late to cancel
    10. Steering is in progress, but already cancelled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_STEERING
                    ZB_BDB().bdb_op_cancelled == ZB_TRUE
        Actions: return RET_IGNORE
  */

  zb_ret_t ret;

  TRACE_MSG(TRACE_ZDO1, ">> bdb_cancel_joining, buf %d", (FMT__P, buf));

  if (buf == 0U)
  {
    TRACE_MSG(TRACE_ERROR, "no buffer provided, ignore this request", (FMT__0));
    return;
  }

  do
  {
    if (ZB_IS_DEVICE_ZC())
    {
      ret = RET_ILLEGAL_REQUEST;
      break;
    }

    if (zb_zdo_joined())
    {
      ret = RET_INVALID_STATE;
      break;
    }

    if (ZB_BDB().bdb_op_cancelled == ZB_TRUE)
    {
      ret = RET_IGNORE;
      break;
    }

    if (ZB_BDB().bdb_commissioning_mode != ZB_BDB_NETWORK_STEERING)
    {
      ret = RET_INVALID_STATE;
      break;
    }

    if (ZB_BDB().bdb_commissioning_step > ZB_BDB_NETWORK_STEERING)
    {
      ret = RET_PENDING;
      break;
    }

    ZB_BDB().bdb_op_cancelled = ZB_TRUE;

    if (ZB_BDB().bdb_commissioning_step < ZB_BDB_NETWORK_STEERING)
    {
      ret = RET_OK;
      break;
    }

    if (try_to_cancel_nwk_discovery_alarm())
    {
      TRACE_MSG(TRACE_ZDO1, "discovery alarm cancelled", (FMT__0));
      ret = RET_OK;
      break;
    }

    ZB_SCHEDULE_CALLBACK(zb_nwk_cancel_network_discovery, buf);
    ret = RET_BUSY;

  } while(0);

  if (ret != RET_BUSY)
  {
    TRACE_MSG(TRACE_ZDO1, "status %d, calling a cb", (FMT__D, ret));
    bdb_send_steering_cancelled_signal(buf, ret);
  }

  TRACE_MSG(TRACE_ZDO1, "<< bdb_cancel_joining, ret %d", (FMT__D, ret));
}


static void nwk_cancel_network_discovery_response(zb_bufid_t buf)
{
  zb_ret_t status;

  TRACE_MSG(TRACE_ZDO1, ">> nwk_cancel_network_discovery_response, buf %d", (FMT__D, buf));

  ZB_ASSERT(buf != 0U);

  status = *ZB_BUF_GET_PARAM(buf, zb_ret_t);

  TRACE_MSG(TRACE_ZDO1, "status %d", (FMT__D, status));

  switch(status)
  {
    case RET_OK:
    case RET_IGNORE:
      /* pass the status to the app */
      break;
    case RET_INVALID_STATE:
      if (try_to_cancel_nwk_discovery_alarm())
      {
        TRACE_MSG(TRACE_ZDO1, "discovery alarm cancelled", (FMT__0));
        status = RET_OK;
        break;
      }
      else
      {
        status = RET_PENDING;
      }
      break;
    default:
      TRACE_MSG(TRACE_ERROR, "unexpected status: %d", (FMT__D, status));
      ZB_ASSERT(0);
      break;
  }

  bdb_send_steering_cancelled_signal(buf, status);

  TRACE_MSG(TRACE_ZDO1, "<< nwk_cancel_network_discovery_response", (FMT__0));
}

#endif /* ZB_JOIN_CLIENT */

#endif /* NCP_MODE_HOST */

static void zb_bdb_close_network_local(zb_bufid_t buf)
{
  zb_zdo_mgmt_permit_joining_req_param_t *req_param;

  req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_permit_joining_req_param_t);

  ZB_BZERO(req_param, sizeof(zb_zdo_mgmt_permit_joining_req_param_t));
  req_param->dest_addr = ZB_PIBCACHE_NETWORK_ADDRESS();
  req_param->permit_duration = 0;
  req_param->tc_significance = 1;

  (void)zb_zdo_mgmt_permit_joining_req(buf, NULL);
}


zb_ret_t zb_bdb_close_network(zb_bufid_t buf)
{
  zb_ret_t ret = RET_OK;
  zb_uint8_t tsn = ZB_ZDO_INVALID_TSN;

  if (buf == ZB_BUF_INVALID)
  {
    buf = zb_buf_get_out();
  }

  if (buf != ZB_BUF_INVALID)
  {
    zb_zdo_mgmt_permit_joining_req_param_t *req_param;


    TRACE_MSG(TRACE_ZDO3, ">> zb_bdb_close_network", (FMT__0));

    req_param = ZB_BUF_GET_PARAM(buf, zb_zdo_mgmt_permit_joining_req_param_t);

    ZB_BZERO(req_param, sizeof(zb_zdo_mgmt_permit_joining_req_param_t));
    req_param->dest_addr = ZB_NWK_BROADCAST_ROUTER_COORDINATOR;
    req_param->permit_duration = 0;
    req_param->tc_significance = 1;

    if (zb_get_device_type() == ZB_NWK_DEVICE_TYPE_COORDINATOR ||
        zb_get_device_type() == ZB_NWK_DEVICE_TYPE_ROUTER)
    {
      tsn = zb_zdo_mgmt_permit_joining_req(buf, zb_bdb_close_network_local);
    }
    else
    {
      tsn = zb_zdo_mgmt_permit_joining_req(buf, NULL);
    }

    if (tsn == ZB_ZDO_INVALID_TSN)
    {
      TRACE_MSG(TRACE_ERROR, "Failed to create permit request", (FMT__0));
      ret = RET_ERROR;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Unable to get buffer", (FMT__0));
    ret = RET_NO_MEMORY;
  }

  TRACE_MSG(TRACE_ZDO3, "<< zb_bdb_close_network", (FMT__0));
  return ret;
}


/*! @} */

#endif /* ZB_BDB_MODE && !BDB_OLD */
