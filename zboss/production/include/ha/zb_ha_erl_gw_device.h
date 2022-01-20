/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2021 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: ERL GW device definition
*/

#ifndef ERL_GW_DEVICE_H
#define ERL_GW_DEVICE_H 1

#if defined ZB_HA_DEFINE_DEVICE_ERL_GW || defined DOXYGEN

#include "zcl/zb_zcl_time.h"

#define ZB_HA_DEVICE_VER_ERL_GW 0  /*!< ERL GW device version */
#define ZB_HA_ERL_GW_DEVICE_ID 0
/* ERL GW clock is the Master clock: it must not be written by ZB commands */
#define ERL_GW_TIME_STATUS_INITIAL ((1 << ZB_ZCL_TIME_MASTER) | (1 << ZB_ZCL_TIME_MASTER_ZONE_DST))

#define ZB_HA_ERL_GW_IN_CLUSTER_NUM 3  /*!< @internal ERL GW IN clusters number */
#define ZB_HA_ERL_GW_OUT_CLUSTER_NUM 7 /*!< @internal ERL GW OUT clusters number */

#define ZB_HA_ERL_GW_CLUSTER_NUM                                      \
  (ZB_HA_ERL_GW_IN_CLUSTER_NUM + ZB_HA_ERL_GW_OUT_CLUSTER_NUM)

/*! @internal Number of attribute for reporting on ERL GW device */
#define ZB_HA_ERL_GW_REPORT_ATTR_COUNT 0

/** @brief Declare cluster list for ERL GW device
    @param cluster_list_name - cluster list variable name
    @param on_off_switch_config_attr_list - attribute list for On/off switch configuration cluster
    @param basic_attr_list - attribute list for Basic cluster
    @param identify_attr_list - attribute list for Identify cluster
 */
#define ZB_HA_DECLARE_ERL_GW_CLUSTER_LIST(                                  \
      cluster_list_name,                                                    \
      basic_attr_list,                                                      \
      identify_attr_list,                                                   \
      time_attr_list)                                                       \
      zb_zcl_cluster_desc_t cluster_list_name[] =                           \
      {                                                                     \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_BASIC,                                          \
          ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                \
          (basic_attr_list),                                                \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                       \
          ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),             \
          (identify_attr_list),                                             \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_TIME,                                           \
          ZB_ZCL_ARRAY_SIZE(time_attr_list, zb_zcl_attr_t),                 \
          (time_attr_list),                                                 \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                                       \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION,                           \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                         \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_DIAGNOSTICS,                                    \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_METERING,                                       \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_MESSAGING,                                      \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
        ZB_ZCL_CLUSTER_DESC(                                                \
          ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,                                 \
          0,                                                                \
          NULL,                                                             \
          ZB_ZCL_CLUSTER_CLIENT_ROLE,                                       \
          ZB_ZCL_MANUF_CODE_INVALID                                         \
        ),                                                                  \
    }

/** @internal @brief Declare simple descriptor for ERL GW device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param in_clust_num - number of supported input clusters
    @param out_clust_num - number of supported output clusters
    @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
    definitions, because these values are used to form simple descriptor type name
*/
#define ZB_ZCL_DECLARE_ERL_GW_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                        \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =                 \
  {                                                                                           \
    ep_id,                                                                                    \
    ZB_AF_HA_PROFILE_ID,                                                                      \
    ZB_HA_ERL_GW_DEVICE_ID,                                                            \
    ZB_HA_DEVICE_VER_ERL_GW,                                                           \
    0,                                                                                        \
    in_clust_num,                                                                             \
    out_clust_num,                                                                            \
    {                                                                                         \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                                \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                             \
      ZB_ZCL_CLUSTER_ID_TIME,                                                                 \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                             \
      ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION,                                                 \
      ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                                               \
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS,                                                          \
      ZB_ZCL_CLUSTER_ID_METERING,                                                             \
      ZB_ZCL_CLUSTER_ID_MESSAGING,                                                            \
      ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,                                                       \
    }                                                                                         \
  }

/** @brief Declare endpoint for ERL GW device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_ERL_GW_EP(ep_name, ep_id, cluster_list) \
  ZB_ZCL_DECLARE_ERL_GW_SIMPLE_DESC(                          \
      ep_name,                                                       \
      ep_id,                                                         \
      ZB_HA_ERL_GW_IN_CLUSTER_NUM,                            \
      ZB_HA_ERL_GW_OUT_CLUSTER_NUM);                          \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name,                                                \
                              ep_id,                                                  \
      ZB_AF_HA_PROFILE_ID,                                           \
      0,                                                             \
      NULL,                                                          \
      ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t),        \
      cluster_list,                                                  \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,       \
                              0, NULL, /* No reporting ctx */                         \
                              0, NULL)

/** @brief Declare ERL GW device context.
    @param device_ctx - device context variable name.
    @param ep_name - endpoint variable name.
*/
#define ZB_HA_DECLARE_ERL_GW_CTX(device_ctx, ep_name)                \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

#endif /* defined ZB_HA_DEFINE_DEVICE_ERL_GW || defined DOXYGEN */

#endif /* ERL_GW_DEVICE_H */
