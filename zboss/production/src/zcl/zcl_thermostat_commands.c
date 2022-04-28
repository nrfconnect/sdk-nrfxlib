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
/* PURPOSE: ZCL Thermostat cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2084

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_THERMOSTAT)

#if defined ZB_ENABLE_HA

#include "zb_zcl.h"

  /** @brief Context Thermostat cluster  */
  static zb_zcl_thermostat_context_t g_thermostat_context;

  void zb_zcl_thermostat_shedule_process(zb_uint8_t param);

  zb_uint8_t gs_thermostat_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_thermostat_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_uint8_t gs_thermostat_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_thermostat_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_SERVER_ROLE_GENERATED_CMD_LIST
};

#else /* defined ZB_ENABLE_HA */

  /** @brief Context Thermostat cluster  */
  static zb_zcl_thermostat_context_t g_thermostat_context;

zb_uint8_t gs_thermostat_client_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_CLIENT_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_thermostat_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_CLIENT_ROLE_GENERATED_CMD_LIST
};

zb_uint8_t gs_thermostat_server_received_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_SERVER_ROLE_RECEIVED_CMD_LIST
};

zb_uint8_t gs_thermostat_server_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_THERMOSTAT_SERVER_ROLE_GENERATED_CMD_LIST
};

#endif /* defined ZB_ENABLE_HA*/

zb_discover_cmd_list_t gs_thermostat_client_cmd_list =
{
  sizeof(gs_thermostat_client_received_commands), gs_thermostat_client_received_commands,
  sizeof(gs_thermostat_client_generated_commands), gs_thermostat_client_generated_commands
};

zb_discover_cmd_list_t gs_thermostat_server_cmd_list =
{
  sizeof(gs_thermostat_server_received_commands), gs_thermostat_server_received_commands,
  sizeof(gs_thermostat_server_generated_commands), gs_thermostat_server_generated_commands
};



void zb_zcl_thermostat_init()
{
  zb_uint8_t endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_init", (FMT__0));

  /* Init - cluster-specific part */
#if defined ZB_ENABLE_HA
  if (endpoint)
  {
    zb_uindex_t i;

    for (i=0; i<ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE; i++)
    {
      g_thermostat_context.schedule_table[i].days = ZB_ZCL_THERMOSTAT_WEEKLY_FREE_RECORD;
    }
    g_thermostat_context.setpoint_mode = ZB_ZCL_THERMOSTAT_PROCESS_NONE;

    ZB_SCHEDULE_CALLBACK(zb_zcl_thermostat_shedule_process, ZB_ZCL_THERMOSTAT_PROCESS_NONE);
  }
#endif /* defined ZB_ENABLE_HA */

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_init", (FMT__0));
}

zb_ret_t check_value_thermostat_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_thermostat_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_thermostat_specific_commands_cli(zb_uint8_t param);

void zb_zcl_thermostat_init_server()
{
  if (zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                                  ZB_ZCL_CLUSTER_SERVER_ROLE,
                                  check_value_thermostat_server,
                                  (zb_zcl_cluster_write_attr_hook_t)NULL,
                                  zb_zcl_process_thermostat_specific_commands_srv) == RET_OK)
  {
    zb_zcl_thermostat_init();
  }
}

void zb_zcl_thermostat_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_thermostat_specific_commands_cli);
}

zb_ret_t check_value_thermostat_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  zb_int16_t min_val = 0;
  zb_int16_t max_val = 0;
  zb_bool_t comp = ZB_TRUE;

  ZVUNUSED(endpoint);
  switch( attr_id )
  {
    case ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID:
      min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_MIN_VALUE;
      max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_MAX_VALUE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_OUTDOOR_TEMPERATURE_ID:
      min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OUTDOOR_TEMPERATURE_MIN_VALUE;
      max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OUTDOOR_TEMPERATURE_MAX_VALUE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_ID:
    {
      zb_int8_t min_val_s8;
      zb_int8_t max_val_s8;

      comp = ZB_FALSE;
      // note: value SIGNED, about (0xff-0) !
      min_val_s8 = (zb_int8_t)ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_MIN_VALUE;
      max_val_s8 = (zb_int8_t)ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_MAX_VALUE;

      TRACE_MSG(TRACE_ZCL1, "int8 comp min %i max %i val %i", (FMT__H_H_H, min_val, max_val, *((zb_int8_t *)value)));
      if (*((zb_int8_t *)value) < min_val_s8 || *((zb_int8_t *)value) > max_val_s8)
      {
        *((zb_int8_t *)value) = (*((zb_int8_t *)value) < min_val) ?
                                (zb_int8_t)ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_MIN_VALUE :
                                (zb_int8_t)ZB_ZCL_THERMOSTAT_LOCAL_TEMPERATURE_CALIBRATION_MAX_VALUE;
      }
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID:
    case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_COOLING_SETPOINT_ID:
    {
      /* First check MinCoolSetpointLimit/MaxCoolSetpointLimit, than
       * AbsMinCoolSetpointLimit/AbsMaxCoolSetpointLimit, than predefined constants. */
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT_ID);
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT_ID);
      if (attr_desc_min && attr_desc_max)
      {
        min_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_min);
        max_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_max);
      }
      else
      {
        attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MIN_COOL_SETPOINT_LIMIT_ID);
        attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MAX_COOL_SETPOINT_LIMIT_ID);

        if (attr_desc_min && attr_desc_max)
        {
          min_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_min);
          max_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_max);
        }
        else
        {
          // note: value SIGNED, about (0xff-0) !
          min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_MIN_VALUE;
          max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_MAX_VALUE;
        }
      }
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID:
    case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID:
    {
      /* First check MinHeatSetpointLimit/MaxHeatSetpointLimit, than
       * AbsMinHeatSetpointLimit/AbsMaxHeatSetpointLimit, than predefined constants. */
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT_ID);
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT_ID);
      if (attr_desc_min && attr_desc_max)
      {
        min_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_min);
        max_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_max);
      }
      else
      {
        attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT_ID);
        attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT_ID);

        if (attr_desc_min && attr_desc_max)
        {
          min_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_min);
          max_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_max);
        }
        else
        {
          // note: value SIGNED, about (0xff-0) !
          min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_MIN_VALUE;
          max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_MAX_VALUE;
        }
      }
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_MIN_HEAT_SETPOINT_LIMIT_ID:
    case ZB_ZCL_ATTR_THERMOSTAT_MAX_HEAT_SETPOINT_LIMIT_ID:
    {
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT_ID);
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT_ID);
      if (attr_desc_min && attr_desc_max)
      {
        min_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_min);
        max_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_max);
      }
      else
      {
        min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_MIN_VALUE;
        max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_MAX_VALUE;
      }
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_MIN_COOL_SETPOINT_LIMIT_ID:
    case ZB_ZCL_ATTR_THERMOSTAT_MAX_COOL_SETPOINT_LIMIT_ID:
    {
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MIN_COOL_SETPOINT_LIMIT_ID);
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_ABS_MAX_COOL_SETPOINT_LIMIT_ID);
      if (attr_desc_min && attr_desc_max)
      {
        min_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_min);
        max_val = (zb_int16_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc_max);
      }
      else
      {
        min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_MIN_VALUE;
        max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_MAX_VALUE;
      }
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID:
      comp = ZB_FALSE;
      ret = (*value < ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_RESERVED) ? RET_OK : RET_ERROR;
      if (ret == RET_OK)
      {
        zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID);

        if ( attr_desc != NULL )
        {
          zb_uint8_t system_mode = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

          switch (*value)
          {
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_ONLY:
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_WITH_REHEAT:
              if (system_mode == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT ||
                  system_mode == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_EMERGENCY_HEATING)
              {
                ret = RET_ERROR;
              }
              break;
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_HEATING_ONLY:
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_HEATING_WITH_REHEAT:
              if (system_mode == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_COOL ||
                  system_mode == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_PRECOOLING)
              {
                ret = RET_ERROR;
              }
              break;

            default:
              // do nothing
              // all other pair Control Seq - System mode are valid
              break;
          }
        }
      }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID:
      comp = ZB_FALSE;
      ret = (*value <= ZB_ZCL_THERMOSTAT_SYSTEM_MODE_SLEEP &&
             *value != 0x02) ? RET_OK : RET_ERROR;  // 0x02 is also reserved

      if (ret == RET_OK)
      {
        zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION_ID);

        if ( attr_desc != NULL )
        {
          zb_uint8_t control_seq_of_operation = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

          switch (control_seq_of_operation)
          {
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_ONLY:
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_COOLING_WITH_REHEAT:
              if (*value == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT ||
                  *value == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_EMERGENCY_HEATING)
              {
                ret = RET_ERROR;
              }
              break;
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_HEATING_ONLY:
            case ZB_ZCL_THERMOSTAT_CONTROL_SEQ_OF_OPERATION_HEATING_WITH_REHEAT:
              if (*value == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_COOL ||
                  *value == ZB_ZCL_THERMOSTAT_SYSTEM_MODE_PRECOOLING)
              {
                ret = RET_ERROR;
              }
              break;
            default:
              // do nothing
              // all other pair Control Seq - System mode are valid
              break;
          }
        }
      }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_START_OF_WEEK_ID:
      ret = (*value <= ZB_ZCL_THERMOSTAT_START_OF_WEEK_SATURDAY) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_MIN_SETPOINT_DEAD_BAND_ID:
      ret = (*value >= ZB_ZCL_THERMOSTAT_MIN_SETPOINT_DEAD_BAND_MIN_VALUE &&
             *value <= ZB_ZCL_THERMOSTAT_MIN_SETPOINT_DEAD_BAND_MAX_VALUE) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_REMOTE_SENSING_ID:
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_ALARM_MASK_ID:
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_RUNNING_MODE_ID:
      ret = (*value == ZB_ZCL_THERMOSTAT_RUNNING_MODE_OFF_VALUE  ||
             *value == ZB_ZCL_THERMOSTAT_RUNNING_MODE_COOL_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_RUNNING_MODE_HEAT_VALUE) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_PI_COOLING_DEMAND_ID:
    {
      zb_uint8_t system_mode;
      zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID);
      ZB_ASSERT(attr_desc);
      system_mode = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
      if (system_mode == (zb_uint8_t)ZB_ZCL_THERMOSTAT_SYSTEM_MODE_OFF ||
          system_mode == (zb_uint8_t)ZB_ZCL_THERMOSTAT_SYSTEM_MODE_HEAT)
      {
        ret = RET_ERROR;
      }
      else
      {
        /* Min value is 0x00 according to ZCL 8 spec. Commented to avoid -Werror=type-limits */
        ret = (/* *value >=  ZB_ZCL_THERMOSTAT_PI_COOLING_DEMAND_MIN_VALUE && */
               *value <=  ZB_ZCL_THERMOSTAT_PI_COOLING_DEMAND_MAX_VALUE) ? RET_OK : RET_ERROR;
      }
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_PI_HEATING_DEMAND_ID:
    {
      zb_uint8_t system_mode;
      zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_SYSTEM_MODE_ID);
      ZB_ASSERT(attr_desc);
      system_mode = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
      if (system_mode == (zb_uint8_t)ZB_ZCL_THERMOSTAT_SYSTEM_MODE_OFF ||
          system_mode == (zb_uint8_t)ZB_ZCL_THERMOSTAT_SYSTEM_MODE_COOL)
      {
        ret = RET_ERROR;
      }
      else
      {
        /* Min value is 0x00 according to ZCL 8 spec. Commented to avoid -Werror=type-limits */
        ret = (/* *value >=  ZB_ZCL_THERMOSTAT_PI_HEATING_DEMAND_MIN_VALUE && */
               *value <=  ZB_ZCL_THERMOSTAT_PI_HEATING_DEMAND_MAX_VALUE) ? RET_OK : RET_ERROR;
      }
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_TEMPERATURE_SETPOINT_HOLD_ID:
      if (*value != (zb_uint8_t)ZB_ZCL_THERMOSTAT_TEMPERATURE_SETPOINT_HOLD_OFF &&
          *value != (zb_uint8_t)ZB_ZCL_THERMOSTAT_TEMPERATURE_SETPOINT_HOLD_OFF)
      {
        ret = RET_ERROR;
      }
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_TEMPERATURE_SETPOINT_HOLD_DURATION_ID:
      /* Min value is 0x0000 according to ZCL 8 spec. Commented to avoid -Werror=type-limits */
      ret = (/* *(zb_uint16_t *)value >=  ZB_ZCL_THERMOSTAT_TEMPERATURE_SETPOINT_HOLD_DURATION_MIN_VALUE && */
             (zb_uint16_t)*value <=  ZB_ZCL_THERMOSTAT_TEMPERATURE_SETPOINT_HOLD_DURATION_MIN_VALUE) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_SETPOINT_CHANGE_SOURCE_TIMESTAMP_ID:
      ret = (*(zb_uint32_t *)value != (zb_uint32_t)-1) ? RET_OK : RET_ERROR;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_MIN_ID:
    {
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_MAX_ID);
      if (*value > ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_max))
      {
        *value = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_max);
      }
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_MAX_ID:
    {
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_MIN_ID);
      if (*value < ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_min))
      {
        *value = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_min);
      }
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_ID:
    {
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_MIN_ID);
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_SETBACK_MAX_ID);

      ret = (*value >= ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_min) && *value <= ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_max)) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_MIN_ID:
    {
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_MAX_ID);
      if (*value > ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_max))
      {
        *value = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_max);
      }
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_MAX_ID:
    {
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_MIN_ID);
      if (*value < ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_min))
      {
        *value = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_min);
      }
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_ID:
    {
      zb_zcl_attr_t *attr_desc_min = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_MIN_ID);
      zb_zcl_attr_t *attr_desc_max = zb_zcl_get_attr_desc_a(endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_SETBACK_MAX_ID);

      ret = (*value >= ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_min) && *value <= ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_max)) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
    }
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_AC_TYPE_ID:
      ret = (*value == ZB_ZCL_THERMOSTAT_AC_TYPE_UNKNOWN_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_TYPE_COOLING_AND_FIXED_SPEED_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_TYPE_HEAT_PUMP_AND_FIXED_SPEED_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_TYPE_COOLING_AND_INVERTER_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_TYPE_HEAT_PUMP_AND_INVERTER_VALUE) ? RET_OK : RET_ERROR;
     comp = ZB_FALSE;
     break;

    case ZB_ZCL_ATTR_THERMOSTAT_AC_REFRIGERANT_TYPE_ID:
      ret =  (*value == ZB_ZCL_THERMOSTAT_AC_REFRIGERANT_TYPE_UNKNOWN_VALUE ||
              *value == ZB_ZCL_THERMOSTAT_AC_REFRIGERANT_TYPE_R22_VALUE ||
              *value == ZB_ZCL_THERMOSTAT_AC_REFRIGERANT_TYPE_R410A_VALUE ||
              *value == ZB_ZCL_THERMOSTAT_AC_REFRIGERANT_TYPE_R407C_VALUE) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_AC_COMPRESSOR_TYPE_ID:
      ret = (*value == ZB_ZCL_THERMOSTAT_AC_COMPRESSOR_TYPE_UNKNOWN_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_COMPRESSOR_TYPE_T1_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_COMPRESSOR_TYPE_T2_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_COMPRESSOR_TYPE_T3_VALUE) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_AC_LOUVER_POSITION_ID:
      ret = (*value == ZB_ZCL_THERMOSTAT_AC_LOUVER_POSITION_FULLY_CLOSED_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_LOUVER_POSITION_FULLY_OPEN_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_LOUVER_POSITION_QUARTER_OPEN_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_LOUVER_POSITION_HALF_OPEN_VALUE ||
             *value == ZB_ZCL_THERMOSTAT_AC_LOUVER_POSITION_THREE_QUARTERS_OPEN_VALUE) ? RET_OK : RET_ERROR;
      comp = ZB_FALSE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_AC_COIL_TEMPERATURE_ID:
      min_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_AC_COIL_TEMPERATURE_MIN_VALUE;
      max_val = (zb_int16_t)ZB_ZCL_THERMOSTAT_AC_COIL_TEMPERATURE_MAX_VALUE;
      break;

    case ZB_ZCL_ATTR_THERMOSTAT_AC_CAPACITY_FORMAT_ID:
      if (*value == ZB_ZCL_THERMOSTAT_AC_CAPACITY_FORMAT_BTUH_VALUE)
      {
        ret = RET_ERROR;
      }
      comp = ZB_FALSE;
      break;

    default:
      comp = ZB_FALSE;
      break;
  }

  if (comp == ZB_TRUE)
  {
    TRACE_MSG(TRACE_ZCL1, "comp min %hd max %hd val %hd", (FMT__D_D_D, min_val, max_val, value));
    ret = (ZB_ZCL_ATTR_GETS16(value) >= min_val &&
           ZB_ZCL_ATTR_GETS16(value) <= max_val) ? RET_OK : RET_ERROR;
  }

  if (ret == RET_OK)
  {
    /* Last check - DeadBand */
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(
      endpoint,
      ZB_ZCL_CLUSTER_ID_THERMOSTAT,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_THERMOSTAT_MIN_SETPOINT_DEAD_BAND_ID);
    zb_int8_t dead_band = ZB_ZCL_THERMOSTAT_MIN_SETPOINT_DEAD_BAND_MIN_VALUE;

    if ( attr_desc != NULL )
    {
      dead_band = (zb_int8_t)ZB_ZCL_GET_ATTRIBUTE_VAL_S8(attr_desc);
    }

    switch( attr_id )
    {
      case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID:
      case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_COOLING_SETPOINT_ID:
      {
        /* OccupiedHeatingSetpoint shall always be below the value specified in the
         * OccupiedCoolingSetpoint by at least MinSetpointDeadBand*/
        attr_desc = zb_zcl_get_attr_desc_a(
          endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT,
          ZB_ZCL_CLUSTER_SERVER_ROLE,
          (attr_id == ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID) ?
          ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID :
          ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID);
        if (attr_desc != NULL)
        {
          TRACE_MSG(TRACE_ZCL1, "heating 0x%x cooling 0x%x dead_band 0x%x", (FMT__D_D_D, ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc), ZB_ZCL_ATTR_GETS16(value), dead_band));

          ret = (ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc) + dead_band <=
                 ZB_ZCL_ATTR_GETS16(value)) ? RET_OK : RET_ERROR;
        }
      }
      break;

      case ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID:
      case ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_HEATING_SETPOINT_ID:
      {
        /* OccupiedCoolingSetpoint shall always be above the value specified in the
         * OccupiedHeatingSetpoint by at least MinSetpointDeadBand*/
        attr_desc = zb_zcl_get_attr_desc_a(
          endpoint,
          ZB_ZCL_CLUSTER_ID_THERMOSTAT,
          ZB_ZCL_CLUSTER_SERVER_ROLE,
          (attr_id == ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID) ?
          ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID :
          ZB_ZCL_ATTR_THERMOSTAT_UNOCCUPIED_COOLING_SETPOINT_ID);
        if (attr_desc != NULL)
        {
          TRACE_MSG(TRACE_ZCL1, "heating 0x%x cooling 0x%x dead_band 0x%x", (FMT__D_D_D, ZB_ZCL_ATTR_GETS16(value), ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc), dead_band));

          ret = (ZB_ZCL_GET_ATTRIBUTE_VAL_S16(attr_desc) - dead_band >=
                 ZB_ZCL_ATTR_GETS16(value)) ? RET_OK : RET_ERROR;
        }
      }
      break;

      default:
        break;
    }
  }
  TRACE_MSG(TRACE_ZCL1, "check_value_thermsostat ret %hd", (FMT__H, ret));
  return ret;
}

/*
 * Invoke User App with Thermostat command.
 */
void zb_zcl_thermostat_invoke_user_app(zb_uint8_t param, zb_uint16_t endpoint16)
{
  zb_ret_t result;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_invoke_user_app %hx endpoint %d",
      (FMT__H_D, param, endpoint16));

  if (ZCL_CTX().device_cb != NULL && g_thermostat_context.setpoint_mode != ZB_ZCL_THERMOSTAT_PROCESS_NONE)
  {
    zb_zcl_device_callback_param_t *user_app_invoke_data =
        ZB_BUF_GET_PARAM(param, zb_zcl_device_callback_param_t);
    zb_zcl_attr_t *attr_desc;
    zb_uint8_t endpoint = (zb_uint8_t)endpoint16;

    user_app_invoke_data->device_cb_id = ZB_ZCL_THERMOSTAT_VALUE_CB_ID;
    user_app_invoke_data->endpoint = endpoint;
    user_app_invoke_data->cb_param.thermostat_value_param.mode = g_thermostat_context.setpoint_mode;

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID);
    ZB_ASSERT(attr_desc);
    user_app_invoke_data->cb_param.thermostat_value_param.heat_setpoint = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
            ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID);
    ZB_ASSERT(attr_desc);
    user_app_invoke_data->cb_param.thermostat_value_param.cool_setpoint = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
    user_app_invoke_data->status = RET_OK;

    g_thermostat_context.setpoint_mode = ZB_ZCL_THERMOSTAT_PROCESS_NONE;

    (ZCL_CTX().device_cb)(param);
    result = user_app_invoke_data->status;
    TRACE_MSG(TRACE_ZCL1, "result %h", (FMT__H, result));
  }
  else
  {
    result = RET_OK;
  }
  ZVUNUSED(result);

  zb_buf_free(param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_invoke_user_app", (FMT__0));
}

#if defined ZB_ENABLE_HA

/*********************** Weekly loop ************************************/

/**
 * Algorithm:
 * Weekly Schedule process is schedule on min between near Alarm Set point and
 * Max ZB_ZCL_THERMOSTAT_TIMER_MAX with number of Alarm Set point as parameter.
 *
 * Process
 * 1. execute nearest Alarm Set point between saved as parameter and current time
 * 2. reschedule self
 */

/**
 * Get last weekly schedule alarm before week_time by specific Weekly Schedule record
 * @param week_time - week time
 * @param index - index of Weekly Schedule record
 */
zb_uint32_t zb_zcl_thermostat_get_previos_timediff(zb_uint32_t week_time, zb_uint8_t index)
{
  zb_uint32_t timediff = ZB_SECOND_PER_WEEK;
  zb_uint32_t start_day = 0;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_get_previos_timediff curr %ld index %hd", (FMT__L_H,
      week_time, index));

  for (i=0; i<7; i++, start_day+=ZB_SECOND_PER_DAY)
  {
    if (g_thermostat_context.schedule_table[index].days & (1<<i) )
    {
      zb_uint32_t delta = ZB_WEEK_TIME_SUBTRACT(week_time, start_day + g_thermostat_context.schedule_table[index].start_time);
      if (delta<timediff)
      {
        timediff = delta;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_get_previos_timediff %d", (FMT__L,
      timediff));

  return timediff;
}

/**
 * Get first weekly schedule alarm after week_time by specific Weekly Schedule record
 * @param week_time - week time
 * @param index - index of Weekly Schedule record
 */
zb_uint32_t zb_zcl_thermostat_get_next_timediff(zb_uint32_t week_time, zb_uint8_t index)
{
  zb_uint32_t timediff = ZB_SECOND_PER_WEEK;
  zb_uint32_t start_day = 0;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_get_next_timediff curr %ld index %hd", (FMT__L_H,
      week_time, index));

  for (i=0; i<7; i++, start_day+=ZB_SECOND_PER_DAY)
  {
    /* We don't know current day of week, so check every day of
     * week. ZB_WEEK_TIME_SUBTRACT takes into account week
     * cycle. Checking all days allows to find minimum scheduling
     * interval. Possible optimization: start calculations with
     * current week day number and break on the 1st found scheduling interval */
    if (g_thermostat_context.schedule_table[index].days & (1<<i))
    {
      zb_uint32_t delta = ZB_WEEK_TIME_SUBTRACT(start_day + g_thermostat_context.schedule_table[index].start_time, week_time);
      if (delta<timediff)
      {
        timediff = delta;
      }
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_get_next_timediff %d", (FMT__L,
      timediff));

  return timediff;
}


/**
 * @brief Get current day of Week
 * @param start_of_week - start of week as Day of Week for Sequence value, table 10.46 in HA spec
 * @param result - Day of Week for Sequence value, table 10.46 in HA spec
 */
zb_uint8_t zb_zcl_thermostat_get_current_day_of_week(zb_uint8_t start_of_week)
{
  zb_uint32_t curr_week_time;
  zb_uint8_t curr_day;
  zb_int8_t diff;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_get_current_day_of_week start_of_week %i", (FMT__H, start_of_week));
  curr_week_time = zb_get_utc_time() % ZB_SECOND_PER_WEEK;
  curr_day = curr_week_time / ZB_SECOND_PER_DAY;

  /* Because 1 jan 1970 was Thursday (start of week - Sunday), we need to convert
     curr_day to days from our start_of_week. */

  TRACE_MSG(TRACE_ZCL1, "curr_day before %i", (FMT__H, curr_day));

  diff = ZB_ZCL_THERMOSTAT_START_OF_WEEK_THURSDAY - start_of_week;

  if (curr_day + diff > ZB_DAYS_PER_WEEK - 1)
  {
    curr_day = curr_day + diff - (ZB_DAYS_PER_WEEK);
  }
  else if (curr_day + diff < 0)
  {
    curr_day = ZB_DAYS_PER_WEEK - 1 - (ZB_ABS(diff) - curr_day);
  }
  else
  {
    curr_day += diff;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_get_current_day_of_week ret %i", (FMT__H, curr_day));
  return (1 << curr_day);
}

/**
 * @brief Convert data from Weekly Schedule record to cluster attributies and invoke User app wrapper
 * @param index - index of Weekly Schedule record
 */
void zb_zcl_thermostat_convert_weekly_alarm_to_attributies(zb_uint8_t index)
{
  zb_uint8_t endpoint;
  zb_bool_t is_new_event;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_convert_weekly_alarm_to_attributies %hd", (FMT__H, index));

  is_new_event = (g_thermostat_context.setpoint_mode == ZB_ZCL_THERMOSTAT_PROCESS_NONE) ? ZB_TRUE : ZB_FALSE;

  ZB_ASSERT(index < ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE);
  g_thermostat_context.setpoint_mode = g_thermostat_context.schedule_table[index].mode;

  endpoint = get_endpoint_by_cluster(ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE);
  ZB_ASSERT(endpoint);

  ZB_ZCL_SET_ATTRIBUTE(endpoint, ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID,
                       (zb_uint8_t*)&(g_thermostat_context.schedule_table[index].cool_setpoint), ZB_FALSE);

  ZB_ZCL_SET_ATTRIBUTE(endpoint, ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID,
                       (zb_uint8_t*)&(g_thermostat_context.schedule_table[index].heat_setpoint), ZB_FALSE);

  if (is_new_event == ZB_TRUE)
  {
    zb_buf_get_out_delayed_ext(zb_zcl_thermostat_invoke_user_app, endpoint, 0);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_convert_weekly_alarm_to_attributies", (FMT__0));
}

void zb_zcl_thermostat_shedule_process(zb_uint8_t param)
{
  zb_uint32_t curr_week_time;
  zb_uint8_t index = param;
  zb_uint32_t min_time;
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_shedule_process index %hd", (FMT__H, param));

  curr_week_time = zb_get_utc_time() % ZB_SECOND_PER_WEEK;

  if (param != ZB_ZCL_THERMOSTAT_PROCESS_NONE)
  {
    zb_uint32_t t;

    /*
      SetPoint fired. Need to check if missed target time. If yes,
      check if there are another Setpoints. Use the closest setpoint
      to set the temperature.
     */
    min_time = ZB_ZCL_THERMOSTAT_SETPOINT_TIME_DELTA;
    t = zb_zcl_thermostat_get_previos_timediff(curr_week_time, index);

    if (t > min_time)
    {
      /* find the closest expired setpoint (within
       * ZB_ZCL_THERMOSTAT_SETPOINT_TIME_DELTA time interval) */
      index = ZB_ZCL_THERMOSTAT_PROCESS_NONE;
      for (i=0; i<ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE; i++)
      {
        if (g_thermostat_context.schedule_table[i].days != ZB_ZCL_THERMOSTAT_WEEKLY_FREE_RECORD)
        {
          zb_uint32_t t = zb_zcl_thermostat_get_previos_timediff(curr_week_time, i);
          if (t<min_time)
          {
            min_time = t;
            index = i;
          }
        }
      }
    }

    if (index != ZB_ZCL_THERMOSTAT_PROCESS_NONE)
    {
      /* start function to invoke user routine to handle setpoint */
      zb_zcl_thermostat_convert_weekly_alarm_to_attributies(index);
    }
  }

  // prepare next loop
  min_time = ZB_ZCL_THERMOSTAT_TIMER_MAX;
  index = ZB_ZCL_THERMOSTAT_PROCESS_NONE;

  for (i=0; i<ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE; i++)
  {
    if (g_thermostat_context.schedule_table[i].days != ZB_ZCL_THERMOSTAT_WEEKLY_FREE_RECORD)
    {
      zb_uint32_t t = zb_zcl_thermostat_get_next_timediff(curr_week_time, i);
      if (t<min_time)
      {
        min_time = t;
        index = i;
      }
    }
  }

  ZB_SCHEDULE_ALARM(zb_zcl_thermostat_shedule_process, index, min_time * ZB_TIME_ONE_SECOND);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_shedule_process", (FMT__0));
}

void zb_zcl_thermostat_remove_record_by_day(zb_uint8_t day)
{
  zb_uindex_t i;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_remove_record_by_day %hd", (FMT__H, day));

  for (i=0; i<ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE; i++)
  {
    TRACE_MSG(TRACE_ZCL3, "remove %hd before %hd after %hd", (FMT__H_H_H,
        i, g_thermostat_context.schedule_table[i].days,
        g_thermostat_context.schedule_table[i].days & ~(day)));

    g_thermostat_context.schedule_table[i].days &= ~(day);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_remove_record_by_day", (FMT__0));
}

#endif /* defined ZB_ENABLE_HA */

/*********************** Cluster command process *************************/

zb_ret_t zb_zcl_thermostat_setpoint_raise_lower_handler(zb_uint8_t param)
{
  zb_zcl_thermostat_setpoint_raise_lower_req_t setpoint_raise_lower_req = {0, 0};
  zb_zcl_parse_status_t status;
  zb_ret_t ret = RET_OK;
  zb_uint8_t endpoint;
  zb_zcl_attr_t *attr_desc_heating_setpoint;
  zb_zcl_attr_t *attr_desc_cooling_setpoint;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_bool_t is_new_event = (g_thermostat_context.setpoint_mode == ZB_ZCL_THERMOSTAT_PROCESS_NONE) ? ZB_TRUE : ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_setpoint_raise_lower_handler %hd", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
  attr_desc_heating_setpoint = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID);
  attr_desc_cooling_setpoint = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_setpoint_raise_lower_handler: param %i", (FMT__H, param));
  ZB_ZCL_THERMOSTAT_GET_SETPOINT_RAISE_LOWER_REQ(param, setpoint_raise_lower_req, status);
  ZB_ASSERT(status == ZB_ZCL_PARSE_STATUS_SUCCESS);
  ZVUNUSED(status);

  TRACE_MSG(TRACE_ZCL2, "mode %hd amount %hd", (FMT__H_H,
      setpoint_raise_lower_req.mode, setpoint_raise_lower_req.amount));

  switch (setpoint_raise_lower_req.mode)
  {
    case ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_HEAT:
    {
      zb_int16_t value = *(zb_int16_t*)attr_desc_heating_setpoint->data_p +
        setpoint_raise_lower_req.amount * ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_AMOUNT_MUL;
      TRACE_MSG(TRACE_ZCL2, "old value %d new value %d", (FMT__D_D,
          *(zb_int16_t*)attr_desc_heating_setpoint->data_p, value));
      if (zb_zcl_set_attr_val(endpoint, ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                              ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID, (zb_uint8_t*)&value, ZB_FALSE) != ZB_ZCL_STATUS_SUCCESS)
      {
        ret = RET_INVALID_PARAMETER_1;
    }
    }
    break;
    case ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_COOL:
    {
      zb_int16_t value = *(zb_int16_t*)attr_desc_cooling_setpoint->data_p +
        setpoint_raise_lower_req.amount * ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_AMOUNT_MUL;
      TRACE_MSG(TRACE_ZCL2, "old value %d new value %d", (FMT__D_D,
          *(zb_int16_t*)attr_desc_cooling_setpoint->data_p, value));
      if (zb_zcl_set_attr_val(endpoint, ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                              ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID, (zb_uint8_t*)&value, ZB_FALSE) != ZB_ZCL_STATUS_SUCCESS)
      {
        ret = RET_INVALID_PARAMETER_1;
    }
    }
    break;
    case ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_MODE_BOTH:
    {
      zb_int16_t heating_value = *(zb_int16_t*)attr_desc_heating_setpoint->data_p +
        setpoint_raise_lower_req.amount * ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_AMOUNT_MUL;
      zb_int16_t cooling_value = *(zb_int16_t*)attr_desc_cooling_setpoint->data_p +
        setpoint_raise_lower_req.amount * ZB_ZCL_THERMOSTAT_SETPOINT_RAISE_LOWER_AMOUNT_MUL;
      TRACE_MSG(TRACE_ZCL2, "old value %d new value %d old value %d new value %d ", (FMT__D_D_D_D,
          *(zb_int16_t*)attr_desc_heating_setpoint->data_p, heating_value,
          *(zb_int16_t*)attr_desc_cooling_setpoint->data_p, cooling_value));
      if (zb_zcl_set_attr_val(endpoint, ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                              ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_HEATING_SETPOINT_ID, (zb_uint8_t*)&heating_value, ZB_FALSE) != ZB_ZCL_STATUS_SUCCESS ||
          zb_zcl_set_attr_val(endpoint, ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                              ZB_ZCL_ATTR_THERMOSTAT_OCCUPIED_COOLING_SETPOINT_ID, (zb_uint8_t*)&cooling_value, ZB_FALSE) != ZB_ZCL_STATUS_SUCCESS)
      {
        ret = RET_INVALID_PARAMETER_1;
    }
    }
    break;

    default:
      ret = RET_ERROR;
  }

  if (ret != RET_ERROR &&  is_new_event)
  {
    g_thermostat_context.setpoint_mode = setpoint_raise_lower_req.mode;
    zb_buf_get_out_delayed_ext(zb_zcl_thermostat_invoke_user_app, endpoint, 0);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_setpoint_raise_lower_handler %hd", (FMT__H, ret));

  return ret;
}

#if defined ZB_ENABLE_HA

/**
 * 1. Read command parameters
 * 2. Remove Weekly Schedule by command parameter day-mask
 * 3. Add new Weekly Schedule
 * 4. Restart Schedule Process
 */
zb_ret_t zb_zcl_thermostat_set_weekly_schedule_handler(zb_uint8_t param)
{
  zb_zcl_thermostat_set_weekly_schedule_req_t req;
  zb_zcl_thermostat_weekly_schedule_point_pair_t pair;
  zb_zcl_parse_status_t status;
  zb_ret_t ret = RET_OK;
  zb_uindex_t i = 0;
  zb_uindex_t index;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_set_weekly_schedule_handler %hd", (FMT__H, param));

  ZB_ZCL_THERMOSTAT_GET_SET_WEEKLY_SCHEDULE_REQ(param, req, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL2, "RET_INVALID_PARAMETER", (FMT__0));
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    zb_zcl_thermostat_remove_record_by_day(req.day_of_week);

    TRACE_MSG(TRACE_ZCL2, "req.num_of_transitions %hd %hd %hd", (FMT__H_H_H,
        req.num_of_transitions, req.day_of_week, req.mode_for_seq));

    for (index=0; index<req.num_of_transitions; index++)
    {
      ZB_ZCL_THERMOSTAT_GET_NEXT_WEEKLY_SCHEDULE_POINT_PAIR_REQ(param, req.mode_for_seq, pair, status);
      if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
      {
        TRACE_MSG(TRACE_ZCL2, "RET_INVALID_PARAMETER", (FMT__0));
        ret = RET_INVALID_PARAMETER;
        break;
      }

      // find free table row
      for (; i < ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE; i++)
      {
        if (g_thermostat_context.schedule_table[i].days == ZB_ZCL_THERMOSTAT_WEEKLY_FREE_RECORD)
        {
          break;
        }
      }

      if (i==ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE)
      {
        ret = RET_OUT_OF_RANGE;
        TRACE_MSG(TRACE_ZCL2, "RET_OUT_OF_RANGE", (FMT__0));
        break;
      }

      TRACE_MSG(TRACE_ZCL3, "Save index %hd", (FMT__H, i));

      // save
      g_thermostat_context.schedule_table[i].days = req.day_of_week;
      g_thermostat_context.schedule_table[i].mode = req.mode_for_seq;
      g_thermostat_context.schedule_table[i].start_time = pair.transition_time * ZB_SECOND_PER_MINUTE;
      g_thermostat_context.schedule_table[i].heat_setpoint = pair.heat_set_point;
      g_thermostat_context.schedule_table[i].cool_setpoint = pair.cool_set_point;
    }

    // reset loop
    ZB_SCHEDULE_ALARM_CANCEL(zb_zcl_thermostat_shedule_process, ZB_ALARM_ANY_PARAM);
    ZB_SCHEDULE_CALLBACK(zb_zcl_thermostat_shedule_process, ZB_ZCL_THERMOSTAT_PROCESS_NONE);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_set_weekly_schedule_handler", (FMT__0));

  return ret;
}

zb_ret_t zb_zcl_thermostat_get_weekly_schedule_handler(zb_uint8_t param)
{
  zb_zcl_thermostat_get_weekly_schedule_req_t req;
  zb_zcl_parse_status_t status;
  zb_ret_t ret = RET_OK;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_get_weekly_schedule_handler %hd", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  ZB_ZCL_THERMOSTAT_GET_GET_WEEKLY_SCHEDULE_REQ(param, req, status);
  if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    ret = RET_INVALID_PARAMETER;
  }
  else
  {
    zb_uint8_t *cmd_ptr;
    zb_uint8_t *count_ptr;
    zb_uindex_t i;

    TRACE_MSG(TRACE_ZCL3, "days %hd mode %hd", (FMT__H_H,
        req.days_to_return, req.mode_to_return));

    ZB_ZCL_THERMOSTAT_INIT_GET_WEEKLY_SCHEDULE_RESP(param, cmd_ptr, cmd_info.seq_number,
        0, req.days_to_return, req.mode_to_return);
    count_ptr = cmd_ptr - 3;

/*TODO: CR:MEDIUM add buffer size check */
    for (i=0; i<ZB_ZCL_THERMOSTAT_WEEKLY_SCHEDULE_TABLE_SIZE; i++)
    {
      TRACE_MSG(TRACE_ZCL3, "index %d days %hd mode %hd", (FMT__H_H_H,
          i, g_thermostat_context.schedule_table[i].days, g_thermostat_context.schedule_table[i].mode));

      if ((g_thermostat_context.schedule_table[i].days & req.days_to_return) == req.days_to_return &&
          (g_thermostat_context.schedule_table[i].mode == req.mode_to_return))
      {
        ZB_ZCL_THERMOSTAT_ADD_VALUE_WEEKLY_SCHEDULE(cmd_ptr,
            g_thermostat_context.schedule_table[i].mode,
            g_thermostat_context.schedule_table[i].start_time / ZB_SECOND_PER_MINUTE,
            g_thermostat_context.schedule_table[i].heat_setpoint,
            g_thermostat_context.schedule_table[i].cool_setpoint);

        (*count_ptr)++;
      }
    }

    ZB_ZCL_THERMOSTAT_SEND_GET_WEEKLY_SCHEDULE_RESP(param, cmd_ptr,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      cmd_info.profile_id,
      NULL);

    ret = RET_BUSY;
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_get_weekly_schedule_handler", (FMT__0));

  return ret;
}

zb_ret_t zb_zcl_thermostat_clear_weekly_schedule_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(param);

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_thermostat_clear_weekly_schedule_handler %hd", (FMT__H, param));

  zb_zcl_thermostat_remove_record_by_day(ZB_ZCL_THERMOSTAT_WEEKLY_ALL);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_thermostat_clear_weekly_schedule_handler", (FMT__0));

  return ret;
}

#ifdef SUPPORT_RELAY_LOG
/**
 * This command not implement yet because has not some data value:
 * relay_status, humidity_in_percentage, set_point,
 * unread_entries. Currently Relay log storing is not defined in the Spec
 */
zb_ret_t zb_zcl_thermostat_get_relay_status_log_handler(zb_uint8_t param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_parsed_hdr_t cmd_info;

  zb_uint16_t time_of_day;
  zb_uint8_t relay_status = 0;
  zb_int16_t local_temperature;
  zb_uint8_t humidity_in_percentage = 0;
  zb_int16_t set_point = 0;
  zb_uint16_t unread_entries = 0;
  zb_zcl_attr_t *attr_desc;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_thermostat_get_relay_status_log_handler %hd", (FMT__H, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  // get data
  time_of_day = (zb_uint16_t)((zb_get_utc_time() % ZB_SECOND_PER_WEEK) / ZB_SECOND_PER_MINUTE);

  attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
    ZB_ZCL_CLUSTER_ID_THERMOSTAT, ZB_ZCL_ATTR_THERMOSTAT_LOCAL_TEMPERATURE_ID);
  ZB_ASSERT(attr_desc);
  local_temperature = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

  // todo get value of:
  // relay_status, humidity_in_percentage, set_point, unread_entries

  ZB_ZCL_THERMOSTAT_SEND_GET_RELAY_STATUS_LOG_RESP(buf,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
      ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
      cmd_info.profile_id, cmd_info.seq_number, NULL,
      time_of_day,
      relay_status,
      local_temperature,
      humidity_in_percentage,
      set_point,
      unread_entries);

  ret = RET_BUSY;

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_thermostat_get_relay_status_log_handler", (FMT__0));

  return ret;
}
#endif

#endif /* defined ZB_ENABLE_HA */

zb_bool_t zb_zcl_process_thermostat_specific_commands(zb_uint8_t param)
{
  zb_bool_t processed = ZB_TRUE;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t ret = RET_OK;

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_thermostat_specific_commands: buf %p",
             (FMT__P, param));

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  if( cmd_info.cmd_direction == ZB_ZCL_FRAME_DIRECTION_TO_SRV )
  {
    switch (cmd_info.cmd_id)
    {
      case ZB_ZCL_CMD_THERMOSTAT_SETPOINT_RAISE_LOWER:
        ret = zb_zcl_thermostat_setpoint_raise_lower_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed SETPOINT_RAISE_LOWER command", (FMT__0));
        break;

#if defined ZB_ENABLE_HA
      case ZB_ZCL_CMD_THERMOSTAT_SET_WEEKLY_SCHEDULE:
        ret = zb_zcl_thermostat_set_weekly_schedule_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed SET_WEEKLY_SCHEDULE command", (FMT__0));
        break;

      case ZB_ZCL_CMD_THERMOSTAT_GET_WEEKLY_SCHEDULE:
        ret = zb_zcl_thermostat_get_weekly_schedule_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed GET_WEEKLY_SCHEDULE command", (FMT__0));
        break;

      case ZB_ZCL_CMD_THERMOSTAT_CLEAR_WEEKLY_SCHEDULE:
        ret = zb_zcl_thermostat_clear_weekly_schedule_handler(param);
        TRACE_MSG(TRACE_ZCL3, "Processed CLEAR_WEEKLY_SCHEDULE command", (FMT__0));
        break;

      case ZB_ZCL_CMD_THERMOSTAT_GET_RELAY_STATUS_LOG:
        // TODO command not release because have not info about Relay Status Log
        ret = RET_IGNORE;
#ifdef SUPPORT_RELAY_LOG
        ret = zb_zcl_thermostat_get_relay_status_log_handler(param);
#endif
        TRACE_MSG(TRACE_ZCL3, "Processed GET_RELAY_STATUS_LOG command", (FMT__0));
        break;

#endif /* defined ZB_ENABLE_HA */

      default:
        processed = ZB_FALSE;
        break;
    }
  }

  if (ret == RET_BUSY)
  {
    // do nothing: buf has reused, processed has set ZB_TRUE
  }
  else if(processed == ZB_FALSE)
  {
    // do nothing: base func will send  else if(processed == ZB_FALSE)
  }
  else if (ret != RET_OK)
  {
    zb_uint8_t status_code;
    switch(ret)
    {
    case RET_OUT_OF_RANGE:
      status_code = ZB_ZCL_STATUS_INSUFF_SPACE;
      break;
    case RET_INVALID_PARAMETER:
      status_code = ZB_ZCL_STATUS_INVALID_FIELD;
      break;
      case RET_INVALID_PARAMETER_1:
      status_code = ZB_ZCL_STATUS_INVALID_VALUE;
      break;
    case RET_IGNORE:
      /* ZCL8: CCB 2477: use UNSUP_COMMAND instead of any other Unsupported command status */
      status_code = ZB_ZCL_STATUS_UNSUP_CMD;
      break;
    default:
      status_code = ZB_ZCL_STATUS_FAIL;
      break;
    }
    ZB_ZCL_SEND_DEFAULT_RESP(param,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                             ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                             cmd_info.profile_id,
                             ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                             cmd_info.seq_number,
                             cmd_info.cmd_id,
                             status_code);
  }
  else if (!(cmd_info.disable_default_response))
  {
    ZB_ZCL_SEND_DEFAULT_RESP(param,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
                             ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
                             ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
                             cmd_info.profile_id,
                             ZB_ZCL_CLUSTER_ID_THERMOSTAT,
                             cmd_info.seq_number,
                             cmd_info.cmd_id,
                             processed == ZB_TRUE ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_FAIL);
  }
  else
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1,
            "< zb_zcl_process_thermostat_specific_commands: processed %d",
            (FMT__D, processed));

  return ZB_TRUE;
}


zb_bool_t zb_zcl_process_thermostat_specific_commands_srv(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_thermostat_server_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_thermostat_specific_commands(param);
}


zb_bool_t zb_zcl_process_thermostat_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_thermostat_client_cmd_list;
    return ZB_TRUE;
  }
  return zb_zcl_process_thermostat_specific_commands(param);
}
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_THERMOSTAT */
