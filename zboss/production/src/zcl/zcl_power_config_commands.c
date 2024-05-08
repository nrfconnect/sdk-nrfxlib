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
/* PURPOSE: ZCL Power Configuration cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2079

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_POWER_CONFIG)

#include "zb_time.h"
#include "zb_zdo.h"
#include "zb_zcl.h"
#include "zcl/zb_zcl_power_config.h"
#include "zcl/zb_zcl_alarms.h"

/** @internal
    @{
*/

zb_ret_t check_value_power_config_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
void zb_zcl_power_config_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code);

void zb_zcl_power_config_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_power_config_server,
                              zb_zcl_power_config_write_attr_hook_server,
                              (zb_zcl_cluster_handler_t)NULL);
}

void zb_zcl_power_config_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              (zb_zcl_cluster_check_value_t)NULL,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

zb_ret_t check_value_power_config_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id);
  ZVUNUSED(value);
  ZVUNUSED(endpoint);

  /* All values for mandatory attributes are allowed, extra check for
   * optional attributes is needed */

  return RET_OK;
}

/*Subtrahend to cast battery voltage attribute IDs to the simplified calculation view*/
#define ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_ATTR_SUBTRAHEND 0x20
/*Subtrahend to cast battery percentage remaining attribute IDs to the simplified calculation view*/
#define ZB_ZCL_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ATTR_SUBTRAHEND 0x21
/*The addend is needed for calculation alarm value from normalized attribute Id*/
#define ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODES_ADDEND 0x20

/*Alarm State attribute shifts (ZCL spec Rev 7 3.3.2.2.4.11)*/
#define ZB_ZCL_POWER_CONFIG_ALARM_STATE_ATTR_BATTERY_SOURCE_LSHIFT 0
#define ZB_ZCL_POWER_CONFIG_ALARM_STATE_ATTR_BATTERY_SOURCE_2_LSHIFT 10
#define ZB_ZCL_POWER_CONFIG_ALARM_STATE_ATTR_BATTERY_SOURCE_3_LSHIFT 20

/*The minuend for getting alarm mask value from normalized attribute ID*/
#define ZB_ZCL_POWER_CONFIG_ALARM_MASKS_MINUEND 16

#define ZB_ZCL_CHECK_THRESHOLD_EXISTENCE(ep, attr_set, is_percentage)                       \
{                                                                                           \
  zb_zcl_attr_t *attr_desc_local;                                                           \
  attr_desc_local = zb_zcl_get_attr_desc_a(                                                 \
    ep,                                                                                     \
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,                                                         \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                                             \
    (is_percentage) ?                                                                       \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID):   \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID));     \
  if (!attr_desc_local)                                                                     \
  {                                                                                         \
    val &= ~ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_MIN_THRESHOLD;                  \
  }                                                                                         \
  attr_desc_local = zb_zcl_get_attr_desc_a(                                                 \
    ep,                                                                                     \
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,                                                         \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                                             \
    (is_percentage) ?                                                                       \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID):      \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID));        \
  if (!attr_desc_local)                                                                     \
  {                                                                                         \
    val &= ~ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE1;                       \
  }                                                                                         \
  attr_desc_local = zb_zcl_get_attr_desc_a(                                                 \
    ep,                                                                                     \
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,                                                         \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                                             \
    (is_percentage) ?                                                                       \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID):      \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID));        \
  if (!attr_desc_local)                                                                     \
  {                                                                                         \
    val &= ~ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE2;                       \
  }                                                                                         \
  attr_desc_local = zb_zcl_get_attr_desc_a(                                                 \
    ep,                                                                                     \
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,                                                         \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                                                             \
    (is_percentage) ?                                                                       \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID):      \
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID));        \
  if (!attr_desc_local)                                                                     \
  {                                                                                         \
    val &= ~ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE3;                       \
  }                                                                                         \
}

void fill_alarm_mask_bitmap(zb_uint8_t ep, enum zb_zcl_power_config_battery_alarm_state_e value,
                                 zb_zcl_attr_t *attr_desc, zb_uint16_t attr_set, zb_bool_t is_percentage)
{
  if (attr_desc)
  {
    zb_uint8_t val = ZB_ZCL_POWER_CONFIG_ALARM_MASKS_MINUEND - value;
    ZB_ZCL_CHECK_THRESHOLD_EXISTENCE(ep, attr_set, is_percentage);
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, val);
  }
}

void fill_alarm_state_bitmap(zb_uint8_t ep, enum zb_zcl_power_config_battery_alarm_state_e value,
                                  zb_zcl_attr_t *attr_desc, zb_uint16_t attr_set, zb_bool_t is_percentage)
{
  if (attr_desc)
  {
    zb_uint32_t attr_val = 0;
    zb_uint8_t val = ZB_ZCL_POWER_CONFIG_ALARM_MASKS_MINUEND - value;
    ZB_ZCL_CHECK_THRESHOLD_EXISTENCE(ep, attr_set, is_percentage);
    if (attr_set == ZB_ZCL_POWER_CONFIG_BATTERY_ATTRIBUTE_SET)
    {
      attr_val = (zb_uint32_t)val << ZB_ZCL_POWER_CONFIG_ALARM_STATE_ATTR_BATTERY_SOURCE_LSHIFT;
    }
    else if (attr_set == ZB_ZCL_POWER_CONFIG_BATTERY_SOURCE_2_ATTRIBUTE_SET)
    {
      attr_val = (zb_uint32_t)val << ZB_ZCL_POWER_CONFIG_ALARM_STATE_ATTR_BATTERY_SOURCE_2_LSHIFT;
    }
    else if (attr_set == ZB_ZCL_POWER_CONFIG_BATTERY_SOURCE_3_ATTRIBUTE_SET)
    {
      attr_val = (zb_uint32_t)val << ZB_ZCL_POWER_CONFIG_ALARM_STATE_ATTR_BATTERY_SOURCE_3_LSHIFT;
    }
    ZB_ZCL_SET_DIRECTLY_ATTR_VAL32(attr_desc, attr_val);
  }
}

/* performs comparison taking into account hysteresis. Returns TRUE
 * if voltage value goes lower then threshold */
zb_bool_t zcl_pwr_value_under_threshold(zb_uint8_t voltage, zb_uint8_t threshold)
{
  zb_bool_t ret = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zcl_pwr_value_under_threshold voltage %hd threshold %hd",
            (FMT__H_H, voltage, threshold));

  /* TODO: implement comparison taking into account hysteresis */
  if ((voltage < threshold) && (threshold != 0xff))
  {
    ret = ZB_TRUE;
  }

  TRACE_MSG(TRACE_ZCL1, "< zcl_pwr_value_under_threshold ret %hd",
            (FMT__H, ret));
  return ret;
}

void send_alarm(zb_uint8_t param)
{
  zb_bool_t ret = ZB_TRUE;
  zb_uint16_t addr = 0;
  zb_uint16_t *user_param = ZB_BUF_GET_PARAM(param, zb_uint16_t);
  zb_uint8_t ep = *user_param;
  zb_uint8_t alarm_code = *user_param >> 8;

  /* AN: For Mains Voltage it needs to check current value just before alarm sending one more time.
   * Maybe Voltage has come back to the [Min Threshold; Max Threshold] range
   */
  if ((alarm_code == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD_ALARM_CODE) ||
      (alarm_code == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD_ALARM_CODE))
  {
    zb_zcl_attr_t *attr_desc;
    zb_uint16_t val,threshold;
    attr_desc = zb_zcl_get_attr_desc_a(
      ep,
      ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_ID);
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
    attr_desc = zb_zcl_get_attr_desc_a(
      ep,
      ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      (alarm_code == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD_ALARM_CODE) ?
      ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD :
      ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD);
    threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
    if (((alarm_code == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD_ALARM_CODE) && (val >= threshold)) ||
        ((alarm_code == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD_ALARM_CODE) && (val <= threshold)) ||
        (threshold == ZB_ZCL_POWER_CONFIG_THRESHOLD_ALARM_OMISSION_VALUE))
    {
      ret = ZB_FALSE;
    }
  }

  if (ret)
  {
    ZB_ZCL_ALARMS_SEND_ALARM_RES(
      param,
      addr,
      ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT,
      0,
      ep,
      ZB_AF_HA_PROFILE_ID,
      NULL, alarm_code, ZB_ZCL_CLUSTER_ID_POWER_CONFIG);
  }
  else
  {
    zb_buf_free(param);
  }
}

void get_send_alarm_buf(zb_uint8_t param, zb_uint16_t user_param)
{
  if (!param)
  {
    zb_buf_get_out_delayed_ext(get_send_alarm_buf, user_param, 0);
  }
  else
  {
    zb_uint16_t dwell_period = 0;
    zb_uint16_t *u_param = ZB_BUF_GET_PARAM(param, zb_uint16_t);
    zb_zcl_attr_t *attr_desc = NULL;
    *u_param = user_param;
    attr_desc = zb_zcl_get_attr_desc_a(
      (zb_uint8_t)user_param,
      ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_POWER_CONFIG_MAINS_DWELL_TRIP_POINT);
    if (attr_desc && ((user_param >> 8 == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD_ALARM_CODE) ||
                      (user_param >> 8 == ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD_ALARM_CODE)))
    {
      dwell_period = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
    }
    zb_schedule_alarm(send_alarm, param, ZB_TIME_ONE_SECOND*dwell_period);
  }
}

void zcl_pwr_cfg_check_mains_voltage(zb_uint8_t ep, zb_uint16_t val)
{
  zb_bool_t ret = ZB_FALSE;
  zb_zcl_attr_t *attr_desc;
  zb_zcl_attr_t *attr_desc_mask;
  zb_uint16_t threshold;

  attr_desc_mask = zb_zcl_get_attr_desc_a(
    ep,
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_POWER_CONFIG_MAINS_ALARM_MASK_ID);

  TRACE_MSG(TRACE_ZCL1, "> zcl_pwr_cfg_check_mains_voltage_value ep %hd, val %d", (FMT__H_D, ep, val));

  if (attr_desc_mask)
  {
    attr_desc = zb_zcl_get_attr_desc_a(
      ep,
      ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MIN_THRESHOLD);
    if (attr_desc)
    {
      threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
      if (val < threshold && threshold != ZB_ZCL_POWER_CONFIG_THRESHOLD_ALARM_OMISSION_VALUE)
      {
        ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_mask, ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MIN_THRESHOLD);
        ret = ZB_TRUE;
      }
    }
    if (!ret)
    {
      attr_desc = zb_zcl_get_attr_desc_a(
        ep,
        ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_MAX_THRESHOLD);
      if (attr_desc)
      {
        threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
        if (val > threshold && threshold != ZB_ZCL_POWER_CONFIG_THRESHOLD_ALARM_OMISSION_VALUE)
        {
          ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc_mask, ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MAX_THRESHOLD);
          ret = ZB_TRUE;
        }
      }
    }
    if (ret)
    {
      zb_af_endpoint_desc_t* endpoint_desc = zb_af_get_endpoint_desc(ep);
      zb_zcl_cluster_desc_t *cluster_desc = get_cluster_desc(endpoint_desc, ZB_ZCL_CLUSTER_ID_ALARMS, ZB_ZCL_CLUSTER_SERVER_ROLE);
      if (cluster_desc)
      {
        zb_uint8_t alarm_mask = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_mask);
        zb_uint16_t u_param = ( ((alarm_mask & ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MAX_THRESHOLD) ?
                                                ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MAX_THRESHOLD :
                                                ZB_ZCL_POWER_CONFIG_MAINS_VOLTAGE_ALARM_CODE_MIN_THRESHOLD)<< 8) | ep;
        zb_buf_get_out_delayed_ext(get_send_alarm_buf, u_param, 0);
       }
    }
  }
}
  /* HA version of Power Configuration declares 3 sets of
   * BatteryInformation and BatterySettings attr sets. This function
   * is used to check new BATTERY_VOLTAGE attribute or BATTERY_PERCENTAGE_REMAINING
   * value in attribute set defined by attr_set value and set BatteryAlarmState
   * if any alarm appears (BatteryVoltageMinThreshold or other threshold check)
   */
void zcl_pwr_cfg_check_battery_voltage_or_percentage_value(zb_uint8_t ep, zb_uint8_t val,
                                                                zb_uint16_t attr_set, zb_bool_t is_percentage)
{
  zb_bool_t ret = ZB_FALSE;
  zb_zcl_attr_t *attr_desc;
  zb_zcl_attr_t *attr_desc_state;
  zb_zcl_attr_t *attr_desc_mask;
  zb_uint8_t threshold;

  TRACE_MSG(TRACE_ZCL1, "> zcl_pwr_cfg_check_val_value ep %hd, val %hd, attr_set %d",
            (FMT__H_H_D, ep, val, attr_set));

  attr_desc_state = zb_zcl_get_attr_desc_a(
    ep,
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_STATE_ID));

  attr_desc_mask = zb_zcl_get_attr_desc_a(
    ep,
    ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID));

  if (attr_desc_state)
  {
    /*** Check Min threshold value ***/
    attr_desc = zb_zcl_get_attr_desc_a(
      ep,
      ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
      ZB_ZCL_CLUSTER_SERVER_ROLE,
      (is_percentage) ?
      (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_MIN_THRESHOLD_ID):
      (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID));
    if(attr_desc)
    {
      threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
      if (zcl_pwr_value_under_threshold(val, threshold))
      {
        fill_alarm_state_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_MIN_THRESHOLD, attr_desc_mask, attr_set, is_percentage);
        if (attr_desc_mask)
        {
          fill_alarm_mask_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_MIN_THRESHOLD, attr_desc_mask, attr_set, is_percentage);
        }
        ret = ZB_TRUE;
      }
    }

    /*** Check Threshold1 value ***/
    if (!ret)
    {
      attr_desc = zb_zcl_get_attr_desc_a(
        ep,
        ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        (is_percentage) ?
        (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD1_ID):
        (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD1_ID));
      if(attr_desc)
      {
        threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
        if (zcl_pwr_value_under_threshold(val, threshold))
        {
          fill_alarm_state_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE1, attr_desc_mask, attr_set, is_percentage);
          if (attr_desc_mask)
          {
            fill_alarm_mask_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE1, attr_desc_mask, attr_set, is_percentage);
          }
          ret = ZB_TRUE;
        }
      }
    }
    /*** Check Threshold2 value ***/
    if (!ret)
    {
      attr_desc = zb_zcl_get_attr_desc_a(
        ep,
        ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        (is_percentage) ?
        (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD2_ID):
        (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD2_ID));
      if(attr_desc)
      {
        threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
        if (zcl_pwr_value_under_threshold(val, threshold))
        {
          fill_alarm_state_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE2, attr_desc_mask, attr_set, is_percentage);
          if (attr_desc_mask)
          {
            fill_alarm_mask_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE2, attr_desc_mask, attr_set, is_percentage);
          }
          ret = ZB_TRUE;
        }
      }
    }

    /*** Check Threshold3 value ***/
    if (!ret)
    {
      attr_desc = zb_zcl_get_attr_desc_a(
        ep,
        ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        (is_percentage) ?
        (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_THRESHOLD3_ID):
        (zb_uint16_t)(attr_set+ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_THRESHOLD3_ID));
      if(attr_desc)
      {
        threshold = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

        if (zcl_pwr_value_under_threshold(val, threshold))
        {
          fill_alarm_state_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE3, attr_desc_mask, attr_set, is_percentage);
          if (attr_desc_mask)
          {
            fill_alarm_mask_bitmap(ep, ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE3, attr_desc_mask, attr_set, is_percentage);
          }
          ret = ZB_TRUE;
        }
      }
    }
    if (ret)
    {
      if (attr_desc_mask)
      {
        zb_af_endpoint_desc_t* endpoint_desc = zb_af_get_endpoint_desc(ep);
        zb_zcl_cluster_desc_t *cluster_desc = get_cluster_desc(endpoint_desc, ZB_ZCL_CLUSTER_ID_ALARMS, ZB_ZCL_CLUSTER_SERVER_ROLE);
        if (cluster_desc)
        {
          zb_uint8_t alarm_mask = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc_mask);
          /* AN: Looks like, that we can send only one "the most significant" alarm.
           * ZCL spec strongly requires (p. 3.3.2.2.4.8 BatteryVoltageThreshold 1-3 Attributes)
           * that BatteryVoltageThresholdN+1 has to be higher than the BatteryVoltageThresholdN.
           *
           * ms-alarm - subtrahend with range [0;4]. It is needed for Alarm code "threshold" nibble (10*[0,1,2] + 4 - ms_alarm)
           */
          zb_uint8_t ms_alarm = (alarm_mask & ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_MIN_THRESHOLD) ? 4 : (
                                (alarm_mask & ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE1) ? 3 : (
                                (alarm_mask & ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE2) ? 2 : (
                                (alarm_mask & ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_STATE_SOURCE1_VOLTAGE3) ? 1 : 0 )));
          zb_uint16_t u_param = (((attr_set+ZB_ZCL_POWER_CONFIG_BATTERY_ALARM_CODES_ADDEND)/2 + (4-ms_alarm)) << 8) | ep;
          zb_buf_get_out_delayed_ext(get_send_alarm_buf, u_param, 0);
        }
      }
    }

  }
  TRACE_MSG(TRACE_ZCL1, "< zcl_pwr_cfg_check_val_value ret %hd", (FMT__H, ret));
}

void zb_zcl_power_config_write_attr_hook_server(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t *new_value, zb_uint16_t manuf_code)
{
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_power_config_write_attr_hook endpoint %hx attr_id 0x%x, manuf_code 0x%x",
            (FMT__H_D_D, endpoint, attr_id, manuf_code));

  ZVUNUSED(manuf_code);

  if (attr_id == ZB_ZCL_ATTR_POWER_CONFIG_MAINS_VOLTAGE_ID)
  {
    zb_uint16_t new_value_16 = ZB_ZCL_ATTR_GET16(new_value);
    zcl_pwr_cfg_check_mains_voltage(endpoint, new_value_16);
  }
  if (attr_id == ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID ||
      attr_id == ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_VOLTAGE_ID ||
      attr_id == ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_VOLTAGE_ID)
  {
    zcl_pwr_cfg_check_battery_voltage_or_percentage_value(endpoint, *new_value,
                                                          attr_id-ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_ATTR_SUBTRAHEND, ZB_FALSE);
  }
  else if (attr_id == ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID ||
           attr_id == ZB_ZCL_ATTR_POWER_CONFIG_BATTERY2_PERCENTAGE_REMAINING_ID ||
           attr_id == ZB_ZCL_ATTR_POWER_CONFIG_BATTERY3_PERCENTAGE_REMAINING_ID)
  {
    zcl_pwr_cfg_check_battery_voltage_or_percentage_value(endpoint, *new_value,
                                                          attr_id-ZB_ZCL_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ATTR_SUBTRAHEND, ZB_TRUE);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_power_config_write_attr_hook", (FMT__0));
}

/**
 *  @} internal
*/

#endif /* ZB_ZCL_SUPPORT_CLUSTER_POWER_CONFIG */
