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
/* PURPOSE: ERL Interface Device definition
*/

#ifndef ZB_ERL_INTERFACE_DEVICE_H
#define ZB_ERL_INTERFACE_DEVICE_H 1

#if defined ZB_HA_DEFINE_DEVICE_ERL_INTERFACE_DEVICE || defined DOXYGEN

#include "zb_ha_erl_device_attrs.h"

/******************* Specific definitions for ERL Interface device **************************/

/** @cond DOXYGEN_ERL_SECTION */

/**
 *  @defgroup ZB_HA_DEFINE_DEVICE_ERL_INTERFACE_DEVICE ERL Interface
 *  @ingroup ZB_ERL_SAMPLES

    @details
    ERL Interface device has 9 server and 2 client clusters: \n
      - Server clusters: \n
        - @ref ZB_ZCL_BASIC \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_TIME \n
        - @ref ZB_ZCL_METER_IDENTIFICATION \n
        - @ref ZB_ZCL_ELECTRICAL_MEASUREMENT \n
        - @ref ZB_ZCL_DIAGNOSTICS \n
        - @ref ZB_ZCL_METERING \n
        - @ref ZB_ZCL_MESSAGING \n
        - @ref ZB_ZCL_DAILY_SCHEDULE \n
      - Client clusters: \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_TIME \n


    @par Example
      @n Declare clusters
         @snippet erl_interface_zc.c DECLARE_CLUSTERS
      @n Register device context
         @snippet erl_interface_zc.c REGISTER_DEVICE_CTX
      @n Signal handler exapmle
         @snippet erl_interface_zc.c SIGNAL_HANDLER
*/

/** @def ZB_ERL_INTERFACE_DEVICE_ID
 *  @brief ERL interface device ID
 */
#define ZB_ERL_INTERFACE_DEVICE_ID 0x0509

/** @def ZB_DEVICE_VER_ERL_INTERFACE
 *  @brief ERL interface device version
 */
#define ZB_DEVICE_VER_ERL_INTERFACE 1

/*! @cond internals_doc */

/** @def ZB_ERL_INTERFACE_DEV_IN_CLUSTER_NUM
 *  @brief ERL IN clusters number
 */
#define ZB_ERL_INTERFACE_DEV_IN_CLUSTER_NUM  9

/** @def ZB_ERL_INTERFACE_DEV_OUT_CLUSTER_NUM
 *  @brief ERL OUT clusters number
 */
#define ZB_ERL_INTERFACE_DEV_OUT_CLUSTER_NUM 2

/** @def ZB_ERL_INTERFACE_DEV_CLUSTER_NUM
 *  @brief Number of clusters for ERL device
 */
#define ZB_ERL_INTERFACE_DEV_CLUSTER_NUM \
  (ZB_ERL_INTERFACE_DEV_IN_CLUSTER_NUM + ZB_ERL_INTERFACE_DEV_OUT_CLUSTER_NUM)

/** @def ZB_ERL_INTERFACE_DEV_REPORT_ATTR_COUNT
 *  @brief Number of attributes for reporting on ERL device
 */
#define ZB_ERL_INTERFACE_DEV_REPORT_ATTR_COUNT     \
  (ZB_ERL_EL_MEASUREMENT_REPORT_ATTR_COUNT +       \
   ZB_ERL_METER_IDENTIFICATION_REPORT_ATTR_COUNT + \
   ZB_ERL_METERING_REPORT_ATTR_COUNT +             \
   ZB_ERL_TIME_REPORT_ATTR_COUNT +                 \
   ZB_ERL_DAILY_SCHEDULE_REPORT_ATTR_COUNT)
/*! @endcond internals_doc */

/** @endcond */ /* DOXYGEN_ERL_SECTION */

/** @def ZB_DECLARE_ERL_INTERFACE_DEV_CLUSTER_LIST
 *  @brief Declare cluster list for ERL device.
 *  @param cluster_list_name - cluster list variable name
 *  @param basic_attr_list - attribute list for Basic cluster
 *  @param time_attr_list - attribute list for Time cluster
 *  @param meter_identification_attr_list
 *  @param el_measurement_attr_list
 *  @param diagnostics_attr_list
 *  @param metering_attr_list
 *  @param daily_schedule_attr_list
 */
#define ZB_DECLARE_ERL_INTERFACE_DEV_CLUSTER_LIST(cluster_list_name,              \
                                                  basic_attr_list,                \
                                                  identify_attr_list,             \
                                                  time_attr_list,                 \
                                                  meter_identification_attr_list, \
                                                  el_measurement_attr_list,       \
                                                  diagnostics_attr_list,          \
                                                  metering_attr_list,             \
                                                  daily_schedule_attr_list)       \
  zb_zcl_cluster_desc_t cluster_list_name[] =                                     \
  {                                                                               \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                    \
      ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),                          \
      (basic_attr_list),                                                          \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                 \
      ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),                       \
      (identify_attr_list),                                                       \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_TIME,                                                     \
      ZB_ZCL_ARRAY_SIZE(time_attr_list, zb_zcl_attr_t),                           \
      (time_attr_list),                                                           \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION,                                     \
      ZB_ZCL_ARRAY_SIZE(meter_identification_attr_list, zb_zcl_attr_t),           \
      (meter_identification_attr_list),                                           \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                                   \
      ZB_ZCL_ARRAY_SIZE(el_measurement_attr_list, zb_zcl_attr_t),                 \
      (el_measurement_attr_list),                                                 \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS,                                              \
      ZB_ZCL_ARRAY_SIZE(diagnostics_attr_list, zb_zcl_attr_t),                    \
      (diagnostics_attr_list),                                                    \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_METERING,                                                 \
      ZB_ZCL_ARRAY_SIZE(metering_attr_list, zb_zcl_attr_t),                       \
      (metering_attr_list),                                                       \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_MESSAGING,                                                \
      0,                                                                          \
      NULL,                                                                       \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,                                           \
      ZB_ZCL_ARRAY_SIZE(daily_schedule_attr_list, zb_zcl_attr_t),                 \
      (daily_schedule_attr_list),                                                 \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                 \
      0,                                                                          \
      NULL,                                                                       \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
    ZB_ZCL_CLUSTER_DESC(                                                          \
      ZB_ZCL_CLUSTER_ID_TIME,                                                     \
      0,                                                                          \
      NULL,                                                                       \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                                   \
      ),                                                                          \
  }

/** @cond internals_doc */

/** @def ZB_ZCL_DECLARE_ERL_INTERFACE_DEV_SIMPLE_DESC
 *  @brief Declare simple descriptor for ERL device
 *  @param ep_name - endpoint variable name
 *  @param ep_id - endpoint ID
 *  @param in_clust_num   - number of supported input clusters
 *  @param out_clust_num  - number of supported output clusters
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name
 */
#define ZB_ZCL_DECLARE_ERL_INTERFACE_DEV_SIMPLE_DESC(ep_name,                  \
                                                     ep_id,                    \
                                                     in_clust_num,             \
                                                     out_clust_num)            \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                         \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)  simple_desc_##ep_name = \
  {                                                                            \
    ep_id,                                                                     \
    ZB_AF_HA_PROFILE_ID,                                                       \
    ZB_ERL_INTERFACE_DEVICE_ID,                                                \
    ZB_DEVICE_VER_ERL_INTERFACE,                                               \
    0,                                                                         \
    in_clust_num,                                                              \
    out_clust_num,                                                             \
    {                                                                          \
      /* Server clusters */                                                    \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                 \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                              \
      ZB_ZCL_CLUSTER_ID_TIME,                                                  \
      ZB_ZCL_CLUSTER_ID_METER_IDENTIFICATION,                                  \
      ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                                \
      ZB_ZCL_CLUSTER_ID_DIAGNOSTICS,                                           \
      ZB_ZCL_CLUSTER_ID_METERING,                                              \
      ZB_ZCL_CLUSTER_ID_MESSAGING,                                             \
      ZB_ZCL_CLUSTER_ID_DAILY_SCHEDULE,                                        \
      /* Client clusters */                                                    \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                              \
      ZB_ZCL_CLUSTER_ID_TIME,                                                  \
    }                                                                          \
  }

/** @endcond */

/** @def ZB_DECLARE_ERL_INTERFACE_DEV_EP
 *  @brief Declare endpoint for ERL device
 *  @param ep_name - endpoint variable name
 *  @param ep_id - endpoint ID
 *  @param cluster_list - endpoint cluster list
 */
#define ZB_DECLARE_ERL_INTERFACE_DEV_EP(ep_name, ep_id, cluster_list)                 \
  ZB_ZCL_DECLARE_ERL_INTERFACE_DEV_SIMPLE_DESC(ep_name,                               \
                                               ep_id,                                 \
                                               ZB_ERL_INTERFACE_DEV_IN_CLUSTER_NUM,   \
                                               ZB_ERL_INTERFACE_DEV_OUT_CLUSTER_NUM); \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info##device_ctx_name,                 \
                                     ZB_ERL_INTERFACE_DEV_REPORT_ATTR_COUNT);         \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name,                                                \
                              ep_id,                                                  \
                              ZB_AF_HA_PROFILE_ID,                                    \
                              0,                                                      \
                              NULL,                                                   \
                              ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), \
                              cluster_list,                                           \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,       \
                              ZB_ERL_INTERFACE_DEV_REPORT_ATTR_COUNT,                 \
                              reporting_info##device_ctx_name,                        \
                              0, NULL)

/** @def ZB_DECLARE_ERL_INTERFACE_DEV_CTX
 *  @brief Declare ERL device context.
 *  @param device_ctx - device context variable name.
 *  @param ep_name - endpoint variable name.
 */
#define ZB_DECLARE_ERL_INTERFACE_DEV_CTX(device_ctx_name, ep_name) \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx_name, ep_name)

#endif /* defined ZB_HA_DEFINE_DEVICE_ERL_INTERFACE_DEVICE || defined DOXYGEN */

#endif /* ZB_ERL_INTERFACE_DEVICE_H */
