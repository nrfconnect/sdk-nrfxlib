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
/* PURPOSE: ZCL general commands implementation
*/


#define ZB_TRACE_FILE_ID 2066

#include "zb_common.h"

#if defined (ZB_ENABLE_ZCL)

#include "zb_zcl.h"
#include "zb_aps.h"
#include "zcl/zb_zcl_common.h"
#include "zcl/zcl_ota_upgrade_commands.h"
#if defined ZB_ENABLE_HA
#include "zb_ha.h"
#endif
#include "zdo_wwah_stubs.h"

#ifdef SWITCH_OFF_SOME_ZCL_TRACE
/* temporary switch trace off here */
#undef TRACE_ZCL1
#define TRACE_ZCL1 TRACE_MEM1
#undef TRACE_ZCL2
#define TRACE_ZCL2 TRACE_MEM2
#undef TRACE_ZCL3
#define TRACE_ZCL3 TRACE_MEM3
#endif

/*
  Maximum attr for Discover Attributes Response
  this limit by packet length (146) and length of MAC, NWK, APS, ZCL headers
  with security headers
*/
#define ZB_ZCL_DISC_ATTR_MAX    14


static zb_bool_t zb_zcl_handle_default_response_commands(zb_uint8_t param);

/*
  Puts attribute value to ZCL packet.
  Returns modified data pointer
 */
/* TODO: check - this function doubles functionality of zb_zcl_put_value_to_packet() */
static zb_uint8_t* zb_zcl_put_attribute_value(zb_uint8_t *data_ptr, zb_zcl_attr_t *attr,
                                       zb_uint8_t *value, zb_uint16_t value_size)
{
  ZB_ASSERT(data_ptr);
  ZB_ASSERT(attr);
  ZB_ASSERT(value);

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_put_attribute_value data_ptr %p, value %p, value_size %hd",
            (FMT__P_P_D, data_ptr, value, value_size));

  switch ( attr->type )
  {
    case ZB_ZCL_ATTR_TYPE_8BIT:
    case ZB_ZCL_ATTR_TYPE_U8:
    case ZB_ZCL_ATTR_TYPE_S8:
    case ZB_ZCL_ATTR_TYPE_BOOL:
    case ZB_ZCL_ATTR_TYPE_8BITMAP:
    case ZB_ZCL_ATTR_TYPE_8BIT_ENUM:
      ZB_ZCL_PACKET_PUT_DATA8(data_ptr, *value);
      break;

    case ZB_ZCL_ATTR_TYPE_16BIT:
    case ZB_ZCL_ATTR_TYPE_U16:
    case ZB_ZCL_ATTR_TYPE_S16:
    case ZB_ZCL_ATTR_TYPE_16BITMAP:
    case ZB_ZCL_ATTR_TYPE_16BIT_ENUM:
    case ZB_ZCL_ATTR_TYPE_SEMI:
    case ZB_ZCL_ATTR_TYPE_CLUSTER_ID:
    case ZB_ZCL_ATTR_TYPE_ATTRIBUTE_ID:
      ZB_ZCL_PACKET_PUT_DATA16(data_ptr, value);
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
      ZB_ZCL_PACKET_PUT_DATA32(data_ptr, value);
      break;

    case ZB_ZCL_ATTR_TYPE_U48:
    case ZB_ZCL_ATTR_TYPE_S48:
    case ZB_ZCL_ATTR_TYPE_48BIT:
    case ZB_ZCL_ATTR_TYPE_48BITMAP:
//TODO: fix it: put bytes taking into account bytes order
      ZB_ZCL_PACKET_PUT_DATA48(data_ptr, value);
      break;

    case ZB_ZCL_ATTR_TYPE_U24:
    case ZB_ZCL_ATTR_TYPE_S24:
    case ZB_ZCL_ATTR_TYPE_24BIT:
    case ZB_ZCL_ATTR_TYPE_24BITMAP:
//TODO: fix it: put bytes taking into account bytes order
      ZB_ZCL_PACKET_PUT_DATA24(data_ptr, value);
      break;

    case ZB_ZCL_ATTR_TYPE_U40:
    case ZB_ZCL_ATTR_TYPE_S40:
    case ZB_ZCL_ATTR_TYPE_40BIT:
    case ZB_ZCL_ATTR_TYPE_40BITMAP:
//TODO: fix it: put bytes taking into account bytes order
      ZB_ZCL_PACKET_PUT_DATA_N(data_ptr, value, 5);
      break;

    case ZB_ZCL_ATTR_TYPE_U56:
    case ZB_ZCL_ATTR_TYPE_S56:
    case ZB_ZCL_ATTR_TYPE_56BIT:
    case ZB_ZCL_ATTR_TYPE_56BITMAP:
//TODO: fix it: put bytes taking into account bytes order
      ZB_ZCL_PACKET_PUT_DATA_N(data_ptr, value, 7);
      break;

    case ZB_ZCL_ATTR_TYPE_OCTET_STRING:
    case ZB_ZCL_ATTR_TYPE_CHAR_STRING:
    case ZB_ZCL_ATTR_TYPE_ARRAY:
    case ZB_ZCL_ATTR_TYPE_CUSTOM_32ARRAY:
    case ZB_ZCL_ATTR_TYPE_LONG_OCTET_STRING:
      ZB_ZCL_PACKET_PUT_DATA_N(data_ptr, value, value_size);
      break;

    case ZB_ZCL_ATTR_TYPE_64BIT:
    case ZB_ZCL_ATTR_TYPE_64BITMAP:
    case ZB_ZCL_ATTR_TYPE_U64:
    case ZB_ZCL_ATTR_TYPE_S64:
    case ZB_ZCL_ATTR_TYPE_DOUBLE:
    case ZB_ZCL_ATTR_TYPE_IEEE_ADDR:
      ZB_ZCL_PACKET_PUT_DATA64(data_ptr, value);
      break;

    case ZB_ZCL_ATTR_TYPE_128_BIT_KEY:
      ZB_ZCL_PACKET_PUT_DATA_N(data_ptr, value, 16);
      break;
    default:
      TRACE_MSG(TRACE_ZCL1, "Error, unsupported type!", (FMT__0));
      ZB_ASSERT(0);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_put_attribute_value ret %p", (FMT__P, data_ptr));
  return data_ptr;
}


/*! @internal @brief ZCL read attrubutes handler continue
    @param param - index of the buffer with read attributes request
*/
static void zb_zcl_read_attr_handler_continue(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_bool_t security = ZB_APS_FC_IS_SECURE(cmd_info->addr_data.common_data.fc);
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t attr_size;
  zb_uint8_t *resp_data;
  zb_uint32_t attr_num;
  zb_zcl_read_attr_req_t *read_attr_req;
  zb_uint16_t attr_id;
  zb_uint8_t i;
  zb_uint32_t bytes_avail;
  zb_uint16_t global_cluster_revision_value;
  zb_uint16_t manuf_code;
  zb_zcl_status_t status;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_read_attr_handler_continue param %hd", (FMT__H, param));
  TRACE_MSG(TRACE_ZCL1, "sizeof(zb_zcl_parsed_hdr_t) %ld", (FMT__L, sizeof(zb_zcl_parsed_hdr_t)));

  ep_desc = zb_af_get_endpoint_desc(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  /* EP existence was checked on command accept - it must be in the list */
  ZB_ASSERT(ep_desc);

  cluster_desc = get_cluster_desc(ep_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);
  /* Cluster existence was checked on command accept - it must be in the list */
  ZB_ASSERT(cluster_desc);

  /* ZCL spec, 2.4.2 Read Attributes Response Command */
  /* Read attribute response
     | ZCL header 3 b | Read attr status rec 1 XX b | Read attr status rec 2 XX b | ...

     Read attr status rec format
     | attr id 2 b | status 1 b | Attr data type 0/1 b | attr value 0/XX b |
  */

  /* ZCL frame format */
  /* frame control 1b | manufacturer code 0/2b | TSN 1b | cmd id 1b | payload XXb */

  /* frame control */
  /* frame type | manufacturer specific | direction | disable default resp */

  /* Construct packet header */
  /* Use runtime_buf buffer for composing and sending response, request buffer
   * will be saved as runtime_buf */
  resp_data = ZB_ZCL_START_PACKET(ZCL_CTX().runtime_buf);

  ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_A(
    resp_data,
    (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV ?
     ZB_ZCL_FRAME_DIRECTION_TO_CLI :
     ZB_ZCL_FRAME_DIRECTION_TO_SRV),
    cmd_info->is_manuf_specific);

  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(
    resp_data,
    cmd_info->seq_number,
    cmd_info->is_manuf_specific,
    cmd_info->manuf_specific,
    ZB_ZCL_CMD_READ_ATTRIB_RESP);

  read_attr_req = (zb_zcl_read_attr_req_t*)zb_buf_begin(param);
  attr_num = zb_buf_len(param) / sizeof(zb_zcl_read_attr_req_t);
  TRACE_MSG(TRACE_ZCL2, "read_attr_req %p, attr_num %d", (FMT__P_D, read_attr_req, attr_num));

  for(i = 0; i < attr_num; i++)
  {
    status = ZB_ZCL_STATUS_SUCCESS;
    manuf_code = (cmd_info->is_manuf_specific) ? cmd_info->manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC;
    ZB_HTOLE16(&attr_id, &read_attr_req->attr_id[i]);
    TRACE_MSG(TRACE_ZCL3, "attr_id %d", (FMT__D, attr_id));
    attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, attr_id, manuf_code);
    TRACE_MSG(TRACE_ZCL3, "attr_desc %p", (FMT__P, attr_desc));

    bytes_avail = ZB_ZCL_GET_BYTES_AVAILABLE(ZCL_CTX().runtime_buf, resp_data,
                                             cmd_info->profile_id, cmd_info->cluster_id);

    TRACE_MSG(TRACE_ZCL3, "attr_desc %p, bytes_avail %d", (FMT__P_D, attr_desc, bytes_avail));

    if (attr_desc == NULL)
    {
      if (attr_id == ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID)
      {
        TRACE_MSG(
          TRACE_ZCL1,
          "Global revision requested. Return ZB_ZCL_GLOBAL_CLUSTER_REVISION_DEFAULT",
          (FMT__0));
        global_cluster_revision_value = ZB_ZCL_GLOBAL_CLUSTER_REVISION_DEFAULT;
        ZB_ZCL_PACKET_PUT_DATA16(resp_data, &attr_id);
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_STATUS_SUCCESS);
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_ATTR_TYPE_U16);
        ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, global_cluster_revision_value);
      }
      else
      {
        status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
        TRACE_MSG(TRACE_ZCL1, "attr is not found id %hd", (FMT__H, read_attr_req[i]));
      }
    }
    else if (attr_desc->data_p == NULL || ZB_BIT_IS_SET(attr_desc->access, ZB_ZCL_ATTR_ACCESS_INTERNAL))
    {
      TRACE_MSG(TRACE_ZCL1, "error, internal attr isn't readable", (FMT__0));
      status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
    }
    else if (!ZB_BIT_IS_SET(attr_desc->access, ZB_ZCL_ATTR_ACCESS_READ_ONLY))
    {
      TRACE_MSG(TRACE_ZCL1, "error, attr access mask 0x%hx - isn't readable", (FMT__H, attr_desc->access));
      status = ZB_BIT_IS_SET(attr_desc->access, ZB_ZCL_ATTR_ACCESS_WRITE_ONLY)
        /* ZCL8: CCB 2477: use NOT_AUTHORIZED instead of Write-only status*/
        ?((zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE) ?
                ZB_ZCL_STATUS_NOT_AUTHORIZED : ZB_ZCL_STATUS_WRITE_ONLY)
        : ZB_ZCL_STATUS_FAIL;
    }
    else
    {
      TRACE_MSG(
        TRACE_ZCL3,
        "attribute: id 0x%x, type 0x%hx",
        (FMT__D_H, attr_desc->id, attr_desc->type));
      attr_size = zb_zcl_get_attribute_size(attr_desc->type, attr_desc->data_p);
      TRACE_MSG(TRACE_ZCL3, "attr_size %hd", (FMT__H, attr_size));

      /* Decrement read attr response by sizeof(zb_uint8_t), because attr value size is
       * calculated separately by attr_size  */
      if (bytes_avail >= (sizeof(zb_zcl_read_attr_res_t) - sizeof(zb_uint8_t) + attr_size))
      {
        ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, attr_desc->id);
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_STATUS_SUCCESS);
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, attr_desc->type);
        resp_data = zb_zcl_put_attribute_value(resp_data, attr_desc, attr_desc->data_p, attr_size);
      }
      else
      {
        /* ZCL7 spec, 2.5.1.3:
           If the status record  does  not  fit,  the  device  SHALL  set  the  status  field  of
           the  corresponding  read  attribute  status  record  to INSUFFICIENT_SPACE and not
           include the data type and value fields.
        */
        status = ZB_ZCL_STATUS_INSUFF_SPACE;
        TRACE_MSG(TRACE_ZCL1, "ERROR, buffer is full (bytes_avail %d)", (FMT__D, bytes_avail));
      }
    }

    if (status != ZB_ZCL_STATUS_SUCCESS)
    {
      if (bytes_avail >= (sizeof(zb_uint16_t) + sizeof(zb_uint8_t)))
      {
        ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, attr_id);
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);
      }
      else
      {
        TRACE_MSG(TRACE_ZCL1, "ERROR, buffer is full; terminate", (FMT__0));
        break;
      }
    }
  }

  ZB_ZCL_FINISH_N_SEND_PACKET_NEW(
    ZCL_CTX().runtime_buf,
    resp_data,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
    ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
    cmd_info->profile_id,
    cmd_info->cluster_id,
    NULL,
    security, ZB_FALSE, 0);

  /* Use runtime buffer to send request, use received buffer with
   * request data_buf as runtime buffer */
  ZCL_CTX().runtime_buf = param;

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_read_attr_handler_continue", (FMT__0));
}

#if defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS)
static void zb_zcl_sync_stats(zb_uint8_t param)
{
  zb_ret_t ret;
#define GET_DIAGNOSTICS_STATS_TIMEOUT_MS 250

  ret = zb_zcl_diagnostics_sync_counters(param, zb_zcl_read_attr_handler_continue);
  if (ret == RET_BUSY)
  {
    TRACE_MSG(TRACE_ZCL1, "diagnostics context is busy! try to get stats after %d ms",
              (FMT__D, GET_DIAGNOSTICS_STATS_TIMEOUT_MS));

    ZB_SCHEDULE_ALARM(zb_zcl_sync_stats,
                      param,
                      GET_DIAGNOSTICS_STATS_TIMEOUT_MS / 1000 * ZB_TIME_ONE_SECOND);
  }
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS */

/*! @internal @brief ZCL read attrubutes handler
    @param param - index of the buffer with read attributes request
*/
void zb_zcl_read_attr_handler(zb_uint8_t param)
{
  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_read_attr_handler param %hd", (FMT__H, param));

#ifdef ZB_TH_ENABLED
  if (TH_CTX().options.block_read_attr_request)
  {
    TRACE_MSG(TRACE_ZCL1, ">>Read attribute handler is blocked by implementation specific meaning", (FMT__0));
    zb_buf_free(param);
    return;
  }
#endif

  /* 12/02/2020 EE CR:MAJOR That is not very good solution because, if our build
   * includes all clusters but application does not declare diagnostic cluster,
   * its code still will be linked.
   * Also, we will need similar approach (blocked load of ZDO level data into ZCL) for WWAH.
   * So need to design a common mechanism for the cluster to intercept read_attr.
   * Add one more type of the cluster handler?
   */
  {
#if defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS)

    zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

    if (cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_DIAGNOSTICS)
    {
      TRACE_MSG(TRACE_ZCL1, "it's the diagnostics cluster! try to sync stats before rsp sending...",
                (FMT__0));
      zb_zcl_sync_stats(param);
    }
    else
#endif /* defined(ZB_ZCL_SUPPORT_CLUSTER_DIAGNOSTICS) */
    {
      zb_zcl_read_attr_handler_continue(param);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_read_attr_handler", (FMT__0));
}

/*! @internal @brief ZCL read attrubutes response handler
    @param param - index of the buffer with read attributes response
*/
static void zb_zcl_read_attr_resp_handler(zb_uint8_t param)
{
  /* 12/02/2020 EE CR:MAJOR That hard-coded intercept breaks our idea os linking
   * only required clusters.  If library is built with control4 support, but
   * application does not use it, control4 lib still will be linked.
   * Need to define better solution (not in the scope of diagnostic cluster, but in the future).
   * Add one more type of the cluster handler?
   */
#ifdef ZB_CONTROL4_NETWORK_SUPPORT
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  if (cmd_info->profile_id == ZB_AF_CONTROL4_PROFILE_ID &&
      cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_CONTROL4_NETWORKING)
  {
    zb_zcl_control4_network_cluster_read_attr_resp_handler(param);
  }
  else
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */
  {
    /* TODO: implement default response handler with notifying
     * user about received attribute value via callback */
    zb_buf_free(param);
  }
}

/*
  Search for attribute descriptor by its ID
*/
zb_zcl_attr_t* zb_zcl_get_attr_desc(zb_zcl_cluster_desc_t *cluster_desc, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL3, ">> zb_zcl_get_attr_desc cluster_desc %p, attr_id 0x%x", (FMT__P_D, cluster_desc, attr_id));
  attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  TRACE_MSG(TRACE_ZCL3, "<< zb_zcl_get_attr_desc, attr_desc %p", (FMT__P, attr_desc));

  return attr_desc;
}

zb_zcl_attr_t* zb_zcl_get_attr_desc_manuf(zb_zcl_cluster_desc_t *cluster_desc, zb_uint16_t attr_id, zb_uint16_t manuf_code)
{
  zb_zcl_attr_t *attr_desc = cluster_desc->attr_desc_list;
  zb_bool_t is_ms_code_check_required = (manuf_code != ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
  zb_ushort_t i = 0;

  TRACE_MSG(TRACE_ZCL3, ">> zb_zcl_get_attr_desc_manuf cluster_desc %p, attr_id 0x%x, manuf_code 0x%x, cluster id: 0x%x",
            (FMT__P_D_D_D, cluster_desc, attr_id, manuf_code, cluster_desc->cluster_id));

  while (attr_desc != NULL)
  {
    zb_bool_t is_ms_bit_set = ZB_BIT_IS_SET(attr_desc->access, ZB_ZCL_ATTR_MANUF_SPEC);
    /* Manufacturer specific bit set correctly if it is required and set or not required and not set, in other words, when this two variables are equal */
    zb_bool_t is_ms_bit_set_correctly = (is_ms_code_check_required == is_ms_bit_set);
    zb_bool_t is_attr_ids_match = (attr_desc->id == attr_id);
    zb_bool_t is_manuf_codes_match = (manuf_code == attr_desc->manuf_code);

    TRACE_MSG(TRACE_ZCL3, "Checking attribute with id 0x%x: is_attr_ids_match %hd, is_manuf_codes_match %hd, is_ms_bit_set %hd, is_ms_bit_set_correctly %hd",
              (FMT__D_H_H_H_H, attr_desc->id, is_attr_ids_match, is_manuf_codes_match, is_ms_bit_set, is_ms_bit_set_correctly));

    if (is_attr_ids_match && is_manuf_codes_match && is_ms_bit_set_correctly)
    {
      TRACE_MSG(TRACE_ZCL3, "attribute is found", (FMT__0));
      break;
    }

    i++;
    attr_desc++;
    if (i >= cluster_desc->attr_count)
    {
      attr_desc = NULL;
      break;
    }
  }

  TRACE_MSG(TRACE_ZCL3, "<< zb_zcl_get_attr_desc_manuf, attr_desc %p", (FMT__P, attr_desc));
  return attr_desc;
}

/* aggregates all calls to find attribute descriptor */
zb_zcl_attr_t* zb_zcl_get_attr_desc_manuf_a(zb_uint8_t ep,
                                            zb_uint16_t cluster_id,
                                            zb_uint8_t cluster_role,
                                            zb_uint16_t attr_id,
                                            zb_uint16_t manuf_code)
{
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL2, ">> zb_zcl_get_attr_desc_manuf_a ep %hd, cluster_id 0x%x, attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D_D, ep, cluster_id, attr_id, manuf_code));

  ep_desc = zb_af_get_endpoint_desc(ep);
  /* EP existence was checked on command accept - it must be in the list */
  ZB_ASSERT(ep_desc);

  cluster_desc = get_cluster_desc(ep_desc, cluster_id, cluster_role);
  /* Cluster existence was checked on command accept - it must be in the list */
  ZB_ASSERT(cluster_desc);

  attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, attr_id, manuf_code);
  TRACE_MSG(TRACE_ZCL2, "<< zb_zcl_get_attr_desc_manuf_a ret %p", (FMT__P, attr_desc));
  return attr_desc;
}

zb_zcl_attr_t* zb_zcl_get_attr_desc_a(zb_uint8_t ep, zb_uint16_t cluster_id, zb_uint8_t cluster_role, zb_uint16_t attr_id)
{
  return zb_zcl_get_attr_desc_manuf_a(ep, cluster_id, cluster_role, attr_id, ZB_ZCL_NON_MANUFACTURER_SPECIFIC);
}

static void zb_zcl_set_attr_val_cmd_post_process(zb_zcl_parsed_hdr_t *cmd_info,
                                                 zb_uint16_t attr_id,
                                                 zb_uint8_t *value)
{
  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_set_attr_val_cmd_post_process, cmd_info %p", (FMT__P, cmd_info));

  ZVUNUSED(attr_id);
  ZVUNUSED(value);

  zb_zcl_set_attr_val_post_process_cluster_specific(cmd_info, attr_id, value);

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_set_attr_val_cmd_post_process", (FMT__0));
}

static void zcl_write_attr_post_process(
  zb_zcl_parsed_hdr_t *cmd_info,
  zb_zcl_attr_t *attr_desc,
  zb_uint8_t *value)
{
  zb_bufid_t buf = zb_buf_get_any();
  zb_ret_t result;
  if (buf != 0U)
  {
    zb_zcl_set_attr_val_cmd_post_process(cmd_info,
                                         attr_desc->id,
                                         value);
    ZB_ZCL_INVOKE_USER_APP_SET_ATTR_WITH_RESULT(
      buf,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->cluster_id,
      attr_desc,
      value,
      result);
    ZVUNUSED(result);
    zb_buf_free(buf);
  }
}

/*! @brief ZCL write attributes command handler
    @param param - reference to buffer (see @ref zb_buf_t) with read attributes request
*/
void zb_zcl_write_attr_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_write_attr_req_t *write_attr_req;
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_attr_t *attr_desc;
  zb_zcl_status_t status;
  zb_uint8_t *resp_data = NULL;
  zb_uint8_t attr_count = 0;
  zb_uint8_t endpoint;
  zb_uint8_t *data_ptr;
  zb_uint32_t buf_len;
  zb_uint16_t manuf_code;
  zb_bool_t can_write_all_attrs = ZB_TRUE;

  TRACE_MSG(
      TRACE_ZCL1,
      ">>zb_zcl_write_attr_handler param %d, cmd_info %p",
      (FMT__H_P, param, cmd_info));

  /* ZCL8 spec, 2.5.3.1 Write attributes Command Frame Format */
  /* ZCL header | Write attr record 1 | Write attr record 2 | ...  */
  /* write attr record: | attr id 2b | attr type 1b | data XXb | */

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
  ep_desc = zb_af_get_endpoint_desc(endpoint);

  /* EP existence was checked on command accept - it must be in the list */
  ZB_ASSERT(ep_desc);

  cluster_desc = get_cluster_desc(ep_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);
  /* Cluster existence was checked on command accept - it must be in the list */
  ZB_ASSERT(cluster_desc);

  if (ZB_ZCL_CMD_WRITE_ATTRIB_NO_RESP != cmd_info->cmd_id)
  {
    /* Construct packet header */
    /* Use runtime_buf buffer for composing and sending response, request buffer
     * will be saved as runtime_buf */
    resp_data = ZB_ZCL_START_PACKET(ZCL_CTX().runtime_buf);

    /* NOTE: currently, Manufacturer specific is not supported */
    ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_A(
      resp_data,
      (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV ?
       ZB_ZCL_FRAME_DIRECTION_TO_CLI :
       ZB_ZCL_FRAME_DIRECTION_TO_SRV),
      cmd_info->is_manuf_specific);

    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(
      resp_data,
      cmd_info->seq_number,
      cmd_info->is_manuf_specific,
      cmd_info->manuf_specific,
      ZB_ZCL_CMD_WRITE_ATTRIB_RESP);
  }

  data_ptr = zb_buf_begin(param);
  buf_len = zb_buf_len(param);
  manuf_code = (cmd_info->is_manuf_specific) ? cmd_info->manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC;

  do
  {
    ZB_ZCL_GENERAL_GET_NEXT_WRITE_ATTR_REQ(data_ptr, buf_len, write_attr_req);

    TRACE_MSG(TRACE_ZCL2, "write_attr_req %p", (FMT__P, write_attr_req));

    if (write_attr_req != NULL)
    {
      attr_count++;
      TRACE_MSG(TRACE_ZCL3, "attr_id %d, attr_type %hd",
                (FMT__D_H, write_attr_req->attr_id, write_attr_req->attr_type));

      attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, write_attr_req->attr_id, manuf_code);
      TRACE_MSG(TRACE_ZCL3, "attr_desc %p", (FMT__P, attr_desc));

      if (attr_desc == NULL)
      {
        if (write_attr_req->attr_id == ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID)
        {
          TRACE_MSG(
            TRACE_ZCL1,
            "Global revision requested. Return READ_ONLY",
            (FMT__0));
          status = ZB_ZCL_STATUS_READ_ONLY;
        }
        else
        {
          TRACE_MSG(TRACE_ZCL1, "error, attr not found", (FMT__0));
          status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
        }
      }
      else if (attr_desc->data_p == NULL)
      {
        TRACE_MSG(TRACE_ZCL1, "error, attr not found", (FMT__0));
        status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
      }
      else if (attr_desc->type != write_attr_req->attr_type)
      {
        TRACE_MSG(TRACE_ZCL1, "error, incorrect attr type", (FMT__0));
        status = ZB_ZCL_STATUS_INVALID_TYPE;
      }
      else
      {
        status = (zb_zcl_status_t)
          zb_zcl_check_attribute_writable(attr_desc,
                                          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                          cmd_info->cluster_id,
                                          ((cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                            ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE),
                                          write_attr_req->attr_value,
                                          ZB_TRUE);
      }

      TRACE_MSG(TRACE_ZCL2, "attr status 0x%hx", (FMT__H, status));

      if (ZB_ZCL_STATUS_SUCCESS == status)
      {
        if (ZB_ZCL_CMD_WRITE_ATTRIB_UNDIV != cmd_info->cmd_id)
        {
          ZVUNUSED(zb_zcl_set_attr_val_manuf(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                     cmd_info->cluster_id,
                                     (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                     ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                     attr_desc->id,
                                     manuf_code,
                                     write_attr_req->attr_value,
                                     ZB_FALSE));
          zcl_write_attr_post_process(cmd_info, attr_desc, write_attr_req->attr_value);
        }
      }
      else
      {
        can_write_all_attrs = ZB_FALSE;

        if (ZB_ZCL_CMD_WRITE_ATTRIB_NO_RESP != cmd_info->cmd_id)
        {
          TRACE_MSG(TRACE_ZCL3, "put error status response, attr_id 0x%x",
                    (FMT__D, write_attr_req->attr_id));
          if (
              /*cstat !MISRAC2012-Rule-13.5 */
              (resp_data != NULL) &&
              /* The following violation of Rule 13.5 seems to be
               * a false positive. There are no side effect to 'zb_zcl_can_cluster_be_fragmented()'.
               * This violation seems to be caused by the fact that
               * 'zb_zcl_can_cluster_be_fragmented()' is an
               * external function, which cannot be analyzed by C-STAT. */
              (ZB_ZCL_GET_BYTES_AVAILABLE(ZCL_CTX().runtime_buf, resp_data,
                                          cmd_info->profile_id, cmd_info->cluster_id)
              >= sizeof(zb_zcl_write_attr_res_t))
          )
          {
            ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);
            ZB_ZCL_PACKET_PUT_DATA16(resp_data, &write_attr_req->attr_id);
          }
        }
      }
    }
  }
  while (write_attr_req != NULL);

  if (ZB_ZCL_CMD_WRITE_ATTRIB_UNDIV == cmd_info->cmd_id
      && can_write_all_attrs)
  {
    TRACE_MSG(TRACE_ZCL1, "WRITE_ATTRIB_UNDIVIDED all attributes checked success", (FMT__0));

    data_ptr = zb_buf_begin(param);
    buf_len = zb_buf_len(param);

    do
    {
      ZB_ZCL_GENERAL_GET_NEXT_WRITE_ATTR_REQ(data_ptr, buf_len, write_attr_req);

      if (write_attr_req != NULL)
      {
        attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, write_attr_req->attr_id, manuf_code);
        TRACE_MSG(TRACE_ZCL3, "attr_desc %p", (FMT__P, attr_desc));

        ZVUNUSED(zb_zcl_set_attr_val_manuf(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                     cmd_info->cluster_id,
                                     (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                     ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE,
                                     attr_desc->id,
                                     manuf_code,
                                     write_attr_req->attr_value,
                                     ZB_FALSE));
        zcl_write_attr_post_process(cmd_info, attr_desc, write_attr_req->attr_value);
      }
    }
    while (write_attr_req != NULL);
  }

  if (ZB_ZCL_CMD_WRITE_ATTRIB_NO_RESP != cmd_info->cmd_id)
  {
    if (can_write_all_attrs)
    {
      if ((resp_data != NULL) && (attr_count > 0U))
      {/* If no error found - report status Ok to all attrs, no attr id
        * is needed */
        TRACE_MSG(TRACE_ZCL3, "report status OK", (FMT__0));
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_STATUS_SUCCESS);
      }
      else
      {/* ZCL spec Rev 7 p. 2.6.3. The command appears to contain the wrong fields, as
        * detected either by the presence of one or more invalid
        * field entries or by there being missing fields.
        * Command not carried out. Implementer has discretion
        * as to whether to return this error or
        * INVALID_FIELD*/
        TRACE_MSG(TRACE_ZCL3, "report status MALFORMED_CMD", (FMT__0));
        ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION( ZCL_CTX().runtime_buf,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
          cmd_info->profile_id,
          cmd_info->cluster_id,
          cmd_info->seq_number,
          cmd_info->cmd_id,
          ZB_ZCL_STATUS_MALFORMED_CMD,
          ZB_ZCL_REVERT_DIRECTION(cmd_info->cmd_direction));
        param = 0;
      }
    }

    if (param > 0U)
    {
      ZB_ZCL_FINISH_N_SEND_PACKET_NEW(ZCL_CTX().runtime_buf, resp_data,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id,
        cmd_info->cluster_id,
        NULL,
        ZB_APS_FC_IS_SECURE(cmd_info->addr_data.common_data.fc),
        ZB_FALSE,
        0U);
    }

    /* Use runtime buffer to send request, use received buffer with
     * request data_buf as runtime buffer */
    ZCL_CTX().runtime_buf = param;

    TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_write_attr_handler", (FMT__0));
  }
  else
  {
    zb_buf_free(param);
  }
  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_write_attr_handler", (FMT__0));

}

#if !(defined ZB_ZCL_DISABLE_REPORTING)
/* Check Configure Report Request parameters to compatibility with given attribute descriptor and proper reporting
   parameters. Returns status of operation. */
static zb_uint8_t check_config_rep_req(zb_zcl_configure_reporting_req_t *config_rep_req, zb_zcl_attr_t *attr_desc)
{
  zb_uint8_t status = ZB_ZCL_STATUS_SUCCESS;

  TRACE_MSG(TRACE_ZCL1, ">> check_config_rep_req", (FMT__0));
  ZB_ASSERT(config_rep_req);

  if (config_rep_req->direction == ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT)
  {
    TRACE_MSG(TRACE_ZCL3, "send report direction", (FMT__0));
    if (!attr_desc)
    {
      if (config_rep_req->attr_id == ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID)
      {
        TRACE_MSG(TRACE_ZCL1, "GLOBAL_CLUSTER_REVISION: attr is not reportable", (FMT__0));
        status = ZB_ZCL_STATUS_UNREPORTABLE_ATTRIB;
      }
      else
      {
      TRACE_MSG(TRACE_ZCL1, "attr not found", (FMT__0));
      status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
    }
    }
    else if (!attr_desc->data_p)
    {
      TRACE_MSG(TRACE_ZCL1, "attr not found", (FMT__0));
      status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
    }
    else
    {
      /* ZCL8 spec 2.5.7.3 Effect on Receipt: if the attribute type is set to array,
         structure, set or bag, set status to UNSUPPORTED_ATTRIBUTE */
      /* TODO: add other types checking */
      if (attr_desc->type == ZB_ZCL_ATTR_TYPE_OCTET_STRING || attr_desc->type == ZB_ZCL_ATTR_TYPE_ARRAY
        || attr_desc->type == ZB_ZCL_ATTR_TYPE_CUSTOM_32ARRAY)
      {
        TRACE_MSG(TRACE_ZCL1, "inalid attr type", (FMT__0));
        status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
      }
      else if (!(attr_desc->access & ZB_ZCL_ATTR_ACCESS_REPORTING))
      {
        TRACE_MSG(TRACE_ZCL1, "attr is not reportable", (FMT__0));
        status = ZB_ZCL_STATUS_UNREPORTABLE_ATTRIB;
      }
      else if (attr_desc->type != config_rep_req->u.clnt.attr_type)
      {
        TRACE_MSG(TRACE_ZCL1, "invalid type attr type %hd, recv type %hd",
                  (FMT__H_H, attr_desc->type, config_rep_req->u.clnt.attr_type));
        status = ZB_ZCL_STATUS_INVALID_TYPE;
      }
      else if (config_rep_req->u.clnt.max_interval != 0 &&
               config_rep_req->u.clnt.max_interval < config_rep_req->u.clnt.min_interval)
      {
        TRACE_MSG(
          TRACE_ZCL1,
          "invalid min/max value min %hd, max %hd",
          ( FMT__H_H,
            config_rep_req->u.clnt.min_interval,
            config_rep_req->u.clnt.max_interval));
        status = ZB_ZCL_STATUS_INVALID_VALUE;
      }
    } /* if !attr_desc*/
   } /* if direction == ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT */
  else
  {
    /* NOTE: For Reports receiving, a cluster (client, as usual) should support the same
       attribute as server, who sends a report. It is implementation specific. */
    TRACE_MSG(TRACE_ZCL3, "recv report direction", (FMT__0));
  }
  TRACE_MSG(TRACE_ZCL1, "<< check_config_rep_req status %hd", (FMT__H, status));

  return status;
}


/*! @internal @brief ZCL configure reporting command handler
    @param param - buffer number
*/
/*! @internal @brief ZCL read reporting configuration command handler
    @param param - buffer number
*/
/**
 *A device shall generate a read reporting configuration
 *response command containing the details of the reporting configuration for each
 *of the attributes specified in the command (see ZCL spec 2.4.10).
 */

void zb_zcl_read_report_config_cmd_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t *resp_data;
  zb_zcl_read_reporting_cfg_req_t read_rep_cfg_req;
  zb_bool_t read_rep_conf_exist;
  zb_uint8_t status;
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_reporting_info_t *attr_rep_info = NULL;
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t free_space = 0;
  zb_uint8_t required_space = 0;
  zb_uint16_t manuf_code;
  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_read_report_config_cmd_handler", (FMT__0));

  /* ZCL spec, 2.4.10 Read Reporting Configuration Response Command */
  /* Read Reporting Configuration response
      | ZCL header 3 b | Attr reporting config rec 1 XX b | Attr reporting config rec 2 XX b | ...

     Attr reporting config rec format
     status 1 b | direction 1 b  | attr id 2 b | attr data type 0/1 b
     | minimum reporting interval 0/2 b  |  maximum reporting interval
     0/2 b | delta 0/Variable | timeout 0/2 b
  */

  /* ZCL frame format */
  /* frame control 1b | manufacturer code 0/2b | TSN 1b | cmd id 1b | payload XXb */

  /* frame control */
  /* frame type | manufacturer specific | direction | disable default resp */

  /* Construct packet header */
  /* Use runtime_buf buffer for composing and sending response, request buffer
   * will be saved as runtime_buf */

  resp_data = ZB_ZCL_START_PACKET(ZCL_CTX().runtime_buf);

  ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_A(
    resp_data, ZB_ZCL_FRAME_DIRECTION_TO_CLI, cmd_info->is_manuf_specific);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(
    resp_data, cmd_info->seq_number,
    cmd_info->is_manuf_specific, cmd_info->manuf_specific,
    ZB_ZCL_CMD_READ_REPORT_CFG_RESP);

  ep_desc = zb_af_get_endpoint_desc(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  /* EP existence was checked on command accept - it must be in the list */
  ZB_ASSERT(ep_desc);

  cluster_desc = get_cluster_desc(ep_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);
  /* Cluster existence was checked on command accept - it must be in the list */
  ZB_ASSERT(cluster_desc);

  manuf_code = (cmd_info->is_manuf_specific) ? cmd_info->manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC;

  do
  {
    /*Try to get attribute reporting configuration record from request */
    ZB_ZCL_GENERAL_GET_NEXT_READ_REP_CFG_REQ(param, read_rep_cfg_req, read_rep_conf_exist);

    if (read_rep_conf_exist)    /*If successful (correct request record found) */
    {
       TRACE_MSG(TRACE_ZCL1, ">>  read_rep_cfg_req.direction = %d read_rep_cfg_req.attr_id = %d )",
                 (FMT__D_D, read_rep_cfg_req.direction ,read_rep_cfg_req.attr_id));

       /* Get available free space in ZCL command buffer*/
       free_space =  ZB_ZCL_GET_BYTES_AVAILABLE(ZCL_CTX().runtime_buf,resp_data,
                                                cmd_info->profile_id, cmd_info->cluster_id);

       /*If there is no space for the smallest responce record*/
       if(free_space<ZB_ZCL_READ_REPORTING_CFG_ERROR_SIZE)
       {
         break;  /*Exit from do...while, finish ZCL buffer and send responce command */
       }

       /*Get attribute descriptor to undestand, is it supported and
        * reportable or not. Calculate status field*/
       attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, read_rep_cfg_req.attr_id, manuf_code);

       if (!attr_desc || !attr_desc->data_p)   /*Unsupported attribute*/
       {
         status = ZB_ZCL_STATUS_UNSUP_ATTRIB;
       } /*Unsupported attribute condition*/

       else if (!(attr_desc->access & ZB_ZCL_ATTR_ACCESS_REPORTING))  /*Unreportable attribute*/
       {
         status = ZB_ZCL_STATUS_UNREPORTABLE_ATTRIB;
       } /*Unreportable attribute condition*/

       else /*If attribute exists and it is reportable*/
       {
         /*Try to get attr_rep_info for attr_id specified in the current
          * request record*/
         attr_rep_info = zb_zcl_find_reporting_info_manuf(ep_desc->ep_id,
                                                          cluster_desc->cluster_id,
                                                          cluster_desc->role_mask,
                                                          read_rep_cfg_req.attr_id,
                                                          (cmd_info->is_manuf_specific) ? cmd_info->manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC);


         if(attr_rep_info)       /*If requested attribute reporting info found*/
         {
           TRACE_MSG(TRACE_ZCL1, "found requested attr_rep_info %p", (FMT__P, attr_rep_info));
           status = ZB_ZCL_STATUS_SUCCESS; /*Set status SUCCESS. See
                                            * spec 2.4.10.1.2 */
         } /*      if(attr_rep_info)                   */
         else  /*If requested attribute reporting info not found*/
         {
           TRACE_MSG(TRACE_ZCL1, "not found requested attr_rep_info %p", (FMT__P, attr_rep_info));
           status = ZB_ZCL_STATUS_NOT_FOUND;  /*Set status
                                                         * NOT_FOUND. See
                                                         * spec 2.5.10.1.2 */
         }
       } /*else   If attribute exists and it is reportable*/



       /*Create responce record depending on status and direction fields*/
       switch(status)
       {
         case ZB_ZCL_STATUS_SUCCESS:
           TRACE_MSG(TRACE_ZCL1, "setting SUCCESS status", (FMT__0));
           /*Calculate required free space for report record, depending on attribute direction value*/
           required_space = ZB_ZCL_READ_REPORTING_CFG_RES_SIZE;
/*
           switch(read_rep_cfg_req.direction)
           {
             case ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT:
               required_space = ZB_ZCL_READ_REPORTING_CFG_FOR_SEND_SIZE;
               break;
             case ZB_ZCL_CONFIGURE_REPORTING_RECV_REPORT:
               required_space = ZB_ZCL_READ_REPORTING_CFG_FOR_RECV_SIZE;
               break;
               } */  /*    switch(read_rep_cfg_req.direction)  */

             if(free_space<required_space) /*If there is not enough
                                          * space for responce
                                          * record in ZCL buffer*/
           {
             read_rep_conf_exist = ZB_FALSE; /*Set condition for exit
                                              * from do...while cycle */
           } /*if(free_space<required_space)*/
           else  /*If we have enough space for responce*/
           {
           TRACE_MSG(TRACE_ZCL1, "setting SUCCESS status", (FMT__0));
             /*Add common entries*/
             ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);                     /*Add status field*/
             ZB_ZCL_PACKET_PUT_DATA8(resp_data, read_rep_cfg_req.direction);   /*Add direction field*/
             ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, read_rep_cfg_req.attr_id);   /*Add attr_id field*/
           TRACE_MSG(TRACE_ZCL1, "setting SUCCESS status end", (FMT__0));
             /*Add entires that depends on direction*/
             switch(read_rep_cfg_req.direction)
             {
               case ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT:
                 TRACE_MSG(TRACE_ZCL1, "case ZB_ZCL_CONFIGURE_REPORTING_SEND_REPORT:", (FMT__0));
                 ZB_ZCL_PACKET_PUT_DATA8(resp_data, attr_desc->type);                            /*Add data_type field*/
                 ZB_ZCL_PACKET_PUT_2DATA16_VAL(resp_data, attr_rep_info->u.send_info.min_interval,   /*Add min_interval field*/
                                               attr_rep_info->u.send_info.max_interval);   /*Add max_interval field*/

                 /* If attribute data type is analog then we need to
                    add delta field that has the same type then the
                    attribute. Else this field shall be omitted. See
                    spec 2.4.10.1.7 */

                 if (zb_zcl_is_analog_data_type(attr_desc->type))
                 {
                   switch (attr_desc->type)
                   {
                     case ZB_ZCL_ATTR_TYPE_U8:
                       ZB_ZCL_PACKET_PUT_DATA8(resp_data, attr_rep_info->u.send_info.delta.u8);    /*Add 8-bit unsigned delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_S8:
                       ZB_ZCL_PACKET_PUT_DATA8(resp_data, attr_rep_info->u.send_info.delta.s8);    /*Add 8-bit signed delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_U16:
                       ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, attr_rep_info->u.send_info.delta.u16);    /*Add 16-bit delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_S16:
                       ZB_ZCL_PACKET_PUT_DATA16(resp_data, &attr_rep_info->u.send_info.delta.s16);    /*Add signed 16-bit delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_U24:
                       ZB_ZCL_PACKET_PUT_DATA24(resp_data, &attr_rep_info->u.send_info.delta.u24);    /*Add 24-bit delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_S24:
                       ZB_ZCL_PACKET_PUT_DATA24(resp_data, &attr_rep_info->u.send_info.delta.s24);    /*Add signed 24-bit delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_U32:
                       ZB_ZCL_PACKET_PUT_DATA32(resp_data, &attr_rep_info->u.send_info.delta.u32);    /*Add 32-bit delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_S32:
                       ZB_ZCL_PACKET_PUT_DATA32(resp_data, &attr_rep_info->u.send_info.delta.s32);    /*Add signed 32-bit delta field*/
                       break;
                     case ZB_ZCL_ATTR_TYPE_U48:
                       ZB_ZCL_PACKET_PUT_DATA48(resp_data, &attr_rep_info->u.send_info.delta.u48);    /*Add 48-bit delta field*/
                       break;
                   }
                 }
                 break;
               case ZB_ZCL_CONFIGURE_REPORTING_RECV_REPORT:
                 TRACE_MSG(TRACE_ZCL1, "case ZB_ZCL_CONFIGURE_REPORTING_RECV_REPORT:", (FMT__0));
                 ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, attr_rep_info->u.recv_info.timeout);   /*Add timeout field*/
                 break;
             } /*    switch(read_rep_cfg_req.direction)  */

           }
           break; /*case ZB_ZCL_STATUS_SUCCESS:*/

         case ZB_ZCL_STATUS_UNSUP_ATTRIB:
         case ZB_ZCL_STATUS_UNREPORTABLE_ATTRIB:
         case ZB_ZCL_STATUS_NOT_FOUND:
         /*We already have enough space for unsuccess report, so it is no need
          * to check it again
          */
           TRACE_MSG(TRACE_ZCL1, "setting UNSUCCESS status", (FMT__0));

           ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);                     /*Add status field*/
           ZB_ZCL_PACKET_PUT_DATA8(resp_data, read_rep_cfg_req.direction);   /*Add direction field*/
           ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, read_rep_cfg_req.attr_id);   /*Add attr_id field*/

           TRACE_MSG(TRACE_ZCL1, "setting UNSUCCESS status end", (FMT__0));

           break;
       } /*switch(status)*/
    } /* if (read_rep_conf_exist)*/
  }  while (read_rep_conf_exist);  /* Stop if there are no more
                                    * attribute reporting
                                    * configuration records in
                                    * request or there is not enough
                                    * space in ZCL command buffer */
  TRACE_MSG(TRACE_ZCL1, "Finishing ZCL packet", (FMT__0));
  TRACE_MSG(TRACE_ZCL1, "Sending request", (FMT__0));
  ZB_ZCL_FINISH_N_SEND_PACKET(ZCL_CTX().runtime_buf, resp_data,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      NULL);

  /* Use runtime buffer to send request, use received buffer with
   * request data_buf as runtime buffer */
  ZCL_CTX().runtime_buf = param;


  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_read_report_config_cmd_handler", (FMT__0));
}


/*! @internal @brief ZCL configure reporting command handler
    @param param - buffer number
*/
void zb_zcl_configure_reporting_handler(zb_uint8_t param)
{
  zb_zcl_configure_reporting_req_t *config_rep_req;

  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t status;
  zb_uint8_t *resp_data;
  zb_short_t req_counter = 0;
  zb_bool_t error_status = ZB_FALSE;
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_ret_t ret;
  zb_uint16_t manuf_code;

  TRACE_MSG(
      TRACE_ZCL1,
      ">>zb_zcl_configure_reporting_handler param %d, cmd_info %p",
      (FMT__H_P, param, cmd_info));

  /* ZCL spec, 2.4.7 Configure Reporting Command Frame */
  /* ZCL header | Attr reporting config record 1 | Attr reporting config record 2| ... */
  /* Attr reporting config record: | Direction = 0, 1b | attr id 2b | attr type 1b | min interval 1b | max interval 1b | delta XXb | */
  /* Attr reporting config record: | Direction = 1, 1b | attr id 2b | timeout XXb | */

  ep_desc = zb_af_get_endpoint_desc(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  /* EP existence was checked on command accept - it must be in the list */
  ZB_ASSERT(ep_desc);

  cluster_desc = get_cluster_desc(ep_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);
  /* Cluster existence was checked on command accept - it must be in the list */
  ZB_ASSERT(cluster_desc);

  /* Construct packet header */
  /* Use runtime_buf buffer for composing and sending response, request buffer
   * will be saved as runtime_buf */
  resp_data = ZB_ZCL_START_PACKET(ZCL_CTX().runtime_buf);

  ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_A(
    resp_data, ZB_ZCL_FRAME_DIRECTION_TO_CLI, cmd_info->is_manuf_specific);
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(
    resp_data, cmd_info->seq_number,
    cmd_info->is_manuf_specific, cmd_info->manuf_specific,
    ZB_ZCL_CMD_CONFIG_REPORT_RESP);

  do
  {
    ZB_ZCL_GENERAL_GET_NEXT_CONFIGURE_REPORTING_REQ(param, config_rep_req);

    TRACE_MSG(TRACE_ZCL2, "config_rep_req %p", (FMT__P, config_rep_req));
    if (config_rep_req)
    {
      req_counter++;

      TRACE_MSG(
          TRACE_ZCL3,
          "direction %hd, attr_id %d, ",
          (FMT__H_D, config_rep_req->direction, config_rep_req->attr_id));
      manuf_code = (cmd_info->is_manuf_specific) ? cmd_info->manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC;
      attr_desc = zb_zcl_get_attr_desc_manuf(cluster_desc, config_rep_req->attr_id, manuf_code);
      TRACE_MSG(TRACE_ZCL3, "attr_desc %p", (FMT__P, attr_desc));

      status = check_config_rep_req(config_rep_req, attr_desc);

      if (status == ZB_ZCL_STATUS_SUCCESS)
      {
        /* Accept reporting setting */
        zb_zcl_attr_addr_info_t addr_info;

        addr_info.src_ep = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
        addr_info.cluster_id = cmd_info->cluster_id;
        addr_info.dst_ep = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint;
        addr_info.dst_short_addr = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr;
        addr_info.profile_id = cmd_info->profile_id;
        addr_info.cluster_role = (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
          ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE;
        addr_info.manuf_code = (cmd_info->is_manuf_specific) ? cmd_info->manuf_specific : ZB_ZCL_NON_MANUFACTURER_SPECIFIC;

        ret = zb_zcl_put_reporting_info_from_req(config_rep_req, &addr_info);
        if (ret != RET_OK)
        {
          TRACE_MSG(TRACE_ERROR, "Error saving reporting info %d", (FMT__D, ret));
          status = ZB_ZCL_STATUS_FAIL;
        }
      }

      if (status != ZB_ZCL_STATUS_SUCCESS)
      {
        /* Construct error response */
        zb_uint16_t bytes_avail;
        error_status = ZB_TRUE;

        TRACE_MSG(TRACE_ZCL3, "put error status response", (FMT__0));
        bytes_avail = ZB_ZCL_GET_BYTES_AVAILABLE(ZCL_CTX().runtime_buf, resp_data,
                                                 cmd_info->profile_id, cmd_info->cluster_id);
        if (bytes_avail >= sizeof(zb_zcl_configure_reporting_res_t))
        {
          ZB_ZCL_PACKET_PUT_DATA8(resp_data, status);
          ZB_ZCL_PACKET_PUT_DATA8(resp_data, config_rep_req->direction);
          ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, config_rep_req->attr_id);
        }
      }
    } /* if config_rep_req */
    else if (!req_counter)
    {
      /* no configuration record was read, error */
      error_status = ZB_TRUE;
      TRACE_MSG(TRACE_ZCL3, "report error in packet", (FMT__0));
      ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_STATUS_MALFORMED_CMD);
    }
  }
  while(config_rep_req);

  if (error_status == ZB_FALSE)
  {
    /* If no error found - report status Ok to all attrs, no attr id
     * is needed */
    TRACE_MSG(TRACE_ZCL3, "report status OK", (FMT__0));
    ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_STATUS_SUCCESS);
  }

  ZB_ZCL_FINISH_N_SEND_PACKET(ZCL_CTX().runtime_buf, resp_data,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      NULL);

  /* Use runtime buffer to send request, use received buffer with
   * request data_buf as runtime buffer */
  ZCL_CTX().runtime_buf = param;

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_configure_reporting_handler", (FMT__0));
}


void zb_zcl_send_report_attr_command(zb_zcl_reporting_info_t *rep_info, zb_uint8_t param)
{
  zb_uint8_t *cmd_data;
  zb_zcl_reporting_info_t *cur_rep_info;
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t bytes_avail;
  zb_uint8_t attr_size;
  zb_uint8_t send_mode;
  zb_uint8_t is_manuf_spec;

  TRACE_MSG(
      TRACE_ZCL1,
      ">> zb_zcl_send_report_attr_command rep_info %p, param %hd",
      (FMT__P_H, rep_info, param));

  attr_desc =
    zb_zcl_get_attr_desc_manuf_a(
      rep_info->ep,
      rep_info->cluster_id,
      rep_info->cluster_role,
      rep_info->attr_id,
      rep_info->manuf_code);

  is_manuf_spec = !!ZB_ZCL_IS_ATTR_MANUF_SPEC(attr_desc);

  /* ZCL spec, 2.4.11 Report Attributes Command */
  /* Read attribute command
     | ZCL header 3 b | Attr Report 1 XX b | Attr Report 1 XX b | ...

     Attr Report format
     | attr id 2 b |  Attr data type 1 b | attr value XX b |
  */
  /* Construct packet header */
  /* Use buffer specified by input param */
  cmd_data = ZB_ZCL_START_PACKET(param);

  /* NOTE: currently, manufacturer specific is not supported */
  ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_REQ_FRAME_CONTROL_A(
    cmd_data, ZB_ZCL_FRAME_DIRECTION_TO_CLI,
    is_manuf_spec, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);

  if (is_manuf_spec)
  {
    TRACE_MSG(TRACE_ERROR, "rep_info->manuf_code 0x%x", (FMT__D, rep_info->manuf_code));
    ZB_ASSERT(rep_info->manuf_code != ZB_ZCL_NON_MANUFACTURER_SPECIFIC);

    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(cmd_data, ZB_ZCL_GET_SEQ_NUM(), ZB_TRUE, rep_info->manuf_code, ZB_ZCL_CMD_REPORT_ATTRIB);
  }
  else
  {
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER(cmd_data, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_REPORT_ATTRIB);
  }

  cur_rep_info = rep_info;

  while (cur_rep_info)
  {
    attr_desc =
      zb_zcl_get_attr_desc_manuf_a(cur_rep_info->ep, cur_rep_info->cluster_id, cur_rep_info->cluster_role, cur_rep_info->attr_id, cur_rep_info->manuf_code);
    TRACE_MSG(TRACE_ZCL3, "attr_desc %p", (FMT__P, attr_desc));

    /* attribute description could not be absent, it is checked while accepting configure report
       command */
    ZB_ASSERT(attr_desc);

    bytes_avail = ZB_ZCL_GET_BYTES_AVAILABLE(param, cmd_data,
                                             cur_rep_info->dst.profile_id, cur_rep_info->cluster_id);
    TRACE_MSG(TRACE_ZCL3, "bytes_avail %hd", (FMT__H, bytes_avail));

    TRACE_MSG(
        TRACE_ZCL3,
        "attribute: id 0x%x, type 0x%hx, manuf_code 0x%x",
        (FMT__D_H_D, attr_desc->id, attr_desc->type, attr_desc->manuf_code));
    attr_size = zb_zcl_get_attribute_size(attr_desc->type, attr_desc->data_p);
    TRACE_MSG(TRACE_ZCL3, "attr_size %hd", (FMT__H, attr_size));

    /* Decrement read attr response by sizeof(zb_uint8_t), because attr value size is
     * calculated separately by attr_size  */
    if (bytes_avail >= (sizeof(zb_zcl_report_attr_req_t) - sizeof(zb_uint8_t) + attr_size))
    {
      ZB_ZCL_PACKET_PUT_DATA16_VAL(cmd_data, attr_desc->id);
      ZB_ZCL_PACKET_PUT_DATA8(cmd_data, attr_desc->type);
      cmd_data = zb_zcl_put_attribute_value(cmd_data, attr_desc, attr_desc->data_p, attr_size);

      zb_zcl_save_reported_value(cur_rep_info, attr_desc);

      ZB_ZCL_CLR_REPORTING_FLAG(cur_rep_info, ZB_ZCL_REPORT_ATTR);
      ZB_ZCL_CLR_REPORTING_FLAG(cur_rep_info, ZB_ZCL_REPORT_IS_ALLOWED);
      ZB_ZCL_CLR_REPORTING_FLAG(cur_rep_info, ZB_ZCL_REPORT_TIMER_STARTED);
      ZB_ZCL_SET_REPORTING_FLAG(cur_rep_info, ZB_ZCL_REPORT_IS_SENT);
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "ERROR, buffer is full", (FMT__0));
      break;
    }

    /* get next report info for sending report to the same remote device */
    cur_rep_info = zb_zcl_get_next_reporting_info(rep_info, is_manuf_spec);
    TRACE_MSG(TRACE_ZCL1, "cur_rep_info %p", (FMT__P, cur_rep_info));
  }

  /* We should always use bindings for reporting (ZCL8, 2.5.7.1.2; ZCL8, 2.5.11.2) */
  send_mode = ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
  TRACE_MSG(TRACE_ZCL1, "ZCL_REPORT send mode ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT:", (FMT__0));

  ZB_ZCL_FINISH_N_SEND_PACKET(param, cmd_data,
      rep_info->dst.short_addr,
      send_mode,
      rep_info->dst.endpoint,
      rep_info->ep /* src ep */,
      rep_info->dst.profile_id,
      rep_info->cluster_id,
      zb_zcl_reporting_cb);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_send_report_attr_command ", (FMT__0));
}

#endif

/*! @internal @brief ZCL discovery attrubutes handler
    @param param - index of the buffer with discovery attributes request
*/
static void zb_zcl_disc_attr_handler(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_zcl_attr_t *attr_desc;
  zb_uint8_t *resp_data;
  zb_zcl_disc_attr_req_t *disc_attr_req;
  zb_uint8_t i, j;
  zb_uint8_t *complete;
  zb_bool_t extended = (zb_bool_t)(cmd_info->cmd_id == ZB_ZCL_CMD_DISCOVER_ATTR_EXT);
  zb_uint32_t max_info_num;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_disc_attr_handler param %hd extended %hd",
            (FMT__H_H, param, extended));

  /* ZCL8 spec, 2.5.13 Discovery Attributes Response Command
     HA1.2 spec 12.1.6.1 Discovery Attributes Extended Response Command */
  /* Discovery attribute (extended) response
     | Discovery complete 1 b | attribute information 1 4 b | attribute information 1 4 b | ...

     attribute information
     | attr id 2 b | Attr data type 1 b | {Attribute access control 1 b - extended info} |
  */

  /* ZCL frame format */
  /* frame control 1b | manufacturer code 0/2b | TSN 1b | cmd id 1b | payload XXb */

  /* frame control */
  /* frame type | manufacturer specific | direction | disable default resp */

  /* TODO: implement correct parsing to get request (check command size, convert endianess if needed) */
  /* We are using the same structure/parsing utility for discover
   * attr/discover attr extended because commands are completely identical */
  disc_attr_req = (zb_zcl_disc_attr_req_t*)zb_buf_begin(param);
  TRACE_MSG(TRACE_ZCL2, "disc_attr_req %p", (FMT__P, disc_attr_req));

  max_info_num = extended ?
    (ZB_ZCL_HI_WO_IEEE_MAX_PAYLOAD_SIZE / sizeof(zb_zcl_disc_attr_ext_info_t)) :
    (ZB_ZCL_HI_WO_IEEE_MAX_PAYLOAD_SIZE / sizeof(zb_zcl_disc_attr_info_t));

  // ZB_ZCL_DISC_ATTR_MAX - max possibly number attr for send
  if (disc_attr_req->maximum > max_info_num)
  {
    ZB_ASSERT(max_info_num <= ZB_UINT8_MAX);
    disc_attr_req->maximum = (zb_uint8_t)max_info_num;
  }

  /* Construct packet header */
  /* Use runtime_buf buffer for composing and sending response, request buffer
   * will be saved as runtime_buf */
  resp_data = ZB_ZCL_START_PACKET(ZCL_CTX().runtime_buf);

  /* generate frame control taking into account manufacturer specific */
  ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_EXT(
    resp_data, cmd_info->is_manuf_specific, ZB_ZCL_REVERT_DIRECTION(cmd_info->cmd_direction));

  /* Generate command header taking into account manufacturer specific */
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(
    resp_data, cmd_info->seq_number, cmd_info->is_manuf_specific, cmd_info->manuf_specific,
    (extended!=ZB_FALSE) ? ZB_ZCL_CMD_DISCOVER_ATTR_EXT_RES : ZB_ZCL_CMD_DISC_ATTRIB_RESP);

  ep_desc = zb_af_get_endpoint_desc(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  /* EP existence was checked on command accept - it must be in the list */
  ZB_ASSERT(ep_desc);

  cluster_desc = get_cluster_desc(ep_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);
  /* Cluster existence was checked on command accept - it must be in the list */
  ZB_ASSERT(cluster_desc);

  // find start
  attr_desc = cluster_desc->attr_desc_list;

  complete = resp_data;
  /* There is the same beginning for both Discover Attr and Discover attr ext Resp */
  ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_DISC_NON_COMPLETE);

  i = 0;
  /* Decrement (cluster_desc->attr_count) by 1 because
    the last record in cluster_desc->attr_desc_list contain is empty record */

  if (cluster_desc->attr_count == 0U)
  {
    /* Special case: need to put ClusterRevision here */
    zb_uint16_t id = ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID;

    ZB_ZCL_PACKET_PUT_DATA16(resp_data, &id);
    ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_ATTR_TYPE_U16);
    if (extended)
    {
      ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_ATTR_ACCESS_READ_ONLY);
    }
    *complete = ZB_ZCL_DISC_COMPLETE;
  }
  else
  {
    for (j = 0; j < cluster_desc->attr_count - 1U && i < disc_attr_req->maximum; j++)
    {
      TRACE_MSG(TRACE_ZCL2, "check attr_id %d", (FMT__D, attr_desc->id));
      if (attr_desc->id >= disc_attr_req->start_attr_id
          && ZB_ZCL_ATTR_CHECK_INTERNAL_ID(attr_desc->id) == 0U)
      {
        if (extended)
        {
          /* For Discover Attr extended perform additional check -
           * manufacturer specific data */
          TRACE_MSG(TRACE_ZCL2, "extended: check is_manuf spec %hd",
                    (FMT__H, cmd_info->is_manuf_specific));
          if (cmd_info->is_manuf_specific && ZB_ZCL_ATTR_CHECK_CUSTOM_ID(attr_desc->id) == 0U)
          {
            /* manufacturer specific attr is requested but this attr "common", skip it */
            TRACE_MSG(TRACE_ZCL2, "extended: skip NON manuf specific attr", (FMT__0));
            continue;
          }
          else if (!cmd_info->is_manuf_specific && ZB_ZCL_ATTR_CHECK_CUSTOM_ID(attr_desc->id) != 0U)
          {
            /* non-manufacturer specific attr is requested but this attr
             * manufacturer specific, skip it */
            TRACE_MSG(TRACE_ZCL2, "extended: skip manuf spec attr", (FMT__0));
            continue;
          }
          else
          {
            /* MISRA rule 15.7 requires empty 'else' branch. */
          }
          /* TODO: need to handle Manufacturer ID code somehow (compare
           * it with this cluster/attribute manufacturer ID) but
           * currently no Manufacturer specific ID is supported for the cluster */
        }

        ZB_ZCL_PACKET_PUT_DATA16_VAL(resp_data, attr_desc->id);
        ZB_ZCL_PACKET_PUT_DATA8(resp_data, attr_desc->type);
        if (extended)
        {
          ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_CONVERT_ATTR_ACCESS_BITMASK(attr_desc->access));
        }
        i++;
      }
      attr_desc++;
    }

   // correct complete field
  *complete = (j < cluster_desc->attr_count - 1U) ? ZB_ZCL_DISC_NON_COMPLETE : ZB_ZCL_DISC_COMPLETE;
  }

  ZB_ZCL_FINISH_N_SEND_PACKET(ZCL_CTX().runtime_buf, resp_data,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      NULL);

  /* Use runtime buffer to send request, use received buffer with
   * request data_buf as runtime buffer */
  ZCL_CTX().runtime_buf = param;

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_disc_attr_handler", (FMT__0));
}

#if !(defined ZB_ZCL_DISABLE_REPORTING) && defined(ZB_COMPILE_ZCL_SAMPLE)

void send_configure_reporting_clnt_req_sample(
    zb_bufid_t data_buf,
    zb_uint16_t attr_id,
    zb_uint8_t attr_type,
    zb_uint8_t *delta,
    zb_uint16_t min_interval,
    zb_uint16_t max_interval,
    zb_uint16_t dst_addr,
    zb_uint8_t dst_ep,
    zb_uint8_t src_ep)
{
  zb_uint8_t *ptr;

  ZB_ZCL_GENERAL_INIT_CONFIGURE_REPORTING_SRV_REQ(data_buf, ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
  ZB_ZCL_GENERAL_ADD_SEND_REPORT_CONFIGURE_REPORTING_REQ(
      ptr,
      attr_id,
      attr_type,
      min_interval,
      max_interval,
      delta);
  ZB_ZCL_GENERAL_SEND_CONFIGURE_REPORTING_REQ(
      data_buf,
      ptr,
      dst_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      dst_ep,
      src_ep,
      ZB_AF_HA_PROFILE_ID,
      ZB_ZCL_CLUSTER_ID_ON_OFF,
      NULL);

}

void send_configure_reporting_srv_req_sample(
    zb_bufid_t data_buf,
    zb_uint16_t attr_id,
    zb_uint16_t timeout,
    zb_uint16_t prof_id,
    zb_uint16_t dst_addr,
    zb_uint8_t dst_ep,
    zb_uint8_t src_ep)
{
  zb_uint8_t *ptr;

  ZB_ZCL_GENERAL_INIT_CONFIGURE_REPORTING_CLI_REQ(data_buf, ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
  ZB_ZCL_GENERAL_ADD_RECV_REPORT_CONFIGURE_REPORTING_REQ(ptr, attr_id, timeout);
  ZB_ZCL_GENERAL_SEND_CONFIGURE_REPORTING_REQ(
      data_buf,
      ptr,
      dst_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      dst_ep,
      src_ep,
      prof_id,
      ZB_ZCL_CLUSTER_ID_ON_OFF,
      NULL);

}


void send_write_attr_req_sample(
    zb_bufid_t data_buf,
    zb_uint16_t attr_id,
    zb_uint8_t attr_type,
    zb_uint8_t *attr_val,
    zb_uint16_t prof_id,
    zb_uint16_t dst_addr,
    zb_uint8_t dst_ep,
    zb_uint8_t src_ep)
{
  zb_uint8_t *ptr;

  ZB_ZCL_GENERAL_INIT_WRITE_ATTR_REQ(data_buf, ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
  ZB_ZCL_GENERAL_ADD_VALUE_WRITE_ATTR_REQ(ptr, attr_id, attr_type, attr_val);
  ZB_ZCL_GENERAL_SEND_WRITE_ATTR_REQ(
      data_buf,
      ptr,
      dst_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      dst_ep,
      src_ep,
      prof_id,
      ZB_ZCL_CLUSTER_ID_ON_OFF,
      NULL);

}

#endif /* ZB_COMPILE_ZCL_SAMPLE */

zb_bool_t zb_zcl_handle_general_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_FALSE;
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_handle_general_commands param %hd", (FMT__H, param));

  TRACE_MSG(TRACE_ZCL1, ">cmd_id 0x%hd cluster 0x%x", (FMT__H_D, cmd_info->cmd_id, cmd_info->cluster_id));

  switch (cmd_info->cmd_id)
  {
    case ZB_ZCL_CMD_READ_ATTRIB:
/* [AV] fantastic side effect for read attribute!
   Time is not 0 or 42 only on PC */
      zb_zcl_read_attr_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_READ_ATTRIB_RESP:
      TRACE_MSG(TRACE_ZCL1, "ZB_ZCL_CMD_READ_ATTRIB_RESP", (FMT__0));

#if defined ZB_SE_COMMISSIONING || (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH && defined ZB_ZCL_ENABLE_WWAH_SERVER)
      if (ZCL_SELECTOR().read_attr_resp_handler != NULL)
      {
        processed = ZCL_SELECTOR().read_attr_resp_handler(param);
      }
#endif /* ZB_SE_COMMISSIONING || (ZB_ZCL_SUPPORT_CLUSTER_WWAH && ZB_ZCL_ENABLE_WWAH_SERVER) */

#if defined (ZB_ZCL_SUPPORT_CLUSTER_TIME)
      if (!processed && (ZB_ZCL_CLUSTER_ID_TIME == cmd_info->cluster_id))
      {
          processed = zb_zcl_time_server_read_attr_handle(param);
      }
#endif
      /*cstat !MISRAC2012-Rule-14.3_a */
      /* The 'processed' variable is defined in the beggining of this function and never takes a
       * value different than 'ZB_FALSE' due to several reasons:
       * Since 'ZB_ZDO_PERIODIC_CHECKIN_BLOCK_ZCL_CMD()' always returns 'ZB_FALSE', the 'if' statement
       *    above is never evaluated as true and the value of 'processed' never changes. A deviation
       *    record has been added;
       * Parts of the current 'case' clause which change the value of 'processed' are disabled using
       *    '#ifdef' guards.
       */
      if (!processed)
      {
        zb_zcl_read_attr_resp_handler(param);
        processed = ZB_TRUE;
      }
      break;

    case ZB_ZCL_CMD_WRITE_ATTRIB:
      TRACE_MSG(TRACE_ZCL2, "CMD_WRITE_ATTRIB default handler", (FMT__0));
      zb_zcl_write_attr_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_WRITE_ATTRIB_RESP:
      /* TODO: implement default response handler with notifying
       * user about received attribute value via callback */
      zb_buf_free(param);
      processed = ZB_TRUE;
      break;

#if !(defined ZB_ZCL_DISABLE_REPORTING)
#ifdef ZB_ZCL_ENABLE_DEFAULT_REPORT_ATTR_PROCESSING
    case ZB_ZCL_CMD_REPORT_ATTRIB:
      TRACE_MSG(TRACE_ZCL2, "CMD_REPORT_ATTRIB default handler", (FMT__0));
      zb_zcl_report_attr_cmd_handler(param);
      processed = ZB_TRUE;
      break;
#endif

    case ZB_ZCL_CMD_CONFIG_REPORT:
      TRACE_MSG(TRACE_ZCL2, "CMD_CONFIG_REPORT default handler", (FMT__0));
      zb_zcl_configure_reporting_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_CONFIG_REPORT_RESP:
      TRACE_MSG(TRACE_ZCL2, "CMD_CONFIG_REPORT default handler", (FMT__0));
      zb_buf_free(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_READ_REPORT_CFG:
      TRACE_MSG(TRACE_ZCL2, "CMD_READ_REPORT_CONFIG default handler", (FMT__0));
      zb_zcl_read_report_config_cmd_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_READ_REPORT_CFG_RESP:
      /* Nothing to do - just skip it */
      break;

#endif

    case ZB_ZCL_CMD_WRITE_ATTRIB_UNDIV:
      TRACE_MSG(TRACE_ZCL2, "CMD_WRITE_ATTRIB_UNDIV default handler", (FMT__0));
      zb_zcl_write_attr_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_WRITE_ATTRIB_NO_RESP:
      TRACE_MSG(TRACE_ZCL2, "CMD_WRITE_ATTRIB_NO_RESP default handler", (FMT__0));
      zb_zcl_write_attr_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_DISC_ATTRIB:
      TRACE_MSG(TRACE_ZCL2, "CMD_DISC_ATTRIB default handler", (FMT__0));
      zb_zcl_disc_attr_handler(param);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_DISC_ATTRIB_RESP:
      break;

    case ZB_ZCL_CMD_DEFAULT_RESP:
      TRACE_MSG(TRACE_ZCL2, "ZB_ZCL_CMD_DEFAULT_RESP", (FMT__0));
      processed = zb_zcl_handle_default_response_commands(param);
      /*cstat !MISRAC2012-Rule-14.3_a */
      if (!processed)
      {
        /** @mdr{00026,0} */
        zb_buf_free(param);
        processed = ZB_TRUE;
      }
      break;

#ifdef ZB_ENABLE_HA
    case ZB_ZCL_CMD_DISCOVER_COMMANDS_RECEIVED:
      zb_zcl_discover_commands_res(param, ZB_TRUE);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_DISCOVER_COMMANDS_GENERATED:
      zb_zcl_discover_commands_res(param, ZB_FALSE);
      processed = ZB_TRUE;
      break;

    case ZB_ZCL_CMD_DISCOVER_ATTR_EXT:
      TRACE_MSG(TRACE_ZCL2, "CMD_DISC_ATTR_EXTENDED default handler", (FMT__0));
      zb_zcl_disc_attr_handler(param);
      processed = ZB_TRUE;
      break;


    case ZB_ZCL_CMD_DISCOVER_COMMANDS_RECEIVED_RES:
    case ZB_ZCL_CMD_DISCOVER_COMMANDS_GENERATED_RES:
    case ZB_ZCL_CMD_DISCOVER_ATTR_EXT_RES:
      /* Nothing to do - just skip it */
      break;

#endif /* ZB_ENABLE_HA */

    default:
      TRACE_MSG(TRACE_ERROR, "unknown ZCL cmd", (FMT__0));
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_handle_general_commands processed %hd", (FMT__H, processed));
  return processed;
}/* zb_bool_t zb_zcl_handle_general_commands(zb_uint8_t param) */


static zb_bool_t zb_zcl_handle_default_response_commands(zb_uint8_t param)
{
  zb_bool_t processed;
  /*cstat !MISRAC2012-Rule-2.2_c */
  /* Even if both '#if' in this function evaluate to false, cmd_info is used in a trace message in
   * the end of this function. */
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_handle_default_response_commands param %hd", (FMT__H, param));

  /* MISRA Rule 16.6
   * Switch-case statements must have at least two switch-clauses. This was not verified in the
   * previous switch when at least one of the switch-clauses was disabled with '#if defined' guards.
   * Removed switch and a if-else structure is now used. */

#if defined ZB_ZCL_ENABLE_DEFAULT_OTA_UPGRADE_PROCESSING
  if (cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_OTA_UPGRADE)
  {
    /* There was a call to
     * processed = zb_zcl_process_ota_upgrade_default_response_commands(param);
     * Now OTA server processes ZB_ZCL_CMD_DEFAULT_RESP itself.
     * That is done to exclude calling of OTA srv not via cluster handler.
     * Removing that dependency excludes OTA code linking if OTA is not declared by the app. */
    processed = zb_zcl_handle_specific_commands(param);
  }
  else
#endif /* defined ZB_ZCL_ENABLE_DEFAULT_OTA_UPGRADE_PROCESSING */
#if defined ZB_ZCL_ENABLE_DEFAULT_TUNNEL_PROCESSING
      if (cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_TUNNEL)
  {
    processed = zb_zcl_process_tunnel_default_response_commands(param) == RET_OK;
  }
  else
#endif /* defined ZB_ZCL_ENABLE_DEFAULT_TUNNEL_PROCESSING */
  {
    TRACE_MSG(TRACE_ZCL1, "Cluster 0x%04x was not processed", (FMT__D, cmd_info->cluster_id));
    processed = ZB_FALSE;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_handle_default_response_commands processed %hd",
            (FMT__H, processed));
  return processed;
} /* zb_uint8_t zb_zcl_handle_default_response_commands(zb_uint8_t param) */


#if defined ZB_ENABLE_HA

static zb_bool_t zb_zcl_compare_current_role(zb_uint8_t endpoint, zb_uint8_t direction, zb_uint16_t cluster_id)
{
  zb_bool_t ret = ZB_FALSE;
  zb_zcl_cluster_desc_t *cluster_desc = NULL;
  zb_af_endpoint_desc_t *endpoint_desc;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_compare_current_role endpoint %hd, direction %hd, cluster_id %d",
            (FMT__H_H_D, endpoint, direction, cluster_id));

  endpoint_desc = zb_af_get_endpoint_desc(endpoint);
  if(endpoint_desc != NULL)
  {
    cluster_desc = get_cluster_desc(endpoint_desc, cluster_id,
                                    (direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                    ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);
  }

  if (cluster_desc != NULL)
  {
    ret = (zb_bool_t)(((direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) && (cluster_desc->role_mask & ZB_ZCL_CLUSTER_SERVER_ROLE)) ||
          ((direction == ZB_ZCL_FRAME_DIRECTION_TO_CLI) && (cluster_desc->role_mask & ZB_ZCL_CLUSTER_CLIENT_ROLE)));
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_compare_current_role ret %hd", (FMT__H, ret));
  return ret;
}

zb_uint8_t zb_zcl_get_cmd_list_by_cluster_id(zb_zcl_cluster_handler_t cluster_id,
  zb_bool_t is_received, zb_uint8_t **cmd_list);

void zb_zcl_discover_commands_res(zb_uint8_t param, zb_bool_t recv_cmd_type)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_disc_cmd_req_t cmd_req;
  zb_uint8_t *cmd_list = NULL;
  zb_uint8_t cmd_count = 0;
  zb_zcl_parse_status_t status;


  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_discover_commands_res param %hd, recv_cmd_type %hd",
            (FMT__H_H, param, recv_cmd_type));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t),
            sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_GENERAL_GET_DISC_COMMAND(&cmd_req, param, status);

  if (status!=ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of Discovery Command",
        (FMT__0));

    zb_zcl_send_default_handler(param, &cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
  }
  else
  {
    if (cmd_info.is_manuf_specific)
    {
      if(ZCL_CTX().disc_manuf_cmd_cb != NULL)
      {
        cmd_count = (*ZCL_CTX().disc_manuf_cmd_cb)(cmd_info.cmd_direction, cmd_info.addr_data.common_data.dst_endpoint,
            cmd_info.cluster_id, cmd_info.manuf_specific, recv_cmd_type, &cmd_list);
      }
    }
    else
    {
      if( zb_zcl_compare_current_role(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.cmd_direction, cmd_info.cluster_id) )
      {
        zb_zcl_cluster_handler_t cluster_handler;

        cluster_handler = zb_zcl_get_cluster_handler_internal(cmd_info.addr_data.common_data.dst_endpoint,
                                                              cmd_info.cluster_id,
                                                              (cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                                              ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);

        if (cluster_handler)
        {
          if ( cluster_handler(ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM) )
          {
            if(recv_cmd_type)
            {
              cmd_count = ZCL_CTX().zb_zcl_cluster_cmd_list->received_cnt;
              cmd_list = ZCL_CTX().zb_zcl_cluster_cmd_list->received;
              ZCL_CTX().zb_zcl_cluster_cmd_list = NULL;
            }
            else
            {
              cmd_count = ZCL_CTX().zb_zcl_cluster_cmd_list->generated_cnt;
              cmd_list = ZCL_CTX().zb_zcl_cluster_cmd_list->generated;
              ZCL_CTX().zb_zcl_cluster_cmd_list = NULL;
            }
          }
          else
          {
            TRACE_MSG(TRACE_ZCL1, "Commands list not supported for Cluster 0x%04x", (FMT__D, cmd_info.cluster_id));
            cmd_count = 0;
          }
        }
        else
        {
          TRACE_MSG(TRACE_ZCL1, "Can't resolve Cluster 0x%04x (not loaded or no commands)", (FMT__D, cmd_info.cluster_id));
          cmd_count = 0;
        }
      }
    }

    // create response
    {
      zb_uint8_t cmd_for_receive = 0;
      zb_uint8_t i;
      zb_uint8_t j;
      zb_uint8_t *cmd_ptr;
      zb_uint8_t *complete_ptr;

      cmd_for_receive = (ZB_ZCL_HI_WO_IEEE_MAX_PAYLOAD_SIZE - sizeof(zb_zcl_disc_cmd_resp_t)) / sizeof(zb_uint8_t);
      if (cmd_for_receive > cmd_req.maximum)
      {
        cmd_for_receive = cmd_req.maximum;
      }

      ZB_ZCL_GENERAL_INIT_DISC_COMMAND_RESP(cmd_ptr, param,
          ZB_ZCL_REVERT_DIRECTION(cmd_info.cmd_direction),
          cmd_info.seq_number,
          (recv_cmd_type!=ZB_FALSE ? ZB_ZCL_CMD_DISCOVER_COMMANDS_RECEIVED_RES :
                  ZB_ZCL_CMD_DISCOVER_COMMANDS_GENERATED_RES),
          cmd_info.is_manuf_specific,
          cmd_info.manuf_specific,
          ZB_ZCL_DISC_CMD_NOT_COMPLETE);

      complete_ptr = cmd_ptr-1;

      j = 0;
      for(i = 0; i < cmd_count; i++)
      {
        if (cmd_list[i] >= cmd_req.start_cmd_id)
        {
          ZB_ZCL_GENERAL_ADD_DISC_COMMAND_RESP(cmd_ptr, cmd_list[i]);
          j++;
          if (j >= cmd_for_receive)
          {
            break;
          }
        }
      }

      if (i>=cmd_count)
      {
        *complete_ptr = ZB_ZCL_DISC_CMD_COMPLETE;
      }

      /* send response */
      ZB_ZCL_GENERAL_FINISH_DISC_COMMAND_RESP(param, cmd_ptr,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
        cmd_info.profile_id,
        cmd_info.cluster_id);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_discover_commands_res", (FMT__0));
}

#endif /* ZB_ENABLE_HA */

#endif /* ZB_ENABLE_ZCL */
