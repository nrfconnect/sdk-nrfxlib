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
/*  PURPOSE: BDD specific commissioning. Formation part.
*/

#define ZB_TRACE_FILE_ID 99

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


#if defined ZB_FORMATION

#ifndef NCP_MODE_HOST
static void nwk_cancel_network_formation_response(zb_bufid_t buf);
#endif /* NCP_MODE_HOST */

static void bdb_formation(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZDO3, "bdb_formation param %hd", (FMT__H, param));

  /* See Figure 5 - Network formation procedure */

  if (ZB_BDB().v_do_primary_scan == ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN
      && ZB_BDB().bdb_primary_channel_set != 0)
  {
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START;
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_BDB().v_scan_channels = ZB_BDB().bdb_primary_channel_set;
    TRACE_MSG(TRACE_ZDO1, "Doing formation channel mask 0x%lx", (FMT__L, ZB_BDB().v_scan_channels));
    /* call directly to simplify cancellation logic */
    zdo_start_formation(param);
  }
  else if ((ZB_BDB().v_do_primary_scan == ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START
            && ZB_BDB().bdb_secondary_channel_set != 0)
           ||
           (ZB_BDB().v_do_primary_scan == ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN
            && ZB_BDB().bdb_secondary_channel_set != 0
            && ZB_BDB().bdb_primary_channel_set == 0))
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_IN_PROGRESS;
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_DONE;
    ZB_BDB().v_scan_channels = ZB_BDB().bdb_secondary_channel_set;
    TRACE_MSG(TRACE_ZDO1, "Doing formation channel mask 0x%lx", (FMT__L, ZB_BDB().v_scan_channels));
    /* call directly to simplify cancellation logic */
    zdo_start_formation(param);
  }
  else
  {
    ZB_BDB().bdb_commissioning_status = ZB_BDB_STATUS_FORMATION_FAILURE;
    TRACE_MSG(TRACE_ZDO1, "BDB: Formation failed run next bdb machine step", (FMT__0));
    ZB_BDB().v_do_primary_scan = ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN;
    ZB_BDB().bdb_commissioning_step <<= 1;
    ZB_SCHEDULE_CALLBACK(bdb_commissioning_machine, param);
  }
}


static void bdb_commissioning_formation_channels_mask(zb_channel_list_t list)
{
  zb_uint32_t mask;

#if defined ZB_SUB_GHZ_ZB30_SUPPORT
  zb_uint8_t used_page;
#endif /* ZB_SUB_GHZ_ZB30_SUPPORT */

  mask = ZB_BDB().v_scan_channels;

  zb_channel_list_init(list);

#if defined ZB_SUB_GHZ_ZB30_SUPPORT
  used_page = zb_aib_channel_page_list_get_first_filled_page();
  zb_channel_page_list_set_mask(list, used_page, mask);
#else
  zb_channel_page_list_set_mask(list, ZB_CHANNEL_LIST_PAGE0_IDX, mask);
#endif /* !ZB_SUB_GHZ_ZB30_SUPPORT */

}


static void bdb_formation_force_link(void)
{
#ifndef NCP_MODE_HOST
  /* introduce some kind of setter? To be solved in the scope of ZBS-241 */
  ZG->nwk.selector.nwk_cancel_nwk_form_resp = nwk_cancel_network_formation_response;
#endif /* NCP_MODE_HOST */
  zdo_formation_force_link();

  FORMATION_SELECTOR().start_formation = bdb_formation;
  FORMATION_SELECTOR().get_formation_channels_mask = bdb_commissioning_formation_channels_mask;
}

#ifdef ZB_COORDINATOR_ROLE

void zb_set_network_coordinator_role(zb_uint32_t channel_mask)
{
  bdb_force_link();
  bdb_formation_force_link();
  zb_set_network_coordinator_role_with_mode(channel_mask, ZB_COMMISSIONING_BDB);
}

void zb_set_network_coordinator_role_ext(zb_channel_list_t channel_list)
{
  bdb_force_link();
  bdb_formation_force_link();
  zb_set_nwk_role_mode_common_ext(ZB_NWK_DEVICE_TYPE_COORDINATOR,
                                  channel_list,
                                  ZB_COMMISSIONING_BDB);
}

#endif /* ZB_COORDINATOR_ROLE */


#ifdef ZB_DISTRIBUTED_SECURITY_ON

void zb_enable_distributed(void)
{
  bdb_formation_force_link();
}

void zb_disable_distributed(void)
{
  FORMATION_SELECTOR().start_formation = NULL;
  FORMATION_SELECTOR().get_formation_channels_mask = NULL;
}

#endif /* ZB_DISTRIBUTED_SECURITY_ON */

#ifndef NCP_MODE_HOST
/* these functions are not supported for NCP now.
   Will be resolved in scope of ZBS-241 */

static void bdb_send_formation_cancelled_signal(zb_bufid_t buf, zb_ret_t status)
{
  TRACE_MSG(TRACE_ZDO1, "bdb_send_formation_cancelled_signal, buf %d, status %d",
            (FMT__D_D, buf, status));

  (void)zb_app_signal_pack_with_detailed_status(buf, ZB_BDB_SIGNAL_FORMATION_CANCELLED, status, 0);
  ZB_SCHEDULE_CALLBACK(zboss_signal_handler, buf);
}


void bdb_cancel_formation(zb_bufid_t buf)
{
  /* The states this function can be called:
     1. No formation (initiated by bdb_start_top_level_commissioning) is in progress
        Conditions: ZB_BDB().bdb_commissioning_mode != ZB_BDB_NETWORK_FORMATION
        Actions: return RET_INVALID_STATE
     2. Formation is in progress, network formation is not sent yet
        Conditions: ZB_BDB().bdb_commissioning_step <= ZB_BDB_NETWORK_FORMATION
        Actions: set ZB_BDB().bdb_op_cancelled flag
                 return RET_OK
     3. Formation is in progress, network formation is scheduled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_FORMATION
        Actions: call zb_nwk_cancel_network_formation()
                 zb_nwk_cancel_network_formation will return RET_OK
                 return RET_OK
     4. Formation is in progress, NWK layer formation is in progress
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_FORMATION
        Actions: call zb_nwk_cancel_network_formation()
                 zb_nwk_cancel_network_formation will return RET_OK (if it possible to cancel)
                 or RET_INVALID_STATE (if it is too late to cancel a formation)
                 return RET_OK or RET_PENDING
     5. Formation is in progress, formation confirm is scheduled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_FORMATION
        Actions: call zb_nwk_cancel_network_formation()
                 zb_nwk_cancel_network_formation will return RET_INVALID_STATE
                 return RET_PENDING
     6. Formation is in progress, formation confirm is scheduled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_FORMATION
        Actions: call zb_nwk_cancel_network_formation()
                 zb_nwk_cancel_network_formation will return RET_INVALID_STATE
                 return RET_PENDING
     7. Formation is in progress, NWK layer formation is already cancelled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_FORMATION
        Actions: call zb_nwk_cancel_network_formation()
                 zb_nwk_cancel_network_formation() will return RET_IGNORE
                 return RET_IGNORE
     8. Formation is in progress, but already cancelled
        Conditions: ZB_BDB().bdb_commissioning_step == ZB_BDB_NETWORK_FORMATION
                    ZB_BDB().bdb_op_cancelled == ZB_TRUE
        Actions: return RET_IGNORE
  */

  zb_ret_t ret;

  TRACE_MSG(TRACE_ZDO1, ">> bdb_cancel_formation, buf %d", (FMT__D, buf));

  if (buf == 0U)
  {
    TRACE_MSG(TRACE_ERROR, "no buffer provided, ignore this request", (FMT__0));
    return;
  }

  do
  {
    if (ZB_BDB().bdb_op_cancelled == ZB_TRUE)
    {
      ret = RET_IGNORE;
      break;
    }

    if (ZB_BDB().bdb_commissioning_mode != ZB_BDB_NETWORK_FORMATION)
    {
      ret = RET_INVALID_STATE;
      break;
    }

    if (ZB_BDB().bdb_commissioning_step > ZB_BDB_NETWORK_FORMATION)
    {
      ret = RET_PENDING;
      break;
    }

    ZB_BDB().bdb_op_cancelled = ZB_TRUE;

    if (ZB_BDB().bdb_commissioning_step < ZB_BDB_NETWORK_FORMATION)
    {
      ret = RET_OK;
      break;
    }

    ZB_SCHEDULE_CALLBACK(zb_nwk_cancel_network_formation, buf);
    ret = RET_BUSY;
  } while (0);

  if (ret != RET_BUSY)
  {
    TRACE_MSG(TRACE_ZDO1, "status %d, calling a cb", (FMT__D, ret));
    bdb_send_formation_cancelled_signal(buf, ret);
  }

  TRACE_MSG(TRACE_ZDO1, "<< bdb_cancel_formation", (FMT__0));
}


static void nwk_cancel_network_formation_response(zb_bufid_t buf)
{
  zb_ret_t status;

  TRACE_MSG(TRACE_ZDO1, ">> nwk_cancel_network_formation_response, buf %d", (FMT__D, buf));

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
      /* it is too late to cancel formation */
      status = RET_PENDING;
      break;
    default:
      TRACE_MSG(TRACE_ERROR, "unexpected status: %d", (FMT__D, status));
      ZB_ASSERT(0);
      break;
  }

  bdb_send_formation_cancelled_signal(buf, status);

  TRACE_MSG(TRACE_ZDO1, "<< nwk_cancel_network_formation_response", (FMT__0));
}

#endif /* NCP_MODE_HOST */

#endif /* ZB_FORMATION */
