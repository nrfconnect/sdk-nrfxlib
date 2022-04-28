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
/* PURPOSE: ZCL Level Contol cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2072

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_LEVEL_CONTROL)

#include "zb_zcl.h"

/* #ifdef ZB_ZCL_SUPPORT_CLUSTER_ON_OFF - use it if extra functionality is needed */

#define ZB_LEVEL_CONTROL_TRANSITION_TIME_UNIT ZB_TIME_ONE_SECOND / 10

#define ZB_LEVEL_CONTROL_TRANSITION_TIME_ONE_SECOND 10

#define ZB_LEVEL_CONTROL_GET_TIME_IN_UNITS()    ZB_TIME_BEACON_INTERVAL_TO_MSEC(ZB_TIMER_GET()) / 100

#define ZB_LEVEL_CONTROL_MIN_TRANSITION_TIME 1

zb_uint8_t gs_level_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_LEVEL_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_discover_cmd_list_t gs_level_client_cmd_list =
{
  0, NULL,
  sizeof(gs_level_server_received_commands), gs_level_server_received_commands
};

zb_discover_cmd_list_t gs_level_server_cmd_list =
{
  sizeof(gs_level_server_received_commands), gs_level_server_received_commands,
  0, NULL
};

static void move_to_level_continue(zb_uint8_t param);
static void level_control_stop_internal(zb_uint8_t endpoint);

static zb_ret_t check_value_level_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
static zb_ret_t check_value_level_control_client(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);

zb_bool_t zb_zcl_process_level_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_level_specific_commands_cli(zb_uint8_t param);

void zb_zcl_level_control_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_level_control_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_level_specific_commands_srv);
}

void zb_zcl_level_control_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              check_value_level_control_client,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_level_specific_commands_cli);
}

static zb_ret_t check_value_level_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID:
      if( ZB_ZCL_ATTR_GET16(value) > ZB_ZCL_LEVEL_CONTROL_CLUSTER_REVISION_MAX )
      {
        ret = RET_ERROR;
      }
      break;
    default:
      ret = RET_OK;
      break;
  }

  return ret;
}

static zb_ret_t check_value_level_control_client(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID:
      if( ZB_ZCL_ATTR_GET16(value) > ZB_ZCL_LEVEL_CONTROL_CLUSTER_REVISION_MAX )
      {
        ret = RET_ERROR;
      }
      break;
    default:
      ret = RET_OK;
      break;
  }

  return ret;
}

zb_zcl_level_control_move_variables_t *level_control_get_move_variables(zb_uint8_t endpoint)
{
  zb_zcl_attr_t* move_status_desc;

  move_status_desc = zb_zcl_get_attr_desc_a(endpoint,
                                            ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                            ZB_ZCL_CLUSTER_SERVER_ROLE,
                                            ZB_ZCL_ATTR_LEVEL_CONTROL_MOVE_STATUS_ID);

  return (move_status_desc) ?
    &((zb_zcl_level_control_move_status_t*)move_status_desc->data_p)->move_var :
    NULL;
}

zb_ret_t level_control_value_set_func(zb_uint8_t endpoint, zb_uint16_t* value, zb_uint16_t remaining_time)
{
  zb_zcl_device_callback_param_t *device_cb_param;
  zb_bufid_t  dev_buf = zb_buf_get_out();
  zb_uint8_t current_value8 = *value & 0xff;
  zb_ret_t ret = RET_OK;

  TRACE_MSG(TRACE_ZCL1, "level_control_value_set_func: level %hd remaining_time %d", (FMT__H_D, current_value8, remaining_time));

  ZB_ASSERT(dev_buf != 0);
  ZB_ASSERT(endpoint);
  device_cb_param = ZB_BUF_GET_PARAM(dev_buf, zb_zcl_device_callback_param_t);

  device_cb_param->device_cb_id = ZB_ZCL_LEVEL_CONTROL_SET_VALUE_CB_ID;
  device_cb_param->endpoint = endpoint;
  device_cb_param->cb_param.level_control_set_value_param.new_value = current_value8;

  if (ZCL_CTX().device_cb != NULL)
  {
    (ZCL_CTX().device_cb)(dev_buf);
    ret = device_cb_param->status;
  }

  if (ret == RET_OK)
  {
    zb_zcl_attr_t* remaining_time_desc;
    /* Use SET_ATTRIBUTE to report attribute change if needed. */
    ZB_ZCL_SET_ATTRIBUTE(endpoint,
                         ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID,
                         &current_value8,
                         ZB_FALSE);

    remaining_time_desc = zb_zcl_get_attr_desc_a(endpoint,
                                                 ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                                 ZB_ZCL_CLUSTER_SERVER_ROLE,
                                                 ZB_ZCL_ATTR_LEVEL_CONTROL_REMAINING_TIME_ID);

    if (remaining_time_desc &&
        (ZB_ZCL_GET_ATTRIBUTE_VAL_16(remaining_time_desc) > remaining_time))
    {
      TRACE_MSG(TRACE_ZCL1, "update remaining_time", (FMT__0));
      ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(remaining_time_desc, remaining_time);
    }
  }

  zb_buf_free(dev_buf);
  return ret;
}

static zb_bool_t level_control_check_req_options(zb_uint8_t param, zb_uint8_t endpoint)
{
  zb_zcl_level_control_req_options_t req_options;
  zb_bool_t res = ZB_TRUE;
  zb_zcl_attr_t *attr_desc;
  zb_zcl_level_control_move_variables_t* move_variables = level_control_get_move_variables(endpoint);

#ifdef ZB_ZCL_SUPPORT_CLUSTER_ON_OFF
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
#endif

  ZB_ASSERT(move_variables);

  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint,
    ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_LEVEL_CONTROL_OPTIONS_ID);

  if (attr_desc != NULL)
  {
    zb_uint8_t options = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

    ZB_ZCL_LEVEL_CONTROL_GET_CMD_OPTIONS(param, req_options);

    /* Check and override bits if needed */
    if (ZB_CHECK_BIT_IN_BIT_VECTOR(&req_options.options_mask,
                                   ZB_ZCL_LEVEL_CONTROL_OPTIONS_EXECUTE_IF_OFF))
    {
      if (ZB_CHECK_BIT_IN_BIT_VECTOR(&req_options.options_override,
                                     ZB_ZCL_LEVEL_CONTROL_OPTIONS_EXECUTE_IF_OFF))
      {
        ZB_SET_BIT_IN_BIT_VECTOR(&options,
                                 ZB_ZCL_LEVEL_CONTROL_OPTIONS_EXECUTE_IF_OFF);
      }
      else
      {
        ZB_CLR_BIT_IN_BIT_VECTOR(&options,
                                 ZB_ZCL_LEVEL_CONTROL_OPTIONS_EXECUTE_IF_OFF);
      }
    }
  
#ifdef ZB_ZCL_SUPPORT_CLUSTER_ON_OFF
    /* ZCL8, 3.10.2.2.8.1 ExecuteIfOff Options Bit
       Command execution SHALL NOT continue beyond the Options procedding if all of thee criteria
       are true:
       - The command is on of the "without On/Off" commands
       - The On/Off cluster exists on the same endpoint as this cluster
       - The OnOff attribute of the On/Off cluster, on this endpoint, is 0x00 (FALSE)
       - The value of the ExecuteIfOff bit is 0
     */

    if (move_variables->addr.cmd_id == ZB_ZCL_CMD_LEVEL_CONTROL_MOVE_TO_LEVEL ||
        move_variables->addr.cmd_id == ZB_ZCL_CMD_LEVEL_CONTROL_MOVE ||
        move_variables->addr.cmd_id == ZB_ZCL_CMD_LEVEL_CONTROL_STEP ||
        move_variables->addr.cmd_id == ZB_ZCL_CMD_LEVEL_CONTROL_STOP)
    {
      ep_desc = zb_af_get_endpoint_desc(endpoint);
      cluster_desc = get_cluster_desc(ep_desc, ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE);
      /* On/Off cluster may not be on the endpoint.
       * In this case return true and continue command execution .*/
      if (cluster_desc)
      {
        attr_desc = zb_zcl_get_attr_desc(cluster_desc, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);

        if (attr_desc &&
            !ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) && /* On/Off attribute is Off */
            !ZB_CHECK_BIT_IN_BIT_VECTOR(&options, ZB_ZCL_LEVEL_CONTROL_OPTIONS_EXECUTE_IF_OFF)) /* ExecuteIfOff
                                                                                               * is False */
        {
          /* ExecuteIfOff policy, deny */
          res = ZB_FALSE;
        }
      }
    }
#endif
  }

  TRACE_MSG(TRACE_ZCL1, "level_control_check_req_options: enabled %hd", (FMT__H, res));
  return res;
}

void level_control_default_resp(zb_uint8_t param, zb_uint16_t user_param)
{
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_uint8_t endpoint = ZB_GET_HI_BYTE(user_param);
  zb_uint8_t status = ZB_GET_LOW_BYTE(user_param);

  TRACE_MSG(TRACE_ZCL1, "level_control_default_resp param %hd user_param %hd %hd", (FMT__H_H_H, param, endpoint, status));

  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  ZB_ZCL_SEND_DEFAULT_RESP(param,
                           move_variables->addr.src_addr,
                           ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                           move_variables->addr.src_endpoint,
                           move_variables->addr.dst_endpoint,
                           move_variables->addr.profile_id,
                           ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                           move_variables->addr.seq_number,
                           move_variables->addr.cmd_id,
                           (status ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL));
}

zb_bool_t level_control_calculate_and_start_cvc(zb_zcl_cvc_input_variables_t *input_var, zb_bool_t is_onoff, zb_uint8_t endpoint)
{
  zb_bool_t status = ZB_TRUE;
  zb_uint8_t alarm_buf_id;
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_bool_t on_value = ZB_TRUE;
  zb_uint8_t alarm_id;

  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  alarm_buf_id = zb_zcl_cvc_calculate_transition_values(input_var);

  status = (alarm_buf_id != ZB_UNDEFINED_BUFFER) ? ZB_TRUE : ZB_FALSE;

  if (status == ZB_TRUE)
  {
    zb_zcl_cvc_variables_t* change_var =
      ZB_BUF_GET_PARAM((alarm_buf_id), zb_zcl_cvc_variables_t);

    if (is_onoff && (change_var->input_var.end_value16 > ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE))
    {
      /* for with_On/Off commands need to send On if necessary,
         see ZCL spec, 3.10.2.3.5 */
      TRACE_MSG(TRACE_ZCL1, "with ON/OFF cmd, set ON", (FMT__0));
      ZB_ZCL_SET_ATTRIBUTE(move_variables->addr.dst_endpoint,
                           ZB_ZCL_CLUSTER_ID_ON_OFF,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                           (zb_uint8_t*)&on_value,
                           ZB_FALSE);
    }

    alarm_id = zb_zcl_cvc_start_alarm(move_variables->addr.dst_endpoint,
                                      ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                      ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID,
                                      alarm_buf_id);

    status = (alarm_id != ZB_ZCL_CVC_INVALID_ALARM_ID) ? ZB_TRUE : ZB_FALSE;

    if (status)
    {
      zb_uint16_t remaining_time = 0;

      if (change_var->steps_number > 1)
      {
        remaining_time = change_var->input_var.transition_time;
      }
      ZB_ZCL_SET_ATTRIBUTE(endpoint,
                           ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           ZB_ZCL_ATTR_LEVEL_CONTROL_REMAINING_TIME_ID,
                           (zb_uint8_t*)&remaining_time,
                           ZB_FALSE);
    }
    else
    {
      zb_buf_free(alarm_buf_id);
    }
  }

  return status;
}

static void move_to_level_handler(
  zb_uint8_t param, zb_bool_t is_onoff, zb_uint8_t endpoint)
{
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_zcl_level_control_move_to_level_req_t move_to_level_req;
  zb_bool_t status;
  zb_zcl_cvc_input_variables_t input_var;
  zb_uint8_t min_value = ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE;
  zb_uint8_t max_value = ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;


  TRACE_MSG(TRACE_ZCL1, "> move_to_level_handler param %i", (FMT__H, param));

  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  ZB_ZCL_LEVEL_CONTROL_GET_MOVE_TO_LEVEL_REQ(param, move_to_level_req, status);

  if (status == ZB_TRUE)
  {
    if (!level_control_check_req_options(param, endpoint))
    {
      TRACE_MSG(TRACE_ZCL1, "Command execution SHALL NOT continue beyond the Options processing", (FMT__0));
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1,
              "ZB_ZCL_LEVEL_CONTROL_GET_MOVE_TO_LEVEL_REQ: new level is %i",
              (FMT__H, move_to_level_req.level));

      TRACE_MSG(TRACE_ZCL1,
              "ZB_ZCL_LEVEL_CONTROL_GET_MOVE_TO_LEVEL_REQ: transition time is %d",
              (FMT__D, move_to_level_req.transition_time));

      level_control_stop_internal(endpoint);

      if (move_to_level_req.transition_time != 0)
      {
        input_var.transition_time = move_to_level_req.transition_time;
      }
      else
      {
        /*If Transition Time given is 0, then need to to move as fast as able */
        input_var.transition_time = ZB_ZCL_LEVEL_CONTROL_TRANSITION_TIME_AS_FAST_AS_ABLE;
      };

      move_variables->is_onoff = is_onoff;

      input_var.current_value16 = move_variables->curr_level;
      input_var.end_value16 = move_to_level_req.level;

      /* This needed to set OFF from move_to_level_continue() when necessary */
      move_variables->end_level = move_to_level_req.level;

      input_var.buf_id = 0;
      input_var.after_processing_cb = move_to_level_continue;
      input_var.value_set_func = level_control_value_set_func;

      input_var.min_value16 = min_value;
      input_var.max_value16 = max_value;
      input_var.overlap = ZB_FALSE;

      status = level_control_calculate_and_start_cvc(&input_var, is_onoff, endpoint);
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "cant read Move REQ", (FMT__0));
  }

  if (!(status && move_variables->addr.disable_default_response))
  {
    zb_uint16_t user_param = status;

    ZB_SET_HI_BYTE(user_param, move_variables->addr.dst_endpoint);
    level_control_default_resp(param, user_param);
  }
  else if (param != 0)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< move_to_level_handler", (FMT__0));
}


static void move_to_level_continue(zb_uint8_t endpoint)
{
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_uint8_t off_value = 0;

  TRACE_MSG(TRACE_ZCL1, "move_to_level_continue ep %hd", (FMT__H, endpoint));
  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  if (move_variables->is_onoff &&
      move_variables->end_level == ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE)
  {

    ZB_ZCL_SET_ATTRIBUTE(move_variables->addr.dst_endpoint,
                         ZB_ZCL_CLUSTER_ID_ON_OFF,
                         ZB_ZCL_CLUSTER_SERVER_ROLE,
                         ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
                         &off_value,
                         ZB_FALSE);
  }
}


static void move_handler(zb_uint8_t param, zb_bool_t is_onoff, zb_uint8_t endpoint)
{
  zb_zcl_level_control_move_req_t move_req;
  zb_int16_t tmp;
  zb_uint8_t diff;
  zb_bool_t status;
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_zcl_cvc_input_variables_t input_var;
  zb_uint8_t min_value = ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE;
  zb_uint8_t max_value = ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;

  TRACE_MSG(TRACE_ZCL1, "> move_handler", (FMT__0));

  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  ZB_ZCL_LEVEL_CONTROL_GET_MOVE_REQ(param, move_req, status);

  if (status == ZB_TRUE)
  {
    if (!level_control_check_req_options(param, endpoint))
    {
      TRACE_MSG(TRACE_ZCL1, "Command execution SHALL NOT continue beyond the Options processing", (FMT__0));
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "move_mode = %i", (FMT__H, move_req.move_mode));
      TRACE_MSG(TRACE_ZCL1, "rate = %i", (FMT__H, move_req.rate));

      level_control_stop_internal(endpoint);

      if (move_req.move_mode <= ZB_ZCL_LEVEL_CONTROL_MOVE_MODE_DOWN)
      {
        move_variables->is_onoff = is_onoff;

        /* end_level for Move command is MIN_VALUE or MAX_VALUE */
        if (!move_req.move_mode)
        {
          move_variables->end_level = ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;
        }
        else
        {
          move_variables->end_level = ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE;
        }

        TRACE_MSG(TRACE_ZCL1, "end_level = %i", (FMT__H, move_variables->end_level));

        /* If Rate is 0xff, need to move fast as able, so transition time setting to minimum,
           see ZCL spec */

        if (move_req.rate == ZB_ZCL_LEVEL_CONTROL_RATE_AS_FAST_AS_ABLE)
        {
          /* Its minimal transition time */
          input_var.transition_time = ZB_ZCL_LEVEL_CONTROL_TRANSITION_TIME_AS_FAST_AS_ABLE;
        }
        else
        {
          tmp = move_variables->curr_level - move_variables->end_level;

          tmp = ZB_ABS(tmp);
          diff = (zb_uint8_t)tmp;

          /* NK: we can work with 1/10 of second already here, to increase accuracy of Move command */
          input_var.transition_time = (diff * ZB_LEVEL_CONTROL_TRANSITION_TIME_ONE_SECOND) / move_req.rate;

          /* Move command operates with seconds, when transition_time is in 1/10 of second.
             If rate is greater then diff, need to do one transition (1 second) for setting level */
          if (!input_var.transition_time)
          {
            input_var.transition_time = ZB_LEVEL_CONTROL_TRANSITION_TIME_ONE_SECOND;
          }
        }

        TRACE_MSG(TRACE_ZCL1,
                  "Redirect to Move to Level: level = %i",
                  (FMT__D, move_variables->end_level));

        TRACE_MSG(TRACE_ZCL1,
                  "Redirect to Move to Level: transition time = %d",
                  (FMT__D, input_var.transition_time));


        //TODO: CVC start
        input_var.current_value16 = move_variables->curr_level;
        input_var.end_value16 = move_variables->end_level;

        input_var.buf_id = 0;
        input_var.after_processing_cb = move_to_level_continue;
        input_var.value_set_func = level_control_value_set_func;

        input_var.min_value16 = min_value;
        input_var.max_value16 = max_value;
        input_var.overlap = ZB_FALSE;

        status = level_control_calculate_and_start_cvc(&input_var, is_onoff, endpoint);
      }
      else
      {
        TRACE_MSG(TRACE_ERROR, "Mode parameter is invalid", (FMT__0));
        status = ZB_FALSE;
      }
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "cant read Move REQ", (FMT__0));
  }

  if (!(status && move_variables->addr.disable_default_response))
  {
    zb_uint16_t user_param = status;

    ZB_SET_HI_BYTE(user_param, move_variables->addr.dst_endpoint);
    level_control_default_resp(param, user_param);
  }
  else if (param != 0)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< move_handler", (FMT__0));
}

static void step_handler(
  zb_uint8_t param, zb_bool_t is_onoff, zb_uint8_t endpoint)
{
  zb_zcl_level_control_step_req_t step_req;
  zb_int16_t tmp;
  zb_uint8_t diff;
  zb_bool_t status;
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_zcl_cvc_input_variables_t input_var;
  zb_uint8_t min_value = ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE;
  zb_uint8_t max_value = ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;

  TRACE_MSG(TRACE_ZCL1, "> step_handler", (FMT__0));

  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  ZB_ZCL_LEVEL_CONTROL_GET_STEP_REQ(param, step_req, status);

  if (status == ZB_TRUE)
  {
    if (!level_control_check_req_options(param, endpoint))
    {
      TRACE_MSG(TRACE_ZCL1, "Command execution SHALL NOT continue beyond the Options processing", (FMT__0));
    }
    else
    {
      TRACE_MSG(TRACE_ZCL1, "step_mode = %i", (FMT__H, step_req.step_mode));
      TRACE_MSG(TRACE_ZCL1, "step_size = %i", (FMT__H, step_req.step_size));
      TRACE_MSG(TRACE_ZCL1, "transition_time = %d", (FMT__D, step_req.transition_time));

      level_control_stop_internal(endpoint);

      if (step_req.step_mode <= ZB_ZCL_LEVEL_CONTROL_STEP_MODE_DOWN)
      {
        move_variables->is_onoff = is_onoff;

        if (step_req.transition_time == 0)
        {
          /*If Transition Time given is 0, then need to to move as fast as able */
          input_var.transition_time = ZB_ZCL_LEVEL_CONTROL_TRANSITION_TIME_AS_FAST_AS_ABLE;
        }

        if (!step_req.step_mode)
        {
          tmp = move_variables->curr_level + step_req.step_size;
          if (tmp > ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE)
          {
            move_variables->end_level = ZB_ZCL_LEVEL_CONTROL_LEVEL_MAX_VALUE;
            diff = move_variables->end_level - move_variables->curr_level;

            input_var.transition_time = diff * step_req.transition_time / step_req.step_size + 1;
          }
          else
          {
            move_variables->end_level = tmp;
            input_var.transition_time = step_req.transition_time;
          }
        }
        else
        {
          tmp = move_variables->curr_level - step_req.step_size;
          if (tmp < ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE)
          {
            move_variables->end_level = ZB_ZCL_LEVEL_CONTROL_LEVEL_MIN_VALUE;
            diff = move_variables->curr_level - move_variables->end_level;
            input_var.transition_time = diff * step_req.transition_time / step_req.step_size + 1;
          }
          else
          {
            move_variables->end_level = tmp;
            input_var.transition_time = step_req.transition_time;
          }
        }

        TRACE_MSG(TRACE_ZCL1, "end level = %i", (FMT__H, move_variables->end_level));
        TRACE_MSG(TRACE_ZCL1,
                  "Redirect to Move to Level: level = %i",
                  (FMT__D, move_variables->end_level));

        TRACE_MSG(TRACE_ZCL1,
                  "Redirect to Move to Level: transition time = %d",
                  (FMT__D, input_var.transition_time));

        input_var.current_value16 = move_variables->curr_level;
        input_var.end_value16 = move_variables->end_level;

        input_var.buf_id = 0;
        input_var.after_processing_cb = move_to_level_continue;
        input_var.value_set_func = level_control_value_set_func;

        input_var.min_value16 = min_value;
        input_var.max_value16 = max_value;
        input_var.overlap = ZB_FALSE;

        status = level_control_calculate_and_start_cvc(&input_var, is_onoff, endpoint);
      }
      else
      {
        TRACE_MSG(TRACE_ERROR, "step_mode parameter is invalid", (FMT__0));
        status = ZB_FALSE;
      }
    }
  }
  else
  {
    TRACE_MSG(TRACE_ERROR, "cant read Step REQ", (FMT__0));
  }

  if (!(status && move_variables->addr.disable_default_response))
  {
    zb_uint16_t user_param = status;

    ZB_SET_HI_BYTE(user_param, move_variables->addr.dst_endpoint);
    level_control_default_resp(param, user_param);
  }
  else if (param != 0)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< step_handler", (FMT__0));
}

static void level_control_stop_internal(zb_uint8_t endpoint)
{
  zb_zcl_level_control_move_status_t* move_status;
  zb_zcl_attr_t* move_status_desc;
  zb_uint8_t alarm_id;
  zb_uint16_t remaining_time = 0;

  ZB_ZCL_SET_ATTRIBUTE(endpoint,
                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_LEVEL_CONTROL_REMAINING_TIME_ID,
                       (zb_uint8_t*)&remaining_time,
                       ZB_FALSE);

  //
  move_status_desc = zb_zcl_get_attr_desc_a(endpoint,
                                            ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                            ZB_ZCL_CLUSTER_SERVER_ROLE,
                                            ZB_ZCL_ATTR_LEVEL_CONTROL_MOVE_STATUS_ID);

  ZB_ASSERT(move_status_desc != NULL);

  move_status = (zb_zcl_level_control_move_status_t*)move_status_desc->data_p;

  alarm_id = zb_zcl_cvc_check_transition_running(move_status->move_var.addr.dst_endpoint,
                                                 ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                                 ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID);

  if (alarm_id != ZB_ZCL_CVC_INVALID_ALARM_ID)
  {
    zb_zcl_cvc_stop_transition(alarm_id);
    TRACE_MSG(TRACE_ZCL3, "prev command killed", (FMT__0));
  }
}

static void stop_handler(zb_uint8_t param, zb_uint8_t endpoint)
{

  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_uint8_t status = ZB_TRUE;

  TRACE_MSG(TRACE_ZCL1, "> stop_handler param %i", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG(TRACE_ZCL1, "cmd_id is %i", (FMT__H, cmd_info.cmd_id));

  move_variables = level_control_get_move_variables(endpoint);
  ZB_ASSERT(move_variables);

  if (level_control_check_req_options(param, endpoint))
  {
    level_control_stop_internal(endpoint);
  }
  else
  {
    TRACE_MSG(TRACE_ZCL1, "Command execution SHALL NOT continue beyond the Options processing", (FMT__0));
  }

  if (!(status && move_variables->addr.disable_default_response))
  {
    zb_uint16_t user_param = status;

    ZB_SET_HI_BYTE(user_param, move_variables->addr.dst_endpoint);
    level_control_default_resp(param, user_param);
  }
  else if (param != 0)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< stop_handler", (FMT__0));
}

zb_bool_t zb_zcl_process_level_control_specific_commands(zb_uint8_t param)
{
  zb_zcl_attr_t* curr_level_desc;
  zb_zcl_attr_t* move_status_desc;
  zb_zcl_level_control_move_variables_t* move_variables;
  zb_uint8_t curr_level;
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_zcl_level_control_move_addr_t main_addr;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_level_control_specific_commands: param %d, cmd %d",
      (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  main_addr.src_addr = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr;
  main_addr.src_endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint;
  main_addr.dst_endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
  main_addr.cmd_id = cmd_info.cmd_id;
  main_addr.seq_number = cmd_info.seq_number;
  main_addr.disable_default_response = (zb_bool_t)cmd_info.disable_default_response;
  main_addr.profile_id = cmd_info.profile_id;

  curr_level_desc = zb_zcl_get_attr_desc_a(main_addr.dst_endpoint,
                                           ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                                           ZB_ZCL_ATTR_LEVEL_CONTROL_CURRENT_LEVEL_ID);

  move_status_desc = zb_zcl_get_attr_desc_a(main_addr.dst_endpoint,
                                            ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                            ZB_ZCL_CLUSTER_SERVER_ROLE,
                                            ZB_ZCL_ATTR_LEVEL_CONTROL_MOVE_STATUS_ID);

  ZB_ASSERT(move_status_desc != NULL);
  ZB_ASSERT(curr_level_desc != NULL);

  curr_level = *(zb_uint8_t*)curr_level_desc->data_p;

  TRACE_MSG(TRACE_ZCL1, "current level is %i", (FMT__H, curr_level));

  move_variables = level_control_get_move_variables(main_addr.dst_endpoint);
  ZB_ASSERT(move_variables);

  move_variables->curr_level = curr_level;

  ZB_MEMCPY(&(move_variables->addr), &main_addr, sizeof(zb_zcl_level_control_move_addr_t));

  switch (main_addr.cmd_id)
  {
    case ZB_ZCL_CMD_LEVEL_CONTROL_MOVE_TO_LEVEL:
      move_to_level_handler(param, ZB_FALSE, main_addr.dst_endpoint);
      TRACE_MSG(TRACE_ZCL3, "Processed MOVE_TO_LEVEL command", (FMT__0));
      break;

    case ZB_ZCL_CMD_LEVEL_CONTROL_MOVE_TO_LEVEL_WITH_ON_OFF:
      move_to_level_handler(param, ZB_TRUE, main_addr.dst_endpoint);
      TRACE_MSG(TRACE_ZCL3, "Processed MOVE_TO_LEVEL_WITH_ON_OFF command", (FMT__0));
      break;

    case ZB_ZCL_CMD_LEVEL_CONTROL_MOVE:
      move_handler(param, ZB_FALSE, main_addr.dst_endpoint);
      TRACE_MSG(TRACE_ZCL3, "Processed MOVE command", (FMT__0));
      break;

    case ZB_ZCL_CMD_LEVEL_CONTROL_MOVE_WITH_ON_OFF:
      move_handler(param, ZB_TRUE, main_addr.dst_endpoint);
      TRACE_MSG(TRACE_ZCL3, "Processed MOVE_WITH_ON_OFF command", (FMT__0));
      break;

    case ZB_ZCL_CMD_LEVEL_CONTROL_STEP:
      step_handler(param, ZB_FALSE, main_addr.dst_endpoint);
      TRACE_MSG(TRACE_ZCL3, "Processed STEP command", (FMT__0));
      break;

    case ZB_ZCL_CMD_LEVEL_CONTROL_STEP_WITH_ON_OFF:
      step_handler(param, ZB_TRUE, main_addr.dst_endpoint);
      TRACE_MSG(TRACE_ZCL3, "Processed STEP_WITH_ON_OFF command", (FMT__0));
      break;

    case ZB_ZCL_CMD_LEVEL_CONTROL_STOP:
    case ZB_ZCL_CMD_LEVEL_CONTROL_STOP_WITH_ON_OFF:
    {
      zb_zcl_level_control_move_addr_t *addr =
        ZB_BUF_GET_PARAM(param, zb_zcl_level_control_move_addr_t);

      ZB_MEMCPY(addr, &main_addr, sizeof(zb_zcl_level_control_move_addr_t));
      stop_handler(param, main_addr.dst_endpoint);

      TRACE_MSG(TRACE_ZCL3, "STOP command processed", (FMT__0));
    }
    break;

    default:
      processed = ZB_FALSE;
      break;
  }

  TRACE_MSG(TRACE_ZCL1,
             "< zb_zcl_process_level_control_specific_commands: processed %d",
             (FMT__D, processed));

  return processed;
}

zb_bool_t zb_zcl_process_level_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_level_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_level_control_specific_commands(param);
}

zb_bool_t zb_zcl_process_level_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_level_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_level_control_specific_commands(param);
}

static inline void zb_zcl_level_control_send_move_cmd_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                           zb_uint8_t ep, zb_uint16_t prof_id,
                                                           zb_uint8_t def_resp, zb_callback_t cb,
                                                           zb_uint8_t move_mode, zb_uint8_t rate,
                                                           zb_uint8_t cmd_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), cmd_id);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

static inline void zb_zcl_level_control_send_move_cmd_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                             zb_uint8_t def_resp, zb_callback_t cb,
                                             zb_uint8_t move_mode, zb_uint8_t rate,
                                             zb_uint8_t options_mask, zb_uint8_t options_override,
                                             zb_uint8_t cmd_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), cmd_id);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

void zb_zcl_level_control_send_move_cmd_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                             zb_uint8_t def_resp, zb_callback_t cb,
                                             zb_uint8_t move_mode, zb_uint8_t rate,
                                             zb_uint8_t options_mask, zb_uint8_t options_override,
                                             zb_uint8_t cmd_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_move_cmd_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_LEVEL_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_move_cmd_rev1(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              move_mode, rate,
                                              cmd_id);
      break;
    case 3:
      zb_zcl_level_control_send_move_cmd_rev3(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              move_mode, rate,
                                              options_mask, options_override,
                                              cmd_id);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_move_cmd_zcl8", (FMT__0));
}

void zb_zcl_level_control_send_move_cmd(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                        zb_uint8_t def_resp, zb_callback_t cb,
                                        zb_uint8_t move_mode, zb_uint8_t rate,
                                        zb_uint8_t cmd_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_move_cmd", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_move_cmd_rev1(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              move_mode, rate,
                                              cmd_id);
      break;
    case 3:
      zb_zcl_level_control_send_move_cmd_rev3(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              move_mode, rate,
                                              ZB_ZCL_LEVEL_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                              ZB_ZCL_LEVEL_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE,
                                              cmd_id);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_move_cmd", (FMT__0));
}

static inline void zb_zcl_level_control_send_move_to_level_cmd_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                    zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                    zb_uint8_t ep, zb_uint16_t prof_id,
                                                                    zb_uint8_t def_resp, zb_callback_t cb,
                                                                    zb_uint8_t level, zb_uint16_t transition_time,
                                                                    zb_uint8_t cmd_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), (cmd_id));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (level));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

static inline void zb_zcl_level_control_send_move_to_level_cmd_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                      zb_uint8_t level, zb_uint16_t transition_time,
                                                      zb_uint8_t options_mask, zb_uint8_t options_override,
                                                      zb_uint8_t cmd_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), (cmd_id));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (level));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

void zb_zcl_level_control_send_move_to_level_cmd_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                      zb_uint8_t level, zb_uint16_t transition_time,
                                                      zb_uint8_t options_mask, zb_uint8_t options_override,
                                                      zb_uint8_t cmd_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_move_to_level_cmd_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_LEVEL_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_move_to_level_cmd_rev1(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       level, transition_time,
                                                       cmd_id);
      break;
    case 3:
      zb_zcl_level_control_send_move_to_level_cmd_rev3(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       level, transition_time,
                                                       options_mask, options_override,
                                                       cmd_id);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_move_to_level_cmd_zcl8", (FMT__0));
}

void zb_zcl_level_control_send_move_to_level_cmd(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                 zb_uint8_t level, zb_uint16_t transition_time,
                                                 zb_uint8_t cmd_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_move_to_level_cmd", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_move_to_level_cmd_rev1(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       level, transition_time,
                                                       cmd_id);
      break;
    case 3:
      zb_zcl_level_control_send_move_to_level_cmd_rev3(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       level, transition_time,
                                                       ZB_ZCL_LEVEL_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                       ZB_ZCL_LEVEL_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE,
                                                       cmd_id);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_move_to_level_cmd", (FMT__0));
}

static inline void zb_zcl_level_control_send_step_cmd_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                           zb_uint8_t ep, zb_uint16_t prof_id,
                                                           zb_uint8_t def_resp, zb_callback_t cb,
                                                           zb_uint8_t step_mode, zb_uint8_t step_size,
                                                           zb_uint16_t transition_time,
                                                           zb_uint8_t cmd_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), cmd_id);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

static inline void zb_zcl_level_control_send_step_cmd_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                             zb_uint8_t def_resp, zb_callback_t cb,
                                             zb_uint8_t step_mode, zb_uint8_t step_size,
                                             zb_uint16_t transition_time,
                                             zb_uint8_t options_mask, zb_uint8_t options_override,
                                             zb_uint8_t cmd_id)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), cmd_id);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

void zb_zcl_level_control_send_step_cmd_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                             zb_uint8_t def_resp, zb_callback_t cb,
                                             zb_uint8_t step_mode, zb_uint8_t step_size,
                                             zb_uint16_t transition_time,
                                             zb_uint8_t options_mask, zb_uint8_t options_override,
                                             zb_uint8_t cmd_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_step_cmd_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_LEVEL_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_step_cmd_rev1(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              step_mode, step_size, transition_time,
                                              cmd_id);
      break;
    case 3:
      zb_zcl_level_control_send_step_cmd_rev3(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              step_mode, step_size, transition_time,
                                              options_mask, options_override,
                                              cmd_id);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_step_cmd_zcl8", (FMT__0));
}

void zb_zcl_level_control_send_step_cmd(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                        zb_uint8_t def_resp, zb_callback_t cb,
                                        zb_uint8_t step_mode, zb_uint8_t step_size,
                                        zb_uint16_t transition_time,
                                        zb_uint8_t cmd_id)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_step_cmd", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_step_cmd_rev1(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              step_mode, step_size, transition_time,
                                              cmd_id);
      break;
    case 3:
      zb_zcl_level_control_send_step_cmd_rev3(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              step_mode, step_size, transition_time,
                                              ZB_ZCL_LEVEL_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                              ZB_ZCL_LEVEL_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE,
                                              cmd_id);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_step_cmd", (FMT__0));
}

static inline void zb_zcl_level_control_send_stop_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                           zb_uint8_t ep, zb_uint16_t prof_id,
                                                           zb_uint8_t def_resp, zb_callback_t cb)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_LEVEL_CONTROL_STOP);
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

static inline void zb_zcl_level_control_send_stop_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                             zb_uint8_t def_resp, zb_callback_t cb,
                                             zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_LEVEL_CONTROL_STOP);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb);
}

void zb_zcl_level_control_send_stop_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                             zb_uint8_t def_resp, zb_callback_t cb,
                                             zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_stop_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_LEVEL_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_stop_req_rev1(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb);
      break;
    case 3:
      zb_zcl_level_control_send_stop_req_rev3(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_stop_req_zcl8", (FMT__0));
}

void zb_zcl_level_control_send_stop_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                        zb_uint8_t def_resp, zb_callback_t cb)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_level_control_send_stop_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_level_control_send_stop_req_rev1(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb);
      break;
    case 3:
      zb_zcl_level_control_send_stop_req_rev3(buffer, dst_addr,
                                              dst_addr_mode, dst_ep,
                                              ep, prof_id,
                                              def_resp, cb,
                                              ZB_ZCL_LEVEL_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                              ZB_ZCL_LEVEL_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_level_control_send_stop_req", (FMT__0));
}

#ifdef ZB_COMPILE_ZCL_SAMPLE

void send_move_to_level_req(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t dst_addr,
                                 zb_uint8_t dst_ep, zb_uint8_t src_ep, zb_uint8_t level,
                                 zb_uint16_t transition_time)
{
  ZB_ZCL_LEVEL_CONTROL_SEND_MOVE_TO_LEVEL_REQ(data_buf,
                                              dst_addr,
                                              ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                              dst_ep,
                                              src_ep,
                                              prof_id, ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                              NULL,
                                              level,
                                              transition_time);
  ZB_ZCL_LEVEL_CONTROL_SEND_MOVE_TO_LEVEL_WITH_ON_OFF_REQ(data_buf,
                                                          dst_addr,
                                                          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                                          dst_ep,
                                                          src_ep,
                                                          prof_id,
                                                          ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                                          NULL,
                                                          level,
                                                          transition_time);
}


void send_move_req(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t dst_addr,
                        zb_uint8_t dst_ep, zb_uint8_t src_ep, zb_uint8_t move_mode,
                        zb_uint8_t rate)
{
  ZB_ZCL_LEVEL_CONTROL_SEND_MOVE_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                     dst_ep, src_ep, prof_id,
                                     ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                     NULL, move_mode, rate);
  ZB_ZCL_LEVEL_CONTROL_SEND_MOVE_WITH_ON_OFF_REQ(data_buf,
                                                 dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                                 dst_ep, src_ep, prof_id,
                                                 ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                                 NULL, move_mode, rate);
}


void send_step_req(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t dst_addr,
                        zb_uint8_t dst_ep, zb_uint8_t src_ep, zb_uint8_t step_mode,
                        zb_uint8_t step_size, zb_uint16_t transition_time)
{
  ZB_ZCL_LEVEL_CONTROL_SEND_STEP_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                               dst_ep, src_ep, prof_id, ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                       NULL, step_mode, step_size, transition_time);
  ZB_ZCL_LEVEL_CONTROL_SEND_STEP_WITH_ON_OFF_REQ(data_buf,
                                                 dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                                 dst_ep, src_ep, prof_id,
                                                 ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                                 NULL, step_mode, step_size, transition_time);
}



void send_stop_req(zb_bufid_t data_buf, zb_uint16_t prof_id, zb_uint16_t dst_addr,
                        zb_uint8_t dst_ep, zb_uint8_t src_ep)
{
  ZB_ZCL_LEVEL_CONTROL_SEND_STOP_REQ(data_buf, dst_addr, ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                                     dst_ep, src_ep, prof_id,
                                     ZB_ZCL_ENABLE_DEFAULT_RESPONSE,
                                     NULL);
}

#endif /* defined ZB_COMPILE_ZCL_SAMPLE */

#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_LEVEL_CONTROL */
