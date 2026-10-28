/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SDC_LOG_MSG_H__
#define SDC_LOG_MSG_H__

#include "mpsl_log_types.h"

#define SDC_LOG_MSGS_LIB_ID   1u
#define SDC_LOG_MSGS_LIB_NAME "SDC"

static const mpsl_log_msg_entry_t sdc_log_msgs[] = {
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0001u, 1, "Fault handler is NULL" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0002u, 1, "MPSL is not initialized" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0003u, 1, "LFCLK is not accurate enough" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 3
	{ 0x0004u, 3, "SDC initialized" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0100u, 1, "CS test override: invalid RFU bits 0x%04x set in override_config" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0101u, 1, "CS test override: override_parameters_length %u > %u, too long" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0102u, 1, "CS test override: channel_length %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0103u, 1, "CS test override: channel[%u] %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0104u, 1, "CS test override: channel_map invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0105u, 1, "CS test override: channel_selection_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0106u, 1, "CS test override: ch3c_shape %u or ch3c_jump %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0107u, 1, "CS test override: main_mode_steps 0 is invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0108u, 1, "CS test override: t_pm_tone_ext %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0109u, 1, "CS test override: antenna_path_permutation_index %u > max valid %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x010au, 1, "CS test override: ss_marker1_position %u or ss_marker2_position %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x010bu, 1, "CS test override: ss_marker_value %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x010cu, 1, "CS test override: cs_sync_payload_pattern 0x%02x invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x010du, 1, "CS test override: stable phase test not allowed for reflector" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x010eu, 1, "CS test override: override_parameters_length %u does not match expected %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x010fu, 1, "CS test override: field of %u bytes exceeds the remaining %u bytes of override_parameters_data" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0110u, 1, "CS test: main_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0111u, 1, "CS test: main_mode_steps override with unused sub_mode_type" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0112u, 1, "CS test: sub_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0113u, 1, "CS test: sub_mode_type %u invalid for main_mode_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0114u, 1, "CS test: sub_mode_type %u invalid for main_mode_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0115u, 1, "CS test: main_mode_repetition %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0116u, 1, "CS test: mode_0_steps %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0117u, 1, "CS test: role %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0118u, 1, "CS test: rtt_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0119u, 1, "CS test: cs_sync_phy %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x011au, 1, "CS test: cs_sync_antenna_selection %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x011bu, 1, "CS test: subevent_len %u < 1250, too short" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x011cu, 1, "CS test: subevent_interval %u (x625us) < subevent_len %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x011du, 1, "CS test: max_num_subevents %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x011eu, 1, "CS test: transmit_power_level %d invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x011fu, 1, "CS test: t_ip1_time %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0120u, 1, "CS test: t_ip2_time %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0121u, 1, "CS test: t_fcs_time %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0122u, 1, "CS test: t_pm_time %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0123u, 1, "CS test: t_sw_time %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0124u, 1, "CS test: tone_antenna_config_selection %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0125u, 1, "CS test: snr_control_initiator %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0126u, 1, "CS test: snr_control_reflector %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0127u, 1, "CS test: channel_map_repetition 0 is invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0130u, 1, "CS test: role %u unsupported, roles_supported 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0131u, 1, "CS test: mode 3 unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0132u, 1, "CS test: RTT AA-only unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0133u, 1, "CS test: RTT sounding sequence unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0134u, 1, "CS test: RTT random sequence unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0135u, 1, "CS test: cs_sync_phy %u unsupported, cs_sync_phys_supported 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0136u, 1, "CS test: cs_sync_antenna_selection %u > num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0137u, 1, "CS test: t_ip1_time %u unsupported, t_ip1_times_supported 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0138u, 1, "CS test: t_ip2_time %u unsupported, t_ip2_times_supported 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0139u, 1, "CS test: t_fcs_time %u unsupported, t_fcs_times_supported 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x013au, 1, "CS test: t_pm_time %u unsupported, t_pm_times_supported 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x013bu, 1, "CS test: t_sw_time %u invalid for tone_antenna_config_selection 0" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x013cu, 1, "CS test: t_sw_time %u < t_sw_time_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x013du, 1, "CS test: tone_antenna_config_selection %u needs %u antenna paths, max_antenna_paths_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x013eu, 1, "CS test: tone_antenna_config_selection %u needs %u antennas, num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x013fu, 1, "CS test: snr_control_initiator %u unsupported, tx_snr_capability 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0140u, 1, "CS test: snr_control_reflector %u unsupported, tx_snr_capability 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0141u, 1, "CS test: unsupported override: ss_marker_position %u, ss_marker_value %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0142u, 1, "CS test: channel selection #3c unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0143u, 1, "CS test: %u channels > CS channel array capacity %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0150u, 1, "CS test: test already running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0151u, 1, "CS test: scheduler start failed, status 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0152u, 1, "CS test end: no CS test running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0160u, 1, "CS read local caps: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0161u, 1, "CS read local caps v2: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0170u, 1, "CS read remote caps: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0171u, 1, "CS read remote caps: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0172u, 1, "CS read remote caps: link not encrypted" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0173u, 1, "CS read remote caps: peer has no CS host support" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0180u, 1, "CS security enable: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0181u, 1, "CS security enable: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0182u, 1, "CS security enable: not central" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0183u, 1, "CS security enable: link not encrypted" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0184u, 1, "CS security enable: already in progress" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0190u, 1, "CS set default settings: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0191u, 1, "CS set default settings: role_enable 0x%02x > max valid 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0192u, 1, "CS set default settings: role %u unsupported, roles_supported 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0193u, 1, "CS set default settings: can't disable role %u used by config_id %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0194u, 1, "CS set default settings: cs_sync_antenna_selection %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0195u, 1, "CS set default settings: cs_sync_antenna_selection %u > num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0196u, 1, "CS set default settings: max_tx_power %d not in [%d, %d]" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a0u, 1, "CS read remote FAE: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a1u, 1, "CS read remote FAE: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a2u, 1, "CS read remote FAE: link not encrypted" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a3u, 1, "CS read remote FAE: feature exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a4u, 1, "CS read remote FAE: peer has no CS host support" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a5u, 1, "CS read remote FAE: CS caps exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01a6u, 1, "CS read remote FAE: LL_CS_FAE_RSP too long for conn interval" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b0u, 1, "CS write cached FAE: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b1u, 1, "CS write cached FAE: feature exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b2u, 1, "CS write cached FAE: peer has no CS host support" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b3u, 1, "CS write cached FAE: CS caps exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b4u, 1, "CS write cached FAE: peer supports no-FAE" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b5u, 1, "CS write cached FAE: FAE table already exchanged" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b6u, 1, "CS write cached FAE: CS config already created" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01b7u, 1, "CS write cached FAE: no memory for CS test context" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01c0u, 1, "CS write cached caps: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01c1u, 1, "CS write cached caps: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01c2u, 1, "CS write cached caps: invalid caps, num_config_supported %u, num_antennae_supported %u, max_antenna_paths_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01c3u, 1, "CS write cached caps: invalid caps, roles_supported 0x%02x, t_sw_time_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01c4u, 1, "CS write cached caps: CS config already created" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01c5u, 1, "CS write cached caps: CS caps already exchanged" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d0u, 1, "CS create config: invalid config params" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d1u, 1, "CS create config: create_context %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d2u, 1, "CS create config: initiator role not enabled in default settings" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d3u, 1, "CS create config: reflector role not enabled in default settings" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d4u, 1, "CS create config: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d5u, 1, "CS create config: LL_CS_CONFIG_REQ too long for conn interval" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d6u, 1, "CS create config: config procedure already in progress" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d7u, 1, "CS create config: link not encrypted" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d8u, 1, "CS create config: peer has no CS host support" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01d9u, 1, "CS create config: CS caps exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01dau, 1, "CS create config: unsupported locally, main_mode_type %u, sub_mode_type %u, rtt_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01dbu, 1, "CS create config: unsupported locally, cs_sync_phy %u, channel_selection_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01dcu, 1, "CS create config: unsupported by peer, main_mode_type %u, sub_mode_type %u, rtt_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01ddu, 1, "CS create config: unsupported by peer, cs_sync_phy %u, channel_selection_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01deu, 1, "CS create config: peer doesn't support reflector role" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01dfu, 1, "CS create config: peer doesn't support initiator role" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01e0u, 1, "CS create config: config_id %u, only config_id %u can be modified" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01e1u, 1, "CS create config: IPT unsupported locally" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01e2u, 1, "CS create config: IPT unsupported by peer" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01e3u, 1, "CS create config: cannot create config while CS procedure running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01f0u, 1, "CS remove config: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01f1u, 1, "CS remove config: cannot remove config while CS procedure running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01f2u, 1, "CS remove config: config_id %u does not match expected %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01f3u, 1, "CS remove config: config_id %u not created" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x01f4u, 1, "CS remove config: config procedure already in progress" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0200u, 1, "CS set chmap class: invalid channel_classification" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0201u, 1, "CS set chmap class: issued %u us after previous, min %u us" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0202u, 1, "CS set chmap class: chmap update already pending on connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0203u, 1, "CS set chmap class: chmap update already pending on connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0210u, 1, "CS set proc params: config_id %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0211u, 1, "CS set proc params: max_procedure_len 0 is invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0212u, 1, "CS set proc params: min_procedure_interval 0 is invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0213u, 1, "CS set proc params: max_procedure_interval 0 is invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0214u, 1, "CS set proc params: max_procedure_interval %u < min_procedure_interval %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0215u, 1, "CS set proc params: min_subevent_len %u > max_subevent_len %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0216u, 1, "CS set proc params: min_subevent_len %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0217u, 1, "CS set proc params: max_subevent_len %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0218u, 1, "CS set proc params: tone_antenna_config_selection %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0219u, 1, "CS set proc params: phy %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x021au, 1, "CS set proc params: preferred_peer_antenna 0x%02x invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x021bu, 1, "CS set proc params: preferred_peer_antenna 0x%02x has < %u antennas set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x021cu, 1, "CS set proc params: snr_control_initiator %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x021du, 1, "CS set proc params: snr_control_reflector %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x021eu, 1, "CS set proc params: %u preferred peer antennas > peer num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x021fu, 1, "CS set proc params: tone_antenna_config_selection %u unsupported by local or peer antennas" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0220u, 1, "CS set proc params: connection_handle 0x%04x > max valid 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0221u, 1, "CS set proc params: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0222u, 1, "CS set proc params: config_id %u not created" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0223u, 1, "CS set proc params: cannot set new params while CS procedure running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0230u, 1, "CS proc enable: config_id %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0231u, 1, "CS proc enable: enable %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0232u, 1, "CS proc enable: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0233u, 1, "CS proc enable: config_id %u not created" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0234u, 1, "CS proc enable: peer has no CS host support" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0235u, 1, "CS proc enable: FAE table exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0236u, 1, "CS proc enable: CS caps exchange not done" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0237u, 1, "CS proc enable: CS security not started" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0238u, 1, "CS proc enable: LL_CS_REQ too long for conn interval" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0239u, 1, "CS proc enable: no memory for CS context" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x023au, 1, "CS proc enable: CS procedure already running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x023bu, 1, "CS proc enable: procedure params not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x023cu, 1, "CS proc enable: config doesn't meet security requirements 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x023du, 1, "CS proc enable: too few channels after channel classification" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x023eu, 1, "CS proc enable: cannot enable while subrate procedure in progress" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x023fu, 1, "CS proc disable: CS procedure not started" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0240u, 1, "CS proc disable: CS start procedure not completed" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0241u, 1, "CS proc disable: CS procedure not running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0242u, 1, "CS proc disable: termination rejected, status 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0250u, 1, "CS set security reqs: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0251u, 1, "CS set security reqs: unknown connection_handle 0x%04x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0252u, 1, "CS set security reqs: cannot set security reqs while CS procedure running" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0253u, 1, "CS set security reqs: cs_security_requirements 0x%08x%08x > max valid 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0254u, 1, "CS set security reqs: 10 ns RTT accuracy unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0255u, 1, "CS set security reqs: NADM unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0260u, 1, "CS set default security reqs: CS host support bit not set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0261u, 1, "CS set default security reqs: cs_security_requirements 0x%08x%08x > max valid 0x%02x" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0262u, 1, "CS set default security reqs: 10 ns RTT accuracy unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0263u, 1, "CS set default security reqs: NADM unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0270u, 1, "CS VS params set: LDO vreg mode unsupported on this SoC" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0271u, 1, "CS VS params set: unknown cs_param_type %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0272u, 1, "CS VS params set: cs_t_pm_length_us %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0280u, 1, "CS config: config_id %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0281u, 1, "CS config: channel_map_repetition %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0282u, 1, "CS config: main_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0283u, 1, "CS config: sub_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0284u, 1, "CS config: main_mode_type 1 with sub_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0285u, 1, "CS config: main_mode_type 2 with sub_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0286u, 1, "CS config: main_mode_type 3 with sub_mode_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0287u, 1, "CS config: min_main_mode_steps %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0288u, 1, "CS config: max_main_mode_steps %u < min_main_mode_steps %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0289u, 1, "CS config: main_mode_repetition %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x028au, 1, "CS config: mode_0_steps %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x028bu, 1, "CS config: cs_sync_phy invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x028cu, 1, "CS config: rtt_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x028du, 1, "CS config: role %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x028eu, 1, "CS config: channel_selection_type %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x028fu, 1, "CS config: ch3c_shape %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x0290u, 1, "CS config: ch3c_jump %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a0u, 1, "CS config: mode 3 unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a1u, 1, "CS config: RTT AA only unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a2u, 1, "CS config: RTT sounding sequence unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a3u, 1, "CS config: RTT random sequence unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a4u, 1, "CS config: cs_sync_phy 2M unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a5u, 1, "CS config: cs_sync_phy unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02a6u, 1, "CS config: channel selection algorithm #3c unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02b0u, 1, "CS caps: num_config_supported %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02b1u, 1, "CS caps: num_antennae_supported %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02b2u, 1, "CS caps: max_antenna_paths_supported %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02b3u, 1, "CS caps: max_antenna_paths_supported %u < num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02b4u, 1, "CS caps: no roles_supported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02b5u, 1, "CS caps: t_sw_time_supported %u invalid" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02c0u, 1, "CS ACI: tone_antenna_config_selection %u needs %u antenna paths, max_antenna_paths_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02c1u, 1, "CS ACI: tone_antenna_config_selection %u needs %u antennas, num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02c2u, 1, "CS ACI: tone_antenna_config_selection %u needs %u peer antennas, peer num_antennae_supported %u" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02d0u, 1, "CS security: tone required but main_mode_type is 1" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02d1u, 1, "CS security: 150 ns RTT accuracy required but config has no RTT steps" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02d2u, 1, "CS security: random or sounding sequence required but config has no RTT steps" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02d3u, 1, "CS security: random or sounding sequence required but rtt_type is AA only" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02d4u, 1, "CS security: NADM required but unsupported" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02e0u, 1, "CS repeat terminate: already requested" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02e1u, 1, "CS repeat terminate: already terminating" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02e2u, 1, "CS repeat terminate: no procedure repeats left" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02f0u, 1, "CS channel map: unused channel enabled" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02f1u, 1, "CS channel map: invalid RFU bit set" },
#endif
#if SDC_LOG_PRINT_LEVEL >= 1
	{ 0x02f2u, 1, "CS channel map: only %u channels enabled" },
#endif
	{ 0xffffu, 0, (const char *)0 },
};

#endif /* SDC_LOG_MSG_H__ */
