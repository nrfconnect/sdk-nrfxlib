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
/* PURPOSE: Network layer API
*/

#ifndef ZB_NWK_H
#define ZB_NWK_H 1

#include "zb_mac.h"
#include "zb_nwk_nib.h"
#include "zb_nwk_mm.h"
#include "zb_nwk_ed_aging.h"

/** \addtogroup nwk_api */
/** @{ */

/** @name NWK common constants and API.
 *  @{
 */

/**
 * @name NWK layer status values
 * @anchor nwk_status
 * @see ZB spec, clause 3.7
 */
/** @{ */
/** A request has been executed successfully. */
#define ZB_NWK_STATUS_SUCCESS                0x00U
/** An invalid or out-of-range parameter has been passed to a primitive from the next higher layer.
 */
#define ZB_NWK_STATUS_INVALID_PARAMETER      0xC1U
/** The next higher layer has issued a request that is invalid or cannot be executed given  the
 * current state of the  NWK layer. */
#define ZB_NWK_STATUS_INVALID_REQUEST        0xC2U
/** An NLME-JOIN.request has been  disallowed. */
#define ZB_NWK_STATUS_NOT_PERMITTED          0xC3U
/** An NLME-NETWORK-FORMATION.request has failed to start a network. */
#define ZB_NWK_STATUS_STARTUP_FAILURE        0xC4U
/** A device with the address supplied to the NLMEDIRECT-JOIN.request is already  present in the
 * neighbor table of the device on  which the NLMEDIRECT-JOIN.request was issued. */
#define ZB_NWK_STATUS_ALREADY_PRESENT        0xC5U
/** Used to indicate that an NLME-SYNC.request has failed at the MAC layer. */
#define ZB_NWK_STATUS_SYNC_FAILURE           0xC6U
/** An NLME-JOIN-DIRECTLY.request has failed because there is no more room in the neighbor table. */
#define ZB_NWK_STATUS_NEIGHBOR_TABLE_FULL    0xC7U
/** An NLME-LEAVE.request has failed because the device addressed in the parameter list is not in
 * the neighbor table of the issuing device. */
#define ZB_NWK_STATUS_UNKNOWN_DEVICE         0xC8U
/** An NLME-GET.request or NLME-SET.request has been  issued with an unknown attribute identifier.
 */
#define ZB_NWK_STATUS_UNSUPPORTED_ATTRIBUTE  0xC9U
/** An NLME-JOIN.request has been issued in an environment where no networks are detectable. */
#define ZB_NWK_STATUS_NO_NETWORKS            0xCAU
/** Security processing has been attempted on an outgoing frame, and has failed because the frame
 * counter has reached its maximum value. */
#define ZB_NWK_STATUS_MAX_FRM_COUNTER        0xCCU
/** Security processing has been attempted on an outgoing frame, and has failed because no key was
 * available with which to process it. */
#define ZB_NWK_STATUS_NO_KEY                 0xCDU
/** Security processing has been attempted on an outgoing frame, and has failed because the security
 * engine produced erroneous output. */
#define ZB_NWK_STATUS_BAD_CCM_OUTPUT         0xCEU
/** An attempt to discover a route has failed due to a lack of routing table or discovery table
 * capacity. */
#define ZB_NWK_STATUS_NO_ROUTING_CAPACITY    0xCFU
/** An attempt to discover a route has failed due to a reason other than a lack of routing capacity.
 */
#define ZB_NWK_STATUS_ROUTE_DISCOVERY_FAILED 0xD0U
/** An NLDE-DATA.request has failed due to a routing failure on the sending device. */
#define ZB_NWK_STATUS_ROUTE_ERROR            0xD1U
/** An attempt to send a broadcast frame or member mode multicast has failed due to the fact that
 * there is no room in the BTT. */
#define ZB_NWK_STATUS_BT_TABLE_FULL          0xD2U
/** An NLDE-DATA.request has failed due to insufficient buffering available. A non-member mode
 * multicast frame was discarded pending route discovery. */
#define ZB_NWK_STATUS_FRAME_NOT_BUFFERED     0xD3U
/** An attempt was made to use a MAC Interface with state that is currently set to FALSE (disabled)
 * or that is unknown to the stack. */
#define ZB_NWK_STATUS_INVALID_INTERFACE      0xD5U
/** @} */

/**
 * @brief Type for NWK layer status values.
 *
 * Holds one of @ref nwk_status. Kept only for backward compatibility as
 * @ref nwk_status were declared previously as enum.
 */
typedef zb_uint8_t zb_nwk_status_t;

/** @brief Multicast types mode. */
typedef enum zb_nwk_multicast_mode_e
{
  ZB_NWK_MULTICAST_MODE_NONMEMBER                    = 0x00, /**< Multicast non-member mode*/
  ZB_NWK_MULTICAST_MODE_MEMBER                       = 0x01  /**< Multicast member mode*/
}
zb_nwk_multicast_mode_t;

#ifdef ZB_APSDE_REQ_ROUTING_FEATURES
/**
 * @name NLDE non-spec extension values
 * @anchor nlde_tx_opt
 *
 * Note: These values were members of `enum zb_nlde_tx_opt_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
/** Non-spec extension: Force mesh route discovery */
#define ZB_NLDE_OPT_FORCE_MESH_ROUTE_DISC (1U << 0)
/** Non-spec extension: Force send route record */
#define ZB_NLDE_OPT_FORCE_SEND_ROUTE_REC (1U << 1)
/** Non-spec extension: Force send route record. Auxillary bitfield for marking route as many-to-one
 * for force rrec sending */
#define ZB_NLDE_OPT_TEMPORARY_MARK_ROUTE_AS_MTO (1U << 2)
/** @} */

/**
 * @brief Type for possible values of NLDE non-spec extension values.
 *
 * @deprecated holds one of @ref nlde_tx_opt. Kept only for backward
 * compatibility as @ref nlde_tx_opt were declared previously as enum. Can be
 * removed in future releases.
 */
typedef zb_uint8_t zb_nlde_tx_opt_e;
#endif

/** @brief 'frame security failed' status mentioned in ZB spec, subclause 4.3.1.2.
  *
  * Security Processing of Incoming Frames but not defined in the table 3.42   Status Codes for
  * Network Status Command Frame
  *
  * Really need this status for intra-pan portability procedure (AZD601,602). Let's use other
  * security status code.
  */
#define ZB_NWK_COMMAND_STATUS_FRAME_SECURITY_FAILED  ZB_NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER
/** @brief Check that NWK command status is security-related.
  * @param st - status code
  * @return 1 if NWK command status is security-related
  */
#define ZB_NWK_COMMAND_STATUS_IS_SECURE(st)                   \
  ((st) == ZB_NWK_COMMAND_STATUS_BAD_FRAME_COUNTER || (st) == \
      ZB_NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER || (st) == \
      ZB_NWK_COMMAND_STATUS_FRAME_SECURITY_FAILED)

/** @} */ /* NWK common constants and API. */

/**
 *  @name NWK data service.
 *  @{
 */

/** @brief Parameters for NLDE-DATA.request primitive. */
typedef struct zb_nlde_data_req_s
{
  zb_uint16_t  dst_addr;        /**< Destination address.  */
  zb_uint8_t   radius;          /**< The distance, in hops, that a frame will be allowed to travel
                                  * through the network. */
  zb_uint8_t addr_mode;    /**< The type of destination address supplied by the DstAddr parameter,
                             * see @ref address_modes  */
  zb_uint8_t nonmember_radius; /**< The distance, in hops, that a multicast frame will be relayed
                                 * by nodes not a member of the group. A value of 0x07 is treated
                                 * as infinity.  */
  zb_uint8_t discovery_route; /**< The DiscoverRoute parameter may be used to control route
                                * discovery operations for the transit of this frame (see
                                * sub-clause3.6.3.5): 0x00 = suppress route discovery 0x01 = enable
                                * route discovery  */
  zb_uint8_t security_enable; /**< The SecurityEnable parameter may be used to enable NWK layer
                                * security processing for the current frame. If the
                                * nwkSecurityLevel attribute of the NIB has a value of 0, meaning
                                * no security, then this parameter will be ignored.  Otherwise,
                                * a value of TRUE denotes that the security processing specified by
                                * the security level will be applied, and a value of FALSE denotes
                                * that no security processing will be applied.  */
  zb_uint8_t ndsu_handle; /**< The handle associated with the NSDU to be transmitted by the NWK
                            * layer entity. */
  /*3.2.1.1.1 spec r21*/
  /* 14/06/2016 CR [AEV] start */
#ifdef ZB_USEALIAS
  zb_uint8_t use_alias;   /**<The next higher layer MAY use the UseAlias
			   * parameter to request alias usage by NWK
			   * layer for the current frame. If the UseAlias
			   * parameter has a value of FALSE, meaning no
			   * alias usage,
			   * then the parameters AliasSrcAddr and AliasSeqNumb will be ignored.
			   * Otherwise, a value of TRUE denotesthat the
			   * values supplied in AliasSrcAddr and AliasSeqNumb are to be used.
			   * Values: TRUE or FALSE
			   */
  zb_uint16_t alias_src_addr;/**The source address to be used for this NSDU.
			      *	If the UseAlias parameter has a value of
			      *	FALSE, the AliasSrcAddr parameter is ignored.
			      * Values: Any valid device address except a broadcast address
			      */
  zb_uint8_t alias_seq_num;  /**The sequence number to be used for this
			      * NSDU. If the UseAlias parameter has a value
			      * of FALSE, the AliasSeqNumb parameter is ignored.
			      * Values: 0x00-0xff
			      */
#endif /*ZB_USEALIAS*/
  /* 14/06/2016 CR [AEV] end */
#ifdef ZB_APSDE_REQ_ROUTING_FEATURES
  zb_uint8_t extension_flags; /** The field for extension flags storing:
                               *  - force mesh route discovery
                               *  - force send route record
                               */
#endif
} zb_nlde_data_req_t;

/** @brief NLDE-DATA.request primitive.
  *
  * This function return immediately.
  *
  * Later @ref zb_nlde_data_confirm will be called to pass NLDE-DATA.request result up.
  * @param param - parameters structure, see @ref zb_nlde_data_req_t. This variable does not pass
  * to other levels, so it can be local variable in the caller.
  * @b Example:
  * @code
  *   {
  *     zb_nlde_data_req_t *req;
  *     zb_uint16_t dst_addr;
  *
  *     req = ZB_GET_BUF_TAIL(buf, sizeof(zb_nlde_data_req_t));
  *     /-- send to parent --/
  *     zb_address_short_by_ref(&dst_addr, ZG->nwk.handle.parent);
  *     TRACE_MSG(
  *         TRACE_APS3, "parent %hd parent_addr %d", (FMT__H_D, ZG->nwk.handle.parent, dst_addr));
  *
  *     req->dst_addr = dst_addr;
  *     req->radius = 0; /-- use default --/
  *     req->addr_mode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
  *     req->discovery_route = 0;
  *     req->security_enable = 0;
  *     req->ndsu_handle = 10;
  *
  *     TRACE_MSG(TRACE_APS3, "Sending nlde_data.request", (FMT__0));
  *     ZB_SCHEDULE_CALLBACK(zb_nlde_data_request, ZB_REF_FROM_BUF(buf));
  *   }
  * @endcode
  */
void zb_nlde_data_request(zb_uint8_t param);

/** @brief NLDE-DATA.confirm primitive.
  *
  * This function called via scheduler by the NWK layer to indicate NLDE-DATA.request result to the
  * APS layer.
  * @note zb_nlde_data_confirm must be defined in the APS layer!
  * NWK layer just calls this function.  NWK and lower lagers does not free nsdu. APS must do it (or it can reuse
  * it, transmit to the another address etc).
  * @param param - sent packet, see @ref zb_buf_t. APS must free or reuse it. Following packet
  * fields are used:
  * @li status - The status of the corresponding request. Can take values: INVALID_REQUEST,
  * MAX_FRM_COUNTER, NO_KEY, BAD_CCM_OUTPUT, ROUTE_ERROR, BT_TABLE_FULL, FRAME_NOT_BUFFERED or any
  * status values returned from security suite or the MCPS-DATA.confirm primitive.
  * @li handle - The handle associated with the NSDU being confirmed.
 */
void zb_nlde_data_confirm(zb_uint8_t param);

/** @brief NLDE-DATA.indication primitive.
  *
  * This function called via scheduler by the NWK layer to pass incoming data packet to the APS
  * layer.
  * @note zb_nlde_data_indication() must be defined in the APS layer! WK layer just calls this function.
  * @param param - The set of octets comprising the NSDU to be transferred (with length).
  *
  * Other fields got from MAC nsdu by macros
  */
void zb_nlde_data_indication(zb_uint8_t param);

/** @} */ /* NWK data service. */

/**
 *  @name NWK management service.
 *  @{
 */

/** @brief Arguments of the NLME-GET.request routine. */
typedef ZB_PACKED_PRE struct zb_nlme_get_request_s
{
  zb_nib_attribute_t nib_attribute; /**< Attribute value, see @ref zb_nib_attribute_t. */
  zb_callback_t      confirm_cb; /*!< User's callback to be called as NLME-GET.confirm  */
} ZB_PACKED_STRUCT
zb_nlme_get_request_t;

/** @brief NLME-GET.request primitive.
  *
  * Perform get NIB attribute.
  * @param param - buffer containing parameters, see @ref zb_nlme_get_request_s.
  * @return RET_OK on success, error code otherwise.
  *
  * @snippet start_ze.c zb_nlme_get_request
  *
  */
void zb_nlme_get_request(zb_uint8_t param);

/** @brief Arguments of the NLME-GET.confirm routine. */
typedef ZB_PACKED_PRE struct zb_nlme_get_confirm_s
{
  zb_nwk_status_t status; /**< The result of the operation. */
  zb_nib_attribute_t nib_attribute;  /**< Attribute value, see @ref zb_nib_attribute_t. */
  zb_uint16_t attribute_length; /**< Length of the attribute's value. */
  /* next is attribute value */
} ZB_PACKED_STRUCT
zb_nlme_get_confirm_t;

/** @brief Arguments of the NLME-SET.request routine. */
typedef ZB_PACKED_PRE struct zb_nlme_set_request_s
{
  zb_nib_attribute_t nib_attribute; /**< Attribute value, see @ref zb_nib_attribute_t. */
  zb_uint16_t        attr_length;          /**< Size of nib_attribute.  */
  zb_callback_t      confirm_cb; /*!< User's callback to be called as NLME-SET.confirm  */
} ZB_PACKED_STRUCT
zb_nlme_set_request_t;

/** @brief NLME-SET.request primitive.
  *
  * Perform set NIB attribute.
  * @param param - buffer containing parameters, see @ref zb_nlme_set_request_s.
  * @return RET_OK on success, error code otherwise.
  *
  * @snippet start_ze.c zb_nlme_set_request
  *
  */
void zb_nlme_set_request(zb_uint8_t param);

/** @brief Arguments of the NLME-SET.confirm routine. */
typedef ZB_PACKED_PRE struct zb_nlme_set_confirm_s
{
  zb_nwk_status_t status;           /**< The result of the operation. */
  zb_nib_attribute_t nib_attribute; /**< Attribute value, see @ref zb_nib_attribute_t. */
} ZB_PACKED_STRUCT
zb_nlme_set_confirm_t;

/**
   Network discovery request parameters.
   This structure describe parameters passed to
   zb_nlme_network_discovery_request call.

   This structure is not packed intentionally: scan_channels_list should be aligned
*/
typedef struct zb_nlme_network_discovery_request_s
{
  zb_channel_list_t scan_channels_list; /**< A list of channel pages and the channels
                                                               * within those pages that the discovery shall
                                                               * be performed upon. */
  zb_uint8_t        scan_duration;                            /**< Time to spend scanning each channel */
}
zb_nlme_network_discovery_request_t;

/**
   NLME-NETWORK-DISCOVERY.request primitive

   This function return immediately.
   Later zb_nlme_network_discovery_confirm will be called to pass NLME-NETWORK-DISCOVERY.confirm result up.

   @param param - buffer containing parameters - @see
   zb_nlme_network_discovery_request_t
   @return nothing
 */
void zb_nlme_network_discovery_request(zb_uint8_t param);


void zb_nwk_cancel_network_discovery(zb_bufid_t buf);


/** @brief Network descriptor.
 *  This is a part of network discovery confirm result.
 */
typedef ZB_PACKED_PRE struct zb_nlme_network_descriptor_s
{
  zb_bitfield_t panid_ref:7; /**< Reference to extended Pan ID
                              * of the network. Was zb_address_pan_id_ref_t.
                              * Must be big enough to hold ZB_PANID_TABLE_SIZE.
                              * ZB_PANID_TABLE_SIZE is now 16, so 7 is more than enough.
                              */
  zb_bitfield_t    channel_page:5; /**< channel page. in r22 0 to 31  */
  zb_bitfield_t    logical_channel:6; /**< The current logical channel occupied by
                                       * the network. In r22 0 to 63 */
  /* use bitfields to fit descriptors array to the single buffer */
  zb_bitfield_t stack_profile:2; /**< Stack profile identifier. 4 bits by
                                  * standard, but need 2 bits
                                  * actually. */
#if 0
  /* Defined by standard, but not actually required for us */
  zb_bitfield_t Zigbee_version:4; /**< The version of the Zigbee protocol */
  zb_bitfield_t beacon_order:4; /**< How often the MAC sub-layer beacon is to
                                 * be transmitted */
  zb_bitfield_t superframe_order:4; /**< The length of the active period of the
                                     * superframe  */
#endif
  zb_bitfield_t permit_joining:1; /**< Indicates that at least one router on
                                   * the network currently permits joining */
  zb_bitfield_t router_capacity:1; /**< True if device is capable of accepting
                                    * join requests from router-capable devices */
  zb_bitfield_t end_device_capacity:1; /**< True if device is capable of accepting
                                    * join requests from end devices */
  zb_bitfield_t processed:1; /**< True if record is processed */
  zb_uint8_t    nwk_update_id;
} ZB_PACKED_STRUCT
zb_nlme_network_descriptor_t;

/** @brief Arguments of the NLME-NETWORK-DISCOVERY.confirm routine. */
typedef ZB_PACKED_PRE struct zb_nlme_network_discovery_confirm_s
{
  zb_ret_t status;  /**< Our error or MAC status codes (see Table 3-11
                     * NLME-NETWORK-DISCOVERY.confirm Parameters:
                     * Any status value returned with the
                     * MLME-SCAN.confirm primitive.) */
  zb_uint8_t network_count; /**< Number of discovered networks */
  /* next here is an array of zb_nlme_network_descriptor_t */
} ZB_PACKED_STRUCT
zb_nlme_network_discovery_confirm_t;

/** @brief NLME-NETWORK-DISCOVERY.confirm primitive.
 *
 * Report the results of the network discovery operation.
 * @param param - buffer containing results - see @ref zb_nlme_network_discovery_confirm_s.
 * @note This structure is passed as data, not as parameter!
 * @b Rationale: it has variable size, so it is
 * not easy to define where it should begin in parameter.
 * @return RET_OK on success, error code otherwise.
 *
 * @code
 * ((zb_bufid_t )ZB_BUF_FROM_REF(param))->u.hdr.status = RET_BUSY;
 * ZB_SCHEDULE_CALLBACK(zb_nlme_network_discovery_confirm, param);
 * @endcode
 */
void zb_nlme_network_discovery_confirm(zb_uint8_t param);

/**
   Arguments of the NLME-NETWORK-FORMATION.request routine.
*/
typedef struct zb_nlme_network_formation_request_s
{
  zb_channel_list_t scan_channels_list; /**< The list of all channel pages and
                                                               * the associated channels that shall be scanned. */
  zb_uint8_t scan_duration; /**< Time to spend scanning each channel */
  zb_uint8_t distributed_network;
  zb_uint16_t distributed_network_address;
  zb_ieee_addr_t extpanid;
#if 0                       /* not supported by mac anyway */
  zb_uint8_t beacon_order; /**< The beacon order */
  zb_uint8_t superframe_order; /**< The superframe order */
  zb_uint8_t battery_life_extension; /**< If true - start support battery
                                           * life extension */
#endif
}
zb_nlme_network_formation_request_t;

#ifdef ZB_FORMATION
/**
   NLME-NETWORK-FORMATION.request primitive

   Starting new Zigbee network with itself as a coordinator.

   @param param - buffer containing parameters - @see
   zb_nlme_network_formation_request_t
   @return RET_OK on success, error code otherwise.

   @snippet zdo_app.c zb_nlme_network_formation_request
 */
void zb_nlme_network_formation_request(zb_uint8_t param);

void zb_nwk_cancel_network_formation(zb_bufid_t param);

#endif /* ZB_FORMATION */

/**
   Arguments of the NLME-NETWORK-FORMATION.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_network_formation_confirm_s
{
  zb_nwk_status_t status; /**< MAC status codes */
} ZB_PACKED_STRUCT
zb_nlme_network_formation_confirm_t;

#ifdef ZB_FORMATION
/**
   NLME-NETWORK-FORMATION.confirm primitive

   Report the results of the network formation request.

   @param param - buffer containing results - @see
   zb_nlme_network_formation_confirm_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_network_formation_confirm(zb_uint8_t param);
#endif /* ZB_FORMATION */

/**
   Arguments of the NLME-NETWORK-FORMATION.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_permit_joining_confirm_s
{
  zb_mac_status_t status; /**< MAC status codes */
} ZB_PACKED_STRUCT
zb_nlme_permit_joining_confirm_t;

/**
   NLME-PERMIT-JOINING.confirm primitive

   Report the results of the permit joining request.

   @param param - buffer containing results - @see
   zb_nlme_permit_joining_confirm_t
   @return RET_OK on success, error code otherwise.

   @snippet nwk_cr_permit_join.c zb_nlme_permit_joining_confirm
 */
void zb_nlme_permit_joining_confirm(zb_uint8_t param);

/**
   Arguments of the NLME-START-ROUTER.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_start_router_request_s
{
  zb_uint8_t beacon_order;                  /**< The beacon order */
  zb_uint8_t superframe_order;              /**< The superframe order */
  zb_uint8_t battery_life_extension;        /**< If true - start support battery
                                           * life extension */
} ZB_PACKED_STRUCT
zb_nlme_start_router_request_t;

/**
   NLME-START-ROUTER.request primitive

   Start router activity

   @param param - buffer containing parameters - @see
   zb_nlme_start_router_request_t
   @return RET_OK on success, error code otherwise.

   @snippet nwk_route_disc/nwk_route_discovery.c zb_nlme_start_router_request

 */
void zb_nlme_start_router_request(zb_uint8_t param);

/**
   Arguments of the NLME-START-ROUTER.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_start_router_confirm_s
{
  zb_mac_status_t status; /**< MAC status codes */
} ZB_PACKED_STRUCT
zb_nlme_start_router_confirm_t;

/**
   NLME-START-ROUTER.confirm primitive

   Report the results of the start router request.

   @param param - buffer containing results - @see
   zb_nlme_start_router_confirm_t
   @return RET_OK on success, error code otherwise.

   @code
     NWK_CONFIRM_STATUS((zb_bufid_t )ZB_BUF_FROM_REF(param), ZB_NWK_STATUS_INVALID_REQUEST, zb_nlme_start_router_confirm);
   @endcode
 */
void zb_nlme_start_router_confirm(zb_uint8_t param);

/**
   Channel energy info.
*/
typedef ZB_PACKED_PRE struct zb_energy_detect_channel_info_s
{
  zb_bitfield_t channel_page_idx : 3;
  zb_bitfield_t channel_number : 5; /*!< bit # in the channel mask. Note: not a logical channel!  */
  zb_uint8_t energy_detected;
} ZB_PACKED_STRUCT zb_energy_detect_channel_info_t;

/**
   The structure is used to convey energy values for each channel that was scanned.
*/
typedef ZB_PACKED_PRE struct zb_energy_detect_list_s
{
  zb_uint8_t channel_count;
  zb_energy_detect_channel_info_t channel_info[ZB_ED_SCAN_MAX_CHANNELS_COUNT];
} ZB_PACKED_STRUCT zb_energy_detect_list_t;

/**
   Arguments of the NLME-ED-SCAN.request routine.
*/
typedef struct zb_nlme_ed_scan_request_s
{
  zb_channel_page_t scan_channels_list[ZB_CHANNEL_PAGES_NUM]; /**< The list of all channel pages and the
                                                               * associated channels that shall be scanned. */
  zb_uint8_t scan_duration;                                   /**< Time to spend scanning each channel */
}
zb_nlme_ed_scan_request_t;


/* ED Scan functions were under ZB_ROUTER_ROLE ifdef,
 * but were switched on for all devices types (for WWAH, PICS item AZD514) */

/**
   NLME-ED_SCAN.request primitive

   Start energy scan

   @param param - buffer containing parameters - @see
   zb_nlme_ed_scan_request_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_ed_scan_request(zb_uint8_t param);

/**
   Arguments of the NLME-ED-SCAN.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_ed_scan_confirm_s
{
  zb_mac_status_t status;               /**< MAC status codes */
  zb_uint32_t unscanned_channels;       /**< Indicate not scanned channels */
  /* next is the list of zb_uint8_t describes energy measurements */
} ZB_PACKED_STRUCT
zb_nlme_ed_scan_confirm_t;

/**
   NLME-ED-SCAN.confirm primitive

   Report the results of the ed scan request.

   @param param - buffer containing results - @see
   zb_nlme_ed_scan_confirm_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_ed_scan_confirm(zb_uint8_t param);


/**
 *  @brief Parameters of the custom NLME-BEACON-SURVEY.request
 */
typedef ZB_PACKED_PRE struct zb_nlme_beacon_survey_scan_request_s
{
  /*!< Channel mask */
  zb_uint32_t channel_mask;
  /*!< Channel page */
  zb_uint8_t channel_page;
  /*!< Scan type of the 'Beacon Survey' procedure (see @mac_scan_type) */
  zb_uint8_t scan_type;
}
ZB_PACKED_STRUCT zb_nlme_beacon_survey_scan_request_t;

/**
 *  @brief Starts of the Beacon Survey scan
 *  @param param - buffer containing
 *         @zb_nlme_beacon_survey_scan_request_t
 */
void zb_nlme_beacon_survey_scan(zb_uint8_t param);

#if defined ZB_BEACON_SURVEY && defined ZB_ZCL_ENABLE_WWAH_SERVER
/**
 *  @brief Report the results of the Beacon Survey scan
 *  @param param - buffer containing @zb_mac_scan_confirm_t
 */
void zb_nlme_beacon_survey_scan_confirm(zb_uint8_t param);
#endif /* ZB_BEACON_SURVEY && ZB_ZCL_ENABLE_WWAH_SERVER */

/**
   Network join method.
*/
/**
 * @name Network join method
 * @anchor nlme_rejoin_method
 */
/** @{ */
#define ZB_NLME_REJOIN_METHOD_ASSOCIATION    0x00U /**< Throught association */
#define ZB_NLME_REJOIN_METHOD_DIRECT         0x01U /**< Join directly or rejoining using the orphaning */
#define ZB_NLME_REJOIN_METHOD_REJOIN         0x02U /**< Using NWK rejoin procedure */
#define ZB_NLME_REJOIN_METHOD_CHANGE_CHANNEL 0x03U /**< Changing the network channel  */
/** @} */

/**
 * @brief Type for network join method.
 *
 * Holds one of @ref nlme_rejoin_method. Kept only for backward compatibility as
 * @ref nlme_rejoin_method were declared previously as enum. Can be removed in future releases.
 */
typedef zb_uint8_t zb_nlme_rejoin_method_t;

/**
   Arguments of the NLME-JOIN.request routine.
*/
typedef struct zb_nlme_join_request_s
{
  zb_ext_pan_id_t extended_pan_id; /**< Extended Pan ID
                                                        * of the network */
  zb_channel_list_t scan_channels_list; /**< The list of all channel pages and
                                                               * the associated channels that shall be scanned. */
  zb_mac_capability_info_t capability_information; /**< The operating capabilities of the
                                      * device */
  zb_nlme_rejoin_method_t rejoin_network; /**< Join network method @see @ref nlme_rejoin_method */
  zb_uint8_t scan_duration; /**< Time to spend scanning each channel */
  zb_uint8_t security_enable;   /*!< If the value of RejoinNetwork is 0x02 and this is TRUE than the device will try to rejoin securely.
                                  Otherwise, this is set to FALSE.  */

  /* There was insecure_rejoin: Insecure rejoining. Originally it was "secure_rejoin,
                                * as specified in the spec. But because by default it is set to
                                * ZB_FALSE (zero), let's handle it as "insecure" flag. */
}
zb_nlme_join_request_t;

/**
   NLME-JOIN.request primitive

   Join to the network

   @param param - buffer containing parameters - @see
   zb_nlme_join_request_t
   @return RET_OK on success, error code otherwise.

   @snippet nwk_leave/zdo_start_zr.c zb_nlme_join_request

 */
void zb_nlme_join_request(zb_uint8_t param);

/**
   Arguments of the NLME-JOIN.indication routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_join_indication_s
{
  zb_uint16_t network_address;                      /**< The network address of an entity */
  zb_ieee_addr_t extended_address;                  /**< 64-bit IEEE address of an entity */
  zb_mac_capability_info_t capability_information;  /**< The operating capabilities of the
                                                     * device */
  zb_nlme_rejoin_method_t rejoin_network;           /**< Join network method @see
                                                     * @ref nlme_rejoin_method */
  zb_uint8_t secure_rejoin;                         /**< Secure joining */
} ZB_PACKED_STRUCT
zb_nlme_join_indication_t;

/**
   NLME-JOIN.indication primitive

   Notify about new joined device.

   @param param - buffer containing parameters - @see
   zb_nlme_join_indication_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_join_indication(zb_uint8_t param);

/**
   Arguments of the NLME-JOIN.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_join_confirm_s
{
  zb_ret_t status;                           /**< MAC or NWK status codes */
  zb_uint16_t network_address;                      /**< Allocated network address */
  zb_ext_pan_id_t extended_pan_id;                  /**< Extended Pan ID
                                                     * of the network */
  zb_uint8_t active_channel;                        /**< Current network channel */
} ZB_PACKED_STRUCT
zb_nlme_join_confirm_t;

/**
   NLME-JOIN.confirm primitive

   Report the results of the join request.

   @param param - buffer containing results - @see zb_nlme_join_confirm_t
   @return RET_OK on success, error code otherwise.

   @snippet doxygen_snippets.dox zb_nlme_join_confirm_nwk_nwk_join_c
 */
void zb_nlme_join_confirm(zb_uint8_t param);

/**
   Arguments of the NLME-DIRECT-JOIN.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_direct_join_request_s
{
  zb_ieee_addr_t device_address;                    /**< 64-bit IEEE address of the device to be directly joined */
  zb_mac_capability_info_t capability_information;  /**< The operating capabilities of the device */
} ZB_PACKED_STRUCT
zb_nlme_direct_join_request_t;

#if defined ZB_ENABLE_ZLL && defined ZB_ROUTER_ROLE
/**
   NLME-DIRECT-JOIN.request primitive

   Directly Join another device to the network

   @param param - buffer containing parameters - @see
   zb_nlme_direct_join_request_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_direct_join_request(zb_uint8_t param);
#endif /* ZB_ENABLE_ZLL && ZB_ROUTER_ROLE */

/**
   Arguments of the NLME-DIRECT-JOIN.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_direct_join_confirm_s
{
  zb_nwk_status_t status;           /**< MAC status codes */
  zb_ieee_addr_t device_address;    /**< 64-bit IEEE address */
} ZB_PACKED_STRUCT
zb_nlme_direct_join_confirm_t;

#if defined ZB_ENABLE_ZLL && defined ZB_ROUTER_ROLE
/**
   NLME-DIRECT-JOIN.confirm primitive

   Report the results of the direct join request.

   @param param - buffer containing results - @see
   zb_nlme_direct_join_confirm_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_direct_join_confirm(zb_uint8_t param);
#endif /* ZB_ENABLE_ZLL && ZB_ROUTER_ROLE */

/**
   Arguments of the NLME-LEAVE.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_leave_request_s
{
  zb_ieee_addr_t device_address; /**< 64-bit IEEE address of the device to
                                  * remove, zero fill if device itself */
  zb_uint8_t remove_children; /**< If true - remove child devices from the
                                    * network */
  zb_uint8_t rejoin; /**< If true - Join after leave */
} ZB_PACKED_STRUCT
zb_nlme_leave_request_t;

/**
   NLME-LEAVE.request primitive

   Leave the network

   @param param - buffer containing parameters - @see
   zb_nlme_leave_request_t
   @return RET_OK on success, error code otherwise.

   @snippet tp_pro_bv-67_zc.c zb_nlme_leave_request
 */
void zb_nlme_leave_request(zb_uint8_t param);

/**
   NLME-LEAVE.indication primitive

   Notify about leave device

   @param param - buffer containing parameters - @see
   zb_nlme_leave_indication_t
   @return RET_OK on success, error code otherwise.

   @code
    zb_nlme_leave_indication_t *request = NULL;
    request = ZB_BUF_GET_PARAM(ZB_BUF_FROM_REF(param), zb_nlme_leave_indication_t);

    if (addr_ref == (zb_address_ieee_ref_t)-1)
    {
      ZB_IEEE_ADDR_ZERO(request->device_address);
    }
    else
    {
      zb_address_ieee_by_ref(request->device_address, addr_ref);
    }
    request->rejoin = rejoin;
    ZB_SCHEDULE_CALLBACK(zb_nlme_leave_indication, param);
   @endcode
 */
void zb_nlme_leave_indication(zb_uint8_t param);

/**
   Arguments of the NLME-LEAVE.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_leave_confirm_s
{
  zb_nwk_status_t status;           /**< MAC status codes */
  zb_ieee_addr_t device_address;    /**< 64-bit IEEE address */
} ZB_PACKED_STRUCT
zb_nlme_leave_confirm_t;

/**
   NLME-LEAVE.confirm primitive

   Report the results of the direct join request.

   @param param - buffer containing results - @see
   zb_nlme_leave_confirm_t
   @return RET_OK on success, error code otherwise.

   @code
      zb_nlme_leave_confirm_t *lc = ZB_BUF_GET_PARAM(buf, zb_nlme_leave_confirm_t);
      lc->status = (zb_nwk_status_t)buf->u.hdr.status;
      if (dest_addr != 0xffff)
      {
        zb_address_ieee_by_short(dest_addr, lc->device_address);
      }
      else
      {
        ZB_IEEE_ADDR_ZERO(lc->device_address);
      }
      ZB_SCHEDULE_CALLBACK(zb_nlme_leave_confirm, param);
   @endcode
 */
void zb_nlme_leave_confirm(zb_uint8_t param);

/**
   Arguments of the NLME-RESET.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_reset_request_s
{
  zb_bool_t warm_start; /**< if false - reset all stack values */
} ZB_PACKED_STRUCT
zb_nlme_reset_request_t;

/**
   NLME-RESET.request primitive

   Perform reset operation

   @param param - buffer containing parameters - @see
   zb_nlme_reset_request_t
   @return RET_OK on success, error code otherwise.

   @snippet zdo_app.c zb_nlme_reset_request
 */
void zb_nlme_reset_request(zb_uint8_t param);

/**
   Arguments of the NLME-RESET.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_reset_confirm_s
{
  zb_nwk_status_t status; /**< The result of the operation */
} ZB_PACKED_STRUCT
zb_nlme_reset_confirm_t;

/**
   NLME-RESET.confirm primitive

   Report the results of the reset request.

   @param param - buffer containing results - @see
   zb_nlme_reset_confirm_t
   @return RET_OK on success, error code otherwise.

   @code
       NWK_CONFIRM_STATUS(buf, ZB_NWK_STATUS_INVALID_REQUEST, zb_nlme_reset_confirm);
   @endcode
 */
void zb_nlme_reset_confirm(zb_uint8_t param);

/**
   Arguments of the NLME-SYNC.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_sync_request_s
{
  zb_uint8_t track; /**< Whether ot not the sync should be maintained for
                     * future beacons */
  zb_time_t  poll_rate; /*!< MAC poll rate */
} ZB_PACKED_STRUCT
zb_nlme_sync_request_t;

/**
   NLME-SYNC.request primitive

   Perform sync operation

   @param param - buffer containing parameters - @see
   zb_nlme_sync_request_t
   @return RET_OK on success, error code otherwise.

   @snippet nwk_sync_ze.c zb_nlme_sync_request
 */
void zb_nlme_sync_request(zb_uint8_t param);

/**
   Arguments of the NLME-SYNC.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_sync_confirm_s
{
  zb_nwk_status_t status; /**< The result of the operation */
} ZB_PACKED_STRUCT
zb_nlme_sync_confirm_t;

/**
   NLME-SYNC.confirm primitive

   Report the results of the sync request.

   @param param - buffer containing results - @see
   zb_nlme_sync_confirm_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_sync_confirm(zb_uint8_t param);

/**
   NLME-STATUS.indication primitive

   Notifies about network fails.

   @param param - buffer containing parameters - @see
   zb_nlme_status_indication_t
   @return RET_OK on success, error code otherwise.

   @code
    zb_nlme_status_indication_t *cmd = ZB_BUF_GET_PARAM(ZB_BUF_FROM_REF(param), zb_nlme_status_indication_t);
    cmd->status = (zb_nwk_command_status_t)status;
    cmd->network_addr = addr_short;
    ZB_SCHEDULE_CALLBACK(zb_nlme_status_indication, param);
   @endcode
 */
void zb_nlme_status_indication(zb_uint8_t param);

/**
   Arguments of the NLME-ROUTE-DISCOVERY.request routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_route_discovery_request_s
{
  zb_addr_mode_t address_mode;  /**< Kind of destination address provided, one of @ref address_modes */
  zb_uint16_t network_addr;     /**< The destination of the route discovery */
  zb_uint8_t radius;            /**< Number of hopes */
  zb_bool_t no_route_cache;    /**< True - no route table should be
                                 * established */
} ZB_PACKED_STRUCT
zb_nlme_route_discovery_request_t;

#ifndef ZB_LITE_NO_NLME_ROUTE_DISCOVERY
/**
   NLME-ROUTE-DISCOVERY.request primitive

   Initiate a route discovery

   @param param - buffer containing parameters - @see
   zb_nlme_route_discovery_request_t
   @return RET_OK on success, error code otherwise.

   @snippet tp_pro_bv_08_zc.c zb_nlme_route_discovery_request
 */
void zb_nlme_route_discovery_request(zb_uint8_t param);

/**
   Arguments of the NLME-ROUTE-DISCOVERY.confirm routine.
*/
typedef ZB_PACKED_PRE struct zb_nlme_route_discovery_confirm_s
{
  zb_uint8_t status;            /**< @see nwk_status indeed  */
} ZB_PACKED_STRUCT
zb_nlme_route_discovery_confirm_t;

/**
   NLME-ROUTE-DISCOVERY.confirm primitive

   Report the results of the route discovery

   @param param - buffer containing results - @see
   zb_nlme_route_discovery_confirm_t
   @return RET_OK on success, error code otherwise.
 */
void zb_nlme_route_discovery_confirm(zb_uint8_t param);
#endif  /* #ifndef ZB_LITE_NO_NLME_ROUTE_DISCOVERY */

/** @} */ /* NWK management service. */

/** @cond internals_doc */

/** @brief Arguments of the NLME-SEND-STATUS.confirm routine. */
typedef ZB_PACKED_PRE struct zb_nlme_send_status_s
{
  zb_uint16_t dest_addr; /**< address to send status information to */
  zb_nlme_status_indication_t status; /**< status information, @ref zb_nlme_status_indication_s */
  zb_uint8_t ndsu_handle; /**<  The handle associated with the NSDU to be transmitted by the NWK
                            *   layer entity */
} ZB_PACKED_STRUCT
zb_nlme_send_status_t;


/** @brief Send status indication primitive.
  *
  * Send status to the remote device.
  * @param param - request params, see @ref zb_nlme_send_status_s.
  * @return nothing.
  * @b Example:
  * @code
  * {
  *   zb_nlme_send_status_t *request = ZB_BUF_GET_PARAM(ZB_BUF_FROM_REF(param), zb_nlme_send_status_t);
  *
  *   request->dest_addr = 0; /-- send status indication to the coordinator --/
  *   request->status.status = ZB_NWK_COMMAND_STATUS_LOW_BATTERY_LEVEL;
  *   request->status.network_addr = ZB_PIBCACHE_NETWORK_ADDRESS();
  *   request->ndsu_handle = ZB_NWK_NON_INTERNAL_NSDU_HANDLE ;
  *
  *   ZB_SCHEDULE_CALLBACK(zb_nlme_send_status, param);
  * }
  * @endcode
 */
void zb_nlme_send_status(zb_uint8_t param);

/** @endcond */ /* internals_doc */

/** @} */

/** \addtogroup nwk_ib */
/** @{ */

/** @internal @brief Size of link status command */
#define ZB_LINK_STATUS_SIZE 3U

#define ZB_NWK_LS_FIRST_FRAME_BIT 5U
#define ZB_NWK_LS_LAST_FRAME_BIT  6U

#define ZB_NWK_LS_FIRST_FRAME_MASK (1U<<ZB_NWK_LS_FIRST_FRAME_BIT)
#define ZB_NWK_LS_LAST_FRAME_MASK  (1U<<ZB_NWK_LS_LAST_FRAME_BIT)
#define ZB_NWK_LS_COUNT_MASK       0x1FU

/* AD: todo replace magic numbers with mask*/
#define ZB_GET_OUTGOING_COST(a)                                                    \
  (*((a) + sizeof(zb_uint16_t)) = ((*((a) + sizeof(zb_uint16_t)) & 0x70U) >> 4U) & 0x07U)
#define ZB_GET_INCOMING_COST(a)                                                    \
  (*((a) + sizeof(zb_uint16_t)) & 0x7U)
#define ZB_LS_SET_INCOMING_COST(a, b) ( *(a) = (*(a) & 0xF8U) | ( (b) & 0x07U) )
#define ZB_LS_SET_OUTGOING_COST(a, b) ( *(a) = (*(a) & 0x0FU) | ( (b) << 4U) )

#define ZB_NWK_LS_GET_COUNT(a) ((a) & ZB_NWK_LS_COUNT_MASK)
#define ZB_NWK_LS_SET_COUNT(_a, _b) ((*(_a)) |= ((*(_a)) & (~ZB_NWK_LS_COUNT_MASK)) | (_b))

#define ZB_NWK_LS_SET_FIRST_FRAME(_a, _b) (*(_a)) |= (((*(_a)) & (~ZB_NWK_LS_FIRST_FRAME_MASK)) | ((_b)<<ZB_NWK_LS_FIRST_FRAME_BIT))
#define ZB_NWK_LS_GET_FIRST_FRAME(_a) (((_a) & ZB_NWK_LS_FIRST_FRAME_MASK) >> ZB_NWK_LS_FIRST_FRAME_BIT)

#define ZB_NWK_LS_SET_LAST_FRAME(_a, _b) (*(_a)) |= (((*(_a)) & (~ZB_NWK_LS_LAST_FRAME_MASK)) | ((_b)<<ZB_NWK_LS_LAST_FRAME_BIT))
#define ZB_NWK_LS_GET_LAST_FRAME(_a) (((_a) & ZB_NWK_LS_FIRST_FRAME_MASK) >> ZB_NWK_LS_FIRST_FRAME_BIT)


/** @} */

/** @cond internals_doc */
/** \addtogroup ZB_NWK */
/** @{ */

/** @brief Initialize NWK stack layer. */
void zb_nwk_init(void);

/** \par Macros to compare network update ids taking overflow into account.

  No recommendations about compare in the specification.  Now suppose overflow if difference
  between values > 1/2 of the entire values diapason. Is it right?
 */
#define NWK_UPDATE_ID_MIDDLE 127

/** @return true if id1 >= id2, taking overflow into account. */
#define ZB_NWK_UPDATE_ID1_GE_ID2(id1, id2) (((zb_uint8_t)(id1) - (zb_uint8_t)(id2)) < NWK_UPDATE_ID_MIDDLE)

/** @return true if id1 < id2, taking overflow into account. */
#define ZB_NWK_UPDATE_ID1_LT_ID2(id1, id2) (((zb_uint8_t)(id1) - (zb_uint8_t)(id2)) > NWK_UPDATE_ID_MIDDLE)

/** @brief Check that link quality is good enough to attempt to join.
  *
  * According to 6.9.8 Link quality indicator (LQI), The minimum and maximum LQI values (0x00 and
  * 0xff) should be associated with the lowest and highest quality compliant signals detectable by
  * the receiver, and LQI values in between should be uniformly distributed between these two
  * limits. At least eight unique values of LQI shall be used.
  *
  * Also, from 3.6.1.4.1.1    Child Procedure:
  * - The link quality for frames received from this device is such that a link cost of at most
  *   3 is produced when calculated as described in sub-clause 3.6.3.1.
  *   @return TRUE if it is ok to join to device with such LQI and FALSE otherwise
  */

#define ZB_LINK_QUALITY_IS_OK_FOR_JOIN(lqi) (((lqi) / (256U / 8U)) >= 1U)

/** @brief Compare link quality.
  * @return TRUE if lqi1 > lqi2
  */
#define ZB_LINK_QUALITY_1_IS_BETTER(lqi1, lqi2) ((lqi1) > (lqi2))

#if defined ZB_PRO_STACK && !defined ZB_NO_NWK_MULTICAST
/** @brief NWK multicast control. */
typedef ZB_PACKED_PRE struct zb_nwk_multicast_control_field_s
{
  /* Better not use bitfields for protocols, use bitmask instead. But keep it now: multicast is already obsolete in r21. */
#ifdef ZB_LITTLE_ENDIAN
    zb_bitfield_t multicast_mode:2;          /**< */
    zb_bitfield_t nonmember_radius:3;        /**< */
    zb_bitfield_t max_nonmember_radius:3;    /**< */
#else
    zb_bitfield_t max_nonmember_radius:3;    /**< */
    zb_bitfield_t nonmember_radius:3;        /**< */
    zb_bitfield_t multicast_mode:2;          /**< */
#endif
} ZB_PACKED_STRUCT
zb_nwk_multicast_control_field_t;
#endif  /* multicast */

#define ZB_NWK_MULTICAST_CONTROL_FIELD_SIZE 1U

/** \par macros to manipulate with nwk packet header. */

/** @brief NWK packet header.

   Fields up to seq_num always presents, other - depending on frame_control
   contents.

   See 3.3.1 for the NWK header definition.
 */
typedef ZB_PACKED_PRE struct zb_nwk_nwk_hdr_s
{
  zb_uint8_t                       frame_control[2];    /**< Frame Control Field */
  zb_uint16_t                      dst_addr;            /**< Destination 16-bit address */
  zb_uint16_t                      src_addr;            /**< Source 16-bit address */
  zb_uint8_t                       radius;              /**< Radius */
  zb_uint8_t                       seq_num;             /**< Sequence Number */
  zb_ieee_addr_t                   dst_ieee_addr;       /**< Destination 64-bit address */
  zb_ieee_addr_t                   src_ieee_addr;       /**< Source 64-bit address */
#if defined ZB_PRO_STACK && !defined ZB_NO_NWK_MULTICAST
  zb_nwk_multicast_control_field_t mcast_control;       /**< NWK multicast control */
#else
  zb_uint8_t                       mcast_control;
#endif
} ZB_PACKED_STRUCT zb_nwk_hdr_t;

typedef ZB_PACKED_PRE struct zb_nwk_alloc_hdr_req_s
{
#ifndef ZB_NO_NWK_MULTICAST
  zb_bool_t      is_multicast;
#endif
  zb_bool_t      is_secured;
  zb_bool_t      is_cmd_frame;
  zb_bool_t      is_has_src_ieee;
  zb_bool_t      is_has_dst_ieee;
  zb_uint16_t    src_addr;
  zb_uint16_t    dst_addr;
  zb_ieee_addr_t dst_ieee_addr;
  zb_ieee_addr_t src_ieee_addr;
} ZB_PACKED_STRUCT zb_nwk_alloc_hdr_req_t;

/** @brief APSDE data indication params transmitted to upper layers */
typedef ZB_PACKED_PRE struct zb_apsde_data_ind_params_s
{
  zb_uint16_t mac_dst_addr; /**< mac dst address. ZB_MAC_SHORT_ADDR_NOT_ALLOCATED
                             * if unknown or long  */
  zb_uint16_t mac_src_addr; /**< mac src address. ZB_MAC_SHORT_ADDR_NOT_ALLOCATED
                             * if unknown or long  */
  zb_uint8_t  lqi;

  zb_int8_t   rssi;

  zb_uint8_t  handle;       /**< handle for transmitted/received packet */

  zb_uint8_t  iface_id;     /**< ID of MAC interface that received the packet */
} ZB_PACKED_STRUCT zb_apsde_data_ind_params_t;

/** @brief Frame type value from the NWK FCF field: data. */
#define ZB_NWK_FRAME_TYPE_DATA     0U

/** @brief Frame type value from the NWK FCF field: command. */
#define ZB_NWK_FRAME_TYPE_COMMAND  1U

/** @brief Zigbee protocol version for GPDF frames */
#define ZB_ZGP_PROTOCOL_VERSION    0x03U

/** \par NWK FCF stored in the order it transmitted over network. Bits access
   macros takes care about using right bytes. No endian conversion is necessary.
   See \see ZB_PKT_16B_ZERO_BYTE / \see ZB_PKT_16B_FIRST_BYTE definition for
   details.

   See 3.3.1.1 for the NWK FCF definition.
 */

/** @brief Get frame type from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
 */
#define ZB_NWK_FRAMECTL_GET_FRAME_TYPE(fctl) ((fctl)[ZB_PKT_16B_ZERO_BYTE] & 3U)

/** @brief Set frame type in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param t - frame type.
 */
#define ZB_NWK_FRAMECTL_SET_FRAME_TYPE(fctl, t ) ((fctl)[ZB_PKT_16B_ZERO_BYTE] |= (t))

/** @brief Get protocol version from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_GET_PROTOCOL_VERSION(fctl) (((fctl)[ZB_PKT_16B_ZERO_BYTE] >> 2U) & 0xfU)

/** @brief Set protocol version in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param v - protocol version.
 */
#define ZB_NWK_FRAMECTL_SET_PROTOCOL_VERSION(fctl, v) ((fctl)[ZB_PKT_16B_ZERO_BYTE] |= ((v) << 2U))

/** @brief Set frame type and protocol version in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param t - frame type.
   @param v - protocol version.
 */
#define ZB_NWK_FRAMECTL_SET_FRAME_TYPE_N_PROTO_VER(fctl, t, v)  \
  ((fctl)[ZB_PKT_16B_ZERO_BYTE] |= ((t) | ((v) << 2U)))

/** @brief Get 'discover route' from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_GET_DISCOVER_ROUTE(fctl) (((fctl)[ZB_PKT_16B_ZERO_BYTE] >> 5U) & 3U)

/** @brief Set 'discover route' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param r - 'discover route' value.
 */
#define ZB_NWK_FRAMECTL_SET_DISCOVER_ROUTE(fctl, r) ((fctl)[ZB_PKT_16B_ZERO_BYTE] |= ((r) << 6U))

#ifndef ZB_NO_NWK_MULTICAST
/** @brief Get multicast flag from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_GET_MULTICAST_FLAG(fctl) (((fctl)[ZB_PKT_16B_FIRST_BYTE]) & 1U)
#else
#define ZB_NWK_FRAMECTL_GET_MULTICAST_FLAG(fctl) 0U
#endif

/** @brief Set multicast flag in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param m - multicast flag value.
 */
#define ZB_NWK_FRAMECTL_SET_MULTICAST_FLAG(fctl, m) ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= (m))

/** @brief Get 'security' from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_GET_SECURITY(fctl) (((fctl)[ZB_PKT_16B_FIRST_BYTE] >> 1U) & 1U)

/** @brief Set 'security' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param s - 'security' value.
 */
#define ZB_NWK_FRAMECTL_SET_SECURITY(fctl, s) ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= ((s) << 1U))

/** @brief Get 'source route' from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_GET_SOURCE_ROUTE(fctl) (((fctl)[ZB_PKT_16B_FIRST_BYTE] >> 2U) & 1U)

/** @brief Set 'source route' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param s - 'source route' value.
 */
#define ZB_NWK_FRAMECTL_SET_SOURCE_ROUTE(fctl, s) ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= ((s) << 2U))

/** @brief Set 'source route' from the NWK header Frame Control field to 0.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_CLEAR_SOURCE_ROUTE(fctl) ((fctl)[ZB_PKT_16B_FIRST_BYTE] &= 0xFBU)

/** @brief Get 'destination IEEE address' from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
*/
#define ZB_NWK_FRAMECTL_GET_DESTINATION_IEEE(fctl) (((fctl)[ZB_PKT_16B_FIRST_BYTE] >> 3U) & 1U)

/** @brief Set 'destination IEEE address' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param d - 'destination IEEE address' value.
 */
#define ZB_NWK_FRAMECTL_SET_DESTINATION_IEEE(fctl, d) ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= ((d) << 3U))

/** @brief Get 'source IEEE address' from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
 */
#define ZB_NWK_FRAMECTL_GET_SOURCE_IEEE(fctl) (((fctl)[ZB_PKT_16B_FIRST_BYTE] >> 4U) & 1U)

/** @brief Set 'source IEEE address' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param s - 'source IEEE address' value.
 */
#define ZB_NWK_FRAMECTL_SET_SOURCE_IEEE(fctl, s) ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= ((s) << 4U))

/** @brief Get 'End Device Initiator' from the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
 */
#define ZB_NWK_FRAMECTL_GET_END_DEVICE_INITIATOR(fctl) (((fctl)[ZB_PKT_16B_FIRST_BYTE] >> 5U) & 1U)

/** @brief Set 'End Device Initiator' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param s - 'End Device Initiator' value.
 */
#define ZB_NWK_FRAMECTL_SET_END_DEVICE_INITIATOR(fctl) ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= (1U << 5U))

/** @brief Set 'End Device Initiator' in the NWK header Frame Control field.

   @param fctl - Frame Control Field of NWK header.
   @param s - 'End Device Initiator' value.
 */
#define ZB_NWK_FRAMECTL_CLR_END_DEVICE_INITIATOR(fctl) ((fctl)[ZB_PKT_16B_FIRST_BYTE] &= ~(1U << 5U))

/** @brief Set 'destination IEEE address' & 'source IEEE address' in the NWK header Frame Control
    field.

   @param fctl - Frame Control Field of NWK header.
   @param s - 'source IEEE address' value.
   @param d - 'destination IEEE address' value.
 */
#define ZB_NWK_FRAMECTL_SET_SRC_DEST_IEEE(fctl, s, d) \
  ((fctl)[ZB_PKT_16B_FIRST_BYTE] |= (((d) << 3U) | ((s) << 4U)))

/** @brief Return size of the header part up to addresses.

   @param is_multicast - use multicast flag.
   @return header part size.
 */
#ifdef ZB_NO_NWK_MULTICAST
#define ZB_NWK_SHORT_HDR_SIZE(is_multicast) ZB_OFFSETOF(zb_nwk_hdr_t, dst_ieee_addr)
#else
#define ZB_NWK_SHORT_HDR_SIZE(is_multicast) \
  (ZB_OFFSETOF(zb_nwk_hdr_t, dst_ieee_addr) + ((is_multicast) ? 1U : 0U))
#endif

/** @brief Return hdr size with only one extended address present.

   @param is_multicast - is use multicast flag.
   @return header part size.
 */
#define ZB_NWK_HALF_HDR_SIZE(is_multicast)  \
  (ZB_OFFSETOF(zb_nwk_hdr_t, src_ieee_addr) + ((is_multicast) ? 1U : 0U))

/** @brief Return full size of the header with extended addresses.

   @param is_multicast - is use multicast flag.
   @return header part size.
 */
#define ZB_NWK_FULL_HDR_SIZE(is_multicast)  \
  (ZB_OFFSETOF(zb_nwk_hdr_t, mcast_control) + ((is_multicast) ? 1U : 0U))

#if defined ZB_PRO_STACK && !defined ZB_NO_NWK_MULTICAST
#define ZB_NWK_IS_MULTICAST_MEMBER_MODE(mac_dst, nwk_frame_ctl)                                \
  ( ZB_NWK_IS_ADDRESS_BROADCAST(mac_dst) && ZB_NWK_FRAMECTL_GET_MULTICAST_FLAG(nwk_frame_ctl))
#else
#define ZB_NWK_IS_MULTICAST_MEMBER_MODE(mac_dst, nwk_frame_ctl) 0U
#endif

/** @brief Calculate network header size.
   @param fctl - pointer on NWK header.
   @return header size.
 */
zb_ushort_t zb_nwk_hdr_base_size(zb_uint8_t *fctl);

#if defined ZB_PRO_STACK && !defined ZB_NO_NWK_MULTICAST

#define ZB_RREQ_MC_MASK 0xBF
#define ZB_RREP_MC_MASK 0xBF
#define ZB_SET_RREQ_MULTICAST(a,b) ( a = (a & ZB_RREQ_MC_MASK) | b<<6U )
#define ZB_GET_RREQ_MULTICAST(a) ( (a & (~ZB_RREQ_MC_MASK)) >>6U )
#define ZB_SET_RREP_MULTICAST(a,b) ( a = (a & ZB_RREP_MC_MASK) | b<<6U )
#define ZB_GET_RREP_MULTICAST(a) ( (a & (~ZB_RREP_MC_MASK)) >>6U )

#else

#define ZB_SET_RREQ_MULTICAST(a,b)
#define ZB_GET_RREQ_MULTICAST(a) 0
#define ZB_SET_RREP_MULTICAST(a,b)
#define ZB_GET_RREP_MULTICAST(a) 0

#endif  /* multicast */

/** @brief Get size of base part of NWK header (without src route and security headers). */
#define ZB_NWK_HDR_GET_BASE_SIZE( nwk_hdr ) \
    zb_nwk_hdr_base_size(((zb_nwk_hdr_t *)nwk_hdr)->frame_control)
/* Get size of NWK header */

#define ZB_NWK_HDR_SIZE( nwk_hdr ) zb_get_nwk_header_size(nwk_hdr)

/**
   Remove network header from an NWK packet placed into a packet buffer.

   To be used by APS layer to cut NWK header to get APS packet.

   @param packet - buffer holding packet
   @param ptr    - (out) pointer to the beginning of packet just after header.
 */
#define ZB_NWK_HDR_CUT(packet, ptr)                                         \
do                                                                          \
{                                                                           \
  zb_nwk_hdr_t *_nwk_hdr = (zb_nwk_hdr_t *)zb_buf_begin(packet);            \
  ptr = zb_buf_cut_left(packet, ZB_NWK_HDR_SIZE(_nwk_hdr));              \
} while (0)

/**
   Get source address from the NWK header

   @param nwk_hdr - pointer to the network packet header
   @param addr - pointer to the short address
 */
#define ZB_NWK_GET_SRC_ADDR(nwk_hdr, addr) \
ZB_LETOH16(addr, &((zb_nwk_hdr_t *)nwk_hdr)->src_addr)

   /**
      Get destination address from the NWK header

      @param nwk_hdr - pointer to the network packet header
      @param addr - pointer to the short address
    */
#define ZB_NWK_GET_DST_ADDR(nwk_hdr, addr) \
ZB_LETOH16(addr, &((zb_nwk_hdr_t *)nwk_hdr)->dst_addr)

#define ZB_NWK_ADDR_TO_LE16(addr) ZB_HTOLE16_ONPLACE(addr)

/**
 * @name Network command constants
 * @anchor nwk_cmd
 *
 * Note: These values were members of `enum zb_nwk_cmd_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NWK_CMD_ROUTE_REQUEST       0x01U /**< Route Request NWK command Id*/
#define ZB_NWK_CMD_ROUTE_REPLY         0x02U /**< Route Reply NWK command Id*/
#define ZB_NWK_CMD_NETWORK_STATUS      0x03U /**< Network Status NWK command Id*/
#define ZB_NWK_CMD_LEAVE               0x04U /**< Leave NWK command Id*/
#define ZB_NWK_CMD_ROUTE_RECORD        0x05U /**< Route Record NWK command Id*/
#define ZB_NWK_CMD_REJOIN_REQUEST      0x06U /**< Rejoin Request NWK command Id*/
#define ZB_NWK_CMD_REJOIN_RESPONSE     0x07U /**< Rejoin Response NWK command Id*/
#define ZB_NWK_CMD_LINK_STATUS         0x08U /**< Link Status NWK command Id*/
#define ZB_NWK_CMD_NETWORK_REPORT      0x09U /**< Network Report NWK command Id*/
#define ZB_NWK_CMD_NETWORK_UPDATE      0x0aU /**< Network Update NWK command Id*/
#define ZB_NWK_CMD_ED_TIMEOUT_REQUEST  0x0bU /**< End Device Timeout Request NWK command Id*/
#define ZB_NWK_CMD_ED_TIMEOUT_RESPONSE 0x0cU /**< End Device Timeout Response NWK command Id*/
#define ZB_NWK_CMD_LINK_POWER_DELTA    0x0dU /**< Link Power Delta command Id */
/** @} */

/**
   Get command id from nwk command packet

   @param buf - pointer to the packet buffer
   @param nwk_hdr_size - network packet header
 */
#define ZB_NWK_CMD_FRAME_GET_CMD_ID(buf, nwk_hdr_size) ( *(zb_uint8_t *)(zb_buf_data(buf, (nwk_hdr_size)) ))

/**
   Get command pointer to the command payload from nwk command packet

   @param buf - pointer to the packet buffer
   @param nwk_hdr_size - network packet header size
 */
#define ZB_NWK_CMD_FRAME_GET_CMD_PAYLOAD(buf, nwk_hdr_size) (zb_buf_data((buf), (zb_uint_t)(nwk_hdr_size) + 1U))

/**
   Route request command
*/

/* Route request command options field */
#ifdef ZB_PRO_STACK
#endif

/**
   Route request structure
*/
typedef ZB_PACKED_PRE struct zb_nwk_cmd_rreq_s
{
  zb_uint8_t opt;                /**< Command options */
  zb_uint8_t rreq_id;            /**< Route request id */
  zb_uint16_t dest_addr;         /**< Final destination address */
  zb_uint8_t path_cost;           /**< Route request total path cost */
  zb_ieee_addr_t dest_long_addr; /**< Final destination long address. */
} ZB_PACKED_STRUCT
zb_nwk_cmd_rreq_t;

/**
   Route request types
*/

/**
 * @name Route request types
 * @anchor nwk_rreq_type
 *
 * Note: These values were members of `enum zb_nwk_rreq_type_e` type but were
 * converted to a set of macros due to MISRA violations.
 */
/** @{ */
#define  ZB_NWK_RREQ_TYPE_NOT_MTORR                    0U
#define  ZB_NWK_RREQ_TYPE_MTORR_RREC_TABLE_SUPPORTED   1U
#define  ZB_NWK_RREQ_TYPE_MTORR_RREC_TABLE_UNSUPPORTED 2U
/** @} */

/**
   Route request Many-to-one field mask
*/
#define ZB_NWK_RREQ_MANY_TO_ONE_FIELD_MASK 0x03U

/**
   Set Route request type
*/
#define ZB_NWK_SET_RREQ_TYPE(opt, type)    ((opt) |= (zb_uint8_t )(((type) & ZB_NWK_RREQ_MANY_TO_ONE_FIELD_MASK) << 3U))

/**
   Get Route request type
*/
#define ZB_NWK_GET_RREQ_TYPE(opt)          (((opt) >> 3) & ZB_NWK_RREQ_MANY_TO_ONE_FIELD_MASK)

/**
   Route request Destination IEEE address field mask
*/
#define ZB_NWK_RREQ_DESTINATION_IEEE_ADDRESS_MASK 0x01U

/**
   Set Route request Destination IEEE address field
*/
#define ZB_NWK_SET_RREQ_HAS_DST_IEEE_ADDR(opt, included)  ((opt) |= (zb_uint8_t )(((included) & ZB_NWK_RREQ_DESTINATION_IEEE_ADDRESS_MASK) << 5U))

/**
   Get Route request Destination IEEE address field
*/
#define ZB_NWK_GET_RREQ_HAS_DST_IEEE_ADDR(opt)            (((opt) >> 5) & ZB_NWK_RREQ_DESTINATION_IEEE_ADDRESS_MASK)

/**
  Set of bits that are supported during forwarding the Route Request.
 */
#define ZB_NWK_RREQ_OPT_SUPPORTED_BITS                    0x78U /* 0b01111000 */

/**
   Route response command
*/

/**
   Route reply structure
*/
typedef ZB_PACKED_PRE struct zb_nwk_cmd_rrep_s
{
  zb_uint8_t opt;                 /**< Command options */
  zb_uint8_t rreq_id;             /**< Route request id */
  zb_uint16_t originator;         /**< Originator address */
  zb_uint16_t responder;          /**< Responder address */
  zb_uint8_t path_cost;           /**< Path cost */
} ZB_PACKED_STRUCT
zb_nwk_cmd_rrep_t;

/**
   Rejoin request command
*/
typedef ZB_PACKED_PRE struct zb_nwk_rejoin_request_s
{
  zb_mac_capability_info_t capability_information; /**< The operating capabilities of the
                                                    * device */
} ZB_PACKED_STRUCT
zb_nwk_rejoin_request_t;

/**
   Rejoin response command
*/
typedef ZB_PACKED_PRE struct zb_nwk_rejoin_response_s
{
  zb_uint16_t network_addr;         /**< Network address */
  zb_uint8_t rejoin_status;         /**< Rejoin status */
} ZB_PACKED_STRUCT
zb_nwk_rejoin_response_t;


#define ZB_NWK_COMMAND_SIZE(payload_size) (1U + (payload_size))

#define ZB_LEAVE_PL_SET_REQUEST(pl)  (pl) |= (1U << 6U)
#define ZB_LEAVE_PL_SET_REJOIN(pl, v) (pl) |= ((ZB_B2U(ZB_U2B(v))) << 5U)
#define ZB_LEAVE_PL_SET_REMOVE_CHILDREN(pl, v)  (pl) |= ((ZB_B2U(ZB_U2B(v))) << 7U)

#define ZB_LEAVE_PL_GET_REQUEST(pl)  (((pl) >> 6U) & 1U)
#define ZB_LEAVE_PL_GET_REJOIN(pl)   (((pl) >> 5U) & 1U)
#define ZB_LEAVE_PL_GET_REMOVE_CHILDREN(pl)  (((pl) >> 7U) & 1U)

#ifdef ZB_JOIN_CLIENT
void zb_nwk_do_leave(zb_uint8_t param, zb_uint8_t rejoin);
#else
#define zb_nwk_do_leave(param, rejoin)
#endif

void zb_nwk_do_rejoin_after_leave(zb_uint8_t param);

void zb_nwk_rejoin_sync_pibcache_with_mac(zb_uint8_t param, zb_callback_t cb);

/**
   Forget remote device or myself
 */
void zb_nwk_forget_device(zb_uint8_t addr_ref);

void zb_nwk_source_routing_record_delete(zb_uint16_t addr);

/**
   Update beacon payload in the PIB.

   To be called after any network configuration change: formation, join etc.
   As a side effect increments NIB Update id.
 */
void zb_nwk_update_beacon_payload(zb_uint8_t param);

/**
   3.4.9 Network Report Command
 */
typedef ZB_PACKED_PRE struct zb_nwk_report_cmd_s
{
  zb_uint8_t command_options;       /**< Command options */
  zb_ext_pan_id_t epid;             /**< 64-bit EPID that identifies the network that the reporting device */
  zb_uint16_t panids[1];            /**< Pan ID Conflict Report list */
} ZB_PACKED_STRUCT zb_nwk_report_cmd_t;

#define ZB_NWK_REPORT_INFO_COUNT(options) ((options) & 0xfU)
#define ZB_NWK_REPORT_COMMAND_ID(options) ((((options) >> 4U)) & 0xfU)
#define ZB_NWK_REPORT_IS_PANID_CONFLICT(options) (ZB_NWK_REPORT_COMMAND_ID(options) == 0U)

#define ZB_NWK_UPDATE_INFO_COUNT(options) ((options) & 0xfU)
#define ZB_NWK_UPDATE_COMMAND_ID(options) ((((options) >> 4U)) & 0xfU)
#define ZB_NWK_UPDATE_IS_PANID_UPDATE(options) (ZB_NWK_UPDATE_COMMAND_ID(options) == 0U)

/**
   3.4.10 Network Update Command
 */
typedef ZB_PACKED_PRE struct zb_nwk_update_cmd_s
{
  zb_uint8_t command_options;   /**< NWK command options*/
  zb_ext_pan_id_t epid;         /**< 64-bit Network ID*/
  zb_uint8_t update_id;         /**< 16-bit Network ID*/
  zb_uint16_t new_panid;        /**< 16-bit new Network ID*/
} ZB_PACKED_STRUCT zb_nwk_update_cmd_t;

#ifdef ZB_PRO_STACK

typedef ZB_PACKED_PRE struct zb_nwk_cmd_rrec_s
{
  /*zb_uint8_t opt;*/ /**< Command options */
  zb_uint8_t relay_cnt; /** relay count */
  zb_uint16_t relay_addr; /** relay address */
} ZB_PACKED_STRUCT
zb_nwk_cmd_rrec_t;

/**
  Send Route record command

  @param cbuf - buffer for command data
  @param scr_addr - 16-bit source address
  @param dst_addr - 16-bit destination address
  @return if OK return RET_OK otherwise error code
 */
zb_ret_t zb_nwk_send_rrec(zb_bufid_t cbuf, zb_uint16_t src_addr, zb_uint16_t dst_addr);

/**
  Send Route record command prior to send a packet

  @param cbuf - buffer for command data
  @param scr_addr - 16-bit source address
  @param dst_addr - 16-bit destination address
  @param mac_dst - 16-bit MAC destination address
  @return if OK return RET_OK otherwise error code
 */
zb_ret_t zb_nwk_send_rrec_prior(zb_bufid_t cbuf, zb_uint16_t src_addr, zb_uint16_t dst_addr, zb_uint16_t mac_dst);

/**
  Route record response handler
 */
void zb_nwk_rrec_handler(zb_bufid_t buf, zb_nwk_hdr_t *nwk_hdr, zb_uint8_t hdr_size);

/**
  Send Many-to-one-route failure network status
 */
void zb_nwk_many_to_one_route_failure(zb_bufid_t buf);

/**
  Check expiry for the source route table
 */
void zb_nwk_source_route_table_expiry(void);

/**
   Clear the route record required field of the routing table entry
 */
void zb_nwk_source_routing_clear_rrec_req(zb_uint16_t src_addr);

/**
  Start Concentrator mode using parameters from NIB
 */
void zb_nwk_concentrator_start(void);

/**
  Stop Concentrator mode
 */
void zb_nwk_concentrator_stop(void);

/*Calculate default timeout for keepalive*/
zb_time_t zb_nwk_get_default_keepalive_timeout(void);

/*Return ZB_TRUE if nearest aging timeout expired*/
zb_bool_t zb_nwk_check_aging(void);

#if !(defined ZB_MACSPLIT_DEVICE)
/**
Send End device timeout request command
 */
void zb_nwk_ed_send_timeout_req(zb_uint8_t param);

/**
End device timeout request command handler
 */
void zb_nwk_ed_timeout_request_handler(zb_bufid_t buf, zb_nwk_hdr_t *nwk_hdr, zb_nwk_ed_timeout_request_t *cmd_ed_time_req);

/**
Send End device timeout response handler
 */

void nwk_timeout_resp_handler(zb_bufid_t buf, zb_nwk_hdr_t *nwk_hdr, zb_nwk_ed_timeout_response_t *cmd_ed_time_resp);
#endif

/**
   3.4.13 Link Power Delta command
 */
typedef enum zb_nwk_power_delta_cmd_options_e
{
  ZB_NWK_LPD_CMD_OPTIONS_NOTIFICATION = 0x0U,
  ZB_NWK_LPD_CMD_OPTIONS_REQUEST      = 0x1U,
  ZB_NWK_LPD_CMD_OPTIONS_RESPONSE     = 0x2U,
  ZB_NWK_LPD_CMD_OPTIONS_RESERVED     = 0x3U,
} zb_nwk_power_delta_cmd_options_t;

#define ZB_NWK_LPD_CMD_OPTIONS_MASK 0x03U

typedef ZB_PACKED_PRE struct zb_nwk_link_power_delta_payload_s
{
  zb_uint8_t cmd_options;
  zb_uint8_t list_count;
} ZB_PACKED_STRUCT
zb_nwk_link_power_delta_payload_t;


typedef ZB_PACKED_PRE struct zb_nwk_power_list_entry_s
{
  zb_uint16_t short_addr;
  zb_uint8_t power_delta;
} ZB_PACKED_STRUCT
zb_nwk_power_list_entry_t;

/**
  Test Conflict Address
  Compare address attributes with data from Address Map

  @param addr - 16-bit address
  @param ieee_addr - 64-bit IEEE address
  @return if has conflict return RET_CONFLICT otherwise RET_OK

 */
zb_ret_t zb_nwk_is_conflict_addr(zb_uint16_t addr, zb_ieee_addr_t ieee_addr);

/**
  Test Conflict Address by DevAnnce frame
  Compare address attributes with data from Address Map
  Data may be not present in Address Map yet

  @param addr - 16-bit address
  @param ieee_addr - 64-bit IEEE address
  @return if has conflict return RET_CONFLICT otherwise RET_OK

 */
zb_ret_t zb_nwk_test_dev_annce(zb_uint16_t addr, zb_ieee_addr_t ieee_addr);

#endif  /* pro stack */

/**
  Set NWK PIB attribute

  @param param - buffer
  @param iface_id - MAC interface ID
  @param attr - attribute ID
  @param value - attribute value
  @param value_size - size of attribute value
  @param cb - callback for run after
 */
void zb_nwk_pib_set(zb_uint8_t param, zb_uint8_t iface_id, zb_uint8_t attr, void *value,
                    zb_ushort_t value_size, zb_callback_t cb);

/**
  Get NWK PIB attribute

  @param param - buffer
  @param iface_id - MAC interface id
  @param attr - attribute ID
  @param cb - callback for run after
 */
void zb_nwk_pib_get(zb_uint8_t param, zb_uint8_t iface_id, zb_uint8_t attr, zb_callback_t cb);

/** @} */
/** @endcond */

/**
  Get NWK header size

  @param hdr - NWK header
  @return size of NWK header

  @code
  zb_uint8_t hdr_size = zb_get_nwk_header_size(hdr);
  @endcode
 */
zb_uint8_t zb_get_nwk_header_size(const zb_nwk_hdr_t *hdr);

/**
   zb_nwk_nib_init
   Initialize NIB variables

   @return nothing
 */
void zb_nwk_nib_init(zb_bool_t is_first);

void zb_nwk_handle_init(void);

#if defined ZB_PRO_STACK && !defined ZB_NO_NWK_MULTICAST
zb_nwk_multicast_control_field_t *zb_get_mc_field_from_header(zb_nwk_hdr_t *hdr);
#define GET_NWK_MCF( nwk_hdr ) (*( zb_get_mc_field_from_header(nwk_hdr)))
#else
#define GET_NWK_MCF( nwk_hdr ) nwk_hdr->mcast_control
#endif

/* Send network status with value 0x0B-Source route failure */
#if defined ZB_PRO_STACK && defined ZB_ROUTER_ROLE
void zb_send_nwk_status_source_route_fail(zb_uint8_t param, zb_uint16_t src_addr);
#endif


/** \addtogroup nwk_api */
/** @{ */

#if defined ZB_PRO_ADDRESS_ASSIGNMENT_CB || defined DOXYGEN

/**
  Set callback NWK address assignment

  Allow the application to set the desired nwk address to the device.

  @param cb - callback
 */
void zb_nwk_set_address_assignment_cb(zb_addr_assignment_cb_t cb);


/**
  Set callback to inform about device association

  @param cb - callback
 */
void zb_nwk_set_dev_associate_cb(zb_addr_assignment_cb_t cb);
#endif

#ifndef ZB_LITE_NO_LINK_COST
/**
   Convert lqi to cost
   lqi related from device type.
   TODO: test algorithm on different platforms. Work with TI CC2520.
   Look at incoming and outgoing routing cost values in the link status message. Value 1
   indicates the best link, 7 indicates the worst one.
 */

#define NWK_LQI_2_COST(lqi) (((lqi) < 0xe0U) ? (7U - (((lqi) >> 5U ) & 0x07U)) : 1U)
#else
#define NWK_LQI_2_COST(lqi) ZB_NWK_STATIC_PATH_COST
#endif

#ifdef ZB_ROUTER_ROLE

/**
  Calc neighbour path cost
 */
#if defined ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED
#define ZB_NWK_NEIGHBOUR_GET_PATH_COST(nbt) zb_nwk_neighbour_get_path_cost((nbt))
#elif defined ZB_NWK_NEIGHBOUR_PATH_COST_LQI_BASED
#define ZB_NWK_NEIGHBOUR_GET_PATH_COST(nbt) NWK_LQI_2_COST((nbt)->lqi)
#else
#error Please specify neighbour path cost calculation method!
#endif /* ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED */

#endif /* ZB_ROUTER_ROLE */

/**
   Convert cost to LQI
   To be used for testing, like in TP/PRO-BV-04
 */
#define NWK_COST_TO_LQI(cost)   ((7U - (cost)) << 5U)

/** @} */

#ifdef ZB_CHECK_OOM_STATUS
void zb_check_oom_status(zb_uint8_t param);

#ifdef ZB_SEND_OOM_STATUS
void zb_oom_status_confirm(zb_uint8_t param);
#endif /* ZB_SEND_OOM_STATUS */
#endif /* ZB_CHECK_OOM_STATUS */

#ifdef ZB_NWK_BLACKLIST

/**
 * @brief add new entry to blacklist
 * @param ext_pan_id - extended Pan ID of network to be blacklisted
 */
void zb_nwk_blacklist_add(zb_ext_pan_id_t ext_pan_id);

/**
 * @brief clear blacklist
 */
void zb_nwk_blacklist_reset(void);

/**
 * @brief checks whether network with given extended Pan ID is blacklisted
 * @param ext_pan_id - extended Pan ID to check
 * @return RET_OK - if network is not blacklisted, RET_ERROR otherwise
 */
zb_ret_t zb_nwk_blacklist_check(zb_ext_pan_id_t ext_pan_id);

/**
 * @brief checks whether blacklist is full
 * @return RET_OK - if blacklist has unused entries, RET_OVERFLOW otherwise
 */
zb_bool_t zb_nwk_blacklist_is_full(void);

zb_bool_t zb_nwk_blacklist_is_empty(void);

#endif

void zb_nwk_unlock_in(zb_uint8_t param);


/**
   Increment counter of TX.

   Normalize TX success/failure using tx counters window
 */
void nwk_txstat_tx_inc(void);

/**
   Increment counter of failed TX. Schedule ZDO failures check routine.

   May normalize TX success/failure using tx counters window
 */
void nwk_txstat_fail_inc(void);

#ifdef ZB_ROUTER_ROLE
/**
   Clear TX statistic counters.

   To be used after channel change.
 */
void nwk_txstat_clear(void);
#else
#define nwk_txstat_clear(q)
#endif

#ifdef ZB_FORMATION
void zb_nwk_cont_without_formation(zb_uint8_t param);
#endif /* ZB_FORMATION */

void zb_nwk_reset_route_expire(zb_uint16_t addr);

void zb_nwk_route_expire(zb_uint16_t addr);

void zb_nwk_load_pib(zb_uint8_t param);

#if defined ZB_ASSERT_SEND_NWK_REPORT
/* Send NWK Network Status pkt on assert with custom payload - epid is 0xdeadbeef + file_id + line_number */
typedef ZB_PACKED_PRE struct zb_nlme_assert_ind_frame_s
{
  /* nwk hdr */
  zb_uint8_t                       frame_control[2];    /**< Frame Control Field */
  zb_uint16_t                      dst_addr;            /**< Destination 16-bit address */
  zb_uint16_t                      src_addr;            /**< Source 16-bit address */
  zb_uint8_t                       radius;              /**< Radius */
  zb_uint8_t                       seq_num;             /**< Sequence Number */
  zb_ieee_addr_t                   dst_ieee_addr;       /**< Destination 64-bit address */

  /* cmd - use Network Report  */
  zb_uint8_t cmd_id;
  zb_uint8_t command_options;

  /* use our structure instead of pan_id */
  zb_uint32_t sig;
  zb_uint16_t file_id;
  zb_int_t line;
} ZB_PACKED_STRUCT
zb_nlme_assert_ind_frame_t;

void zb_nlme_send_assert_ind(zb_uint16_t file_id, zb_int_t line);
#endif

void zb_nwk_restart_aging(void);

zb_ret_t zb_nwk_validate_leave_req(zb_uint16_t src_addr);

#ifdef ZB_LIMIT_VISIBILITY
zb_bool_t zb_mac_is_long_addr_visible(const zb_ieee_addr_t ieee_addr);
#endif

#define ZB_NWK_NEIGHBOR_ERROR_VALUE     0xffU

zb_uint8_t zb_nwk_get_nbr_rel_by_short(zb_uint16_t addr);
zb_uint8_t zb_nwk_get_nbr_rel_by_ieee(const zb_ieee_addr_t addr);

zb_uint8_t zb_nwk_get_nbr_dvc_type_by_short(zb_uint16_t addr);
zb_uint8_t zb_nwk_get_nbr_dvc_type_by_ieee(const zb_ieee_addr_t addr);

zb_uint8_t zb_nwk_get_nbr_rx_on_idle_by_short(zb_uint16_t addr);
zb_uint8_t zb_nwk_get_nbr_rx_on_idle_short_or_false(zb_uint16_t addr);
zb_uint8_t zb_nwk_get_nbr_rx_on_idle_by_ieee(zb_ieee_addr_t addr);

#if defined ZB_STORE_NBR_INFO_IN_ALIEN_MAC

#define ALIEN_MAC_NBR_ENT_ADD(ent) zb_mac_alien_nbr_ent_add(ent)
#define ALIEN_MAC_NBR_ENT_DELETE(ent) zb_mac_alien_nbr_ent_delete(ent)
#define ALIEN_MAC_NBR_ENT_DELETE_ALL() zb_mac_alien_nbr_ent_delete_all()

#else /* ZB_STORE_NBR_INFO_IN_ALIEN_MAC */

#define ALIEN_MAC_NBR_ENT_ADD(ent)
#define ALIEN_MAC_NBR_ENT_DELETE(ent)
#define ALIEN_MAC_NBR_ENT_DELETE_ALL()

#endif  /* ZB_STORE_NBR_INFO_IN_ALIEN_MAC */

typedef struct zb_nwk_neighbor_element_s
{
  zb_uint8_t relationship;
  zb_uint8_t device_type;
  zb_bool_t rx_on_when_idle;
  zb_uint8_t depth;
  zb_uint8_t permit_joining;
}
zb_nwk_neighbor_element_t;

zb_ret_t zb_nwk_get_neighbor_element(zb_uint16_t addr, zb_bool_t create_if_absent, zb_nwk_neighbor_element_t *update);
zb_ret_t zb_nwk_set_neighbor_element(zb_uint16_t addr, zb_nwk_neighbor_element_t *update);

zb_ret_t zb_nwk_delete_neighbor_by_short(zb_uint16_t addr);
zb_ret_t zb_nwk_delete_neighbor_by_ieee(zb_ieee_addr_t addr);

void zb_nwk_send_direct_leave_req(zb_uint8_t param, zb_uint16_t dst_addr);

zb_bool_t nwk_is_lq_bad_for_direct(zb_int8_t rssi, zb_uint8_t lqi);
void nwk_maybe_force_send_via_routing(zb_uint16_t addr);

void nwk_internal_lock_in(void);
void nwk_internal_unlock_in(void);

void zb_mcps_build_data_request(zb_bufid_t buf, zb_uint16_t src_addr_param, zb_uint16_t dst_addr_param, zb_uint8_t tx_options_param, zb_uint8_t msdu_hande_param);

#ifdef SNCP_MODE
zb_ret_t zb_nwk_force_route_sending(zb_bool_t enable);
#endif

void nwk_mark_nwk_encr1(zb_bufid_t buf, zb_uint16_t file_id, zb_uint16_t line);
#define nwk_mark_nwk_encr(buf) nwk_mark_nwk_encr1((buf), ZB_TRACE_FILE_ID, __LINE__)

void nwk_router_start_common(zb_uint8_t param);

#ifdef ZB_FORMATION
void zb_nwk_formation_force_link(void);
#endif /* ZB_FORMATION */

#if defined ZB_PARENT_CLASSIFICATION && defined ZB_ROUTER_ROLE
void nwk_set_tc_connectivity(zb_uint8_t val);

zb_bool_t nwk_get_tc_connectivity(void);
#endif /* ZB_PARENT_CLASSIFICATION && ZB_ROUTER_ROLE */

#ifndef ZB_MAC_INTERFACE_SINGLE

zb_uint32_t zb_nwk_get_octet_duration_us(void);
#define ZB_NWK_OCTET_DURATION_US (zb_nwk_get_octet_duration_us())

#else

#define ZB_NWK_OCTET_DURATION_US (zb_uint32_t)(ZB_2_4_GHZ_OCTET_DURATION_USEC)

#endif

/* 01/15/2019 EE CR:MINOR For 2.4-only mode this solution is ok from
 * the code size point of view: you pass your octets up to the top and
 * convert it to BI using a macro, so all calculation are done at
 * compile time. But for MM case compiler will insert function call
 * and division everywhere. It is not optimal.
 * Even while you can't eliminate a function call, you can exclude additional calculations.
 * Function call can get time in octets and return time in BE.
 */
#define ZB_NWK_OCTETS_TO_US(octets_cnt) ((octets_cnt) * ZB_NWK_OCTET_DURATION_US)
#define ZB_NWK_OCTETS_TO_BI(octets_cnt) ZB_MILLISECONDS_TO_BEACON_INTERVAL(ZB_NWK_OCTETS_TO_US(octets_cnt) / 1000U)

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
/* Internal Control4 functions to enable/disable Control4 controller behavior emulation */
void zb_disable_control4_emulator();

void zb_enable_control4_emulator();
#endif /* ZB_CONTROL4_NETWORK_SUPPORT */

#ifndef ZB_MACSPLIT_DEVICE
zb_ext_neighbor_tbl_ent_t *nwk_choose_parent(zb_address_pan_id_ref_t panid_ref, zb_mac_capability_info_t capability_information);
#endif /* ZB_MACSPLIT_DEVICE */

#endif /* ZB_NWK_H */
