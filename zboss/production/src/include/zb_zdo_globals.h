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
/* PURPOSE: ZDO globals definition
*/

#ifndef ZB_ZDO_GLOBALS_H
#define ZB_ZDO_GLOBALS_H 1

#ifdef ZB_TEST_PROFILE
#include "zb_test_profile.h"
#endif /* ZB_TEST_PROFILE */
#include "zb_zdo_rjb.h"
#include "zb_zdo.h"
#include "zdo_diagnostics.h"
#ifdef ZB_PHY_TESTING_MODE
#include "zboss_api_mac.h"
#endif /* ZB_PHY_TESTING_MODE */

/*! @cond internals_doc */
/*! \addtogroup zdo_ib */
/*! @{ */

#define ZB_ZDO_NODE_DESC() (&ZG->zdo.conf_attr.node_desc)
#define ZB_ZDO_NODE_POWER_DESC() (&ZG->zdo.conf_attr.node_power_desc)
#define ZB_ZDO_SIMPLE_DESC() (&ZG->zdo.conf_attr.zdo_simple_desc)
#define ZB_ZDO_SIMPLE_DESC_LIST() (ZG->zdo.conf_attr.simple_desc_list)
#define ZB_ZDO_SIMPLE_DESC_NUMBER() (ZG->zdo.conf_attr.simple_desc_number)

/**
  ZDO Configuration Attributes
 */
typedef struct zb_zdo_configuration_attributes_e
{
  /* nwk_time_btwn_scans and nwk_scan_attempts moved to zb_commissioning_ctx_t */

  zb_af_node_desc_t       node_desc;        /*!< Node Descriptors */
  zb_af_node_power_desc_t node_power_desc;  /*!< Node Power Descriptors */
  zb_af_simple_desc_8_9_t zdo_simple_desc; /* TODO: remove it, ZDO simple descriptor is not needed - simple descriptors for EP >= 1 are used */
  /* TODO: make real list support, if multiple EP are supported */
  /* TODO: each Zigbee device application declares a device context and a list of EPs. Each EP
     stores its simple descriptor => no need store it here, it is better to use that storage */
  zb_af_simple_desc_1_1_t *simple_desc_list[ZB_MAX_EP_NUMBER];  /*!< Simple Descriptors table */
  zb_uint8_t simple_desc_number;                                /*!< Number elements of Simple Descriptors table */

#ifndef ZB_LITE_NO_END_DEVICE_BIND
  zb_uint8_t enddev_bind_timeout; /* Timeout value in seconds employed
                                   * in End Device Binding */
#endif
  zb_uint8_t permit_join_duration; /*!< Permit join duration, 0x00 - disable
                                    * join, 0xff - join is allowed forever */


} zb_zdo_configuration_attributes_t;


/*! @} */

/*! \addtogroup ZB_ZDO */
/*! @{ */


/**
   Some ZDO private data
 */
#define ZB_TEST_DATA_SIZE 10U

#define ZB_ZDO_MAX_CLUSTER_LIST 15U

#define ZB_ZDO_MAX_JOINING_TRY_ATTEMPTS 3U

typedef struct zb_zdo_rejoin_ctx_s
{
  zb_channel_list_t channels_list;
  zb_ext_pan_id_t   ext_pan_id;
  zb_bool_t         secure_rejoin;
} zb_zdo_rejoin_ctx_t;


/**
   Some ZDO internal flags
 */
typedef struct zp_zdo_handle_s
{
  zb_bitbool_t rejoin:1;                    /*!< if !0, this is rejoin  */
  zb_bitbool_t tc_significance:1;           /*!< data for permit_joining */
  zb_bitbool_t channel_update_disabled:1;   /*!< if !0, Channel update is disabled */
  zb_uint8_t permit_joining_param;          /*!< if !0, nlme-permit_joining will
                                             * be executed */
  zb_uint8_t permit_duration;               /*!< data for permit_joining */
  zb_uint8_t dev_annce_param;               /*!< if !0, this is buffer id - device annonce
                                             * is sent */
  zb_uint8_t key_sw;                        /*!< if !0, key switch is sent and must switch
                                             * the key after this buffer sent  */
  zb_uint8_t mgmt_leave_resp_buf;           /*!< if !0, nlme_leave_request for itself will be executed
                                             * set after device was requested to leave the network
                                             * via mgmt_leave_req */
  zb_zdo_rejoin_ctx_t rejoin_ctx;
#ifdef ZB_MACSPLIT_HOST
  zb_bool_t start_no_autostart;              /*!< if ZB_TRUE, device started with start_no_autostart
                                              * Used only for macsplit host*/
#endif
} zp_zdo_handle_t;

/* Flag to set and check if channel update is disabled */
#define ZB_ZDO_NETWORK_MGMT_CHANNEL_UPDATE_IS_DISABLED() (ZG->zdo.handle.channel_update_disabled != 0U)

/* Flag to set and check if channel update is disabled */
#define ZB_ZDO_IS_REJOIN_SECURE() (ZG->zdo.handle.rejoin_ctx.secure_rejoin != 0U)

typedef struct zb_zdo_pim_s
{
  zb_time_t fast_poll_interval;
  zb_time_t fast_poll_timeout;
  zb_time_t long_poll_interval;
  zb_time_t turbo_poll_interval;
  zb_time_t turbo_poll_min;
  zb_time_t turbo_poll_max;
  zb_time_t turbo_poll_timeout;
  zb_time_t work_poll_max;
  zb_time_t work_poll_interval;
  zb_uint_t turbo_poll_n_packets;
  zb_time_t first_poll_time;
  zb_uint_t n_polls_per_data;
  zb_time_t turbo_leave_alarm_time;
#ifdef SNCP_MODE
  zb_callback_t single_poll_cb;     /* set when executed Single Poll from Host */
#endif
  zb_bitbool_t turbo_prohibit:1;
  zb_bitbool_t fast_poll_on:1;
  zb_bitbool_t poll_in_progress:1;
  zb_bitbool_t poll_stop:1;
  zb_bitbool_t was_in_turbo_poll:1; /* special value to indicate that pkt was not received,
                                      * the turbo poll should be restarted*/
} zb_zdo_pim_t;

/* Callback types */
#define CB_TYPE_DEFAULT       0x00U
#define CB_TYPE_TSN           0x01U
#define CB_TYPE_INDEX         0x02U

typedef ZB_PACKED_PRE struct zdo_cb_hash_ent_s
{
  zb_callback_t func;
  zb_uint8_t    tsn;
  zb_uint8_t    resp_counter;
  zb_bitfield_t    type:2;
  zb_bitfield_t    clock_counter:6; /*!< Used to track entry's life-time in hash */
} ZB_PACKED_STRUCT zdo_cb_hash_ent_t;

#define ZB_ZDO_CHECK_FAILS_MASK     1U      /* flag to check if channel interference check is in progress */
#define ZB_ZDO_SEND_WITH_ACK_MASK   (1U << 1U) /* flag to specify sending with acknowledgement */
#define ZB_ZDO_CHANNEL_CHANGED_MASK (1U << 2U) /* flag shows that channel change opration was performed recently (server side) */
#define ZB_ZDO_TC_REJOIN_ENABLE_MASK (1U << 3U) /* flag that allows or disallows Trust Center Rejoin (Unsequred rejoin) */
#define ZB_ZDO_UNAUTH_MASK (1U << 4U) /* indicates unauthenticated join case */

/* TODO: kill bitmasks here - use bitfields */
#define ZB_ZDO_SET_CHECK_FAILS()   (ZG->zdo.flags |= ZB_ZDO_CHECK_FAILS_MASK)
#define ZB_ZDO_CLEAR_CHECK_FAILS() (ZG->zdo.flags &= ~ZB_ZDO_CHECK_FAILS_MASK)
#define ZB_ZDO_GET_CHECK_FAILS()   (ZG->zdo.flags & ZB_ZDO_CHECK_FAILS_MASK)

#define ZB_ZDO_SET_SEND_WITH_ACK()   (ZG->zdo.flags |= ZB_ZDO_SEND_WITH_ACK_MASK)
#define ZB_ZDO_CLEAR_SEND_WITH_ACK() (ZG->zdo.flags &= ~ZB_ZDO_SEND_WITH_ACK_MASK)
#define ZB_ZDO_GET_SEND_WITH_ACK()   (ZG->zdo.flags & ZB_ZDO_SEND_WITH_ACK_MASK)

#define ZB_ZDO_SET_CHANNEL_CHANGED()   (ZG->zdo.flags |= ZB_ZDO_CHANNEL_CHANGED_MASK)
#define ZB_ZDO_CLEAR_CHANNEL_CHANGED() (ZG->zdo.flags &= ~ZB_ZDO_CHANNEL_CHANGED_MASK)
#define ZB_ZDO_GET_CHANNEL_CHANGED()   (ZG->zdo.flags & ZB_ZDO_CHANNEL_CHANGED_MASK)

#define ZB_ZDO_SET_UNAUTH()   (ZG->zdo.flags |= ZB_ZDO_UNAUTH_MASK)
#define ZB_ZDO_CLEAR_UNAUTH() (ZG->zdo.flags &= ~ZB_ZDO_UNAUTH_MASK)
#define ZB_ZDO_GET_UNAUTH()   (ZG->zdo.flags & ZB_ZDO_UNAUTH_MASK)
#ifdef ZB_ZDO_DENY_LEAVE_CONFIG
#define ZB_ZDO_SET_LEAVE_REQ_ALLOWED( v ) (ZG->zdo.leave_req_allowed = ( v ))
#endif
#define MAX_N_CLUSTER_FILTERS 2U

typedef struct zdo_cluster_filters_s
{
  zb_uint_t n_filters;
  zdo_cluster_filter_t filters[MAX_N_CLUSTER_FILTERS];
} zdo_cluster_filters_t;

struct zb_zdo_device_annce_s;
/*! @endcond */

#if defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE
typedef struct zb_joining_list_ctx_s
{
  zb_joining_list_op_q_t operation_queue;
  zb_callback_t current_op_cb;
  zb_bool_t is_consistent;
  zb_uint8_t update_id;
  zb_uint16_t list_expiry_interval; /* mibIeeeExpiryInterval (minutes) -  must be in mac PIB,
                                       but the strict implementation isn't worth the effort for now */

  /* context for zdo_nwk_joining_list_resp */
  zb_uint8_t tsn;
  zb_uint16_t dst_addr;
  zb_callback_t broadcast_confirm_cb;
  zb_uint8_t original_buffer;

  /* context for announce */
  zb_uint8_t next_start_index;
  zb_uint8_t current_list_size;

  /* context for request */
  zb_uint8_t prev_update_id;
} zb_joining_list_ctx_t;
#endif /* defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE */

#if defined ZB_ZCL_SUPPORT_CLUSTER_WWAH
typedef ZB_PACKED_PRE struct
{
#ifdef ZB_ZCL_ENABLE_WWAH_SERVER
  /* Flags and vars moved there from WWAH cluster ctx or used instead of direct access to WWZH cluster attributes. */
  zb_bitfield_t parent_classification_enabled:1;
  zb_bitfield_t key_commands_broadcast_disallowed:1; /* note: allowed by default */
  zb_bitfield_t configuration_mode_disabled:1;       /* note: enabled by default */
  zb_bitfield_t leave_without_rejoin_disallowed:1;   /* note: allowed by default */
  zb_bitfield_t interpan_disabled:1;                 /* note: enabled by default */

  /* ======= Survey Beacon related context ======= */
  /**
   *  @brief Reference to a buffer with Survey Beacons Response parameters
   *
   */
  zb_bufid_t survey_beacons_resp_ref;
#endif
} zb_zdo_hubs_context_t;
#endif  /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */

typedef zb_bool_t (*zb_zdo_match_desc_resp_handler_t)(zb_bufid_t);

typedef struct zb_zdo_func_selector_s
{
  zb_zdo_match_desc_resp_handler_t match_desc_resp_handler;
  zb_callback_t app_link_key_ind_handler;
} zb_zdo_func_selector_t;

/**
    ZDO_CTX structure
 */
typedef struct zb_zdo_globals_s
{
  zp_zdo_handle_t handle;                           /*!< */
  zb_uint8_t      tsn;                              /*!< */
  zb_zdo_tsn_policy_t tsn_policy;                   /*!< see zb_zdo_tsn_policy_e */
#ifdef ZBOSS_ZDO_APP_TSN_ENABLE
  zdo_app_tsn_entry_t app_tsn_table[ZBOSS_ZDO_APP_TSN_TABLE_SIZE];
#endif
  zb_device_handler_t   af_data_cb;                 /*!< Callback of zb_apsde_data_indication
                                                     * function */
  /*!< callback which called when ZDO command is sent (except cases when it
   * failed to be sent, or when no answers are supposed - in such case ZDO
   * callback passed to ZDO call is called) */
  zb_callback_t   af_zdo_data_conf_cb;
  zb_uint8_t      max_parent_threshold_retry;       /*!< */
  zb_uint8_t      parent_threshold_retry;           /*!< */
  zdo_cb_hash_ent_t zdo_cb[ZDO_TRAN_TABLE_SIZE];    /*!< */
#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
  zb_callback_t   system_server_discovery_cb;       /*!< Callback of System server discovery */
  zb_uint8_t      system_server_discovery_tsn;      /*!< */
#endif
#ifndef ZB_LITE_NO_OLD_CB
  zb_callback_t   app_leave_cb;                     /*!< Indication for application that leave
                                                     * performed and no further actions scheduled*/
#endif
  zb_callback_t   app_addr_resp_cb;                 /*!< Callback of zb_zdo_addr_resp_handle
                                                     * function */
  /* TODO: change flags to bitfields */
  zb_uint8_t      flags;                            /*!< */
  zb_bitfield_t   zdo_ed_scan_busy:1;
#ifdef ZB_ZDO_DENY_LEAVE_CONFIG
  zb_bitfield_t   leave_req_allowed:1; /*! allow/deny ZDO leave request */
#endif
  zb_uint8_t      long_timer; /*!< Long timer, in minutes */
  zb_callback_t   long_timer_cb; /*!< Callback for long timer */
  zb_uint8_t      long_timer_param; /*!< Parameter for long timer */
#ifdef ZB_ED_FUNC
  zb_uint8_t      parent_link_failure;              /*!< */
#endif
  zb_uint16_t     num_packets;                      /*!< Number of packets for diag purpose*/
  zb_uint16_t     num_retries;                      /*!< Number of retries for diag purpose*/

#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
  zb_uint8_t      permit_join_time; /*!< LAst permit join time in seconds */
#endif

#ifdef ZB_ROUTER_ROLE
/*NBT element to start parent_annce from */
  zb_uint8_t parent_annce_position;
#endif

#if defined ZB_ENABLE_INTER_PAN_NON_DEFAULT_CHANNEL
  zb_af_inter_pan_handler_t af_inter_pan_data_cb;   /*!< Application callback called when inter-pan packet is received */
#endif

  zb_zdo_configuration_attributes_t conf_attr; /*!< Device Configuration
                                                * Attributes */
  union zb_zdo_ctx_u
  {
    struct zb_nwk_upd_req_ctx_s
    {
      zb_uint8_t tsn;                       /*!< */
      zb_uint16_t dst_addr;                 /*!< */
      zb_callback_t cb;
      zb_bitfield_t ed_scan_busy:1;
      zb_bitfield_t enhanced:1;
    }
    nwk_upd_req;
    struct zb_get_peer_short_addr_s
    {
      zb_address_ieee_ref_t dst_addr_ref;   /*!< */
      zb_callback_t cb;                     /*!< */
      zb_uint8_t param;                     /*!< */
    }
    get_short_addr_ctx;
#ifndef ZB_LITE_NO_END_DEVICE_BIND
    struct zb_zdo_end_device_bind_ctx_s
    {
#define ZB_ZDO_BIND_DEV_1 0U
#define ZB_ZDO_BIND_DEV_2 1U
#define ZB_ZDO_PEER_DEVICE_NUM(num) (((num) + 1U) % 2U)

      struct zb_zdo_bind_device_info_s
      {
        zb_uint8_t end_device_bind_param; /*!< Reference to buffer with
                                           * end_device_bind_req command */
        zb_uint16_t binding_target;  /*!< The address of the target for the binding */
        zb_uint8_t  src_endp;        /*!< The endpoint on the binding device */
        zb_uint16_t cluster_list[ZB_ZDO_MAX_CLUSTER_LIST]; /* matched clusters list */
        zb_uint8_t cluster_num;      /*!< number of matched clusters,
                                    * found during end device bind */
      } bind_device_info[2];
      zb_uint8_t current_device; /*!< current device to send bind/unbind requests */
    }
    end_device_bind_ctx;
#endif  /* #ifndef ZB_LITE_NO_END_DEVICE_BIND */
  }
  zdo_ctx;                                  /*!< */

#ifdef ZB_TEST_PROFILE
  union zb_test_profile_ctx_u
  {
    struct zb_tp_counted_packets_s
    {
      zb_tp_transmit_counted_packets_param_t params; /*!< */
      zb_callback_t user_cb;                        /*!< */
      zb_uint16_t counter;                          /*!< */
    }
    tp_counted_packets;
    struct zb_tp_buffer_test_request_s
    {
      zb_callback_t user_cb;                        /*!< */
    }
    zb_tp_buffer_test_request;
  }
  test_prof_ctx;
#endif

#ifdef ZB_REJOIN_BACKOFF
  zb_zdo_rejoin_backoff_t zdo_rejoin_backoff;
#endif

  zb_device_annce_cb_t device_annce_cb;  /*!< This CB is called on receiving device_annce command */
  zb_callback_t leave_ind_cb;  /*!< Callback for NWK leave indication from child */
  zb_zdo_responce_cb_t zb_zdo_responce_cb;
  zb_zdo_set_channel_confirm_cb_t set_channel_confirm_cb;
  zb_apsme_update_device_ind_cb update_device_ind_cb  /*!< This callback will be called when application receives APSME_UPDATE_DEVICE */;
#ifdef ZB_MAC_DUTY_CYCLE_MONITORING
  zb_zdo_duty_cycle_mode_ind_cb_t duty_cycle_mode_ind_cb;  /*!< This callback will be called when MAC Duty Cycle mode changed */
#endif
  zb_assert_indication_cb_t assert_indication_cb; /*!< This callback will be called when application falls into ZB_ASSERT() */

  zb_zdo_pim_t pim;

  zb_uint8_t nwk_upd_notify_pkt_limit;

#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t nwk_addr_req_pending_tsns[ZB_N_BUF_IDS]; /* Address search feature: TSNs of nwk_addr_req pkt for
                                             * every buffer */
  zb_uint8_t nwk_addr_req_pending_mask[(ZB_N_BUF_IDS + 7U) / 8U];  /* Address search feature: mask of pending pkt for
                                             * every buffer */
#else
  /* Pointers are set in zb_init_configurable_mem() */
  zb_uint8_t *nwk_addr_req_pending_tsns;
  zb_uint8_t *nwk_addr_req_pending_mask;
#endif  /* ZB_CONFIGURABLE_MEM */

#ifdef APS_FRAGMENTATION
#ifndef ZB_CONFIGURABLE_MEM
  zb_uint8_t node_desc_req_pending_mask[(ZB_N_BUF_IDS + 7U) / 8U];  /* Node Desc feature: mask of pending pkt for
                                             * every buffer */
#else
  zb_uint8_t *node_desc_req_pending_mask;
#endif /* ZB_CONFIGURABLE_MEM */
#endif /* APS_FRAGMENTATION */

  zb_callback_t continue_start_after_nwk_cb;
  zb_callback_t nlme_reset_cb;

#ifdef ZB_FILTER_OUT_CLUSTERS
  zdo_cluster_filters_t cluster_filters;
#endif

#if defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE
  zb_joining_list_ctx_t joining_list_ctx;
#endif /* defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE */
#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
  /* Part of WWAH context noved into SoC part of NCP / r23 All Hubs context */
  zb_zdo_hubs_context_t hubs_ctx;
#endif

#if defined(ZDO_DIAGNOSTICS)
  zdo_diagnostics_ctx_t diagnostics_ctx;
#endif /* defined(ZDO_DIAGNOSTICS) */

  zb_zdo_func_selector_t selector;
#ifdef ZB_PHY_TESTING_MODE
  zb_phy_test_mode_notification_cb_t test_mode_notification_cb;
#endif
} zb_zdo_globals_t;


#define ZDO_CTX() ZG->zdo

#define ZDO_SELECTOR() ZDO_CTX().selector

#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
/* IN-core part of WWAH context / r23 All Hubs context */
#define HUBS_CTX() ZDO_CTX().hubs_ctx
#endif /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */

#if defined(ZDO_DIAGNOSTICS)
#define ZDO_DIAGNOSTICS_CTX() ZDO_CTX().diagnostics_ctx
#endif /* defined(ZDO_DIAGNOSTICS) */

#define ZB_ZDO_SET_ED_SCAN_BUSY()   (ZG->zdo.zdo_ed_scan_busy = 1U)
#define ZB_ZDO_CLEAR_ED_SCAN_BUSY() (ZG->zdo.zdo_ed_scan_busy = 0U)
#define ZB_ZDO_GET_ED_SCAN_BUSY() ZG->zdo.zdo_ed_scan_busy

/* Internally inside the stack use direct context access */
#define ZB_SET_JOINED_STATUS(v) ZB_TCPOL().node_is_on_a_network = (v)

/* Converts a buffer with data into a packed signal with the data */
void zb_app_signal_pack_with_data(zb_uint8_t param, zb_uint32_t signal_code, zb_int16_t status);
/* Buffer is replaced with signal information, data_size bytes are reserved for additional singal data */
void *zb_app_signal_pack(zb_uint8_t param, zb_uint32_t signal_code, zb_int16_t status, zb_uint8_t data_size);

void *zb_app_signal_pack_with_detailed_status(zb_uint8_t param, zb_uint32_t signal_code,
                                              zb_ret_t status, zb_uint8_t data_size);
/* NWK channel manager */
void zb_zdo_check_channel_conditions(zb_uint8_t param);

#if defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE
zb_bool_t zb_ieee_joining_list_schedule(zb_callback_t func, zb_uint8_t param);
void zb_ieee_joining_list_cb_completed(void);
void zb_ieee_joining_list_op_delay(void);
zb_bool_t zb_ieee_joining_list_put_cb(zb_callback_t func, zb_uint8_t param);
void zb_joining_list_reset_clear_timer(void);
#endif /* defined ZB_JOINING_LIST_SUPPORT && defined ZB_ROUTER_ROLE */

void zb_zdo_forced_parent_link_failure(zb_uint8_t param);

#ifdef SNCP_MODE
void zb_zdo_poll_parent_single(zb_uint8_t param, zb_callback_t cb);
#endif

/*! @endcond */

#endif /* ZB_ZDO_GLOBALS_H */
