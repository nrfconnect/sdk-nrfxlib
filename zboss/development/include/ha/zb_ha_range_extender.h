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
/* PURPOSE: Range extender device definition
*/

#ifndef ZB_HA_RANGE_EXTENDER_H
#define ZB_HA_RANGE_EXTENDER_H 1

#if defined ZB_HA_DEFINE_DEVICE_RANGE_EXTENDER || defined DOXYGEN

/** @cond DOXYGEN_HA_SECTION */

/**
 *  @defgroup ha_range_extender Range Extender
 *  @ingroup ZB_HA_DEVICES
    @{
    @details
    Range Extender device has 2 clusters (see spec 7.4.8): \n
        - @ref ZB_ZCL_IDENTIFY \n
        - @ref ZB_ZCL_BASIC

    Range Extender device sample

    @par Example

    - Declaring clusters:

        @snippet HA_samples/common/zcl_basic_attr_list.h BASIC_CLUSTER_COMMON
        @snippet HA_samples/range_extender/sample_zc.c COMMON_DECLARATION

    - Registering device list:

        @snippet HA_samples/range_extender/sample_zc.c REGISTER

    - Example of command handler:

        @snippet HA_samples/range_extender/sample_zc.c COMMAND_HANDLER
        @snippet HA_samples/range_extender/sample_zc.c ZCL_COMMAND_HANDLER

    @par
*/

#define ZB_HA_DEVICE_VER_RANGE_EXTENDER 0  /*!< Range extender device version */

/** @cond internals_doc */
#define ZB_HA_RANGE_EXTENDER_IN_CLUSTER_NUM 2   /*!< Range extender IN (server) clusters
                                                  number */
#define ZB_HA_RANGE_EXTENDER_OUT_CLUSTER_NUM 0  /*!< Range extender OUT (client) clusters
                                                  number */

#define ZB_HA_RANGE_EXTENDER_CLUSTER_NUM                                       \
  (ZB_HA_RANGE_EXTENDER_IN_CLUSTER_NUM + ZB_HA_RANGE_EXTENDER_OUT_CLUSTER_NUM)

/** Number of attribute for reporting on Range extender device */
#define ZB_HA_RANGE_EXTENDER_REPORT_ATTR_COUNT    0

/** @endcond */

/** @brief Declare cluster list for Range extender device
    @param cluster_list_name - cluster list variable name
    @param basic_attr_list - attribute list for Basic cluster
    @param identify_attr_list - attribute list for Identify cluster
 */
#define ZB_HA_DECLARE_RANGE_EXTENDER_CLUSTER_LIST(        \
    cluster_list_name,                                    \
    basic_attr_list,                                      \
    identify_attr_list)                                   \
zb_zcl_cluster_desc_t cluster_list_name[] =               \
{                                                         \
  ZB_ZCL_CLUSTER_DESC(                                    \
    ZB_ZCL_CLUSTER_ID_IDENTIFY,                           \
    ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t), \
    (identify_attr_list),                                 \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                           \
    ZB_ZCL_MANUF_CODE_INVALID                             \
  ),                                                      \
  ZB_ZCL_CLUSTER_DESC(                                    \
    ZB_ZCL_CLUSTER_ID_BASIC,                              \
    ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),    \
    (basic_attr_list),                                    \
    ZB_ZCL_CLUSTER_SERVER_ROLE,                           \
    ZB_ZCL_MANUF_CODE_INVALID                             \
  )                                                       \
}

/** @cond internals_doc */

/** @brief Declare simple descriptor for Range extender device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param in_clust_num - number of supported input clusters
    @param out_clust_num - number of supported output clusters
*/
#define ZB_ZCL_DECLARE_RANGE_EXTENDER_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                                         \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =                  \
  {                                                                                            \
    ep_id,                                                                                     \
    ZB_AF_HA_PROFILE_ID,                                                                       \
    ZB_HA_RANGE_EXTENDER_DEVICE_ID,                                                            \
    ZB_HA_DEVICE_VER_RANGE_EXTENDER,                                                           \
    0,                                                                                         \
    in_clust_num,                                                                              \
    out_clust_num,                                                                             \
    {                                                                                          \
      ZB_ZCL_CLUSTER_ID_BASIC,                                                                 \
      ZB_ZCL_CLUSTER_ID_IDENTIFY                                                               \
    }                                                                                          \
  }

/** @endcond */ /* internals_doc */

/** @brief Declare endpoint for Range extender device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_RANGE_EXTENDER_EP(ep_name, ep_id, cluster_list)                           \
  ZB_ZCL_DECLARE_RANGE_EXTENDER_SIMPLE_DESC(ep_name, ep_id,                                     \
      ZB_HA_RANGE_EXTENDER_IN_CLUSTER_NUM, ZB_HA_RANGE_EXTENDER_OUT_CLUSTER_NUM);               \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, 0, NULL,                              \
                          ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
                              (zb_af_simple_desc_1_1_t*)&simple_desc_##ep_name,                 \
                              0, NULL, /* No reporting ctx */                                   \
                              0, NULL) /* No CVC ctx */


/** @brief Declare Range Extender device context.
    @param device_ctx - device context variable name.
    @param ep_name - endpoint variable name.
*/
#define ZB_HA_DECLARE_RANGE_EXTENDER_CTX(device_ctx, ep_name)           \
  ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx, ep_name)

/*! @} */

/** @endcond */ /* DOXYGEN_HA_SECTION */

#endif /* ZB_HA_DEFINE_DEVICE_RANGE_EXTENDER */

#endif /* ZB_HA_RANGE_EXTENDER_H */
