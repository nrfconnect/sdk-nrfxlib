/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file sdc_hci_cmd_le.h
 *
 * @defgroup sdc_hci_le SoftDevice Controller HCI Interface for LE Controller commands
 * @ingroup sdc_hci
 *
 * All APIs in this header file are expected to be called from the
 * same execution priority as mpsl_low_priority_process.
 * Not doing so will lead to undefined behavior.
 *
 * @{
 */

#ifndef SDC_HCI_CMD_LE_H__
#define SDC_HCI_CMD_LE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mdk/compiler_abstraction.h>

/**
 * @defgroup HCI_TYPES Types
 * @{
 */

/** @brief HCI LE OpCode Field values. */
enum sdc_hci_opcode_le
{
    /** @brief See @ref sdc_hci_cmd_le_set_event_mask(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EVENT_MASK = 0x2001,
    /** @brief See @ref sdc_hci_cmd_le_read_buffer_size(). */
    SDC_HCI_OPCODE_CMD_LE_READ_BUFFER_SIZE = 0x2002,
    /** @brief See @ref sdc_hci_cmd_le_read_local_supported_features(). */
    SDC_HCI_OPCODE_CMD_LE_READ_LOCAL_SUPPORTED_FEATURES = 0x2003,
    /** @brief See @ref sdc_hci_cmd_le_set_random_address(). */
    SDC_HCI_OPCODE_CMD_LE_SET_RANDOM_ADDRESS = 0x2005,
    /** @brief See @ref sdc_hci_cmd_le_set_adv_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_ADV_PARAMS = 0x2006,
    /** @brief See @ref sdc_hci_cmd_le_read_adv_physical_channel_tx_power(). */
    SDC_HCI_OPCODE_CMD_LE_READ_ADV_PHYSICAL_CHANNEL_TX_POWER = 0x2007,
    /** @brief See @ref sdc_hci_cmd_le_set_adv_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_ADV_DATA = 0x2008,
    /** @brief See @ref sdc_hci_cmd_le_set_scan_response_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_SCAN_RESPONSE_DATA = 0x2009,
    /** @brief See @ref sdc_hci_cmd_le_set_adv_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_ADV_ENABLE = 0x200a,
    /** @brief See @ref sdc_hci_cmd_le_set_scan_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_SCAN_PARAMS = 0x200b,
    /** @brief See @ref sdc_hci_cmd_le_set_scan_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_SCAN_ENABLE = 0x200c,
    /** @brief See @ref sdc_hci_cmd_le_create_conn(). */
    SDC_HCI_OPCODE_CMD_LE_CREATE_CONN = 0x200d,
    /** @brief See @ref sdc_hci_cmd_le_create_conn_cancel(). */
    SDC_HCI_OPCODE_CMD_LE_CREATE_CONN_CANCEL = 0x200e,
    /** @brief See @ref sdc_hci_cmd_le_read_filter_accept_list_size(). */
    SDC_HCI_OPCODE_CMD_LE_READ_FILTER_ACCEPT_LIST_SIZE = 0x200f,
    /** @brief See @ref sdc_hci_cmd_le_clear_filter_accept_list(). */
    SDC_HCI_OPCODE_CMD_LE_CLEAR_FILTER_ACCEPT_LIST = 0x2010,
    /** @brief See @ref sdc_hci_cmd_le_add_device_to_filter_accept_list(). */
    SDC_HCI_OPCODE_CMD_LE_ADD_DEVICE_TO_FILTER_ACCEPT_LIST = 0x2011,
    /** @brief See @ref sdc_hci_cmd_le_remove_device_from_filter_accept_list(). */
    SDC_HCI_OPCODE_CMD_LE_REMOVE_DEVICE_FROM_FILTER_ACCEPT_LIST = 0x2012,
    /** @brief See @ref sdc_hci_cmd_le_conn_update(). */
    SDC_HCI_OPCODE_CMD_LE_CONN_UPDATE = 0x2013,
    /** @brief See @ref sdc_hci_cmd_le_set_host_channel_classification(). */
    SDC_HCI_OPCODE_CMD_LE_SET_HOST_CHANNEL_CLASSIFICATION = 0x2014,
    /** @brief See @ref sdc_hci_cmd_le_read_channel_map(). */
    SDC_HCI_OPCODE_CMD_LE_READ_CHANNEL_MAP = 0x2015,
    /** @brief See @ref sdc_hci_cmd_le_read_remote_features(). */
    SDC_HCI_OPCODE_CMD_LE_READ_REMOTE_FEATURES = 0x2016,
    /** @brief See @ref sdc_hci_cmd_le_encrypt(). */
    SDC_HCI_OPCODE_CMD_LE_ENCRYPT = 0x2017,
    /** @brief See @ref sdc_hci_cmd_le_rand(). */
    SDC_HCI_OPCODE_CMD_LE_RAND = 0x2018,
    /** @brief See @ref sdc_hci_cmd_le_enable_encryption(). */
    SDC_HCI_OPCODE_CMD_LE_ENABLE_ENCRYPTION = 0x2019,
    /** @brief See @ref sdc_hci_cmd_le_long_term_key_request_reply(). */
    SDC_HCI_OPCODE_CMD_LE_LONG_TERM_KEY_REQUEST_REPLY = 0x201a,
    /** @brief See @ref sdc_hci_cmd_le_long_term_key_request_negative_reply(). */
    SDC_HCI_OPCODE_CMD_LE_LONG_TERM_KEY_REQUEST_NEGATIVE_REPLY = 0x201b,
    /** @brief See @ref sdc_hci_cmd_le_read_supported_states(). */
    SDC_HCI_OPCODE_CMD_LE_READ_SUPPORTED_STATES = 0x201c,
    /** @brief See @ref sdc_hci_cmd_le_test_end(). */
    SDC_HCI_OPCODE_CMD_LE_TEST_END = 0x201f,
    /** @brief See @ref sdc_hci_cmd_le_set_data_length(). */
    SDC_HCI_OPCODE_CMD_LE_SET_DATA_LENGTH = 0x2022,
    /** @brief See @ref sdc_hci_cmd_le_read_suggested_default_data_length(). */
    SDC_HCI_OPCODE_CMD_LE_READ_SUGGESTED_DEFAULT_DATA_LENGTH = 0x2023,
    /** @brief See @ref sdc_hci_cmd_le_write_suggested_default_data_length(). */
    SDC_HCI_OPCODE_CMD_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH = 0x2024,
    /** @brief See @ref sdc_hci_cmd_le_add_device_to_resolving_list(). */
    SDC_HCI_OPCODE_CMD_LE_ADD_DEVICE_TO_RESOLVING_LIST = 0x2027,
    /** @brief See @ref sdc_hci_cmd_le_remove_device_from_resolving_list(). */
    SDC_HCI_OPCODE_CMD_LE_REMOVE_DEVICE_FROM_RESOLVING_LIST = 0x2028,
    /** @brief See @ref sdc_hci_cmd_le_clear_resolving_list(). */
    SDC_HCI_OPCODE_CMD_LE_CLEAR_RESOLVING_LIST = 0x2029,
    /** @brief See @ref sdc_hci_cmd_le_read_resolving_list_size(). */
    SDC_HCI_OPCODE_CMD_LE_READ_RESOLVING_LIST_SIZE = 0x202a,
    /** @brief See @ref sdc_hci_cmd_le_set_address_resolution_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_ADDRESS_RESOLUTION_ENABLE = 0x202d,
    /** @brief See @ref sdc_hci_cmd_le_set_resolvable_private_address_timeout(). */
    SDC_HCI_OPCODE_CMD_LE_SET_RESOLVABLE_PRIVATE_ADDRESS_TIMEOUT = 0x202e,
    /** @brief See @ref sdc_hci_cmd_le_read_max_data_length(). */
    SDC_HCI_OPCODE_CMD_LE_READ_MAX_DATA_LENGTH = 0x202f,
    /** @brief See @ref sdc_hci_cmd_le_read_phy(). */
    SDC_HCI_OPCODE_CMD_LE_READ_PHY = 0x2030,
    /** @brief See @ref sdc_hci_cmd_le_set_default_phy(). */
    SDC_HCI_OPCODE_CMD_LE_SET_DEFAULT_PHY = 0x2031,
    /** @brief See @ref sdc_hci_cmd_le_set_phy(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PHY = 0x2032,
    /** @brief See @ref sdc_hci_cmd_le_set_adv_set_random_address(). */
    SDC_HCI_OPCODE_CMD_LE_SET_ADV_SET_RANDOM_ADDRESS = 0x2035,
    /** @brief See @ref sdc_hci_cmd_le_set_ext_adv_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EXT_ADV_PARAMS = 0x2036,
    /** @brief See @ref sdc_hci_cmd_le_set_ext_adv_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EXT_ADV_DATA = 0x2037,
    /** @brief See @ref sdc_hci_cmd_le_set_ext_scan_response_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EXT_SCAN_RESPONSE_DATA = 0x2038,
    /** @brief See @ref sdc_hci_cmd_le_set_ext_adv_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EXT_ADV_ENABLE = 0x2039,
    /** @brief See @ref sdc_hci_cmd_le_read_max_adv_data_length(). */
    SDC_HCI_OPCODE_CMD_LE_READ_MAX_ADV_DATA_LENGTH = 0x203a,
    /** @brief See @ref sdc_hci_cmd_le_read_number_of_supported_adv_sets(). */
    SDC_HCI_OPCODE_CMD_LE_READ_NUMBER_OF_SUPPORTED_ADV_SETS = 0x203b,
    /** @brief See @ref sdc_hci_cmd_le_remove_adv_set(). */
    SDC_HCI_OPCODE_CMD_LE_REMOVE_ADV_SET = 0x203c,
    /** @brief See @ref sdc_hci_cmd_le_clear_adv_sets(). */
    SDC_HCI_OPCODE_CMD_LE_CLEAR_ADV_SETS = 0x203d,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_PARAMS = 0x203e,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_DATA = 0x203f,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_ENABLE = 0x2040,
    /** @brief See @ref sdc_hci_cmd_le_set_ext_scan_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EXT_SCAN_PARAMS = 0x2041,
    /** @brief See @ref sdc_hci_cmd_le_set_ext_scan_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_EXT_SCAN_ENABLE = 0x2042,
    /** @brief See @ref sdc_hci_cmd_le_ext_create_conn(). */
    SDC_HCI_OPCODE_CMD_LE_EXT_CREATE_CONN = 0x2043,
    /** @brief See @ref sdc_hci_cmd_le_periodic_adv_create_sync(). */
    SDC_HCI_OPCODE_CMD_LE_PERIODIC_ADV_CREATE_SYNC = 0x2044,
    /** @brief See @ref sdc_hci_cmd_le_periodic_adv_create_sync_cancel(). */
    SDC_HCI_OPCODE_CMD_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL = 0x2045,
    /** @brief See @ref sdc_hci_cmd_le_periodic_adv_terminate_sync(). */
    SDC_HCI_OPCODE_CMD_LE_PERIODIC_ADV_TERMINATE_SYNC = 0x2046,
    /** @brief See @ref sdc_hci_cmd_le_add_device_to_periodic_adv_list(). */
    SDC_HCI_OPCODE_CMD_LE_ADD_DEVICE_TO_PERIODIC_ADV_LIST = 0x2047,
    /** @brief See @ref sdc_hci_cmd_le_remove_device_from_periodic_adv_list(). */
    SDC_HCI_OPCODE_CMD_LE_REMOVE_DEVICE_FROM_PERIODIC_ADV_LIST = 0x2048,
    /** @brief See @ref sdc_hci_cmd_le_clear_periodic_adv_list(). */
    SDC_HCI_OPCODE_CMD_LE_CLEAR_PERIODIC_ADV_LIST = 0x2049,
    /** @brief See @ref sdc_hci_cmd_le_read_periodic_adv_list_size(). */
    SDC_HCI_OPCODE_CMD_LE_READ_PERIODIC_ADV_LIST_SIZE = 0x204a,
    /** @brief See @ref sdc_hci_cmd_le_read_transmit_power(). */
    SDC_HCI_OPCODE_CMD_LE_READ_TRANSMIT_POWER = 0x204b,
    /** @brief See @ref sdc_hci_cmd_le_read_rf_path_compensation(). */
    SDC_HCI_OPCODE_CMD_LE_READ_RF_PATH_COMPENSATION = 0x204c,
    /** @brief See @ref sdc_hci_cmd_le_write_rf_path_compensation(). */
    SDC_HCI_OPCODE_CMD_LE_WRITE_RF_PATH_COMPENSATION = 0x204d,
    /** @brief See @ref sdc_hci_cmd_le_set_privacy_mode(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PRIVACY_MODE = 0x204e,
    /** @brief See @ref sdc_hci_cmd_le_set_connless_cte_transmit_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_CONNLESS_CTE_TRANSMIT_PARAMS = 0x2051,
    /** @brief See @ref sdc_hci_cmd_le_set_connless_cte_transmit_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_CONNLESS_CTE_TRANSMIT_ENABLE = 0x2052,
    /** @brief See @ref sdc_hci_cmd_le_set_conn_cte_transmit_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_CONN_CTE_TRANSMIT_PARAMS = 0x2055,
    /** @brief See @ref sdc_hci_cmd_le_conn_cte_response_enable(). */
    SDC_HCI_OPCODE_CMD_LE_CONN_CTE_RESPONSE_ENABLE = 0x2057,
    /** @brief See @ref sdc_hci_cmd_le_read_antenna_information(). */
    SDC_HCI_OPCODE_CMD_LE_READ_ANTENNA_INFORMATION = 0x2058,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_receive_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_RECEIVE_ENABLE = 0x2059,
    /** @brief See @ref sdc_hci_cmd_le_periodic_adv_sync_transfer(). */
    SDC_HCI_OPCODE_CMD_LE_PERIODIC_ADV_SYNC_TRANSFER = 0x205a,
    /** @brief See @ref sdc_hci_cmd_le_periodic_adv_set_info_transfer(). */
    SDC_HCI_OPCODE_CMD_LE_PERIODIC_ADV_SET_INFO_TRANSFER = 0x205b,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_SYNC_TRANSFER_PARAMS = 0x205c,
    /** @brief See @ref sdc_hci_cmd_le_set_default_periodic_adv_sync_transfer_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_DEFAULT_PERIODIC_ADV_SYNC_TRANSFER_PARAMS = 0x205d,
    /** @brief See @ref sdc_hci_cmd_le_read_buffer_size_v2(). */
    SDC_HCI_OPCODE_CMD_LE_READ_BUFFER_SIZE_V2 = 0x2060,
    /** @brief See @ref sdc_hci_cmd_le_read_iso_tx_sync(). */
    SDC_HCI_OPCODE_CMD_LE_READ_ISO_TX_SYNC = 0x2061,
    /** @brief See @ref sdc_hci_cmd_le_set_cig_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_CIG_PARAMS = 0x2062,
    /** @brief See @ref sdc_hci_cmd_le_set_cig_params_test(). */
    SDC_HCI_OPCODE_CMD_LE_SET_CIG_PARAMS_TEST = 0x2063,
    /** @brief See @ref sdc_hci_cmd_le_create_cis(). */
    SDC_HCI_OPCODE_CMD_LE_CREATE_CIS = 0x2064,
    /** @brief See @ref sdc_hci_cmd_le_remove_cig(). */
    SDC_HCI_OPCODE_CMD_LE_REMOVE_CIG = 0x2065,
    /** @brief See @ref sdc_hci_cmd_le_accept_cis_request(). */
    SDC_HCI_OPCODE_CMD_LE_ACCEPT_CIS_REQUEST = 0x2066,
    /** @brief See @ref sdc_hci_cmd_le_reject_cis_request(). */
    SDC_HCI_OPCODE_CMD_LE_REJECT_CIS_REQUEST = 0x2067,
    /** @brief See @ref sdc_hci_cmd_le_create_big(). */
    SDC_HCI_OPCODE_CMD_LE_CREATE_BIG = 0x2068,
    /** @brief See @ref sdc_hci_cmd_le_create_big_test(). */
    SDC_HCI_OPCODE_CMD_LE_CREATE_BIG_TEST = 0x2069,
    /** @brief See @ref sdc_hci_cmd_le_terminate_big(). */
    SDC_HCI_OPCODE_CMD_LE_TERMINATE_BIG = 0x206a,
    /** @brief See @ref sdc_hci_cmd_le_big_create_sync(). */
    SDC_HCI_OPCODE_CMD_LE_BIG_CREATE_SYNC = 0x206b,
    /** @brief See @ref sdc_hci_cmd_le_big_terminate_sync(). */
    SDC_HCI_OPCODE_CMD_LE_BIG_TERMINATE_SYNC = 0x206c,
    /** @brief See @ref sdc_hci_cmd_le_request_peer_sca(). */
    SDC_HCI_OPCODE_CMD_LE_REQUEST_PEER_SCA = 0x206d,
    /** @brief See @ref sdc_hci_cmd_le_setup_iso_data_path(). */
    SDC_HCI_OPCODE_CMD_LE_SETUP_ISO_DATA_PATH = 0x206e,
    /** @brief See @ref sdc_hci_cmd_le_remove_iso_data_path(). */
    SDC_HCI_OPCODE_CMD_LE_REMOVE_ISO_DATA_PATH = 0x206f,
    /** @brief See @ref sdc_hci_cmd_le_iso_transmit_test(). */
    SDC_HCI_OPCODE_CMD_LE_ISO_TRANSMIT_TEST = 0x2070,
    /** @brief See @ref sdc_hci_cmd_le_iso_receive_test(). */
    SDC_HCI_OPCODE_CMD_LE_ISO_RECEIVE_TEST = 0x2071,
    /** @brief See @ref sdc_hci_cmd_le_iso_read_test_counters(). */
    SDC_HCI_OPCODE_CMD_LE_ISO_READ_TEST_COUNTERS = 0x2072,
    /** @brief See @ref sdc_hci_cmd_le_iso_test_end(). */
    SDC_HCI_OPCODE_CMD_LE_ISO_TEST_END = 0x2073,
    /** @brief See @ref sdc_hci_cmd_le_set_host_feature(). */
    SDC_HCI_OPCODE_CMD_LE_SET_HOST_FEATURE = 0x2074,
    /** @brief See @ref sdc_hci_cmd_le_read_iso_link_quality(). */
    SDC_HCI_OPCODE_CMD_LE_READ_ISO_LINK_QUALITY = 0x2075,
    /** @brief See @ref sdc_hci_cmd_le_enhanced_read_transmit_power_level(). */
    SDC_HCI_OPCODE_CMD_LE_ENHANCED_READ_TRANSMIT_POWER_LEVEL = 0x2076,
    /** @brief See @ref sdc_hci_cmd_le_read_remote_transmit_power_level(). */
    SDC_HCI_OPCODE_CMD_LE_READ_REMOTE_TRANSMIT_POWER_LEVEL = 0x2077,
    /** @brief See @ref sdc_hci_cmd_le_set_path_loss_reporting_params(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PATH_LOSS_REPORTING_PARAMS = 0x2078,
    /** @brief See @ref sdc_hci_cmd_le_set_path_loss_reporting_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PATH_LOSS_REPORTING_ENABLE = 0x2079,
    /** @brief See @ref sdc_hci_cmd_le_set_transmit_power_reporting_enable(). */
    SDC_HCI_OPCODE_CMD_LE_SET_TRANSMIT_POWER_REPORTING_ENABLE = 0x207a,
    /** @brief See @ref sdc_hci_cmd_le_set_data_related_address_changes(). */
    SDC_HCI_OPCODE_CMD_LE_SET_DATA_RELATED_ADDRESS_CHANGES = 0x207c,
    /** @brief See @ref sdc_hci_cmd_le_set_default_subrate(). */
    SDC_HCI_OPCODE_CMD_LE_SET_DEFAULT_SUBRATE = 0x207d,
    /** @brief See @ref sdc_hci_cmd_le_subrate_request(). */
    SDC_HCI_OPCODE_CMD_LE_SUBRATE_REQUEST = 0x207e,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_subevent_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_SUBEVENT_DATA = 0x2082,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_response_data(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_RESPONSE_DATA = 0x2083,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_sync_subevent(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_SYNC_SUBEVENT = 0x2084,
    /** @brief See @ref sdc_hci_cmd_le_ext_create_conn_v2(). */
    SDC_HCI_OPCODE_CMD_LE_EXT_CREATE_CONN_V2 = 0x2085,
    /** @brief See @ref sdc_hci_cmd_le_set_periodic_adv_params_v2(). */
    SDC_HCI_OPCODE_CMD_LE_SET_PERIODIC_ADV_PARAMS_V2 = 0x2086,
};

/** @brief LE subevent Code values. */
enum sdc_hci_subevent_le
{
    /** @brief See @ref sdc_hci_subevent_le_subrate_change_t. */
    SDC_HCI_SUBEVENT_LE_SUBRATE_CHANGE = 0x23,
};

/** @brief Advertising Event Properties parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t connectable_adv : 1;
    uint16_t scannable_adv : 1;
    uint16_t directed_adv : 1;
    uint16_t high_duty_cycle_adv : 1;
    uint16_t legacy_adv_packets : 1;
    uint16_t omit_adv_address : 1;
    uint16_t include_tx_power : 1;
    uint16_t rfu : 9;
} sdc_hci_le_adv_event_properties_params_t;

/** @brief LE BIG Create Sync array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t bis;
} sdc_hci_le_big_create_sync_array_params_t;

/** @brief LE Create CIS array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t cis_conn_handle;
    uint16_t acl_conn_handle;
} sdc_hci_le_create_cis_array_params_t;

/** @brief LE Extended Create Connection [v1] array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t scan_interval;
    uint16_t scan_window;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t max_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_length;
    uint16_t max_ce_length;
} sdc_hci_le_ext_create_conn_array_params_t;

/** @brief LE Extended Create Connection [v2] array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t scan_interval;
    uint16_t scan_window;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t max_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_length;
    uint16_t max_ce_length;
} sdc_hci_le_ext_create_conn_v2_array_params_t;

/** @brief LE event mask.
 *
 * If the bit in the Event_Mask is set to a one,
 * then the event associated with that bit will be enabled.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t le_connection_complete_event : 1;
    uint8_t le_advertising_report_event : 1;
    uint8_t le_connection_update_complete_event : 1;
    uint8_t le_read_remote_features_complete_event : 1;
    uint8_t le_long_term_key_request_event : 1;
    uint8_t le_remote_connection_parameter_request_event : 1;
    uint8_t le_data_length_change_event : 1;
    uint8_t le_read_local_p256_public_key_complete_event : 1;
    uint8_t le_generate_dhkey_complete_event : 1;
    uint8_t le_enhanced_connection_complete_event : 1;
    uint8_t le_directed_advertising_report_event : 1;
    uint8_t le_phy_update_complete_event : 1;
    uint8_t le_extended_advertising_report_event : 1;
    uint8_t le_periodic_advertising_sync_established_event : 1;
    uint8_t le_periodic_advertising_report_event : 1;
    uint8_t le_periodic_advertising_sync_lost_event : 1;
    uint8_t le_scan_timeout_event : 1;
    uint8_t le_advertising_set_terminated_event : 1;
    uint8_t le_scan_request_received_event : 1;
    uint8_t le_channel_selection_algorithm_event : 1;
    uint8_t le_connectionless_iq_report_event : 1;
    uint8_t le_connection_iq_report_event : 1;
    uint8_t le_cte_request_failed_event : 1;
    uint8_t le_periodic_advertising_sync_transfer_received_event : 1;
    uint8_t le_cis_established_event : 1;
    uint8_t le_cis_request_event : 1;
    uint8_t le_create_big_complete_event : 1;
    uint8_t le_terminate_big_complete_event : 1;
    uint8_t le_big_sync_established_event : 1;
    uint8_t le_big_sync_lost_event : 1;
    uint8_t le_request_peer_sca_complete_event : 1;
    uint8_t le_path_loss_threshold_event : 1;
    uint8_t le_transmit_power_reporting_event : 1;
    uint8_t le_biginfo_advertising_report_event : 1;
    uint8_t le_subrate_change_event : 1;
    uint8_t le_periodic_advertising_sync_established_event_v2 : 1;
    uint8_t le_periodic_advertising_report_event_v2 : 1;
    uint8_t le_periodic_advertising_sync_transfer_received_event_v2 : 1;
    uint8_t le_periodic_advertising_subevent_data_request_event : 1;
    uint8_t le_periodic_advertising_response_report_event : 1;
    uint8_t le_enhanced_connection_complete_event_v2 : 1;
} sdc_hci_le_le_event_mask_t;

/** @brief LE features.
 *
 * See Core_v5.4, Vol 6, Part B, Section 4.6
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t le_encryption : 1;
    uint8_t connection_parameters_request : 1;
    uint8_t extended_reject_indication : 1;
    uint8_t slave_initiated_features_exchange : 1;
    uint8_t le_ping : 1;
    uint8_t le_data_packet_length_extension : 1;
    uint8_t ll_privacy : 1;
    uint8_t extended_scanner_filter_policies : 1;
    uint8_t le_2m_phy : 1;
    uint8_t stable_modulation_index_transmitter : 1;
    uint8_t stable_modulation_index_receiver : 1;
    uint8_t le_coded_phy : 1;
    uint8_t le_extended_advertising : 1;
    uint8_t le_periodic_advertising : 1;
    uint8_t channel_selection_algorithm_2 : 1;
    uint8_t le_Power_class_1 : 1;
    uint8_t minimum_number_of_used_channels_procedure : 1;
    uint8_t connection_cte_request : 1;
    uint8_t connection_cte_response : 1;
    uint8_t connectionless_cte_transmitter : 1;
    uint8_t connectionless_cte_receiver : 1;
    uint8_t antenna_switching_during_cte_transmission_aod : 1;
    uint8_t antenna_switching_during_cte_reception_aoa : 1;
    uint8_t receiving_constant_tone_extensions : 1;
    uint8_t periodic_advertising_sync_transfer_sender : 1;
    uint8_t periodic_advertising_sync_transfer_recipient : 1;
    uint8_t sleep_clock_accuracy_updates : 1;
    uint8_t remote_public_key_validation : 1;
    uint8_t connected_isochronous_stream_master : 1;
    uint8_t connected_isochronous_stream_slave : 1;
    uint8_t isochronous_broadcaster : 1;
    uint8_t synchronized_receiver : 1;
    uint8_t isochronous_channels_host_support : 1;
    uint8_t le_power_control_request : 1;
    uint8_t le_power_change_indication : 1;
    uint8_t le_path_loss_monitoring : 1;
    uint8_t periodic_advertising_adi_support : 1;
    uint8_t connection_subrating : 1;
    uint8_t connection_subrating_host_support : 1;
    uint8_t channel_classification : 1;
    uint8_t advertising_coding_selection : 1;
    uint8_t advertising_coding_selection_host_support : 1;
    uint8_t rfu42 : 1;
    uint8_t periodic_advertising_with_responses_advertiser : 1;
    uint8_t periodic_advertising_with_responses_scanner : 1;
} sdc_hci_le_le_features_t;

/** @brief Options parameters for HCI_LE_Periodic_Advertising_Create_Sync command. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t use_periodic_adv_list : 1;
    uint8_t disable_reporting : 1;
    uint8_t duplicate_filter_initially_enabled : 1;
    uint8_t rfu : 5;
} sdc_hci_le_periodic_adv_create_sync_options_params_t;

/** @brief LE Set CIG Parameters array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cis_id;
    uint16_t max_sdu_c_to_p;
    uint16_t max_sdu_p_to_c;
    uint8_t phy_c_to_p;
    uint8_t phy_p_to_c;
    uint8_t rtn_c_to_p;
    uint8_t rtn_p_to_c;
} sdc_hci_le_set_cig_params_array_params_t;

/** @brief LE Set CIG Parameters output array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_le_set_cig_params_output_array_params_t;

/** @brief LE Set CIG Parameters Test array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cis_id;
    uint8_t nse;
    uint16_t max_sdu_c_to_p;
    uint16_t max_sdu_p_to_c;
    uint16_t max_pdu_c_to_p;
    uint16_t max_pdu_p_to_c;
    uint8_t phy_c_to_p;
    uint8_t phy_p_to_c;
    uint8_t bn_c_to_p;
    uint8_t bn_p_to_c;
} sdc_hci_le_set_cig_params_test_array_params_t;

/** @brief LE Set CIG Parameters Test output array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_le_set_cig_params_test_output_array_params_t;

/** @brief LE Set Connection CTE Transmit Parameters array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t antenna_ids;
} sdc_hci_le_set_conn_cte_transmit_params_array_params_t;

/** @brief LE Set Connectionless CTE Transmit Parameters array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t antenna_ids;
} sdc_hci_le_set_connless_cte_transmit_params_array_params_t;

/** @brief Change Reasons parameters for HCI_LE_Set_Data_Related_Address_Changes command. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t change_on_adv_data_change : 1;
    uint8_t change_on_scan_response_data_change : 1;
    uint8_t rfu : 6;
} sdc_hci_le_set_data_related_address_changes_reasons_params_t;

/** @brief LE Set Extended Advertising Enable array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint16_t duration;
    uint8_t max_ext_adv_events;
} sdc_hci_le_set_ext_adv_enable_array_params_t;

/** @brief LE Set Extended Scan Parameters array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t scan_type;
    uint16_t scan_interval;
    uint16_t scan_window;
} sdc_hci_le_set_ext_scan_params_array_params_t;

/** @brief Enable parameters for HCI_LE_Set_Periodic_Advertising_Enable command. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t enable : 1;
    uint8_t include_adi : 1;
    uint8_t rfu : 6;
} sdc_hci_le_set_periodic_adv_enable_params_t;

/** @brief Enable parameters for HCI_LE_Set_Periodic_Advertising_Receive_Enable command. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t enable_reporting : 1;
    uint8_t enable_duplicate_filtering : 1;
    uint8_t rfu : 6;
} sdc_hci_le_set_periodic_adv_receive_enable_params_t;

/** @brief LE Set Periodic Advertising Subevent Data array parameters. */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t subevent;
    uint8_t response_slot_start;
    uint8_t response_slot_count;
    uint8_t subevent_data_length;
    uint8_t subevent_data[];
} sdc_hci_le_set_periodic_adv_subevent_data_array_params_t;

/** @} end of HCI_TYPES */

/**
 * @defgroup HCI_EVENTS Events
 * @{
 */

/** @brief LE Subrate Change.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.7.65.35
 *
 * The HCI_LE_Subrate_Change event is used to indicate that a Connection
 * Subrate Update procedure has completed and some parameters of the speci-
 * fied connection have changed.
 *
 * This event shall be issued if the HCI_LE_Subrate_Request command was
 * issued by the Host or the parameters are updated successfully following a
 * request from the peer device. If no parameters are updated following a request
 * from the peer device or the parameters were changed using the Connection
 * Update procedure, then this event shall not be issued.
 */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t status;
    uint16_t conn_handle;
    uint16_t subrate_factor;
    uint16_t peripheral_latency;
    uint16_t continuation_number;
    uint16_t supervision_timeout;
} sdc_hci_subevent_le_subrate_change_t;

/** @} end of HCI_EVENTS */

/**
 * @defgroup HCI_COMMAND_PARAMETERS Parameters
 * @{
 */

/** @brief LE Set Event Mask command parameter(s). */
typedef union __PACKED __ALIGN(1)
{
    sdc_hci_le_le_event_mask_t params;
    uint8_t raw[8];
} sdc_hci_cmd_le_set_event_mask_t;

/** @brief LE Read Buffer Size [v1] return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t le_acl_data_packet_length;
    uint8_t total_num_le_acl_data_packets;
} sdc_hci_cmd_le_read_buffer_size_return_t;

/** @brief LE Read Local Supported Features return parameter(s). */
typedef union __PACKED __ALIGN(1)
{
    sdc_hci_le_le_features_t params;
    uint8_t raw[8];
} sdc_hci_cmd_le_read_local_supported_features_return_t;

/** @brief LE Set Random Address command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t random_address[6];
} sdc_hci_cmd_le_set_random_address_t;

/** @brief LE Set Advertising Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t adv_interval_min;
    uint16_t adv_interval_max;
    uint8_t adv_type;
    uint8_t own_address_type;
    uint8_t peer_address_type;
    uint8_t peer_address[6];
    uint8_t adv_channel_map;
    uint8_t adv_filter_policy;
} sdc_hci_cmd_le_set_adv_params_t;

/** @brief LE Read Advertising Physical Channel Tx Power return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    int8_t tx_power_level;
} sdc_hci_cmd_le_read_adv_physical_channel_tx_power_return_t;

/** @brief LE Set Advertising Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_data_length;
    uint8_t adv_data[31];
} sdc_hci_cmd_le_set_adv_data_t;

/** @brief LE Set Scan Response Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t scan_response_data_length;
    uint8_t scan_response_data[31];
} sdc_hci_cmd_le_set_scan_response_data_t;

/** @brief LE Set Advertising Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_enable;
} sdc_hci_cmd_le_set_adv_enable_t;

/** @brief LE Set Scan Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t le_scan_type;
    uint16_t le_scan_interval;
    uint16_t le_scan_window;
    uint8_t own_address_type;
    uint8_t scanning_filter_policy;
} sdc_hci_cmd_le_set_scan_params_t;

/** @brief LE Set Scan Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t le_scan_enable;
    uint8_t filter_duplicates;
} sdc_hci_cmd_le_set_scan_enable_t;

/** @brief LE Create Connection command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t le_scan_interval;
    uint16_t le_scan_window;
    uint8_t initiator_filter_policy;
    uint8_t peer_address_type;
    uint8_t peer_address[6];
    uint8_t own_address_type;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t max_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_length;
    uint16_t max_ce_length;
} sdc_hci_cmd_le_create_conn_t;

/** @brief LE Read Filter Accept List Size return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t filter_accept_list_size;
} sdc_hci_cmd_le_read_filter_accept_list_size_return_t;

/** @brief LE Add Device To Filter Accept List command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t address_type;
    uint8_t address[6];
} sdc_hci_cmd_le_add_device_to_filter_accept_list_t;

/** @brief LE Remove Device From Filter Accept List command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t address_type;
    uint8_t address[6];
} sdc_hci_cmd_le_remove_device_from_filter_accept_list_t;

/** @brief LE Connection Update command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;
    uint16_t max_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_length;
    uint16_t max_ce_length;
} sdc_hci_cmd_le_conn_update_t;

/** @brief LE Set Host Channel Classification command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t channel_map[5];
} sdc_hci_cmd_le_set_host_channel_classification_t;

/** @brief LE Read Channel Map command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_read_channel_map_t;

/** @brief LE Read Channel Map return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t channel_map[5];
} sdc_hci_cmd_le_read_channel_map_return_t;

/** @brief LE Read Remote Features command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_read_remote_features_t;

/** @brief LE Encrypt command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t key[16];
    uint8_t plaintext_data[16];
} sdc_hci_cmd_le_encrypt_t;

/** @brief LE Encrypt return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t encrypted_data[16];
} sdc_hci_cmd_le_encrypt_return_t;

/** @brief LE Rand return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint64_t random_number;
} sdc_hci_cmd_le_rand_return_t;

/** @brief LE Enable Encryption command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t random_number[8];
    uint8_t encrypted_diversifier[2];
    uint8_t long_term_key[16];
} sdc_hci_cmd_le_enable_encryption_t;

/** @brief LE Long Term Key Request Reply command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t long_term_key[16];
} sdc_hci_cmd_le_long_term_key_request_reply_t;

/** @brief LE Long Term Key Request Reply return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_long_term_key_request_reply_return_t;

/** @brief LE Long Term Key Request Negative Reply command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_long_term_key_request_negative_reply_t;

/** @brief LE Long Term Key Request Negative Reply return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_long_term_key_request_negative_reply_return_t;

/** @brief LE Read Supported States return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    /** @brief See the table in Core v5.4, Vol 4, Part E, Section 7.8.27. */
    uint8_t le_states[8];
} sdc_hci_cmd_le_read_supported_states_return_t;

/** @brief LE Test End return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t num_packets;
} sdc_hci_cmd_le_test_end_return_t;

/** @brief LE Set Data Length command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t tx_octets;
    uint16_t tx_time;
} sdc_hci_cmd_le_set_data_length_t;

/** @brief LE Set Data Length return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_set_data_length_return_t;

/** @brief LE Read Suggested Default Data Length return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t suggested_max_tx_octets;
    uint16_t suggested_max_tx_time;
} sdc_hci_cmd_le_read_suggested_default_data_length_return_t;

/** @brief LE Write Suggested Default Data Length command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t suggested_max_tx_octets;
    uint16_t suggested_max_tx_time;
} sdc_hci_cmd_le_write_suggested_default_data_length_t;

/** @brief LE Add Device To Resolving List command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t peer_identity_address_type;
    uint8_t peer_identity_address[6];
    uint8_t peer_irk[16];
    uint8_t local_irk[16];
} sdc_hci_cmd_le_add_device_to_resolving_list_t;

/** @brief LE Remove Device From Resolving List command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t peer_identity_address_type;
    uint8_t peer_identity_address[6];
} sdc_hci_cmd_le_remove_device_from_resolving_list_t;

/** @brief LE Read Resolving List Size return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t resolving_list_size;
} sdc_hci_cmd_le_read_resolving_list_size_return_t;

/** @brief LE Set Address Resolution Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t address_resolution_enable;
} sdc_hci_cmd_le_set_address_resolution_enable_t;

/** @brief LE Set Resolvable Private Address Timeout command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t rpa_timeout;
} sdc_hci_cmd_le_set_resolvable_private_address_timeout_t;

/** @brief LE Read Maximum Data Length return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t supported_max_tx_octets;
    uint16_t supported_max_tx_time;
    uint16_t supported_max_rx_octets;
    uint16_t supported_max_rx_time;
} sdc_hci_cmd_le_read_max_data_length_return_t;

/** @brief LE Read PHY command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_read_phy_t;

/** @brief LE Read PHY return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t tx_phy;
    uint8_t rx_phy;
} sdc_hci_cmd_le_read_phy_return_t;

/** @brief LE Set Default PHY command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
} sdc_hci_cmd_le_set_default_phy_t;

/** @brief LE Set PHY command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
    uint16_t phy_options;
} sdc_hci_cmd_le_set_phy_t;

/** @brief LE Set Advertising Set Random Address command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t random_address[6];
} sdc_hci_cmd_le_set_adv_set_random_address_t;

/** @brief LE Set Extended Advertising Parameters [v1] command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    union __PACKED __ALIGN(1) {
        sdc_hci_le_adv_event_properties_params_t params;
        uint8_t raw[2];
    } adv_event_properties;
    uint32_t primary_adv_interval_min : 24;
    uint32_t primary_adv_interval_max : 24;
    uint8_t primary_adv_channel_map;
    uint8_t own_address_type;
    uint8_t peer_address_type;
    uint8_t peer_address[6];
    uint8_t adv_filter_policy;
    int8_t adv_tx_power;
    uint8_t primary_adv_phy;
    uint8_t secondary_adv_max_skip;
    uint8_t secondary_adv_phy;
    uint8_t adv_sid;
    uint8_t scan_request_notification_enable;
} sdc_hci_cmd_le_set_ext_adv_params_t;

/** @brief LE Set Extended Advertising Parameters [v1] return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    int8_t selected_tx_power;
} sdc_hci_cmd_le_set_ext_adv_params_return_t;

/** @brief LE Set Extended Advertising Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_preference;
    uint8_t adv_data_length;
    uint8_t adv_data[];
} sdc_hci_cmd_le_set_ext_adv_data_t;

/** @brief LE Set Extended Scan Response Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_preference;
    uint8_t scan_response_data_length;
    uint8_t scan_response_data[];
} sdc_hci_cmd_le_set_ext_scan_response_data_t;

/** @brief LE Set Extended Advertising Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t enable;
    uint8_t num_sets;
    sdc_hci_le_set_ext_adv_enable_array_params_t array_params[];
} sdc_hci_cmd_le_set_ext_adv_enable_t;

/** @brief LE Read Maximum Advertising Data Length return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t max_adv_data_length;
} sdc_hci_cmd_le_read_max_adv_data_length_return_t;

/** @brief LE Read Number of Supported Advertising Sets return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t num_supported_adv_sets;
} sdc_hci_cmd_le_read_number_of_supported_adv_sets_return_t;

/** @brief LE Remove Advertising Set command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
} sdc_hci_cmd_le_remove_adv_set_t;

/** @brief LE Set Periodic Advertising Parameters [v1] command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint16_t periodic_adv_interval_min;
    uint16_t periodic_adv_interval_max;
    uint16_t periodic_adv_properties;
} sdc_hci_cmd_le_set_periodic_adv_params_t;

/** @brief LE Set Periodic Advertising Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t adv_data_length;
    uint8_t adv_data[];
} sdc_hci_cmd_le_set_periodic_adv_data_t;

/** @brief LE Set Periodic Advertising Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    union __PACKED __ALIGN(1) {
        sdc_hci_le_set_periodic_adv_enable_params_t params;
        uint8_t raw[1];
    } enable;
    uint8_t adv_handle;
} sdc_hci_cmd_le_set_periodic_adv_enable_t;

/** @brief LE Set Extended Scan Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t own_address_type;
    uint8_t scanning_filter_policy;
    uint8_t scanning_phys;
    sdc_hci_le_set_ext_scan_params_array_params_t array_params[];
} sdc_hci_cmd_le_set_ext_scan_params_t;

/** @brief LE Set Extended Scan Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t enable;
    uint8_t filter_duplicates;
    uint16_t duration;
    uint16_t period;
} sdc_hci_cmd_le_set_ext_scan_enable_t;

/** @brief LE Extended Create Connection [v1] command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t initiator_filter_policy;
    uint8_t own_address_type;
    uint8_t peer_address_type;
    uint8_t peer_address[6];
    uint8_t initiating_phys;
    sdc_hci_le_ext_create_conn_array_params_t array_params[];
} sdc_hci_cmd_le_ext_create_conn_t;

/** @brief LE Periodic Advertising Create Sync command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    union __PACKED __ALIGN(1) {
        sdc_hci_le_periodic_adv_create_sync_options_params_t params;
        uint8_t raw[1];
    } options;
    uint8_t adv_sid;
    uint8_t adv_address_type;
    uint8_t adv_address[6];
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t sync_cte_type;
} sdc_hci_cmd_le_periodic_adv_create_sync_t;

/** @brief LE Periodic Advertising Terminate Sync command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t sync_handle;
} sdc_hci_cmd_le_periodic_adv_terminate_sync_t;

/** @brief LE Add Device To Periodic Advertiser List command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_address_type;
    uint8_t adv_address[6];
    uint8_t adv_sid;
} sdc_hci_cmd_le_add_device_to_periodic_adv_list_t;

/** @brief LE Remove Device From Periodic Advertiser List command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_address_type;
    uint8_t adv_address[6];
    uint8_t adv_sid;
} sdc_hci_cmd_le_remove_device_from_periodic_adv_list_t;

/** @brief LE Read Periodic Advertiser List Size return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t periodic_adv_list_size;
} sdc_hci_cmd_le_read_periodic_adv_list_size_return_t;

/** @brief LE Read Transmit Power return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    int8_t min_tx_power;
    int8_t max_tx_power;
} sdc_hci_cmd_le_read_transmit_power_return_t;

/** @brief LE Read RF Path Compensation return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    int16_t rf_tx_path_compensation_value;
    int16_t rf_rx_path_compensation_value;
} sdc_hci_cmd_le_read_rf_path_compensation_return_t;

/** @brief LE Write RF Path Compensation command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    int16_t rf_tx_path_compensation_value;
    int16_t rf_rx_path_compensation_value;
} sdc_hci_cmd_le_write_rf_path_compensation_t;

/** @brief LE Set Privacy Mode command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t peer_identity_address_type;
    uint8_t peer_identity_address[6];
    uint8_t privacy_mode;
} sdc_hci_cmd_le_set_privacy_mode_t;

/** @brief LE Set Connectionless CTE Transmit Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t cte_length;
    uint8_t cte_type;
    uint8_t cte_count;
    uint8_t switching_pattern_length;
    sdc_hci_le_set_connless_cte_transmit_params_array_params_t array_params[];
} sdc_hci_cmd_le_set_connless_cte_transmit_params_t;

/** @brief LE Set Connectionless CTE Transmit Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t cte_enable;
} sdc_hci_cmd_le_set_connless_cte_transmit_enable_t;

/** @brief LE Set Connection CTE Transmit Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t cte_types;
    uint8_t switching_pattern_length;
    sdc_hci_le_set_conn_cte_transmit_params_array_params_t array_params[];
} sdc_hci_cmd_le_set_conn_cte_transmit_params_t;

/** @brief LE Set Connection CTE Transmit Parameters return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_set_conn_cte_transmit_params_return_t;

/** @brief LE Connection CTE Response Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t enable;
} sdc_hci_cmd_le_conn_cte_response_enable_t;

/** @brief LE Connection CTE Response Enable return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_conn_cte_response_enable_return_t;

/** @brief LE Read Antenna Information return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t supported_switching_sampling_rates;
    uint8_t num_antennae;
    uint8_t max_switching_pattern_length;
    uint8_t max_cte_length;
} sdc_hci_cmd_le_read_antenna_information_return_t;

/** @brief LE Set Periodic Advertising Receive Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t sync_handle;
    union __PACKED __ALIGN(1) {
        sdc_hci_le_set_periodic_adv_receive_enable_params_t params;
        uint8_t raw[1];
    } enable;
} sdc_hci_cmd_le_set_periodic_adv_receive_enable_t;

/** @brief LE Periodic Advertising Sync Transfer command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t service_data;
    uint16_t sync_handle;
} sdc_hci_cmd_le_periodic_adv_sync_transfer_t;

/** @brief LE Periodic Advertising Sync Transfer return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_periodic_adv_sync_transfer_return_t;

/** @brief LE Periodic Advertising Set Info Transfer command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t service_data;
    uint8_t adv_handle;
} sdc_hci_cmd_le_periodic_adv_set_info_transfer_t;

/** @brief LE Periodic Advertising Set Info Transfer return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_periodic_adv_set_info_transfer_return_t;

/** @brief LE Set Periodic Advertising Sync Transfer Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t mode;
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t cte_type;
} sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params_t;

/** @brief LE Set Periodic Advertising Sync Transfer Parameters return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params_return_t;

/** @brief LE Set Default Periodic Advertising Sync Transfer Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t mode;
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t cte_type;
} sdc_hci_cmd_le_set_default_periodic_adv_sync_transfer_params_t;

/** @brief LE Read Buffer Size [v2] return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t le_acl_data_packet_length;
    uint8_t total_num_le_acl_data_packets;
    uint16_t iso_data_packet_length;
    uint8_t total_num_iso_data_packets;
} sdc_hci_cmd_le_read_buffer_size_v2_return_t;

/** @brief LE Read ISO TX Sync command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_read_iso_tx_sync_t;

/** @brief LE Read ISO TX Sync return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t packet_sequence_number;
    uint32_t tx_time_stamp;
    uint32_t time_offset : 24;
} sdc_hci_cmd_le_read_iso_tx_sync_return_t;

/** @brief LE Set CIG Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cig_id;
    uint32_t sdu_interval_c_to_p : 24;
    uint32_t sdu_interval_p_to_c : 24;
    uint8_t worst_case_sca;
    uint8_t packing;
    uint8_t framing;
    uint16_t max_transport_latency_c_to_p;
    uint16_t max_transport_latency_p_to_c;
    uint8_t cis_count;
    sdc_hci_le_set_cig_params_array_params_t array_params[];
} sdc_hci_cmd_le_set_cig_params_t;

/** @brief LE Set CIG Parameters return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cig_id;
    uint8_t cis_count;
    sdc_hci_le_set_cig_params_output_array_params_t array_params[];
} sdc_hci_cmd_le_set_cig_params_return_t;

/** @brief LE Set CIG Parameters Test command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cig_id;
    uint32_t sdu_interval_c_to_p : 24;
    uint32_t sdu_interval_p_to_c : 24;
    uint8_t ft_c_to_p;
    uint8_t ft_p_to_c;
    uint16_t iso_interval;
    uint8_t worst_case_sca;
    uint8_t packing;
    uint8_t framing;
    uint8_t cis_count;
    sdc_hci_le_set_cig_params_test_array_params_t array_params[];
} sdc_hci_cmd_le_set_cig_params_test_t;

/** @brief LE Set CIG Parameters Test return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cig_id;
    uint8_t cis_count;
    sdc_hci_le_set_cig_params_test_output_array_params_t array_params[];
} sdc_hci_cmd_le_set_cig_params_test_return_t;

/** @brief LE Create CIS command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cis_count;
    sdc_hci_le_create_cis_array_params_t array_params[];
} sdc_hci_cmd_le_create_cis_t;

/** @brief LE Remove CIG command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cig_id;
} sdc_hci_cmd_le_remove_cig_t;

/** @brief LE Remove CIG return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t cig_id;
} sdc_hci_cmd_le_remove_cig_return_t;

/** @brief LE Accept CIS Request command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_accept_cis_request_t;

/** @brief LE Reject CIS Request command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t reason;
} sdc_hci_cmd_le_reject_cis_request_t;

/** @brief LE Reject CIS Request return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_reject_cis_request_return_t;

/** @brief LE Create BIG command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t big_handle;
    uint8_t adv_handle;
    uint8_t num_bis;
    uint32_t sdu_interval : 24;
    uint16_t max_sdu;
    uint16_t max_transport_latency;
    uint8_t rtn;
    uint8_t phy;
    uint8_t packing;
    uint8_t framing;
    uint8_t encryption;
    uint8_t broadcast_code[16];
} sdc_hci_cmd_le_create_big_t;

/** @brief LE Create BIG Test command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t big_handle;
    uint8_t adv_handle;
    uint8_t num_bis;
    uint32_t sdu_interval : 24;
    uint16_t iso_interval;
    uint8_t nse;
    uint16_t max_sdu;
    uint16_t max_pdu;
    uint8_t phy;
    uint8_t packing;
    uint8_t framing;
    uint8_t bn;
    uint8_t irc;
    uint8_t pto;
    uint8_t encryption;
    uint8_t broadcast_code[16];
} sdc_hci_cmd_le_create_big_test_t;

/** @brief LE Terminate BIG command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t big_handle;
    uint8_t reason;
} sdc_hci_cmd_le_terminate_big_t;

/** @brief LE BIG Create Sync command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t big_handle;
    uint16_t sync_handle;
    uint8_t encryption;
    uint8_t broadcast_code[16];
    uint8_t mse;
    uint16_t big_sync_timeout;
    uint8_t num_bis;
    sdc_hci_le_big_create_sync_array_params_t array_params[];
} sdc_hci_cmd_le_big_create_sync_t;

/** @brief LE BIG Terminate Sync command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t big_handle;
} sdc_hci_cmd_le_big_terminate_sync_t;

/** @brief LE BIG Terminate Sync return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t big_handle;
} sdc_hci_cmd_le_big_terminate_sync_return_t;

/** @brief LE Request Peer SCA command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_request_peer_sca_t;

/** @brief LE Setup ISO Data Path command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t data_path_direction;
    uint8_t data_path_id;
    uint8_t codec_id[5];
    uint32_t controller_delay : 24;
    uint8_t codec_config_length;
    uint8_t codec_config[];
} sdc_hci_cmd_le_setup_iso_data_path_t;

/** @brief LE Setup ISO Data Path return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_setup_iso_data_path_return_t;

/** @brief LE Remove ISO Data Path command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t data_path_direction;
} sdc_hci_cmd_le_remove_iso_data_path_t;

/** @brief LE Remove ISO Data Path return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_remove_iso_data_path_return_t;

/** @brief LE ISO Transmit Test command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t payload_type;
} sdc_hci_cmd_le_iso_transmit_test_t;

/** @brief LE ISO Transmit Test return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_iso_transmit_test_return_t;

/** @brief LE ISO Receive Test command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t payload_type;
} sdc_hci_cmd_le_iso_receive_test_t;

/** @brief LE ISO Receive Test return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_iso_receive_test_return_t;

/** @brief LE ISO Read Test Counters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_iso_read_test_counters_t;

/** @brief LE ISO Read Test Counters return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint32_t received_sdu_count;
    uint32_t missed_sdu_count;
    uint32_t failed_sdu_count;
} sdc_hci_cmd_le_iso_read_test_counters_return_t;

/** @brief LE ISO Test End command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_iso_test_end_t;

/** @brief LE ISO Test End return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint32_t received_sdu_count;
    uint32_t missed_sdu_count;
    uint32_t failed_sdu_count;
} sdc_hci_cmd_le_iso_test_end_return_t;

/** @brief LE Set Host Feature command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t bit_number;
    uint8_t bit_value;
} sdc_hci_cmd_le_set_host_feature_t;

/** @brief LE Read ISO Link Quality command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_read_iso_link_quality_t;

/** @brief LE Read ISO Link Quality return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint32_t tx_unacked_packets;
    uint32_t tx_flushed_packets;
    uint32_t tx_last_subevent_packets;
    uint32_t retransmitted_packets;
    uint32_t crc_error_packets;
    uint32_t rx_unreceived_packets;
    uint32_t duplicate_packets;
} sdc_hci_cmd_le_read_iso_link_quality_return_t;

/** @brief LE Enhanced Read Transmit Power Level command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t phy;
} sdc_hci_cmd_le_enhanced_read_transmit_power_level_t;

/** @brief LE Enhanced Read Transmit Power Level return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t phy;
    int8_t current_tx_power_level;
    int8_t max_tx_power_level;
} sdc_hci_cmd_le_enhanced_read_transmit_power_level_return_t;

/** @brief LE Read Remote Transmit Power Level command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t phy;
} sdc_hci_cmd_le_read_remote_transmit_power_level_t;

/** @brief LE Set Path Loss Reporting Parameters command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t high_threshold;
    uint8_t high_hysteresis;
    uint8_t low_threshold;
    uint8_t low_hysteresis;
    uint16_t min_time_spent;
} sdc_hci_cmd_le_set_path_loss_reporting_params_t;

/** @brief LE Set Path Loss Reporting Parameters return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_set_path_loss_reporting_params_return_t;

/** @brief LE Set Path Loss Reporting Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t enable;
} sdc_hci_cmd_le_set_path_loss_reporting_enable_t;

/** @brief LE Set Path Loss Reporting Enable return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_set_path_loss_reporting_enable_return_t;

/** @brief LE Set Transmit Power Reporting Enable command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint8_t local_enable;
    uint8_t remote_enable;
} sdc_hci_cmd_le_set_transmit_power_reporting_enable_t;

/** @brief LE Set Transmit Power Reporting Enable return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
} sdc_hci_cmd_le_set_transmit_power_reporting_enable_return_t;

/** @brief LE Set Data Related Address Changes command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    union __PACKED __ALIGN(1) {
        sdc_hci_le_set_data_related_address_changes_reasons_params_t params;
        uint8_t raw[1];
    } change_reasons;
} sdc_hci_cmd_le_set_data_related_address_changes_t;

/** @brief LE Set Default Subrate command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t subrate_min;
    uint16_t subrate_max;
    uint16_t max_latency;
    uint16_t continuation_number;
    uint16_t supervision_timeout;
} sdc_hci_cmd_le_set_default_subrate_t;

/** @brief LE Subrate Request command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t conn_handle;
    uint16_t subrate_min;
    uint16_t subrate_max;
    uint16_t max_latency;
    uint16_t continuation_number;
    uint16_t supervision_timeout;
} sdc_hci_cmd_le_subrate_request_t;

/** @brief LE Set Periodic Advertising Subevent Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t num_subevents;
    uint8_t array_params[];
} sdc_hci_cmd_le_set_periodic_adv_subevent_data_t;

/** @brief LE Set Periodic Advertising Subevent Data return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
} sdc_hci_cmd_le_set_periodic_adv_subevent_data_return_t;

/** @brief LE Set Periodic Advertising Response Data command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t sync_handle;
    uint16_t request_event;
    uint8_t request_subevent;
    uint8_t response_subevent;
    uint8_t response_slot;
    uint8_t response_data_length;
    uint8_t response_data[];
} sdc_hci_cmd_le_set_periodic_adv_response_data_t;

/** @brief LE Set Periodic Advertising Response Data return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t sync_handle;
} sdc_hci_cmd_le_set_periodic_adv_response_data_return_t;

/** @brief LE Set Periodic Sync Subevent command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t sync_handle;
    uint16_t periodic_adv_properties;
    uint8_t num_subevents;
    uint8_t subevents[];
} sdc_hci_cmd_le_set_periodic_sync_subevent_t;

/** @brief LE Set Periodic Sync Subevent return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint16_t sync_handle;
} sdc_hci_cmd_le_set_periodic_sync_subevent_return_t;

/** @brief LE Extended Create Connection [v2] command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint8_t subevent;
    uint8_t initiator_filter_policy;
    uint8_t own_address_type;
    uint8_t peer_address_type;
    uint8_t peer_address[6];
    uint8_t initiating_phys;
    sdc_hci_le_ext_create_conn_v2_array_params_t array_params[];
} sdc_hci_cmd_le_ext_create_conn_v2_t;

/** @brief LE Set Periodic Advertising Parameters [v2] command parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
    uint16_t periodic_adv_interval_min;
    uint16_t periodic_adv_interval_max;
    uint16_t periodic_adv_properties;
    uint8_t num_subevents;
    uint8_t subevent_interval;
    uint8_t response_slot_delay;
    uint8_t response_slot_spacing;
    uint8_t num_response_slots;
} sdc_hci_cmd_le_set_periodic_adv_params_v2_t;

/** @brief LE Set Periodic Advertising Parameters [v2] return parameter(s). */
typedef struct __PACKED __ALIGN(1)
{
    uint8_t adv_handle;
} sdc_hci_cmd_le_set_periodic_adv_params_v2_return_t;

/** @} end of HCI_COMMAND_PARAMETERS */

/**
 * @defgroup HCI_VS_API API
 * @{
 */
/** @brief LE Set Event Mask.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.1
 *
 * The HCI_LE_Set_Event_Mask command is used to control which LE events
 * are generated by the HCI for the Host. If the bit in the LE_Event_Mask is set to
 * a one, then the event associated with that bit will be enabled. The event mask
 * allows the Host to control which events will interrupt it.
 *
 * The Controller shall ignore those bits which are reserved for future use or
 * represent events which it does not support. If the Host sets any of these bits to
 * 1, the Controller shall act as if they were set to 0.
 *
 * For LE events to be generated, the LE Meta event bit in the Event_Mask shall
 * also be set. If that bit is not set, then LE events shall not be generated,
 * regardless of how the LE_Event_Mask is set.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Event_Mask command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_event_mask(const sdc_hci_cmd_le_set_event_mask_t * p_params);

/** @brief LE Read Buffer Size [v1].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.2
 *
 * This command is used to read the maximum size of the data portion of ACL
 * data packets and isochronous data packets sent from the Host to the Control-
 * ler. The Host shall segment the data transmitted to the Controller according to
 * these values so that the HCI ACL Data packets and HCI ISO Data packets will
 * contain data up to this size (“data” includes optional fields in the HCI ISO Data
 * packet, such as ISO_SDU_Length). The HCI_LE_Read_Buffer_Size
 * command also returns the total number of HCI LE ACL Data packets and iso-
 * chronous data packets that can be stored in the data buffers of the Controller.
 * The HCI_LE_Read_Buffer_Size command shall be issued by the Host before it
 * sends any data to an LE Controller (see Section 4.1.1). If the Controller sup-
 * ports HCI ISO Data packets, it shall return non-zero values for the ISO_Data_-
 * Packet_Length and Total_Num_ISO_Data_Packets parameters.
 *
 * If the Controller returns a length value of zero for ACL data packets, the Host
 * shall use the HCI_Read_Buffer_Size command to determine the size of the
 * data buffers (shared between BR/EDR and LE transports).
 *
 * Note: Both the HCI_Read_Buffer_Size command and the HCI_LE_Read_-
 * Buffer_Size command may return buffer length and number of packets param-
 * eter values that are nonzero. This allows a Controller to offer different buffers
 * and number of buffers for BR/EDR data packets and LE data packets.
 *
 * The LE_ACL_Data_Packet_Length return parameter shall be used to deter-
 * mine the maximum size of the L2CAP PDU segments that are contained in
 * ACL data packets, and which are transferred from the Host to the Controller to
 * be broken up into packets by the Link Layer. The Total_Num_LE_ACL_Data_-
 * Packets return parameter contains the total number of HCI ACL Data packets
 * that can be stored in the data buffers of the Controller. The Host determines
 * how to divide the buffers between different connection handles.
 *
 * The ISO_Data_Packet_Length return parameter shall be used to determine the
 * maximum size of the SDU segments that are contained in isochronous data
 * packets, and which are transferred from the Host to the Controller. The
 * Total_Num_ISO_Data_Packets return parameter contains the total number of
 * isochronous data packets that can be stored in the data buffers of the
 * Controller. The Host determines how to divide the buffers between different
 * connection handle(s).
 *
 * Note: The LE_ACL_Data_Packet_Length and ISO_Data_Packet_Length
 * return parameters do not include the length of the HCI ACL Data packet header
 * or the HCI ISO Data packet header respectively.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Buffer_Size command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_buffer_size(sdc_hci_cmd_le_read_buffer_size_return_t * p_return);

/** @brief LE Read Local Supported Features.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.3
 *
 * This command requests the list of the supported LE features for the Controller.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Local_Supported_Features command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_local_supported_features(sdc_hci_cmd_le_read_local_supported_features_return_t * p_return);

/** @brief LE Set Random Address.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.4
 *
 * The HCI_LE_Set_Random_Address command is used by the Host to set the
 * LE Random Device Address in the Controller (see [Vol 6] Part B, Section 1.3).
 *
 * If this command is used to change the address, the new random address shall
 * take effect for advertising no later than the next successful HCI_LE_Set_-
 * Advertising_Enable command, for scanning no later than the next successful
 * HCI_LE_Set_Scan_Enable command or HCI_LE_Set_Extended_Scan_-
 * Enable command, and for initiating no later than the next successful HCI_LE_-
 * Create_Connection command or HCI_LE_Extended_Create_Connection
 * command.
 *
 * Note: If the extended advertising commands are in use, this command only
 * affects the address used for scanning and initiating. The addresses used for
 * advertising are set by the HCI_LE_Set_Advertising_Set_Random_Address
 * command (see Section 7.8.52).
 *
 * If the Host issues this command when any of advertising (created using legacy
 * advertising commands), scanning, or initiating are enabled, the Controller shall
 * return the error code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Random_Address command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_random_address(const sdc_hci_cmd_le_set_random_address_t * p_params);

/** @brief LE Set Advertising Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.5
 *
 * The HCI_LE_Set_Advertising_Parameters command is used by the Host to set
 * the advertising parameters.
 *
 * The Advertising_Interval_Min shall be less than or equal to the
 * Advertising_Interval_Max. The Advertising_Interval_Min and
 * Advertising_Interval_Max should not be the same value to enable the
 * Controller to determine the best advertising interval given other activities.
 *
 * For high duty cycle directed advertising, i.e. when Advertising_Type is 0x01
 * (ADV_DIRECT_IND, high duty cycle), the Advertising_Interval_Min and
 * Advertising_Interval_Max parameters are not used and shall be ignored.
 *
 * The Advertising_Type is used to determine the packet type that is used for
 * advertising when advertising is enabled.
 *
 * Own_Address_Type parameter indicates the type of address being used in the
 * advertising packets.
 *
 * If Own_Address_Type equals 0x02 or 0x03, the Peer_Address parameter
 * contains the peer’s Identity Address and the Peer_Address_Type parameter
 * contains the Peer’s Identity Type (i.e. 0x00 or 0x01). These parameters are
 * used to locate the corresponding local IRK in the resolving list; this IRK is used
 * to generate the own address used in the advertisement.
 *
 * If directed advertising is performed, i.e. when Advertising_Type is set to 0x01
 * (ADV_DIRECT_IND, high duty cycle) or 0x04 (ADV_DIRECT_IND, low duty
 * cycle mode), then the Peer_Address_Type and Peer_Address shall be valid.
 *
 * If Own_Address_Type equals 0x02 or 0x03, the Controller generates the
 * peer’s Resolvable Private Address using the peer’s IRK corresponding to the
 * peer’s Identity Address contained in the Peer_Address parameter and peer’s
 * Identity Address Type (i.e. 0x00 or 0x01) contained in the Peer_Address_Type
 * parameter.
 * The Advertising_Channel_Map is a bit field that indicates the advertising
 * channel indices that shall be used when transmitting advertising packets. At
 * least one channel bit shall be set in the Advertising_Channel_Map parameter.
 *
 * The Advertising_Filter_Policy parameter shall be ignored when directed
 * advertising is enabled.
 *
 * The Host shall not issue this command when advertising is enabled in the
 * Controller; if it is the Command Disallowed error code shall be used.
 *
 * If the advertising interval range provided by the Host (Advertising_Interval_Min,
 * Advertising_Interval_Max) does not overlap with the advertising interval range
 * supported by the Controller, then the Controller shall return the Unsupported
 * Feature or Parameter Value (0x11) error code.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Advertising_Parameters command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_adv_params(const sdc_hci_cmd_le_set_adv_params_t * p_params);

/** @brief LE Read Advertising Physical Channel Tx Power.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.6
 *
 * The HCI_LE_Read_Advertising_Physical_Channel_Tx_Power command is
 * used by the Host to read the transmit power level used for LE advertising
 * physical channel packets.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Advertising_Physical_Channel_Tx_Power command
 * has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_adv_physical_channel_tx_power(sdc_hci_cmd_le_read_adv_physical_channel_tx_power_return_t * p_return);

/** @brief LE Set Advertising Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.7
 *
 * The HCI_LE_Set_Advertising_Data command is used to set the data used in
 * advertising packets that have a data field.
 * Only the significant part of the Advertising_Data should be transmitted in the
 * advertising packets, as defined in [Vol 3] Part C, Section 11.
 * If advertising is currently enabled, the Controller shall use the new data in
 * subsequent advertising events. If an advertising event is in progress when this
 * command is issued, the Controller may use the old or new data for that event.
 * If advertising is currently disabled, the data shall be kept by the Controller and
 * used once advertising is enabled.
 * The default Advertising_Data_Length shall be zero and the default
 * Advertising_Data shall be 31 zero octets.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Advertising_Data command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_adv_data(const sdc_hci_cmd_le_set_adv_data_t * p_params);

/** @brief LE Set Scan Response Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.8
 *
 * This command is used to provide data used in Scanning Packets that have a
 * data field.
 *
 * Only the significant part of the Scan_Response_Data should be transmitted in
 * the Scanning Packets, as defined in [Vol 3] Part C, Section 11.
 *
 * If advertising is currently enabled, the Controller shall use the new data in
 * subsequent advertising events. If an advertising event is in progress when this
 * command is issued, the Controller may use the old or new data for that event.
 * If advertising is currently disabled, the data shall be kept by the Controller and
 * used once advertising is enabled.
 *
 * The default Scan_Response_Data_Length shall be zero and the default
 * Scan_Response_Data shall be 31 zero octets.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Scan_Response_Data command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_scan_response_data(const sdc_hci_cmd_le_set_scan_response_data_t * p_params);

/** @brief LE Set Advertising Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.9
 *
 * The HCI_LE_Set_Advertising_Enable command is used to request the
 * Controller to start or stop advertising. The Controller manages the timing of
 * advertisements as per the advertising parameters given in the
 * HCI_LE_Set_Advertising_Parameters command.
 *
 * The Controller shall continue advertising until the Host issues an HCI_LE_-
 * Set_Advertising_Enable command with Advertising_Enable set to 0x00
 * (Advertising is disabled). a connection is created using the advertising, or the
 * Advertising is timed out due to high duty cycle Directed Advertising. In these
 * cases, advertising is then disabled.
 *
 * If Advertising_Enable is set to 0x01, the advertising parameters' Own_-
 * Address_Type parameter is set to 0x00, and the device does not have a public
 * address, the Controller should return an error code which should be Invalid HCI
 * Command Parameters (0x12).
 *
 * If Advertising_Enable is set to 0x01, the advertising parameters' Own_-
 * Address_Type parameter is set to 0x01, and the random address for the device
 * has not been initialized using the HCI_LE_Set_Random_Address command,
 * the Controller shall return the error code Invalid HCI Command Parameters
 * (0x12).
 *
 * If Advertising_Enable is set to 0x01, the advertising parameters' Own_-
 * Address_Type parameter is set to 0x02, the Controller's resolving list did not
 * contain a matching entry, and the device does not have a public address, the
 * Controller should return an error code which should be Invalid HCI Command
 * Parameters (0x12).
 *
 * If Advertising_Enable is set to 0x01, the advertising parameters' Own_-
 * Address_Type parameter is set to 0x03, the Controller's resolving list did not
 * contain a matching entry, and the random address for the device has not been
 * initialized using the HCI_LE_Set_Random_Address command, the Controller
 * shall return the error code Invalid HCI Command Parameters.(0x12).
 *
 * Enabling advertising when it is already enabled can cause the random address
 * to change. Disabling advertising when it is already disabled has no effect.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Advertising_Enable command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * If the Advertising_Type parameter is 0x01 (ADV_DIRECT_IND, high duty
 * cycle) and the directed advertising fails to create a connection, an
 * HCI_LE_Connection_Complete or HCI_LE_Enhanced_Connection_Complete
 * event shall be generated with the Status code set to Advertising Timeout
 * (0x3C).
 *
 * If the Advertising_Type parameter is 0x00 (ADV_IND), 0x01 (ADV_DIRECT_-
 * IND, high duty cycle), or 0x04 (ADV_DIRECT_IND, low duty cycle) and a
 * connection is created, an HCI_LE_Connection_Complete or HCI_LE_-
 * Enhanced_Connection_Complete event shall be generated.
 *
 * Note: There is a possible race condition if the Advertising_Enable parameter is
 * set to 0x00 (Disable) and the Advertising_Type parameter is 0x00, 0x01, or
 * 0x04. The advertisements might not be stopped before a connection is
 * created, and therefore both the HCI_Command_Complete event and either an
 * HCI_LE_Connection_Complete event or an HCI_LE_Enhanced_Connection_-
 * Complete event could be generated. This can also occur when high duty cycle
 * directed advertising is timed out and this command disables advertising.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_adv_enable(const sdc_hci_cmd_le_set_adv_enable_t * p_params);

/** @brief LE Set Scan Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.10
 *
 * The HCI_LE_Set_Scan_Parameters command is used to set the scan
 * parameters.
 *
 * The LE_Scan_Type parameter controls the type of scan to perform.
 *
 * The LE_Scan_Interval and LE_Scan_Window parameters are recommenda-
 * tions from the Host on how long (LE_Scan_Window) and how frequently
 * (LE_Scan_Interval) the Controller should scan (See [Vol 6] Part B, Section
 * 4.4.3). The LE_Scan_Window parameter shall always be set to a value smaller
 * or equal to the value set for the LE_Scan_Interval parameter. If they are set to
 * the same value scanning should be run continuously.
 *
 * Own_Address_Type parameter indicates the type of address being used in the
 * scan request packets.
 *
 * The Host shall not issue this command when scanning is enabled in the
 * Controller; if it is the Command Disallowed error code shall be used.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Scan_Parameters command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_scan_params(const sdc_hci_cmd_le_set_scan_params_t * p_params);

/** @brief LE Set Scan Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.11
 *
 * The HCI_LE_Set_Scan_Enable command is used to start and stop scanning
 * for legacy PDUs (but not extended PDUs, even if the device supports extended
 * advertising). Scanning is used to discover advertising devices nearby.
 *
 * The Filter_Duplicates parameter controls whether the Link Layer should filter
 * out duplicate advertising reports (Filtering_Enabled) to the Host, or if the Link
 * Layer should generate advertising reports for each packet received
 * (Filtering_Disabled). See [Vol 6] Part B, Section 4.4.3.5. If LE_Scan_Enable is
 * set to 0x00 then Filter_Duplicates shall be ignored.
 *
 * If LE_Scan_Enable is set to 0x01, the scanning parameters' Own_Address_-
 * Type parameter is set to 0x00 or 0x02, and the device does not have a public
 * address, the Controller should return an error code which should be Invalid HCI
 * Command Parameters (0x12).
 *
 * If LE_Scan_Enable is set to 0x01, the scanning parameters' Own_Address_-
 * Type parameter is set to 0x01 or 0x03, and the random address for the device
 * has not been initialized using the HCI_LE_Set_Random_Address command,
 * the Controller shall return the error code Invalid HCI Command Parameters
 * (0x12).
 *
 * If the LE_Scan_Enable parameter is set to 0x01 and scanning is already
 * enabled, any change to the Filter_Duplicates setting shall take effect.
 *
 * Disabling scanning when it is disabled has no effect.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Scan_Enable command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * Zero or more HCI_LE_Advertising_Report events are generated by the
 * Controller based on legacy advertising packets received and the duplicate
 * filtering. More than one advertising packet may be reported in each
 * HCI_LE_Advertising_Report event. No report shall be issued for extended
 * advertising PDUs.
 *
 * When the Scanning_Filter_Policy is set to 0x02 or 0x03 (see Section 7.8.10)
 * and a directed advertisement was received where the advertiser used a
 * resolvable private address which the Controller is unable to resolve, an
 * HCI_LE_Directed_Advertising_Report event shall be generated instead of an
 * HCI_LE_Advertising_Report event.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_scan_enable(const sdc_hci_cmd_le_set_scan_enable_t * p_params);

/** @brief LE Create Connection.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.12
 *
 * The HCI_LE_Create_Connection command is used to create an ACL
 * connection, with the local device in the Central role, to a connectable
 * advertiser.
 * If a connection is created with the local device in the Peripheral role while this
 * command is pending, then this command remains pending.
 *
 * The LE_Scan_Interval and LE_Scan_Window parameters are recommenda-
 * tions from the Host on how long (LE_Scan_Window) and how frequently
 * (LE_Scan_Interval) the Controller should scan. The LE_Scan_Window param-
 * eter shall be set to a value smaller or equal to the value set for the LE_Scan_-
 * Interval parameter. If both are set to the same value, scanning should run
 * continuously.
 * The Initiator_Filter_Policy is used to determine whether the Filter Accept List is
 * used. If the Filter Accept List is not used, the Peer_Address_Type and the
 * Peer_Address parameters specify the address type and address of the
 * advertising device to connect to.
 * Peer_Address_Type parameter indicates the type of address used in the
 * connectable advertisement sent by the peer. The Host shall not set
 * Peer_Address_Type to either 0x02 or 0x03 if both the Host and the Controller
 * support the HCI_LE_Set_Privacy_Mode command. If a Controller that
 * supports the HCI_LE_Set_Privacy_Mode command receives the
 * HCI_LE_Create_Connection command with Peer_Address_Type set to either
 * 0x02 or 0x03, it may use either device privacy mode or network privacy mode
 * for that peer device.
 * Peer_Address parameter indicates the Peer’s Public Device Address, Random
 * (static) Device Address, Non-Resolvable Private Address or Resolvable
 * Private Address depending on the Peer_Address_Type parameter.
 * Own_Address_Type parameter indicates the type of address being used in the
 * connection request packets.
 * The Connection_Interval_Min and Connection_Interval_Max parameters
 * define the minimum and maximum allowed connection interval. The
 * Connection_Interval_Min parameter shall not be greater than the
 * Connection_Interval_Max parameter.
 * The Max_Latency parameter defines the maximum allowed Peripheral latency
 * (see [Vol 6] Part B, Section 4.5.1).
 * The Supervision_Timeout parameter defines the link supervision timeout for
 * the connection. The Supervision_Timeout in milliseconds shall be larger than
 * (1 + Max_Latency) × Connection_Interval_Max × 2, where
 * Connection_Interval_Max is given in milliseconds. (See [Vol 6] Part B, Section
 * 4.5.2).
 * The Min_CE_Length and Max_CE_Length parameters provide the Controller
 * with the expected minimum and maximum length of the connection events.
 * The Min_CE_Length parameter shall be less than or equal to the
 * Max_CE_Length parameter. The Controller is not required to use these values.
 * If the Host issues this command when another HCI_LE_Create_Connection
 * command is pending in the Controller, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * If the Own_Address_Type parameter is set to 0x00 and the device does not
 * have a public address, the Controller should return an error code which should
 * be Invalid HCI Command Parameters (0x12).
 * If the Own_Address_Type parameter is set to 0x01 and the random address for
 * the device has not been initialized using the HCI_LE_Set_Random_Address
 * command, the Controller shall return the error code Invalid HCI Command
 * Parameters (0x12).
 * If the Own_Address_Type parameter is set to 0x02, the Initiator_Filter_Policy
 * parameter is set to 0x00, the Controller's resolving list did not contain a
 * matching entry, and the device does not have a public address, the Controller
 * should return an error code which should be Invalid HCI Command Parameters
 * (0x12).
 * If the Own_Address_Type parameter is set to 0x02, the Initiator_Filter_Policy
 * parameter is set to 0x01, and the device does not have a public address, the
 * Controller should return an error code which should be Invalid HCI Command
 * Parameters (0x12).
 * If the Own_Address_Type parameter is set to 0x03, the Initiator_Filter_Policy
 * parameter is set to 0x00, the Controller's resolving list did not contain a
 * matching entry, and the random address for the device has not been initialized
 * using the HCI_LE_Set_Random_Address command, the Controller shall return
 * the error code Invalid HCI Command Parameters (0x12).
 * If the Own_Address_Type parameter is set to 0x03, the Initiator_Filter_Policy
 * parameter is set to 0x01, and the random address for the device has not been
 * initialized using the HCI_LE_Set_Random_Address command, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Create_Connection command, the
 * Controller sends the HCI_Command_Status event to the Host. An HCI_LE_-
 * Connection_Complete or HCI_LE_Enhanced_Connection_Complete event
 * shall be generated when a connection is created because of this command or
 * the connection creation procedure is cancelled; until one of these events is
 * generated, the command is considered pending. If a connection is created and
 * the Controller supports the LE Channel Selection Algorithm #2 feature, this
 * event shall be immediately followed by an HCI_LE_Channel_Selection_-
 * Algorithm event.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_create_conn(const sdc_hci_cmd_le_create_conn_t * p_params);

/** @brief LE Create Connection Cancel.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.13
 *
 * The HCI_LE_Create_Connection_Cancel command is used to cancel the
 * HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection
 * commands.
 *
 * If no HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection
 * command is pending, then the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Create_Connection_Cancel command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * If the cancellation was successful then, after the HCI_Command_Complete
 * event for the HCI_LE_Create_Connection_Cancel command, either an
 * HCI_LE_Connection_Complete or an HCI_LE_Enhanced_Connection_-
 * Complete event shall be generated. In either case, the event shall be sent with
 * the error code Unknown Connection Identifier (0x02).
 *
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_create_conn_cancel(void);

/** @brief LE Read Filter Accept List Size.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.14
 *
 * The HCI_LE_Read_Filter_Accept_List_Size command is used to read the total
 * number of Filter Accept List entries that can be stored in the Controller.
 *
 * Note: The number of entries that can be stored is not fixed and the Controller
 * can change it at any time (e.g. because the memory used to store the Filter
 * Accept List can also be used for other purposes).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Filter_Accept_List_Size command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_filter_accept_list_size(sdc_hci_cmd_le_read_filter_accept_list_size_return_t * p_return);

/** @brief LE Clear Filter Accept List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.15
 *
 * The HCI_LE_Clear_Filter_Accept_List command is used to clear the Filter
 * Accept List stored in the Controller.
 *
 * This command shall not be used when:
 * • any advertising filter policy uses the Filter Accept List and advertising is
 *   enabled,
 * • the scanning filter policy uses the Filter Accept List and scanning is enabled,
 *   or
 * • the initiator filter policy uses the Filter Accept List and an
 *   HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection
 *   command is pending.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Clear_Filter_Accept_List command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_clear_filter_accept_list(void);

/** @brief LE Add Device To Filter Accept List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.16
 *
 * The HCI_LE_Add_Device_To_Filter_Accept_List command is used to add a
 * single device to the Filter Accept List stored in the Controller.
 *
 * This command shall not be used when:
 * • any advertising filter policy uses the Filter Accept List and advertising is
 *   enabled,
 * • the scanning filter policy uses the Filter Accept List and scanning is enabled,
 *   or
 * • the initiator filter policy uses the Filter Accept List and an
 *   HCI_LE_Create_Connection or HCI_LE_Extended_Create_Connection
 *   command is pending.
 *
 * When a Controller cannot add a device to the Filter Accept List because there
 * is no space available, it shall return the error code Memory Capacity Exceeded
 * (0x07).
 *
 * If the device is already in the Filter Accept List, the Controller should not add
 * the device to the Filter Accept List again and should return success.
 *
 * Address shall be ignored when Address_Type is set to 0xFF.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Add_Device_To_Filter_Accept_List command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_add_device_to_filter_accept_list(const sdc_hci_cmd_le_add_device_to_filter_accept_list_t * p_params);

/** @brief LE Remove Device From Filter Accept List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.17
 *
 * The HCI_LE_Remove_Device_From_Filter_Accept_List command is used to
 * remove a single device from the Filter Accept List stored in the Controller.
 * This command shall not be used when:
 * • any advertising filter policy uses the Filter Accept List and advertising is
 *   enabled,
 * • the scanning filter policy uses the Filter Accept List and scanning is enabled,
 *   or
 * • the initiator filter policy uses the Filter Accept List and an HCI_LE_-
 *   Create_Connection or HCI_LE_Extended_Create_Connection command is
 *   pending.
 *
 * Address shall be ignored when Address_Type is set to 0xFF.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Remove_Device_From_Filter_Accept_List command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_remove_device_from_filter_accept_list(const sdc_hci_cmd_le_remove_device_from_filter_accept_list_t * p_params);

/** @brief LE Connection Update.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.18
 *
 * The HCI_LE_Connection_Update command is used to change the ACL
 * connection parameters. This command may be issued on both the Central and
 * Peripheral.
 *
 * The Connection_Interval_Min and Connection_Interval_Max parameters are
 * used to define the minimum and maximum allowed connection interval. The
 * Connection_Interval_Min parameter shall not be greater than the
 * Connection_Interval_Max parameter.
 *
 * The Max_Latency parameter shall define the maximum allowed Peripheral
 * latency.
 *
 * The Supervision_Timeout parameter shall define the link supervision timeout
 * for the LE link. The Supervision_Timeout in milliseconds shall be larger than
 * (1 + Max_Latency) × Subrate_Factor × Connection_Interval_Max × 2, where
 * Connection_Interval_Max is given in milliseconds and Subrate_Factor is the
 * current subrate factor of the connection.
 *
 * The Min_CE_Length and Max_CE_Length are information parameters
 * providing the Controller with a hint about the expected minimum and maximum
 * length of the connection events. The Min_CE_Length shall be less than or
 * equal to the Max_CE_Length.
 *
 * The actual parameter values selected by the Link Layer may be different from
 * the parameter values provided by the Host through this command.
 *
 * If this command completes successfully and the connection interval has
 * changed, then the subrating factor shall be set to 1 and the continuation
 * number to 0. In this case, Max_Latency must be interpreted in underlying
 * connection events. Otherwise the subrating factor and continuation number
 * shall be unchanged and Max_Latency must be interpreted in subrated events.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Connection_Update command, the
 * Controller sends the HCI_Command_Status event to the Host. The HCI_LE_-
 * Connection_Update_Complete event shall be generated after the connection
 * parameters have been applied by the Controller or if the command
 * subsequently fails.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_conn_update(const sdc_hci_cmd_le_conn_update_t * p_params);

/** @brief LE Set Host Channel Classification.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.19
 *
 * The HCI_LE_Set_Host_Channel_Classification command allows the Host to
 * specify a channel classification for the data, secondary advertising, periodic,
 * and isochronous physical channels based on its “local information”. This
 * classification persists until overwritten with a subsequent
 * HCI_LE_Set_Host_Channel_Classification command or until the Controller is
 * reset using the HCI_Reset command (see [Vol 6] Part B, Section 4.5.8.1).
 *
 * If this command is used, the Host should send it within 10 seconds of knowing
 * that the channel classification has changed. The interval between two
 * successive commands sent shall be at least one second.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Host_Channel_Classification command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_host_channel_classification(const sdc_hci_cmd_le_set_host_channel_classification_t * p_params);

/** @brief LE Read Channel Map.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.20
 *
 * The HCI_LE_Read_Channel_Map command returns the current Channel_Map
 * for the specified Connection_Handle. The returned value indicates the state of
 * the Channel_Map specified by the last transmitted or received Channel_Map
 * (in a CONNECT_IND or LL_CHANNEL_MAP_IND message) for the specified
 * Connection_Handle, regardless of whether the Central has received an
 * acknowledgment. If the connection handle does not identify an ACL
 * connection, the Controller shall reject the command and should return the error
 * code Unknown Connection Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Channel_Map command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_channel_map(const sdc_hci_cmd_le_read_channel_map_t * p_params,
                                        sdc_hci_cmd_le_read_channel_map_return_t * p_return);

/** @brief LE Read Remote Features.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.21
 *
 * This command requests, from the remote device identified by the
 * Connection_Handle, the features used on the connection and the features
 * supported by the remote device. For details see [Vol 6] Part B, Section 4.6.
 *
 * This command may be issued on both the Central and Peripheral.
 *
 * Note: If a connection already exists between the two devices and the features
 * have already been fetched on that connection, the Controller may use a
 * cached copy of the features.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Read_Remote_Features command,
 * the Controller shall send the HCI_Command_Status event to the Host. When
 * the Controller has completed the procedure to determine the remote features
 * or has determined that it will be using a cached copy, the Controller shall send
 * an HCI_LE_Read_Remote_Features_Complete event to the Host.
 *
 * The HCI_LE_Read_Remote_Features_Complete event contains the status of
 * this command and the parameter describing the features used on the
 * connection and the features supported by the remote device.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_remote_features(const sdc_hci_cmd_le_read_remote_features_t * p_params);

/** @brief LE Encrypt.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.22
 *
 * The HCI_LE_Encrypt command is used to request the Controller to encrypt the
 * Plaintext_Data in the command using the Key given in the command and
 * returns the Encrypted_Data to the Host. The AES-128 bit block cypher is
 * defined in NIST Publication FIPS-197 (http://csrc.nist.gov/publications/fips/
 * fips197/fips-197.pdf).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Encrypt command has completed, an HCI_Command_-
 * Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_encrypt(const sdc_hci_cmd_le_encrypt_t * p_params,
                               sdc_hci_cmd_le_encrypt_return_t * p_return);

/** @brief LE Rand.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.23
 *
 * The HCI_LE_Rand command is used to request the Controller to generate 8
 * octets of random data to be sent to the Host. The Random_Number shall be
 * generated according to [Vol 2] Part H, Section 2 if the LE Feature (LE
 * Encryption) is supported.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Rand command has completed, an HCI_Command_-
 * Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_rand(sdc_hci_cmd_le_rand_return_t * p_return);

/** @brief LE Enable Encryption.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.24
 *
 * The HCI_LE_Enable_Encryption command is used to authenticate the given
 * encryption key associated with the remote device specified by the
 * Connection_Handle, and once authenticated will encrypt the connection. The
 * parameters are as defined in [Vol 3] Part H, Section 2.4.4.
 *
 * If the connection is already encrypted then the Controller shall pause
 * connection encryption before attempting to authenticate the given encryption
 * key, and then re-encrypt the connection. While encryption is paused no user
 * data shall be transmitted.
 *
 * If the Connection_Handle parameter identifies an ACL with an associated CIS
 * that has been created, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * On an authentication failure, the connection shall be automatically
 * disconnected by the Link Layer. If this command succeeds, then the
 * connection shall be encrypted.
 * This command shall only be used when the local device’s role is Central.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Enable_Encryption command it
 * shall send the HCI_Command_Status event to the Host. If the connection is not
 * encrypted when this command is issued, an HCI_Encryption_Change event
 * shall occur when encryption has been started for the connection. If the connec-
 * tion is encrypted when this command is issued, an HCI_Encryption_Key_-
 * Refresh_Complete event shall occur when encryption has been resumed.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_enable_encryption(const sdc_hci_cmd_le_enable_encryption_t * p_params);

/** @brief LE Long Term Key Request Reply.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.25
 *
 * The HCI_LE_Long_Term_Key_Request_Reply command is used to reply to an
 * HCI_LE_Long_Term_Key_Request event from the Controller, and specifies the
 * Long_Term_Key parameter that shall be used for this Connection_Handle. The
 * Long_Term_Key is used as defined in [Vol 6] Part B, Section 5.1.3.
 * This command shall only be used when the local device’s role is Peripheral.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Long_Term_Key_Request_Reply command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_long_term_key_request_reply(const sdc_hci_cmd_le_long_term_key_request_reply_t * p_params,
                                                   sdc_hci_cmd_le_long_term_key_request_reply_return_t * p_return);

/** @brief LE Long Term Key Request Negative Reply.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.26
 *
 * The HCI_LE_Long_Term_Key_Request_Negative_Reply command is used to
 * reply to an HCI_LE_Long_Term_Key_Request event from the Controller if the
 * Host cannot provide a Long Term Key for this Connection_Handle.
 *
 * This command shall only be used when the local device’s role is Peripheral.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Long_Term_Key_Request_Negative_Reply command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_long_term_key_request_negative_reply(const sdc_hci_cmd_le_long_term_key_request_negative_reply_t * p_params,
                                                            sdc_hci_cmd_le_long_term_key_request_negative_reply_return_t * p_return);

/** @brief LE Read Supported States.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.27
 *
 * The HCI_LE_Read_Supported_States command reads the states and state
 * combinations that the Link Layer supports. See [Vol 6] Part B, Section 1.1.1.
 *
 * LE_States is an 8-octet bit field. If a bit is set to 1 then this state or state
 * combination is supported by the Controller. Multiple bits in LE_States may be
 * set to 1 to indicate support for multiple state and state combinations.
 *
 * Note: This command only provides information about the supported states that
 * can be used with legacy advertising. It does not provide information about
 * states, and combinations of states, that can only be used with the extended
 * advertising commands (see Section 3.1.1).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Supported_States command has completed, an
 * HCI_Command_Complete event will be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_supported_states(sdc_hci_cmd_le_read_supported_states_return_t * p_return);

/** @brief LE Test End.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.30
 *
 * This command is used to stop any test which is in progress. The Num_Packets
 * for a transmitter test shall be reported as 0x0000. The Num_Packets is an
 * unsigned number and contains the number of received packets.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Test_End command has completed, an HCI_Command_-
 * Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_test_end(sdc_hci_cmd_le_test_end_return_t * p_return);

/** @brief LE Set Data Length.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.33
 *
 * The HCI_LE_Set_Data_Length command allows the Host to suggest the
 * maximum transmission payload size and maximum packet transmission time
 * (connMaxTxOctets and connMaxTxTime - see [Vol 6] Part B, Section 4.5.10) to
 * be used for LL Data PDUs on a given connection. The Controller may use
 * smaller or larger values based on local information.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Data_Length command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * If the command causes the maximum transmission packet size or maximum
 * packet transmission time to change, an HCI_LE_Data_Length_Change event
 * shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_data_length(const sdc_hci_cmd_le_set_data_length_t * p_params,
                                       sdc_hci_cmd_le_set_data_length_return_t * p_return);

/** @brief LE Read Suggested Default Data Length.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.34
 *
 * The HCI_LE_Read_Suggested_Default_Data_Length command allows the
 * Host to read the Host's suggested values (Suggested_Max_TX_Octets and
 * Suggested_Max_TX_Time) for the Controller's maximum transmitted number
 * of payload octets and maximum packet transmission time for packets
 * containing LL Data PDUs to be used for new connections (see [Vol 6] Part B,
 * Section 4.5.10).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Suggested_Default_Data_Length command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_suggested_default_data_length(sdc_hci_cmd_le_read_suggested_default_data_length_return_t * p_return);

/** @brief LE Write Suggested Default Data Length.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.35
 *
 * The HCI_LE_Write_Suggested_Default_Data_Length command allows the
 * Host to specify its suggested values for the Controller's maximum transmission
 * number of payload octets and maximum packet transmission time for packets
 * containing LL Data PDUs to be used for new connections. The Controller may
 * use smaller or larger values for connInitialMaxTxOctets and
 * connInitialMaxTxTime based on local information. (See [Vol 6] Part B, Section
 * 4.5.10).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Write_Suggested_Default_Data_Length command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_write_suggested_default_data_length(const sdc_hci_cmd_le_write_suggested_default_data_length_t * p_params);

/** @brief LE Add Device To Resolving List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.38
 *
 * The HCI_LE_Add_Device_To_Resolving_List command is used to add one
 * device to the resolving list used to generate and resolve Resolvable Private
 * Addresses in the Controller.
 *
 * This command shall not be used when address resolution is enabled in the
 * Controller and:
 * • Advertising (other than periodic advertising) is enabled,
 * • Scanning is enabled, or
 * • an HCI_LE_Create_Connection, HCI_LE_Extended_Create_Connection, or
 *   HCI_LE_Periodic_Advertising_Create_Sync command is pending.
 *
 * This command may be used at any time when address resolution is disabled in
 * the Controller.
 *
 * The added device shall be set to Network Privacy mode.
 *
 * If Peer_Identity_Address_Type is 0x01 and Peer_Identity_Address is not a
 * static address, then the Controller should return the error code Invalid HCI
 * Command Parameters (0x12).
 *
 * When a Controller cannot add a device to the list because there is no space
 * available, it shall return the error code Memory Capacity Exceeded (0x07).
 *
 * If an entry already exists in the resolving list with the same four parameter
 * values, the Controller shall either reject the command or not add the device to
 * the resolving list again and return success. If the command is rejected then the
 * error code Invalid HCI Command Parameters (0x12) should be used.
 *
 * If there is an existing entry in the resolving list with the same Peer_Identity_-
 * Address and Peer_Identity_Address_Type, or with the same non-zero
 * Peer_IRK, the Controller should return the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Add_Device_To_Resolving_List command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_add_device_to_resolving_list(const sdc_hci_cmd_le_add_device_to_resolving_list_t * p_params);

/** @brief LE Remove Device From Resolving List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.39
 *
 * The HCI_LE_Remove_Device_From_Resolving_List command is used to
 * remove one device from the resolving list used to resolve Resolvable Private
 * Addresses in the Controller.
 *
 * This command shall not be used when address resolution is enabled in the
 * Controller and:
 * • Advertising (other than periodic advertising) is enabled,
 * • Scanning is enabled, or
 * • an HCI_LE_Create_Connection, HCI_LE_Extended_Create_Connection, or
 *   HCI_LE_Periodic_Advertising_Create_Sync command is pending.
 *
 * This command may be used at any time when address resolution is disabled in
 * the Controller.
 *
 * When a Controller cannot remove a device from the resolving list because it is
 * not found, it shall return the error code Unknown Connection Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Remove_Device_From_Resolving_List command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_remove_device_from_resolving_list(const sdc_hci_cmd_le_remove_device_from_resolving_list_t * p_params);

/** @brief LE Clear Resolving List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.40
 *
 * The HCI_LE_Clear_Resolving_List command is used to remove all devices
 * from the resolving list used to resolve Resolvable Private Addresses in the
 * Controller.
 *
 * This command shall not be used when address resolution is enabled in the
 * Controller and:
 * • Advertising (other than periodic advertising) is enabled,
 * • Scanning is enabled, or
 * • an HCI_LE_Create_Connection, HCI_LE_Extended_Create_Connection, or
 *   HCI_LE_Periodic_Advertising_Create_Sync command is pending.
 *
 * This command may be used at any time when address resolution is disabled in
 * the Controller.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Clear_Resolving_List command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_clear_resolving_list(void);

/** @brief LE Read Resolving List Size.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.41
 *
 * The HCI_LE_Read_Resolving_List_Size command is used to read the total
 * number of entries in the resolving list that can be stored in the Controller.
 *
 * Note: The number of entries that can be stored is not fixed and the Controller
 * can change it at any time (e.g. because the memory used to store the list can
 * also be used for other purposes).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Resolving_List_Size command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_resolving_list_size(sdc_hci_cmd_le_read_resolving_list_size_return_t * p_return);

/** @brief LE Set Address Resolution Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.44
 *
 * The HCI_LE_Set_Address_Resolution_Enable command is used to enable
 * resolution of Resolvable Private Addresses in the Controller. This causes the
 * Controller to use the resolving list whenever the Controller receives a local or
 * peer Resolvable Private Address.
 *
 * This command shall not be used when:
 * • Advertising (other than periodic advertising) is enabled,
 * • Scanning is enabled, or
 * • an HCI_LE_Create_Connection, HCI_LE_Extended_Create_Connection, or
 *   HCI_LE_Periodic_Advertising_Create_Sync command is pending.
 *
 * Enabling address resolution when it is already enabled, or disabling it when it is
 * already disabled, has no effect.
 *
 * The requirements in [Vol 6] Part B, Section 6 related to the generation of
 * Resolvable Private Addresses and the privacy of the device are independent of
 * this command.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Address_Resolution_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_address_resolution_enable(const sdc_hci_cmd_le_set_address_resolution_enable_t * p_params);

/** @brief LE Set Resolvable Private Address Timeout.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.45
 *
 * The HCI_LE_Set_Resolvable_Private_Address_Timeout command set the
 * length of time the Controller uses a Resolvable Private Address before a new
 * resolvable private address is generated and starts being used.
 *
 * This timeout applies to all resolvable private addresses generated by the
 * Controller.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Resolvable_Private_Address_Timeout command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_resolvable_private_address_timeout(const sdc_hci_cmd_le_set_resolvable_private_address_timeout_t * p_params);

/** @brief LE Read Maximum Data Length.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.46
 *
 * The HCI_LE_Read_Maximum_Data_Length command allows the Host to read
 * the Controller’s maximum supported payload octets and packet duration times
 * for transmission and reception (Supported_Max_TX_Octets, Supported_Max_-
 * TX_Time, Supported_Max_RX_Octets, and Supported_Max_RX_Time, see
 * [Vol 6] Part B, Section 4.5.10).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Maximum_Data_Length command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_max_data_length(sdc_hci_cmd_le_read_max_data_length_return_t * p_return);

/** @brief LE Read PHY.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.47
 *
 * The HCI_LE_Read_PHY command is used to read the current transmitter PHY
 * and receiver PHY on the connection identified by the Connection_Handle.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_PHY command has completed, an HCI_Command_-
 * Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_phy(const sdc_hci_cmd_le_read_phy_t * p_params,
                                sdc_hci_cmd_le_read_phy_return_t * p_return);

/** @brief LE Set Default PHY.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.48
 *
 * The HCI_LE_Set_Default_PHY command allows the Host to specify its
 * preferred values for the transmitter PHY and receiver PHY to be used for all
 * subsequent connections over the LE transport.
 *
 * The All_PHYs parameter is a bit field that allows the Host to specify, for each
 * direction, whether it has no preference among the PHYs that the Controller
 * supports in a given direction or whether it has specified particular PHYs that it
 * prefers in the TX_PHYs or RX_PHYs parameter.
 *
 * The TX_PHYs parameter is a bit field that indicates the transmitter PHYs that
 * the Host prefers the Controller to use. If the All_PHYs parameter specifies that
 * the Host has no preference, the TX_PHYs parameter shall be ignored;
 * otherwise at least one bit shall be set to 1.
 *
 * The RX_PHYs parameter is a bit field that indicates the receiver PHYs that the
 * Host prefers the Controller to use. If the All_PHYs parameter specifies that the
 * Host has no preference, the RX_PHYs parameter shall be ignored; otherwise
 * at least one bit shall be set to 1.
 *
 * If the Host sets, in the TX_PHYs or RX_PHYs parameter, a bit for a PHY that
 * the Controller does not support, including a bit that is reserved for future use,
 * the Controller shall return the error code Unsupported Feature or Parameter
 * Value (0x11).
 *
 * If the Controller does not support asymmetric connections (see [Vol 6] Part B,
 * Section 4.6.9.1) and the Host sets All_PHYs to 0x00 and TX_PHYs to a
 * different value than RX_PHYs, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Default_PHY command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_default_phy(const sdc_hci_cmd_le_set_default_phy_t * p_params);

/** @brief LE Set PHY.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.49
 *
 * The HCI_LE_Set_PHY command is used to set the PHY preferences for the
 * connection identified by the Connection_Handle. The Controller might not be
 * able to make the change (e.g. because the peer does not support the
 * requested PHY) or may decide that the current PHY is preferable (e.g.,
 * because it could not schedule other activities if the requested PHY was used or
 * because it requires a PHY that supports Constant Tone Extensions).
 *
 * The All_PHYs parameter is a bit field that allows the Host to specify, for each
 * direction, whether it has no preference among the PHYs that the Controller
 * supports in a given direction or whether it has specified particular PHYs that it
 * prefers in the TX_PHYs or RX_PHYs parameter.
 *
 * The TX_PHYs parameter is a bit field that indicates the transmitter PHYs that
 * the Host prefers the Controller to use. If the All_PHYs parameter specifies that
 * the Host has no preference, the TX_PHYs parameter shall be ignored;
 * otherwise at least one bit shall be set to 1.
 *
 * The RX_PHYs parameter is a bit field that indicates the receiver PHYs that the
 * Host prefers the Controller to use. If the All_PHYs parameter specifies that the
 * Host has no preference, the RX_PHYs parameter shall be ignored; otherwise
 * at least one bit shall be set to 1.
 *
 * The Controller shall request a change unless it determines that this is
 * unnecessary or that the current PHY is preferable, in which case it may, but
 * need not, request a change.
 *
 * The PHY preferences provided by the HCI_LE_Set_PHY command override
 * those provided via the HCI_LE_Set_Default_PHY command (Section 7.8.48)
 * or any preferences previously set using the HCI_LE_Set_PHY command on
 * the same connection.
 *
 * The PHY_Options parameter is a bit field that allows the Host to specify
 * options for PHYs. The default value for a new connection shall be all zero bits.
 * The Controller may override any preferred coding for transmitting on the LE
 * Coded PHY.
 * The Host may specify a preferred coding even if it prefers not to use the LE
 * Coded transmitter PHY since the Controller may override the PHY preference.
 *
 * If the Host sets, in the TX_PHYs or RX_PHYs parameter, a bit for a PHY that
 * the Controller does not support, including a bit that is reserved for future use,
 * the Controller shall return the error code Unsupported Feature or Parameter
 * Value (0x11).
 *
 * If the Controller does not support asymmetric connections (see [Vol 6] Part B,
 * Section 4.6.9.1) and the Host sets All_PHYs to 0x00 and TX_PHYs to a
 * different value than RX_PHYs, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Set_PHY command, the Controller
 * shall send the HCI_Command_Status event to the Host. The HCI_LE_PHY_-
 * Update_Complete event shall be generated either when one or both PHY
 * changes or when the Controller determines that neither PHY will change
 * immediately.
 *
 * Note: If the peer negotiation resulted in no change to either PHY, this is not an
 * error and the HCI_LE_PHY_Update_Complete event will contain a status
 * indicating success.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_phy(const sdc_hci_cmd_le_set_phy_t * p_params);

/** @brief LE Set Advertising Set Random Address.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.52
 *
 * The HCI_LE_Set_Advertising_Set_Random_Address command is used by the
 * Host to set the random device address specified by the Random_Address
 * parameter. This address is used in the Controller (see [Vol 6] Part B, Section
 * 1.3.2) for the advertiser's address contained in the advertising PDUs for the
 * advertising set specified by the Advertising_Handle parameter.
 *
 * This command may be issued at any time after an advertising set identified by
 * the Advertising_Handle parameter has been created using the HCI_LE_Set_-
 * Extended_Advertising_Parameters command (see Section 7.8.53). However, if
 * the Host issues this command while the advertising set identified by the
 * Advertising_Handle parameter is using connectable advertising and is
 * enabled, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If this command is used to change the address, the new random address shall
 * take effect for advertising no later than the next successful HCI_LE_Set_-
 * Extended_Advertising_Enable command and for periodic advertising no later
 * than the next successful HCI_LE_Periodic_Advertising_Enable command.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Advertising_Set_Random_Address command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_adv_set_random_address(const sdc_hci_cmd_le_set_adv_set_random_address_t * p_params);

/** @brief LE Set Extended Advertising Parameters [v1].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.53
 *
 * The HCI_LE_Set_Extended_Advertising_Parameters command is used by the
 * Host to set the advertising parameters.
 * The Advertising_Handle parameter identifies the advertising set whose
 * parameters are being configured.
 *
 * The Advertising_Event_Properties parameter describes the type of advertising
 * event that is being configured and its basic properties. The type shall be one
 * supported by the Controller. In particular, the following restrictions apply to this
 * parameter:
 * • If legacy advertising PDU types are being used, then the parameter value
 *   shall be one of those specified in Table 7.2. If the advertising set already
 *   contains data, the type shall be one that supports advertising data and the
 *   amount of data shall not exceed 31 octets.
 *
 *                                                            Advertising
 *                                                            Event         Advertising
 *  Event Type                      PDU Type                  Properties    Data
 *
 *  Connectable and scannable       ADV_IND                   0b00010011    Supported
 *  undirected
 *
 *  Connectable directed            ADV_DIRECT_IND            0b00010101    Not allowed
 *  (low duty cycle)
 *
 *  Connectable directed            ADV_DIRECT_IND            0b00011101    Not allowed
 *  (high duty cycle)
 *
 *  Scannable undirected            ADV_SCAN_IND              0b00010010    Supported
 *
 *  Non-connectable and non-        ADV_NONCONN_IND           0b00010000    Supported
 *  scannable undirected
 *
 * Table 7.2: Advertising_Event_Properties values for legacy PDUs
 *
 * • If extended advertising PDU types are being used (bit 4 = 0) then:
 *           The advertisement shall not be both connectable and scannable.
 *           High duty cycle directed connectable advertising (≤ 3.75 ms
 *           advertising interval) shall not be used (bit 3 = 0).
 *
 * If the Advertising_Event_Properties parameter does not describe an event type
 * supported by the Controller, contains an invalid bit combination, or specifies a
 * type that does not support advertising data when the advertising set already
 * contains some, the Controller shall return the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * The parameters beginning with “Secondary” are only valid when extended
 * advertising PDU types are being used (bit 4 = 0).
 *
 * The Own_Address_Type parameter shall be ignored for undirected
 * anonymous advertising (bit 2 = 0 and bit 5 = 1).
 *
 * If Directed advertising is selected, the Peer_Address_Type and Peer_Address
 * shall be valid and the Advertising_Filter_Policy parameter shall be ignored.
 * The Primary_Advertising_Interval_Min parameter shall be less than or equal to
 * the Primary_Advertising_Interval_Max parameter. The Primary_Advertising_-
 * Interval_Min and Primary_Advertising_Interval_Max parameters should not be
 * the same value so that the Controller can choose the best advertising interval
 * given other activities.
 *
 * For high duty cycle connectable directed advertising event type
 * (ADV_DIRECT_IND), the Primary_Advertising_Interval_Min and
 * Primary_Advertising_Interval_Max parameters are not used and shall be
 * ignored.
 *
 * If the primary advertising interval range provided by the Host
 * (Primary_Advertising_Interval_Min, Primary_Advertising_Interval_Max) does
 * not overlap with the advertising interval range supported by the Controller, then
 * the Controller shall return the error code Unsupported Feature or Parameter
 * Value (0x11).
 *
 * The Primary_Advertising_Channel_Map is a bit field that indicates the advertis-
 * ing channel indices that shall be used when transmitting advertising packets. At
 * least one channel bit shall be set in the Primary_Advertising_Channel_Map
 * parameter.
 *
 * The Own_Address_Type parameter specifies the type of address being used in
 * the advertising packets. For random addresses, the address is specified by the
 * HCI_LE_Set_Advertising_Set_Random_Address command.
 *
 * If Own_Address_Type equals 0x02 or 0x03, the Peer_Address parameter
 * contains the peer’s Identity Address and the Peer_Address_Type parameter
 * contains the peer’s Identity Type (i.e., 0x00 or 0x01). These parameters are
 * used to locate the corresponding local IRK in the resolving list; this IRK is used
 * to generate their own address used in the advertisement.
 *
 * The Advertising_TX_Power parameter indicates the maximum power level at
 * which the advertising packets are to be transmitted on the advertising physical
 * channels. The Controller shall choose a power level lower than or equal to the
 * one specified by the Host.
 *
 * The Primary_Advertising_PHY parameter indicates the PHY on which the
 * advertising packets are transmitted on the primary advertising physical chan-
 * nel. If legacy advertising PDUs are being used, the Primary_Advertising_PHY
 * shall indicate the LE 1M PHY. The Secondary_Advertising_PHY parameter
 * indicates the PHY on which the advertising packets are be transmitted on the
 * secondary advertising physical channel. If the Host specifies a PHY that is not
 * supported by the Controller, including a value that is reserved for future use, it
 * should return the error code Unsupported Feature or Parameter Value (0x11).
 * If Constant Tone Extensions are enabled for the advertising set and
 * Secondary_Advertising_PHY specifies a PHY that does not allow Constant
 * Tone Extensions, the Controller shall return the error code Command Disal-
 * lowed (0x0C).
 * If the Primary_Advertising_PHY indicates the LE Coded PHY, then the
 * Primary_Advertising_PHY_Options shall indicate the Host's preference or
 * requirement concerning coding scheme. Otherwise, Primary_Advertising_-
 * PHY_Options shall be ignored. If the Secondary_Advertising_PHY indicates
 * the LE Coded PHY, then the Secondary_Advertising_PHY_Options shall indi-
 * cate the Host's preference or requirement concerning coding scheme (includ-
 * ing for periodic advertising). Otherwise, Secondary_Advertising_PHY_Options
 * shall be ignored. If the Host specifies that it requires a specific coding (i.e.,
 * value 0x03 or 0x04) in the Primary_Advertising_PHY_Options or
 * Secondary_Advertising_PHY_Options and the Controller supports the LE Fea-
 * ture (Advertising Coding Selection) but is currently unable to provide all the
 * required settings, then the Controller shall return the error code Command Dis-
 * allowed (0x0C).
 *
 * The Secondary_Advertising_Max_Skip parameter is the maximum number of
 * advertising events that can be skipped before the AUX_ADV_IND can be sent.
 *
 * The Advertising_SID parameter specifies the value to be transmitted in the
 * Advertising SID subfield of the ADI field of the Extended Header of those
 * advertising physical channel PDUs that have an ADI field. If the advertising set
 * only uses PDUs that do not contain an ADI field, Advertising_SID shall be
 * ignored.
 *
 * The Scan_Request_Notification_Enable parameter indicates whether the
 * Controller shall send notifications upon the receipt of a scan request PDU that
 * is in response to an advertisement from the specified advertising set that
 * contains its device address and is from a scanner that is allowed by the
 * advertising filter policy.
 *
 * The Controller shall set the Selected_TX_Power return parameter to the
 * transmit power that it will use for transmitting the advertising packets for the
 * specified advertising set. The Controller shall only change this value if
 * requested by the Host. If the radiated power level will vary between packets
 * (e.g., because of frequency-dependent properties of the transmitter) then the
 * value should be the best estimate of the maximum power used.
 *
 * If the Host issues this command when advertising is enabled for the specified
 * advertising set, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If the Host issues this command when periodic advertising is enabled for the
 * specified advertising set and connectable, scannable, legacy, or anonymous
 * advertising is specified, the Controller shall return the error code Invalid HCI
 * Command Parameters (0x12).
 *
 * If periodic advertising is enabled for the advertising set and the
 * Secondary_Advertising_PHY parameter does not specify the PHY currently
 * being used for the periodic advertising, the Controller shall return the error
 * code Command Disallowed (0x0C).
 * If the Advertising_Handle does not identify an existing advertising set and the
 * Controller is unable to support a new advertising set at present, the Controller
 * shall return the error code Memory Capacity Exceeded (0x07).
 *
 * If the advertising set already contains advertising data or scan response data,
 * extended advertising is being used, and the length of the data is greater than
 * the maximum that the Controller can transmit within the longest possible
 * auxiliary advertising segment consistent with the parameters, the Controller
 * shall return the error code Packet Too Long (0x45). If advertising on the LE
 * Coded PHY, the S=8 coding shall be assumed unless the current advertising
 * parameters require the use of S=2 for an advertising physical channel, in which
 * case the S=2 coding shall be assumed for that advertising physical channel.
 *
 * If the Controller does not support the LE Feature (Advertising Coding
 * Selection) and the Host does not set both Primary_Advertising_PHY_Options
 * and Secondary_Advertising_PHY_Options to zero, the Controller shall return
 * the error code Unsupported Feature or Parameter Value (0x11).
 *
 * Missing parameters:
 *
 * When a version of this command is issued that does not include all the
 * parameters, the following values shall be used:
 *
 *  Parameter                                        Value
 *
 *  Primary_Advertising_PHY_Options                 0x00
 *
 *  Secondary_Advertising_PHY_Options               0x00
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Extended_Advertising_Parameters command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_ext_adv_params(const sdc_hci_cmd_le_set_ext_adv_params_t * p_params,
                                          sdc_hci_cmd_le_set_ext_adv_params_return_t * p_return);

/** @brief LE Set Extended Advertising Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.54
 *
 * The HCI_LE_Set_Extended_Advertising_Data command is used to set the
 * data used in advertising PDUs that have a data field. This command may be
 * issued at any time after an advertising set identified by the Advertising_Handle
 * parameter has been created using the HCI_LE_Set_Extended_Advertising_-
 * Parameters command (see Section 7.8.53), regardless of whether advertising
 * in that set is enabled or disabled.
 *
 * If advertising is currently enabled for the specified advertising set, the
 * Controller shall use the new data in subsequent extended advertising events
 * for this advertising set. If an extended advertising event is in progress when
 * this command is issued, the Controller may use the old or new data for that
 * event.
 *
 * If advertising is currently disabled for the specified advertising set, the data
 * shall be kept by the Controller and used once advertising is enabled for that
 * set. The data shall be discarded when the advertising set is removed.
 *
 * Only the significant part of the advertising data should be transmitted in the
 * advertising packets as defined in [Vol 3] Part C, Section 11.
 *
 * The Host may set the advertising data in one or more operations using the
 * Operation parameter in the command. If the combined length of the data
 * exceeds the capacity of the advertising set identified by the Advertising_-
 * Handle parameter (see Section 7.8.57 LE Read Maximum Advertising Data
 * Length command) or the amount of memory currently available, all the data
 * shall be discarded and the Controller shall return the error code Memory
 * Capacity Exceeded (0x07).
 *
 * If the advertising set uses extended advertising and the combined length of the
 * data is greater than the maximum that the Controller can transmit within the
 * longest possible auxiliary advertising segment consistent with the current
 * parameters of the advertising set (using the current advertising interval if
 * advertising is enabled), all the data shall be discarded and the Controller shall
 * return the error code Packet Too Long (0x45). If advertising on the LE Coded
 * PHY, the S=8 coding shall be assumed unless the current advertising
 * parameters require the use of S=2 for an advertising physical channel, in which
 * case the S=2 coding shall be assumed for that advertising physical channel.
 *
 * If Operation indicates the start of new data (values 0x01 or 0x03), then any
 * existing partial or complete advertising data shall be discarded.
 *
 * If the advertising data is discarded by the command or the combined length of
 * the data after the command completes is zero, the advertising set will have no
 * advertising data.
 *
 * If Operation is 0x04, the behavior is the same as if the current advertising data
 * had been sent again; this can be used to cause the Advertising DID value to be
 * updated (see [Vol 6] Part B, Section 4.4.2.11).
 *
 * The Fragment_Preference parameter provides a hint to the Controller as to
 * whether advertising data should be fragmented.
 *
 * If the advertising set specifies a type that does not support advertising data, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the advertising set uses legacy advertising PDUs that support advertising
 * data and either Operation is not 0x03 or the Advertising_Data_Length
 * parameter exceeds 31 octets, the Controller shall return the error code Invalid
 * HCI Command Parameters (0x12).
 *
 * If Operation is 0x04 and:
 * • advertising is currently disabled for the advertising set;
 * • the advertising set contains no data;
 * • the advertising set uses legacy PDUs; or
 * • Advertising_Data_Length is not zero;
 *
 * then the Controller shall return the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * If Operation is not 0x03 or 0x04 and Advertising_Data_Length is zero, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If advertising is currently enabled for the specified advertising set and
 * Operation does not have the value 0x03 or 0x04, the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, then the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Extended_Advertising_Data command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_ext_adv_data(const sdc_hci_cmd_le_set_ext_adv_data_t * p_params);

/** @brief LE Set Extended Scan Response Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.55
 *
 * The HCI_LE_Set_Extended_Scan_Response_Data command is used to
 * provide scan response data used in scanning response PDUs. This command
 * may be issued at any time after the advertising set identified by the
 * Advertising_Handle parameter has been created using the HCI_LE_Set_-
 * Extended_Advertising_Parameters command (see Section 7.8.53) regardless
 * of whether advertising in that set is enabled or disabled.
 *
 * If advertising is currently enabled for the specified advertising set, the
 * Controller shall use the new data in subsequent extended advertising events
 * for this advertising set. If an extended advertising event is in progress when
 * this command is issued, the Controller may use the old or new data for that
 * event.
 *
 * If advertising is currently disabled for the specified advertising set, the data
 * shall be kept by the Controller and used once advertising is enabled for that
 * set. The data shall be discarded when the advertising set is removed.
 *
 * Only the significant part of the scan response data should be transmitted in the
 * advertising packets as defined in [Vol 3] Part C, Section 11.
 *
 * The Host may set the scan response data in one or more operations using the
 * Operation parameter in the command. If the combined length of the data
 * exceeds the capacity of the advertising set identified by the
 * Advertising_Handle parameter (see Section 7.8.57 LE Read Maximum
 * Advertising Data Length command) or the amount of memory currently
 * available, all the data shall be discarded and the Controller shall return the
 * error code Memory Capacity Exceeded (0x07).
 *
 * If Operation indicates the start of new data (values 0x01 or 0x03), then any
 * existing partial or complete scan response data shall be discarded.
 *
 * If the scan response data is discarded by the command or the combined length
 * of the data after the command completes is zero, the advertising set will have
 * no scan response data.
 * The Fragment_Preference parameter provides a hint to the Controller as to
 * whether advertising data should be fragmented.
 * If the advertising set is non-scannable and the Host uses this command other
 * than to discard existing data, the Controller shall return the error code Invalid
 * HCI Command Parameters (0x12). If the advertising set uses scannable leg-
 * acy advertising PDUs and either Operation is not 0x03 or the Scan_-
 * Response_Data_Length parameter exceeds 31 octets, the Controller shall
 * return the error code Invalid HCI Command Parameters (0x12).
 *
 * If Operation is not 0x03 and Scan_Response_Data_Length is zero, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If advertising is currently enabled for the specified advertising set and
 * Operation does not have the value 0x03, the Controller shall return the error
 * code Command Disallowed (0x0C).
 *
 * If the advertising set uses extended advertising and the combined length of the
 * data is greater than the maximum that the Controller can transmit within the
 * longest possible auxiliary advertising segment consistent with the current
 * parameters of the advertising set (using the current advertising interval if
 * advertising is enabled), all the data shall be discarded and the Controller shall
 * return the error code Packet Too Long (0x45). If advertising on the LE Coded
 * PHY, the S=8 coding shall be assumed unless the current advertising
 * parameters require the use of S=2 for an advertising physical channel, in which
 * case the S=2 coding shall be assumed for that advertising physical channel.
 *
 * If the advertising set uses scannable extended advertising PDUs, advertising is
 * currently enabled for the specified advertising set, and Scan_Response_-
 * Data_Length is zero, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, then the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Extended_Scan_Response_Data command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_ext_scan_response_data(const sdc_hci_cmd_le_set_ext_scan_response_data_t * p_params);

/** @brief LE Set Extended Advertising Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.56
 *
 * The HCI_LE_Set_Extended_Advertising_Enable command is used to request
 * the Controller to enable or disable one or more advertising sets using the
 * advertising sets identified by the Advertising_Handle[i] parameter. The Control-
 * ler manages the timing of advertisements in accordance with the advertising
 * parameters given in the HCI_LE_Set_Extended_Advertising_Parameters
 * command. The Num_Sets parameter is the number of advertising sets
 * contained in the parameter arrays. If Enable and Num_Sets are both set to
 * 0x00, then all advertising sets are disabled.
 *
 * The Controller shall only start an advertising event when the corresponding
 * advertising set is enabled. The Controller shall continue advertising until all
 * advertising sets have been disabled. An advertising set shall be disabled when
 * the Host issues an HCI_LE_Set_Extended_Advertising_Enable command with
 * the Enable parameter set to 0x00 (Advertising is disabled), a connection is cre-
 * ated using that advertising set, the duration specified in the Duration[i] parame-
 * ter expires, or the number of extended advertising events transmitted for the
 * set exceeds the Max_Extended_Advertising_Events[i] parameter.
 *
 * The Duration[i] parameter indicates the duration for which that advertising set
 * is enabled. The duration begins at the start of the first advertising event of this
 * advertising set. The Controller should not start an extended advertising event
 * that it cannot complete within the duration.
 *
 * If the advertising is high duty cycle connectable directed advertising, then
 * Duration[i] shall be less than or equal to 1.28 seconds and shall not be equal to 0.
 *
 * The Max_Extended_Advertising_Events[i] parameter, if non-zero, indicates the
 * maximum number of extended advertising events that shall be sent prior to
 * disabling the extended advertising set even if the Duration[i] parameter has not
 * expired.
 *
 * Duration[i] and Max_Extended_Advertising_Events[i] shall be ignored when
 * Enable is set to 0x00.
 *
 * If the HCI_LE_Set_Extended_Advertising_Enable command is sent again for
 * an advertising set while that set is enabled, the timer used for the duration and
 * the number of events counter are reset and any change to the random address
 * shall take effect.
 *
 * Disabling the advertising set identified by the Advertising_Handle[i] parameter
 * does not disable any periodic advertising associated with that set.
 *
 * Disabling an advertising set that is already disabled has no effect.
 *
 * If the same advertising set is identified by more than one entry in the
 * Advertising_Handle[i] arrayed parameter, then the Controller shall return the
 * error code Invalid HCI Command Parameters (0x12).
 *
 * If the advertising set corresponding to the Advertising_Handle[i] parameter
 * does not exist, then the Controller shall return the error code Unknown
 * Advertising Identifier (0x42).
 *
 * The remainder of this section only applies if Enable is set to 0x01.
 *
 * If Num_Sets is set to 0x00, the Controller shall return the error code Invalid HCI
 * Command Parameters (0x12).
 *
 * If the advertising set contains partial advertising data or partial scan response
 * data, the Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the advertising set uses scannable extended advertising PDUs and no scan
 * response data is currently provided, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * If the advertising set uses connectable extended advertising PDUs and the
 * advertising data in the advertising set will not fit in the AUX_ADV_IND PDU,
 * the Controller shall return the error code Invalid HCI Command Parameters
 * (0x12).
 *
 * Note: The maximum amount of data that will fit in the PDU depends on which
 * options are selected and on the maximum length of PDU that the Controller is
 * able to transmit.
 *
 * If extended advertising is being used and the length of any advertising data or
 * of any scan response data is greater than the maximum that the Controller can
 * transmit within the longest possible auxiliary advertising segment consistent
 * with the chosen advertising interval, the Controller shall return the error code
 * Packet Too Long (0x45). If advertising on the LE Coded PHY, the S=8 coding
 * shall be assumed unless the current advertising parameters require the use of
 * S=2 for an advertising physical channel, in which case the S=2 coding shall be
 * assumed for that advertising physical channel.
 *
 * If the advertising set's Own_Address_Type parameter is set to 0x00 and the
 * device does not have a public address, the Controller should return an error
 * code which should be Invalid HCI Command Parameters (0x12).
 * If the advertising set's Own_Address_Type parameter is set to 0x01 and the
 * random address for the advertising set has not been initialized using the
 * HCI_LE_Set_Advertising_Set_Random_Address command, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the advertising set's Own_Address_Type parameter is set to 0x02, the
 * Controller's resolving list did not contain a matching entry, and the device does
 * not have a public address, the Controller should return an error code which
 * should be Invalid HCI Command Parameters (0x12).
 *
 * If the advertising set's Own_Address_Type parameter is set to 0x03, the
 * Controller's resolving list did not contain a matching entry, and the random
 * address for the advertising set has not been initialized using the
 * HCI_LE_Set_Advertising_Set_Random_Address command, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Extended_Advertising_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * If the Duration[i] parameter is set to a value other than 0x0000, an HCI_LE_-
 * Advertising_Set_Terminated event shall be generated when the duration spec-
 * ified in the Duration[i] parameter expires. However, if the advertising set is for
 * high duty cycle connectable directed advertising and no connection is created
 * before the duration expires, an HCI_LE_Connection_Complete or HCI_LE_-
 * Enhanced_Connection_Complete event with the Status parameter set to the
 * error code Advertising Timeout (0x3C) may be generated instead of or in addi-
 * tion to the HCI_LE_Advertising_Set_Terminated event. If the Controller gener-
 * ates both events, they may be in either order.
 *
 * If the Max_Extended_Advertising_Events[i] parameter is set to a value other
 * than 0x00, an HCI_LE_Advertising_Set_Terminated event shall be generated
 * when the maximum number of extended advertising events has been
 * transmitted by the Controller.
 * If the advertising set is connectable and a connection gets created, an
 * HCI_LE_Connection_Complete or HCI_LE_Enhanced_Connection_Complete
 * event shall be generated followed by an HCI_LE_Advertising_Set_Terminated
 * event with the Status parameter set to 0x00. The Controller should not send
 * any other events in between these two events. If the Controller supports the LE
 * Channel Selection Algorithm #2 feature, then the HCI_LE_Advertising_Set_-
 * Terminated event may be immediately preceded or followed by an HCI_LE_-
 * Channel_Selection_Algorithm event.
 *
 * Note: If this command is used to disable advertising at about the same time
 * that a connection is established or the advertising duration expires, there is a
 * possible race condition in that it is possible to receive both an HCI_LE_-
 * Connection_Complete, HCI_LE_Enhanced_Connection_Complete, or
 * HCI_LE_Advertising_Set_Terminated event and the HCI_Command_-
 * Complete event for this command.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_ext_adv_enable(const sdc_hci_cmd_le_set_ext_adv_enable_t * p_params);

/** @brief LE Read Maximum Advertising Data Length.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.57
 *
 * The HCI_LE_Read_Maximum_Advertising_Data_Length command is used to
 * read the maximum length of data supported by the Controller for use as
 * advertisement data or scan response data in an advertising event or as
 * periodic advertisement data.
 *
 * Note: The maximum amount may be fragmented across multiple PDUs (see
 * [Vol 6] Part B, Section 2.3.4.9).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Maximum_Advertising_Data_Length command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_max_adv_data_length(sdc_hci_cmd_le_read_max_adv_data_length_return_t * p_return);

/** @brief LE Read Number of Supported Advertising Sets.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.58
 *
 * The HCI_LE_Read_Number_of_Supported_Advertising_Sets command is
 * used to read the maximum number of advertising sets supported by the
 * advertising Controller at the same time.
 *
 * Note: The number of advertising sets that can be supported is not fixed and the
 * Controller can change it at any time because the memory used to store
 * advertising sets can also be used for other purposes.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Number_of_Supported_Advertising_Sets command
 * has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_number_of_supported_adv_sets(sdc_hci_cmd_le_read_number_of_supported_adv_sets_return_t * p_return);

/** @brief LE Remove Advertising Set.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.59
 *
 * The HCI_LE_Remove_Advertising_Set command is used to remove an
 * advertising set from the Controller.
 *
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, then the Controller shall return the error code Unknown Advertising
 * Identifier (0x42). If advertising or periodic advertising on the advertising set is
 * enabled, then the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Remove_Advertising_Set command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_remove_adv_set(const sdc_hci_cmd_le_remove_adv_set_t * p_params);

/** @brief LE Clear Advertising Sets.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.60
 *
 * The HCI_LE_Clear_Advertising_Sets command is used to remove all existing
 * advertising sets from the Controller.
 *
 * If advertising or periodic advertising is enabled on any advertising set, then the
 * Controller shall return the error code Command Disallowed (0x0C).
 *
 * Note: All advertising sets are cleared on HCI reset.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Clear_Advertising_Sets command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_clear_adv_sets(void);

/** @brief LE Set Periodic Advertising Parameters [v1].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.61
 *
 * The HCI_LE_Set_Periodic_Advertising_Parameters command is used by the
 * Host to set the parameters for periodic advertising.
 *
 * The Advertising_Handle parameter identifies the advertising set whose
 * periodic advertising parameters are being configured. If the corresponding
 * advertising set does not already exist, then the Controller shall return the error
 * code Unknown Advertising Identifier (0x42).
 *
 * The Periodic_Advertising_Interval_Min parameter shall be less than or equal to
 * the Periodic_Advertising_Interval_Max parameter. The Periodic_Advertising_-
 * Interval_Min and Periodic_Advertising_Interval_Max parameters should not be
 * the same value to enable the Controller to determine the best advertising
 * interval given other activities.
 *
 * If the periodic advertising interval range provided by the Host
 * (Periodic_Advertising_Interval_Min, Periodic_Advertising_Interval_Max) does
 * not overlap with the periodic advertising interval range supported by the
 * Controller, then the Controller shall return an error which should use the error
 * code Unsupported Feature or Parameter Value (0x11).
 *
 * The Periodic_Advertising_Properties parameter indicates which fields should
 * be included in the advertising packet.
 *
 * The Num_Subevents parameter identifies the number of subevents that shall
 * be transmitted for each periodic advertising event. If the Num_Subevents
 * parameter value is 0x00, then the Subevent_Interval, Response_Slot_Delay,
 * Response_Slot_Spacing, and Num_Response_Slots parameters shall be
 * ignored.
 *
 * The Subevent_Interval parameter identifies the time between the subevents of
 * PAwR. The Subevent_Interval shall be less than or equal to the
 * Periodic_Advertising_Interval_Min divided by the Num_Subevents of the
 * advertising set.
 *
 * The Response_Slot_Delay parameter identifies the time between the start of
 * the advertising packet at the start of a subevent and the start of the first
 * response slot. The Response_Slot_Delay shall be less than the
 * Subevent_Interval.
 *
 * The Response_Slot_Spacing parameter identifies the time between the start of
 * two consecutive response slots. The Response_Slot_Spacing shall be less
 * than or equal to 10 × (Subevent_Interval - Response_Slot_Delay) /
 * Num_Response_Slots. If the Num_Response_Slots parameter is set to 1, then
 * the Controller shall ignore the Response_Slot_Spacing parameter.
 *
 * The Num_Response_Slots parameter identifies the number of response slots
 * in a subevent. If the Num_Response_Slots parameter value is 0x00, then the
 * Response_Slot_Delay and Response_Slot_Spacing parameters shall be
 * ignored.
 *
 * If the advertising set identified by the Advertising_Handle specified scannable,
 * connectable, legacy, or anonymous advertising, the Controller shall return the
 * error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command when periodic advertising is enabled for the
 * specified advertising set, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * If the Advertising_Handle does not identify an advertising set that is already
 * configured for periodic advertising and the Controller is unable to support more
 * periodic advertising at present, the Controller shall return the error code
 * Memory Capacity Exceeded (0x07).
 *
 * If the advertising set already contains periodic advertising data and the length
 * of the data is greater than the maximum that the Controller can transmit within
 * a periodic advertising interval of Periodic_Advertising_Interval_Max, the
 * Controller shall return the error code Packet Too Long (0x45). If advertising on
 * the LE Coded PHY, the S=8 coding shall be assumed unless the current
 * advertising parameters require the use of S=2 for an advertising physical
 * channel, in which case the S=2 coding shall be assumed for that advertising
 * physical channel.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Parameters command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_params(const sdc_hci_cmd_le_set_periodic_adv_params_t * p_params);

/** @brief LE Set Periodic Advertising Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.62
 *
 * The HCI_LE_Set_Periodic_Advertising_Data command is used to set the data
 * used in periodic advertising PDUs. This command may be issued at any time
 * after the advertising set identified by the Advertising_Handle parameter has
 * been configured for periodic advertising using the HCI_LE_Set_Periodic_-
 * Advertising_Parameters command (see Section 7.8.61), regardless of whether
 * periodic advertising in that set is enabled or disabled. If the advertising set has
 * not been configured for periodic advertising, then the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * If periodic advertising is currently enabled for the specified advertising set, the
 * Controller shall use the new data in subsequent periodic advertising events for
 * this advertising set. If a periodic advertising event is in progress when this
 * command is issued, the Controller may use the old or new data for that event.
 *
 * If periodic advertising is currently disabled for the specified advertising set, the
 * data shall be kept by the Controller and used once periodic advertising is
 * enabled for that set. The data shall be discarded when the advertising set is
 * removed.
 *
 * Only the significant part of the periodic advertising data should be transmitted
 * in the advertising packets as defined in [Vol 3] Part C, Section 11.
 *
 * The Host may set the periodic advertising data in one or more operations using
 * the Operation parameter in the command. If the combined length of the data
 * exceeds the capacity of the advertising set identified by the Advertising_-
 * Handle parameter (see Section 7.8.57 LE Read Maximum Advertising Data
 * Length command) or the amount of memory currently available, all the data
 * shall be discarded and the Controller shall return the error code Memory
 * Capacity Exceeded (0x07).
 *
 * If the combined length of the data is greater than the maximum that the Con-
 * troller can transmit within the current periodic advertising interval (if periodic
 * advertising is currently enabled) or the Periodic_Advertising_Interval_Max for
 * the advertising set (if currently disabled), all the data shall be discarded and the
 * Controller shall return the error code Packet Too Long (0x45). If advertising on
 * the LE Coded PHY, the S=8 coding shall be assumed unless the current adver-
 * tising parameters require the use of S=2 for an advertising physical channel, in
 * which case the S=2 coding shall be assumed for that advertising physical
 * channel.
 *
 * If Operation indicates the start of new data (values 0x01 or 0x03), then any
 * existing partial or complete data shall be discarded.
 *
 * If Operation is 0x04, then the behavior is the same as if the current periodic
 * advertising data had been sent again; this can be used to cause the
 * Advertising DID value to be updated (see [Vol 6] Part B, Section 4.4.2.11).
 *
 * If Operation is 0x04 and:
 * • periodic advertising is currently disabled for the advertising set;
 * • the periodic advertising set contains no data; or
 * • Advertising_Data_Length is not zero;
 *
 * then the Controller shall return the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * If Operation is not 0x03 or 0x04 and Advertising_Data_Length is zero, then the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the periodic advertising data is discarded by the command or the combined
 * length of the data after the command completes is zero, the advertising set will
 * have no periodic advertising data.
 *
 * If periodic advertising is currently enabled for the specified advertising set and
 * Operation does not have the value 0x03 or 0x04, then the Controller shall
 * return the error code Command Disallowed (0x0C).
 *
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, then the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Data command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_data(const sdc_hci_cmd_le_set_periodic_adv_data_t * p_params);

/** @brief LE Set Periodic Advertising Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.63
 *
 * The HCI_LE_Set_Periodic_Advertising_Enable command is used to request
 * the Controller to enable or disable the periodic advertising for the advertising
 * set specified by the Advertising_Handle parameter (ordinary advertising is not
 * affected).
 *
 * If the advertising set is not currently enabled (see the HCI_LE_Set_Extended_-
 * Advertising_Enable command), the periodic advertising is not started until the
 * advertising set is enabled. Once the advertising set has been enabled, the
 * Controller shall continue periodic advertising until the Host issues an HCI_LE_-
 * Set_Periodic_Advertising_Enable command with bit 0 of Enable set to 0 (peri-
 * odic advertising is disabled). Disabling the advertising set has no effect on the
 * periodic advertising once the advertising set has been enabled.
 *
 * The Controller manages the timing of advertisements in accordance with the
 * advertising parameters given in the HCI_LE_Set_Periodic_Advertising_-
 * Parameters command.
 *
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * If bit 0 of Enable is set to 1 (periodic advertising is enabled) and the advertising
 * set contains partial periodic advertising data, the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * If bit 0 of Enable is set to 1 and the Host has not issued the HCI_LE_Set_-
 * Periodic_Advertising_Parameters command for the advertising set, the Con-
 * troller shall either use vendor-specified parameters or return the error code
 * Command Disallowed (0x0C).
 *
 * If bit 0 of Enable is set to 1 and the length of the periodic advertising data is
 * greater than the maximum that the Controller can transmit within the chosen
 * periodic advertising interval, the Controller shall return the error code Packet
 * Too Long (0x45). If advertising on the LE Coded PHY, the S=8 coding shall be
 * assumed unless the current advertising parameters require the use of S=2 for
 * an advertising physical channel, in which case the S=2 coding shall be
 * assumed for that advertising physical channel.
 * If bit 0 of Enable is set to 1 and the advertising set identified by the
 * Advertising_Handle specified scannable, connectable, legacy, or anonymous
 * advertising, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If bit 0 of Enable is set to 0 and the Controller supports the Periodic Advertising
 * ADI Support feature, then the Controller shall ignore bit 1.
 *
 * If bit 1 of Enable is set to 1 and the Controller does not support the Periodic
 * Advertising ADI Support feature, the Controller shall return an error which
 * should use the error code Unsupported Feature or Parameter Value (0x11).
 *
 * Enabling periodic advertising when it is already enabled can cause the random
 * address to change. Disabling periodic advertising when it is already disabled
 * has no effect.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_enable(const sdc_hci_cmd_le_set_periodic_adv_enable_t * p_params);

/** @brief LE Set Extended Scan Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.64
 *
 * The HCI_LE_Set_Extended_Scan_Parameters command is used to set the
 * extended scan parameters to be used on the advertising physical channels.
 *
 * The Scanning_PHYs parameter indicates the PHY(s) on which the advertising
 * packets should be received on the primary advertising physical channel. The
 * Host may enable one or more scanning PHYs. If the Host specifies a PHY that
 * is not supported by the Controller, including a bit that is reserved for future use,
 * it should return the error code Unsupported Feature or Parameter Value
 * (0x11). The Scan_Type[i], Scan_Interval[i], and Scan_Window[i] parameters
 * array elements are ordered in the same order as the set bits in the
 * Scanning_PHY parameter, starting from bit 0. The number of array elements is
 * determined by the number of bits set in the Scanning_PHY parameter.
 *
 * The Scan_Type[i] parameter specifies the type of scan to perform.
 *
 * The Scan_Interval[i] and Scan_Window[i] parameters are recommendations
 * from the Host on how long (Scan_Window[i]) and how frequently
 * (Scan_Interval[i]) the Controller should scan (see [Vol 6] Part B, Section 4.4.3);
 * however the frequency and length of the scan is implementation specific. If the
 * requested scan cannot be supported by the implementation, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * The Own_Address_Type parameter indicates the type of address being used
 * in the scan request packets.
 *
 * If the Host issues this command when scanning is enabled in the Controller,
 * the Controller shall return the error code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Extended_Scan_Parameters command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_ext_scan_params(const sdc_hci_cmd_le_set_ext_scan_params_t * p_params);

/** @brief LE Set Extended Scan Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.65
 *
 * The HCI_LE_Set_Extended_Scan_Enable command is used to enable or
 * disable scanning for both legacy and extended advertising PDUs.
 *
 * The Enable parameter determines whether scanning is enabled or disabled. If
 * it is set to 0x00, the remaining parameters shall be ignored.
 *
 * If Enable is set to 0x01 and the Host has not issued the HCI_LE_Set_-
 * Extended_Scan_Parameters command, the Controller shall either use vendor-
 * specified parameters or return the error code Command Disallowed (0x0C).
 *
 * The Filter_Duplicates parameter controls whether the Link Layer should filter
 * out duplicate advertising reports (filtering duplicates enabled) to the Host or if
 * the Link Layer should generate advertising reports for each packet received
 * (filtering duplicates disabled). See [Vol 6] Part B, Section 4.4.3.5.
 *
 * If the Filter_Duplicates parameter is set to 0x00, all advertisements received
 * from advertisers shall be sent to the Host in advertising report events.
 *
 * If the Filter_Duplicates parameter is set to 0x01, duplicate advertisements
 * should not be sent to the Host in advertising report events until scanning is
 * disabled.
 *
 * If the Filter_Duplicates parameter is set to 0x02, duplicate advertisements in a
 * single scan period should not be sent to the Host in advertising report events;
 * this setting shall only be used if both Period and Duration are non-zero. If
 * Filter_Duplicates is set to 0x02 and either Period or Duration to zero, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Duration parameter is zero or both the Duration parameter and Period
 * parameter are non-zero, the Controller shall continue scanning until scanning
 * is disabled by the Host issuing an HCI_LE_Set_Extended_Scan_Enable
 * command with the Enable parameter set to 0x00 (Scanning is disabled). The
 * Period parameter shall be ignored when the Duration parameter is zero.
 *
 * If the Duration parameter is non-zero and the Period parameter is zero, the
 * Controller shall continue scanning until the duration specified in the Duration
 * parameter has expired.
 * If both the Duration and Period parameters are non-zero and the Duration is
 * greater than or equal to the Period, the Controller shall return the error code
 * Invalid HCI Command Parameters (0x12).
 *
 * When the Duration and Period parameters are non-zero, the Controller shall
 * scan for the duration of the Duration parameter within a scan period specified
 * by the Period parameter. After the scan period has expired, a new scan period
 * shall begin and scanning shall begin again for the duration specified. The scan
 * periods continue until the Host disables scanning.
 *
 * If the HCI_LE_Set_Extended_Scan_Enable command with Enable set to 0x01
 * is sent while scanning is already enabled, the timers used for duration and
 * period are reset to the new parameter values and a new scan period is started.
 * Any change to the Filter_Duplicates setting or the random address shall take
 * effect.
 *
 * Disabling scanning when it is disabled has no effect.
 *
 * Note: The duration of a scan period refers to the time spent scanning on both
 * the primary and secondary advertising physical channels. However, expiry of
 * the duration does not prevent the Link Layer from scanning for and receiving
 * auxiliary packets of received advertisements.
 *
 * If Enable is set to 0x01, the scanning parameters' Own_Address_Type
 * parameter is set to 0x00 or 0x02, and the device does not have a public
 * address, the Controller should return an error code which should be Invalid HCI
 * Command Parameters (0x12).
 *
 * If Enable is set to 0x01, the scanning parameters' Own_Address_Type
 * parameter is set to 0x01 or 0x03, and the random address for the device has
 * not been initialized using the HCI_LE_Set_Random_Address command, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Extended_Scan_Enable command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * Zero or more LE Extended Advertising Reports are generated by the Controller
 * based on any advertising packets received and the duplicate filtering in effect.
 * More than one advertising packet may be reported in each HCI_LE_-
 * Extended_Advertising_Report event.
 *
 * At the end of a single scan (Duration non-zero but Period zero), an HCI_LE_-
 * Scan_Timeout event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_ext_scan_enable(const sdc_hci_cmd_le_set_ext_scan_enable_t * p_params);

/** @brief LE Extended Create Connection [v1].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.66
 *
 * The HCI_LE_Extended_Create_Connection command is used to create an
 * ACL connection, with the local device in the Central role, to a connectable
 * advertiser. The command is also used to create an ACL connection between a
 * periodic advertiser and a synchronized device.
 *
 * If a connection is created with the local device in the Peripheral role while this
 * command is pending, then this command remains pending.
 * The Advertising_Handle parameter is used to identify the periodic advertising
 * train.
 *
 * The Subevent parameter is used to identify the subevent where a connection
 * request shall be initiated from a periodic advertising train.
 *
 * The Advertising_Handle and Subevent parameters shall be set to 0xFF if these
 * parameters are not used.
 *
 * If the Advertising_Handle and Subevent parameters are set to valid values,
 * then the Initiator_Filter_Policy, Initiating_PHYs, Scan_Interval[i], and
 * Scan_Window[i] parameters shall be ignored.
 *
 * The Initiator_Filter_Policy parameter is used to determine whether the Filter
 * Accept List is used. If the Filter Accept List is not used, the
 * Peer_Address_Type and the Peer_Address parameters specify the address
 * type and address of the advertising device to connect to.
 *
 * The Own_Address_Type parameter indicates the type of address being used
 * in the connection request packets.
 *
 * The Peer_Address_Type parameter indicates the type of address used in the
 * connectable advertisement sent by the peer.
 *
 * The Peer_Address parameter indicates the Peer’s Public Device Address,
 * Random (static) Device Address, Non-Resolvable Private Address, or
 * Resolvable Private Address depending on the Peer_Address_Type parameter.
 *
 * The Initiating_PHYs parameter indicates the PHY(s) on which the advertising
 * packets should be received on the primary advertising physical channel and
 * the PHYs for which connection parameters have been specified. The Host may
 * enable one or more initiating PHYs. If the Host specifies a PHY that is not
 * supported by the Controller, including a bit that is reserved for future use, the
 * latter should return the error code Unsupported Feature or Parameter Value
 * (0x11). The array elements of the arrayed parameters are ordered in the same
 * order as the set bits in the Initiating_PHYs parameter, starting from bit 0. The
 * number of array elements is determined by the number of bits set in the
 * Initiating_PHYs parameter. When a connectable advertisement is received and
 * a connection request is sent on one PHY, scanning on any other PHYs is
 * terminated.
 *
 * The Scan_Interval[i] and Scan_Window[i] parameters are recommendations
 * from the Host on how long (Scan_Window[i]) and how frequently
 * (Scan_Interval[i]) the Controller should scan (see [Vol 6] Part B, Section 4.5.3);
 * however the frequency and length of the scan is implementation specific. If the
 * requested scan cannot be supported by the implementation, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12). If bit 1 is
 * set in Initiating_PHYs, the values for the LE 2M PHY shall be ignored.
 * The Connection_Interval_Min[i] and Connection_Interval_Max[i] parameters
 * define the minimum and maximum allowed connection interval. The
 * Connection_Interval_Min[i] parameter shall not be greater than the
 * Connection_Interval_Max[i] parameter.
 *
 * The Max_Latency[i] parameter defines the maximum allowed Peripheral
 * latency (see [Vol 6] Part B, Section 4.5.1).
 *
 * The Supervision_Timeout[i] parameter defines the link supervision timeout for
 * the connection. The Supervision_Timeout[i] in milliseconds shall be larger than
 * (1 + Max_Latency[i]) × Connection_Interval_Max[i] × 2, where Connection_-
 * Interval_Max[i] is given in milliseconds (see [Vol 6] Part B, Section 4.5.2).
 *
 * The Min_CE_Length[i] and Max_CE_Length[i] parameters provide the
 * Controller with the expected minimum and maximum length of the connection
 * events. The Min_CE_Length[i] parameter shall be less than or equal to the
 * Max_CE_Length[i] parameter. The Controller is not required to use these
 * values.
 *
 * Where the connection is made on a PHY whose bit is not set in the Initiating_-
 * PHYs parameter, the Controller shall use the Connection_Interval_Min[i],
 * Connection_Interval_Max[i], Max_Latency[i], Supervision_Timeout[i],
 * Min_CE_Length[i], and Max_CE_Length[i] parameters for an implementation-
 * chosen PHY whose bit is set in the Initiating_PHYs parameter.
 *
 * If the Host issues this command when another HCI_LE_Extended_Create_-
 * Connection command is pending in the Controller, the Controller shall return
 * the error code Command Disallowed (0x0C).
 *
 * If the Own_Address_Type parameter is set to 0x00 and the device does not
 * have a public address, the Controller should return an error code which should
 * be Invalid HCI Command Parameters (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x01 and the random address for
 * the device has not been initialized using the HCI_LE_Set_Random_Address
 * command, the Controller shall return the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x02, the Initiator_Filter_Policy
 * parameter is set to 0x00, the Controller's resolving list did not contain a
 * matching entry, and the device does not have a public address, the Controller
 * should return an error code which should be Invalid HCI Command Parameters
 * (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x02, the Initiator_Filter_Policy
 * parameter is set to 0x01, and the device does not have a public address, the
 * Controller should return an error code which should be Invalid HCI Command
 * Parameters (0x12).
 * If the Own_Address_Type parameter is set to 0x03, the Initiator_Filter_Policy
 * parameter is set to 0x00, the Controller's resolving list did not contain a
 * matching entry, and the random address for the device has not been initialized
 * using the HCI_LE_Set_Random_Address command, the Controller shall return
 * the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x03, the Initiator_Filter_Policy
 * parameter is set to 0x01, and the random address for the device has not been
 * initialized using the HCI_LE_Set_Random_Address command, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Initiating_PHYs parameter does not have at least one bit set for a PHY
 * allowed for scanning on the primary advertising physical channel, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command and the Controller has insufficient resources to
 * handle any more connections, the Controller shall return the error code
 * Connection Rejected due to Limited Resources (0x0D).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Extended_Create_Connection
 * command, the Controller sends the HCI_Command_Status event to the Host.
 * An HCI_LE_Enhanced_Connection_Complete event shall be generated when
 * a connection is created because of this command or the connection creation
 * procedure is cancelled; until the event is generated, the command is consid-
 * ered pending. If a connection creation is discarded, then the error code Con-
 * nection Failed to be Established / Synchronization Timeout (0x3E) shall be
 * used. If a connection is created, this event shall be immediately followed by an
 * HCI_LE_Channel_Selection_Algorithm event.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_ext_create_conn(const sdc_hci_cmd_le_ext_create_conn_t * p_params);

/** @brief LE Periodic Advertising Create Sync.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.67
 *
 * The HCI_LE_Periodic_Advertising_Create_Sync command is used to
 * synchronize with a periodic advertising train from an advertiser and begin
 * receiving periodic advertising packets.
 *
 * This command may be issued whether or not scanning is enabled and
 * scanning may be enabled and disabled (see the LE Set Extended Scan Enable
 * command) while this command is pending. However, synchronization can only
 * occur when scanning is enabled. While scanning is disabled, no attempt to
 * synchronize will take place.
 *
 * The Options parameter is used to determine whether the Periodic Advertiser
 * List is used, whether HCI_LE_Periodic_Advertising_Report events for this
 * periodic advertising train are initially enabled or disabled, and whether
 * duplicate reports are filtered or not. If the Periodic Advertiser List is not used,
 * the Advertising_SID, Advertiser Address_Type, and Advertiser Address
 * parameters specify the periodic advertising device to listen to; otherwise they
 * shall be ignored.
 *
 * The Advertising_SID parameter, if used, specifies the value that shall match
 * the Advertising SID subfield in the ADI field of the received advertisement for it
 * to be used to synchronize.
 *
 * The Skip parameter specifies the maximum number of consecutive periodic
 * advertising events that the receiver may skip after successfully receiving a
 * periodic advertising packet.
 *
 * The Sync_Timeout parameter specifies the maximum permitted time between
 * successful receives. If this time is exceeded, synchronization is lost.
 *
 * The Sync_CTE_Type parameter specifies whether to only synchronize to
 * periodic advertising with certain types of Constant Tone Extension (a value of 0
 * indicates that the presence or absence of a Constant Tone Extension is
 * irrelevant). If the periodic advertising has the wrong type of Constant Tone
 * Extension then:
 * • If bit 0 of Options is set, the Controller shall ignore this address and SID and
 *   continue to search for other periodic advertisements.
 * • Otherwise, the Controller shall cancel the synchronization with the error
 *   code Unsupported Remote Feature (0x1A).
 *
 * If the periodic advertiser changes the type of Constant Tone Extension after the
 * scanner has synchronized with the periodic advertising, the scanner's Link
 * Layer shall remain synchronized.
 *
 * If the Host sets all the non-reserved bits of the Sync_CTE_Type parameter to
 * 1, the Controller shall return the error code Command Disallowed (0x0C).
 *
 * Irrespective of the value of the Skip parameter, the Controller should stop
 * skipping packets before the Sync_Timeout would be exceeded.
 *
 * If the Host issues this command when another HCI_LE_Periodic_Advertising_-
 * Create_Sync command is pending, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * If the Host issues this command with bit 0 of Options not set and with
 * Advertising_SID, Advertiser_Address_Type, and Advertiser_Address the same
 * as those of a periodic advertising train that the Controller is already
 * synchronized to, the Controller shall return the error code Connection Already
 * Exists (0x0B).
 *
 * If the Host issues this command and the Controller has insufficient resources to
 * handle any more periodic advertising trains, the Controller shall return the error
 * code Memory Capacity Exceeded (0x07).
 *
 * If bit 1 of Options is set to 1 and the Controller supports the Periodic
 * Advertising ADI Support feature, then the Controller shall ignore bit 2.
 *
 * If bit 1 of Options is set to 0, bit 2 is set to 1, and the Controller does not
 * support the Periodic Advertising ADI Support feature, then the Controller shall
 * return an error which should use the error code Unsupported Feature or
 * Parameter Value (0x11).
 *
 * If bit 1 of the Options parameter is set to 1 and the Controller does not support
 * the HCI_LE_Set_Periodic_Advertising_Receive_Enable command, the
 * Controller shall return the error code Connection Failed to be Established /
 * Synchronization Timeout (0x3E).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Periodic_Advertising_Create_Sync command has been
 * received, the Controller sends the HCI_Command_Status event to the Host.
 * An HCI_LE_Periodic_Advertising_Sync_Established event shall be generated
 * when the Controller starts receiving periodic advertising packets.
 *
 * When the Controller receives periodic advertising packets then, if reporting is
 * enabled, it sends HCI_LE_Periodic_Advertising_Report events to the Host.
 *
 * Note: The HCI_LE_Periodic_Advertising_Sync_Established event can be sent
 * as a result of synchronization being canceled by an HCI_LE_Periodic_-
 * Advertising_Create_Sync_Cancel command.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_periodic_adv_create_sync(const sdc_hci_cmd_le_periodic_adv_create_sync_t * p_params);

/** @brief LE Periodic Advertising Create Sync Cancel.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.68
 *
 * The HCI_LE_Periodic_Advertising_Create_Sync_Cancel command is used to
 * cancel the HCI_LE_Periodic_Advertising_Create_Sync command while it is
 * pending.
 *
 * If the Host issues this command while no HCI_LE_Periodic_Advertising_-
 * Create_Sync command is pending, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Periodic_Advertising_Create_Sync_Cancel command has
 * completed, the Controller sends an HCI_Command_Complete event to the
 * Host.
 *
 * After the HCI_Command_Complete is sent and if the cancellation was
 * successful, the Controller sends an HCI_LE_Periodic_Advertising_Sync_-
 * Established event to the Host with the error code Operation Cancelled by Host
 * (0x44).
 *
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_periodic_adv_create_sync_cancel(void);

/** @brief LE Periodic Advertising Terminate Sync.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.69
 *
 * The HCI_LE_Periodic_Advertising_Terminate_Sync command is used to stop
 * reception of the periodic advertising train identified by the Sync_Handle
 * parameter.
 *
 * If the periodic advertising train corresponding to the Sync_Handle parameter
 * does not exist, then the Controller shall return the error code Unknown
 * Advertising Identifier (0x42).
 *
 * Following successful completion of this command the Sync_Handle is
 * destroyed.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Periodic_Advertising_Terminate_Sync command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_periodic_adv_terminate_sync(const sdc_hci_cmd_le_periodic_adv_terminate_sync_t * p_params);

/** @brief LE Add Device To Periodic Advertiser List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.70
 *
 * The HCI_LE_Add_Device_To_Periodic_Advertiser_List command is used to
 * add an entry, consisting of a single device address and SID, to the Periodic
 * Advertiser list stored in the Controller. Any additions to the Periodic Advertiser
 * list take effect immediately. If the entry is already on the list, the Controller shall
 * return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command when an HCI_LE_Periodic_Advertising_-
 * Create_Sync command is pending, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * When a Controller cannot add an entry to the Periodic Advertiser list because
 * the list is full, the Controller shall return the error code Memory Capacity
 * Exceeded (0x07).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Add_Device_To_Periodic_Advertiser_List command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_add_device_to_periodic_adv_list(const sdc_hci_cmd_le_add_device_to_periodic_adv_list_t * p_params);

/** @brief LE Remove Device From Periodic Advertiser List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.71
 *
 * The HCI_LE_Remove_Device_From_Periodic_Advertiser_List command is
 * used to remove one entry from the list of Periodic Advertisers stored in the
 * Controller. Removals from the Periodic Advertisers List take effect immediately.
 *
 * If the Host issues this command when an HCI_LE_Periodic_Advertising_-
 * Create_Sync command is pending, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * When a Controller cannot remove an entry from the Periodic Advertiser list
 * because it is not found, the Controller shall return the error code Unknown
 * Advertising Identifier (0x42).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Remove_Device_From_Periodic_Advertiser_List
 * command has completed, an HCI_Command_Complete event shall be
 * generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_remove_device_from_periodic_adv_list(const sdc_hci_cmd_le_remove_device_from_periodic_adv_list_t * p_params);

/** @brief LE Clear Periodic Advertiser List.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.72
 *
 * The HCI_LE_Clear_Periodic_Advertiser_List command is used to remove all
 * entries from the list of Periodic Advertisers in the Controller.
 *
 * If this command is used when an HCI_LE_Periodic_Advertising_Create_Sync
 * command is pending, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Clear_Periodic_Advertiser_List command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_clear_periodic_adv_list(void);

/** @brief LE Read Periodic Advertiser List Size.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.73
 *
 * The HCI_LE_Read_Periodic_Advertiser_List_Size command is used to read
 * the total number of Periodic Advertiser list entries that can be stored in the
 * Controller.
 *
 * Note: The number of entries that can be stored is not fixed and the Controller
 * can change it at any time (e.g., because the memory used to store the list can
 * also be used for other purposes).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Periodic_Advertiser_List_Size command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_periodic_adv_list_size(sdc_hci_cmd_le_read_periodic_adv_list_size_return_t * p_return);

/** @brief LE Read Transmit Power.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.74
 *
 * The HCI_LE_Read_Transmit_Power command is used to read the minimum
 * and maximum transmit powers supported by the Controller across all
 * supported PHYs.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read _Transmit_Power command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_transmit_power(sdc_hci_cmd_le_read_transmit_power_return_t * p_return);

/** @brief LE Read RF Path Compensation.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.75
 *
 * The HCI_LE_Read_RF_Path_Compensation command is used to read the RF
 * path compensation value parameters used in the Tx power level and RSSI
 * calculation.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_RF_Path_Compensation command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_rf_path_compensation(sdc_hci_cmd_le_read_rf_path_compensation_return_t * p_return);

/** @brief LE Write RF Path Compensation.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.76
 *
 * The HCI_LE_Write_RF_Path_Compensation command is used to indicate the
 * RF path gain or loss between the RF transceiver and the antenna contributed
 * by intermediate components. A positive value means a net RF path gain and a
 * negative value means a net RF path loss. The RF_TX_Path_Compensation_-
 * Value parameter shall be used by the Controller to calculate the radiative Tx
 * power level used in HCI commands, HCI events, Advertising physical channel
 * PDUs, and Link Layer Control PDUs using the following equation:
 *
 * Radiative Tx power level = Tx power level at RF transceiver output + RF_TX_-
 * Path_Compensation_Value
 *
 * For example, if the Tx power level is +4 (dBm) at RF transceiver output and the
 * RF_TX_Path_Compensation_Value is -1.5 (dB), the radiative Tx power level is
 * +4+(-1.5) = 2.5 (dBm).
 *
 * The RF_RX_Path_Compensation_Value parameter shall be used by the
 * Controller to calculate the RSSI value reported to the Host using the following
 * equation:
 *
 * Rx power level at RF transceiver input = Rx power level at antenna + RF_RX_-
 * Path_Compensation_Value
 *
 * For example, if the Rx power level is -45 (dBm) at RF transceiver input and the
 * RF_RX_Path_Compensation_Value is -3.2 (dB), the Rx power level at antenna
 * is -41.8 (dBm).
 *
 * The default values for the RF path compensation are vendor-specific.
 *
 * This command can be issued at any time. If this command is issued during an
 * ongoing over-the-air RF activity, the Controller may apply the Tx path
 * compensation immediately or after a vendor-specific delay.
 *
 * The Controller shall apply a change to the Tx path compensation value either
 * by leaving the power at the transceiver output unchanged and altering the
 * radiative Tx power level or by altering the power at the transceiver output to
 * maintain any previously chosen radiative Tx power level.
 *
 * If the Host needs to maintain a specific radiative transmit power level for an
 * advertising set, it should disable that set before issuing this command then,
 * after the command completes, reissue the HCI_LE_Set_Extended_-
 * Advertising_Parameters command for that set and then re-enable it.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Write_RF_Path_Compensation command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * If the command leads to a change in the local radiative transmit power level for
 * an LE ACL connection, then the Controller shall generate an HCI_LE_-
 * Transmit_Power_Reporting event if local reporting is enabled and initiate a
 * Link Layer Power Change Indication procedure if remote reporting is enabled.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_write_rf_path_compensation(const sdc_hci_cmd_le_write_rf_path_compensation_t * p_params);

/** @brief LE Set Privacy Mode.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.77
 *
 * The HCI_LE_Set_Privacy_Mode command is used to allow the Host to specify
 * the privacy mode to be used for a given entry on the resolving list. The effect of
 * this setting is specified in [Vol 6] Part B, Section 4.7.
 *
 * When an entry on the resolving list is removed, the mode associated with that
 * entry shall also be removed.
 *
 * This command shall not be used when address resolution is enabled in the
 * Controller and:
 * • Advertising (other than periodic advertising) is enabled,
 * • Scanning is enabled, or
 * • an HCI_LE_Create_Connection, HCI_LE_Extended_Create_Connection, or
 *   HCI_LE_Periodic_Advertising_Create_Sync command is pending.
 *
 * This command may be used at any time when address resolution is disabled in
 * the Controller.
 *
 * If the device is not on the resolving list, the Controller shall return the error
 * code Unknown Connection Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Privacy_Mode command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_privacy_mode(const sdc_hci_cmd_le_set_privacy_mode_t * p_params);

/** @brief LE Set Connectionless CTE Transmit Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.80
 *
 * The HCI_LE_Set_Connectionless_CTE_Transmit_Parameters command is
 * used to set the type, length, and antenna switching pattern for the transmission
 * of Constant Tone Extensions in any periodic advertising on the advertising set
 * identified by the Advertising_Handle parameter.
 *
 * The CTE_Count parameter specifies how many packets with a Constant Tone
 * Extension are to be transmitted in each periodic advertising event. If the
 * number of packets that would otherwise be transmitted is less than this, the
 * Controller shall transmit sufficient AUX_CHAIN_IND PDUs with no AdvData to
 * make up the number. However, if a change in circumstances since this
 * command was issued means that the Controller can no longer schedule all of
 * these packets, it should transmit as many as possible.
 *
 * If the Host issues this command when Constant Tone Extensions have been
 * enabled in the advertising set, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * The Switching_Pattern_Length and Antenna_IDs[i] parameters are only used
 * when transmitting an AoD Constant Tone Extension and shall be ignored if
 * CTE_Type specifies an AoA Constant Tone Extension.
 *
 * If the CTE_Length parameter is greater than the maximum length of Constant
 * Tone Extension supported, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * If the Host requests a type of Constant Tone Extension that the Controller does
 * not support, the Controller shall return the error code Unsupported Feature or
 * Parameter Value (0x11).
 *
 * If the Controller is unable to schedule CTE_Count packets in each event, the
 * Controller shall return the error code Unsupported Feature or Parameter Value
 * (0x11).
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * If Switching_Pattern_Length is greater than the maximum length of switching
 * pattern supported by the Controller (see Section 7.8.87), the Controller shall
 * return the error code Unsupported Feature or Parameter Value (0x11).
 *
 * If the Controller determines that any of the Antenna_IDs[i] values do not
 * identify an antenna in the device's antenna array, it shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * Note: Some Controllers may be unable to determine which values do or do not
 * identify an antenna.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Connectionless_CTE_Transmit_Parameters
 * command has completed, an HCI_Command_Complete event shall be
 * generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_connless_cte_transmit_params(const sdc_hci_cmd_le_set_connless_cte_transmit_params_t * p_params);

/** @brief LE Set Connectionless CTE Transmit Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.81
 *
 * The HCI_LE_Set_Connectionless_CTE_Transmit_Enable command is used to
 * request that the Controller enables or disables the use of Constant Tone
 * Extensions in any periodic advertising on the advertising set identified by
 * Advertising_Handle.
 *
 * In order to start sending periodic advertisements containing a Constant Tone
 * Extension, the Host must also enable periodic advertising using the
 * HCI_LE_Set_Periodic_Advertising_Enable command (see Section 7.8.63).
 *
 * Note: Periodic advertising can only be enabled when advertising is enabled on
 * the same advertising set, but can continue after advertising has been disabled.
 *
 * If the Host issues this command before it has issued the
 * HCI_LE_Set_Periodic_Advertising_Parameters command (see Section 7.8.61)
 * for the advertising set, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * Once enabled, the Controller shall continue advertising with Constant Tone
 * Extensions until either one of the following occurs:
 * • The Host issues an HCI_LE_Set_Connectionless_CTE_Transmit_Enable
 *   command with CTE_Enable set to 0x00 (disabling Constant Tone
 *   Extensions but allowing periodic advertising to continue).
 * • The Host issues an HCI_LE_Set_Periodic_Advertising_Enable command
 *   (see Section 7.8.63) with Enable set to 0x00 (disabling periodic advertising).
 *   If periodic advertising is re-enabled then it shall continue to contain Constant
 *   Tone Extensions.
 *
 * If the Host issues this command before it has issued the
 * HCI_LE_Set_Connectionless_CTE_Transmit_Parameters command for the
 * advertising set, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If the periodic advertising is on a PHY that does not allow Constant Tone
 * Extensions, the Controller shall return the error code Command Disallowed
 * (0x0C).
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * The Host may issue this command when advertising or periodic advertising is
 * enabled in the advertising set.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Connectionless_CTE_Transmit_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_connless_cte_transmit_enable(const sdc_hci_cmd_le_set_connless_cte_transmit_enable_t * p_params);

/** @brief LE Set Connection CTE Transmit Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.84
 *
 * The HCI_LE_Set_Connection_CTE_Transmit_Parameters command is used
 * to set the antenna switching pattern and permitted Constant Tone Extension
 * types used for transmitting Constant Tone Extensions requested by the peer
 * device on the connection identified by the Connection_Handle parameter.
 *
 * If the Host issues this command when Constant Tone Extension responses
 * have been enabled on the connection, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * If the CTE_Types parameter has a bit set for a type of Constant Tone
 * Extension that the Controller does not support, the Controller shall return the
 * error code Unsupported Feature or Parameter Value (0x11).
 *
 * The Switching_Pattern_Length and Antenna_IDs[i] parameters are only used
 * when transmitting an AoD Constant Tone Extension and shall be ignored when
 * CTE_Types does not have a bit set for an AoD Constant Tone Extension; they
 * do not affect the transmission of an AoA Constant Tone Extension.
 *
 * If Switching_Pattern_Length is greater than the maximum length of switching
 * pattern supported by the Controller, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * If the Controller determines that any of the Antenna_IDs[i] values do not
 * identify an antenna in the device's antenna array, it shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * Note: Some Controllers may be unable to determine which values do or do not
 * identify an antenna.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Connection_CTE_Transmit_Parameters command
 * has completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_conn_cte_transmit_params(const sdc_hci_cmd_le_set_conn_cte_transmit_params_t * p_params,
                                                    sdc_hci_cmd_le_set_conn_cte_transmit_params_return_t * p_return);

/** @brief LE Connection CTE Response Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.86
 *
 * The HCI_LE_Connection_CTE_Response_Enable command is used to
 * request the Controller to respond to LL_CTE_REQ PDUs with LL_CTE_RSP
 * PDUs on the specified connection.
 *
 * If the Host issues this command before issuing the HCI_LE_Set_Connection_-
 * CTE_Transmit_Parameters command at least once on the connection, the
 * Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Host issues this command when the transmitter PHY for the connection is
 * not a PHY that allows Constant Tone Extensions, the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * If the transmitter PHY for the connection changes to a PHY that does not allow
 * Constant Tone Extensions, then the Controller shall automatically disable
 * Constant Tone Extension responses.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Connection_CTE_Response_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_conn_cte_response_enable(const sdc_hci_cmd_le_conn_cte_response_enable_t * p_params,
                                                sdc_hci_cmd_le_conn_cte_response_enable_return_t * p_return);

/** @brief LE Read Antenna Information.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.87
 *
 * The HCI_LE_Read_Antenna_Information command allows the Host to read
 * the switching rates, the sampling rates, the number of antennae, and the
 * maximum length of a transmitted Constant Tone Extension supported by the
 * Controller.
 *
 * If the Controller does not support antenna switching, the value of
 * Max_Switching_Pattern_Length shall still be valid but will not be used by the
 * Host.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Antenna_Information command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_antenna_information(sdc_hci_cmd_le_read_antenna_information_return_t * p_return);

/** @brief LE Set Periodic Advertising Receive Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.88
 *
 * The HCI_LE_Set_Periodic_Advertising_Receive_Enable command will enable
 * or disable reports for the periodic advertising train identified by the
 * Sync_Handle parameter.
 *
 * The Enable parameter determines whether reporting and duplicate filtering are
 * enabled or disabled. If the value is the same as the current state, the command
 * has no effect.
 *
 * If the periodic advertising train corresponding to the Sync_Handle parameter
 * does not exist, the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * If the Host sets both bits 0 and 1 of Enable and the Controller does not support
 * the Periodic Advertising ADI Support feature, then the Controller shall return an
 * error which should use the error code Unsupported Feature or Parameter
 * Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Receive_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_receive_enable(const sdc_hci_cmd_le_set_periodic_adv_receive_enable_t * p_params);

/** @brief LE Periodic Advertising Sync Transfer.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.89
 *
 * The HCI_LE_Periodic_Advertising_Sync_Transfer command is used to
 * instruct the Controller to send synchronization information about the periodic
 * advertising train identified by the Sync_Handle parameter to a connected
 * device.
 *
 * The Service_Data parameter is a value provided by the Host for use by the
 * Host of the peer device. It is not used by the Controller.
 *
 * The connected device is identified by the Connection_Handle parameter.
 *
 * If the periodic advertising train corresponding to the Sync_Handle parameter
 * does not exist, the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * If the Connection_Handle parameter does not identify a current connection, the
 * Controller shall return the error code Unknown Connection Identifier (0x02).
 *
 * If the remote device has not indicated support for the Periodic Advertising Sync
 * Transfer - Recipient feature, the Controller shall return the error code
 * Unsupported Remote Feature (0x1A).
 *
 * Note: This command may complete before the periodic advertising
 * synchronization information is sent. No indication is given as to how the
 * recipient handled the information.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Periodic_Advertising_Sync_Transfer command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_periodic_adv_sync_transfer(const sdc_hci_cmd_le_periodic_adv_sync_transfer_t * p_params,
                                                  sdc_hci_cmd_le_periodic_adv_sync_transfer_return_t * p_return);

/** @brief LE Periodic Advertising Set Info Transfer.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.90
 *
 * The HCI_LE_Periodic_Advertising_Set_Info_Transfer command is used to
 * instruct the Controller to send synchronization information about the periodic
 * advertising in an advertising set to a connected device.
 *
 * The Advertising_Handle parameter identifies the advertising set. If the
 * parameters in the advertising set have changed since the periodic advertising
 * was first enabled, the current parameters – not the original ones – are sent.
 *
 * The Service_Data parameter is a value provided by the Host to identify the
 * periodic advertising train to the peer device. It is not used by the Controller.
 *
 * The connected device is identified by the Connection_Handle parameter.
 *
 * If the advertising set corresponding to the Advertising_Handle parameter does
 * not exist, the Controller shall return the error code Unknown Advertising
 * Identifier (0x42).
 *
 * If periodic advertising is not currently in progress for the advertising set, the
 * Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Connection_Handle parameter does not identify a current connection, the
 * Controller shall return the error code Unknown Connection Identifier (0x02).
 *
 * If the remote device has not indicated support for the Periodic Advertising Sync
 * Transfer - Recipient feature, the Controller shall return the error code
 * Unsupported Remote Feature (0x1A).
 *
 * Note: This command may complete before the periodic advertising
 * synchronization information is sent. No indication is given as to how the
 * recipient handled the information.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Periodic_Advertising_Set_Info_Transfer command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_periodic_adv_set_info_transfer(const sdc_hci_cmd_le_periodic_adv_set_info_transfer_t * p_params,
                                                      sdc_hci_cmd_le_periodic_adv_set_info_transfer_return_t * p_return);

/** @brief LE Set Periodic Advertising Sync Transfer Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.91
 *
 * The HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters command
 * is used to specify how the Controller will process periodic advertising
 * synchronization information received from the device identified by the
 * Connection_Handle parameter (the "transfer mode").
 *
 * The Mode parameter specifies the action to be taken when periodic advertising
 * synchronization information is received. If Mode is 0x00, the Controller will
 * ignore the information. Otherwise it will notify the Host and synchronize to the
 * periodic advertising. Mode also specifies whether periodic advertising reports
 * are initially enabled or disabled and whether duplicates are filtered.
 *
 * The Skip parameter specifies the number of consecutive periodic advertising
 * packets that the receiver may skip after successfully receiving a periodic
 * advertising packet.
 *
 * The Sync_Timeout parameter specifies the maximum permitted time between
 * successful receives. If this time is exceeded, synchronization is lost.
 *
 * Irrespective of the value of the Skip parameter, the Controller should stop
 * skipping packets before the Sync_Timeout would be exceeded.
 *
 * The CTE_Type parameter specifies whether to only synchronize to periodic
 * advertising with certain types of Constant Tone Extension. If the periodic
 * advertiser changes the type of the Constant Tone Extension after the Controller
 * has synchronized with the periodic advertising, it shall remain synchronized.
 *
 * Note: A value of 0 (i.e. all bits clear) indicates that the presence or absence of
 * a Constant Tone Extension is irrelevant.
 *
 * This command does not affect any processing of any periodic advertising
 * synchronization information already received from the peer device, whether or
 * not the Controller has yet synchronized to the periodic advertising train it
 * describes.
 *
 * The parameter values provided by this command override those provided via
 * the HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters
 * command (Section 7.8.92) or any preferences previously set using the
 * HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters command on
 * the same connection.
 *
 * If the Connection_Handle parameter does not identify a current connection, the
 * Controller shall return the error code Unknown Connection Identifier (0x02).
 *
 * If the Host sets Mode to 0x03 and the Controller does not support the Periodic
 * Advertising ADI Support feature, then the Controller shall return an error which
 * should use the error code Unsupported Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters
 * command has completed, an HCI_Command_Complete event shall be
 * generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params(const sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params_t * p_params,
                                                             sdc_hci_cmd_le_set_periodic_adv_sync_transfer_params_return_t * p_return);

/** @brief LE Set Default Periodic Advertising Sync Transfer Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.92
 *
 * The HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_Parameters
 * command is used to specify the initial value for the mode, skip, timeout, and
 * Constant Tone Extension type (set by the HCI_LE_Set_Periodic_Advertising_-
 * Sync_Transfer_Parameters command; see Section 7.8.91) to be used for all
 * subsequent connections over the LE transport.
 *
 * The Mode parameter specifies the initial action to be taken. If Mode is 0x00,
 * the Controller will ignore the information. Otherwise it will notify the Host and
 * synchronize to the periodic advertising. Mode also specifies whether periodic
 * advertising reports are initially enabled or disabled and whether duplicates are
 * filtered.
 *
 * The Skip parameter specifies the number of consecutive periodic advertising
 * packets that the receiver may skip after successfully receiving a periodic
 * advertising packet.
 *
 * The Sync_Timeout parameter specifies the maximum permitted time between
 * successful receives. If this time is exceeded, synchronization is lost.
 *
 * The CTE_Type parameter specifies whether to only synchronize to periodic
 * advertising with certain types of Constant Tone Extension. If the periodic
 * advertiser changes the type of the Constant Tone Extension after the Controller
 * has synchronized with the periodic advertising, it shall remain synchronized.
 *
 * Note: A value of 0 (i.e. all bits clear) indicates that the presence or absence of
 * a Constant Tone Extension is irrelevant.
 *
 * This command does not affect any existing connection.
 *
 * If the Host sets Mode to 0x03 and the Controller does not support the Periodic
 * Advertising ADI Support feature, then the Controller shall return an error which
 * should use the error code Unsupported Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Default_Periodic_Advertising_Sync_Transfer_-
 * Parameters command has completed, an HCI_Command_Complete event
 * shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_default_periodic_adv_sync_transfer_params(const sdc_hci_cmd_le_set_default_periodic_adv_sync_transfer_params_t * p_params);

/** @brief LE Read Buffer Size [v2].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.2
 *
 * This command is used to read the maximum size of the data portion of ACL
 * data packets and isochronous data packets sent from the Host to the Control-
 * ler. The Host shall segment the data transmitted to the Controller according to
 * these values so that the HCI ACL Data packets and HCI ISO Data packets will
 * contain data up to this size (“data” includes optional fields in the HCI ISO Data
 * packet, such as ISO_SDU_Length). The HCI_LE_Read_Buffer_Size
 * command also returns the total number of HCI LE ACL Data packets and iso-
 * chronous data packets that can be stored in the data buffers of the Controller.
 * The HCI_LE_Read_Buffer_Size command shall be issued by the Host before it
 * sends any data to an LE Controller (see Section 4.1.1). If the Controller sup-
 * ports HCI ISO Data packets, it shall return non-zero values for the ISO_Data_-
 * Packet_Length and Total_Num_ISO_Data_Packets parameters.
 *
 * If the Controller returns a length value of zero for ACL data packets, the Host
 * shall use the HCI_Read_Buffer_Size command to determine the size of the
 * data buffers (shared between BR/EDR and LE transports).
 *
 * Note: Both the HCI_Read_Buffer_Size command and the HCI_LE_Read_-
 * Buffer_Size command may return buffer length and number of packets param-
 * eter values that are nonzero. This allows a Controller to offer different buffers
 * and number of buffers for BR/EDR data packets and LE data packets.
 *
 * The LE_ACL_Data_Packet_Length return parameter shall be used to deter-
 * mine the maximum size of the L2CAP PDU segments that are contained in
 * ACL data packets, and which are transferred from the Host to the Controller to
 * be broken up into packets by the Link Layer. The Total_Num_LE_ACL_Data_-
 * Packets return parameter contains the total number of HCI ACL Data packets
 * that can be stored in the data buffers of the Controller. The Host determines
 * how to divide the buffers between different connection handles.
 *
 * The ISO_Data_Packet_Length return parameter shall be used to determine the
 * maximum size of the SDU segments that are contained in isochronous data
 * packets, and which are transferred from the Host to the Controller. The
 * Total_Num_ISO_Data_Packets return parameter contains the total number of
 * isochronous data packets that can be stored in the data buffers of the
 * Controller. The Host determines how to divide the buffers between different
 * connection handle(s).
 *
 * Note: The LE_ACL_Data_Packet_Length and ISO_Data_Packet_Length
 * return parameters do not include the length of the HCI ACL Data packet header
 * or the HCI ISO Data packet header respectively.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_Buffer_Size command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_buffer_size_v2(sdc_hci_cmd_le_read_buffer_size_v2_return_t * p_return);

/** @brief LE Read ISO TX Sync.
 *
 * The description below is extracted from Core_v5.3,
 * Vol 4, Part E, Section 7.8.96
 *
 * This command is used to read the TX_Time_Stamp and Time_Offset of a
 * transmitted SDU identified by the Packet_Sequence_Number on a CIS or BIS
 * identified by the Connection_Handle parameter on the Central or Peripheral.
 *
 * The Packet_Sequence_Number parameter contains the sequence number of a
 * transmitted SDU.
 *
 * The TX_Time_Stamp and Time_Offset parameters are described in [Vol 6] Part
 * G, Section 3.3 and [Vol 6] Part G, Section 3.1 respectively. When the
 * Connection_Handle identifies a CIS or BIS that is transmitting unframed PDUs,
 * the value of Time_Offset returned shall be zero.
 *
 * If the Host issues this command with a connection handle that does not exist,
 * or the connection handle is not associated with a CIS or BIS, the Controller
 * shall return the error code Unknown Connection Identifier (0x02).
 *
 * If the Host issues this command on an existing connection handle for a CIS or
 * BIS that is not configured for transmitting SDUs, the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * If the Host issues this command before an SDU has been transmitted by the
 * Controller, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_ISO_TX_Sync command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_iso_tx_sync(const sdc_hci_cmd_le_read_iso_tx_sync_t * p_params,
                                        sdc_hci_cmd_le_read_iso_tx_sync_return_t * p_return);

/** @brief LE Set CIG Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.97
 *
 * The HCI_LE_Set_CIG_Parameters command is used by a Central'’s Host to
 * create a CIG and to set the parameters of one or more CISes that are
 * associated with a CIG in the Controller.
 *
 * The CIG_ID parameter identifies a CIG. This parameter is allocated by the
 * Central's Host and passed to the Peripheral's Host through the Link Layers
 * during the process of creating a CIS. If the CIG_ID does not exist, then the
 * Controller shall first create a new CIG. Once the CIG is created (whether
 * through this command or previously), the Controller shall modify or add CIS
 * configurations in the CIG that is identified by the CIG_ID and update all the
 * parameters that apply to the CIG.
 *
 * The SDU_Interval_C_To_P parameter specifies the time interval between the
 * start of consecutive SDUs from the Central's Host for all the CISes in the CIG.
 * This parameter shall be ignored for all CISes that are unidirectional from
 * Peripheral to Central.
 *
 * The SDU_Interval_P_To_C parameter specifies the time interval between the
 * start of consecutive SDUs from the Peripheral's Host for all the CISes in the
 * CIG. This parameter shall be ignored for all CISes that are unidirectional from
 * Central to Peripheral.
 * The Worst_Case_SCA parameter shall be the worst-case sleep clock accuracy
 * of all the Peripherals that will participate in the CIG. The Host should get the
 * sleep clock accuracy from all the Peripherals before issuing this command. If
 * the Host cannot get the sleep clock accuracy from all the Peripherals, it shall
 * set the Worst_Case_SCA parameter to zero.
 *
 * Note: The Worst_Case_SCA parameter can be used by the Link Layer to
 * better allow for clock drift when scheduling the CISes in the CIG. For example,
 * if a CIS has more than two subevents, the Link Layer of the Central can set the
 * timing of the subevents such that the worst case drift in the Peripheral's clock
 * will not exceed 2 × Sub_Interval. This prevents the Peripheral from
 * synchronizing its timing to the wrong subevent (adjacent subevents cannot be
 * on the same channel).
 *
 * The Packing parameter indicates the preferred method of arranging subevents
 * of multiple CISes. The subevents can be arranged in Sequential or Interleaved
 * arrangement (see [Vol 6] Part B, Section 4.5.14.2). This is a recommendation
 * to the Controller which the Controller may ignore. This parameter shall be
 * ignored when there is only one CIS in the CIG.
 *
 * The Framing parameter indicates the format of the CIS Data PDUs of the
 * specified CISes. If the Framing parameter is set to 1 then the CIS Data PDUs
 * of the specified CISes shall be framed. If the Framing parameter is set to 0 the
 * CIS Data PDUs of a given CIS may be either unframed or framed (determined
 * separately for each specified CIS) (see [Vol 6] Part G, Section 1).
 *
 * The Max_Transport_Latency_C_To_P parameter contains the maximum
 * transport latency from the Central to the Peripheral, in milliseconds, as
 * described in [Vol 6] Part G, Section 3.2.1 and [Vol 6] Part G, Section 3.2.2. This
 * parameter shall be ignored for all CISes that are unidirectional from Peripheral
 * to Central.
 *
 * The Max_Transport_Latency_P_To_C parameter contains the maximum
 * transport latency from the Peripheral to the Central, in milliseconds, as
 * described in [Vol 6] Part G, Section 3.2.1 and [Vol 6] Part G, Section 3.2.2. This
 * parameter shall be ignored for all CISes that are unidirectional from Central to
 * Peripheral.
 *
 * The CIS_Count parameter indicates the number of CIS configurations being
 * modified or added by this command. The Controller shall set the CIS_Count
 * return parameter equal to this.
 *
 * The CIS_ID[i] parameter identifies a CIS and is set by the Central’s Host and
 * passed to the Peripheral’s Host through the Link Layers during the process of
 * establishing a CIS.
 *
 * The Max_SDU_C_To_P[i] parameter identifies the maximum size of an SDU
 * from the Central’s Host. If the CIS is unidirectional from Peripheral to Central,
 * this parameter shall be set to 0. If a CIS configuration that is being modified
 * has a data path set in the Central to Peripheral direction and the Host has
 * specified that Max_SDU_C_To_P[i] shall be set to zero, the Controller shall
 * return the error code Command Disallowed (0x0C).
 *
 * The Max_SDU_P_To_C[i] parameter identifies the maximum size of an SDU
 * from the Peripheral's Host. If the CIS is unidirectional from Central to
 * Peripheral, this parameter shall be set to 0. If a CIS configuration that is being
 * modified has a data path set in the Peripheral to Central direction and the Host
 * has specified that Max_SDU_P_To_C[i] shall be set to zero, the Controller
 * shall return the error code Command Disallowed (0x0C).
 *
 * The PHY_C_To_P[i] parameter identifies which PHY to use for transmission
 * from the Central to the Peripheral. The Host shall set at least one bit in this
 * parameter and the Controller shall pick a PHY from the bits that are set.
 *
 * The PHY_P_To_C[i] parameter identifies which PHY to use for transmission
 * from the Peripheral to the Central. The Host shall set at least one bit in this
 * parameter and the Controller shall pick a PHY from the bits that are set.
 *
 * The RTN_C_To_P[i] (Retransmission Number) parameter contains the number
 * of times that a CIS Data PDU should be retransmitted from the Central to
 * Peripheral before being acknowledged or flushed (irrespective of which CIS
 * events the retransmission opportunities occur in). If the CIS is unidirectional
 * from Peripheral to Central, this parameter shall be ignored. Otherwise, this
 * parameter is a recommendation to the Controller which the Controller may
 * ignore.
 *
 * The RTN_P_To_C[i] parameter contains the number of times that a CIS Data
 * PDU should be retransmitted from the Peripheral to Central before being
 * acknowledged or flushed (irrespective of which CIS events the retransmission
 * opportunities occur in). If the CIS is unidirectional from Central to Peripheral,
 * this parameter shall be ignored. Otherwise, this parameter is a
 * recommendation to the Controller which the Controller may ignore.
 *
 * In each direction, if the Controller satisfies the recommendation, then every
 * PDU will have at least RTN+1 opportunities for transmission (assuming that the
 * initial transmission of that PDU happens at the earliest allowed subevent). The
 * RTN value indicates that the Host is recommending that the Controller selects
 * a combination of CIS parameters that satisfy the inequality:
 *
 * NSE * FT - NSE ÷ BN * ( BN - 1 ) ≥ RTN + 1
 *
 * If the Status return parameter is non-zero, then the state of the CIG and its CIS
 * configurations shall not be changed by the command. If the CIG did not already
 * exist, it shall not be created.
 *
 * If the Status return parameter is zero, then the Controller shall set the
 * Connection_Handle arrayed return parameter to the connection handle(s)
 * corresponding to the CIS configurations specified in the CIS_IDs command
 * parameter, in the same order. If the same CIS_ID is being reconfigured, the
 * same connection handle shall be returned.
 * The connection handle of a CIS shall refer to the CIS when it exists and to the
 * configuration of the CIS stored in a CIG when the CIG exists but the CIS with
 * that CIS_ID does not.
 *
 * If the Host issues this command when the CIG is not in the configurable state,
 * the Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Host attempts to create a CIG or set parameters that exceed the
 * maximum supported resources in the Controller, the Controller shall return the
 * error code Memory Capacity Exceeded (0x07).
 *
 * If the Host attempts to set CIS parameters that exceed the maximum
 * supported connections in the Controller, the Controller shall return the error
 * code Connection Limit Exceeded (0x09).
 *
 * If the Host sets, in the PHY_C_To_P[i] or PHY_P_To_C[i] parameters, a bit for
 * a PHY that the Controller does not support, including a bit that is reserved for
 * future use, the Controller shall return the error code Unsupported Feature or
 * Parameter Value (0x11).
 *
 * If the Controller does not support asymmetric PHYs and the Host sets
 * PHY_C_To_P[i] to a different value than PHY_P_To_C[i], the Controller shall
 * return the error code Unsupported Feature or Parameter Value (0x11).
 *
 * If the Host specifies an invalid combination of CIS parameters, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12) or the
 * error code Unsupported Feature or Parameter Value (0x11); it should return
 * the error code Invalid HCI Command Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_CIG_Parameters command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_cig_params(const sdc_hci_cmd_le_set_cig_params_t * p_params,
                                      sdc_hci_cmd_le_set_cig_params_return_t * p_return);

/** @brief LE Set CIG Parameters Test.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.98
 *
 * The HCI_LE_Set_CIG_Parameters_Test command should only be used for
 * testing purposes.
 *
 * The command is used by a Central's Host to create a CIG and to set the
 * parameters of one or more CISes that are associated with a CIG in the
 * Controller.
 *
 * The CIG_ID parameter identifies a CIG. This parameter is allocated by the
 * Central's Host and passed to the Peripheral's Host through the Link Layers
 * during the process of creating a CIS. If the CIG_ID does not exist, then the
 * Controller shall first create a new CIG. Once the CIG is created (whether
 * through this command or previously), the Controller shall modify or add CIS
 * configurations in the CIG that is identified by the CIG_ID and update all the
 * parameters that apply to the CIG.
 *
 * The SDU_Interval_C_To_P parameter specifies the time interval of periodic
 * SDUs from the Central's Host.
 * The SDU_Interval_P_To_C parameter specifies the time interval of periodic
 * SDUs from the Peripheral's Host.
 *
 * The FT_C_To_P parameter identifies the maximum time for a payload from the
 * Central to Peripheral to be transmitted and re-transmitted, after which it is
 * flushed (see [Vol 6] Part B, Section 4.5.13.5). This parameter is expressed in
 * multiples of ISO_Interval.
 *
 * The FT_P_To_C parameter identifies the maximum time for a payload from the
 * Peripheral to Central to be transmitted and re-transmitted, after which it is
 * flushed (see[Vol 6] Part B, Section 4.5.13.5). This parameter is expressed in
 * multiples of ISO_Interval.
 *
 * The ISO_Interval parameter specifies the time between two consecutive CIS
 * anchor points.
 *
 * The CIS_Count parameter contains the number of CIS configurations being
 * added or modified by this command. The Controller shall set the CIS_Count
 * return parameter equal to this.
 *
 * The CIS_ID[i] parameter identifies the CIS and is set by the Central's Host and
 * passed to the Peripheral's Host through the Link Layers during the process of
 * establishing a CIS.
 *
 * The Worst_Case_SCA parameter is the worst-case sleep clock accuracy of all
 * the Peripherals that will participate in the CIG. The Host should get the sleep
 * clock accuracy from all the Peripherals before issuing this command. In case
 * the Host cannot get the sleep clock accuracy from all the Peripherals, it shall
 * set the Worst_Case_SCA parameter to zero.
 *
 * Note: The Worst_Case_SCA parameter can be used by the Link Layer to
 * better allow for clock drift when scheduling the CISes in the CIG. For example,
 * if a CIS has more than two subevents, the Link Layer of the Central can set the
 * timing of the subevents such that the worst case drift in the Peripheral's clock
 * will not exceed 2 × Sub_Interval. This prevents the Peripheral from
 * synchronizing its timing to the wrong subevent (adjacent subevents cannot be
 * on the same channel).
 *
 * The Packing parameter is used to indicate the preferred method of arranging
 * subevents of multiple CISes. The subevents can be arranged in Sequential or
 * Interleaved arrangement. This is a recommendation to the Controller which it
 * may ignore. This parameter shall be ignored when there is only one CIS in the
 * CIG.
 *
 * The Framing parameter indicates the format of the CIS Data PDUs of all the
 * CISes. If the Framing parameter is set to 1 then the CIS Data PDUs of the
 * specified CISes shall be framed, and when set to 0 they shall be unframed (see
 * [Vol 6] Part G, Section 1).
 *
 * The CIS_ID[i] parameter is used to identify a CIS.
 * The NSE[i] parameter identifies the maximum number of subevents for each
 * CIS in a CIG event.
 *
 * The Max_SDU_C_To_P[i] parameter identifies the maximum size of SDU from
 * the Central’s Host. If the CIS is unidirectional from Peripheral to Central, this
 * parameter shall be set to 0. If a CIS configuration that is being modified has a
 * data path set in the Central to Peripheral direction and the Host has specified
 * that Max_SDU_C_To_P[i] shall be set to zero, the Controller shall return the
 * error code Command Disallowed (0x0C). The minimum value of the
 * Max_SDU_Size parameter in the ISO Transmit Test mode when the
 * Payload_Type = 1 or 2 shall be 4 octets.
 *
 * The Max_SDU_P_To_C[i] parameter identifies the maximum size of SDU from
 * the Peripheral’s Host. If the CIS is unidirectional from Central to Peripheral, this
 * parameter shall be set to 0. If a CIS configuration that is being modified has a
 * data path set in the Peripheral to Central direction and the Host has specified
 * that Max_SDU_P_To_C[i] shall be set to zero, the Controller shall return the
 * error code Command Disallowed (0x0C).The minimum value of the Max_SDU
 * parameter in the ISO Transmit Test mode when the Payload_Type = 1 or 2
 * shall be 4 octets.
 *
 * The Max_PDU_C_To_P[i] parameter identifies the maximum size PDU from
 * the Central to Peripheral.
 *
 * The Max_PDU_P_To_C[i] parameter identifies the maximum size PDU from
 * the Peripheral to Central.
 *
 * The PHY_C_To_P[i] parameter identifies the PHY to be used for transmission
 * of packets from the Central to the Peripheral. The Host shall set only one bit in
 * this parameter and the Controller shall use the PHY set by the Host.
 *
 * The PHY_P_To_C[i] parameter identifies the PHY to be used for transmission
 * of packets from the Peripheral to the Central. The Host shall set only one bit in
 * this parameter and the Controller shall use the PHY set by the Host.
 *
 * The BN_C_To_P[i] parameter identifies the burst number for Central to
 * Peripheral (see [Vol 6] Part B, Section 4.5.13). If the CIS is unidirectional from
 * Peripheral to Central, this parameter shall be set to zero.
 *
 * The BN_P_To_C[i] parameter identifies the burst number for Peripheral to
 * Central (see [Vol 6] Part B, Section 4.5.13). If the CIS is unidirectional from
 * Central to Peripheral, this parameter shall be set to zero.
 *
 * If the Status return parameter is non-zero, then the state of the CIG and its CIS
 * configurations shall not be changed by the command. If the CIG did not already
 * exist, it shall not be created.
 *
 * If the Status return parameter is zero, then the Controller shall set the
 * Connection_Handle arrayed return parameter to the connection handle(s)
 * corresponding to the CIS configurations specified in the CIS_IDs command
 * parameter, in the same order. If the same CIS_ID is being reconfigured, the
 * same connection handle shall be returned.
 *
 * If the Host issues this command when the CIG is not in the configurable state,
 * the Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Host attempts to create a CIG or set parameters that exceed the
 * maximum supported resources in the Controller, the Controller shall return the
 * error code Memory Capacity Exceeded (0x07).
 *
 * If the Host attempts to set CIS parameters that exceed the maximum
 * supported connections in the Controller, the Controller shall return the error
 * code Connection Limit Exceeded (0x09).
 *
 * If the Host attempts to set an invalid combination of CIS parameters, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12)
 * or the error code Unsupported Feature or Parameter Value (0x11); it should
 * return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host sets, in the PHY_C_To_P[i] or PHY_P_To_C[i] parameters, a bit for
 * a PHY that the Controller does not support, including a bit that is reserved for
 * future use, the Controller shall return the error code Unsupported Feature or
 * Parameter Value (0x11).
 *
 * If the Controller does not support asymmetric PHYs and the Host sets
 * PHY_C_To_P[i] to a different value than PHY_P_To_C[i], the Controller shall
 * return the error code Unsupported Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_CIG_Parameters_Test command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_cig_params_test(const sdc_hci_cmd_le_set_cig_params_test_t * p_params,
                                           sdc_hci_cmd_le_set_cig_params_test_return_t * p_return);

/** @brief LE Create CIS.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.99
 *
 * The HCI_LE_Create_CIS command is used by the Central's Host to create
 * one or more CISes using the connections identified by the ACL_Connection_-
 * Handle arrayed parameter.
 *
 * The CIS_Count parameter is the total number of CISes created by this
 * command.
 *
 * The CIS_Connection_Handle[i] parameter specifies the connection handle
 * corresponding to the configuration of the CIS to be created and whose
 * configuration is already stored in a CIG.
 *
 * The ACL_Connection_Handle[i] parameter specifies the connection handle of
 * the ACL connection associated with each CIS to be created. The list of the
 * ACL_Connection_Handles shall be in the same order as the list of the
 * CIS_Connection_Handles e.g., CIS_Connection_Handle[1] will connect to the
 * Peripheral associated with the ACL_Connection_Handle[1].
 *
 * If this command is issued on the Central before the devices have performed
 * the Feature Exchange procedure, then the Controller shall complete that pro-
 * cedure before initiating the Connected Isochronous Stream Creation procedure
 * (see [Vol 6] Part B, Section 5.1.15).
 *
 * If any ACL_Connection_Handle[i] is not the handle of an existing ACL
 * connection or any CIS_Connection_Handle[i] is not the handle of a CIS or CIS
 * configuration, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * If the Host attempts to create a CIS that has already been created, the
 * Controller shall return the error code Connection Already Exists (0x0B).
 *
 * If two different elements of the CIS_Connection_Handle arrayed parameter
 * identify the same CIS, the Controller shall return the error code Invalid HCI
 * Command Parameters (0x12).
 *
 * If the Host issues this command before all the HCI_LE_CIS_Established
 * events from the previous use of the command have been generated, the
 * Controller shall return the error code Command Disallowed (0x0C).
 * If the Host issues this command on an ACL_Connection_Handle where the
 * Controller is the Peripheral, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * Note: The order of the CIS connection handles in this command does not relate
 * to the order of connection handles in the return parameters of the HCI_LE_-
 * Set_CIG_Parameters command or the HCI_LE_Set_CIG_Parameters_Test
 * command.
 *
 * If the Host issues this command when the Connected Isochronous Stream
 * (Host Support) feature bit (see [Vol 6] Part B, Section 4.6.27) is not set, the
 * Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Host specified an invalid combination of parameters in the HCI_LE_Set_-
 * CIG_Parameters or HCI_LE_Set_CIG_Parameters_Test command that cre-
 * ated the CIS configuration, but the Controller could not detect the problem
 * without knowing the properties of the ACL connection associated with the CIS,
 * then the Controller shall return an error which should use the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * Note: If an error is reported in the HCI_Command_Status event, it means that
 * no CIS is created and the Host cannot determine which CIS had the error.
 * Therefore, in the case of an error that only affects one CIS of several, reporting
 * the error in the HCI_LE_CIS_Established event for that CIS means that the
 * remaining CISes are still created and the Host can determine which CIS had
 * the error.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Create_CIS command, the
 * Controller sends the HCI_Command_Status event to the Host. An
 * HCI_LE_CIS_Established event will be generated for each CIS when it is
 * established or if it is disconnected or considered lost before being established;
 * until all the events are generated, the command remains pending.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_create_cis(const sdc_hci_cmd_le_create_cis_t * p_params);

/** @brief LE Remove CIG.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.100
 *
 * The HCI_LE_Remove_CIG command is used by the Central’s Host to remove
 * the CIG identified by CIG_ID.
 *
 * The CIG_ID parameter contains the identifier of the CIG.
 *
 * This command shall delete the CIG_ID and also delete the
 * Connection_Handles of the CIS configurations stored in the CIG.
 *
 * This command shall also remove the isochronous data paths that are
 * associated with the Connection_Handles of the CIS configurations, which is
 * equivalent to issuing the HCI_LE_Remove_ISO_Data_Path command (see
 * Section 7.8.110).
 *
 * If the Host tries to remove a CIG which is in the active state, then the Controller
 * shall return the error code Command Disallowed (0x0C).
 *
 * If the Host issues this command with a CIG_ID that does not exist, the
 * Controller shall return the error code Unknown Connection Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Remove_CIG command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_remove_cig(const sdc_hci_cmd_le_remove_cig_t * p_params,
                                  sdc_hci_cmd_le_remove_cig_return_t * p_return);

/** @brief LE Accept CIS Request.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.101
 *
 * The HCI_LE_Accept_CIS_Request command is used by the Peripheral's Host
 * to inform the Controller to accept the request for the CIS that is identified by the
 * Connection_Handle.
 *
 * The command shall only be issued after an HCI_LE_CIS_Request event has
 * occurred. The event contains the Connection_Handle of the CIS.
 *
 * If the Peripheral's Host issues this command with a Connection_Handle that
 * does not exist, or the Connection_Handle is not for a CIS, the Controller shall
 * return the error code Unknown Connection Identifier (0x02).
 *
 * If the Peripheral's Host issues this command with a Connection_Handle for a
 * CIS that has already been established or that already has an HCI_LE_-
 * Accept_CIS_Request or HCI_LE_Reject_CIS_Request command in progress,
 * the Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Central’s Host issues this command, the Controller shall return the error
 * code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Accept_CIS_Request command,
 * the Controller sends the HCI_Command_Status event to the Host. An
 * HCI_LE_CIS_Established event will be generated when the CIS is established
 * or is considered lost before being established.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_accept_cis_request(const sdc_hci_cmd_le_accept_cis_request_t * p_params);

/** @brief LE Reject CIS Request.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.102
 *
 * The HCI_LE_Reject_CIS_Request command is used by the Peripheral's Host
 * to inform the Controller to reject the request for the CIS that is identified by the
 * Connection_Handle.
 *
 * The command shall only be issued after an HCI_LE_CIS_Request event has
 * occurred. The event contains the Connection_Handle of the CIS.
 *
 * When this command succeeds, the Controller shall delete the
 * Connection_Handle of the requested CIS.
 *
 * The Reason command parameter indicates the reason for rejecting the CIS
 * request.
 *
 * If the Peripheral's Host issues this command with a Connection_Handle that is
 * not for a CIS, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * If the Peripheral's Host issues this command with a Connection_Handle for a
 * CIS that has already been established or that already has an HCI_LE_-
 * Accept_CIS_Request or HCI_LE_Reject_CIS_Request command in progress,
 * the Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the Central’s Host issues this command, the Controller shall return the error
 * code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Reject_CIS_Request command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_reject_cis_request(const sdc_hci_cmd_le_reject_cis_request_t * p_params,
                                          sdc_hci_cmd_le_reject_cis_request_return_t * p_return);

/** @brief LE Create BIG.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.103
 *
 * The HCI_LE_Create_BIG command is used to create a BIG with one or more
 * BISes (see [Vol 6] Part B, Section 4.4.6). All BISes in a BIG have the same
 * value for all parameters.
 *
 * The BIG_Handle contains the identifier of the BIG. This parameter is allocated
 * by the Host and used by the Controller and the Host to identify a BIG.
 *
 * The Advertising_Handle identifies the associated periodic advertising train of
 * the BIG (see [Vol 6] Part B, Section 4.4.5.1).
 *
 * The Num_BIS parameter contains the total number of BISes in the BIG.
 *
 * The SDU_Interval parameter contains the time interval of the periodic SDUs.
 *
 * The Max_SDU parameter contains the maximum size of an SDU.
 *
 * The Max_Transport_Latency parameter is the maximum transport latency (in
 * milliseconds) as described in [Vol 6] Part G, Section 3.2.1 and [Vol 6] Part G,
 * Section 3.2.2. This includes pre-transmissions.
 *
 * The RTN (Retransmission Number) parameter contains the number of times
 * every PDU should be retransmitted, irrespective of which BIG events the
 * retransmissions occur in. This is a recommendation to the Controller which the
 * Controller may ignore.
 *
 * The PHY parameter is a bit field that indicates the PHY used for transmission
 * of PDUs of BISes in the BIG. The Host shall set at least one bit in this
 * parameter and the Controller shall pick a PHY from the bits set. If the Host
 * sets, in the PHY parameter, a bit for a PHY that the Controller does not
 * support, including a bit that is reserved for future use, the Controller shall return
 * the error code Unsupported Feature or Parameter Value (0x11).
 *
 * The Packing parameter is used to indicate the preferred method of arranging
 * subevents of multiple BISes. The subevents can be arranged in Sequential or
 * Interleaved arrangement. This is a recommendation to the Controller which it
 * may ignore. This parameter shall be ignored when there is only one BIS in the
 * BIG.
 *
 * The Framing parameter indicates the format for sending BIS Data PDUs. If the
 * Framing parameter is set to 1 then BIS Data PDUs shall be Framed and when
 * set to 0 they may be unframed (see [Vol 6] Part G, Section 1).
 *
 * The Encryption parameter identifies the encryption mode of the BISes. If the
 * Encryption parameter is set to 1 (encrypted), the Broadcast_Code is used in
 * the encryption of payloads (see [Vol 6] Part B, Section 4.4.6.10).
 *
 * The Broadcast_Code parameter is used to generate the encryption key for
 * encrypting payloads of all BISes. When the Encryption parameter is set to 0
 * (unencrypted), the Broadcast_Code parameter shall be set to zero by the Host
 * and ignored by the Controller.
 *
 * If the Controller cannot create all BISes of the BIG or if Num_BIS exceeds the
 * maximum value supported by the Controller, it shall return the error code
 * Connection Rejected due to Limited Resources (0x0D).
 *
 * If the Advertising_Handle does not identify a periodic advertising train or the
 * periodic advertising train is associated with another BIG, the Controller shall
 * return the error code Unknown Advertising Identifier (0x42).
 *
 * If the Host issues this command with a BIG_Handle for a BIG that is already
 * created, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If the Host specifies an invalid combination of BIG parameters, the Controller
 * shall return an error which should use the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * If the length of the associated periodic advertising, with the BIGInfo added to
 * the ACAD, is greater than the maximum that the Controller can transmit within
 * the periodic advertising interval, then the Controller shall return an error and
 * should use the error code Packet Too Long (0x45). If advertising on the LE
 * Coded PHY, then the S=8 coding shall be assumed unless the current
 * advertising parameters require the use of S=2 for an advertising physical
 * channel, in which case the S=2 coding shall be assumed for that advertising
 * physical channel.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Create_BIG command, the Control-
 * ler sends the HCI_Command_Status event to the Host. When the HCI_LE_-
 * Create_BIG command has completed, the HCI_LE_Create_BIG_Complete
 * event is generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_create_big(const sdc_hci_cmd_le_create_big_t * p_params);

/** @brief LE Create BIG Test.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.104
 *
 * The HCI_LE_Create_BIG_Test command should only be used for testing
 * purposes.
 *
 * The command is used to create one or more BISes of a BIG (see [Vol 6] Part B,
 * Section 4.4.6). All BISes in the BIG have the same values for all parameters.
 *
 * The BIG_Handle contains the identifier of the BIG. This parameter is allocated
 * by the Host and used by the Controller and the Host to identify a BIG.
 *
 * The Advertising_Handle identifies the associated periodic advertising train of
 * the BIG.
 *
 * The Num_BIS parameter contains the total number of BISes in the BIG.
 *
 * The SDU_Interval parameter specifies the time interval of the periodic SDUs.
 *
 * The ISO_Interval parameter contains the time duration between two
 * consecutive BIG anchor points.
 *
 * The NSE (Number of SubEvents) parameter contains the total number of
 * subevents that are used to transmit BIS Data PDUs for each BIS in a BIG
 * event. The NSE parameter shall be greater than or equal to IRC * BN.
 * The Max_SDU parameter contains the maximum size, in octets, of an SDU.
 * The minimum value of the Max_SDU parameter in the ISO Transmit Test mode
 * when the Payload_Type = 1 or 2 shall be 4.
 *
 * The Max_PDU parameter contains the maximum size of every BIS Data PDU
 * for every BIS in the BIG.
 *
 * The PHY parameter is a bit field that indicates the PHY used for transmission
 * of PDUs of BISes in the BIG. The Host shall set only one bit in this parameter
 * and the Controller shall use the PHY set by the Host. If the Host sets, in the
 * PHY parameter, a bit for a PHY that the Controller does not support, including
 * a bit that is reserved for future use, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * The Packing parameter indicates the preferred method of arranging subevents
 * of multiple BISes. The subevents can be arranged in Sequential or Interleaved
 * arrangement. This is a recommendation to the Controller which it may ignore.
 * This parameter shall be ignored when there is only one BIS in the BIG.
 *
 * The Framing parameter indicates the format of BIS Data PDUs. If the Framing
 * parameter is set to 1 then BIS Data PDUs shall be Framed and when set to 0
 * the BIS Data PDUs shall be unframed (see [Vol 6] Part G, Section 1).
 *
 * The BN (Burst Number) parameter contains the number of new payloads for
 * each BIS in a BIS event.
 *
 * The IRC (Immediate Repetition Count) parameter contains the number of times
 * the scheduled data packet is transmitted (see [Vol 6] Part B, Section 4.4.6).
 * The IRC parameter shall be an integer in the range 1 to (NSE ÷ BN).
 *
 * The PTO (Pre_Transmission_Offset) parameter contains the offset in number
 * of ISO_Intervals for pre transmissions of data packets (see [Vol 6] Part B,
 * Section 4.4.6).
 *
 * The Encryption parameter identifies the encryption mode of the BISes in the
 * BIG. If the Encryption parameter is set to 1 (encrypted), the Broadcast_Code is
 * used in the encryption of payloads (see [Vol 6] Part B, Section 4.4.6).
 *
 * The Broadcast_Code parameter is a 16-octet field that is used to generate the
 * session key to encrypt payloads of all BISes in the BIG. When the Encryption
 * parameter is set to 0 (unencrypted), all 16 octets of the Broadcast_Code
 * parameter shall be set to zero by the Host and ignored by the Controller.
 *
 * If the Controller cannot create all BISes of the BIG or if Num_BIS exceeds the
 * maximum value supported by the Controller, it shall return the error code
 * Connection Rejected due to Limited Resources (0x0D).
 *
 * If the Advertising_Handle does not identify a periodic advertising train or the
 * periodic advertising train is associated with another BIG, the Controller shall
 * return the error code Unknown Advertising Identifier (0x42).
 * If the Host issues this command with a BIG_Handle for a BIG that is already
 * created, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * If the value of the Max_PDU, NSE, BN, IRC or PTO parameters exceeds the
 * values supported by the Controller, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * If the Host specifies an invalid combination of BIG parameters, the Controller
 * shall return an error. If the value of the NSE parameter is not an integer
 * multiple of BN, or NSE is less than (IRC * BN), or the parameters are not in the
 * specified range, these errors shall use the error code Unsupported Feature or
 * Parameter Value (0x11). The errors in all other circumstances should use the
 * error code Invalid HCI Command Parameters (0x12).
 *
 * If the length of the associated periodic advertising, with the BIGInfo added to
 * the ACAD, is greater than the maximum that the Controller can transmit within
 * the periodic advertising interval, then the Controller shall return an error and
 * should use the error code Packet Too Long (0x45). If advertising on the LE
 * Coded PHY, then the S=8 coding shall be assumed unless the current
 * advertising parameters require the use of S=2 for an advertising physical
 * channel, in which case the S=2 coding shall be assumed for that advertising
 * physical channel.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Create_BIG_Test command, the
 * Controller sends the HCI_Command_Status event to the Host. When the
 * HCI_LE_Create_BIG_Test command has completed, the HCI_LE_Create_-
 * BIG_Complete event is generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_create_big_test(const sdc_hci_cmd_le_create_big_test_t * p_params);

/** @brief LE Terminate BIG.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.105
 *
 * The HCI_LE_Terminate_BIG command is used to terminate a BIG identified by
 * the BIG_Handle parameter. The command also terminates the transmission of
 * all BISes of the BIG, destroys the associated connection handles of the BISes
 * in the BIG and removes the data paths for all BISes in the BIG.
 *
 * The Reason parameter is used to indicate the reason why the BIG is to be
 * terminated.
 *
 * If the BIG_Handle does not identify a BIG, the Controller shall return the error
 * code Unknown Advertising Identifier (0x42).
 *
 * If the Controller is not the Isochronous Broadcaster for the BIG identified by
 * BIG_Handle, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Terminate_BIG command, the
 * Controller sends the HCI_Command_Status event to the Host.
 *
 * When the HCI_LE_Terminate_BIG command has completed, the HCI_LE_-
 * Terminate_BIG_Complete event will be generated.
 * If the Host attempts to terminate a BIG while the process of establishment of
 * the BIG is in progress (i.e. HCI_LE_Create_BIG_Complete event has not been
 * generated) the process of establishment shall stop and the Controller shall
 * generate the HCI_LE_Create_BIG_Complete event to the Host with the error
 * code Operation Cancelled by Host (0x44).
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_terminate_big(const sdc_hci_cmd_le_terminate_big_t * p_params);

/** @brief LE BIG Create Sync.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.106
 *
 * The HCI_LE_BIG_Create_Sync command is used to synchronize to a BIG
 * described in the periodic advertising train specified by the Sync_Handle
 * parameter.
 *
 * The BIG_Handle parameter is assigned by the Host to identify the
 * synchronized BIG.
 *
 * The Encryption parameter indicates whether the Broadcast_Code parameter is
 * valid.
 *
 * The Broadcast_Code parameter is a 16-octet field that is used to generate the
 * session key to encrypt or decrypt payloads of an encrypted BIS.
 * Broadcast_Code shall be ignored by the Controller if Encryption is set to 0x00.
 *
 * If Encryption is set to 0x00 for an encrypted BIG or is set to 0x01 for an
 * unencrypted BIG, then the Controller shall return the error Encryption Mode
 * Not Acceptable (0x25).
 *
 * The MSE (Maximum Subevents) parameter is the maximum number of
 * subevents that a Controller should use to receive data payloads in each
 * interval for a BIS.
 *
 * The BIG_Sync_Timeout parameter specifies the maximum permitted time
 * between successful receptions of BIS PDUs. If this time is exceeded,
 * synchronization is lost. When the Controller establishes synchronization and if
 * the BIG_Sync_Timeout set by the Host is less than 6 * ISO_Interval, the
 * Controller shall set the timeout to 6 * ISO_Interval.
 *
 * The Num_BIS parameter contains the number of BIS indices specified in the
 * BIS arrayed parameter. The number of BISes requested may be a subset of
 * the BISes available in the BIG.
 * The BIS arrayed parameter is a list of indices corresponding to BIS(es) in the
 * synchronized BIG. The list of BIS indices shall be in ascending order and shall
 * not contain any duplicates. This list of specified BIS(es) may be all or a subset
 * of BISes available in the BIG.
 *
 * If the Sync_Handle does not exist, the Controller shall return the error code
 * Unknown Advertising Identifier (0x42).
 *
 * If the Host sends this command with a BIG_Handle that is already allocated,
 * the Controller shall return the error code Command Disallowed (0x0C).
 *
 * If the information describing the BIG does not specify a PHY supported by the
 * Controller or does not specify exactly one PHY, the Controller shall return the
 * error code Unsupported Feature or Parameter Value (0x11).
 *
 * If the Num_BIS parameter is greater than the total number of BISes in the BIG,
 * the Controller shall return the error code Unsupported Feature or Parameter
 * Value (0x11).
 *
 * If the Host sends this command when the Controller is in the process of
 * synchronizing to any BIG, i.e. the HCI_LE_BIG_Sync_Established event has
 * not been generated, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * If the Controller is unable to receive PDUs from the specified number of BISes
 * in the synchronized BIG, it shall return the error code Connection Rejected Due
 * To Limited Resources (0x0D).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_BIG_Create_Sync command, the
 * Controller sends the HCI_Command_Status event to the Host. When the
 * HCI_LE_BIG_Create_Sync command has completed, the HCI_LE_BIG_-
 * Sync_Established event will be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_big_create_sync(const sdc_hci_cmd_le_big_create_sync_t * p_params);

/** @brief LE BIG Terminate Sync.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.107
 *
 * The HCI_LE_BIG_Terminate_Sync command is used to stop synchronizing or
 * cancel the process of synchronizing to the BIG identified by the BIG_Handle
 * parameter. The command also terminates the reception of BISes in the BIG
 * specified in the HCI_LE_BIG_Create_Sync command, destroys the associated
 * connection handles of the BISes in the BIG and removes the data paths for all
 * BISes in the BIG.
 *
 * If the Host issues this command with a BIG_Handle that does not exist, the
 * Controller shall return the error code Unknown Advertising Identifier (0x42).
 *
 * If the Host issues this command for a BIG which it is neither synchronized to
 * nor in the process of synchronizing to, then the Controller shall return the error
 * code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_BIG_Terminate_Sync command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * If the Host attempts to terminate synchronization with a BIG while the process
 * of synchronization with that BIG is in progress (i.e. HCI_LE_BIG_Sync_-
 * Established event has not been generated) the process of synchronization
 * shall stop, and the Controller shall generate the HCI_LE_BIG_Sync_-
 * Established event to the Host with the error code Operation Cancelled by Host
 * (0x44).
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_big_terminate_sync(const sdc_hci_cmd_le_big_terminate_sync_t * p_params,
                                          sdc_hci_cmd_le_big_terminate_sync_return_t * p_return);

/** @brief LE Request Peer SCA.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.108
 *
 * This command is used to read the Sleep Clock Accuracy (SCA) of the peer
 * device.
 *
 * The Connection_Handle parameter is the connection handle of the ACL
 * connection.
 *
 * If the Host sends this command with a Connection_Handle that does not exist,
 * or the Connection_Handle is not for an ACL the Controller shall return the error
 * code Unknown Connection Identifier (0x02).
 *
 * If the Host sends this command and the peer device does not support the
 * Sleep Clock Accuracy Updates feature, the Controller shall return the error
 * code Unsupported Feature or Parameter Value (0x11) in the HCI_LE_-
 * Request_Peer_SCA_Complete event.
 *
 * If the Host issues this command when the Controller is aware (e.g., through a
 * previous feature exchange) that the peer device's Link Layer does not support
 * the Sleep Clock Accuracy Updates feature, the Controller shall return the error
 * code Unsupported Remote Feature (0x1A).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Request_Peer_SCA command, the
 * Controller sends the HCI_Command_Status event to the Host. When the
 * HCI_LE_Request_Peer_SCA command has completed, the HCI_LE_-
 * Request_Peer_SCA_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_request_peer_sca(const sdc_hci_cmd_le_request_peer_sca_t * p_params);

/** @brief LE Setup ISO Data Path.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.109
 *
 * The HCI_LE_Setup_ISO_Data_Path command is used to identify and create
 * the isochronous data path between the Host and the Controller for a CIS, CIS
 * configuration, or BIS identified by the Connection_Handle parameter. This
 * command can also be used to configure a codec for each data path. When a
 * connection is created no data paths are set up for that connection.
 *
 * The input and output directions are defined from the perspective of the
 * Controller, so "input" refers to data flowing from the Host to the Controller.
 *
 * If the Host issues this command more than once for the same Connection_-
 * Handle and direction before issuing the HCI_LE_Remove_ISO_Data_Path
 * command for that Connection_Handle and direction, the Controller shall return
 * the error code Command Disallowed (0x0C).
 *
 * If the Host issues this command for a CIS on a Peripheral before it has issued
 * the HCI_LE_Accept_CIS_Request command for that CIS, then the Controller
 * shall return the error code Command Disallowed (0x0C).
 *
 * The Data_Path_Direction parameter specifies the direction for which the data
 * path is being configured.
 *
 * The Data_Path_ID parameter specifies the data transport path used. When set
 * to 0x00, the data path shall be over the HCI transport. When set to a value in
 * the range 0x01 to 0xFE, the data path shall use a vendor-specific transport
 * interface (e.g., a PCM interface) with logical transport numbers. The meanings
 * of these logical transport numbers are vendor-specific.
 *
 * If the Host issues this command for a vendor-specific data transport path that
 * has not been configured, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * If the Host attempts to set a data path with a Connection Handle that does not
 * exist or that is not for a CIS, CIS configuration, or BIS, the Controller shall
 * return the error code Unknown Connection Identifier (0x02).
 * If the Host attempts to set an output data path using a connection handle that is
 * for an Isochronous Broadcaster, for an input data path on a Synchronized
 * Receiver, or for a data path for the direction on a unidirectional CIS where BN
 * is set to 0, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * The Codec_ID parameter specifies the coding format used over the air.
 *
 * When Data_Path_Direction is set to 0x00 (input), the Controller_Delay param-
 * eter specifies the delay at the data source from the reference time of an SDU to
 * the CIG reference point (see [Vol 6] Part B, Section 4.5.14.1) or BIG anchor
 * point (see [Vol 6] Part B, Section 4.4.6.4). When Data_Path_Direction is set to
 * 0x01 (output), Controller_Delay specifies the delay from the
 * SDU_Synchronization_Reference to the point in time at which the Controller
 * begins to transfer the corresponding data to the data path interface. The Host
 * should use the HCI_Read_Local_Supported_Controller_Delay command to
 * obtain a suitable value for Controller_Delay.
 *
 * Note: Controller vendors may provide additional guidance to the Host on how
 * to select a suitable Controller_Delay value from the range of values provided
 * by the HCI_Read_Local_Supported_Controller_Delay command for various
 * configurations of the data path interface.
 *
 * The Codec_Configuration parameter specifies codec-specific configuration
 * information for the specified direction.
 *
 * If the Host issues this command with Codec_Configuration_Length non-zero
 * and Codec_ID set to transparent air mode, the Controller shall return the error
 * code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command with codec-related parameters that exceed the
 * bandwidth and latency allowed on the established CIS or BIS identified by the
 * Connection_Handle parameter, the Controller shall return the error code
 * Invalid HCI Command Parameters (0x12).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Setup_ISO_Data_Path command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_setup_iso_data_path(const sdc_hci_cmd_le_setup_iso_data_path_t * p_params,
                                           sdc_hci_cmd_le_setup_iso_data_path_return_t * p_return);

/** @brief LE Remove ISO Data Path.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.110
 *
 * The HCI_LE_Remove_ISO_Data_Path command is used to remove the input
 * and/or output data path(s) associated with a CIS, CIS configuration, or BIS
 * identified by the Connection_Handle parameter.
 *
 * The Data_Path_Direction parameter specifies which directions are to have the
 * data path removed.
 *
 * If the Host issues this command with a Connection_Handle that does not exist
 * or is not for a CIS, CIS configuration, or BIS, the Controller shall return the
 * error code Unknown Connection Identifier (0x02).
 *
 * If the Host issues this command for a data path that has not been set up (using
 * the HCI_LE_Setup_ISO_Data_Path command), the Controller shall return the
 * error code Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Remove_ISO_Data_Path command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_remove_iso_data_path(const sdc_hci_cmd_le_remove_iso_data_path_t * p_params,
                                            sdc_hci_cmd_le_remove_iso_data_path_return_t * p_return);

/** @brief LE ISO Transmit Test.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.111
 *
 * The HCI_LE_ISO_Transmit_Test command should only be used in the ISO
 * Test mode and only for testing purposes.
 *
 * The command is used to configure an established CIS or BIS specified by the
 * Connection_Handle parameter, and transmit test payloads which are
 * generated by the Controller.
 *
 * The Payload_Type parameter defines the configuration of SDUs in the
 * payload.
 *
 * If the Host issues this command with a connection handle that does not exist,
 * or the Connection_Handle command parameter is not associated with a CIS or
 * a BIS, the Controller shall return the error code Unknown Connection Identifier
 * (0x02).
 *
 * If the Host issues this command when the value of the transmit BN parameter
 * of the CIS is set to zero, the Controller shall return the error code Unsupported
 * Feature or Parameter Value (0x11).
 *
 * If the Host has set the input data path for the CIS or BIS identified by the
 * connection handle, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_ISO_Transmit_Test command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_iso_transmit_test(const sdc_hci_cmd_le_iso_transmit_test_t * p_params,
                                         sdc_hci_cmd_le_iso_transmit_test_return_t * p_return);

/** @brief LE ISO Receive Test.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.112
 *
 * The HCI_LE_ISO_Receive_Test command should only be used in the ISO Test
 * mode and only for testing purposes.
 *
 * The command is used to configure an established CIS or a synchronized BIG
 * specified by the Connection_Handle parameter to receive payloads.
 *
 * When using this command for a BIS, the Host shall synchronize with a BIG
 * using the HCI_LE_BIG_Create_Sync command before invoking this
 * command.
 *
 * The Payload_Type parameter defines the configuration of SDUs in the
 * payload.
 *
 * If the Host issues this command with a connection handle that is not for an
 * established CIS or a BIS, the Controller shall return the error code Unknown
 * Connection Identifier (0x02).
 *
 * If the Host issues this command when the value of the receive BN parameter of
 * the CIS or BIS is set to zero, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * If the Host has set the output data path for the CIS or BIS identified by the
 * Connection_Handle parameter, the Controller shall return the error code
 * Command Disallowed (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_ISO_Receive_Test command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_iso_receive_test(const sdc_hci_cmd_le_iso_receive_test_t * p_params,
                                        sdc_hci_cmd_le_iso_receive_test_return_t * p_return);

/** @brief LE ISO Read Test Counters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.113
 *
 * The HCI_LE_ISO_Read_Test_Counters command should only be used in the
 * ISO Test mode and only for testing purposes.
 *
 * The command is used to read the test counters (see [Vol 6] Part B, Section 7)
 * in the Controller which is configured in ISO Receive Test mode for a CIS or BIS
 * specified by the Connection_Handle. Reading the test counters does not reset
 * the test counters.
 *
 * The Received_SDU_Count, Missed_SDU_Count and Failed_SDU_Count
 * parameters are set in the ISO Receive Test mode (see [Vol 6] Part B, Section
 * 7.2).
 *
 * If the Host issues this command with a Connection_Handle parameter that is
 * not for an established CIS or a BIS, the Controller shall return the error code
 * Unknown Connection Identifier (0x02).
 *
 * If the Host issues this command for a CIS or BIS that is not configured in the
 * ISO Receive Test mode, the Controller shall return the error code Unsupported
 * Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_ISO_Read_Test_Counters command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_iso_read_test_counters(const sdc_hci_cmd_le_iso_read_test_counters_t * p_params,
                                              sdc_hci_cmd_le_iso_read_test_counters_return_t * p_return);

/** @brief LE ISO Test End.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.114
 *
 * The HCI_LE_ISO_Test_End command should only be used in the ISO Test
 * mode and only for testing purposes.
 *
 * The command is used to terminate the ISO Transmit and/or Receive Test
 * mode for a CIS or BIS specified by the Connection_Handle parameter but does
 * not terminate the CIS or BIS.
 *
 * When the Host terminates the ISO Test mode for a CIS or BIS that is set to ISO
 * Transmit Test mode only, the test counters in the return parameters shall be set
 * to zero.
 *
 * When the Host terminates the ISO Test mode for a CIS or BIS that is set to the
 * ISO Receive Test mode, the return parameters contain the values of the test
 * counters as defined in [Vol 6] Part B, Section 7.
 *
 * If the Host issues this command with a Connection_Handle that is not for an
 * established CIS or a BIS, the Controller shall return the error code Unknown
 * Connection Identifier (0x02).
 *
 * If the Host issues this command for a CIS or BIS that is not configured in the
 * ISO Transmit or Receive Test mode, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_ISO_Test_End command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_iso_test_end(const sdc_hci_cmd_le_iso_test_end_t * p_params,
                                    sdc_hci_cmd_le_iso_test_end_return_t * p_return);

/** @brief LE Set Host Feature.
 *
 * The description below is extracted from Core_v5.3,
 * Vol 4, Part E, Section 7.8.115
 *
 * The HCI_LE_Set_Host_Feature command is used by the Host to set or clear a
 * bit controlled by the Host in the Link Layer FeatureSet stored in the Controller
 * (see [Vol 6] Part B, Section 4.6).
 *
 * The Bit_Number parameter specifies the bit position in the FeatureSet.
 *
 * The Bit_Value parameter specifies whether the feature is enabled or disabled.
 *
 * If Bit_Number specifies a feature bit that is not controlled by the Host, the
 * Controller shall return the error code Unsupported Feature or Parameter Value
 * (0x11).
 *
 * If Bit_Value is set to 0x01 and Bit_Number specifies a feature bit that requires
 * support of a feature that the Controller does not support, the Controller shall
 * return the error code Unsupported Feature or Parameter Value (0x11).
 *
 * If the Host issues this command while the Controller has a connection to
 * another device, the Controller shall return the error code Command Disallowed
 * (0x0C).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Host_Feature command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_host_feature(const sdc_hci_cmd_le_set_host_feature_t * p_params);

/** @brief LE Read ISO Link Quality.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.116
 *
 * This command returns the values of various counters related to link quality that
 * are associated with the isochronous stream specified by the
 * Connection_Handle parameter.
 *
 * This command may be issued on both the Central and Peripheral if the
 * connection handle identifies a CIS and on the Synchronized Receiver if the
 * connection handle identifies a BIS.
 *
 * Each of the remaining return parameters shall contain the current value of the
 * corresponding counter; all the values shall be recorded at the same moment.
 * Each counter shall be a 32-bit unsigned value, shall be initialized to zero when
 * the isochronous stream is created, and shall be incremented as described
 * below. If a counter is not associated with the type of isochronous stream
 * specified, the value of the parameter shall be ignored.
 *
 *                                   Associated
 *  Counter                          Streams              When Incremented
 *
 *  TX_UnACKed_Packets               CIS                  The Link Layer does not receive an
 *                                                        acknowledgment for a CIS Data PDU
 *                                                        that it transmitted at least once by its
 *                                                        flush point (see [Vol 6] Part B, Section
 *                                                        4.5.13.5).
 *
 *  TX_Flushed_Packets               CIS                  The Link Layer does not transmit a spe-
 *                                                        cific payload by its flush point.
 *
 *  TX_Last_Subevent_-               CIS in Peripheral    The Link Layer transmits a CIS Data
 *  Packets                          role                 PDU in the last subevent of a CIS
 *                                                        event.
 *
 *  Retransmitted_Packets            CIS                  The Link Layer retransmits a CIS Data
 *                                                        PDU.
 *
 * Table 7.3: Isochronous streams link quality counters
 *                                 Associated
 *  Counter                        Streams                When Incremented
 *
 *  CRC_Error_Packets              CIS and BIS            The Link Layer receives a packet with a
 *                                                        CRC error.
 *
 *  RX_Unreceived_Packets          CIS and BIS            The Link Layer does not receive a spe-
 *                                                        cific payload by its flush point (on a
 *                                                        CIS) or the end of the event it is associ-
 *                                                        ated with (on a BIS; see [Vol 6] Part B,
 *                                                        Section 4.4.6.6).
 *
 *  Duplicate_Packets              CIS                    The Link Layer receives a retransmis-
 *                                                        sion of a CIS Data PDU.
 *
 * Table 7.3: Isochronous streams link quality counters
 *
 * If the Connection_Handle parameter does not identify a current CIS connection
 * or a BIS that the Controller is synchronized to, the Controller shall return the
 * error code Unknown Connection Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Read_ISO_Link_Quality command has completed, an
 * HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_iso_link_quality(const sdc_hci_cmd_le_read_iso_link_quality_t * p_params,
                                             sdc_hci_cmd_le_read_iso_link_quality_return_t * p_return);

/** @brief LE Enhanced Read Transmit Power Level.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.117
 *
 * The HCI_LE_Enhanced_Read_Transmit_Power_Level command is used to
 * read the current and maximum transmit power levels of the local Controller on
 * the ACL connection identified by the Connection_Handle parameter and the
 * PHY indicated by the PHY parameter.
 *
 * If the Host sets PHY to a value that the Controller does not support, including a
 * value that is reserved for future use, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * If the Connection_Handle parameter does not identify a current ACL
 * connection, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Enhanced_Read_Transmit_Power_Level command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_enhanced_read_transmit_power_level(const sdc_hci_cmd_le_enhanced_read_transmit_power_level_t * p_params,
                                                          sdc_hci_cmd_le_enhanced_read_transmit_power_level_return_t * p_return);

/** @brief LE Read Remote Transmit Power Level.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.118
 *
 * The HCI_LE_Read_Remote_Transmit_Power_Level command is used to read
 * the transmit power level used by the remote Controller on the ACL connection
 * that is identified by the Connection_Handle parameter and the PHY indicated
 * by the PHY parameter.
 *
 * The local Controller may use the remote transmit power level value obtained
 * from a prior Power Change Indication or Power Control Request procedure
 * (see [Vol 6] Part B, Section 5.1.17 and [Vol 6] Part B, Section 5.1.18). If the
 * Controller chooses not to use these prior values, or if no prior value is available
 * for one or more of the remote transmit power level, maximum transmit power
 * level, or minimum transmit power level, the local Controller shall initiate a new
 * Power Control Request procedure to obtain the remote transmit power level.
 *
 * If the Host sets PHY to a value that the Controller does not support, including a
 * value that is reserved for future use, the Controller shall return the error code
 * Unsupported Feature or Parameter Value (0x11).
 *
 * If the Connection_Handle parameter does not identify a current ACL
 * connection, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Read_Remote_Transmit_Power_-
 * Level command, the Controller shall send the HCI_Command_Status event to
 * the Host. When the Controller has determined the remote transmit power, it
 * shall generate an HCI_LE_Transmit_Power_Reporting event with Reason
 * 0x02.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_read_remote_transmit_power_level(const sdc_hci_cmd_le_read_remote_transmit_power_level_t * p_params);

/** @brief LE Set Path Loss Reporting Parameters.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.119
 *
 * The HCI_LE_Set_Path_Loss_Reporting_Parameters command is used to set
 * the path loss threshold reporting parameters for the ACL connection identified
 * by the Connection_Handle parameter.
 *
 * The path loss threshold-based mechanism is described in [Vol 6] Part B,
 * Section 4.5.16. For each zone boundary, the upwards boundary shall equal the
 * threshold plus the hysteresis and the downwards boundary shall equal the
 * threshold minus the hysteresis.
 *
 * If the Host issues this command with High_Threshold+High_Hysteresis greater
 * than 0xFF or with Low_Threshold less than Low_Hysteresis, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command with Low_Threshold greater than
 * High_Threshold or with Low_Threshold+Low_Hysteresis greater than
 * High_Threshold–High_Hysteresis, the Controller shall return the error code
 * Invalid HCI Command Parameters (0x12).
 *
 * The Min_Time_Spent parameter indicates the minimum time that the Controller
 * shall observe the path loss has crossed the threshold before the Controller
 * generates an event for the threshold crossing. The Host should specify a
 * suitable value based on the connection interval, subrate factor, and Peripheral
 * latency.
 *
 * If the Host issues this command when path loss monitoring is enabled, the
 * Controller shall override the existing path loss threshold reporting parameters
 * with the parameters provided in this command.
 *
 * The High_Threshold and the Low_Threshold parameters are common to all
 * PHYs supported by the Controller. However, the Host can reissue this
 * command with suitable parameters whenever a PHY switch is detected.
 *
 * If the Connection_Handle parameter does not identify a current ACL
 * connection, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Path_Loss_Reporting_Parameters command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * If the Host issues this command when path loss monitoring is enabled, and if
 * the new parameters mean that the path loss is now in a different zone, an
 * HCI_LE_Path_Loss_Threshold event shall be generated as soon as possible
 * irrespective of the Min_Time_Spent parameter and the timer shall be reset.
 *
 * If the Host issues this command with High_Threshold parameter set to 0xFF,
 * then the Controller shall not generate an HCI_LE_Path_Loss_Threshold event
 * with Zone_Entered set to 0x02.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_path_loss_reporting_params(const sdc_hci_cmd_le_set_path_loss_reporting_params_t * p_params,
                                                      sdc_hci_cmd_le_set_path_loss_reporting_params_return_t * p_return);

/** @brief LE Set Path Loss Reporting Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.120
 *
 * The HCI_LE_Set_Path_Loss_Reporting_Enable command is used to enable
 * or disable path loss reporting for the ACL connection identified by the
 * Connection_Handle parameter.
 *
 * If the Enable parameter is set to 0x01 and no prior LE Power Control Request
 * procedure has been initiated on the ACL connection, then the Controller may
 * need to initiate a new LE Power Control Request procedure on that ACL.
 *
 * Path loss reporting is disabled when the connection is first created.
 *
 * If the Host issues this command before it has issued the HCI_LE_Set_Path_-
 * Loss_Reporting_Parameters command on this connection, the Controller shall
 * return the error code Command Disallowed (0x0C).
 *
 * If the Connection_Handle parameter does not identify a current ACL
 * connection, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * Enabling path loss monitoring when it is already enabled or disabling path loss
 * monitoring when it is already disabled has no effect.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Path_Loss_Reporting_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * When reporting is enabled and was previously disabled, the Controller shall
 * generate an HCI_LE_Path_Loss_Threshold event as soon as it has a reliable
 * measurement of the path loss. If the Controller has to query the remote
 * Controller for its transmit power level, then it shall generate this event within
 * Tpath_loss_enable from the time it receives a response to its query. Otherwise, the
 * Controller shall generate this event within Tpath_loss_enable from the time the
 * command is issued. Tpath_loss_enable shall be Min_Time_Spent + 6 connection
 * events or, if longer, 2 connection events where the Controller actually receives
 * a packet from the peer, where Min_Time_Spent is specified by the
 * HCI_LE_Set_Path_Loss_Reporting_Parameters command.
 *
 * After the initial event on reporting being enabled, the Controller shall generate
 * this event each time it determines that the path loss has moved to a different
 * zone and stayed in that zone for Min_Time_Spent.As stated in [Vol 6] Part B,
 * Section 4.5.16, two consecutive events must not indicate the same zone.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_path_loss_reporting_enable(const sdc_hci_cmd_le_set_path_loss_reporting_enable_t * p_params,
                                                      sdc_hci_cmd_le_set_path_loss_reporting_enable_return_t * p_return);

/** @brief LE Set Transmit Power Reporting Enable.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.121
 *
 * The HCI_LE_Set_Transmit_Power_Reporting_Enable command is used to
 * enable or disable the reporting to the local Host of transmit power level
 * changes in the local and remote Controllers for the ACL connection identified
 * by the Connection_Handle parameter.
 *
 * If the Remote_Enable parameter is set to 0x01 and no prior LE Power Control
 * Request procedure has been initiated on the ACL connection, then the Control-
 * ler shall initiate a new LE Power Control Request procedure on that ACL.
 *
 * Reporting is disabled when the connection is first created.
 *
 * If the Connection_Handle parameter does not identify a current ACL
 * connection, the Controller shall return the error code Unknown Connection
 * Identifier (0x02).
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Transmit_Power_Reporting_Enable command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * When local reporting is enabled, the Controller shall generate an HCI_LE_-
 * Transmit_Power_Reporting event with Reason 0x00 each time the local
 * transmit power level is changed.
 *
 * When remote reporting is enabled, the Controller shall generate an HCI_LE_-
 * Transmit_Power_Reporting event with Reason 0x01 each time it becomes
 * aware that the remote transmit power level has changed.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_transmit_power_reporting_enable(const sdc_hci_cmd_le_set_transmit_power_reporting_enable_t * p_params,
                                                           sdc_hci_cmd_le_set_transmit_power_reporting_enable_return_t * p_return);

/** @brief LE Set Data Related Address Changes.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.122
 *
 * The HCI_LE_Set_Data_Related_Address_Changes command specifies
 * circumstances when the Controller shall refresh any Resolvable Private
 * Address used by the advertising set identified by the Advertising_Handle
 * parameter, whether or not the address timeout period has been reached. This
 * command may be used while advertising is enabled.
 *
 * The Change_Reasons parameter specifies the reason(s) for refreshing
 * addresses. The default when an advertising set is created, or if legacy
 * advertising commands (see Section 3.1.1) are used, is for all bits to be clear.
 *
 * If extended advertising commands (see Section 3.1.1) are being used and the
 * advertising set corresponding to the Advertising_Handle parameter does not
 * exist, or if no command specified in Table 3.2 has been used, then the
 * Controller shall return the error code Unknown Advertising Identifier (0x42).
 *
 * If legacy advertising commands are being used, the Controller shall ignore the
 * Advertising_Handle parameter.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Data_Related_Address_Changes command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_data_related_address_changes(const sdc_hci_cmd_le_set_data_related_address_changes_t * p_params);

/** @brief LE Set Default Subrate.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.123
 *
 * The HCI_LE_Set_Default_Subrate command is used by the Host to set the ini-
 * tial values for the acceptable parameters for subrating requests, as defined by
 * the HCI_LE Subrate_Request command (see Section 7.8.124), for all future
 * ACL connections where the Controller is the Central. This command does not
 * affect any existing connection.
 *
 * The parameters have the same meanings and restrictions as those in the
 * HCI_LE_Subrate_Request command.
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Set_Default_Subrate command, the
 * Controller sends the HCI_Command_Complete event to the Host.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_default_subrate(const sdc_hci_cmd_le_set_default_subrate_t * p_params);

/** @brief LE Subrate Request.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.124
 *
 * The HCI_LE_Subrate_Request command is used by a Central or a Peripheral
 * to request a change to the subrating factor and/or other parameters (see [Vol 6]
 * Part B, Section 4.5.1) applied to an existing connection using the Connection
 * Subrate Update procedure.
 *
 * The Subrate_Min and Subrate_Max parameters specify the range of accept-
 * able subrating factors being requested.
 *
 * The Max_Latency parameter specifies the maximum Peripheral latency in units
 * of subrated connection events. The same maximum shall apply irrespective of
 * the subrating factor actually chosen.
 *
 * The Continuation_Number parameter specifies the number of underlying con-
 * nection intervals to remain active after a packet (other than an empty packet) is
 * transmitted or received.
 *
 * The Supervision_Timeout parameter specifies the link supervision timeout for
 * the connection. The Supervision_Timeout, in milliseconds, shall be greater
 * than 2 × current connection interval × Subrate_Max × (Max_Latency + 1).
 *
 * If this command is issued on the Central, the following rules shall apply when
 * the Controller initiates the Connection Subrate Update procedure (see [Vol 6]
 * Part B, Section 5.1.19):
 * • The Peripheral latency shall be less than or equal to Max_Latency.
 * • The subrate factor shall be between Subrate_Min and Subrate_Max.
 * • The continuation number shall be equal to the lesser of Continuation_-
 *   Number and (subrate factor - 1).
 * • The connection supervision timeout shall be equal to Supervision_Timeout.
 *
 * If this command is issued on the Central, it also sets the acceptable parame-
 * ters for requests from the Peripheral (see [Vol 6] Part B, Section 5.1.20). The
 * acceptable parameters set by this command override those provided via the
 * HCI_LE_Set_Default_Subrate command or any values set by previous uses of
 * this command on the same connection.
 * If this command is issued on the Central before the devices have performed
 * the Feature Exchange procedure, then the Controller shall complete that pro-
 * cedure before initiating the Connection Subrate Update procedure.
 *
 * If this command is issued on the Peripheral, the following rules shall apply
 * when the Controller initiates the Connection Subrate Request procedure:
 * • The Peripheral latency shall be less than or equal to Max_Latency.
 * • The minimum and maximum subrate factors shall be between Subrate_Min
 *   and Subrate_Max.
 * • The continuation number shall be equal to the lesser of Continuation_-
 *   Number and (maximum subrate factor - 1).
 * • The connection supervision timeout shall be equal to Supervision_Timeout.
 *
 * If the Connection_Handle parameter does not identify a current ACL connec-
 * tion, the Controller shall return the error code Unknown Connection Identifier
 * (0x02).
 *
 * If the Host issues this command with parameters such that Subrate_Max ×
 * (Max_Latency + 1) is greater than 500 or the current connection interval ×
 * Subrate_Max × (Max_Latency + 1) is greater than or equal to half the
 * Supervision_Timeout parameter, the Controller shall return the error code
 * Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command with Subrate_Max less than Subrate_Min, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command with Continuation_Number greater than or
 * equal to Subrate_Max, then the Controller shall return the error code Invalid
 * HCI Command Parameters (0x12).
 *
 * If the Central's Host issues this command when the Connection Subrating
 * (Host Support) bit is not set in the Peripheral's FeatureSet, the Controller shall
 * return the error code Unsupported Remote Feature (0x1A).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Subrate_Request command, the
 * Controller sends the HCI_Command_Status event to the Host. An HCI_LE_-
 * Subrate_Change event shall be generated when the Connection Subrate
 * Update procedure has completed.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_subrate_request(const sdc_hci_cmd_le_subrate_request_t * p_params);

/** @brief LE Set Periodic Advertising Subevent Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.125
 *
 * The HCI_LE_Set_Periodic_Advertising_Subevent_Data command is used by
 * the Host to set the data for one or more subevents of PAwR in reply to an
 * HCI_LE_Periodic_Advertising_Subevent_Data_Request event. The data for a
 * subevent shall be transmitted only once.
 *
 * The Advertising_Handle parameter identifies the advertising set whose peri-
 * odic advertising subevent data is being set. If the corresponding advertising set
 * does not already exist, then the Controller shall return the error code Unknown
 * Advertising Identifier (0x42).
 *
 * The Num_Subevents parameter is the number of subevent data contained in
 * the parameter arrays.
 *
 * The Subevent arrayed parameter identifies the subevent of the PAwR that is
 * being set. If the Host provides a subevent value that is outside of the range of
 * subevents requested using the HCI_LE_Periodic_Advertising_Subevent_-
 * Data_Request event, then the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * The Response_Slot_Start and Response_Slot_Count arrayed parameters
 * identify the starting response slot and the number of response slots that are
 * expected to be used in this subevent.
 *
 * The Subevent_Data_Length arrayed parameter determines the length of the
 * Subevent_Data that is significant.
 *
 * The Subevent_Data arrayed parameter contains the advertising data to be
 * transmitted in the subevent of the advertising set. If the combined data length
 * is greater than the maximum that the Controller can transmit within the current
 * subevent interval, then all data shall be discarded and the Controller shall
 * return the error code Packet Too Long (0x45). If advertising on the LE Coded
 * PHY, then the S=8 coding shall be assumed unless the current advertising
 * parameters require the use of S=2 for an advertising physical channel, in which
 * case the S=2 coding shall be assumed for that advertising physical channel.
 * If the Subevent_Data cannot be transmitted because, for example, the subev-
 * ent where this data would have been sent has already passed or is too early,
 * then the Controller shall return the error code Too Late (0x46) or Too Early
 * (0x47) and discard the data.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Subevent_Data command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_subevent_data(const sdc_hci_cmd_le_set_periodic_adv_subevent_data_t * p_params,
                                                      sdc_hci_cmd_le_set_periodic_adv_subevent_data_return_t * p_return);

/** @brief LE Set Periodic Advertising Response Data.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.126
 *
 * The HCI_LE_Set_Periodic_Advertising_Response_Data command is used by
 * the Host to set the data for a response slot in a specific subevent of the PAwR
 * identified by the Sync_Handle. The data for a response slot shall be transmit-
 * ted only once.
 *
 * The Request_Event parameter identifies the periodic advertising event in
 * which the periodic advertising packet that the Host is responding to was
 * received.
 *
 * The Request_Subevent parameter identifies the subevent in which the periodic
 * advertising packet that the Host is responding to was received.
 *
 * The Response_Subevent parameter identifies the subevent that the response
 * shall be sent in.
 *
 * The Response_Slot parameter identifies the response slot in the subevent
 * identified by the Response_Subevent parameter in which this response data is
 * to be transmitted.
 *
 * The Response_Data_Length specifies the length of the Response_Data that is
 * significant.
 *
 * The Response_Data contains the advertising data to be transmitted in the
 * response slot. If the Response_Data_Length is greater than the maximum that
 * the Controller can transmit within the response slot, then the Response_Data
 * shall be discarded and the Controller shall return the error code Packet Too
 * Long (0x45). If advertising on the LE Coded PHY, then the S=8 coding shall be
 * assumed unless the current advertising parameters require the use of S=2 for
 * an advertising physical channel, in which case the S=2 coding shall be
 * assumed for that advertising physical channel.
 *
 * If the response slot identified by the Response_Slot parameter has passed by
 * the time this command is received by the Controller, the Controller shall return
 * the error code Too Late (0x46) and discard the Response_Data parameter.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Periodic_Advertising_Response_Data command has com-
 * pleted, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_response_data(const sdc_hci_cmd_le_set_periodic_adv_response_data_t * p_params,
                                                      sdc_hci_cmd_le_set_periodic_adv_response_data_return_t * p_return);

/** @brief LE Set Periodic Sync Subevent.
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.127
 *
 * The HCI_LE_Set_Periodic_Sync_Subevent command is used to instruct the
 * Controller to synchronize with a subset of the subevents within a PAwR train
 * identified by the Sync_Handle parameter, listen for packets sent by the peer
 * device and pass any received data up to the Host. If the Controller is synchro-
 * nized with any subevents that are not in the subset of subevents in this
 * command, then the Controller shall no longer synchronize with those subev-
 * ents.
 *
 * The Periodic_Advertising_Properties parameter indicates which fields should
 * be included in the AUX_SYNC_SUBEVENT_RSP PDUs.
 *
 * The Num_Subevents parameter identifies the number of values in the sub-
 * events parameter.
 *
 * The Subevents arrayed parameter identifies the subevents that the Controller
 * shall synchronize with.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Sync_Subevent command has completed,
 * an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_sync_subevent(const sdc_hci_cmd_le_set_periodic_sync_subevent_t * p_params,
                                                  sdc_hci_cmd_le_set_periodic_sync_subevent_return_t * p_return);

/** @brief LE Extended Create Connection [v2].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.66
 *
 * The HCI_LE_Extended_Create_Connection command is used to create an
 * ACL connection, with the local device in the Central role, to a connectable
 * advertiser. The command is also used to create an ACL connection between a
 * periodic advertiser and a synchronized device.
 *
 * If a connection is created with the local device in the Peripheral role while this
 * command is pending, then this command remains pending.
 * The Advertising_Handle parameter is used to identify the periodic advertising
 * train.
 *
 * The Subevent parameter is used to identify the subevent where a connection
 * request shall be initiated from a periodic advertising train.
 *
 * The Advertising_Handle and Subevent parameters shall be set to 0xFF if these
 * parameters are not used.
 *
 * If the Advertising_Handle and Subevent parameters are set to valid values,
 * then the Initiator_Filter_Policy, Initiating_PHYs, Scan_Interval[i], and
 * Scan_Window[i] parameters shall be ignored.
 *
 * The Initiator_Filter_Policy parameter is used to determine whether the Filter
 * Accept List is used. If the Filter Accept List is not used, the
 * Peer_Address_Type and the Peer_Address parameters specify the address
 * type and address of the advertising device to connect to.
 *
 * The Own_Address_Type parameter indicates the type of address being used
 * in the connection request packets.
 *
 * The Peer_Address_Type parameter indicates the type of address used in the
 * connectable advertisement sent by the peer.
 *
 * The Peer_Address parameter indicates the Peer’s Public Device Address,
 * Random (static) Device Address, Non-Resolvable Private Address, or
 * Resolvable Private Address depending on the Peer_Address_Type parameter.
 *
 * The Initiating_PHYs parameter indicates the PHY(s) on which the advertising
 * packets should be received on the primary advertising physical channel and
 * the PHYs for which connection parameters have been specified. The Host may
 * enable one or more initiating PHYs. If the Host specifies a PHY that is not
 * supported by the Controller, including a bit that is reserved for future use, the
 * latter should return the error code Unsupported Feature or Parameter Value
 * (0x11). The array elements of the arrayed parameters are ordered in the same
 * order as the set bits in the Initiating_PHYs parameter, starting from bit 0. The
 * number of array elements is determined by the number of bits set in the
 * Initiating_PHYs parameter. When a connectable advertisement is received and
 * a connection request is sent on one PHY, scanning on any other PHYs is
 * terminated.
 *
 * The Scan_Interval[i] and Scan_Window[i] parameters are recommendations
 * from the Host on how long (Scan_Window[i]) and how frequently
 * (Scan_Interval[i]) the Controller should scan (see [Vol 6] Part B, Section 4.5.3);
 * however the frequency and length of the scan is implementation specific. If the
 * requested scan cannot be supported by the implementation, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12). If bit 1 is
 * set in Initiating_PHYs, the values for the LE 2M PHY shall be ignored.
 * The Connection_Interval_Min[i] and Connection_Interval_Max[i] parameters
 * define the minimum and maximum allowed connection interval. The
 * Connection_Interval_Min[i] parameter shall not be greater than the
 * Connection_Interval_Max[i] parameter.
 *
 * The Max_Latency[i] parameter defines the maximum allowed Peripheral
 * latency (see [Vol 6] Part B, Section 4.5.1).
 *
 * The Supervision_Timeout[i] parameter defines the link supervision timeout for
 * the connection. The Supervision_Timeout[i] in milliseconds shall be larger than
 * (1 + Max_Latency[i]) × Connection_Interval_Max[i] × 2, where Connection_-
 * Interval_Max[i] is given in milliseconds (see [Vol 6] Part B, Section 4.5.2).
 *
 * The Min_CE_Length[i] and Max_CE_Length[i] parameters provide the
 * Controller with the expected minimum and maximum length of the connection
 * events. The Min_CE_Length[i] parameter shall be less than or equal to the
 * Max_CE_Length[i] parameter. The Controller is not required to use these
 * values.
 *
 * Where the connection is made on a PHY whose bit is not set in the Initiating_-
 * PHYs parameter, the Controller shall use the Connection_Interval_Min[i],
 * Connection_Interval_Max[i], Max_Latency[i], Supervision_Timeout[i],
 * Min_CE_Length[i], and Max_CE_Length[i] parameters for an implementation-
 * chosen PHY whose bit is set in the Initiating_PHYs parameter.
 *
 * If the Host issues this command when another HCI_LE_Extended_Create_-
 * Connection command is pending in the Controller, the Controller shall return
 * the error code Command Disallowed (0x0C).
 *
 * If the Own_Address_Type parameter is set to 0x00 and the device does not
 * have a public address, the Controller should return an error code which should
 * be Invalid HCI Command Parameters (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x01 and the random address for
 * the device has not been initialized using the HCI_LE_Set_Random_Address
 * command, the Controller shall return the error code Invalid HCI Command
 * Parameters (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x02, the Initiator_Filter_Policy
 * parameter is set to 0x00, the Controller's resolving list did not contain a
 * matching entry, and the device does not have a public address, the Controller
 * should return an error code which should be Invalid HCI Command Parameters
 * (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x02, the Initiator_Filter_Policy
 * parameter is set to 0x01, and the device does not have a public address, the
 * Controller should return an error code which should be Invalid HCI Command
 * Parameters (0x12).
 * If the Own_Address_Type parameter is set to 0x03, the Initiator_Filter_Policy
 * parameter is set to 0x00, the Controller's resolving list did not contain a
 * matching entry, and the random address for the device has not been initialized
 * using the HCI_LE_Set_Random_Address command, the Controller shall return
 * the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Own_Address_Type parameter is set to 0x03, the Initiator_Filter_Policy
 * parameter is set to 0x01, and the random address for the device has not been
 * initialized using the HCI_LE_Set_Random_Address command, the Controller
 * shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Initiating_PHYs parameter does not have at least one bit set for a PHY
 * allowed for scanning on the primary advertising physical channel, the
 * Controller shall return the error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command and the Controller has insufficient resources to
 * handle any more connections, the Controller shall return the error code
 * Connection Rejected due to Limited Resources (0x0D).
 *
 * Event(s) generated (unless masked away):
 * When the Controller receives the HCI_LE_Extended_Create_Connection
 * command, the Controller sends the HCI_Command_Status event to the Host.
 * An HCI_LE_Enhanced_Connection_Complete event shall be generated when
 * a connection is created because of this command or the connection creation
 * procedure is cancelled; until the event is generated, the command is consid-
 * ered pending. If a connection creation is discarded, then the error code Con-
 * nection Failed to be Established / Synchronization Timeout (0x3E) shall be
 * used. If a connection is created, this event shall be immediately followed by an
 * HCI_LE_Channel_Selection_Algorithm event.
 *
 * @param[in]  p_params Input parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_ext_create_conn_v2(const sdc_hci_cmd_le_ext_create_conn_v2_t * p_params);

/** @brief LE Set Periodic Advertising Parameters [v2].
 *
 * The description below is extracted from Core_v5.4,
 * Vol 4, Part E, Section 7.8.61
 *
 * The HCI_LE_Set_Periodic_Advertising_Parameters command is used by the
 * Host to set the parameters for periodic advertising.
 *
 * The Advertising_Handle parameter identifies the advertising set whose
 * periodic advertising parameters are being configured. If the corresponding
 * advertising set does not already exist, then the Controller shall return the error
 * code Unknown Advertising Identifier (0x42).
 *
 * The Periodic_Advertising_Interval_Min parameter shall be less than or equal to
 * the Periodic_Advertising_Interval_Max parameter. The Periodic_Advertising_-
 * Interval_Min and Periodic_Advertising_Interval_Max parameters should not be
 * the same value to enable the Controller to determine the best advertising
 * interval given other activities.
 *
 * If the periodic advertising interval range provided by the Host
 * (Periodic_Advertising_Interval_Min, Periodic_Advertising_Interval_Max) does
 * not overlap with the periodic advertising interval range supported by the
 * Controller, then the Controller shall return an error which should use the error
 * code Unsupported Feature or Parameter Value (0x11).
 *
 * The Periodic_Advertising_Properties parameter indicates which fields should
 * be included in the advertising packet.
 *
 * The Num_Subevents parameter identifies the number of subevents that shall
 * be transmitted for each periodic advertising event. If the Num_Subevents
 * parameter value is 0x00, then the Subevent_Interval, Response_Slot_Delay,
 * Response_Slot_Spacing, and Num_Response_Slots parameters shall be
 * ignored.
 *
 * The Subevent_Interval parameter identifies the time between the subevents of
 * PAwR. The Subevent_Interval shall be less than or equal to the
 * Periodic_Advertising_Interval_Min divided by the Num_Subevents of the
 * advertising set.
 *
 * The Response_Slot_Delay parameter identifies the time between the start of
 * the advertising packet at the start of a subevent and the start of the first
 * response slot. The Response_Slot_Delay shall be less than the
 * Subevent_Interval.
 *
 * The Response_Slot_Spacing parameter identifies the time between the start of
 * two consecutive response slots. The Response_Slot_Spacing shall be less
 * than or equal to 10 × (Subevent_Interval - Response_Slot_Delay) /
 * Num_Response_Slots. If the Num_Response_Slots parameter is set to 1, then
 * the Controller shall ignore the Response_Slot_Spacing parameter.
 *
 * The Num_Response_Slots parameter identifies the number of response slots
 * in a subevent. If the Num_Response_Slots parameter value is 0x00, then the
 * Response_Slot_Delay and Response_Slot_Spacing parameters shall be
 * ignored.
 *
 * If the advertising set identified by the Advertising_Handle specified scannable,
 * connectable, legacy, or anonymous advertising, the Controller shall return the
 * error code Invalid HCI Command Parameters (0x12).
 *
 * If the Host issues this command when periodic advertising is enabled for the
 * specified advertising set, the Controller shall return the error code Command
 * Disallowed (0x0C).
 *
 * If the Advertising_Handle does not identify an advertising set that is already
 * configured for periodic advertising and the Controller is unable to support more
 * periodic advertising at present, the Controller shall return the error code
 * Memory Capacity Exceeded (0x07).
 *
 * If the advertising set already contains periodic advertising data and the length
 * of the data is greater than the maximum that the Controller can transmit within
 * a periodic advertising interval of Periodic_Advertising_Interval_Max, the
 * Controller shall return the error code Packet Too Long (0x45). If advertising on
 * the LE Coded PHY, the S=8 coding shall be assumed unless the current
 * advertising parameters require the use of S=2 for an advertising physical
 * channel, in which case the S=2 coding shall be assumed for that advertising
 * physical channel.
 *
 * Event(s) generated (unless masked away):
 * When the HCI_LE_Set_Periodic_Advertising_Parameters command has
 * completed, an HCI_Command_Complete event shall be generated.
 *
 * @param[in]  p_params Input parameters.
 * @param[out] p_return Extra return parameters.
 *
 * @retval 0 if success.
 * @return Returns value between 0x01-0xFF in case of error.
 *         See Vol 2, Part D, Error for a list of error codes and descriptions.
 */
uint8_t sdc_hci_cmd_le_set_periodic_adv_params_v2(const sdc_hci_cmd_le_set_periodic_adv_params_v2_t * p_params,
                                                  sdc_hci_cmd_le_set_periodic_adv_params_v2_return_t * p_return);

/** @} end of HCI_VS_API */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* SDC_HCI_CMD_LE_H__ */
