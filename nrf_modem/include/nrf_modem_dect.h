/*
 * Copyright (c) 2026 Nordic Semiconductor ASA. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor ASA.
 * The use, copying, transfer or disclosure of such information is prohibited except by
 * express written agreement with Nordic Semiconductor ASA.
 */

/**
 * @file nrf_modem_dect.h
 *
 * @defgroup nrf_modem_dect DECT NR+ interface
 * @{
 * @brief Application interface for DECT NR+.
 *
 * @anchor DECT-MAC-SPEC
 * Links to pertaining specification and regulations.
 * - [DECT-2020 NR Part 2: Radio reception and transmission requirements]
 * (https://www.etsi.org/deliver/etsi_ts/103600_103699/10363602/01.05.01_60/ts_10363602v010501p.pdf)
 * - [DECT-2020 NR Part 4: MAC specification]
 * (https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/01.05.01_60/ts_10363604v010501p.pdf)
 * - [DECT Harmonised Standard for access to radio spectrum; Part 2: DECT-2020 NR ]
 * (https://etsi.org/deliver/etsi_en/301400_301499/30140602/03.01.01_60/en_30140602v030101p.pdf)
 * - [47 CFR Part 15.323]
 */

#ifndef NRF_MODEM_DECT_H
#define NRF_MODEM_DECT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <nrf_modem_toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Array size definitions */
#define NRF_MODEM_DECT_MAC_MAX_NUMBER_OF_FLOWS 6
#define NRF_MODEM_DECT_MAC_MAX_CHANNELS_IN_NETWORK_SCAN_REQ 20
#define NRF_MODEM_DECT_MAC_MAX_CHANNELS_IN_RSSI_SCAN 20
#define NRF_MODEM_DECT_MAC_MAX_CLUSTERS_IN_CLUSTER_RECEIVE_REQ 2
#define NRF_MODEM_DECT_MAC_MAX_ADDITIONAL_NW_BEACON_CHANNELS 3
#define NRF_MODEM_DECT_MAC_MAX_NUM_ADDITIONAL_PHY_CAPA 7
#define NRF_MODEM_DECT_MAC_RSSI_MEAS_SUBSLOT_COUNT (24 * 2)
#define NRF_MODEM_DECT_MAC_MAX_BAND_INFO_ELEMS 5
#define NRF_MODEM_DECT_MAC_INTEGRITY_KEY_LENGTH 16
#define NRF_MODEM_DECT_MAC_CIPHER_KEY_LENGTH 16
#define NRF_MODEM_DECT_MAC_IPV6_ADDRESS_ARRAY_SIZE 16

/**
 * @brief nRF Modem DECT MAC status codes
 * @defgroup nrf_modem_dect_mac_status definitions for operation status.
 * @{
 */
enum nrf_modem_dect_mac_err {
/** Success. */
NRF_MODEM_DECT_MAC_STATUS_OK = 0x0000,
/** Generic failure. */
NRF_MODEM_DECT_MAC_STATUS_FAIL = 0x0001,
/** Invalid parameter. */
NRF_MODEM_DECT_MAC_STATUS_INVALID_PARAM = 0x0002,
/** Request not possible in current state. */
NRF_MODEM_DECT_MAC_STATUS_NOT_ALLOWED = 0x0003,
/** Operation not possible due to missing configuration. */
NRF_MODEM_DECT_MAC_STATUS_NO_CONFIG = 0x0004,
/** Given RD not found, operation cannot be completed. */
NRF_MODEM_DECT_MAC_STATUS_RD_NOT_FOUND = 0x0005,
/** Unable to complete request due to temperature too high or low. */
NRF_MODEM_DECT_MAC_STATUS_TEMP_FAILURE = 0x0006,
/**
 * Unable to complete request due to missing radio resources to deliver the message to target RD.
 */
NRF_MODEM_DECT_MAC_STATUS_NO_RESOURCES = 0x0007,
/** Request failed due to no response from target RD. */
NRF_MODEM_DECT_MAC_STATUS_NO_RESPONSE = 0x0008,
/** Request failed because target RD rejected it. */
NRF_MODEM_DECT_MAC_STATUS_NW_REJECT = 0x0009,
/** Unable to complete request due to insufficient memory. */
NRF_MODEM_DECT_MAC_STATUS_NO_MEMORY = 0x000A,
/** No RSSI scan results for the requested channel exists. */
NRF_MODEM_DECT_MAC_STATUS_NO_RSSI_RESULTS = 0x000B,
/** Request failed because DLC discard timer (dlc_sdu_lifetime) expired. */
NRF_MODEM_DECT_MAC_STATUS_DLC_DISCARD_TIMER_EXPIRED = 0x000C,
};
/** @} */

/**
 * @brief nRF Modem DECT MAC definitions
 * @defgroup nrf_modem_dect_mac_definitions Definitions for operation parameters.
 * @{
 */

/**
 * @brief Dect system modes.
 */
enum nrf_modem_dect_control_systemmode {
	/** MAC */
	NRF_MODEM_DECT_MODE_MAC = 1,
	/** None, stack is deinitialized to lowest power mode */
	NRF_MODEM_DECT_MODE_NONE = 3,
};

/**
 * @brief Dect functional modes.
 */
enum nrf_modem_dect_control_functional_mode {
	/** Minimal mode */
	NRF_MODEM_DECT_CONTROL_FUNCTIONAL_MODE_DEACTIVATE = 0,
	/** Normal mode */
	NRF_MODEM_DECT_CONTROL_FUNCTIONAL_MODE_ACTIVATE = 1,
};

/**
 * @brief Association indication statuses. Association enables parent to route messages
 * to associated child
 */
enum nrf_modem_dect_mac_association_indication_status {
	/** Success */
	NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_SUCCESS = 0,
	/** Short ID conflict */
	NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_SHORT_ID_CONFLICT = 1,
	/** Long ID conflict */
	NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_LONG_ID_CONFLICT = 2,
	/** Max number of neighbours */
	NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_MAX_NUM_NEIGHBOURS = 3,
	/** Radio Device capability mismatch. */
	NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_RD_CAPA_MISMATCH = 4,
	/** No resources for response */
	NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_NO_RESOURCES_FOR_RESPONSE = 5,
};

/**
 * @brief Association reject causes.
 */
enum nrf_modem_dect_mac_association_reject_cause {
	/** No radio capacity */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_CAUSE_NO_RADIO_CAPACITY = 0,
	/** No hardware capacity */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_CAUSE_NO_HW_CAPACITY = 1,
	/** Conflicted short ID */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_CAUSE_CONFLICTED_SHORT_ID = 2,
	/** Security needed */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_CAUSE_SECURITY_NEEDED = 3,
	/** Other reason */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_CAUSE_OTHER_REASON = 4,
};

/**
 * @brief Association reject times. Application must wait the time before re-attempt
 * the association with same parent.
 */
enum nrf_modem_dect_mac_association_reject_time {
	/** zero seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_0S = 0,
	/** 5 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_5S = 1,
	/** 10 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_10S = 2,
	/** 30 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_30S = 3,
	/** 60 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_60S = 4,
	/** 120 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_120S = 5,
	/** 180 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_180S = 6,
	/** 300 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_300S = 7,
	/** 600 seconds */
	NRF_MODEM_DECT_MAC_ASSOCIATION_REJECT_TIME_600S = 8,
};

/**
 * @brief Association setup causes.
 */
enum nrf_modem_dect_mac_association_setup_cause {
	/** Initialization */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_INIT = 0,
	/** New set of flows */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_NEW_SET_OF_FLOWS = 1,
	/** Mobility */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_MOBILITY = 2,
	/** After error */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_AFTER_ERROR = 3,
	/** Change operating channel */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_CHANGE_OPER_CHANNEL = 4,
	/** Change operating mode */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_CHANGE_OPER_MODE = 5,
	/** Other reason */
	NRF_MODEM_DECT_MAC_ASSOCIATION_SETUP_CAUSE_OTHER_REASON = 6,
};

/**
 * @brief Cluster beacon sending periods.
 */
enum nrf_modem_dect_mac_cluster_beacon_period {
	/** 10 milliseconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_10_MS = 0,
	/** 50 milliseconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_50_MS = 1,
	/** 100 milliseconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_100_MS = 2,
	/** 500 milliseconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_500_MS = 3,
	/** 1 second */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_1000_MS = 4,
	/** 1.5 seconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_1500_MS = 5,
	/** 2 seconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_2000_MS = 6,
	/** 4 seconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_4000_MS = 7,
	/** 8 seconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_8000_MS = 8,
	/** 16 seconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_16000_MS = 9,
	/** 32 seconds */
	NRF_MODEM_DECT_MAC_CLUSTER_BEACON_PERIOD_32000_MS = 10,
};

/**
 * @brief Cluster maximum TX powers.
 */
enum nrf_modem_dect_mac_cluster_max_tx_power {
	/** -12 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_NEGATIVE_12_DB = 4,
	/** -8 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_NEGATIVE_8_DB = 5,
	/** -4 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_NEGATIVE_4_DB = 6,
	/** 0 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_0_DB = 7,
	/** 4 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_4_DB = 8,
	/** 7 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_7_DB = 9,
	/** 10 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_10_DB = 10,
	/** 13 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_13_DB = 11,
	/** 16 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_16_DB = 12,
	/** 19 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_19_DB = 13,
	/** 21 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_21_DB = 14,
	/** 23 dB */
	NRF_MODEM_DECT_MAC_CLUSTER_MAX_TX_POWER_23_DB = 15,
};

/**
 * @brief Communication methods.
 */
enum nrf_modem_dect_mac_communication_method {
	/**  Random access resource configuration */
	NRF_MODEM_DECT_MAC_COMMUNICATION_METHOD_RACH = 0,
	/** Polling */
	NRF_MODEM_DECT_MAC_COMMUNICATION_METHOD_POLLING = 1,
	/** Paging */
	NRF_MODEM_DECT_MAC_COMMUNICATION_METHOD_PAGING = 2,
};

/**
 * @brief Counts to trigger. Count to trigger can be used in decision to
 * associate to another parent.
 */
enum nrf_modem_dect_mac_count_to_trigger {
	/** 1 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_1 = 0,
	/** 2 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_2 = 1,
	/** 3 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_3 = 2,
	/** 4 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_4 = 3,
	/** 5 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_5 = 4,
	/** 6 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_6 = 5,
	/** 7 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_7 = 6,
	/** 8 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_8 = 7,
	/** 16 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_16 = 8,
	/** 24 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_24 = 9,
	/** 32 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_32 = 10,
	/** 40 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_40 = 11,
	/** 48 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_48 = 12,
	/** 64 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_64 = 13,
	/** 128 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_128 = 14,
	/** 256 */
	NRF_MODEM_DECT_MAC_COUNT_TO_TRIGGER_256 = 15,
};

/**
 * @brief Data link control (DLC) service types.
 */
enum nrf_modem_dect_dlc_service_type {
	/** Transparent mode */
	NRF_MODEM_DECT_DLC_SERVICE_TYPE_0 = 0,
	/** Segmentation mode */
	NRF_MODEM_DECT_DLC_SERVICE_TYPE_1 = 1,
	/** ARQ mode */
	NRF_MODEM_DECT_DLC_SERVICE_TYPE_2 = 2,
	/** Segmentation and ARQ mode */
	NRF_MODEM_DECT_DLC_SERVICE_TYPE_3 = 3,
};

/**
 * @brief HARQ delay subslots.
 */
enum nrf_modem_dect_mac_harq_delay_subslots {
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_0 = 0,
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_1 = 1,
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_2 = 2,
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_3 = 3,
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_4 = 4,
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_5 = 5,
	NRF_MODEM_DECT_MAC_HARQ_DELAY_SUBSLOTS_6 = 6,
};

/**
 * @brief HARQ delay times.
 */
enum nrf_modem_dect_mac_harq_delay_time {
	/** 105 us */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_105US = 0,
	/** 200 us */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_200US = 1,
	/** 400 us */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_400US = 2,
	/** 800 us */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_800US = 3,
	/** 1 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_1MS = 4,
	/** 2 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_2MS = 5,
	/** 4 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_4MS = 6,
	/** 6 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_6MS = 7,
	/** 8 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_8MS = 8,
	/** 10 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_10MS = 9,
	/** 20 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_20MS = 10,
	/** 30 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_30MS = 11,
	/** 40 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_40MS = 12,
	/** 50 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_50MS = 13,
	/** 60 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_60MS = 14,
	/** 70 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_70MS = 15,
	/** 80 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_80MS = 16,
	/** 90 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_90MS = 17,
	/** 100 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_100MS = 18,
	/** 120 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_120MS = 19,
	/** 140 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_140MS = 20,
	/** 160 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_160MS = 21,
	/** 180 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_180MS = 22,
	/** 200 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_200MS = 23,
	/** 240 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_240MS = 24,
	/** 280 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_280MS = 25,
	/** 320 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_320MS = 26,
	/** 360 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_360MS = 27,
	/** 400 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_400MS = 28,
	/** 450 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_450MS = 29,
	/** 500 ms */
	NRF_MODEM_DECT_MAC_HARQ_DELAY_TIME_500MS = 30,
};

/**
 * @brief MAC information element types
 */
enum nrf_modem_dect_mac_ie_type {
	/** Security info */
	NRF_MODEM_DECT_MAC_IE_TYPE_SECURITY_INFO = 1,
	/** Route info */
	NRF_MODEM_DECT_MAC_IE_TYPE_ROUTE_INFO = 2,
	/** Resource allocation */
	NRF_MODEM_DECT_MAC_IE_TYPE_RESOURCE_ALLOCATION = 3,
	/** Random access resource */
	NRF_MODEM_DECT_MAC_IE_TYPE_RANDOM_ACCESS_RESOURCE = 4,
	/** Radio Device capability */
	NRF_MODEM_DECT_MAC_IE_TYPE_RD_CAPABILITY = 5,
	/** Neighbouring */
	NRF_MODEM_DECT_MAC_IE_TYPE_NEIGHBOURING = 6,
	/** Load info */
	NRF_MODEM_DECT_MAC_IE_TYPE_LOAD_INFO = 9,
	/** Configuration request */
	NRF_MODEM_DECT_MAC_IE_TYPE_CONFIGURATION_REQUEST = 10,
	/** Measurement report */
	NRF_MODEM_DECT_MAC_IE_TYPE_MEASUREMENT_REPORT = 11,
	/** Radio device status */
	NRF_MODEM_DECT_MAC_IE_TYPE_RADIO_DEVICE_STATUS = 12,
};

/**
 * @brief Max modulation and coding schemes.
 */
enum nrf_modem_dect_mac_max_mcs {
	/** BPSK, N_bps=1, R=1/2 */
	NRF_MODEM_DECT_MAC_MAX_MCS_0 = 0,
	/** QPSK, N_bps=2, R=1/2 */
	NRF_MODEM_DECT_MAC_MAX_MCS_1 = 1,
	/** QPSK, N_bps=2, R=3/4 */
	NRF_MODEM_DECT_MAC_MAX_MCS_2 = 2,
	/** 16-QAM, N_bps=4, R=1/2 */
	NRF_MODEM_DECT_MAC_MAX_MCS_3 = 3,
	/** 16-QAM, N_bps=4, R=3/4 */
	NRF_MODEM_DECT_MAC_MAX_MCS_4 = 4,
	/** 64-QAM, N_bps=6, R=2/3 */
	NRF_MODEM_DECT_MAC_MAX_MCS_5 = 5,
	/** 64-QAM, N_bps=6, R=3/4 */
	NRF_MODEM_DECT_MAC_MAX_MCS_6 = 6,
	/** 64-QAM, N_bps=6, R=5/6 */
	NRF_MODEM_DECT_MAC_MAX_MCS_7 = 7,
	/** 256-QAM, N_bps=8, R=3/4 */
	NRF_MODEM_DECT_MAC_MAX_MCS_8 = 8,
	/** 256-QAM, N_bps=8, R=5/6 */
	NRF_MODEM_DECT_MAC_MAX_MCS_9 = 9,
	/** 1024-QAM, N_bps=10, R=3/4 */
	NRF_MODEM_DECT_MAC_MAX_MCS_10 = 10,
	/** 1024-QAM, N_bps=10, R=5/6 */
	NRF_MODEM_DECT_MAC_MAX_MCS_11 = 11,
};

/**
 * @brief Band group index.
 */
enum nrf_modem_dect_mac_band_group_index {
	/** RF frequencies operating near 2GHz */
	NRF_MODEM_DECT_MAC_PHY_BAND_GROUP_IDX0 = 0,
	/** RF frequencies operating near 1GHz */
	NRF_MODEM_DECT_MAC_PHY_BAND_GROUP_IDX1 = 1,
};

/**
 * @brief Bands. Countries allow different bands. Bands frequencies are defined
 * in ETSI TS 103 636-2
 */
enum nrf_modem_dect_mac_band {
	/** band 1 */
	NRF_MODEM_DECT_MAC_PHY_BAND1 = 1,
	/** Band 2 */
	NRF_MODEM_DECT_MAC_PHY_BAND2 = 2,
	/** Band 4 */
	NRF_MODEM_DECT_MAC_PHY_BAND4 = 4,
	/** Band 9 */
	NRF_MODEM_DECT_MAC_PHY_BAND9 = 9,
	/** Band 22 */
	NRF_MODEM_DECT_MAC_PHY_BAND22 = 22,
};

/**
 * @brief Network beacon sending periods.
 */
enum nrf_modem_dect_mac_nw_beacon_period {
	/** 50 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_50_MS = 0,
	/** 100 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_100_MS = 1,
	/** 500 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_500_MS = 2,
	/** 1000 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_1000_MS = 3,
	/** 1500 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_1500_MS = 4,
	/** 2000 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_2000_MS = 5,
	/** 4000 ms */
	NRF_MODEM_DECT_MAC_NW_BEACON_PERIOD_4000_MS = 6,
};

/**
 * @brief Network ID filter modes.
 */
enum nrf_modem_dect_mac_nw_id_filter_mode {
	/** No network ID filtering */
	NRF_MODEM_DECT_MAC_NW_ID_FILTER_MODE_NONE = 0,
	/** 24MSB bits of network ID must match */
	NRF_MODEM_DECT_MAC_NW_ID_FILTER_MODE_24MSB = 1,
	/** Complete network ID must match */
	NRF_MODEM_DECT_MAC_NW_ID_FILTER_MODE_32BIT = 3,
};

/**
 * @brief Random Access repeat types.
 */
enum nrf_modem_dect_mac_ra_repeat_type {
	/** Single */
	NRF_MODEM_DECT_MAC_RA_REPEAT_TYPE_SINGLE = 0,
	/** Frames */
	NRF_MODEM_DECT_MAC_RA_REPEAT_TYPE_FRAMES = 1,
	/** Subslots */
	NRF_MODEM_DECT_MAC_RA_REPEAT_TYPE_SUBSLOTS = 2,
};

/**
 * @brief Radio Device capability fourier transform scaling factors.
 *
 * Used for Orthogonal Frequency Division Multiplexing (OFDM) signal generation.
 */
enum nrf_modem_dect_mac_rd_capability_beta {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_BETA_1 = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_BETA_2 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_BETA_4 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_BETA_8 = 3,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_BETA_12 = 4,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_BETA_16 = 5,
};

/**
 * @brief Radio device capability DLC service types.
 */
enum nrf_modem_dect_mac_rd_capability_dlc_service_type {
	/** Transparent mode */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_DLC_SERVICE_TYPE_0 = 0,
	/** Segmentation mode */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_DLC_SERVICE_TYPE_1 = 1,
	/** ARQ mode */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_DLC_SERVICE_TYPE_2 = 2,
	/** Segmentation and ARQ mode */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_DLC_SERVICE_TYPE_3_2_1 = 3,
	/** All modes */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_DLC_SERVICE_TYPE_ALL = 4,
};

/**
 * @brief Radio Device capability number of HARQs.
 */
enum nrf_modem_dect_mac_rd_capability_num_harq {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_NUM_HARQ_1 = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_NUM_HARQ_2 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_NUM_HARQ_4 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_NUM_HARQ_8 = 3,
};

/**
 * @brief Max number of spatial stream transmission capabilities.
 */
enum nrf_modem_dect_mac_rd_capability_max_nss {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MAX_NSS_1 = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MAX_NSS_2 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MAX_NSS_4 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MAX_NSS_8 = 3,
};

/**
 * @brief Radio Device capability subcarrier scaling factors.
 */
enum nrf_modem_dect_mac_rd_capability_muu {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MUU_1 = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MUU_2 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MUU_4 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MUU_8 = 3,
};

/**
 * @brief Radio Device capability operating modes.
 * FT is a parent device that accepts child device associations
 * PT is a child device, not accepting further child device associations
 */
enum nrf_modem_dect_mac_rd_capability_operating_mode {
	/** Portable termination point mode only */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_OPERATING_MODE_PT_ONLY = 0,
	/** Fixed termination point mode only */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_OPERATING_MODE_FT_ONLY = 1,
	/** Supports both portable and fixed termination point mode */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_OPERATING_MODE_PT_FT = 2,
};

/**
 * @brief Radio Device power classes.
 */
enum nrf_modem_dect_mac_rd_capability_power_class {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_POWER_CLASS_1 = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_POWER_CLASS_2 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_POWER_CLASS_3 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_POWER_CLASS_4 = 3,
};

/**
 * @brief Supported specification version
 */
enum nrf_modem_dect_mac_rd_capability_release {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RELEASE_1 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RELEASE_2 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RELEASE_3 = 3,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RELEASE_4 = 4,
};

/**
 * @brief Reception capability of the TX diversity transmission.
 */
enum nrf_modem_dect_mac_rd_capability_rx_for_tx_div {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_FOR_TX_DIV_1 = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_FOR_TX_DIV_2 = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_FOR_TX_DIV_4 = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_FOR_TX_DIV_8 = 3,
};

/**
 * @brief Radio Device capability RX gain.
 */
enum nrf_modem_dect_mac_rd_capability_rx_gain {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_NEGATIVE_10_DB = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_NEGATIVE_8_DB = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_NEGATIVE_6_DB = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_NEGATIVE_4_DB = 3,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_NEGATIVE_2_DB = 4,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_0_DB = 5,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_2_DB = 6,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_4_DB = 7,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_RX_GAIN_6_DB = 8,
};

/**
 * @brief Radio Device capability soft buffer size.
 */
enum nrf_modem_dect_mac_rd_capability_soft_buffer_size {
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_16K = 0,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_25K = 1,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_32K = 2,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_64K = 3,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_128K = 4,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_256K = 5,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_512K = 6,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_1024K = 7,
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_SOFT_BUFFER_SIZE_2048K = 8,
};

/**
 * @brief Radio Device status.
 */
enum nrf_modem_dect_mac_rd_status {
	/** Memory is full */
	NRF_MODEM_DECT_MAC_RD_STATUS_MEMORY_FULL = 1,
	/** Normal operation */
	NRF_MODEM_DECT_MAC_RD_STATUS_NORMAL_OPERATION = 2,
};

/**
 * @brief Radio Device status duration.
 */
enum nrf_modem_dect_mac_rd_status_duration {
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_50MS = 0,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_100MS = 1,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_200MS = 2,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_400MS = 3,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_600MS = 4,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_800MS = 5,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_1000MS = 6,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_1500MS = 7,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_2000MS = 8,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_3000MS = 9,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_4000MS = 10,
	NRF_MODEM_DECT_MAC_RD_STATUS_DURATION_UNKNOWN = 11,
};

/**
 * @brief Association release cause.
 */
enum nrf_modem_dect_mac_release_cause {
	/** Connection termination */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_CONNECTION_TERMINATION = 0,
	/** Mobility */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_MOBILITY = 1,
	/** Radio device has been inactive for too long */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_LONG_INACTIVITY = 2,
	/** Incompatible configuration */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_INCOMPATIBLE_CONFIGURATION = 3,
	/** Insufficient hardware resources */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_INSUFFICIENT_HW_RESOURCES = 4,
	/** Insufficient radio resources */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_INSUFFICIENT_RADIO_RESOURCES = 5,
	/** Bad radio quality */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_BAD_RADIO_QUALITY = 6,
	/** Security error */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_SECURITY_ERROR = 7,
	/** Other error */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_OTHER_ERROR = 8,
	/** Other reason */
	NRF_MODEM_DECT_MAC_RELEASE_CAUSE_OTHER_REASON = 9,
	/** RACH resource failure */
	DECT_MAC_RELEASE_CAUSE_RACH_RESOURCE_FAILURE = 10,
};

/**
 * @brief Resource allocation repeat type.
 */
enum nrf_modem_dect_mac_resource_allocation_repeat_type {
	/** Single */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_REPEAT_TYPE_SINGLE = 0,
	/** Frames */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_REPEAT_TYPE_FRAMES = 1,
	/** Subslots */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_REPEAT_TYPE_SUBSLOTS = 2,
	/** Frames group */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_REPEAT_TYPE_FRAMES_GROUP = 3,
	/** Subslots group */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_REPEAT_TYPE_SUBSLOTS_GROUP = 4,
};

/**
 * @brief Allocation type.
 */
enum nrf_modem_dect_mac_resource_allocation_type {
	/** Release all */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_TYPE_RELEASE_ALL = 0,
	/** Downlink allocation */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_TYPE_DOWNLINK_ALLOCATION = 1,
	/** Uplink allocation */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_TYPE_UPLINK_ALLOCATION = 2,
	/** Uplink and downlink allocations */
	NRF_MODEM_DECT_MAC_RESOURCE_ALLOCATION_TYPE_BOTH_ALLOCATIONS = 3,
};

/**
 * @brief Schedule resource fail time.
 */
enum nrf_modem_dect_mac_sched_resource_fail_time {
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_20MS   = 2,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_50MS   = 3,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_100MS  = 4,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_200MS  = 5,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_500MS  = 6,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_1000MS = 7,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_1500MS = 8,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_3000MS = 9,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_4000MS = 10,
	NRF_MODEM_DECT_MAC_SCHED_RESOURCE_FAIL_TIME_5000MS = 11,
};

/**
 * @brief TX power.
 */
enum nrf_modem_dect_mac_tx_power {
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_40_DB = 0,
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_30_DB = 1,
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_20_DB = 2,
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_16_DB = 3,
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_12_DB = 4,
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_8_DB = 5,
	NRF_MODEM_DECT_MAC_TX_POWER_NEGATIVE_4_DB = 6,
	NRF_MODEM_DECT_MAC_TX_POWER_0_DB = 7,
	NRF_MODEM_DECT_MAC_TX_POWER_4_DB = 8,
	NRF_MODEM_DECT_MAC_TX_POWER_7_DB = 9,
	NRF_MODEM_DECT_MAC_TX_POWER_10_DB = 10,
	NRF_MODEM_DECT_MAC_TX_POWER_13_DB = 11,
	NRF_MODEM_DECT_MAC_TX_POWER_16_DB = 12,
	NRF_MODEM_DECT_MAC_TX_POWER_19_DB = 13,
	NRF_MODEM_DECT_MAC_TX_POWER_21_DB = 14,
	NRF_MODEM_DECT_MAC_TX_POWER_23_DB = 15,
};

/**
 * @brief Quality threshold.
 */
enum nrf_modem_dect_mac_quality_threshold {
	NRF_MODEM_DECT_MAC_QUALITY_THRESHOLD_0 = 0,
	NRF_MODEM_DECT_MAC_QUALITY_THRESHOLD_3 = 1,
	NRF_MODEM_DECT_MAC_QUALITY_THRESHOLD_6 = 2,
	NRF_MODEM_DECT_MAC_QUALITY_THRESHOLD_9 = 3,
};

/**
 * @brief Random access resource configuration policy.
 */
enum nrf_modem_dect_mac_rach_config_policy {
	/** Fill */
	NRF_MODEM_DECT_MAC_RACH_CONFIG_POLICY_FILL = 1,
	/** Periodic */
	NRF_MODEM_DECT_MAC_RACH_CONFIG_POLICY_PERIODIC = 2,
};

/**
 * @brief DLC SDU discard timer.
 */
enum nrf_modem_dect_dlc_sdu_lifetime {
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_0_5_MS = 1,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_1_MS = 2,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_5_MS = 3,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_10_MS = 4,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_20_MS = 5,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_30_MS = 6,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_40_MS = 7,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_50_MS = 8,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_60_MS = 9,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_70_MS = 10,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_80_MS = 11,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_90_MS = 12,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_100_MS = 13,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_150_MS = 14,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_200_MS = 15,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_250_MS = 16,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_300_MS = 17,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_500_MS = 18,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_750_MS = 19,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_1_S = 20,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_1_5_S = 21,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_2_S = 22,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_2_5_S = 23,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_3_S = 24,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_4_S = 25,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_5_S = 26,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_6_S = 27,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_8_S = 28,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_16_S = 29,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_32_S = 30,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_60_S = 31,
	NRF_MODEM_DECT_DLC_SDU_LIFETIME_INFINITY = 255,
};
/**
 * @brief Radio Device capability security modes.
 */
enum nrf_modem_dect_mac_rd_capability_mac_security {
	/** None */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MAC_SECURITY_NONE = 0,
	/** Mode 1 */
	NRF_MODEM_DECT_MAC_RD_CAPABILITY_MAC_SECURITY_MODE_1 = 1,
};

/**
 * @brief Security mode.
 */
enum nrf_modem_dect_mac_security_mode {
	/** None */
	NRF_MODEM_DECT_MAC_SECURITY_MODE_NONE = 0,
	/** Mode 1 */
	NRF_MODEM_DECT_MAC_SECURITY_MODE_1 = 1,
};

/**
 * @brief MAC IPv6 address type
 */
enum nrf_modem_dect_mac_ipv6_address_type {
	/** None */
	NRF_MODEM_DECT_MAC_IPV6_ADDRESS_TYPE_NONE = 0,
	/** Prefix of the IPv6 address */
	NRF_MODEM_DECT_MAC_IPV6_ADDRESS_TYPE_PREFIX = 1,
	/** Complete IPv6 address */
	NRF_MODEM_DECT_MAC_IPV6_ADDRESS_TYPE_FULL = 2
};

/**
 * @brief DLC flow control status.
 */
enum nrf_modem_dect_dlc_flow_control_status {
	NRF_MODEM_DECT_DLC_FLOW_CTRL_STATUS_ON = 0,
	NRF_MODEM_DECT_DLC_FLOW_CTRL_STATUS_OFF = 1,
};

/**
 * @brief nRF Modem DECT MAC information elements.
 * @defgroup nrf_modem_dect_mac_ie Definitions for DECT MAC information elements.
 * @{
 */

/**
 * @brief Route information Element.
 */
struct nrf_modem_dect_mac_route_info_ie {
	/** Route cost */
	uint8_t route_cost;
	/** Sequence number of network level application data */
	uint8_t application_sequence_number;
	/** Long RD ID of the FT at the top of the mesh tree (sink having internet connection) */
	uint32_t sink_address;
} __nrf_modem_attr_packed;

/**
 * @brief  Resource allocation descriptor in resource IEs.
 */
struct nrf_modem_dect_mac_ie_resource_alloc {
	/** The first frame where resource allocation is valid. */
	uint8_t start_subslot;
	/** Indicate whether the length is indicated in slots (true) or subslots (false). */
	bool use_slots;
	/** The length of the resource allocation in subslots or in slots. */
	uint16_t length;
} __nrf_modem_attr_packed;

/**
 * @brief Resource allocation Information element.
 */
struct nrf_modem_dect_mac_resource_allocation_ie {
	/* Flags */
	union {
		uint8_t bit_mask;
		struct {
			uint8_t additional_alloc : 1;
			uint8_t has_short_rd_id : 1;
			uint8_t sys_frame_num_has_offset : 1;
			uint8_t reserved_1 : 1;
			uint8_t has_channel_indication : 1;
			uint8_t has_fail_time : 1;
			uint8_t reserved_2 : 2;
		} flags;
	};
	/**
	 * Allocation type, see @ref nrf_modem_dect_mac_resource_allocation_type for
	 * valid options.
	 */
	uint8_t allocation_type;
	/**
	 * Allocation repeat type, see @ref nrf_modem_dect_mac_resource_allocation_repeat_type
	 * for valid options.
	 */
	uint8_t repeat_type;
	/**
	 * Allocation repetition in subslots or frames depending on repetition type.
	 * Not valid if repetition type is SINGLE.
	 */
	uint8_t repetition;
	/** Allocation validity in frames. Allocation is permanent if set to 0xFF */
	uint8_t validity;
	/**
	 * System Frame Number (SFN) offset.
	 * Start offset of allocation validity in frames from first frame of cluster period.
	 */
	uint8_t sfn_offset;
	/**
	 * Start offset of allocation validity in frames from first frame of cluster period., see
	 * @ref nrf_modem_dect_mac_sched_resource_fail_time for valid options.
	 */
	uint8_t sched_resource_fail_time;
	/** Short RD ID of the RD this allocation is for. */
	uint16_t short_id;
	/** Channel number of allocation. */
	uint16_t channel;
	/** Downlink allocation (valid if @c allocation_type is @p DOWNLINK or @p BOTH ) */
	struct nrf_modem_dect_mac_ie_resource_alloc downlink_allocation;
	/** Uplink allocation (valid if @c allocation_type is @p UPLINK or @p BOTH ) */
	struct nrf_modem_dect_mac_ie_resource_alloc uplink_allocation;

} __nrf_modem_attr_packed;

/**
 * @brief Random access resource Information Element.
 */
struct nrf_modem_dect_mac_random_access_resource_ie {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t system_frame_num_has_offset : 1;
			uint8_t has_channel_indication : 1;
			uint8_t has_response_channel_indication : 1;
			uint8_t max_rach_tx_length_in_slots : 1;
			uint8_t response_window_half_frame_delay : 1;
		} flags;
	};
	/**
	 * Allocation repeat type,
	 * see @ref nrf_modem_dect_mac_ra_repeat_type for valid options.
	 */
	uint8_t repeat_type;
	/**
	 * Minimum upper bound of random access backoff timer value randomization space, [0,7].
	 * Actual value used is 8 × @p cw_min_sig.
	 */
	uint8_t cw_min_sig;
	/**
	 * Maximum upper bound of random access backoff timer value randomization space, [0,7].
	 * Actual value used is 8 × @p cw_max_sig.
	 */
	uint8_t cw_max_sig;
	/** Maximum length of random access transmission in subslots. */
	uint8_t max_rach_tx_length;
	/** Random access response window length in subslots. */
	uint8_t response_window_length;
	/**
	 * Allocation repetition in subslots or frames depending on repetition type.
	 * Not valid if repetition type is @p SINGLE.
	 */
	uint8_t repetition;
	/** Allocation validity in frames. Allocation is permanent if set to 0xFF. */
	uint8_t validity;
	/**
	 * Start offset of allocation validity in frames from first frame of cluster period.
	 * Valid if @p system_frame_num_has_offset is set in @p flags.
	 */
	uint8_t sfn_offset;

	/**
	 * Channel number of allocation.
	 * If not present, allocation is valid for channel where the IE was received.
	 * Valid if @p has_channel_indication is set in @p flags.
	 */
	uint16_t channel;
	/**
	 * Channel for RA response message.
	 * Valid if @p has_response_channel_indication is set in @p flags.
	 */
	uint16_t response_channel;
	/** Allocation details */
	struct nrf_modem_dect_mac_ie_resource_alloc allocation;
} __nrf_modem_attr_packed;

/**
 * @brief Radio Device physical capabilities.
 */
struct nrf_modem_dect_mac_rd_capa_phy_capabilities {
	/**
	 * RD power class
	 * See @ref nrf_modem_dect_mac_rd_capability_power_class for valid options.
	 */
	uint8_t power_class;
	/**
	 * Reception capability of spatial stream transmissions.
	 * See @ref nrf_modem_dect_mac_rd_capability_max_nss for valid options.
	 */
	uint8_t max_nss;
	/**
	 * Reception capability of the TX diversity transmission.
	 * See @ref nrf_modem_dect_mac_rd_capability_rx_for_tx_div for valid options.
	 */
	uint8_t rx_for_tx_div;
	/**
	 * Receiver sensitivity capability.
	 * See @ref nrf_modem_dect_mac_rd_capability_rx_gain for valid options.
	 */
	uint8_t rx_gain;
	/**
	 * Maximum supported MCS for the reception.
	 * See @ref nrf_modem_dect_mac_max_mcs for valid options.
	 */
	uint8_t max_mcs;
	/**
	 * HARQ soft buffer size in bytes.
	 * See @ref nrf_modem_dect_mac_rd_capability_soft_buffer_size for valid options.
	 */
	uint8_t soft_buffer_size;
	/**
	 * The number of parallel HARQ processes.
	 * See @ref nrf_modem_dect_mac_rd_capability_num_harq for valid options.
	 */
	uint8_t num_harq;
	/**
	 * The processing time for creating the HARQ feedback.
	 * See @ref nrf_modem_dect_mac_harq_delay_subslots for valid options.
	 */
	uint8_t harq_delay;
} __nrf_modem_attr_packed;

/**
 * @brief Radio Device capability Information Element.
 */
struct nrf_modem_dect_mac_rd_capability_ie {
	/**
	 * True if Group assignment operation is supported.
	 */
	bool group_assign_supported;
	/**
	 * True if paging is supported.
	 */
	bool paging_supported;
	/**
	 * True if mesh is supported.
	 */
	bool mesh_supported;
	/**
	 * True if scheduled access is supported.
	 */
	bool scheduled_access_supported;
	/**
	 * True if DECT_Delay and reception of Broadcast Indication IE in RA operation is supported.
	 */
	bool dect_delay_supported;
	/**
	 * True if half-duplex operation is supported.
	 */
	bool half_dup_supported;
	/**
	 * Supported specification version.
	 * See @ref nrf_modem_dect_mac_rd_capability_release for valid options.
	 */
	uint8_t release;
	/**
	 * Operating mode
	 * See @ref nrf_modem_dect_mac_rd_capability_operating_mode for valid options.
	 */
	uint8_t operating_mode;
	/**
	 * Supported MAC security
	 * See @ref nrf_modem_dect_mac_rd_capability_mac_security for valid options.
	 */
	uint8_t mac_security;
	/**
	 * Supported DLC service type
	 * See @ref nrf_modem_dect_mac_rd_capability_dlc_service_type for valid options.
	 */
	uint8_t dlc_service_type;
	/**
	 * Physical layer capabilities for existing connection.
	 */
	struct nrf_modem_dect_mac_rd_capa_phy_capabilities phy_capabilities;
	/**
	 * Number of additional physical capabilities.
	 */
	uint8_t num_additional_phy_capabilities;
	/**
	 * Additional physical capabilities.
	 */
	struct {
		/**
		 * Subcarrier scaling factor μ, see
		 * @ref nrf_modem_dect_mac_rd_capability_muu for valid options.
		 */
		uint8_t muu;
		/**
		 * Fourier transform scaling factor β, see
		 * @ref nrf_modem_dect_mac_rd_capability_beta for valid options.
		 */
		uint8_t beta;
		/**
		 * Physical layer capabilities for μ, β
		 */
		struct nrf_modem_dect_mac_rd_capa_phy_capabilities phy_capa;

	} additional_phy_capa[NRF_MODEM_DECT_MAC_MAX_NUM_ADDITIONAL_PHY_CAPA];
} __nrf_modem_attr_packed;

/**
 * @brief Neighbouring Information Element.
 */
struct nrf_modem_dect_mac_neighbouring_ie {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_long_rd_id : 1;
			uint8_t has_rd_class : 1;
			uint8_t has_snr : 1;
			uint8_t has_rssi2 : 1;
			uint8_t has_power_constraints : 1;
			uint8_t has_next_cluster_channel : 1;
			uint8_t has_time_to_next : 1;
		} flags;
	};
	/**
	 * Network beacon period, see @ref nrf_modem_dect_mac_nw_beacon_period for valid
	 * options.
	 */
	uint8_t network_beacon_period;
	/**
	 * Cluster beacon period, see @ref nrf_modem_dect_mac_cluster_beacon_period for
	 * valid options.
	 */
	uint8_t cluster_beacon_period;
	/** RSSI-2 measurement result */
	uint8_t rssi2;
	/** Signal to noise ratio measurement result */
	uint8_t snr;
	/** Radio device class mu (μ) */
	uint8_t rd_class_mu;
	/** Radio device class beta (β) */
	uint8_t rd_class_beta;
	/** Channel of neighbour’s next cluster period */
	uint16_t next_cluster_channel;
	/** Long RD ID of the neighbour */
	uint32_t long_rd_id;
	/** Time to the start of neighbours next cluster period start */
	uint32_t time_to_next;
} __nrf_modem_attr_packed;

/**
 * @brief Load info Information Element.
 */
struct nrf_modem_dect_mac_load_info_ie {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_rd_pt_load : 1;
			uint8_t has_ie_rach_load : 1;
			uint8_t has_channel_load : 1;
		} flags;
	};
	/**
	 * Current traffic load percentage from the maximum traffic load that can be routed by the
	 * RD in FT mode, 0 = 0% and 0xFF = 100%.
	 */
	uint8_t traffic_load_percentage;
	/** Number of associated RDs in FT mode as percentage value. */
	uint8_t number_of_ft_associations;
	/** Number of associated RDs in PR mode as percentage value. */
	uint8_t number_of_pt_associations;
	/** Percentage of used RACH resources from all configured RACH resources. */
	uint8_t rach_percentage;
	/** Percentage of subslots detected as ‘free’. */
	uint8_t free_subslots_percentage;
	/** Percentage of subslots detected as ‘busy’. */
	uint8_t busy_subslots_percentage;
	/** Maximum number of allowed associated RDs. */
	uint16_t max_number_of_rd_associations;
} __nrf_modem_attr_packed;

/**
 * @brief Measurement report Information Element.
 */
struct nrf_modem_dect_mac_measurement_report_ie {
	/** Measurement report IE flag bit mask. */
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_snr : 1;
			uint8_t has_rssi_2 : 1;
			uint8_t has_rssi_1 : 1;
			uint8_t has_rx_count : 1;
		} flags;
	};

	/**
	 * If true, result is obtained from DL reception of RAR,
	 * else result is obtained from DL scheduled resources.
	 */
	bool measurement_result_from_rach;
	/** Signal to noise ratio measurement result. */
	uint8_t snr;
	/** RSSI-2 measurement result as defined in @ref DECT-MAC-SPEC "DECT-2020 NR Part 2". */
	uint8_t rssi_2;
	/** RSSI-1 measurement result as defined in @ref DECT-MAC-SPEC "DECT-2020 NR Part 2". */
	uint8_t rssi_1;
	/** Number of transmission attempts made to transmit a MAC PDU. */
	uint8_t tx_count;
} __nrf_modem_attr_packed;

/**
 * @brief Radio Device status Information Element.
 *
 * @param status Radio device status, see @ref nrf_modem_dect_mac_rd_status for valid options.
 * @param duration Duration, see @ref nrf_modem_dect_mac_rd_status_duration for valid options.
 */
struct nrf_modem_dect_mac_radio_device_status_ie {
	/** Radio device status, see @ref nrf_modem_dect_mac_rd_status for valid options. */
	uint16_t status;
	/**
	 * If status is MEMORY_FULL, this field gives estimation for duration of situation.
	 * If status is NORMAL_OPERATION, value indicates how long the abnormal situation lasted.
	 * See @ref nrf_modem_dect_mac_rd_status_duration for valid options.
	 */
	uint8_t duration;
} __nrf_modem_attr_packed;

/**
 * @brief Information Element
 */
struct nrf_modem_dect_mac_ie {
	/** IE type, one of enum nrf_modem_dect_mac_ie_type. */
	uint8_t ie_type;
	/** Information Element of type specified by @c ie_type. */
	union {
		/** Route info */
		struct nrf_modem_dect_mac_route_info_ie route_info;
		/** Resource allocation */
		struct nrf_modem_dect_mac_resource_allocation_ie resource_allocation;
		/** Random access resource */
		struct nrf_modem_dect_mac_random_access_resource_ie random_access_resource;
		/** Radio device capability */
		struct nrf_modem_dect_mac_rd_capability_ie rd_capability;
		/** Neighbouring */
		struct nrf_modem_dect_mac_neighbouring_ie neighbouring;
		/** Load info */
		struct nrf_modem_dect_mac_load_info_ie load_info;
		/** Measurement report */
		struct nrf_modem_dect_mac_measurement_report_ie measurement_report;
		/** Radio device status */
		struct nrf_modem_dect_mac_radio_device_status_ie radio_device_status;
	} ie;
} __nrf_modem_attr_packed;

/** @} nrf_modem_dect_mac_ie */
/** @} nrf_modem_dect_mac_definitions */

/**
 * @brief nRF Modem DECT MAC callback parameters
 * @defgroup nrf_modem_dect_mac_cb_params Definitions for DECT MAC callbacks.
 * @{
 */

/**
 * @brief Association response parameters.
 */
struct nrf_modem_dect_mac_association_response {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_harq_config : 1;
			uint8_t has_group_id_and_resource_tag : 1;
		} flags;
	};
	/**
	 * If false, then association request was not accepted and only fields
	 * reject_cause and reject_time are present in this message.
	 */
	bool ack_status;
	/** Cause of the rejection. */
	enum nrf_modem_dect_mac_association_reject_cause reject_cause;
	/**
	 * The time other RD must wait before sending new association requests to
	 * this RD.
	 */
	enum nrf_modem_dect_mac_association_reject_time reject_time;
	/**
	 * Number of accepted flows.
	 * Use 0x07 to accept all the flows from the association request.
	 */
	uint8_t number_of_flows;
	/** Number of HARQ processes the RD is accepting to use for its reception. */
	uint8_t number_of_rx_harq_processes;
	/**
	 * The maximum HARQ re-transmission delay that the RD is
	 *  accepting to use in reception.
	 */
	uint8_t max_number_of_harq_re_rx;
	/** Number of HARQ processes the RD is accepting to use for its transmission. */
	uint8_t number_of_tx_harq_processes;
	/**
	 * The maximum HARQ re-transmission delay that the RD is accepting to use in
	 * transmissions.
	 */
	uint8_t max_number_of_harq_re_tx;
	/**
	 * IDs of the flows that have been accepted.
	 * Not set if number_of_flows = 0x07.
	 * Size based on number_of_flows.
	 */
	uint8_t flow_id[NRF_MODEM_DECT_MAC_MAX_NUMBER_OF_FLOWS];
	/** Group ID for which the resource assignment is valid. */
	uint8_t group_id;
	/** Resource Tag value. */
	uint8_t resource_tag;
};

struct nrf_modem_dect_mac_rssi_result {
	/** The carrier number */
	uint16_t channel;

	/** Percentage of busy subslots over measured frames [0 .. 100] */
	uint8_t busy_percentage;

	/**
	 * Measurement results for subslots of one measured frame
	 *
	 * Subslot is either busy, free, possible or not measured.
	 * One bit per subslot in each array. The bit values are defined as follows:
	 *
	 * - busy:       1 - subslot was measured as busy,      0 - not busy
	 * - free:       1 - subslot was measured as free,      0 - not free
	 * - possible    1 - subslot was measured as possible,  0 - not possible
	 */
	uint8_t busy[NRF_MODEM_DECT_MAC_RSSI_MEAS_SUBSLOT_COUNT / 8];
	uint8_t free[NRF_MODEM_DECT_MAC_RSSI_MEAS_SUBSLOT_COUNT / 8];
	uint8_t possible[NRF_MODEM_DECT_MAC_RSSI_MEAS_SUBSLOT_COUNT / 8];
};

/**
 * @brief DECT MAC control_functional_mode callback parameters.
 */
struct nrf_modem_dect_mac_control_functional_mode_cb_params {
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC control_configure callback parameters.
 */
struct nrf_modem_dect_mac_control_configure_cb_params {
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC control_systemmode callback parameters.
 */
struct nrf_modem_dect_mac_control_systemmode_cb_params {
	enum nrf_modem_dect_mac_err status;
};

/** Received signal information */
struct nrf_modem_dect_mac_rx_signal_info_t {
	/** MCS index. See Table A-1 in @ref DECT-MAC-SPEC "DECT-2020 NR Part 4". */
	uint8_t mcs;
	/**
	 * Transmit power, [0,15].
	 * See table 6.2.1-3a in @ref DECT-MAC-SPEC "DECT-2020 NR Part 2".
	 */
	uint8_t transmit_power;
	/**
	 * Received signal strength indicator (RSSI-2) of PDC, value in dBm.
	 */
	int8_t rssi_2;
	/**
	 * Received signal to noise indicator (SNR) of PDC, value in dB.
	 */
	int8_t snr;
};

/** IPv6 Address configuration */
struct nrf_modem_dect_mac_ipv6_address_config_t {
	/** Address type */
	enum nrf_modem_dect_mac_ipv6_address_type type;
	/**
	 * IPv6 Address
	 * valid bytes: PREFIX - 8 bytes, FULL - 16 bytes
	 */
	uint8_t address[NRF_MODEM_DECT_MAC_IPV6_ADDRESS_ARRAY_SIZE];
};

/**
 * @brief Association operation callback parameters.
 */
struct nrf_modem_dect_mac_association_cb_params {
	/** Status of operation. */
	enum nrf_modem_dect_mac_err status;
	/** Flags. */
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_association_response : 1;
		} flags;
	};
	/** Long Radio Device ID of cluster maintainer. */
	uint32_t long_rd_id;
	/** Received signal information */
	struct nrf_modem_dect_mac_rx_signal_info_t rx_signal_info;
	/** IPv6 address configuration */
	struct nrf_modem_dect_mac_ipv6_address_config_t ipv6_config;
	/** Number of MAC IEs received */
	uint8_t number_of_ies;
	/** Received Association response */
	struct nrf_modem_dect_mac_association_response association_response;
	/** Information elements. */
	struct nrf_modem_dect_mac_ie *ies;
};

/**
 * @brief DECT MAC mac_association_release callback parameters.
 */
struct nrf_modem_dect_mac_association_release_cb_params {
	/** Long Radio Device ID */
	uint32_t long_rd_id;
};

/**
 * @brief DECT MAC mac_cluster_beacon_receive callback parameters.
 */
struct nrf_modem_dect_mac_cluster_beacon_receive_cb_params {
	/** Number of clusters */
	size_t num_clusters;
	/** Array of cluster statuses, size given by @c num_clusters */
	uint16_t cluster_status[NRF_MODEM_DECT_MAC_MAX_CLUSTERS_IN_CLUSTER_RECEIVE_REQ];
};

/**
 * @brief DECT MAC mac_cluster_beacon_receive_stop callback parameters.
 */
struct nrf_modem_dect_mac_cluster_beacon_receive_stop_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC mac_cluster_configure callback parameters.
 */
struct nrf_modem_dect_mac_cluster_configure_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC mac_cluster_info callback parameters.
 */
struct nrf_modem_dect_mac_cluster_info_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
	/** Cluster status information, valid only if status == NRF_MODEM_DECT_MAC_STATUS_OK */
	struct nrf_modem_dect_mac_cluster_status_info {
		/** Number of received Association requests. */
		uint16_t num_association_requests;
		/** Number of failed Associations, includes rejections. */
		uint16_t num_association_failures;
		/** Number of neighbors. */
		uint16_t num_neighbors;
		/** Number of neighbors in FTPT-mode. */
		uint16_t num_ftpt_neighbors;
		/** Number of received RACH PDCs. */
		uint32_t num_rach_rx_pdc;
		/** Number of RACH PCC CRC failures. */
		uint32_t num_rach_rx_pcc_crc_failures;
		/** Current RSSI scan result of cluster channel */
		struct nrf_modem_dect_mac_rssi_result  rssi_result;
	} info;
};

/**
 * @brief DECT MAC mac_neighbor_info callback parameters.
 */
struct nrf_modem_dect_mac_neighbor_info_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
	/** Long Radio ID of neighbor */
	uint32_t long_rd_id;

	/** Below info is valid only if status == NRF_MODEM_DECT_MAC_STATUS_OK */
	/** True if neighbor is associated */
	bool associated;
	/** True if neighbor operates in FT mode. */
	bool ft_mode;
	/**
	 * PT-mode neighbor: cluster channel
	 * FT-mode neighbor: Last known operating channel of it's cluster
	 */
	uint16_t channel;
	/** Network ID */
	uint32_t network_id;
	/** Time in ms since neighbor is last seen */
	uint32_t time_since_last_rx_ms;
	/** Last received signal information */
	struct nrf_modem_dect_mac_rx_signal_info_t last_rx_signal_info;
	/**
	 * Average transmission power used by neighbor (PHY type 1 transmissions).
	 * Value in dB.
	 *
	 * Valid for FT-mode neighbors only.
	 */
	int8_t beacon_average_rx_txpower;
	/**
	 * Average received signal strength indicator (RSSI-2) of PDC
	 * (PHY type 1 transmissions).
	 * Value in dBm.
	 *
	 * Valid for FT-mode neighbors only.
	 */
	int16_t beacon_average_rx_rssi_2;
	/**
	 * Average received signal to noise indicator (SNR) of PDC
	 * (PHY type 1 transmissions).
	 * Value in dB.
	 *
	 * Valid for FT-mode neighbors only.
	 */
	int8_t beacon_average_rx_snr;

	/** Neighbor status information, valid only if neighbor is associated */
	struct nrf_modem_dect_mac_neighbor_status_info {
		/**
		 * Number of missed cluster beacons
		 *
		 * Valid for FT-mode neighbors only.
		 */
		uint8_t total_missed_cluster_beacons;
		/**
		 * Current number of consecutive missed cluster beacons
		 *
		 * Valid for FT-mode neighbors only.
		 */
		uint8_t current_consecutive_missed_cluster_beacons;
		/**
		 * Number of received paging IEs
		 *
		 * Valid for FT-mode neighbors only.
		 */
		uint8_t num_rx_paging;
		/**
		 * Average MCS used (PHY type 2 transmissions)
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint8_t average_tx_mcs;
		/**
		 * Average transmission power (PHY type 2 transmissions).
		 * Value in dB.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		int8_t average_tx_txpower;
		/**
		 * Average MCS used by neighbor (PHY type 2 transmissions)
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint8_t average_rx_mcs;
		/**
		 * Average transmission power used by neighbor (PHY type 2 transmissions).
		 * Value in dB.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		int8_t average_rx_txpower;
		/**
		 * Average received signal to noise indicator (SNR) of PDC
		 * (PHY type 2 transmissions).
		 * Value in dB.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		int8_t average_rx_snr;
		/**
		 * Average received signal strength indicator (RSSI-2) of PDC
		 * (PHY type 2 transmissions).
		 * Value in dBm.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		int16_t average_rx_rssi_2;
		/**
		 * Number of RACH transmissions attempts.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_tx_attempts;
		/**
		 * Number of LBT failures.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_lbt_failures;
		/**
		 * Number of received RACH PDCs.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_rx_pdc;
		/**
		 * Number of RACH PDC CRC failures.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_rx_pdc_crc_failures;
		/**
		 * Number of missed RACH response.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_no_response;
		/**
		 * Number of received HARQ ACKs.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_harq_ack;
		/**
		 * Number of received HARQ NACKs.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_harq_nack;
		/**
		 * Number of transmitted ARQ retransmissions
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint16_t num_arq_retx;
		/**
		 * Data inactive time in ms.
		 *
		 * Note: Statistics are gathered only from PHY type 2 transmissions
		 * Beacon messages are excluded.
		 */
		uint32_t inactive_time_ms;
	} status_info;
};

/**
 * @brief DECT MAC mac_neighbor_list callback parameters.
 */
struct nrf_modem_dect_mac_neighbor_list_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
	/** Number of neighbors in Long Radio ID list */
	uint32_t num_neighbors;
	/** List of Long Radio IDs, NULL in case num_neighbors == 0 */
	uint32_t *long_rd_ids;
};

/**
 * @brief DECT MAC dlc_data_tx callback parameters.
 */
struct nrf_modem_dect_dlc_data_tx_cb_params {
	/** Transaction ID */
	uint32_t transaction_id;
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
	/** Flow ID */
	uint8_t flow_id;
	/** Long RD ID */
	uint32_t long_rd_id;
};

/**
 * @brief DECT MAC dlc_data_discard callback parameters.
 */
struct nrf_modem_dect_dlc_data_discard_cb_params {
	/** Transaction ID */
	uint32_t transaction_id;
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
	/** Flow ID */
	uint8_t flow_id;
	/** Long RD ID */
	uint32_t long_rd_id;
};

/**
 * @brief DECT MAC mac_network_beacon_configure callback parameters.
 */
struct nrf_modem_dect_mac_network_beacon_configure_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC mac_network_scan callback parameters.
 */
struct nrf_modem_dect_mac_network_scan_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
	/** Number of scanned channels */
	uint8_t num_scanned_channels;
};

/**
 * @brief DECT MAC mac_network_scan_stop callback parameters.
 */
struct nrf_modem_dect_mac_network_scan_stop_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC mac_rssi_scan callback parameters.
 */
struct nrf_modem_dect_mac_rssi_scan_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC mac_rssi_scan_stop callback parameters.
 */
struct nrf_modem_dect_mac_rssi_scan_stop_cb_params {
	/** Operation status */
	enum nrf_modem_dect_mac_err status;
};

/**
 * @brief DECT MAC operation callbacks.
 *
 * Callbacks for modem operation messages.
 *
 * @note
 * All callbacks are executed in ISR.
 * Take care of offloading any intensive operations as necessary.
 */
struct nrf_modem_dect_mac_op_callbacks {
	/**
	 * @brief Functional mode set operation callback
	 */
	void (*control_functional_mode)(
		struct nrf_modem_dect_mac_control_functional_mode_cb_params *params);
	/**
	 * @brief Configuration operation callback
	 */
	void (*control_configure)(struct nrf_modem_dect_mac_control_configure_cb_params *params);
	/**
	 * @brief Systemmode set operation callback
	 */
	void (*control_systemmode)(struct nrf_modem_dect_mac_control_systemmode_cb_params *params);
	/**
	 * @brief Association request callback
	 *
	 * See NRF_MODEM_DECT_MAC_ASSOCIATION_CB_* for bit mask values.
	 */
	void (*association)(struct nrf_modem_dect_mac_association_cb_params *params);
	/**
	 * @brief Association release request callback
	 *
	 * See NRF_MODEM_DECT_MAC_ASSOCIATION_CB_* for bit mask values.
	 */
	void (*association_release)(
		struct nrf_modem_dect_mac_association_release_cb_params *params);
	/**
	 * @brief Cluster beacon reception request callback
	 */
	void (*cluster_beacon_receive)(
		struct nrf_modem_dect_mac_cluster_beacon_receive_cb_params *params);
	/**
	 * @brief Cluster beacon reception stop request callback
	 */
	void (*cluster_beacon_receive_stop)(
		struct nrf_modem_dect_mac_cluster_beacon_receive_stop_cb_params *params);
	/**
	 * @brief Cluster configuration request callback
	 */
	void (*cluster_configure)(
		struct nrf_modem_dect_mac_cluster_configure_cb_params *params);
	/**
	 * @brief Cluster info request callback
	 */
	void (*cluster_info)(
		struct nrf_modem_dect_mac_cluster_info_cb_params *params);
	/**
	 * @brief Neighbor info request callback
	 */
	void (*neighbor_info)(
		struct nrf_modem_dect_mac_neighbor_info_cb_params *params);
	/**
	 * @brief Neighbor list request callback
	 */
	void (*neighbor_list)(
		struct nrf_modem_dect_mac_neighbor_list_cb_params *params);
	/**
	 * @brief DLC data transmission request callback
	 */
	void (*dlc_data_tx)(struct nrf_modem_dect_dlc_data_tx_cb_params *params);
	/**
	 * @brief DLC data discard request callback
	 */
	void (*dlc_data_discard)(struct nrf_modem_dect_dlc_data_discard_cb_params *params);
	/**
	 * @brief Network beacon configuration request callback
	 */
	void (*network_beacon_configure)(
		struct nrf_modem_dect_mac_network_beacon_configure_cb_params *params);
	/**
	 * @brief Network scan request callback
	 */
	void (*network_scan)(struct nrf_modem_dect_mac_network_scan_cb_params *params);
	/**
	 * @brief Network scan stop request callback
	 */
	void (*network_scan_stop)(
		struct nrf_modem_dect_mac_network_scan_stop_cb_params *params);
	/**
	 * @brief RSSI scan request callback
	 */
	void (*rssi_scan)(struct nrf_modem_dect_mac_rssi_scan_cb_params *params);
	/**
	 * @brief RSSI scan stop request callback
	 */
	void (*rssi_scan_stop)(
		struct nrf_modem_dect_mac_rssi_scan_stop_cb_params *params);
};

/**
 * @brief Network beacon. Network beacon is optional and may speed up finding the
 * cluster beacon channel.
 */
struct nrf_modem_dect_mac_network_beacon {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_max_tx_power : 1;
			uint8_t has_power_constraints : 1;
			uint8_t has_current_cluster_channel : 1;
		} flags;
	};
	/** Number of additional network beacon channels included in beacon */
	uint8_t num_network_beacon_channels;
	/** Network beacon send period */
	enum nrf_modem_dect_mac_nw_beacon_period network_beacon_period;
	/** Cluster beacon send period */
	enum nrf_modem_dect_mac_cluster_beacon_period cluster_beacon_period;
	/** Cluster maximum TX power */
	enum nrf_modem_dect_mac_cluster_max_tx_power max_tx_power;
	/** Operating channel of the cluster for the next cluster beacon period. */
	uint16_t next_cluster_channel;
	/** Current cluster channel */
	uint16_t current_cluster_channel;
	/** Network beacon channels */
	uint16_t network_beacon_channels[NRF_MODEM_DECT_MAC_MAX_ADDITIONAL_NW_BEACON_CHANNELS];
	/** Time in microseconds until the next network beacon period starts. */
	uint32_t time_to_next;
};

/**
 * @brief Association request
 *
 * @note This is received as part of an association notification, and contains an association
 *       request from another device.
 */
struct nrf_modem_dect_mac_association_request {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_power_constraints : 1;
			uint8_t has_current_cluster_channel : 1;
		} flags;
	};
	/** True if RD operates in FT mode. */
	bool ft_mode;
	/** Setup cause for the association. */
	enum nrf_modem_dect_mac_association_setup_cause setup_cause;
	/** Number of different flows RD would like to setup. */
	uint8_t number_of_flows;
	/** Number of HARQ processes the RD is requesting to use for its transmission. */
	uint8_t number_of_tx_harq_processes;
	/** The maximum HARQ re-transmission delay that the RD use in its transmissions. */
	enum nrf_modem_dect_mac_harq_delay_time tx_retransmission_delay;
	/** Number of HARQ processes the RD is requesting to use for its reception. */
	uint8_t number_of_rx_harq_processes;
	/** The maximum HARQ re-transmission delay that the RD expects in reception. */
	enum nrf_modem_dect_mac_harq_delay_time rx_retransmission_delay;
	/** IDs of the flows that RD would like to setup. */
	uint8_t flow_id[NRF_MODEM_DECT_MAC_MAX_NUMBER_OF_FLOWS];
	/** Network beacon send period */
	enum nrf_modem_dect_mac_nw_beacon_period network_beacon_period;
	/** Cluster beacon send period */
	enum nrf_modem_dect_mac_cluster_beacon_period cluster_beacon_period;
	/** Operating channel of the cluster for the next cluster beacon period. */
	uint16_t next_cluster_channel;
	/** Operating channel of the cluster for the current cluster beacon period. */
	uint16_t current_cluster_channel;
	/** Time in microseconds until the next association request period starts. */
	uint32_t time_to_next;
};

/**
 * @brief Cluster beacon. Cluster beacon is sent by FT (parent)
 * devices and include the random access window when parent can be contacted.
 *
 * @note This is received as part of a cluster beacon notification.
 */
struct nrf_modem_dect_mac_cluster_beacon {
	/** Flags */
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_max_tx_power : 1;
			uint8_t has_power_constraints : 1;
			uint8_t has_frame_offset : 1;
			uint8_t has_next_cluster_channel : 1;
			uint8_t has_time_to_next : 1;
		} flags;
	};
	/** Current System Frame number. */
	uint8_t system_frame_number;
	/** Network beacon send periodicity. */
	enum nrf_modem_dect_mac_nw_beacon_period network_beacon_period;
	/** Cluster beacon send periodicity. */
	enum nrf_modem_dect_mac_cluster_beacon_period cluster_beacon_period;
	/** COUNT_TO_TRIGGER value for mobility. */
	enum nrf_modem_dect_mac_count_to_trigger count_to_trigger;
	/** RELATIVE_QUALITY threshold for mobility. */
	enum nrf_modem_dect_mac_quality_threshold relative_quality;
	/** MIN_QUALITY threshold for initial selection of this cluster. */
	enum nrf_modem_dect_mac_quality_threshold min_quality;
	/** Cluster Max TX power. */
	enum nrf_modem_dect_mac_cluster_max_tx_power max_tx_power;
	/**
	 * Indicates how many subslots the cluster beacon is delayed from the start of the frame.
	 */
	uint16_t frame_offset;
	/** Operating channel of the cluster for the next cluster beacon period. */
	uint16_t next_cluster;
	/** Time in microseconds until the next cluster beacon period starts. */
	uint32_t time_to_next;
};

/**
 * @brief RSSI scan notification parameters.
 */
struct nrf_modem_dect_mac_rssi_scan_ntf_cb_params {
	/** The carrier number. */
	uint16_t channel;
	/** Percentage of busy subslots over measured frames [0 .. 100]. */
	uint8_t busy_percentage;
	/** Size of @c busy, @c possible and @c free arrays. */
	size_t rssi_meas_array_size;
	/** Bit is 1 if subslot is measured as busy. */
	const uint8_t *busy;
	/** Bit is 1 if subslot is measured as possible. */
	const uint8_t *possible;
	/** Bit is 1 if subslot is measured as free. */
	const uint8_t *free;
};

/**
 * @brief Cluster beacon notification parameters.
 */
struct nrf_modem_dect_mac_cluster_beacon_ntf_cb_params {
	/** Reception channel number */
	uint16_t channel;
	/** Transmitter Short Radio Device ID */
	uint16_t transmitter_short_rd_id;
	/** Transmitter Long Radio Device ID */
	uint32_t transmitter_long_rd_id;
	/** Network ID */
	uint32_t network_id;
	/** Received signal information */
	struct nrf_modem_dect_mac_rx_signal_info_t rx_signal_info;
	/** Number of MAC information elements received. */
	uint8_t number_of_ies;
	/** Received cluster beacon. */
	struct nrf_modem_dect_mac_cluster_beacon beacon;
	/** Received information elements. */
	struct nrf_modem_dect_mac_ie *ies;
};

/**
 * @brief Network beacon notification parameters.
 */
struct nrf_modem_dect_mac_network_beacon_ntf_cb_params {
	/** Reception channel number */
	uint16_t channel;
	/** Transmitter short Radio Device ID */
	uint16_t transmitter_short_rd_id;
	/** Transmitter long Radio Device ID */
	uint32_t transmitter_long_rd_id;
	/** Network ID */
	uint32_t network_id;
	/** Received signal information */
	struct nrf_modem_dect_mac_rx_signal_info_t rx_signal_info;
	/** Number of MAC information elements received */
	uint8_t number_of_ies;
	/** Received network beacon */
	struct nrf_modem_dect_mac_network_beacon beacon;
	/** Received MAC information elements */
	struct nrf_modem_dect_mac_ie *ies;
};

/**
 * @brief Association notification parameters.
 */
struct nrf_modem_dect_mac_association_ntf_cb_params {
	/** Association procedure status. */
	enum nrf_modem_dect_mac_association_indication_status status;
	/** Chosen method to deliver data to neighbor, valid only if status ==
	 *  NRF_MODEM_DECT_MAC_ASSOCIATION_INDICATION_STATUS_SUCCESS.
	 */
	enum nrf_modem_dect_mac_communication_method tx_method;
	/** Received signal information */
	struct nrf_modem_dect_mac_rx_signal_info_t rx_signal_info;
	/** Short RD ID */
	uint16_t short_rd_id;
	/** Long RD ID */
	uint32_t long_rd_id;
	/** Number of MAC information elements received */
	uint8_t number_of_ies;
	/** Received association request */
	struct nrf_modem_dect_mac_association_request association_request;
	/** Received MAC information elements */
	struct nrf_modem_dect_mac_ie *ies;
};

/**
 * @brief Association release notification parameters.
 */
struct nrf_modem_dect_mac_association_release_ntf_cb_params {
	/** Cause of the association release */
	enum nrf_modem_dect_mac_release_cause release_cause;
	/** Long Radio Device ID of neighbor */
	uint32_t long_rd_id;
};

/**
 * @brief Cluster channel load change notification parameters.
 */
struct nrf_modem_dect_mac_cluster_ch_load_change_ntf_cb_params {
	/** RSSI result */
	struct nrf_modem_dect_mac_rssi_result rssi_result;

};

/**
 * @brief Neighbor inactivity notification parameters.
 */
struct nrf_modem_dect_mac_neighbor_inactivity_ntf_cb_params {
	/** Long Radio Device ID of neighbor */
	uint32_t long_rd_id;
};

/**
 * @brief Neighbor paging failure notification parameters.
 */
struct nrf_modem_dect_mac_neighbor_paging_failure_ntf_cb_params {
	/** Long Radio Device ID of neighbor. */
	uint32_t long_rd_id;
};

/**
 * @brief Band info.
 */
struct nrf_modem_dect_mac_phy_band_info {
	/** Band group index. */
	enum nrf_modem_dect_mac_band_group_index band_group_index;
	/** Band. */
	enum nrf_modem_dect_mac_band band;
	/** Power class. See table 6.2.1-1 in @ref DECT-MAC-SPEC "DECT-2020 NR Part 2". */
	uint8_t power_class;
	/**  Minimum allowed carrier number for this band. */
	uint16_t min_carrier;
	 /** Maximum allowed carrier number for this band. */
	uint16_t max_carrier;
};

/**
 * @brief Capability notification parameters.
 */
struct nrf_modem_dect_mac_capability_ntf_cb_params {
	/** Maximum supported MCS. */
	uint8_t max_mcs;
	/** Number of supported band info elements. */
	uint8_t num_band_info_elems;
	/** Supported band info. */
	struct nrf_modem_dect_mac_phy_band_info
		band_info_elems[NRF_MODEM_DECT_MAC_MAX_BAND_INFO_ELEMS];
};

/**
 * @brief Cluster Beacon reception failure notification parameters.
 */
struct nrf_modem_dect_mac_cluster_beacon_rx_failure_ntf_cb_params {
	/** Long Radio ID of neighbor. */
	uint32_t long_rd_id;
};

/**
 * @brief IPv6 config update notification parameters.
 */
struct nrf_modem_dect_mac_ipv6_config_update_ntf_cb_params {
	/** IPv6 address configuration */
	struct nrf_modem_dect_mac_ipv6_address_config_t ipv6_config;
};

/**
 * @brief DLC data reception notification parameters.
 */
struct nrf_modem_dect_dlc_data_rx_ntf_cb_params {
	/** Flow ID */
	uint8_t flow_id;
	/** Long Radio Device ID of sender device */
	uint32_t long_rd_id;
	/** Data payload */
	void *data;
	/** Length of data payload. */
	size_t data_len;
};

struct nrf_modem_dect_dlc_flow_control_ntf_cb_params {
	/** Flow control status */
	enum nrf_modem_dect_dlc_flow_control_status status;
};

/**
 * @brief DECT MAC notification callbacks.
 *
 * Callbacks for modem notification messages.
 *
 * @note
 * All callbacks are executed in ISR.
 * Take care of offloading any intensive operations as necessary.
 */
struct nrf_modem_dect_mac_ntf_callbacks {
	/**
	 * @brief Association notification callback.
	 *
	 * @param params Association notification callback parameters.
	 */
	void (*association_ntf)(
		struct nrf_modem_dect_mac_association_ntf_cb_params *params);
	/**
	 * @brief Association release notification callback.
	 *
	 * @param params Association release notification parameters.
	 */
	void (*association_release_ntf)(
		struct nrf_modem_dect_mac_association_release_ntf_cb_params *params);
	/**
	 * @brief Cluster channel load change notification callback.
	 *
	 * @param params Cluster channel load change notification parameters.
	 */
	void (*cluster_ch_load_change_ntf)(
		struct nrf_modem_dect_mac_cluster_ch_load_change_ntf_cb_params *params);
	/**
	 * @brief Neighbor inactivity notification callback.
	 *
	 * @param params Neighbor inactivity notification parameters.
	 */
	void (*neighbor_inactivity_ntf)(
		struct nrf_modem_dect_mac_neighbor_inactivity_ntf_cb_params *params);
	/**
	 * @brief Neighbor paging failure notification callback.
	 *
	 * @param params Neighbor paging failure notification parameters.
	 */
	void (*neighbor_paging_failure_ntf)(
		struct nrf_modem_dect_mac_neighbor_paging_failure_ntf_cb_params *params);
	/**
	 * @brief RSSI scan notification callback.
	 *
	 * @details
	 * Contains measurement results for subslots of one measured frame.
	 * Subslot is either busy, free, possible or not measured.
	 * One bit per subslot in each array. The bit values are defined as follows:
	 *
	 * @param params RSSI scan notification callback parameters.
	 */
	void (*rssi_scan_ntf)(struct nrf_modem_dect_mac_rssi_scan_ntf_cb_params *params);
	/**
	 * @brief Cluster beacon notification callback.
	 *
	 * @param params Cluster beacon notification callback parameters.
	 */
	void (*cluster_beacon_ntf)(
		struct nrf_modem_dect_mac_cluster_beacon_ntf_cb_params *params);
	/**
	 * @brief Cluster beacon RX failure notification callback.
	 *
	 * @param params Cluster beacon RX failure notification callback parameters.
	 */
	void (*cluster_beacon_rx_failure_ntf)(
		struct nrf_modem_dect_mac_cluster_beacon_rx_failure_ntf_cb_params *params);
	/**
	 * @brief IPv6 config update notification callback.
	 *
	 * @param params IPv6 config update notification callback parameters.
	 */
	void (*ipv6_config_update_ntf)(
		struct nrf_modem_dect_mac_ipv6_config_update_ntf_cb_params *params);
	/**
	 * @brief Network beacon notification callback.
	 *
	 * @param params Network beacon notification callback parameters.
	 */
	void (*network_beacon_ntf)(
		struct nrf_modem_dect_mac_network_beacon_ntf_cb_params *params);
	/**
	 * @brief Capability notification callback.
	 *
	 * @param params Capability notification callback parameters.
	 */
	void (*capability_ntf)(
		struct nrf_modem_dect_mac_capability_ntf_cb_params *params);
	/**
	 * @brief DLC data reception notification.
	 *
	 * @param params DLC data reception notification callback parameters.
	 */
	void (*dlc_data_rx_ntf)(struct nrf_modem_dect_dlc_data_rx_ntf_cb_params *params);
	/**
	 * @brief DLC flow control notification
	 *
	 * @param params DLC flow control notification callback parameters.
	 */
	void (*dlc_flow_control_ntf)(struct nrf_modem_dect_dlc_flow_control_ntf_cb_params *params);
};

/** @} nrf_modem_dect_mac_cb_params */

/**
 * @brief nRF Modem DECT MAC functions and function parameters
 * @defgroup nrf_modem_dect_mac_func DECT MAC function and function parameter declarations.
 * @{
 */

/**
 * @brief Set application callbacks for MAC notifications.
 *
 * The application must set the callbacks for MAC operations before attempting other operations.
 *
 * @param op_cb Application callbacks for modem operation messages.
 * @param ntf_cb Application callbacks for modem notification messages.
 *
 * @retval 0 On success.
 * @retval -NRF_EINVAL @p op_cb or @p ntf_cb is NULL, or one or more of the callbacks are NULL.
 */
int nrf_modem_dect_mac_callback_set(const struct nrf_modem_dect_mac_op_callbacks *op_cb,
	const struct nrf_modem_dect_mac_ntf_callbacks *ntf_cb);

/**
 * @brief Set DECT system mode.
 *
 * @param dect_mode DECT system mode.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_control_systemmode_set(enum nrf_modem_dect_control_systemmode dect_mode);

/**
 * @brief Set functional mode.
 *
 * @param mode Functional mode.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_control_functional_mode_set(enum nrf_modem_dect_control_functional_mode mode);


/** @brief Control configuration parameters */
struct nrf_modem_dect_control_configure_params {
	/** Maximum TX power. */
	enum nrf_modem_dect_mac_tx_power max_tx_power;
	/** Max modulation and coding scheme (MCS). */
	enum nrf_modem_dect_mac_max_mcs max_mcs;
	/**
	 * RX RSSI level for power control, value in dBm.
	 * Device expects to receive transmissions (MCS#1) at this RSSI level.
	 */
	int8_t expected_mcs1_rx_rssi_level;
	/** Device long radio device identifier. */
	uint32_t long_rd_id;
	/** Physical band group index. */
	enum nrf_modem_dect_mac_band_group_index phy_band_group_index;
	/** Power save. */
	bool power_save;
	/** Security configuration. */
	struct {
		/** Security mode. */
		enum nrf_modem_dect_mac_security_mode mode;
		/** Security keys.*/
		uint8_t integrity_key[NRF_MODEM_DECT_MAC_INTEGRITY_KEY_LENGTH];
		uint8_t cipher_key[NRF_MODEM_DECT_MAC_CIPHER_KEY_LENGTH];
	} security;
	/**
	 * Averaging length used in statistics collection.
	 * Value given as power of two (length = 2^stats_averaging_length).
	 * Range: [1,7]
	 */
	uint8_t stats_averaging_length;
};

/**
 * @brief Set control configuration.
 *
 * @param params configuration parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_control_configure(struct nrf_modem_dect_control_configure_params *params);

/**
 * @brief TX flow configuration.
 */
struct nrf_modem_dect_mac_tx_flow_config {
	/**
	 * Flow ID
	 * See Table 6.3.4-2 in @ref DECT-MAC-SPEC "DECT-2020 NR Part 2"
	 */
	uint8_t flow_id;
	/**
	 * Priority [3, 6].
	 * Smaller number, higher priority. Valid only for user plane data flow.
	 * Higher layer signalling flows (flow_id=1, flow_id=2) have fixed priority
	 * (1 and 2 respectively).
	 */
	uint8_t priority;
	/** DLC service type. */
	enum nrf_modem_dect_dlc_service_type dlc_service_type;
	/** DLC SDU discard timer. */
	enum nrf_modem_dect_dlc_sdu_lifetime dlc_sdu_lifetime;
};

/**
 * @brief Triggers for notifying change in parent status.
 */
struct nrf_modem_dect_mac_parent_info_triggers {
	/**
	 * Consecutive cluster beacon misses before triggering Cluster Beacon RX Fail notification.
	 * Use zero to disable notifications.
	 */
	uint8_t num_beacon_rx_failures;
};

/**
 * @brief Association request parameters
 */
struct nrf_modem_dect_mac_association_params {
	/* Long Radio Device ID. */
	uint32_t long_rd_id;
	/** Network ID. */
	uint32_t network_id;
	/** Parent info notification triggers. */
	struct nrf_modem_dect_mac_parent_info_triggers info_triggers;
	/** Number of flows. */
	uint16_t num_flows;
	/** TX flow configurations. */
	struct nrf_modem_dect_mac_tx_flow_config *tx_flow_configs;
};

/**
 * @brief Association request
 *
 * @param params Association request parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_association(struct nrf_modem_dect_mac_association_params *params);

/**
 * @brief Association request parameters
 */
struct nrf_modem_dect_mac_association_release_params {
	/** Release cause. */
	enum nrf_modem_dect_mac_release_cause release_cause;
	/** Long Radio Device ID to device to release. */
	uint32_t long_rd_id;
};

/**
 * @brief Association release request
 *
 * @param params Association release request parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_association_release(
	struct nrf_modem_dect_mac_association_release_params *params);

/**
 * @brief Cluster beacon confiuration.
 */
struct nrf_modem_dect_mac_cluster_beacon_config {
		/** Cluster beacon send periodicity, [0,10] */
		enum nrf_modem_dect_mac_cluster_beacon_period cluster_beacon_period;
		/** The carrier number of cluster */
		uint16_t cluster_channel;
		/** Long Radio ID of cluster maintainer */
		uint32_t long_rd_id;
		/** Network ID of cluster */
		uint32_t network_id;
};

/**
 * @brief Cluster beacon receive request parameters.
 */
struct nrf_modem_dect_mac_cluster_beacon_receive_params {
	/** Number of cluster beacon configurations. */
	uint8_t num_configs;
	/** Cluster beacon configurations. */
	struct nrf_modem_dect_mac_cluster_beacon_config *configs;
};

/**
 * @brief Cluster beacon receive request.
 *
 * @param params Cluster beacon receive request parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_cluster_beacon_receive(
	struct nrf_modem_dect_mac_cluster_beacon_receive_params *params);

/**
 * @brief Cluster beacon receive request.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_cluster_beacon_receive_stop(void);

/**
 * @brief Association configuration.
 */
struct nrf_modem_dect_mac_association_config {
	/** Maximum number of associations to accept. */
	uint16_t max_num_neighbours;
	/** Maximum number of FT associations to accept. Not supported yet. */
	uint16_t max_num_ft_neighbours;

	/** Neighbor info notification triggers */
	struct {
		/**
		 * Neighbor inactivity timer that triggers Inactivity notification
		 * Time in milliseconds. Use 0 to disable notifications.
		 */
		uint32_t inactivity_timer;
	} neighbor_info_triggers;

	struct {
		/**
		 * Priority (smaller number, higher priority), valid only for user plane data
		 * flow higher layer signalling flows (indexes 0 and 1) have fixed priority
		 * (1 and 2 respectively) Range: [3, 6]
		 */
		uint8_t priority;

		/** DLC service type */
		enum nrf_modem_dect_dlc_service_type dlc_service_type;
		/** DLC SDU discard timer. */
		enum nrf_modem_dect_dlc_sdu_lifetime dlc_sdu_lifetime;
	} default_tx_flow_config[NRF_MODEM_DECT_MAC_MAX_NUMBER_OF_FLOWS];
};

/**
 * @brief Cluster configuration.
 */
struct nrf_modem_dect_mac_cluster_config {
	union {
		uint8_t bit_mask;
		struct {
			uint8_t has_max_tx_power : 1;
			uint8_t has_rach_config : 1;
		} flags;
	};
	/** TX power used for cluster and network beacon transmission */
	enum nrf_modem_dect_mac_tx_power beacon_tx_power;
	/** COUNT_TO_TRIGGER value for mobility */
	enum nrf_modem_dect_mac_count_to_trigger count_to_trigger;
	/** Relative quality threshold for mobility */
	enum nrf_modem_dect_mac_quality_threshold relative_quality;
	/** MIN_QUALITY threshold for initial selection of this cluster */
	enum nrf_modem_dect_mac_quality_threshold min_quality;
	/** Cluster Max TX power */
	enum nrf_modem_dect_mac_cluster_max_tx_power cluster_max_tx_power;
	/** Cluster beacon send periodicity */
	enum nrf_modem_dect_mac_cluster_beacon_period cluster_beacon_period;
	/** The carrier number to operate on */
	uint16_t cluster_channel;
	/** Network ID */
	uint32_t network_id;
	/** Random access configuration */
	struct nrf_modem_dect_mac_cluster_rach_config {
		/** RACH configuration policy */
		enum nrf_modem_dect_mac_rach_config_policy policy;
		/** Common random access parameters */
		struct nrf_modem_dect_mac_rach_common_config {
			/** Length of the RA response window in subslots, [0,15]. */
			uint8_t response_window_length;
			/**
			 * Maximum RACH transmission length in subslots, [0,MAX],
			 * MAX depends on subcarrier scaling factor.
			 */
			uint8_t max_transmission_length;
			/**
			 * Minimum upper edge for random delay.
			 * Range: [0,7]
			 * Actual value: CW_MIN = 8 * cw_min_sig
			 */
			uint8_t cw_min_sig;
			/**
			 * Maximum upper edge for random delay.
			 * Range: [0,7]
			 * Actual value: CW_MAX = 256 * cw_max_sig
			 */
			uint8_t cw_max_sig;
		} common;
		/** Configuration policy dependent parameters */
		struct nrf_modem_dect_mac_rach_config {
			struct nrf_modem_dect_mac_rach_config_fill {
				/**
				 * Percentage of cluster period to allocate for RACH resources
				 * [1,100].
				 */
				uint8_t percentage;
			} fill;
			struct nrf_modem_dect_mac_rach_config_periodic {
				/**
				 * Distance in subslots between begin of consecutive
				 * random access resource allocations.
				 */
				uint16_t period;
				/** Length in subslots of each random access resource allocation. */
				uint16_t length;
			} periodic;
		} config;
	} rach_configuration;

	/** Cluster info indication triggers */
	struct cluster_info_triggers {
		/**
		 * Busy percentage change threshold compared to last report that triggers channel
		 * load change indication.
		 * Range: [1,100], Setting to 0 disables indication.
		 */
		uint8_t busy_threshold;
	} triggers;

	/**
	 * IPv6 address configuration. If type is set to other than NONE then IPv6 address is
	 * delivered to accepted associations with the Association Response.
	 */
	struct nrf_modem_dect_mac_ipv6_address_config_t ipv6_config;
};

/**
 * @brief Cluster configure request parameters.
 */
struct nrf_modem_dect_mac_cluster_configure_params {
	/** Cluster beacon start offset in subslots relative to framing of default base time. */
	uint8_t cluster_period_start_offset;
	/** Association configuration. */
	struct nrf_modem_dect_mac_association_config *association_config;
	/** Cluster configuration. */
	struct nrf_modem_dect_mac_cluster_config *cluster_config;
};

/**
 * @brief Cluster configure request.
 *
 * @param params Request parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_cluster_configure(
	struct nrf_modem_dect_mac_cluster_configure_params *params);

struct nrf_modem_dect_mac_network_beacon_configure_params {
	/** Channel where Network beacon is sent. Setting to 0 disables ongoing beacon sending. */
	uint16_t channel;
	/** Network beacon send period */
	enum nrf_modem_dect_mac_nw_beacon_period nw_beacon_period;
	/** Number of additional network beacon channels. */
	uint8_t num_additional_channels;
	/**
	 * Additional Network beacon channels list.
	 * At most 3 additional channels can be listed. Channels are listed in sent Network beacon.
	 */
	uint16_t *additional_channels;
};

/**
 * @brief Network beacon configuration request.
 *
 * @param params configuration parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_network_beacon_configure(
	struct nrf_modem_dect_mac_network_beacon_configure_params *params);

/**
 * @brief Network scan request parameters.
 */
struct nrf_modem_dect_mac_network_scan_params {
	/** Network id filter mode */
	enum nrf_modem_dect_mac_nw_id_filter_mode network_id_filter_mode;
	/**
	 * Network ID filter, only matching beacons are delivered to application.
	 * In 24MSB mode, set only 24-bit filter.
	 */
	uint32_t network_id_filter;
	/** Time to wait in a channel for beacons, [1, 60000] ms. */
	uint16_t scan_time;
	/** Band to scan if whole band scan is requested */
	uint8_t band;
	/**
	 * Number of channels to scan.
	 * If set to 0, all channels in band indicated by band are scanned.
	 */
	uint8_t num_channels;
	/** Channels where beacons are scanned. Ignored if num_channels is 0. */
	uint16_t channel_list[NRF_MODEM_DECT_MAC_MAX_CHANNELS_IN_NETWORK_SCAN_REQ];
};

/**
 * @brief Network scan request
 *
 * @param params Network scan parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_network_scan(struct nrf_modem_dect_mac_network_scan_params *params);

/**
 * @brief Network scan stop request.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_network_scan_stop(void);

/**
 * @brief RSSI scan request parameters.
 */
struct nrf_modem_dect_mac_rssi_scan_params {
	/** Number of frames to be scanned on a channel. */
	uint8_t channel_scan_length;
	/**
	 * RSSI-1 min threshold limit as dBm [-127 .. 0].
	 * Measurement "value" is packed as "free" if (value < threshold_low)
	 */
	int8_t threshold_min;
	/**
	 * RSSI-1 max threshold limit as dBm [-127 .. 0].
	 * Measurement "value" is packed as "busy" if (value > threshold_high).
	 */
	int8_t threshold_max;
	/**
	 * Number of channels in channel_list.
	 * Set to 0 if scan of whole band indicated by band is requested.
	 */
	uint8_t num_channels;
	/** Band to scan if whole band scan is requested. */
	uint8_t band;
	/** List of channels to be scanned. Channels are scanned in given order. */
	uint16_t channel_list[NRF_MODEM_DECT_MAC_MAX_CHANNELS_IN_RSSI_SCAN];
};

/**
 * @brief RSSI scan request.
 *
 * @param params RSSI scan request parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_rssi_scan(struct nrf_modem_dect_mac_rssi_scan_params *params);

/**
 * @brief RSSI scan stop request.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_rssi_scan_stop(void);

/**
 * @brief Cluster info request.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_cluster_info(void);

/**
 * @brief Neighbor info request parameters.
 */
struct nrf_modem_dect_mac_neighbor_info_params {
	/** Neighbor Long Radio ID */
	uint32_t long_rd_id;
};

/**
 * @brief Neighbor info request.
 *
 * @param params Neighbor info request parameters.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_neighbor_info(struct nrf_modem_dect_mac_neighbor_info_params *params);

/**
 * @brief Neighbor list request.
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_mac_neighbor_list(void);

/**
 * @brief DLC TX data params
 */
struct nrf_modem_dect_dlc_data_tx_params {
	/** Transaction identifier, modem uses this for transmission confirmation. */
	uint32_t transaction_id;
	/** Flow ID used for data transmission. */
	uint8_t flow_id;
	/** Long Radio ID of target neighbor. */
	uint32_t long_rd_id;
	/** Data to send. */
	void *data;
	/** Length of data to send. */
	size_t data_len;
};

/**
 * @brief DLC data transmission request.
 *
 * @param params Transmission parameters
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_dlc_data_tx(struct nrf_modem_dect_dlc_data_tx_params *params);

/**
 * @brief DLC data discard request params.
 */
struct nrf_modem_dect_dlc_data_discard_params {
	/** Transaction identifier of the DLC data to be discarded */
	uint32_t transaction_id;
	/** Flow ID */
	uint8_t flow_id;
	/** Long Radio ID of neighbor */
	uint32_t long_rd_id;
};

/**
 * @brief DLC data discard request.
 *
 * @param params Data discard parameters
 *
 * @retval 0 On success.
 * @retval -NRF_EPERM The modem library is not initialized.
 * @retval -NRF_EFAULT Invalid callback configuration.
 * @retval -NRF_EINVAL Invalid input parameters.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_dlc_data_discard(struct nrf_modem_dect_dlc_data_discard_params *params);

/** @} nrf_modem_dect_mac_func */

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_DECT_H */
/** @} nrf_modem_dect */
