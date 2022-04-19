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

/**
 *  @brief Declare attribute list for Color control cluster (client).
 *  @param attr_list - attribute list name.
 */
#define ZB_ZCL_DECLARE_COLOR_CONTROL_CLIENT_ATTRIB_LIST(attr_list)                   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_COLOR_CONTROL) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/*!
  @brief Declare attribute list for Color Control cluster (extended with hue and saturation attributes)
  @param attr_list - attribute list name
  @param current_hue - pointer to variable to store current_hue attribute value
  @param current_saturation - pointer to variable to store current_saturation attribute value
  @param color_mode - pointer to variable to store color_mode attribute value
  @param options - pointer to variable to store options attribute value
  @param number_primaries - pointer to variable to store number_primaries attribute value
  @param enhanced_color_mode - pointer to variable to store Enhanced Color Mode attribute value
  @param color_capabilities - pointer to variable to store Color Capabilities attribute value
*/
#define ZB_ZCL_DECLARE_COLOR_CONTROL_ATTRIB_LIST_HS(attr_list, current_hue, current_saturation,    \
    color_mode, options, number_primaries, enhanced_color_mode, color_capabilities)                \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_COLOR_CONTROL)               \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_HUE_ID, (current_hue))                    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_CURRENT_SATURATION_ID, (current_saturation))      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_MODE_ID, (color_mode))                      \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_OPTIONS_ID, (options))                            \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_NUMBER_OF_PRIMARIES_ID, (number_primaries))       \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_ENHANCED_COLOR_MODE_ID, (enhanced_color_mode))    \
  ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_COLOR_CONTROL_COLOR_CAPABILITIES_ID, (color_capabilities))      \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @} */

#endif /* ZB_ZCL_COLOR_CONTROL_ADDONS_H__ */
