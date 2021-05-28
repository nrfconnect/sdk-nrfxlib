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
/* PURPOSE: ZCL Shade Configuration cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2082

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_SHADE_CONFIG)

#include "zb_zcl.h"

#define CONFIGURE_MOST_OPENED 0
#define CONFIGURE_MOST_CLOSED 1

zb_ret_t check_value_shade_config_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_shade_config_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_SHADE_CONFIG,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_shade_config_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_shade_config_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_SHADE_CONFIG,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_shade_config_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}

void shade_normal_processing(zb_uint8_t ep_id, zb_uint16_t cluster_id, zb_uint8_t value)
{
  zb_zcl_attr_t* closed_limit_desc;
  zb_uint16_t* closed_limit;
  zb_zcl_attr_t* mode_desc;
  zb_uint8_t* mode;
  zb_uint16_t shade_position = 0;
  zb_uint8_t ret = 1;
  zb_zcl_device_callback_param_t *device_cb_param;
  zb_bufid_t buf = zb_buf_get_out();

  TRACE_MSG(TRACE_ZCL1, "> shade_normal_processing ep %i", (FMT__H, ep_id));

  ZB_ASSERT(buf != 0);

  closed_limit_desc = zb_zcl_get_attr_desc_a(ep_id,
                                             ZB_ZCL_CLUSTER_ID_SHADE_CONFIG,
                                             ZB_ZCL_CLUSTER_SERVER_ROLE,
                                             ZB_ZCL_ATTR_SHADE_CONFIG_CLOSED_LIMIT_ID);

  mode_desc = zb_zcl_get_attr_desc_a(ep_id,
                                     ZB_ZCL_CLUSTER_ID_SHADE_CONFIG,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_SHADE_CONFIG_MODE_ID);

  ZB_ASSERT(closed_limit_desc != NULL && mode_desc != NULL);

  closed_limit = (zb_uint16_t*)closed_limit_desc->data_p;
  mode = (zb_uint8_t*)mode_desc->data_p;

  if (*mode == ZB_ZCL_ATTR_SHADE_CONFIG_MODE_NORMAL)
  {
    switch (cluster_id)
    {
    case ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL:
      shade_position =
        *closed_limit * (ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE_SHADE - value)
        / ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE_SHADE;
      ret = 0;
      break;
    case ZB_ZCL_CLUSTER_ID_ON_OFF:
      shade_position = *closed_limit * (1 - value);
      ret = 0;
      break;
    }

    if (ret == 0)
    {
      TRACE_MSG(TRACE_ZCL1, "closed_limit %d", (FMT__D, *closed_limit));
      TRACE_MSG(TRACE_ZCL1, "shade_position %d", (FMT__D, shade_position));

      ZB_ASSERT(ZCL_CTX().device_cb != NULL);
      device_cb_param = ZB_BUF_GET_PARAM(buf, zb_zcl_device_callback_param_t);

      device_cb_param->device_cb_id = ZB_ZCL_SHADE_SET_VALUE_CB_ID;
      device_cb_param->endpoint = ep_id;
      device_cb_param->cb_param.shade_set_value_param.new_value = shade_position;
      device_cb_param->cb_param.shade_set_value_param.is_config = ZB_FALSE;

      (ZCL_CTX().device_cb)(buf);

      ret = device_cb_param->status;

      zb_buf_free(buf);

      if (ret != RET_OK)
      {
        TRACE_MSG(TRACE_ZCL1, "Error with setting new Shade Position", (FMT__0));
      }
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Error, mode is not NORMAL", (FMT__0));
  }
  TRACE_MSG(TRACE_ZCL1, "< shade_normal_processing", (FMT__0));
}

void shade_config_processing(zb_uint8_t ep_id)
{
  zb_uint16_t closed_limit;
  zb_zcl_attr_t* mode_desc;
  zb_uint8_t* mode;
  zb_zcl_device_callback_param_t *device_cb_param;
  zb_bufid_t buf = zb_buf_get_out();


  TRACE_MSG(TRACE_ZCL1, "> shade_config_processing ep %i", (FMT__H, ep_id));

  mode_desc = zb_zcl_get_attr_desc_a(ep_id,
                                     ZB_ZCL_CLUSTER_ID_SHADE_CONFIG,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_SHADE_CONFIG_MODE_ID);

  ZB_ASSERT(mode_desc != NULL && buf != 0);

  mode = (zb_uint8_t*)mode_desc->data_p;


  TRACE_MSG(TRACE_ZCL1, "mode %i", (FMT__H, *mode));

  if (*mode == ZB_ZCL_ATTR_SHADE_CONFIG_MODE_CONFIGURE)
  {
    ZB_ASSERT(ZCL_CTX().device_cb != NULL);

/* In Configure mode 0 means 'Go to most opened', 1 - 'Go to most closed' */

    device_cb_param = ZB_BUF_GET_PARAM(buf, zb_zcl_device_callback_param_t);

    device_cb_param->device_cb_id = ZB_ZCL_SHADE_SET_VALUE_CB_ID;
    device_cb_param->endpoint = ep_id;
    device_cb_param->cb_param.shade_set_value_param.new_value = CONFIGURE_MOST_OPENED;
    device_cb_param->cb_param.shade_set_value_param.is_config = ZB_TRUE;

    (ZCL_CTX().device_cb)(buf);


    device_cb_param->cb_param.shade_set_value_param.new_value = CONFIGURE_MOST_CLOSED;
    device_cb_param->cb_param.shade_set_value_param.is_config = ZB_TRUE;

    (ZCL_CTX().device_cb)(buf);

    device_cb_param->device_cb_id = ZB_ZCL_SHADE_GET_VALUE_CB_ID;

    (ZCL_CTX().device_cb)(buf);
    /* TODO: check - free buffer after a call? */

    closed_limit = device_cb_param->cb_param.shade_get_value_param.ret_value;

    /* Calculate ClosedLimit attribute */
    if (closed_limit > 0)
    {
      TRACE_MSG(TRACE_ZCL1, "ClosedLimit = %d, setting", (FMT__D, closed_limit));
      ZB_ZCL_SET_ATTRIBUTE(ep_id,
                           ZB_ZCL_CLUSTER_ID_SHADE_CONFIG,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           ZB_ZCL_ATTR_SHADE_CONFIG_CLOSED_LIMIT_ID,
                           (zb_uint8_t*)&closed_limit,
                           ZB_FALSE);
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Cannot set ClosedLimit = 0", (FMT__0));
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Not CONFIGURE mode", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "< shade_config_processing", (FMT__0));
}
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_SHADE_CONFIG */
