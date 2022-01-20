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
/* PURPOSE: IAS Warning Device
*/

#ifndef ZB_HA_IAS_WARNING_H
#define ZB_HA_IAS_WARNING_H 1

#if defined ZB_HA_DEFINE_DEVICE_IAS_WARNING_DEVICE || defined DOXYGEN

/** @cond DOXYGEN_HA_SECTION */

/**
 *  @defgroup ZB_HA_IAS_WARNING IAS Warning Device
 *  @ingroup ZB_HA_DEVICES
 *  @{
    @details
    IAS Warning Device has 6 clusters (see spec 7.8.4): \n
        - @ref ZB_ZCL_BASIC \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_IAS_WD \n
        - @ref ZB_ZCL_IAS_ZONE \n
        - @ref ZB_ZCL_SCENES \n
        - @ref ZB_ZCL_GROUPS \n

    Sample use IAS Warning Device

    @par Example

    - Declaring clusters:

        @snippet HA_samples/common/zcl_basic_attr_list.h BASIC_CLUSTER_COMMON
        @snippet HA_samples/common/zcl_ias_zone_attr_list.h IAS_ZONE_CLUSTER_COMMON
        @snippet HA_samples/ias_warning_device/sample_zc.c COMMON_DECLARATION

    - Registering device list:

        @snippet HA_samples/ias_warning_device/sample_zc.c REGISTER

    - Example of command handler:

        @snippet HA_samples/ias_warning_device/sample_zc.c COMMAND_HANDLER
        @snippet HA_samples/ias_warning_device/sample_zc.c ZCL_COMMAND_HANDLER

    @par
*/

#define ZB_HA_IAS_WARNING_VER 0      /**< Device version */

#define ZB_HA_IAS_WARNING_IN_CLUSTER_NUM  6 /**< Input clusters number. */
#define ZB_HA_IAS_WARNING_OUT_CLUSTER_NUM 0 /**< Output clusters number. */

/*! @internal Number of attribute for reporting on IAS Warning device */
#define ZB_HA_IAS_WARNING_REPORT_ATTR_COUNT   \
  (ZB_ZCL_IAS_ZONE_REPORT_ATTR_COUNT + ZB_ZCL_IAS_WD_REPORT_ATTR_COUNT)

/**
 *  @brief Declare cluster list for IAS Warning device.
 *  @param cluster_list_name [IN] - cluster list variable name.
 *  @param basic_attr_list [IN] - attribute list for Basic cluster.
 *  @param identify_attr_list [IN] - attribute list for Identify cluster.
 *  @param ias_wd_attr_list [IN] - attribute list for IAS WD cluster.
 *  @param ias_zone_attr_list [IN] - attribute list for IAS Zone cluster.
 *  @param scenes_attr_list [IN] - attribute list for Scenes cluster.
 *  @param groups_attr_list [IN] - attribute list for Groups cluster.
 */
#define ZB_HA_DECLARE_IAS_WARNING_CLUSTER_LIST(                    \
  cluster_list_name,                                               \
  basic_attr_list,                                                 \
  identify_attr_list,                                              \
  ias_wd_attr_list,                                                \
  ias_zone_attr_list,                                              \
  scenes_attr_list,                                                \
  groups_attr_list)                                                \
      zb_zcl_cluster_desc_t cluster_list_name[] =                  \
      {                                                            \
        ZB_ZCL_CLUSTER_DESC(                                       \
          ZB_ZCL_CLUSTER_ID_BASIC,                                 \
          ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),       \
          (basic_attr_list),                                       \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                              \
          ZB_ZCL_MANUF_CODE_INVALID                                \
        ),                                                         \
        ZB_ZCL_CLUSTER_DESC(                                       \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                              \
          ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),    \
          (identify_attr_list),                                    \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                              \
          ZB_ZCL_MANUF_CODE_INVALID                                \
        ),                                                         \
        ZB_ZCL_CLUSTER_DESC(                                       \
          ZB_ZCL_CLUSTER_ID_IAS_WD,                                \
          ZB_ZCL_ARRAY_SIZE(ias_wd_attr_list, zb_zcl_attr_t),      \
          (ias_wd_attr_list),                                      \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                              \
          ZB_ZCL_MANUF_CODE_INVALID                                \
        ),                                                         \
        ZB_ZCL_CLUSTER_DESC(                                       \
          ZB_ZCL_CLUSTER_ID_IAS_ZONE,                              \
          ZB_ZCL_ARRAY_SIZE(ias_zone_attr_list, zb_zcl_attr_t),    \
          (ias_zone_attr_list),                                    \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                              \
          ZB_ZCL_MANUF_CODE_INVALID                                \
        ),                                                         \
        ZB_ZCL_CLUSTER_DESC(                                       \
          ZB_ZCL_CLUSTER_ID_SCENES,                                \
          ZB_ZCL_ARRAY_SIZE(scenes_attr_list, zb_zcl_attr_t),      \
          (scenes_attr_list),                                      \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                              \
          ZB_ZCL_MANUF_CODE_INVALID                                \
        ),                                                         \
        ZB_ZCL_CLUSTER_DESC(                                       \
          ZB_ZCL_CLUSTER_ID_GROUPS,                                \
          ZB_ZCL_ARRAY_SIZE(groups_attr_list, zb_zcl_attr_t),      \
          (groups_attr_list),                                      \
          ZB_ZCL_CLUSTER_SERVER_ROLE,                              \
          ZB_ZCL_MANUF_CODE_INVALID                                \
        ),                                                         \
      }

/**
 *  @brief Declare simple descriptor for IAS Warning device.
 *  @param ep_name - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param in_clust_num [IN] - number of supported input clusters.
 *  @param out_clust_num [IN] - number of supported output clusters.
 *  @note in_clust_num, out_clust_num should be defined by numeric constants, not variables or any
 *  definitions, because these values are used to form simple descriptor type name.
 */
#define ZB_HA_DECLARE_IAS_WARNING_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num)   \
      ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);    \
      ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)     \
            simple_desc_##ep_name =                           \
      {                                                       \
        ep_id,                                                \
        ZB_AF_HA_PROFILE_ID,                                  \
        ZB_HA_IAS_WARNING_DEVICE_ID,                          \
        ZB_HA_IAS_WARNING_VER,                                \
        0,                                                    \
        in_clust_num,                                         \
        out_clust_num,                                        \
        {                                                     \
          ZB_ZCL_CLUSTER_ID_BASIC,                            \
          ZB_ZCL_CLUSTER_ID_IDENTIFY,                         \
          ZB_ZCL_CLUSTER_ID_IAS_WD,                           \
          ZB_ZCL_CLUSTER_ID_IAS_ZONE,                         \
          ZB_ZCL_CLUSTER_ID_SCENES,                           \
          ZB_ZCL_CLUSTER_ID_GROUPS,                           \
        }                                                     \
      }

/**
 *  @brief Declare endpoint for IAS Warning device.
 *  @param ep_name [IN] - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param cluster_list [IN] - endpoint cluster list.
 */
#define ZB_HA_DECLARE_IAS_WARNING_EP(ep_name, ep_id, cluster_list)             \
      ZB_HA_DECLARE_IAS_WARNING_SIMPLE_DESC(                   \
          ep_name,                                             \
          ep_id,                                               \
          ZB_HA_IAS_WARNING_IN_CLUSTER_NUM,                    \
          ZB_HA_IAS_WARNING_OUT_CLUSTER_NUM);                  \
      ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## device_ctx_name,     \
                                         ZB_HA_IAS_WARNING_REPORT_ATTR_COUNT); \
      ZB_AF_DECLARE_ENDPOINT_DESC(                                             \
        ep_name,                                                               \
            ep_id,                                             \
            ZB_AF_HA_PROFILE_ID,                               \
            0,                                                 \
            NULL,                                              \
            ZB_ZCL_ARRAY_SIZE(                                 \
                cluster_list,                                  \
                zb_zcl_cluster_desc_t),                        \
            cluster_list,                                      \
        (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,                      \
        ZB_HA_IAS_WARNING_REPORT_ATTR_COUNT, reporting_info## device_ctx_name, 0, NULL)

#define ZB_HA_DECLARE_IAS_WARNING_CTX(device_ctx, ep_name)      \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

/**
 *  @}
 */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* defined ZB_HA_DEFINE_DEVICE_IAS_WARNING_DEVICE || defined DOXYGEN */

#endif /* ZB_HA_IAS_WARNING_H */
