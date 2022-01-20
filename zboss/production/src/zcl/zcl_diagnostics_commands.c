/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZCL Diagnostics cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2065

#include "zb_common.h"

#if defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS)

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_diagnostics.h"
#include "zdo_diagnostics.h"

/** @internal
    @{
*/

zb_zcl_diagnostics_ctx_t diagnostics_ctx_zcl;

zb_ret_t check_value_diagnostics_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_diagnostics_init_server()
{
  ZB_BZERO(&diagnostics_ctx_zcl.mac_data, sizeof(zb_mac_diagnostic_info_t));
  ZB_BZERO(&diagnostics_ctx_zcl.zdo_data, sizeof(zdo_diagnostics_info_t));

  diagnostics_ctx_zcl.sync_data_cb = NULL;
  diagnostics_ctx_zcl.cb_param = 0;

  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DIAGNOSTICS,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_diagnostics_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_diagnostics_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_DIAGNOSTICS,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

static void zb_zcl_diagnostics_sync_counters_cb(zb_uint8_t param)
{
  zdo_diagnostics_full_stats_t *full_stats = zb_buf_begin(param);

  TRACE_MSG(TRACE_ZCL3, ">>zb_zcl_diagnostics_sync_counters_cb(), param %hd",
            (FMT__H, param));

  TRACE_MSG(TRACE_ZCL3, "status 0x%hx", (FMT__H, full_stats->status));

  if (full_stats->status == MAC_SUCCESS)
  {
    ZB_MEMCPY(&diagnostics_ctx_zcl.mac_data,
              &full_stats->mac_stats,
              sizeof(zb_mac_diagnostic_info_t));
    ZB_MEMCPY(&diagnostics_ctx_zcl.zdo_data,
              &full_stats->zdo_stats,
              sizeof(zdo_diagnostics_info_t));
  }

  if (diagnostics_ctx_zcl.sync_data_cb)
  {
    TRACE_MSG(TRACE_ZCL3, "schedule cb 0x%p, cb_param %hd",
              (FMT__P_H, diagnostics_ctx_zcl.sync_data_cb, diagnostics_ctx_zcl.cb_param));

    ZB_SCHEDULE_CALLBACK(diagnostics_ctx_zcl.sync_data_cb, diagnostics_ctx_zcl.cb_param);

    diagnostics_ctx_zcl.cb_param = 0;
    diagnostics_ctx_zcl.sync_data_cb = NULL;
  }

  zb_buf_free(param);

  TRACE_MSG(TRACE_ZCL3, "<<zb_zcl_diagnostics_sync_counters_cb()", (FMT__0));
}

zb_ret_t zb_zcl_diagnostics_sync_counters(zb_uint8_t cb_param, zb_callback_t cb)
{
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL3, ">>zb_zcl_diagnostics_sync_counters(), cb_param %hd, cb 0x%p",
            (FMT__H_P, cb_param, cb));

  if (diagnostics_ctx_zcl.sync_data_cb == NULL)
  {
    ret = ZDO_DIAGNOSTICS_GET_STATS(zb_zcl_diagnostics_sync_counters_cb);
    if (ret == RET_OK)
    {
      diagnostics_ctx_zcl.sync_data_cb = cb;
      diagnostics_ctx_zcl.cb_param = cb_param;
      TRACE_MSG(TRACE_ZCL3, "Synchronization is scheduled!", (FMT__0));
    }
  }
  else
  {
    ret = RET_BUSY;
    TRACE_MSG(TRACE_ZCL3, "another request is scheduled! cb 0x%p, cb_param 0x%hx",
              (FMT__P_H, diagnostics_ctx_zcl.sync_data_cb, diagnostics_ctx_zcl.cb_param));
  }

  TRACE_MSG(TRACE_ZCL3, "<<zb_zcl_diagnostics_sync_counters()", (FMT__0));

  return ret;
}

zb_ret_t check_value_diagnostics_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}

/**
 *  @} internal
 */

#endif /* defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS) */
