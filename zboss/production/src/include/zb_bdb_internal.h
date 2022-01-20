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
/* PURPOSE: Internal BDB header file.
*/

#ifndef ZB_BDB_COMISSIONING_H
#define ZB_BDB_COMISSIONING_H 1

#ifdef ZB_BDB_MODE
/**
 *  \addtogroup bdb_mode_commissioning
 *  @{
 *    @details
 *
 */

#define ZB_BDB_COMM_QUERY_TIME        4U

typedef enum bdb_join_machine_e
{
  ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_START = 0,
  ZB_BDB_JOIN_MACHINE_PRIMARY_SCAN = 1,
  ZB_BDB_JOIN_MACHINE_SECONDARY_SCAN_DONE = 3,
  ZB_BDB_JOIN_MACHINE_DEVINFO_GATHER = 4,
  ZB_BDB_JOIN_MACHINE_ADDING_TO_NETWORK = 5,
} bdb_join_machine_t;


/** @cond internals_doc */
typedef ZB_PACKED_PRE struct zb_bdb_comm_respondent_info_s
{
  zb_address_ieee_ref_t addr_ref; /*!< Address reference in Translation Table for "respondent" */
  zb_uint8_t ep_list[ZB_BDB_COMM_ACTIVE_ENDP_LIST_LEN];  /*!< Respondent's active endpoints list */
#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  zb_uint8_t ep_cnt;   /*!< Count of endpoints received during finding and binding */
#endif

  zb_uint8_t eps_checked; /*!< Count of the currently checked endpoints */
  zb_bufid_t simple_desc_resp_buf;
  zb_bufid_t curr_bind_req_buf;
  zb_uindex_t curr_cluster_idx;
} ZB_PACKED_STRUCT zb_bdb_comm_respondent_info_t;

typedef enum bdb_commissioning_signal_e
{
  BDB_COMM_SIGNAL_BAD,          /* 0 */

  BDB_COMM_SIGNAL_INIT_START,   /* 1 */
  BDB_COMM_SIGNAL_INIT_SECURE_REJOIN,
  BDB_COMM_SIGNAL_INIT_TC_REJOIN,
  BDB_COMM_SIGNAL_INIT_FINISH,

  BDB_COMM_SIGNAL_TOUCHLINK_START, /* 5 */
  BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_DONE,
  BDB_COMM_SIGNAL_TOUCHLINK_INITIATOR_FAILED,
  BDB_COMM_SIGNAL_TOUCHLINK_NOTIFY_TASK_RESULT,
  BDB_COMM_SIGNAL_TOUCHLINK_FINISH,

  BDB_COMM_SIGNAL_NETWORK_STEERING_START, /* 10 */
  BDB_COMM_SIGNAL_NETWORK_STEERING_DISCOVERY_FAILED,
  BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_EX_FAILURE,
  BDB_COMM_SIGNAL_NETWORK_STEERING_TCLK_DONE,
  /* TODO: BDB_COMM_SIGNAL_NETWORK_STEERING_LEAVE, */
  BDB_COMM_SIGNAL_NETWORK_STEERING_FINISH,

  BDB_COMM_SIGNAL_NETWORK_FORMATION_START, /* 15 */
  BDB_COMM_SIGNAL_NETWORK_FORMATION_FINISH,

  BDB_COMM_SIGNAL_FINDING_N_BINDING_START, /* 17 */
  BDB_COMM_SIGNAL_FINDING_N_BINDING_FINISH,

  BDB_COMM_SIGNAL_REJOIN_START, /* 19 */
  BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_CURRENT_CHANNEL,
  BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_CURRENT_CHANNEL,
  BDB_COMM_SIGNAL_REJOIN_TRY_TC_REJOIN_ON_ALL_CHANNELS,
  BDB_COMM_SIGNAL_REJOIN_TRY_SECURE_REJOIN_ON_ALL_CHANNELS,
  BDB_COMM_SIGNAL_REJOIN_FINISH,

  BDB_COMM_SIGNAL_FINISH, /* 25 */

  BDB_COMM_SIGNAL_NWK_FORMATION_OK, /* 26 */
  BDB_COMM_SIGNAL_NWK_START_ROUTER_CONF,
  BDB_COMM_SIGNAL_LEAVE_DONE,
  BDB_COMM_SIGNAL_NWK_JOIN_FAILED,
  BDB_COMM_SIGNAL_NWK_JOIN_DONE,
  BDB_COMM_SIGNAL_NWK_AUTH_FAILED,

  BDB_COMM_N_SIGNALS
} bdb_commissioning_signal_t;

enum bdb_commissioning_rejoin_reason_e
{
  BDB_COMM_REJOIN_REASON_UNSPECIFIED = 0,
  BDB_COMM_REJOIN_REASON_POLL_CONTROL_CHECK_IN,
};

/*!
* @ brief BDB Commissioning rejoin context
*/
typedef ZB_PACKED_PRE struct bdb_commissioning_rejoin_ctx_t
{
  zb_bitfield_t rejoin_by_checkin_failure:1;
  zb_bitfield_t rr_have_unique_tclk:1;
  zb_bitfield_t waiting:1;
  zb_bitfield_t reserved:5;

  zb_uint8_t rr_sv_device_type; /* zb_nwk_device_type_t */
  zb_uint16_t rr_sv_parent_short;
  zb_ieee_addr_t rr_sv_parent_long;
  zb_neighbor_tbl_ent_t rr_sv_parent_nent;
  zb_uint8_t rr_sv_authenticated;
  zb_uint8_t rr_retries;
  zb_uint8_t rr_ignore_start_router_conf;
  zb_uint16_t rr_global_retries;
  zb_uint8_t rr_skip_savepoint;

  bdb_commissioning_signal_t last_rejoin_signal;
  bdb_commissioning_signal_t next_rejoin_signal;
}
ZB_PACKED_STRUCT bdb_commissioning_rejoin_ctx_t;

/**
 * BDB commissioning context
 */
typedef ZB_PACKED_PRE struct zb_bdb_comm_ctx_s
{
  /** State of commissioning */
  zb_bdb_comm_state_t state;
  zb_ret_t status;
  /** Callback function invoked when factory fresh or network steering operations finish */
  zb_callback_t user_cb;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Callback function invoked when binding operation start */
  zb_bdb_comm_binding_callback_t finding_binding_progress_cb;
/* [AV] After having separated binding user callbacks from f&b complete callback
    the last one can be stored in the user_cb field. */
#endif

  /** Duration of PermitJoining and IdentifyTime */
  zb_uint16_t duration;
/*Data array to store info from Identity_Query_Resp */
  zb_bdb_comm_respondent_info_t respondent[BDB_MAX_IDENTIFY_QUERY_RESP_NUMBER];
  zb_uint8_t respondent_number;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Endpoint which participate in finding and binding*/
  zb_uint8_t ep;
#endif

  /** Endpoint descriptor*/
  zb_af_endpoint_desc_t *ep_desc;

#if defined(ZB_BDB_ENABLE_FINDING_BINDING)
  /** Count of endpoints received during finding and binding */
  zb_uint8_t ep_cnt;

  /** Signals that at least one endpoint was bound during finding and binding;
    * it is used to invoke user callback if no endpoint was bound
    */
  zb_bool_t was_bound;
#endif
  /*EZ mode in progress flag. Sets for EZScanTimeout*/
  zb_bool_t ez_scan_inprogress;
  /** Reference to the buffer holding simple descriptor response */
  zb_uint8_t simple_desc_buf_ref;

  /* ------------- */
  bdb_commissioning_signal_t signal;
  bdb_commissioning_rejoin_ctx_t rejoin;

  /* Moved here from BDB_CTX */
#define FIRST_GENERAL_BDB_FIELD bdb_commissioning_group_id
  /* BDB attributes */
  zb_uint16_t bdb_commissioning_group_id; /*!< specifies the identifier of the group on which the initiator applies finding & binding.  */
  zb_uint8_t bdb_commissioning_mode;      /*!< @see zb_bdb_commissioning_mode_mask_t See Table 4  Bits of the bdbCommissioningMode attribute:
0
Touchlink: 0 = Do not attempt Touchlink commissioning 1 = Attempt Touchlink commissioning
1
Network steering: 0 = Do not attempt network steering 1 = Attempt network steering
2
Network formation: 0 = Do not attempt to form a network 1 = Attempt to form a network, according to device type2
3
Finding & binding: 0 = Do not attempt finding & binding 1 = Attempt finding & binding
                                           */
  zb_uint8_t  bdb_commissioning_status; /*!< see zb_bdb_error_codes_e  */

  zb_uint32_t   bdb_primary_channel_set;
  zb_uint32_t   bdb_secondary_channel_set;
  zb_uint32_t   v_scan_channels;
  zb_uint32_t   bdb_commissioning_time;
  zb_uint8_t    bdb_scan_duration;
  zb_uint8_t    bdb_commissioning_step;

  zb_bitfield_t v_do_primary_scan:3; /*!< a bit more than vDoPromaryScan in
                                      * BDB: really scan & join machine
                                      * state. @see enum bdb_join_machine_e */

  zb_bitfield_t bdb_ext_channel_scan:1; /*!< Touchlink performs ext scan if 1 */
  zb_bitfield_t ignore_aps_channel_mask:1; /*!< Non standard, but useful: if 1,
                                            * use hard-coded channels set. if 0,
                                            * mask channels sets by
                                            * aps_channel_mask. To be used to
                                            * debug at single channel, or 2
                                            * channels etc */


  zb_uint8_t    bdb_application_signal;  /* Application signal code to be passed into
                                          * zb_zdo_startup_complete */
#ifdef ZB_BDB_TOUCHLINK
  zb_uint8_t    tl_first_channel_rpt;
  zb_uint8_t    tl_channel_i;
#endif  /* ZB_BDB_TOUCHLINK */
  zb_bitfield_t bdb_force_router_rejoin:1;      /* Force rejoin for the router */
  zb_bitfield_t bdb_tc_rejoin_after_reboot:1;   /* Is TC rejoin started when reboot signal is scheduled */
  zb_bitfield_t bdb_tc_rejoin_active:1;         /* Is TC rejoin active */
  zb_bitfield_t bdb_op_cancelled:1;   /* if the BDB operation (steering or formation) is cancelled */
  } ZB_PACKED_STRUCT zb_bdb_comm_ctx_t;
/** @endcond */ /* internals_doc */

extern zb_bdb_comm_ctx_t g_bdb_ctx;

#define BDB_COMM_CTX() g_bdb_ctx

#if defined ZB_BDB_MODE && !defined BDB_OLD
void bdb_commissioning_machine(zb_uint8_t param);
void bdb_commissioning_signal(bdb_commissioning_signal_t sig, zb_uint8_t param);
#endif /* ZB_BDB_MODE && !BDB_OLD */

#if defined ZB_BDB_ENABLE_FINDING_BINDING
void zb_bdb_finding_binding_init_ctx(void);

void zb_bdb_process_identify_query_res(zb_uint8_t param);
#endif /* ZB_BDB_ENABLE_FINDING_BINDING */

/** @cond touchlink */

#if defined ZB_BDB_MODE && defined ZB_BDB_TOUCHLINK && !defined ZB_COORDINATOR_ONLY && defined ZB_DISTRIBUTED_SECURITY_ON
void bdb_touchlink_target_start(zb_uint8_t param);
void bdb_touchlink_initiator(zb_uint8_t param);
#endif /* ZB_BDB_MODE && ZB_BDB_TOUCHLINK && !ZB_COORDINATOR_ONLY && ZB_DISTRIBUTED_SECURITY_ON */

/** @endcond */ /* touchlink */

#ifdef ZB_COORDINATOR_ROLE
#ifdef ZB_ROUTER_ROLE
void bdb_remove_joiner(zb_uint8_t param);
#endif /* ZB_ROUTER_ROLE */
#endif /* ZB_COORDINATOR_ROLE */

#if defined ZB_BDB_MODE && !defined BDB_OLD
#ifdef ZB_BDB_TOUCHLINK
void bdb_check_fn(void);
#endif /* ZB_BDB_TOUCHLINK */

void bdb_start_rejoin_recovery(zb_uint8_t param, zb_uint16_t user_param);

zb_bool_t bdb_joined(void);

void bdb_force_link(void);

#endif /* ZB_BDB_MODE && !BDB_OLD */


/** @}  */ /* bdb_mode_comissioning */

/** @} */ /* bdb_mode_commissioning */

#endif /* ZB_BDB_MODE */

#endif /* ZB_BDB_COMISSIONING_H */
