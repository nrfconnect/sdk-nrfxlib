/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
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
/* PURPOSE: ZBOSS AF API
*/

#ifndef ZBOSS_API_AF_H
#define ZBOSS_API_AF_H 1

/**
 *  @addtogroup af_api
 *  @{
 */

/**
 * @addtogroup af_data_service AF data service
 * @{
 */

/**
   Node descriptor
 */
typedef ZB_PACKED_PRE struct zb_af_node_desc_s
{
  zb_uint16_t          node_desc_flags;            /*!< node description */
  zb_uint8_t           mac_capability_flags;       /*!< mac capability */
  zb_uint16_t          manufacturer_code;          /*!< Manufacturer code */
  zb_uint8_t           max_buf_size;               /*!< Maximum buffer size */
  zb_uint16_t          max_incoming_transfer_size; /*!< Maximum incoming transfer size */
  zb_uint16_t          server_mask;                /*!< Server mask */
  zb_uint16_t          max_outgoing_transfer_size; /*!< Maximum outgoing transfer size */
  zb_uint8_t           desc_capability_field;      /*!< Descriptor capability field */
}
ZB_PACKED_STRUCT
zb_af_node_desc_t;

typedef zb_nwk_device_type_t zb_logical_type_t;

/** @} */ /* af_data_service */

/**
 *  @addtogroup af_node_desc_dev_types AF device types
 *  @{
 */
/** Device type - Coordinator */
#define ZB_COORDINATOR ZB_NWK_DEVICE_TYPE_COORDINATOR
/** Device type - Router */
#define ZB_ROUTER      ZB_NWK_DEVICE_TYPE_ROUTER
/** Device type - End Device */
#define ZB_END_DEVICE  ZB_NWK_DEVICE_TYPE_ED
/*! @} */ /* af_node_desc_dev_types */

/**
 * @addtogroup af_common_constants AF common constants
 * @{
 */
/**
  Node descriptor - frequency value
  */
enum zb_freq_band_e
{
  ZB_FREQ_BAND_868 = 1,                 /*!< 868-868.6 MHz BPSK */
  ZB_FREQ_BAND_902 = 1 << 2,            /*!< 902-928 MHz BPSK */
  ZB_FREQ_BAND_2400 = 1 << 3,           /*!< 2400-2483.5 MHz */
  ZB_FREQ_BAND_SUB_GHZ_EU_FSK = 1 << 4, /*!< Zigbee R22: European FSK sub-GHz
                                         * bands (863-876MHz, 915-921MHz) */
};

/**
  Node descriptor - server mask
  */
enum zb_server_mask_bit_e
{
  ZB_PRIMARY_TRUST_CENTER = 1,              /*!< Primary Trust Center */
  ZB_BACKUP_TRUST_CENTER = 1 << 1,          /*!< Backup Trust Center */
  ZB_PRIMARY_BINDING_TABLE_CENTER = 1 << 2, /*!< Primary Binding Table Center */
  ZB_BACKUP_BINDING_TABLE_CENTER = 1 << 3,  /*!< Backup Binding Table Center */
  ZB_PRIMARY_DISCOVERY_CACHE = 1 << 4,      /*!< Primary Discovery Cache */
  ZB_BACKUP_DISCOVERY_CACHE = 1 << 5,       /*!< Backup Discovery Cache */
  ZB_NETWORK_MANAGER = 1 << 6               /*!< Network Manager */
};

/**
  Node descriptor - capability mask
  */
enum zb_desc_capability_e
{
  ZB_EXT_ACTIVE_EP_LIST = 1,       /*!< Extended Active Endpoint List Available */
  ZB_EXT_SIMPLE_DESC_LIST = 1 <<1  /*!< Extended Simple Descriptor List Available */
};

/**
  Power descriptor types
  */
typedef enum zb_current_power_mode_e
{
  ZB_POWER_MODE_SYNC_ON_WHEN_IDLE = 0,
  /*!< Receiver synchronized with the receiver on when idle subfield of the node descriptor. */
  ZB_POWER_MODE_COME_ON_PERIODICALLY = 1,
  /*!< Receiver comes on periodically as defined by the node power descriptor. */
  ZB_POWER_MODE_COME_ON_WHEN_STIMULATED = 2
  /*!< Receiver comes on when stimulated, for example, by a user pressing a button. */
}
zb_current_power_mode_t;

/**
  Power source type
 */
typedef enum zb_power_src_e
{
  ZB_POWER_SRC_CONSTANT = 1,                 /*!< Constant (mains) power */
  ZB_POWER_SRC_RECHARGEABLE_BATTERY = 1 << 1, /*!< Rechargeable battery */
  ZB_POWER_SRC_DISPOSABLE_BATTERY = 1 << 2    /*!< Disposable battery */
}
zb_power_src_t;

/**
  Power source level values
 */
typedef enum zb_power_source_level_e
{
  ZB_POWER_LEVEL_CRITICAL = 0, /*!< Critical */
  ZB_POWER_LEVEL_33 = 4,       /*!< 33% */
  ZB_POWER_LEVEL_66 = 8,       /*!< 66% */
  ZB_POWER_LEVEL_100 = 12      /*!< 100% */
}
zb_power_source_level_t;
/** @} */ /* af_common_constants */

/**
 * @addtogroup af_data_service AF data service
 * @{
 */

/**
   Node power descriptor
 */
typedef ZB_PACKED_PRE struct zb_af_node_power_desc_s
{
/**   Field name               Bits
 * Current power mode:         4@n
 * Available power sources:    4@n
 * Current power source:       4@n
 * Current power source level: 4
 */
  zb_uint16_t power_desc_flags;
} ZB_PACKED_STRUCT zb_af_node_power_desc_t;


/** @cond internals_doc */
#define __CAT__(a, b, c, d, e) a##b##c##d##e
/** @endcond */ /*internals_doc */

/** Generate simple descriptor type name */
#define ZB_AF_SIMPLE_DESC_TYPE(in_num, out_num)  __CAT__(zb_af_simple_desc_,in_num,_,out_num,_t)

/**
   Declares Simple descriptor type

   @param in_clusters_count - number of input clusters in descriptor
   @param out_clusters_count - number of output clusters in descriptor

   @b Example:
   @code
     ZB_DECLARE_SIMPLE_DESC(5, 5);
   @endcode
 */

#define ZB_DECLARE_SIMPLE_DESC(in_clusters_count, out_clusters_count)   \
  typedef ZB_PACKED_PRE struct zb_af_simple_desc_ ## in_clusters_count ## _ ## out_clusters_count ## _s \
  {                                                                                       \
    zb_uint8_t    endpoint;                 /* Endpoint */                                \
    zb_uint16_t   app_profile_id;           /* Application profile identifier */          \
    zb_uint16_t   app_device_id;            /* Application device identifier */           \
    zb_bitfield_t app_device_version:4;     /* Application device version */              \
    zb_bitfield_t reserved:4;               /* Reserved */                                \
    zb_uint8_t    app_input_cluster_count;  /* Application input cluster count */         \
    zb_uint8_t    app_output_cluster_count; /* Application output cluster count */        \
    /* Application input and output cluster list */                                       \
    zb_uint16_t   app_cluster_list[in_clusters_count + out_clusters_count];               \
  } ZB_PACKED_STRUCT                                                                      \
  zb_af_simple_desc_ ## in_clusters_count ## _ ## out_clusters_count ## _t

/** @} */ /* af_data_service */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */
/** General descriptor type */
ZB_DECLARE_SIMPLE_DESC(1,1);
/** ZDO descriptor type */
ZB_DECLARE_SIMPLE_DESC(8,9);

/**
  Set simple descriptor parameters
  @param simple_desc - pointer to simple descriptor
  @param endpoint - Endpoint
  @param app_profile_id - Application profile identifier
  @param app_device_id - Application device identifier
  @param app_device_version - Application device version
  @param app_input_cluster_count - Application input cluster count
  @param app_output_cluster_count - Application output cluster count

  @par Example
  @snippet doxygen_snippets.dox zb_set_simple_descriptor_certification_TP_ZDO_BV-09_tp_zdo_bv_09_zc_c
  @par

  See tp_zdo_bv_09, tp_zdo_bv-11 samples
*/
void zb_set_simple_descriptor(zb_af_simple_desc_1_1_t *simple_desc,
                              zb_uint8_t  endpoint, zb_uint16_t app_profile_id,
                              zb_uint16_t app_device_id, zb_bitfield_t app_device_version,
                              zb_uint8_t app_input_cluster_count, zb_uint8_t app_output_cluster_count);

/**
  Set input cluster item
  @param simple_desc - pointer to simple descriptor
  @param cluster_number - cluster item number
  @param cluster_id - cluster ID

  @par Example
  @snippet doxygen_snippets.dox zb_set_simple_descriptor_certification_TP_ZDO_BV-09_tp_zdo_bv_09_zc_c
  @par

  See tp_zdo_bv_09, tp_zdo_bv-11 samples
*/
void zb_set_input_cluster_id(zb_af_simple_desc_1_1_t *simple_desc, zb_uint8_t cluster_number, zb_uint16_t cluster_id);

/*! @brief Set output cluster item
    @param simple_desc - pointer to simple descriptor
    @param cluster_number - cluster item number
    @param cluster_id - cluster ID

  @par Example
  @snippet doxygen_snippets.dox zb_set_simple_descriptor_certification_TP_ZDO_BV-09_tp_zdo_bv_09_zc_c
  @par

  See tp_zdo_bv_09, tp_zdo_bv-11 samples
*/
void zb_set_output_cluster_id(zb_af_simple_desc_1_1_t *simple_desc, zb_uint8_t cluster_number, zb_uint16_t cluster_id);

/**
  Set default descriptors values for FFD.
  @param device_type - device type ZB_COORDINATOR or ZB_ROUTER

  @code
    zb_set_default_ffd_descriptor_values(ZB_COORDINATOR);
  @endcode

  See tp_zdo_bv_09, tp_zdo_bv-11 samples
 */
void zb_set_default_ffd_descriptor_values(zb_logical_type_t device_type);

/**
  Set default descriptors values for end device.
 */
void zb_set_default_ed_descriptor_values(void);
/** @} */ /* af_management_service */

/**
 * @addtogroup af_common_constants AF common constants
 * @{
 */
/*! Profile identifiers */
enum zb_af_profile_id_e
{
  /** ZDO profile ID */
  ZB_AF_ZDO_PROFILE_ID  = 0x0000,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE1_ID  = 0x0101,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE2_ID  = 0x0102,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE3_ID  = 0x0103,
  /** HA profile ID*/
  ZB_AF_HA_PROFILE_ID  = 0x0104,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE4_ID  = 0x0105,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE5_ID  = 0x0106,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE6_ID  = 0x0107,
  /** Legacy profile */
  ZB_AF_LEGACY_PROFILE7_ID  = 0x0108,
  /** SE profile ID */
  ZB_AF_SE_PROFILE_ID  = 0x0109,
  /** ZLL profile identifier. */
  ZB_AF_ZLL_PROFILE_ID      = 0xc05e,
  /** Wildcard profile identifier. */
  ZB_AF_WILDCARD_PROFILE_ID = 0xffff,
  /** GreenPower profile ID */
  ZB_AF_GP_PROFILE_ID = 0xA1E0,
#ifdef ZB_CONTROL4_NETWORK_SUPPORT
  /** Control4 profile identifier. */
  ZB_AF_CONTROL4_PROFILE_ID = 0xC25D,
#endif
};
/** @} */ /* af_common_constants */

/** @cond internals_doc */
#if !(defined ZB_ZCL_DISABLE_REPORTING) || defined(DOXYGEN)
struct zb_zcl_reporting_info_s; /* Forward declaration */
#endif
struct zb_zcl_cluster_desc_s;   /* Forward declaration */
/** @endcond */ /* internals_doc */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */
/**
 * Device command handler
 *
 * @param param - index of buffer with ZCL command
 * @return status (see @ref zb_bool_t)
 */
typedef zb_uint8_t (*zb_device_handler_t)(zb_uint8_t param);
/** @} */ /* af_management_service */
/**
 * @addtogroup af_data_service AF data service
 * @{
 */

/** Endpoint description for user applications. */

/*! @brief Endpoint descriptor. */
typedef ZB_PACKED_PRE struct zb_af_endpoint_desc_s
{
  zb_uint8_t ep_id;               /*!< Endpoint ID */
  zb_uint16_t profile_id;         /*!< Application profile, which implemented on endpoint */
  zb_device_handler_t device_handler;   /*!< Device specific callback, handles all commands */
  /*! @brief Identify notification callback

      If set, will be called on identification start and stop indicating start
      event with a non-zero argument
  */
  zb_callback_t identify_handler;
  zb_uint8_t reserved_size; /*!< Unused parameter (reserved for future use) */
  zb_voidp_t reserved_ptr; /*!< Unused parameter (reserved for future use) */
  zb_uint8_t cluster_count;       /*!< Number of supported clusters */
  struct zb_zcl_cluster_desc_s *cluster_desc_list;  /*!< Supported clusters list */
  zb_af_simple_desc_1_1_t *simple_desc; /*!< Simple descriptor */
#if defined ZB_ENABLE_ZLL || defined DOXYGEN
  zb_uint8_t group_id_count;
#endif /* defined ZB_ENABLE_ZLL || defined DOXYGEN */

  /* Moved here from device descriptor */
  zb_uint8_t rep_info_count;           /*!< Number of reporting info slots */
  /* Reporting info structure should be better stored in ZCL main,
   * moved here to allow compile-time memory allocating for
   * reporting_info array, see ZB_AF_DECLARE_DEVICE_CTX() */
  struct zb_zcl_reporting_info_s *reporting_info; /*!< Attributes reporting information */

  zb_uint8_t cvc_alarm_count;          /*!< Number of continuous value change alarm slots */
  struct zb_zcl_cvc_alarm_variables_s *cvc_alarm_info;
}ZB_PACKED_STRUCT
zb_af_endpoint_desc_t;
/** @} */ /* af_data_service */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */

/**
 *  @brief Search for endpoint (device) descriptor by endpoint ID.
 *  @param ep_id - identifier of the endpoint.
 *  @returns NULL if no endpoint found, or pointer to the endpoint descriptor.
 */
zb_af_endpoint_desc_t* zb_af_get_endpoint_desc(zb_uint8_t ep_id);

/**
 *  @brief Check endpoint registration in ZCL.
 *  @param _ep - identifier of the endpoint under consideration.
 *  @returns ZB_TRUE if endpoint is registered, ZB_FALSE otherwise.
 */
#define ZB_AF_IS_EP_REGISTERED( _ep ) (zb_af_get_endpoint_desc( _ep ))?(ZB_TRUE):(ZB_FALSE)
/** @} */ /* af_management_service */
/** @cond internals_doc */

/**
  AF device context, contains a list of registered endpoints
*/
typedef ZB_PACKED_PRE struct zb_af_device_ctx_s
{
  zb_uint8_t ep_count;                 /*!< Number of endpoints on device */
  zb_af_endpoint_desc_t **ep_desc_list; /*!< Endpoint list */
}ZB_PACKED_STRUCT
zb_af_device_ctx_t;

/** @endcond */ /* internals_doc */

/**
 * @addtogroup af_management_service AF management service
 * @{
 */
/**
   General macro to declare endpoint description list
   @param ep_list_name - name of ep list structure
*/
#define ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_list_name) \
  zb_af_endpoint_desc_t *ep_list_name[] = {

/**
  @brief Initialize endpoint descriptor

   @param ep_name - endpoint name
  @param ep_id - endpoint ID
  @param profile_id - ID of profile deployed on this endpoint
   @param reserved_length - unused parameter
   @param reserved_ptr - unused parameter
  @param cluster_number - number of clusters deployed on endpoint
  @param cluster_list - pointer to cluster list
  @param simple_desc - pointer to simple descriptor
  @param rep_count - maximum number of attributes that are
  being reported on a device
  @param rep_ctx - reporting context variable name (NULL if no reporting context)
  @param lev_ctrl_count - number of level control attributes
  @param lev_ctrl_ctx - level control context variable name (NULL if no level control context)
  @see DR-TAR-TC-02 sample
*/
#define ZB_AF_DECLARE_ENDPOINT_DESC(     \
  ep_name,                               \
  ep_id,                                 \
  profile_id,                            \
  reserved_length,                       \
  reserved_ptr,                          \
  cluster_number,                        \
  cluster_list,                          \
  simple_desc,                           \
  rep_count,                             \
  rep_ctx,                               \
  lev_ctrl_count,                        \
  lev_ctrl_ctx)                          \
  zb_af_endpoint_desc_t ep_name =        \
  {                                      \
    ep_id,                               \
    profile_id,                          \
    NULL,                                \
    NULL,                                \
    reserved_length,                     \
    (zb_voidp_t)reserved_ptr,            \
    cluster_number,                      \
    cluster_list,                        \
    simple_desc,                         \
    rep_count,                           \
    rep_ctx,                             \
    lev_ctrl_count,                      \
    lev_ctrl_ctx                         \
  }

/** Finishes endpoint list declaration */
#define ZB_AF_FINISH_DECLARE_ENDPOINT_LIST     }

/**
 *  @brief Allows application to register cluster commands handler for a specific endpoint
 *  @param endpoint - Endpoint number
 *  @param handler - Pointer to a function, function type zb_device_handler_t
 *  @attention Endpoint should be registered. ZB_AF_SET_ENDPOINT_HANDLER() should be called after
 *  device context is registered, using call to ZB_AF_REGISTER_DEVICE_CTX()
 *
 *  See HA samples
 */
#define ZB_AF_SET_ENDPOINT_HANDLER(endpoint, handler)                 \
  (zb_af_get_endpoint_desc((endpoint))->device_handler = (handler))

/*! @brief Sets identify notification handler for endpoint

    Utilized by ZCL Identify cluster
    @param endpoint number
    @param handler to set
    @attention Assumes that endpoint already registered
 *
 *  See HA samples
*/
#define ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(endpoint, handler)    \
  (zb_af_get_endpoint_desc((endpoint))->identify_handler = (handler))

#define ZB_ZCL_CLUSTER_DESC(cluster_id, attr_count, attr_desc_list, cluster_role_mask, manuf_code)         \
{                                                                                                          \
  (cluster_id),                                                                                            \
  (attr_count),                                                                                            \
  (attr_desc_list),                                                                                        \
  (cluster_role_mask),                                                                                     \
  (manuf_code),                                                                                            \
  (((cluster_role_mask) == ZB_ZCL_CLUSTER_SERVER_ROLE) ? cluster_id##_SERVER_ROLE_INIT : \
   (((cluster_role_mask) == ZB_ZCL_CLUSTER_CLIENT_ROLE) ? cluster_id##_CLIENT_ROLE_INIT : NULL)) \
}

/** WARNING: This API is deprecated, use ZBOSS_DECLARE_DEVICE_CTX instead. */
#define ZB_AF_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_name, ep_count, reporting_attr_count) \
  zb_zcl_reporting_info_t reporting_info## device_ctx_name[reporting_attr_count]; \
  zb_af_device_ctx_t device_ctx_name =                                  \
  {                                                                     \
    ep_count,                                                           \
    ep_list_name,                                                       \
    reporting_attr_count,                                               \
    reporting_info## device_ctx_name,                                   \
    0,                                                                  \
    NULL                                                                \
  }

/** WARNING: This API is deprecated, use ZBOSS_DECLARE_DEVICE_CTX instead. */
#define ZB_AF_DECLARE_DEVICE_CTX_NO_REP(device_ctx_name, ep_list_name, ep_count) \
  zb_af_device_ctx_t device_ctx_name =                                  \
  {                                                                     \
    ep_count,                                                           \
    ep_list_name,                                                       \
    0,                                                                  \
    NULL,                                                               \
    0,                                                                  \
    NULL                                                                \
  }

/**
  Declare device reporting context
  @param rep_ctx - reporting context variable name
  @param rep_count - number of reporting attributes
*/
#define ZBOSS_DEVICE_DECLARE_REPORTING_CTX(rep_ctx, rep_count) \
  zb_zcl_reporting_info_t rep_ctx[rep_count]

/**
  Declare device level control context
  @param lev_ctrl_ctx - level control context variable name
  @param lev_ctrl_count - number of level control attributes
*/
#define ZBOSS_DEVICE_DECLARE_LEVEL_CONTROL_CTX(lev_ctrl_ctx, lev_ctrl_count) \
  zb_zcl_cvc_alarm_variables_t lev_ctrl_ctx[lev_ctrl_count]

/**
  Declare application's device context
  @param device_ctx_name - device context variable name
  @param ep_list_name - name of endpoint list
  @param ep_count - number of supported endpoints
*/
#define ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_name, ep_count) \
  zb_af_device_ctx_t device_ctx_name =                                  \
  {                                                                     \
    ep_count,                                                           \
    ep_list_name,                                                       \
  }

/**
  Declare application's device context for single-endpoint device

  @note Device has an additional Green Power endpoint if it is ZC or ZR
  and GPPB feature is enabled (Mandatory for Zigbee 3.0 ZC/ZR).

  @param device_ctx_name - device context variable name
  @param ep_name - variable holding endpoint context
*/
#define ZBOSS_DECLARE_DEVICE_CTX_1_EP(device_ctx_name, ep_name)        \
  ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_list_##device_ctx_name)         \
    &ep_name,                                                          \
  ZB_AF_FINISH_DECLARE_ENDPOINT_LIST;                                  \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_##device_ctx_name, \
                           (ZB_ZCL_ARRAY_SIZE(ep_list_##device_ctx_name, zb_af_endpoint_desc_t*)))

/**
  Declare application's device context for two-endpoint device

  @note Device has an additional Green Power endpoint if it is ZC or ZR
  and GPPB feature (Mandatory for Zigbee 3.0 ZC/ZR) is enabled.

  @param device_ctx_name - device context variable name
  @param ep1_name - variable holding context for endpoint 1
  @param ep2_name - variable holding context for endpoint 2
*/
#define ZBOSS_DECLARE_DEVICE_CTX_2_EP(device_ctx_name, ep1_name, ep2_name) \
  ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_list_##device_ctx_name)          \
    &ep1_name,                                                          \
    &ep2_name,                                                          \
  ZB_AF_FINISH_DECLARE_ENDPOINT_LIST;                                   \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_##device_ctx_name,  \
                           (ZB_ZCL_ARRAY_SIZE(ep_list_##device_ctx_name, zb_af_endpoint_desc_t*)))

/**
  Declare application's device context for three-endpoint device

  @note Device has an additional Green Power endpoint if it is ZC or ZR
  and GPPB feature (Mandatory for Zigbee 3.0 ZC/ZR) is enabled.

  @param device_ctx_name - device context variable name
  @param ep1_name - variable holding context for endpoint 1
  @param ep2_name - variable holding context for endpoint 2
  @param ep3_name - variable holding context for endpoint 3
*/
#define ZBOSS_DECLARE_DEVICE_CTX_3_EP(device_ctx_name, ep1_name, ep2_name, ep3_name) \
  ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_list_##device_ctx_name)          \
    &ep1_name,                                                          \
    &ep2_name,                                                          \
    &ep3_name,                                                          \
  ZB_AF_FINISH_DECLARE_ENDPOINT_LIST;                                   \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_##device_ctx_name,  \
                           (ZB_ZCL_ARRAY_SIZE(ep_list_##device_ctx_name, zb_af_endpoint_desc_t*)))

/**
  Declare application's device context for for-endpoint device

  @note Device has an additional Green Power endpoint if it is ZC or ZR
  and GPPB feature (Mandatory for Zigbee 3.0 ZC/ZR) is enabled.

  @param device_ctx_name - device context variable name
  @param ep1_name - variable holding context for endpoint 1
  @param ep2_name - variable holding context for endpoint 2
  @param ep3_name - variable holding context for endpoint 3
  @param ep4_name - variable holding context for endpoint 4
*/
#define ZBOSS_DECLARE_DEVICE_CTX_4_EP(device_ctx_name, ep1_name, ep2_name, ep3_name, ep4_name) \
  ZB_AF_START_DECLARE_ENDPOINT_LIST(ep_list_##device_ctx_name)          \
    &ep1_name,                                                          \
    &ep2_name,                                                          \
    &ep3_name,                                                          \
    &ep4_name,                                                          \
  ZB_AF_FINISH_DECLARE_ENDPOINT_LIST;                                   \
  ZBOSS_DECLARE_DEVICE_CTX(device_ctx_name, ep_list_##device_ctx_name,  \
                           (ZB_ZCL_ARRAY_SIZE(ep_list_##device_ctx_name, zb_af_endpoint_desc_t*)))


/** @cond internals_doc */
void zb_zcl_init_reporting_ctx(void);
void zb_zcl_reset_reporting_ctx(void);


/**
  Register device context.
  @param device_ctx - pointer to device context
 */
void zb_af_register_device_ctx(zb_af_device_ctx_t *device_ctx);
/*! @endcond */
/**
  Register device context.
  @param _device_ctx - Pointer to the device context

  @b Example:
  @snippet onoff_server/on_off_output_zc.c af_register_device_context
*/
#define ZB_AF_REGISTER_DEVICE_CTX(_device_ctx) zb_af_register_device_ctx(_device_ctx)

/**
 *  @brief Set Device user application callback.
 *  The macro sets a callback being called upon to need inform User App about change device attribute or
 *  need to execute H/W command
 *  @param func_ptr - a callback to set (of type @ref zb_callback_t).
 *  @hideinitializer
 *
 * @b Example:
 * @snippet onoff_server/on_off_output_zc.c zcl_register_device_cb_example_cb
 * @par Register callback:
 * @snippet onoff_server/on_off_output_zc.c zcl_register_device_cb
*/
#define ZB_ZCL_REGISTER_DEVICE_CB(func_ptr) ZCL_CTX().device_cb = (func_ptr)

/**
  Callback for notifying user about status of ZCL/ZDO command transmission
 */
typedef void (*zb_af_transmission_res_cb_t) (zb_uint8_t status);
/** @} */ /* af_management_service */
/*! @} */

#endif /* ZBOSS_API_AF_H */
