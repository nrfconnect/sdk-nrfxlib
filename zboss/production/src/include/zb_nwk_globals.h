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
/* PURPOSE: NWK subsystem globals
*/

#ifndef ZB_NWK_GLOBALS_H
#define ZB_NWK_GLOBALS_H 1

#include "zb_config.h"
#include "zb_mac.h"
#include "zb_nwk_neighbor.h"
#include "zb_nwk.h"

/*! @cond internals_doc */
/*! \addtogroup ZB_NWK */
/*! @{ */


zb_uint_t zb_calc_non_zero_bits_in_bit_vector(zb_uint8_t *vector, zb_uint_t size);

#define ZB_DUMP_IEEE_ADDR(iee_addr) TRACE_MSG(TRACE_COMMON3, "ieeeaddr " TRACE_FORMAT_64, (FMT__A, TRACE_ARG_64(iee_addr)))


/**
 * @name NWK State
 * @anchor nlme_state
 *
 * Note: These values were members of `enum zb_nlme_state_t` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_NLME_STATE_IDLE                      0U
#define ZB_NLME_STATE_DISC                      1U
#define ZB_NLME_STATE_FORMATION_ED_SCAN         2U
#define ZB_NLME_STATE_FORMATION_ACTIVE_SCAN     3U
#define ZB_NLME_STATE_FORMATION                 4U
#define ZB_NLME_STATE_ROUTER                    5U
#define ZB_NLME_STATE_ED_SCAN                   6U
#define ZB_NLME_STATE_REJOIN                    7U
#define ZB_NLME_STATE_ORPHAN_SCAN               8U
#define ZB_NLME_STATE_RESET                     9U /* unused currently */
/* Specific case for searching networks in BDB:
 * when device failed authentication it leaves network
 * but does not clear nib to be able join to other networks.
 */
#define ZB_NLME_STATE_RESET_NO_NIB_REINIT       10U
#define ZB_NLME_STATE_PANID_CONFLICT_RESOLUTION 11U
#define ZB_NLME_STATE_ZDO_STARTING              12U

#define ZB_NLME_STATE_PIB_LOAD1                 13U
#define ZB_NLME_STATE_PIB_LOAD2                 14U
#define ZB_NLME_STATE_PIB_LOAD3                 15U
#define ZB_NLME_STATE_PIB_LOAD4                 16U
#define ZB_NLME_STATE_PIB_LOAD5                 17U
#define ZB_NLME_STATE_PIB_LOAD6                 18U
#define ZB_NLME_STATE_PIB_LOAD7                 19U
#define ZB_NLME_STATE_PIB_LOAD8                 20U
#define ZB_NLME_STATE_PIB_LOAD9                 21U
#define ZB_NLME_STATE_PIB_LOAD10                22U
#define ZB_NLME_STATE_PIB_LOAD_SRC_MATCH_TBL    23U

/*!< State to execute the 'Survey Beacon' procedure */
#define ZB_NLME_STATE_SURVEY_BEACON             24U
/** @} */

/* Broadcast transaction record */
/* 01/15/2019 EE CR:MINOR Why don't pack for IAR? It seems like very old problem of 2012-11-08. Probably can pack and decrease RAM usage */
typedef ZB_PACKED_PRE struct zb_nwk_btr_s     /* do not pack for IAR */
{
/* 01/15/2019 EE CR:MINOR May exclude 'used' and operate with special
 * value of expiration_time. But in such case can't use
 * NWK_ARRAY_GET_ENT and friends. Maybe, it is good.
 * Removing of bitfield may decrease code ize.
 */

  /* Using short address itself instead of addr ref in order to avoid problematic
     situations during address conflicts. For example, if short address is changed for an address
     reference and the same packet is received, a new BTR will be created (and rebroadcasted as well).
  */
  zb_uint16_t   source_addr;            /*!< Source address  */
  zb_uint8_t    sequence_number;        /*!< Sequence address */
  zb_bitfield_t used:1;                 /*!< Record is used */
  zb_bitfield_t expiration_time:7;      /*!< Expiration time */
} ZB_PACKED_STRUCT
zb_nwk_btr_t;

/* Passive ack bit array size */
#define ZB_NWK_BRCST_PASSIVE_ACK_ARRAY_SIZE ((ZB_NEIGHBOR_TABLE_SIZE + 7U) / 8U)

/**
   Broadcast retransmition info
   Important! This struct must ZB_PACKED_STRUCT for ZB_NEED_ALIGN
 */
typedef ZB_PACKED_PRE struct zb_nwk_broadcast_retransmit_s
{
  zb_bitfield_t retries_left:2;        /*!< Number of currently available transmission attempts */

  zb_bitfield_t been_broadcasted:1;  /*<! Whether the buffer was broadcasted at least once */
#define BRRT_MAX_RETRANSMIT_COUNTDOWN_VALUE  0XFU /* ((1<<4) - 1) */
  zb_bitfield_t retransmit_countdown:4; /* Number of BI until tx. */
  zb_bitfield_t mcps_data_req_in_buf:1; /* If 1, buffer has mcps-data.req in its params section, else apsde-data.ind params */
  zb_uint8_t buf;                     /* <! Buffer to be sent broadcast */
  /* 01/15/2019 EE CR:MAJOR Can we exclude neighbor_table_iterator by passing it via second parameter of 2-arguments callback?

     AN: it is dependent on the specific zb_nwk_broadcast_retransmit_t anyway, so in such case we
     must use three parameters: an empty buffer to send, and ID from BRRT table and a
     neighbor_table_iterator. The latter two can be squeezed into a uint16_t, but is it worth it?

     EE: why not if it costs us nothing?

     AN: that would cause the following problems:
       a) We couldn't use delayed buffer allocation for unicast transmissions since there is no guarantee that a BRRT entry would be the same by the time when a buffer is ready. If neighbor_table_iterator is passed as well, that could result in using iterator from one device for another - some packets will not be sent, some will be sent twice.
       b) Since broadcast retransmission can be completed at multiple points, it is important to always have consistent state at hand. Currently neighbor_table_iterator also serves as an indicator that we are done with the generation of unicast packets and the original buffer can be released.
   */
  zb_uint8_t neighbor_table_iterator; /*!< Next child to unicast broadcast frames to */

  /* get src_addr, dst_addr, seq_num from buf which is here and have nwk header
   * at the data begin */
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t passive_ack[ZB_NWK_BRCST_PASSIVE_ACK_ARRAY_SIZE]; /* passive ack
                                                                * bit array */
#else
  zb_uint8_t *passive_ack;
#endif
}  ZB_PACKED_STRUCT
zb_nwk_broadcast_retransmit_t;

/**
  Information about incoming mgmt leave requests
 */
typedef struct zb_in_mgmt_leave_pending_list_s /* do not pack for IAR */
{
  zb_uint16_t            src_addr;      /*!< Source address to send mgmt resp
                                         * to. Our local address if this is local mgmt leave req */
  zb_uint8_t             buf_ref;       /*!< Buffer for this leave*/
  zb_uint8_t             tsn;           /*!< TSN of outgoing ZCL packet */
} ZB_PACKED_STRUCT zb_in_mgmt_leave_pending_list_t;


typedef struct zb_leave_ind_notify_s
{
  zb_address_ieee_ref_t addr_ref;       /*!< Address of device for leave */
  zb_uint8_t rejoin;                    /*!< Is rejoin after leave */
} zb_leave_ind_notify_t;

/**
  leave context
*/
typedef struct zb_leave_context_s
{
  /*!< * Pending incoming mgmt leave req, or self mgmt leave req */
  zb_in_mgmt_leave_pending_list_t pending_list[ZB_ZDO_PENDING_LEAVE_SIZE];
  zb_leave_ind_notify_t leave_ind_prnt;                             /*!< */
  zb_leave_ind_notify_t leave_ind_zed;                              /*!< */
  zb_uint8_t pending_list_bm;                                       /*!< */
  zb_bitfield_t rejoin_after_leave:1;                               /*!< */
  zb_bitfield_t remove_children:1;                                  /*!< */
  zb_bitfield_t reserved:6;
} zb_leave_context_t;

#define ZB_SET_LEAVE_PENDING(i) ZG->nwk.leave_context.pending_list_bm |= (1U<<(i))
#define ZB_RESET_LEAVE_PENDING(i) ZG->nwk.leave_context.pending_list_bm &= ~(1U<<(i))
#define ZB_IS_LEAVE_PENDING(i) (ZG->nwk.leave_context.pending_list_bm & (1U<<(i)))
#define ZB_IS_ANY_LEAVE_PENDING() (ZG->nwk.leave_context.pending_list_bm != 0U)

/**
   rejoin command context
 */
typedef struct zb_rejoin_context_s /* do not pack for IAR */
{
  zb_uint16_t addr;                     /*!< */
  zb_address_ieee_ref_t addr_ref;
  zb_uint8_t secure_rejoin;
  zb_uint8_t rx_on;
#ifdef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
  zb_bufid_t rejoin_rsp_param;          /*!< Buf with nwk header. */
  zb_bufid_t mlme_set_conf_param;       /*!< This param is needed to determine ent in rejoin table
                                               for which confirm has arrived. */
#endif /* ZB_MAC_PENDING_BIT_SOURCE_MATCHING */
} ZB_PACKED_STRUCT zb_rejoin_context_t;


#define ZB_NWK_IN_Q_SIZE (ZB_IOBUF_POOL_SIZE/2U)

#ifndef ZB_CONFIGURABLE_MEM
/* Buffer for pending incoming packets. Big enough to exclude packets loss. */
ZB_RING_BUFFER_DECLARE(zb_nwk_in_q, zb_uint8_t, (ZB_NWK_IN_Q_SIZE));
#endif

#ifdef ZB_NWK_BLACKLIST
/**
   Blacklist array
  */
typedef struct zb_nwk_blacklist_s
{
  zb_uint8_t used;              /*!< Used records number */
  zb_ext_pan_id_t blacklisted[ZB_NWK_BLACKLIST_SIZE];
} zb_nwk_blacklist_t;
#endif

/**
   Place to store values between multiply operations
*/
typedef struct zb_nwk_handle_s  /* do not pac for IAR */
{
  zb_uint8_t state;        /*!< Current network subsystem state @ref nlme_state */
  zb_bool_t operation_cancelled; /*!< Network formation or network discovery is cancelled */
  zb_address_ieee_ref_t parent; /*!< parent address (valid if the device is not ZC and joined)  */

  union tmp_u
  {
    struct join_tmp_s
    {
      zb_nlme_join_request_t saved_join_req;
      zb_ext_neighbor_tbl_ent_t *parent;
    } join;
    struct rejoin_tmp_s         /*!< Rejoin options */
    {
      zb_nlme_join_request_t saved_join_req;
      zb_ext_pan_id_t extended_pan_id;
      zb_mac_capability_info_t capability_information;
      zb_ext_neighbor_tbl_ent_t *parent;
      zb_uint8_t unsecured_rejoin;
      zb_uint8_t poll_attempts;
      zb_uint8_t poll_req;
      zb_callback_t cb;
    } rejoin;
#ifdef ZB_FORMATION
    struct formation_s
    {
      zb_uint8_t channel_pan_count[ZB_CHANNEL_PAGES_NUM][ZB_CHANNEL_PAGE_MAX_CHANNELS_COUNT];
      zb_uint8_t panid_handled_bm[ZB_PANID_TABLE_SIZE / 8U + 1U];
    } formation;
#endif
  } tmp;

  zb_callback_t run_after_update_beacon_payload;    /*!< Run after update beacon payload */

  zb_uint16_t status_addr; /* Used to report NWK status indication */

#ifdef ZB_PRO_STACK
  zb_uint8_t               send_link_status_index;  /*!< Current Index of short_sorted address table  */
#endif /* ZB_PRO_STACK */

#if defined ZB_MAC_POWER_CONTROL
  zb_uint8_t send_power_delta_index; /* Current index of short_sorted address
                                      * table for NWK Power Delta command (notification */
  zb_uint16_t lpd_resp_addr;         /* Address of recepient, LPD frame
                                      * response to be sent to */
  zb_ieee_addr_t lpd_leave_ieee; /* IEEE to delete from MAC power table */
#endif

#ifdef ZB_ROUTER_ROLE
/*Pointer to the end device that expires first*/
  zb_neighbor_tbl_ent_t     *next_aging_end_device;
#endif
#if !(defined ZB_ED_ROLE && defined ZB_ED_RX_OFF_WHEN_IDLE)
  zb_nwk_btr_t btt[ZB_NWK_BTR_TABLE_SIZE]; /*!< Broadcast transaction
                                            * table see 3.6.5 */
  zb_uint8_t btt_cnt;               /*!< */
#endif

#ifdef ZB_ROUTER_ROLE
  zb_nwk_broadcast_retransmit_t brrt[ZB_NWK_BRR_TABLE_SIZE]; /* Broadcast
                                                              * retransmition
                                                              table */
  zb_uint8_t brrt_cnt;
  zb_uint8_t brrt_in_progress;                               /* Broadcast retransmission is in progress */
  zb_rejoin_context_t rejoin_req_table[ZB_NWK_REJOIN_REQUEST_TABLE_SIZE];
  zb_uint8_t  rejoin_req_table_cnt;
#endif

#if defined ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN && defined ZB_ROUTER_ROLE
  /* When ZR/ZC discovers address conflict for remote device or itself (ZR)
   * it places conflicting address into this table (addr_ref for conflicting device
   * used as index). Entry will be cleared in 2 cases:
   * - Device that discovers conflict sending  Network Status
   * - another Network Status with identical payload was received
   */
#endif

  zb_uint8_t     unsecure_frame_attempt; /*!< current attempt to unsecure frame */

#ifndef ZB_CONFIGURABLE_MEM
  zb_nwk_in_q_t  input_q;       /*!< input queue: exclude races  */
  /* input_q is a struct here, so need & to access it in the way unified with configurable mem build. */
#define ZB_NWK_IN_Q (&ZG->nwk.handle.input_q)

#else
  /* If configurable memory, no buffer allocate here. Instead assign a pointer in zb_init_configurable_mem(). */
  zb_byte_array_t  *input_q;      /*!< input queue: a pointer  */
#define ZB_NWK_IN_Q (ZG->nwk.handle.input_q)

#endif  /* ZB_CONFIGURABLE_MEM */

  zb_uint8_t     input_blocked_by; /*!< buffer which blocked nwk input processing  */

#ifndef ZB_LITE_NO_PANID_CONFLICT_DETECTION
  zb_uint16_t new_panid;                                     /*!< */

  zb_bitbool_t panid_conflict:1;                             /*!< Flag 'Has Pan ID conflict' */
#endif                                                       /* panid conflict enabled */

  zb_bitbool_t permit_join:1; /*!< True if permit join is in progress */
  zb_bitbool_t reserved:1;
  /*zb_bitfield_t joined:1; moved to AIB.tcpolicy and defined ZB_JOINED() macro      Non-zero if the device is joined into the network */

  zb_bitbool_t router_started:1;                             /*!< True if the device is a router and it is started */

  zb_bitbool_t poll_in_progress:1;                           /*!< */
  zb_bitbool_t joined_restart:1;                             /*!< True if the device started
                                                              * with NWK parameters from NVRAM */
  zb_bitbool_t rejoin_capability_alloc_address:1;            /*!< True if address is selected by the
                                                              * parent; False if self-assigned */
  zb_bitbool_t addr_conflict_ctx_send_nwk_status:1;          /*!< True if nwk status command must be
                                                              * broadcast during address conflict
                                                              * resolution */
  zb_bitbool_t nwk_broadcast_delivery_time_passed:1;         /*<! Flag indicating that
                                                              * nwkNetworkBroadcastDeliveryTime
                                                              * have passed since device start.
                                                              * Needed for CCB 2033. */
#if defined ZB_MAC_PENDING_BIT_SOURCE_MATCHING && defined ZB_ROUTER_ROLE
  zb_uint8_t src_match_nbr_idx;                              /*!< External loop index for SRC Match restoring */
#endif
  zb_channel_list_t scan_channels_list;                      /*!< Channels to be scanned. */
  zb_uint8_t        scan_iface_idx;                          /*!< Index of current MAC iface in nwkMacInterfaceTable used for scan. */
  zb_uint8_t        scan_duration;                           /*!< Time to spend scanning each channel. */
  zb_uint8_t        ed_list_param;                           /*!< Index of buffer for ED scan results. */
} zb_nwk_handle_t;

#ifdef ZB_ROUTER_ROLE
#define ZB_INVALID_BRRT_IDX 0xFFU
ZB_ASSERT_COMPILE_DECL(ZB_NWK_BRR_TABLE_SIZE < ZB_INVALID_BRRT_IDX);
#endif

/**
 * @name NWK Neighbor relationship between neighbors
 * @anchor nwk_relationship
*/
/** @{ */
#define ZB_NWK_RELATIONSHIP_PARENT                0x00u
#define ZB_NWK_RELATIONSHIP_CHILD                 0x01u
#define ZB_NWK_RELATIONSHIP_SIBLING               0x02u
#define ZB_NWK_RELATIONSHIP_NONE_OF_THE_ABOVE     0x03u
#define ZB_NWK_RELATIONSHIP_PREVIOUS_CHILD        0x04u
#define ZB_NWK_RELATIONSHIP_UNAUTHENTICATED_CHILD 0x05u
/** @} */


/**
 * @name NWK route state
 * @anchor nwk_route_state
*/
/** @{ */
#define ZB_NWK_ROUTE_STATE_ACTIVE               0U
#define ZB_NWK_ROUTE_STATE_DISCOVERY_UNDERWAY   1U
#define ZB_NWK_ROUTE_STATE_DISCOVERY_INACTIVE   2U
#define ZB_NWK_ROUTE_STATE_VALIDATION_UNDERWAY  3U
/* actually ZB_NWK_ROUTE_STATE_DISCOVERY_FAILED never used, so 2 bits are
 * enough for state */
#define ZB_NWK_ROUTE_STATE_DISCOVERY_FAILED     4U
#define ZB_NWK_ROUTE_STATE_NUM                  5U
/** @} */


/**
 * @name NWK address allocation method
 * @anchor nwk_address_allocation
*/
/** @{ */
#define ZB_NWK_ADDRESS_ALLOC_METHOD_DISTRIBUTED 0U
#define ZB_NWK_ADDRESS_ALLOC_METHOD_STOCHASTIC  1U
/** @} */


/* NWK Broadcast delivery time See 3.5.2.1 */
/* = 2*nwkMaxDepth* ((0.05+(nwkcMaxBroadcastJitter/2))+ nwkPassiveAckTimeout*nwkBroadcastRetries/1000)
Note that nwkcMaxBroadcastJitter and nwkPassiveAckTimeout are measured in OctetDurations (!).


In PICS NDF4 for PRO:

nwkBroadcastDeliveryTime = 0x44AA27 Octet
durations (9 seconds on
2.4 GHz)
nwkPassiveAckTimeout =
0x3D098 Octet
Durations9 (500 ms on
2.4 GHz) maximum
nwkMaxBroadcastRetries
= 2


 */
#if 0
#if defined ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN
#define ZB_NWK_BROADCAST_DELIVERY_TIME()                                  \
  (2U * ZB_NWK_STOCH_DEPTH * ((ZB_NWK_MAX_BROADCAST_JITTER_INTERVAL/2U) + \
                             (ZB_NWK_PASSIVE_ACK_TIMEOUT * ZB_NWK_MAX_BROADCAST_RETRIES)/1000U))

#else
#define ZB_NWK_BROADCAST_DELIVERY_TIME()        \
  (2U * ZB_NWK_TREE_ROUTING_DEPTH * ((ZB_NWK_MAX_BROADCAST_JITTER_INTERVAL/2U) + \
                                    (ZB_NWK_PASSIVE_ACK_TIMEOUT * ZB_NWK_MAX_BROADCAST_RETRIES)/1000U))
#endif
#endif  /* 0 */


/**
 * @name OOM Status Confirm
 * @anchor oom_status
 *
 * Note: These values were members of `enum zb_oom_status_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define ZB_OOM_STATUS_NOT_SENT           0U
#define ZB_OOM_STATUS_SENT_NOT_CONFIRMED 1U
#define ZB_OOM_STATUS_SENT_CONFIRMED     2U
/** @} */

/**
   NWK global context
*/
typedef struct zb_nwk_globals_s
{
  zb_nib_t           nib;               /*!< ZB_NIB */
  zb_nwk_pib_cache_t pib_cache;         /*!< Cache frequently used PIB attributes */
  zb_nwk_handle_t    handle;            /*!< Place to store values between multiple operations */
  zb_neighbor_tbl_t  neighbor;          /*!< Neighbor table */
  zb_leave_context_t leave_context;     /*!< Leave context */
#ifdef ZB_PRO_ADDRESS_ASSIGNMENT_CB
  zb_addr_assignment_cb_t addr_cb;        /*!< Callback for Address assignment for any child */
  zb_addr_assignment_cb_t dev_associate_cb; /*!< Callback to inform that a device associated to the network */
#endif
  zb_ieee_addr_t associating_address;
#ifdef ZB_CHECK_OOM_STATUS
  zb_time_t oom_timestamp;
  zb_time_t oom_last_sent;
#ifdef ZB_SEND_OOM_STATUS
  zb_uint8_t oom_status_buf_ref;                   /*!< Reference to buffer used to send diagnostics messages*/
#endif
  zb_bitbool_t oom_presents:1;        /*!< OOM presents flag */
  zb_bitfield_t oom_status:2;         /*!< \see oom_status */
  zb_bitfield_t reserved:4;
#endif  /* ZB_CHECK_OOM_STATUS */
  zb_bitbool_t panid_conflict_auto_resolution:1;   /* CCB2713 */
#ifdef ZB_NWK_BLACKLIST
  zb_nwk_blacklist_t blacklist;
#endif

  struct linkage_selector_s
  {
    zb_callback_t formation_ed_scan_confirm;
    zb_callback_t formation_select_channel;
    zb_callback_t formation_mlme_start_conf;
    zb_callback_t panid_conflict_in_beacon;
    zb_callback_t panid_conflict_got_network_report;
    zb_callback_t panid_conflict_network_update_recv;
    zb_callback_t no_active_links_left_cb;
    zb_bool_t (*should_accept_frame_before_join)(zb_bufid_t);
    zb_callback_t nwk_cancel_nwk_disc_resp;
    zb_callback_t nwk_cancel_nwk_form_resp;
  } selector;

#ifndef ZB_MAC_INTERFACE_SINGLE
    zb_uindex_t        mac_interfaces_count;
    zb_mac_interface_t mac_interfaces[ZB_NWK_MAC_IFACE_TBL_SIZE];
#endif /* ZB_MAC_INTERFACE_SINGLE */

} zb_nwk_globals_t;


/* For stack internals access NIB directly */
#ifdef ZB_NIB
#undef ZB_NIB
#define ZB_NIB() ZG->nwk.nib
#endif

/* For stack internals access PIBCACHE directly */
#ifdef ZB_PIB_CACHE
#undef ZB_PIB_CACHE
/*cstat !MISRAC2012-Rule-5.2_c99 !MISRAC2012-Rule-5.4_c99 */
/* The following violation of rules 5.2 and 5.4 seem to be false positive
 * At one time, only one macro is currently defined, but MISRA violations
 * 5.2 and 5.4 require at least two definitions simultaneously.
 * This violation seems to be caused by #undef usage, which cannot
 * be analyzed by C-STAT correctly. */
#define ZB_PIB_CACHE() (&ZG->nwk.pib_cache)
#endif

#define ZB_NWK_GET_RREQ_ID() (++ZB_NIB().rreq_id)

#define ZB_NWK_MAC_IFACE_TBL() ZB_NIB().mac_iface_tbl
#define ZB_NWK_MAC_IFACE_TBL_ENTRY(iface_id) (&ZB_NIB().mac_iface_tbl[iface_id])

#ifdef ZB_NWK_USE_SEND_JITTER
#define ZB_NWK_JITTER(interval) ((interval) + ZB_RANDOM_JTR(ZB_NWK_OCTETS_TO_BI(ZB_NWKC_MAX_BROADCAST_JITTER_OCTETS)))
#else
#define ZB_NWK_JITTER(interval) (interval)
#endif

void zb_set_bit_in_bit_vector(zb_uint8_t *v, zb_ushort_t b);

zb_bool_t zb_check_bit_in_bit_vector(zb_uint8_t *v, zb_ushort_t b);

void zb_clr_bit_in_bit_vector(zb_uint8_t *v, zb_ushort_t b);

void zb_add_short_addr_sorted(zb_address_ieee_ref_t ref, zb_uint16_t short_addr);

zb_ret_t zb_address_reuse_pan_id(zb_uint16_t short_pan_id, zb_ext_pan_id_t pan_id, zb_address_pan_id_ref_t panid_ref);

zb_ret_t zb_address_delete_pan_id(zb_address_pan_id_ref_t panid_ref);

void nwk_clear_pending_table(void);
void nwk_clear_pending_table_for_destination(zb_uint16_t dst_addr);

void zb_nwk_load_pib_confirm(zb_uint8_t param);

#ifdef ZB_FORMATION
void zb_nwk_call_mlme_start(zb_uint8_t param);
#endif /* ZB_FORMATION */

#ifdef ZB_JOIN_CLIENT
void zb_nwk_sync_pibcache_with_mac(zb_uint8_t param, zb_callback_t cb);
#else
#define zb_nwk_sync_pibcache_with_mac(param, cb)
#endif

#ifdef ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
void nbt_inc_in_pkt_count(zb_neighbor_tbl_ent_t *ent);
#endif /* ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ */

#if defined ZB_MAC_POWER_CONTROL
/* Routine to be called by child (ZED/ZR) */
void zb_nwk_lpd_joined_child(zb_uint8_t param);

/* Routine to be called by child (ZED/ZR) */
void zb_nwk_lpd_joined_parent(zb_uint8_t param, zb_uint16_t short_addr);
#endif /* ZB_MAC_POWER_CONTROL */

void zb_nlme_rejoin_request_pre_handler(zb_uint8_t param);

/*! @} */
/*! @endcond */

#endif /* ZB_NWK_GLOBALS_H */
