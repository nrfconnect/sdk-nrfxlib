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
/* PURPOSE: Zigbee cluster library, the main module
*/

#define ZB_TRACE_FILE_ID 2074

#include "zb_common.h"

/* #define ZB_ZCL_ENABLE_DEFAULT_ALARMS_PROCESSING */

#if defined (ZB_ENABLE_ZCL)

#include "zb_zcl.h"
#include "zb_aps.h"
#include "zcl/zb_zcl_common.h"

#if !(defined ZB_ZCL_DISABLE_REPORTING)
#include "zcl/zb_zcl_reporting.h"
#endif

#include "zcl/zb_zcl_cvc_commands.h"
#include "zcl/zcl_ota_upgrade_commands.h"
#ifdef ZB_ENABLE_HA_SAS
#include "ha/zb_ha_sas.h"
#endif
#include "zb_bdb_internal.h"
#include "zb_zdo.h"
#include "zb_aps.h"
#ifdef ZB_ENABLE_SE
#include "zb_se.h"
#endif

#include "zdo_wwah_stubs.h"

static zb_uint_t zb_zcl_get_header_size(zb_bufid_t buf)
{
  zb_uint8_t* zcl_packet = zb_buf_begin(buf);
  return ZB_ZCL_GET_HEADER_SIZE(*zcl_packet);
}

#if defined HA_CERTIFICATION
zb_bool_t zb_zcl_rejected_command(zb_uint8_t param);
#endif /* HA_CERTIFICATION */

void zb_zcl_init()
{
  TRACE_MSG(TRACE_ZCL1, ">>zcl_init", (FMT__0));

  ZB_BZERO(&ZCL_CTX(), sizeof(zb_zcl_globals_t));

  ZB_BZERO(&ZCL8_CTX(), sizeof(zb_zcl8_globals_t));
/*
  min_interval and max_interval are set to ZERO value by BZERO call
#if !(defined ZB_ZCL_DISABLE_REPORTING)
  ZCL_CTX().reporting_ctx.min_interval == ZB_ZCL_TIMEOUT_ZERO;
  ZCL_CTX().reporting_ctx.max_interval == ZB_ZCL_TIMEOUT_ZERO;
#endif
*/

  ZCL_CTX().runtime_buf = zb_buf_get_any();

  ZB_ASSERT(ZCL_CTX().runtime_buf);

#if !(defined ZB_ZCL_DISABLE_REPORTING)
/*
  Initialize reporting context, allocate free out buffer for reporting.
  TODO: comment if reporting is not supported.
*/
  zb_zcl_init_reporting_ctx();
#endif

#if defined ZB_ENABLE_HA_SAS
  zb_ha_process_sas();
#endif /* defined ZB_ENABLE_HA_SAS */

  TRACE_MSG(TRACE_ZCL1, "<<zcl_init", (FMT__0));
}

zb_uint8_t zb_zcl_handle(zb_uint8_t param)     /* VS Fixed */
{
  zb_zcl_parsed_hdr_t *cmd_info_ptr =
    (zb_zcl_parsed_hdr_t *)ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_uint8_t status; /* zcl_status */
  zb_bool_t processed = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_handle param %hd", (FMT__H, param));

  TRACE_MSG(
      TRACE_ZCL2,
      "f type %d cmd %d",
      (FMT__D_D, cmd_info_ptr->is_common_command, cmd_info_ptr->cmd_id));

  status = zb_zcl_check_accept_command(param);
  if (status == ZB_ZCL_STATUS_SUCCESS)
  {
    if (cmd_info_ptr->is_common_command)
    {
      processed = zb_zcl_handle_general_commands(param);
    }
    else
    {
      processed = zb_zcl_handle_specific_commands(param);
    }

    if (!processed)
    {
      /* TODO: return correct status by zb_zcl_handle_general_commands() */
      TRACE_MSG(TRACE_ZCL1, "command is not processed", (FMT__0));
      /* ZCL8: CCB 2477: use UNSUP_COMMAND instead of any other Unsupported command status */
      if (zb_zcl_get_backward_compatible_statuses_mode() == ZB_ZCL_STATUSES_ZCL8_MODE)
      {
        status = ZB_ZCL_STATUS_UNSUP_CMD;
      }
      else
      {
        if (cmd_info_ptr->is_manuf_specific)
        {
          status = cmd_info_ptr->is_common_command ?
            ZB_ZCL_STATUS_UNSUP_MANUF_GEN_CMD : ZB_ZCL_STATUS_UNSUP_MANUF_CLUST_CMD;
        }
        else
        {
          status = cmd_info_ptr->is_common_command ?
            ZB_ZCL_STATUS_UNSUP_GEN_CMD : ZB_ZCL_STATUS_UNSUP_CLUST_CMD;
        }
      }
    }
  } /* zb_zcl_check_accept_command() */
  else
  {
#if defined HA_CERTIFICATION
    if (zb_zcl_rejected_command(param) )
    {
      processed = ZB_TRUE;
    }
    else
#endif /* HA_CERTIFICATION */
      TRACE_MSG(TRACE_ZCL1, "skip command processing", (FMT__0));
  }

  if (processed)
  {
    status = ZB_ZCL_STATUS_SUCCESS;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_handle, ret %hd", (FMT__H, status));

  return status;
}

void zb_zcl_deinit()
{
  TRACE_MSG(TRACE_ZCL1, ">>zcl_deinit", (FMT__0));

  TRACE_MSG(TRACE_ZCL1, "<<zcl_deinit", (FMT__0));
}

/* FIXME: why is this routine in zcl_main.c while it used only once from zb_zcl_process_device_command which is in zcl_common.c?? */
/* Function parses ZCL header and puts values to zb_zcl_parsed_hdr_t
 * structure. Also, it
 * 1) copies values APSDE data indication - it will be used during ZCL command handling
 * 2) check if command processing allowed by WWAH cluster */

zb_zcl_status_t zb_zcl_parse_header(zb_uint8_t param, zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_apsde_data_indication_t *ind = ZB_BUF_GET_PARAM(param, zb_apsde_data_indication_t);
  zb_uint8_t *zcl_header;
  zb_zcl_status_t ret = ZB_ZCL_STATUS_SUCCESS;
  zb_uint8_t frame_type;

  TRACE_MSG(
      TRACE_ZCL1,
      ">> zb_zcl_parse_header: param %hd h 0x%hx sz %hd, dst endp %hd",
      (FMT__H_H_H_H, param, zb_buf_get_handle(param), zb_buf_len(param), ind->dst_endpoint));

  /* buffer should have enough data for parsing command! */
  if (zb_buf_len(param) < zb_zcl_get_header_size(param))
  {
    TRACE_MSG(TRACE_ERROR, "ERROR: buffer should have enough data for parsing command!", (FMT__0));
    return ZB_ZCL_STATUS_MALFORMED_CMD;
  }

  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.addr_type = ZB_ZCL_ADDR_TYPE_SHORT;
  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr = ind->src_addr;
  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint = ind->src_endpoint;
  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_addr = ind->dst_addr;
  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint = ind->dst_endpoint;
  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).fc = ind->fc;

#ifdef ZB_ENABLE_SE
  if (ZB_SE_MODE())
  {
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).aps_key_source = ind->aps_key_source;
    ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).aps_key_attrs = ind->aps_key_attrs;
  }
#endif
#if (defined ZB_ENABLE_SE) || (defined ZB_ZCL_SUPPORT_CLUSTER_WWAH)
  ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).aps_key_from_tc = ind->aps_key_from_tc;
#endif

  cmd_info->cluster_id = ind->clusterid;
  cmd_info->profile_id = ind->profileid;

  zcl_header = zb_buf_begin(param);

  /* ZCL frame format */
  /* frame control 1b | manufacturer code 0/2b | TSN 1b | cmd id 1b | payload XXb */

  /* frame control */
  /* frame type | manufacturer specific | direction | disable default resp */

  /* Parse ZCL frame control */
  frame_type = ZB_ZCL_GET_FRAME_TYPE(*zcl_header);
  cmd_info->is_common_command = (frame_type == ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC ) ? ZB_FALSE : ZB_TRUE;
  if( frame_type != ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC && frame_type != ZB_ZCL_FRAME_TYPE_COMMON)
  {
    ret = ZB_ZCL_STATUS_INVALID_FIELD;
  }
  cmd_info->cmd_direction = ZB_ZCL_GET_DIRECTION(*zcl_header);
  cmd_info->disable_default_response = (ZB_ZCL_GET_DISABLE_DEFAULT_RESPONSE(*zcl_header) != 0U )?(ZB_TRUE ):(ZB_FALSE );
  if (ZB_ZCL_GET_MANUF_SPECIFIC(*zcl_header) != 0U)
  {
    cmd_info->is_manuf_specific = ZB_TRUE;
    zcl_header++;
    ZB_ZCL_PACKET_GET_DATA16(&(cmd_info->manuf_specific), zcl_header);
  }
  else
  {
    /* there is no manufacturer specific sub field, get TSN field */
    cmd_info->is_manuf_specific = ZB_FALSE;
    zcl_header++;
  }

  /* Get TSN */
  cmd_info->seq_number = *zcl_header;
  zcl_header++;

  /* Get command ID */
  cmd_info->cmd_id = *zcl_header;

  /* Note: there was cut of ZCL header here. Moved to zb_zcl_process_device_command() - the only caller of this routine. */
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_parse_header ret = %hd", (FMT__H, ret));

  return ret;
}

zb_ret_t zb_zcl_add_cluster_handlers(zb_uint16_t cluster_id,
                                     zb_uint8_t cluster_role,
                                     zb_zcl_cluster_check_value_t cluster_check_value,
                                     zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook,
                                     zb_zcl_cluster_handler_t cluster_handler)
{
  zb_ret_t ret = RET_EMPTY;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_add_cluster_handlers cluster 0x%x role %hd",
            (FMT__H_D, cluster_id, cluster_role));
  TRACE_MSG(TRACE_ZCL1, "check_value %p write_attr_hook %p handler %p",
            (FMT__P_P_P, cluster_check_value, cluster_write_attr_hook, cluster_handler));
  if ((cluster_handler != NULL || cluster_check_value != NULL || cluster_write_attr_hook != NULL)
      /*cstat !MISRAC2012-Rule-13.5 !MISRAC2012-Rule-13.6 */
      /* After some investigation, the following violations of Rule 13.5 and 13.6 seem to be false
       * positives. There are no side effect to 'ZCL_CTX()'. This violation seems to be caused by
       * the fact that 'ZCL_CTX()' is an external macro, which cannot be analyzed by C-STAT. */
      && ZCL_CTX().zcl_handlers_cnt < ZB_ARRAY_SIZE(ZCL_CTX().zcl_handlers))
  {
    zb_uindex_t i = 0;
    /* Search if we already have handler */
    while (i < ZCL_CTX().zcl_handlers_cnt)
    {
      if (ZCL_CTX().zcl_handlers[i].cluster_id == cluster_id
          /*cstat !MISRAC2012-Rule-13.5 */
          /* After some investigation, the following violation of Rule 13.5 seems to be a false
           * positive. There are no side effect to 'ZCL_CTX()'. This violation seems to be caused by
           * the fact that 'ZCL_CTX()' is an external macro, which cannot be analyzed by C-STAT. */
          && ZCL_CTX().zcl_handlers[i].cluster_role == cluster_role)
      {
        break;
      }
      ++i;
    }
    if (i == ZCL_CTX().zcl_handlers_cnt)
    {
      /* add new entry */
      ZCL_CTX().zcl_handlers[i].cluster_id = cluster_id;
      ZCL_CTX().zcl_handlers[i].cluster_role = cluster_role;
      ZCL_CTX().zcl_handlers[i].cluster_handler = cluster_handler;
      ZCL_CTX().zcl_handlers[i].cluster_check_value = cluster_check_value;
      ZCL_CTX().zcl_handlers[i].cluster_write_attr_hook = cluster_write_attr_hook;
      ++ZCL_CTX().zcl_handlers_cnt;
      ret = RET_OK;
    }
    else
    {
      ret = RET_ALREADY_EXISTS;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_add_cluster_handlers cluster ret %d", (FMT__D, ret));

  return ret;
}

zb_zcl_cluster_handler_t zb_zcl_get_cluster_handler(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = 0;
  zb_zcl_cluster_handler_t ret = NULL;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_get_cluster_handler: cluster_id 0x%x cluster_role %hd",
            (FMT__D_H, cluster_id, cluster_role));

  /* Search if we already have handler */
  while (i < ZCL_CTX().zcl_handlers_cnt)
  {
    if (ZCL_CTX().zcl_handlers[i].cluster_id == cluster_id
        /*cstat !MISRAC2012-Rule-13.5 */
        /* After some investigation, the following violation of Rule 13.5 seems to be a false
         * positive. There are no side effect to 'ZCL_CTX()'. This violation seems to be caused by
         * the fact that 'ZCL_CTX()' is an external macro, which cannot be analyzed by C-STAT. */
        && (ZCL_CTX().zcl_handlers[i].cluster_role == cluster_role
            /* || ZCL_CTX().zcl_handlers[i].cluster_role == ZB_ZCL_CLUSTER_MIXED_ROLE */))
    {
      ret = ZCL_CTX().zcl_handlers[i].cluster_handler;
      break;
    }
    ++i;
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_get_cluster_handler ret %p", (FMT__P, ret));

  return ret;
}

zb_zcl_cluster_check_value_t zb_zcl_get_cluster_check_value(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = 0;
  ZVUNUSED(cluster_role);

  TRACE_MSG(TRACE_ZCL3, "zb_zcl_get_cluster_check_value: cluster_id %d cluster_role %hd",
            (FMT__D_H, cluster_id, cluster_role));

  /* Search if we already have handler */
  while (i < ZCL_CTX().zcl_handlers_cnt)
  {
    if (ZCL_CTX().zcl_handlers[i].cluster_id == cluster_id
        /*cstat !MISRAC2012-Rule-13.5 */
        /* After some investigation, the following violation of Rule 13.5 seems to be a false
         * positive. There are no side effect to 'ZCL_CTX()'. This violation seems to be caused by
         * the fact that 'ZCL_CTX()' is an external macro, which cannot be analyzed by C-STAT. */
        && ZCL_CTX().zcl_handlers[i].cluster_role == cluster_role)
    {
      return ZCL_CTX().zcl_handlers[i].cluster_check_value;
    }
    ++i;
  }
  return NULL;
}

zb_zcl_cluster_write_attr_hook_t zb_zcl_get_cluster_write_attr_hook(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = 0;
  ZVUNUSED(cluster_role);

  TRACE_MSG(TRACE_ZCL3, "zb_zcl_get_cluster_write_attr_hook: cluster_id %d cluster_role %hd",
            (FMT__D_H, cluster_id, cluster_role));

  /* Search if we already have handler */
  while (i < ZCL_CTX().zcl_handlers_cnt)
  {
    if (ZCL_CTX().zcl_handlers[i].cluster_id == cluster_id
        /*cstat !MISRAC2012-Rule-13.5 */
        /* After some investigation, the following violation of Rule 13.5 seems to be a false
         * positive. There are no side effect to 'ZCL_CTX()'. This violation seems to be caused by
         * the fact that 'ZCL_CTX()' is an external macro, which cannot be analyzed by C-STAT. */
        && ZCL_CTX().zcl_handlers[i].cluster_role == cluster_role)
    {
      return ZCL_CTX().zcl_handlers[i].cluster_write_attr_hook;
    }
    ++i;
  }
  return NULL;
}

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
zb_ret_t zb_zcl_add_control4_cluster_handlers(zb_uint16_t cluster_id, zb_uint8_t cluster_role,
                                              zb_zcl_cluster_check_value_t cluster_check_value,
                                              zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook,
                                              zb_zcl_cluster_handler_t cluster_handler)
{
  zb_ret_t ret = RET_EMPTY;

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_add_control4_cluster_handlers cluster 0x%x role %hd",
            (FMT__H_D, cluster_id, cluster_role));

  if (cluster_handler || cluster_check_value || cluster_write_attr_hook)
  {
    zb_uindex_t i = (cluster_role == ZB_ZCL_CLUSTER_SERVER_ROLE ? 0 : 1);

    /* add new entry */
    ZCL_CTX().control4_zcl_handlers[i].cluster_id = cluster_id;
    ZCL_CTX().control4_zcl_handlers[i].cluster_role = cluster_role;
    ZCL_CTX().control4_zcl_handlers[i].cluster_handler = cluster_handler;
    ZCL_CTX().control4_zcl_handlers[i].cluster_check_value = cluster_check_value;
    ZCL_CTX().control4_zcl_handlers[i].cluster_write_attr_hook = cluster_write_attr_hook;
    ret = RET_OK;
  }

  return ret;
}

zb_zcl_cluster_handler_t zb_zcl_get_control4_cluster_handler(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = (cluster_role == ZB_ZCL_CLUSTER_SERVER_ROLE ? 0 : 1);

  TRACE_MSG(TRACE_ZCL1, "zb_zcl_get_control4_cluster_handler: cluster_id 0x%x cluster_role %hd",
            (FMT__D_H, cluster_id, cluster_role));

  /* Search if we already have handler */
  if (ZCL_CTX().control4_zcl_handlers[i].cluster_id == cluster_id &&
      ZCL_CTX().control4_zcl_handlers[i].cluster_role == cluster_role)
  {
    return ZCL_CTX().control4_zcl_handlers[i].cluster_handler;
  }

  return NULL;
}

zb_zcl_cluster_check_value_t zb_zcl_get_control4_cluster_check_value(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = (cluster_role == ZB_ZCL_CLUSTER_SERVER_ROLE ? 0 : 1);

  TRACE_MSG(TRACE_ERROR, "zb_zcl_get_control4_cluster_check_value: cluster_id %d cluster_role %hd",
            (FMT__D_H, cluster_id, cluster_role));

  /* Search if we already have handler */
  if (ZCL_CTX().control4_zcl_handlers[i].cluster_id == cluster_id
      && ZCL_CTX().control4_zcl_handlers[i].cluster_role == cluster_role)
  {
    return ZCL_CTX().control4_zcl_handlers[i].cluster_check_value;
  }

  return NULL;
}

zb_zcl_cluster_write_attr_hook_t zb_zcl_get_control4_cluster_write_attr_hook(zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_uindex_t i = (cluster_role == ZB_ZCL_CLUSTER_SERVER_ROLE ? 0 : 1);

  TRACE_MSG(TRACE_ERROR, "zb_zcl_get_control4_cluster_write_attr_hook: cluster_id %d cluster_role %hd",
            (FMT__D_H, cluster_id, cluster_role));

  /* Search if we already have handler */
  if (ZCL_CTX().control4_zcl_handlers[i].cluster_id == cluster_id
      && ZCL_CTX().control4_zcl_handlers[i].cluster_role == cluster_role)
  {
    return ZCL_CTX().control4_zcl_handlers[i].cluster_write_attr_hook;
  }

  return NULL;
}

static zb_bool_t zb_zcl_check_control4_cluster_handler(zb_uint8_t endpoint)
{
  zb_bool_t res = ZB_FALSE;

  ZVUNUSED(endpoint);

#ifdef ZB_ED_FUNC
  res = (zb_bool_t )(zb_control4_network_permitted() && endpoint == ZB_CONTROL4_NETWORK_ENDPOINT);
#endif /* ZB_ED_FUNC */

  return res;
}
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */

zb_zcl_cluster_handler_t zb_zcl_get_cluster_handler_internal(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_zcl_cluster_handler_t cluster_handler;

  ZVUNUSED(endpoint);

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
  if (zb_zcl_check_control4_cluster_handler(endpoint))
  {
    cluster_handler = zb_zcl_get_control4_cluster_handler(cluster_id, cluster_role);
  }
  else
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */
  {
    cluster_handler = zb_zcl_get_cluster_handler(cluster_id, cluster_role);
  }

  return cluster_handler;
}

zb_zcl_cluster_check_value_t zb_zcl_internal_get_cluster_check_value(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_zcl_cluster_check_value_t cluster_check_value;

  ZVUNUSED(endpoint);

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
  if (zb_zcl_check_control4_cluster_handler(endpoint))
  {
    cluster_check_value = zb_zcl_get_control4_cluster_check_value(cluster_id, cluster_role);
  }
  else
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */
  {
    cluster_check_value = zb_zcl_get_cluster_check_value(cluster_id, cluster_role);
  }

  return cluster_check_value;
}

zb_zcl_cluster_write_attr_hook_t zb_zcl_internal_get_cluster_write_attr_hook(zb_uint8_t endpoint, zb_uint16_t cluster_id, zb_uint8_t cluster_role)
{
  zb_zcl_cluster_write_attr_hook_t cluster_write_attr_hook;

  ZVUNUSED(endpoint);

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
  if (zb_zcl_check_control4_cluster_handler(endpoint))
  {
    cluster_write_attr_hook = zb_zcl_get_control4_cluster_write_attr_hook(cluster_id, cluster_role);
  }
  else
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */
  {
    cluster_write_attr_hook = zb_zcl_get_cluster_write_attr_hook(cluster_id, cluster_role);
  }

  return cluster_write_attr_hook;
}


zb_bool_t zb_zcl_handle_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed;
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_handle_specific_commands param %hd", (FMT__H, param));

  {
    zb_zcl_cluster_handler_t cluster_handler;

    cluster_handler = zb_zcl_get_cluster_handler_internal(cmd_info->addr_data.common_data.dst_endpoint,
                                                          cmd_info->cluster_id,
                                                          cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV ?
                                                          ZB_ZCL_CLUSTER_SERVER_ROLE :
                                                          ZB_ZCL_CLUSTER_CLIENT_ROLE);
    if (cluster_handler != NULL)
    {
      processed = cluster_handler(param);
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "Cluster 0x%04x was not processed", (FMT__D, cmd_info->cluster_id));
      processed = ZB_FALSE;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_handle_specific_commands processed %hd", (FMT__H, processed));
  return processed;
}/* zb_uint8_t zb_zcl_handle_specific_commands(zb_uint8_t param) */

#if defined HA_CERTIFICATION
/* During HA certification came out that for some commands which
 * should not be handled (command with direction "to server" is sent
 * to client for example) we need to send a response anyway, but with
 * "empty" body */
zb_bool_t zb_zcl_rejected_command(zb_uint8_t param)
{
  zb_zcl_parsed_hdr_t *cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);
  zb_zcl_cluster_desc_t *cluster_desc;
  zb_af_endpoint_desc_t *endpoint_desc;
  zb_bool_t ret = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, ">> zb_zcl_rejected_command %hd", (FMT__H, param));

  endpoint_desc = zb_af_get_endpoint_desc(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);
  cluster_desc = get_cluster_desc(ep_desc, cmd_info->cluster_id,
                                  (cmd_info->cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV) ?
                                  ZB_ZCL_CLUSTER_SERVER_ROLE : ZB_ZCL_CLUSTER_CLIENT_ROLE);

  ZB_ZCL_DEBUG_DUMP_HEADER(cmd_info);

  /* if command is manufacturer specific - skip handling it by default */
  TRACE_MSG(TRACE_ZCL2, "manuf_spec %hd", (FMT__H, cmd_info->is_manuf_specific));
  if (cluster_desc &&
      !cmd_info->is_manuf_specific &&
      cmd_info->is_common_command)
  {
    switch(cmd_info->cmd_id)
    {
    case ZB_ZCL_CMD_DISCOVER_COMMANDS_RECEIVED:
    case ZB_ZCL_CMD_DISCOVER_COMMANDS_GENERATED:
      ret = zb_zcl_handle_general_commands(param);
      break;

    case ZB_ZCL_CMD_DISCOVER_ATTR_EXT:
    {
      zb_uint8_t *resp_data;
      resp_data = ZB_ZCL_START_PACKET(ZCL_CTX().runtime_buf);

      /* generate frame control taking into account manufacturer specific */
      ZB_ZCL_CONSTRUCT_GENERAL_COMMAND_RESP_FRAME_CONTROL_EXT(
        resp_data, cmd_info->is_manuf_specific, ZB_ZCL_REVERT_DIRECTION(cmd_info->cmd_direction));
      /* Generate command header taking into account manufacturer specific */
      ZB_ZCL_CONSTRUCT_COMMAND_HEADER_EXT(
        resp_data, cmd_info->seq_number, cmd_info->is_manuf_specific, cmd_info->manuf_specific,
        ZB_ZCL_CMD_DISCOVER_ATTR_EXT_RES);
      /* There is the same beginning for both Discover Attr and Discover attr ext Resp */
      ZB_ZCL_PACKET_PUT_DATA8(resp_data, ZB_ZCL_DISC_COMPLETE);

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
      ret = ZB_TRUE;
    }
    break;


    case ZB_ZCL_CMD_DISCOVER_COMMANDS_RECEIVED_RES:
    case ZB_ZCL_CMD_DISCOVER_COMMANDS_GENERATED_RES:
    case ZB_ZCL_CMD_DISCOVER_ATTR_EXT_RES:
      ret = ZB_FALSE;
      break;

    default:
      ret = ZB_FALSE;
      break;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_rejected_command %hd", (FMT__H, ret));
  return ret;
}
#endif /* HA_CERTIFICATION */


void zb_zcl_send_cmd_tsn(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_zcl_frame_direction_t direction,
  zb_uint8_t src_ep,
  const void *payload,
  zb_uint8_t payload_size,
  zb_zcl_put_payload_cb_t put_payload,
  zb_zcl_cluster_id_t cluster_id,
  zb_zcl_disable_default_response_t def_resp,
  zb_uint8_t cmd_id,
  zb_uint8_t tsn,
  zb_callback_t cb
  )
{
  zb_uint16_t profile_id = get_profile_id_by_endpoint(src_ep);
  zb_uint32_t bytes_avail;
#ifdef APS_FRAGMENTATION
  /* zb_uint16_t payload_out_cnt = 3; */
  /* It was static variable initially in SE code. Seems can use a runtime buffer for it. Hope no conflict here. */
  /* zb_uint8_t *payload_temp_out; */
#endif
  zb_uint8_t *cmd_ptr;

  TRACE_MSG(TRACE_ZCL2, "zb_zcl_send_cmd: param %hd (cluster_id 0x%x cmd_id %d)", (FMT__H_D_D, param, cluster_id, cmd_id));
  TRACE_MSG(TRACE_ZCL2, "payload size %hd", (FMT__H, payload_size));
  cmd_ptr = ZB_ZCL_START_PACKET(param);
  ZB_ZCL_CONSTRUCT_SET_FRAME_CONTROL(*cmd_ptr,
                                     ZB_ZCL_FRAME_TYPE_CLUSTER_SPECIFIC,
                                     ZB_ZCL_NOT_MANUFACTURER_SPECIFIC,
                                     direction,
                                     def_resp);
  cmd_ptr++;
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER(cmd_ptr, tsn, cmd_id);

#ifdef APS_FRAGMENTATION
  bytes_avail = (ZB_ASDU_MAX_FRAG_LEN - ZB_ZCL_GET_BYTES_WRITTEN(param, cmd_ptr));
#else
  bytes_avail = (ZB_ZCL_HI_MAX_PAYLOAD_SIZE - ZB_ZCL_GET_BYTES_WRITTEN(param, cmd_ptr));
#endif

  if (bytes_avail < payload_size)
  {
    TRACE_MSG(TRACE_ERROR, "zb_zcl_send_cmd: ERROR, buffer is full (cluster_id %d cmd_id %d)", (FMT__D_D, cluster_id, cmd_id));
    zb_buf_free(param);
    return;
  }
  else
  {
    TRACE_MSG(TRACE_ZCL3, "zb_zcl_send_cmd: buffer have enough space (bytes_avail = %hd)", (FMT__H, bytes_avail));
  }
  if (put_payload != NULL && payload != NULL)
  {
    cmd_ptr = put_payload(cmd_ptr, payload);
  }
  else if (put_payload == NULL && payload != NULL)
  {
    ZB_MEMCPY(cmd_ptr, payload, payload_size);
    cmd_ptr += payload_size;
  }
  else
  {
    /* MISRA rule 15.7 requires empty 'else' branch. */
  }

  TRACE_MSG(TRACE_ZCL3, "zb_zcl_send_cmd addr_type %d to " TRACE_FORMAT_64,
            (FMT__D_A, dst_addr_mode, TRACE_ARG_64(dst_addr->addr_long)));

  ZB_ZCL_FINISH_N_SEND_PACKET(param, cmd_ptr,
                              *dst_addr,
                              dst_addr_mode,
                              dst_ep,
                              src_ep,
                              profile_id,
                              cluster_id,
                              cb);
}

void zb_zcl_send_cmd(
  zb_uint8_t param,
  const zb_addr_u *dst_addr,
  zb_aps_addr_mode_t dst_addr_mode,
  zb_uint8_t dst_ep,
  zb_zcl_frame_direction_t direction,
  zb_uint8_t src_ep,
  const void *payload,
  zb_uint8_t payload_size,
  zb_zcl_put_payload_cb_t put_payload,
  zb_zcl_cluster_id_t cluster_id,
  zb_zcl_disable_default_response_t def_resp,
  zb_uint8_t cmd_id,
  zb_callback_t cb
  )
{
  zb_zcl_send_cmd_tsn(param, dst_addr, dst_addr_mode, dst_ep, direction,
  src_ep, payload, payload_size, put_payload, cluster_id, def_resp, cmd_id, ZB_ZCL_GET_SEQ_NUM(), cb);
}


/* Helper for sending default responses */
zb_bool_t zb_zcl_send_default_handler(zb_uint8_t param,
  const zb_zcl_parsed_hdr_t *cmd_info, zb_zcl_status_t status)
{
  zb_bool_t ret;

  TRACE_MSG(TRACE_ZCL3, ">zb_zcl_send_default_handler param %hd, status: %hd", (FMT__H_H, param, status));

  do
  {
    if (!ZB_ZCL_CHECK_IF_SEND_DEFAULT_RESP(*cmd_info, status))
    {
      ret = ZB_FALSE;
      break;
    }

    ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION(
      param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      cmd_info->seq_number,
      cmd_info->cmd_id,
      status,
      ZB_ZCL_REVERT_DIRECTION(cmd_info->cmd_direction));

    param = 0;
    ret = ZB_TRUE;
  } while (0);

  if (param != 0u)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL3, "<zb_zcl_send_default_handler %d", (FMT__D, ret));
  return ret;
}

void zb_zcl_process_command_finish(zb_bufid_t buffer, zb_zcl_parsed_hdr_t *pcmd_info, zb_uint8_t status)
{
  if (ZB_ZCL_CHECK_IF_SEND_DEFAULT_RESP(*(pcmd_info), status))
  {
    ZB_ZCL_SEND_DEFAULT_RESP(
      (buffer),
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).dst_endpoint,
      (pcmd_info)->profile_id,
      (pcmd_info)->cluster_id,
      (pcmd_info)->seq_number,
      (pcmd_info)->cmd_id,
      (status));
  }
  else
  {
    TRACE_MSG( TRACE_ZCL3, "Default response disabled", (FMT__0));
    zb_buf_free(buffer);
  }
}

void zb_zcl_process_command_finish_new(zb_bufid_t buffer, zb_zcl_parsed_hdr_t *pcmd_info, zb_uint8_t status)
{
  if (ZB_ZCL_CHECK_IF_SEND_DEFAULT_RESP(*(pcmd_info), status))
  {
    ZB_ZCL_SEND_DEFAULT_RESP_NEW(
      (buffer),
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA((pcmd_info)).dst_endpoint,
      (pcmd_info)->profile_id,
      (pcmd_info)->cluster_id,
      (pcmd_info)->seq_number,
      (pcmd_info)->cmd_id,
      (status),
      ZB_APS_FC_IS_SECURE((pcmd_info)->addr_data.common_data.fc));
  }
  else
  {
    TRACE_MSG( TRACE_ZCL3, "Default response disabled", (FMT__0));
    zb_buf_free(buffer);
  }
}

void zb_zcl_send_default_resp_ext(zb_uint8_t param,
                                  const zb_zcl_parsed_hdr_t *cmd_info, zb_zcl_status_t status)
{
  if (ZB_ZCL_CHECK_IF_SEND_DEFAULT_RESP(*cmd_info, status))
  {
    ZB_ZCL_SEND_DEFAULT_RESP_EXT(
      param,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
      cmd_info->profile_id,
      cmd_info->cluster_id,
      cmd_info->seq_number,
      cmd_info->cmd_id,
      status,
      ZB_ZCL_REVERT_DIRECTION(cmd_info->cmd_direction),
      cmd_info->is_manuf_specific,
      cmd_info->manuf_specific,
      NULL);
  }
  else
  {
    zb_buf_free(param);
  }
}

#endif /* defined (ZB_ENABLE_ZCL) */
