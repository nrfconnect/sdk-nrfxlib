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
/* PURPOSE: ZCL IAS Zone cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2070

#if defined ZB_ENABLE_HA
/* #include "ha/zb_zcl_ias_zone_ha_adds.h" */
#endif /* defined ZB_ENABLE_HA */

#include "zb_common.h"

#if defined ZB_ZCL_SUPPORT_CLUSTER_IAS_ZONE

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_ias_zone.h"
#include "zb_nvram.h"
#include "zb_osif.h"
#include "zb_zdo.h"
#include "zb_aps.h"

/** @internal
    @{
*/

static zb_uint8_t gs_ias_zone_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IAS_ZONE_CLIENT_ROLE_RECEIVED_CMD_LIST
};

static zb_uint8_t gs_ias_zone_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_IAS_ZONE_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_ias_zone_server_cmd_list =
{
  sizeof(gs_ias_zone_server_received_commands), gs_ias_zone_server_received_commands,
  sizeof(gs_ias_zone_client_received_commands), gs_ias_zone_client_received_commands
};

zb_discover_cmd_list_t gs_ias_zone_client_cmd_list =
{
  sizeof(gs_ias_zone_client_received_commands), gs_ias_zone_client_received_commands,
  sizeof(gs_ias_zone_server_received_commands), gs_ias_zone_server_received_commands
};


void zb_zcl_ias_zone_restore_curr_zone_sens_level(zb_uint8_t endpoint);
zb_ret_t zb_zcl_ias_zone_init_mode(zb_zcl_ias_zone_int_ctx_t *int_ctx, zb_uint8_t mode, zb_uint16_t val);
void zb_zcl_ias_zone_restore_normal_mode(zb_uint8_t endpoint);
void zb_zcl_ias_zone_set_test_bit_delayed(zb_uint8_t endpoint, zb_uint8_t test_bit);
void zb_zcl_ias_zone_set_test_bit(zb_uint8_t param, zb_uint16_t user_param);
zb_ret_t check_value_ias_zone_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
void zb_zcl_ias_zone_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);
zb_bool_t zb_zcl_process_ias_zone_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_ias_zone_specific_commands_cli(zb_uint8_t param);

void zb_zcl_ias_zone_init_server()
{
  zb_uint8_t ep_id = zb_get_current_endpoint_id();

  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IAS_ZONE,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_ias_zone_server,
                              zb_zcl_ias_zone_write_attr_hook_server,
                              zb_zcl_process_ias_zone_specific_commands_srv);

  zb_zcl_ias_zone_check_cie_addr_on_zcl_initialization(ep_id);
}

void zb_zcl_ias_zone_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_IAS_ZONE,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_ias_zone_specific_commands_cli);
}

zb_ret_t check_value_ias_zone_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(endpoint);

  TRACE_MSG(TRACE_ZCL1, "> check_value_ias_zone", (FMT__0));

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_ID:
    {
      if( ZB_ZCL_IAS_ZONE_NUMBER_OF_ZONE_SENSITIVITY_LEVELS_SUPPORTED_MIN_VALUE > *value )
      {
        ret = RET_ERROR;
      }
      break;
    }
    case ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID:
    {
      zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET);
      ZB_ASSERT(attr_desc != NULL);
      if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == ZB_B2U(ZB_TRUE))
      {
        attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID);
        if (ZB_MEMCMP(attr_desc->data_p, value, zb_zcl_get_attribute_size(attr_desc->type, value)) != 0)
        {
          ret = RET_UNAUTHORIZED;
        }
      }
    }
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_ias_zone ret %hd", (FMT__H, ret));
  return ret;
}

static zb_ret_t check_cie_establishment(zb_uint8_t endpoint)
{
  zb_ret_t ret = RET_OK;

  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET);
  ZB_ASSERT(attr_desc);

  if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == ZB_B2U(ZB_FALSE))
  {
    TRACE_MSG(TRACE_ZCL1, "CIE addr isn't set on IAS Zone server", (FMT__0));
    ret = RET_UNAUTHORIZED;
  }

  return ret;
}

static zb_bool_t check_cie_authorization(zb_uint8_t endpoint, zb_uint16_t short_addr)
{
  zb_uint16_t cie_short_addr;

  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID);
  ZB_ASSERT(attr_desc);

  /* AN: We have already put entry in adress table during step 2 of each commissioning method. */
  cie_short_addr = zb_address_short_by_ieee(attr_desc->data_p);

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR);

  ZB_ASSERT(attr_desc);

  return (cie_short_addr == ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) && 
          cie_short_addr == short_addr) ? ZB_TRUE : ZB_FALSE;
}

zb_ret_t zb_zcl_ias_zone_put_cie_address_to_binding_whitelist(zb_uint8_t ep_id)
{
  zb_ret_t ret = RET_OK;

  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(ep_id,
    ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID);
  ZB_ASSERT(attr_desc);

  ret = zb_aps_add_binding_whitelist_entry(ep_id, ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, (zb_uint8_t *)attr_desc->data_p);

  return ret;
}

/** @brief Initiate Test mode command */
static zb_ret_t zb_zcl_ias_zone_init_test_mode_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_zone_init_test_mode_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_init_test_mode_handler param %d", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_IAS_ZONE_GET_INITIATE_TEST_MODE_REQ(&payload, param, status);

  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ZONE_GET_INITIATE_TEST_MODE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ret = check_cie_establishment(endpoint);

    if (ret == RET_OK)
    {
      if(check_cie_authorization(endpoint, cmd_info.addr_data.common_data.source.u.short_addr))
      {
        zb_zcl_attr_t *attr_desc_level = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID);
        zb_zcl_attr_t *attr_desc_int_ctx = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID);
        zb_zcl_ias_zone_int_ctx_t *int_ctx;
        zb_ret_t retcode;

        ZB_ASSERT(attr_desc_level != NULL);
        ZB_ASSERT(attr_desc_int_ctx != NULL);

        int_ctx = (zb_zcl_ias_zone_int_ctx_t*)attr_desc_int_ctx->data_p;
        int_ctx->restore_current_zone_sens_level = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_level);

        /* NK: Set attribute value directly. Reason: do not call general_cb with SENSITIVITY_SET param here,
         * use only one general_cb - with TEST_MODE. It can do some specific checks of this attribute
         * and return some result with retcode. */
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_level, payload.current_zone_sens_level);

        retcode = zb_zcl_ias_zone_init_mode(int_ctx, ZB_ZCL_INIT_TEST_MODE, payload.current_zone_sens_level);

        if (retcode == RET_INVALID_PARAMETER)
        {
          /* NK: Send ZCL response with invalid parameter code. */
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_level, int_ctx->restore_current_zone_sens_level);
          return retcode;
        }
        else
        {
          /* NK: If zb_zcl_ias_zone_init_mode() returned RET_OK, than do not set test bit from ZCL layer. */
          if (retcode != RET_OK)
          {
            zb_zcl_ias_zone_set_test_bit_delayed(endpoint, 1);
          }

          /* Schedule current_zone_sens_level restore and turning to normal mode */
          ZB_SCHEDULE_ALARM(zb_zcl_ias_zone_restore_normal_mode, endpoint, payload.test_mode_duration * ZB_TIME_ONE_SECOND);
        }
      }
      else
      {
        ret = RET_UNAUTHORIZED;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_init_test_mode_handler ret %hx", (FMT__H, ret));
  return ret;
}

void zb_zcl_ias_zone_set_test_bit_delayed(zb_uint8_t endpoint, zb_uint8_t test_bit)
{
  zb_buf_get_out_delayed_ext(zb_zcl_ias_zone_set_test_bit, endpoint | (test_bit << 8), 0);
}

void zb_zcl_ias_zone_set_test_bit(zb_uint8_t param, zb_uint16_t user_param)
{
  zb_uint8_t endpoint = ZB_GET_LOW_BYTE(user_param);
  zb_uint8_t test_bit = ZB_GET_HI_BYTE(user_param);
  zb_uint16_t curr_zone_status;

  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID);

  ZB_ASSERT(attr_desc != NULL);

  curr_zone_status = *(zb_uint16_t*)attr_desc->data_p;
  if (test_bit)
  {
    curr_zone_status |= (zb_uint16_t)ZB_ZCL_IAS_ZONE_ZONE_STATUS_TEST;
  }
  else
  {
    curr_zone_status &= ~(zb_uint16_t)ZB_ZCL_IAS_ZONE_ZONE_STATUS_TEST;
  }

  zb_zcl_ias_zone_set_status(endpoint, curr_zone_status, 0, param);
}

/** @brief Initiate Normal mode command */
static zb_ret_t zb_zcl_ias_zone_init_normal_mode_handler(zb_uint8_t param)
{
  zb_uint8_t alarm_buf_param = 0;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc_int_ctx;
  zb_zcl_ias_zone_int_ctx_t *int_ctx;
  zb_ret_t retcode;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_init_normal_mode_handler param %d", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  retcode = check_cie_establishment(endpoint);

  if (retcode == RET_OK)
  {
    if(check_cie_authorization(endpoint, cmd_info.addr_data.common_data.source.u.short_addr))
    {
      attr_desc_int_ctx = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID);

      ZB_ASSERT(attr_desc_int_ctx != NULL);

      int_ctx = (zb_zcl_ias_zone_int_ctx_t*)attr_desc_int_ctx->data_p;

            ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(zb_zcl_ias_zone_restore_normal_mode, ZB_ALARM_ANY_PARAM, &alarm_buf_param);

      if (alarm_buf_param)
      {
        zb_zcl_ias_zone_restore_normal_mode(alarm_buf_param);
      }
      else
      {
        /* NK: Turn on Normal Mode even if it was no Test Mode command before. */
        retcode = zb_zcl_ias_zone_init_mode(int_ctx, ZB_ZCL_INIT_NORMAL_MODE, 0);

        /* NK: If zb_zcl_ias_zone_init_mode() returned RET_OK, then do not set test bit from ZCL layer. */
        if (retcode != RET_OK)
        {
          zb_zcl_ias_zone_set_test_bit_delayed(endpoint, 0);
          retcode = RET_OK;
        }
      }
      TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_init_normal_mode_handler", (FMT__0));
    }
    else
    {
      retcode = RET_UNAUTHORIZED;
    }
  }
  return retcode;
}

#if defined ZB_ENABLE_HA

/* Invoke User App for "Enroll Response" command
 */
void zb_zcl_ias_zone_enroll_response_invoke_user_app(zb_uint8_t param)
{
  zb_zcl_ias_zone_enroll_response_user_app_schedule_t* invoke_data =
      ZB_BUF_GET_PARAM(param, zb_zcl_ias_zone_enroll_response_user_app_schedule_t);
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t result;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_enroll_response_invoke_user_app param %hd", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, &(invoke_data->cmd_info), sizeof(zb_zcl_parsed_hdr_t));

  if (ZCL_CTX().device_cb != NULL)
  {

    zb_zcl_device_callback_param_t *user_app_data = ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    ZB_MEMMOVE(&(user_app_data->cb_param.enroll_response_value_param),
        &(invoke_data->param),
        sizeof(zb_zcl_ias_zone_enroll_response_value_param_t));
    user_app_data->device_cb_id = ZB_ZCL_IAS_ZONE_ENROLL_RESPONSE_VALUE_CB_ID;

    user_app_data->status = RET_OK;
    (ZCL_CTX().device_cb)(param);
    result = user_app_data->status;
  }
  else
  {
    result = RET_NOT_IMPLEMENTED;
  }

  ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, zb_zcl_get_zcl_status_from_ret(result));

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_enroll_response_invoke_user_app", (FMT__0));
}

#endif /* defined ZB_ENABLE_HA */

/** @brief Zone Enroll Response command */
static zb_ret_t zone_enroll_res_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_ias_zone_enroll_res_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zone_enroll_res_handler param %d", (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_IAS_ZONE_GET_ZONE_ENROLL_RES(&payload, param, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_IAS_ZONE_GET_ZONE_ENROLL_RES",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else
  {
    ret = check_cie_establishment(endpoint);

    if (ret == RET_OK)
    {
      if(check_cie_authorization(endpoint, cmd_info.addr_data.common_data.source.u.short_addr))
      {
        zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID);
        ZB_ASSERT(attr_desc);

        ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc,
          payload.code==ZB_ZCL_IAS_ZONE_ENROLL_RESPONCE_CODE_SUCCESS ?
          ZB_ZCL_IAS_ZONE_ZONESTATE_ENROLLED :
          ZB_ZCL_IAS_ZONE_ZONESTATE_NOT_ENROLLED);

#if defined ZB_ENABLE_HA

        /* ZCL 8.2.2.3.1.1 says (about 'zone_id' field):
           "This field is only relevant if the response code is success". */
        if (payload.code == ZB_ZCL_IAS_ZONE_ENROLL_RESPONCE_CODE_SUCCESS)
        {
          attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID);
          ZB_ASSERT(attr_desc);
          if ((*(zb_uint8_t*)attr_desc->data_p) != (zb_uint8_t)payload.zone_id)
          {
            ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, payload.zone_id);
#ifdef ZB_USE_NVRAM
            /* If we fail, trace is given and assertion is triggered */
            (void)zb_nvram_write_dataset(ZB_NVRAM_HA_DATA);
#endif
          }
        }
        ZB_ZCL_IAS_ZONE_ENROLL_RESPONSE_SCHEDULE_USER_APP(param, &cmd_info,
                                                          payload.code,
                                                          payload.zone_id);
        ret = RET_BUSY;
#endif /* defined ZB_ENABLE_HA */
      }
      else
      {
        ret = RET_UNAUTHORIZED;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zone_enroll_res_handler ret %hx", (FMT__H, ret));
  return ret;
}

zb_ret_t zb_zcl_ias_zone_init_mode(zb_zcl_ias_zone_int_ctx_t *int_ctx, zb_uint8_t mode, zb_uint16_t val)
{
  return (int_ctx->general_cb) ? int_ctx->general_cb(mode, val) : RET_ERROR;
}

/* NK:TODO: Modify this: zb_zcl_ias_zone_restore_normal_mode(endpoint, restore_bit); to use this
 * func in scheduling and in normal mode handling */
void zb_zcl_ias_zone_restore_normal_mode(zb_uint8_t endpoint)
{
  zb_zcl_attr_t *attr_desc_level = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID);
  zb_zcl_attr_t *attr_desc_int_ctx = zb_zcl_get_attr_desc_a(endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID);
  zb_zcl_ias_zone_int_ctx_t *int_ctx;
  zb_ret_t retcode;

  ZB_ASSERT(attr_desc_level != NULL);
  ZB_ASSERT(attr_desc_int_ctx != NULL);

  int_ctx = (zb_zcl_ias_zone_int_ctx_t*)attr_desc_int_ctx->data_p;

  ZB_ASSERT(attr_desc_int_ctx);

  /* NK: Temporary disable current_zone_sens_level restoring */

  /* ZB_ZCL_SET_ATTRIBUTE(endpoint, ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID, */
  /*                      &(int_ctx->restore_current_zone_sens_level), ZB_FALSE); */

  retcode = zb_zcl_ias_zone_init_mode(int_ctx, ZB_ZCL_INIT_NORMAL_MODE, 0);

  /* NK: If zb_zcl_ias_zone_init_mode() returned RET_OK, then do not set test bit from ZCL layer. */
  if (retcode != RET_OK)
  {
    zb_zcl_ias_zone_set_test_bit_delayed(endpoint, 0);
  }
}

zb_bool_t zb_zcl_process_ias_zone_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_ias_zone_specific_commands: param %d, cmd %d",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_IAS_ZONE == cmd_info.cluster_id);

  if(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction)
  {
    switch( cmd_info.cmd_id )
    {
    case ZB_ZCL_CMD_IAS_ZONE_ZONE_ENROLL_RESPONSE_ID:
      status = zone_enroll_res_handler(param);
      break;

    case ZB_ZCL_CMD_IAS_ZONE_INITIATE_NORMAL_OPERATION_MODE_ID:
      status = zb_zcl_ias_zone_init_normal_mode_handler(param);
      break;

    case ZB_ZCL_CMD_IAS_ZONE_INITIATE_TEST_MODE_ID:
      status = zb_zcl_ias_zone_init_test_mode_handler(param);
      break;

    default:
      processed = ZB_FALSE;
      break;
    }
  }
  else  // ZB_ZCL_FRAME_DIRECTION_TO_CLI
  {
    switch( cmd_info.cmd_id )
    {
    case ZB_ZCL_CMD_IAS_ZONE_ZONE_STATUS_CHANGE_NOT_ID:
      // send response below the code
      break;

    default:
      processed = ZB_FALSE;
      break;
    }
  }

  if( processed )
  {
    if( cmd_info.disable_default_response && status == RET_OK)
    {
      TRACE_MSG( TRACE_ZCL3,
                 "Default response disabled",
                 (FMT__0));
      zb_buf_free(param);
    }
    else if (status != RET_BUSY)
    {
      zb_uint8_t zcl_status = ZB_ZCL_STATUS_SUCCESS;
      switch (status)
      {
        case RET_UNAUTHORIZED:
          zcl_status = ZB_ZCL_STATUS_NOT_AUTHORIZED;
          break;
        case RET_INVALID_PARAMETER: 
        case RET_INVALID_PARAMETER_1:
          zcl_status = ZB_ZCL_STATUS_INVALID_FIELD;
          break;
        default:
          break;
      }
      ZB_ZCL_SEND_DEFAULT_RESP_DIRECTION( param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.profile_id,
          ZB_ZCL_CLUSTER_ID_IAS_ZONE,
          cmd_info.seq_number,
          cmd_info.cmd_id,
          zcl_status,
          cmd_info.cmd_direction==ZB_ZCL_FRAME_DIRECTION_TO_SRV
            ? ZB_ZCL_FRAME_DIRECTION_TO_CLI
            : ZB_ZCL_FRAME_DIRECTION_TO_SRV);
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_ias_zone_specific_commands: processed %d",
             (FMT__D, processed));
  return processed;
}


zb_bool_t zb_zcl_process_ias_zone_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ias_zone_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_ias_zone_specific_commands(param);
}

zb_bool_t zb_zcl_process_ias_zone_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_ias_zone_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_ias_zone_specific_commands(param);
}
/**********************************************************************/

/* Set or clear Zone Status bits
 * for User App
 */
void zb_zcl_ias_zone_change_status(zb_uint8_t param)
{
  zb_zcl_ias_zone_status_param_t* params = ZB_BUF_GET_PARAM(param, zb_zcl_ias_zone_status_param_t);
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t val;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_change_status param %hd", (FMT__H, param));

  attr_desc = zb_zcl_get_attr_desc_a(params->endpoint,
        ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID);
  ZB_ASSERT(attr_desc);

  val = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

  TRACE_MSG(TRACE_ZCL2, "old val 0x%x bits 0x%x is_set %hd", (FMT__D_D_H,
      val, params->bits, params->is_set));

  if(params->is_set)
  {
    val |= (params->bits);
  }
  else
  {
    val &= ~(params->bits);
  }

  TRACE_MSG(TRACE_ZCL2, "new val 0x%x", (FMT__D, val));

  zb_zcl_ias_zone_set_status(
    params->endpoint, val,
    0 /* delay value is not used for in pure ZCL (no HA) */,
    param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_change_status", (FMT__0));
}

zb_bool_t zb_zcl_ias_zone_set_status(
  zb_uint8_t ep,
  zb_uint16_t new_val,
  zb_uint16_t delay,
  zb_uint8_t buf_param)
{
  zb_zcl_attr_t *attr_desc;
  zb_bool_t ret = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1,
            ">> zb_zcl_ias_zone_set_status ep %hd, buf_param %hd, new_val %d, delay %d",
            (FMT__H_H_D_D, ep, buf_param, new_val, delay));

  if (buf_param != ZB_UNDEFINED_BUFFER)
  {
    zb_zcl_ias_zone_notification_param_t* notify_param =
      ZB_BUF_GET_PARAM(buf_param, zb_zcl_ias_zone_notification_param_t);

    TRACE_MSG(TRACE_ZCL2, "buffer is provided %hd", (FMT__H, buf_param));
    notify_param->ep = ep;
    notify_param->status_val = new_val;
    notify_param->delay = delay;

    ret = zb_zcl_ias_zone_check_attr_notify(buf_param);
  }
  else
  {
    /*
      Notification is NOT sent if buffer is not specified. User App
      should provide buffer for a command.
      Reason: simplify source code and make memory using transparent
      for use App
    */
  }

  attr_desc = zb_zcl_get_attr_desc_a(
    ep, ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID);
  ZB_ASSERT(attr_desc);
  TRACE_MSG(TRACE_ZCL2, "set new value for zone status anyway %xd", (FMT__D, new_val));

  ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc, new_val);

  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_ias_zone_set_status ret %hd", (FMT__H, ret));
  return ret;
}

zb_bool_t zb_zcl_ias_zone_check_attr_notify(
  zb_uint8_t buf_param)
{
  zb_bool_t send_not = ZB_FALSE;
  zb_zcl_attr_t *attr_desc;
  zb_uint16_t cur_val;
  zb_uint8_t zone_state;
  zb_zcl_ias_zone_notification_param_t* notify_param =
    ZB_BUF_GET_PARAM(buf_param, zb_zcl_ias_zone_notification_param_t);

  TRACE_MSG(TRACE_ZCL1,
    ">> zb_zcl_ias_zone_check_attr_notify buf_param %hd",
    (FMT__H,  buf_param));

  attr_desc = zb_zcl_get_attr_desc_a(
    notify_param->ep, ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATUS_ID);
  ZB_ASSERT(attr_desc);
  cur_val = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

  /* send notification if ZoneState is enroll */
  attr_desc = zb_zcl_get_attr_desc_a(
    notify_param->ep, ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_IAS_ZONE_ZONESTATE_ID);
  ZB_ASSERT(attr_desc);
  zone_state = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

  TRACE_MSG(TRACE_ZCL2, "zone_state %hd", (FMT__H, zone_state));
  if (zone_state == ZB_ZCL_IAS_ZONE_ZONESTATE_ENROLLED)
  {
    zb_uint16_t test_clear = 0xFFFF;
    zb_uint16_t test_set =
      ZB_ZCL_IAS_ZONE_ZONE_STATUS_ALARM1    |
      ZB_ZCL_IAS_ZONE_ZONE_STATUS_ALARM2    |
      ZB_ZCL_IAS_ZONE_ZONE_STATUS_TAMPER    |
      ZB_ZCL_IAS_ZONE_ZONE_STATUS_BATTERY   |
      ZB_ZCL_IAS_ZONE_ZONE_STATUS_TROUBLE   |
      ZB_ZCL_IAS_ZONE_ZONE_STATUS_AC_MAINS
#if defined ZB_ENABLE_HA
      | ZB_ZCL_IAS_ZONE_ZONE_STATUS_TEST
      | ZB_ZCL_IAS_ZONE_ZONE_STATUS_BATTERY_DEFECT
#endif /* defined ZB_ENABLE_HA*/
      ;

    TRACE_MSG(TRACE_ZCL2, "current val 0x%x new val 0x%x", (FMT__D_D, cur_val, notify_param->status_val));

    /* TODO: check this if-check!!! */
    if (/* old -> new : (0->1)=1 only */
        (~(cur_val & test_set) & (notify_param->status_val & test_set) ) ||
        /* old -> new : (1->0)=1 only  && set StatusRestore */
        ( ((cur_val & test_clear) & ~(notify_param->status_val & test_clear) ) &&
          (cur_val & ZB_ZCL_IAS_ZONE_ZONE_STATUS_RESTORE)) )
    {
      TRACE_MSG(TRACE_ZCL2, "Will send notification", (FMT__0));
      send_not = ZB_TRUE;
      ZB_SCHEDULE_CALLBACK(zb_zcl_ias_zone_send_status_change_not, buf_param);
    }
  } /* zone state == ENROLLED */

  /* NK:WARNING: Do not free the buffer here, even if notification is not sent! It is application responsibility! */
  TRACE_MSG(TRACE_ZCL1, "<< zb_zcl_ias_zone_check_attr_notify send_not %hd", (FMT__H, send_not));
  return send_not;
}


void zb_zcl_ias_zone_send_status_change_not(zb_uint8_t param)
{
  zb_zcl_ias_zone_notification_param_t *notify_param =
    ZB_BUF_GET_PARAM(param, zb_zcl_ias_zone_notification_param_t);
  zb_zcl_attr_t *attr_desc;
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_ias_zone_int_ctx_t *int_ctx;
  zb_uint8_t endpoint;
  zb_uint16_t zone_status;
  zb_uint16_t delay;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_send_status_change_not param %d",
      (FMT__H, param));

  endpoint = notify_param->ep;
  zone_status = notify_param->status_val;
  delay = notify_param->delay;
  ep_desc = zb_af_get_endpoint_desc(endpoint);


  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                     ZB_ZCL_CLUSTER_ID_IAS_ZONE,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID);
  ZB_ASSERT(attr_desc != NULL);
  int_ctx = (zb_zcl_ias_zone_int_ctx_t*)attr_desc->data_p;


#if defined ZB_ENABLE_HA
  {
    zb_uint8_t zone_id;
    zb_uint8_t cie_ep;
    zb_uint16_t cie_addr;

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_IAS_ZONE_ZONEID_ID);
    zone_id = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_CUSTOM_CIE_EP);
    cie_ep = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_IAS_ZONE,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR);
    cie_addr = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    TRACE_MSG(TRACE_ZCL1, "send notification cie addr %d, ep %hd", (FMT__D_H, cie_addr, cie_ep));
    ZB_ZCL_IAS_ZONE_SEND_STATUS_CHANGE_NOTIFICATION_REQ(
      param,
      cie_addr, ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT, //ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      cie_ep, endpoint, ep_desc->profile_id, int_ctx->process_result_cb,
      zone_status, 0, zone_id,
      delay);


  }

#else /* defined ZB_ENABLE_HA */

  ZB_ZCL_IAS_ZONE_SEND_STATUS_CHANGE_NOTIFICATION_REQ(
    param,
    zb_get_short_address(), ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
    0, endpoint, ep_desc->profile_id, int_ctx->process_result_cb,
    zone_status, 0);

#endif /* defined ZB_ENABLE_HA*/

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_send_status_change_not", (FMT__0));
}

/**********************************************************************/

/**
 *  @} internal
*/


void zb_zcl_ias_zone_register_cb(
    zb_uint8_t endpoint,
    zb_callback_t process_result_cb,
    zb_ias_zone_app_callback_t general_cb)
{
  zb_zcl_attr_t* attr_desc;
  zb_zcl_ias_zone_int_ctx_t  *int_ctx;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_queue_registry ep %hx", (FMT__H, endpoint));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                     ZB_ZCL_CLUSTER_ID_IAS_ZONE,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID);
  ZB_ASSERT(attr_desc != NULL);

  int_ctx = (zb_zcl_ias_zone_int_ctx_t*)attr_desc->data_p;
  int_ctx->process_result_cb = process_result_cb;
  int_ctx->general_cb = general_cb;

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_queue_registry", (FMT__0));
}

static void handle_bind_confirm(zb_uint8_t param)
{
  if (zb_buf_get_status(param) != RET_OK)
  {
    TRACE_MSG(TRACE_ZCL1, "Failed to create CIE apt binding!", (FMT__0));
  }

  zb_buf_free(param);
}

static void handle_bind_check_response(zb_bufid_t param)
{
  zb_aps_check_binding_resp_t *check_binding_resp = NULL;

  check_binding_resp = ZB_BUF_GET_PARAM(param, zb_aps_check_binding_resp_t);

  if (check_binding_resp->exists == ZB_TRUE)
  {
    TRACE_MSG(TRACE_ZCL3, "CIE binding already exists!", (FMT__0));
    zb_buf_free(param);
  }
  else
  {
    /* zb_zcl_set_attr_val_cmd_post_process put zdo bind request params into the buffer body */
    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zdo_bind_req_param_t),
              zb_buf_begin(param),
              sizeof(zb_zdo_bind_req_param_t));

    zb_zdo_bind_req(param, handle_bind_confirm);
  }
}

void zb_zcl_ias_zone_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  zb_zcl_attr_t* attr_desc;
  zb_zcl_ias_zone_int_ctx_t  *int_ctx;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_ias_zone_write_attr_hook endpoint %hx attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D, endpoint, attr_id, manuf_code));

  attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                     ZB_ZCL_CLUSTER_ID_IAS_ZONE,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_IAS_ZONE_INT_CTX_ID);
  ZB_ASSERT(attr_desc != NULL);

  ZB_ASSERT(new_value);

  int_ctx = (zb_zcl_ias_zone_int_ctx_t*)attr_desc->data_p;

  if(attr_id == ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID)
  {
    if (!ZB_IEEE_ADDR_IS_ZERO(new_value))
    {
      TRACE_MSG(TRACE_ZCL1, "valid CIE addr is being set", (FMT__0));
      attr_desc = zb_zcl_get_attr_desc_a(endpoint,
                                     ZB_ZCL_CLUSTER_ID_IAS_ZONE,
                                     ZB_ZCL_CLUSTER_SERVER_ROLE,
                                     ZB_ZCL_ATTR_CUSTOM_CIE_ADDR_IS_SET);
      ZB_ASSERT(attr_desc != NULL);

      if (ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) == ZB_B2U(ZB_FALSE))
      {
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, ZB_B2U(ZB_TRUE));
      }
      if (int_ctx->general_cb)
      {
        int_ctx->general_cb(ZB_ZCL_VALID_CIE_ADDR_SET, 0);
      }
    }
  }
  else if (attr_id == ZB_ZCL_ATTR_IAS_ZONE_CURRENT_ZONE_SENSITIVITY_LEVEL_ID)
  {
    if (int_ctx->general_cb)
    {
      int_ctx->general_cb(ZB_ZCL_CURR_ZONE_SENSITIVITY_LEVEL_SET, *new_value);
    }
  }
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_ias_zone_write_attr_hook", (FMT__0));
}

void zb_zcl_ias_zone_check_cie_addr_on_zcl_initialization(zb_uint8_t ep_id)
{
  zb_ret_t ret;
  zb_ieee_addr_t zeroed_ieee_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  ret = zb_aps_add_binding_whitelist_entry(ep_id, ZB_ZCL_CLUSTER_ID_IAS_ZONE, ZB_ZCL_CLUSTER_SERVER_ROLE, zeroed_ieee_addr);
  if (ret != RET_OK)
  {
    TRACE_MSG(TRACE_ERROR, "Add binding whitelist zeroed IEEE addr entry for ias zone failed ret=%hu", (FMT__D, ret));
    ZB_ASSERT(0);
  }
}

void zb_zcl_ias_set_attr_val_post_process(zb_zcl_parsed_hdr_t *cmd_info, zb_uint16_t attr_id, zb_uint8_t *value)
{
  ZVUNUSED(cmd_info);
  ZVUNUSED(attr_id);
  ZVUNUSED(value);

  if (attr_id == ZB_ZCL_ATTR_IAS_ZONE_IAS_CIE_ADDRESS_ID)
  {
    /* ZCL 8.2.2.2.3 (Trip-to-Pair, Auto-Enroll-Response, Auto-Enroll-Request):
     * "The IAS Zone server MAY configure a binding table entry for the IAS CIE’s
     * address because all of its communication will be directed to the IAS CIE." */
    zb_zdo_bind_req_param_t *zdo_bind_req;
    zb_aps_check_binding_req_t *check_binding_req;
    zb_zcl_attr_t *attr_desc;
    zb_ret_t ret;

    zb_bufid_t buf = zb_buf_get_out();

    ZB_ASSERT(buf != ZB_BUF_INVALID);

    attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                       cmd_info->cluster_id, ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id);

    zdo_bind_req = (zb_zdo_bind_req_param_t *)zb_buf_initial_alloc(buf, sizeof(*zdo_bind_req));

    ZB_BZERO(zdo_bind_req, sizeof(zb_zdo_bind_req_param_t));

    /* get "set CIE address" originator's ieee address */

    ret = zb_address_ieee_by_short(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr,
                                   zdo_bind_req->dst_address.addr_long);

    /* If address table entry doesn't exist, it should be added */

    if (ret == RET_NOT_FOUND)
    {
      zb_address_ieee_ref_t addr_ref;
      zb_address_update(value, ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr, ZB_TRUE, &addr_ref);
    }
    /* Compare "set CIE address" originator's ieee address to
     * CIE address from request and to current CIE ieee address
     * in IAS Zone Cluster's attribute */

    if (ret != RET_OK)
    {
      TRACE_MSG(TRACE_ZCL2, "Failed to obtain CIE ieee address from addr map, skip addr check!",
                (FMT__0));

      ZB_IEEE_ADDR_COPY(zdo_bind_req->dst_address.addr_long, value);
      ret = RET_OK;
    }
    else
    {
      if (ZB_MEMCMP(zdo_bind_req->dst_address.addr_long, value,
                    zb_zcl_get_attribute_size(attr_desc->type, value)) != 0)
      {
        ret = RET_ERROR;
      }
    }

    /* Check the attribute was really updated with the new address value.
       If not, don't create a new binding */
    if (ret == RET_OK)
    {
      if (ZB_MEMCMP(attr_desc->data_p, value,
                    zb_zcl_get_attribute_size(attr_desc->type, value)) != 0)
      {
        ret = RET_ERROR;
      }
    }

    if (ret == RET_ERROR)
    {
      TRACE_MSG(TRACE_ZCL2, "Originator's ieee check failed!", (FMT__0));
    }

    ret = zb_zcl_ias_zone_put_cie_address_to_binding_whitelist(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint);

    if (ret == RET_OK)
    {
      attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                         cmd_info->cluster_id, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_EP);
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc,ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint);

      attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint,
                                         cmd_info->cluster_id, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_CUSTOM_CIE_SHORT_ADDR);
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(attr_desc,ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).source.u.short_addr);

      /* Fill in the rest fields of binding request */
      zb_get_long_address(zdo_bind_req->src_address);
      zdo_bind_req->src_endp = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).dst_endpoint;
      zdo_bind_req->dst_endp = ZB_ZCL_PARSED_HDR_SHORT_DATA(cmd_info).src_endpoint;
      zdo_bind_req->cluster_id = cmd_info->cluster_id;
      zdo_bind_req->dst_addr_mode = ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
      zdo_bind_req->req_dst_addr = zb_get_short_address();

      check_binding_req = ZB_BUF_GET_PARAM(buf, zb_aps_check_binding_req_t);
      ZB_BZERO(check_binding_req, sizeof(*check_binding_req));

      check_binding_req->cluster_id = cmd_info->cluster_id;
      check_binding_req->src_endpoint = ZB_ZCL_BROADCAST_ENDPOINT;
      check_binding_req->response_cb = handle_bind_check_response;

      zb_aps_check_binding_request(buf);
      buf = ZB_BUF_INVALID;
    }

    if (buf != ZB_BUF_INVALID)
    {
      zb_buf_free(buf);
    }
  }
}
#endif /* ZB_ZCL_SUPPORT_CLUSTER_IAS_ZONE */
