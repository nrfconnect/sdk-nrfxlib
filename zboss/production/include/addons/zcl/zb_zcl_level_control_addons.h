/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZB_ZCL_LEVEL_CONTROL_ADDONS_H__
#define ZB_ZCL_LEVEL_CONTROL_ADDONS_H__

#include "zboss_api.h"

/*! \addtogroup zb_zcl_level_control_addons */
/*! @{ */

/**@brief Level Control cluster attributes according to ZCL Spec 3.10.2.3. */
typedef struct
{
    zb_uint8_t  current_level;
    zb_uint16_t remaining_time;
} zb_zcl_level_control_attrs_t;

/**@brief Macro equivalent to @ref ZB_ZCL_LEVEL_CONTROL_SEND_STEP_CMD, but command is sent without APS ACK. */
#define ZB_ZCL_LEVEL_CONTROL_SEND_STEP_CMD_NO_APS_ACK(buffer,           \
                                           addr,                        \
                                           dst_addr_mode,               \
                                           dst_ep,                      \
                                           ep,                          \
                                           prfl_id,                     \
                                           def_resp,                    \
                                           cb,                          \
                                           step_mode,                   \
                                           step_size,                   \
                                           transition_time,             \
                                           cmd_id)                      \
{                                                                       \
  zb_uint8_t* ptr = ZB_ZCL_START_PACKET_REQ(buffer)                     \
  ZB_ZCL_CONSTRUCT_SPECIFIC_COMMAND_REQ_FRAME_CONTROL(ptr, def_resp)    \
  ZB_ZCL_CONSTRUCT_COMMAND_HEADER_REQ(ptr, ZB_ZCL_GET_SEQ_NUM(), cmd_id);   \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_mode));                            \
  ZB_ZCL_PACKET_PUT_DATA8(ptr, (step_size));                            \
  ZB_ZCL_PACKET_PUT_DATA16_VAL(ptr, (transition_time));                 \
  ZB_ZCL_SEND_COMMAND_SHORT_WITHOUT_ACK((buffer), ptr,                  \
                            addr, dst_addr_mode,                        \
                            dst_ep, ep, prfl_id,                        \
                            ZB_ZCL_CLUSTER_ID_LEVEL_CONTROL, cb, 0);    \
}

/**@brief Macro equivalent to @ref ZB_ZCL_LEVEL_CONTROL_SEND_STEP_REQ, but command is sent without APS ACK. */
#define ZB_ZCL_LEVEL_CONTROL_SEND_STEP_REQ_NO_APS_ACK(buffer,        \
                                           addr,                     \
                                           dst_addr_mode,            \
                                           dst_ep,                   \
                                           ep,                       \
                                           prfl_id,                  \
                                           def_resp,                 \
                                           cb,                       \
                                           step_mode,                \
                                           step_size,                \
                                           transition_time)          \
{                                                                    \
  ZB_ZCL_LEVEL_CONTROL_SEND_STEP_CMD_NO_APS_ACK(buffer,              \
                                     addr,                           \
                                     dst_addr_mode,                  \
                                     dst_ep,                         \
                                     ep,                             \
                                     prfl_id,                        \
                                     def_resp,                       \
                                     cb,                             \
                                     step_mode,                      \
                                     step_size,                      \
                                     transition_time,                \
                                     ZB_ZCL_CMD_LEVEL_CONTROL_STEP); \
}

/**
 *  @brief Declare attribute list for Level control cluster (client).
 *  @param attr_list - attribute list name.
 */
#define ZB_ZCL_DECLARE_LEVEL_CONTROL_CLIENT_ATTRIB_LIST(attr_list)                   \
  ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_LEVEL_CONTROL) \
  ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/** @} */

#endif /* ZB_ZCL_LEVEL_CONTROL_ADDONS_H__ */
