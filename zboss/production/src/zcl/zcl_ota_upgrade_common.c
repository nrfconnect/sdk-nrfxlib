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
/* PURPOSE: ZCL OTA Cluster - common part for Server and Client
*/

#define ZB_TRACE_FILE_ID 47

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
/*
  That routines are linked if either Client or Server role is enabled.
 */

zb_ret_t zb_zcl_check_value_ota_upgrade(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = ZB_TRUE;
  ZVUNUSED(endpoint);
  ZVUNUSED(value);

  TRACE_MSG(TRACE_ZCL1, "> check_value_ota_upgrade", (FMT__0));

  switch( attr_id )
  {
  /* remove for NTS certification - test 9.5.9
   * TH sends wrong value - 6000 (e.q. 25 minutes) */
#ifdef NTS_HACK
    case ZB_ZCL_ATTR_OTA_UPGRADE_MIN_BLOCK_REQUE_ID:
         ret = 258 > *(zb_uint16_t*)value
            ? RET_OK : RET_ERROR;
      break;
#endif

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< check_value_ota_upgrade ret %hd", (FMT__H, ret));
  return ret;
}
/*************************** Helper functions *************************/
/** @brief Get 8bit attribute value from OTA Upgrade cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @return attribute value
*/
zb_uint8_t zb_zcl_ota_upgrade_get8(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, attr_id);
  ZB_ASSERT(attr_desc);
  return ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
}

/** @brief Get 16bit attribute value from OTA Upgrade cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @return attribute value
*/
zb_uint16_t zb_zcl_ota_upgrade_get16(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, attr_id);
  ZB_ASSERT(attr_desc);
  return ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
}

/** @brief Get 16bit attribute value from OTA Upgrade cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @return attribute value
*/
zb_uint32_t zb_zcl_ota_upgrade_get32(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, attr_id);
  ZB_ASSERT(attr_desc);
  return ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
}

void zb_zcl_ota_upgrade_set_ota_status(zb_uint8_t endpoint, zb_uint8_t status)
{
  zb_zcl_attr_t *attr_desc;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_set_ota_status endpoint %hd status %hd", (FMT__H_H, endpoint, status));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
       ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
   ZB_ASSERT(attr_desc);

   ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, status);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_set_ota_status", (FMT__0));
}

/** @brief Default Response command */
zb_uint8_t zb_zcl_process_ota_upgrade_default_response_commands(zb_uint8_t param)
{
  zb_uint8_t ret = ZB_FALSE;
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_default_resp_payload_t* default_res;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_ota_upgrade_default_response_commands %hx", (FMT__H, param));

  if( cmd_info -> cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI &&
      cmd_info -> is_common_command &&
      cmd_info -> cmd_id == ZB_ZCL_CMD_DEFAULT_RESP)
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
         ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_IMAGE_STATUS_ID);
     ZB_ASSERT(attr_desc);

    default_res = ZB_ZCL_READ_DEFAULT_RESP(param);
    TRACE_MSG(TRACE_ZCL2, "ZB_ZCL_CMD_DEFAULT_RESP: command_id 0x%hx, status: 0x%hx",
              (FMT__H_H, default_res->command_id, default_res->status));

    if(ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL != ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) &&
        ZB_ZCL_STATUS_ABORT== default_res->status)
    {
      // set attribute
      zb_zcl_ota_upgrade_set_ota_status(endpoint, ZB_ZCL_OTA_UPGRADE_IMAGE_STATUS_NORMAL);

      // call User App
      ZB_ZCL_OTA_UPGRADE_ABORT_USER_APP(param);
    }

    zb_buf_free(param);

    ret = ZB_TRUE;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_ota_upgrade_default_response_commands", (FMT__0));

  return ret;
}

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE || defined DOXYGEN */
