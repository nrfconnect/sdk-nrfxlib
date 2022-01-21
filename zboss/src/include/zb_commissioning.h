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
/* PURPOSE: commissioning layer definitions
*/

#ifndef ZB_COMMISSIONING_H
#define ZB_COMMISSIONING_H 1

#include "zb_common.h"

typedef zb_uint8_t zb_commissioning_signal_t;

typedef void (*zb_commissioning_signal_func_t)(zb_commissioning_signal_t, zb_bufid_t);
typedef zb_uint8_t (*zb_commissioning_get_permit_join_duration_func_t)(void);
typedef zb_bool_t (*zb_commissioning_must_use_installcode_func_t)(zb_bool_t is_client);
typedef void (*zb_commissioning_formation_channels_mask_t)(zb_channel_list_t list);
typedef zb_uint8_t (*zb_commissioning_get_scan_duration_func_t)(void);
typedef void (*zb_commissioning_get_scan_channel_mask_func_t)(zb_channel_list_t channel_mask);
typedef zb_bool_t (*zb_commissioning_is_in_tc_rejoin_func_t)(void);

#define ZB_COMM_SIGNAL_START 1u
#define ZB_COMM_SIGNAL_NWK_DISC_FAILED 2u
#define ZB_COMM_SIGNAL_JOIN_FAILED 3u
#define ZB_COMM_SIGNAL_AUTH_FAILED 4u
#define ZB_COMM_SIGNAL_INITIATE_REJOIN 5u
#define ZB_COMM_SIGNAL_DEV_ANNCE_SENT 6u
#define ZB_COMM_SIGNAL_ROUTER_STARTED 7u
#define ZB_COMM_SIGNAL_TCLK_UPDATE_COMPLETE 8u
#define ZB_COMM_SIGNAL_TCLK_UPDATE_FAILED 9u
#define ZB_COMM_SIGNAL_LEAVE_DONE 10u
#define ZB_COMM_SIGNAL_AUTH_OK 11u
#define ZB_COMM_SIGNAL_SECUR_FAILED 12u /* do we need it? Isn't it the same as TCLK_UPDATE_FAILED? */
#define ZB_COMM_SIGNAL_REJOIN_AFTER_SECUR_FAILED 13u /* key switch failure */
#define ZB_COMM_SIGNAL_LEAVE_WITH_REJOIN 14u
#define ZB_COMM_SIGNAL_SECURED_REJOIN 15u
#define ZB_COMM_SIGNAL_FORMATION_DONE 16u
#define ZB_COMM_SIGNAL_FORMATION_FAILED 17u
#define ZB_COMM_SIGNAL_AUTHENTICATE_REMOTE 18u
#define ZB_COMM_SIGNAL_TCLK_VERIFIED_REMOTE 19u
#define ZB_COMM_SIGNAL_DEVICE_LEFT 20u


typedef struct zb_formation_func_selector_s
{
  zb_callback_t start_formation;
  zb_commissioning_formation_channels_mask_t get_formation_channels_mask;
} zb_formation_func_selector_t;


typedef struct zb_commissioning_func_selector_s
{
  zb_commissioning_signal_func_t signal;

#ifdef ZB_ROUTER_ROLE
  zb_commissioning_get_permit_join_duration_func_t get_permit_join_duration;
#endif /* ZB_ROUTER_ROLE */

#ifndef NCP_MODE_HOST
  zb_commissioning_must_use_installcode_func_t must_use_install_code;
#endif /* !NCP_MODE_HOST */

#ifdef ZB_JOIN_CLIENT
  zb_commissioning_get_scan_duration_func_t get_scan_duration;
  zb_commissioning_get_scan_channel_mask_func_t get_scan_channel_mask;
#endif /* ZB_JOIN_CLIENT */

  zb_commissioning_is_in_tc_rejoin_func_t is_in_tc_rejoin;
} zb_commissioning_func_selector_t;

typedef struct zb_commissioning_ctx_s
{
  zb_commissioning_type_t commissioning_type;
  struct zb_discovery_ctx_s
  {
    zb_uint8_t scanlist_ref;                  /* Buffer with scanlist */
    zb_ushort_t scanlist_idx;                 /* nwk descriptor index in scanlist */
    zb_uint8_t scanlist_join_attempt_n;       /* Indicates the current joining
                                               * attempt number */
    zb_uint8_t disc_count;                /*!< */
    zb_uint16_t nwk_time_btwn_scans; /* Integer value representing the
                                      * time duration (in milliseconds)
                                      * between each NWK discovery
                                      * attempt. This attribute has a
                                      * default value of 100 (milliseconds) */
    zb_uint8_t nwk_scan_attempts; /* Integer value representing the
                                   * number of scan attempts to make
                                   * before the NWK layer decides which
                                   * Zigbee coordinator or router to
                                   * associate with (see sub-clause
                                   * 2.5.5.5). This attribute has
                                   * default value of 5 */

  } discovery_ctx;

#ifdef ZB_FORMATION
  zb_formation_func_selector_t formation_selector;
#endif /* ZB_FORMATION */

  zb_commissioning_func_selector_t commissioning_selector;
} zb_commissioning_ctx_t;

#define COMM_CTX() ZG->commissioning_ctx
#define FORMATION_SELECTOR() COMM_CTX().formation_selector
#define COMM_SELECTOR() COMM_CTX().commissioning_selector

void zdo_commissioning_init(void);
void zdo_commissioning_start(zb_uint8_t param);
#if !defined NCP_MODE_HOST && defined ZB_SECURITY_INSTALLCODES && !defined ZB_SECURITY_INSTALLCODES_ONLY
zb_bool_t zdo_secur_must_use_installcode(zb_bool_t is_client);
#endif /* !NCP_MODE_HOST && ZB_SECURITY_INSTALLCODES && !ZB_SECURITY_INSTALLCODES_ONLY */
void zdo_comm_set_permit_join_after_router_start(zb_uint8_t param);
void zdo_handle_nlme_network_discovery_confirm(zb_uint8_t param);
void zdo_join_to_nwk_descr(zb_uint8_t param);
void zdo_authenticated_send_device_annce(zb_uint8_t param);
void zdo_reset_scanlist(zb_bool_t do_free);
void zdo_call_nlme_reset(zb_uint8_t param, zb_bool_t warm_start, zb_callback_t cb);
void zdo_next_nwk_discovery_req(zb_uint8_t param);
zb_bool_t zb_distributed_security_enabled(void);
#if !defined NCP_MODE_HOST && defined ZB_COORDINATOR_ROLE
void zdo_commissioning_secure_rejoin_setup_lk_alarm(zb_uint8_t param);
void zdo_commissioning_tclk_verified_remote(zb_address_ieee_ref_t param);
void zdo_commissioning_device_left(zb_address_ieee_ref_t param);
#endif /* !NCP_MODE_HOST && ZB_COORDINATOR_ROLE */
#ifdef ZB_FORMATION
void zdo_commissioning_send_nwk_key_to_joined_dev(zb_uint8_t param, zb_uint16_t user_param);
void zdo_commissioning_authenticate_remote(zb_bufid_t param);
#endif /* ZB_FORMATION */
void zdo_commissioning_leave_with_rejoin(zb_uint8_t param);

#ifdef ZB_JOIN_CLIENT
void zdo_commissioning_join_via_scanlist(zb_uint8_t param);
void zdo_commissioning_nwk_discovery_failed(zb_uint8_t param);
void zdo_commissioning_join_failed(zb_uint8_t param);
void zdo_commissioning_authentication_failed(zb_uint8_t param);
void zdo_commissioning_handle_dev_annce_sent_event(zb_uint8_t param);
void zdo_commissioning_start_router_confirm(zb_uint8_t param);
void zdo_commissioning_initiate_rejoin(zb_uint8_t param);
zb_bool_t zdo_secur_waiting_for_tclk_update(void);
void zdo_retry_joining(zb_uint8_t param);
void zdo_commissioning_tclk_upd_complete(zb_uint8_t param);
void zdo_commissioning_tclk_upd_failed(zb_uint8_t param);
void zdo_commissioning_authenticated(zb_uint8_t param);
void zdo_commissioning_dev_annce_sent(zb_uint8_t param);
void zdo_commissioning_secur_failed(zb_uint8_t param);
void zdo_commissioning_leave_done(zb_uint8_t param);
void zdo_inform_app_leave(zb_uint8_t leave_type);
#endif



#endif /* ZB_COMMISSIONING_H */
