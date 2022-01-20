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
/* PURPOSE: ZCL OTA Cluster - routines which always linked because called from the main ZCL logic.
*/

#define ZB_TRACE_FILE_ID 45

#include "zb_common.h"

/** @internal
    @{
*/

#if defined (ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE) || defined DOXYGEN

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_ota_upgrade.h"
#include "zb_zdo.h"
#include "zb_aps.h"
#include "zdo_wwah_stubs.h"

/* That code may be called from zcl_poll_control_commands.c, so linked always.*/
zb_uint8_t zb_zcl_ota_upgrade_get_ota_status(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_get_ota_status endpoint %hd", (FMT__H, endpoint));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
       ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
  ZB_ASSERT(attr_desc);

  status = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_get_ota_status %hd", (FMT__H, status));

  return status;
}


void zcl_ota_abort_and_set_tc(zb_uint8_t param)
{
  zb_zcl_cluster_handler_t cluster_handler;

  cluster_handler = zb_zcl_get_cluster_handler(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                                               ZB_ZCL_CLUSTER_CLIENT_ROLE);
  if (cluster_handler)
  {
    zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
    cmd_info->disable_default_response = ZB_TRUE;
    cmd_info->cluster_id = ZB_ZCL_CLUSTER_ID_OTA_UPGRADE;
    cmd_info->cmd_id = ZB_ZCL_CMD_OTA_UPGRADE_INTERNAL_ABORT_ID;
    cluster_handler(param);
  }
  else
  {
    zb_buf_free(param);
  }
}

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE || defined DOXYGEN */
