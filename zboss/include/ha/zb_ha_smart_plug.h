/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: HA Smart Plug device definition
*/

#ifndef ZB_HA_SMART_PLUG_H
#define ZB_HA_SMART_PLUG_H 1

#if defined ZB_HA_DEFINE_DEVICE_SMART_PLUG || defined DOXYGEN

/******************* Specific definitions for window covering device **************************/

/** @cond DOXYGEN_HA_SECTION */

/**
 *  @defgroup ZB_HA_DEFINE_DEVICE_SMART_PLUG Smart Plug
 *  @ingroup ZB_HA_DEVICES
 *  @{
    @details
        - @ref ZB_ZCL_BASIC \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_ON_OFF \n
        - @ref ZB_ZCL_METERING \n
        - @ref ZB_ZCL_ELECTRICAL_MEASUREMENT

*/

#define ZB_HA_DEVICE_VER_SMART_PLUG 1  /*!< Smart Plug device version */

/*! @cond internals_doc */
#define ZB_HA_SMART_PLUG_IN_CLUSTER_NUM 5 /*!< Smart Plug IN (server) clusters number */
#define ZB_HA_SMART_PLUG_OUT_CLUSTER_NUM 1 /*!< Smart Plug OUT (client) clusters number */

#define ZB_HA_SMART_PLUG_CLUSTER_NUM (ZB_HA_SMART_PLUG_IN_CLUSTER_NUM + \
  ZB_HA_SMART_PLUG_OUT_CLUSTER_NUM)

/*! Number of attribute for reporting on Smart Plug device */
#define ZB_HA_SMART_PLUG_REPORT_ATTR_COUNT         \
  (ZB_ZCL_METERING_REPORT_ATTR_COUNT               \
   + ZB_ZCL_ON_OFF_REPORT_ATTR_COUNT)

/*! @endcond */

/*!
  @brief Declare cluster list for Smart Plug device
  @param cluster_list_name - cluster list variable name
  @param basic_attr_list - attribute list for Basic cluster
  @param identify_attr_list - attribute list for Identify cluster
  @param on_off_attr_list - attribute list for On/Off cluster
  @param metering_attr_list - attribute list for Metering cluster
  @param el_measurement_attr_list - attribute list for Electrical Measurement cluster
 */
#define ZB_HA_DECLARE_SMART_PLUG_CLUSTER_LIST(                    \
  cluster_list_name,                                              \
  basic_attr_list,                                                \
  identify_attr_list,                                             \
  on_off_attr_list,                                               \
  metering_attr_list,                                             \
  el_measurement_attr_list)                                       \
  zb_zcl_cluster_desc_t cluster_list_name[] =                     \
  {                                                               \
    ZB_ZCL_CLUSTER_DESC(                                          \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                 \
      ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),       \
      (identify_attr_list),                                       \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                   \
    ),                                                            \
    ZB_ZCL_CLUSTER_DESC(                                          \
      ZB_ZCL_CLUSTER_ID_BASIC,                                    \
      ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),          \
      (basic_attr_list),                                          \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                   \
    ),                                                            \
    ZB_ZCL_CLUSTER_DESC(                                          \
      ZB_ZCL_CLUSTER_ID_ON_OFF,                                   \
      ZB_ZCL_ARRAY_SIZE(on_off_attr_list, zb_zcl_attr_t),         \
      (on_off_attr_list),                                         \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                   \
    ),                                                            \
    ZB_ZCL_CLUSTER_DESC(                                          \
      ZB_ZCL_CLUSTER_ID_METERING,                                 \
      ZB_ZCL_ARRAY_SIZE(metering_attr_list, zb_zcl_attr_t),       \
      (metering_attr_list),                                       \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                   \
    ),                                                            \
    ZB_ZCL_CLUSTER_DESC(                                          \
      ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                   \
      ZB_ZCL_ARRAY_SIZE(el_measurement_attr_list, zb_zcl_attr_t), \
      (el_measurement_attr_list),                                 \
      ZB_ZCL_CLUSTER_SERVER_ROLE,                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                   \
    ),                                                            \
    ZB_ZCL_CLUSTER_DESC(                                          \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                 \
      0,                                                          \
      NULL,                                                       \
      ZB_ZCL_CLUSTER_CLIENT_ROLE,                                 \
      ZB_ZCL_MANUF_CODE_INVALID                                   \
    )                                                             \
  }


/*!
  @brief Declare simple descriptor for Smart Plug device
  @param ep_name - endpoint variable name
  @param ep_id - endpoint ID
  @param in_clust_num - number of supported input clusters
  @param out_clust_num - number of supported output clusters
*/
#define ZB_ZCL_DECLARE_SMART_PLUG_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                     \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =              \
  {                                                                                        \
    ep_id,                                                                                 \
    ZB_AF_HA_PROFILE_ID,                                                                   \
    ZB_HA_SMART_PLUG_DEVICE_ID,                                                            \
    ZB_HA_DEVICE_VER_SMART_PLUG,                                                           \
    0,                                                                                     \
    in_clust_num,                                                                          \
    out_clust_num,                                                                         \
    {                                                                                      \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                             \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                          \
      ZB_ZCL_CLUSTER_ID_ON_OFF,                                                            \
      ZB_ZCL_CLUSTER_ID_METERING,                                                          \
      ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                                            \
      ZB_ZCL_CLUSTER_ID_IDENTIFY,                                                          \
    }                                                                                      \
  }

/*!
  @brief Declare endpoint for Smart Plug device
  @param ep_name - endpoint variable name
  @param ep_id - endpoint ID
  @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_SMART_PLUG_EP(ep_name, ep_id, cluster_list)           \
  ZB_ZCL_DECLARE_SMART_PLUG_SIMPLE_DESC(ep_name, ep_id,                     \
    ZB_HA_SMART_PLUG_IN_CLUSTER_NUM, ZB_HA_SMART_PLUG_OUT_CLUSTER_NUM); \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## device_ctx_name,    \
                                     ZB_HA_SMART_PLUG_REPORT_ATTR_COUNT); \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID,        \
    0,                                                                          \
    NULL,                                                                       \
    ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list,       \
    (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,                     \
    ZB_HA_SMART_PLUG_REPORT_ATTR_COUNT, reporting_info## device_ctx_name, 0, NULL)


/*!
  @brief Declare application's device context for Smart Plug device
  @param device_ctx - device context variable
  @param ep_name - endpoint variable name
*/
#define ZB_HA_DECLARE_SMART_PLUG_CTX(device_ctx, ep_name)                                                   \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)



/* #define ZB_ZCL_ENABLE_DEFAULT_OTA_UPGRADE_PROCESSING     */
/*! @} */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ZB_HA_DEFINE_DEVICE_SMART_PLUG */

#endif /* ZB_HA_SMART_PLUG_H */
