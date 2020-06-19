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
/*  PURPOSE: Public Network layer API
*/
#ifndef ZB_ZBOSS_API_NWK_H
#define ZB_ZBOSS_API_NWK_H 1

/** \addtogroup nwk_api */
/** @{ */

/** @addtogroup nwk_common_constants NWK common constants
 * @{
 */
/** @brief Network broadcast addresses types. */
typedef enum zb_nwk_broadcast_address_e
{
  ZB_NWK_BROADCAST_ALL_DEVICES        = 0xFFFF, /**< All devices in PAN */
  ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE    = 0xFFFD, /**< macRxOnWhenIdle = TRUE */
  ZB_NWK_BROADCAST_ROUTER_COORDINATOR = 0xFFFC, /**< All routers and coordinator */
  ZB_NWK_BROADCAST_LOW_POWER_ROUTER   = 0xFFFB, /**< Low power routers only */

  ZB_NWK_BROADCAST_RESERVED           = 0xFFF8 /**< Reserved value */
}
zb_nwk_broadcast_address_t;
/** @} */ /* nwk_common_constants */

/** @addtogroup nwk_management_service NWK management service
 *  @{
 */
/** @brief Check that address is broadcast.
  * @param addr - 16-bit address
  * @return TRUE if address is broadcast, FALSE otherwise
  */
/* #define ZB_NWK_IS_ADDRESS_BROADCAST(addr) ( ((addr) & 0xFFF0) == 0xFFF0 ) */
#define ZB_NWK_IS_ADDRESS_BROADCAST(addr) ( (addr) >= ZB_NWK_BROADCAST_RESERVED )
/** @} */ /* nwk_management_service */

/** @addtogroup nwk_common_constants NWK common constants
 * @{
 */
/**
   Network device type
*/
typedef enum zb_nwk_device_type_e
{
  ZB_NWK_DEVICE_TYPE_COORDINATOR,       /*!< Device - Coordinator */
  ZB_NWK_DEVICE_TYPE_ROUTER,            /*!< Device - Router */
  ZB_NWK_DEVICE_TYPE_ED,                /*!< Device - End device */
  ZB_NWK_DEVICE_TYPE_NONE               /*!< Unknown Device */
}
zb_nwk_device_type_t;


/** @brief Leave types */
typedef enum zb_nwk_leave_type_e
{
  ZB_NWK_LEAVE_TYPE_RESET                            = 0x00, /**< Leave without rejoin */
  ZB_NWK_LEAVE_TYPE_REJOIN                           = 0x01  /**< Leave with rejoin */
} zb_nwk_leave_type_t;


/** @cond internals_doc */
/** @brief Network command status codes. */
typedef enum zb_nwk_command_status_e
{
  ZB_NWK_COMMAND_STATUS_NO_ROUTE_AVAILABLE           = 0x00, /**< No route available */
  ZB_NWK_COMMAND_STATUS_TREE_LINK_FAILURE            = 0x01, /**< Tree link failure */
  ZB_NWK_COMMAND_STATUS_NONE_TREE_LINK_FAILURE       = 0x02, /**< None-tree link failure */
  ZB_NWK_COMMAND_STATUS_LOW_BATTERY_LEVEL            = 0x03, /**< Low battery level */
  ZB_NWK_COMMAND_STATUS_NO_ROUTING_CAPACITY          = 0x04, /**< No routing capacity */
  ZB_NWK_COMMAND_STATUS_NO_INDIRECT_CAPACITY         = 0x05, /**< No indirect capacity */
  ZB_NWK_COMMAND_STATUS_INDIRECT_TRANSACTION_EXPIRY  = 0x06, /**< Indirect transaction expiry */
  ZB_NWK_COMMAND_STATUS_TARGET_DEVICE_UNAVAILABLE    = 0x07, /**< Target device unavailable */
  ZB_NWK_COMMAND_STATUS_TARGET_ADDRESS_UNALLOCATED   = 0x08, /**< Target address unallocated */
  ZB_NWK_COMMAND_STATUS_PARENT_LINK_FAILURE          = 0x09, /**< Parent link failure */
  ZB_NWK_COMMAND_STATUS_VALIDATE_ROUTE               = 0x0a, /**< Validate route */
  ZB_NWK_COMMAND_STATUS_SOURCE_ROUTE_FAILURE         = 0x0b, /**< Source route failure */
  ZB_NWK_COMMAND_STATUS_MANY_TO_ONE_ROUTE_FAILURE    = 0x0c, /**< Many-to-one route failure */
  ZB_NWK_COMMAND_STATUS_ADDRESS_CONFLICT             = 0x0d, /**< Address conflict */
  ZB_NWK_COMMAND_STATUS_VERIFY_ADDRESS               = 0x0e, /**< Verify address */
  ZB_NWK_COMMAND_STATUS_PAN_IDENTIFIER_UPDATE        = 0x0f, /**< Pan ID update */
  ZB_NWK_COMMAND_STATUS_NETWORK_ADDRESS_UPDATE       = 0x10, /**< Network address update */
  ZB_NWK_COMMAND_STATUS_BAD_FRAME_COUNTER            = 0x11, /**< Bad frame counter  */
  ZB_NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER      = 0x12, /**< Bad key sequence number */
  ZB_NWK_COMMAND_STATUS_UNKNOWN_COMMAND              = 0x13, /**< Command received is not known */
}
zb_nwk_command_status_t;
/** @endcond */ /* internals_doc */
/** @} */ /* nwk_common_constants */

/** @cond internals_doc */
/** @brief Arguments of the NLME-STATUS.indication routine. */
typedef ZB_PACKED_PRE struct zb_nlme_status_indication_s
{
  zb_uint8_t status; /**< Error code associated with the failure */
  zb_uint16_t network_addr;  /**< Network device address associated with the status information */
  zb_uint8_t unknown_command_id; /**< Unknown command ID
                                    (required for ZB_NWK_COMMAND_STATUS_UNKNOWN_COMMAND) */
} ZB_PACKED_STRUCT
zb_nlme_status_indication_t;
/** @endcond */ /* internals_doc */

/** @addtogroup nwk_common_constants NWK common constants
 * @{
 */
/**
   LQI undefined value
 */
#define ZB_MAC_LQI_UNDEFINED 0
/**
   RSSI undefined value
 */
#define ZB_MAC_RSSI_UNDEFINED 0x7f


/**
   Number of the first channel
*/
/* #define ZB_MAC_START_CHANNEL_NUMBER 11 */
/**
   Maximal number of the channels
 */
#define ZB_MAC_MAX_CHANNEL_NUMBER   26
/**
   Total number of supported channels
 */
#define ZB_MAC_SUPPORTED_CHANNELS   (ZB_MAC_MAX_CHANNEL_NUMBER + 1)

/** @cond DOXYGEN_SE_SECTION */
/* See D.10.2.1.3.2 Channel Pages */

/**
   The first logical channel for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_START_LOGICAL_CHANNEL 0

/* 08/21/2017 EE CR:MINOR Am I right this value is always 0 for any page? Do we ever need separate constants? */
/**
   Number of the first channel in channel mask for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_START_CHANNEL_NUMBER 0
/**
   The last logical channel for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_MAX_LOGICAL_CHANNEL 26
/**
   Number of the last channel in channel mask for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE28_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE28_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_START_LOGICAL_CHANNEL 27
/**
   Number of the first channel in channel mask for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_START_CHANNEL_NUMBER 0
/**
   The last logical channel for Page 29 Sub GHz (for solid range 27-34)
*/
#define ZB_PAGE29_SUB_GHZ_PRE_MAX_LOGICAL_CHANNEL 34
/**
   The last logical channel for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_MAX_LOGICAL_CHANNEL 62
/**
   Number of the last channel in channel mask for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE29_SUB_GHZ_PRE_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE29_SUB_GHZ_START_LOGICAL_CHANNEL + 1 /* 62 channel */)

/**
   The first logical channel for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_START_LOGICAL_CHANNEL 35
/**
   Number of the first channel in channel mask for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_START_CHANNEL_NUMBER 0
/**
   The last logical channel for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_MAX_LOGICAL_CHANNEL 61
/**
   Number of the last channel in channel mask for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE30_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE30_SUB_GHZ_START_LOGICAL_CHANNEL)

/**
   The first logical channel for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_START_LOGICAL_CHANNEL 0
/**
   Number of the first channel in channel mask for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_START_CHANNEL_NUMBER 0
/**
   The last logical channel for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_MAX_LOGICAL_CHANNEL 26
/**
   Number of the last channel in channel mask for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_MAX_CHANNEL_NUMBER (ZB_PAGE31_SUB_GHZ_MAX_LOGICAL_CHANNEL - \
                                              ZB_PAGE31_SUB_GHZ_START_LOGICAL_CHANNEL)
  /** @endcond */ /* DOXYGEN_SE_SECTION */
/* ZB_MAC_START_CHANNEL_NUMBER */
/**
   The first logical channel for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_START_LOGICAL_CHANNEL 11
/* Let's keep compatibility with bits layout in ZBOSS without sub-gig: start from 11 in 2.4.
   Actually, only subgig pages 28 and 31 are ok too (logical == bit#). Pages 29 and 30 are special case.
 */
/**
   Number of the first channel in channel mask for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_START_CHANNEL_NUMBER ZB_PAGE0_2_4_GHZ_START_LOGICAL_CHANNEL
/**
   The last logical channel in channel mask for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_MAX_LOGICAL_CHANNEL ZB_MAC_MAX_CHANNEL_NUMBER
/**
   Number of the last channel in channel mask for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_MAX_CHANNEL_NUMBER  ZB_MAC_MAX_CHANNEL_NUMBER
/* (ZB_PAGE0_2_4_GHZ_MAX_LOGICAL_CHANNEL - \ */
/*  ZB_PAGE0_2_4_GHZ_START_LOGICAL_CHANNEL) */

/** @cond DOXYGEN_SE_SECTION */
/**
   Maximal number of channels for Page 28 Sub GHz
*/
#define ZB_PAGE28_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE28_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE28_SUB_GHZ_START_CHANNEL_NUMBER + 1)
/**
   Maximal number of channels for Page 29 Sub GHz
*/
#define ZB_PAGE29_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE29_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE29_SUB_GHZ_START_CHANNEL_NUMBER + 1)
/**
   Maximal number of channels for Page 30 Sub GHz
*/
#define ZB_PAGE30_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE30_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE30_SUB_GHZ_START_CHANNEL_NUMBER + 1)
/**
   Maximal number of channels for Page 31 Sub GHz
*/
#define ZB_PAGE31_SUB_GHZ_MAX_CHANNELS_COUNT (ZB_PAGE31_SUB_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE31_SUB_GHZ_START_CHANNEL_NUMBER + 1)
/** @endcond */ /* DOXYGEN_SE_SECTION */
/**
   Maximal number of channels for Page 0 2.4GHz
*/
#define ZB_PAGE0_2_4_GHZ_MAX_CHANNELS_COUNT  (ZB_PAGE0_2_4_GHZ_MAX_CHANNEL_NUMBER - \
                                              ZB_PAGE0_2_4_GHZ_START_CHANNEL_NUMBER  + 1)
/** @cond DOXYGEN_SE_SECTION */
/**
   Maximal number of channels for all pages
*/
#define ZB_ED_SCAN_MAX_CHANNELS_COUNT                                   \
  ((ZB_IO_BUF_SIZE - sizeof(zb_uint8_t)) / sizeof(zb_energy_detect_channel_info_t))

#define ZB_CHANNEL_PAGE_MAX_CHANNELS_COUNT ZB_PAGE28_SUB_GHZ_MAX_CHANNELS_COUNT
/** @endcond */ /* DOXYGEN_SE_SECTION */
/** @} */ /* nwk_common_constants */

/** @cond internals_doc */
/** @addtogroup nwk_pib_cache PIB cache
 *  @{
 */

/**
  Data structure used to cache frequently used PIB attributes.

  Useful when using alien MAC layer without direct access to PIB: blockable "get"
  interface to PIB is too complex and slow.
 */
typedef struct zb_nwk_pib_cache_s
{
  zb_uint16_t             mac_short_address;             /*!< The 16-bit address that the device uses
                                                           to communicate in the PAN. */
  zb_uint16_t             mac_pan_id;                    /*!< The 16-bit identifier of the PAN on which
                                                           the device is operating. If this value is 0xffff,
                                                           the device is not associated. */
  zb_ieee_addr_t          mac_extended_address;          /*!< The 64-bit (IEEE) address assigned to the device. */

  zb_uint8_t              mac_association_permit;        /*!< Indication of whether a coordinator is currently
                                                           allowing association. A value of TRUE indicates*/
  zb_uint8_t              mac_rx_on_when_idle;           /*!< Indication of whether the MAC sublayer is to enable
                                                           its receiver during idle periods. */
  zb_uint8_t              phy_current_page;              /*!< Index of current physical channel page  */
  zb_uint8_t              phy_current_channel;           /*!< Index of current physical channel */
} zb_nwk_pib_cache_t;

/** @} */


/**
  * @name PIB cache mirror
  * \par Here are cache of PIB / NIB mirror of PIB values.
  *
  * After that values modified in NWK or upper, must sync it with MAC using MLME-SET.
  *
  * @{
  */

/**
  Function used to access PIB cache.
  @return pointer to PIB cache.
  */
zb_nwk_pib_cache_t *zb_nwk_get_pib_cache(void);
/**
  Macro used to access PIB cache.
  @return pointer to PIB cache.
  */
#define ZB_PIB_CACHE() zb_nwk_get_pib_cache()

#ifndef NCP_MODE_HOST
/** Cached value of device network address */
#define ZB_PIBCACHE_NETWORK_ADDRESS()  ZB_PIB_CACHE()->mac_short_address
/** Cached value of device Pan ID */
#define ZB_PIBCACHE_PAN_ID()           ZB_PIB_CACHE()->mac_pan_id
/** Cached value of device extended address */
#define ZB_PIBCACHE_EXTENDED_ADDRESS() ZB_PIB_CACHE()->mac_extended_address
/** Cached value of RxOnWhenIdle attribute */
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE()  ZB_PIB_CACHE()->mac_rx_on_when_idle
/** Cached value of AssociationPermit attribute */
#define ZB_PIBCACHE_ASSOCIATION_PERMIT() ZB_PIB_CACHE()->mac_association_permit
/** Cached value of CurrentChannel attribute */
#define ZB_PIBCACHE_CURRENT_CHANNEL()  ZB_PIB_CACHE()->phy_current_channel
/** Cached value of CurrentPage attribute */
#define ZB_PIBCACHE_CURRENT_PAGE()  ZB_PIB_CACHE()->phy_current_page

#else /* NCP_MODE_HOST */

/** Cached value of device network address */
#define ZB_PIBCACHE_NETWORK_ADDRESS()  zb_get_short_address()
/** Cached value of RxOnWhenIdle attribute */
#define ZB_PIBCACHE_RX_ON_WHEN_IDLE()  zb_get_rx_on_when_idle()

#endif /* !NCP_MODE_HOST */

/**
   Arguments of the NLME-PERMIT_JOINING.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_permit_joining_request_s
{
  zb_uint8_t permit_duration; /**< Time in seconds during which the coordinator
                               * or router will allow associations */
} ZB_PACKED_STRUCT
zb_nlme_permit_joining_request_t;

/**
   NLME-PERMIT-JOINING.request primitive

   Allow/disallow network joining

   @param param - buffer containing parameters - @see
   zb_nlme_network_formation_request_t
   @return RET_OK on success, error code otherwise.

   @snippet doxygen_snippets.dox zb_nlme_permit_joining_request
 */
void zb_nlme_permit_joining_request(zb_uint8_t param);


/** @} */
/** @endcond */ /* internals_doc */

/** @addtogroup nwk_panid_conflicts NWK PAN ID conflicts
 *  @{
 */

#define ZB_PAN_ID_CONFLICT_INFO_MAX_PANIDS_COUNT  16

/** Structure describing a detected PAN ID conflict */
typedef ZB_PACKED_PRE struct zb_pan_id_conflict_info_s
{
  zb_uint16_t panid_count;      /* <! Count of neighboring PAN IDs. */
  zb_uint16_t panids[ZB_PAN_ID_CONFLICT_INFO_MAX_PANIDS_COUNT];
                                /* <! Array of such PAN IDs.
                                 *  Only values at indices 0..(panid_count - 1) will be taken
                                 *  into consideration. */
} ZB_PACKED_STRUCT zb_pan_id_conflict_info_t;

/**
   Allow/disallow network joining

   @param param - pointer to a structure containing PAN ID conflict
   information - @see zb_pan_id_conflict_info_t

 * @b Example:
 ZB_ZDO_SIGNAL_CUT_HEADER macro may be used to remove from the buffer everything but the signal parameter (zb_pan_id_conflict_info_t structure in this case)
   @code

void zboss_signal_handler(zb_uint8_t param)
{
  zb_pan_id_conflict_info_t *info = NULL;
  zb_zdo_app_signal_hdr_t *sg_p = NULL;
  zb_zdo_app_signal_type_t sig = zb_get_app_signal(param, &sg_p);

  if (ZB_GET_APP_SIGNAL_STATUS(param) == 0)
  {
    switch (sig)
    {
      case ZB_NWK_SIGNAL_PANID_CONFLICT_DETECTED:
        info = ZB_ZDO_SIGNAL_GET_PARAMS(sg_p, zb_pan_id_conflict_info_t);
        ZB_ZDO_SIGNAL_CUT_HEADER(ZB_BUF_FROM_REF(param));
        zb_start_pan_id_conflict_resolution(param);
        break;
    }
  }
}
   @endcode
 */
void zb_start_pan_id_conflict_resolution(zb_uint8_t param);


/**
   This function must be used for enabling/disabling automatic PAN ID conflict resolution
   If the automatic resolution is disabled, ZBOSS will issue a ZB_NWK_SIGNAL_PANID_CONFLICT_DETECTED
   signal each time it receives Network Report about PAN ID conflict or (in case the device is
   the network manager) each time it detects such conflicts.

   By default the automatic PAN ID conflict resolution is disabled.

   That call also enabled panid conflict resolution - see zb_enable_panid_conflict_resolution();

   @param status - ZB_TRUE in order to enable automatic PAN ID conflict, ZB_FALSE otherwise
 */
void zb_enable_auto_pan_id_conflict_resolution(zb_bool_t status);

/** @cond internals_doc */
/**
   Enables panid conflict resolution.

   Call of that function forces linking of panid conflict resolution
   code and switching on panid conflict resolution and detection
   logic.
 */
void zb_enable_panid_conflict_resolution(void);
/** @endcond */ /* internals_doc */
/** @} */ /* nwk_panid_conflicts */

#ifdef ZB_LOW_SECURITY_MODE
/** @addtogroup nwk_management_service NWK management service
 * @{
 */
/**
    Public API to set device security level to 0
*/
void zb_disable_nwk_security(void);

/**
    Public API to set device security level to 5
*/
void zb_enable_nwk_security(void);

/**
   Configure Destination IEEE policy for out NWK frames

   @param put_always - if ZB_TRUE, put DST IEEE for all NWK frame types, if it is set to ZB_FALSE -
   put it for NWK Command frame type only
   Default value is ZB_TRUE.
*/
void zb_nwk_set_ieee_policy(zb_bool_t put_always);
/** @} */ /* nwk_management_service */
#endif /*ZB_LOW_SECURITY_MODE*/

/** @addtogroup nwk_mtorr NWK MTORR functionality
 * @{
 */
/**
   Enable Concentrator mode for the device (disabled by default).
   It's possible to call this function to send MTORR immediately, e.g. after a new device joined the network.
   It does affect only for Coordinator role.

   @param radius - the hop count radius for concentrator route discoveries.
   If the value is set zero then the default radius will be used.
   @param disc_time - the time in seconds between concentrator route discoveries.
   If the value is set to zero, the route discoveries are done by the application layer only.
*/
void zb_start_concentrator_mode(zb_uint8_t radius, zb_uint32_t disc_time);

/**
   Disable Concentrator mode for the device.
   It does affect only for Coordinator role.
*/
void zb_stop_concentrator_mode(void);
/** @} */ /* nwk_mtorr */

/** @cond internals_doc */
/**
  Change Transmit Failure Threshold for Neighbor table.
  This threshold defines how many packets may fail the delivery before stack will start Address
  Discovery.
  Formula for discovery start: if device is a neighbor and packet can not be delivered, try to
  - do "Transmit Failure Threshold" attempts of packet delivery
  - if packet is not delivered, wait "Transmit Failure Timeout"
  - if packet is not delivered, remove device from neighbors and start address discovery procedure
  @param transmit_failure_cnt - new threshold (1 to 255)
  Default value - @ref ZB_ZDO_NEIGHBOR_FAILURE_PKT_THRESHOLD
*/
void zb_set_nbt_transmit_failure_threshold(zb_uint8_t transmit_failure_cnt);
/**
  Change Transmit Failure Timeout for Neighbor table.
  This timeout defines how much time stack will wait before starting Network Address
  discovery. Countdown starts after Transmit Failure Threshold condition is met.
  @param transmit_failure_timeout - new timeout in seconds (0 to 255)
  Default value - @ref ZB_ZDO_NEIGHBOR_UNREACHABLE_TIMEOUT
*/
void zb_set_nbt_transmit_failure_timeout(zb_uint8_t transmit_failure_timeout);
/** @endcond */ /* internals_doc */

/** @addtogroup nwk_management_service NWK management service
 * @{
 */
/**
   Get short address of the parent node.

   @return Short address of the parent node or ZB_UNKNOWN_SHORT_ADDR if the device isn't joined to a network.
 */
zb_uint16_t zb_nwk_get_parent(void);
/** @} */ /* nwk_management_service */
/** @} */

#endif /*#ifndef ZB_ZBOSS_API_NWK_H*/
