/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_ZCL_COLOR_CONTROL_ADDONS_H__
#define ZB_ZCL_COLOR_CONTROL_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_color_control_addons */
/*! @{ */

/**@brief Color information set for Color cluster attributes according to ZCL Spec 5.2.2.2.1. */
typedef struct
{
    zb_uint8_t     current_hue;
    zb_uint8_t     current_saturation;
    zb_uint8_t     color_mode;
    zb_uint8_t     options;
    zb_uint8_t     enhanced_color_mode;
    zb_uint16_t    color_capabilities;
} zb_zcl_color_ctrl_attrs_set_color_inf_t;

/**@brief Color information extended set for Color cluster attributes according to ZCL Spec 5.2.2.2.1. */
typedef struct
{
    zb_uint8_t     current_hue;
    zb_uint8_t     current_saturation;
    zb_uint16_t    remaining_time;
    zb_uint16_t    current_X;
    zb_uint16_t    current_Y;
    zb_uint16_t    color_temperature;
    zb_uint8_t     color_mode;
    zb_uint8_t     options;
    zb_uint16_t    enhanced_current_hue;
    zb_uint8_t     enhanced_color_mode;
    zb_uint8_t     color_loop_active;
    zb_uint8_t     color_loop_direction;
    zb_uint16_t    color_loop_time;
    zb_uint16_t    color_loop_start_enhanced_hue;
    zb_uint16_t    color_loop_stored_enhanced_hue;
    zb_uint16_t    color_capabilities;
    zb_uint16_t    color_temp_physical_min_mireds;
    zb_uint16_t    color_temp_physical_max_mireds;
    zb_uint16_t    couple_color_temp_to_level_min_mireds;
    zb_uint16_t    start_up_color_temp_mireds;
} zb_zcl_color_ctrl_attrs_set_color_inf_ext_t;

/**@brief Defined primaries information attribute set for Color cluster attributes according to ZCL Spec 5.2.2.2.2. */
typedef struct
{
    zb_uint8_t      number_primaries;
} zb_zcl_color_ctrl_attrs_set_defined_primaries_inf_t;

/**@brief Defined primaries information extended attribute set for Color cluster attributes according to ZCL Spec 5.2.2.2.2. */
typedef struct
{
    zb_uint8_t      number_primaries;
    zb_uint16_t     primary_1_X;
    zb_uint16_t     primary_1_Y;
    zb_uint8_t      primary_1_intensity;
    zb_uint16_t     primary_2_X;
    zb_uint16_t     primary_2_Y;
    zb_uint8_t      primary_2_intensity;
    zb_uint16_t     primary_3_X;
    zb_uint16_t     primary_3_Y;
    zb_uint8_t      primary_3_intensity;
} zb_zcl_color_ctrl_attrs_set_defined_primaries_inf_ext_t;

/**@brief Color cluster extended attributes according to ZCL Spec 5.2.2.2.3. */
typedef struct
{
    zb_uint16_t     primary_4_X;
    zb_uint16_t     primary_4_Y;
    zb_uint8_t      primary_4_intensity;
    zb_uint16_t     primary_5_X;
    zb_uint16_t     primary_5_Y;
    zb_uint8_t      primary_5_intensity;
    zb_uint16_t     primary_6_X;
    zb_uint16_t     primary_6_Y;
    zb_uint8_t      primary_6_intensity;
} zb_zcl_color_ctrl_attrs_set_add_prim_inf_ext_t;

/**@brief Color cluster attributes according to ZCL Spec 5.2.2.2. */
typedef struct
{
    zb_zcl_color_ctrl_attrs_set_color_inf_t             set_color_info;
    zb_zcl_color_ctrl_attrs_set_defined_primaries_inf_t set_defined_primaries_info;
} zb_zcl_color_control_attrs_t;

/**@brief Color cluster extended attributes according to ZCL Spec 5.2.2.2. */
typedef struct
{
    zb_zcl_color_ctrl_attrs_set_color_inf_ext_t             set_color_info;
    zb_zcl_color_ctrl_attrs_set_defined_primaries_inf_ext_t set_defined_primaries_info;
    zb_zcl_color_ctrl_attrs_set_add_prim_inf_ext_t          set_additional_defined_primaries_info;
} zb_zcl_color_control_attrs_ext_t;

/**@brief Macro equivalent to @ref ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_SATURATION_REQ, but command is sent without APS ACK. */
#define ZB_ZCL_COLOR_CONTROL_SEND_MOVE_TO_HUE_SATURATION_REQ_NO_APS_ACK(buffer, addr, dst_addr_mode,\
  dst_ep, ep, prfl_id, def_resp, cb, hue, saturation, transition_time)                              \
{                                                                                                   \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                                                 \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, (def_resp))                              \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), ZB_ZCL_CMD_COLOR_CONTROL_MOVE_TO_HUE_SATURATION);    \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (hue));                                                              \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (saturation));                                                       \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                                             \
  ZB_ZCL_SEND_COMMAND_SHORT_WITHOUT_ACK((buffer), ptr, addr, dst_addr_mode,                         \
                                dst_ep, ep, prfl_id, ZB_ZCL_CLUSTER_ID_COLOR_CONTROL, cb, 0);       \
}

/** @} */

#endif /* ZB_ZCL_COLOR_CONTROL_ADDONS_H__ */
