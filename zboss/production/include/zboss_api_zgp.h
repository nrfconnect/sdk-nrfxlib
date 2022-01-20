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
/* PURPOSE: common definitions for ZGP profile
*/

#ifndef ZBOSS_API_ZGP_H
#define ZBOSS_API_ZGP_H 1

#include "zb_config.h"
#include "zb_address.h"
#include "zboss_api_buf.h"
#include "zb_types.h"

/********************************************************************/
/********* Type declarations specific to ZGP profile ****************/
/********************************************************************/

/**
   @addtogroup zgp_common
   @{
*/

#define ZGP_ENDPOINT 242

/** @brief ZGPD application ID */
typedef enum zb_zgp_app_id_e
{
  ZB_ZGP_APP_ID_0000    = 0x00, /**< ApplicationID value 0b000 - usage of the SrcID */
  ZB_ZGP_APP_ID_0001    = 0x01, /**< ApplicationID value 0b001 - LPED */
  ZB_ZGP_APP_ID_0010    = 0x02, /**< ApplicationID value 0b010 - usage of the GPD IEEE address.*/
  ZB_ZGP_APP_ID_INVALID = 0x07, /**< Invalid ApplicationID */
}
zb_zgp_app_id_t;

/**
 * @brief ZGPD address
 *
 * ZGPD is identified in network whether by SrcId or its IEEE address. */
typedef ZB_PACKED_PRE union zb_zgpd_addr_u
{
  zb_uint32_t     src_id;    /**< ZGPD SrcId @see ZGP spec, A.1.4.1.4 */
  zb_ieee_addr_t  ieee_addr; /**< ZGPD IEEE address */
} ZB_PACKED_STRUCT
zb_zgpd_addr_t;

enum zb_zgp_endpoint_e
{
  ZB_ZGP_COMMUNICATION_ENDPOINT,
  ZB_ZGP_ALL_ENDPOINTS = 255
};

/**
 * @brief ZGPD identification info (ZGPD ID)
 *
 * ZGPD is identified by SrcId if ApplicationID is @ref ZB_ZGP_APP_ID_0000.
 * Otherwise, ZGPD is identified by its IEEE address. */
typedef ZB_PACKED_PRE struct zb_zgpd_id_s
{
  zb_uint8_t         app_id;  /**< One of the @ref zb_zgp_app_id_t values */
  zb_uint8_t         endpoint;/**< Identifier of the GPD endpoint, which jointly with the GPD IEEE address identifies a unique logical GPD device.*/
  zb_zgpd_addr_t     addr;    /**< ZGPD SrcId or IEEE address */
} ZB_PACKED_STRUCT
zb_zgpd_id_t;

#define ZGP_ATTR_OPT_GET_REMAINING_LEN(opt) ((opt) & 0xFU)
#define ZGP_ATTR_OPT_GET_REPORTED(opt)      (((opt) >> 4U) & 0x1U)
#define ZGP_ATTR_OPT_GET_VAL_PRESENT(opt)   (((opt) >> 5U) & 0x1U)

#define ZB_APP_DESCR_ATTR_VAL_SIZE 8U
typedef struct zgp_attr_record_s
{
  zb_uint16_t id;
  zb_uint8_t  data_type;

  /*
  zb_bitfield_t remaining_len:4;

    7344 The Reported sub-field is a Boolean flag which indicates if the attribute as identified by the AttributeID
    7345 field is reported by the GPD in operation, or if it is background data required for processing of a report-
    7346 ed attribute only conveyed once at commissioning time.
    7350 If Reported = 0b1, Attribute Offset within Report field is present, otherwise it is absent

  zb_bitfield_t reported:1;
  zb_bitfield_t attr_val_present:1;
  zb_bitfield_t reserved:2;
   */
  zb_uint8_t  options;

  /*
  7358 The Attribute Offset within Report field, when present, carries the start position (in bytes) of the data
  7359 point identified by the AttributeID of the ClusterID in the report payload. The Attribute Offset within
  7360 Report = 0x00 corresponds to the octet immediately following the Report identifier field in the pay-
  7361 load of the GPD Compact Attribute Reporting command.
  */
  zb_uint8_t offset;

  /*
  7362 The Attribute value field, when present, carries the actual fixed value of that attribute; *the length and
  7363 type of this field are determined by the AttributeID of the ClusterID (in case of manufacturer-specific
  7364 attributes or clusters, corresponding to the ManufacturerID).
  */
  zb_uint8_t value[ZB_APP_DESCR_ATTR_VAL_SIZE];
}zgp_attr_record_t;

typedef struct zgp_data_point_desc_options_s
{
  zb_bitfield_t attr_records_num:3;
  zb_bitfield_t cluster_type:1; /* server == 1, client == 0 */
  zb_bitfield_t manuf_id_present:1;
  zb_bitfield_t reserved:3;
}zgp_data_point_desc_options_t;

#define ZB_APP_DESCR_ATTR_REC_SIZE 4U
typedef struct zgp_data_point_desc_s
{
  zgp_data_point_desc_options_t options;
  zb_uint16_t cluster_id;
  zb_uint16_t manuf_id;
  zgp_attr_record_t attr_records_data[ZB_APP_DESCR_ATTR_REC_SIZE]; /* contains array of not parsed zgp_attr_record_t */
}zgp_data_point_desc_t;

typedef struct zgp_report_desc_options_s
{
  zb_bitfield_t timeout_present:1;
  zb_bitfield_t reserved:7;
}zgp_report_desc_options_t;

#define ZB_APP_DESCR_REPORT_DATA_SIZE 32U
typedef ZB_PACKED_PRE struct zgp_report_desc_s
{
  zgp_report_desc_options_t  options;
  zb_uint16_t timeout;
  zb_uint8_t  point_descs_data_len;
  zb_uint8_t  point_descs_data[ZB_APP_DESCR_REPORT_DATA_SIZE]; /* contains array of not parsed zgp_data_point_desc_t */
}ZB_PACKED_STRUCT zgp_report_desc_t;

#define SIZE_BY_APP_ID(app_id) (((app_id) == ZB_ZGP_APP_ID_0000) ? 4U : 8U)
#define ZGPD_ID_SIZE(zgpd_id) SIZE_BY_APP_ID((zgpd_id)->app_id)

#define ZB_ZGPD_IDS_ARE_EQUAL(id1, id2) \
  (((id1)->app_id == (id2)->app_id) && \
   (((id1)->app_id == ZB_ZGP_APP_ID_0000) ? \
   ((id1)->addr.src_id == (id2)->addr.src_id) : \
    (!ZB_MEMCMP(&(id1)->addr.ieee_addr, &(id2)->addr.ieee_addr, sizeof(zb_ieee_addr_t)) &&\
     ((id1)->endpoint == (id2)->endpoint))))

/* From ZGP spec, A. 1.4.1.4:
 * ZGPD ID value of 0x00000000 indicates unspecified.
 * ... 0xffffffff indicates all.
 */
#define ZB_ZGP_SRC_ID_UNSPECIFIED 0x00000000U
#define ZB_ZGP_SRC_ID_ALL         (zb_uint32_t)(~0u)

#define ZB_INIT_ZGPD_ID(zgpd_id) \
{ \
  (zgpd_id)->app_id = ZB_ZGP_APP_ID_0000; \
  (zgpd_id)->addr.src_id = ZB_ZGP_SRC_ID_UNSPECIFIED; \
}

#define ZB_ZGPD_IS_SPECIFIED(zgpd_id) \
  (((zgpd_id)->app_id != ZB_ZGP_APP_ID_0000) \
  || ((zgpd_id)->addr.src_id != ZB_ZGP_SRC_ID_UNSPECIFIED))

#define ZB_MAKE_ZGPD_ID(zgpd_id, s_app_id, s_endpoint, s_addr)\
  {\
    (zgpd_id).app_id = (s_app_id);\
    (zgpd_id).endpoint = (s_endpoint);\
    (zgpd_id).addr = (s_addr);\
  }

/**
 * @brief ZGP security level type
 * @see ZGP spec, A.1.5.3.2 */
enum zb_zgp_security_level_e
{
  ZB_ZGP_SEC_LEVEL_NO_SECURITY           = 0x00,  /**< No security */
  ZB_ZGP_SEC_LEVEL_REDUCED               = 0x01,  /**< 1LSB of frame counter and short (2B) MIC */
  ZB_ZGP_SEC_LEVEL_FULL_NO_ENC           = 0x02,  /**< Full (4B) frame counter and full (4B) MIC */
  ZB_ZGP_SEC_LEVEL_FULL_WITH_ENC         = 0x03,  /**< Encryption & full (4B) frame counter and
                                                       full (4B) MIC */
};

/**
The gpSharedSecurityKeyType attribute can take the following values from Table 48:
Table 48 - Values of gpSecurityKeyType

See also Table 12 - Mapping between the gpSecurityKeyType and the SecurityKey
sub-field of the Extended NWK Frame Control field
*/
enum zb_zgp_security_key_type_e
{
  ZB_ZGP_SEC_KEY_TYPE_NO_KEY             = 0x00,  /**< No key */
  ZB_ZGP_SEC_KEY_TYPE_NWK                = 0x01,  /**< Zigbee NWK key */
  ZB_ZGP_SEC_KEY_TYPE_GROUP              = 0x02,  /**< ZGPD group key */
  ZB_ZGP_SEC_KEY_TYPE_GROUP_NWK_DERIVED  = 0x03,  /**< NWK-key derived ZGPD group key */
  ZB_ZGP_SEC_KEY_TYPE_ZGPD_INDIVIDUAL    = 0x04,  /**< (Individual) out-of-the-box ZGPD key */
  ZB_ZGP_SEC_KEY_TYPE_DERIVED_INDIVIDUAL = 0x07,  /**< Derived individual ZGPD key */
};

/********************************************************************/
/******** ZGP Device Type declarations and configuration ************/
/********************************************************************/

/**
 * @brief Possible ZGPD device identifiers
 * @see ZGP spec, A.4.3 */
typedef enum zb_zgpd_dev_id_e
{
  ZB_ZGP_SIMPLE_GEN_1_STATE_SWITCH_DEV_ID = 0x00, /**< Simple Generic 1-state ZGP switch */
  ZB_ZGP_ON_OFF_SWITCH_DEV_ID             = 0x02, /**< ZGP On/Off switch */
  ZB_ZGP_LEVEL_CONTROL_SWITCH_DEV_ID      = 0x03, /**< ZGP Level Control Switch */
#define ZB_ZGP_LVL_CTRL_SWITCH_DEV_ID ZB_ZGP_LEVEL_CONTROL_SWITCH_DEV_ID
  ZB_ZGP_TEMPERATURE_SENSOR_DEV_ID        = 0x30, /**< ZGP temperature sensor */
  ZB_ZGP_ENVIRONMENT_SENSOR_DEV_ID        = 0x33, /**< ZGP Temperature + Humidity sensor */
  ZB_ZGP_MANUF_SPECIFIC_DEV_ID            = 0xfe, /**< Manufactures-specific; 2
                                                   * more fields in the
                                                   * Commissioning frame. See 4.1
                                                   * Manufacturer Specific Green
                                                   * Power Device Type Use case
                                                   * in Green Power Device
                                                   * Manufacturer Specific Device & Command
                                                   * Definition Proposal
                                                  */
  ZB_ZGP_UNDEFINED_DEV_ID                 = 0xff, /**< Undefined device type */
}
zb_zgpd_dev_id_t;

/**
 * @brief Manufacturer-specific device identifiers for GreanPeak
 * @see ZGP spec, A.4.3 */
typedef enum zb_zgpd_manuf_specific_dev_id_e
{
  /* TODO: ZB_ZGP_MS_DOOR_SENSOR_DEV_ID is deprecated. Remove it when there will be
   * no testing devices using this type. GreanPeak door sensors use
   * ZB_ZGP_MS_DOOR_WINDOW_SENSOR_DEV_ID now. */
  ZB_ZGP_MS_DOOR_SENSOR_DEV_ID            = 0x00, /**< IAS Zone - Door Sensor*/
  ZB_ZGP_MS_DOOR_WINDOW_SENSOR_DEV_ID     = 0x01, /**< IAS Zone - Door/Window Sensor*/
  ZB_ZGP_MS_LEAKAGE_SENSOR_DEV_ID         = 0x02, /**< IAS Zone - Leakage Sensor */
  ZB_ZGP_MS_HUMIDITY_SENSOR_DEV_ID        = 0x03, /**< Relative Humidity Sensor */
  ZB_ZGP_MS_MOTION_SENSOR_DEV_ID          = 0x05, /**< IAS Zone - Motion sensor */
  ZB_ZGP_MS_MOVEMENT_SENSOR_DEV_ID        = 0x07, /**< IAS Zone - Movement sensor */
  ZB_ZGP_MS_SMART_PLUG_DEV_ID             = 0X08, /**< GreenPeak Greenpower smart plug */
  ZB_ZGP_MS_KEY_FOB_DEV_ID                = 0x0A, /**< GreenPeak Greenpower IAS Zone - Key fob */
  ZB_ZGP_MS_SMOKE_DETECTOR_DEV_ID         = 0x0b, /**< IAS Zone - Smoke Detector */
  ZB_ZGP_MS_BED_SENSOR_DEV_ID             = 0x0c, /**< IAS Zone - Bed Sensor */
  ZB_ZGP_MS_PARKING_SENSOR_DEV_ID         = 0xc0, /**< Occupancy - Parking sensor */
}
zb_zgpd_manuf_specific_dev_id_t;

/**
   @}
*/

/********************************************************************/
/*********************** Sink definitions ***************************/
/********************************************************************/
#if defined ZB_ENABLE_ZGP_SINK || defined DOXYGEN
/**
   @addtogroup zgp_sink
   @{
*/
/**
 * @brief Mapping of ZGPD command ID to Zigbee ZCL command ID
 */
typedef struct zgp_to_zb_cmd_mapping_s
{
  zb_uint8_t zgp_cmd_id;  /**< ZGPD command ID */
  zb_uint8_t zb_cmd_id;   /**< Zigbee ZCL command ID */
}
zgp_to_zb_cmd_mapping_t;

typedef struct zgps_cluster_rec_s
{
  zb_uint16_t cluster_id;   /** Cluster ID to which specified ZGPD commands are translated
                                (see @ref zcl_cluster_id) */
/**
 * Options field of cluster table entry
 *
 * [0-1]        role mask          client/server/both.
 *                                 Note: this role specifies the original cluster role, not
 *                                 the cluster role to which this command will be addressed.
 *                                 E.g. for On/Off/Toggle commands it should be client role
 *                                 (these command send from client to server).
 *
 */
  zb_uint8_t  options;
  zb_uint8_t  cmd_ids[ZB_ZGP_MATCH_TBL_MAX_CMDS_FOR_MATCH]; /**< Supported commands by sink */
}
zgps_dev_cluster_rec_t;

/** @cond DOXYGEN_INTERNAL_DOC */
#define GET_CLUSTER_ROLE(cluster) \
  (cluster->options & 0x03U)

typedef ZB_PACKED_PRE union zgps_device_id_u
{
  /* dev_id.zgpd_dev_id matches with ZGPD Device ID from Commissioning frame @see zb_zgpd_dev_id_t */
  zb_uint8_t  zgpd_dev_id;
  /* match with app_info.manuf_model_id. */
  zb_uint16_t	zgpd_manuf_model;
}
zgps_device_id_t;

typedef ZB_PACKED_PRE struct zgps_dev_match_rec_s
{
  /* Cluster idxes possible for that device id. Not used slots must be filled by ZB_ZCL_CLUSTER_IDX_UNDEFINED.
     Cluster idx - index in zgps_dev_cluster_rec_t clusters_tbl[].
   */
  zb_uint16_t		clusters[ZB_ZGP_TBL_MAX_CLUSTERS];
  zb_uint16_t           manuf_id;
  /* match with device info from Commissioning frame:
     if zgpd_dev_id != ZB_ZGP_MANUF_SPECIFIC_DEV_ID, match by GPD Device id
     if zgpd_dev_id == ZB_ZGP_MANUF_SPECIFIC_DEV_ID, match by app_info.manuf_model_id
  */
  zgps_device_id_t 	dev_id;
}
ZB_PACKED_STRUCT zgps_dev_match_rec_t;

#define IS_STANDART_ZGPS_DEVICE(dev_match_rec) \
  (dev_match_rec->manuf_id == ZB_ZGPD_MANUF_ID_UNSPEC)
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */


/*
  Using of match table.

  Match table is a static const data declared in the application.

  During GPD commissioning, using information from Commissiponing frame, ZBOSS seeks for matched entry in match_tbl.
  Match is done by device id or model id - see calls to zb_zgps_get_dev_matching_tbl_index() or zb_zgps_get_ms_dev_matching_tbl_index()
  Entry index is written into the Sink table.

  Command translation (without details about attr reporting):
  - get Sink table entry by GPD address
  - get matxh_tbl entry by index in Sink table.
  - scan entire match_tbl[]: use match_tbl[i].clusters as an index in clusters_tbl.
  - in each clusters_tbl entry scan clusters_tbl[i].cmd_ids[] for matching GPD command
  - seek for appropriate clister in out local Simple desc (decide which EP to map to)
  - map GPD command to ZCL command by scanning cmd_mapping[]
 */

/**
 * @brief Necessary information for filling translation table for any ZGPD
 *        during commissioning
 *
 * Includes matching table and command ID mappings. Given some ZGPD device ID it is
 * possible to fill translation table entry with help of this structure.
 */
typedef struct zb_zgps_match_info_s
{
  const zb_uint8_t                       match_tbl_size;
  /* clusters list to be matched by device id or manufacturer id got from Commissioning frame. */
  const ZB_CODE zgps_dev_match_rec_t    *match_tbl;
  const zb_uint8_t                       cmd_mappings_count;
  const ZB_CODE zgp_to_zb_cmd_mapping_t *cmd_mapping;
  const zb_uint8_t                       clusters_tbl_size;
  /* clusters table used to translate ZB_GPDF_CMD_ATTR_REPORT / ZB_GPDF_CMD_MANUF_SPEC_ATTR_REPORT */
  const ZB_CODE zgps_dev_cluster_rec_t  *clusters_tbl;
}
zb_zgps_match_info_t;
/** @} */ /* zgp_sink */
#endif  /* ZB_ENABLE_ZGP_SINK */

#ifdef ZB_ENABLE_ZGP_DIRECT
enum zb_zgp_data_handle_e
{
  ZB_ZGP_HANDLE_DEFAULT_HANDLE,
  ZB_ZGP_HANDLE_REMOVE_CHANNEL_CONFIG,
  ZB_ZGP_HANDLE_REMOVE_COMMISSIONING_REPLY,
  ZB_ZGP_HANDLE_ADD_COMMISSIONING_REPLY,
  ZB_ZGP_HANDLE_ADD_CHANNEL_CONFIG,
  ZB_ZGP_HANDLE_REMOVE_AFTER_FAILED_COMM,
  ZB_ZGP_HANDLE_REMOVE_BY_USER_REQ,

/**
 * The first handle that can be used by application for gp-data.req primitives.
 * Application may use any greater or equal handle value to match request with
 * confirmation.
 *
 * @see zb_zgps_send_data().
 */
  ZB_ZGP_HANDLE_APP_DATA,
};

#define ZB_GP_TX_QUEUE_ENTRY_LIFETIME_NONE ZB_MIN_TIME_VAL
#define ZB_GP_TX_QUEUE_ENTRY_LIFETIME_INF  ZB_MAX_TIME_VAL

#define ZB_GP_DATA_REQ_USE_GP_TX_QUEUE   0x01U
#define ZB_GP_DATA_REQ_USE_CSMA_CA_BIT   0x02U
#define ZB_GP_DATA_REQ_USE_MAC_ACK_BIT   0x04U
#define ZB_GP_DATA_REQ_MAINT_FRAME_TYPE  0x08U

#define ZB_GP_DATA_REQ_FRAME_TYPE(tx_opt) \
  ((tx_opt >> 3) & 0x03)

#define ZB_GP_DATA_REQ_ACTION_REMOVE_GPDF ZB_FALSE
#define ZB_GP_DATA_REQ_ACTION_ADD_GPDF    ZB_TRUE

#define ZB_CGP_DATA_REQ_USE_CSMA_CA_BIT 0x01
#define ZB_CGP_DATA_REQ_USE_MAC_ACK_BIT 0X02

#endif  /* ZB_ENABLE_ZGP_DIRECT */

/********************************************************************/
/*********************** Proxy definitions **************************/
/********************************************************************/

/**
   @addtogroup zgp_common
   @{
*/

/** Values of gpsCommunicationMode attribute
 *  Table 27
*/
typedef enum zgp_communication_mode_e
{
  ZGP_COMMUNICATION_MODE_FULL_UNICAST = 0,
  /*Groupcast - one of the communication modes used for tunneling GPD commands between the
  proxies and sinks. In Zigbee terms, it is the APS level multicast, with NWK level broadcast to the
  RxOnWhenIdle=TRUE (0xfffd) broadcast address.*/
  ZGP_COMMUNICATION_MODE_GROUPCAST_DERIVED = 1,
  ZGP_COMMUNICATION_MODE_GROUPCAST_PRECOMMISSIONED = 2,
  ZGP_COMMUNICATION_MODE_LIGHTWEIGHT_UNICAST = 3
} zgp_communication_mode_t;

/* A.3.3.2.4 gpsCommissioningExitMode attribute */
typedef enum zgp_commissioning_exit_mode_e
{
  ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION  = (1<<0),
  ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS                  = (1<<1),
  ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT = (1<<2),
  ZGP_COMMISSIONING_EXIT_MODE_ON_CWE_OR_PS                        = (ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS),
  ZGP_COMMISSIONING_EXIT_MODE_ON_CWE_OR_PCM                       = (ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT),
  ZGP_COMMISSIONING_EXIT_MODE_ALL                                 = (ZGP_COMMISSIONING_EXIT_MODE_ON_COMMISSIONING_WINDOW_EXPIRATION |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_PAIRING_SUCCESS |
                                                                     ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT)
} zgp_commissioning_exit_mode_t;

/*! @} */

/********************************************************************/
/******************** Commissioning definitions *********************/
/********************************************************************/

/**
   @addtogroup zgp_common
   @{
*/

/**
 * @brief Possible commissioning result
 */
typedef enum zb_zgp_comm_status_e
{
  /** Commissioning with some device completed successfully */
  ZB_ZGP_COMMISSIONING_COMPLETED,
  /** Commissioning failed. The reason can be:
   * - parameters of connection can't be negotiated
   * - ZGPD device behaves incorrectly
   * - Maximum number of connected ZGPDs is reached */
  ZB_ZGP_COMMISSIONING_FAILED,
  /** Commissioning failed, because of timeout */
  ZB_ZGP_COMMISSIONING_TIMED_OUT,
  /** No functionality match with commissioning device is found.
   *  Maybe matching table is not provided by user application */
  ZB_ZGP_COMMISSIONING_NO_MATCH_ERROR,
  /** Commissioning failed, because some internal error occurred in stack.
   * This type of error is recoverable, so next commissioning attempt can
   * be successful */
  ZB_ZGP_COMMISSIONING_INTERNAL_ERROR,
  /** Commissioning failed, because some critical error has occurred.
   * Normal functioning of ZGP subsystem is not possible
   * (e.g. physical operational channel can't be changed) */
  ZB_ZGP_COMMISSIONING_CRITICAL_ERROR,
  /** User cancelled commissioning by calling zb_zgps_stop_commissioning() */
  ZB_ZGP_COMMISSIONING_CANCELLED_BY_USER,
  /** ZGPD sent Decommissioning command */
  ZB_ZGP_ZGPD_DECOMMISSIONED,
}
zb_zgp_comm_status_t;

#if defined ZB_ENABLE_ZGP_SINK || defined DOXYGEN
/** @cond DOXYGEN_INTERNAL_DOC */
/**
 * @brief Commissioning callback type
 *
 * @note this is legacy API. Use ZB_ZGP_SIGNAL_COMMISSIONING signal passed to
 * zboss_signal_handler instead!
 *
 * Commissioning callback notifies user about commissioning complete. So, ZGP
 * device is in the operational mode at the time of calling this callback.
 * Result variable carries the status of commissioning: whether some device
 * successfully commissioned or not.
 */
typedef void (ZB_CODE * zb_zgp_comm_completed_cb_t)(
    zb_zgpd_id_t *zgpd_id,
    zb_zgp_comm_status_t result);
/** @endcond */

/**
 * @brief Commissioning request callback type
 *
 * Commissioning request callback notifies application about commissioning
 * attempt from ZGPD. Callback is called when ZGPD sends commissioning frame.
 * Callback provides to user ZGPD ID and ZGPD Device ID. Based on these
 * parameters user application should decide whether commissioning procedure
 * should be continued or not.
 *
 * Commissioning request callback should be set during ZGP initialization using
 * @ref ZB_ZGP_REGISTER_COMM_REQ_CB macro. If callback is not registered, then
 * ZBOSS accepts all incoming commissioning attempts.
 *
 * @param zgpd_id         [in]  ZGPD ID
 * @param device_id       [in]  ZGPD device ID
 * @param manuf_id        [in]  Manufacturer ID (meaningful if device_id = 0xFE or 0xFF)
 * @param manuf_model_id  [in]  Manufacturer model ID (meaningful if device_id = 0xFE or 0xFF)
 * @param ieee_addr       [in]  ZGPD long IEEE address if available, otherwise filled with zeroes
 *
 * @see zb_zgps_accept_commissioning
 * @see ZB_IS_64BIT_ADDR_ZERO
 */
typedef void (ZB_CODE * zb_zgp_comm_req_cb_t)(
    zb_zgpd_id_t    *zgpd_id,
    zb_uint8_t       device_id,
    zb_uint16_t      manuf_id,
    zb_uint16_t      manuf_model_id,
    zb_ieee_addr_t   ieee_addr);

/*! @} */

/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/**
 * @brief Application confirm callback type
 *
 * Application confirm callback notifies application about data
 * request attempt to ZGPD. Callback is called when data frame is sent
 * to ZGPD. Callback provides to command ID and its status. Based on these
 * parameters user application should decide further actions.
 *
 * Application confirm callback should be set during ZGP initialization using
 * @ref ZB_ZGP_REGISTER_APP_CFM_CB macro.
 *
 * @param cmd_id          [in]  Command ID
 * @param status          [in]  Confirmation status
 * @param data_ptr        [in]  Pointer to data payload
 * @param zgpd_id         [in]  ZGPD ID
 * @param handle          [in]  ZGP handle
 */
typedef void (ZB_CODE * zb_zgp_app_cfm_cb_t)(
  zb_uint8_t    cmd_id,
  zb_int16_t    status,
  zb_uint8_t   *data_ptr,
  zb_zgpd_id_t *zgpd_id,
  zb_uint8_t    handle);

/*! @} */
/*! @endcond */


/**
   @addtogroup zgp_sink
   @{
*/

  /**
   * @brief Application callback, indication of the attempted commissioning in GPS operational mode.
   *
   * Application commissioning indication callback notifies application about commissioning
   * attempt from ZGPD. Callback is called when commissioning frame or notification is received
   * from ZGPD. Callback provides to zgpd_id and full incoming packet's data. Based on these
   * parameters user application should decide further actions.
   *
   * Application commissioning indication callback should be set during ZGP initialization using
   * @ref ZB_ZGP_REGISTER_APP_CIC_CB macro.
   *
   * @param zgpd_id         [in]  ZGPD ID
   * @param param           [in]  buffer index, containing GPDF
   */
typedef void (ZB_CODE * zb_zgp_app_comm_ind_cb_t)(
  zb_zgpd_id_t *zgpd_id,
  zb_uint8_t    param);


/*! @} */

/**
   @addtogroup zgp_sink
   @{
*/

/** @cond DOXYGEN_INTERNAL_DOC */
void zb_zgps_set_match_info(const zb_zgps_match_info_t *info);
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/**
 * @ingroup zgp_sink
 * @brief Set matching information that is used to fill ZGP command - ZCL
 * cluster translation table.
 * @param [in]  info  Matching information of type @ref zb_zgps_match_info_t
 */
#define ZB_ZGP_SET_MATCH_INFO(info)             \
{ \
  zb_zgps_set_match_info((info));               \
}

/** @cond DOXYGEN_INTERNAL_DOC */
void zb_zgps_register_comm_req_cb(zb_zgp_comm_req_cb_t cb);
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/**
 * @ingroup zgp_sink
 * @brief Register commissioning request callback
 *
 * @param cb [in]  Commissioning request callback (@ref zb_zgp_comm_req_cb_t)
 *
 * @if DOXIGEN_INTERNAL_DOC
 * @snippet tests/zgp/gppb/test_gps_decommissioning/dut_gps.c accept_comm
 * @endif
 */
#define ZB_ZGP_REGISTER_COMM_REQ_CB(cb)         \
{ \
  zb_zgps_register_comm_req_cb((cb)); \
}

/** @cond DOXYGEN_INTERNAL_DOC */
void zb_zgps_register_app_cic_cb(zb_zgp_app_comm_ind_cb_t cb);
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/**
 * @ingroup zgp_sink
 * @brief Register application commissioning indication callback
 *
 * @param cb [in]  Application commissioning indication callback (@ref zb_zgp_app_comm_ind_cb_t)
 */
#define ZB_ZGP_REGISTER_APP_CIC_CB(cb)          \
{ \
  zb_zgps_register_app_cic_cb((cb)); \
}

/*! @} */
/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/**
 * @brief Register application confirm callback
 *
 * @param cb [in]  Application confirm callback (@ref zb_zgp_app_cfm_cb_t)
 */
void zb_zgps_register_app_cfm_cb(zb_zgp_app_cfm_cb_t cb);
#define ZB_ZGP_REGISTER_APP_CFM_CB(cb) \
{ \
  zb_zgps_register_app_cfm_cb((cb)); \
}

/*! @} */
/*! @endcond */

/**
   @addtogroup zgp_common
   @{
*/
#endif  /* ZB_ENABLE_ZGP_SINK */

enum zb_zgpd_switch_type_e
{
  ZB_GPD_SWITCH_TYPE_BUTTON                        = 0x01, /* 0b01 */
  ZB_GPD_SWITCH_TYPE_ROCKER                        = 0x02, /* 0b10 */
};
/********************************************************************/
/********************* GPDF command IDs *****************************/
/********************************************************************/

/**
 * @brief Command identifiers sent from or to ZGPD */
enum zb_zgpd_cmd_id_e
{
  ZB_GPDF_CMD_RECALL_SCENE0                        = 0x10,
  ZB_GPDF_CMD_RECALL_SCENE1                        = 0x11,
  ZB_GPDF_CMD_RECALL_SCENE2                        = 0x12,
  ZB_GPDF_CMD_RECALL_SCENE3                        = 0x13,
  ZB_GPDF_CMD_RECALL_SCENE4                        = 0x14,
  ZB_GPDF_CMD_RECALL_SCENE5                        = 0x15,
  ZB_GPDF_CMD_RECALL_SCENE6                        = 0x16,
  ZB_GPDF_CMD_RECALL_SCENE7                        = 0x17,
  /* @note recall scene 8-11 are got from LCGW. But GPPB specification
   * does not define recall scenes 8-11. Only 8 scenes are
   * supported. See Table 49 Payloadless GPDF commands 6236 sent by
   * GPD */
  ZB_GPDF_CMD_RECALL_SCENE8                        = 0x18,
  ZB_GPDF_CMD_RECALL_SCENE9                        = 0x19,
  ZB_GPDF_CMD_RECALL_SCENE10                       = 0x1A,
  ZB_GPDF_CMD_RECALL_SCENE11                       = 0x1B,
#define ZB_GPDF_CMD_STORE_SCENE0                       ZB_GPDF_CMD_RECALL_SCENE8
#define ZB_GPDF_CMD_STORE_SCENE1                       ZB_GPDF_CMD_RECALL_SCENE9
#define ZB_GPDF_CMD_STORE_SCENE2                       ZB_GPDF_CMD_RECALL_SCENE10
#define ZB_GPDF_CMD_STORE_SCENE3                       ZB_GPDF_CMD_RECALL_SCENE11
  ZB_GPDF_CMD_STORE_SCENE4                        = 0x1C,
  ZB_GPDF_CMD_STORE_SCENE5                        = 0x1D,
  ZB_GPDF_CMD_STORE_SCENE6                        = 0x1E,
  ZB_GPDF_CMD_STORE_SCENE7                        = 0x1F,
  /* on/off */
  ZB_GPDF_CMD_OFF                                  = 0x20,
  ZB_GPDF_CMD_ON                                   = 0x21,
  ZB_GPDF_CMD_TOGGLE                               = 0x22,
  /* level control */
  ZB_GPDF_CMD_MOVE_UP                              = 0x30,
  ZB_GPDF_CMD_MOVE_DOWN                            = 0x31,
  ZB_GPDF_CMD_STEP_UP                              = 0x32,
  ZB_GPDF_CMD_STEP_DOWN                            = 0x33,
  ZB_GPDF_CMD_LC_STOP                              = 0x34,
#define ZB_GPDF_CMD_LVL_CTRL_STOP ZB_GPDF_CMD_LC_STOP
  ZB_GPDF_CMD_MOVE_UP_W_ONOFF                      = 0x35,
#define ZB_GPDF_CMD_MOVE_UP_WITH_ON_OFF ZB_GPDF_CMD_MOVE_UP_W_ONOFF
  ZB_GPDF_CMD_MOVE_DOWN_W_ONOFF                    = 0x36,
#define ZB_GPDF_CMD_MOVE_DOWN_WITH_ON_OFF ZB_GPDF_CMD_MOVE_DOWN_W_ONOFF
  ZB_GPDF_CMD_STEP_UP_W_ONOFF                      = 0x37,
#define ZB_GPDF_CMD_MOVE_STEP_ON ZB_GPDF_CMD_STEP_UP_W_ONOFF
  ZB_GPDF_CMD_STEP_DOWN_W_ONOFF                    = 0x38,
#define ZB_GPDF_CMD_MOVE_STEP_OFF ZB_GPDF_CMD_STEP_DOWN_W_ONOFF
  /* Color Control */
  ZB_GPDF_CMD_MOVE_HUE_STOP                        = 0x40,
  ZB_GPDF_CMD_MOVE_HUE_UP                          = 0x41,
  ZB_GPDF_CMD_MOVE_HUE_DOWN                        = 0x42,
  ZB_GPDF_CMD_STEP_HUE_UP                          = 0x43,
  ZB_GPDF_CMD_STEP_HUE_DOWN                        = 0x44,
  ZB_GPDF_CMD_MOVE_SATURATION_STOP                 = 0x45,
  ZB_GPDF_CMD_MOVE_SATURATION_UP                   = 0x46,
  ZB_GPDF_CMD_MOVE_SATURATION_DOWN                 = 0x47,
  ZB_GPDF_CMD_STEP_SATURATION_UP                   = 0x48,
  ZB_GPDF_CMD_STEP_SATURATION_DOWN                 = 0x49,
  ZB_GPDF_CMD_MOVE_COLOR                           = 0x4A,
  ZB_GPDF_CMD_STEP_COLOR                           = 0x4B,
  /* Simple Generic Switch */
  ZB_GPDF_CMD_PRESS_1_OF_1                         = 0x60,
  ZB_GPDF_CMD_RELEASE_1_OF_1                       = 0x61,
  ZB_GPDF_CMD_PRESS_1_OF_2                         = 0x62,
  ZB_GPDF_CMD_RELEASE_1_OF_2                       = 0X63,

  ZB_GPDF_CMD_8BIT_VECTOR_PRESS                    = 0X69,
  ZB_GPDF_CMD_8BIT_VECTOR_RELEASE                  = 0X6A,

  ZB_GPDF_CMD_ATTR_REPORT                          = 0xA0,
  ZB_GPDF_CMD_MANUF_SPEC_ATTR_REPORT               = 0xA1,
  ZB_GPDF_CMD_MULTI_CLUSTER_ATTR_REPORT            = 0xA2,
  ZB_GPDF_CMD_MANUF_SPEC_MULTI_CLUSTER_ATTR_REPORT = 0xA3,
  ZB_GPDF_CMD_REQUEST_ATTRIBUTES                   = 0xA4,
  ZB_GPDF_CMD_READ_ATTR_RESP                       = 0xA5,

  ZB_GPDF_CMD_ZCL_TUNNELING_FROM_ZGPD              = 0xA6,

  ZB_GPDF_CMD_COMPACT_ATTR_REPORTING               = 0xA8,
  /* Manufacturer-defined GPD commands (payload is manufacturer-specific) */
  ZB_GPDF_CMD_MANUF_DEFINED_B0                     = 0xB0,
  ZB_GPDF_CMD_MANUF_DEFINED_BF                     = 0xBF,
  /* commissioning from ZGPD */
  ZB_GPDF_CMD_COMMISSIONING                        = 0xE0,
  ZB_GPDF_CMD_DECOMMISSIONING                      = 0xE1,
  ZB_GPDF_CMD_SUCCESS                              = 0xE2,
  ZB_GPDF_CMD_CHANNEL_REQUEST                      = 0xE3,
  ZB_GPDF_CMD_APPLICATION_DESCR                    = 0xE4,

  /* GPDF commands sent to GPD */
  ZB_GPDF_CMD_COMMISSIONING_REPLY                  = 0xF0,
  ZB_GPDF_CMD_WRITE_ATTRIBUTES                     = 0xF1,
  ZB_GPDF_CMD_READ_ATTRIBUTES                      = 0xF2,
  ZB_GPDF_CMD_CHANNEL_CONFIGURATION                = 0xF3,

  ZB_GPDF_CMD_ZCL_TUNNELING_TO_ZGPD                = 0xF6,
};

/*! @} */

/********************************************************************/
/**************** Macros for sending GPDF packets *******************/
/********************************************************************/

/**
   @cond internal
   @addtogroup zgp_internal
   @{
*/


/**
 * @brief Start constructing GPDF packet
 *
 * @param [in] zbbuf Buffer for GPDF packet
 */
#define ZB_START_GPDF_PACKET(zbbuf)                    \
   zb_buf_reuse(zbbuf)

/**
 * @brief Put 1 byte into GPDF packet
 *
 * @param ptr [in] Destination memory address, where val should be copied
 * @param val [in] Pointer to 1-byte value to be put in packet
 */
#define ZB_GPDF_PUT_UINT8(ptr, val) \
  ( *(ptr) = (val), (ptr)++ )

/**
 * @brief Put 2 bytes into GPDF packet
 *
 * @param ptr [in] Destination memory address, where val should be copied
 * @param val [in] Pointer to 2-byte value to be put in packet
 */
#define ZB_GPDF_PUT_UINT16(ptr, val) \
  ( ZB_HTOLE16((ptr), (val)), (ptr) += 2 )


/**
 * @brief Put 4 bytes into GPDF packet
 *
 * @param ptr [in] Destination memory address, where val should be copied
 * @param val [in] Pointer to 4-byte value to be put in packet
 */
#define ZB_GPDF_PUT_UINT32(ptr, val) \
  ( ZB_HTOLE32((ptr), (val)), (ptr) += 4 )


/**
 * @brief Finish constructing GPDF frame
 *
 * @param zbbuf [in] Buffer with GPDF
 * @param ptr   [in] Pointer to the GPDF tail
 */
void zb_finish_gpdf_packet(zb_bufid_t buf_ref, zb_uint8_t** ptr);
#define ZB_FINISH_GPDF_PACKET(zbbuf, ptr) zb_finish_gpdf_packet(zbbuf, &ptr)


/**
 * @brief RX channel in the next attempt parameter of ZGPD Channel request command
 * @see ZGP spec, A.4.2.1.4
 */
#define ZB_GPDF_CHANNEL_REQ_NEXT_RX_CHANNEL(par) \
  ((par) & 0x0FU)


typedef struct zb_gpdf_comm_app_info_options_s
{
  zb_bitfield_t manuf_id_present:1;
  zb_bitfield_t manuf_model_id_present:1;
  zb_bitfield_t gpd_cmds_present:1;
  zb_bitfield_t cluster_list_present:1;
  zb_bitfield_t switch_info_present:1;
  zb_bitfield_t app_descr_flw:1;
  zb_bitfield_t reserved:2;
}zb_gpdf_comm_app_info_options_t;

typedef ZB_PACKED_PRE struct zb_gpdf_comm_switch_gen_cfg_s
{
  zb_bitfield_t num_of_contacts:4;
  zb_bitfield_t switch_type:2;
  zb_bitfield_t reserved:2;
}ZB_PACKED_STRUCT zb_gpdf_comm_switch_gen_cfg_t;

/* DEPRECATED: Typo in structure field was fixes -
 * old name, with the typo, will be removed in the next Major release */
#define num_of_contacs num_of_contacts

typedef struct zb_gpdf_comm_switch_info_s
{
  zb_uint8_t    len;                       /**< Length of switch info */
  zb_gpdf_comm_switch_gen_cfg_t configuration;
  zb_uint8_t    current_contact_status;
}zb_gpdf_comm_switch_info_t;

typedef struct zb_gpdf_comm_app_info_s
{
  zb_gpdf_comm_app_info_options_t options;

  zb_uint16_t   manuf_id;                  /**< Manufacturer ID */
  zb_uint16_t   manuf_model_id;            /**< Manufacturer model ID */

  zb_uint8_t    gpd_cmds_len;              /**< Number of GPD commands */
  /* ToDo: implement GPD commands list */

  zb_bitfield_t srv_cluster_num:4;         /**< Number of server clusterIDs */
  zb_bitfield_t client_cluster_num:4;      /**< Number of client clusterIDs */
  /* ToDo: implement server/client cluster list */

  zb_gpdf_comm_switch_info_t switch_info;
}zb_gpdf_comm_app_info_t;

/**
 * @brief ZGPD Commissioning command parameters
 * @see ZGP spec, A.4.2.1.1
 */
typedef struct zb_gpdf_comm_params_s
{
  zb_uint8_t zgpd_device_id;    /**< ZGPD Device ID */
  zb_uint8_t options;           /**< Options */
  zb_uint8_t ext_options;       /**< Extended options */
  zb_gpdf_comm_app_info_t app_info; /**< Application information */

  /* TODO: Add fields "Number of GP commands", "GP command ID list",
   * "Number of cluster reports", "ClusterReportN" */
}
zb_gpdf_comm_params_t;

/**
 * @brief ZGPD Commissioning reply parameters
 * @see ZGP spec, A.4.2.1.1
 */
typedef struct zb_gpdf_comm_reply_s
{
  zb_uint8_t     options;                       /**< Options */
  zb_uint16_t    pan_id;                        /**< Pan ID if requested */
  zb_uint8_t     security_key[ZB_CCM_KEY_SIZE]; /**< ZGPD key */
  zb_uint8_t     key_mic[ZB_CCM_M];             /**< ZGPD key MIC */
  zb_uint32_t    frame_counter;                 /**< ZGPD key encryption counter */
}
zb_gpdf_comm_reply_t;

#define ZB_GPDF_COMM_REPLY_PAN_ID_PRESENT(options) ((options) & 0x01U)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_PRESENT(options) (((options) >> 1U) & 0x01U)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_ENCRYPTED(options) (((options) >> 2U) & 0x01U)
#define ZB_GPDF_COMM_REPLY_SEC_LEVEL(options) (((options) >> 3U) & 0x03U)
#define ZB_GPDF_COMM_REPLY_SEC_KEY_TYPE(options) (((options) >> 5U) & 0x07U)

/**
 * @brief Construct options field of commissioning command from given values
 * @see ZGP spec, A.4.2.1.1.2
 */
#define ZB_GPDF_COMM_OPT_FLD(sn_cap, rx_cap, ms_ext, pan_id_req, \
    sec_key_req, fixed_loc, ext_opt) \
  (   (sn_cap) \
   | ((rx_cap)      << 1U) \
   | ((ms_ext)      << 2U) \
   | ((pan_id_req)  << 4U) \
   | ((sec_key_req) << 5U) \
   | ((fixed_loc)   << 6U) \
   | ((ext_opt)     << 7U) )

/**
 * @brief Value of Extended Options bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_EXT_OPT_PRESENT(options) \
  ((options) >> 7U)

/**
 * @brief Value of GP security Key request bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEC_KEY_REQ(options) \
  (((options) >> 5U) & 0x01U)

/**
 * @brief Value of Pan ID request bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_PAN_ID_REQ(options) \
  (((options) >> 4U) & 0x01U)

/**
 * @brief Value of MAC sequence number capability bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEQ_NUM_CAPS(options) \
  ((options) & 0x01U)

/**
 * @brief Value of RxOnCapability bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_RX_CAPABILITY(options) \
  (((options) >> 1U) & 0x01U)

/**
 * @brief Value of Fixed location bit in
 * options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_FIX_LOC(options) \
  (((options) >> 6U) & 0x01U)

/**
 * @brief Value of GPD MS extensions present bit in
 * Options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_APP_INF_PRESENT(options) \
  (((options) >> 2U) & 0x01U)

/**
 * @brief Construct Extended Options field of commissioning command
 * @see ZGP spec, A.4.2.1.1.3
 */
#define ZB_GPDF_COMM_EXT_OPT_FLD(sec_cap, key_type, key_present, \
    key_enc, out_counter) \
  (   (sec_cap) \
   | ((key_type)    << 2U) \
   | ((key_present) << 5U) \
   | ((key_enc)     << 6U) \
   | ((out_counter) << 7U))

/**
 * @brief Value of GPD Key present bit in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_ZGPD_KEY_PRESENT(ext_options) \
  (((ext_options) >> 5U) & 0x01U)

/**
 * @brief Value of GPD Key encryption bit in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_ZGPD_KEY_ENCRYPTED(ext_options) \
  (((ext_options) >> 6U) & 0x01U)

/**
 * @brief Value of GPD outgoing counter present bit in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_ZGPD_OUT_COUNTER_PRESENT(ext_options) \
  ((ext_options) >> 7U)

/**
 * @brief Value of SecurityLevel capabilities bits in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEC_LEVEL_CAPS(ext_options) \
  ((ext_options) & 0x03U)

/**
 * @brief Value of SecurityKey type bits in
 * Extended options field of commissioning command
 */
#define ZB_GPDF_COMM_OPT_SEC_KEY_TYPE(ext_options) \
  ((ext_options >> 2U) & 0x07U)

/**
 * @brief Construct MS extensions field of commissioning command
 * @see docs-13-0146-04-batt-green-powermanufacturer-specific-device-type-command.docx
 */
#define ZB_GPDF_COMM_MS_EXT_FLD(_manuf_id, _manuf_model_id, _gp_cmd_list, _clstr_reports) \
  (   (_manuf_id) \
   | ((_manuf_model_id) << 1U) \
   | ((_gp_cmd_list)    << 2U) \
   | ((_clstr_reports)  << 3U))

/**
 * @brief Construct Options field of commissioning reply command
 * @see ZGP spec, A.4.2.1.2.1
 */
#define ZB_GPDF_COMM_REPLY_OPT_FLD(pan_id_present, key_present, \
    key_enc, sec_level, key_type) \
  (   (pan_id_present) \
   | ((key_present) << 1U) \
   | ((key_enc)     << 2U) \
   | ((sec_level)   << 3U) \
   | ((key_type)    << 5U))

/** @} */
/*! @endcond */


/**
   @cond internals_doc
   @addtogroup zgp_internal
   @{
*/

/**
 * @brief Send commissioning GPDF with ZGPD securityLevel capabilities set to 0
 */
#define ZB_SEND_COMMISSIONING_GPDF_WITHOUT_SEC(buf, device_id, sn_cap, rx_cap) \
{ \
  zb_uint8_t* ptr = (zb_uint8_t*)ZB_START_GPDF_PACKET(buf); \
  (void)zb_buf_alloc_left(buf, 3U, ptr); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_CMD_COMMISSIONING); \
  ZB_GPDF_PUT_UINT8(ptr, device_id); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_COMM_OPT_FLD(sn_cap, rx_cap, 0U, 0U, 0U, 0U, 0U)); \
  ZB_SEND_GPDF_CMD(ZB_REF_FROM_BUF(buf)); \
}

/**
 * @brief Attribute report field of attribute reporting command
 * @see ZGP spec, A.4.2.3.1
 */
typedef struct zb_gpdf_attr_report_fld_s
{
  zb_uint16_t attr_id;   /**< Attribute ID specific to cluster */
  zb_uint8_t attr_type;  /**< Attribute type (see @ref zcl_attr_type) */
  void* data_p;     /**< Attribute data */
}
zb_gpdf_attr_report_fld_t;

/**
 * @brief Start constructing ZGPD attribute reporting command
 *
 * @param buf        [in]  Buffer for GPDF command
 * @param cluster_id [in]  Cluster ID of attributes being reported
 * @param ptr        [out] Pointer to the current tail of GPDF
 */
#define ZB_ZGPD_ATTR_REPORTING_CMD_INIT(buf, cluster_id, ptr) \
{ \
  ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_CMD_ATTR_REPORT); \
  ZB_GPDF_PUT_UINT16(ptr, &cluster_id); \
}

/**
 * @brief Put attribute report field into attribute reporting command
 *
 * Macro should be called only after attribute reporting command is
 * initialized with @ref ZB_ZGPD_ATTR_REPORTING_CMD_INIT
 * @param ptr   [in,out]  Pointer to the tail of attribute reporting command
 * @param attr  [in]      Attribute report field (see @zb_gpdf_attr_report_fld_t)
 */
#define ZB_ZGPD_ATTR_REPORTING_CMD_NEXT(ptr, attr) \
{ \
  ZB_GPDF_PUT_UINT16(ptr, &attr.attr_id); \
  ZB_GPDF_PUT_UINT8(ptr, attr.attr_type); \
  ptr = zb_zcl_put_value_to_packet(ptr, attr.attr_type, (zb_uint8_t *)attr.data_p); \
}

/**
 * @brief Finish constructing ZGPD attribute reporting command
 *
 * @param buf        [in] Buffer for GPDF command
 * @param ptr        [in] Pointer to the tail of GPDF
 */
#define ZB_ZGPD_ATTR_REPORTING_CMD_FINISH(buf, ptr) \
{ \
  ZB_FINISH_GPDF_PACKET(buf, ptr);	\
}

/**
 * @brief Value of multi-record bit of options field
 *        in ZGPD Request attributes or Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_IS_MULTI_RECORD(opts) \
  (opts & 0x01U)

/**
 * @brief Value of multi-record bit of options field
 *        in ZGPD Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_WRITE_ATTR_IS_MULTI_RECORD \
  ZB_GPDF_REQUEST_ATTR_IS_MULTI_RECORD

/**
 * @brief Value of "manufacturer field present" bit of options field
 *        in ZGPD Request attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_MANUF_FIELD_PRESENT(opts) \
  ((opts >> 1U) & 0x01U)

/**
 * @brief Value of "manufacturer field present" bit of options field
 *        in ZGPD Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_WRITE_ATTR_MANUF_FIELD_PRESENT \
  ZB_GPDF_REQUEST_ATTR_MANUF_FIELD_PRESENT

/**
 * @brief Construct value of options field
 *        in ZGPD Request attributes or Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.1)
 */
#define ZB_GPDF_REQUEST_ATTR_OPTIONS_FLD(multi_record, manuf_present) \
  ((multi_record) | (manuf_present << 1U))

/**
 * @brief Construct value of options field
 *        in ZGPD Write attributes command
 *        (ZGP spec, rev. 26 A.4.2.6.3)
 */
#define ZB_GPDF_WRITE_ATTR_OPTIONS_FLD \
  ZB_GPDF_REQUEST_ATTR_OPTIONS_FLD

/**
 * @brief Parse one Cluster Record request field of
 * ZGPD Request attributes command
 * @param rec         pointer to record to parse of type zb_uint8_t*
 * @param cluster_id  Cluster ID (out)
 * @param attr_count  Attribute count (out)
 * @param attrs  Pointer to attributes list (out)
 */
#define ZB_GPDF_REQUEST_ATTR_PARSE_RECORD(rec, cluster_id, attr_count, attrs) \
{ \
  ZB_LETOH16((cluster_id), (rec)); \
  *(attr_count) = (rec)[2]/sizeof(zb_uint16_t); \
  *(attrs) = (zb_uint16_t *)(void *)(&(rec)[3]); \
}

/**
 * @brief Start constructing ZGP write attribute command
 *
 * @param buf        [in]  Buffer for GPDF command
 * @param cluster_id [in]  Cluster ID of attributes being reported
 * @param ptr        [out] Pointer to the current tail of GPDF
 */
#define ZB_ZGPD_WRITE_ATTR_CMD_INIT(buf, ptr) \
{ \
  ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_WRITE_ATTR_OPTIONS_FLD(0,0)); \
}

/**
 * @brief Start constructing ZGP write attribute command
 *
 * @param buf        [in]  Buffer for GPDF command
 * @param manuf_id   [in]  Manufacturer ID
 * @param ptr        [out] Pointer to the current tail of GPDF
 */
#define ZB_ZGPD_WRITE_ATTR_MANUF_CMD_INIT(buf, manuf_id, ptr) \
{ \
  ptr = ZB_START_GPDF_PACKET(buf); \
  ZB_GPDF_PUT_UINT8(ptr, ZB_GPDF_WRITE_ATTR_OPTIONS_FLD(0,1)); \
  ZB_GPDF_PUT_UINT16(ptr, &(manuf_id)); \
}

/**
 * @brief Start cluster record field in write attributes command
 *
 * Macro should be called only after write attributes command is
 * initialized with @ref ZB_ZGPD_WRITE_ATTR_MANUF_CMD_INIT or
 * @ref ZB_ZGPD_WRITE_ATTR_CMD_INIT
 * @param ptr             [in,out]  Pointer to the tail of write attributes command
 * @param cluster_id      [in]      Cluster ID
 * @param record_list_len [in]      Length in bytes of following Write Attribute Records list
 */
#define ZB_ZGPD_WRITE_ATTR_NEW_CLUSTER_RECORD(ptr, cluster_id, record_list_len) \
{ \
  ZB_GPDF_PUT_UINT16(ptr, &cluster_id); \
  ZB_GPDF_PUT_UINT8(ptr, (record_list_len)); \
}

/**
 * @brief Put write attribute record into write attributes command
 *
 * Macro should be called only after new cluster record is
 * initialized with @ref ZB_ZGPD_WRITE_ATTR_NEXT_CLUSTER_RECORD
 * @param ptr       [in,out]  Pointer to the tail of write attributes command
 * @param attr_id   [in]      Attribute ID
 * @param attr_type [in]      Attribute type
 * @param value     [in]      Pointer to attribute value
 */
#define ZB_ZGPD_WRITE_ATTR_CMD_NEXT_ATTR(ptr, attr_id, attr_type, value) \
{ \
  ZB_GPDF_PUT_UINT16(ptr, &attr_id); \
  ZB_GPDF_PUT_UINT8(ptr, attr_type); \
  ptr = zb_zcl_put_value_to_packet(ptr, attr_type, (zb_uint8_t *)(value)); \
}

/**
 * @brief Finish constructing ZGPD write attributes command
 *
 * @param buf        [in] Buffer for GPDF command
 * @param ptr        [in] Pointer to the tail of GPDF
 */
#define ZB_ZGPD_WRITE_ATTR_CMD_FINISH(buf, ptr) \
{ \
  ZB_FINISH_GPDF_PACKET(buf, ptr);	\
}

/* [AV] tmm it'll be better to name macros like this once
   and don't rename them like #def macro_write_init(...) macro_read_init(...),
   because there is no difference
*/
#define ZB_ZGPD_CLUSTER_CMD_INIT(_buf, _ptr) \
  ZB_ZGPD_WRITE_ATTR_CMD_INIT(_buf, _ptr)
#define ZB_ZGPD_CLUSTER_CMD_MANUF_INIT(_buf, _manuf_id, _ptr)	\
  ZB_ZGPD_WRITE_ATTR_MANUF_CMD_INIT(_buf, _manuf_id, _ptr)
#define ZB_ZGPD_NEW_CLUSTER_RECORD(_ptr, _cluster_id, _record_list_len) \
  ZB_ZGPD_WRITE_ATTR_NEW_CLUSTER_RECORD(_ptr, _cluster_id, _record_list_len)
#define ZB_ZGPD_READ_ATTR_CMD_NEXT_ATTR(_ptr, _attr_id) \
{ \
  ZB_GPDF_PUT_UINT16(_ptr, &_attr_id); \
}
#define ZB_ZGPD_CLUSTER_CMD_FINISH(_buf, _ptr) \
{ \
  ZB_FINISH_GPDF_PACKET(_buf, _ptr); \
}

#ifdef ZB_ENABLE_ZGP_DIRECT
typedef struct zb_zgps_send_cmd_params_s
{
  zb_uint8_t     cmd_id;
  zb_zgpd_id_t   zgpd_id;
  zb_ieee_addr_t ieee_addr;
  zb_time_t      lifetime;
  zb_uint8_t     tx_options;
  zb_uint8_t     handle;
}
zb_zgps_send_cmd_params_t;

/**
 * @brief Send provided packet to ZGPD
 *
 * Buffer data is command payload to send.
 * Other parameters are in the buffer tail (see @ref zb_zgps_send_cmd_params_t).
 *
 * @param param[in, out]    Reference to buffer.
 *
 * @note maximum length of data payload is @ref ZB_ZGP_TX_CMD_PLD_MAX_SIZE
 *
 * @note zb_gp_data_cfm is called from:
 *  - gp_data_req_send_cnf       to notify about status of adding data to tx_packet_info_queue;
 *  - notify_about_expired_entry to notify about expired entry;
 *  - zb_cgp_data_cfm            to notify about status from MAC layer.
 *
 * @note Status of confirm (ZGP TX queue is used) can be:
 *      ZB_ZGP_STATUS_ENTRY_REPLACED
 *      ZB_ZGP_STATUS_ENTRY_ADDED
 *      ZB_ZGP_STATUS_ENTRY_EXPIRED
 *      ZB_ZGP_STATUS_ENTRY_REMOVED
 *      ZB_ZGP_STATUS_TX_QUEUE_FULL
 *
 *      MAC_SUCCESS
 *
 * @note Status of confirm (ZGP TX queue is not used) can be:
 *      ZB_ZGP_STATUS_TX_QUEUE_FULL
 *
 *      MAC_SUCCESS
 *      MAC_NO_ACK
 *
 */
void zb_zgps_send_data(zb_uint8_t param);
#endif  /* ZB_ENABLE_ZGP_DIRECT */
/*! @}
 *  @endcond */

/**
   @addtogroup zgp_sink
   @{
*/

/**
 * @brief Put ZGPS into commissioning mode
 *
 * It is safe to call this function when device is already in
 * commissioning mode. In this case function does nothing.
 *
 * @param[in] timeout       Maximum commissioning time in beacon intervals. \n
 *                      0 means no timeout. \n
 *                      If timeout occurs, then result of commissioning is
 *                      @ref ZB_ZGP_COMMISSIONING_TIMED_OUT
 *
 * @snippet light_sample/light_coordinator_combo/light_zc.c zgps_start_comm
 */
void zb_zgps_start_commissioning(zb_time_t timeout);

/**
 * @brief Switch ZGPS back to operational mode from commissioning
 *
 * @cond DOXYGEN_INTERNAL_DOC
 * After commissioning is cancelled, user is notified with
 * @ref zb_zgp_comm_completed_cb_t with ZB_ZGP_COMMISSIONING_CANCELLED_BY_USER
 * status.
 * @endcond
 *
 * @snippet light_sample/light_coordinator_combo/light_zc.c zgps_stop_comm
 */
void zb_zgps_stop_commissioning(void);

/**
 * @brief Accept/reject ZGPD commissioning attempt
 *
 * This function should be called as an answer to commissioning request made by
 * stack via @ref zb_zgp_comm_req_cb_t. Also, it can be called from @ref
 * zb_zgp_comm_req_cb_t callback as well as outside it.
 *
 * @param[in] accept - If ZB_TRUE, then stack will continue ongoing commissioning
 *                     process with ZGPD \n
 *                     Otherwise ongoing commissioning process will be
 *                     terminated
 * @if DOXIGEN_INTERNAL_DOC
 * @snippet tests/zgp/gppb/test_gps_decommissioning/dut_gps.c accept_comm
 * @endif
 */
void zb_zgps_accept_commissioning(zb_bool_t accept);

/**
 * @brief Remove all the information about ZGPD from stack
 *
 * In ZGP there is no way to say ZGPD to leave the network.
 * ZGPD can leave network by itself using "Decommissioning" command,
 * but ZGPS can miss this command if it was not in commissioning state.
 *
 * This function removes all the information related to specified ZGPD
 * from stack.
 *
 * @param buf_ref  reference to the free buffer
 * @param zgpd_id  identifier of ZGPD to be removed
 *
 * @note It is safe to free or overwrite memory pointed by zgpd_id
 *       after call
 */
void zb_zgps_delete_zgpd(zb_uint8_t buf_ref, zb_zgpd_id_t *zgpd_id);

/**
 * @brief Remove all the information about ALL ZGPD from stack
 *
 */
void zb_zgps_delete_all_zgpd(void);

/**
   Get LQI and RSSI last time received from that ZGPD.
 */
void zb_zgps_get_diag_data(zb_zgpd_id_t *zgpd_id, zb_uint8_t *lqi, zb_int8_t *rssi);

/*! @} */

/**
   @addtogroup zgp_sink
   @{
*/

/**
   Fill security level constant to be passed to zb_zgps_set_security_level()

   Described in A.3.3.2.6 gpsSecurityLevel attribute.

   @param sec_lvl  @ref zb_zgp_security_level_e Minimal GPD Security Level sub-field contains the minimum gpdSecurityLevel this sink accepts
   @param with_link_key 1 bit - Protection with the gpLinkKey sub-field, indicates if
   the GPDs attempting the pairing are required to support protecting the
   over-the-air exchange of the GPD Key
   @param involve_tc always zero for the current GPPB specification

   @snippet light_sample/light_coordinator_combo/light_zc.c zgps_set_secur_level
 */
#define ZB_ZGP_FILL_GPS_SECURITY_LEVEL(sec_lvl, with_link_key, involve_tc)\
  (((sec_lvl) & 3U) | ((!!(with_link_key)) << 2U) | ((!!(involve_tc)) << 3U))


/**
   Set gpsSecurityLevel GP cluster attribute of gpcb

   Described in A.3.3.2.6 gpsSecurityLevel attribute.

   @param level Security level to set

   @snippet light_sample/light_coordinator_combo/light_zc.c zgps_set_secur_level
  */
void zb_zgps_set_security_level(zb_uint_t level);

/**
   Set gpSharedSecurityKeyType GP cluster attribute

   Described in A.3.3.3.1 gpSharedSecurityKeyType attribute.

   @param key_type Security key type to set (@see zb_zgp_security_key_type_e)
  */
void zb_zgp_set_shared_security_key_type(zb_uint_t key_type);

/**
   Set gpSharedSecurityKey GP cluster attribute

   Described in A.3.3.3.1 gpSharedSecurityKey attribute.

   @param key Security key to set
  */
void zb_zgp_set_shared_security_key(zb_uint8_t *key);

/**
   Set gpsCommissioningExitMode GP cluster attribute

   Described in A.3.3.2.4 gpsCommissioningExitMode attribute.

   @param cem Commissioning exit mode to set (@see zgp_commissioning_exit_mode_t)
  */
void zb_zgps_set_commissioning_exit_mode(zb_uint_t cem);

/**
   Set gpsCommunicationMode GP cluster attribute of gpcb

   Described in A.3.3.2.3 gpsCommunicationMode attribute

   @param mode @ref zgp_communication_mode_t communication mode

   @snippet light_sample/light_coordinator_combo/light_zc.c set_comm_mode
  */
void zb_zgps_set_communication_mode(zgp_communication_mode_t mode);

/**
 * Application function to override translation of 8-bit vector command (generic switch)
 *
 * If this function is not implemented in the application, then ZBOSS
 * performs a default translation as recommended by ZGP spec (see Green Power
 * Basic specification v1.1.1, tables 51, 52). If there is no default
 * translation found, then the received command is dropped.
 *
 * If this function is implemented by the the application, then application is
 * fully responsible for a translation of GPD 8-bit vector commands. For any
 * return code but RET_OK, ZBOSS will stop command processing and drop it.
 *
 * Note: The translation is done to GPDF command ID, not to ZCL command ID.
 *
 * @param[in] vector_8bit_cmd_id incoming command ID: press (0x69) or release(0x6a)
 * @param[in] switch_type switch type of the command's originator (see ZGP spec. A.4.2.1.1.10)
 * @param[in] num_of_contacts number of contacts command's originator provides
 * @param[in] contact_status contacts status from the payload of the received command
 * @param[out] zgp_cmd_out GPDF command ID to which incoming command should be translated
 * @return RET_OK if translation is successful.
 *
 * See Green Power Basic specification v1.1.1, chapters A.3.6.2.2.2, A.4.2.2.1 for more information.
 *
 * @snippet simple_combo/zc_combo.c convert_8bit_vector
 */
zb_ret_t zb_zgp_convert_8bit_vector(zb_uint8_t vector_8bit_cmd_id,      /* press or release cmd */
                                    zb_uint8_t switch_type,             /* see zb_zgpd_switch_type_e */
                                    zb_uint8_t num_of_contacts,
                                    zb_uint8_t contact_status,
                                    zb_uint8_t *zgp_cmd_out);

#ifdef ZB_ENABLE_ZGP_DIRECT
/**
   Set ZBOSS to skip all incoming GPDF.

   To be used for testing only.
   Use that function with ZB_TRUE parameter to prevent Combo device from
   receiving GPDFS thus always working thru Proxy device.

   @param skip if ZB_TRUE, skip incoming GP frames
  */

void       zb_zgp_set_skip_gpdf(zb_uint8_t skip);
zb_uint8_t zb_zgp_get_skip_gpdf(void);
void       zb_zgp_sync_pib(zb_uint8_t param);

#endif  /* ZB_ENABLE_ZGP_DIRECT */

/*! @} */

#ifndef ZB_ZGPD_ROLE

typedef enum zgp_proxy_comm_mode_action_e
{
  ZGP_PROXY_COMM_MODE_LEAVE,
  ZGP_PROXY_COMM_MODE_ENTER
} zgp_proxy_comm_mode_action_t;

typedef enum zgp_proxy_comm_mode_ch_present_e
{
  ZGP_PROXY_COMM_MODE_CHANNEL_PRESENT,
  ZGP_PROXY_COMM_MODE_CHANNEL_NOT_PRESENT
} zgp_proxy_comm_mode_ch_present_t;

typedef enum zgp_proxy_comm_mode_communication_e
{
  ZGP_PROXY_COMM_MODE_BROADCAST,
  ZGP_PROXY_COMM_MODE_UNICAST
} zgp_proxy_comm_mode_communication_t;

/**
 * @brief Setup options for zb_zgp_cluster_proxy_commissioning_mode_req()
 *
 * @param action        [in] enter/exit (see @ref zgp_proxy_comm_mode_action_t)
 * @param exit_mode     [in] exit mode (see @ref zgp_commissioning_exit_mode_t)
 * @param ch_present    [in] is channel present (see @ref zgp_proxy_comm_mode_ch_present_t)
 * @param communication [in] communication type (see @ref zgp_proxy_comm_mode_communication_t)
 */
#define ZB_ZGP_FILL_PROXY_COMM_MODE_OPTIONS(action, exit_mode, ch_present, communication) \
  ((!!(action)) | (((exit_mode) & 7) << 1) | ((!!(ch_present)) << 4) | (((communication)&3) << 5))

/**
 * @brief Perform Proxy Commissioning mode request
 *
 * @param param      [in]  Buffer reference
 * @param options    [in]  Request options (see @ref ZB_ZGP_FILL_PROXY_COMM_MODE_OPTIONS)
 * @param comm_wind  [in]  Commissioning window
 * @param channel    [in]  Channel where perform commissioning (0x0b for channel 11)
 * @param cb         [in]  Callback call if needed after send request
 *
 * @see ZGP spec, A.3.3.5.3
*/
void zb_zgp_cluster_proxy_commissioning_mode_req(zb_uint8_t    param,
                                                 zb_uint8_t    options,
                                                 zb_uint16_t   comm_wind,
                                                 zb_uint8_t    channel,
                                                 zb_callback_t cb);

/**
 * @brief Perform Proxy Commissioning mode enter request
 *
 * @param param      [in]  Buffer reference
 * @param exit_mode  [in] exit mode (see @ref zgp_commissioning_exit_mode_t)
 * @param comm_wind  [in]  Commissioning window
 * @param cb         [in]  Callback call if needed after send request
 *
 * @see ZGP spec, A.3.3.5.3
 */
void zgp_cluster_send_proxy_commissioning_mode_enter_req(zb_uint8_t    param,
                                                         zb_uint8_t    exit_mode,
                                                         zb_uint16_t   comm_window,
                                                         zb_callback_t cb);

/**
 * @brief Perform Proxy Commissioning mode leave request
 *
 * @param param      [in]  Buffer reference
 * @param cb         [in]  Callback call if needed after send request
 *
 * @see ZGP spec, A.3.3.5.3
 */
void zgp_cluster_send_proxy_commissioning_mode_leave_req(zb_uint8_t param, zb_callback_t cb);

/**
 * @brief Setup options for zb_zgp_cluster_gp_pairing_req()
 *
 * @param app_id               [in]  ZGPD application id (@see zb_zgp_app_id_t)
 * @param add_sink             [in]  Add sink flag
 * @param remove_gpd           [in]  Remove GPD flag
 * @param comm_mode            [in]  Communication mode (@see zgp_communication_mode_t)
 * @param gpd_fixed            [in]  ZGPD is fixed
 * @param gpd_mac_sec_num_cap  [in]  MAC sequence number capability
 * @param sec_lvl              [in]  SecurityLevel (@see zb_zgp_security_level_e)
 * @param sec_key_type         [in]  SecurityKeyType (@see zb_zgp_security_key_type_e)
 * @param gpd_sec_key_prsnt    [in]  Is key presented
 * @param assigned_alias_prsnt [in]  Is assigned alias presented
 * @param frwrd_radius_prsnt   [in]  Is radius in the groupcast forwarding presented
 *
 * @see ZGP spec, A.3.3.5.2
 */
#define ZB_ZGP_FILL_GP_PAIRING_OPTIONS(app_id, add_sink, remove_gpd, comm_mode,\
                                       gpd_fixed, gpd_mac_sec_num_cap, sec_lvl,\
                                       sec_key_type, frame_cnt_prsnt,\
                                       gpd_sec_key_prsnt, assigned_alias_prsnt, \
                                       frwrd_radius_prsnt)\
  (((app_id) & 7) | ((!!(add_sink)) << 3) | ((!!(remove_gpd)) << 4) | (((comm_mode) & 3) << 5) | (((!!(add_sink)) ? (!!(gpd_fixed)) : 0) << 7) | (((!!(add_sink)) ? (!!(gpd_mac_sec_num_cap)) : 0) << 8) | (((!!(add_sink)) ? ((sec_lvl) & 3) : 0) << 9) | (((!!(add_sink)) ? ((sec_key_type) & 7) : 0) << 11) | ((!!(frame_cnt_prsnt)) << 14) | (((!!(add_sink)) ? (!!(gpd_sec_key_prsnt)) : 0) << 15) | (((!!(add_sink)) ? (!!(assigned_alias_prsnt)) : 0) << 16) | (((!!(add_sink)) ? (!!(frwrd_radius_prsnt)) : 0) << 17))

/**
 * @brief Perform GP Pairing request
 *
 * @param param              [in]  Buffer reference
 * @param dst_addr           [in]  Destination address
 * @param dst_addr_mode      [in]  Destination address mode
 * @param options            [in]  Request options
 * @param gpd_id             [in]  ZGPD ID
 * @param endpoint           [in]  Endpoint of ZGPD if APP_ID eq 0010
 * @param dev_id             [in]  Device ID
 * @param sec_frame_counter  [in]  Security frame counter of ZGPD
 * @param key                [in]  Pointer to the security key
 * @param assigned_alias     [in]  Assigned alias in the groupcast forwarding of the GPDF packet
 * @param frwd_radius        [in]  Radius in the groupcast forwarding of the GPDF packet
 * @param cb                 [in]  Callback call if needed after send request
 *
 * @see ZGP spec, A.3.3.5.2
 */
void zb_zgp_cluster_gp_pairing_req(zb_uint8_t     param,
                                   zb_uint16_t    dst_addr,
                                   zb_uint8_t     dst_addr_mode,
                                   zb_uint32_t    options,
                                   zb_zgpd_addr_t gpd_id,
                                   zb_uint8_t     endpoint,
                                   zb_uint8_t     dev_id,
                                   zb_uint32_t    sec_frame_counter,
                                   zb_uint8_t    *key,
                                   zb_uint16_t    assigned_alias,
                                   zb_uint8_t     frwd_radius,
                                   zb_uint16_t    group_id,
                                   zb_callback_t  cb);

/**
 * @brief Perform send zcl gp sink table request for ZGP cluster
 *
 * @param buf_ref        [in]  Buffer reference
 * @param dst_addr       [in]  Destination address
 * @param dst_addr_mode  [in]  Destination address mode
 * @param options        [in]  Request options
 * @param zgpd_id        [in]  Pointer to ZGPD ID
 * @param index          [in]  The Index field carries the index value of the Sink Table entry is requested
 * @param cb             [in]  Call callback if needed after sending request
 *
 * @see ZGP spec, A.3.3.4.7
 *
 */
void zgp_cluster_send_gp_sink_table_request(zb_uint8_t    buf_ref,
                                            zb_uint16_t   dst_addr,
                                            zb_uint8_t    dst_addr_mode,
                                            zb_uint8_t    options,
                                            zb_zgpd_id_t *zgpd_id,
                                            zb_uint8_t    index,
                                            zb_callback_t cb);

/**
 * @brief Perform send zcl gp proxy table request for ZGP cluster
 *
 * @param buf_ref        [in]  Buffer reference
 * @param dst_addr       [in]  Destination address
 * @param dst_addr_mode  [in]  Destination address mode
 * @param options        [in]  Request options
 * @param zgpd_id        [in]  Pointer to ZGPD ID
 * @param index          [in]  The Index field carries the index value of the Proxy Table entry is requested
 * @param cb             [in]  Call callback if needed after sending request
 *
 * @see ZGP spec, A.3.4.3.1
 *
 */
void zgp_cluster_send_gp_proxy_table_request(zb_uint8_t    buf_ref,
                                             zb_uint16_t   dst_addr,
                                             zb_uint8_t    dst_addr_mode,
                                             zb_uint8_t    options,
                                             zb_zgpd_id_t *zgpd_id,
                                             zb_uint8_t    index,
                                             zb_callback_t cb);

typedef ZB_PACKED_PRE struct zb_zgp_cluster_list_s
{
  zb_uint8_t server_cl_num;
  zb_uint8_t client_cl_num;
  zb_uint16_t server_clusters[ZB_ZGP_MAX_PAIRED_CONF_CLUSTERS];
  zb_uint16_t client_clusters[ZB_ZGP_MAX_PAIRED_CONF_CLUSTERS];
}
ZB_PACKED_STRUCT zb_zgp_cluster_list_t;

typedef ZB_PACKED_PRE struct zgp_pair_group_list_s
{
  zb_uint16_t sink_group;
  zb_uint16_t alias;
}
ZB_PACKED_STRUCT zgp_pair_group_list_t;

/* >> Data structures for Application Description */
typedef enum zgp_app_descr_status_e
{
  ZGP_APP_TBL_ENT_STATUS_FREE              = 0,
  ZGP_APP_TBL_ENT_STATUS_INIT_WITH_SW_INFO = 1,
  ZGP_APP_TBL_ENT_STATUS_INIT              = 2,
  ZGP_APP_TBL_ENT_STATUS_APP_DESCR_PROCESS = 3,
  ZGP_APP_TBL_ENT_STATUS_COMPLETE          = 4
} zgp_app_descr_status_t;

typedef ZB_PACKED_PRE struct zgp_app_tbl_ent_options_s
{
  zb_bitfield_t ieee_addr_present:1;
  zb_bitfield_t switch_info_present:1;
  zb_bitfield_t reserved:6;
}ZB_PACKED_STRUCT zgp_app_tbl_ent_options_t;

typedef ZB_PACKED_PRE struct zgp_app_tbl_ent_info_s
{
  zb_zgpd_addr_t                addr;
  zgp_app_tbl_ent_options_t     options;
  zb_uint8_t                    total_reports_num;
  zb_gpdf_comm_switch_gen_cfg_t switch_info_configuration;
  zb_uint8_t                    reserved;
}ZB_PACKED_STRUCT zgp_app_tbl_ent_info_t;


#define ZB_APP_DESCR_REPORTS_NUM 4
typedef ZB_PACKED_PRE struct zgp_app_tbl_ent_s
{
  zgp_app_tbl_ent_info_t info;
  zgp_report_desc_t      reports[ZB_APP_DESCR_REPORTS_NUM];
}ZB_PACKED_STRUCT zgp_app_tbl_ent_t;

typedef struct zgp_runtime_app_tbl_ent_s
{
  zb_uint8_t status; /* zgp_app_descr_status_t */
  zb_uint8_t receive_reports_num;
  zb_uint8_t reply_buf;
  zb_bool_t  need_reply;
  zgp_app_tbl_ent_t base;
}zgp_runtime_app_tbl_ent_t;

/* << Data structures for Application Description */

typedef struct zgp_tbl_ent_s
{
  zb_zgpd_addr_t   zgpd_id;             /**< ID of the paired ZGPD */
  zb_uint16_t      options;             /**< The options for the reception from ZGPD */
  zb_uint16_t      zgpd_assigned_alias; /**< The commissioned 16-bit ID to be used
                                          as alias for ZGPD */

  zb_uint32_t      security_counter; /**< The incoming security frame counter for ZGPD */
  zb_uint8_t       zgpd_key[ZB_CCM_KEY_SIZE]; /**< Security key for the GPD */
  zb_uint8_t       endpoint;                  /**< Endpoint pair of IEEE:EP if App ID is 010.  */
  zb_uint8_t       sec_options;               /**< Security options */
  zb_uint8_t       groupcast_radius;    /**< To limit the range of the groupcast */

  zb_uint8_t       is_sink;

  union
  {
    struct zgp_proxy_tbl_ent_s
    {
      zb_uint16_t ext_options;
      struct zgp_lwsink_addr_list_s
      {
        zb_address_ieee_ref_t addr_ref;
      } lwsaddr[ZB_ZGP_MAX_LW_UNICAST_ADDR_PER_GPD];
      zgp_pair_group_list_t sgrp[ZB_ZGP_MAX_SINK_GROUP_PER_GPD];
    } proxy;
    struct zgp_sink_tbl_ent_s
    {
      zb_uint8_t       device_id;           /**< ZGPD Device ID fot from Commissioning frame @see zb_zgpd_dev_id_t */
      zgp_pair_group_list_t sgrp[ZB_ZGP_MAX_SINK_GROUP_PER_GPD];
      zb_uint8_t match_dev_tbl_idx; /**< index in matching table matched by device_id or app_info.manuf_model_id  */
      /**
       * Extension to the table (field is not presented in specification).
       *
       * Dest IEEE address to use in GPT to GPD packets even if GPD is identified by SrcID
       */
      zb_ieee_addr_t   ieee_addr;
    } sink;
  } u;
} zgp_tbl_ent_t;

#if defined ZB_ENABLE_ZGP_SINK || defined ZGP_COMMISSIONING_TOOL
typedef zgp_tbl_ent_t zb_zgp_sink_tbl_ent_t;
#endif  /* defined ZB_ENABLE_ZGP_SINK || defined ZGP_COMMISSIONING_TOOL */
#ifdef ZB_ENABLE_ZGP_PROXY
typedef zgp_tbl_ent_t zb_zgp_proxy_tbl_ent_t;
#endif  /* ZB_ENABLE_ZGP_PROXY */

#if defined ZB_ENABLE_ZGP_SINK || defined  ZGP_COMMISSIONING_TOOL
/* TODO: update description */
/**
 * @brief Perform send zcl gp pairing configuration command for ZGP cluster
 *
 * @param buf_ref        [in]  Buffer reference
 * @param dst_addr       [in]  Destination address
 * @param dst_addr_mode  [in]  Destination address mode
 * @param actions        [in]  Actions
 * @param ent            [in]  Sink table entry
 * @param def_resp       [in]  Enable ZCL default response if TRUE
 * @param cb             [in]  Call callback if needed after sending request
 *
 * @see ZGP spec, A.3.3.4.6
 *
 */
void zgp_cluster_send_pairing_configuration(zb_uint8_t             buf_ref,
                                            zb_uint16_t            dst_addr,
                                            zb_uint8_t             dst_addr_mode,
                                            zb_uint8_t             actions,
                                            zb_zgp_sink_tbl_ent_t *ent,
                                            zb_uint8_t             num_paired_endpoints,
                                            zb_uint8_t            *paired_endpoints,
                                            zb_uint8_t             app_info,
                                            zb_uint16_t            manuf_id,
                                            zb_uint16_t            model_id,
                                            zb_uint8_t             num_gpd_commands,
                                            zb_uint8_t            *gpd_commands,
                                            zb_zgp_cluster_list_t *cluster_list,
                                            zb_callback_t          cb);
#endif  /* defined ZB_ENABLE_ZGP_SINK || defined  ZGP_COMMISSIONING_TOOL */

#ifdef ZGP_COMMISSIONING_TOOL
/**
 * @brief Perform send zcl gp sink commissioning mode command for ZGP cluster
 *
 * @param buf_ref        [in]  Buffer reference
 * @param dst_addr       [in]  Destination address
 * @param dst_addr_mode  [in]  Destination address mode
 * @param options        [in]  Options
 * @param endpoint       [in]  Endpoint
 * @param cb             [in]  Call callback if needed after sending request
 *
 * @see ZGP spec, A.3.3.4.8
 *
 */
void zgp_cluster_send_gp_sink_commissioning_mode(zb_uint8_t    buf_ref,
                                                 zb_uint16_t   dst_addr,
                                                 zb_uint8_t    dst_addr_mode,
                                                 zb_uint8_t    options,
                                                 zb_uint8_t    endpoint,
                                                 zb_callback_t cb);
#endif  /* ZGP_COMMISSIONING_TOOL */

void zgp_gp_set_shared_security_key_type(enum zb_zgp_security_key_type_e type);
#define ZGP_GP_SET_SHARED_SECURITY_KEY_TYPE(type) zgp_gp_set_shared_security_key_type(type)

#ifdef ZGP_CLUSTER_TEST
/* Application specific zcl command handler for test purposes */
typedef zb_uint8_t (*zgp_cluster_app_zcl_cmd_handler_t)(zb_uint8_t buf_ref);

/**
 * @brief Setup specific application zcl command handler for test purpose
 *
 * @param handler [in]  Specific application zcl command handler pointer
 */
void zgp_cluster_set_app_zcl_cmd_handler(zgp_cluster_app_zcl_cmd_handler_t handler);
#endif  /* ZGP_CLUSTER_TEST */
#endif  /* !ZB_ZGPD_ROLE */

#endif /* ZBOSS_API_ZGP_H */
