/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2022 DSR Corporation, Denver CO, USA.
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
/* PURPOSE: Globals data definitionfor APS subsystem
*/

#ifndef ZB_APS_GLOBALS_H
#define ZB_APS_GLOBALS_H 1

/*! @cond internals_doc */
/*! \addtogroup ZB_APS */
/*! @{ */

#include "zb_secur.h"

/* I don't know why aliases for addr mode were declared, but setting it to 0 and 1 causes
   need to convert it back to "normal" addr mode. So, let's declare this aliaces equal to
   common addr mode values.
   According to 2.2.4.3.1 APSME-BIND.request addr mode values 0x00 and 0x02 are reserved,
   only 0x01 and 0x03 are used */
#define ZB_APS_BIND_DST_ADDR_GROUP ZB_APS_ADDR_MODE_16_GROUP_ENDP_NOT_PRESENT
#define ZB_APS_BIND_DST_ADDR_LONG  ZB_APS_ADDR_MODE_64_ENDP_PRESENT

/**
   Binding table search result for dst request
 */
typedef ZB_PACKED_PRE struct zb_aps_bind_dst_record_s
{
  zb_uint8_t            dst_addr_mode;   /*!< destination address mode flag, 0
                                          * - group address, otherwise long
                                          * address plus dest endpoint */
  ZB_PACKED_PRE union
  {
    zb_uint16_t group_addr;                /*!< group address */
    zb_aps_bind_long_dst_addr_t long_addr; /*!< @see zb_asp_long_dst_addr_t */
  } u;

} ZB_PACKED_STRUCT zb_aps_bind_dst_record_t;

#define ZB_APS_IS_TRANS_INDEX(dtbli, tansi) \
  (ZG->aps.binding.dst_table[(dtbli)].trans_index[(tansi)/8U] & (1U << ((tansi)%8U)))

#define ZB_APS_BINDING_TABLE_WHITELIST_SIZE 4U
/**
   Binding table whitelist
 */
typedef ZB_PACKED_PRE struct zb_aps_binding_table_whitelist_s
{
  zb_uint8_t ep_id;                        /*!< Endpoint ID */
  zb_uint16_t cluster_id;                  /*!< Cluster ID */
  zb_uint8_t cluster_role;                 /*!< Cluster role */
  zb_ieee_addr_t addr;                     /*!< Whitelist address */
} ZB_PACKED_STRUCT zb_aps_binding_table_whitelist_t;

/**
   Global binding table
*/
typedef ZB_PACKED_PRE struct zb_aps_binding_table_s
{
  zb_uint8_t              src_n_elements;                               /*!< Count elements in source table */
  zb_uint8_t              dst_n_elements;                               /*!< Count elements in destination table */
  zb_uint8_t              whitelist_n_elements;                         /*!< Count elements in whitelist table */
#ifdef SNCP_MODE
  zb_uint8_t              remote_bind_offset;                           /*!< Offset to attribute id's to remote binding requests */
#else
  /* FIXME: why align here? */
  zb_uint8_t              align[1];
#endif
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t              trans_table[ZB_APS_BIND_TRANS_TABLE_SIZE];    /*!< Buffers for simultaneous sendings */
  zb_aps_bind_src_table_t src_table[ZB_APS_SRC_BINDING_TABLE_SIZE];     /*!< Source table */
  zb_aps_bind_dst_table_t dst_table[ZB_APS_DST_BINDING_TABLE_SIZE];     /*!< Destination table */
#else
  zb_uint8_t              *trans_table;
  zb_aps_bind_src_table_t *src_table;
  zb_aps_bind_dst_table_t *dst_table;
#endif
  zb_aps_binding_table_whitelist_t whitelist_table[ZB_APS_BINDING_TABLE_WHITELIST_SIZE]; /*!< Whitelist table */

} ZB_PACKED_STRUCT zb_aps_binding_table_t;

ZB_ASSERT_IF_NOT_ALIGNED_TO_4(zb_aps_binding_table_t);

/**
  Group table entry
 */
typedef ZB_PACKED_PRE struct zb_aps_group_table_ent_s
{
  zb_uint16_t    group_addr;                                    /*!< Group address */
  zb_uint8_t     endpoints[ZB_APS_ENDPOINTS_IN_GROUP_TABLE];    /*!< Endpoints table */
  zb_uint8_t     n_endpoints;                                   /*!< Count elements in endpoints
                                                                 * table */
  zb_uint8_t aligned[1];
} ZB_PACKED_STRUCT zb_aps_group_table_ent_t;


ZB_RING_BUFFER_DECLARE(zb_aps_grp_up_q, zb_uint8_t, ZB_APS_GROUP_UP_Q_SIZE);

/**
   Group addressing data structure
 */
typedef struct zb_aps_group_table_s
{
  zb_aps_group_table_ent_t groups[ZB_APS_GROUP_TABLE_SIZE]; /*!< APS group table */
  zb_uint8_t              n_groups;                         /*!< # of entries in APS group table */
#ifndef ZB_LITE_APS_DONT_TX_PACKET_TO_MYSELF
  zb_aps_grp_up_q_t        local_dup_q; /*!< queue to be used to pass sending
                                          group addresses packets to myself */
#endif
} zb_aps_group_table_t;


/**
   r21 Table 4.33 Trust Center Policy Values
 */
typedef struct zb_tc_policy_s
{
  /* TC policy attributes */
  zb_bitbool_t allow_joins:1;
  zb_bitbool_t use_white_list:1;
#ifdef ZB_SECURITY_INSTALLCODES
  zb_bitbool_t require_installcodes:1;                      /*!< bdbJoinUsesInstallCodeKey
                                                             * is equal to TRUE, the Trust
                                                             * Center only permits a node
                                                             * to join its network if
                                                             * a corresponding install
                                                             * code derived preconfigured
                                                             * link key  associated
                                                             * with the node has been
                                                             * preinstalled   */
#endif
#ifndef ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
  zb_bitbool_t update_trust_center_link_keys_required:1;
#endif
  zb_bitbool_t ignore_unsecure_tc_rejoins:1;   /*<! some devices require ignoring unsecure rejoin from their part to initialize secure rejoin*/
  zb_bitbool_t allow_tc_rejoins:1;   /*<! whether Trust Center Rejoin is allowed */
  zb_bitbool_t allow_remote_policy_change:1;
  zb_bitfield_t allow_tc_link_key_requests:2;
  zb_bitfield_t allow_application_link_key_requests:2;
  zb_bitfield_t network_key_update_method:2;
  zb_uint32_t   network_key_update_period;
  /* bdbJoiningNodeEui64 not really used: BDB spec supposes it ig global
   * variable and only single note can join at a time */
  /* bdbJoiningNodeNewTCLinkKey not really used: BDB spec supposes it ig global
   * variable and only single note can join at a time */

  zb_bitbool_t node_is_on_a_network:1; /* was ZG->nwk.handle.joined. */

  /*not used zb_bitfield_t bdb_node_join_link_key_type:2;*/ /*!< the type of link key (see
                                                * sub-clause 6.3) with which
                                                * the node was able to decrypt
                                                * the network key. @see bdb_node_join_link_key_type  */

  zb_time_t     trust_center_node_join_timeout;
  zb_bitfield_t tclk_exchange_attempts:4; /*!< the number of key establishment attempts that have been made to establish a new link key after joining.  */
  zb_bitfield_t tclk_exchange_attempts_max:4; /*!< the maximum number of key establishment attempts that will be made before giving up on the key establishment.  */

  /* node-side security policy */
  zb_bitbool_t accept_new_unsolicited_trust_center_link_key:1;
  zb_bitbool_t accept_new_unsolicited_application_link_key:1;
  zb_bitbool_t waiting_for_tclk_exchange:1;       /*!< Set to 1 when node start Link Key exchange
                                                   * procedure. Reset to zero when TC LK exchange
                                                   * successfully completes or failed */
  zb_bitbool_t is_distributed:1;

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
#ifdef ZB_ED_FUNC
  zb_bitbool_t permit_control4_network:1; /*!< if 1, Joining Control4 Network is permitted */
#endif
  zb_bitbool_t control4_network_emulator:1; /*!< if 1, Emulates Control4 Network behaviour: Transport Key ext src= 0xff,0xff(...) */
#endif /* defined ZB_ED_FUNC && defined ZB_CONTROL4_NETWORK_SUPPORT */
  zb_bitbool_t aps_unencrypted_transport_key_join:1;         /*for joining the devices requiring APS unencrypted Transport key*/
  zb_bitbool_t tc_swapped:1;                                 /*!< 1 if TC is just swapped.  */
  zb_bitfield_t authenticate_always:1;                        /*!< If 1, then zb_authenticate_dev()
                                                               * ignore permit_join value */
  zb_bitbool_t allow_unsecure_tc_rejoins:1;   /*<! allow joiner devices perform TC rejoin, when there is
                                                   no unique TCLK */
}
zb_tc_policy_t;

#ifndef ZB_SECURITY_INSTALLCODES
#define ZB_JOIN_USES_INSTALL_CODE_KEY(is_client) ZB_FALSE
#elif defined ZB_SECURITY_INSTALLCODES_ONLY
#define ZB_JOIN_USES_INSTALL_CODE_KEY(is_client) ZB_TRUE
#else
zb_bool_t zdo_secur_must_use_installcode(zb_bool_t is_client);
#define ZB_JOIN_USES_INSTALL_CODE_KEY(is_client) zdo_secur_must_use_installcode(is_client)
#endif

/**
   APS Informational Base memory-resident data structure
 */
typedef struct zb_apsib_s
{
  zb_uint8_t  aps_counter;

/**
   Start (field name) of the APSIB section to be saved in NVRAM
 */
#define APSIB_SAVE_START aps_designated_coordinator

  /* table 2.138 - Startup parameters */
  zb_bitfield_t    aps_designated_coordinator : 1; /*!< This boolean flag indicates whether the
                                            device should assume on startup that it must
                                            become a Zigbee coordinator.  */
  zb_bitfield_t   aps_insecure_join : 1; /*!< A boolean flag, which defaults to TRUE and
                                   indicates whether it is OK to use insecure
                                   join on startup.  */

  zb_bitfield_t   aps_use_nvram:1; /*!< if 1, use nvram (load/save)   */
  zb_bitfield_t   aps_nvram_erase_at_start:1; /*!< if 1, erase nvram at start  */
  zb_bitfield_t   always_rejoin:1; /*!< if 1, forbidden Association if Rejoin fail */
  zb_bitfield_t   is_tc:1;             /*!< true if this is a trusted center */
  zb_bitfield_t   tc_address_locked:1; /*!< true if TC address was locked */
  zb_bitfield_t   reserve : 1;

  zb_channel_list_t aps_channel_mask_list; /*!< This is the masks list containing allowable
                                                                    * channels on which the device may attempt
                                                                    * to form or join a network at startup time. */
  zb_ext_pan_id_t aps_use_extended_pan_id; /*!< The 64-bit identifier of the network to join
                                             or form.  */
//#ifndef ZB_COORDINATOR_ONLY
  zb_ieee_addr_t  trust_center_address;    /*!< Trust Center IEEE address */
//#endif

#ifndef ZB_NO_NWK_MULTICAST
  zb_uint8_t      aps_nonmember_radius;     /*!< Non-member radius for NWK multicast, a value of 0x07 is treated as infinity see 3.3.1.8.2 sub-clause */

  /* TODO: What value is required for mcf.max_nonmember_radius, where it should be set? */
  zb_uint8_t      aps_max_nonmember_radius; /*!< Maximum non-member radius value for multicast transmission */
#endif                                      /* ZB_NO_NWK_MULTICAST */
  zb_uint32_t outgoing_frame_counter; /*!< OutgoingFrameCounter for APS security
                                       * Aux header when using standard key */

  zb_uint8_t tc_standard_key[ZB_CCM_KEY_SIZE];      /*!< Trust Center Standard Key */
#ifdef ZB_DISTRIBUTED_SECURITY_ON
  zb_uint8_t tc_standard_distributed_key[ZB_CCM_KEY_SIZE];      /*!< Distributed Standard Key */
#endif
  zb_uint8_t coordinator_version; /* Value from the node descriptor */
  zb_aps_device_key_pair_storage_t aps_device_key_pair_storage; /*!< APS Application Key pair table */

#if defined ZB_COORDINATOR_ROLE && defined ZB_SECURITY_INSTALLCODES
#ifndef ZB_CONFIGURABLE_MEM
  zb_aps_installcode_storage_t installcodes_table[ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE];
#else
  zb_aps_installcode_storage_t *installcodes_table;
#endif
  zb_secur_ic_add_t *installcode_to_add;
#endif
#ifdef ZB_SECURITY_INSTALLCODES
  zb_uint8_t installcode[ZB_CCM_KEY_SIZE+ZB_CCM_KEY_CRC_SIZE];
  zb_uint8_t installcode_type;
#endif

#ifdef APS_FRAGMENTATION
  zb_uint8_t aps_interframe_delay;
  /* TODO: Should be array of endpoints */
  zb_uint8_t aps_max_window_size;
#endif
  zb_tc_policy_t tcpolicy;
  zb_uint8_t    bdb_remove_device_param; /*!< Used to store buffer param with Remove zb. */

} zb_apsib_t;

#ifdef ZB_DISTRIBUTED_SECURITY_ON
#define IS_DISTRIBUTED_SECURITY() zb_aib_tcpol_get_is_distributed_security()
#else
#define IS_DISTRIBUTED_SECURITY() ZB_FALSE
#endif

#ifdef ZB_CONTROL4_NETWORK_SUPPORT
#define IS_CONTROL4_NETWORK_EMULATOR() ZB_TCPOL().control4_network_emulator
#else
#define IS_CONTROL4_NETWORK_EMULATOR() ZB_FALSE
#endif

/**
 * @name APS retransmission state
 * @anchor aps_retrans_ent_state
 *
 * Note: These values were members of `enum zb_aps_retrans_ent_state_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define  ZB_APS_RETRANS_ENT_FREE                                          0U
#define  ZB_APS_RETRANS_ENT_SENT_MAC_NOT_CONFIRMED_ALRM_RUNNING           1U
#define  ZB_APS_RETRANS_ENT_SENT_MAC_NOT_CONFIRMED_APS_ACKED_ALRM_RUNNING 2U
#define  ZB_APS_RETRANS_ENT_SENT_MAC_CONFIRMED_ALRM_RUNNING               3U
#define  ZB_APS_RETRANS_ENT_KILL_AT_MAC_CONFIRM                           4U
#define  ZB_APS_RETRANS_ENT_FRAG_SENT_MAC_CONFIRMED_AFTER_APS_ACKED       5U
/** @} */

/* Moved zb_aps_retrans_ent_t to zboss_api_internal.h */

typedef struct zb_aps_retrans_s
{
#ifndef ZB_CONFIGURABLE_MEM
  zb_aps_retrans_ent_t hash[ZB_N_APS_RETRANS_ENTRIES];      /*!< */
#else
  zb_aps_retrans_ent_t *hash;
#endif
  zb_uint8_t           n_packets;
} zb_aps_retrans_t;


typedef struct zb_aps_tmp_s
{
  zb_uint8_t            neighbor_table_iterator;   /*!< */
} zb_aps_tmp_t;

#define ZB_APS_SET_ZDO_ED_SCAN_FLAG()   ZG->aps.zdo_ed_scan = 1;
#define ZB_APS_CLEAR_ZDO_ED_SCAN_FLAG() ZG->aps.zdo_ed_scan = 0;
#define ZB_APS_GET_ZDO_ED_SCAN_FLAG()   ZG->aps.zdo_ed_scan

#define ZB_APS_SET_CHANNEL_MANAGER_ED_SCAN_FLAG()   ZG->aps.chm_ed_scan = 1;
#define ZB_APS_CLEAR_CHANNEL_MANAGER_ED_SCAN_FLAG() ZG->aps.chm_ed_scan = 0;
#define ZB_APS_GET_CHANNEL_MANAGER_ED_SCAN_FLAG()   ZG->aps.chm_ed_scan

/* zb_aps_dup_tbl_ent_t moved to zboss_api_core.h */

typedef struct zb_aps_duplicate_s
{
#ifndef ZB_CONFIGURABLE_MEM
  zb_aps_dup_tbl_ent_t   dups_table[ZB_APS_DUPS_TABLE_SIZE]; /*!< Duplicates table */
#else
  zb_aps_dup_tbl_ent_t   *dups_table;
#endif
} zb_aps_duplicate_t;

/**
 * @name APS input fragmentation stages
 * @anchor aps_in_frag_states
 *
 * Note: These values were members of `enum zb_aps_in_frag_states_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define APS_IN_FRAG_EMPTY                0x00U
#define APS_IN_FRAG_RECEIVING            0x01U
#define APS_IN_FRAG_WINDOW_RECEIVED      0x02U
#define APS_IN_FRAG_WINDOW_MERGED_NO_ACK 0x03U
#define APS_IN_FRAG_WINDOW_MERGED        0x04U
#define APS_IN_FRAG_COMPLETE             0x05U
/** @} */

typedef ZB_PACKED_PRE struct zb_aps_in_fragmented_frame_s
{
#define ZB_APS_IN_FRAGMENTED_FRAME_EMPTY 0xFFFFu
/* 07/31/2019 EE CR:MINOR Why not addr ref? */
  zb_uint16_t src_addr;   /* 0xFFFF for empty entries */
  zb_uint8_t aps_counter;
  zb_uint8_t total_blocks_num;
  zb_uint8_t current_window;
  zb_bufid_t window_buffers[ZB_APS_MAX_FRAGMENT_NUM_IN_WINDOW];
  zb_bufid_t buffer;
  ZB_PACKED_PRE struct
  {
    zb_bitbool_t  assemble_in_progress:1; /* probably, we don't need that */
    zb_bitbool_t  ack_timer_scheduled:1;
    zb_bitfield_t state:3;
    zb_bitfield_t aps_ack_retry_cnt:3;
  } ZB_PACKED_STRUCT flags;
} ZB_PACKED_STRUCT zb_aps_in_fragment_frame_t;

/**
 * @name APS output fragmentation stages
 * @anchor aps_out_frag_stages
 *
 * These values were members of `enum zb_aps_out_frag_stages_e` type but were converted to a
 * set of macros due to MISRA violations.
 */
/** @{ */
#define APS_FRAG_INACTIVE         0x00U /*!< Fragmentation queue empty */
#define APS_FRAG_RECEIVED         0x01U /*!< apsde_data received */
#define APS_FRAG_GET_MAX_TRANSFER 0x02U /*!< Send node descriptor and receive max transfer size */
#define APS_FRAG_CHAIN_PREPARED   0x03U /*!< Prepare chain */
#define APS_FRAG_BUFFERS_PREPARED 0x04U /*!< Prepare buffers */
#define APS_FRAG_TRANSMISSION     0x05U /*!< Fragmented transmission in progress */
#define APS_FRAG_FINALIZING       0x06U /*!< Transmission completed, to free chains */
/** @} */

#define ZB_APS_BLOCK_REF_SIZE 32U
#define ZB_APS_BLOCK_MASK_SIZE (ZB_APS_BLOCK_REF_SIZE/8U)
typedef struct zb_aps_out_fragmented_frame_s
{
  zb_uint8_t aps_counter;
  zb_uint8_t total_blocks_num;
  zb_uint8_t blocks_sent_mask[ZB_APS_BLOCK_MASK_SIZE];
  zb_uint8_t blocks_retry_mask[ZB_APS_BLOCK_MASK_SIZE];
  zb_uint8_t current_window;
  zb_uint8_t block_ref[ZB_APS_BLOCK_REF_SIZE];

  zb_uint8_t retry_count;
  /* ZB_APS_RETRANS_ENT_SENT_MAC_NOT_CONFIRMED_ALRM_RUNNING case: block_num and block_ack of
   * received ack */
  zb_uint8_t wait_block_ack;
  zb_uint8_t wait_block_num;
  zb_uint8_t state;
  /* 2019-03-06 CR:MAJOR What is the reason of this field? It is not used anywhere in
   * the code. */
  zb_uint8_t dst_max_in;
  zb_uint8_t data_param;
  zb_bool_t transmission_is_scheduled;
  zb_uint8_t addr_mode;
} zb_aps_out_fragment_frame_t;

#ifdef APS_FRAGMENTATION
#define ZB_APS_INVALID_MAX_TRANS_SIZE 0U
#define ZB_APS_INVALID_MAX_BUFFER_SIZE 0U
#define ZB_APS_MAX_TRANS_SIZE_TABLE_SIZE 10U
//#define ZB_APS_MAX_TRANS_SIZE_TABLE_SIZE 30
/* FIXME: Is 1000 sec enough? Looks like we do not need to update it too frequently. */
/* Timeouts changed to reduce the number of extra sleep/awake pairs on hardware */
#define ZB_APS_MAX_TRANS_SIZE_CLOCK 20U
#define ZB_APS_MAX_TRANS_SIZE_TMO (50U * ZB_TIME_ONE_SECOND)

typedef struct zb_aps_max_trans_size_s
{
  zb_uint8_t addr_ref;
  zb_uint16_t max_in_trans_size;
  zb_uint8_t max_buffer_size;
  zb_uint8_t clock;
} zb_aps_max_trans_size_t;
#endif

typedef void (*zb_aps_binding_added_handler_t)(zb_uint16_t bind_tbl_idx);

typedef struct zb_aps_func_selector_s
{
  zb_callback_t authhenticate_child_directly;
  zb_aps_binding_added_handler_t new_binding_handler;
} zb_aps_func_selector_t;

#define APS_OUT_FRAG_QUEUE_SIZE 2U
ZB_RING_BUFFER_DECLARE(zb_aps_out_frag_q, zb_bufid_t, APS_OUT_FRAG_QUEUE_SIZE);


/**
   APS subsystem globals
 */
typedef struct zb_aps_globals_s
{
  zb_apsib_t             aib;           /*!< AIB parameters */
  zb_aps_binding_table_t binding;       /*!< Binding table */
  zb_aps_retrans_t       retrans;       /*!< Retrans table */

  zb_aps_group_table_t   group;         /*!< Group table */
  zb_aps_tmp_t           tmp;           /*!< */
  zb_aps_duplicate_t     dups;
  zb_aps_func_selector_t selector;

  /* Lets use 1 simultaneous fragment rx and 1 simultaneous fragment tx (does not affect
     non-fragment frames). Specification allows that:

     2.2.8.4.5  Fragmented Transmissions - Reception and Rejection, and Acknowledgements
     If an incoming fragmented transaction is already in progress but the addressing and APS
     counter fields do not match those of the received frame, then the received frame may
     optionally be rejected or handled independently as a further transaction.

     TODO: May extend to 1 fragmented transaction from 1 neighbor (if needed).
 */
#ifdef APS_FRAGMENTATION
  zb_aps_in_fragment_frame_t in_frag[ZB_APS_MAX_IN_FRAGMENT_TRANSMISSIONS];
  zb_aps_out_fragment_frame_t out_frag;
  zb_aps_max_trans_size_t max_trans_size[ZB_APS_MAX_TRANS_SIZE_TABLE_SIZE];
  zb_aps_out_frag_q_t    out_frag_q;
#endif

  zb_bitbool_t              authenticated:1;                   /*!< if ZB_TRUE, the device is authenticated */
  zb_bitbool_t              dups_alarm_running:1;              /*!< if ZB_TRUE, the dups packet is processed */
  zb_bitbool_t              max_trans_size_alarm_running:1;    /*!< if ZB_TRUE, we are process dups packet */
  zb_time_t                 dups_alarm_start;
#ifdef ZB_APS_USER_PAYLOAD
  zb_aps_user_payload_callback_t aps_user_payload_cb;
#endif
} zb_aps_globals_t;

#ifdef APS_FRAGMENTATION
#define ZB_INVALID_FRAG_ID 0xFFU
ZB_ASSERT_COMPILE_DECL(ZB_APS_MAX_IN_FRAGMENT_TRANSMISSIONS < ZB_INVALID_FRAG_ID);
#endif

#ifdef ZB_APS_USER_PAYLOAD
zb_bool_t zb_aps_call_user_payload_cb(zb_uint8_t param);

#define ZB_APS_CALL_USER_PAYLOAD_CB(param) zb_aps_call_user_payload_cb(param)
#else
#define ZB_APS_CALL_USER_PAYLOAD_CB(param) ZB_FALSE
#endif /* ZB_APS_USER_PAYLOAD */

/**
   Get APS counter field from AIB.

   @return APS counter
 */
#define ZB_AIB_APS_COUNTER() ZG->aps.aib.aps_counter

/**
   Increment APS counter AIB field.
*/
#define ZB_AIB_APS_COUNTER_INC() ZG->aps.aib.aps_counter++

#define ZB_AIB() ZG->aps.aib

#define APS_SELECTOR() ZG->aps.selector

#define ZB_AIB_APS_CHANNEL_MASK(idx) ZG->aps.aib.aps_channel_mask_list[(idx)]

/**
   Macro to access TC policy implemented according to r21
 */
#define ZB_TCPOL() ZG->aps.aib.tcpolicy

void zb_aib_tcpol_set_update_trust_center_link_keys_required(zb_bool_t enable);
zb_bool_t zb_aib_tcpol_get_update_trust_center_link_keys_required(void);
zb_bool_t zb_aib_tcpol_get_allow_unsecure_tc_rejoins(void);

#ifdef ZB_DISTRIBUTED_SECURITY_ON
void zb_aib_tcpol_set_is_distributed_security(zb_bool_t enable);
void zb_aib_tcpol_clear_is_distributed_security(void);
zb_bool_t zb_aib_tcpol_get_is_distributed_security(void);
#endif /* ZB_DISTRIBUTED_SECURITY_ON */

void zb_aib_set_trust_center_address(const zb_ieee_addr_t address);
void zb_aib_get_trust_center_address(zb_ieee_addr_t address);
zb_uint16_t zb_aib_get_trust_center_short_address(void);
zb_bool_t zb_aib_trust_center_address_zero(void);
zb_bool_t zb_aib_trust_center_address_unknown(void);
zb_bool_t zb_aib_trust_center_address_cmp(const zb_ieee_addr_t address);

void zb_aib_set_coordinator_version(zb_uint8_t version);
zb_uint8_t zb_aib_get_coordinator_version(void);

void zb_aib_get_channel_page_list(zb_channel_list_t list);

void zb_aib_tcpol_set_authenticate_always(zb_bool_t authenticate_always);

/**
   Macro to access BDB attributes.

   Partially shared with TC policy, so use same data structure and sometimes
   fields aliases
 */

/* ZB_BDB was an alias to TCPOLICY. To divide ZBOSS by NCP Host and SoC let's
 * put commissioning-related BDB context into ZCL while keep security-related
 * fields in TCPOL trying to make BDB security more generic. */
#define ZB_BDB() BDB_COMM_CTX()

#ifdef ZB_BDB_MODE

#ifndef ZB_LITE_BDB_ONLY_COMMISSIONING
#define ZB_IN_BDB() (ZB_COMMISSIONING_TYPE() == ZB_COMMISSIONING_BDB)
#else
#define ZB_IN_BDB() ZB_TRUE
#endif

#else
#define ZB_IN_BDB() ZB_FALSE
#endif

#define ZG_APS_BIND_DST_TABLE( _p ) ZG->aps.binding.dst_table[ _p ]

#define ZB_APS_DUPS() ZG->aps.dups

/** @def ZB_SE_MODE
 *  @brief if 1, then SE mode is enabled
 */
#if defined ZB_ENABLE_SE || defined SNCP_MODE
#define ZB_SE_MODE() (ZB_COMMISSIONING_TYPE() == ZB_COMMISSIONING_SE)
#else
#define ZB_SE_MODE() ZB_FALSE
#endif

/*! @} */
/*! @endcond */

#endif /* ZB_APS_GLOBALS_H */
