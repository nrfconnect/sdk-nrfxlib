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
/* PURPOSE: ZCL Scenes cluster specific commands handling and service functions
*/

#define ZB_TRACE_FILE_ID 2081

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_SCENES)

#include "zb_bufpool.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_scenes.h"
#include "zb_zdo.h"
#include "zb_aps.h"

zb_uint8_t gs_scenes_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_SCENES_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_scenes_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_SCENES_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_scenes_client_cmd_list =
{
  sizeof(gs_scenes_client_received_commands), gs_scenes_client_received_commands,
  sizeof(gs_scenes_server_received_commands), gs_scenes_server_received_commands
};

zb_discover_cmd_list_t gs_scenes_server_cmd_list =
{
  sizeof(gs_scenes_server_received_commands), gs_scenes_server_received_commands,
  sizeof(gs_scenes_client_received_commands), gs_scenes_client_received_commands
};

zb_bool_t zb_zcl_process_scenes_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_scenes_specific_commands_cli(zb_uint8_t param);
zb_ret_t check_value_scenes(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

void zb_zcl_scenes_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_SCENES,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_scenes,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_scenes_specific_commands_srv);
}

void zb_zcl_scenes_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_SCENES,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              check_value_scenes,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_scenes_specific_commands_cli);
}

zb_ret_t check_value_scenes(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID:
  {
#ifdef ZB_NEED_ALIGN
    zb_uint16_t v;
    ZB_MEMCPY(&v, value, 2);
    ret = (v <= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MAX_VALUE) ? RET_OK : RET_OUT_OF_RANGE;
#else
    ret = (*((zb_uint16_t *)value) <= ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_MAX_VALUE) ? RET_OK : RET_OUT_OF_RANGE;
#endif
      break;
  }

    case ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID:
      ret = ( ZB_ZCL_CHECK_BOOL_VALUE(*value) ) ? RET_OK : RET_ERROR;
      break;

    case ZB_ZCL_ATTR_SCENES_NAME_SUPPORT_ID:
      /* Only most significant bit is meaningful, see ZCL spec, subclause
       3.7.2.2.1.5 */
      /* Acc. to ZCL spec, subclause 1.6.2 Reserved Bit Fields, reserved fields
       * should be ignored. These fields are not marked as reserved directly,
       * but aren't they de-facto reserved? */
      ret = ZB_TRUE;
      break;

    case ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID:
      if( ZB_ZCL_ATTR_GET16(value) > ZB_ZCL_SCENES_CLUSTER_REVISION_MAX )
      {
        ret = RET_ERROR;
      }
      break;  

    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_scenes ret %hd", (FMT__H, ret));
  return ret;
}
/** @internal @brief Processes Store scene command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_store_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info);

/** @internal @brief Processes Add scene command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_add_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info, zb_bool_t is_enhanced);

/** @internal @brief Processes View scene command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_view_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info, zb_bool_t is_enhanced);

/** @internal @brief Processes Remove scene command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_remove_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info);

/** @internal @brief Processes Remove all scenes command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_remove_all_scenes_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info);

/** @internal @brief Processes Get scene membership command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_get_scene_membership_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info);

/** @internal @brief Processes Recall scene command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_recall_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info);

#ifndef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
/** @internal @brief Processes Copy Scene command
    @param param - reference number of the command buffer
*/
static void zb_zcl_scenes_process_copy_scene(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info);
#endif /* !ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */

zb_bool_t zb_zcl_process_scenes_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_process_scenes_specific_commands: param %d, cmd %d",
      (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_SCENES == cmd_info.cluster_id);

  if (ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction)
  {
    switch (cmd_info.cmd_id)
    {
      case ZB_ZCL_CMD_SCENES_ADD_SCENE:
        zb_zcl_scenes_process_add_scene_command(param, &cmd_info, ZB_FALSE);
        break;
      case ZB_ZCL_CMD_SCENES_VIEW_SCENE:
        zb_zcl_scenes_process_view_scene_command(param, &cmd_info, ZB_FALSE);
        break;
      case ZB_ZCL_CMD_SCENES_REMOVE_SCENE:
        zb_zcl_scenes_process_remove_scene_command(param, &cmd_info);
        break;
      case ZB_ZCL_CMD_SCENES_REMOVE_ALL_SCENES:
        zb_zcl_scenes_process_remove_all_scenes_command(param, &cmd_info);
        break;
      case ZB_ZCL_CMD_SCENES_STORE_SCENE:
        zb_zcl_scenes_process_store_scene_command(param, &cmd_info);
        break;
      case ZB_ZCL_CMD_SCENES_RECALL_SCENE:
        zb_zcl_scenes_process_recall_scene_command(param, &cmd_info);
        break;
      case ZB_ZCL_CMD_SCENES_GET_SCENE_MEMBERSHIP:
        zb_zcl_scenes_process_get_scene_membership_command(param, &cmd_info);
        break;
#ifndef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
      case ZB_ZCL_CMD_SCENES_ENHANCED_ADD_SCENE:
        zb_zcl_scenes_process_add_scene_command(param, &cmd_info, ZB_TRUE);
        break;
      case ZB_ZCL_CMD_SCENES_ENHANCED_VIEW_SCENE:
        zb_zcl_scenes_process_view_scene_command(param, &cmd_info, ZB_TRUE);
        break;
      case ZB_ZCL_CMD_SCENES_COPY_SCENE:
        zb_zcl_scenes_process_copy_scene(param, &cmd_info);
        break;
#endif /* !ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */
      default:
        TRACE_MSG(
            TRACE_ERROR,
            "ERROR Unsupported Scenes cluster command 0x%hx",
            (FMT__H, cmd_info.cmd_id));
        processed = ZB_FALSE;
        break;
    }
  }
  else
  {
    TRACE_MSG(TRACE_ZCL2, "Received scene resp command", (FMT__0));
  }

  TRACE_MSG(
      TRACE_ZCL1,
      "< zb_zcl_process_scenes_specific_commands: processed %hd",
      (FMT__H, processed));

  return processed;
} /* zb_bool_t zb_zcl_process_scenes_specific_commands(zb_uint8_t param) */


zb_bool_t zb_zcl_process_scenes_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_scenes_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_scenes_specific_commands(param);
}


zb_bool_t zb_zcl_process_scenes_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_scenes_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_scenes_specific_commands(param);
}

zb_uint8_t zb_zcl_scenes_process_store_scene(zb_uint8_t param, zb_zcl_scenes_store_scene_req_t* req, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_uint8_t store_scene_status = ZB_ZCL_STATUS_SUCCESS;

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
                                    ZB_ZCL_SCENES_STORE_SCENE_CB_ID, RET_ERROR, cmd_info, req, &store_scene_status);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  return store_scene_status;
}

static void zb_zcl_scenes_process_store_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_scenes_store_scene_req_t* req;
  zb_zcl_scenes_store_scene_req_t req_copy;
  zb_uint8_t scene_count;
  zb_zcl_status_t store_scene_status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_attr_t* scene_valid_desc;
  zb_bool_t set_scene_valid = ZB_FALSE;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_process_store_scene_command param %hd",
      (FMT__H, param));

  ZB_ZCL_SCENES_GET_STORE_SCENE_REQ(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  scene_valid_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                            ZB_ZCL_CLUSTER_ID_SCENES,
                                            ZB_ZCL_CLUSTER_SERVER_ROLE,
                                            ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID);
  ZB_ASSERT(scene_valid_desc);

  TRACE_MSG(TRACE_ZCL1, "Update SceneValid: FALSE", (FMT__0));
  ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_valid_desc, ZB_FALSE);

  ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_store_scene_req_t));
  store_scene_status = (zb_zcl_status_t) zb_zcl_scenes_process_store_scene(param, &req_copy, cmd_info);

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
    {
      zb_zcl_attr_t* scene_count_desc;
      if (store_scene_status == ZB_ZCL_STATUS_SUCCESS)
      {
        scene_count_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                                  ZB_ZCL_CLUSTER_ID_SCENES,
                                                  ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                  ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID);
        if (scene_count_desc && scene_count_desc->data_p)
        {
          scene_count = ZB_ZCL_GET_ATTRIBUTE_VAL_8(scene_count_desc);
          TRACE_MSG(TRACE_ZCL1,
                    "increase scene count: %hd -> %hd",
                    (FMT__H_H, scene_count, scene_count + 1));
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_count_desc, scene_count + 1);
        }
        set_scene_valid = ZB_TRUE;
      }
    }
    /* FALLTHRU */
    case RET_ALREADY_EXISTS:
    {
      ZB_ZCL_SCENES_SEND_STORE_SCENE_RES(
        param,
        cmd_info->seq_number,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id,
        NULL,
        store_scene_status,
        req_copy.group_id,
        req_copy.scene_id);
        set_scene_valid = ZB_TRUE;
    }
    break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }
  if (set_scene_valid)
  {
    TRACE_MSG(TRACE_ZCL1, "Update SceneValid: TRUE", (FMT__0));
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_valid_desc, ZB_TRUE);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_scenes_process_store_scene_command", (FMT__0));
} /* void zb_zcl_scenes_process_store_scene_command(zb_uint8_t param) */

static void zb_zcl_scenes_process_add_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info, zb_bool_t is_enhanced)
{
  zb_zcl_scenes_add_scene_req_t* req;
  zb_zcl_scenes_add_scene_req_t req_copy;
  zb_uint8_t add_scene_status = ZB_ZCL_STATUS_SUCCESS;

  /* Initialize with invalid IDs */
  req_copy.group_id = 0xFF;
  req_copy.scene_id = 0xFF;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_process_add_scene_command param %hd",
      (FMT__H, param));

#ifdef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
  ZVUNUSED(is_enhanced);
#endif /* ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */

  ZB_ZCL_SCENES_GET_ADD_SCENE_REQ_COMMON(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }
  else if (ZCL_CTX().device_cb)
  {
    ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_add_scene_req_t));
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(
      param,
#ifndef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
      (is_enhanced) ? (ZB_ZCL_SCENES_ENHANCED_ADD_SCENE_CB_ID) : (ZB_ZCL_SCENES_ADD_SCENE_CB_ID),
#else
      ZB_ZCL_SCENES_ADD_SCENE_CB_ID,
#endif /* !ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */
      RET_ERROR,
      cmd_info,
      &req_copy,
      &add_scene_status);
    (ZCL_CTX().device_cb)(param);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
    {
      zb_zcl_attr_t* scene_count_desc;
      if (add_scene_status == ZB_ZCL_STATUS_SUCCESS)
      {
        zb_uint8_t scene_count;

        scene_count_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                                  ZB_ZCL_CLUSTER_ID_SCENES,
                                                  ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                  ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID);
        if (scene_count_desc && scene_count_desc->data_p)
        {
          scene_count = ZB_ZCL_GET_ATTRIBUTE_VAL_8(scene_count_desc);
          TRACE_MSG(TRACE_ZCL1,
            "increase scene count: %hd -> %hd",
                    (FMT__H_H, scene_count, scene_count + 1));
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_count_desc, scene_count + 1);
        }
      }
    }
    /* FALLTHRU */
    case RET_ALREADY_EXISTS:
    {
#ifdef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
      ZVUNUSED(is_enhanced);
#else
      if (is_enhanced)
      {
        ZB_ZCL_SCENES_SEND_ENHANCED_ADD_SCENE_RES(
          param,
          cmd_info->seq_number,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
          cmd_info->profile_id,
          NULL,
          add_scene_status,
          req_copy.group_id,
          req_copy.scene_id);
      }
      else
#endif /* ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */
      {
        ZB_ZCL_SCENES_SEND_ADD_SCENE_RES(
          param,
          cmd_info->seq_number,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
          cmd_info->profile_id,
          NULL,
          add_scene_status,
          req_copy.group_id,
          req_copy.scene_id);
      }
    }
    break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_scenes_process_add_scene_command", (FMT__0));
}/* void zb_zcl_scenes_process_add_scene_command(zb_uint8_t param) */

static void zb_zcl_scenes_process_view_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info, zb_bool_t is_enhanced)
{
  zb_zcl_scenes_view_scene_req_t* req;
  zb_zcl_scenes_view_scene_req_t req_copy;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_process_view_scene_command param %hd",
      (FMT__H, param));

#ifdef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
  ZVUNUSED(is_enhanced);
#endif /* ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */

  ZB_ZCL_SCENES_GET_VIEW_SCENE_REQ(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }
  else if (ZCL_CTX().device_cb)
  {
    ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_view_scene_req_t));
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(
      param,
#ifndef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED
      (is_enhanced) ? (ZB_ZCL_SCENES_ENHANCED_VIEW_SCENE_CB_ID) : (ZB_ZCL_SCENES_VIEW_SCENE_CB_ID),
#else
      ZB_ZCL_SCENES_VIEW_SCENE_CB_ID,
#endif /* !ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */
      RET_ERROR,
      cmd_info,
      &req_copy,
      NULL);
    (ZCL_CTX().device_cb)(param);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      /*We don't send a View Scene Resp command. Application must send reply
       * for this command.
       */
      zb_buf_free(param);
      break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_scenes_process_view_scene_command", (FMT__0));
} /* void zb_zcl_scenes_process_view_scene_command(zb_uint8_t param) */

static void zb_zcl_scenes_process_remove_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_scenes_remove_scene_req_t* req;
  zb_zcl_scenes_remove_scene_req_t req_copy;

  zb_uint8_t remove_scene_status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_attr_t* scene_count_desc;

  /* Initialize with invalid IDs */
  req_copy.group_id = 0xFF;
  req_copy.scene_id = 0xFF;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_process_remove_scene_command param %hd",
      (FMT__H, param));

  ZB_ZCL_SCENES_GET_REMOVE_SCENE_REQ(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }
  ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_remove_scene_req_t));
  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
          ZB_ZCL_SCENES_REMOVE_SCENE_CB_ID, RET_ERROR, cmd_info, &req_copy, &remove_scene_status);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
    {
      if (remove_scene_status == ZB_ZCL_STATUS_SUCCESS)
      {
        zb_uint8_t scene_count;

        scene_count_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                                  ZB_ZCL_CLUSTER_ID_SCENES,
                                                  ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                  ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID);
        if (scene_count_desc && scene_count_desc->data_p)
        {
          /* This attribute is not reportable, so may set attribute value directly */
          scene_count = ZB_ZCL_GET_ATTRIBUTE_VAL_8(scene_count_desc);
          TRACE_MSG(TRACE_ZCL1,
                    "decrease scene count: %hd -> %hd",
                    (FMT__H_H, scene_count, scene_count - 1));
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_count_desc, scene_count - 1);
        }
      }

      ZB_ZCL_SCENES_SEND_REMOVE_SCENE_RES(
        param,
        cmd_info->seq_number,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id,
        NULL,
        remove_scene_status,
        req_copy.group_id,
        req_copy.scene_id);
    }
    break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_scenes_process_remove_scene_command", (FMT__0));
} /* void zb_zcl_scenes_process_remove_scene_command(zb_uint8_t param) */

zb_uint8_t zb_zcl_scenes_process_remove_all_scenes(zb_uint8_t param, zb_zcl_scenes_remove_all_scenes_req_t* req, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_uint8_t remove_all_scenes_status = ZB_ZCL_STATUS_SUCCESS;

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
                                    ZB_ZCL_SCENES_REMOVE_ALL_SCENES_CB_ID, RET_ERROR, cmd_info, req,
                                    &remove_all_scenes_status);
  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  return remove_all_scenes_status;
}

static void zb_zcl_scenes_process_remove_all_scenes_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_scenes_remove_all_scenes_req_t* req;
  zb_zcl_scenes_remove_all_scenes_req_t req_copy;
  zb_uint8_t remove_all_scenes_status = ZB_ZCL_STATUS_SUCCESS;
  zb_zcl_attr_t* scene_count_desc;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_process_remove_all_scenes_command param %hd",
      (FMT__H, param));

  ZB_ZCL_SCENES_GET_REMOVE_ALL_SCENES_REQ(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }

  ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_remove_all_scenes_req_t));
  remove_all_scenes_status = zb_zcl_scenes_process_remove_all_scenes(param, &req_copy, cmd_info);

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
    {
      if (remove_all_scenes_status == ZB_ZCL_STATUS_SUCCESS)
      {
        scene_count_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                                  ZB_ZCL_CLUSTER_ID_SCENES,
                                                  ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                  ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID);
        if (scene_count_desc && scene_count_desc->data_p)
        {
          /* This attribute is not reportable, so may set attribute value directly */
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_count_desc, 0);
        }
      }

      ZB_ZCL_SCENES_SEND_REMOVE_ALL_SCENES_RES(
        param,
        cmd_info->seq_number,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
        cmd_info->profile_id,
        NULL,
        remove_all_scenes_status,
        req_copy.group_id);
    }
    break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_scenes_process_remove_all_scenes_command", (FMT__0));
} /* void zb_zcl_scenes_process_remove_all_scenes_command(zb_uint8_t param) */

static void zb_zcl_scenes_process_get_scene_membership_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_scenes_get_scene_membership_req_t* req;
  zb_zcl_scenes_get_scene_membership_req_t req_copy;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_process_get_scene_membership_command param %hd",
      (FMT__H, param));

  ZB_ZCL_SCENES_GET_GET_SCENE_MEMBERSHIP_REQ(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }
  ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_get_scene_membership_req_t));
  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
        ZB_ZCL_SCENES_GET_SCENE_MEMBERSHIP_CB_ID, RET_ERROR, cmd_info, &req_copy, NULL);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      /*We don't send a Get Scene Membership Resp command. Application must send reply
       * for this command.
       */
      zb_buf_free(param);
      break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(
      TRACE_ZCL1,
      "< zb_zcl_scenes_process_get_scene_membership_command",
      (FMT__0));
} /* zb_zcl_scenes_process_get_scene_membership_command(zb_uint8_t param) */

zb_uint8_t zb_zcl_scenes_process_recall_scene(zb_uint8_t param, zb_zcl_scenes_recall_scene_req_t* req, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_uint8_t recall_scene_status = ZB_ZCL_STATUS_SUCCESS;

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
     ZB_ZCL_SCENES_RECALL_SCENE_CB_ID, RET_ERROR, cmd_info, req, &recall_scene_status);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }
  return recall_scene_status;
}

static void zb_zcl_scenes_process_recall_scene_command(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_scenes_recall_scene_req_t* req;
  zb_zcl_scenes_recall_scene_req_t req_copy;
  zb_zcl_status_t recall_scene_status = ZB_ZCL_STATUS_SUCCESS;
  zb_uint8_t req_len;

  TRACE_MSG( TRACE_ZCL1, "> zb_zcl_scenes_process_recall_scene_command param %hd", (FMT__H, param));

  ZB_ZCL_SCENES_GET_RECALL_SCENE_REQ(param, req, req_len);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
    return;
  }
  else
  {
    ZB_MEMCPY(&req_copy, req, req_len);
    if (req_len < ZB_ZCL_SCENES_RECALL_SCENE_REQ_PAYLOAD_LEN)
    {
      req_copy.transition_time = ZB_ZCL_SCENES_RECALL_SCENE_REQ_TRANSITION_TIME_INVALID_VALUE;  
    }
    recall_scene_status = (zb_zcl_status_t)zb_zcl_scenes_process_recall_scene(param, &req_copy, cmd_info);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
    {
      /* Update CurrentGroup, CurrentScene and SceneValid attributes. */
      zb_zcl_attr_t* attr_desc;

      attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                         ZB_ZCL_CLUSTER_ID_SCENES,
                                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                                         ZB_ZCL_ATTR_SCENES_CURRENT_GROUP_ID);
      ZB_ASSERT(attr_desc);
      TRACE_MSG(TRACE_ZCL1, "Update CurrentGroup: 0x%x", (FMT__D, req_copy.group_id));
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, req_copy.group_id);

      attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                         ZB_ZCL_CLUSTER_ID_SCENES,
                                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                                         ZB_ZCL_ATTR_SCENES_CURRENT_SCENE_ID);
      ZB_ASSERT(attr_desc);
      TRACE_MSG(TRACE_ZCL1, "Update CurrentScene: 0x%hx", (FMT__H, req_copy.scene_id));
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, req_copy.scene_id);

      attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                         ZB_ZCL_CLUSTER_ID_SCENES,
                                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                                         ZB_ZCL_ATTR_SCENES_SCENE_VALID_ID);
      ZB_ASSERT(attr_desc);
      TRACE_MSG(TRACE_ZCL1, "Update SceneValid: TRUE", (FMT__0));
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ZB_TRUE);

      zb_zcl_send_default_handler(param, cmd_info, recall_scene_status);
    }
      break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(
      TRACE_ZCL1,
      "< zb_zcl_scenes_process_recall_scene_command",
      (FMT__0));
} /* zb_zcl_scenes_process_recall_scene_command(zb_uint8_t param) */

void zb_zcl_scenes_remove_all_scenes_in_all_endpoints_by_group_id(zb_uint8_t param, zb_uint16_t group_id)
{
  zb_zcl_scenes_remove_all_scenes_req_t req;

  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_remove_all_scenes_in_all_endpoints_by_group_id param %hd group_id 0x%x",
      (FMT__H_D, param, group_id));

  req.group_id = group_id;

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_SCENES_INTERNAL_REMOVE_ALL_SCENES_ALL_ENDPOINTS_CB_ID, RET_ERROR, NULL, &req, NULL);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  zb_buf_free(param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_scenes_remove_all_scenes_in_all_endpoints_by_group_id", (FMT__0));
}/* void zb_zcl_scenes_remove_scenes_in_all_endpoints(zb_uint16_t...) */

void zb_zcl_scenes_remove_all_scenes_in_all_endpoints(zb_uint8_t param)
{
  TRACE_MSG(
      TRACE_ZCL1,
      "> zb_zcl_scenes_remove_all_scenes_in_all_endpoints param %hd",
      (FMT__H, param));

  ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(param,
    ZB_ZCL_SCENES_INTERNAL_REMOVE_ALL_SCENES_ALL_ENDPOINTS_ALL_GROUPS_CB_ID, RET_ERROR, NULL, NULL, NULL);

  if (ZCL_CTX().device_cb)
  {
    (ZCL_CTX().device_cb)(param);
  }

  zb_buf_free(param);

}/* void zb_zcl_scenes_remove_scenes_in_all_endpoints(zb_uint16_t...) */


static inline void zb_zcl_scenes_send_recall_scene_req_rev1(zb_bufid_t buffer,
                                                            const zb_addr_u *dst_addr,
                                                            zb_uint8_t dst_addr_mode,
                                                            zb_uint8_t dst_ep,
                                                            zb_uint8_t ep,
                                                            zb_uint16_t prof_id,
                                                            zb_uint8_t dis_default_resp,
                                                            zb_callback_t callback,
                                                            zb_uint16_t group_id,
                                                            zb_uint8_t scene_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL
  (ptr
    , dis_default_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(
      ptr,
      ZB_ZCL_GET_SEQ_NUM(),
      ZB_ZCL_CMD_SCENES_RECALL_SCENE);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_SCENES, callback);
}

static inline void zb_zcl_scenes_send_recall_scene_req_rev2(zb_bufid_t buffer,
                                              const zb_addr_u *dst_addr,
                                              zb_uint8_t dst_addr_mode,
                                              zb_uint8_t dst_ep,
                                              zb_uint8_t ep,
                                              zb_uint16_t prof_id,
                                              zb_uint8_t dis_default_resp,
                                              zb_callback_t callback,
                                              zb_uint16_t group_id,
                                              zb_uint8_t scene_id,
                                              zb_uint16_t transition_time)
{
    zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
    ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL
    (ptr
     , dis_default_resp)
    ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(
        ptr,
        ZB_ZCL_GET_SEQ_NUM(),
        ZB_ZCL_CMD_SCENES_RECALL_SCENE);
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (group_id));
    ZB_ZCL_PACKET_PUT_DATA8(ptr, (scene_id));
    ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_SCENES, callback);
}

void zb_zcl_scenes_send_recall_scene_req_zcl8(zb_bufid_t buffer,
                                              const zb_addr_u *dst_addr,
                                              zb_uint8_t dst_addr_mode,
                                              zb_uint8_t dst_ep,
                                              zb_uint8_t ep,
                                              zb_uint16_t prof_id,
                                              zb_uint8_t dis_default_resp,
                                              zb_callback_t callback,
                                              zb_uint16_t group_id,
                                              zb_uint8_t scene_id,
                                              zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_scenes_send_recall_scene_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_SCENES_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_SCENES,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      zb_zcl_scenes_send_recall_scene_req_rev1(buffer,
                                              dst_addr,
                                              dst_addr_mode,
                                              dst_ep,
                                              ep,
                                              prof_id,
                                              dis_default_resp,
                                              callback,
                                              group_id,
                                              scene_id);
      break;
    case 2:
      /* FALLTHROUGH */
    case 3:
      zb_zcl_scenes_send_recall_scene_req_rev2(buffer,
                                              dst_addr,
                                              dst_addr_mode,
                                              dst_ep,
                                              ep,
                                              prof_id,
                                              dis_default_resp,
                                              callback,
                                              group_id,
                                              scene_id,
                                              transition_time);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_scenes_send_recall_scene_req_zcl8", (FMT__0));
}

void zb_zcl_scenes_send_recall_scene_req(zb_bufid_t buffer,
                                         const zb_addr_u *dst_addr,
                                         zb_uint8_t dst_addr_mode,
                                         zb_uint8_t dst_ep,
                                         zb_uint8_t ep,
                                         zb_uint16_t prof_id,
                                         zb_uint8_t dis_default_resp,
                                         zb_callback_t callback,
                                         zb_uint16_t group_id,
                                         zb_uint8_t scene_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_scenes_send_recall_scene_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_SCENES,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      zb_zcl_scenes_send_recall_scene_req_rev1(buffer,
                                              dst_addr,
                                              dst_addr_mode,
                                              dst_ep,
                                              ep,
                                              prof_id,
                                              dis_default_resp,
                                              callback,
                                              group_id,
                                              scene_id);
      break;
    case 2:
      /* FALLTHROUGH */
    case 3:
      zb_zcl_scenes_send_recall_scene_req_rev2(buffer,
                                              dst_addr,
                                              dst_addr_mode,
                                              dst_ep,
                                              ep,
                                              prof_id,
                                              dis_default_resp,
                                              callback,
                                              group_id,
                                              scene_id,
                                              ZB_ZCL_RECALL_SCENES_TRANSITION_TIME_NOT_USED_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_scenes_send_recall_scene_req", (FMT__0));
}

#ifndef ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED

static void zb_zcl_scenes_process_copy_scene(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info)
{
  zb_zcl_scenes_copy_scene_req_t* req;
  zb_zcl_scenes_copy_scene_req_t req_copy;
  zb_uint8_t num_copied_scenes = 0x00U;

  TRACE_MSG(TRACE_ZCL1, ">>zb_zcl_scenes_process_copy_scene(), param %hd", (FMT__H, param));

  /* Initialize with invalid IDs */
  req_copy.group_id_from = 0xFF;
  req_copy.scene_id_from = 0xFF;
  req_copy.group_id_to = 0xFF;
  req_copy.scene_id_to = 0xFF;

  ZB_ZCL_SCENES_GET_COPY_SCENE_REQ(param, req);

  if (!req)
  {
    zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_INVALID_FIELD);
  }
  else if (ZCL_CTX().device_cb)
  {
    ZB_MEMCPY(&req_copy, req, sizeof(zb_zcl_scenes_copy_scene_req_t));
    ZB_ZCL_DEVICE_CMD_PARAM_INIT_WITH(
      param,
      ZB_ZCL_SCENES_COPY_SCENE_CB_ID,
      RET_ERROR,
      cmd_info,
      &req_copy,
      &num_copied_scenes);

    TRACE_MSG(TRACE_ZCL1, "call device cb %p", (FMT__P, ZCL_CTX().device_cb));
    (ZCL_CTX().device_cb)(param);
  }

  switch (ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param))
  {
    case RET_OK:
      if (num_copied_scenes != 0U)
      {
        zb_uint8_t scene_count;

        zb_zcl_attr_t* scene_count_desc =
          zb_zcl_get_attr_desc_a(
            ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
            ZB_ZCL_CLUSTER_ID_SCENES,
            ZB_ZCL_CLUSTER_SERVER_ROLE,
            ZB_ZCL_ATTR_SCENES_SCENE_COUNT_ID);

        if (scene_count_desc && scene_count_desc->data_p)
        {
          /* This attribute is not reportable, so may set attribute value directly */
          scene_count = ZB_ZCL_GET_ATTRIBUTE_VAL_8(scene_count_desc);
          if (scene_count + num_copied_scenes <= ZB_ZCL_MAX_SCENE_TABLE_RECORDS)
          {
            TRACE_MSG(TRACE_ZCL1, "increase scene count: from %hd to %hd",
                      (FMT__H_H, scene_count, scene_count + num_copied_scenes));
            ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(scene_count_desc, scene_count + num_copied_scenes);
          }
          else
          {
            TRACE_MSG(TRACE_ERROR, "ERROR: not enough space to copy scenes! scene_count_attr %hd, num_copied_scenes %hd, ZB_ZCL_MAX_SCENE_TABLE_RECORDS %hd",
                      (FMT__H_H_H, num_copied_scenes, scene_count, ZB_ZCL_MAX_SCENE_TABLE_RECORDS));
          }
        }
      }

      /* We do not send a Copy Scene Resp command.
       * Application must send reply for this command.
       */
      zb_buf_free(param);
      break;

    case RET_ERROR:
      TRACE_MSG(TRACE_ZCL1, "ERROR during command processing: "
                "User callback failed with err=%d.",
                (FMT__D, ZB_ZCL_DEVICE_CMD_PARAM_STATUS(param)));
      /* FALLTHRU */
    default:
      zb_zcl_send_default_handler(param, cmd_info, ZB_ZCL_STATUS_FAIL);
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "<<zb_zcl_scenes_process_copy_scene()", (FMT__0));

} /* void zb_zcl_scenes_process_copy_scene(zb_uint8_t param, const zb_zcl_parsed_hdr_t *cmd_info) */

#endif /* !ZB_ZCL_SCENES_OPTIONAL_COMMANDS_DISABLED */

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_SCENES*/
