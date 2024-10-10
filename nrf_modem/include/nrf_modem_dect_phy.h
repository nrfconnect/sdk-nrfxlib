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

/** @brief Symbol RSSI not measured. */
#define NRF_MODEM_DECT_PHY_RSSI_NOT_MEASURED 0
/** @brief Temperature not measured. */
#define NRF_MODEM_DECT_PHY_TEMP_NOT_MEASURED 999
/** @brief RSSI-2 not measured. */
#define NRF_MODEM_DECT_PHY_RSSI2_NOT_MEASURED -32768
/** @brief Signal to noise ratio not measured. */
#define NRF_MODEM_DECT_PHY_SNR_NOT_MEASURED -32768

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
	 * @brief Operation is not executing; it cannot be stopped.
	 */
	NRF_MODEM_DECT_PHY_ERR_NO_ONGOING_OP = 0x3,
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
 * @brief Modem PHY configuration.
 *
 * Includes radio state transition latencies and operating temperature limit.
 */
struct nrf_modem_dect_phy_modem_cfg {
	/**
	 * @brief Maximum operating temperature, in degrees Celsius.
	 *
	 * The minimum resolution is 1 degree Celsius.
	 */
	int16_t temperature_limit;
	/**
	 * @brief Latency of radio state transitions, in modem time units.
	 */
	struct {
		/**
		 * @brief From Idle to RSSI RF sampling start.
		 */
		uint32_t idle_to_rssi;
		/**
		 * @brief From RF sampling of RSSI to Idle.
		 */
		uint32_t rssi_to_idle;
		/**
		 * @brief From Idle to RX RF sampling start.
		 */
		uint32_t idle_to_rx;
		/**
		 * @brief From RF sampling of RX to Idle.
		 */
		uint32_t rx_to_idle;
		/**
		 * @brief From RF sampling of RX with RSSI measurement to Idle.
		 */
		uint32_t rx_rssi_to_idle;
		/**
		 * @brief From RX stop request to RF sampling stop.
		 */
		uint32_t rx_stop_to_rf_off;
		/**
		 * @brief From RX with RSSI measurement stop request to RF sampling stop.
		 */
		uint32_t rx_rssi_stop_to_rf_off;
		/**
		 * @brief From Idle to TX RF sample transmission start.
		 */
		uint32_t idle_to_tx;
		/**
		 * @brief From Idle to TX with LBT monitoring RF sampling start.
		 */
		uint32_t idle_to_tx_with_lbt;
		/**
		 * @brief From TX RF sample transmission end (with or without LBT) to Idle.
		 */
		uint32_t tx_to_idle;
		/**
		 * @brief From Idle to TX with LBT monitoring RF sampling start in TX-RX operation.
		 */
		uint32_t idle_to_txrx_with_lbt;
		/**
		 * @brief From TX RF sample transmission end
		 *	  to RX RF sampling start in TX-RX operation.
		 */
		uint32_t txrx_tx_to_rx;
	} latency;
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
		 * @brief Power class.
		 */
		uint8_t power_class;
		/**
		 * @brief Supported reception capability of spatial stream transmissions.
		 */
		uint8_t rx_spatial_streams;
		/**
		 * @brief Reception capability of TX diversity transmission.
		 */
		uint8_t rx_tx_diversity;
		/**
		 * @brief Receiver sensitivity in dB.
		 */
		int8_t rx_gain;
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
	 * @brief Semicontinous reception.
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
 * Radio link identifier identifies a radio link using fields that are available in Physical Layer
 * Control Field.
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
 *  See 6.2.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
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
 * @brief Reception parameters (PCC).
 */
struct nrf_modem_dect_phy_rx_pcc_status {
	/**
	 * @brief Synchronization Training Field start time.
	 *
	 * Start time of the STF of this reception in modem time.
	 */
	uint64_t stf_start_time;
	/**
	 * @brief Physical layer control field type.
	 *
	 * Valid values are 0 and 1 corresponding types 1 and 2, respectively.
	 *
	 * See 6.2.1 in @ref DECT-SPEC "DECT-2020 NR Part 4".
	 */
	uint8_t phy_type;
	/**
	 * @brief Validity of the physical header.
	 */
	enum nrf_modem_dect_phy_hdr_status header_status;
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
};

/**
 * @brief Reception parameters (PDC).
 */
struct nrf_modem_dect_phy_rx_pdc_status {
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
};

/**
 * @brief Reception parameters for PCC CRC failures.
 */
struct nrf_modem_dect_phy_rx_pcc_crc_failure {
	/**
	 * @brief Synchronization Training Field start time.
	 *
	 * Start time of the STF of this reception in modem time.
	 */
	uint64_t stf_start_time;
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
};

/**
 * @brief Reception parameters for PDC CRC failures.
 */
struct nrf_modem_dect_phy_rx_pdc_crc_failure {
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
};

/**
 * @brief RSSI measurement.
 */
struct nrf_modem_dect_phy_rssi_meas {
	/**
	 * @brief Measurement start time, in modem time units.
	 */
	uint64_t meas_start_time;
	/**
	 * @brief Handle of the operation.
	 */
	uint32_t handle;
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
	 * If a symbol is measured, its measurement is in the interval [-1, -140].
	 * If the measurement is saturated, the measured signal strength is reported
	 * as a positive integer. If a symbol is not measured, its value is reported
	 * as @ref NRF_MODEM_DECT_PHY_RSSI_NOT_MEASURED.
	 */
	int8_t *meas;
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
	 * @brief Handle for the RX operation.
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
	 * @brief RSSI measurement reporting interval.
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
	 * `rssi_level` will only be used for the first successful reception. Subsequent receptions
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
	 * @brief Handle for the TX operation.
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
	 * The period that the channel needs to be measured "free" or "possible" before initiating
	 * transmission.
	 *
	 * The period is split into up to 64 individual integration periods, each period
	 * being an integer multiple of a symbol duration (2880 modem time units). All integration
	 * periods have the same length, maximum 7 symbols. The maximum number of integration
	 * periods is used. For example, LBT periods of up to 64 symbols will be split into one
	 * symbol long integration periods, and LBT periods of 65-128 symbols will be split into two
	 * symbol long integration periods. If necessary the LBT period may be shortened slightly
	 * to fit these constraints while still not exceeding the given LBT length, e.g.
	 * a period of 71 symbols will effectively be 70 symbols long (35 periods of 2 symbols).
	 *
	 * In case the channel is initially detected "busy" the modem will not continue measuring
	 * the channel. Instead the modem will report an error. Rescheduling a transmission is left
	 * up to the higher layer.
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
	 * @brief Handle for the RSSI operation.
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
	 * @brief RSSI measurements reporting interval, in subslots.
	 */
	enum nrf_modem_dect_phy_rssi_interval reporting_interval;
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

/**
 * @brief DECT PHY callbacks.
 *
 * Callbacks for user operations and incoming data.
 *
 * @note
 * All callbacks are executed in ISR.
 */
struct nrf_modem_dect_phy_callbacks {
	/**
	 * @brief DECT PHY initialization callback.
	 *
	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_TEMP_HIGH
	 * - @ref NRF_MODEM_DECT_PHY_ERR_PROD_LOCK
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param temp Temperature in degrees Celsius.
	 * @param err Operation result.
	 * @param[in] cfg Modem configuration.
	 */
	void (*init)(const uint64_t *time, int16_t temp, enum nrf_modem_dect_phy_err err,
		     const struct nrf_modem_dect_phy_modem_cfg *cfg);

	/**
	 * @brief Callback to indicate the completion of RX, TX and RSSI measurement operations.
	 *
	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
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
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param temp Temperature in degrees Celsius.
	 * @param err Operation result.
	 * @param handle Operation handle.
	 */
	void (*op_complete)(const uint64_t *time, int16_t temperature,
			    enum nrf_modem_dect_phy_err err, uint32_t handle);

	/**
	 * @brief Callback to receive RSSI measurements.
	 *
	 * Callback function to receive the RSSI measurements requested
	 * with @ref nrf_modem_dect_phy_rssi or @ref nrf_modem_dect_phy_rx.
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param[in] meas RSSI measurement.
	 */
	void (*rssi)(const uint64_t *time, const struct nrf_modem_dect_phy_rssi_meas *meas);

	/**
	 * @brief Callback for @ref nrf_modem_dect_phy_rx_stop.
	 *
	 * When an RX operation is stopped, the operation complete callback will be called
	 * once the operation has fully stopped. That is, it is possible that the
	 * operation hasn't been fully stopped at the time this callback is called.

	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_UNSUPPORTED_OP
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NO_ONGOING_OP
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param err Operation result.
	 * @param handle Operation handle.
	 */
	void (*rx_stop)(const uint64_t *time, enum nrf_modem_dect_phy_err err, uint32_t handle);

	/**
	 * @brief Callback for control channel reception.
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param status Reception data.
	 * @param[in] hdr Physical layer control header.
	 */
	void (*pcc)(const uint64_t *time, const struct nrf_modem_dect_phy_rx_pcc_status *status,
		    const union nrf_modem_dect_phy_hdr *hdr);

	/**
	 * @brief Callback for CRC failures on the physical control channel.
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param[in] crc_failure CRC failure information.
	 */
	void (*pcc_crc_err)(const uint64_t *time,
			    const struct nrf_modem_dect_phy_rx_pcc_crc_failure *crc_failure);

	/**
	 * @brief Callback for data channel reception.
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param status Reception data.
	 * @param[in] data Data received.
	 * @param len Size of received data.
	 */
	void (*pdc)(const uint64_t *time, const struct nrf_modem_dect_phy_rx_pdc_status *status,
		    const void *data, uint32_t len);

	/**
	 * @brief Callback for CRC failures on the physical data channel.
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param[in] crc_failure CRC failure information.
	 */
	void (*pdc_crc_err)(const uint64_t *time,
			    const struct nrf_modem_dect_phy_rx_pdc_crc_failure *crc_failure);

	/**
	 * @brief Callback for @ref nrf_modem_dect_phy_link_config.
	 *
	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NO_MEMORY
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 * - @ref NRF_MODEM_DECT_PHY_ERR_INVALID_PARAMETER
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param err Operation result.
	 */
	void (*link_config)(const uint64_t *time, enum nrf_modem_dect_phy_err err);

	/**
	 * @brief Callback for @ref nrf_modem_dect_phy_time_get.
	 *
	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param err Operation result.
	 */
	void (*time_get)(const uint64_t *time, enum nrf_modem_dect_phy_err err);

	/**
	 * @brief Callback for @ref nrf_modem_dect_phy_capability_get.
	 *
	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param err Operation result.
	 * @param[in] capability PHY capabilities.
	 */
	void (*capability_get)(const uint64_t *time, enum nrf_modem_dect_phy_err err,
			       const struct nrf_modem_dect_phy_capability *capability);

	/**
	 * @brief Callback for @ref nrf_modem_dect_phy_deinit.
	 *
	 * The @p err parameter indicates the result of the operation.
	 * It can be one of the following values:
	 *
	 * - @ref NRF_MODEM_DECT_PHY_SUCCESS
	 * - @ref NRF_MODEM_DECT_PHY_ERR_NOT_ALLOWED
	 *
	 * @param[in] time Modem time, in modem time units.
	 * @param err Operation result.
	 */
	void (*deinit)(const uint64_t *time, enum nrf_modem_dect_phy_err err);
};

/**
 * @brief Configuration for DECT PHY interface.
 */
struct nrf_modem_dect_phy_init_params {
	/**
	 * @brief HARQ RX buffer expiry time, in microseconds.
	 *
	 * A HARQ process soft buffer allocation is released when there have been no receptions
	 * to that buffer within the buffer's expiry time.
	 *
	 * Maximum supported value: 5000000.
	 */
	uint32_t harq_rx_expiry_time_us;
	/**
	 * @brief Number of HARQ processes.
	 *
	 * The HARQ reception buffer is divided equally between processes.
	 * Supported values: 1, 2, 4, 8.
	 */
	uint8_t harq_rx_process_count;
};

/**
 * @brief Set application callbacks for PHY operations.
 *
 * The application must set the callbacks for PHY operations before attempting other operations.
 * That includes PHY initialization itself, that is, nrf_modem_dect_phy_init().
 *
 * @param cb Application callbacks.
 *
 * @return 0 On success.
 * @retval -NRF_EFAULT @p cb is @c NULL.
 * @retval -NRF_EINVAL One of the fields in @p cb is @c NULL.
 */
int nrf_modem_dect_phy_callback_set(const struct nrf_modem_dect_phy_callbacks *cb);

/**
 * @brief Retrieve DECT PHY capabilities.
 *
 * This operation is asynchronous. The result of the operation is sent to the
 * @ref nrf_modem_dect_phy_callbacks.capability_get callback.
 *
 * @return 0 On success.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_capability_get(void);

/**
 * @brief Initialize DECT PHY interface.
 *
 * This operation is asynchronous. The result of the operation is sent to the
 * @ref nrf_modem_dect_phy_callbacks.init callback.
 *
 * @param[in] params  Configuration parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_init(const struct nrf_modem_dect_phy_init_params *params);

/**
 * @brief De-initialize DECT PHY interface.
 *
 * Cancel all operations and de-initialize the PHY.
 *
 * This operation is asynchronous. The result of the operation is sent to the
 * @ref nrf_modem_dect_phy_callbacks.deinit callback.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_deinit(void);

/**
 * @brief Schedule a reception.
 *
 * Schedule a continuous, semicontinuous or single shot reception at a given time, with parameters.
 *
 * Incoming data received on the physical layer control and data channels is sent to the
 * @ref nrf_modem_dect_phy_callbacks.pcc and @ref nrf_modem_dect_phy_callbacks.pdc callbacks.
 *
 * When the operation has completed, either successfully or unsuccessfully, the result is sent
 * to the @ref nrf_modem_dect_phy_callbacks.op_complete callback, with a `handle`
 * parameter equal to the `handle` specified in @p params.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_rx(const struct nrf_modem_dect_phy_rx_params *params);

/**
 * @brief Schedule a transmission.
 *
 * Schedule a data transmission with given parameters.
 *
 * When the operation has completed, either successfully or unsuccessfully, the result is sent
 * to the @ref nrf_modem_dect_phy_callbacks.op_complete callback, with a `handle`
 * parameter equal to the `handle` specified in @p params.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
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
 * When the operation has completed, either successfully or unsuccessfully, the result is sent
 * to the @ref nrf_modem_dect_phy_callbacks.op_complete callback, with a `handle`
 * parameter equal to the `handle` specified in @p params.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
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
 * When the TX operation has completed, either successfully or unsuccessfully, the result is sent
 * to the @ref nrf_modem_dect_phy_callbacks.op_complete callback, with a `handle`
 * parameter equal to the `handle` specified in @p params.tx.
 *
 * When the RX operation has completed, either successfully or unsuccessfully, the result is sent
 * to the @ref nrf_modem_dect_phy_callbacks.op_complete callback, with a `handle`
 * parameter equal to the `handle` specified in @p params.rx.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to the modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_tx_rx(const struct nrf_modem_dect_phy_tx_rx_params *params);

/**
 * @brief Schedule an RSSI measurement operation.
 *
 * Schedule an RSSI measurement operation with given parameters.
 * The measurements are sent to the @ref nrf_modem_dect_phy_callbacks.rssi callback.
 *
 * When the operation has completed, either successfully or unsuccessfully, the result is sent
 * to the @ref nrf_modem_dect_phy_callbacks.op_complete callback, with a `handle`
 * parameter equal to the `handle` specified in @p params.
 *
 * @param[in] params  Operation parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_rssi(const struct nrf_modem_dect_phy_rssi_params *params);

/**
 * @brief Stop an ongoing reception.
 *
 * Stop an ongoing reception before its duration has ended; this can take more or less
 * time depending on the internal state of the operation at the time the request is received.
 *
 * This operation is asynchronous. The result of the operation is sent to the
 * @ref nrf_modem_dect_phy_callbacks.rx_stop callback.
 *
 * @note
 * Only RX operations can be stopped.
 *
 * @param handle  Handle of the operation to be stopped.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_rx_stop(uint32_t handle);

/**
 * @brief Configure links.
 *
 * This operation is asynchronous. The result of the operation is sent to the
 * @ref nrf_modem_dect_phy_callbacks.link_config callback.
 *
 * @param[in] params Link configuration parameters.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid, or @p params is @c NULL.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 * @retval -NRF_ENOSYS Functionality not implemented.
 */
int nrf_modem_dect_phy_link_config(const struct nrf_modem_dect_phy_link_config_params *params);

/**
 * @brief Query modem time.
 *
 * Retrieve modem time, in modem time units.
 *
 * This operation is asynchronous. The result of the operation is sent to the
 * @ref nrf_modem_dect_phy_callbacks.time_get callback.
 *
 * @retval 0           Request was sent to modem.
 * @retval -NRF_EPERM  The Modem library is not initialized.
 * @retval -NRF_EFAULT Callback configuration is invalid.
 * @retval -NRF_ENOMEM Not enough shared memory for this request.
 */
int nrf_modem_dect_phy_time_get(void);

#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_DECT_PHY_H__ */
/** @} */
