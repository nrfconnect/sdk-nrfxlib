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
/* PURPOSE: ZCL: implements attribute values check
*/

#define ZB_TRACE_FILE_ID 2060

#include "zb_common.h"

#if defined ZB_ENABLE_ZCL

#include "zb_zcl.h"
#include "zb_aps.h"
#include "zcl/zb_zcl_common.h"
#include "zb_zdo.h"

/*!
  Check if attribute value is valid or not
  @param cluster_id - cluster ID
  @param cluster_role - cluster role (@ref zcl_cluster_role)
  @param endpoint - endpoint
  @param attr_id - attribute ID
  @param value - pointer to attribute data
  @return ZB_TRUE if data value is valid, ZB_FALSE otherwise
*/
#if 0
zb_bool_t zb_zcl_check_attr_value(zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *value)
{
  zb_bool_t ret = ZB_TRUE;
  zb_ret_t cb_ret = RET_IGNORE;

  zb_zcl_cluster_check_value_t cluster_check_value;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_check_attr_value cluster_id %d, endpoint %hd, attr_id %d, value %p",
            (FMT__D_H_D_P, cluster_id, endpoint, attr_id, value));

  ZB_ASSERT(value);


  if (ZCL_CTX().app_check_attr_value_cb)
  {
    cb_ret = ZCL_CTX().app_check_attr_value_cb(cluster_id, cluster_role, endpoint, attr_id, value);
  }

  cluster_check_value = zb_zcl_get_cluster_check_value(cluster_id,
                                                       cluster_role);

  if (cb_ret != RET_IGNORE)
  {
    ret = (cb_ret == RET_OK) ? ZB_TRUE : ZB_FALSE;
  }
  else if (cluster_check_value)
  { 
    ret = (cluster_check_value(attr_id, endpoint, value) == RET_ERROR) ?
          ZB_FALSE : ZB_TRUE;
    /*ret = cluster_check_value(attr_id, endpoint, value);*/
  }
  else
  {
    if (zb_zcl_get_cluster_handler(cluster_id, ZB_ZCL_CLUSTER_SERVER_ROLE))
    {
      TRACE_MSG(TRACE_ZCL3, "Cluster presents (0x%x), all attribute values allowed", (FMT__D, cluster_id));
      ret = ZB_TRUE;
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Error, cluster is not supported %x", (FMT__D, cluster_id));
      return ZB_FALSE;
    }
  }
/* CR: AEV: Where is call to ZCL_CTX().zb_zcl_check_attr_value_cb? Why it is removed? */
/* Code Fix:DD: This callback was renamed to app_check_attr_value_cb. This callback is invoked
before cluster's check attribute value callback. */
  return ret;
}
#endif

zb_ret_t zb_zcl_check_attr_value(zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *value)
{
  zb_ret_t ret;
  zb_ret_t cb_ret = RET_IGNORE;

  zb_zcl_cluster_check_value_t cluster_check_value;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_check_attr_value cluster_id %d, endpoint %hd, attr_id %d, value %p",
            (FMT__D_H_D_P, cluster_id, endpoint, attr_id, value));

  ZB_ASSERT(value);


  if (ZCL_CTX().app_check_attr_value_cb != NULL)
  {
    cb_ret = ZCL_CTX().app_check_attr_value_cb(cluster_id, cluster_role, endpoint, attr_id, value);
  }

  cluster_check_value = zb_zcl_internal_get_cluster_check_value(endpoint,
                                                                cluster_id,
                                                                cluster_role);

  if (cb_ret != RET_IGNORE)
  {
    ret = cb_ret;
  }
  else if (cluster_check_value != NULL)
  {
    ret = cluster_check_value(attr_id, endpoint, value);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL3, "Cluster (0x%x) has not check_value function, all attribute values allowed", (FMT__D, cluster_id));
    ret = RET_OK;
  }
/* DD: Callback ZCL_CTX().zb_zcl_check_attr_value_cb was renamed to app_check_attr_value_cb. 
       This callback is invoked before cluster's check attribute value callback. */
  return ret;
}
/*!
  Hook on Write Attribute command
  @param endpoint - endpoint
  @param cluster_id - cluster ID
  @param cluster_id - cluster role (@ref zcl_cluster_role)
  @param attr_id - attribute ID
  @param new_value - new value of attribute
*/
void zb_zcl_write_attr_hook(
  zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role,
  zb_uint16_t attr_id, zb_uint8_t *new_value)
{
  zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook;
  ZVUNUSED(endpoint);
  ZVUNUSED(cluster_id);
  ZVUNUSED(attr_id);
  ZVUNUSED(new_value);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_write_attr_hook cluster_id %d, endpoint %hd, attr_id %d",
            (FMT__D_H_D, cluster_id, endpoint, attr_id));

  cluster_write_attr_hook = zb_zcl_internal_get_cluster_write_attr_hook(endpoint,
                                                                        cluster_id,
                                                                        cluster_role);
  if (cluster_write_attr_hook != NULL)
  {
    cluster_write_attr_hook(endpoint, attr_id, new_value);
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_write_attr_hook", (FMT__0));
}

#endif /* ZB_ENABLE_ZCL */
