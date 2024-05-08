/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ZCL, common functions implementation
*/

#define ZB_TRACE_FILE_ID 2063
#include "zb_common.h"
#include "zb_zcl.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zcl/zb_zcl_common.h"
#include "zb_ncp.h"
#include "zdo_wwah_stubs.h"

#if defined (ZB_ENABLE_ZCL) || defined (ZB_ENABLE_ZGPD_ATTR_REPORTING)

static void broadcast_endpoint_delivery_step(zb_uint8_t param, zb_uint16_t bc_buf_ref);

static void zb_zcl_init_periodic_activities(zb_uint8_t param)
{
  if (param == 0U)
  {
    zb_ret_t ret = zb_buf_get_out_delayed(zb_zcl_init_periodic_activities);
    if (ret != RET_OK)
    {
      TRACE_MSG(TRACE_ERROR, "Failed zb_buf_get_out_delayed [%d]", (FMT__D, ret));
    }
  }
  else
  {
#ifdef ZB_HA_ENABLE_POLL_CONTROL_SERVER
    zb_uint8_t ep = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_POLL_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE);
    if (ep != 0U)
    {
      zb_zcl_poll_control_start(param, ep);
    }
    else
    {
      zb_buf_free(param);
    }
#else
    zb_buf_free(param);
#endif
  }
}


/**
   Register user or internal ZBOSS ZCL device context.

   To be used to hide GP cluster from the application.
   Note that ZB_AF_REGISTER_DEVICE_CTX for user ctx must be called before
   registering internal ZBOSS ctx. In practice it means used must call
   ZB_AF_REGISTER_DEVICE_CTX macro before zdo_dev_start().
 */
void zb_zcl_register_device_ctx(zb_af_device_ctx_t *device_ctx)
{
  zb_ret_t ret;
  zb_uint_t i;

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be
   * a false positive. There are no side effect to 'ZCL_CTX()'. This
   * violation seems to be caused by the fact that 'ZCL_CTX()' is an
   * external macro, which cannot be analyzed by C-STAT. */
  for (i = 0; i < ZB_ARRAY_SIZE(ZCL_CTX().device_ctx_arr); ++i)
  {
    if (ZCL_CTX().device_ctx_arr[i] == NULL)
    {
      ZCL_CTX().device_ctx_arr[i] = device_ctx;
      break;
    }
  }

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be
   * a false positive. There are no side effect to 'ZCL_CTX()'. This
   * violation seems to be caused by the fact that 'ZCL_CTX()' is an
   * external macro, which cannot be analyzed by C-STAT. */
  ZB_ASSERT(i < ZB_ARRAY_SIZE(ZCL_CTX().device_ctx_arr));
  if (ZCL_CTX().device_ctx == NULL)
  {
    ZCL_CTX().device_ctx = (device_ctx);
    ret = zb_buf_get_out_delayed(zb_zcl_init_periodic_activities);
    if (ret != RET_OK)
    {
      TRACE_MSG(TRACE_ERROR, "Failed zb_buf_get_out_delayed [%d]", (FMT__D, ret));
    }
  }
}

/* The purpose of this variable is to provide an access to endpoint ID
 * that is under initialization at the moment for cluster initialization functions
 * The best soultion is to add ep_id as an input parameter to
 * void (*zb_zcl_cluster_init_t)(void)
 */

static zb_uint8_t g_current_endpoint_id = ZB_ZCL_BROADCAST_ENDPOINT;

/*! @brief get endpoint ID that is under initialization
    @return endpoint ID
 */
zb_uint8_t zb_get_current_endpoint_id(void)
{
  return g_current_endpoint_id;
}

void zb_zcl_init_endpoint(zb_af_endpoint_desc_t* ep)
{
  zb_uindex_t j;

  TRACE_MSG(TRACE_ZCL2, ">> zb_zcl_init_endpoint, ep_id %d", (FMT__D, ep->ep_id));

  /* Init attribute, if need */
  if(ZCL_CTX().set_default_attr_cb != NULL)
  {
    (ZCL_CTX().set_default_attr_cb)(ep->ep_id);
  }

  g_current_endpoint_id = ep->ep_id;
  /* For every cluster in ep_desc - add declared handlers to table */
  for (j = 0; j < ep->cluster_count; j++)
  {
    if (ep->cluster_desc_list[j].cluster_init != NULL)
    {
      ep->cluster_desc_list[j].cluster_init();
    }
#ifndef ZB_ZCL_DISABLE_REPORTING
    /* Configure default reporting */
    zb_zcl_put_default_reporting_info_for_cluster(
      ep->ep_id,
      ep->profile_id,
      &ep->cluster_desc_list[j]);
#endif
  }
  g_current_endpoint_id = ZB_ZCL_BROADCAST_ENDPOINT;

  TRACE_MSG(TRACE_ZCL2, "<< zb_zcl_init_endpoint", (FMT__0));
}

zb_bool_t zb_zcl_check_cluster_list(void)
{
  zb_uint8_t i, j, k, l;
  zb_uint16_t cluster_id1, cluster_id2;
  for (i = 0U; i < ZCL_CTX().device_ctx->ep_count; i++)
  {
    for (j = 0U; j < ZCL_CTX().device_ctx->ep_desc_list[i]->cluster_count; j++)
    {
      for (k = i; k < ZCL_CTX().device_ctx->ep_count; k++)
      {
        for (l = j + 1U; l < ZCL_CTX().device_ctx->ep_desc_list[k]->cluster_count; l++)
        {
          cluster_id1 = (ZCL_CTX().device_ctx->ep_desc_list[i]->cluster_desc_list + j)->cluster_id;
          cluster_id2 = (ZCL_CTX().device_ctx->ep_desc_list[k]->cluster_desc_list + l)->cluster_id;
          /* Can not have 2 instances of listed clusters.
             TODO: Divide by roles? 2 Groups clients or 2 Thermostat clients may be ok, but
             currently it is not needed.
          */
          if (cluster_id1 == cluster_id2 &&
              (ZCL_CTX().device_ctx->ep_desc_list[i]->cluster_desc_list + j)->role_mask ==
              (ZCL_CTX().device_ctx->ep_desc_list[k]->cluster_desc_list + l)->role_mask &&
              (cluster_id1 == ZB_ZCL_CLUSTER_ID_GROUPS ||
               cluster_id1 == ZB_ZCL_CLUSTER_ID_OTA_UPGRADE ||
               cluster_id1 == ZB_ZCL_CLUSTER_ID_POLL_CONTROL ||
               cluster_id1 == ZB_ZCL_CLUSTER_ID_SUB_GHZ ||
               cluster_id1 == ZB_ZCL_CLUSTER_ID_THERMOSTAT ||
               cluster_id1 == ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT ||
               cluster_id1 == ZB_ZCL_CLUSTER_ID_WWAH))
          {
            TRACE_MSG(TRACE_ERROR, "cluster_id 0x%x cluster_role %hd is not unique!",
             (FMT__D_H, cluster_id1,
             (ZCL_CTX().device_ctx->ep_desc_list[i]->cluster_desc_list + j)->role_mask));
            return ZB_FALSE;
          }
        }
      }
    }
  }
  return ZB_TRUE;
}

/* Function returns attribute size in bytes. If invalid attribute type is specified, 0xff is returned
 param attr_type - attribute data type
 param attr_value - attribute data value, is used to calculate size
 of attributes with variable size
 */
zb_uint8_t zb_zcl_get_attribute_size(zb_uint8_t attr_type, zb_uint8_t *attr_value)
{
  zb_uint32_t ret = (zb_uint32_t)~0U;
  TRACE_MSG(
      TRACE_ZCL1,
      ">> zb_zcl_get_attribute_size: attr_type 0x%hx attr_value %p",
      (FMT__H_P, attr_type, attr_value));

  switch ( attr_type )
  {
    case ZB_ZCL_ATTR_TYPE_8BIT:
    case ZB_ZCL_ATTR_TYPE_U8:
    case ZB_ZCL_ATTR_TYPE_S8:
    case ZB_ZCL_ATTR_TYPE_BOOL:
    case ZB_ZCL_ATTR_TYPE_8BITMAP:
    case ZB_ZCL_ATTR_TYPE_8BIT_ENUM:
      ret = sizeof(zb_uint8_t);
      break;

    case ZB_ZCL_ATTR_TYPE_16BIT:
    case ZB_ZCL_ATTR_TYPE_U16:
    case ZB_ZCL_ATTR_TYPE_S16:
    case ZB_ZCL_ATTR_TYPE_16BITMAP:
    case ZB_ZCL_ATTR_TYPE_16BIT_ENUM:
    case ZB_ZCL_ATTR_TYPE_SEMI:
    case ZB_ZCL_ATTR_TYPE_CLUSTER_ID:
    case ZB_ZCL_ATTR_TYPE_ATTRIBUTE_ID:
      ret = sizeof(zb_uint16_t);
      break;

    case ZB_ZCL_ATTR_TYPE_32BIT:
    case ZB_ZCL_ATTR_TYPE_U32:
    case ZB_ZCL_ATTR_TYPE_S32:
    case ZB_ZCL_ATTR_TYPE_32BITMAP:
    case ZB_ZCL_ATTR_TYPE_UTC_TIME:
    case ZB_ZCL_ATTR_TYPE_TIME_OF_DAY:
    case ZB_ZCL_ATTR_TYPE_DATE:
    case ZB_ZCL_ATTR_TYPE_BACNET_OID:
    case ZB_ZCL_ATTR_TYPE_SINGLE:
      ret = sizeof(zb_uint32_t);
      break;

    case ZB_ZCL_ATTR_TYPE_S48:
    case ZB_ZCL_ATTR_TYPE_U48:
    case ZB_ZCL_ATTR_TYPE_48BIT:
    case ZB_ZCL_ATTR_TYPE_48BITMAP:
//TODO: fix it, use correct type
      ret = ZB_48BIT_SIZE;
      break;

    case ZB_ZCL_ATTR_TYPE_S24:
    case ZB_ZCL_ATTR_TYPE_U24:
    case ZB_ZCL_ATTR_TYPE_24BIT:
    case ZB_ZCL_ATTR_TYPE_24BITMAP:
//TODO: fix it, use correct type
      ret = ZB_24BIT_SIZE;
      break;

    case ZB_ZCL_ATTR_TYPE_U40:
    case ZB_ZCL_ATTR_TYPE_S40:
    case ZB_ZCL_ATTR_TYPE_40BIT:
    case ZB_ZCL_ATTR_TYPE_40BITMAP:
      ret = 5;
      break;

    case ZB_ZCL_ATTR_TYPE_U56:
    case ZB_ZCL_ATTR_TYPE_S56:
    case ZB_ZCL_ATTR_TYPE_56BIT:
    case ZB_ZCL_ATTR_TYPE_56BITMAP:
      ret = 7;
      break;

    case ZB_ZCL_ATTR_TYPE_64BIT:
    case ZB_ZCL_ATTR_TYPE_64BITMAP:
    case ZB_ZCL_ATTR_TYPE_U64:
    case ZB_ZCL_ATTR_TYPE_S64:
    case ZB_ZCL_ATTR_TYPE_DOUBLE:
      ret = sizeof(zb_64bit_data_t);
      break;

    case ZB_ZCL_ATTR_TYPE_OCTET_STRING:
    case ZB_ZCL_ATTR_TYPE_CHAR_STRING:
      if ( attr_value != NULL && *attr_value != ZB_ZCL_INVALID_STRING_VALUE)
      {
        /* the first byte of the string or array has the size */
        zb_uint8_t ret_u8 = *(zb_uint8_t*)attr_value + 1U;
        ret = (zb_uint32_t)ret_u8;
      }
      break;

    case ZB_ZCL_ATTR_TYPE_ARRAY:
    case ZB_ZCL_ATTR_TYPE_CUSTOM_32ARRAY:
    case ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING:
    {
      zb_uint16_t array_size = 0;

      if (attr_type == ZB_ZCL_ATTR_TYPE_ARRAY ||
          attr_type == ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING)
      {
        ZB_ZCL_ARRAY_GET_SIZE(&array_size, attr_value);
      }
      else
      {
        ZB_BYTE_32ARRAY_GET_SIZE(&array_size, attr_value);
      }

      if (array_size != ZB_ZCL_INVALID_ARRAY_VALUE)
      {
        /* the first 2 bytes of the array has the size */
        /* NK:FIXME:FIXME:FIXME: Cast 2 bytes to 1! Current implementation! All arrays
         * used now are less then 0xff length. Discussed with VS. */
        ret = (zb_uint32_t)array_size + 2U;
      }
      break;
    }

    case ZB_ZCL_ATTR_TYPE_IEEE_ADDR:
      ret = sizeof(zb_ieee_addr_t);
      break;

    case ZB_ZCL_ATTR_TYPE_128_BIT_KEY:
      ret = 16U;
      break;

    default:
      TRACE_MSG(TRACE_ZCL1, "Error, unsupported type!", (FMT__0));
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_get_attribute_size: result %hd", (FMT__D, ret));
  ZB_ASSERT(ret <= ZB_UINT8_MAX);
  return (zb_uint8_t)ret;
}

/*
  Put attribute value to command packet, fix endian if needed.
  Return: modified command pointer
*/
zb_uint8_t* zb_zcl_put_value_to_packet(zb_uint8_t *cmd_ptr, zb_uint8_t attr_type, zb_uint8_t *attr_value)
{
  zb_uint16_t len;

  TRACE_MSG(TRACE_ZCL1,
            ">> zb_zcl_put_value_to_packet: cmd_ptr %p, attr_type 0x%hx attr_value %p",
            (FMT__P_H_P, cmd_ptr, attr_type, attr_value));

  switch ( attr_type )
  {
    case ZB_ZCL_ATTR_TYPE_8BIT:
    case ZB_ZCL_ATTR_TYPE_U8:
    case ZB_ZCL_ATTR_TYPE_S8:
    case ZB_ZCL_ATTR_TYPE_BOOL:
    case ZB_ZCL_ATTR_TYPE_8BITMAP:
    case ZB_ZCL_ATTR_TYPE_8BIT_ENUM:
      ZB_ZCL_PACKET_PUT_DATA8(cmd_ptr, *attr_value);
      break;

    case ZB_ZCL_ATTR_TYPE_16BIT:
    case ZB_ZCL_ATTR_TYPE_U16:
    case ZB_ZCL_ATTR_TYPE_S16:
    case ZB_ZCL_ATTR_TYPE_16BITMAP:
    case ZB_ZCL_ATTR_TYPE_16BIT_ENUM:
      ZB_ZCL_PACKET_PUT_DATA16(cmd_ptr, attr_value);
      break;

    case ZB_ZCL_ATTR_TYPE_32BIT:
    case ZB_ZCL_ATTR_TYPE_U32:
    case ZB_ZCL_ATTR_TYPE_S32:
    case ZB_ZCL_ATTR_TYPE_32BITMAP:
    case ZB_ZCL_ATTR_TYPE_UTC_TIME:
      ZB_ZCL_PACKET_PUT_DATA32(cmd_ptr, attr_value);
      break;

    case ZB_ZCL_ATTR_TYPE_U48:
    case ZB_ZCL_ATTR_TYPE_S48:
//TODO: fix it, put data taking into account bytes order
      ZB_ZCL_PACKET_PUT_DATA48(cmd_ptr, attr_value);
      break;

    case ZB_ZCL_ATTR_TYPE_S24:
    case ZB_ZCL_ATTR_TYPE_U24:
    case ZB_ZCL_ATTR_TYPE_24BIT:
    case ZB_ZCL_ATTR_TYPE_24BITMAP:
//TODO: fix it, put data taking into account bytes order
      ZB_ZCL_PACKET_PUT_DATA24(cmd_ptr, attr_value);
      break;

    case ZB_ZCL_ATTR_TYPE_OCTET_STRING:
    case ZB_ZCL_ATTR_TYPE_CHAR_STRING:
      if ( attr_value != NULL && *attr_value != ZB_ZCL_INVALID_STRING_VALUE)
      {
        /* the first byte of the string or array has the size */
        zb_uint8_t len_u8 = *(zb_uint8_t *)attr_value + 1U;
        len = (zb_uint16_t)len_u8;
        ZB_ZCL_PACKET_PUT_DATA_N(cmd_ptr, attr_value, len);
      }
      break;

    case ZB_ZCL_ATTR_TYPE_ARRAY:
    case ZB_ZCL_ATTR_TYPE_CUSTOM_32ARRAY:
    case ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING:
    {
      zb_uint16_t array_size = 0;

      if (attr_type == ZB_ZCL_ATTR_TYPE_ARRAY ||
          attr_type == ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING)
      {
        ZB_ZCL_ARRAY_GET_SIZE(&array_size, attr_value);
      }
      else
      {
        ZB_BYTE_32ARRAY_GET_SIZE(&array_size, attr_value);
      }

      if (array_size != ZB_ZCL_INVALID_ARRAY_VALUE)
      {
        /* the first 2 bytes of the array has the size */
        /* NK:FIXME:FIXME:FIXME: Cast 2 bytes to 1! Current implementation! All arrays
         * used now are less then 0xff length. Discussed with VS. */

        len = array_size + 2U;
        ZB_ZCL_PACKET_PUT_DATA_N(cmd_ptr, attr_value, len);
      }
      break;
    }

    case ZB_ZCL_ATTR_TYPE_64BIT:
    case ZB_ZCL_ATTR_TYPE_IEEE_ADDR:
      ZB_ZCL_PACKET_PUT_DATA64(cmd_ptr, attr_value);
      break;

    case ZB_ZCL_ATTR_TYPE_128_BIT_KEY:
      ZB_ZCL_PACKET_PUT_DATA_N(cmd_ptr, attr_value, 16);
      break;

    default:
      TRACE_MSG(TRACE_ZCL1, "Error, unsupported type!", (FMT__0));
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_put_value_to_packet: ret %p", (FMT__P, cmd_ptr));
  return cmd_ptr;
}

#if defined (ZB_ENABLE_ZCL)
/*
  Function returns data size in bytes for analog data types. If invalid data type is specified, 0xff is returned
  param attr_type - attribute data type
*/
zb_uint8_t zb_zcl_get_analog_attribute_size(zb_uint8_t attr_type)
{
  zb_uint32_t ret = 0;

  switch (attr_type)
  {
    case ZB_ZCL_ATTR_TYPE_U8:
      ret = sizeof(zb_uint8_t);
      break;

    case ZB_ZCL_ATTR_TYPE_U16:
    case ZB_ZCL_ATTR_TYPE_S16:
      ret = sizeof(zb_uint16_t);
      break;

    case ZB_ZCL_ATTR_TYPE_U24:
    case ZB_ZCL_ATTR_TYPE_S24:
      ret = ZB_24BIT_SIZE;
      break;

    case ZB_ZCL_ATTR_TYPE_U32:
    case ZB_ZCL_ATTR_TYPE_S32:
      ret = sizeof(zb_uint32_t);
      break;

    case ZB_ZCL_ATTR_TYPE_U48:
    case ZB_ZCL_ATTR_TYPE_S48:
      ret = ZB_48BIT_SIZE;
      break;

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "get_analog_attribute_size %hd", (FMT__H, ret));
  ZB_ASSERT(ret <= ZB_UINT8_MAX);
  return (zb_uint8_t)ret;
}

/*
  Function returns ZB_TRUE for analog data types, ZB_FALSE otherwise
*/
zb_bool_t zb_zcl_is_analog_data_type(zb_uint8_t attr_type)
{
  zb_bool_t ret = ZB_FALSE;
  switch (attr_type)
  {
    case ZB_ZCL_ATTR_TYPE_U8:
    case ZB_ZCL_ATTR_TYPE_U16:
    case ZB_ZCL_ATTR_TYPE_U24:
    case ZB_ZCL_ATTR_TYPE_U32:
    case ZB_ZCL_ATTR_TYPE_U48:
    case ZB_ZCL_ATTR_TYPE_S8:
    case ZB_ZCL_ATTR_TYPE_S16:
    case ZB_ZCL_ATTR_TYPE_S24:
    case ZB_ZCL_ATTR_TYPE_S32:
    case ZB_ZCL_ATTR_TYPE_S48:
    case ZB_ZCL_ATTR_TYPE_UTC_TIME:
      ret = ZB_TRUE;
      break;

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_is_analog_data_type %hd, is analog %hd", (FMT__H_H, attr_type, ret));
  return ret;
}


#define ZCL_DEVICE_CONTEXT_ITERATOR(idx)                                                           \
  {                                                                                                \
    zb_uint32_t dci;                                                                               \
    zb_af_device_ctx_t *device_ctx;                                                                \
    zb_uint32_t device_ctx_arr_size = ZB_ARRAY_SIZE(ZCL_CTX().device_ctx_arr);                     \
    for (dci = 0; dci < device_ctx_arr_size; dci++)                                                \
    {                                                                                              \
      (idx) = 0;                                                                                   \
      device_ctx = ZCL_CTX().device_ctx_arr[dci];                                                  \
      if (device_ctx == NULL)                                                                      \
      {                                                                                            \
        break;                                                                                     \
      }

#define ZCL_DEVICE_CONTEXT_ITERATOR_END                                                            \
    }                                                                                              \
  }

#define ZCL_DEVICE_CONTEXT_BREAK break

zb_af_endpoint_desc_t* zb_af_get_endpoint_desc(zb_uint8_t ep_id)
{
  zb_uindex_t i;
  zb_af_endpoint_desc_t *ep_desc = NULL;

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  TRACE_MSG(TRACE_ZCL3, ">>get_endpoint_desc %p %p ep_id %hd", (FMT__P_P_H,
                                                                &ZCL_CTX(), device_ctx, ep_id));
  while (i < device_ctx->ep_count)
  {
    TRACE_MSG(TRACE_ZCL3, "device_ctx %p, ep_count %hd",
              (FMT__P_H, device_ctx, device_ctx->ep_count));

    TRACE_MSG(TRACE_ZCL3, "ep_id %hd", (FMT__H, device_ctx->ep_desc_list[i]->ep_id));
    if (device_ctx->ep_desc_list[i]->ep_id == ep_id)
    {
      ep_desc = device_ctx->ep_desc_list[i];
      ZCL_DEVICE_CONTEXT_BREAK;
    }
    i++;
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZCL3, "<<get_endpoint_desc desc %p", (FMT__P, ep_desc));
  return ep_desc;
}

/**
   Get ZCL TSN stored in ZCL frame
   @param buffer - pointer to a buffer with ZCL frame
   @return TSN value
*/
zb_uint8_t zb_zcl_get_tsn_from_packet(zb_bufid_t buffer)
{
  zb_uint8_t tsn;
  zb_uint8_t *cmd_buf = zb_buf_begin(buffer);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_get_tsn_from_packet %p", (FMT__P, buffer));

  /* ZCL spec 2.3.1 ZCL frame format */
  /* | Frame control 1b | Manuf code 0/2b | TSN 1b | Cmd id 1b | Payload xx b | */

  /* cmd_buf points to Frame control */
  if (!ZB_ZCL_IS_MANUF_SPECIFIC(*cmd_buf))
  {
    /* no manufacturer code */
    tsn = *(cmd_buf + sizeof(zb_uint8_t));
  }
  else
  {
    /* manufacturer specific code is set */
    tsn = *(cmd_buf + sizeof(zb_uint8_t) + sizeof(zb_uint16_t));
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_get_tsn_from_packet tsn %hd", (FMT__H, tsn));
  return tsn;
}

/**
   Register callback that should be called when APS acknowledge is
   received for ZCL command
   @param param - index to buffer with ZCL command
   @param cb - pointer to callback function
*/
zb_ret_t zb_zcl_register_cb(zb_uint8_t param, zb_callback_t cb)
{
  zb_ret_t           ret;
  zb_uint32_t        hash;
  zb_uint32_t        i;
  zcl_cb_hash_ent_t *ent;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_register_cb param %hd, cb %p", (FMT__H_P, param, cb));

  ZB_ASSERT(param);

  /* Handle frequent case of zero cb. In that case in
   * zb_zcl_ack_callback() we were scanning entire zcl_cb[] to find nothing.
   * Now add an entry with empty cb to speedup zb_zcl_ack_callback().
   * We can live without an entry with empty cb, so, if we got into it, can reuse it.
   */
  ret = RET_NO_MEMORY;

  hash = ZB_ZCL_TRAN_TABLE_HASH_PARAM(param);

  i = hash;
  do
  {
    ent = &ZCL_CTX().zcl_cb[i];
    if (ent->buf == 0U
        || ent->func == NULL)
    {
      /* found free slot */
      ent->func = cb;
      ent->buf  = param;

      ret = RET_OK;
      break;
    }

    i = (i + 1U) % ZCL_TRAN_TABLE_SIZE;
  }
  while (i != hash);
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_register_cb ret %d", (FMT__D, ret));

  return ret;
}

/**
   Call callback on receiving APS acknowledge for ZCL command
 */
zb_ret_t zb_zcl_ack_callback(zb_uint8_t param)
{
  zb_uint32_t        hash;
  zb_uint32_t        i;
  zcl_cb_hash_ent_t *ent;
  zb_ret_t           ret = RET_NOT_FOUND;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_ack_callback %hd", (FMT__H, param));

  hash = ZB_ZCL_TRAN_TABLE_HASH_PARAM(param);
  i = hash;
  do
  {
    ent = &ZCL_CTX().zcl_cb[i];
    if (ent->buf == param)
    {
      TRACE_MSG(TRACE_ZCL2, "found callback %p", (FMT__P, ZCL_CTX().zcl_cb[i].func));

      /* Added +1 turbo poll in zb_zcl_finish_and_send_packet_common() but failed
       * to send - need not turbo poll for the next packet. Else may increase
       * turbo poll count multiple times. */
      if (ZB_IS_DEVICE_ZED())
      {
        if (zb_buf_get_status(param) != RET_OK)
        {
          zb_zdo_pim_turbo_poll_cancel_packet();
        }
      }

      if (ent->func != NULL)
      {
        /* found - schedule it to execution */
        ZB_SCHEDULE_CALLBACK(ent->func, param);
        ret = RET_OK;
        ent->func = NULL;
      }

      /* mark slot as free */
      ent->buf  = 0;

      TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_ack_callback ret %d", (FMT__D, ret));
      return ret;
    }
    i = (i + 1U) % ZCL_TRAN_TABLE_SIZE;
  }
  while (i != hash);

  /* It is ok to have no entry in zcl_cb if zb_zcl_register_cb was called with
   * zero cb and then overwritten by another entry. */
  TRACE_MSG(TRACE_ZCL3, "buf 0x%hx not found - maybe, it is ok", (FMT__H, param));

  /* free buffer on caller level */

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_ack_callback NOT FOUND", (FMT__0));
  return RET_NOT_FOUND;
}

#ifndef ZB_LITTLE_ENDIAN
/*
  Fix data endian, if needed, and store it in the same buffer
 */
void zb_zcl_fix_endian(zb_uint8_t *data_ptr, zb_uint8_t data_type)
{
  union int_vars_e
  {
    zb_uint16_t u16;
    zb_uint32_t u32;
    zb_ieee_addr_t u64;
    zb_uint48_t u48;
    zb_int24_t i24;
  }
  int_vars;

  ZB_ASSERT(data_ptr);

  switch ( data_type )
  {
    case ZB_ZCL_ATTR_TYPE_16BIT:
    case ZB_ZCL_ATTR_TYPE_U16:
    case ZB_ZCL_ATTR_TYPE_S16:
    case ZB_ZCL_ATTR_TYPE_16BITMAP:
      ZB_HTOLE16(&int_vars.u16, data_ptr);
      ZB_MEMCPY(data_ptr, &int_vars.u16, sizeof(zb_uint16_t));
      break;

    case ZB_ZCL_ATTR_TYPE_32BIT:
    case ZB_ZCL_ATTR_TYPE_U32:
    case ZB_ZCL_ATTR_TYPE_S32:
    case ZB_ZCL_ATTR_TYPE_32BITMAP:
      ZB_HTOLE32(&int_vars.u32, data_ptr);
      ZB_MEMCPY(data_ptr, &int_vars.u32, sizeof(zb_uint32_t));
      break;

    case ZB_ZCL_ATTR_TYPE_64BIT:
    case ZB_ZCL_ATTR_TYPE_IEEE_ADDR:
      ZB_HTOLE64(&int_vars.u64, data_ptr);
      ZB_MEMCPY(data_ptr, &int_vars.u64, sizeof(zb_ieee_addr_t));
      break;

    case ZB_ZCL_ATTR_TYPE_U48:
    case ZB_ZCL_ATTR_TYPE_S48:
      ZB_HTOLE48(&int_vars.u48, data_ptr);
      ZB_MEMCPY(data_ptr, &int_vars.u48, ZB_48BIT_SIZE);
      break;

    case ZB_ZCL_ATTR_TYPE_S24:
    case ZB_ZCL_ATTR_TYPE_U24:
      ZB_HTOLE24(&int_vars.i24, data_ptr);
      ZB_MEMCPY(data_ptr, &int_vars.i24, ZB_24BIT_SIZE);
      break;

    default:
      break;
  }
}
#endif /* ZB_LITTLE_ENDIAN */

/* Check, if attribute value maybe set to a new value
 * Note: check_access specifies if it is needed to perform read-only
 * check: end-user application may chanage read-only attributes
*/
zb_uint8_t zb_zcl_check_attribute_writable(
    zb_zcl_attr_t* attr_desc,
    zb_uint8_t endpoint,
    zb_uint16_t cluster_id,
    zb_uint8_t cluster_role,
    zb_uint8_t *new_value,
    zb_bool_t check_access)
{
  zb_uint8_t status = ZB_ZCL_STATUS_SUCCESS;

  TRACE_MSG(TRACE_ZCL1,
      ">> zb_zcl_check_attribute_writable, attr_desc %p, endpoint %hd, cluster %d, check_access %hd",
      (FMT__P_H_D_H, attr_desc, endpoint, cluster_id, check_access));
  TRACE_MSG(TRACE_ZCL1, "attr_id 0x%x", (FMT__D, attr_desc->id));

  if (attr_desc != NULL && attr_desc->data_p != NULL)
  {
    if (check_access)
    {
      if (ZB_BIT_IS_SET(attr_desc->access, ZB_ZCL_ATTR_ACCESS_INTERNAL))
      {
        TRACE_MSG(TRACE_ZCL1, "error, internal attr isn't writable", (FMT__0));
        status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
      }
      else if (!ZB_BIT_IS_SET(attr_desc->access, ZB_ZCL_ATTR_ACCESS_WRITE_ONLY))
      {
        TRACE_MSG(TRACE_ZCL1, "error, access to R/O attr", (FMT__0));
        status = ZB_ZCL_STATUS_READ_ONLY;
      }
      else
      {
        /* MISRA rule 15.7 requires empty 'else' branch. */
      }
    }
    /* NOTE: Need to check for authorized write access, but no description for it in spec */
    if (ZB_ZCL_STATUS_SUCCESS == status)
        //&& zb_zcl_check_attr_value(cluster_id, cluster_role, endpoint, attr_desc->id, new_value) == RET_ERROR /-- ZB_FALSE) --/
    {
      zb_ret_t ret = zb_zcl_check_attr_value_manuf(cluster_id, cluster_role, endpoint, attr_desc->id, attr_desc->manuf_code, new_value);
      if (ret == RET_ERROR)
      {
        TRACE_MSG(TRACE_ZCL1, "error, invalid value", (FMT__0));
        status = ZB_ZCL_STATUS_INVALID_VALUE;
      }
      else if (ret == RET_OUT_OF_RANGE)
      {
        TRACE_MSG(TRACE_ZCL1, "limit reached value, return SUCCESS acc. to ZCL8", (FMT__0));
        status = ZB_ZCL_STATUS_SUCCESS;
      }
      else if (ret == RET_BLOCKED)
      {
        TRACE_MSG(TRACE_ZCL1, "Blocked by internal check, response of status READ_ONLY.", (FMT__0));
        status = ZB_ZCL_STATUS_READ_ONLY;
      }
      else if (ret == RET_UNAUTHORIZED)
      {
        TRACE_MSG(TRACE_ZCL1, "unathorized value", (FMT__0));
        status = ZB_ZCL_STATUS_NOT_AUTHORIZED;
      }
      else
      {
        /* MISRA rule 15.7 requires empty 'else' branch. */
      }
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "error, not supported attr", (FMT__0));
    status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_check_attribute_writable, ret %hx", (FMT__H, status));
  return status;
}

/* Sets attribute value, for other endpoint of singleton attribute
 * check: end-user application may chanage read-only attributes
*/
static void zb_zcl_conform_singleton(zb_uint8_t ep_first, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id, zb_uint8_t *value, zb_uint16_t manuf_code)
{
  zb_uindex_t i;
  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_conform_singleton ep %hd, cluster %d, attr 0x%x, manuf_code 0x%x",
            (FMT__H_D_D_D, ep_first, cluster_id, attr_id, manuf_code));

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  while (device_ctx != NULL && i < device_ctx->ep_count)
  {
    TRACE_MSG(TRACE_ZCL3, "ep_id %hd", (FMT__H, device_ctx->ep_desc_list[i]->ep_id));
    if (device_ctx->ep_desc_list[i]->ep_id != ep_first)
    {
      zb_zcl_attr_t *attr_desc;
      zb_zcl_cluster_desc_t *cluster_desc = get_cluster_desc(
        device_ctx->ep_desc_list[i], cluster_id, cluster_role);

      if (cluster_desc == NULL)
      {
        continue;
      }

      attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, attr_id, manuf_code);
      if (attr_desc == NULL)
      {
        continue;
      }

      zb_zcl_write_attr_hook(device_ctx->ep_desc_list[i]->ep_id,
                             cluster_id, cluster_role, attr_id, value, manuf_code);

      ZB_MEMCPY(attr_desc->data_p, value,
                    zb_zcl_get_attribute_size(attr_desc->type, value));

#if !(defined ZB_ZCL_DISABLE_REPORTING)
      /* check, if reporting is needed for an attribute */
      if (ZB_ZCL_IS_ATTR_REPORTABLE(attr_desc))
      {
        zb_zcl_mark_attr_for_reporting_manuf(device_ctx->ep_desc_list[i]->ep_id,
                                             cluster_id, cluster_role, attr_id, manuf_code);
      }
#endif
    }
    i++;
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_conform_singleton", (FMT__0));
}

/* Sets attribute value, perform all needed checks before and after
 * setting new value
 * Note: access_check specifies if it is needed to perform read-only
 * check: end-user application may chanage read-only attributes
*/
zb_zcl_status_t zb_zcl_set_attr_val_manuf(zb_uint8_t ep, zb_uint16_t cluster_id,
                                          zb_uint8_t cluster_role,
                                          zb_uint16_t attr_id,
                                          zb_uint16_t manuf_code,
                                          zb_uint8_t *value,
                                          zb_bool_t check_access)
{
  zb_zcl_attr_t *attr_desc;
  zb_zcl_status_t status = ZB_ZCL_STATUS_FAIL;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_set_attr_val_manuf check_access %hd, ep %hd, cluster role %hd, cluster 0x%x, attr %d manuf %d",
            (FMT__H_H_H_D_D_D, check_access, ep, cluster_role, cluster_id, attr_id, manuf_code));

  attr_desc = zb_zcl_get_attr_desc_manuf_a(ep, cluster_id, cluster_role, attr_id, manuf_code);
  TRACE_MSG(TRACE_ZCL1, "attr_desc %p, value %p", (FMT__P_P, attr_desc, value));

  if (attr_desc != NULL && value != NULL)
  {
    status = (zb_zcl_status_t)zb_zcl_check_attribute_writable(attr_desc, ep, cluster_id, cluster_role, value, check_access);
    if (status == ZB_ZCL_STATUS_SUCCESS)
    {
      zb_zcl_write_attr_hook(ep, cluster_id, cluster_role, attr_id, value, attr_desc->manuf_code);

      ZB_MEMCPY(attr_desc->data_p, value,
                zb_zcl_get_attribute_size(attr_desc->type, value));

#if !(defined ZB_ZCL_DISABLE_REPORTING)
      /* check, if reporting is needed for an attribute */
      if (ZB_ZCL_IS_ATTR_REPORTABLE(attr_desc))
      {
        zb_zcl_mark_attr_for_reporting_manuf(ep, cluster_id, cluster_role, attr_id, attr_desc->manuf_code);
      }
#endif

      if (ZB_ZCL_IS_ATTR_SINGLETON(attr_desc))
      {
        zb_zcl_conform_singleton(ep, cluster_id, cluster_role, attr_id, value, attr_desc->manuf_code);
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_set_attr_val_manuf, cluster_id 0x%x, attr_id 0x%x status %hx",
           (FMT__D_D_H, cluster_id, attr_id, status));
  return status;
}


zb_zcl_status_t zb_zcl_set_attr_val(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role,
                                    zb_uint16_t attr_id, zb_uint8_t *value, zb_bool_t check_access)
{
  zb_zcl_attr_t *attr_desc;
  zb_zcl_status_t status = ZB_ZCL_STATUS_FAIL;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_set_attr_val check_access %hd, ep %hd, cluster role %hd, cluster 0x%x, attr %d",
            (FMT__H_H_H_D_D, check_access, ep, cluster_role, cluster_id, attr_id));

  attr_desc = zb_zcl_get_attr_desc_a(ep, cluster_id, cluster_role, attr_id);
  TRACE_MSG(TRACE_ZCL1, "attr_desc %p, value %p", (FMT__P_P, attr_desc, value));

  if (attr_desc != NULL && value != NULL)
  {
    status = (zb_zcl_status_t)zb_zcl_check_attribute_writable(attr_desc, ep, cluster_id, cluster_role, value, check_access);
    if (status == ZB_ZCL_STATUS_SUCCESS)
    {
      zb_zcl_write_attr_hook(ep, cluster_id, cluster_role, attr_id, value, attr_desc->manuf_code);

      ZB_MEMCPY(attr_desc->data_p, value,
                zb_zcl_get_attribute_size(attr_desc->type, value));

#if !(defined ZB_ZCL_DISABLE_REPORTING)
      /* check, if reporting is needed for an attribute */
      if (ZB_ZCL_IS_ATTR_REPORTABLE(attr_desc))
      {
        zb_zcl_mark_attr_for_reporting_manuf(ep, cluster_id, cluster_role, attr_id, attr_desc->manuf_code);
      }
#endif

      if (ZB_ZCL_IS_ATTR_SINGLETON(attr_desc))
      {
        zb_zcl_conform_singleton(ep, cluster_id, cluster_role, attr_id, value, attr_desc->manuf_code);
      }
    }
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_set_attr_val, cluster_id 0x%x, attr_id 0x%x status %hx",
           (FMT__D_D_H, cluster_id, attr_id, status));
  return status;
}


zb_bool_t zb_zcl_is_target_endpoint(
  zb_af_endpoint_desc_t *ep_desc, zb_uint16_t profile_id)
{
  zb_bool_t match = (ep_desc != NULL && ep_desc->simple_desc != NULL);

  TRACE_MSG(TRACE_ZCL2, "> zb_zcl_is_target_endpoint: ep_desc %p, profile_id 0x%x",
            (FMT__H_H, ep_desc, profile_id));

  if (!match)
  {
    TRACE_MSG(TRACE_ZCL3, "ep or simple desc for it doesn't exist!", (FMT__0));
  }

  /* Endpoint and its simple descriptor exist: check if profile matches */
  /*
    Deliver packet to endpoint if
     - profiles are same;
     - profile is wildcard and  endpoint is HA or ZLL;
     - profile is ZLL and endpoint is HA;
     - profile is legacy and endpoint is HA (r21 spec, 2.3.3.2 Profile ID Endpoint Matching rules)
  */
  if (match)
  {
    zb_uint16_t ep_profile_id = ep_desc->profile_id;

    match = (zb_bool_t)(profile_id == ep_profile_id);

    if (!match)                 /* Profiles are not the same */
    {
      zb_bool_t profile_is_legacy =
        (zb_bool_t)((profile_id != ZB_AF_HA_PROFILE_ID)
                    && (profile_id >= ZB_AF_LEGACY_PROFILE1_ID)
                    && (profile_id <= ZB_AF_LEGACY_PROFILE7_ID));

      match = (zb_bool_t)
        (
          /* profile is wildcard and  endpoint is HA or ZLL */
          ((profile_id == ZB_AF_WILDCARD_PROFILE_ID) &&
           ((ep_profile_id == ZB_AF_HA_PROFILE_ID) || (ep_profile_id == ZB_AF_ZLL_PROFILE_ID)))
          ||
#if 0
          /* It is wrong because it doesn't correspond r21 specification
             (2.3.3.2 Profile ID Endpoint Matching rules) */

          /* profile is HA and endpoint is ZLL */
          ((profile_id == ZB_AF_HA_PROFILE_ID) &&
           (ep_profile_id == ZB_AF_ZLL_PROFILE_ID))
          ||
#endif
          /* profile is ZLL and endpoint is HA */
          ((profile_id == ZB_AF_ZLL_PROFILE_ID) &&
           (ep_profile_id == ZB_AF_HA_PROFILE_ID))
          ||
          /* profile is legacy and endpoint is HA */
          (profile_is_legacy && (ep_profile_id == ZB_AF_HA_PROFILE_ID))
          );
    }

    if (!match)
    {
      TRACE_MSG(TRACE_ZCL3, "ep profile 0x%x doesn't match cmd profile 0x%x!",
                (FMT__D_D, ep_profile_id, profile_id));
    }
  }

  TRACE_MSG(TRACE_ZCL2, "< zb_zcl_is_target_endpoint, match %hd",
            (FMT__H, match));

  return match;
}

zb_uint8_t zb_zcl_get_next_target_endpoint(
  zb_uint8_t start_from_ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t profile_id)
{
  zb_uint8_t ep = 0;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL2,
            "> zb_zcl_get_next_target_endpoint, start_from_ep %hd, cluster_id 0x%x, profile_id 0x%x",
            (FMT__H_D_D, start_from_ep, cluster_id, profile_id));

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  {

    if (start_from_ep != 0U)
    {
      for (i = 0; i < device_ctx->ep_count; i++)
      {
        if (device_ctx->ep_desc_list[i]->ep_id == start_from_ep)
        {
          start_from_ep = 0;
          break;
        }
      }

      /* Start from the next to start_from_ep endpoint descriptor */
      i++;
    }

    for (; start_from_ep == 0U && i < device_ctx->ep_count; i++)
    {
      zb_af_endpoint_desc_t *ep_desc = device_ctx->ep_desc_list[i];

      if ((cluster_role == ZB_ZCL_CLUSTER_ANY_ROLE)
              ? (get_cluster_desc(ep_desc, cluster_id, ZB_ZCL_CLUSTER_SERVER_ROLE) != NULL
                 /*cstat !MISRAC2012-Rule-13.5 */
                 /* After some investigation, the following violation of Rule 13.5 seems to be
                  * a false positive. There are no side effect to
                  * 'zb_aib_trust_center_address_cmp()'. */
                 || get_cluster_desc(ep_desc, cluster_id, ZB_ZCL_CLUSTER_CLIENT_ROLE) != NULL)
              : get_cluster_desc(ep_desc, cluster_id, cluster_role) != NULL)
      {
        if (zb_zcl_is_target_endpoint(ep_desc, profile_id))
        {
          ep = ep_desc->ep_id;
          ZCL_DEVICE_CONTEXT_BREAK;
        }
      }
    }
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZCL2, "< zb_zcl_get_next_target_endpoint, ep %hd", (FMT__H, ep));

  return ep;
}


#if defined ZB_APS_ENCRYPTION_PER_CLUSTER
void zb_zcl_set_cluster_encryption(zb_uint8_t endpoint_id, zb_uint16_t cluster_id, zb_uint8_t encrypt)
{
  zb_af_endpoint_desc_t *ep_desc;
  zb_uindex_t i = 0;
  zb_uint8_t *cluster_encryption;
  zb_uint8_t clusters_count;
  TRACE_MSG(TRACE_ZCL2, ">> zb_zcl_set_cluster_encryption, ep %hd, cluster 0x%x, encrypt %d", (FMT__H_D_D, endpoint_id, cluster_id, encrypt));
  ep_desc = zb_af_get_endpoint_desc(endpoint_id);

  if (ep_desc)
  {
    clusters_count = ep_desc->simple_desc->app_input_cluster_count +
      ep_desc->simple_desc->app_output_cluster_count;
    cluster_encryption = (zb_uint8_t*)&(ep_desc->simple_desc->app_cluster_list[clusters_count]);
    while (i < clusters_count)
    {
      if (ep_desc->simple_desc->app_cluster_list[i] == cluster_id)
      {
        if (encrypt)
        {
          ZB_SET_BIT_IN_BIT_VECTOR(cluster_encryption, i);
          TRACE_MSG(TRACE_ZCL2, "encryption for cluster 0x%x is ON", (FMT__D, cluster_id));
        }
        else
        {
          ZB_CLR_BIT_IN_BIT_VECTOR(cluster_encryption, i);
          TRACE_MSG(TRACE_ZCL2, "encryption for cluster 0x%x is OFF", (FMT__D, cluster_id));
        }
        /* TODO: Set encryption for different cluster roles independently. */
        break;
      }
      i++;
    }
  }
  TRACE_MSG(TRACE_ZCL2, "<< zb_zcl_set_cluster_encryption", (FMT__0));
}

zb_bool_t cluster_get_encryption_from_desc(zb_uint8_t endpoint_id, zb_uint16_t cluster_id)
{
  zb_af_endpoint_desc_t *ep_desc;
  zb_uindex_t i = 0;
  zb_uint8_t *cluster_encryption;
  zb_uint8_t clusters_count;
  zb_bool_t res = ZB_FALSE;

  ep_desc = zb_af_get_endpoint_desc(endpoint_id);

  if (ep_desc)
  {
    clusters_count = ep_desc->simple_desc->app_input_cluster_count +
      ep_desc->simple_desc->app_output_cluster_count;
    cluster_encryption = (zb_uint8_t*)&(ep_desc->simple_desc->app_cluster_list[clusters_count]);
    while (i < clusters_count)
    {
      if (ep_desc->simple_desc->app_cluster_list[i] == cluster_id)
      {
        if (ZB_CHECK_BIT_IN_BIT_VECTOR(cluster_encryption, i))
        {
          res = ZB_TRUE;
        }
        break;
      }
      i++;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "cluster_get_encryption_from_desc: ep %hd cluster 0x%x res %hd", (FMT__H_D_H, endpoint_id, cluster_id, res));

  return res;
}
#endif  /* ZB_APS_ENCRYPTION_PER_CLUSTER */

zb_bool_t cluster_needs_aps_encryption(zb_uint8_t endpoint_id, zb_uint16_t cluster_id)
{
  zb_bool_t res = ZB_FALSE;
  zb_uint16_t profile_id = get_profile_id_by_endpoint(endpoint_id);

  if (endpoint_id == 0U)
  {
    return res;
  }

  /* See SE 1.4 spec, Table 5-12 – Security Key Assignments per Cluster
   *
   * Unless stated otherwise, any ZCL clusters added to a Smart Energy endpoint shall be APS
   * encrypted. ZCL clusters without APS encryption shall be located on another (non-Smart Energy)
   * endpoint. The same restrictions shall also apply to manufacturer-specific clusters.
   */
#ifdef ZB_ENABLE_SE
  if (profile_id == ZB_AF_SE_PROFILE_ID)
  {
    switch (cluster_id)
    {
      case ZB_ZCL_CLUSTER_ID_BASIC:
      case ZB_ZCL_CLUSTER_ID_IDENTIFY:
      case ZB_ZCL_CLUSTER_ID_ALARMS:
      case ZB_ZCL_CLUSTER_ID_POWER_CONFIG:
      case ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT:
        res = ZB_FALSE;
        break;

      default:
        res = ZB_TRUE;
        break;
    }
  }
  else
#endif
      if (profile_id == ZB_AF_HA_PROFILE_ID
          && cluster_id == ZB_ZCL_CLUSTER_ID_WWAH)
  {
    /* WWAH cluster is always encrypted */
    res = ZB_TRUE;
  }
#ifdef ZB_APS_ENCRYPTION_PER_CLUSTER
  else
  {
    res = cluster_get_encryption_from_desc(endpoint_id, cluster_id);
  }
#endif
#if defined ZB_ENABLE_SE && !defined ZB_APS_ENCRYPTION_PER_CLUSTER
  else
  {
    /* MISRA rule 15.7 requires empty 'else' branch. */
  }
#endif /* defined ZB_ENABLE_SE && !defined ZB_APS_ENCRYPTION_PER_CLUSTER */

  TRACE_MSG(TRACE_ZCL1, "cluster_needs_aps_encryption: ep %hd cluster 0x%x res %hd", (FMT__H_D_H, endpoint_id, cluster_id, res));

  return res;
}

static void ep_process_zcl_cmd(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t ep;
  zb_af_endpoint_desc_t *ep_desc;
  zb_uint8_t status = ZB_ZCL_STATUS_FAIL;

  TRACE_MSG(TRACE_ZCL2, "> ep_process_zcl_cmd, param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  ep = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
  ep_desc = zb_af_get_endpoint_desc(ep);
  ZB_ASSERT(ep_desc);

#ifdef ZB_ZCL_SUPPORT_CLUSTER_BASIC
  if(!zb_zcl_check_is_device_enabled(ep, cmd_info.cmd_id, cmd_info.cluster_id, cmd_info.is_common_command))
  {
    zb_buf_free(param);
    status = ZB_ZCL_STATUS_SUCCESS;
    goto send_default_response; /* to avoid sending default response */
  }
#endif

#if defined SNCP_MODE
  if (!ZB_SE_MODE())
#else
  if (ZB_SE_MODE())
#endif
  /*cstat !MISRAC2012-Rule-2.1_b */
  /** @mdr{00010,3} */
  {
#if !defined ZB_ED_ROLE && defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
    zb_bool_t is_dev_suspended = zb_subghz_srv_device_suspended(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr);
    if (
    /* Special check for ZB_ZCL_CMD_OTA_UPGRADE_IMAGE_BLOCK_ID:
b. If active, the OTA server shall respond to any Image Block Request
command with an 3090 Image Block Response command with a status of
WAIT_FOR_DATA.
    */
#if defined ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE && defined ZB_HA_ENABLE_OTA_UPGRADE_SERVER && defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
      zb_subghz_catch_ota_image_block_req(&param)
#else
      ZB_FALSE
#endif
/*
The Coordinator device shall respond to ZCL-layer messages received
from any device 3092 whilst in the 'Critical' state with a
Suspend ZCL Messages command indicating the 3093 required period of
suspension. The incoming request packet will be ignored.
 */
      || is_dev_suspended)
    {
      status = ZB_ZCL_STATUS_SUCCESS; /* to avoid sending default response */
      goto send_default_response;
    }
#endif  /* ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ */
    /* SE 1.4 spec, sublclause 5.4.6
     *
     * ... all SE applications shall use and validate the Security key usage as listed in Table 5-12.
     * If link key encryption is NOT used but required, the receiving device shall generate a
     * ZCL Default Response, employing the Network Key, with a FAILURE (0x01) status code.
     */

    TRACE_MSG(TRACE_ZCL2, "ZCL in: cluster: 0x%x, security: %d",
              (FMT__D_D, cmd_info.cluster_id,
               cluster_needs_aps_encryption(ep, cmd_info.cluster_id)));

#ifdef ZB_ENABLE_SE
    TRACE_MSG(TRACE_ZCL2, "APS key attrs: 0x%x, key source: 0x%x",
              (FMT__D_D,
               cmd_info.addr_data.common_data.aps_key_attrs,
               cmd_info.addr_data.common_data.aps_key_source));
#endif

    /* SE+BDB mode: May be no APS encryption (0 key attrs, 0 key source).
       TODO: Check somehow that we have any verified key (CBKE or non-CBKE) with src device? */
    /* SE mode: check key is verified by CBKE */
    if (cluster_needs_aps_encryption(ep, cmd_info.cluster_id) == ZB_TRUE &&
        (!ZB_APS_FC_IS_SECURE(cmd_info.addr_data.common_data.fc)
#ifdef ZB_ENABLE_SE
         || (ZB_SE_MODE()
             && (cmd_info.addr_data.common_data.aps_key_source != ZB_SECUR_KEY_SRC_CBKE
                 || cmd_info.addr_data.common_data.aps_key_attrs != ZB_SECUR_VERIFIED_KEY))
#endif
          )
      )
    {
      TRACE_MSG(TRACE_ZCL2, "ZCL in: DROP frame due to unverified/non-cbke APS key was used to secure it", (FMT__0));
      status = ZB_ZCL_STATUS_FAIL;

      /* DV: NOTE I think this is the case when 'goto' leaves code readable and makes
       *    it shorter. It is used to jump to the function end to the common error processing
       *    block. Using 'else' here will create nested block. The epic case of multiple nested
       *    blocks + multiple #ifdef clauses is zb_nlde_data_indication_continue() which is
       *    almost unreadable and hardly modifiable.
       */
      goto send_default_response;
    }

    TRACE_MSG(TRACE_ZCL2, "ZCL (in): PASS frame, verified CBKE key was used to secure it", (FMT__0));
  }

  /* On receipt of "APS Link Key Authorization" command, a device SHALL henceforth enforce APS
   * (trust center) level security for all cluster commands (including global cluster commands i.e.
   * Read/Write Attribute) except for those in the exemption list given. */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
  if (ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR() &&
      ((ZB_ZDO_CHECK_IF_APS_LINK_KEY_AUTHORIZATION_NEEDED(cmd_info.cluster_id) &&
        !cmd_info.addr_data.common_data.aps_key_from_tc) ||
       !ZB_ZDO_CHECK_SRC_ADDRESS_FOR_CLUSTER(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                                             cmd_info.cluster_id)))
    {
     TRACE_MSG(TRACE_ZCL2, "ZCL in: not authorized by WWAH", (FMT__0));
     status = ZB_ZCL_STATUS_NOT_AUTHORIZED;
     goto send_default_response;
   }
#endif
  TRACE_MSG(TRACE_ZCL2, "ep %hd, dev handler %p", (FMT__H_P, ep, ep_desc->device_handler));

  /* Copy zcl cmd before processing in device_handler.
     Rationale: device_handler may decide that command is processed and reuse/clear the buffer, but
     for some cases we still need to process this cmd in stack. Copy this cmd to another buffer, and
     process it after.
   */
#ifdef ZB_TH_ENABLED
  ZB_TH_PUSH_PACKET(ZB_TH_ZCL_DATA, ZB_TH_PRIMITIVE_INDICATION, param);

  if (ep_desc->device_handler)
  {
      TRACE_MSG(TRACE_ZCL2, "call ep handler %hd", (FMT__H, ep));
      status = ((*ep_desc->device_handler)(param) ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);
  }
#else
  if (ep_desc->device_handler != NULL
#if defined ZB_SE_COMMISSIONING || (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
      && (ZCL_SELECTOR().block_zcl_cmd == NULL
          || !ZCL_SELECTOR().block_zcl_cmd(&cmd_info))
#endif /* ZB_SE_COMMISSIONING || (ZB_ZCL_SUPPORT_CLUSTER_WWAH && ZB_ZCL_ENABLE_WWAH_SERVER) */
    )
  {
    status = (*ep_desc->device_handler)(param) != 0U ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL;
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "skip ep handler ep %hd", (FMT__H, ep));
  }
#endif

  if (status != ZB_ZCL_STATUS_SUCCESS)
  {
    status = zb_zcl_handle(param);
  }

send_default_response:
  if (status != ZB_ZCL_STATUS_SUCCESS)
  {
    zb_zcl_send_default_resp_ext(param, &cmd_info, (zb_zcl_status_t)status);
  }

  TRACE_MSG(TRACE_ZCL2, "< ep_process_zcl_cmd", (FMT__0));
}


static void broadcast_endpoint_call_next_delivery_step(zb_uint8_t param)
{
  zb_ret_t ret = zb_buf_get_out_delayed_ext(broadcast_endpoint_delivery_step, param, 0);
  if (ret != RET_OK)
  {
    TRACE_MSG(TRACE_ERROR, "Failed zb_buf_get_out_delayed [%d]", (FMT__D, ret));
    zb_buf_free(param);
  }
}


static void broadcast_endpoint_delivery_step(zb_uint8_t param, zb_uint16_t bc_buf_ref)
{
  zb_zcl_parsed_hdr_t *cmd_info_ptr;
  zb_uint8_t ep;
  zb_uint32_t process_command_jitter;

  ZB_ASSERT(bc_buf_ref <= ZB_UINT8_MAX);
  cmd_info_ptr = ZB_BUF_GET_PARAM((zb_uint8_t)bc_buf_ref, zb_zcl_parsed_hdr_t);
  zb_buf_copy(param, (zb_uint8_t)bc_buf_ref);

  ep = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info_ptr).dst_endpoint;

  TRACE_MSG(TRACE_ZCL2, "> broadcast_endpoint_delivery_step, param %hd, bc_buf_ref %hd",
            (FMT__H_H, param, bc_buf_ref));
  TRACE_MSG(TRACE_ZCL3, "deliver to ep %hd", (FMT__H, ep));

  /*
     ZCL 6.0
     2.3.2.2 A device processing a ZCL message sent to the broadcast endpoint
     SHOULD jitter messages that are sent in response
  */
  process_command_jitter = ZB_RANDOM_JTR(ZB_ZCL_BROADCAST_ENDPOINT_CMD_RESP_JITTER);
  ZB_ASSERT(process_command_jitter <= ZB_UINT16_MAX);
  ZB_SCHEDULE_ALARM(ep_process_zcl_cmd, param, (zb_uint16_t)process_command_jitter);

  ep = zb_zcl_get_next_target_endpoint(
    ep,
    cmd_info_ptr->cluster_id,
    cmd_info_ptr->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV ?
    ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE,
    cmd_info_ptr->profile_id);
  /* Cmd is delivered to all matching endpoints: free initial buffer */
  if (ep == 0U)
  {
    ZB_ASSERT(bc_buf_ref <= ZB_UINT8_MAX);
    zb_buf_free((zb_uint8_t)bc_buf_ref);
  }
  /* Proceed to next endpoint */
  else
  {
    TRACE_MSG(TRACE_ZCL3, "wait new buf to handle ep %hd, jitter %d", (FMT__H_D, ep, process_command_jitter));

    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info_ptr).dst_endpoint = ep;
    ZB_ASSERT(bc_buf_ref <= ZB_UINT8_MAX);
    ZB_SCHEDULE_ALARM(broadcast_endpoint_call_next_delivery_step, (zb_uint8_t)bc_buf_ref, process_command_jitter);
  }
}


void zb_zcl_process_parsed_zcl_cmd(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info_ptr = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_parsed_zcl_cmd %hd", (FMT__H, param));

  if (ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info_ptr).dst_endpoint == ZB_ZCL_BROADCAST_ENDPOINT)
  {
    zb_uint8_t ep = zb_zcl_get_next_target_endpoint(
      0,
      cmd_info_ptr->cluster_id,
      cmd_info_ptr->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV ?
      ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE,
      cmd_info_ptr->profile_id);

    /* Fixed assert when the coordinator receives its own message relayed by the smart plug */
    /* ZB_ASSERT(ep); */

    if (ep == 0U)
    {
      /* Frame with broadcast ep was passed to zcl by zb_apsde_data_indication() with
       * ZB_ZCL_CLUSTER_ANY_ROLE. If we receive such frame [Identify Query for example]
       * but zcl cluster role does not match, just drop the frame.
       */
      TRACE_MSG(TRACE_ZCL1, "zb_zcl_process_parsed_zcl_cmd(): role does not match, drop", (FMT__0));
      zb_buf_free(param);
    }
    else
    {
      zb_ret_t ret;

      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info_ptr).dst_endpoint = ep;
      ret = zb_buf_get_out_delayed_ext(broadcast_endpoint_delivery_step, param, 0);
      if (ret != RET_OK)
      {
        TRACE_MSG(TRACE_ERROR, "Failed zb_buf_get_out_delayed [%d]", (FMT__D, ret));
        zb_buf_free(param);
      }
    }
  }
  else
  {
    ep_process_zcl_cmd(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_parsed_zcl_cmd", (FMT__0));
}

void zb_zcl_process_device_command(zb_uint8_t param)
{
  /* ZCL command information surely present */
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_parsed_hdr_t *cmd_info_ptr;
  zb_zcl_status_t status;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_device_command %hd", (FMT__H, param));

  ZB_BZERO(&cmd_info, sizeof(cmd_info));
  status = zb_zcl_parse_header(param, &cmd_info);

  if (!NCP_CATCH_ZCL_PACKET(param, &cmd_info, status))
  {
    /* Cut ZCL header, asdu buffer points to ZCL payload */
    /* Note: moved here from zb_zcl_parse_header because we must not cut it for NCP. */
    ZB_ZCL_CUT_HEADER(param);

    /* save parsed data to buffer */
    cmd_info_ptr = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
    ZB_MEMCPY(cmd_info_ptr, &cmd_info, sizeof(zb_zcl_parsed_hdr_t));

    if(status == ZB_ZCL_STATUS_SUCCESS)
    {
      zb_zcl_process_parsed_zcl_cmd(param);
    }
    else
    {
      zb_zcl_send_default_resp_ext(param, &cmd_info, status);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_device_command", (FMT__0));
}

/*
  Search for cluster descriptor by its ID
*/
zb_zcl_cluster_desc_t* get_cluster_desc(const zb_af_endpoint_desc_t* ep_desc, zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = 0;
  zb_zcl_cluster_desc_t *cluster_desc = NULL;

  TRACE_MSG(TRACE_ZCL3, ">>get_cluster_desc ep_desc %p, cluster_id 0x%x cluster_role %hd", (FMT__P_D_H, ep_desc, cluster_id, cluster_role));
  if (ep_desc != NULL)
  {
    while (i < ep_desc->cluster_count)
    {
      if (ep_desc->cluster_desc_list[i].cluster_id == cluster_id &&
          ep_desc->cluster_desc_list[i].role_mask == cluster_role)
      {
        cluster_desc = &ep_desc->cluster_desc_list[i];
        break;
      }
      i++;
    }
  }

  TRACE_MSG(TRACE_ZCL3, "<<get_cluster_desc cluster_desc %p", (FMT__P, cluster_desc));
  return cluster_desc;
}

zb_uint8_t get_endpoint_by_cluster(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uint8_t endpoint = 0;
  zb_uindex_t i;
  zb_af_endpoint_desc_t *ep_desc;

  TRACE_MSG(TRACE_ZCL3, ">>get_endpoint_by_cluster cluster id = 0x%x", (FMT__D, cluster_id));

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  while (i < device_ctx->ep_count)
  {
    ep_desc = device_ctx->ep_desc_list[i];
    if (get_cluster_desc(ep_desc, cluster_id, cluster_role) != NULL)
    {
      endpoint = device_ctx->ep_desc_list[i]->ep_id;
      ZCL_DEVICE_CONTEXT_BREAK;
    }
    i++;
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZCL1, "<<get_endpoint_by_cluster endpoint = %hd", (FMT__H, endpoint));
  return endpoint;
}

zb_bool_t is_cluster_present(zb_uint16_t cluster_id)
{
  zb_bool_t ret = ZB_FALSE;
  zb_uindex_t i, j = 0;
  zb_af_endpoint_desc_t *ep_desc;

  TRACE_MSG(TRACE_ZCL1, ">>is_cluster_present cluster id = %hd", (FMT__H, cluster_id));

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  while (i < device_ctx->ep_count)
  {
    ep_desc = device_ctx->ep_desc_list[i];
    if (ep_desc != NULL)
    {
      while (j < ep_desc->cluster_count)
      {
        if (ep_desc->cluster_desc_list[j].cluster_id == cluster_id)
        {
          ret = ZB_TRUE;
          break;
        }
        j++;
      }
    }
    if (ret == ZB_TRUE)
    {
      ZCL_DEVICE_CONTEXT_BREAK;
    }
    i++;
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZCL1, "<<is_cluster_present ret = %hd", (FMT__H, ret));
  return ret;
}

zb_uint16_t get_profile_id_by_endpoint(zb_uint8_t endpoint_id)
{
  zb_uint16_t profile_id = ZB_AF_WILDCARD_PROFILE_ID;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL3, ">>get_profile_id_by_endpoint endpoint_id = %hd", (FMT__H, endpoint_id));

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  while (i < device_ctx->ep_count)
  {
    if (device_ctx->ep_desc_list[i]->ep_id == endpoint_id)
    {
      profile_id = device_ctx->ep_desc_list[i]->profile_id;
      ZCL_DEVICE_CONTEXT_BREAK;
    }
    i++;
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZCL1, "<<get_profile_id_by_endpoint profile_id = 0x%x", (FMT__D, profile_id));
  return profile_id;
}

zb_af_endpoint_desc_t *get_endpoint_by_cluster_with_role(
    zb_uint16_t cluster_id,
    zb_uint8_t cluster_role)
{
  zb_uint_t i;
  zb_af_endpoint_desc_t *ret = NULL;
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;

  TRACE_MSG(TRACE_ZGP2, ">> get_endpoint_by_cluster_with_role, cluster_id 0x%x, cluster_role %hd",
      (FMT__D_H, cluster_id, cluster_role));

  /*cstat !MISRAC2012-Rule-13.6 */
  /* After some investigation, the following violation of Rule 13.6 seems to be a false positive.
   * There are no side effects to the macro 'ZCL_CTX()' used inside 'ZCL_DEVICE_CONTEXT_ITERATOR()'.
   * This violation seems to be caused by the fact that 'ZCL_CTX()' is an external macro, which
   * cannot be analyzed by C-STAT.*/
  ZCL_DEVICE_CONTEXT_ITERATOR(i);
  while (i < device_ctx->ep_count)
  {
    ep_desc = device_ctx->ep_desc_list[i];
    cluster_desc = get_cluster_desc(ep_desc, cluster_id, cluster_role);
    TRACE_MSG(TRACE_ZGP2, "cluster_desc %p role_mask 0x%x", (FMT__P_D, cluster_desc, cluster_desc ? cluster_desc->role_mask : -1));
    if ((cluster_desc != NULL) && (cluster_desc->role_mask == cluster_role))
    {
      ret = ep_desc;
      ZCL_DEVICE_CONTEXT_BREAK;
    }
    i++;
  }
  ZCL_DEVICE_CONTEXT_ITERATOR_END;

  TRACE_MSG(TRACE_ZGP2, "<< get_endpoint_by_cluster_with_role, ret %p", (FMT__P, ret));

  return ret;
}


/* Check if command should be handled - check command direction and
 * cluster role */
zb_uint8_t zb_zcl_check_accept_command(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_af_endpoint_desc_t *endpoint_desc;
  zb_uint8_t status;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_check_accept_command %hd", (FMT__H, param));

  endpoint_desc = zb_af_get_endpoint_desc(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  cluster_desc = get_cluster_desc(endpoint_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);

  ZB_ZCL_DEBUG_DUMP_HEADER(cmd_info);

  /* if command is manufacturer specific - skip handling it by default */
  TRACE_MSG(TRACE_ZCL2, "manuf_spec %hd", (FMT__H, cmd_info->is_manuf_specific));

  /* ZCL8: CCB 2477: use UNSUP_COMMAND instead of any other Unsupported command status */
  if (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE)
  {
    status = ZB_ZCL_STATUS_UNSUP_CMD;
  }
  else
  {
    if (cmd_info->is_manuf_specific)
    {
      status = cmd_info->is_common_command ?
        ZB_ZCL_STATUS_UNSUP_MANUF_GEN_CMD : ZB_ZCL_STATUS_UNSUP_MANUF_CLUST_CMD;
    }
    else
    {
      status = cmd_info->is_common_command ?
        ZB_ZCL_STATUS_UNSUP_GEN_CMD : ZB_ZCL_STATUS_UNSUP_CLUST_CMD;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "check cluster, cluster_desc %p cluster_desc->attr_desc_list %p",
            (FMT__P_P, cluster_desc, cluster_desc ? cluster_desc->attr_desc_list : NULL));
  if (cluster_desc != NULL)
  {
    TRACE_MSG(TRACE_ZCL1, "cmd_direction %hd, cluster role %hx",
              (FMT__H_H, cmd_info->cmd_direction, cluster_desc->role_mask));
    if ((cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI
         && ZB_BIT_IS_SET(cluster_desc->role_mask, ZB_ZCL_CLUSTER_CLIENT_ROLE))
        || (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV
            && ZB_BIT_IS_SET(cluster_desc->role_mask, ZB_ZCL_CLUSTER_SERVER_ROLE)))
    {
      status = ZB_ZCL_STATUS_SUCCESS;
    }

#ifdef ZB_ENABLE_SE
    if (zb_zdo_joined()
        && !(ZB_IS_DEVICE_ZC() && ZB_SUBGHZ_SWITCH_MODE())
        && (cluster_desc->cluster_id) == ZB_ZCL_CLUSTER_ID_SUB_GHZ
        /*cstat !MISRAC2012-Rule-13.5 */
        /* After some investigation, the following violation of Rule 13.5 seems to be
         * a false positive. There are no side effect to 'zb_get_current_page()'. This
         * violation seems to be caused by the fact that 'zb_get_current_page()' is an
         * external function, which cannot be analyzed by C-STAT. */
        && zb_get_current_page() == 0U)
    {
      status = ZB_ZCL_STATUS_UNSUP_CLUST;
      TRACE_MSG(TRACE_ZCL2, "Not in SubGHz mode - filter out SubGHz cluster", (FMT__0));
    }
#endif
  }
  else
  {
    status = ZB_ZCL_STATUS_UNSUP_CLUST;
  }
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_check_accept_command %hd", (FMT__H, status));

  return status;
}

#ifdef ZB_NEED_ALIGN

zb_uint16_t zb_zcl_attr_get16(zb_uint8_t *value)
{
  zb_uint16_t v;
  ZB_MEMCPY(&v, value, 2);
  return v;
}

zb_int16_t zb_zcl_attr_gets16(zb_uint8_t *value)
{
  zb_int16_t v;
  ZB_MEMCPY(&v, value, 2);
  return v;
}

zb_uint32_t zb_zcl_attr_get32(zb_uint8_t *value)
{
  zb_uint32_t v;
  ZB_MEMCPY(&v, value, 4);
  return v;
}

zb_int32_t zb_zcl_attr_gets32(zb_uint8_t *value)
{
  zb_int32_t v;
  ZB_MEMCPY(&v, value, 4);
  return v;
}

zb_int24_t zb_zcl_attr_get24(zb_uint8_t *value)
{
  zb_int24_t v;

#ifdef ZB_UINT24_48_SUPPORT
  ZB_MEMCPY(&v, value, 3);
#else
  ZB_MEMCPY(&v, value, 4);
#endif
  return v;
}

zb_uint48_t zb_zcl_attr_get48(zb_uint8_t *value)
{
  zb_uint48_t v;
#ifdef ZB_UINT24_48_SUPPORT
  ZB_MEMCPY(&v, value, 6);
#else
  ZB_MEMCPY(&v, value, 4);
#endif
  return v;
}
#endif

#ifndef ZB_LITTLE_ENDIAN
void zb_reverse_bytes(zb_uint8_t ZB_XDATA *ptr, zb_uint8_t ZB_XDATA *val, zb_uint8_t size)
{
  zb_uindex_t i;
  for (i=0; i<size; i++)
  {
    ptr[i] = val[size-1 - i];
  }
}
#endif /* !ZB_LITTLE_ENDIAN */

#if TRACE_ENABLED(TRACE_ZCL1)
void zb_zcl_dump_cmd(zb_bufid_t buf)
{
  zb_uint8_t *ptr;
  ptr = zb_buf_begin(buf);
  TRACE_MSG(TRACE_ZCL1, "ZCL packet: buf %p, ref %hd, len %hd", (FMT__P_H_H, buf, buf, zb_buf_len(buf)));
  TRACE_MSG(TRACE_ZCL1, "FC: 0x%hx TSN: 0x%hx CMD: 0x%hx", (FMT__H_H_H, *ptr, *(ptr + 1), *(ptr + 2)));
}
#endif /* TRACE_ENABLED(TRACE_ZCL1) */

#if TRACE_ENABLED(TRACE_ZCL3)
void dump_zcl_header(zb_zcl_parsed_hdr_t *header)
{
  ZVUNUSED(header);

  TRACE_MSG(TRACE_ZCL3, "> dump_zcl_header at 0x%hx", (FMT__H, header));
  TRACE_MSG(
      TRACE_ZCL3,
      "src: address 0x%x, endpoint %hd",
      (FMT__D_H,
       ZB_ZCL_PARSED_HDR_SHORT_DATA(header).source.u.short_addr,
       ZB_ZCL_PARSED_HDR_SHORT_DATA(header).src_endpoint));
  TRACE_MSG(
      TRACE_ZCL3,
      "dst: address 0x%x, endpoint %hd",
      (FMT__D_H,
       ZB_ZCL_PARSED_HDR_SHORT_DATA(header).dst_addr,
       ZB_ZCL_PARSED_HDR_SHORT_DATA(header).dst_endpoint));
  TRACE_MSG(TRACE_ZCL3, "profile 0x%x, cluster 0x%x, cmdID 0x%x",
            (FMT__D_D_H, header->profile_id, header->cluster_id, header->cmd_id));
  TRACE_MSG(
      TRACE_ZCL3,
      "command direction flag (0 - to server): %hd",
      (FMT__H, header->cmd_direction));
  TRACE_MSG(
      TRACE_ZCL3,
      "sequence number: %hd, is common: %hd",
      (FMT__H_H, header->seq_number, header->is_common_command));
  TRACE_MSG(
      TRACE_ZCL3,
      "manufacturer specific %hd, manuf specific data %d",
      (FMT__H_D, header->is_manuf_specific, header->manuf_specific));
  TRACE_MSG(TRACE_ZCL3, "def resp disabled: %hd", (FMT__H, header->disable_default_response));
  TRACE_MSG(TRACE_ZCL3, "< dump_zcl_header", (FMT__0));
}
#endif /* TRACE_ENABLED(TRACE_ZCL3) */

void zb_zcl_schedule_status_abort(zb_bufid_t  buffer, zb_addr_u *addr,
                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                  zb_uint8_t ep, zb_callback_t cb)
{
  zb_zcl_command_send_status_t *cmd_send_status =
    ZB_BUF_GET_PARAM(buffer, zb_zcl_command_send_status_t);
  zb_nwk_unlock_in(buffer);
  cmd_send_status->status       = (zb_ret_t)ZB_ZCL_STATUS_ABORT;
  cmd_send_status->src_endpoint = (ep);
  cmd_send_status->dst_endpoint = (dst_ep);
  cmd_send_status->dst_addr.addr_type    = (dst_addr_mode);
  if ((dst_addr_mode) == ZB_APS_ADDR_MODE_64_ENDP_PRESENT)
  {
    ZB_IEEE_ADDR_COPY(cmd_send_status->dst_addr.u.ieee_addr, (void const *)(addr));
  }
  else
  {
    ZB_MEMCPY(&cmd_send_status->dst_addr.u.short_addr, &(addr), sizeof(zb_uint16_t));
  }
  ZB_SCHEDULE_CALLBACK(cb, buffer);
}

#ifdef ZB_USEALIAS
void zb_zcl_send_command_short_alias(zb_bufid_t buffer,
                                          zb_addr_u *addr, zb_uint8_t dst_addr_mode,
                                          zb_uint8_t dst_ep, zb_uint8_t ep,
                                          zb_uint16_t prof_id, zb_uint16_t cluster_id,
                                          zb_uint8_t radius, zb_callback_t cb, zb_uint8_t use_alias,
                                          zb_uint16_t alias_addr, zb_uint8_t alias_seq)
{
  zb_apsde_data_req_t *apsde_req;
#ifdef ZB_ZCL_SUPPORT_CLUSTER_BASIC
  zb_zcl_frame_hdr_short_t *zcl_cmd = (zb_zcl_frame_hdr_short_t *)zb_buf_begin(buffer);
  if (!zb_zcl_check_is_device_enabled(ep,
                                      zcl_cmd->command_id,
                                      cluster_id,
                                      (zcl_cmd->frame_ctrl.frame_type == ZB_ZCL_FRAME_TYPE_COMMON) ? ZB_TRUE : ZB_FALSE))
  {
    (cb != NULL) ? ZB_ZCL_SCHEDULE_STATUS_ABORT(buffer, addr, dst_addr_mode, dst_ep, ep, cb) : zb_buf_free(buffer);
    return;
  }
#endif
  apsde_req = ZB_BUF_GET_PARAM(buffer, zb_apsde_data_req_t);

  ZB_BZERO(apsde_req, sizeof(zb_apsde_data_req_t));
  apsde_req->addr_mode = dst_addr_mode;
  apsde_req->dst_addr = *addr;
  apsde_req->dst_endpoint = dst_ep;
  apsde_req->src_endpoint = ep;
  apsde_req->profileid = prof_id;
  apsde_req->clusterid = cluster_id;
  apsde_req->radius = radius;

  if (use_alias == ZB_TRUE)
  {
    apsde_req->alias_src_addr = alias_addr;
    apsde_req->alias_seq_num = alias_seq;
    apsde_req->use_alias = ZB_TRUE;
  }

  if ((apsde_req->addr_mode != ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT) &&
      (!ZB_NWK_IS_ADDRESS_BROADCAST(addr->addr_short)))
  {
    apsde_req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
  }

  if (zb_zcl_register_cb(buffer, cb) == RET_OK)
  {
    ZB_ZCL_DEBUG_DUMP_COMMAND(buffer);
    TRACE_MSG(TRACE_ZDO3, "Call zb_zdo_pim_start_turbo_poll_packets 1", (FMT__0));
    zb_zdo_pim_start_turbo_poll_packets(1);
    ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, buffer);
  }
  else
  {
    /* cb != NULL here. */
    ZB_ZCL_SCHEDULE_STATUS_ABORT(buffer, addr, dst_addr_mode, dst_ep, ep, cb);
  }
}
#endif /* ZB_USEALIAS */

void zb_zcl_send_command_short_schedule(zb_bufid_t buffer,
                                             zb_uint16_t addr, zb_uint8_t dst_addr_mode,
                                             zb_uint8_t dst_ep, zb_uint8_t ep,
                                             zb_uint16_t prof_id, zb_uint16_t cluster_id,
                                             zb_callback_t cb, zb_uint64_t delay)
{
  zb_apsde_data_req_t *apsde_req;
#ifdef ZB_ZCL_SUPPORT_CLUSTER_BASIC
  zb_zcl_frame_hdr_short_t *zcl_cmd = (zb_zcl_frame_hdr_short_t *)zb_buf_begin(buffer);
  if (!zb_zcl_check_is_device_enabled(ep,
                                      zcl_cmd->command_id,
                                      cluster_id,
                                      (zcl_cmd->frame_ctrl.frame_type == ZB_ZCL_FRAME_TYPE_COMMON) ? ZB_TRUE : ZB_FALSE))
  {
    zb_addr_u short_addr;
    short_addr.addr_short = addr;
    (cb != NULL) ? ZB_ZCL_SCHEDULE_STATUS_ABORT(buffer, &short_addr, dst_addr_mode, dst_ep, ep, cb) : zb_buf_free(buffer);
    return;
  }
#endif
  apsde_req = ZB_BUF_GET_PARAM(buffer, zb_apsde_data_req_t);

  ZB_BZERO(apsde_req, sizeof(zb_apsde_data_req_t));
  apsde_req->addr_mode = dst_addr_mode;
  apsde_req->dst_addr.addr_short = addr;
  apsde_req->dst_endpoint = dst_ep;
  apsde_req->src_endpoint = ep;
  apsde_req->profileid = prof_id;
  apsde_req->clusterid = cluster_id;

  if ((apsde_req->addr_mode != ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT) &&
      (!ZB_NWK_IS_ADDRESS_BROADCAST(addr)))
  {
    apsde_req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
  }

  if (zb_zcl_register_cb(buffer, cb) == RET_OK)
  {
    ZB_ZCL_DEBUG_DUMP_COMMAND(buffer);
#ifdef ZB_ED_FUNC
    TRACE_MSG(TRACE_ZDO3, "Call zb_zdo_pim_start_turbo_poll_packets 1 after alarm", (FMT__0));
    ZB_SCHEDULE_ALARM(zb_zdo_pim_start_turbo_poll_packets, 1, ZB_MILLISECONDS_TO_BEACON_INTERVAL(delay));
#endif
    ZB_SCHEDULE_ALARM(zb_apsde_data_request, buffer, ZB_MILLISECONDS_TO_BEACON_INTERVAL(delay));
  }
  else
  {
    /* cb != NULL here. */
    ZB_ZCL_SCHEDULE_STATUS_ABORT(buffer, &(apsde_req->dst_addr), dst_addr_mode, dst_ep, ep, cb);
  }
}

#ifdef APS_FRAGMENTATION

zb_bool_t zb_zcl_can_cluster_be_fragmented(zb_uint16_t profile_id, zb_uint16_t cluster_id)
{
  zb_bool_t ret;
  ZVUNUSED (profile_id);
  /* clusters with variable size fields */
  /* if (profile_id == ZB_AF_SE_PROFILE_ID)
  {
    ret = ZB_TRUE;
}
  else */
  {
    ret = (cluster_id == ZB_ZCL_CLUSTER_ID_TUNNELING
        || cluster_id == ZB_ZCL_CLUSTER_ID_PRICE
        || cluster_id == ZB_ZCL_CLUSTER_ID_MESSAGING
        || cluster_id == ZB_ZCL_CLUSTER_ID_METERING
        || cluster_id == ZB_ZCL_CLUSTER_ID_KEY_ESTABLISHMENT
        || cluster_id == ZB_ZCL_CLUSTER_ID_PREPAYMENT
        || cluster_id == ZB_ZCL_CLUSTER_ID_CALENDAR
        || cluster_id == ZB_ZCL_CLUSTER_ID_DEVICE_MANAGEMENT
        || cluster_id == ZB_ZCL_CLUSTER_ID_EVENTS
        || cluster_id == ZB_ZCL_CLUSTER_ID_DRLC
        || cluster_id == ZB_ZCL_CLUSTER_ID_ENERGY_MANAGEMENT
        || cluster_id == ZB_ZCL_CLUSTER_ID_MDU_PAIRING
        || cluster_id == ZB_ZCL_CLUSTER_ID_SUB_GHZ
      ) ? ZB_TRUE : ZB_FALSE;
  }
  return ret;
}
#endif /* APS_FRAGMENTATION */

static zb_ret_t zb_zcl_finish_and_send_packet_common(zb_bufid_t buffer,
                                                     zb_uint8_t *ptr,
                                                     zb_callback_t cb,
                                                     zb_bool_t disable_aps_ack,
                                                     zb_uint16_t delay)
{
#ifdef APS_FRAGMENTATION
  zb_bool_t frag;
#endif

  zb_ret_t res = RET_OK;
  zb_apsde_data_req_t *apsde_req = ZB_BUF_GET_PARAM(buffer, zb_apsde_data_req_t);
  zb_zcl_frame_hdr_short_t *zcl_cmd = (zb_zcl_frame_hdr_short_t *)zb_buf_begin(buffer);

  zb_uint8_t dst_addr_mode = apsde_req->addr_mode;
  zb_addr_u *dst_addr = &apsde_req->dst_addr;
  zb_uint8_t dst_ep = apsde_req->dst_endpoint;
  zb_uint8_t ep = apsde_req->src_endpoint;
  zb_uint16_t cluster_id = apsde_req->clusterid;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_finish_and_send_packet_common param %hd delay %d", (FMT__H_D, buffer, delay));

#ifdef ZB_ZCL_SUPPORT_CLUSTER_BASIC
  if (!zb_zcl_check_is_device_enabled(apsde_req->src_endpoint,
                                      zcl_cmd->command_id ,
                                      apsde_req->clusterid,
                                      (zcl_cmd->frame_ctrl.frame_type == ZB_ZCL_FRAME_TYPE_COMMON) ? ZB_TRUE : ZB_FALSE))
  {
    if (cb != NULL)
    {
      ZB_ZCL_SCHEDULE_STATUS_ABORT(buffer, dst_addr, dst_addr_mode, dst_ep, ep, cb);
      /* If cb isn't NULL caller should handle result in zb_zcl_command_send_status_t param */
      return RET_OK;
    }
    else
    {
      zb_buf_free(buffer);
      return RET_CANCELLED;
    }
  }
#endif

#ifdef APS_FRAGMENTATION
  frag = zb_zcl_can_cluster_be_fragmented(apsde_req->profileid, cluster_id);
#endif

  if (ptr != NULL)
  {
    if (ZB_ZCL_GET_BYTES_WRITTEN(buffer, ptr) > zb_buf_len(buffer))
    {
      /* TODO: we are currently using zb_buf_alloc_left to set buffer length. Change it to better
         suited functions. */
      TRACE_MSG(TRACE_ZCL3, "extending packet to %d bytes",
                (FMT__H, ZB_ZCL_GET_BYTES_WRITTEN(buffer, ptr) - zb_buf_len(buffer)));
      (void)zb_buf_alloc_left(buffer, ZB_ZCL_GET_BYTES_WRITTEN(buffer, ptr) - zb_buf_len(buffer));
    }
  }

  if (  !(((zb_aps_addr_mode_t)(dst_addr_mode) == ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT)
          || ((apsde_req->addr_mode == ZB_APS_ADDR_MODE_16_ENDP_PRESENT) &&
              (ZB_NWK_IS_ADDRESS_BROADCAST(dst_addr->addr_short))))
   /* "Require APS ACKs on Unicasts" command enforces that all unicast commands have APS ACKs enabled,
    * except for clusters in exception list. This also applies to global cluster commands (i.e.
    * read/write attribute commands). */
      && ZB_ZDO_CHECK_IF_APS_ACK_NEEDED(cluster_id)
      && !disable_aps_ack
      )
  {
    apsde_req->tx_options |= ZB_APSDE_TX_OPT_ACK_TX;
  }

#ifdef APS_FRAGMENTATION
  if (frag)
  {
    apsde_req->tx_options |= ZB_APSDE_TX_OPT_FRAG_PERMITTED;
  }
#endif
  /*cstat -MISRAC2012-Rule-2.1_b -MISRAC2012-Rule-14.3_b */
  if (res == RET_OK
      && !IS_DISTRIBUTED_SECURITY()
      && ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR()
      && ZB_ZDO_CHECK_IF_APS_LINK_KEY_AUTHORIZATION_NEEDED(cluster_id))
  {
    /*cstat +MISRAC2012-Rule-2.1_b +MISRAC2012-Rule-14.3_b */
    /** @mdr{00023,2} */
    /** @mdr{00015,10} */
    zb_ieee_addr_t dst_ieee;
    ZB_IEEE_ADDR_ZERO(dst_ieee);
    switch (dst_addr_mode)
    {
      case ZB_APS_ADDR_MODE_16_ENDP_PRESENT:
        if (zb_address_ieee_by_short(dst_addr->addr_short, dst_ieee) != RET_OK)
        {
          res = RET_UNAUTHORIZED;
        };
        break;
      case ZB_APS_ADDR_MODE_64_ENDP_PRESENT:
        ZB_IEEE_ADDR_COPY(dst_ieee, dst_addr);
        break;
      case ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT:
        /* TODO: See notes below:
         * Bind mode, neither dst addr nor ep is known at this moment.
         * Real destination address will be known only in apsde_data_req.  */
        break;
      case ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT:
        /*FALLTHRU*/
      default:
        /* Note that enabling "APS Link Key Authorization" requirement also means that
         * group/broadcast command for these clusters SHALL no longer be accepted since there is
         * no way to establish an APS Key for a Zigbee Group. */
        res = RET_UNAUTHORIZED;
        break;
    }

#ifndef ZB_COORDINATOR_ONLY
    /*cstat -MISRAC2012-Rule-2.1_b */
    if (ZB_ZDO_CHECK_IF_WWAH_SERVER_BEHAVIOR()
        && dst_addr_mode != ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT)
    {
      /*cstat +MISRAC2012-Rule-2.1_b */
      /** @mdr{00015,1} */
      if (res == RET_OK
          /*cstat !MISRAC2012-Rule-13.5 */
          /* After some investigation, the following violation of Rule 13.5 seems to be
           * a false positive. There are no side effect to 'zb_aib_trust_center_address_cmp()'. This
           * violation seems to be caused by the fact that 'zb_aib_trust_center_address_cmp()' is an
           * external function, which cannot be analyzed by C-STAT. */
          && !zb_aib_trust_center_address_cmp(dst_ieee))
      {
        res = RET_UNAUTHORIZED;
      }
    }
#endif
    if (res == RET_OK
        /*cstat !MISRAC2012-Rule-13.5 */
        /* After some investigation, the following violation of Rule 13.5 seems to be
         * a false positive. There are no side effect to 'zb_secur_get_link_key_by_address()'. This
         * violation seems to be caused by the fact that 'zb_secur_get_link_key_by_address()' is an
         * external function, which cannot be analyzed by C-STAT. */
        && zb_secur_get_link_key_by_address(dst_ieee, ZB_SECUR_VERIFIED_KEY) != NULL)
    {
      apsde_req->tx_options |= ZB_APSDE_TX_OPT_SECURITY_ENABLED;
    }
    else
    {
      res = RET_UNAUTHORIZED;
      TRACE_MSG(TRACE_ERROR,
                "ZCL (out): DROP frame! Cluster 0x%x forced to use TC with TCLK encryption!",
                (FMT__D, cluster_id));
    }
  }

#if defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
  if (ZCL_CTX().subghz_ctx.cli.suspend_zcl_messages)
  {
    TRACE_MSG(TRACE_ZCL2, "ZCL (out): DROP frame! ZCL messages suspended by SubGHz Cluster", (FMT__0));
    res = RET_CANCELLED;
  }
#endif

  if (res == RET_OK)
  {
    if (cluster_needs_aps_encryption(ep, cluster_id) == ZB_TRUE)
    {
      zb_ieee_addr_t dst_ieee;
      zb_aps_device_key_pair_set_t *key = NULL;
      zb_secur_key_attributes_t key_attrib =
#if defined ZB_CERTIFICATION_HACKS && defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
        ZB_CERT_HACKS().zcl_subghz_cluster_test && cluster_id == ZB_ZCL_CLUSTER_ID_SUB_GHZ ?
          ZB_SECUR_ANY_KEY_ATTR : ZB_SECUR_VERIFIED_KEY;
#else
          ZB_SECUR_VERIFIED_KEY;
#endif

      TRACE_MSG(TRACE_ZCL2, "ZCL (out): cluster: 0x%x, APS security required: yes",
                (FMT__D, cluster_id));
      ZB_IEEE_ADDR_ZERO(dst_ieee);

      switch (dst_addr_mode)
      {
        case ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT:
          /* Bind mode, neither dst addr nor ep is known at this moment.
           * Real destination address will be known only in apsde_data_req.
           *
           * TODO: pass the desired attribute ZB_SECUR_KEY_SRC_CBKE of the link key
           * to the APS level!
           */
          TRACE_MSG(TRACE_ZCL2, "Bind mode!", (FMT__0));
          break;

        case ZB_APS_ADDR_MODE_16_ENDP_PRESENT:
          res = zb_address_ieee_by_short(dst_addr->addr_short, dst_ieee);
          if (res == RET_OK)
          {
            key = zb_secur_get_link_key_by_address(dst_ieee, key_attrib);
            TRACE_MSG(TRACE_ZCL2, "Short addr mode!", (FMT__0));
          }
          break;

        case ZB_APS_ADDR_MODE_64_ENDP_PRESENT:
          ZB_IEEE_ADDR_COPY(dst_ieee, dst_addr);
          key = zb_secur_get_link_key_by_address(dst_ieee, key_attrib);
          TRACE_MSG(TRACE_ZCL2, "Long addr mode!", (FMT__0));
          break;

        case ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT:
        default:
          /* What to do with group address mode? Is it possible in SE? */
          ZB_ASSERT(0);
          break;
      }

      /* NK:FIXME: No check below (in apsde_data_req) that key which will be used to encrypt is
       * VERIFIED. May modify zb_secur_get_link_key_pair_set() - it already have
       * "valid_only" parameter.
       */

      /* SE+BDB: Just check that we have a VERIFIED key - it may be not CBKE.
         TODO: For legacy devices it may be no VERIFIED key - what to do?
       */
      if ((key == NULL
#ifdef ZB_ENABLE_SE
           || (ZB_SE_MODE()
               && key->key_source != ZB_SECUR_KEY_SRC_CBKE
#if defined ZB_CERTIFICATION_HACKS && defined ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
                && !ZB_CERT_HACKS().zcl_subghz_cluster_test
#endif
              )
#endif /* ZB_ENABLE_SE */
           )
          && (dst_addr_mode != ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT))
      {
        if ((zcl_cmd->command_id == ZB_ZCL_CMD_DEFAULT_RESP)
            && (zcl_cmd->frame_ctrl.manufacturer == 0U)
            && (zcl_cmd->frame_ctrl.frame_type == ZB_ZCL_FRAME_TYPE_COMMON)
            /* TODO: describe where is that status in the packet. Ref to the spec etc. */
            && (*((zb_uint8_t *)zcl_cmd + 4U * sizeof(zb_uint8_t)) == ZB_ZCL_STATUS_FAIL))
        {
          TRACE_MSG(TRACE_ERROR,
                    "ZCL (out): Specific case - failure default response will be sent without encryption if input command [%hd] cluster [%d] was incorrectly secured!",
                    (FMT__H_D, *((zb_uint8_t *)zcl_cmd + 3 * sizeof(zb_uint8_t)), cluster_id));
        }
        else
        {
          TRACE_MSG(TRACE_ERROR,
                    "ZCL (out): DROP frame! Cluster 0x%x needs APS encryption but there is no CBKE "
                    "key available!",
                    (FMT__D, cluster_id));

          res = RET_UNAUTHORIZED;
        }
      }
      else
      {
        apsde_req->tx_options |= ZB_APSDE_TX_OPT_SECURITY_ENABLED;
      }
    }
    else
    {
      TRACE_MSG(TRACE_ZCL2, "ZCL (out): cluster: 0x%x, APS security required: no",
                (FMT__D, cluster_id));
    }

#ifdef ZB_ENABLE_SE
    if (res == RET_OK
        /*cstat !MISRAC2012-Rule-14.3_b */
        /** @mdr{00010,6} */
        && ZB_SE_MODE()
        /*cstat !MISRAC2012-Rule-13.5 */
        /* After some investigation, the following violation of Rule 13.5 seems to be
         * a false positive. There are no side effect to 'se_is_high_freq_msg()'. This
         * violation seems to be caused by the fact that 'se_is_high_freq_msg()' is an
         * external function, which cannot be analyzed by C-STAT. */
        && (ZCL_SELECTOR().is_high_freq_msg != NULL
            && ZCL_SELECTOR().is_high_freq_msg(buffer)))
    {
      zb_uint8_t *hdr = zb_buf_begin(buffer);
      ZB_ZCL_SET_DISABLE_DEFAULT_RESPONSE(*hdr, 1U);
      apsde_req->tx_options &= ~ZB_APSDE_TX_OPT_ACK_TX;
      TRACE_MSG(TRACE_ZCL3, "High frequency message. Remove APS ACK req and disable default resp",
                (FMT__0));
    }
#endif
      /*
       * APS Acknowledgments shall be suppressed on High Frequency messages
       * (messages that are repeated at a rate higher than once every 30
       * seconds) when utilizing sub-GHz frequencies; the Acknowledgement
       * Request sub-field in the Frame Control field of the APDU Frame Header
       * shall be set to 0.
       *
       * The Disable Default Response sub-field in the ZCL Frame Control field
       * shall be set to 1 (disables positive Default Responses) on High
       * Frequency messages when utilizing sub-GHz 2816 frequencies.
       */
  }

  if (res == RET_OK)
  {
    res = zb_zcl_register_cb(buffer, (cb));
  }

  if (res == RET_OK)
  {
    zb_uint8_t *hdr = zb_buf_begin(buffer);

    ZB_ZCL_DEBUG_DUMP_COMMAND(buffer);
    if (!ZB_U2B(ZB_ZCL_GET_DISABLE_DEFAULT_RESPONSE(*hdr)))
    {
      /* TODO: Poll to number of fragments here (ZG->aps.out_frag.block_num). */
      TRACE_MSG(TRACE_ZDO3, "Call zb_zdo_pim_start_turbo_poll_packets 1", (FMT__0));
      zb_zdo_pim_start_turbo_poll_packets(1);
    }
    ZB_TH_PUSH_PACKET(ZB_TH_ZCL_DATA, ZB_TH_PRIMITIVE_REQUEST, buffer);

    if (delay == 0U)
    {
      ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, buffer);
    }
    else
    {
      ZB_SCHEDULE_ALARM(zb_apsde_data_request, buffer, delay);
    }
  }
  else if (cb != NULL)
  {
    zb_zcl_schedule_status_abort(buffer, dst_addr, dst_addr_mode, dst_ep, ep, cb);
    /* If cb isn't NULL caller should handle result in zb_zcl_command_send_status_t param */
    res = RET_OK;
  }
  else
  {
    /* MISRA rule 15.7 requires empty 'else' branch. */
  }

  return res;
}

zb_ret_t zb_zcl_finish_and_send_packet(zb_bufid_t  buffer, zb_uint8_t *ptr,
                                   const zb_addr_u *dst_addr, zb_uint8_t dst_addr_mode,
                                   zb_uint8_t dst_ep, zb_uint8_t ep,
                                   zb_uint16_t prof_id, zb_uint16_t cluster_id,
                                   zb_callback_t cb)
{
  zb_apsde_data_req_t *apsde_req = ZB_BUF_GET_PARAM(buffer, zb_apsde_data_req_t);
  ZB_BZERO(apsde_req, sizeof(zb_apsde_data_req_t));
  apsde_req->addr_mode = (dst_addr_mode);
  ZB_MEMCPY(&apsde_req->dst_addr, (dst_addr),
            (dst_addr_mode == ZB_APS_ADDR_MODE_64_ENDP_PRESENT ? sizeof(zb_addr_u) : sizeof(zb_uint16_t)));
  apsde_req->dst_endpoint = (dst_ep);
  apsde_req->src_endpoint = (ep);
  apsde_req->profileid = (prof_id);
  apsde_req->clusterid = (cluster_id);
  return zb_zcl_finish_and_send_packet_common(buffer, ptr, cb, ZB_FALSE, 0);
}

zb_ret_t zb_zcl_finish_and_send_packet_new(zb_bufid_t  buffer, zb_uint8_t *ptr,
                                   const zb_addr_u *dst_addr, zb_uint8_t dst_addr_mode,
                                   zb_uint8_t dst_ep, zb_uint8_t ep,
                                   zb_uint16_t prof_id, zb_uint16_t cluster_id,
                                   zb_callback_t cb, zb_bool_t aps_secured,
                                   zb_bool_t disable_aps_ack, zb_uint16_t delay)
{
  zb_apsde_data_req_t *apsde_req = ZB_BUF_GET_PARAM(buffer, zb_apsde_data_req_t);
  ZB_BZERO(apsde_req, sizeof(zb_apsde_data_req_t));
  apsde_req->addr_mode = (dst_addr_mode);
  ZB_MEMCPY(&apsde_req->dst_addr, (dst_addr),
            (dst_addr_mode == ZB_APS_ADDR_MODE_64_ENDP_PRESENT ? sizeof(zb_addr_u) : sizeof(zb_uint16_t)));
  apsde_req->dst_endpoint = (dst_ep);
  apsde_req->src_endpoint = (ep);
  apsde_req->profileid = (prof_id);
  apsde_req->clusterid = (cluster_id);
  apsde_req->tx_options = aps_secured ? (zb_uint8_t)ZB_APSDE_TX_OPT_SECURITY_ENABLED : 0U;
  return zb_zcl_finish_and_send_packet_common(buffer, ptr, cb, disable_aps_ack, delay);
}


void *zb_zcl_start_command_header(zb_bufid_t zbbuf, zb_uint8_t frame_ctl, zb_uint16_t manuf_code, zb_uint8_t cmd_id, zb_uint8_t* tsn)
{
  zb_uint8_t l_tsn;
  zb_uint8_t* data_ptr = ZB_ZCL_START_PACKET(zbbuf);

  *data_ptr = frame_ctl;
  data_ptr++;
  l_tsn = ZB_ZCL_GET_SEQ_NUM();
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(data_ptr, l_tsn, ZB_ZCL_IS_MANUF_SPECIFIC(frame_ctl),
                                      manuf_code, cmd_id);
  if (tsn != NULL)
  {
    *tsn = l_tsn;
  }
  return data_ptr;
}

zb_zcl_globals_t *zb_zcl_get_ctx()
{
  return &ZG->zcl;
}

zb_zcl8_globals_t *zb_zcl8_get_ctx()
{
  static zb_zcl8_globals_t zb_zcl8_ctx;

  return &zb_zcl8_ctx;
}

zb_ret_t zb_zcl_set_peer_revision_callback(zb_zcl_peer_revision_cb_t cb)
{
  ZCL8_CTX().peer_revision_cb = cb;

  /* NULL, if a user wants to reset the callback */
  if (NULL == cb)
  {
    return RET_OK;
  }
  else
  {
    return zb_zcl_set_backward_comp_mode(ZB_ZCL_COMPATIBILITY_MODE);
  }
}

zb_ret_t zb_zcl_set_backward_comp_mode(zb_uint8_t mode)
{
  if (mode > ZB_ZCL_COMPATIBILITY_MODE)
  {
    return RET_OUT_OF_RANGE;
  }
  else
  {
    ZCL8_CTX().backward_comp_mode = mode;
    return RET_OK;
  }
}

zb_uint8_t zb_zcl_get_backward_comp_mode(void)
{
  return ZCL8_CTX().backward_comp_mode;
}

zb_uint16_t zb_zcl_get_cluster_rev_by_mode(zb_uint16_t api_revision, const zb_addr_u *dst_addr, 
                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                           zb_uint16_t cluster_id, zb_uint8_t cluster_role,
                                           zb_uint8_t src_ep)
{
  zb_uint8_t mode;
  zb_zcl_attr_t *attr_desc_cluster_rev;
  zb_uint16_t rev;
  zb_uint16_t rev_attr_value;
  zb_ieee_addr_t ieee_addr;
  zb_uint16_t short_addr;
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_get_cluster_rev_by_mode", (FMT__0));

  mode = zb_zcl_get_backward_comp_mode();

  TRACE_MSG(TRACE_ZCL3, "mode %d", (FMT__H, mode));

  attr_desc_cluster_rev = zb_zcl_get_attr_desc_a(src_ep, cluster_id,
                                       cluster_role,
                                       ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID);

  if (attr_desc_cluster_rev != NULL)
  {
    rev_attr_value  = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc_cluster_rev);
  }
  else
  {
    /* it means the attribute wasn't declared for the cluster */
    rev_attr_value = ZB_ZCL_CLUSTER_REV_MIN;
  }

  TRACE_MSG(TRACE_ZCL3, "rev_attr_value %d", (FMT__D, rev_attr_value));

  switch(mode)
  {
    case ZB_ZCL_LEGACY_MODE:
      rev = (api_revision > rev_attr_value) ? rev_attr_value : api_revision;
      break;
    case ZB_ZCL_AUTO_MODE:
      /* rev_attr_value is compared to maximum available cluster revision while the revision value is being set,
         so we don't have to check for maximum value here */
      rev = rev_attr_value;
      break;
    case ZB_ZCL_COMPATIBILITY_MODE:
      if (NULL == ZCL8_CTX().peer_revision_cb)
      {
        rev = ZB_ZCL_CLUSTER_REV_MIN;
      }
      else
      {        
        if (dst_addr_mode == ZB_APS_ADDR_MODE_64_ENDP_PRESENT)
        {
          ZB_IEEE_ADDR_COPY(ieee_addr, dst_addr);
        }
        else
        {
          ZB_MEMCPY(&short_addr, (dst_addr), sizeof(short_addr));
          ret = zb_address_ieee_by_short(short_addr, ieee_addr);
        }
        if (RET_OK == ret)
        {
          rev = (*ZCL8_CTX().peer_revision_cb)(ieee_addr, cluster_id,
                                              ZB_ZCL_REVERT_CLUSTER_ROLE(cluster_role), dst_ep);
          if (ZB_ZCL_PEER_CLUSTER_REV_UNKNOWN == rev)
          {
            rev = ZB_ZCL_CLUSTER_REV_MIN;
          }
          else
          {
            rev = (rev > rev_attr_value) ? rev_attr_value : rev;
          }
        }
        else
        {
          rev = ZB_ZCL_CLUSTER_REV_MIN;
        }
      }
      break;
    default:
      rev = rev_attr_value;
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_get_cluster_rev_by_mode %d", (FMT__H, rev));

  return rev;
}

zb_uint8_t zb_zcl_get_backward_compatible_statuses_mode(void)
{
  return ZCL8_CTX().backward_compatible_statuses_mode;
}

zb_ret_t zb_zcl_set_backward_compatible_statuses_mode(zb_uint8_t statuses_mode)
{
  if (statuses_mode > ZB_ZCL_STATUSES_ZCL8_MODE)
  {
    return RET_OUT_OF_RANGE;
  }
  else
  {
    ZCL8_CTX().backward_compatible_statuses_mode = statuses_mode;
    return RET_OK;
  }
}

zb_zcl_status_t zb_zcl_zcl8_statuses_conversion(zb_zcl_status_t status)
{
  if (zb_zcl_get_backward_comp_mode() == ZB_ZCL_LEGACY_MODE)
  {
    return status;
  }
  else
  {
    zb_zcl_status_t ret_status;
    switch(status)
    {
      case ZB_ZCL_STATUS_DUPE_EXISTS: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_LIMIT_REACHED:
        ret_status = ZB_ZCL_STATUS_SUCCESS;
        break;
      case ZB_ZCL_STATUS_INCONSISTENT_STARTUP_STATE: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_DEFINED_OUT_OF_BAND: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_ACTION_DENIED: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_HW_FAIL: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_SW_FAIL:
        ret_status = ZB_ZCL_STATUS_FAIL;
        break;
      case ZB_ZCL_STATUS_INCONSISTENT: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_CALIB_ERR:
        ret_status = ZB_ZCL_STATUS_RESERVED;
        break;
      case ZB_ZCL_STATUS_UNSUP_CLUST_CMD: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_UNSUP_GEN_CMD: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_UNSUP_MANUF_CLUST_CMD: /* FALLTHROUGH */
      case ZB_ZCL_STATUS_UNSUP_MANUF_GEN_CMD:
        ret_status = ZB_ZCL_STATUS_UNSUP_CMD;
        break;
      case ZB_ZCL_STATUS_WRITE_ONLY:
        ret_status = ZB_ZCL_STATUS_NOT_AUTHORIZED;
        break;
      default:
        ret_status = status;
        break;
    }
    return ret_status;
  }
}

zb_zcl_status_t zb_zcl_get_zcl_status_from_ret(zb_ret_t result)
{
  zb_zcl_status_t zcl_result;

  switch (result)
  {
    case RET_OK:
      zcl_result = ZB_ZCL_STATUS_SUCCESS;
      break;

    case RET_INVALID_PARAMETER_1:
      zcl_result = ZB_ZCL_STATUS_INVALID_FIELD;
      break;

    case RET_INVALID_PARAMETER:
      zcl_result = ZB_ZCL_STATUS_INVALID_VALUE;
      break;

    case RET_NOT_IMPLEMENTED:
      zcl_result = ZB_ZCL_STATUS_UNSUP_CMD;
      break;

    default:
      zcl_result = (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                  ZB_ZCL_STATUS_FAIL : ZB_ZCL_STATUS_HW_FAIL;
      break;
  }

  return zcl_result;
}

#endif /* defined (ZB_ENABLE_ZCL) */

#endif /* defined (ZB_ENABLE_ZCL) || defined (ZB_ENABLE_ZGPD_ATTR_REPORTING) */
