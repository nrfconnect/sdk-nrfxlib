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
/* PURPOSE:
*/

#define ZB_TRACE_FILE_ID 82

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_REL_HUMIDITY_MEASUREMENT)

zb_ret_t check_value_rel_humidity_measurement_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_rel_humidity_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_rel_humidity_measurement_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_rel_humidity_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_rel_humidity_measurement_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "> check_value_rel_humidity_measurement", (FMT__0));

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID:
      if( ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_UNKNOWN == ZB_ZCL_ATTR_GET16(value) )
      {
        ret = RET_OK;
      }
      else
      {
        zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(
            endpoint,
            ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
            ZB_ZCL_CLUSTER_SERVER_ROLE,
            ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID);

        ZB_ASSERT(attr_desc);

        ret = (ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) ==
                ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_UNDEFINED
            || ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) <= ZB_ZCL_ATTR_GET16(value))
          ? RET_OK : RET_ERROR;

        if(ret)
        {
          attr_desc = zb_zcl_get_attr_desc_a(
              endpoint,
              ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT,
              ZB_ZCL_CLUSTER_SERVER_ROLE,
              ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID);

          ZB_ASSERT(attr_desc);

          ret = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) == ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_UNDEFINED ||
                ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc)
            ? RET_OK : RET_ERROR;
        }
      }
      break;

    case ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID:
      ret = (
#if ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_MIN_VALUE != 0
          ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_MIN_VALUE <= ZB_ZCL_ATTR_GET16(value) &&
#endif
              (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_MAX_VALUE) )
              ? RET_OK : RET_ERROR;
      break;

    case ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID:
      ret = ( (ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_MIN_VALUE <= ZB_ZCL_ATTR_GET16(value)) &&
              (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_MAX_VALUE) )
              ? RET_OK : RET_ERROR;
      break;

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< check_value_rel_humidity_measurement ret %hd", (FMT__H, ret));
  return ret;
}
#endif /* if defined (ZB_ZCL_SUPPORT_CLUSTER_REL_HUMIDITY_MEASUREMENT) */
