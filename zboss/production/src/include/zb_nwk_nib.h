/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2024 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: NWK NIB database
*/

#ifndef ZB_NWK_NIB_H
#define ZB_NWK_NIB_H 1

/**
 *  \addtogroup nwk_ib
 *    @{
 *      @par NWK NIB
 *      Some NIB fields are indeed PIB fields. Use macros to access it.
 */


/*!  NWK NIB Attributes */
typedef enum zb_nib_attribute_e
{
  /**
     A sequence number used to identify outgoing frames.
    */
  ZB_NIB_ATTRIBUTE_SEQUENCE_NUMBER                   = 0X81,
  /**
     The maximum time duration in OctetDurations allowed for the parent and
     all child devices to retransmit a broadcast message
     (passive acknowledgment timeout).
    */
  ZB_NIB_ATTRIBUTE_PASSIVE_ASK_TIMEOUT               = 0X82,
  /**
     The maximum number of retries allowed after a broadcast transmission failure.
    */
  ZB_NIB_ATTRIBUTE_MAX_BROADCAST_RETRIES             = 0X83,
  /**
     The number of children a device is allowed to have on its current network.
     @note When nwkAddrAlloc has a value of 0x02, indicating stochastic
     addressing, the value of this attribute is implementation-dependent.
    */
  ZB_NIB_ATTRIBUTE_MAX_CHILDREN                      = 0X84,
  /**
     The depth a device can have.
    */
  ZB_NIB_ATTRIBUTE_MAX_DEPTH                         = 0X85,
  /** The number of routers any one device is allowed to have as children.
      This value is determined by the Zigbee coordinator for all devices
      in the network. If nwkAddrAlloc is 0x02 this value not used.
    */
  ZB_NIB_ATTRIBUTE_MAX_ROUTERS                       = 0X86,
  /**
     The current set of neighbor table entries in the device.
    */
  ZB_NIB_ATTRIBUTE_NEIGHBOR_TABLE                    = 0X87,
  /**
     Time duration in OctetDurations that a broadcast message needs to encompass
     the entire network. This is a calculated quantity based on other NIB
     attributes.
    */
  ZB_NIB_ATTRIBUTE_BROADCAST_DELIVERY_TIME           = 0X88,
  /**
     If this is set to 0, the NWK layer shall calculate link cost from all
     neighbor nodes using the LQI values reported by the MAC layer; other-wise,
     it shall report a constant value.
    */
  ZB_NIB_ATTRIBUTE_REPORT_CONSTANT_COST              = 0X89,
  /**
     Reserved.
    */
  ZB_NIB_ATTRIBUTE_ROUTE_DISCOVERY_RETRIES_PERMITTED = 0X8A,
  /**
     The current set of routing table entries in the device.
    */
  ZB_NIB_ATTRIBUTE_ROUTE_TABLE                       = 0X8B,
  /**
     The current route symmetry setting:
        - TRUE means that routes are considered to be comprised of symmetric links.
       Backward and forward routes are created during one-route discovery
       and they are identical.
        - FALSE indicates that routes are not consider to be comprised of symmetric
       links. Only the forward route is stored during route discovery.
    */
  ZB_NIB_ATTRIBUTE_SYM_LINK                          = 0X8E,
  /**
     This field shall contain the device capability information established at
     network joining time.
    */
  ZB_NIB_ATTRIBUTE_CAPABILITY_INFORMATION            = 0X8F,
  /**
     A value that determines the method used to assign addresses:
      - 0x00 = use distributed address allocation
      - 0x01 = reserved
      - 0x02 = use stochastic address allocation
    */
  ZB_NIB_ATTRIBUTE_ADDR_ALLOC                        = 0X90,
  /**
     A flag that determines whether the NWK layer should assume the ability to
     use hierarchical routing:
      - TRUE = assume the ability to use hierarchical routing.
      - FALSE = never use hierarchical routing.
    */
  ZB_NIB_ATTRIBUTE_USE_TREE_ROUTING                  = 0X91,
  /**
     The address of the designated network channel manager function.
    */
  ZB_NIB_ATTRIBUTE_MANAGER_ADDR                      = 0X92,
  /**
     The maximum number of hops in a source route.
    */
  ZB_NIB_ATTRIBUTE_MAX_SOURCE_ROUTE                  = 0X93,
  /**
     The value identifying a snapshot of the network settings with which this
     node is operating with.
    */
  ZB_NIB_ATTRIBUTE_UPDATE_ID                         = 0X94,
  /**
     The maximum time (in superframe periods) that a transaction is stored by a
     coordinator and indicated in its beacon. This attribute reflects the value
     of the MAC PIB attribute macTransaction-PersistenceTime and any changes
     made by the higher layer will be reflected in the MAC PIB attribute value
     as well.
    */
  ZB_NIB_ATTRIBUTE_TRANSACTION_PERSISTENCE_TIME      = 0X95,
  /**
     The 16-bit address that the device uses to communicate with the PAN.
     This attribute reflects the value of the MAC PIB attribute macShortAddress
     and any changes made by the higher layer will be reflected in the MAC PIB
     attribute value as well.
    */
  ZB_NIB_ATTRIBUTE_NETWORK_ADDRESS                   = 0X96,
  /**
     The identifier of the Zigbee stack profile in use for this device.
    */
  ZB_NIB_ATTRIBUTE_STACK_PROFILE                     = 0X97,
  /**
     The current set of broadcast transaction table entries in the device.
    */
  ZB_NIB_ATTRIBUTE_BROADCAST_TRANSACTION_TABLE       = 0X98,
  /**
     The set of group identifiers, in the range 0x0000 - 0xffff, for groups of
     which this device is a member.
    */
  ZB_NIB_ATTRIBUTE_GROUP_ID_TABLE                    = 0X99,
  /**
     The Extended PAN Identifier for the PAN of which the device is a member.
     The value 0x0000000000000000 means the Extended PAN Identifier is unknown.
    */
  ZB_NIB_ATTRIBUTE_EXTENDED_PANID                    = 0X9A,
  /**
     A flag determining the layer where multicast messaging occurs.
      - TRUE = multicast occurs at the network layer.
      - FALSE= multicast oc-curs at the APS layer and using the APS header.
    */
  ZB_NIB_ATTRIBUTE_USE_MULTICAST                     = 0X9B,
  /**
     The route record table.
    */
  ZB_NIB_ATTRIBUTE_ROUTE_RECORD_TABLE                = 0X9C,
  /**
     A flag determining if this device is a concentrator.
      - TRUE = Device is a concentrator.
      - FALSE = Device is not a concentrator.
    */
  ZB_NIB_ATTRIBUTE_IS_CONCENTRATOR                   = 0X9D,
  /**
     The hop count radius for concentrator route discoveries.
    */
  ZB_NIB_ATTRIBUTE_CONCENTRATOR_RADIUS               = 0X9E,
  /**
     The time in seconds between concentrator route discoveries.
     If set to 0x0000, the discoveries are done at start up and by
     the next higher layer only.
    */
  ZB_NIB_ATTRIBUTE_CONCENTRATOR_DESCOVERY_TIME       = 0X9F,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_SECURITY_LEVEL                    = 0XA0,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_SECURITY_MATERIAL_SET             = 0XA1,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_ACTIVE_KEY_SEQ_NUMBER             = 0XA2,
  /**
     Security attribute defined in Chapter 4. @see ZB R21 specification.
    */
  ZB_NIB_ATTRIBUTE_ALL_FRESH                         = 0XA3,
  /**
     Not used.
    */
  ZB_NIB_ATTRIBUTE_SECURE_ALL_FRAMES                 = 0XA5,
  /**
     The time in seconds between link status command frames.
    */
  ZB_NIB_ATTRIBUTE_LINK_STATUS_PERIOD                = 0XA6,
  /**
     The number of missed link status command frames before resetting the
     link costs to zero.
    */
  ZB_NIB_ATTRIBUTE_ROUTER_AGE_LIMIT                  = 0XA7,
  /**
     A flag that determines whether the NWK layer should detect and correct
     conflicting addresses:
     - TRUE = assume ad-dresses are unique.
     - FALSE = addresses may not be unique.
  */
  ZB_NIB_ATTRIBUTE_UNIQUE_ADDR                       = 0XA8,
  /**
     The current set of 64-bit IEEE to 16-bit network address map.
    */
  ZB_NIB_ATTRIBUTE_ADDRESS_MAP                       = 0XA9,
  /**
     A flag that determines if a time stamp indication is provided on incoming
     and outgoing packets.
      - TRUE= time indication provided.
      - FALSE = no time indication provided.
    */
  ZB_NIB_ATTRIBUTE_TIME_STAMP                        = 0X8C,
  /**
     This NIB attribute should, at all times, have the same value as macPANId.
    */
  ZB_NIB_ATTRIBUTE_PAN_ID                            = 0X80,
  /**
     A count of unicast transmissions made by the NWK layer on this device.
     Each time the NWK layer transmits a unicast frame, by invoking the
     MCPS-DATA.request primitive of the MAC sub-layer, it shall increment this
     counter. When either the NHL performs an NLME-SET.request on this attribute
     or if the value of nwkTxTotal rolls over past 0xffff the NWK layer shall
     reset to 0x00 each Transmit Failure field contained in the neighbor table.
    */
  ZB_NIB_ATTRIBUTE_TX_TOTAL                          = 0X8D,
  /**
     This policy determines whether or not a remote NWK leave request command
     frame received by the local device is accepted.
    */
  ZB_NIB_ATTRIBUTE_LEAVE_REQ_ALLOWED                 = 0xAA,

  /* TODO: FILL MISSING ATTRIBUTES!!! */

  /**
     A table of lower-layer interfaces managed by the network layer.
   */
  ZB_NIB_ATTRIBUTE_MAC_INTERFACE_TABLE               = 0xAF,
}
zb_nib_attribute_t;

/** @brief return NWK sequence number used to identify outgoing frames */
#define ZB_NIB_SEQUENCE_NUMBER() ZB_NIB().sequence_number
/** @brief increment NWK sequence number used to identify outgoing frames */
#define ZB_NIB_SEQUENCE_NUMBER_INC() (ZB_NIB().sequence_number++)

/** @brief Maximum network depth a device can have */
#define ZB_NIB_MAX_DEPTH() ZB_NIB().max_depth
/** @brief Device network depth */
#define ZB_NIB_DEPTH() ZB_NIB().depth

/**
 * Returns current type of the device.
 */
zb_bool_t zb_is_device_zc(void);

zb_bool_t zb_is_device_zr(void);

zb_bool_t zb_is_device_zed(void);

zb_bool_t zb_is_device_zc_or_zr(void);

#if defined ZB_ED_ROLE || defined DOXYGEN

/**
   @cond DOXYGEN_GENERAL_SECTION
   @}
   @{
   \addtogroup zboss_general_api
 */

/* ZED-only build */

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() ZB_FALSE
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() ZB_FALSE
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() ZB_TRUE
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() ZB_FALSE
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() ZB_TRUE

/**
     @{
     \addtogroup nwk_ib
     @}
     @endcond
 */

#elif (defined ZB_ROUTER_ROLE && !defined ZB_ED_FUNC && !defined ZB_COORDINATOR_ROLE)

/* ZR-only build, can't switch into ED mode */

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() ZB_FALSE
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() ZB_TRUE
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() ZB_FALSE
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() ZB_TRUE
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() ZB_FALSE

#elif (defined ZB_ROUTER_ROLE && defined ZB_ED_FUNC && !defined ZB_COORDINATOR_ROLE)

/* ZR-ZED only build, ZR can switch into ED mode at runtime */

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() ZB_FALSE
/** @brief returns true if device is Router */
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() (zb_is_device_zr())
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() (zb_is_device_zed())
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() (ZB_IS_DEVICE_ZR())
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() (!ZB_IS_DEVICE_ZR())

#elif (defined ZB_COORDINATOR_ROLE && defined ZB_COORDINATOR_ONLY)

/* ZC-only build */

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() ZB_TRUE
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() ZB_FALSE
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() ZB_FALSE
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() ZB_TRUE
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() ZB_TRUE

#else

/* Full build: zc-zr-zed switchable at runtime */

/** @brief returns true if device is Coordinator */
#define ZB_IS_DEVICE_ZC() (zb_is_device_zc())
/** @brief returns true if device is Router */
#define ZB_IS_DEVICE_ZR() (zb_is_device_zr())
/** @brief returns true if device is End Device */
#define ZB_IS_DEVICE_ZED() (zb_is_device_zed())
/** @brief returns true if device is Coordinator or Router */
#define ZB_IS_DEVICE_ZC_OR_ZR() (zb_is_device_zc_or_zr())
/** @brief returns true if device is not Router */
#define ZB_IS_DEVICE_NOT_ZR() (!ZB_IS_DEVICE_ZR())

#endif  /* roles select */

/**
   Check that the device at ZC working in Switch mode (routing between 2.4 and sub-ghz).

   Currently Switch is not implemented, so always return 0.
 */
#define ZB_SUBGHZ_SWITCH_MODE() ZB_FALSE

#define ZB_SET_SUBGHZ_SWITCH_MODE(mode)

/** @brief logical device type, @see @ref nwk_device_type */
#define ZB_NIB_DEVICE_TYPE() (ZB_NIB().device_type + 0U)


#ifdef ZB_PRO_STACK
/** @brief number of source routes in device source routing table */
#define ZB_NIB_SRCRT_CNT() ZB_NIB().nwk_src_route_cnt
#endif

/** @brief Device extended Pan ID */
#define ZB_NIB_EXT_PAN_ID() ZB_NIB().extended_pan_id
/** @brief The value of ZB_NIB_ATTRIBUTE_UPDATE_ID attribute */
#define ZB_NIB_UPDATE_ID() ZB_NIB().update_id

/** @brief Device security level; always is 5 */
#define ZB_NIB_SECURITY_LEVEL() ZB_SECURITY_LEVEL
/** @brief \deprecated unsupported */
#define ZB_SET_NIB_SECURITY_LEVEL(v)

/* use tree routing? */
#ifdef ZB_NWK_TREE_ROUTING
/** @brief returns USE_TREE_ROUTING attribute value */
#define ZB_NIB_GET_USE_TREE_ROUTING() ZB_NIB().use_tree_routing
/** @brief Set USE_TREE_ROUTING attribute value to 'v' */
#define ZB_NIB_SET_USE_TREE_ROUTING(v) (ZB_NIB().use_tree_routing = (v))
#else
/** @brief Use tree routing - unsupported */
#define ZB_NIB_GET_USE_TREE_ROUTING() 0
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_USE_TREE_ROUTING(v)
#endif

/* use nwk multicast? */
#ifndef ZB_NO_NWK_MULTICAST
/** @brief returns ZB_NIB_GET_USE_MULTICAST attribute value */
#define ZB_NIB_GET_USE_MULTICAST() ZB_NIB().nwk_use_multicast
/** @brief Set ZB_NIB_SET_USE_MULTICAST attribute value to 'v' */
#define ZB_NIB_SET_USE_MULTICAST( v ) (ZB_NIB().nwk_use_multicast = ( v ))
#else
/** @brief Use multicast - unsupported */
#define ZB_NIB_GET_USE_MULTICAST() 0
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_USE_MULTICAST( v )
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
/** @brief LINK_STATUS_PERIOD attribute */
#define ZB_NIB_GET_LINK_STATUS_PERIOD() ZB_NIB().link_status_period
/** @brief ROUTER_AGE_LIMIT attribute */
#define ZB_NIB_GET_ROUTER_AGE_LIMIT()   ZB_NIB().router_age_limit
#else
/** @brief LINK_STATUS_PERIOD attribute */
#define ZB_NIB_GET_LINK_STATUS_PERIOD() ZB_NWK_LINK_STATUS_PERIOD
/** @brief ROUTER_AGE_LIMIT attribute */
#define ZB_NIB_GET_ROUTER_AGE_LIMIT()   ZB_NWK_ROUTER_AGE_LIMIT
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_POWER_DELTA
/* [R22STUBS]: TODO: Implement proper comments */
#define ZB_NIB_GET_POWER_DELTA_PERIOD(i)                                \
  ZB_NWK_RANDOM_TIME_PERIOD(ZB_NIB().mac_iface_tbl[i].link_power_data_rate * ZB_TIME_ONE_SECOND, \
                            ZB_NWK_LINK_POWER_DELTA_TRANSMIT_RATE_JITTER * ZB_TIME_ONE_SECOND)

/** @brief use custom value for mac_iface_tbl[i].link_power_data_rate */
#define ZB_NIB_SET_POWER_DELTA_PERIOD(i, v) (ZB_NIB().mac_iface_tbl[i].link_power_data_rate = ( v ))

/** @brief mode for Link Power Delta Negotiation */
#define ZB_NIB_NWK_LPD_CMD_MODE() (ZB_NIB().nwk_lpd_cmd_mode)

#else
/* [R22STUBS]: TODO: Implement proper comments */
#define ZB_NIB_GET_POWER_DELTA_PERIOD(i)                                \
  ZB_NWK_RANDOM_TIME_PERIOD(ZB_NWK_LINK_POWER_DELTA_TRANSMIT_RATE * ZB_TIME_ONE_SECOND, \
                            ZB_NWK_LINK_POWER_DELTA_TRANSMIT_RATE_JITTER * ZB_TIME_ONE_SECOND)

#define ZB_NIB_SET_POWER_DELTA_PERIOD(i, v)
#define ZB_NIB_NWK_LPD_CMD_MODE() 0

#endif

/* is leave request allowed? */
#ifdef ZB_PRO_STACK
/** Is leave request allowed */
/** @brief LEAVE_REQ_ALLOWED attribute */
#define ZB_NIB_GET_LEAVE_REQ_ALLOWED() ZB_NIB().leave_req_allowed
/** @brief Set LEAVE_REQ_ALLOWED attribute value to 'v' */
#define ZB_NIB_SET_LEAVE_REQ_ALLOWED( v ) (ZB_NIB().leave_req_allowed = ( v ))
#else
/** @brief LEAVE_REQ_ALLOWED - always true */
#define ZB_NIB_GET_LEAVE_REQ_ALLOWED() ZB_TRUE
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_LEAVE_REQ_ALLOWED( v )
#endif

#ifdef SNCP_MODE
/** Force route record sending */
/** @brief Get force route record attribute */
#define ZB_NIB_GET_FORCE_RREC_SENDING() ZB_NIB().nwk_force_rrec_sending
/** @brief Set force route record attribute value to 'v' */
#define ZB_NIB_SET_FORCE_RREC_SENDING( v ) (ZB_NIB().nwk_force_rrec_sending = ( v ))
#endif

#ifdef ZB_PRO_STACK
/** @brief LEAVE_REQ_WITHOUT_REJOIN_ALLOWED attribute */
#define ZB_NIB_GET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED() ZB_NIB().leave_req_without_rejoin_allowed
/** @brief Set LEAVE_REQ_WITHOUT_REJOIN_ALLOWED attribute value to 'v' */
#define ZB_NIB_SET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED( v ) (ZB_NIB().leave_req_without_rejoin_allowed = ( v ))
#else
/** @brief LEAVE_REQ_WITHOUT_REJOIN_ALLOWED - always true */
#define ZB_NIB_GET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED() ZB_TRUE
/** @brief \deprecated unsupported */
#define ZB_NIB_SET_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED( v )
#endif

#ifndef ZB_LITE_NO_SOURCE_ROUTING
/** @brief IS_CONCENTRATOR attribute */
#define ZB_NIB_GET_IS_CONCENTRATOR() ZB_NIB().nwk_is_concentrator
/** @brief Set IS_CONCENTRATOR attribute to 'v' */
#define ZB_NIB_SET_IS_CONCENTRATOR( v ) (ZB_NIB().nwk_is_concentrator = ( v ))
/** @brief CONCENTRATOR_RADIUS attribute */
#define ZB_NIB_GET_CONCENTRATOR_RADIUS() ZB_NIB().nwk_concentrator_radius
/** @brief Set CONCENTRATOR_RADIUS attribute to 'v' */
#define ZB_NIB_SET_CONCENTRATOR_RADIUS( v ) (ZB_NIB().nwk_concentrator_radius = ( v ))
/** @brief CONCENTRATOR_DISC_TIME attribute */
#define ZB_NIB_GET_CONCENTRATOR_DISC_TIME() ZB_NIB().nwk_concentrator_disc_time
/** @brief Set CONCENTRATOR_DISC_TIME attribute to 'v' */
#define ZB_NIB_SET_CONCENTRATOR_DISC_TIME( v ) (ZB_NIB().nwk_concentrator_disc_time = ( v ))
#else
/** @brief IS_CONCENTRATOR attribute */
#define ZB_NIB_GET_IS_CONCENTRATOR() ZB_FALSE
/** @brief Set IS_CONCENTRATOR attribute to 'v' */
#define ZB_NIB_SET_IS_CONCENTRATOR( v )
/** @brief CONCENTRATOR_RADIUS attribute */
#define ZB_NIB_GET_CONCENTRATOR_RADIUS() 0
/** @brief Set CONCENTRATOR_RADIUS attribute to 'v' */
#define ZB_NIB_SET_CONCENTRATOR_RADIUS( v )
/** @brief CONCENTRATOR_DISC_TIME attribute */
#define ZB_NIB_GET_CONCENTRATOR_DISC_TIME() 0
/** @brief Set CONCENTRATOR_DISC_TIME attribute to 'v' */
#define ZB_NIB_SET_CONCENTRATOR_DISC_TIME( v )
#endif /* ZB_LITE_NO_SOURCE_ROUTING */

/** @brief SECURITY_MATERIAL attribute */
#define ZB_NIB_SECURITY_MATERIAL() ZB_NIB().secur_material_set
/** @brief NWK_MANAGER_ADDR attribute */
#define ZB_NIB_NWK_MANAGER_ADDR() ZB_NIB().nwk_manager_addr

/** @brief total number of transmissions made by NWK */
#define ZB_NIB_NWK_TX_TOTAL() (ZB_NIB().tx_stat.tx_total + 0U)
/** @brief number of transmissions failed by NWK */
#define ZB_NIB_NWK_TX_FAIL()  (ZB_NIB().tx_stat.tx_fail + 0U)

/**
   NWK route discovery
*/
typedef struct zb_nwk_route_discovery_s /* do not pack for IAR */
{
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise   */
  zb_bitfield_t expiration_time:7; /*!< Countdown timer indicating when route
                                    * discovery expires. ZB_NWK_ROUTE_DISCOVERY_EXPIRY 10 */
  zb_uint8_t request_id; /*!< Sequence number for a route request */
  /* TODO: use 1 byte - index in the translation table */
  zb_uint16_t source_addr; /*!< 16-bit network address of the route
                            * requests initiator */
  /* TODO: use 1 byte - index in the translation table */
  zb_uint16_t sender_addr; /*!< 16-bit network address of the device that
                            * has sent the most recent lowest cost route
                            * request */
  zb_uint16_t dest_addr; /*!< 16-bit network destination address of this
                          * request */
  zb_uint8_t forward_cost; /*!< Path cost from the source of the route request
                            * to the current device */
  zb_uint8_t residual_cost; /*!< Path cost from the current to the destination
                             * device */
} ZB_PACKED_STRUCT
zb_nwk_route_discovery_t;

/**
   NWK pending list element
*/
typedef struct zb_nwk_pend_s    /* do not pack for IAR */
{
  zb_uint16_t dest_addr; /*!< 16-bit network destination address of this
                          * request */
  zb_uint8_t  param; /*!< buffer waiting for route discovery */
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise */
  zb_bitfield_t expiry:5; /*!< expiration time. ZB_NWK_PENDING_ENTRY_EXPIRY 20,
                           * 5-bits i */
  zb_bitfield_t waiting_buf:1; /*!< if pending buffer waits new buffer to
                                 * start route discovery */
  zb_bitfield_t reserved:1;

  zb_uint8_t    handle;
} ZB_PACKED_STRUCT
zb_nwk_pend_t;

/**
   Part of the secured material stored in array.

   Other secured material components stored elsewhere:
   OutgoingFrameCounter - NIB
   IncomingFrameCounterSet - neighbor table
 */
typedef struct zb_secur_material_set_t
{
  zb_uint8_t          key[ZB_CCM_KEY_SIZE];  /*!< NWK security key */
  zb_uint8_t          key_seq_number;        /*!< NWK security key sequence number */
} zb_secur_material_set_t;

/**
   TX counters window size.
 */
/* ZB_TX_STAT_WINDOW_SIZE must be bigger than ZB_TX_TOTAL_THRESHOLD, else  of
 * transmitted packets never reach ZB_TX_TOTAL_THRESHOLD in case of continuous
 * failed transfers. */
#define ZB_TX_STAT_WINDOW_SIZE (ZB_TX_TOTAL_THRESHOLD + 1U)

/**
   Data structure for TX statistics
 */
typedef struct zb_tx_stat_window_s
{
  zb_uint8_t tx_ok_cnts[ZB_TX_STAT_WINDOW_SIZE];
  zb_uint8_t tx_ok_used;
  zb_uint8_t tx_ok_i;
  zb_uint16_t tx_total;
  zb_uint16_t tx_fail;
} zb_tx_stat_window_t;

/**
  nwkMacInterfaceTable
*/
typedef ZB_PACKED_PRE struct zb_nwk_mac_iface_tbl_ent_s
{
  /* [0] */
  zb_bitfield_t index:5;        /**< A unique index that can be used to
                                 * identify an entry  */
  zb_bitfield_t state:1;        /**< State indicates the interface is enabled
                                 * or disabled  */
  zb_bitfield_t routers_allowed:1; /**< Indicates whether routers are allowed to
                                    * join to this device on this interface */
  zb_bitfield_t beacons_supported:1; /**< Indicates whether the current
                                      * interface supports beacons  */

  /* [1] */
  zb_bitfield_t ehn_beacons_supported:1; /**< Indicates whether the current
                                      * interface supports Enhanced beacons */
  zb_bitfield_t scan_type:1;    /**< The type of scan to be used when
                                 * performing a scan for NLME-NETWORK-DISCOVERY.req */
  zb_bitfield_t reserved:6;     /**< Fit into zb_uint16_t */

  /* [2] */
  zb_uint16_t link_power_data_rate; /**< The rate, in seconds, of how often a
                                     * Link Power Delta request is
                                     * generated. In bands where this is
                                     * optional, it should be set to 0,
                                     * disabling the function. The default
                                     * value should be 16.  */

  /* [4] */
  zb_channel_page_t channel_in_use;   /**< The current channel in use by the
                                       * device. Only a single channel may be
                                       * selected at one time. */

  /* [8] */
  zb_channel_list_t supported_channels; /**< Indicates the pages and channels that
                                         * are supported by this interface. */
  /* [8 + 4*{1 or 5}] */
  /* Total: 12 or 28 bytes */
}
ZB_PACKED_STRUCT zb_nwk_mac_iface_tbl_ent_t;

/**
   This is NWK NIB residential in memory.
   It is not clear yet when it will be save to nvram and when read.
*/
typedef struct zb_nib_s
{
  zb_uint16_t    passive_ack_timeout;    /*!< Maximum time duration (in octets) allowed for
					   the parent and all child to retransmit a broadcast message */
  zb_uint8_t     sequence_number;        /*!< A sequence number used to identify outgoing frames */
  zb_uint8_t     max_broadcast_retries;  /*!< The maximum number of retries allowed after a broadcast transmission failure. */
  zb_ext_pan_id_t  extended_pan_id;      /*!< Extended Pan ID for the PAN for which the device is a member */
  zb_nwk_device_type_t device_type;      /*!< Current device role, @see @ref nwk_device_type */
  zb_uint8_t     update_id;              /*!< nwkUpdateId - The value identifying a snapshot of the network settings with which this node is operating with. */
#if defined ZB_NWK_MESH_ROUTING && defined ZB_ROUTER_ROLE
#ifndef ZB_CONFIGURABLE_MEM
  zb_nwk_routing_t routing_table[ZB_NWK_ROUTING_TABLE_SIZE]; /*!< Routing table */
#else
  zb_nwk_routing_t *routing_table;
#endif
  zb_nwk_pend_t pending_table[ZB_NWK_PENDING_TABLE_SIZE];    /*!< store pending buffers while route discovery is in progress */
  zb_nwk_route_discovery_t route_disc_table[ZB_NWK_ROUTE_DISCOVERY_TABLE_SIZE]; /*!< Route discovery table */
#ifndef ZB_LITE_NO_NLME_ROUTE_DISCOVERY
  zb_uint16_t aps_rreq_addr;                                 /*!< APS layer call us to find path to this address */
#endif
  zb_uint8_t routing_table_cnt;                              /*!< Routing table used elements */
  zb_uint8_t route_disc_table_cnt;                           /*!< Discovery table used elements */

  zb_uint8_t rreq_id;                                        /*!< ID, increments each new route discovery procedure */
  zb_uint8_t pending_table_cnt;                              /*!< number of used elements inside pending buffer */
#endif /* ZB_NWK_MESH_ROUTING && ZB_ROUTER_ROLE */

  zb_uint8_t     max_depth;              /*!< The depth a device can have */

#ifdef ZB_ROUTER_ROLE
  zb_uint8_t     max_children;           /*!< The number of children a device is allowed to have */
  zb_uint8_t     router_child_num;       /*!< Number of child devices with router capability */
  zb_uint8_t     ed_child_num;           /*!< Number of child ed devices */
#endif

#if defined ZB_NWK_DISTRIBUTED_ADDRESS_ASSIGN && defined ZB_ROUTER_ROLE
  zb_uint8_t     max_routers;            /*!< The number of routers any one device is allowed to have as children. */
  zb_uint16_t    cskip;                  /*!< Cskip value - size of the address sub-block beeing distributed */
#endif
  zb_uint8_t     depth;                  /*!< current node depth */


  zb_secur_material_set_t secur_material_set[ZB_SECUR_N_SECUR_MATERIAL]; /*!< Set of network security
                                                                            material descriptors
                                                                            capable of maintaining
                                                                            an active and alternate
                                                                            network key.  */
  zb_uint8_t              active_key_seq_number; /*!< The sequence number of
                                                   the active network key in
                                                   nwkSecurityMaterialSet.  */
  zb_uint32_t             outgoing_frame_counter; /*!< OutgoingFrameCounter
                                                   * stored here (not in the
                                                   * secured material).
                                                   * Rationale: will never use
                                                   * "old" key - why store more
                                                   * then 1 counter?
                                                   */
  zb_uint32_t             prev_outgoing_frame_counter;


  zb_uint16_t nwk_manager_addr; /*!< The address of the designated
                                 * network channel manager function. */

#ifdef ZB_PRO_STACK
#ifndef ZB_LITE_NO_SOURCE_ROUTING
#ifndef ZB_CONFIGURABLE_MEM
  zb_nwk_rrec_t nwk_src_route_tbl[ZB_NWK_MAX_SRC_ROUTES];
#else
  zb_nwk_rrec_t *nwk_src_route_tbl;
#endif
  zb_uint8_t nwk_src_route_cnt;
  zb_bool_t nwk_is_concentrator;           /*!< A flag determining if this device is a concentrator */
  zb_uint8_t nwk_concentrator_radius;      /*!< The hop count radius for concentrator route discoveries */
  zb_uint32_t nwk_concentrator_disc_time;  /*!< The time in seconds between concentrator route discoveries. If set to 0x0000,
                                                the discoveries are done at start up and by the next higher layer only */
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
  zb_uint8_t link_status_period;  /*!< Table 3.44 NIB Attributes - nwkLinkStatusPeriod */
  zb_uint8_t router_age_limit;    /*!< Table 3.44 NIB Attributes - nwkRouterAgeLimit */
#endif
  /*zb_uint8_t dummy_8_bit;  ES: dummy 8 bit for aligning next bitfield for 32bit word. TI Compiler 18.1.3 at optimization_level=4 rewrite all word at assigning value to bitfield. */
  zb_bitfield_t leave_req_allowed:1; /*!< See: docs-11-5378-12-0csg-Zigbee-pro-errata-for-r20 NIB entry nwkLeaveRequestAllowed */
  zb_bitfield_t leave_req_without_rejoin_allowed:1; /* DA: parameter set to ignore leave requests w/o rejoin. Reason: R21 core stack spec. */
                                                       /* DA: please note, that I set default value to 1, can't find it in current version of spec */
#ifndef ZB_NO_NWK_MULTICAST
  zb_bitfield_t nwk_use_multicast:1;     /*!< Multicast determination flag */
#endif
#endif  /* ZB_PRO_STACK */

  zb_bitfield_t disable_rejoin:1; /*!< Forbid rejoin - for Rejoin request set Rejoin response with error status */
#ifdef ZB_NWK_TREE_ROUTING
  zb_bitfield_t use_tree_routing:1;      /*!< if device is able to use tree routing */
#endif
  zb_bitfield_t addr_alloc:1;            /*!< Address assign method see @ref nwk_address_allocation */

  zb_bitfield_t uniq_addr:1;             /*!< Table 3.44 NIB Attributes - nwkUniqueAddr */


#if defined (ZB_PRO_STACK)
  zb_bitfield_t reserve:4; /*!< There was security level defined in R22 (spec Table 3-58 NIB Attributes).
                                Currently, ZBOSS won't run without security.
                                So, there was a decision to remove possibility of changing security level.
                                Now, it can be defined only as constant value. */
#else
  zb_bitfield_t reserve:7;
#endif

  /* all_fresh is always 0 for Standard security */
  zb_bitfield_t           active_secur_material_i:2; /*!< index in
                                                      * secur_material_set for
                                                      * keys with
                                                      * key_seq_number == active_key_seq_number  */

  zb_bitfield_t           nwk_hub_connectivity:1; /*!< This indicates whether
                                                   * the router has Hub Connectivity as defined by a
                                                   * higher level application.
                                                   * The higher level application sets this value and the
                                                   * stack advertises it.
                                                   *
                                                   * see r23 spec, 3.5.2 NWK Information Base
                                                   */

  zb_bitfield_t           nwk_long_uptime:1; /*!< Custom attribute!
                                              *
                                              * Long Uptime is defined to be a device that has
                                              * been up for 24 hours or more. Short Uptime is
                                              * less than 24 hours
                                              *
                                              * see Zigbee WWAH Requirements,
                                              * C-20 Prioritized Parent Functionality
                                              */

  zb_bitfield_t nwk_report_constant_cost:1; /*!< If this is set to 0, the NWK
                                             * layer shall calculate link cost
                                             * from all neighbor nodes using the
                                             * LQI values reported by the MAC
                                             * layer; otherwise, it shall report
                                             * a constant value. */

#ifdef ZB_NWK_CONFIGURABLE_DST_IEEE_IN_HDR
  zb_bitfield_t           ieee_policy:1;     /*! If 1, put DST and SRC IEEE for NWK commands (it
                                              *  is mandatory by ZB spec). If 0, include
                                              *  it for all frame types.
                                              *  Default value is 0. */
#endif /* ZB_NWK_CONFIGURABLE_DST_IEEE_IN_HDR */

#ifdef SNCP_MODE
  zb_bitfield_t nwk_force_rrec_sending:1; /*!< If the flag is set to 1, local Zigbee Router device
                                            * considers it's working with Low-RAM concentrator
                                            * unless MTORR is received and sends Route record
                                            * before each packet */
#endif

  zb_tx_stat_window_t tx_stat;  /*!< TX/TX fail counters  */
  zb_uint8_t nwk_keepalive_modes;
  zb_uint8_t nwk_parent_information;
  zb_uint8_t nwk_ed_timeout_default;
/**
  The period for sending the keepalive to the router parent shall be determined by the
  * manufacturer of the device and is not specified by this standard. It is
  * recommended that the period allows the end device to send 3 keepalive
  * messages during the Device Timeout period. This will help insure that a
  * single missed keepalive message will not age out the end device on the router
  * parent.
 */
  zb_time_t nwk_ed_keepalive_timeout;

  zb_nwk_mac_iface_tbl_ent_t mac_iface_tbl[ZB_NWK_MAC_IFACE_TBL_SIZE]; /*!< nwkMacInterfaceTable */

  zb_uint8_t nwk_lpd_cmd_mode;  	/*!< Current method of link power delta negotiation.
                                         *   See: 3.4.13 Link Power Delta Command
                                         *   Table 3-56: Command Options: Type Values */
} zb_nib_t;

#ifdef ZB_NIB
#undef ZB_NIB
#endif
/**
  Function used to access NWK Information Base.
  @return pointer to NWK Information Base.
  */
zb_nib_t *zb_nwk_get_nib(void);
/**
  Macro used to access NWK Information Base.
  @return pointer to NWK Information Base.
  */
/*cstat !MISRAC2012-Rule-5.2_c99 !MISRAC2012-Rule-5.4_c99 */
/* The following violation of rules 5.2 and 5.4 seem to be false positive
 * At one time, only one macro is currently defined, but MISRA violations
 * 5.2 and 5.4 require at least two definitions simultaneously..
 * This violation seems to be caused by #undef usage, which cannot
 * be analyzed by C-STAT correctly. */
#define ZB_NIB() zb_nwk_get_nib()

/** @} */

#endif /* ZB_NWK_NIB_H */
