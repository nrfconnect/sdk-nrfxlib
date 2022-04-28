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
/* PURPOSE: ZCL OTA Upgrade cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 46

#include "zb_common.h"

/** @internal
    @{
*/

#if defined (ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE) || defined DOXYGEN

#if defined ZB_HA_ENABLE_OTA_UPGRADE_SERVER || defined DOXYGEN

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_ota_upgrade.h"
#include "zb_zdo.h"
#include "zb_aps.h"
#include "zdo_wwah_stubs.h"

static zb_uint8_t gs_ota_upgrade_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_OTA_UPGRADE_SERVER_ROLE_RECEIVED_CMD_LIST
};

static zb_uint8_t gs_ota_upgrade_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_OTA_UPGRADE_CLIENT_ROLE_RECEIVED_CMD_LIST
};


static zb_discover_cmd_list_t gs_ota_upgrade_server_cmd_list =
{
  sizeof(gs_ota_upgrade_server_received_commands), gs_ota_upgrade_server_received_commands,
  sizeof(gs_ota_upgrade_client_received_commands), gs_ota_upgrade_client_received_commands
};

static zb_bool_t zb_zcl_process_ota_upgrade_specific_commands_srv(zb_uint8_t param);

void zb_zcl_ota_upgrade_init_srv()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ota_upgrade_specific_commands_srv);
}

static zb_ieee_addr_t zb_zcl_ota_upgrade_def_address = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static zb_ret_t zcl_ota_upgrade_srv_query_img_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr,
  zb_uint16_t image_type, zb_uint16_t manufacturer_code, zb_uint32_t version, zb_uint8_t *table_idx);
static void zcl_ota_upgrade_srv_upgrade_started_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr,
  zb_uint16_t image_type, zb_uint32_t version);
static void zcl_ota_upgrade_srv_upgrade_aborted_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr,
  zb_uint16_t image_type, zb_uint32_t version);
static zb_ret_t zcl_ota_upgrade_srv_upgrade_end_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr, zb_uint8_t status,
  zb_uint16_t image_type, zb_uint32_t version, zb_uint32_t* upgrade_time);

/******************************** Helper function ****************************************/

#define ZB_ZCL_OTA_FILE_DST_ADDR(header)              \
   ( (((header)->fc) & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_DEVICE_SPECIFIC) \
      ? (zb_uint8_t*)(header) + sizeof(zb_zcl_ota_upgrade_file_header_t) +                             \
          (( ((header)->fc) & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_CREDENTIAL_VER) ? sizeof(zb_uint8_t): 0)  \
      : zb_zcl_ota_upgrade_def_address)

static zb_uint16_t zb_zcl_ota_file_min_hw_version(zb_zcl_ota_upgrade_file_header_t *header)
{
  zb_uint8_t *ptr = (zb_uint8_t*)header;
  zb_uint16_t tmp_ptr_val = 0;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_file_min_hw_version %p", (FMT__P, header));

  ZB_ASSIGN_UINT16(&tmp_ptr_val, ( ptr + sizeof(zb_zcl_ota_upgrade_file_header_t)
            + ((header->fc & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_CREDENTIAL_VER) ? sizeof(zb_uint8_t): 0)
            + ((header->fc & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_DEVICE_SPECIFIC) ? sizeof(zb_ieee_addr_t): 0)));

  return (header->fc & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_HW_VER)
        ? tmp_ptr_val
        : 0;
}

static zb_uint16_t zb_zcl_ota_file_max_hw_version(zb_zcl_ota_upgrade_file_header_t *header)
{
  zb_uint8_t *ptr = (zb_uint8_t*)header;
  zb_uint16_t tmp_ptr_val = 0;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_file_max_hw_version %p", (FMT__P, header));

  ZB_ASSIGN_UINT16(&tmp_ptr_val, ( ptr + sizeof(zb_zcl_ota_upgrade_file_header_t)
            + ((header->fc & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_CREDENTIAL_VER) ? sizeof(zb_uint8_t): 0)
            + ((header->fc & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_DEVICE_SPECIFIC) ? sizeof(zb_ieee_addr_t): 0)
            + sizeof(zb_uint16_t)));

  return (header->fc & ZB_ZCL_OTA_UPGRADE_FILE_HEADER_FC_HW_VER)
        ? tmp_ptr_val
        : 0xffff;
}

static zb_zcl_ota_upgrade_server_variable_t * get_upgrade_server_variables(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_OTA_UPGRADE_SERVER_DATA_ID);
  ZB_ASSERT(attr_desc);
  return (zb_zcl_ota_upgrade_server_variable_t*)attr_desc->data_p;
}

/******************************** App function ****************************************/

/* public API */
void zb_zcl_ota_upgrade_init_server(zb_uint8_t endpoint, zb_zcl_ota_upgrade_next_data_ind_t next_data_ind_cb)
{
  zb_zcl_ota_upgrade_server_variable_t *vars;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_init_server %hx", (FMT__H, endpoint));

  vars = get_upgrade_server_variables(endpoint);

  for(i=0; i<vars->table_length; i++)
  {
    vars->table[i].file_header = NULL;
  }

  ZB_ASSERT(next_data_ind_cb);

  ZCL_CTX().ota_upgrade_next_data_ind_cb = next_data_ind_cb;

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_init_server", (FMT__0));
}

/* public API */
zb_ret_t zb_zcl_ota_upgrade_insert_file(zb_uint8_t param)
{
  zb_zcl_ota_upgrade_server_insert_file_t* insert_data =
      ZB_BUF_GET_PARAM(param, zb_zcl_ota_upgrade_server_insert_file_t);
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_server_variable_t *vars;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_insert_file %hx", (FMT__H, param));

  endpoint = insert_data->endpoint;

  vars = get_upgrade_server_variables(endpoint);

  TRACE_MSG(TRACE_ZCL2, "index %hx", (FMT__H, insert_data->index));
  if(insert_data->index < vars->table_length)
  {
    zb_zcl_ota_upgrade_file_header_t *head = (zb_zcl_ota_upgrade_file_header_t*)insert_data->ota_file;

    // Fill table
    vars->table[insert_data->index].file_header = head;
    vars->table[insert_data->index].upgrade_time = insert_data->upgrade_time;

    TRACE_MSG(TRACE_ZCL2, "index = %d 0x%x", (FMT__H_P, insert_data->index, vars->table[insert_data->index].file_header));

    if (insert_data->notify_on)
    {
      // Send Image Notify
      if(ZB_ZCL_OTA_FILE_IS_FILE_SPECIFIC(head))
      {
        zb_uint16_t dest_addr;
        zb_ieee_addr_t dest_ieee;

        ZB_IEEE_ADDR_COPY(dest_ieee, ZB_ZCL_OTA_FILE_DST_ADDR(head));
        zb_inverse_bytes(dest_ieee,sizeof(zb_ieee_addr_t));

        dest_addr = zb_address_short_by_ieee(dest_ieee);

        TRACE_MSG(TRACE_ZCL2, "short 0x%x long "TRACE_FORMAT_64"", (FMT__D_A,
            dest_addr, TRACE_ARG_64(ZB_ZCL_OTA_FILE_DST_ADDR(head))));
        ZB_ZCL_OTA_UPGRADE_SEND_IMAGE_NOTIFY_REQ(param,
            dest_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT, ZB_ZCL_BROADCAST_ENDPOINT,
            endpoint, ZB_AF_HA_PROFILE_ID, ZB_TRUE, NULL,
            ZB_ZCL_OTA_UPGRADE_IMAGE_NOTIFY_PAYLOAD_JITTER_CODE_IMAGE_VER,
            *(vars->query_jitter),
            head->manufacturer_code,
            head->image_type,
            head->file_version);
      }
      else // broadcast
      {
        zb_uint16_t short_addr = ZB_NWK_BROADCAST_ALL_DEVICES;
        ZB_ZCL_OTA_UPGRADE_SEND_IMAGE_NOTIFY_REQ(param,
            short_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT, ZB_ZCL_BROADCAST_ENDPOINT,
            endpoint, ZB_AF_HA_PROFILE_ID, ZB_TRUE, NULL,
            ZB_ZCL_OTA_UPGRADE_IMAGE_NOTIFY_PAYLOAD_JITTER_CODE_IMAGE_VER,
            *(vars->query_jitter),
            head->manufacturer_code,
            head->image_type,
            head->file_version);
      }
    }
    else
    {
      zb_buf_free(param);
    }
  }
  else
  {
    ret = RET_INVALID_PARAMETER_1;
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_insert_file ret %hx", (FMT__H, ret));

  return ret;
}

/* public API */
zb_ret_t zb_zcl_ota_upgrade_remove_file(zb_uint8_t param)
{
  zb_zcl_ota_upgrade_server_remove_file_t* remove_data =
      ZB_BUF_GET_PARAM(param, zb_zcl_ota_upgrade_server_remove_file_t);
  zb_ret_t ret = RET_OK;

  zb_zcl_ota_upgrade_server_variable_t *vars = get_upgrade_server_variables(remove_data->endpoint);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ota_upgrade_remove_file %hx", (FMT__H, param));

  if(remove_data->index < vars->table_length)
  {
    vars->table[remove_data->index].file_header = 0;
  }
  else
  {
    ret = RET_INVALID_PARAMETER_1;
  }

  zb_buf_free(param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ota_upgrade_remove_file ret %hx", (FMT__H, ret));

  return ret;
}

void zb_zcl_ota_upgrade_send_query_next_image_response(zb_zcl_parsed_hdr_t *zcl_parsed_hdr, zb_bool_t image_present, zb_uindex_t table_idx)
{
  zb_uint8_t endpoint;
  zb_zcl_ota_upgrade_server_variable_t *vars;
  zb_bufid_t param;

  TRACE_MSG(TRACE_ZCL1,
            ">zb_zcl_ota_upgrade_send_query_next_image_response, image_presend %d, index %d",
            (FMT__D_D, image_present, table_idx));

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).dst_endpoint;
  vars = get_upgrade_server_variables(endpoint);

  param = zb_buf_get_out();

  if (param)
  {
    if (image_present)
    {
      if(table_idx < vars->table_length)
      {
        TRACE_MSG(TRACE_ZCL1, "Found OTA file ver 0x%x",
                  (FMT__L, vars->table[table_idx].file_header->file_version));

        ZB_ZCL_OTA_UPGRADE_SEND_QUERY_NEXT_IMAGE_RES(
          param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).dst_endpoint,
          ZB_AF_HA_PROFILE_ID, zcl_parsed_hdr->seq_number,
          ZB_ZCL_STATUS_SUCCESS,
          vars->table[table_idx].file_header->manufacturer_code,
          vars->table[table_idx].file_header->image_type,
          vars->table[table_idx].file_header->file_version,
          vars->table[table_idx].file_header->total_image_size);
      }
      else
      {
        TRACE_MSG(TRACE_ERROR, "incorrect table_idx: %d", (FMT__D, table_idx));
      }
    }
    else
    {
      ZB_ZCL_OTA_UPGRADE_SEND_QUERY_NEXT_IMAGE_RES(
        param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_parsed_hdr).dst_endpoint,
        ZB_AF_HA_PROFILE_ID, zcl_parsed_hdr->seq_number,
        ZB_ZCL_STATUS_NO_IMAGE_AVAILABLE,
        0, 0, 0, 0);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "failed to get out buf, response won't be sent", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "<zb_zcl_ota_upgrade_send_query_next_image_response", (FMT__0));
}

/** @brief Query Next Image Request command */
static zb_ret_t query_next_image_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_NOT_FOUND;
  zb_zcl_ota_upgrade_query_next_image_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> query_next_image_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_QUERY_NEXT_IMAGE_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_QUERY_NEXT_IMAGE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t i = 0U;
    zb_uint8_t endpoint;
    zb_zcl_ota_upgrade_server_variable_t *vars;

    endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    vars = get_upgrade_server_variables(endpoint);
    ret = zcl_ota_upgrade_srv_query_img_invoke_user_app(
      param, &ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source,
      payload.image_type, payload.manufacturer, payload.file_version, &i);

    if (ret == RET_NOT_FOUND)
    {
      /* No user callback, try to search in table (old-style) */
      for(i=0; i<vars->table_length; i++)
      {
        if(vars->table[i].file_header)
        {
          TRACE_MSG(TRACE_ZCL2, "File %d %p", (FMT__H_P, i, vars->table[i].file_header));
          if( payload.manufacturer == vars->table[i].file_header->manufacturer_code &&
              payload.image_type == vars->table[i].file_header->image_type &&
              ZB_ZCL_OTA_UPGRADE_VERSION_CMP(
                vars->table[i].file_header->file_version, payload.file_version) &&
              //ZB_ZCL_OTA_FILE_IS_FILE_SPECIFIC(vars->table[i].file_header)==ZB_FALSE &&
              // hw version
              ( !(payload.fc & ZB_ZCL_OTA_UPGRADE_QUERY_NEXT_IMAGE_HW_VERSION) ||
                ( payload.hw_version >= zb_zcl_ota_file_min_hw_version(vars->table[i].file_header) &&
                  payload.hw_version <= zb_zcl_ota_file_max_hw_version(vars->table[i].file_header) )
                ) )
          {
            ret = RET_OK;
            break;
          }
        }
      }
    }

    if (ret == RET_OK && i < vars->table_length)
    {
      TRACE_MSG(TRACE_ZCL1, "Found OTA file ver 0x%x", (FMT__L, vars->table[i].file_header->file_version));
      zb_zcl_ota_upgrade_send_query_next_image_response(&cmd_info, ZB_TRUE, i);
    }
    else if (ret == RET_BLOCKED)
    {
      TRACE_MSG(TRACE_ZCL1, "Application will call zb_zcl_upgrade_send_query_next_image_response later",
                (FMT__0));
    }
    else
    {
      zb_zcl_ota_upgrade_send_query_next_image_response(&cmd_info, ZB_FALSE, 0);
    }
    ret = RET_BUSY;
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< query_next_image_handler ret %hx", (FMT__H, ret));
  return ret;
}

void zb_zcl_ota_upgrade_send_image_block_response(zb_zcl_parsed_hdr_t *zcl_header, zb_uindex_t table_idx,
                                                  zb_bool_t block_present,
                                                  zb_uint32_t offset, zb_uint8_t data_size,
                                                  const zb_uint8_t* data)
{
  zb_bufid_t param;
  zb_uint8_t endpoint;
  zb_zcl_ota_upgrade_server_variable_t *vars;


  TRACE_MSG(TRACE_ZCL1, ">zb_zcl_ota_upgrade_send_image_block_response", (FMT__0));
  TRACE_MSG(TRACE_ZCL1, "table_idx %d, block_present %d, offset %d, data_size %d",
            (FMT__D_D_D_D, table_idx, block_present, offset, data_size));

  param = zb_buf_get_out();

  if (param)
  {
    endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).dst_endpoint;
    vars = get_upgrade_server_variables(endpoint);
    if (block_present)
    {
      ZB_ZCL_OTA_UPGRADE_SEND_IMAGE_BLOCK_SUCCESS_RES(
        param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).dst_endpoint,
        ZB_AF_HA_PROFILE_ID, zcl_header->seq_number,
        vars->table[table_idx].file_header->manufacturer_code,
        vars->table[table_idx].file_header->image_type,
        vars->table[table_idx].file_header->file_version,
        offset,
        data_size,
        data
        );
    }
    else
    {
      /* ZCL8 spec, 11.13.6.5.2 No Image Available
         If either manufacturer code or image type or file version information in the request command is invalid or the OTA
         upgrade file for the client for some reason has disappeared which result in the server no longer able to retrieve the file,
         it SHALL send default response command with NO_IMAGE_AVAILABLE status to the client.
      */
      ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION(
        param,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).source.u.short_addr,
        ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(zcl_header).dst_endpoint,
        zcl_header->profile_id,
        ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
        zcl_header->seq_number,
        zcl_header->cmd_id,
        ZB_ZCL_STATUS_NO_IMAGE_AVAILABLE,
        (ZB_ZCL_FRAME_DIRECTION_TO_CLI == zcl_header->cmd_direction ?
         ZB_ZCL_FRAME_DIRECTION_TO_SRV :
         ZB_ZCL_FRAME_DIRECTION_TO_CLI));
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "Failed to get out buf, response won't be sent", (FMT__0));
  }

  TRACE_MSG(TRACE_ZCL1, "<zb_zcl_ota_upgrade_send_image_block_response", (FMT__0));
}

/** @brief Image Block Request command */
static zb_ret_t image_block_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_image_block_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_parsed_hdr_t *cmd_info_p = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG(TRACE_ZCL1, "> image_block_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, cmd_info_p, sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_IMAGE_BLOCK_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_ota_upgrade_server_variable_t *vars = get_upgrade_server_variables(endpoint);
    zb_uindex_t i;

    for(i=0; i<vars->table_length; i++)
    {
      if(vars->table[i].file_header)
      {
        if( payload.manufacturer == vars->table[i].file_header->manufacturer_code &&
            payload.image_type == vars->table[i].file_header->image_type &&
            vars->table[i].file_header->file_version == payload.file_version )
        {
          zb_uint32_t total_image_size = vars->table[i].file_header->total_image_size;
          if( payload.file_offset < total_image_size)
          {
            zb_uint8_t *data = NULL;
            zb_uint8_t  data_size =
                (payload.data_size_max <= ZB_ZCL_OTA_UPGRADE_QUERY_IMAGE_BLOCK_DATA_SIZE_MAX) ?
                payload.data_size_max :
                ZB_ZCL_OTA_UPGRADE_QUERY_IMAGE_BLOCK_DATA_SIZE_MAX;

            if(data_size > total_image_size - payload.file_offset)
            {
              data_size = total_image_size - payload.file_offset;
            }

            ret = ZCL_CTX().ota_upgrade_next_data_ind_cb(i, cmd_info_p, payload.file_offset, data_size, &data);

            if (payload.file_offset == 0)
            {
              if (ret == RET_BLOCKED
                  || (ret ==  RET_OK && data) )
              {
                /* Will hope RET_BLOCKED results means data will be sent later */
                zcl_ota_upgrade_srv_upgrade_started_invoke_user_app(
                  param, &ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source,
                  payload.image_type, payload.file_version);
              }
            }

            if (ret == RET_OK
                && data)
            {
                zb_zcl_ota_upgrade_send_image_block_response(cmd_info_p, i, ZB_TRUE, payload.file_offset,
                                                             data_size, data);
                zb_buf_free(param);
              ret = RET_BUSY;
            }
            else if (ret == RET_BLOCKED)
            {
              TRACE_MSG(TRACE_ZCL1, "Application will call zb_zcl_ota_upgrade_send_image_block_response later",
                        (FMT__0));
              ret = RET_BUSY;
              zb_buf_free(param);
            }
          }
          break;
        }
      }
    }
    if (ret != RET_BUSY)
    {
      zcl_ota_upgrade_srv_upgrade_aborted_invoke_user_app(
        param, &ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source,
        payload.image_type, payload.file_version);

      zb_zcl_ota_upgrade_send_image_block_response(cmd_info_p, 0, ZB_FALSE, 0, 0, NULL);
      zb_buf_free(param);

      ret = RET_BUSY;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< image_block_handler ret %hx", (FMT__H, ret));
  return ret;
}

#if 0
/** @brief Image Page Request command */
static zb_ret_t image_page_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_image_page_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> image_page_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_IMAGE_PAGE_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_IMAGE_PAGE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_ota_upgrade_server_variable_t *vars = get_upgrade_server_variables(endpoint);
    zb_uindex_t index;

    //todo
      ret = RET_BUSY;
  }

  TRACE_MSG(TRACE_ZCL1, "< image_page_handler ret %hx", (FMT__H, ret));
  return ret;
}
#endif /*0*/

/** @brief Upgrade End Request command */
static zb_ret_t upgrade_end_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_upgrade_end_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint32_t upgrade_time = ZB_ZCL_OTA_UPGRADE_UPGRADE_TIME_DEF_VALUE;

  TRACE_MSG(TRACE_ZCL1, "> upgrade_end_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_UPGRADE_END_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_UPGRADE_END_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_ota_upgrade_server_variable_t *vars = get_upgrade_server_variables(endpoint);
    zb_uindex_t i = vars->table_length;

    if(payload.status==ZB_ZCL_STATUS_SUCCESS)
    {
      for(i=0; i<vars->table_length; i++)
      {
        if(vars->table[i].file_header)
        {
          if( payload.manufacturer == vars->table[i].file_header->manufacturer_code &&
              payload.image_type == vars->table[i].file_header->image_type &&
              vars->table[i].file_header->file_version == payload.file_version )
          {
            upgrade_time = vars->table[i].upgrade_time;
            break;
          }
        }
      }
    }

    /* pass the pointer to upgrade time field to give an application the means of setting upgrade time
       FIXME: refactor this command and the whole cluster using IN/OUT params API */
    /*ret = */
    zcl_ota_upgrade_srv_upgrade_end_invoke_user_app(
      param, &ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source, payload.status,
      payload.image_type, payload.file_version, &upgrade_time);

    if(payload.status==ZB_ZCL_STATUS_SUCCESS)
    {
      if (i < vars->table_length)
      {
	      vars->table[i].upgrade_time = upgrade_time;
      }
      else
      {
        ret = RET_ERROR;
      }
    }

    if (ret != RET_ERROR)
    {
      if (payload.status==ZB_ZCL_STATUS_SUCCESS)
      {
        ZB_ZCL_OTA_UPGRADE_SEND_UPGRADE_END_RES(
          param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          ZB_AF_HA_PROFILE_ID, cmd_info.seq_number,
          payload.manufacturer,
          payload.image_type,
          payload.file_version,
          *(vars->current_time),
          vars->table[i].upgrade_time);
      }
      else
      {
        ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION(
          param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.profile_id,
          ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
          cmd_info.seq_number,
          cmd_info.cmd_id,
          ZB_ZCL_STATUS_SUCCESS,
          (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction ?
            ZB_ZCL_FRAME_DIRECTION_TO_SRV :
            ZB_ZCL_FRAME_DIRECTION_TO_CLI));
      }
      ret = RET_BUSY;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< upgrade_end_handler ret %hx", (FMT__H, ret));
  return ret;
}

/** @brief Query Specific File command */
static zb_ret_t query_specific_file_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ota_upgrade_query_specific_file_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> query_specific_file_handler %hx", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_OTA_UPGRADE_GET_QUERY_SPECIFIC_FILE_REQ(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_OTA_UPGRADE_GET_QUERY_SPECIFIC_FILE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_ota_upgrade_server_variable_t *vars = get_upgrade_server_variables(endpoint);
    zb_uindex_t i;

    for(i=0; i<vars->table_length; i++)
    {
      TRACE_MSG(TRACE_ZCL2, "index = %d", (FMT__H, i));
      if(vars->table[i].file_header)
      {
        TRACE_MSG(TRACE_ZCL2, "manuf %d long "TRACE_FORMAT_64"", (FMT__D_A,
            vars->table[i].file_header->manufacturer_code,
            TRACE_ARG_64(ZB_ZCL_OTA_FILE_DST_ADDR(vars->table[i].file_header))));

        TRACE_MSG(TRACE_ZCL2, "if %d %d %d %d %d", (FMT__H_H_H_H_H,
            ZB_IEEE_ADDR_CMP(ZB_ZCL_OTA_FILE_DST_ADDR(vars->table[i].file_header), payload.node_addr),
            payload.manufacturer == vars->table[i].file_header->manufacturer_code,
            payload.image_type == vars->table[i].file_header->image_type,
            ZB_ZCL_OTA_UPGRADE_VERSION_CMP(
                 vars->table[i].file_header->file_version, payload.file_version),
            payload.stack_version == vars->table[i].file_header->stack_version ) );

        TRACE_MSG(TRACE_ZCL2, "file value "TRACE_FORMAT_64" payload value "TRACE_FORMAT_64"", (FMT__A_A,
            TRACE_ARG_64(ZB_ZCL_OTA_FILE_DST_ADDR(vars->table[i].file_header)), TRACE_ARG_64(payload.node_addr)));

        if( ZB_IEEE_ADDR_CMP(ZB_ZCL_OTA_FILE_DST_ADDR(vars->table[i].file_header), payload.node_addr) &&
            payload.manufacturer == vars->table[i].file_header->manufacturer_code &&
            payload.image_type == vars->table[i].file_header->image_type &&
            ZB_ZCL_OTA_UPGRADE_VERSION_CMP(
                 vars->table[i].file_header->file_version, payload.file_version) &&
            payload.stack_version == vars->table[i].file_header->stack_version )
        {
          ZB_ZCL_OTA_UPGRADE_SEND_QUERY_SPECIFIC_FILE_RES(param,
              ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
              ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
              ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
              ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
              ZB_AF_HA_PROFILE_ID, cmd_info.seq_number,
              ZB_ZCL_STATUS_SUCCESS,
              vars->table[i].file_header->manufacturer_code,
              vars->table[i].file_header->image_type,
              vars->table[i].file_header->file_version,
              vars->table[i].file_header->total_image_size);

          ret = RET_BUSY;
          break;
        }
      }
    }
    if(ret!=RET_BUSY)
    {
      ZB_ZCL_OTA_UPGRADE_SEND_QUERY_SPECIFIC_FILE_RES(param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          ZB_AF_HA_PROFILE_ID, cmd_info.seq_number,
          ZB_ZCL_STATUS_NO_IMAGE_AVAILABLE,
          0, 0, 0, 0);

      ret = RET_BUSY;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< query_specific_file_handler ret %hx", (FMT__H, ret));
  return ret;
}


static zb_ret_t zcl_ota_upgrade_srv_query_img_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr,
  zb_uint16_t image_type, zb_uint16_t manufacturer_code, zb_uint32_t version, zb_uint8_t *table_idx)
{
  zb_ret_t ret;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zcl_ota_upgrade_srv_query_img_invoke_user_app, param %hd",
            (FMT__H, param));
  ret = RET_NOT_FOUND;
  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  if (ZCL_CTX().device_cb)
  {
    zb_zcl_device_callback_param_t *user_app_data;
    zb_zcl_ota_upgrade_srv_query_img_param_t *value;

    user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    value = &user_app_data->cb_param.ota_upgrade_srv_query_img_param;
    value->zcl_addr = zcl_addr;
    value->image_type = image_type;
    value->manufacturer_code = manufacturer_code;
    value->version = version;
    value->table_idx = table_idx;
    /* we need to save the ZCL header to send a response later using the same buffer */
    ZB_MEMCPY(&value->zcl_parsed_hdr, &cmd_info, sizeof(zb_zcl_parsed_hdr_t));
    user_app_data->device_cb_id = ZB_ZCL_OTA_UPGRADE_SRV_QUERY_IMAGE_CB_ID;
    user_app_data->endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_SERVER_ROLE);
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
    ret = user_app_data->status;
  }
  TRACE_MSG(TRACE_ZCL1, "< zcl_ota_upgrade_srv_query_img_invoke_user_app, ret %hd",
            (FMT__H, ret));
  return ret;
}


static void zcl_ota_upgrade_srv_upgrade_started_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr,
  zb_uint16_t image_type, zb_uint32_t version)
{
  TRACE_MSG(TRACE_ZCL1, "> zcl_ota_upgrade_srv_upgrade_started_invoke_user_app, param %hd",
            (FMT__H, param));

  if (ZCL_CTX().device_cb)
  {
    zb_zcl_device_callback_param_t *user_app_data;
    zb_zcl_ota_upgrade_srv_upgrade_started_param_t *value;

    user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    value = &user_app_data->cb_param.ota_upgrade_srv_upgrade_started_param;
    value->zcl_addr = zcl_addr;
    value->image_type = image_type;
    value->version = version;
    user_app_data->device_cb_id = ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_STARTED_CB_ID;
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
  }
  TRACE_MSG(TRACE_ZCL1, "< zcl_ota_upgrade_srv_upgrade_started_invoke_user_app",
            (FMT__0));
}


static void zcl_ota_upgrade_srv_upgrade_aborted_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr,
  zb_uint16_t image_type, zb_uint32_t version)
{
    TRACE_MSG(TRACE_ZCL1, "> zcl_ota_upgrade_srv_upgrade_aborted_invoke_user_app, param %hd",
            (FMT__H, param));

  if (ZCL_CTX().device_cb)
  {
    zb_zcl_device_callback_param_t *user_app_data;
    zb_zcl_ota_upgrade_srv_upgrade_aborted_param_t *value;

    user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    value = &user_app_data->cb_param.ota_upgrade_srv_upgrade_aborted_param;
    value->zcl_addr = zcl_addr;
    value->image_type = image_type;
    value->version = version;
    user_app_data->device_cb_id = ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_ABORTED_CB_ID;
    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
  }
  TRACE_MSG(TRACE_ZCL1, "< zcl_ota_upgrade_srv_upgrade_aborted_invoke_user_app",
            (FMT__0));
}

static zb_ret_t zcl_ota_upgrade_srv_upgrade_end_invoke_user_app(
  zb_uint8_t param, zb_zcl_addr_t *zcl_addr, zb_uint8_t status,
  zb_uint16_t image_type, zb_uint32_t version, zb_uint32_t *upgrade_time)
{
  zb_ret_t ret;

  TRACE_MSG(TRACE_ZCL1, "> zcl_ota_upgrade_srv_upgrade_end_invoke_user_app, param %hd",
            (FMT__H, param));
  ret = RET_NOT_FOUND;
  if (ZCL_CTX().device_cb)
  {
    zb_zcl_device_callback_param_t *user_app_data;
    zb_zcl_ota_upgrade_srv_upgrade_end_param_t *value;

    user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    value = &user_app_data->cb_param.ota_upgrade_srv_upgrade_end_param;
    value->zcl_addr = zcl_addr;
    value->status = status;
    value->image_type = image_type;
    value->version = version;
    value->upgrade_time = upgrade_time;
    user_app_data->device_cb_id = ZB_ZCL_OTA_UPGRADE_SRV_UPGRADE_END_CB_ID;
    user_app_data->endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE, ZB_ZCL_CLUSTER_CLIENT_ROLE);
    (ZCL_CTX().device_cb)(param);
    ret = user_app_data->status;
  }
  TRACE_MSG(TRACE_ZCL1, "< zcl_ota_upgrade_srv_upgrade_end_invoke_user_app, ret %hd",
            (FMT__H, ret));
  return ret;
}


static zb_bool_t zb_zcl_process_ota_srv_upgrade_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_ota_upgrade_specific_commands: param %d, cmd %d",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_OTA_UPGRADE == cmd_info.cluster_id);

  switch( cmd_info.cmd_id )
  {
  case ZB_ZCL_CMD_OTA_UPGRADE_QUERY_NEXT_IMAGE_ID:
    ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);
    status = query_next_image_handler(param);
    break;

  case ZB_ZCL_CMD_OTA_UPGRADE_IMAGE_BLOCK_ID:
    ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);
    status = image_block_handler(param);
    break;
/*  // not release yet
  case ZB_ZCL_CMD_OTA_UPGRADE_IMAGE_PAGE_ID:
    ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);
    status = image_page_handler(param);
    break;
*/
  case ZB_ZCL_CMD_OTA_UPGRADE_UPGRADE_END_ID:
    ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);
    status = upgrade_end_handler(param);
    break;

  case ZB_ZCL_CMD_OTA_UPGRADE_QUERY_SPECIFIC_FILE_ID:
    ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);
    status = query_specific_file_handler(param);
    break;

    /* Called from zb_zcl_handle_default_response_commands->zb_zcl_process_ota_upgrade_default_response_commands->zb_zcl_handle_general_commands */
  case ZB_ZCL_CMD_DEFAULT_RESP:
    /* This is not a normall call to specific commands processing.
       This is a replacement of direct call from zb_zcl_handle_default_response_commands().
       It made to exclude OTA code linking if OTA cluster is not declared by the app.
     */
    return (zb_bool_t)zb_zcl_process_ota_upgrade_default_response_commands(param);
    break;

  default:
    processed = ZB_FALSE;
    break;
  }

  if( processed )
  {
    if( (ZB_NWK_IS_ADDRESS_BROADCAST(cmd_info.addr_data.common_data.dst_addr) ||
         (cmd_info.disable_default_response)) && status == RET_OK)
    {
      TRACE_MSG( TRACE_ZCL3, "Default response disabled", (FMT__0));
      zb_buf_free(param);
    }
    else if (status != RET_BUSY)
    {
       ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION( param,
               ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
               ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
               ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
               ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
               cmd_info.profile_id,
               ZB_ZCL_CLUSTER_ID_OTA_UPGRADE,
               cmd_info.seq_number,
               cmd_info.cmd_id,
               (status==RET_OK ? ZB_ZCL_STATUS_SUCCESS :
                  (status==RET_INVALID_PARAMETER_2 ? ZB_ZCL_STATUS_MALFORMED_CMD :
                        ZB_ZCL_STATUS_INVALID_FIELD)),
              (ZB_ZCL_FRAME_DIRECTION_TO_CLI == cmd_info.cmd_direction ?
                      ZB_ZCL_FRAME_DIRECTION_TO_SRV :
                      ZB_ZCL_FRAME_DIRECTION_TO_CLI));
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_ota_upgrade_specific_commands: processed %d",
             (FMT__D, processed));
  return processed;
}

zb_bool_t zb_zcl_process_ota_upgrade_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ota_upgrade_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_ota_srv_upgrade_specific_commands(param);
}

/**
 *  @} internal
*/

#endif /* defined ZB_HA_ENABLE_OTA_UPGRADE_SERVER || defined DOXYGEN */

#endif  /* ZB_ZCL_SUPPORT_CLUSTER_OTA_UPGRADE */
