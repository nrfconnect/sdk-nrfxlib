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
/* PURPOSE: ZCL Color Control cluster specific commands handling
*/

#define ZB_TRACE_FILE_ID 2062

#include "zb_common.h"

#if defined (ZB_ZCL_SUPPORT_CLUSTER_COLOR_CONTROL)

#include "zb_time.h"
#include "zb_zcl.h"
#include "zb_aps.h"

static void zb_zcl_color_control_update_color_attrbute(zb_uint8_t endpoint);

/** @internal
    @{
*/

static zb_uint8_t gs_color_control_client_generated_commands[] =
{
  ZB_ZCL_CLUSTER_ID_COLOR_CONTROL_CLIENT_ROLE_GENERATED_CMD_LIST
};

static zb_discover_cmd_list_t gs_color_control_client_cmd_list =
{
  0, NULL,
  sizeof(gs_color_control_client_generated_commands), gs_color_control_client_generated_commands
};

static zb_discover_cmd_list_t gs_color_control_server_cmd_list =
{
  sizeof(gs_color_control_client_generated_commands), gs_color_control_client_generated_commands,
  0, NULL
};

zb_ret_t check_value_color_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_ret_t check_value_color_control_client(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value);
zb_bool_t zb_zcl_process_color_control_specific_commands_srv(zb_uint8_t param);
zb_bool_t zb_zcl_process_color_control_specific_commands_cli(zb_uint8_t param);
void zb_zcl_process_color_control_specific_commands_srv_2param(zb_uint8_t buf2_param, zb_uint16_t cmd_param);

void zb_zcl_color_control_init_server()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              check_value_color_control_server,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_color_control_specific_commands_srv);
}

void zb_zcl_color_control_init_client()
{
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                              ZB_ZCL_CLUSTER_CLIENT_ROLE,
                              check_value_color_control_client,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              zb_zcl_process_color_control_specific_commands_cli);
}

zb_ret_t check_value_color_control_server(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;
  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID:
      ret = (*value <= ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID:
      ret = (*value <= ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID:
    case ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ID:
    case ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ID:
      ret = (*value <= ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_CURRENT_HUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ID:
      ret = (*value <= ZB_ZCL_CMD_COLOR_CONTROL_LOOP_DIRECTION_INCREMENT) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ID:
      ret = (ZB_ZCL_ATTR_GET16(value) <= ZB_ZCL_COLOR_CONTROL_COLOR_CAPABILITIES_MAX_VALUE) ? RET_OK : RET_ERROR;
      break;
    case ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID:
      if( ZB_ZCL_ATTR_GET16(value) > ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX )
      {
        ret = RET_ERROR;
      }
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL1, "check_value_color_control ret %hd", (FMT__H, ret));
  return ret;
}

zb_ret_t check_value_color_control_client(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  zb_ret_t ret = RET_OK;

  ZVUNUSED(endpoint);

  switch( attr_id )
  {
    case ZB_ZCL_ATTR_GLOBAL_CLUSTER_REVISION_ID:
      if( ZB_ZCL_ATTR_GET16(value) > ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX )
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
/** @addtogroup ZB_ZCL_COLOR_CONTROL
    ZCL Color Control cluster Helper functions
    @{
*/

/** @brief Get 8bit attribute value from Color Control cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @return attribute value
*/
static zb_uint8_t zb_zcl_color_control_get8(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id);
  ZB_ASSERT(attr_desc);
  return ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
}

/** @brief Get 16bit attribute value from Color Control cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @return attribute value
*/
static zb_uint16_t zb_zcl_color_control_get16(zb_uint8_t endpoint, zb_uint16_t attr_id)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
    ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, attr_id);
  ZB_ASSERT(attr_desc);
  return ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
}

/** @brief Set 8bit attribute value from Color Control cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @param value - new attribute value
*/
static void zb_zcl_color_control_set8(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint8_t value)
{
  /* Use SET_ATTRIBUTE to report attribute change if needed. */
  ZB_ZCL_SET_ATTRIBUTE(endpoint,
                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       attr_id,
                       &value,
                       ZB_FALSE);
}

/** @brief Set 16bit attribute value from Color Control cluster
    @param endpoint - endpoint
    @param attr_id - attribute ID
    @param value - new attribute value
*/
static void zb_zcl_color_control_set16(zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint16_t value)
{
  /* Use SET_ATTRIBUTE to report attribute change if needed. */
  ZB_ZCL_SET_ATTRIBUTE(endpoint,
                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                       ZB_ZCL_CLUSTER_SERVER_ROLE,
                       attr_id,
                       (zb_uint8_t*)&value,
                       ZB_FALSE);
}

/** @brief Set color mode attribute and call change attribute color - see ZCL spec 5.2.2.3.1
    @param endpoint - endpoint
    @param mode - new color mode
*/
static void zb_zcl_color_control_set_color_mode(zb_uint8_t endpoint, zb_uint8_t mode)
{
  zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID);
  ZB_ASSERT(attr_desc);

  if(ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc)!=mode)
  {
    // TODO convert color attribute
    zb_zcl_color_control_update_color_attrbute(endpoint);
  }

  ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(attr_desc, mode);

  zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID, mode);
}

/** @brief Invoke User App and set new value of attribute
    @param param - buffer with @see zb_zcl_color_control_invoke_user_t. Also use User App
    @return result invoke User App.
*/
static zb_ret_t zb_zcl_color_control_invoke_user_and_set_attribute(zb_uint8_t param)
{
  zb_ret_t status = RET_OK;
  zb_zcl_attr_t * attr_desc;
  zb_zcl_color_control_invoke_user_t user_data;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_color_control_invoke_user_and_set_attribute %hd", (FMT__H, param));
  ZB_MEMCPY(&user_data, ZB_BUF_GET_PARAM(param, zb_zcl_color_control_invoke_user_t),
        sizeof(zb_zcl_color_control_invoke_user_t));

  attr_desc = zb_zcl_get_attr_desc_a(user_data.endpoint,
      ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, user_data.attr_id);

  ZB_ASSERT(attr_desc);

  if(attr_desc->type == ZB_ZCL_ATTR_TYPE_U16)
  {
    if( ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) != user_data.new_value)
    {
      ZB_ZCL_INVOKE_USER_APP_SET_ATTR_WITH_RESULT(param, user_data.endpoint,
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, attr_desc, &(user_data.new_value), status);
      TRACE_MSG(TRACE_ZCL2, "attr_desc %p new value %d", (FMT__P_D, attr_desc, user_data.new_value));

      /* ZB_ZCL_SET_DIRECTLY_ATTR_VAL16(user_data.attr_desc, user_data.new_value); */
      /* Yes, it is not the best way to set attribute - SET_ATTRIBUTE will search again for
       * attr_desc by ep/cluster/attr_id when we already have it.
       * TODO: Modify ZCL api to bypass these getters if attr_desc is given.
       */
      ZB_ZCL_SET_ATTRIBUTE(user_data.endpoint,
                           ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           user_data.attr_id,
                           (zb_uint8_t*)&user_data.new_value,
                           ZB_FALSE);
    }
  }
  else
  {
    zb_uint8_t val8 = (zb_uint8_t)(user_data.new_value);
    if( ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) != val8)
    {
      ZB_ZCL_INVOKE_USER_APP_SET_ATTR_WITH_RESULT(param, user_data.endpoint,
        ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, attr_desc, &val8, status);
      TRACE_MSG(TRACE_ZCL2, "attr_desc %p new value %hd", (FMT__P_H, attr_desc, val8));
      /* ZB_ZCL_SET_DIRECTLY_ATTR_VAL8(user_data.attr_desc, val8); */
      ZB_ZCL_SET_ATTRIBUTE(user_data.endpoint,
                           ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                           ZB_ZCL_CLUSTER_SERVER_ROLE,
                           user_data.attr_id,
                           (zb_uint8_t*)&val8,
                           ZB_FALSE);
    }
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_color_control_invoke_user_and_set_attribute %hd", (FMT__D, status));
  return status;
}

/**
 *  @}
*/

/** @addtogroup ZB_ZCL_COLOR_CONTROL
    ZCL Color Control cluster Convert color between triplex "Hue,Saturation" - "ColorXY" - "Color Temperature"
    @{
    Use convert in/out RGB
    Need to recode without use float type.
*/

#define ABS(a) ((a)>0 ? (a) : -(a))

typedef struct zb_zcl_color_control_color_convert_s
{
  zb_uint8_t hue;           /** Hue */
  zb_uint8_t saturation;    /** Saturation */
  zb_uint8_t color_x;       /** Color X, high byte */
  zb_uint8_t color_y;       /** Color Y, high byte */
  zb_uint16_t color_temp;    /** Color Temperature, high byte */
} zb_zcl_color_control_color_convert_t;

#define ZB_ZCL_COLOR_CONTROL_CONVERT_COLOR_COUNT  8

static zb_zcl_color_control_color_convert_t zb_zcl_color_control_convert_table[ZB_ZCL_COLOR_CONTROL_CONVERT_COLOR_COUNT] =
{
    { 0x00, 0x00,   0,   0,     0},   // black
    { 0x00, 0xff, 242, 255,  5000},   // white
    { 0x00, 0xff, 106,  54,  4013},   // red
    { 0x2a, 0xff, 196, 237,  4000},   // red+green
    { 0x55, 0xff,  91, 182,  5713},   // green
    { 0x80, 0xff, 137, 201,  9406},   // green+blue
    { 0xaa, 0xff,  46,  18, 25000},   // blue
    { 0xd5, 0xff, 151,  73,  4020},   // blue+red
};

static zb_uint8_t zb_zcl_color_control_hue_sat_to_index(zb_uint8_t hue, zb_uint8_t sat)
{
  zb_uint8_t index=0;
  zb_uint16_t offset = ABS(hue-zb_zcl_color_control_convert_table[0].hue)+
                        ABS(sat-zb_zcl_color_control_convert_table[0].saturation);
  zb_uint8_t i;
  zb_uint16_t tmp;

  for(i=1; i<ZB_ZCL_COLOR_CONTROL_CONVERT_COLOR_COUNT; i++)
  {
    tmp = ABS(hue-zb_zcl_color_control_convert_table[i].hue)+
          ABS(sat-zb_zcl_color_control_convert_table[i].saturation);
    if(tmp<offset)
    {
      index = i;
      offset = tmp;
    }
  }

  return index;
}

static zb_uint8_t zb_zcl_color_control_color_xy_to_index(zb_uint16_t color_x, zb_uint16_t color_y)
{
  zb_uint8_t x=color_x/256;
  zb_uint8_t y=color_y/256;
  zb_uint8_t index=0;
  zb_uint16_t offset = ABS(x-zb_zcl_color_control_convert_table[0].color_x)+
                        ABS(y-zb_zcl_color_control_convert_table[0].color_y);
  zb_uint8_t i;
  zb_uint16_t tmp;

  for(i=1; i<ZB_ZCL_COLOR_CONTROL_CONVERT_COLOR_COUNT; i++)
  {
    tmp = ABS(x-zb_zcl_color_control_convert_table[i].color_x)+
          ABS(y-zb_zcl_color_control_convert_table[i].color_y);
    if(tmp<offset)
    {
      index = i;
      offset = tmp;
    }
  }

  return index;
}

static zb_uint8_t zb_zcl_color_control_color_temp_to_index(zb_uint16_t color_temp)
{
  zb_uint8_t index=0;
  zb_uint16_t offset = ABS(color_temp-zb_zcl_color_control_convert_table[0].color_temp);
  zb_uint8_t i;
  zb_uint16_t tmp;

  for(i=1; i<ZB_ZCL_COLOR_CONTROL_CONVERT_COLOR_COUNT; i++)
  {
    tmp = ABS(color_temp-zb_zcl_color_control_convert_table[i].color_temp);
    if(tmp<offset)
    {
      index = i;
      offset = tmp;
    }
  }

  return index;
}

static void zb_zcl_color_control_update_color_attrbute(zb_uint8_t endpoint)
{
  zb_uint8_t index;
  zb_uint8_t hue = ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_DEFAULT_VALUE;
  zb_uint8_t sat;
  zb_uint16_t x;
  zb_uint16_t y;
  zb_uint16_t temp;
  zb_uint8_t old_mode;
  zb_uint16_t ex_hue;

  old_mode = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID);
  switch(old_mode)
  {
    case ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX:
      ex_hue = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID);
      hue = ex_hue >> 8;
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, hue);
      // next - run ordinary hue and saturation mode
      /* FALLTHROUGH */
    case ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION:
      hue = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID);
      sat = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID);
      index = zb_zcl_color_control_hue_sat_to_index(hue, sat);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID,
          zb_zcl_color_control_convert_table[index].color_x << 8);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID,
          zb_zcl_color_control_convert_table[index].color_y << 8);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID,
          zb_zcl_color_control_convert_table[index].color_temp);
      break;
    case ZB_ZCL_COLOR_CONTROL_COLOR_MODE_CURRENT_X_Y:
      x = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID);
      y = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID);
      index = zb_zcl_color_control_color_xy_to_index(x, y);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,
          zb_zcl_color_control_convert_table[index].hue);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,
          zb_zcl_color_control_convert_table[index].saturation);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID,
          zb_zcl_color_control_convert_table[index].color_temp);
      hue = zb_zcl_color_control_convert_table[index].hue;
      break;
    case ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE:
      temp = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID);
      index = zb_zcl_color_control_color_temp_to_index(temp);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID,
          zb_zcl_color_control_convert_table[index].color_x << 8);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID,
          zb_zcl_color_control_convert_table[index].color_y << 8);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,
          zb_zcl_color_control_convert_table[index].hue);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,
          zb_zcl_color_control_convert_table[index].saturation);
      hue = zb_zcl_color_control_convert_table[index].hue;
      break;
  }

  if(old_mode!=ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX)
  {
    zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID, hue << 8);
  }
}

#if 0

#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)>(b) ? (a) : (b))

/** Convert RGB to Hue,Saturation
 * RGB - [0..0xffff]
 * Hue,Sat - [0..0xff]
*/
#define RGB_to_HS(r, g, b, h, s)                            \
{                                                           \
  zb_uint16_t min, max, delta;                              \
  min = MIN( (r), (g) );                                    \
  min = MIN( min, (b) );                                    \
  max = MAX( (r), (g) );                                    \
  max = MAX( max, (b) );                                    \
  delta = max - min;                                        \
  if( max == 0 )                                            \
  {                                                         \
    (s) = (h) = 0;                                          \
  }                                                         \
  else                                                      \
  {                                                                   \
    s = (zb_uint8_t)(ZB_UINT8_MAX * delta / max);  /* s  */           \
                                                                      \
    if( (r) == max )      /* between yellow & magenta*/               \
      h = (zb_uint8_t)(ZB_UINT8_MAX/6 * ( (g) - (b) ) / delta);       \
    else if( (g) == max ) /* between cyan & yellow*/                  \
      h = (zb_uint8_t)(ZB_UINT8_MAX/6 * (2 + ( (b) - (r) ) / delta)); \
    else                  /* between magenta & cyan*/                 \
      h = (zb_uint8_t)(ZB_UINT8_MAX/6 * (4 + ( (r) - (g) ) / delta)); \
  }                                                                   \
}

/** Convert Hue,Saturation,V to RGB
 * RGB - [0..0xffff]
 * Hue,Sat - [0..0xff]
 * V always = (ZB_UINT16_MAX-1)
*/
#define HSV_to_RGB(h, s, v, r, g, b )           \
{                                               \
  zb_uint8_t i;                                 \
  zb_uint8_t sector = ZB_UINT8_MAX/6;           \
  zb_uint16_t f, p, q, t;                       \
  if( s == 0 ) {  /* achromatic (grey)*/        \
    r = g = b = (v);                            \
  }                                             \
  else                                          \
  {                                             \
    i = h / sector;  /* sector 0 to 5 */        \
    f = h % sector; /* factorial part of h*/    \
    p = (zb_uint16_t)(v * ( 1.0 - s/ZB_UINT8_MAX ));                            \
    q = (zb_uint16_t)(v * ( 1.0 - s/ZB_UINT8_MAX * f/(float)sector ));          \
    t = (zb_uint16_t)(v * ( 1.0 - s/ZB_UINT8_MAX * ( 1 - f/(float)sector ) ));  \
    switch( i ) {                               \
      case 0: r = (v); g = t; b = p; break;     \
      case 1: r = q; g = (v); b = p; break;     \
      case 2: r = p; g = (v); b = t; break;     \
      case 3: r = p; g = q; b = (v); break;     \
      case 4: r = t; g = p; b = (v); break;     \
      case 5:                                   \
      default: r = (v); g = p; b = q; break;    \
    }                                           \
  }                                             \
}

/** Convert ColorXY to RGB
 * r, g, b - [0, 0xffff]
 * x, y- [0, 0xffff]
 * */
#define XY_to_RGB(x, y, r, g, b)                                  \
{                                                                 \
  zb_uint16_t z = ZB_UINT16_MAX - (x) - (y);                      \
  r = (zb_uint16_t)( 3.240479*(x) -1.537150*(y) -0.498535*(z));   \
  g = (zb_uint16_t)(-0.969256*(x) +1.875992*(y) +0.041556*(z));   \
  b = (zb_uint16_t)( 0.055648*(x) -0.204043*(y) +1.057311*(z));   \
}

/** Convert RGB to ColorXY
 * r, g, b - [0, 0xffff]
 * x, y - [0, 0xffff]
 * */
#define RGB_to_XY(r, g, b, x, y)                                  \
{                                                                 \
  x = (zb_uint16_t)( 0.412453*(r) +0.357580*(g) +0.180423*(b));   \
  y = (zb_uint16_t)( 0.212671*(r) +0.715160*(g) +0.072169*(b));   \
/*z = (zb_uint16_t)( 0.019334*(r) +0.119193*(g) +0.950227*(b)); */\
}

/** Convert Color temperature to ColorXY
 * T - Color temperature, t_kelvin=1000000/T, see spec 5.2.2.2.1.8
 * t form define = 1000/t_kelvin = T/1000
 * */
#define KELVIN_to_XY(T, x, y)                                                 \
{                                                                             \
  /*float t = 1000.0/t_kelvin;*/                                              \
  float t = T/1000.0;                                                         \
  float X, Y;                                                                 \
  if(T>1000/4  /*t_kelvin < 4000.0f*/)                                        \
  {                                                                           \
    X = ((-0.2661239f*t - 0.2343580f)*t + 0.8776956f)*t + 0.17991f;           \
    if(T>1000000/2222 /*t_kelvin < 2222.0f*/)                                 \
      Y = ((-1.1063814f*X - 1.34811020f)*X + 2.18555832f)*X - 0.20219683f;    \
    else                                                                      \
      Y = ((-0.9549476f*X - 1.37418593f)*X + 2.09137015f)*X - 0.16748867f;    \
  }                                                                           \
  else                                                                        \
  {                                                                           \
    X = ((-3.0258469f*t + 2.1070379f)*t + 0.2226347f)*t + 0.24039f;           \
    Y =  ((3.0817580f*X - 5.8733867f)*X + 3.75112997f)*X - 0.37001483f;       \
  }                                                                           \
  x = (zb_uint16_t)(ZB_UINT16_MAX*X);                                         \
  y = (zb_uint16_t)(ZB_UINT16_MAX*Y);                                         \
}

/** Convert ColorXY to Color temperature
 * T - Color temperature, t_kelvin=1000000/T, see spec 5.2.2.2.1.8
 * T = 1000000/t_kelvin
 * */
#define XY_to_KELVIN(x, y, T)                                                 \
{                                                                             \
  float n = (x - 0.3320*ZB_UINT16_MAX)/(y - 0.1858*ZB_UINT16_MAX);            \
  T = (zb_uint16_t)1000000/(((-449*n + 3525)*n - 6823.3)*n + 5520.33);        \
}

/** @brief Update Color attributes when change Color Mode attribute
    @param endpoint - endpoint
*/
static void zb_zcl_color_control_update_color_attrbute(zb_uint8_t endpoint)
{
  zb_uint16_t r, g, b;
  zb_uint8_t hue;
  zb_uint8_t sat;
  zb_uint16_t x;
  zb_uint16_t y;
  zb_uint16_t temp;
  zb_uint8_t old_mode = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID);
#if defined ZB_ENABLE_ZLL
  zb_uint16_t ex_hue;
  old_mode = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID);
#endif

  switch(old_mode)
  {
#if defined ZB_ENABLE_ZLL
    case ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX:
      ex_hue = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID);
      hue = ex_hue >> 8;
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, hue);
      // next - run ordinary hue and saturation mode
#endif
    case ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION:
      hue = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID);
      sat = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID);
      HSV_to_RGB(hue, sat, (ZB_UINT16_MAX-1), r, g, b);
      RGB_to_XY(r, g, b, x, y);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID, x);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID, y);
      XY_to_KELVIN(x, y, temp);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID, temp);
      break;
    case ZB_ZCL_COLOR_CONTROL_COLOR_MODE_CURRENT_X_Y:
      x = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID);
      y = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID);
      XY_to_RGB(x, y, r, g, b);
      RGB_to_HS(r, g, b, hue, sat);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, hue);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID, sat);
      XY_to_KELVIN(x, y, temp);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID, temp);
      break;
    case ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE:
      temp = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID);
      KELVIN_to_XY(temp, x, y);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID, x);
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID, y);
      XY_to_RGB(x, y, r, g, b);
      RGB_to_HS(r, g, b, hue, sat);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, hue);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID, sat);
      break;
  }

#if defined ZB_ENABLE_ZLL
  if(old_mode!=ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX)
  {
    zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID, hue << 8);
  }
#endif
}

#endif
/**
 *  @}
*/

/**
 * @brief Define for Invoke User App and set attribute value
 * see @ref zb_zcl_color_control_invoke_user_s
 *
 * @param buf - buffer with struct @ref zb_zcl_color_control_invoke_user_s 'zb_zcl_color_control_invoke_user_t'
 * @param attr_desc_ - attribute descriptor
 * @param val - new attribute value
 * @param status - invoke User App result
 */
static zb_bool_t zb_zcl_color_control_invoke_user_app(zb_bufid_t buf, zb_uint8_t endpoint, zb_uint16_t attr_id, zb_uint16_t val)
{
  zb_zcl_color_control_invoke_user_t *user_data =
      ZB_BUF_GET_PARAM((buf), zb_zcl_color_control_invoke_user_t);
  user_data->endpoint = endpoint;
  user_data->attr_id = attr_id;
  user_data->new_value = val;
  return ((zb_bool_t) zb_zcl_color_control_invoke_user_and_set_attribute(buf));
}

/** @brief Process one interation of move or step command for one attribute
 * @param el_data - pointer on data, @see zb_zcl_color_control_loop_element_t
 * @return result invoke User App */
static zb_bool_t zb_zcl_process_color_control_element_loop(zb_zcl_color_control_loop_element_t *el_data)
{
  zb_zcl_attr_t * attr_desc;
  zb_int32_t val;
  zb_ret_t status;
  zb_bool_t is_16bit;
  zb_bool_t is_finish = ZB_FALSE;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_element_loop", (FMT__0));

  attr_desc = zb_zcl_get_attr_desc_a(el_data->endpoint,
      ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, el_data->attr_id);
  ZB_ASSERT(attr_desc);

  // calcutate new value
  is_16bit = (attr_desc->type == ZB_ZCL_ATTR_TYPE_U16) ? ZB_TRUE : ZB_FALSE;
  val = (is_16bit) ? ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc) :
                     ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);
  TRACE_MSG(TRACE_ZCL2, "val_old %d delta %d", (FMT__D_D, val, el_data->value));
  val += el_data->value;

  if(el_data->value> 0 && val > el_data->limit)
  {
    if (el_data->is_continue)
    {
      val -= (is_16bit ? ZB_UINT16_MAX : ZB_UINT8_MAX);
    }
    else
    {
      val = el_data->limit; is_finish = ZB_TRUE;
    }
  }
  else if(el_data->value< 0 && val < el_data->limit)
  {
    if (el_data->is_continue)
    {
        val += (is_16bit ? ZB_UINT16_MAX : ZB_UINT8_MAX);
    }
    else
    {
      val = el_data->limit; is_finish = ZB_TRUE;
    }
  }

  TRACE_MSG(TRACE_ZCL2, "val_new %d", (FMT__D, val));

  status = zb_zcl_color_control_invoke_user_app(el_data->buffer_id, el_data->endpoint, el_data->attr_id, val);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_element_loop %hd", (FMT__D, status));

  return (status==RET_OK && !is_finish) ? ZB_TRUE : ZB_FALSE;
}

static zb_bool_t color_control_check_req_options(zb_uint8_t param, zb_uint8_t endpoint)
{
  zb_zcl_color_control_req_options_t req_options;
  zb_bool_t res = ZB_TRUE;
  zb_zcl_attr_t *attr_desc;

#ifdef ZB_ZCL_SUPPORT_CLUSTER_ON_OFF
  zb_af_endpoint_desc_t *ep_desc;
  zb_zcl_cluster_desc_t *cluster_desc;
#endif

  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint,
    ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_COLOR_CONTROL_OPTIONS_ID);

  if (attr_desc != NULL)
  {
    zb_uint8_t options = ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc);

    ZB_ZCL_COLOR_CONTROL_GET_CMD_OPTIONS(param, req_options);

    /* Check and override bits if needed */
    if (ZB_CHECK_BIT_IN_BIT_VECTOR(&req_options.options_mask,
                                   ZB_ZCL_COLOR_CONTROL_OPTIONS_EXECUTE_IF_OFF))
    {
      if (ZB_CHECK_BIT_IN_BIT_VECTOR(&req_options.options_override,
                                     ZB_ZCL_COLOR_CONTROL_OPTIONS_EXECUTE_IF_OFF))
      {
        ZB_SET_BIT_IN_BIT_VECTOR(&options,
                                 ZB_ZCL_COLOR_CONTROL_OPTIONS_EXECUTE_IF_OFF);
      }
      else
      {
        ZB_CLR_BIT_IN_BIT_VECTOR(&options,
                                 ZB_ZCL_COLOR_CONTROL_OPTIONS_EXECUTE_IF_OFF);
      }
    }


#ifdef ZB_ZCL_SUPPORT_CLUSTER_ON_OFF
    /* ZCL7, 5.2.2.2.1.30 ExecuteIfOff Options Bit
       Command execution SHALL NOT continue beyond the Options procedding if all of thee criteria
       are true:
       - The On/Off cluster exists on the same endpoint as this cluster
       - The OnOff attribute of the On/Off cluster, on this endpoint, is 0x00 (FALSE)
       - The value of the ExecuteIfOff bit is 0
     */
    ep_desc = zb_af_get_endpoint_desc(endpoint);
    cluster_desc = get_cluster_desc(ep_desc, ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_SERVER_ROLE);
    /* On/Off cluster may not be on the endpoint.
     * In this case return true and continue command execution .*/
    if (cluster_desc)
    {
      attr_desc = zb_zcl_get_attr_desc(cluster_desc, ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID);

      if (attr_desc &&
          !ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc) && /* On/Off attrbute is Off */
          !ZB_CHECK_BIT_IN_BIT_VECTOR(&options, ZB_ZCL_LEVEL_CONTROL_OPTIONS_EXECUTE_IF_OFF)) /* ExecuteIfOff
                                                                                               * is False */
      {
        /* ExecuteIfOff policy, deny */
        res = ZB_FALSE;
      }
    }
#endif
  }

  TRACE_MSG(TRACE_ZCL1, "color_control_check_req_options: enabled %hd", (FMT__H, res));
  return res;
}

static zb_bool_t color_control_check_color_capabilities(zb_uint8_t endpoint, zb_uint8_t cmd_id)
{
  zb_bool_t res = ZB_FALSE;
  zb_zcl_attr_t *attr_desc;

  attr_desc = zb_zcl_get_attr_desc_a(
    endpoint,
    ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
    ZB_ZCL_CLUSTER_SERVER_ROLE,
    ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ID);

  if (attr_desc != NULL)
  {
    zb_uint16_t color_capabilites = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    switch (cmd_id)
    {
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE:
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE:
      case ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE:
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION:
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION:
      case ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION:
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION:
        res = (zb_bool_t)(color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_HUE_SATURATION);
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR:
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR:
      case ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR:
        res = (zb_bool_t)(color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_X_Y);
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE:
      case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE:
      case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE:
      case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION:
        res = (zb_bool_t)(color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_EX_HUE);
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET:
        res = (zb_bool_t)(color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_COLOR_LOOP);
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP:
        res = (zb_bool_t)((color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_HUE_SATURATION)
                       || (color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_EX_HUE)
                       || (color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_X_Y)
                       || (color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_COLOR_TEMP));
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE:
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE:
      case ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE:
        res = (zb_bool_t)(color_capabilites & ZB_ZCL_COLOR_CONTROL_CAPABILITIES_COLOR_TEMP);
        break;

      default:
        res = ZB_FALSE;
    }
  }

  TRACE_MSG(TRACE_ZCL1, "color_control_check_color_capabilities: supported %hd", (FMT__H, res));

  return res;
}

/** @brief Process one interation of move command
 * @param param - buffer with move loop data, @see zb_zcl_color_control_move_loop_t
 *
 * Function convert from time offset between current time and last processed time plus
 * non-used msec (saved in loop data as time_mod fields) to delta value.
 * New non-used msec save in loop data as time_mod fields.
 *
 * Its make separate for first and second (if need) attribute.
 * */
static void zb_zcl_process_color_control_move_loop(zb_uint8_t param)
{
  zb_time_t current_time = ZB_TIMER_GET();
  zb_zcl_color_control_move_loop_t loop_data;
  zb_zcl_color_control_loop_element_t element;
  zb_bool_t is_continue = ZB_FALSE;
  zb_uint16_t time_offset;
  zb_int16_t delta;
  zb_bool_t is_non_stop_attr;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_loop %hd", (FMT__H, param));

  // save loop data - param buffer may change for invoke User App
  ZB_MEMCPY(&loop_data, ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_loop_t),
      sizeof(zb_zcl_color_control_move_loop_t));

  // calc time offset
  time_offset = ZB_TIME_BEACON_INTERVAL_TO_MSEC(ZB_TIME_SUBTRACT(current_time, loop_data.time_last));
  TRACE_MSG(TRACE_ZCL1, "time offset %hd msec time_mod1 %d time_mod2 %d", (FMT__D_D_D,
    time_offset, loop_data.time_mod, loop_data.time_mod2));

  // prepare struct for process each move command
  is_non_stop_attr = ((loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID)
#if defined ZB_ENABLE_ZLL
        || (loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID)
#endif /*defined ZB_ENABLE_ZLL*/
        ) ? ZB_TRUE : ZB_FALSE;

  // calc delta attribute value and update non-used msec for first attribute
  delta = loop_data.rate * (time_offset + loop_data.time_mod) / ZB_ZCL_COLOR_CONTROL_SEC_TO_MSEC;
  loop_data.time_mod = (time_offset + loop_data.time_mod) - (delta *  ZB_ZCL_COLOR_CONTROL_SEC_TO_MSEC / loop_data.rate);

  // update first attribute
  ZB_ZCL_COLOR_CONTROL_FILL_LOOP_ELEMENT(element,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint, param,
      loop_data.attr_id, delta, is_non_stop_attr, loop_data.limit);

  if (zb_zcl_process_color_control_element_loop( &element ))
  {
    if(loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID)
    {
      if(zb_zcl_color_control_get16(element.endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID)
         != loop_data.limit)
      {
        is_continue = ZB_TRUE;
      }
    }
    else
    {
      is_continue = ZB_TRUE;
    }
  }

  // if second attribute is exist
  if(loop_data.attr_id2!=0)
  {
  // calc delta attribute value and update non-used msec for second attribute
    delta = loop_data.rate2 * (time_offset + loop_data.time_mod2) / ZB_ZCL_COLOR_CONTROL_SEC_TO_MSEC;
    loop_data.time_mod2 = (time_offset + loop_data.time_mod2) - (delta *  ZB_ZCL_COLOR_CONTROL_SEC_TO_MSEC / loop_data.rate2);

  // update second attribute
    ZB_ZCL_COLOR_CONTROL_FILL_LOOP_ELEMENT(element,
        ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint, param,
        loop_data.attr_id2, delta, ZB_FALSE, loop_data.limit2);
    if (zb_zcl_process_color_control_element_loop( &element ))
    {
      is_continue = ZB_TRUE;
    }
  }

  //
  if (is_continue)
  {
    TRACE_MSG(TRACE_ZCL2, "Schedule next", (FMT__0));
    loop_data.time_last = current_time;
    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_loop_t), &loop_data, sizeof(loop_data));
    ZB_SCHEDULE_ALARM(zb_zcl_process_color_control_move_loop, param, 5);
  }
  else    // send response command
  {
    TRACE_MSG(TRACE_ZCL2, "Finish", (FMT__0));
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_loop", (FMT__0));
}

/** @brief Process one interation of "move to" command
 * @param param - buffer with move to loop data, @see zb_zcl_color_control_move_to_loop_t
 *
 * Function convert from time offset between current time and last processed time plus
 * non-used msec (saved in loop data as time_mod fields) to delta value.
 * New non-used msec save in loop data as time_mod fields.
 *
 * Its make separate for first and second (if need) attribute.
 * */
static void zb_zcl_process_color_control_move_to_loop(zb_uint8_t param)
{
  zb_zcl_color_control_move_to_loop_t loop_data;
  zb_time_t current_time = ZB_TIMER_GET();
  zb_uint16_t delta_time;
  zb_int32_t rm_time_unit;
  zb_int32_t val;
  zb_zcl_attr_t * attr_desc;
  zb_ret_t status;
  zb_bool_t is_16bit;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_to_loop %hd curr_time %d", (FMT__H_D, param, current_time));

  ZB_MEMCPY(&loop_data, ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_to_loop_t),
      sizeof(zb_zcl_color_control_move_to_loop_t));

  delta_time = ZB_BEACON_INTERVAL_TO_TIME_UNITS(ZB_TIME_SUBTRACT(current_time, loop_data.start_time));
  rm_time_unit = (zb_int32_t)loop_data.transition_time - delta_time;

  attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint,
      ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, loop_data.attr_id);
  ZB_ASSERT(attr_desc);

  TRACE_MSG(TRACE_ZCL2, "start time %d curr time %d all time %d delta time %d remaining time %d", (FMT__D_D_D_D_D,
      loop_data.start_time, current_time, loop_data.transition_time, delta_time, rm_time_unit));

  // process first attribute
  is_16bit = (attr_desc->type == ZB_ZCL_ATTR_TYPE_U16) ? ZB_TRUE : ZB_FALSE;
  if (rm_time_unit!=0)
  {
    zb_int32_t unit_max = is_16bit ? ZB_UINT16_MAX : ZB_UINT8_MAX;
    zb_int32_t all_delta =
      (loop_data.finish_value > loop_data.start_value) ?
      (loop_data.finish_value - loop_data.start_value) :
      (loop_data.start_value - loop_data.finish_value);
    zb_int32_t step_value;

    if((loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID)
        || (loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID)
    )
    {
      if (loop_data.incr_direction && loop_data.finish_value < loop_data.start_value)
      {
        all_delta += unit_max;
      }
      if (!loop_data.incr_direction && loop_data.finish_value > loop_data.start_value)
      {
        all_delta -= unit_max;
      }
    }

    step_value =
      (delta_time == 0U) ?
      (all_delta) :
      (zb_int32_t)(all_delta * delta_time / loop_data.transition_time);

    val =
      (loop_data.finish_value > loop_data.start_value) ?
      (loop_data.start_value + step_value) :
      (loop_data.start_value - step_value);

    if (val > unit_max)
    {
      val -= unit_max;
    }
  }
  else
  {
    val = loop_data.finish_value;
  }

  //
  TRACE_MSG(TRACE_ZCL2, "attr_id 0x%x val_st %d val_end %u val %d", (FMT__D_D_D_D,
      loop_data.attr_id, loop_data.start_value, loop_data.finish_value, val));
  status = zb_zcl_color_control_invoke_user_app(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint, loop_data.attr_id, val);

  // process second attribute
  if(loop_data.attr_id2!=0)
  {
    attr_desc = zb_zcl_get_attr_desc_a(ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint,
        ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, loop_data.attr_id2);
    ZB_ASSERT(attr_desc);

    if (rm_time_unit!=0)
    {
      zb_int32_t all_delta =
        (loop_data.finish_value2 > loop_data.start_value2) ?
        (loop_data.finish_value2 - loop_data.start_value2) :
        (loop_data.start_value2 - loop_data.finish_value2);

      zb_int32_t step_value =
        (delta_time == 0U) ?
        (all_delta) :
        (zb_int32_t)(all_delta * delta_time / loop_data.transition_time);

      val =
        (loop_data.finish_value2 > loop_data.start_value2) ?
        (loop_data.start_value2 + step_value) :
        (loop_data.start_value2 - step_value);
    }
    else
    {
      val = loop_data.finish_value2;
    }

    TRACE_MSG(TRACE_ZCL2, "attr_id2 0x%x val_st2 %d val_end2 %d val %d", (FMT__D_D_D_D,
        loop_data.attr_id2, loop_data.start_value2, loop_data.finish_value2, val));
    status = zb_zcl_color_control_invoke_user_app(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint, loop_data.attr_id2, val);
  }

  // set Remaining Time attr, if exist
  TRACE_MSG(TRACE_ZCL2, "Remaining Time units %d", (FMT__D, rm_time_unit));
  zb_zcl_color_control_set16(ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint,
      ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID,
      rm_time_unit>0 ? rm_time_unit : 0);

  //
  if (rm_time_unit>0)
  {
    TRACE_MSG(TRACE_ZCL2, "Schedule next", (FMT__0));
    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_to_loop_t), &loop_data, sizeof(loop_data));
    ZB_SCHEDULE_ALARM(zb_zcl_process_color_control_move_to_loop, param, 5);
  }
  else    // finish
  {
    TRACE_MSG(TRACE_ZCL2, "Finish. Status = %d", (FMT__D, status));
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_to_loop", (FMT__0));
}

/** @brief Process one interation of step command
 * @param param - buffer with step loop data, @see zb_zcl_color_control_step_loop_t
 *
 * Function calculate number steps between between current time and last processed time.
 * If steps >0 then correct attributes value and update last processed time.
 * Last process time = previous last process time plus time for steps (sharp).
 * New last process time <= Current lime.
 * */
static void zb_zcl_process_color_control_step_loop(zb_uint8_t param)
{
  zb_zcl_color_control_step_loop_t loop_data;
  zb_zcl_color_control_loop_element_t element;
  zb_bool_t is_continue = ZB_FALSE;
  zb_time_t current_time = ZB_TIMER_GET();
  zb_uint8_t steps;
  zb_bool_t is_non_stop_attr;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_step_loop %hd", (FMT__H, param));

  ZB_MEMCPY(&loop_data, ZB_BUF_GET_PARAM(param, zb_zcl_color_control_step_loop_t),
      sizeof(zb_zcl_color_control_step_loop_t));

  TRACE_MSG(TRACE_ZCL2, "endpoint %hd", (FMT__H,
      ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint));

  // calc steps
  steps = ZB_TIME_SUBTRACT(current_time, loop_data.last_time) / loop_data.step_time;
  if(steps!=0)
  {
    is_non_stop_attr = ((loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID)
        || (loop_data.attr_id == ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID)
         ) ? ZB_TRUE : ZB_FALSE;

    ZB_ZCL_COLOR_CONTROL_FILL_LOOP_ELEMENT(element,
         ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint, param,
         loop_data.attr_id, steps*loop_data.rate, is_non_stop_attr, loop_data.limit);
    is_continue = zb_zcl_process_color_control_element_loop( &element );

    if (loop_data.attr_id2!=0)
    {
      ZB_ZCL_COLOR_CONTROL_FILL_LOOP_ELEMENT(element,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint,
          param, loop_data.attr_id2, steps*loop_data.rate2,
          ZB_FALSE, loop_data.limit2);
      is_continue |= zb_zcl_process_color_control_element_loop( &element );
    }
  }
  else
  {
    is_continue = ZB_TRUE;
  }

  if ( is_continue )
  {
    zb_time_t alarm_time = ZB_TIME_SUBTRACT(
          loop_data.last_time + (steps+1)*loop_data.step_time, current_time);
    if(alarm_time>ZB_HALF_MAX_TIME_VAL)
    {
      alarm_time = 0;
    }
    loop_data.last_time += steps*loop_data.step_time;

    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_color_control_step_loop_t), &loop_data, sizeof(loop_data));
    ZB_SCHEDULE_ALARM(zb_zcl_process_color_control_step_loop, param, alarm_time);
  }
  else    // finish
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_step_loop", (FMT__0));
}

/** @brief Stop any loop command
 * @param func - address loop funcion
 * @param size_loop_data - sizeof of loop data structure
 *
 * All struct data for loop command (@see zb_zcl_color_control_move_to_loop_t,
 * @see zb_zcl_color_control_move_loop_t, @see zb_zcl_color_control_step_loop_t)
 * contain @see zb_zcl_parsed_hdr_t as first field.
 *
 * Fuction find and remove schedule alarm by 'func' parameters,
 * get association buffer, copy @see zb_zcl_parsed_hdr_t struct by size_loop_data
 * parameter and invoke ZB_ZCL_PROCESS_COMMAND_FINISH.
 * */
static void zb_zcl_process_color_control_stop(zb_callback_t func, zb_uint8_t size_loop_data)
{
  zb_uint8_t param;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_stop %p %hd", (FMT__P_H, func, size_loop_data));

  ZB_SCHEDULE_ALARM_CANCEL_AND_GET_BUF(func, ZB_ALARM_ALL_CB, &param);
  if(param!=0)
  {
    zb_buf_free(param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_stop", (FMT__0));
}

/** @brief Stop all loop commands from Color Control cluster
 * @param endpoint - endpoint. Not used. Function stop All loop command without
 * current endpoint
 * */
static void zb_zcl_process_color_control_stop_all_commands(zb_uint8_t endpoint)
{
  (void)endpoint;
  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_stop_all_commands", (FMT__0));
  zb_zcl_process_color_control_stop(zb_zcl_process_color_control_move_to_loop,
      sizeof(zb_zcl_color_control_move_to_loop_t));
  zb_zcl_process_color_control_stop(zb_zcl_process_color_control_move_loop,
      sizeof(zb_zcl_color_control_move_loop_t));
  zb_zcl_process_color_control_stop(zb_zcl_process_color_control_step_loop,
      sizeof(zb_zcl_color_control_step_loop_t));
  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_stop_all_commands", (FMT__0));
}

///////////////////
/** @brief Move To Hue command
    @see ZCL spec, subclause 5.2.2.3.2 */
static zb_ret_t zb_zcl_process_color_control_move_to_hue_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_to_hue_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_to_hue_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_HUE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_HUE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.hue > ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MAX_VALUE)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_bool_t direction = ZB_FALSE;
    zb_uint32_t val_end;
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_uint8_t curr_val = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID);

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

    switch(payload.direction)
    {
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SHORTEST:
        val_end = payload.hue + (payload.hue <= curr_val ? ZB_UINT8_MAX : 0);
        direction = (val_end - curr_val <= ZB_UINT8_MAX/2) ? ZB_TRUE : ZB_FALSE;
        break;
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_LONGEST:
        val_end = payload.hue + (payload.hue <= curr_val ? ZB_UINT8_MAX : 0) ? ZB_TRUE : ZB_FALSE;
        direction = (val_end - curr_val >= ZB_UINT8_MAX/2) ? ZB_TRUE : ZB_FALSE;
        break;
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_UP:
        direction=ZB_TRUE;
        break;
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_DOWN:
        direction=ZB_FALSE;
        break;
    }

    ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,
        curr_val, payload.hue, direction, payload.transition_time);

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_to_hue_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move Hue command
    @see ZCL8 spec, subclause 5.2.2.3.5 */
static zb_ret_t zb_zcl_process_color_control_move_hue_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_hue_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_hue_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_HUE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_HUE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    if (payload.move_mode == ZB_ZCL_CMD_COLOR_CONTROL_MOVE_STOP)
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
    }
    /* Condition is fixed according to CCB2501*/
    else if(payload.rate==0 && (payload.move_mode == ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP ||
                                payload.move_mode == ZB_ZCL_CMD_COLOR_CONTROL_MOVE_DOWN))
    {
      ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
      ret = RET_BUSY;   // not need send answer yet
    }
    else
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
      zb_zcl_color_control_set_color_mode(endpoint,
          ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

      ZB_ZCL_COLOR_CONTROL_FILL_MOVE_DATA(param, &cmd_info,
          ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,
          (payload.move_mode==ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP ? 1 : -1) *
          payload.rate, ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MAX_VALUE);

      ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_loop, param);

      ret = RET_BUSY;

      ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
      buf2_param = 0;
    }
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_hue_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Step Hue command
    @see ZCL spec, subclause 5.2.2.3.4 */
static zb_ret_t zb_zcl_process_color_control_step_hue_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_step_hue_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_step_hue_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_STEP_HUE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_STEP_HUE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.step_size == 0)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint,
          ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

    ZB_ZCL_COLOR_CONTROL_FILL_STEP_DATA(param, &cmd_info,
        ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,
        payload.step_size*(payload.step_mode== ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP ? 1 : -1),
        payload.transition_time,
        (payload.step_mode== ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP ? ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MAX_VALUE : 0));

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_step_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_step_hue_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move To Saturation command
    @see ZCL spec, subclause 5.2.2.3.5 */
static zb_ret_t zb_zcl_process_color_control_move_to_saturation_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_to_saturation_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_to_saturation_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_SATURATION_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_SATURATION_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.saturation > ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MAX_VALUE)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_uint8_t sat = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID);

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

    ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,
        sat, payload.saturation, (sat < payload.saturation), payload.transition_time);

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_to_saturation_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move Saturation command
    @see ZCL spec, subclause 5.2.2.3.6 */
static zb_ret_t zb_zcl_process_color_control_move_saturation_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_saturation_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_saturation_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_SATURATION_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_SATURATION_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    if(payload.move_mode==ZB_ZCL_CMD_COLOR_CONTROL_MOVE_STOP)
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
    }
    else if(payload.rate==0)
    {
      ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
      ret = RET_BUSY;   // not need send answer yet
    }
    else
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
      zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

      ZB_ZCL_COLOR_CONTROL_FILL_MOVE_DATA(param, &cmd_info,
          ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,
          (payload.move_mode==ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP ? 1 : -1) *
          payload.rate, ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MAX_VALUE);

      ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_loop, param);

      ret = RET_BUSY;

      ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
      buf2_param = 0;
    }
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_saturation_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Step Saturation command
    @see ZCL spec, subclause 5.2.2.3.7 */
static zb_ret_t zb_zcl_process_color_control_step_saturation_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_step_saturation_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_step_saturation_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_STEP_SATURATION_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_STEP_SATURATION_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.step_size == 0)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

    ZB_ZCL_COLOR_CONTROL_FILL_STEP_DATA(param, &cmd_info,  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID,
        payload.step_size*(payload.step_mode== ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP ? 1 : -1),
        payload.transition_time,
        (payload.step_mode== ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP ? ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MAX_VALUE : 0));

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_step_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_step_saturation_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move To Hue And Saturation command
    @see ZCL spec, subclause 5.2.2.3.8 */
static zb_ret_t zb_zcl_process_color_control_move_to_hue_saturation_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_to_hue_saturation_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_to_hue_saturation_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_HUE_SATURATION_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_HUE_SATURATION_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.hue > ZB_ZCL_COLOR_CONTROL_CURRENT_HUE_MAX_VALUE ||
           payload.saturation > ZB_ZCL_COLOR_CONTROL_CURRENT_SATURATION_MAX_VALUE)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_zcl_color_control_move_to_loop_t *loop_data;
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
              ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID);
    ZB_ASSERT(attr_desc);

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

    ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID,
        ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc), payload.hue,
        payload.hue > ZB_ZCL_GET_ATTRIBUTE_VAL_8(attr_desc), payload.transition_time);

    loop_data = ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_to_loop_t);
    loop_data->attr_id2 = ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID;
    loop_data->start_value2 =  zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID);
    loop_data->finish_value2 = payload.saturation;

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_to_hue_saturation_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move To Color command
    @see ZCL spec, subclause 5.2.2.3.9 */
static zb_ret_t zb_zcl_process_color_control_move_to_color_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_to_color_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_to_color_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_COLOR_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_COLOR_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.color_x > ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE ||
           payload.color_y > ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_zcl_color_control_move_to_loop_t *loop_data;
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
              ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID);
    ZB_ASSERT(attr_desc);

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_CURRENT_X_Y);

    ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID,
        ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc), payload.color_x,
        payload.color_x > ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc), payload.transition_time);

    //
    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
              ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID);
    ZB_ASSERT(attr_desc);

    loop_data = ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_to_loop_t);
    loop_data->attr_id2 = ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID;
    loop_data->start_value2 = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);
    loop_data->finish_value2 = payload.color_y;

    TRACE_MSG(TRACE_ZCL2, "start time %d val1 start 0x%x val1 end 0x%x val2 start 0x%x val2 end 0x%x",
        (FMT__D_D_D_D_D, loop_data->start_time, loop_data->start_value, loop_data->finish_value,
            loop_data->start_value2, loop_data->finish_value2));

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_to_color_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move Color command
    @see ZCL spec, subclause 5.2.2.3.10 */
static zb_ret_t zb_zcl_process_color_control_move_color_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_color_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_color_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_COLOR_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_COLOR_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    if (payload.rate_x==0 && payload.rate_y==0)
    {
        zb_zcl_process_color_control_stop_all_commands(endpoint);
    }
    else
    {
      zb_zcl_color_control_move_loop_t *loop_data;

      zb_zcl_process_color_control_stop_all_commands(endpoint);
      zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_CURRENT_X_Y);

      ZB_ZCL_COLOR_CONTROL_FILL_MOVE_DATA(param, &cmd_info,
          ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID, payload.rate_x,
          ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE);

      loop_data = ZB_BUF_GET_PARAM((param), zb_zcl_color_control_move_loop_t);
      loop_data->attr_id2 = ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID;
      loop_data->rate2 = payload.rate_y;
      loop_data->limit2 = ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE;

      ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_loop, param);
      ret = RET_BUSY;

      ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
      buf2_param = 0;
    }
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_color_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Step Color command
    @see ZCL spec, subclause 5.2.2.3.11 */
static zb_ret_t zb_zcl_process_color_control_step_color_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_step_color_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_step_color_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_STEP_COLOR_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_STEP_COLOR_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_zcl_color_control_step_loop_t *loop_data;
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_CURRENT_X_Y);

    ZB_ZCL_COLOR_CONTROL_FILL_STEP_DATA(param, &cmd_info,  ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_X_ID,
        payload.step_x, payload.transition_time,
        ((zb_int16_t)payload.step_x>0 ? ZB_ZCL_COLOR_CONTROL_CURRENT_X_MAX_VALUE : 0));

    loop_data = ZB_BUF_GET_PARAM(param, zb_zcl_color_control_step_loop_t);
    loop_data->attr_id2 = ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_Y_ID;
    loop_data->rate2 = payload.step_y;
    loop_data->limit2 = ((zb_int16_t)payload.step_y>0 ? ZB_ZCL_COLOR_CONTROL_CURRENT_Y_MAX_VALUE : 0);

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_step_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_step_color_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Move To Color Temperature command
    @see ZCL spec, subclause 5.2.2.3.12 */
static zb_ret_t zb_zcl_process_color_control_move_to_color_temp_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_to_color_temperature_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_to_color_temp_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_COLOR_TEMPERATURE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_TO_COLOR_TEMP_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.color_temperature == ZB_ZCL_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIREDS_INVALID_VALUE)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload parameter", (FMT__0));
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
              ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID);
    ZB_ASSERT(attr_desc);

    TRACE_MSG(TRACE_ZCL1, "payload.color_temperature %d", (FMT__D, payload.color_temperature));

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    TRACE_MSG(TRACE_ZCL1, "Prepare loop", (FMT__0));

    if( payload.color_temperature < zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ID))
    {
      payload.color_temperature = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ID);
    }
    else if (payload.color_temperature > zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ID))
    {
      payload.color_temperature = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ID);
    }

      zb_zcl_process_color_control_stop_all_commands(endpoint);
      zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE);

      ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID,
          ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc), payload.color_temperature,
          payload.color_temperature > ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc), payload.transition_time);

      ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);
      ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH(buf2_param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
    }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_to_color_temp_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Enhanced Move To Hue command
    @see ZCL spec, subclause 5.2.2.3.14 */
static zb_ret_t zb_zcl_process_color_control_enhanced_move_to_hue_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_enhanced_move_to_hue_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_enhanced_move_to_hue_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_TO_HUE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_TO_HUE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_bool_t direction = ZB_FALSE;
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_uint16_t curr_val = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID);
    zb_uint32_t val_end;

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);
    zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID,
            ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX);

    switch(payload.direction)
    {
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SHORTEST:
        val_end = payload.enhanced_hue + (payload.enhanced_hue <= curr_val ? ZB_UINT16_MAX : 0);
        direction = (val_end - curr_val <= ZB_UINT16_MAX/2) ? ZB_TRUE : ZB_FALSE;
        break;
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_LONGEST:
        val_end = payload.enhanced_hue + (payload.enhanced_hue <= curr_val ? ZB_UINT8_MAX : 0);
        direction = (val_end - curr_val >= ZB_UINT16_MAX/2) ? ZB_TRUE : ZB_FALSE;
        break;
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_UP:
        direction=ZB_TRUE;
        break;
      case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_DOWN:
        direction=ZB_FALSE;
        break;
    }

    ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID,
        curr_val, payload.enhanced_hue, direction, payload.transition_time);

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_enhanced_move_to_hue_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Enhanced Move Hue command
    @see ZCL spec, subclause 5.2.2.3.15 */
static zb_ret_t zb_zcl_process_color_control_enhanced_move_hue_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_enhanced_move_hue_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_enhanced_move_hue_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_HUE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_HUE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    if(payload.move_mode==ZB_ZCL_CMD_COLOR_CONTROL_MOVE_STOP)
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
    }
    else if(payload.rate==0)
    {
      ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
      ret = RET_BUSY;   // not need send answer yet
    }
    else
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
      zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID
        , ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX);

      ZB_ZCL_COLOR_CONTROL_FILL_MOVE_DATA(param, &cmd_info,
        ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID,
        (payload.move_mode==ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP ? 1 : -1) *
        payload.rate, ZB_ZCL_COLOR_CONTROL_ENHANCED_HUE_MAX_DEF_VALUE);

      ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_loop, param);

      ret = RET_BUSY;

      ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
      buf2_param = 0;
    }
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_enhanced_move_hue_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Enhanced Step Hue command
    @see ZCL spec, subclause 5.2.2.3.16 */
static zb_ret_t zb_zcl_process_color_control_enhanced_step_hue_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_enhanced_step_hue_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_enhanced_step_hue_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_STEP_HUE_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_STEP_HUE_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (payload.step_size == 0)
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);
    zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID
      , ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX);

    ZB_ZCL_COLOR_CONTROL_FILL_STEP_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID,
        payload.step_size*(payload.step_mode== ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP ? 1 : -1),
        payload.transition_time, ZB_ZCL_COLOR_CONTROL_ENHANCED_HUE_MAX_DEF_VALUE);

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_step_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_enhanced_step_hue_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Enhanced Move To Hue Saturation command
    @see ZCL spec, subclause 5.2.2.3.17 */
static zb_ret_t zb_zcl_process_color_control_enhanced_move_to_hue_saturation_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_enhanced_move_to_hue_saturation_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_enhanced_move_to_hue_saturation_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));

  ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_TO_HUE_SATURATION_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_ENHANCED_MOVE_TO_HUE_SATURATION_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_zcl_color_control_move_to_loop_t *loop_data;
    zb_uint8_t endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;
    zb_zcl_attr_t *attr_desc = zb_zcl_get_attr_desc_a(endpoint,
              ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID);
    ZB_ASSERT(attr_desc);

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);
    zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID
      , ZB_ZCL_COLOR_CONTROL_COLOR_EX_MODE_HUE_SATURATION_EX);

    ZB_ZCL_COLOR_CONTROL_FILL_MOVE_TO_DATA(param, &cmd_info, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID,
        ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc), payload.enhanced_hue,
        payload.enhanced_hue > ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc), payload.transition_time);

    loop_data = ZB_BUF_GET_PARAM(param, zb_zcl_color_control_move_to_loop_t);
    loop_data->attr_id2 = ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID;
    loop_data->start_value2 = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID);
    loop_data->finish_value2 = payload.saturation;

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_to_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_enhanced_move_to_hue_saturation_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Process Color Loop Set command
 *
 *
 * Its continuous loop:
 * If the color loop is active, the device shall cycle over the complete range of values of the  1
 * EnhancedCurrentHue attribute in the direction of the ColorLoopDirection attribute over the time  2
 * specified in the ColorLoopTime attribute.
 *
 * For make one loop only need uncomment all string with 'is_finish' variable
 * and 'if (!is_finish) .. else ...' condition
 *
 * */
static void zb_zcl_process_color_control_color_loop(zb_uint8_t param)
{
  zb_zcl_color_control_color_loop_set_t loop_data;
  zb_zcl_attr_t * attr_desc;
  zb_uint8_t endpoint;
  zb_time_t current_time = ZB_TIMER_GET();
  zb_uint8_t loop_active;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_color_loop %hd", (FMT__H, param));

 ZB_MEMCPY(&loop_data, ZB_BUF_GET_PARAM(param, zb_zcl_color_control_color_loop_set_t),
      sizeof(zb_zcl_color_control_color_loop_set_t));

  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&(loop_data.cmd_info)).dst_endpoint;

  loop_active = zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ID);

  if (loop_active != ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_DEACTIVATE)
  {
    zb_int8_t direction;
    zb_uint16_t max_time;
    zb_int32_t val;
    zb_uint16_t val16;

    direction = (zb_zcl_color_control_get8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ID)
           == ZB_ZCL_CMD_COLOR_CONTROL_LOOP_DIRECTION_INCREMENT) ?  1 : -1;

    max_time = zb_zcl_color_control_get16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ID);

    attr_desc = zb_zcl_get_attr_desc_a(endpoint,
        ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID);
    ZB_ASSERT(attr_desc);
    val = ZB_ZCL_GET_ATTRIBUTE_VAL_16(attr_desc);

    TRACE_MSG(TRACE_ZCL1, "val %hd direct %hd max_time %hd max time beacon %d delta time %d", (FMT__D_D_D_D_D,
      val, direction, max_time, ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000* max_time),
      ZB_TIME_SUBTRACT(current_time, loop_data.last_time)));

    val += direction * (ZB_UINT16_MAX / (ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000* max_time)) *
                       ZB_TIME_SUBTRACT(current_time, loop_data.last_time));
    if (val >= (zb_int32_t)ZB_UINT16_MAX)
    {
      val -= ZB_UINT16_MAX;
    }
    if (val < 0)
    {
      val += ZB_UINT16_MAX;
    }

    val16 = (zb_uint16_t)val;

    TRACE_MSG(TRACE_ZCL1, "new val %hd", (FMT__D, val16));

    (void)zb_zcl_color_control_invoke_user_app(param, endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID, val16);
  }

  {
    zb_time_t alarm_time = ZB_TIME_SUBTRACT(loop_data.last_time + ZB_ZCL_COLOR_CONTROL_TIMER_BEACON_INTERVAL,
                                        current_time);
    if (alarm_time > ZB_HALF_MAX_TIME_VAL)
    {
      alarm_time = 0;
    }
    loop_data.last_time = current_time;

    ZB_MEMCPY(ZB_BUF_GET_PARAM(param, zb_zcl_color_control_color_loop_set_t), &loop_data, sizeof(loop_data));
    ZB_SCHEDULE_ALARM(zb_zcl_process_color_control_color_loop, param, alarm_time);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_color_loop", (FMT__0));
}

/** @brief Process Color Loop Set command
    @see ZCL spec, subclause 5.2.2.3.18 */
static zb_ret_t zb_zcl_process_color_control_color_loop_set_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_color_loop_set_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_color_loop_set_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_COLOR_CONTROL_GET_COLOR_LOOP_SET_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_COLOR_LOOP_SET_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_HUE_SATURATION);

    TRACE_MSG(TRACE_ZCL3, "payload.update_flags 0x%hx", (FMT__H, payload.update_flags));

    if((payload.update_flags & ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_DIRECTION) != 0)
    {
      TRACE_MSG(TRACE_ZCL3, "ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_DIRECTION: 0x%hx", (FMT__H, payload.direction));
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_DIRECTION_ID, payload.direction);
    }

    if((payload.update_flags & ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_TIME) != 0)
    {
      TRACE_MSG(TRACE_ZCL3, "ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_TIME: 0x%x", (FMT__D, payload.time));
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_TIME_ID, payload.time);
    }

    if((payload.update_flags & ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_START_HUE) != 0)
    {
      TRACE_MSG(TRACE_ZCL3, "ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_START_HUE: 0x%x", (FMT__D, payload.start_hue));
      zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_START_ENHANCED_HUE_ID, payload.start_hue);
    }

    if((payload.update_flags & ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_ACTION) != 0)
    {
      TRACE_MSG(TRACE_ZCL3, "ZB_ZCL_CMD_COLOR_CONTROL_LOOP_UPDATE_ACTION: 0x%hx", (FMT__H, payload.action));
      zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_ACTIVE_ID,
              payload.action==ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_DEACTIVATE ? 0 : 1);

      switch(payload.action)
      {
      case ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_DEACTIVATE:
        zb_zcl_process_color_control_stop(zb_zcl_process_color_control_color_loop,
              sizeof(zb_zcl_color_control_color_loop_set_t));

        ZB_ZCL_COLOR_CONTROL_COPY_ATTRIBUTE_16(endpoint,
              ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID,
              ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_STORED_ENHANCED_HUE_ID);
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_START_HUE:
        ZB_ZCL_COLOR_CONTROL_COPY_ATTRIBUTE_16(endpoint,
              ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_START_ENHANCED_HUE_ID,
              ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID);

        ZB_ZCL_COLOR_CONTROL_FILL_COLOR_LOOP_SET_DATA(param, &cmd_info);
        ZB_SCHEDULE_ALARM(zb_zcl_process_color_control_color_loop, param, 0);
        ret = RET_BUSY;
        ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
        buf2_param = 0;
        break;

      case ZB_ZCL_CMD_COLOR_CONTROL_LOOP_ACTION_CURRENT_HUE:
        ZB_ZCL_COLOR_CONTROL_COPY_ATTRIBUTE_16(endpoint,
              ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_LOOP_STORED_ENHANCED_HUE_ID,
              ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_CURRENT_HUE_ID);

        ZB_ZCL_COLOR_CONTROL_FILL_COLOR_LOOP_SET_DATA(param, &cmd_info);
        ZB_SCHEDULE_ALARM(zb_zcl_process_color_control_color_loop, param, 0);
        ret = RET_BUSY;
        ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
        buf2_param = 0;
        break;

      default:
        ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
        ret = RET_BUSY;   // not need send answer yet
        break;
      }
    }
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_color_loop_set_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Process Color Loop Stop command
    @see ZCL spec, subclause 5.2.2.3.19 */
static zb_ret_t zb_zcl_process_color_control_stop_move_step_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_stop_move_step_handler param %hx",
      (FMT__H, param));

    ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
    endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  if (!color_control_check_color_capabilities(endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (color_control_check_req_options(param, endpoint))
  {
    zb_zcl_color_control_set16(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_REMAINING_TIME_ID, 0);

    zb_zcl_process_color_control_stop_all_commands(endpoint);
  }

  zb_buf_free(buf2_param);

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_stop_move_step_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/* Get Temperature Limit by payload max/min field & direction */
static zb_uint16_t zb_zcl_color_control_get_color_temperature_limit(
    zb_uint8_t endpoint, zb_bool_t is_up, zb_uint16_t min, zb_uint16_t max)
{
  zb_uint16_t limit = is_up ? max : min;
  if (limit ==0)
  {
    limit = zb_zcl_color_control_get16(endpoint,
        is_up ? ((zb_uint16_t) ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ID) :
                ((zb_uint16_t) ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ID));
  }
  return limit;
}

/** @brief Process Move Color Temperature command
    @see ZCL spec, subclause 5.2.2.3.20 */
static zb_ret_t zb_zcl_process_color_control_move_color_temp_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_move_color_temp_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_move_color_temp_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_COLOR_CONTROL_GET_MOVE_COLOR_TEMP_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_MOVE_COLOR_TEMP_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    if(payload.move_mode==ZB_ZCL_CMD_COLOR_CONTROL_MOVE_STOP)
    {
      zb_zcl_process_color_control_stop_all_commands(endpoint);
    }
    else if(payload.rate==0)
    {
      ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_INVALID_VALUE);
      ret = RET_BUSY;   // not need send answer yet
    }
    else
    {
      zb_uint16_t limit = zb_zcl_color_control_get_color_temperature_limit(
        endpoint, (zb_bool_t)(payload.move_mode == ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP),
        payload.color_temp_min, payload.color_temp_max);

      zb_zcl_process_color_control_stop_all_commands(endpoint);
      zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE);

      ZB_ZCL_COLOR_CONTROL_FILL_MOVE_DATA(param, &cmd_info,
        ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID,
        (payload.move_mode== ZB_ZCL_CMD_COLOR_CONTROL_MOVE_UP ? 1 : -1) * payload.rate, limit);

      ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_move_loop, param);

      ret = RET_BUSY;

      ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
      buf2_param = 0;
    }
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_move_color_temp_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

/** @brief Process Step Color Temperature command
    @see ZCL spec, subclause 5.2.2.3.21 */
static zb_ret_t zb_zcl_process_color_control_step_color_temp_handler(zb_uint8_t param, zb_uint8_t buf2_param)
{
  zb_ret_t ret = RET_OK;
  zb_zcl_color_control_step_color_temp_req_t payload;
  zb_zcl_parse_status_t status;
  zb_zcl_parsed_hdr_t cmd_info;
  zb_uint8_t endpoint;

  TRACE_MSG(TRACE_ZCL1, "> zb_zcl_process_color_control_step_color_temp_handler param %hx",
      (FMT__H, param));

  ZB_MEMCPY(&cmd_info, ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t), sizeof(zb_zcl_parsed_hdr_t));
  endpoint = ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint;

  ZB_ZCL_COLOR_CONTROL_GET_STEP_COLOR_TEMP_REQ(param, payload, status);
  if (!color_control_check_color_capabilities(endpoint, cmd_info.cmd_id))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_UNSUP_CMD);
    ret = RET_BUSY;   // not need send answer yet
  }
  else if (status != ZB_ZCL_PARSE_STATUS_SUCCESS)
  {
    TRACE_MSG(TRACE_ZCL1, "Error payload of ZB_ZCL_COLOR_CONTROL_GET_STEP_COLOR_TEMP_REQ",
        (FMT__0));
    ret = RET_INVALID_PARAMETER_1;
  }
  else if (!color_control_check_req_options(param, endpoint))
  {
    ZB_ZCL_PROCESS_COMMAND_FINISH(param, &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    ret = RET_BUSY;   // not need send answer yet
  }
  else
  {
    zb_uint16_t limit;

    if (payload.transition_time == 0)
    {
      payload.transition_time++;
    }

    zb_zcl_process_color_control_stop_all_commands(endpoint);
    limit = zb_zcl_color_control_get_color_temperature_limit(
        endpoint, (zb_bool_t)(payload.step_mode == ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP),
        payload.color_temp_min, payload.color_temp_max);

    zb_zcl_color_control_set_color_mode(endpoint, ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE);
    zb_zcl_color_control_set8(endpoint, ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID
      , ZB_ZCL_COLOR_CONTROL_COLOR_MODE_TEMPERATURE);

    ZB_ZCL_COLOR_CONTROL_FILL_STEP_DATA(param, &cmd_info,  ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ID,
        payload.step_size*(payload.step_mode== ZB_ZCL_CMD_COLOR_CONTROL_STEP_UP ? 1 : -1),
        payload.transition_time, limit);

    ZB_SCHEDULE_CALLBACK(zb_zcl_process_color_control_step_loop, param);

    ret = RET_BUSY;

    ZB_ZCL_PROCESS_COMMAND_FINISH((buf2_param), &cmd_info, ZB_ZCL_STATUS_SUCCESS);
    buf2_param = 0;
  }

  if (buf2_param != 0)
  {
    zb_buf_free(buf2_param);
  }

  TRACE_MSG(TRACE_ZCL1, "< zb_zcl_process_color_control_step_color_temp_handler ret %hx",
      (FMT__H, ret));
  return ret;
}

zb_bool_t zb_zcl_process_color_control_specific_commands_srv(zb_uint8_t param)
{
  zb_bool_t processed = ZB_FALSE;
  zb_zcl_parsed_hdr_t *cmd_info;

  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_color_control_server_cmd_list;
    return ZB_TRUE;
  }

  cmd_info = ZB_BUF_GET_PARAM(param, zb_zcl_parsed_hdr_t);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_color_control_specific_commands_srv: parambuf %d, cmd_info %d",
             (FMT__H_H, param, cmd_info->cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_COLOR_CONTROL == cmd_info->cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info->cmd_direction);

  if ( (cmd_info->cmd_id <= ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE) ||
       (cmd_info->cmd_id >= ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE &&
        cmd_info->cmd_id <= ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET) ||
       cmd_info->cmd_id == ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP ||
       cmd_info->cmd_id == ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE ||
       cmd_info->cmd_id == ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE
    )
  {
    zb_buf_get_out_delayed_ext(zb_zcl_process_color_control_specific_commands_srv_2param, param, 0);
    processed = ZB_TRUE;
  }

  return processed;
}

void zb_zcl_process_color_control_specific_commands_srv_2param(zb_uint8_t buf2_param, zb_uint16_t cmd_param)
{
  zb_zcl_parsed_hdr_t cmd_info;
  zb_ret_t status = RET_OK;
  zb_uint8_t param = cmd_param;

  ZB_ZCL_COPY_PARSED_HEADER(param, &cmd_info);

  TRACE_MSG( TRACE_ZCL1,
             "> zb_zcl_process_color_control_specific_commands_srv_2param: parambuf %d, cmd_info %d",
             (FMT__H_H, param, cmd_info.cmd_id));

  ZB_ASSERT(ZB_ZCL_CLUSTER_ID_COLOR_CONTROL == cmd_info.cluster_id);
  ZB_ASSERT(ZB_ZCL_FRAME_DIRECTION_TO_SRV == cmd_info.cmd_direction);

  switch( cmd_info.cmd_id )
  {
  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE:
    status = zb_zcl_process_color_control_move_to_hue_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE:
    status = zb_zcl_process_color_control_move_hue_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE:
    status = zb_zcl_process_color_control_step_hue_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION:
    status = zb_zcl_process_color_control_move_to_saturation_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION:
    status = zb_zcl_process_color_control_move_saturation_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION:
    status = zb_zcl_process_color_control_step_saturation_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION:
    status = zb_zcl_process_color_control_move_to_hue_saturation_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR:
    status = zb_zcl_process_color_control_move_to_color_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR:
    status = zb_zcl_process_color_control_move_color_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR:
    status = zb_zcl_process_color_control_step_color_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE:
    status = zb_zcl_process_color_control_move_to_color_temp_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE:
    status = zb_zcl_process_color_control_enhanced_move_to_hue_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE:
    status = zb_zcl_process_color_control_enhanced_move_hue_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE:
    status = zb_zcl_process_color_control_enhanced_step_hue_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION:
    status = zb_zcl_process_color_control_enhanced_move_to_hue_saturation_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET:
    status = zb_zcl_process_color_control_color_loop_set_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP:
    status = zb_zcl_process_color_control_stop_move_step_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE:
    status = zb_zcl_process_color_control_move_color_temp_handler(param, buf2_param);
    break;

  case ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE:
    status = zb_zcl_process_color_control_step_color_temp_handler(param, buf2_param);
    break;

  default:
    /* processed = ZB_FALSE; */
    ZB_ASSERT(0);               /* should not happen */
    break;
  }

  /* if( processed ) */
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
      ZB_ZCL_SEND_DEFAULT_RESP( param,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).source.u.short_addr,
          ZB_APS_ADDR_MODE_16_ENDP_PRESENT,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).src_endpoint,
          ZB_ZCL_PARSED_HDR_SHORT_DATA(&cmd_info).dst_endpoint,
          cmd_info.profile_id,
          ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
          cmd_info.seq_number,
          cmd_info.cmd_id,
          status==RET_OK ? ZB_ZCL_STATUS_SUCCESS : ZB_ZCL_STATUS_INVALID_FIELD);
    }
  }

  TRACE_MSG( TRACE_ZCL1,
             "< zb_zcl_process_color_control_specific_commands_srv_2param",
             (FMT__0));
}

zb_bool_t zb_zcl_process_color_control_specific_commands_cli(zb_uint8_t param)
{
  if ( ZB_ZCL_GENERAL_GET_CMD_LISTS_PARAM == param )
  {
    ZCL_CTX().zb_zcl_cluster_cmd_list = &gs_color_control_client_cmd_list;
    return ZB_TRUE;
  }
  return ZB_FALSE;
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE implementation */

static inline void zb_zcl_color_control_send_move_to_hue_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                    zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                    zb_uint8_t ep, zb_uint16_t prof_id,
                                                    zb_uint8_t def_resp, zb_callback_t cb,
                                                    zb_uint8_t hue, zb_uint8_t direction,
                                                    zb_uint16_t transition_time,
                                                    zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_to_hue_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                  zb_uint8_t ep, zb_uint16_t prof_id,
                                                                  zb_uint8_t def_resp, zb_callback_t cb,
                                                                  zb_uint8_t hue, zb_uint8_t direction,
                                                                  zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_to_hue_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                    zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                    zb_uint8_t ep, zb_uint16_t prof_id,
                                                    zb_uint8_t def_resp, zb_callback_t cb,
                                                    zb_uint8_t hue, zb_uint8_t direction,
                                                    zb_uint16_t transition_time,
                                                    zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_hue_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_hue_req_rev1(buffer, dst_addr,
                                                     dst_addr_mode, dst_ep,
                                                     ep, prof_id,
                                                     def_resp, cb,
                                                     hue, direction,
                                                     transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_hue_req_rev3(buffer, dst_addr,
                                                     dst_addr_mode, dst_ep,
                                                     ep, prof_id,
                                                     def_resp, cb,
                                                     hue, direction,
                                                     transition_time,
                                                     options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_hue_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_to_hue_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                               zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                               zb_uint8_t ep, zb_uint16_t prof_id,
                                               zb_uint8_t def_resp, zb_callback_t cb,
                                               zb_uint8_t hue, zb_uint8_t direction,
                                               zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_hue_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_hue_req_rev1(buffer, dst_addr,
                                                     dst_addr_mode, dst_ep,
                                                     ep, prof_id,
                                                     def_resp, cb,
                                                     hue, direction,
                                                     transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_hue_req_rev3(buffer, dst_addr,
                                                     dst_addr_mode, dst_ep,
                                                     ep, prof_id,
                                                     def_resp, cb,
                                                     hue, direction,
                                                     transition_time,
                                                     ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                     ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_hue_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE implementation */

static inline void zb_zcl_color_control_send_move_hue_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                 zb_uint8_t move_mode, zb_uint8_t rate,
                                                 zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_hue_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                               zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                               zb_uint8_t ep, zb_uint16_t prof_id,
                                                               zb_uint8_t def_resp, zb_callback_t cb,
                                                               zb_uint8_t move_mode, zb_uint8_t rate)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_hue_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                 zb_uint8_t move_mode, zb_uint8_t rate,
                                                 zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_hue_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_hue_req_rev1(buffer, dst_addr,
                                                  dst_addr_mode, dst_ep,
                                                  ep, prof_id,
                                                  def_resp, cb,
                                                  move_mode, rate);
      break;
    case 3:
      zb_zcl_color_control_send_move_hue_req_rev3(buffer, dst_addr,
                                                  dst_addr_mode, dst_ep,
                                                  ep, prof_id,
                                                  def_resp, cb,
                                                  move_mode, rate,
                                                  options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_hue_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_hue_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                            zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                            zb_uint8_t ep, zb_uint16_t prof_id,
                                            zb_uint8_t def_resp, zb_callback_t cb,
                                            zb_uint8_t move_mode, zb_uint8_t rate)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_hue_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_hue_req_rev1(buffer, dst_addr,
                                                     dst_addr_mode, dst_ep,
                                                     ep, prof_id,
                                                     def_resp, cb,
                                                     move_mode, rate);
      break;
    case 3:
      zb_zcl_color_control_send_move_hue_req_rev3(buffer, dst_addr,
                                                     dst_addr_mode, dst_ep,
                                                     ep, prof_id,
                                                     def_resp, cb,
                                                     move_mode, rate,
                                                     ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                     ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_hue_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE implementation */

static inline void zb_zcl_color_control_send_step_hue_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                 zb_uint8_t step_mode, zb_uint8_t step_size,
                                                 zb_uint8_t transition_time,
                                                 zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_step_hue_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                               zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                               zb_uint8_t ep, zb_uint16_t prof_id,
                                                               zb_uint8_t def_resp, zb_callback_t cb,
                                                               zb_uint8_t step_mode, zb_uint8_t step_size,
                                                               zb_uint8_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_step_hue_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                 zb_uint8_t step_mode, zb_uint8_t step_size,
                                                 zb_uint8_t transition_time,
                                                 zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_hue_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_hue_req_rev1(buffer, dst_addr,
                                                  dst_addr_mode, dst_ep,
                                                  ep, prof_id,
                                                  def_resp, cb,
                                                  step_mode, step_size,
                                                  transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_step_hue_req_rev3(buffer, dst_addr,
                                                  dst_addr_mode, dst_ep,
                                                  ep, prof_id,
                                                  def_resp, cb,
                                                  step_mode, step_size,
                                                  transition_time,
                                                  options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_hue_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_step_hue_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                            zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                            zb_uint8_t ep, zb_uint16_t prof_id,
                                            zb_uint8_t def_resp, zb_callback_t cb,
                                            zb_uint8_t step_mode, zb_uint8_t step_size,
                                            zb_uint8_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_hue_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_hue_req_rev1(buffer, dst_addr,
                                                  dst_addr_mode, dst_ep,
                                                  ep, prof_id,
                                                  def_resp, cb,
                                                  step_mode, step_size,
                                                  transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_step_hue_req_rev3(buffer, dst_addr,
                                                  dst_addr_mode, dst_ep,
                                                  ep, prof_id,
                                                  def_resp, cb,
                                                  step_mode, step_size,
                                                  transition_time,
                                                  ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                  ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_hue_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION implementation */

static inline void zb_zcl_color_control_send_move_to_saturation_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                           zb_uint8_t ep, zb_uint16_t prof_id,
                                                           zb_uint8_t def_resp, zb_callback_t cb,
                                                           zb_uint8_t saturation,
                                                           zb_uint16_t transition_time,
                                                           zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_to_saturation_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                         zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                         zb_uint8_t ep, zb_uint16_t prof_id,
                                                                         zb_uint8_t def_resp, zb_callback_t cb,
                                                                         zb_uint8_t saturation,
                                                                         zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_to_saturation_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                           zb_uint8_t ep, zb_uint16_t prof_id,
                                                           zb_uint8_t def_resp, zb_callback_t cb,
                                                           zb_uint8_t saturation,
                                                           zb_uint16_t transition_time,
                                                           zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_saturation_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_saturation_req_rev1(buffer, dst_addr,
                                                            dst_addr_mode, dst_ep,
                                                            ep, prof_id,
                                                            def_resp, cb,
                                                            saturation,
                                                            transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_saturation_req_rev3(buffer, dst_addr,
                                                            dst_addr_mode, dst_ep,
                                                            ep, prof_id,
                                                            def_resp, cb,
                                                            saturation,
                                                            transition_time,
                                                            options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_saturation_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_to_saturation_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                      zb_uint8_t saturation,
                                                      zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_saturation_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_saturation_req_rev1(buffer, dst_addr,
                                                            dst_addr_mode, dst_ep,
                                                            ep, prof_id,
                                                            def_resp, cb,
                                                            saturation,
                                                            transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_saturation_req_rev3(buffer, dst_addr,
                                                            dst_addr_mode, dst_ep,
                                                            ep, prof_id,
                                                            def_resp, cb,
                                                            saturation,
                                                            transition_time,
                                                            ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                            ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_saturation_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION implementation */

static inline void zb_zcl_color_control_send_move_saturation_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint8_t move_mode, zb_uint8_t rate,
                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_saturation_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                                      zb_uint8_t move_mode, zb_uint8_t rate)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (rate));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_saturation_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint8_t move_mode, zb_uint8_t rate,
                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_saturation_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_saturation_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate);
      break;
    case 3:
      zb_zcl_color_control_send_move_saturation_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate,
                                                         options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_saturation_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_saturation_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint8_t move_mode, zb_uint8_t rate)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_saturation_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_saturation_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate);
      break;
    case 3:
      zb_zcl_color_control_send_move_saturation_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_saturation_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION implementation */

static inline void zb_zcl_color_control_send_step_saturation_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint8_t step_mode, zb_uint8_t step_size,
                                                        zb_uint8_t transition_time,
                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_step_saturation_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                                      zb_uint8_t step_mode, zb_uint8_t step_size,
                                                                      zb_uint8_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_step_saturation_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint8_t step_mode, zb_uint8_t step_size,
                                                        zb_uint8_t transition_time,
                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_saturation_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_saturation_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode, step_size,
                                                         transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_step_saturation_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode, step_size,
                                                         transition_time,
                                                         options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_saturation_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_step_saturation_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint8_t step_mode, zb_uint8_t step_size,
                                                   zb_uint8_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_saturation_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_saturation_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode, step_size,
                                                         transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_step_saturation_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode, step_size,
                                                         transition_time,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_saturation_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION implementation */

static inline void zb_zcl_color_control_send_move_to_hue_saturation_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                               zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                               zb_uint8_t ep, zb_uint16_t prof_id,
                                                               zb_uint8_t def_resp, zb_callback_t cb,
                                                               zb_uint8_t hue,
                                                               zb_uint8_t saturation,
                                                               zb_uint16_t transition_time,
                                                               zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_to_hue_saturation_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                                                             zb_uint8_t def_resp, zb_callback_t cb,
                                                                             zb_uint8_t hue,
                                                                             zb_uint8_t saturation,
                                                                             zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_to_hue_saturation_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                               zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                               zb_uint8_t ep, zb_uint16_t prof_id,
                                                               zb_uint8_t def_resp, zb_callback_t cb,
                                                               zb_uint8_t hue,
                                                               zb_uint8_t saturation,
                                                               zb_uint16_t transition_time,
                                                               zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_hue_saturation_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_hue_saturation_req_rev1(buffer, dst_addr,
                                                                dst_addr_mode, dst_ep,
                                                                ep, prof_id,
                                                                def_resp, cb,
                                                                hue,
                                                                saturation,
                                                                transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_hue_saturation_req_rev3(buffer, dst_addr,
                                                                dst_addr_mode, dst_ep,
                                                                ep, prof_id,
                                                                def_resp, cb,
                                                                hue,
                                                                saturation,
                                                                transition_time,
                                                                options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_hue_saturation_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_to_hue_saturation_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                          zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                          zb_uint8_t ep, zb_uint16_t prof_id,
                                                          zb_uint8_t def_resp, zb_callback_t cb,
                                                          zb_uint8_t hue,
                                                          zb_uint8_t saturation,
                                                          zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_hue_saturation_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_hue_saturation_req_rev1(buffer, dst_addr,
                                                                dst_addr_mode, dst_ep,
                                                                ep, prof_id,
                                                                def_resp, cb,
                                                                hue,
                                                                saturation,
                                                                transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_hue_saturation_req_rev3(buffer, dst_addr,
                                                                dst_addr_mode, dst_ep,
                                                                ep, prof_id,
                                                                def_resp, cb,
                                                                hue,
                                                                saturation,
                                                                transition_time,
                                                                ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                                ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_hue_saturation_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR implementation */

static inline void zb_zcl_color_control_send_move_to_color_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                      zb_uint16_t color_x,
                                                      zb_uint16_t color_y,
                                                      zb_uint16_t transition_time,
                                                      zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_x));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_y));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_to_color_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                    zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                    zb_uint8_t ep, zb_uint16_t prof_id,
                                                                    zb_uint8_t def_resp, zb_callback_t cb,
                                                                    zb_uint16_t color_x,
                                                                    zb_uint16_t color_y,
                                                                    zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_x));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_y));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_to_color_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                      zb_uint16_t color_x,
                                                      zb_uint16_t color_y,
                                                      zb_uint16_t transition_time,
                                                      zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_color_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_color_req_rev1(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       color_x,
                                                       color_y,
                                                       transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_color_req_rev3(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       color_x,
                                                       color_y,
                                                       transition_time,
                                                       options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_color_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_to_color_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                 zb_uint16_t color_x,
                                                 zb_uint16_t color_y,
                                                 zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_color_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_color_req_rev1(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       color_x,
                                                       color_y,
                                                       transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_color_req_rev3(buffer, dst_addr,
                                                       dst_addr_mode, dst_ep,
                                                       ep, prof_id,
                                                       def_resp, cb,
                                                       color_x,
                                                       color_y,
                                                       transition_time,
                                                       ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                       ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_color_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR implementation */

static inline void zb_zcl_color_control_send_move_color_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint16_t rate_x,
                                                   zb_uint16_t rate_y,
                                                   zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate_x));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate_y));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_color_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                                 zb_uint16_t rate_x,
                                                                 zb_uint16_t rate_y)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate_x));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate_y));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_color_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint16_t rate_x,
                                                   zb_uint16_t rate_y,
                                                   zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_color_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_color_req_rev1(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    rate_x,
                                                    rate_y);
      break;
    case 3:
      zb_zcl_color_control_send_move_color_req_rev3(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    rate_x,
                                                    rate_y,
                                                    options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_color_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_color_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                              zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                              zb_uint8_t ep, zb_uint16_t prof_id,
                                              zb_uint8_t def_resp, zb_callback_t cb,
                                              zb_uint16_t rate_x,
                                              zb_uint16_t rate_y)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_color_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_color_req_rev1(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    rate_x,
                                                    rate_y);
      break;
    case 3:
      zb_zcl_color_control_send_move_color_req_rev3(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    rate_x,
                                                    rate_y,
                                                    ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                    ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_color_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR implementation */

static inline void zb_zcl_color_control_send_step_color_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint16_t step_x,
                                                   zb_uint16_t step_y,
                                                   zb_uint16_t transition_time,
                                                   zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_x));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_y));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_step_color_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                 zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                 zb_uint8_t ep, zb_uint16_t prof_id,
                                                                 zb_uint8_t def_resp, zb_callback_t cb,
                                                                 zb_uint16_t step_x,
                                                                 zb_uint16_t step_y,
                                                                 zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_x));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_y));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_step_color_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint16_t step_x,
                                                   zb_uint16_t step_y,
                                                   zb_uint16_t transition_time,
                                                   zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_color_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_color_req_rev1(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    step_x,
                                                    step_y,
                                                    transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_step_color_req_rev3(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    step_x,
                                                    step_y,
                                                    transition_time,
                                                    options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_color_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_step_color_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                              zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                              zb_uint8_t ep, zb_uint16_t prof_id,
                                              zb_uint8_t def_resp, zb_callback_t cb,
                                              zb_uint16_t step_x,
                                              zb_uint16_t step_y,
                                              zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_color_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_color_req_rev1(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    step_x,
                                                    step_y,
                                                    transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_step_color_req_rev3(buffer, dst_addr,
                                                    dst_addr_mode, dst_ep,
                                                    ep, prof_id,
                                                    def_resp, cb,
                                                    step_x,
                                                    step_y,
                                                    transition_time,
                                                    ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                    ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_color_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE implementation */

static inline void zb_zcl_color_control_send_move_to_color_temperature_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                  zb_uint8_t ep, zb_uint16_t prof_id,
                                                                  zb_uint8_t def_resp, zb_callback_t cb,
                                                                  zb_uint16_t color_temperature,
                                                                  zb_uint16_t transition_time,
                                                                  zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temperature));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_to_color_temperature_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                                zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                                zb_uint8_t ep, zb_uint16_t prof_id,
                                                                                zb_uint8_t def_resp, zb_callback_t cb,
                                                                                zb_uint16_t color_temperature,
                                                                                zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temperature));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_to_color_temperature_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                  zb_uint8_t ep, zb_uint16_t prof_id,
                                                                  zb_uint8_t def_resp, zb_callback_t cb,
                                                                  zb_uint16_t color_temperature,
                                                                  zb_uint16_t transition_time,
                                                                  zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_color_temperature_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_color_temperature_req_rev1(buffer, dst_addr,
                                                                   dst_addr_mode, dst_ep,
                                                                   ep, prof_id,
                                                                   def_resp, cb,
                                                                   color_temperature,
                                                                   transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_color_temperature_req_rev3(buffer, dst_addr,
                                                                   dst_addr_mode, dst_ep,
                                                                   ep, prof_id,
                                                                   def_resp, cb,
                                                                   color_temperature,
                                                                   transition_time,
                                                                   options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_color_temperature_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_to_color_temperature_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                                             zb_uint8_t def_resp, zb_callback_t cb,
                                                             zb_uint16_t color_temperature,
                                                             zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_to_color_temperature_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_to_color_temperature_req_rev1(buffer, dst_addr,
                                                                   dst_addr_mode, dst_ep,
                                                                   ep, prof_id,
                                                                   def_resp, cb,
                                                                   color_temperature,
                                                                   transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_move_to_color_temperature_req_rev3(buffer, dst_addr,
                                                                   dst_addr_mode, dst_ep,
                                                                   ep, prof_id,
                                                                   def_resp, cb,
                                                                   color_temperature,
                                                                   transition_time,
                                                                   ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                                   ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_to_color_temperature_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE implementation */

static inline void zb_zcl_color_control_send_enhanced_move_to_hue_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                                             zb_uint8_t def_resp, zb_callback_t cb,
                                                             zb_uint16_t enhanced_hue,
                                                             zb_uint8_t direction,
                                                             zb_uint16_t transition_time,
                                                             zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (enhanced_hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_enhanced_move_to_hue_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                           zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                           zb_uint8_t ep, zb_uint16_t prof_id,
                                                                           zb_uint8_t def_resp, zb_callback_t cb,
                                                                           zb_uint16_t enhanced_hue,
                                                                           zb_uint8_t direction,
                                                                           zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (enhanced_hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_enhanced_move_to_hue_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                             zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                             zb_uint8_t ep, zb_uint16_t prof_id,
                                                             zb_uint8_t def_resp, zb_callback_t cb,
                                                             zb_uint16_t enhanced_hue,
                                                             zb_uint8_t direction,
                                                             zb_uint16_t transition_time,
                                                             zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_move_to_hue_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_move_to_hue_req_rev1(buffer, dst_addr,
                                                              dst_addr_mode, dst_ep,
                                                              ep, prof_id,
                                                              def_resp, cb,
                                                              enhanced_hue,
                                                              direction,
                                                              transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_move_to_hue_req_rev3(buffer, dst_addr,
                                                              dst_addr_mode, dst_ep,
                                                              ep, prof_id,
                                                              def_resp, cb,
                                                              enhanced_hue,
                                                              direction,
                                                              transition_time,
                                                              options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_move_to_hue_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_enhanced_move_to_hue_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint16_t enhanced_hue,
                                                        zb_uint8_t direction,
                                                        zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_move_to_hue_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_move_to_hue_req_rev1(buffer, dst_addr,
                                                              dst_addr_mode, dst_ep,
                                                              ep, prof_id,
                                                              def_resp, cb,
                                                              enhanced_hue,
                                                              direction,
                                                              transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_move_to_hue_req_rev3(buffer, dst_addr,
                                                              dst_addr_mode, dst_ep,
                                                              ep, prof_id,
                                                              def_resp, cb,
                                                              enhanced_hue,
                                                              direction,
                                                              transition_time,
                                                              ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                              ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_move_to_hue_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE implementation */

static inline void zb_zcl_color_control_send_enhanced_move_hue_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                          zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                          zb_uint8_t ep, zb_uint16_t prof_id,
                                                          zb_uint8_t def_resp, zb_callback_t cb,
                                                          zb_uint8_t move_mode,
                                                          zb_uint16_t rate,
                                                          zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_enhanced_move_hue_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                                        zb_uint8_t move_mode,
                                                                        zb_uint16_t rate)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_enhanced_move_hue_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                          zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                          zb_uint8_t ep, zb_uint16_t prof_id,
                                                          zb_uint8_t def_resp, zb_callback_t cb,
                                                          zb_uint8_t move_mode,
                                                          zb_uint16_t rate,
                                                          zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_move_hue_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_move_hue_req_rev1(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           move_mode,
                                                           rate);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_move_hue_req_rev3(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           move_mode,
                                                           rate,
                                                           options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_move_hue_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_enhanced_move_hue_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                     zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                     zb_uint8_t ep, zb_uint16_t prof_id,
                                                     zb_uint8_t def_resp, zb_callback_t cb,
                                                     zb_uint8_t move_mode,
                                                     zb_uint16_t rate)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_move_hue_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_move_hue_req_rev1(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           move_mode,
                                                           rate);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_move_hue_req_rev3(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           move_mode,
                                                           rate,
                                                           ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                           ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_move_hue_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE implementation */

static inline void zb_zcl_color_control_send_enhanced_step_hue_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                          zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                          zb_uint8_t ep, zb_uint16_t prof_id,
                                                          zb_uint8_t def_resp, zb_callback_t cb,
                                                          zb_uint8_t step_mode,
                                                          zb_uint16_t step_size,
                                                          zb_uint16_t transition_time,
                                                          zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_enhanced_step_hue_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                                        zb_uint8_t step_mode,
                                                                        zb_uint16_t step_size,
                                                                        zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_STEP_HUE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_enhanced_step_hue_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                          zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                          zb_uint8_t ep, zb_uint16_t prof_id,
                                                          zb_uint8_t def_resp, zb_callback_t cb,
                                                          zb_uint8_t step_mode,
                                                          zb_uint16_t step_size,
                                                          zb_uint16_t transition_time,
                                                          zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_step_hue_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_step_hue_req_rev1(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           step_mode,
                                                           step_size,
                                                           transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_step_hue_req_rev3(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           step_mode,
                                                           step_size,
                                                           transition_time,
                                                           options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_step_hue_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_enhanced_step_hue_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                     zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                     zb_uint8_t ep, zb_uint16_t prof_id,
                                                     zb_uint8_t def_resp, zb_callback_t cb,
                                                     zb_uint8_t step_mode,
                                                     zb_uint16_t step_size,
                                                     zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_step_hue_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_step_hue_req_rev1(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           step_mode,
                                                           step_size,
                                                           transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_step_hue_req_rev3(buffer, dst_addr,
                                                           dst_addr_mode, dst_ep,
                                                           ep, prof_id,
                                                           def_resp, cb,
                                                           step_mode,
                                                           step_size,
                                                           transition_time,
                                                           ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                           ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_step_hue_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION implementation */

static inline void zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                                        zb_uint16_t enhanced_hue,
                                                                        zb_uint8_t saturation,
                                                                        zb_uint16_t transition_time,
                                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (enhanced_hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                                                      zb_uint16_t enhanced_hue,
                                                                                      zb_uint8_t saturation,
                                                                                      zb_uint16_t transition_time)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_SATURATION);
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (enhanced_hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                                        zb_uint16_t enhanced_hue,
                                                                        zb_uint8_t saturation,
                                                                        zb_uint16_t transition_time,
                                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_rev1(buffer, dst_addr,
                                                                         dst_addr_mode, dst_ep,
                                                                         ep, prof_id,
                                                                         def_resp, cb,
                                                                         enhanced_hue,
                                                                         saturation,
                                                                         transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_rev3(buffer, dst_addr,
                                                                         dst_addr_mode, dst_ep,
                                                                         ep, prof_id,
                                                                         def_resp, cb,
                                                                         enhanced_hue,
                                                                         saturation,
                                                                         transition_time,
                                                                         options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                                   zb_uint16_t enhanced_hue,
                                                                   zb_uint8_t saturation,
                                                                   zb_uint16_t transition_time)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_rev1(buffer, dst_addr,
                                                                         dst_addr_mode, dst_ep,
                                                                         ep, prof_id,
                                                                         def_resp, cb,
                                                                         enhanced_hue,
                                                                         saturation,
                                                                         transition_time);
      break;
    case 3:
      zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req_rev3(buffer, dst_addr,
                                                                         dst_addr_mode, dst_ep,
                                                                         ep, prof_id,
                                                                         def_resp, cb,
                                                                         enhanced_hue,
                                                                         saturation,
                                                                         transition_time,
                                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_enhanced_move_to_hue_saturation_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET implementation */

static inline void zb_zcl_color_control_send_color_loop_set_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                       zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                       zb_uint8_t ep, zb_uint16_t prof_id,
                                                       zb_uint8_t def_resp, zb_callback_t cb,
                                                       zb_uint8_t update_flags,
                                                       zb_uint8_t action,
                                                       zb_uint8_t direction,
                                                       zb_uint16_t time,
                                                       zb_uint16_t start_hue,
                                                       zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (update_flags));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (action));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (time));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (start_hue));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_color_loop_set_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                     zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                     zb_uint8_t ep, zb_uint16_t prof_id,
                                                                     zb_uint8_t def_resp, zb_callback_t cb,
                                                                     zb_uint8_t update_flags,
                                                                     zb_uint8_t action,
                                                                     zb_uint8_t direction,
                                                                     zb_uint16_t time,
                                                                     zb_uint16_t start_hue)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_COLOR_LOOP_SET);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (update_flags));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (action));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (direction));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (time));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (start_hue));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_color_loop_set_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                       zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                       zb_uint8_t ep, zb_uint16_t prof_id,
                                                       zb_uint8_t def_resp, zb_callback_t cb,
                                                       zb_uint8_t update_flags,
                                                       zb_uint8_t action,
                                                       zb_uint8_t direction,
                                                       zb_uint16_t time,
                                                       zb_uint16_t start_hue,
                                                       zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_color_loop_set_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_color_loop_set_req_rev1(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb,
                                                        update_flags,
                                                        action,
                                                        direction,
                                                        time,
                                                        start_hue);
      break;
    case 3:
      zb_zcl_color_control_send_color_loop_set_req_rev3(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb,
                                                        update_flags,
                                                        action,
                                                        direction,
                                                        time,
                                                        start_hue,
                                                        options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_color_loop_set_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_color_loop_set_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                  zb_uint8_t ep, zb_uint16_t prof_id,
                                                  zb_uint8_t def_resp, zb_callback_t cb,
                                                  zb_uint8_t update_flags,
                                                  zb_uint8_t action,
                                                  zb_uint8_t direction,
                                                  zb_uint16_t time,
                                                  zb_uint16_t start_hue)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_color_loop_set_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_color_loop_set_req_rev1(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb,
                                                        update_flags,
                                                        action,
                                                        direction,
                                                        time,
                                                        start_hue);
      break;
    case 3:
      zb_zcl_color_control_send_color_loop_set_req_rev3(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb,
                                                        update_flags,
                                                        action,
                                                        direction,
                                                        time,
                                                        start_hue,
                                                        ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                        ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_color_loop_set_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP implementation */

static inline void zb_zcl_color_control_send_stop_move_step_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                       zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                       zb_uint8_t ep, zb_uint16_t prof_id,
                                                       zb_uint8_t def_resp, zb_callback_t cb,
                                                       zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_stop_move_step_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                     zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                     zb_uint8_t ep, zb_uint16_t prof_id,
                                                                     zb_uint8_t def_resp, zb_callback_t cb)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STOP_MOVE_STEP);
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_stop_move_step_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                       zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                       zb_uint8_t ep, zb_uint16_t prof_id,
                                                       zb_uint8_t def_resp, zb_callback_t cb,
                                                       zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_stop_move_step_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_stop_move_step_req_rev1(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb);
      break;
    case 3:
      zb_zcl_color_control_send_stop_move_step_req_rev3(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb,
                                                        options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_stop_move_step_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_stop_move_step_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                  zb_uint8_t ep, zb_uint16_t prof_id,
                                                  zb_uint8_t def_resp, zb_callback_t cb)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_stop_move_step_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_stop_move_step_req_rev1(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb);
      break;
    case 3:
      zb_zcl_color_control_send_stop_move_step_req_rev3(buffer, dst_addr,
                                                        dst_addr_mode, dst_ep,
                                                        ep, prof_id,
                                                        def_resp, cb,
                                                        ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                        ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_stop_move_step_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE implementation */

static inline void zb_zcl_color_control_send_move_color_temp_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                       zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                       zb_uint8_t ep, zb_uint16_t prof_id,
                                                       zb_uint8_t def_resp, zb_callback_t cb,
                                                       zb_uint8_t move_mode,
                                                       zb_uint16_t rate,
                                                       zb_uint16_t color_temp_min,
                                                       zb_uint16_t color_temp_max,
                                                       zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_min));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_max));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_move_color_temp_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                                      zb_uint8_t move_mode,
                                                                      zb_uint16_t rate,
                                                                      zb_uint16_t color_temp_min,
                                                                      zb_uint16_t color_temp_max)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (move_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (rate));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_min));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_max));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_move_color_temp_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                       zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                       zb_uint8_t ep, zb_uint16_t prof_id,
                                                       zb_uint8_t def_resp, zb_callback_t cb,
                                                       zb_uint8_t move_mode,
                                                       zb_uint16_t rate,
                                                       zb_uint16_t color_temp_min,
                                                       zb_uint16_t color_temp_max,
                                                       zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_color_temp_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_color_temp_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate,
                                                         color_temp_min,
                                                         color_temp_max);
      break;
    case 3:
      zb_zcl_color_control_send_move_color_temp_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate,
                                                         color_temp_min,
                                                         color_temp_max,
                                                         options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_color_temp_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_move_color_temp_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                  zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                  zb_uint8_t ep, zb_uint16_t prof_id,
                                                  zb_uint8_t def_resp, zb_callback_t cb,
                                                  zb_uint8_t move_mode,
                                                  zb_uint16_t rate,
                                                  zb_uint16_t color_temp_min,
                                                  zb_uint16_t color_temp_max)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_move_color_temp_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_move_color_temp_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate,
                                                         color_temp_min,
                                                         color_temp_max);
      break;
    case 3:
      zb_zcl_color_control_send_move_color_temp_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         move_mode,
                                                         rate,
                                                         color_temp_min,
                                                         color_temp_max,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_move_color_temp_req", (FMT__0));
}

/* ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE implementation */

static inline void zb_zcl_color_control_send_step_color_temp_req_rev3(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint8_t step_mode,
                                                        zb_uint16_t step_size,
                                                        zb_uint16_t transition_time,
                                                        zb_uint16_t color_temp_min,
                                                        zb_uint16_t color_temp_max,
                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_min));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_max));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_mask));
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (options_override));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

static inline void zb_zcl_color_control_send_step_color_temp_req_rev1(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                                      zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                                      zb_uint8_t ep, zb_uint16_t prof_id,
                                                                      zb_uint8_t def_resp, zb_callback_t cb,
                                                                      zb_uint8_t step_mode,
                                                                      zb_uint16_t step_size,
                                                                      zb_uint16_t transition_time,
                                                                      zb_uint16_t color_temp_min,
                                                                      zb_uint16_t color_temp_max)
{
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_STEP_COLOR_TEMPERATURE);
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (step_size));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_min));
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (color_temp_max));
  zb_zcl_finish_and_send_packet(buffer, ptr, dst_addr, dst_addr_mode, dst_ep, ep, prof_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb);
}

void zb_zcl_color_control_send_step_color_temp_req_zcl8(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                        zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                        zb_uint8_t ep, zb_uint16_t prof_id,
                                                        zb_uint8_t def_resp, zb_callback_t cb,
                                                        zb_uint8_t step_mode,
                                                        zb_uint16_t step_size,
                                                        zb_uint16_t transition_time,
                                                        zb_uint16_t color_temp_min,
                                                        zb_uint16_t color_temp_max,
                                                        zb_uint8_t options_mask, zb_uint8_t options_override)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_color_temp_req_zcl8", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_COLOR_CONTROL_CLUSTER_REVISION_MAX,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_color_temp_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode,
                                                         step_size,
                                                         transition_time,
                                                         color_temp_min,
                                                         color_temp_max);
      break;
    case 3:
      zb_zcl_color_control_send_step_color_temp_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode,
                                                         step_size,
                                                         transition_time,
                                                         color_temp_min,
                                                         color_temp_max,
                                                         options_mask, options_override);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_color_temp_req_zcl8", (FMT__0));
}

void zb_zcl_color_control_send_step_color_temp_req(zb_bufid_t buffer, const zb_addr_u *dst_addr,
                                                   zb_uint8_t dst_addr_mode, zb_uint8_t dst_ep,
                                                   zb_uint8_t ep, zb_uint16_t prof_id,
                                                   zb_uint8_t def_resp, zb_callback_t cb,
                                                   zb_uint8_t step_mode,
                                                   zb_uint16_t step_size,
                                                   zb_uint16_t transition_time,
                                                   zb_uint16_t color_temp_min,
                                                   zb_uint16_t color_temp_max)
{
  zb_uint16_t rev;

  TRACE_MSG(TRACE_ZCL3, "> zb_zcl_color_control_send_step_color_temp_req", (FMT__0));

  rev = zb_zcl_get_cluster_rev_by_mode(ZB_ZCL_CLUSTER_REV_MIN,
                                       dst_addr, dst_addr_mode, dst_ep,
                                       ZB_ZCL_CLUSTER_ID_COLOR_CONTROL,
                                       ZB_ZCL_CLUSTER_CLIENT_ROLE, ep);

  TRACE_MSG(TRACE_ZCL3, "rev is %d", (FMT__D, rev));

  switch(rev)
  {
    case ZB_ZCL_CLUSTER_REV_MIN:
      /* FALLTHROUGH */
    case 2:
      zb_zcl_color_control_send_step_color_temp_req_rev1(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode,
                                                         step_size,
                                                         transition_time,
                                                         color_temp_min,
                                                         color_temp_max);
      break;
    case 3:
      zb_zcl_color_control_send_step_color_temp_req_rev3(buffer, dst_addr,
                                                         dst_addr_mode, dst_ep,
                                                         ep, prof_id,
                                                         def_resp, cb,
                                                         step_mode,
                                                         step_size,
                                                         transition_time,
                                                         color_temp_min,
                                                         color_temp_max,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_MASK_DEFAULT_FIELD_VALUE,
                                                         ZB_ZCL_COLOR_CONTROL_OPTIONS_OVERRIDE_DEFAULT_FIELD_VALUE);
      break;
    default:
      break;
  }

  TRACE_MSG(TRACE_ZCL3, "< zb_zcl_color_control_send_step_color_temp_req", (FMT__0));
}

/**
 *  @} internal
*/
#endif /* defined ZB_ZCL_SUPPORT_CLUSTER_COLOR_CONTROL */
