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
/* PURPOSE: Global context definition
*/

#ifndef ZB_G_CONTEXT_H
#define ZB_G_CONTEXT_H 1

#include "zb_osif.h"

#ifdef ZB_MINIMAL_CONTEXT
#error "This header should not be used with ZB_MINIMAL_CONTEXT macro"
#endif /* ZB_MINIMAL_CONTEXT */

#if defined ZB_CONFIGURABLE_MEM || defined DOXYGEN
/*
  When configuring some memory object sizes at application link time
  instead of ZBOSS library compile time, use variables instead of
  constants in ZBOSS.

  Keep old macros to prevent massive ZBOSS
  sources change and to be able to switch off configurable memory
  feature.

  Declarations of variables defining parameters sizes.
  Also redefinitions of constants used in ZBOSS normally.
 */

/**
@internal
@{
@addtoroup configurable_mem_internals
@{
 */
/**
   Redefinition of ZB_IOBUF_POOL_SIZE: use variable gc_pool_size inside the stack kernel.
 */
extern zb_uint_t gc_pool_size;
#define ZB_IOBUF_POOL_SIZE gc_pool_size

extern zb_uint8_t gc_buf_pool_bitmap_size;
#define ZB_BUF_POOL_BITMAP_SIZE gc_buf_pool_bitmap_size

extern zb_uint_t gc_mac_pending_queue_size;
#define ZB_MAC_PENDING_QUEUE_SIZE gc_mac_pending_queue_size

extern zb_uint_t gc_n_aps_retrans_entries;
#define ZB_N_APS_RETRANS_ENTRIES gc_n_aps_retrans_entries

extern zb_uint_t gc_sched_q_size;
#define ZB_SCHEDULER_Q_SIZE gc_sched_q_size

extern zb_uint_t gc_n_aps_key_pair;
#define ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE gc_n_aps_key_pair

extern zb_uint_t gc_addr_table_size;
#define ZB_IEEE_ADDR_TABLE_SIZE gc_addr_table_size

#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
extern zb_uint_t gc_child_hash_table_size;
#define ZB_CHILD_HASH_TABLE_SIZE gc_child_hash_table_size
#endif  /* ZB_MAC_SOFTWARE_PB_MATCHING */

extern zb_uint_t gc_neighbor_table_size;
#define ZB_NEIGHBOR_TABLE_SIZE gc_neighbor_table_size

extern zb_uint_t gc_routing_table_size;
#define ZB_NWK_ROUTING_TABLE_SIZE gc_routing_table_size

extern zb_uint_t gc_aps_dups_table_size;
#define ZB_APS_DUPS_TABLE_SIZE gc_aps_dups_table_size

extern zb_uint8_t gc_aps_bind_src_table_size;
#define ZB_APS_SRC_BINDING_TABLE_SIZE gc_aps_bind_src_table_size

extern zb_uint8_t gc_aps_bind_dst_table_size;
#define ZB_APS_DST_BINDING_TABLE_SIZE gc_aps_bind_dst_table_size

extern zb_uint8_t gc_trans_table_size;
#define ZB_APS_BIND_TRANS_TABLE_SIZE gc_trans_table_size

extern zb_uint8_t gc_single_trans_index_size;
#define ZB_SINGLE_TRANS_INDEX_SIZE gc_single_trans_index_size

extern zb_uint_t gc_sched_stack_unprotected_q_size;
#define ZB_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL gc_sched_stack_unprotected_q_size

extern zb_uint8_t gc_nwk_max_source_routes;
#define ZB_NWK_MAX_SRC_ROUTES gc_nwk_max_source_routes

/**
 @}
 @}
*/

#endif  /* ZB_CONFIGURABLE_MEM */


/*! @cond internals_doc */
/*! \addtogroup ZB_BASE */
/*! @{ */

/**
   \par Define 'global context' - common global data structure.
   Vladimir got strange problems in Keil with accessing global variables
   implemented in another modules.
   Also, wants to easily track memory usage.
   So, unite it all into global context.

   Initially suppose global context will be defined here and implemented in the
   single .c module and passed to all functions by pointer.
   To be able to exclude such pointer pass let's define global context access by
   a macro. The macro can be easily substituted by the global variable access
   or parameter access.

   Globals can be accessed using ZG macro in constructions like ZG->foo.

   Some subsystems has its own structures in the globals (for example, APS
   globals). It can be accesses by special macros, like APSG->bar.
 */

struct zb_globals_s;
typedef struct zb_globals_s zb_globals_t;

struct zb_intr_globals_s;
typedef ZB_VOLATILE struct zb_intr_globals_s zb_intr_globals_t;

extern zb_globals_t g_zb;
extern zb_intr_globals_t g_izb;

/**
   Macro to access globals
 */
/* Hope compiler can optimize &g_zb-> to g_zb. */
#define ZG (&g_zb)
#define ZIG (&g_izb)

/*
  Per-subsystem globals files are named like zb_xxx_globals.h and included here.
 */
#include "zb_scheduler.h"
#include "zb_sleep.h"
#include "zb_bufpool_globals.h"
#if (!(defined ZB_MACSPLIT_DEVICE)) || (defined ZB_TH_ENABLED)
#include "zb_addr_globals.h"
#include "zb_nwk_globals.h"
#include "zb_aps_globals.h"
#include "zb_zdo_globals.h"
#include "zb_commissioning.h"
#endif  /* (!(defined ZB_MACSPLIT_DEVICE)) || (defined ZB_TH_ENABLED) */

#ifdef ZB_ENABLE_ZLL
#include "zll/zb_zll_config.h"
#include "zboss_api_tl.h"
#endif

#include "zb_ha.h"
#include "zb_zcl.h"

#ifdef ZB_ENABLE_ZLL
#include "zll/zb_zll_common.h"
#endif

#include "zb_time.h"

#ifdef ZB_USE_NVRAM
#include "zb_nvram_globals.h"
#endif

#ifdef ZB_USE_BUTTONS
#include "zb_led_button.h"
#endif

#ifdef ZB_USE_ERROR_INDICATION
#include "zb_error_indication.h"
#endif

#ifdef USE_ZB_WATCHDOG
#include "zb_watchdog.h"
#endif

#ifdef ZB_ENABLE_SE_MIN_CONFIG
#include "zb_se.h"
#endif

#ifdef ZB_TH_ENABLED
#include "zb_th_internal.h"
#endif

/* Declaration of zb_io_ctx_t on some MAC platforms, like nsng */
#include "zb_mac.h"

#if defined ZB_MACSPLIT
/* Host side of out MAC split - for zb_io_ctx_t */
#include "zb_macsplit_transport.h"
#endif /* defined ZB_MACSPLIT */

/* Alien MAC connected via some serial protocol: not our MAC split,
 * but similar concept. In Linux it shares some i/o logic with our
 * macsplit. Need it here for for zb_io_ctx_t. */
#if defined ZB_ALIEN_SERIAL_MAC
#include "zb_alien_mac_transport.h"
#endif

#if defined TC_SWAPOUT && defined ZB_COORDINATOR_ROLE
#include "zb_tcswap.h"
#endif


typedef struct zb_sec_globals_s
{
  zb_bufid_t              encryption_buf[ZB_NWK_MAC_IFACE_TBL_SIZE]; /* buffer used for NWK encryption */

/* 08/22/2018 EE CR:MINOR APS_FRAME_SECURITY is never used now. */
/* MAC-split host is not considered as alien MAC in multi-MAC configuration, but
 * it also requires second encryption buffer */
#if defined ZB_ALIEN_MAC || defined ZB_ENABLE_ZGP_SECUR || defined APS_FRAME_SECURITY || !defined ZB_MAC_INTERFACE_SINGLE
  zb_bufid_t              encryption_buf2[ZB_NWK_MAC_IFACE_TBL_SIZE]; /* buffer used for APS encryption */
#endif
#if defined TC_SWAPOUT && defined ZB_COORDINATOR_ROLE
  zb_tcswap_t tcswap;
#endif
#ifdef SNCP_MODE
  zb_bitbool_t accept_partner_lk:1;   /**!< enable/disable acceptance of partner LK establishment procedure */
#endif
} zb_sec_globals_t;

#define SEC_CTX() ZG->sec



/* CR: 04/13/2016 CR:MINOR EE: change comments documenting this structure and
 * its fields to be used by Doxygen */

#ifdef ZB_CERTIFICATION_HACKS
/* Flags and attributes for tests, allows to switch stack behavior in run-time.
 * Used to simulate legacy device behavior, error behavior e.t.c.
 * By default set to zero and does not affect stack behavior.
 */

/** @brief Informs application that Request Key is received during TCLK update
    @param param - buffer index
    @param keypair_i - index in apsLinkKeyPairSet corresponding to device
                       from which the current command is received
    @returns ZB_TRUE if apsme_request_key.indication  will be dropped
             ZB_FALSE if command will be proceeded as usual
*/
typedef zb_bool_t (*zb_req_key_ind_cb_t) (zb_uint8_t param, zb_uint16_t keypair_i);

/** @brief Informs application that CCM was calculated (before frame will be encrypted)
    @param ccm_p - pointer to calculated ccm (16 bytes in length)
    @param hdr   - pointer to APS or NWK hdr
    @param hdr_len - size of hdr in bytes
    @returns ZB_TRUE if application does not wants to call this callback in future
             ZB_FALSE otherwise
*/
typedef zb_bool_t (*zb_ccm_check_cb_t) (zb_uint8_t *ccm_p,
                                        zb_uint8_t *hdr,
                                        zb_ushort_t hdr_len);

/** @brief Called when APS adds security header (before adding counter to packet)
           to outgoing packet.
    @param p_sec_counter - pointer to counter that will be added to packet,
                           change if needed
*/
typedef void (*zb_aps_secur_counter_hack_cb_t) (zb_uint32_t *p_sec_counter);

/** @brief Called on joiner (in BDB) before apsme_request_key or apsme_verify_key request.
    @param param - buffer index
    @returns ZB_TRUE if request primitive will be dropped
             ZB_FALSE otherwise
*/
typedef zb_bool_t (*zb_req_call_cb_t) (zb_uint8_t param);

/** @brief Called on ZDO CLI command receiving.
    @param param - buffer index
    @returns ZB_TRUE if the command was handled by application side
             ZB_FALSE otherwise
*/
typedef zb_bool_t (*zb_zdo_af_handler_cb) (zb_uint8_t param, zb_uint16_t cb_param);

/**
   Flags, callbacks and attributes for tests, allows to switch stack behavior in run-time.
   Note: some customers express their concern regarding existing of hacks in ZBOSS.
   Don't worry, please: that 'hacks' are flags forcing some Test Harness behavior in the stack core:
   to simulate legacy device behavior, error behavior e.t.c.
   By default set to zero and does not affect stack behavior.
 */
typedef struct zb_cert_hacks_s
{
  zb_uint8_t     src_binding_table_size;         /*!< Use variables instead of ZB_APS_SRC_BINDING_TABLE_SIZE
                                                      and ZB_APS_DST_BINDING_TABLE_SIZE.
                                                      @see test PRO_R21/certification/TP_APS_BV-20-I:
                                                      limit binding table size. By default both are set to
                                                      ZB_APS_SRC_BINDING_TABLE_SIZE and
                                                      ZB_APS_DST_BINDING_TABLE_SIZE correspondingly. */
  zb_uint8_t     dst_binding_table_size;         /*!< see comment for src_binding_table_size */
  zb_ieee_addr_t auth_excluded_dev_addr;         /*!< Exclude authentication for this device. Simple
                                                      special feature for test TP/SEC/BV-29-I.
                                                      "gZC causes DUT ZED2 to be removed from the network
                                                      via APS Remove by including ZED2 in the exclusion list
                                                      of the trust center".
                                                      This is not full-features exclusion list. */
  zb_uint8_t     override_nwk_protocol_version;  /*!< Override protocol version in nwk header
                                                      debug feature for 13.68 NWK Unknown NWK
                                                      Protocol Version. */
  zb_int16_t     send_update_device_unencrypted; /*!< Debug stuff to send next update_device unsecured.
                                                      If <= 0 (default), switch off this debug trick.
                                                      If > 0, send send_update_device_unencrypted times
                                                      encrypted, then once unencrypted, then always
                                                      encrypted.
                                                      @see tp_r20_bv-01_zr.c */

  zb_req_key_ind_cb_t            req_key_ind_cb;            /*!< This callback is called on TC when
                                                                 TC receives Request Key command
                                                                 during TC LK update.
                                                                 @ee tests CS-ICK-TC-02 and
                                                                 CS-ICK-TC-03 */
  zb_req_call_cb_t               req_key_call_cb;           /*!< This callback called on joiner
                                                                 device before it calls
                                                                 APSME.REQUEST.KEY primitive.
                                                                 Passed up with buffer in params.
                                                                 @see test CS-ICK-TC-02. */
  zb_req_call_cb_t               verify_key_call_cb;        /*!< This callback called on joiner
                                                                 device before it calls
                                                                 APSME.VERIFY.KEY primitive.
                                                                 Passed up with buffer in params.
                                                                 @see test CS-ICK-TC-02. */
  zb_callback_t                  deliver_nwk_key_cb;        /*!< This callback called before TC
                                                                 initiates transporting nwk key to
                                                                 joiner (in bdb mode).
                                                                 Passed up with buffer in params.
                                                                 @see test CS-ICK-TC-03. */
  zb_callback2_t                 deliver_conf_key_cb;       /*!< This callback called before TC
                                                                 sends confirm key to joiner (TC
                                                                 link key).
                                                                 Passed up with buffer and keypair_i
                                                                 (for joiners apsDeviceKeyPairSet entry)
                                                                 in params.
                                                                 @see test CS-NFS-TC-01. */
  zb_ccm_check_cb_t              ccm_check_cb;              /*!< Called before frame encryption.
                                                                 @see cs-nfs-tc-01 */
  zb_aps_secur_counter_hack_cb_t secur_aps_counter_hack_cb; /*!< Called before APS frame security
                                                                 counter will be added to packet.
                                                                 @see cs-nfs-tc-01 */
  zb_zdo_af_handler_cb              zdo_af_handler_cb;      /*!< Callback that will be called when
                                                                 device receives ZDO command.
                                                                 Pass buffer param and Cluster_Id
                                                                 in arguments.
                                                                 @see fb-pre-tc-03a */

  zb_bitfield_t override_tc_significance_flag: 1;              /*!< Allows to override default value
                                                                    of tc_significance in MgmtPermitJoin;
                                                                    Default value is 0 -
                                                                    tc_significance can
                                                                    not be overwritten by application.
                                                                    @see cn-nst-tc-10 */
  zb_bitfield_t disable_end_device_timeout_request_at_join: 1; /*!< Disable sending End Device
                                                                    Timeout Request to parent when
                                                                    ED joining.Used to simulate
                                                                    legacy end device in
                                                                    tp_r21_bv-19 test. */
  zb_bitfield_t enable_leave_to_router_hack: 1;                /*!< This flag is used in r20 and
                                                                    earlier tests to allow sending
                                                                    nwk leave to router.
                                                                    According to 3.2.2.16.3
                                                                    (NLME-LEAVE.request: Effect on
                                                                    Receipt) of r21 Router and
                                                                    Coordinator can send nwk leave
                                                                    only to End Device childrens. */
  zb_bitfield_t disable_association_response: 1;               /*!< Toggle this flag to disable/enable
                                                                    sending association response
                                                                    in run-time. See cn-nsa-tc-03.
                                                                    If set to 1 buffer with
                                                                    association indication primitive
                                                                    will be dropped in
                                                                    zb_mlme_associate_indication.
                                                                    @see cn-nsa-tc-03. */
  zb_bitfield_t report_legacy_stack_revision_in_node_descr: 1; /*!< Simulate legacy device: stack
                                                                    revision field in server flags
                                                                    will be set to 0. */
  zb_bitfield_t force_ext_addr_req: 1;                         /*!< force ext addr req for BDB
                                                                    certification tests
                                                                    @see test FB-PRE-TC-01A (dutzr). */
  zb_bitfield_t disable_aps_acknowledgements: 1;               /*!< Disable APS acks for incoming
                                                                    aps-data frames. */
  zb_bitfield_t disable_in_out_cost_updating: 1;               /*!< For TP_PRO_BV_04 need not change in/out
                                                                    costs by LQI */
  zb_bitfield_t aps_security_off: 1;                           /*!< run-time APS security switch off.
                                                                    If TRUE, does not encrypt APS commands */
  zb_bitfield_t enable_alldoors_key: 1;                        /*!< All APS keys will be the same as default
                                                                    including pre-configured */
  zb_bitfield_t allow_nwk_encryption_for_aps_frames: 1;        /*!< Returns r20 and earlier behavior for
                                                                    encryption: allows to encrypt APS frame
                                                                    by nwk key (if set to 1).
                                                                    @see cs-nf-tc-01 */
  zb_bitfield_t drop_verify_key_indication: 1;                 /*!< Drops apsme_verify_key,indication when
                                                                    set to 1
                                                                    @see cs-nfs-tc-01 */
  zb_bitfield_t stay_on_network_after_auth_failure: 1;         /*!< If set to 1 router device (in bdb)
                                                                    will not leave network after
                                                                    authentication failure and/or
                                                                    tclk exchange failure.
                                                                    @see cs-nfs-tc-02 */
  zb_bitfield_t pass_incoming_zdo_cmd_to_app: 1;               /*!< If set to 1 stack will pass incoming
                                                                    ZDO clients commands to application.
                                                                    Use zdo_af_handler_cb to handle them.
                                                                    @see fp-bpre-tc-03a */
  zb_bitfield_t disable_frame_retransmission: 1;               /*!< If set to 1 device will not retransmit
                                                                    packet from to another devices.
                                                                    @see fp-bpre-tc-03a */
  zb_bitfield_t force_frame_indication: 1;                     /*!< If set to 1 device will deliver all
                                                                    frames ato APS (including frames destined
                                                                    to another devices).
                                                                    @see fp-bpre-tc-03a */
  zb_bitfield_t delay_pending_tx_on_rresp: 1;                  /*!< If set to 1, delay send NWK
                                                                 data when route established:
                                                                 wait for better route. Specially
                                                                 for the test TP_PRO_BV-04 */
  zb_bitfield_t use_route_for_neighbor:1; /*!< if 1, when forwarding nwk,
                                           * check for route better than
                                           * direct tx to the neighbor. Specially
                                           * for the test
                                           * TP_PRO_BV-04.   */
  zb_bitfield_t disable_cyclic_tc_rejoin: 1; /*!< If 1, device will not rejoin after Trust Center
                                              * rejoin failure - but leave network instead */
  zb_bitfield_t allow_entry_for_unregistered_ep: 1;             /*!< If 1, device will be allowed to create
                                                                  entry for unregistered endpoint
                                                                  in APS group table */
  zb_bitfield_t disable_addr_req_on_neighbor_move:1; /*!< If set to 1, disable nwk
                                                    * addr req if neighbor moved
                                                    * out of us */
  zb_bitfield_t disable_unencrypted_update_device:1; /*!< If set to 1, disable unencrypted Update device
                                                      * (simulating a legacy device) - required by TP/R20/BV-03 */
  zb_bitfield_t disable_encrypted_update_device:1; /*!< If set to 1, disable unencrypted Update device
                                                    * (simulating a legacy device) - required by TP/R20/BV-04 */
  zb_bitfield_t check_zc_long_addr_is_visible:1; /*!< If set to 1, upon receipt frame from ZC (addr = 0x0000)
                                                  * device will check that ZC long address is visible looking up to nwk header.
                                                  * Used in TP/PRO/BV-29 and TP/PRO/BV-30. */
  zb_bitfield_t disable_pan_id_conflict_detection:1; /*!< If set to 1, device will not discover PAN
                                                      * ID conflict */
  zb_bitfield_t frag_drop_1st_block_1st_time:1; /*!< If set to 1, device will drop 1st block of
                                                 * fragmented tx 1st time (TP/PRO/BV-43) */
  zb_bitfield_t frag_skip_0_and_2nd_block_1st_time:1; /*!< If set to 1, device will not send 0 and
                                                        3rd block of fragmented tx 1st time (TP/PRO/BV-44) */
  zb_bitfield_t frag_skip_node_descr:1; /*!< If set to 1, device will not send Node Descriptor
                                         * request to clarify max buffer size and max in/out
                                         * transfer size, it is already set to maximum possible */
  zb_bitfield_t frag_disable_custom_ack_timer:1; /*!< If set to 1, ZBOSS does sends APS acks for
                                                       fragmented transmissions only in the cases
                                                       required by the specs. */
  zb_bitfield_t aps_counter_custom_setup:1;            //replace APS counter
  zb_uint8_t    aps_counter_custom_value;              //custom value of APS counter
  zb_bitfield_t nwk_counter_custom_setup:1;            //replace NWK counter
  zb_uint8_t    nwk_counter_custom_value;              //custom value of NWK counter
  zb_uint16_t address_to_rejoin; /* address of device to rejoin at for TP/R21/BV-02 */
  zb_bitfield_t enable_rejoin_to_specified_device:1; /* enable rejoin at specified device */
  zb_bitfield_t set_empty_beacon_payload:1;              /* set empty beacon payload (check pan id conflict) */
  zb_bitfield_t zdo_mgmt_nwk_update_force_scan_count:1; /*!< If set to 1, scan_count field will always
                                                         * be appended to Mgmt_NWK_Update_req packet */
  zb_bitfield_t disable_addr_conflict_check_on_update_device:1; /* Disable check for address conflict
                                                                 * upon reception of Update device - refer to
                                                                 * TP/PRO/BV-17 SECURED network */
  zb_bitfield_t delayed_rejoin_resp:1;                  /* Send rejoin resp after nwk status pkt
                                                           during address conflict resolution */
  zb_bitfield_t disable_beacon_send:1;                  /* Disabled responding with a beacon TP/PRO/BV-17 */
  zb_bitfield_t disable_discovery_route:1; /*!< Disable Discovery route bitfield in NWK FC */
  zb_uint8_t disable_frame_retransmission_countdown; 	/*!< Number of retransmitted packets to another
						         *   devices, before retransmission will
						         *   be disabled.
							 *   @see FB-PRE-TC-03B */
  zb_bitfield_t disable_power_negotiation_support:1; 	/*!< Clear Power Negotiation Support bit
							 *   in the End Device Timeout Response.
							 *   @see TP/R22/SGMB-9*/
  zb_bitfield_t extended_beacon_send_jitter:1;          /*!< If set to 1, a larger jitter will be
                                                         *    used when handling a beacon request. */
  zb_bitfield_t aps_mcast_addr_overridden:1;             /*!< If set to 1, the value for NWK destination
                                                          * address will be overridden with the value
                                                          * of aps_mcast_nwk_dst_addr field  */
  zb_bitfield_t aps_disable_addr_update_on_update_device:1; /*!< Disable updating nwkAddressMap with
                                                             64bit address received via UPDATE-DEVICE. */
  zb_bitfield_t nwk_disable_passive_acks:1;              /*!< If set to 1, the device will not keep track of
                                                              received passive acks */
  zb_uint16_t aps_mcast_nwk_dst_addr; /* CCB2469: Broadcast NWK address for
                                       * APS groupcast (multicast) */
  zb_uint8_t frag_block_size; /*!< If set to a nonzero value, APS will use it as a size of block
                                payload. */
  zb_bitfield_t allow_cbke_from_zc_ncp_dev:1; /*Allow ZC/TC NCP to perform CBKE */
  zb_bitfield_t break_nwk_fcf_counter:1;      /*!< Flag to break the NWK FC failure counter once */
  zb_bitfield_t break_aps_fcf_counter:1;      /*!< Flag to break the APS FC failure counter once */
  zb_bitfield_t break_nwk_key:1;              /*!< NWK key distortion flag for one encryption operation */
  zb_bitfield_t break_aps_key:1;              /*!< APS key distortion flag for one encryption operation */
  zb_bitfield_t make_frame_not_valid:1;       /*!< Make a combination of MAC frame control fields not valid. Once. */
  zb_bitfield_t zc_flood_mode:1;              /*!< Forces the MAC to send packets without CSMA-CA */
  zb_bitfield_t use_transport_key_for_aps_frames:1; /*!< Use transport key to secure APS frame. Once. */
  zb_bitfield_t certificate_priority_from_certdb:1; /*!< Change certificate source priority for CBKE:
                                                           1. from cert db (NVRAM);
                                                           2. from production configuration.
                                                         In a normal situation, vice versa. */
  zb_bitfield_t use_preconfigured_aps_link_key:1; /*!< Use preconfigured in application APS Link Key
                                                   *   Used into TP_R20_BV-01 */
  zb_bitfield_t nwk_leave_from_unknown_addr:1; /*!< Send all nwk leave_req with src ieee addr=<nwk_leave_unknown_addr>
                                                    and short=<nwk_leave_unknown_short> */
  zb_bitfield_t low_ram_concentrator:1;       /*!< Forces coordinator to send no route cache in mtorr */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
  zb_bitbool_t zcl_subghz_cluster_test:1;          /*!< If ZCL cluster needs APS encryption - ignore it; MAC duty cycle substituted mode */
#endif

  zb_bitfield_t tc_rejoin_mac_cap_wrong_dev_type:1; /* Toggle FFD bit in MAC capabilities for rejoin req */
  zb_bitfield_t tc_rejoin_mac_cap_wrong_rx_on_when_idle:1; /* Toggle FFD bit in MAC capabilities for rejoin req */
  zb_bitfield_t aps_drop_next_ack:1; /* Drop next APS ack */
  zb_bitfield_t aps_send_dup_tunneled_frame:1;

  zb_bitfield_t tc_rejoin_aps_decrypt_error:1; /* Simulate TC rejoin without known aps key */
  zb_ieee_addr_t nwk_leave_from_unknown_ieee_addr; /*!< IEEE source address used in nwk_leave if `nwk_leave_from_unknown_addr` is set */
  zb_uint16_t nwk_leave_from_unknown_short_addr; /*!< Short source address used in nwk_leave if `nwk_leave_from_unknown_addr` is set */
} zb_cert_hacks_t;

#define ZB_CERT_HACKS() ZG->cert_hacks
#endif

#ifdef ZB_STACK_REGRESSION_TESTING_API

/* Used in custom regression ZBOSS tests
 * see: ZBOSS/tests/regression_tests/
 */

/* Flags and attributes for tests, allows to switch stack behavior in run-time.
 * Used to simulate legacy device behavior, error behavior e.t.c.
 * By default set to zero and does not affect stack behavior.
 */

typedef struct zb_reg_api_s
{
  zb_uint8_t zcl_ota_custom_query_jitter;		/*!< Custom value of ota query jitter instead of
                                                         *   randomly chosen
                                                         */
  zb_int8_t osif_interrupts_balance;                    /*!< The difference between
                                                         *   zb_nrf52840_enable_all_inter and
                                                         *   zb_nrf52840_disable_all_inter calls count
                                                         */
  zb_bool_t bdb_allow_multiple_fb_targets;              /*!< The flag to allow use
                                                         *   zb_bdb_finding_binding_target for
                                                         *   multiple endpoints in the same time
                                                         */
  zb_callback_t sched_int_cb;                           /*!< Callback called in zb_schedule_callback() */
  zb_bool_t sched_cb_int_called;                        /*!< Flag for control that interrupt was not
                                                         *   called from ZB_INT_DISABLE() section
                                                         *   in ZB_SCHEDULE_CALLBACK code */
  zb_bool_t get_buf_delayed_int_called;                 /*!< Flag for control that interrupt was not
                                                         *   called from ZB_INT_DISABLE() section
                                                         *   in zb_get_buf_delayed_2param code */
  zb_bool_t perform_crash_on_nvram_write;               /*!< The flag to perform crash
                                                         *   on NVRAM write */
  zb_bool_t disable_sending_nwk_key;                    /*!< Do not send nwk key during association to
                                                         *   the next device which attempts to associate.
                                                         *   After one first usage flag will be resetted. */
  zb_bool_t extended_beacon_send_delay;                 /*!< If set to 1, a larger delay will be
                                                         *    used when handling a beacon request. */
  zb_bool_t enable_custom_best_parent;                  /*!< Enable custom short address for the first
                                                         *   association attempt. Will be disabled
                                                         *   after used. */
  zb_bool_t set_short_custom_best_parent;               /*!< Set custom short address for the first
                                                         *   association attempt. */
  zb_bool_t disable_extended_nwk_src;                   /*!< Disable including extended src address into
                                                         *   nwk payload of packet */
  zb_bool_t allow_zero_ieee_addr;                       /*!< Allow to initialize a device with zero
                                                         *   IEEE address */
  zb_bool_t ignore_nwk_key;                             /*!< The flag to enable ignoring network key
                                                         *   from Trust Center */
  zb_bool_t disable_aps_acknowledgements;               /*!< Disable APS ACKs for incoming
                                                         *   APS packets. */
} zb_reg_api_t;

#define ZB_REGRESSION_TESTS_API() ZG->reg_api

/* This define needs as nwk key disabling is used not only for regression tests but
 * it also used in the CTH functionality.
 */

#define ZB_NWK_KEY_DISABLE() (ZB_REGRESSION_TESTS_API().disable_sending_nwk_key)
#define ZB_NWK_KEY_DISABLE_RESET() ZB_REGRESSION_TESTS_API().disable_sending_nwk_key = ZB_FALSE

#else

#define ZB_NWK_KEY_DISABLE() 0
#define ZB_NWK_KEY_DISABLE_RESET()


#endif  /* ZB_STACK_REGRESSION_TESTING_API */

#if defined NCP_MODE && !defined NCP_MODE_HOST
/*
 * Part of NCP device context, that can be configured through dynamic memory
 * configuration feature.
 */
typedef struct zb_ncp_globals_s
{
#ifndef ZB_CONFIGURABLE_MEM
  zb_ncp_pending_calls_t calls[ZB_N_BUF_IDS];
#else
  zb_ncp_pending_calls_t *calls;
#endif
}
zb_ncp_globals_t;

#define ZB_NCP_CTX_CALLS() ZG->ncp.calls

#endif /* NCP_MODE && !NCP_MODE_HOST */

/**
   Global data area for data not to be accessed from interrupt handlers
*/
struct zb_globals_s
{
  zb_sched_globals_t      sched;    /*!< Global schedule context */
  zb_buf_pool_t           bpool;    /*!< Global buffer pool context */
#if (!(defined ZB_MACSPLIT_DEVICE)) || (defined ZB_TH_ENABLED)
  zb_addr_globals_t       addr;     /*!< Global address context */
#endif /* (!(defined ZB_MACSPLIT_DEVICE)) || (defined ZB_TH_ENABLED) */

#ifndef NCP_MODE_HOST
#if (!(defined ZB_MACSPLIT_DEVICE)) || (defined ZB_TH_ENABLED)
  zb_nwk_globals_t        nwk;      /*!< Global NWK context - NIB */
  zb_aps_globals_t        aps;      /*!< Global APS context - AIB */
  zb_zdo_globals_t        zdo;      /*!< Global ZDO context - ZDO_CTX */
#endif /* (!(defined ZB_MACSPLIT_DEVICE)) || (defined ZB_TH_ENABLED) */
  zb_sec_globals_t        sec;      /*!< Global security context - SEC_CTX */
#endif /* !defined NCP_MODE_HOST */

#if !defined ZB_MACSPLIT_DEVICE || defined ZB_TH_ENABLED
  zb_commissioning_ctx_t commissioning_ctx;
#endif /* !defined ZB_MACSPLIT_DEVICE || defined ZB_TH_ENABLED */

#if defined ZB_ENABLE_ZCL || defined ZB_ENABLE_ZGPD_ATTR_REPORTING
  zb_zcl_globals_t        zcl;      /*!< Global ZCL context - ZCL_CTX */
#endif /* defined ZB_ENABLE_ZCL || defined ZB_ENABLE_ZGPD_ATTR_REPORTING */
#if defined ZB_ENABLE_ZLL
  zb_zll_ctx_t            zll;
#endif /* defined ZB_ENABLE_ZLL */
#if defined ZB_ENABLE_ZGP_INFRA
  zb_zgp_ctx_t            zgp;
#endif /* defined ZB_ENABLE_ZGP_EP */
#if defined ZB_USE_NVRAM
  zb_nvram_globals_t     nvram;     /*!< Global NVRAM context - ZB_NVRAM */
#endif // defined ZB_USE_NVRAM
#ifdef ZB_USE_BUTTONS
  zb_buttons_global_t    button;
#endif
#ifdef ZB_USE_ERROR_INDICATION
  zb_error_indication_ctx_t err_ind;
#endif /* ZB_USE_ERROR_INDICATION */
#ifdef USE_ZB_WATCHDOG
  zb_watchdog_t watchdog[ZB_N_WATCHDOG];
#endif
#ifdef ZB_CHECK_OOM_STATUS
  zb_uint8_t oom_check_enabled;
#endif
#ifdef ZB_CERTIFICATION_HACKS
  zb_cert_hacks_t cert_hacks;
#endif
#ifdef ZB_STACK_REGRESSION_TESTING_API
  zb_reg_api_t reg_api;
#endif
#ifdef ZB_USE_SLEEP
  zb_sleep_ctx_t sleep;
#endif
#ifdef ZB_TH_ENABLED
  zb_th_context_t th_ctx; /* Global test harness context. */
#endif
#if defined NCP_MODE && !defined NCP_MODE_HOST
  zb_ncp_globals_t ncp; /* Part of NCP device context. */
#endif
};

#if defined ENABLE_USB_SERIAL_IMITATOR || defined DOXYGEN

typedef struct zb_usbc_ctx_s
{
  int rpipe;
  int wpipe;
} zb_usbc_ctx_t;

#endif /* defined ENABLE_USB_SERIAL_IMITATOR || defined DOXYGEN */


/**
   Global data area for data to be accessed from interrupt handlers
 */
struct zb_intr_globals_s
{
#ifdef ZB_HAVE_IOCTX
  zb_io_ctx_t             ioctx;
#endif
/* Note: MAC split I/O context is not used on hardware, but it's used on Linux platform */
#if defined(ZB_MACSPLIT_HOST) || (defined(ZB_MACSPLIT_DEVICE) && defined(ZB_PLATFORM_LINUX))
  zb_macsplit_io_ctx_t    macsplit_ioctx;
#endif
#if defined( ENABLE_USB_SERIAL_IMITATOR )
  zb_usbc_ctx_t           usbctx; /*!< USB imitator IO context. */
#endif /* defined( ENABLE_USB_SERIAL_IMITATOR ) */
  zb_timer_t              time;
};


#define ZB_IOCTX() g_izb.ioctx
#define ZB_MACSPLIT_IOCTX() g_izb.macsplit_ioctx
#define ZB_TIMER_CTX() g_izb.time
#define SER_CTX() ZB_IOCTX().serial_ctx
#define USB_CTX() ZB_IOCTX().userial_ctx
#define SPI_CTX() ZB_IOCTX().spi_ctx


/*! @} */
/*! @endcond */

#endif /* ZB_G_CONTEXT_H */
