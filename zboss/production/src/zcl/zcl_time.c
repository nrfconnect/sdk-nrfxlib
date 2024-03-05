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
/* PURPOSE: ZCL: Time cluster implementation
*/

#define ZB_TRACE_FILE_ID 2085

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_TIME)

#include "zb_zcl.h"
#include "zb_aps.h"
#include "zcl/zb_zcl_common.h"

#define ZB_ZCL_TIME_SERVER_SYNCHRONIZATION_TIMEOUT_SECONDS 100
zb_ret_t check_value_time_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
void zb_zcl_time_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);

#ifdef THAT_CB_IS_NEVER_USED_CAUSING_WARNING
static zb_zcl_time_set_real_time_clock_t       zb_zcl_set_real_time_clock_cb;
#endif
static zb_zcl_time_sync_time_server_found_cb_t zb_zcl_time_server_found_cb = NULL;
static zb_bool_t                               zb_zcl_search_in_process = ZB_FALSE;
static zb_uint8_t                              zb_zcl_src_search_endpoint   = 0;

zb_bool_t check_value_time(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

static void zb_zcl_time_sync_stop(zb_uint8_t unused)
{
  ZVUNUSED(unused);
  if (zb_zcl_time_server_found_cb != NULL)
  {
    zb_zcl_time_server_found_cb(RET_TIMEOUT, 0, 0, 0 , 0);
  }
  zb_zcl_search_in_process = ZB_FALSE;
}


static zb_uint8_t zb_zcl_time_sync_get_auth_level(zb_uint8_t time_status, zb_uint16_t short_addr)
{
  if (ZB_ZCL_TIME_TIME_STATUS_MASTER_BIT_IS_SET(time_status) && ZB_ZCL_TIME_TIME_STATUS_SUPERSEDING_BIT_IS_SET(time_status))
  {
    return ((0x0000 == short_addr) ?
            ZB_ZCL_TIME_COORDINATOR_WITH_MASTER_AND_SUPERSEDING_BITS :
            ZB_ZCL_TIME_HAS_MASTER_AND_SUPERSEDING_BITS);
  }
  else if (ZB_ZCL_TIME_TIME_STATUS_MASTER_BIT_IS_SET(time_status))
  {
    return ZB_ZCL_TIME_HAS_MASTER_BIT;
  }
  else if (ZB_ZCL_TIME_TIME_STATUS_SYNCHRONIZED_BIT_IS_SET(time_status))
  {
    return ZB_ZCL_TIME_HAS_SYNCHRONIZED_BIT;
  }
  else
  {
    return ZB_ZCL_TIME_SERVER_NOT_CHOSEN;
  }
}

zb_bool_t zb_zcl_time_server_read_attr_handle(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_read_attr_res_t *resp = NULL;
  zb_uint8_t time_status = ZB_ZCL_TIME_TIME_STATUS_DEFAULT_VALUE;
  zb_uint32_t nw_time = ZB_ZCL_TIME_TIME_DEFAULT_VALUE;
  zb_uint8_t fails = 0;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_time_server_read_attr_handle %hd", (FMT__H, param));

  if (cmd_info->cluster_id != ZB_ZCL_CLUSTER_ID_TIME || !zb_zcl_search_in_process)
  {
    TRACE_MSG(TRACE_ZCL1, "Not handled cluster id %ld", (FMT__H, cmd_info->cluster_id));

    return ZB_FALSE;
  }


  ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, resp);

  while (resp != NULL)
  {
    if (resp->status != ZB_ZCL_STATUS_SUCCESS)
    {
      TRACE_MSG(TRACE_ZCL1, "Attribute read error: attr 0x%x, status %d", (FMT__D_D, resp->attr_id, resp->status));
      ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, resp);
      fails++;

      continue;
    }

    TRACE_MSG(TRACE_ZCL1, "read attr resp: src_addr 0x%x ep %hd",
              (FMT__D_D, ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr, ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint));

    if (ZB_ZCL_ATTR_TIME_TIME_STATUS_ID == resp->attr_id)
    {
      time_status = resp->attr_value[0];
      TRACE_MSG(TRACE_ZCL1, "Time status attr value: 0x%hx", (FMT__H, time_status));
    }
    else if (ZB_ZCL_ATTR_TIME_TIME_ID == resp->attr_id)
    {
      nw_time = ZB_ZCL_ATTR_GET32(resp->attr_value);
      TRACE_MSG(TRACE_ZCL1, "Time attr value: %ld", (FMT__L, nw_time));
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Unknown attribute: attr 0x%x", (FMT__D, resp->attr_id));
      fails++;
    }

    ZB_ZCL_GENERAL_GET_NEXT_READ_ATTR_RES(param, resp);
  }


  if (0 == fails)
  {
    zb_uint8_t curr_auth_level;

    curr_auth_level = zb_zcl_time_sync_get_auth_level(time_status,
                                                      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr);

    if (zb_zcl_time_server_found_cb != NULL)
    {
      zb_zcl_time_server_found_cb(RET_OK, curr_auth_level, ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
                                  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint, nw_time);
    }
    else
    {
      TRACE_MSG(TRACE_ZCL3, "WARNING!!! there is no callback set to handle time server found", (FMT__0));
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Mallformed Read Time Status attribute response", (FMT__H, param));
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_time_server_read_attr_handle", (FMT__0));

  return ZB_TRUE;
}


static void zb_zcl_time_server_start_search_cb(zb_uint8_t param)
{
  zb_zdo_match_desc_resp_t *resp = (zb_zdo_match_desc_resp_t *)zb_buf_begin(param);
  zb_uint8_t dst_endpoint;
  zb_uint16_t short_addr;
  zb_uint8_t *match_ep;
  zb_uint8_t *cmd_ptr;
  zb_apsde_data_indication_t *ind = ZB_BUF_GET_PARAM(param, zb_apsde_data_indication_t);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_time_server_start_search_cb %hd", (FMT__H, param));

  TRACE_MSG(TRACE_ZCL2, "resp match_len %hd", (FMT__H, resp->match_len));

  if (resp->status == ZB_ZDP_STATUS_SUCCESS && resp->match_len > 0)
  {
    /* Match EP list follows right after response header */
    match_ep = (zb_uint8_t*)(resp + 1);

    /* set EP value directly to attribute value */
    /* we are searching for exact cluster, so only 1 EP maybe found */
    dst_endpoint = *match_ep;
    short_addr = ind->src_addr;
    /* ZB_BUF_CLEAR_PARAM(ZB_BUF_FROM_REF(param)); */

    /* Send Read time status and time attributes */
    ZB_ZCL_GENERAL_INIT_READ_ATTR_REQ(param, cmd_ptr, ZB_ZCL_ENABLE_DEFAULT_RESPONSE);
    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, (ZB_ZCL_ATTR_TIME_TIME_STATUS_ID));
    ZB_ZCL_GENERAL_ADD_ID_READ_ATTR_REQ(cmd_ptr, (ZB_ZCL_ATTR_TIME_TIME_ID));
    ZB_ZCL_GENERAL_SEND_READ_ATTR_REQ(
        param, cmd_ptr, short_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT, dst_endpoint, zb_zcl_src_search_endpoint,
         ZB_AF_HA_PROFILE_ID, (ZB_ZCL_CLUSTER_ID_TIME), NULL);


    TRACE_MSG(TRACE_ZCL2, "find_time cluster addr %d ep %hd",
              (FMT__D_H, short_addr, dst_endpoint));


  }
  else
  {
    zb_buf_free(param);
  }


  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_time_server_start_search_cb", (FMT__0));
}


static void zb_zcl_time_server_start_search(zb_uint8_t param)
{
  zb_zdo_match_desc_param_t *req;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_time_server_start_search %hd", (FMT__H, param));

  req = zb_buf_initial_alloc(param, sizeof(zb_zdo_match_desc_param_t) + (1) * sizeof(zb_uint16_t));

  req->nwk_addr = ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE;
  req->addr_of_interest = ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE;
  req->profile_id = ZB_AF_HA_PROFILE_ID;
  req->num_in_clusters = 1;
  req->num_out_clusters = 0;
  req->cluster_list[0] = ZB_ZCL_CLUSTER_ID_TIME;

  zb_zdo_match_desc_req(param, zb_zcl_time_server_start_search_cb);


  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_time_server_start_search", (FMT__0));
}


void zb_zcl_time_server_synchronize(zb_uint8_t endpoint, zb_zcl_time_sync_time_server_found_cb_t cb)
{
  zb_uint8_t time_status;
  zb_zcl_attr_t *attr = zb_zcl_get_attr_desc_a(endpoint, ZB_ZCL_CLUSTER_ID_TIME, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_TIME_TIME_STATUS_ID);
  ZB_ASSERT(attr);
  time_status = ((zb_uint8_t *)attr->data_p)[0];


  if (zb_zcl_time_sync_get_auth_level(time_status, zb_get_short_address()) <= ZB_ZCL_TIME_HAS_SYNCHRONIZED_BIT)
  {
    zb_zcl_time_server_found_cb = cb;
    zb_zcl_src_search_endpoint = endpoint;
    zb_zcl_search_in_process = ZB_TRUE;
    ZB_SCHEDULE_ALARM(zb_zcl_time_sync_stop, 0, ZB_TIME_ONE_SECOND * ZB_ZCL_TIME_SERVER_SYNCHRONIZATION_TIMEOUT_SECONDS);
    zb_buf_get_out_delayed(zb_zcl_time_server_start_search);
  }
}

void zb_zcl_time_init_server()
{
  ZB_ZCL_TIME_SET_REAL_TIME_CLOCK_CB((zb_zcl_time_set_real_time_clock_t)NULL);

  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_TIME,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_time_server,
                              zb_zcl_time_write_attr_hook_server,
                              (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_set_real_time_clock_callback(zb_zcl_time_set_real_time_clock_t cb)
{
  ZVUNUSED(cb);
#ifdef THAT_CB_IS_NEVER_USED_CAUSING_WARNING
  zb_zcl_set_real_time_clock_cb = cb;
#endif
}

void zb_zcl_time_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_TIME,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_time_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_zcl_attr_t *time_status_desc;

  TRACE_MSG(TRACE_ZCL1, ">> check_value_time <<", (FMT__0));

  time_status_desc = zb_zcl_get_attr_desc_a(endpoint,
                                            ZB_ZCL_CLUSTER_ID_TIME,
                                            ZB_ZCL_CLUSTER_SERVER_ROLE,
                                            ZB_ZCL_ATTR_TIME_TIME_STATUS_ID);

  if (ZB_ZCL_ATTR_TIME_TIME_ID == attr_id)
  {
    if (ZB_ZCL_TIME_TIME_STATUS_MASTER_BIT_IS_SET(ZB_ZCL_GET_ATTRIBUTE_VAL_8(time_status_desc)) ||
        ZB_ZCL_ATTR_GET32(value) == ZB_ZCL_TIME_TIME_INVALID_VALUE)
    {
      TRACE_MSG(TRACE_ZCL1, "Master bit is set (Time synchronization is not allowed) or invalid time value",
                (FMT__0));

      return RET_BLOCKED;
    }
  }
  else if (ZB_ZCL_ATTR_TIME_TIME_STATUS_ID == attr_id)
  {
    /* The Synchronized bit must be explicitly written to indicate this - i.e.,
     * it is not set automatically on writing to the Time attribute.
     * If the Master bit is 1, the value of this bit is 0.
     *
     * ZCL 2.5.3.3 Write Attributes Command - Effect on Receipt
     * 4. If the device is not currently accepting write attribute commands for the attribute,
     * the status field of the corresponding write attribute status record SHALL be set to
     * NOT_AUTHORIZED or READ_ONLY.
     */

    if (ZB_ZCL_TIME_TIME_STATUS_MASTER_BIT_IS_SET(ZB_ZCL_GET_ATTRIBUTE_VAL_8(time_status_desc)) &&
        ZB_ZCL_TIME_TIME_STATUS_SYNCHRONIZED_BIT_IS_SET(*value))
    {
      TRACE_MSG(TRACE_ZCL1, "Don't write Time Status attribute: Master Bit is set", (FMT__0));
      return RET_BLOCKED;
    }

    /* Master(0-bit) and MasterZoneDst(2nd-bit) bits are not writeable */

    *value = (ZB_ZCL_GET_ATTRIBUTE_VAL_8(time_status_desc) & 0x05) | (*value & 0xFA);
  }
  else if (ZB_ZCL_ATTR_TIME_DST_START_ID == attr_id
           || ZB_ZCL_ATTR_TIME_DST_END_ID == attr_id)
  {
    if (ZB_ZCL_TIME_TIME_STATUS_MASTER_ZONE_DST_BIT_IS_SET(ZB_ZCL_GET_ATTRIBUTE_VAL_8(time_status_desc)))
    {
      TRACE_MSG(TRACE_ZCL1, "MasterZoneDst bit is set. Time synchronization is not allowed.",
                (FMT__0));
      return RET_BLOCKED;
    }
    else if (ZB_ZCL_TIME_TIME_INVALID_VALUE == ZB_ZCL_ATTR_GET32(value))
    {
      zb_uint32_t val = ZB_ZCL_ATTR_GET32(value);
      TRACE_MSG(TRACE_ZCL1, "Invalid attribute value! attr_id == 0x%x, value == 0x%lx",
                (FMT__D_L, attr_id, val));
      return RET_ERROR;
    }
  }

  return RET_OK;
}

void zb_zcl_time_update_current_time(zb_uint8_t endpoint)
{
  zb_uint32_t time_value;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_time_update_current_time ep %hd", (FMT__H, endpoint));

  time_value = zb_get_utc_time();
  /* This time setting does not produce any ZCL packets, so no need to catch the status and
   * send it in response.
   */
  ZB_ZCL_SET_ATTRIBUTE(endpoint,
                       ZB_ZCL_CLUSTER_ID_TIME,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_TIME_TIME_ID,
                       (zb_uint8_t *)(&time_value),
                       ZB_FALSE);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_time_update_current_time", (FMT__0));
}

static zb_uint32_t zb_zcl_color_control_get32(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_TIME, ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id);
  return (attr_desc) ? ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc) : ZB_ZCL_TIME_TIME_INVALID_VALUE;
}

static zb_int32_t zb_zcl_color_control_getS32(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_TIME, ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id);
  return (attr_desc) ? ZB_ZCL_GET_ATTRIBUTE_VAL_S32(attr_desc) : 0;
}

/** @brief Hook on Write attribute */
void zb_zcl_time_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  zb_zcl_attr_t* attr_desc;
  zb_uint32_t
    time = ZB_ZCL_TIME_TIME_INVALID_VALUE,
    standard_time = ZB_ZCL_TIME_TIME_INVALID_VALUE,
    dst_start = ZB_ZCL_TIME_TIME_INVALID_VALUE,
    dst_end = ZB_ZCL_TIME_TIME_INVALID_VALUE;
  zb_int32_t time_zone = 0, dst_shift = 0;
  zb_bool_t update_standard_time = ZB_FALSE;
  zb_bool_t update_local_time = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_time_write_attr_hook_server endpoint %hx attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D, endpoint, attr_id, manuf_code));

  ZVUNUSED(manuf_code);

  switch (attr_id)
  {
    case ZB_ZCL_ATTR_TIME_TIME_ID:
    {
      time = ZB_ZCL_ATTR_GET32(new_value);

      /* Update LastSetTime */
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                         ZB_ZCL_CLUSTER_ID_TIME,
                                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                                         ZB_ZCL_ATTR_TIME_LAST_SET_TIME_ID);
      if (attr_desc != NULL)
      {
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, time);
        TRACE_MSG(TRACE_ZCL1, "Update LastSetTime to 0x%lx",
                  (FMT__L, ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc)));
      }

      update_standard_time = ZB_TRUE;
      update_local_time = ZB_TRUE;
    }
    break;

    case ZB_ZCL_ATTR_TIME_TIME_ZONE_ID:
    {
      time_zone = ZB_ZCL_ATTR_GET32(new_value);

      update_standard_time = ZB_TRUE;
    }
    break;

    case ZB_ZCL_ATTR_TIME_DST_START_ID:
    {
      dst_start = ZB_ZCL_ATTR_GET32(new_value);

      update_local_time = ZB_TRUE;
    }
    break;

    case ZB_ZCL_ATTR_TIME_DST_END_ID:
    {
      dst_end = ZB_ZCL_ATTR_GET32(new_value);

      update_local_time = ZB_TRUE;
    }
    break;

    case ZB_ZCL_ATTR_TIME_DST_SHIFT_ID:
    {
      dst_shift = ZB_ZCL_ATTR_GETS32(new_value);

      update_local_time = ZB_TRUE;
    }
    break;

    default:
      break;
  }

  if (attr_id != ZB_ZCL_ATTR_TIME_TIME_ID)
  {
    time = zb_zcl_color_control_get32(endpoint, ZB_ZCL_ATTR_TIME_TIME_ID);
  }

  /* Update StandardTime */
  if (update_standard_time)
  {
    if (attr_id != ZB_ZCL_ATTR_TIME_TIME_ZONE_ID)
    {
      time_zone = zb_zcl_color_control_getS32(endpoint, ZB_ZCL_ATTR_TIME_TIME_ZONE_ID);
    }

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                       ZB_ZCL_CLUSTER_ID_TIME,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_TIME_STANDARD_TIME_ID);

    if (attr_desc && time != ZB_ZCL_TIME_TIME_INVALID_VALUE)
    {
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, time + time_zone);
      TRACE_MSG(TRACE_ZCL1, "Update StandardTime to 0x%lx",
                (FMT__L, ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc)));
      standard_time = ZB_ZCL_GET_ATTRIBUTE_VAL_32(attr_desc);
    }
  }

  /* Update LocalTime */
  if (update_local_time)
  {
    if (attr_id != ZB_ZCL_ATTR_TIME_DST_START_ID)
    {
      dst_start = zb_zcl_color_control_get32(endpoint, ZB_ZCL_ATTR_TIME_DST_START_ID);
    }

    if (attr_id != ZB_ZCL_ATTR_TIME_DST_END_ID)
    {
      dst_end = zb_zcl_color_control_get32(endpoint, ZB_ZCL_ATTR_TIME_DST_END_ID);
    }

    if (attr_id != ZB_ZCL_ATTR_TIME_DST_SHIFT_ID)
    {
      dst_shift = zb_zcl_color_control_getS32(endpoint, ZB_ZCL_ATTR_TIME_TIME_ZONE_ID);
    }

    if (standard_time == ZB_ZCL_TIME_TIME_INVALID_VALUE)
    {
      standard_time = zb_zcl_color_control_get32(endpoint, ZB_ZCL_ATTR_TIME_STANDARD_TIME_ID);
    }

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                       ZB_ZCL_CLUSTER_ID_TIME,
                                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                                       ZB_ZCL_ATTR_TIME_LOCAL_TIME_ID);

    if (attr_desc &&
        standard_time != ZB_ZCL_TIME_TIME_INVALID_VALUE &&
        dst_start != ZB_ZCL_TIME_TIME_INVALID_VALUE &&
        dst_end != ZB_ZCL_TIME_TIME_INVALID_VALUE)
    {
      if (dst_start <= time && time <= dst_end)
      {
        /* Local Time = Standard Time + DstShift */
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, standard_time + dst_shift);
      }
      else
      {
        /* Local Time = Standard Time */
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, standard_time);
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_time_write_attr_hook_server", (FMT__0));
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_TIME */
