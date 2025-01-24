/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_dect_phy.h
 *
 * @defgroup nrf_modem_dect_phy DECT PHY interface
 * @{
 * @brief DECT PHY API.
 *
 * @anchor DECT-SPEC
 * Links to pertaining specification and regulations.
 * - [DECT-2020 NR Part 2: Radio reception and transmission requirements]
 * (https://www.etsi.org/deliver/etsi_ts/103600_103699/10363602/01.05.01_60/ts_10363602v010501p.pdf)
 * - [DECT-2020 NR Part 4: MAC specification]
 * (https://www.etsi.org/deliver/etsi_ts/103600_103699/10363604/01.05.01_60/ts_10363604v010501p.pdf)
 * - [DECT Harmonised Standard for access to radio spectrum; Part 2: DECT-2020 NR ]
 * (https://etsi.org/deliver/etsi_en/301400_301499/30140602/03.01.01_60/en_30140602v030101p.pdf)
 * - [47 CFR Part 15.323]
 * (https://www.ecfr.gov/current/title-47/section-15.323)
 */
#ifndef NRF_MODEM_DECT_PHY_H__
#define NRF_MODEM_DECT_PHY_H__

#include <stdint.h>
#include <nrf_modem_toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Modem time tick rate. */
#define NRF_MODEM_DECT_MODEM_TIME_TICK_RATE_KHZ 69120
/** @brief Duration of a single DECT symbol in modem time units. */
#define NRF_MODEM_DECT_SYMBOL_DURATION 2880

/** @brief Maximum LBT measurement period in modem time units. */
#define NRF_MODEM_DECT_LBT_PERIOD_MAX (110 * NRF_MODEM_DECT_SYMBOL_DURATION)
/** @brief Minimum LBT measurement period in modem time units. */
#define NRF_MODEM_DECT_LBT_PERIOD_MIN (2 * NRF_MODEM_DECT_SYMBOL_DURATION)

/** @brief Signal to noise ratio not measured. */
#define NRF_MODEM_DECT_PHY_SNR_NOT_MEASURED -32768
/** @brief Symbol RSSI not measured. */
#define NRF_MODEM_DECT_PHY_RSSI_NOT_MEASURED 0
/** @brief RSSI-2 not measured. */
#define NRF_MODEM_DECT_PHY_RSSI2_NOT_MEASURED -32768
/** @brief Temperature not measured. */
#define NRF_MODEM_DECT_PHY_TEMP_NOT_MEASURED 999
/** @brief Voltage not measured. */
#define NRF_MODEM_DECT_PHY_VOLTAGE_NOT_MEASURED 0

/** @brief Special handle to cancel all operations in @ref nrf_modem_dect_phy_cancel */
#define NRF_MODEM_DECT_PHY_HANDLE_CANCEL_ALL UINT32_MAX
/** @brief Buffer status and Channel quality indicator not used. */
#define NRF_MODEM_DECT_PHY_BS_CQI_NOT_USED 0
/** @brief Unspecified link identifier. */
#define NRF_MODEM_DECT_PHY_LINK_UNSPECIFIED ((struct nrf_modem_dect_phy_link_id){ 0 })

/**
 * @brief DECT PHY errors.
 */
enum nrf_modem_dect_phy_err {
	/**
	 * @brief Success.
	 */
	NRF_MODEM_DECT_PHY_SUCCESS = 0,
	/**
	 * @brief TX operation canceled because LBT channel was busy.
	 */
	NRF_MODEM_DECT_PHY_ERR_LBT_CHANNEL_BUSY = 0x1,
	/**
	 * @brief Operation is not an RX operation; only RX operations may be stopped.
	 */
	NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_OP = 0x2,
	/**
	 * @brief Operation not found.
	 */
	NRF_MODEM_DECT_PHY_ERR_NOT_FOUND = 0x3,
	/**
	 * @brief Modem out of memory.
	 */
	NRF_MODEM_DECT_PHY_ERR_NO_MEMORY = 0x4,
	/**
	 * @brief Request not possible in current state.
	 */
	NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED = 0x5,
	/**
	 * @brief HARQ feedback transmission successful.
	 *
	 *  Out of memory for HARQ processes, HARQ reset requested with feedback format 6.
	 */
	NRF_MODEM_DECT_PHY_OK_WITH_HARQ_RESET = 0x6,
	/**
	 * @brief Operation start time is in the past.
	 */
	NRF_MODEM_DECT_PHY_ERR_OP_START_TIME_LATE = 0x1000,
	/**
	 * @brief Listen-before-talk start time is in the past.
	 */
	NRF_MODEM_DECT_PHY_ERR_LBT_START_TIME_LATE = 0x1001,
	/**
	 * @brief RF start time is in the past.
	 */
	NRF_MODEM_DECT_PHY_ERR_RF_START_TIME_LATE = 0x1002,
	/**
	 * @brief Another operation is executing; operation cannot start immediately.
	 */
	NRF_MODEM_DECT_PHY_ERR_INVALID_START_TIME = 0x1003,
	/**
	 * @brief Another operation is scheduled to be executed at the specified time.
	 */
	NRF_MODEM_DECT_PHY_ERR_OP_SCHEDULING_CONFLICT = 0x1004,
	/**
	 * @brief Operation start time missed and can't be executed.
	 */
	NRF_MODEM_DECT_PHY_ERR_OP_TIMEOUT = 0x1005,
	/**
	 * @brief No ongoing HARQ RX; unable to transmit a HARQ response.
	 */
	NRF_MODEM_DECT_PHY_ERR_NO_ONGOING_HARQ_RX = 0x1006,
	/**
	 * @brief Could not copy parameters in time.
	 */
	NRF_MODEM_DECT_PHY_ERR_PARAMETER_UNAVAILABLE = 0x1007,
	/**
	 * @brief Could not copy payload in time.
	 */
	NRF_MODEM_DECT_PHY_ERR_PAYLOAD_UNAVAILABLE = 0x1008,
	/**
	 * @brief Operation canceled.
	 */
	NRF_MODEM_DECT_PHY_ERR_OP_CANCELED = 0x1009,
	/**
	 * @brief Operation not executed because combined operation failed.
	 */
	NRF_MODEM_DECT_PHY_ERR_COMBINED_OP_FAILED = 0x100A,
	/**
	 * @brief Operation is not allowed in the current radio mode.
	 */
	NRF_MODEM_DECT_PHY_ERR_RADIO_MODE_CONFLICT = 0x100B,
	/**
	 * @brief Unsupported carrier.
	 */
	NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_CARRIER = 0x6000,
	/**
	 * @brief Unsupported TBS size.
	 */
	NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_DATA_SIZE = 0x6001,
	/**
	 * @brief Invalid network ID.
	 */
	NRF_MODEM_DECT_PHY_ERR_INVALID_NETWORK_ID = 0x6002,
	/**
	 * @brief Invalid physical header format or type.
	 */
	NRF_MODEM_DECT_PHY_ERR_INVALID_PHY_HEADER = 0x6003,
	/**
	 * @brief Operation duration out of allowed range.
	 */
	NRF_MODEM_DECT_PHY_ERR_INVALID_DURATION = 0x6004,
	/**
	 * @brief Invalid parameter.
	 */
	NRF_MODEM_DECT_PHY_ERR_INVALID_PARAMETER = 0x6005,
	/**
	 * @brief TX power limit exceeded.
	 */
	NRF_MODEM_DECT_PHY_ERR_TX_POWER_OVER_MAX_LIMIT = 0x6006,
	/**
	 * @brief Modem error.
	 */
	NRF_MODEM_DECT_PHY_ERR_MODEM_ERROR = 0x7000,
	/**
	 * @brief Modem error, invalid RF state.
	 */
	NRF_MODEM_DECT_PHY_ERR_MODEM_ERROR_RF_STATE = 0x7001,
	/**
	 * @brief Modem temperature is equal or higher than the critical temperature limit.
	 */
	NRF_MODEM_DECT_PHY_ERR_TEMP_HIGH = 0x7002,
	/**
	 * @brief Failed to lock device after production.
	 */
	NRF_MODEM_DECT_PHY_ERR_PROD_LOCK = 0x7003,
};

/**
 * @brief Modem PHY capabilities.
 */
struct nrf_modem_dect_phy_capability {
	/**
	 * @brief DECT NR+ version.
	 */
	uint8_t dect_version;
	/**
	 * @brief Number of elements in @ref variant.
	 */
	uint8_t variant_count;
	/**
	 * @brief Capability variants.
	 */
	struct {
		/**
		 * @brief Supported reception capability of spatial stream transmissions.
		 */
		uint8_t rx_spatial_streams;
		/**
		 * @brief Reception capability of TX diversity transmission.
		 */
		uint8_t rx_tx_diversity;
		/**
		 * @brief Maximum supported modulation and coding scheme for reception.
		 */
		uint8_t mcs_max;
		/**
		 * @brief HARQ soft buffer size.
		 */
		uint32_t harq_soft_buf_size;
		/**
		 * @brief Maximum number of HARQ processes.
		 */
		uint8_t harq_process_count_max;
		/**
		 * @brief HARQ feedback delay, in subslots.
		 */
		uint8_t harq_feedback_delay;
		/**
		 * @brief Subcarrier scaling factor.
		 */
		uint8_t mu;
		/**
		 * @brief Fourier transform scaling factor.
		 */
		uint8_t beta;
	} variant[];
};

/**
 * @brief Band information.
 */
struct nrf_modem_dect_phy_band {
	/**
	 * @brief Band group index.
	 *
	 * Value 0 refers to RF frequencies operating near 2GHz and
	 * value 1 to RF frequencies near 1 GHz.
	 */
	uint8_t band_group_index;
	/**
	 * @brief Band number.
	 */
	uint8_t band_number;
	/**
	 * @brief Receiver sensitivity capability in dB.
	 */
	int8_t rx_gain;
	/**
	 * @brief Power class.
	 */
	uint8_t power_class;
	/**
	 * @brief Minimum carrier number.
	 */
	uint16_t min_carrier;
	/**
	 * @brief Maximum carrier number.
	 */
	uint16_t max_carrier;
};

/**
 * @brief Reception mode.
 */
enum nrf_modem_dect_phy_rx_mode {
	/**
	 * @brief Continuous reception.
	 *
	 * The reception is continued automatically after PDC reception.
	 */
	NRF_MODEM_DECT_PHY_RX_MODE_CONTINUOUS,
	/**
	 * @brief Semi-continuous reception.
	 *
	 * Similar to continuous reception, but will stop after receiving a unicast PDC.
	 */
	NRF_MODEM_DECT_PHY_RX_MODE_SEMICONTINUOUS,
	/**
	 * @brief Single-shot reception.
	 *
	 * The reception is terminated automatically after the first successful PDC reception.
	 */
	NRF_MODEM_DECT_PHY_RX_MODE_SINGLE_SHOT,
};

/**
 * @brief RSSI reporting interval.
 */
enum nrf_modem_dect_phy_rssi_interval {
	/**
	 * @brief Do not measure RSSI.
	 *
	 * This value is only allowed for RX operations.
	 */
	NRF_MODEM_DECT_PHY_RSSI_INTERVAL_OFF = 0,
	/**
	 * @brief RSSI reporting interval is 12 slots (5ms).
	 */
	NRF_MODEM_DECT_PHY_RSSI_INTERVAL_12_SLOTS = 12,
	/**
	 * @brief RSSI reporting interval is 24 slots (10ms).
	 */
	NRF_MODEM_DECT_PHY_RSSI_INTERVAL_24_SLOTS = 24,
};

/**
 * @brief Physical header type 1.
 *
 * See 6.2 in @ref DECT-SPEC "DECT-2020 NR Part 4".
 */
struct nrf_modem_dect_phy_hdr_type_1 {
	/**
	 * @brief Packet length in subslots or slots.
	 */
	uint8_t packet_length : 4;
	/**
	 * @brief Indicates whether the packet_length is given in subslots (0) or slots (1).
	 */
	uint8_t packet_length_type : 1;
	/**
	 * @brief Format of the control header Type 1.
	 */
	uint8_t header_format : 3;
	/**
	 * @brief Short network ID
	 */
	uint8_t short_network_id;
	/**
	 * @brief Transmitter ID high byte
	 */
	uint8_t transmitter_id_hi;
	/**
	 * @brief Transmitter ID low byte
	 */
	uint8_t transmitter_id_lo;
	/**
	 * @brief Defines the transmissions modulation and coding scheme.
	 */
	uint8_t df_mcs : 3;
	/**
	 * @brief Reserved
	 */
	uint8_t reserved : 1;
	/**
	 * @brief Transmit power
	 */
	uint8_t transmit_power : 4;
} __nrf_modem_attr_packed;

/**
 * @brief Feedback format types.
 *
 * See 6.2.2 in @ref DECT-SPEC "DECT-2020 NR Part 4".
 */
union nrf_modem_dect_phy_feedback {
	struct {
		uint8_t transmission_feedback0 : 1;
		uint8_t harq_process_number0 : 3;
		uint8_t format : 4;
		uint8_t cqi : 4;
		uint8_t buffer_status : 4;
	} format1;
	struct {
		uint8_t mimo_feedback : 1;
		uint8_t codebook_index : 3;
		uint8_t format : 4;
		uint8_t cqi : 4;
		uint8_t buffer_status : 4;
	} format2;
	struct {
		uint8_t transmission_feedback0 : 1;
		uint8_t harq_process_number0 : 3;
		uint8_t format : 4;
		uint8_t cqi : 4;
		uint8_t transmission_feedback1 : 1;
		uint8_t harq_process_number1 : 3;
	} format3;
	struct {
		uint8_t harq_feedback_bitmap_proc3 : 1;
		uint8_t harq_feedback_bitmap_proc2 : 1;
		uint8_t harq_feedback_bitmap_proc1 : 1;
		uint8_t harq_feedback_bitmap_proc0 : 1;
		uint8_t format : 4;
		uint8_t cqi : 4;
		uint8_t harq_feedback_bitmap_proc7 : 1;
		uint8_t harq_feedback_bitmap_proc6 : 1;
		uint8_t harq_feedback_bitmap_proc5 : 1;
		uint8_t harq_feedback_bitmap_proc4 : 1;
	} format4;
	struct {
		uint8_t transmission_feedback : 1;
		uint8_t harq_process_number : 3;
		uint8_t format : 4;
		uint8_t codebook_index : 6;
		uint8_t mimo_feedback : 2;
	} format5;
	struct {
		uint8_t reserved : 1;
		uint8_t harq_process_number : 3;
		uint8_t format : 4;
		uint8_t cqi : 4;
		uint8_t buffer_status : 4;
	} format6;
} __nrf_modem_attr_packed;

/**
 * @brief Physical header type 2.
 *
 * See 6.2 in @ref DECT-SPEC "DECT-2020 NR Part 4".
 */
struct nrf_modem_dect_phy_hdr_type_2 {
	/**
	 * @brief Packet length in subslots or slots.
	 */
	uint8_t packet_length : 4;
	/**
	 * @brief Indicates whether the packet_length is given in subslots (0) or slots (1).
	 */
	uint8_t packet_length_type : 1;
	/**
	 * @brief Format of the control header Type 2.
	 */
	uint8_t header_format : 3;
	/**
	 * @brief Short network ID
	 */
	uint8_t short_network_id;
	/**
	 * @brief Transmitter ID high byte
	 */
	uint8_t transmitter_id_hi;
	/**
	 * @brief Transmitter ID low byte
	 */
	uint8_t transmitter_id_lo;
	/**
	 * @brief Defines the transmissions modulation and coding scheme.
	 */
	uint8_t df_mcs : 4;
	/**
	 * @brief Transmit power.
	 */
	uint8_t transmit_power : 4;
	/**
	 * @brief Receiver ID high byte.
	 */
	uint8_t receiver_id_hi;
	/**
	 * @brief Receiver ID low byte.
	 */
	uint8_t receiver_id_lo;
	/**
	 * @brief HARQ process number.
	 *
	 * Valid only for header format: 000.
	 */
	uint8_t df_harq_process_num : 3;
	/**
	 * @brief New data indication.
	 *
	 * Valid only for header format: 000.
	 */
	uint8_t df_new_data_indication : 1;
	/**
	 * @brief Redundancy version.
	 *
	 * Valid only for header format: 000.
	 */
	uint8_t df_redundancy_version : 2;
	/**
	 * @brief Number of spatial streams.
	 */
	uint8_t num_spatial_streams : 2;
	/**
	 * @brief Feedback info.
	 */
	union nrf_modem_dect_phy_feedback feedback;
} __nrf_modem_attr_packed;

/**
 * @brief Physical header.
 *
 * See 6.2.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
 */
union nrf_modem_dect_phy_hdr {
	/**
	 * @brief Header type 1.
	 */
	uint8_t type_1[5];
	/**
	 * @brief Header type 2.
	 */
	uint8_t type_2[10];
	/**
	 * @brief Header type 1, bitfield accessor.
	 */
	struct nrf_modem_dect_phy_hdr_type_1 hdr_type_1;
	/**
	 * @brief Header type 2, bitfield accessor.
	 */
	struct nrf_modem_dect_phy_hdr_type_2 hdr_type_2;
};

/**
 * @brief Physical header status.
 */
enum nrf_modem_dect_phy_hdr_status {
	/**
	 * @brief PHY header is valid; PDC can be received.
	 */
	NRF_MODEM_DECT_PHY_HDR_STATUS_VALID = 0,
	/**
	 * @brief PHY header is invalid; PDC can't be received.
	 */
	NRF_MODEM_DECT_PHY_HDR_STATUS_INVALID = 1,
	/**
	 * @brief PHY header is valid, but RX operation ends before PDC reception.
	 */
	NRF_MODEM_DECT_PHY_HDR_STATUS_VALID_RX_END = 2,
};

/**
 * @brief Radio link identifier.
 *
 * Identifies a radio link using fields that are available in Physical Layer Control Field.
 */
struct nrf_modem_dect_phy_link_id {
	/**
	 * @brief Short Network ID.
	 *
	 * See 6.2.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint8_t short_network_id;
	/**
	 * @brief Short Radio Device ID.
	 *
	 * Big endian encoding.
	 * Used for transmitter identity in @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint16_t short_rd_id;
};

/**
 * @brief Specification for reception filter.
 *
 * For any enabled filter element the received PCC packet must match the corresponding filter
 * element value for the PCC to be accepted. PCC rejection will stop PDC reception and return
 * processing to synchronization search.
 *
 * See 6.2.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
 */
struct nrf_modem_dect_phy_rx_filter {
	/**
	 * @brief Short Network ID.
	 *
	 * The Short Network ID field of a received PCC MAC Physical Header
	 * must match this value for the PCC to be accepted.
	 *
	 * See 6.2.1 and 4.2.3.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint8_t short_network_id;
	/**
	 * @brief Short Network ID filter activation.
	 *
	 * Set to 0 to disable Short Network ID filtering.
	 * Set to 1 to enable Short Network ID filtering.
	 */
	uint8_t is_short_network_id_used;
	/**
	 * @brief Receiver Identity.
	 *
	 * If set to 0, filtering based on Receiver Identity is disabled.
	 *
	 * For any other value, the Receiver Identify field in a packet's MAC Physical Header
	 * must match the value (or contain the broadcast ID) for the packet to be accepted.
	 *
	 * See 6.2.1 and 4.2.3.3 in @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint16_t receiver_identity;
};

/**
 * @brief RX operation.
 */
struct nrf_modem_dect_phy_rx_params {
	/**
	 * @brief Operation start time as modem time.
	 *
	 * If zero, the operation will be executed immediately.
	 *
	 * This kind of scheduling can only be done when the modem is idle.
	 */
	uint64_t start_time;
	/**
	 * @brief Handle for the operation.
	 *
	 * An application-defined handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Network ID for scrambling.
	 *
	 * Can be zero if receiving only beacons.
	 */
	uint32_t network_id;
	/**
	 * @brief Reception mode.
	 */
	enum nrf_modem_dect_phy_rx_mode mode;
	/**
	 * @brief RSSI measurement reporting interval, in slots.
	 */
	enum nrf_modem_dect_phy_rssi_interval rssi_interval;
	/**
	 * @brief Radio link identifier.
	 *
	 * Set to @ref NRF_MODEM_DECT_PHY_LINK_UNSPECIFIED if expecting to receive from any Radio
	 * Device within the primary network (either the primary link or any downlink device).
	 *
	 * Set to the link id of a secondary link if expecting to receive over that link.
	 */
	struct nrf_modem_dect_phy_link_id link_id;
	/**
	 * @brief Expected antenna signal level of the reception.
	 *
	 * Values in range [-128, -1] are dBm values for the expected reception level.
	 *
	 * Value 0 is interpreted as MAC having no knowledge of the expected level. This initiates a
	 * fast AGC algorithm that tries to adjust the reception gain level based on the received
	 * signal level.
	 *
	 * If the duration of the RX operation is long enough to receive multiple slots,
	 * this value will only be used for the first successful reception. Subsequent receptions
	 * during the same RX operation are initiated using a level that has been adjusted based on
	 * the previous successful reception (slow AGC).
	 */
	int8_t rssi_level;
	/**
	 * @brief The absolute channel frequency number for the reception.
	 *
	 * See 5.4.2 of @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	uint16_t carrier;
	/**
	 * @brief Duration of reception in modem time units.
	 */
	uint32_t duration;
	/**
	 * @brief Reception filter for this operation.
	 */
	struct nrf_modem_dect_phy_rx_filter filter;
};

/**
 * @brief TX operation.
 */
struct nrf_modem_dect_phy_tx_params {
	/**
	 * @brief Operation start time as modem time.
	 *
	 * If zero, the operation will be executed immediately.
	 *
	 * This kind of scheduling can only be done when the modem is idle.
	 */
	uint64_t start_time;
	/**
	 * @brief Handle for the operation.
	 *
	 * An application-defined handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Network ID for scrambling.
	 *
	 * Must not be zero.
	 */
	uint32_t network_id;
	/**
	 * @brief Physical layer control field type.
	 *
	 * Valid values are 0 and 1 corresponding Type 1 and Type 2.
	 *
	 * See 6.2.1 of @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint8_t phy_type;
	/**
	 * @brief Listen before talk maximum RSSI threshold, in dBm.
	 *
	 * The measured RSSI-1 needs to be smaller than this value for the channel
	 * to be considered available for TX.
	 *
	 * @note
	 * RSSI measurements during LBT are quantized to implementation defined limits,
	 * so the value given here may not be exactly the applied LBT RSSI threshold.
	 *
	 * See 5.1.2 of @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	int8_t lbt_rssi_threshold_max;
	/**
	 * @brief The absolute channel frequency number for the transmission.
	 *
	 * See 5.4.2 of @ref DECT-SPEC "DECT-2020 NR Part 2".
	 *
	 * @warning
	 * You must ensure that you are complying with applicable regulations when transmitting.
	 * If you are operating your device in Europe, you must refer to table 4.3.2.3-2
	 * of @ref DECT-SPEC "DECT Harmonised Standard".
	 * If you are operating your device in the United States, you must refer to
	 * @ref DECT-SPEC "47 CFR Part 15.323".
	 */
	uint16_t carrier;
	/**
	 * @brief Listen before talk period in modem time units.
	 *
	 * This is the required duration for the channel to be assessed as "free" or "available"
	 * before starting transmission. This duration is divided into as many as 64 separate
	 * integration periods, with each period being a multiple of the symbol duration.
	 * Each integration period is of equal length, up to a maximum of 7 symbols.
	 * The maximum number of integration periods is utilized. For instance, LBT durations of
	 * up to 64 symbols are divided into integration periods of one symbol each, and
	 * LBT durations of 65-128 symbols are divided into two-symbol integration periods.
	 * The LBT duration may be adjusted slightly to meet these requirements without exceeding
	 * the specified LBT length, for example, a duration of 71 symbols would be adjusted to
	 * 70 symbols (35 periods of 2 symbols each).
	 *
	 * If the channel is initially found to be "busy," the modem will cease further measurement
	 * of the channel and will report an error instead. It is then the responsibility of the
	 * higher layer to reschedule the transmission.
	 *
	 * If set to zero, LBT is not used. Otherwise the acceptable value range is
	 * [@ref NRF_MODEM_DECT_LBT_PERIOD_MIN, @ref NRF_MODEM_DECT_LBT_PERIOD_MAX].
	 */
	uint32_t lbt_period;
	/**
	 * @brief Physical header corresponding to the physical layer control field type.
	 */
	union nrf_modem_dect_phy_hdr *phy_header;
	/**
	 * @brief Buffer status and channel quality indicator.
	 *
	 * Only used for HARQ transmissions, with @ref nrf_modem_dect_phy_tx_harq.
	 *
	 * Set to @ref NRF_MODEM_DECT_PHY_BS_CQI_NOT_USED otherwise.
	 *
	 * See 6.2.2-3 and 6.2.2-4 of @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint8_t bs_cqi;
	/**
	 * @brief The data to be sent.
	 */
	uint8_t *data;
	/**
	 * @brief Size of the data in bytes.
	 */
	uint32_t data_size;
};

/**
 * @brief Combined TX and RX operation.
 */
struct nrf_modem_dect_phy_tx_rx_params {
	/**
	 * @brief TX operation.
	 */
	struct nrf_modem_dect_phy_tx_params tx;
	/**
	 * @brief RX operation.
	 *
	 * The start time of the RX operation is relative to the end
	 * of the TX operation and it must include the guard time.
	 */
	struct nrf_modem_dect_phy_rx_params rx;
};

/**
 * @brief RSSI-1 measurement operation.
 */
struct nrf_modem_dect_phy_rssi_params {
	/**
	 * @brief Operation start time as modem time.
	 *
	 * If zero, the operation will be executed immediately.
	 *
	 * This kind of scheduling can only be done when the modem is idle.
	 */
	uint64_t start_time;
	/**
	 * @brief Handle for the operation.
	 *
	 * An application-defined handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief The absolute channel frequency number for the RSSI measurement operation.
	 *
	 * See 5.4.2 of @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	uint16_t carrier;
	/**
	 * @brief Duration of measurement in subslots.
	 *
	 * The maximum duration is 297600 subslots.
	 * The minimum duration is 1 subslot.
	 */
	uint32_t duration;
	/**
	 * @brief RSSI measurements reporting interval, in slots.
	 */
	enum nrf_modem_dect_phy_rssi_interval reporting_interval;
};

/**
 * @brief Radio modes.
 *
 * Different radio modes yield different performance and power consumption.
 */
enum nrf_modem_dect_phy_radio_mode {
	/**
	 * @brief Low latency.
	 *
	 * This mode has the lowest latency, the best RX/TX switching performance,
	 * and the highest power consumption. This is the only mode that supports
	 * starting operations immediately, that is, operations whose configured
	 * start time is zero.
	 */
	NRF_MODEM_DECT_PHY_RADIO_MODE_LOW_LATENCY,
	/**
	 * @brief Low latency with standby.
	 *
	 * This mode has the same RX/TX switching performance as the low latency mode,
	 * but higher operation start-up latency due to radio entering standby mode
	 * when possible. Power consumption is thus lower compared to the low latency mode.
	 */
	NRF_MODEM_DECT_PHY_RADIO_MODE_LOW_LATENCY_WITH_STANDBY,
	/**
	 * @brief Listen-before-talk disabled, with standby.
	 *
	 * This mode has the lowest power consumption, due the to modem entering
	 * standby mode when possible and not using Listen-Before-Talk, at the cost
	 * of higher start-up latency and worse RX/TX switching performance compared
	 * to the other radio modes.
	 */
	NRF_MODEM_DECT_PHY_RADIO_MODE_NON_LBT_WITH_STANDBY,
	/**
	 * @brief Number of radio modes available.
	 */
	NRF_MODEM_DECT_PHY_RADIO_MODE_COUNT,
};

/**
 * @brief Radio configuration operation.
 */
struct nrf_modem_dect_phy_radio_config_params {
	/**
	 * @brief Operation start time as modem time.
	 *
	 * If zero, the operation will be executed immediately.
	 *
	 * This kind of scheduling can only be done when the modem is idle.
	 */
	uint64_t start_time;
	/**
	 * @brief Handle for the operation.
	 *
	 * An application-defined handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Desired radio mode.
	 */
	enum nrf_modem_dect_phy_radio_mode radio_mode;
};

/**
 * @brief DECT PHY latency information.
 *
 * Includes radio state transition and configuration latencies.
 */
struct nrf_modem_dect_phy_latency_info {
	/**
	 * @brief Radio mode specific latencies.
	 *
	 * These latencies must be used according to the currently active radio mode
	 * and operation type (immediate or scheduled).
	 */
	struct {
		/**
		 * @brief Scheduled operation transition delay.
		 *
		 * The delay between transitioning from one scheduled operation to the next.
		 *
		 * The start time of a scheduled operation must account for the start time
		 * of the previous operation, plus its duration, plus the operation
		 * transition delay in the current radio mode.
		 */
		uint32_t scheduled_operation_transition;
		/**
		 * @brief Additional delay for scheduled operations.
		 *
		 * Used together with the values in @ref operation to calculate the total
		 * operation start latency for scheduled operations.
		 */
		uint32_t scheduled_operation_startup;
		/**
		 * @brief Delay in transitioning from one radio mode to another.
		 */
		uint32_t radio_mode_transition[NRF_MODEM_DECT_PHY_RADIO_MODE_COUNT];
	} radio_mode[NRF_MODEM_DECT_PHY_RADIO_MODE_COUNT];

	/**
	 * @brief Duration of radio state transitions.
	 *
	 * Immediate operations can use these values directly.
	 * Scheduled operations must account for this value plus the value
	 * of @ref scheduled_operation_startup.
	 */
	struct {
		struct {
			/**
			 * @brief Receive operation start latency (IDLE to RF ON).
			 */
			uint32_t idle_to_active;
			/**
			 * @brief RSSI operation end latency (RF OFF to IDLE).
			 */
			uint32_t active_to_idle_rssi;
			/**
			 * @brief RX operation end latency (RF OFF to IDLE).
			 */
			uint32_t active_to_idle_rx;
			/**
			 * @brief RX with RSSI operation end latency (RF OFF to IDLE).
			 */
			uint32_t active_to_idle_rx_rssi;
			/**
			 * @brief Operation stop latency (STOP_REQ to RF OFF).
			 */
			uint32_t stop_to_rf_off;
		} receive;
		struct {
			/**
			 * @brief Transmit operation start latency (IDLE to RF ON).
			 */
			uint32_t idle_to_active;
			/**
			 * @brief Transmit operation end latency (RF OFF to IDLE).
			 */
			uint32_t active_to_idle;
		} transmit;
	} operation;

	/**
	 * @brief Duration of DECT PHY stack operations.
	 */
	struct {
		/**
		 * @brief Duration of DECT PHY stack initialization.
		 *
		 * Duration of @ref nrf_modem_dect_phy_init().
		 */
		uint32_t initialization;
		/**
		 * @brief Duration of DECT PHY stack deinitialization.
		 *
		 * Duration of @ref nrf_modem_dect_phy_deinit().
		 */
		uint32_t deinitialization;
		/**
		 * @brief Duration of DECT PHY stack configuration.
		 *
		 * Duration of @ref nrf_modem_dect_phy_configure().
		 */
		uint32_t configuration;
		/**
		 * @brief Duration of DECT PHY stack activation.
		 *
		 * Duration of @ref nrf_modem_dect_phy_activate().
		 */
		uint32_t activation;
		/**
		 * @brief Duration of DECT PHY stack deactivation.
		 *
		 * Duration of @ref nrf_modem_dect_phy_deactivate().
		 */
		uint32_t deactivation;
	} stack;
};

/**
 * @brief Link configuration parameters.
 */
struct nrf_modem_dect_phy_link_config_params {
	/**
	 * @brief Primary link.
	 *
	 * The modem tracks the frequency of the primary link for Automatic Frequency Control
	 * purposes. This frequency is propagated down the tree, that is, the Radio Device uses
	 * this frequency for all its radio operations except those directed to secondary links.
	 */
	struct nrf_modem_dect_phy_link_id primary_link_id;
	/**
	 * @brief Number of secondary links.
	 */
	uint8_t secondary_link_count;
	/**
	 * @brief Secondary links.
	 *
	 * The modem tracks the frequency of secondary links for Automatic Frequency Control
	 * purposes. When communicating with a secondary link the modem compensates the frequency
	 * difference between the secondary link and the primary link.
	 */
	struct nrf_modem_dect_phy_link_id secondary_link_ids[];
};

/** Events */

/**
 * @brief Initialization event.
 */
struct nrf_modem_dect_phy_init_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_TEMP_HIGH
	 * - @ref NRF_MODEM_DECT_PHY_ERR_PROD_LOCK
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief Temperature in degrees Celsius.
	 */
	int16_t temp;
	/**
	 * @brief Voltage in millivolts.
	 */
	uint16_t voltage;
	/**
	 * @brief Operating temperature limit, in degrees Celsius.
	 */
	uint16_t temperature_limit;
};

/**
 * @brief Operation complete event.
 */
struct nrf_modem_dect_phy_op_complete_event {
	/**
	 * @brief Handle of the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_LBT_CHANNEL_BUSY
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NO_MEMORY
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 * - @ref NRF_MODEM_DECT_PHY_OK_WITH_HARQ_RESET
	 * - @ref NRF_MODEM_DECT_PHY_ERR_OP_START_TIME_LATE
	 * - @ref NRF_MODEM_DECT_PHY_ERR_LBT_START_TIME_LATE
	 * - @ref NRF_MODEM_DECT_PHY_ERR_RF_START_TIME_LATE
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_START_TIME
	 * - @ref NRF_MODEM_DECT_PHY_ERR_OP_SCHEDULING_CONFLICT
	 * - @ref NRF_MODEM_DECT_PHY_ERR_OP_TIMEOUT
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NO_ONGOING_HARQ_RX
	 * - @ref NRF_MODEM_DECT_PHY_ERR_PARAMETER_UNAVAILABLE
	 * - @ref NRF_MODEM_DECT_PHY_ERR_PAYLOAD_UNAVAILABLE
	 * - @ref NRF_MODEM_DECT_PHY_ERR_OP_CANCELED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_COMBINED_OP_FAILED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_CARRIER
	 * - @ref NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_DATA_SIZE
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_NETWORK_ID
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_PHY_HEADER
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_DURATION
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_PARAMETER
	 * - @ref NRF_MODEM_DECT_PHY_ERR_TEMP_HIGH
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief Temperature in degrees Celsius.
	 */
	int16_t temp;
	/**
	 * @brief Voltage in millivolts.
	 */
	uint16_t voltage;
};

/**
 * @brief RSSI measurement event.
 */
struct nrf_modem_dect_phy_rssi_event {
	/**
	 * @brief Handle of the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Measurement start time, in modem time units.
	 */
	uint64_t meas_start_time;
	/**
	 * @brief The absolute channel frequency number on which the measurements were made.
	 */
	uint16_t carrier;
	/**
	 * @brief Number of measurements in @ref meas.
	 */
	uint16_t meas_len;
	/**
	 * @brief RSSI measurements, in dBm.
	 *
	 * If a symbol is measured, its measurement is in the interval [-140, -1].
	 * If the measurement is saturated, the measured signal strength is reported
	 * as a positive integer. If a symbol is not measured, its value is reported
	 * as @ref NRF_MODEM_DECT_PHY_RSSI_NOT_MEASURED.
	 */
	int8_t *meas;
};

struct nrf_modem_dect_phy_cancel_event {
	/**
	 * @brief Handle of the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_OP
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_FOUND
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 */
	enum nrf_modem_dect_phy_err err;
};

/**
 * @brief PCC reception event.
 */
struct nrf_modem_dect_phy_pcc_event {
	/**
	 * @brief Synchronization Training Field start time.
	 *
	 * Start time of the STF of this reception in modem time.
	 */
	uint64_t stf_start_time;
	/**
	 * @brief Handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Validity of the physical header.
	 */
	enum nrf_modem_dect_phy_hdr_status header_status;
	/**
	 * @brief Physical layer control field type.
	 *
	 * Valid values are 0 and 1 corresponding types 1 and 2, respectively.
	 *
	 * See 6.2.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint8_t phy_type;
	/**
	 * @brief PHY header.
	 */
	union nrf_modem_dect_phy_hdr hdr;
	/**
	 * @brief Received signal strength indicator (RSSI-2).
	 *
	 * Values are in dBm with 0.5 dBm resolution (Q14.1).
	 *
	 * See 8.3 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t rssi_2;
	/**
	 * @brief Received signal to noise indicator (SNR).
	 *
	 * Values are dB values with 0.25 dB resolution (Q13.2).
	 *
	 * See 8.4 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t snr;
	/**
	 * @brief Transaction ID.
	 *
	 * Used to map PCC data with corresponding PDC data.
	 */
	uint16_t transaction_id;
};

/**
 * @brief PCC reception CRC failure event.
 */
struct nrf_modem_dect_phy_pcc_crc_failure_event {
	/**
	 * @brief Synchronization Training Field start time.
	 *
	 * Start time of the STF of this reception in modem time.
	 */
	uint64_t stf_start_time;
	/**
	 * @brief Handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Received signal strength indicator (RSSI-2).
	 *
	 * Values are in dBm with 0.5 dBm resolution (Q14.1).
	 *
	 * See 8.3 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t rssi_2;
	/**
	 * @brief Received signal to noise indicator (SNR).
	 *
	 * Values are dB values with 0.25 dB resolution (Q13.2).
	 *
	 * See 8.4 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t snr;
	/**
	 * @brief Transaction ID.
	 *
	 * Used to map PCC data with corresponding PDC data.
	 */
	uint16_t transaction_id;
};

/**
 * @brief PDC reception event.
 */
struct nrf_modem_dect_phy_pdc_event {
	/**
	 * @brief Handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Received signal strength indicator (RSSI-2).
	 *
	 * Values are in dBm with 0.5 dBm resolution (Q14.1).
	 *
	 * See 8.3 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t rssi_2;
	/**
	 * @brief Received signal to noise indicator (SNR).
	 *
	 * Values are dB values with 0.25 dB resolution (Q13.2).
	 *
	 * See 8.4 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t snr;
	/**
	 * @brief Transaction ID.
	 *
	 * Used to map PCC data with corresponding PDC data.
	 */
	uint16_t transaction_id;
	/**
	 * @brief Received data payload.
	 */
	void *data;
	/**
	 * @brief Data payload length, in bytes.
	 */
	size_t len;
};

/**
 * @brief PDC CRC failure event.
 */
struct nrf_modem_dect_phy_pdc_crc_failure_event {
	/**
	 * @brief Handle for the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Received signal strength indicator (RSSI-2).
	 *
	 * Values are in dBm with 0.5 dBm resolution (Q14.1).
	 *
	 * If this value is not measured, it is reported as
	 * @ref NRF_MODEM_DECT_PHY_RSSI2_NOT_MEASURED.
	 *
	 * See 8.3 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t rssi_2;
	/**
	 * @brief Received signal to noise indicator (SNR).
	 *
	 * Values are dB values with 0.25 dB resolution (Q13.2).
	 *
	 * If this value is not measured, it is reported as
	 * @ref NRF_MODEM_DECT_PHY_SNR_NOT_MEASURED.
	 *
	 * See 8.4 in @ref DECT-SPEC "DECT-2020 NR Part 2".
	 */
	int16_t snr;
	/**
	 * @brief Transaction ID.
	 *
	 * Used to map PCC data with corresponding PDC data.
	 */
	uint16_t transaction_id;
};

/**
 * @brief DECT PHY stack configuration event.
 */
struct nrf_modem_dect_phy_configure_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_PARAMETER
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief Temperature in degrees Celsius.
	 */
	int16_t temp;
	/**
	 * @brief Voltage in millivolts.
	 */
	uint16_t voltage;
};

/**
 * @brief Radio configuration event.
 */
struct nrf_modem_dect_phy_radio_config_event {
	/**
	 * @brief Handle of the operation.
	 */
	uint32_t handle;
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 */
	enum nrf_modem_dect_phy_err err;
};

struct nrf_modem_dect_phy_link_config_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NO_MEMORY
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_PARAMETER
	 */
	enum nrf_modem_dect_phy_err err;
};

/**
 * @brief Time get event.
 */
struct nrf_modem_dect_phy_time_get_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 */
	enum nrf_modem_dect_phy_err err;
};

/**
 * @brief Capability get event.
 */
struct nrf_modem_dect_phy_capability_get_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief PHY capability.
	 */
	struct nrf_modem_dect_phy_capability *capability;
};

/**
 * @brief Band information event.
 */
struct nrf_modem_dect_phy_band_get_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief Supported bands as a bitmap.
	 */
	uint32_t supported_bands;
	/**
	 * @brief Number of elements in @c band.
	 */
	uint32_t band_count;
	/**
	 * @brief Band information.
	 */
	struct nrf_modem_dect_phy_band *band;
};

/**
 * @brief STF cover sequence control event.
 */
struct nrf_modem_dect_phy_stf_control_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 */
	enum nrf_modem_dect_phy_err err;
};

/**
 * @brief Deinitialization event.
 */
struct nrf_modem_dect_phy_deinit_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 */
	enum nrf_modem_dect_phy_err err;
};

/**
 * @brief Activation event.
 */
struct nrf_modem_dect_phy_activate_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief Temperature in degrees Celsius.
	 */
	int16_t temp;
	/**
	 * @brief Voltage in millivolts.
	 */
	uint16_t voltage;
};

/**
 * @brief Deactivation event.
 */
struct nrf_modem_dect_phy_deactivate_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 */
	enum nrf_modem_dect_phy_err err;
};

/**
 * @brief Latency event.
 */
struct nrf_modem_dect_phy_latency_info_event {
	/**
	 * @brief Operation result.
	 *
	 * Can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 */
	enum nrf_modem_dect_phy_err err;
	/**
	 * @brief PHY latency info.
	 */
	struct nrf_modem_dect_phy_latency_info *latency_info;
};

enum nrf_modem_dect_phy_event_id {
	/**
	 * @brief Event to indicate the completion of the DECT PHY stack initialization.
	 */
	NRF_MODEM_DECT_PHY_EVT_INIT,
	/**
	 * @brief Event to indicate the completion of the DECT PHY stack deinitialization.
	 */
	NRF_MODEM_DECT_PHY_EVT_DEINIT,
	/**
	 * @brief Event to indicate the completion of the DECT PHY stack configuration.
	 */
	NRF_MODEM_DECT_PHY_EVT_CONFIGURE,
	/**
	 * @brief Event to indicate the completion of radio mode configuration.
	 */
	NRF_MODEM_DECT_PHY_EVT_RADIO_CONFIG,
	/**
	 * @brief Event to indicate the completion of the DECT PHY stack activation.
	 */
	NRF_MODEM_DECT_PHY_EVT_ACTIVATE,
	/**
	 * @brief Event to indicate the completion of the DECT PHY stack deactivation.
	 */
	NRF_MODEM_DECT_PHY_EVT_DEACTIVATE,
	/**
	 * @brief Event to indicate the completion of RX, TX and RSSI measurement operations.
	 */
	NRF_MODEM_DECT_PHY_EVT_COMPLETED,
	/**
	 * @brief Event to indicate the cancellation of an operation.
	 */
	NRF_MODEM_DECT_PHY_EVT_CANCELED,
	/**
	 * @brief Event carrying RSSI measurements.
	 */
	NRF_MODEM_DECT_PHY_EVT_RSSI,
	/**
	 * @brief Event for control channel reception.
	 */
	NRF_MODEM_DECT_PHY_EVT_PCC,
	/**
	 * @brief Event for CRC failures on the physical control channel.
	 */
	NRF_MODEM_DECT_PHY_EVT_PCC_ERROR,
	/**
	 * @brief Event for data channel reception.
	 */
	NRF_MODEM_DECT_PHY_EVT_PDC,
	/**
	 * @brief Event for CRC failures on the physical data channel.
	 */
	NRF_MODEM_DECT_PHY_EVT_PDC_ERROR,
	/**
	 * @brief Event carrying modem time.
	 */
	NRF_MODEM_DECT_PHY_EVT_TIME,
	/**
	 * @brief Event carrying capabilities.
	 */
	NRF_MODEM_DECT_PHY_EVT_CAPABILITY,
	/**
	 * @brief Event carrying band information.
	 */
	NRF_MODEM_DECT_PHY_EVT_BANDS,
	/**
	 * @brief Event carrying latency information.
	 */
	NRF_MODEM_DECT_PHY_EVT_LATENCY,
	/**
	 * @brief Event to indicate the completion of link configuration.
	 */
	NRF_MODEM_DECT_PHY_EVT_LINK_CONFIG,
	/**
	 * @brief Event to indicate the completion of STF configuration.
	 */
	NRF_MODEM_DECT_PHY_EVT_STF_CONFIG,
};

/**
 * @brief DECT PHY event.
 */
struct nrf_modem_dect_phy_event {
	/**
	 * @brief Event ID.
	 */
	enum nrf_modem_dect_phy_event_id id;
	/**
	 * @brief Modem time, in modem time units.
	 */
	uint64_t time;
	/**
	 * @brief Event data.
	 */
	union {
		struct nrf_modem_dect_phy_init_event init;
		struct nrf_modem_dect_phy_deinit_event deinit;
		struct nrf_modem_dect_phy_activate_event activate;
		struct nrf_modem_dect_phy_deactivate_event deactivate;
		struct nrf_modem_dect_phy_configure_event configure;
		struct nrf_modem_dect_phy_radio_config_event radio_config;
		struct nrf_modem_dect_phy_op_complete_event op_complete;
		struct nrf_modem_dect_phy_rssi_event rssi;
		struct nrf_modem_dect_phy_cancel_event cancel;
		struct nrf_modem_dect_phy_pcc_event pcc;
		struct nrf_modem_dect_phy_pcc_crc_failure_event pcc_crc_err;
		struct nrf_modem_dect_phy_pdc_event pdc;
		struct nrf_modem_dect_phy_pdc_crc_failure_event pdc_crc_err;
		struct nrf_modem_dect_phy_time_get_event time_get;
		struct nrf_modem_dect_phy_capability_get_event capability_get;
		struct nrf_modem_dect_phy_band_get_event band_get;
		struct nrf_modem_dect_phy_latency_info_event latency_get;
		struct nrf_modem_dect_phy_stf_control_event stf_cover_seq_control;
		struct nrf_modem_dect_phy_link_config_event link_config;
	};
};

/**
 * @brief DECT PHY configuration parameters.
 */
struct nrf_modem_dect_phy_config_params {
	/**
	 * @brief Band group index.
	 *
	 * Allowed values: 0 or 1.
	 * Value 0 refers to RF frequencies operating near 2GHz and
	 * value 1 to RF frequencies near 1 GHz.
	 */
	uint8_t band_group_index;
	/**
	 * @brief Number of HARQ processes.
	 *
	 * The HARQ reception buffer is divided equally between processes.
	 * Supported values: 1, 2, 4, 8.
	 */
	uint8_t harq_rx_process_count;
	/**
	 * @brief HARQ RX buffer expiry time, in microseconds.
	 *
	 * A HARQ process soft buffer allocation is released when there have been no receptions
	 * to that buffer within the buffer's expiry time.
	 *
	 * Maximum supported value: 5000000.
	 */
	uint32_t harq_rx_expiry_time_us;
};

/**
 * @brief Application handler prototype for PHY events.
 */
typedef void (*nrf_modem_dect_phy_event_handler_t)(const struct nrf_modem_dect_phy_event *event);

/**
 * @brief Set the application event handler for PHY events.
 *
 * The application must set the handler for events coming the DECT PHY before attempting
 * other operations.
 *
 * @param handler Event handler.
 *
 * @return 0 On success.
 * @retval -NRF_EFAULT @p handler is @c NULL.
 */
int nrf_modem_dect_phy_event_handler_set(nrf_modem_dect_phy_event_handler_t handler);

/**
 * @brief Retrieve DECT PHY capabilities.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_CAPABILITY event.
 *
 * @return 0 On success.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_capability_get(void);

/**
 * @brief Initialize the DECT PHY interface.
 *
 * Initialize the DECT PHY interface and associated hardware resources.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_INIT event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_init(void);

/**
 * @brief Deinitialize the DECT PHY interface.
 *
 * Deinitialize the DECT PHY interface and all associated hardware resources.
 * This implicitly cancels all operations, both executing or scheduled for execution.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_DEINIT event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_deinit(void);

/**
 * @brief Activate the DECT PHY software stack in given radio mode.
 *
 * Before each activation, the DECT PHY software stack must be configured using
 * @ref nrf_modem_dect_phy_configure.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_ACTIVATE event.
 *
 * @param mode Desired radio mode.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_activate(enum nrf_modem_dect_phy_radio_mode mode);

/**
 * @brief Deactivate the DECT PHY software stack.
 *
 * Deactivation implicitly cancels all operations, both executing or scheduled for execution.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_DEACTIVATE event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_deactivate(void);

/**
 * @brief Schedule a reception.
 *
 * Schedule a reception with given parameters.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event.
 *
 * Data received on the physical layer control and data channels is sent to the application
 * in the @ref NRF_MODEM_DECT_PHY_EVT_PCC and @ref NRF_MODEM_DECT_PHY_EVT_PDC events respectively.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_rx(const struct nrf_modem_dect_phy_rx_params *params);

/**
 * @brief Schedule a transmission.
 *
 * Schedule a data transmission with given parameters.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_tx(const struct nrf_modem_dect_phy_tx_params *params);

/**
 * @brief Schedule a HARQ response TX operation.
 *
 * A HARQ response TX can be scheduled as a response to a PCC callback or PCC CRC failure callback.
 *
 * Scheduling a HARQ response is time critical and therefore scheduling should be done directly
 * from the PCC callback. The MAC should have prepared data towards possible HARQ recipients in
 * advance so that it can directly call this function without further delays.
 *
 * This function should only be used to send a Physical Layer Control Field: Type 2 packet with
 * Feedback info format 1 in response to a PCC notification for an ongoing RX so that the receiver
 * identity of the packet being transmitted matches the transmitter identity of the packet being
 * received. In that case modem will inject ACK or NACK feedback into the Transmission feedback
 * field based on the PDC reception result once the reception is complete.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_tx_harq(const struct nrf_modem_dect_phy_tx_params *params);

/**
 * @brief Schedule one TX and one RX operation.
 *
 * Schedule one TX and one RX operation to be executed one after the other.
 * The scheduling of both operations has to be successful, or no operation will be scheduled.
 *
 * The RX operation is executed only if the TX operation is successful, including LBT, if used.
 * The RX operation scheduling is relative to the end of the TX operation and must include
 * the guard time.
 *
 * These operations are performed asynchronously.
 * Completion of each of operation is indicated by one @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to the modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_tx_rx(const struct nrf_modem_dect_phy_tx_rx_params *params);

/**
 * @brief Schedule an RSSI measurement operation.
 *
 * Schedule an RSSI measurement operation with given parameters.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event.
 * The measurements are sent in the @ref NRF_MODEM_DECT_PHY_EVT_RSSI event.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_rssi(const struct nrf_modem_dect_phy_rssi_params *params);

/**
 * @brief Cancel an operation.
 *
 * Cancel an ongoing operation or an operation scheduled to be executed.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_CANCELED event.
 *
 * When an operation is canceled, a @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event is sent to the
 * application. If the operation is canceled while it is executing, the event reports that the
 * operation completed successfully. If the operation was scheduled but not yet executing, the
 * @ref NRF_MODEM_DECT_PHY_EVT_COMPLETED event will indicate the status as
 * @ref NRF_MODEM_DECT_PHY_ERR_OP_CANCELED.
 *
 * @param handle  Handle of the operation to be canceled,
 *		  or @ref NRF_MODEM_DECT_PHY_HANDLE_CANCEL_ALL to cancel all operations.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_cancel(uint32_t handle);

/**
 * @brief Configure the PHY stack.
 *
 * This operation can only be performed when the DECT PHY is deactivated.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_CONFIGURE event.
 *
 * @param[in] params PHY configuration parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_configure(const struct nrf_modem_dect_phy_config_params *params);

/**
 * @brief Configure radio mode.
 *
 * Configure the radio for the desired operation latency, RX/TX performance and power consumption.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_RADIO_CONFIG event.
 *
 * @param[in] params Radio mode configuration parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_radio_config(const struct nrf_modem_dect_phy_radio_config_params *params);

/**
 * @brief Configure links.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_LINK_CONFIG event.
 *
 * @param[in] params Link configuration parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ENOSYS Functionality not implemented.
 */
int nrf_modem_dect_phy_link_config(const struct nrf_modem_dect_phy_link_config_params *params);

/**
 * @brief Retrieve band information.
 *
 * This operation can only be performed when the DECT PHY is deactivated.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_BANDS event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_band_get(void);

/**
 * @brief Retrieve latency information.
 *
 * This operation can only be performed when the DECT PHY is deactivated.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_LATENCY event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_latency_get(void);

/**
 * @brief Retrieve modem time.
 *
 * Retrieve modem time, in modem time units.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_TIME event.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_time_get(void);

/**
 * @brief STF cover sequence control.
 *
 * Enable or disable STF cover sequence.
 * The default, applied at each initialization, is enabled.
 *
 * @note
 * This API is intended for certification purposes only.
 * It should not be used for normal operation.
 *
 * This operation is performed asynchronously.
 * Completion of this operation is indicated by the @ref NRF_MODEM_DECT_PHY_EVT_STF_CONFIG event.
 *
 * @param rx_enable Enable STF cover sequence for reception.
 * @param tx_enable Enable STF cover sequence for transmission.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT No event handler is set.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_stf_cover_seq_control(bool rx_enable, bool tx_enable);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_DECT_PHY_H__ */
/** @} */
