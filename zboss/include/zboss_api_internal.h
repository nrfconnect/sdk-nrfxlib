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
/*  PURPOSE: ZBOSS internal data structures required for configurable memory.
*/

#ifndef ZB_ZBOSS_API_INTERNAL_H
#define ZB_ZBOSS_API_INTERNAL_H 1

#include "zb_vendor.h"
#include "zb_config.h"
#include "zb_types.h"
#include "zb_errors.h"
#include "zb_osif.h"
#include "zb_debug.h"
#include "zb_trace.h"
#include "zb_pooled_list.h"
#include "zb_address.h"

/* Data structures moved from subsystem internal headers for configurable memory implementation. */

/**
   Parameters for storing data in a pending queue

   Moved there from MAC to be able to implement MAC indirect quote size configuration.
*/
typedef struct zb_mac_pending_data_s
{
  zb_addr_u dst_addr;         /**< Destination address */
  zb_uint8_t      dst_addr_mode;    /**< Destination address mode @ref zb_addr_mode_t */
  zb_uint8_t      pending_param;                 /**< Pointer to pending data */
}
zb_mac_pending_data_t;


/**
   APS retransmissions
 */
typedef ZB_PACKED_PRE struct zb_aps_retrans_ent_s
{
  zb_uint16_t  addr;            /*!< Destination address*/
  zb_uint16_t  clusterid;       /*!< Cluster ID*/
  zb_uint8_t   aps_counter;     /*!< APS counter */
  zb_uint8_t   src_endpoint;    /*!< Source endpoint */
  zb_uint8_t   dst_endpoint;    /*!< Destination endpoint */
  zb_uint8_t   buf;             /*!< Buffer index for retranslate */

  zb_bitfield_t aps_retries:4;  /*!< Number of attempts */
  zb_bitfield_t nwk_insecure:1; /*!< Flag 'Is NWK secure' */
  zb_bitfield_t state:3;        /*!< \see zb_aps_retrans_ent_state_e */
} ZB_PACKED_STRUCT zb_aps_retrans_ent_t;


typedef ZB_PACKED_PRE struct zb_cb_q_ent_s
{
  zb_callback_t func_ptr;        /*!< function to call  */
  zb_uint16_t user_param;        /*!< user parameter */
  zb_uint8_t param;              /*!< parameter to pass to 'func'  */
}
ZB_PACKED_STRUCT
zb_cb_q_ent_t;


typedef ZB_PACKED_PRE struct zb_delayed_buf_q_ent_s
{
  zb_callback_t func_ptr;        /*!< function to call  */
  zb_uint16_t   user_param;      /*!< user parameter */
  zb_bitfield_t buf_cnt:7;       /*!< number of buffers to allocate */
  zb_bitfield_t is_2param:1;     /*!< whether this is a 2param callback */
}
ZB_PACKED_STRUCT
zb_delayed_buf_q_ent_t;

/**
   Delayed (scheduled to run after timeout) callbacks queue entry.
 */
typedef ZB_PACKED_PRE struct zb_tm_q_ent_s
{
  zb_callback_t func;           /*!< function to call  */
  zb_time_t run_time;           /*!< time to run at  */
  zb_uint8_t param;             /*!< parameter to pass to 'func'  */
  ZB_POOLED_LIST8_FIELD(next);
}
ZB_PACKED_STRUCT
zb_tm_q_ent_t;

/**
   Installcode hash for TC

   Do not store installcode itself in RAM - keep only 4 bytes per entry in RAM.
 */
typedef ZB_PACKED_PRE struct zb_aps_installcode_storage_s
{
  zb_uint32_t    nvram_offset:28;      /*!< offset of zb_aps_installcode_nvram_t
                                          record in nvram. 28 bit supports 256k
                                          page - hope, it is enough  */
  zb_bitfield_t  nvram_page:4;         /*!< nvram page. Hope 16 pages are enough  */
} ZB_PACKED_STRUCT zb_aps_installcode_storage_t;


typedef ZB_PACKED_PRE struct zb_aps_device_key_pair_array_s
{
  zb_uint32_t    nvram_offset:27;            /*!< offset of zb_aps_device_key_pair_storage_t
                                               record in nvram. 27 bit supports 128k
                                               page - hope, it is enough  */
  /* Try to fit into 6 bytes instead of 8 */
  zb_lbitfield_t  outgoing_frame_counter:21;/*!< Outgoing value for APS frame
                                             * outgoing counter. Used for
                                             * communication with TC only, so
                                             * hope 2^21 ~ 10^10 values are enough
                                             */
#ifndef ZB_NO_CHECK_INCOMING_SECURE_APS_FRAME_COUNTERS
  zb_uint32_t     incoming_frame_counter; /*!< Incoming value,for APS frame incoming counter */
#endif
} ZB_PACKED_STRUCT zb_aps_device_key_pair_array_t;



/**
   NWK routing
*/
typedef ZB_PACKED_PRE struct zb_nwk_routing_s /* do not pack for IAR */
{
  zb_bitfield_t used:1; /*!< 1 if entry is used, 0 - otherwise */
  zb_bitfield_t status:2; /*!< The status of the route, @see zb_nwk_route_state_t */
#ifndef ZB_LITE_NO_SOURCE_ROUTING
  zb_bitfield_t no_route_cache:1;        /*!< Dest does not store source routes. */
  zb_bitfield_t many_to_one:1;           /*!< Dest is the concentrator and many-to-one
                                          * request was used  */
  zb_bitfield_t route_record_required:1; /*!< Route record command frame should
                                          * be sent to the dest prior to the
                                          * next data packet */
#endif
#ifndef ZB_NO_NWK_MULTICAST
  zb_bitfield_t group_id_flag:1; /*!< Indicates that dest_addr is a Group ID */
#endif
#if ZB_NWK_ROUTING_TABLE_EXPIRY < 64
  zb_bitfield_t expiry:6;  /*!< expiration time. max value -
                            * ZB_NWK_ROUTING_TABLE_EXPIRY (60) */
#else
  zb_uint8_t expiry;
#endif
#if !defined ZB_CONFIGURABLE_MEM && ZB_IEEE_ADDR_TABLE_SIZE <= 127
  zb_bitfield_t next_hop_addr_ref:7;
#else
  zb_uint8_t next_hop_addr_ref; /*!< ref to network address of the next
                                            * hop on the way to the destination */
#endif
  zb_uint16_t dest_addr; /*!< 16-bit network address or Group ID of this route */
} ZB_PACKED_STRUCT
zb_nwk_routing_t;

#if defined ZB_PRO_STACK && !defined ZB_LITE_NO_SOURCE_ROUTING
/**
  Route Record Table Entry Format, Table 3.45
*/
typedef struct zb_nwk_rrec_s    /* do not pack for IAR */
{
  zb_bitfield_t used:1;                     /*!< Is record used */
  zb_bitfield_t expiry:7;                   /*!< Expiration time */
  zb_uint8_t  count;                        /*!< Count hops */
  zb_uint16_t addr;                         /*!< Destination address */
  zb_uint16_t path[ZB_NWK_MAX_PATH_LENGTH]; /*!< Full path */
} ZB_PACKED_STRUCT zb_nwk_rrec_t;

#endif

typedef ZB_PACKED_PRE struct zb_aps_dup_tbl_ent_s
{
  zb_uint8_t addr_ref;          /*!< Reference to addr_map */
  zb_uint8_t counter;                      /*!< APS frame counter */
  zb_bitfield_t clock:7;                   /*!< Expiry clock counter */
  zb_bitfield_t is_unicast:1;              /*!< Is delivery mode unicast */
} ZB_PACKED_STRUCT zb_aps_dup_tbl_ent_t;


/**
   Global binding table - source part
*/
typedef ZB_PACKED_PRE struct zb_aps_bind_src_table_s
{
  zb_address_ieee_ref_t src_addr;   /*!< source address as ref from nwkAddressMap */
  zb_uint8_t            src_end;    /*!< source endpoint */
  zb_uint16_t           cluster_id; /*!< cluster id */
} ZB_PACKED_STRUCT zb_aps_bind_src_table_t;

/**
   Global binding table - destination part with 64-bit address
*/
typedef ZB_PACKED_PRE struct zb_aps_bind_long_dst_addr_s
{
  zb_address_ieee_ref_t dst_addr;        /*!< destination address as ref from nwkAddressMap */
  zb_uint8_t            dst_end;         /*!< destination endpoint */
} ZB_PACKED_STRUCT zb_aps_bind_long_dst_addr_t;

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_APS_BIND_TRANS_TABLE_SIZE ((ZB_IOBUF_POOL_SIZE + 15)/16 *4)

/* it should be 4-byte aligned if it is stored in NVRAM */
#define ZB_SINGLE_TRANS_INDEX_SIZE (((ZB_APS_BIND_TRANS_TABLE_SIZE + 31) / 32) * 4)
#endif

/**
   Global binding table - destination part
*/
typedef ZB_PACKED_PRE struct zb_aps_bind_dst_table_s
{
#ifdef ZB_CONFIGURABLE_MEM
  /* WARNING: this field will be rewritten if APS binding dataset is present in NVRAM */
  zb_uint8_t            *trans_index;
#endif /* defined ZB_CONFIGURABLE_MEM */

  ZB_PACKED_PRE union
  {
    zb_uint16_t group_addr;                /*!< group address */
    zb_aps_bind_long_dst_addr_t long_addr; /*!< @see zb_asp_long_dst_addr_t */
  } u;

#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t            trans_index[ZB_SINGLE_TRANS_INDEX_SIZE];
#endif /* defined ZB_CONFIGURABLE_MEM */
  zb_uint8_t            align;

  zb_bitfield_t         dst_addr_mode:3;   /*!< destination address mode flag, 0
                                            * - group address, otherwise long
                                            * address plus dest endpoint */
  zb_bitfield_t         src_table_index:5; /*!< index from zb_asp_src_table_t */
} ZB_PACKED_STRUCT zb_aps_bind_dst_table_t;

/**
   Neighbor table entry
 */
typedef ZB_PACKED_PRE struct zb_neighbor_tbl_ent_s /* not need to pack it at IAR */
{
  /* 0 */
  zb_bitfield_t             used:1;         /*!< Record has used */
  zb_bitfield_t             ext_neighbor:1;   /*!< if 1, this is ext neighbor
                                               * record, else base neighbor  */

  zb_bitfield_t             device_type:2; /*!< Neighbor device type - \see
                                            * zb_nwk_device_type_t */

  zb_bitfield_t             depth:4; /*!< The network depth of this
                                       device. A value of 0x00
                                       indicates that this device is the
                                       Zigbee coordinator for the
                                       network.  */
  /* 1,2 */
  zb_bitfield_t             permit_joining:1; /*!< A value of TRUE indicates that at
                                                least one Zigbee router on the
                                                network currently permits joining,
                                                i.e. its NWK has been issued an
                                                NLME-PERMIT-JOINING
                                                primitive and, the time limit if
                                                given, has not yet expired.  */

  zb_bitfield_t             rx_on_when_idle:1; /*!< Indicates if neighbor receiver
                                                 enabled during idle periods:
                                                 TRUE = Receiver is on
                                                 FALSE = Receiver is off
                                                 This field should be present for
                                                 entries that record the parent or
                                                 children of a Zigbee router or
                                                 Zigbee coordinator.  */

  zb_bitfield_t             relationship:3; /*!< The relationship between the
                                              neighbor and the current device:
                                              0x00=neighbor is the parent
                                              0x01=neighbor is a child
                                              0x02=neighbor is a sibling
                                              0x03=none of the above
                                              0x04=previous child
                                              0x05=unauthenticated child
                                              This field shall be present in every
                                              neighbor table entry.
                                              \see zb_nwk_relationship_e
                                            */

  zb_bitfield_t             need_rejoin:1; 	/*!< Need send rejoin response without receive request */

  zb_bitfield_t             send_via_routing: 1;  /*!< Due to bad link to that device send packets
                                                   *   via NWK routing.
                                                   */

  zb_bitfield_t             keepalive_received:1; /*!< This value indicates at least one keepalive
                                                   *   has been received from the end device since
                                                   *   the router has rebooted.
                                                   */

  zb_bitfield_t             mac_iface_idx:5;  /*!< An index into the MAC Interface Table
                                               * indicating what interface the neighbor or
                                               * child is bound to. */

  zb_bitfield_t             transmit_failure_cnt:4;  /*!< Transmit failure counter (used to initiate
                                                      * device address
                                                      * search). */
  /* 3 */
  zb_uint8_t                lqi;  /*!< Link quality. Also used to calculate
                                   * incoming cost */
  /* 4 */
  zb_int8_t                 rssi;  /*!< Received signal strength indicator */
  /* 5 */
  ZB_PACKED_PRE union {
    ZB_PACKED_PRE struct zb_ext_neighbor_s
    {
      /* 0 */
      zb_uint16_t               short_addr; /*!< 16-bit network address of the
                                              neighboring device */
      /* 2 */

      zb_ieee_addr_compressed_t long_addr; /*!< 64-bit address (packed) */
      /* 8 */
      zb_uint8_t                update_id; /*!< This field reflects the value of nwkUpdateId from the NIB.  */
      /* 9 */
      zb_bitfield_t             panid_ref:5; /*!< ref to the extended Pan id  */

      zb_bitfield_t             router_capacity:1; /*!< This value is set to TRUE if this
                                                     device is capable of accepting
                                                     join requests from router-
                                                     capable devices and is set to
                                                     FALSE otherwise.   */
      zb_bitfield_t             end_device_capacity:1; /*!< This value is set to TRUE if the
                                                         device is capable of accepting
                                                         join requests from end devices
                                                         seeking to join the network and
                                                         is set to FALSE otherwise.  */
      zb_bitfield_t             potential_parent:1; /*!< This field usage - see
                                                      3.6.1.4.1.1  Child Procedure:
                                                      If the Status parameter indicates a refusal to permit
                                                      joining on the part of the neighboring device (that is, PAN at capacity or PAN
                                                      access denied), then the device attempting to join should set the Potential parent
                                                      bit to 0 in the corresponding neighbor table entry to indicate a failed join attempt.
                                                    */
      /* 10 */
      zb_uint8_t                channel_page; /*!< The current channel page occupied by the network.  */
      /* 11 */
      zb_bitfield_t             logical_channel:6; /*!< The current logical channel
                                                     occupied by the network.  */

      zb_bitfield_t             stack_profile:2; /*!< A ZBOSS profile identifier.   */

#ifdef ZB_PARENT_CLASSIFICATION
      /* 12 */
      zb_uint8_t                classification_mask;
#endif
      /* 13 */
    } ZB_PACKED_STRUCT ext;
    ZB_PACKED_PRE struct zb_base_neighbor_s
    {
      zb_uint8_t                key_seq_number; /*!< key number for which
                                                 * incoming_frame_counter is valid  */
#ifndef ZB_ROUTER_ROLE           /* no routing at ZED - simplify*/
      zb_address_ieee_ref_t     addr_ref;         /*!< address translation entry */
      zb_uint8_t                nwk_ed_timeout; /*End device timeout - @see nwk_requested_timeout_t */
#else                                           /* ZR,ZC */
#if !defined ZB_CONFIGURABLE_MEM && ZB_IEEE_ADDR_TABLE_SIZE < 128
      /* Won 1 byte here, so base is 11 bytes (== ext) */
      zb_bitfield_t             addr_ref:7; /*!< address translation entry */
#else
      /* If configurable mem build, can have >127 addresses, so need a byte here. */
      zb_address_ieee_ref_t     addr_ref;
#endif                                            /* if 7 bits are enough */

      zb_bitfield_t             nwk_ed_timeout:4; /*End device timeout - @see nwk_requested_timeout_t */

      /* Following fields present only if nwkSymLink = TRUE - this is PRO, not 2007 */
      zb_bitfield_t             outgoing_cost:3;  /*!< The cost of an outgoing
                                                   * link. Got from link status. */
      /* In other words, if a device fails to receive nwkRouterAgeLimit link status
         messages from a router neighbor in a row, the old outgoing cost information is
         discarded. In this case, the neighbor entry is considered stale and may be reused if
         necessary to make room for a new neighbor. */
#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
      zb_bitfield_t             age:5;  /*!< The number of nwkLinkStatusPeriod intervals since a
                                         * link status command was received */
#else
      /* actually ZB_NWK_ROUTER_AGE_LIMIT is 3, so 2 bits is enough */
      zb_bitfield_t             age:2;
#endif
#endif                                                  /* ZB_ROUTER_ROLE */
      zb_uint32_t               incoming_frame_counter; /*!< incoming NWK frame counter
                                                         * for this device after
                                                         * key change */
      zb_time_t                 time_to_expire; /*Time stamp for ED aging*/
    } ZB_PACKED_STRUCT base;
  } ZB_PACKED_STRUCT u;
  /* TODO: move it to base ?? */
#if !defined ZB_ED_ROLE && defined ZB_MAC_DUTY_CYCLE_MONITORING
  zb_lbitfield_t is_subghz:1;        /*!< if 1, this is Sub-GHz device */
  zb_lbitfield_t suspended:1;        /*!< if 1, SuspendZCLMessages was sent to the device */
  zb_lbitfield_t pkt_count:22;       /*!< count of packets received from this device */
#define MAX_NBT_PKT_COUNT ((1u<<22)-1)
  zb_lbitfield_t subghz_ep:8;        /*!< endpoint with Sub-GHz cluster on remote device */
#endif
} ZB_PACKED_STRUCT
zb_neighbor_tbl_ent_t;

/**
   Kind of negotiation before TX
*/
typedef enum zb_mac_tx_wait_e
{
  ZB_MAC_TX_WAIT_ACK,
  ZB_MAC_TX_WAIT_CSMACA,
  ZB_MAC_TX_WAIT_ZGP,
  ZB_MAC_TX_WAIT_NONE,
} zb_mac_tx_wait_t;

enum zb_mac_tx_status_e
{
  ZB_TRANS_TX_OK = 0,
  ZB_TRANS_CHANNEL_BUSY_ERROR,
  ZB_TRANS_TX_RETRY_COUNT_EXCEEDED,
  ZB_TRANS_TX_LBT_TO,
  ZB_TRANS_NO_ACK
};

#endif /* ZB_ZBOSS_API_INTERNAL_H */
