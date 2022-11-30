/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_dm.h
 *
 * @defgroup nrf_dm nRF Distance Measurement interface
 *
 * Interface for the Nordic distance measurement library.
 * @{
 */

#ifndef __NRF_DM_H_
#define __NRF_DM_H_

#include <stdint.h>
#include "nrf.h"
#include "nrf_dm_version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Quality of the procedure
 */
typedef enum {
	NRF_DM_QUALITY_OK = 0,
	NRF_DM_QUALITY_POOR = 1,
	NRF_DM_QUALITY_DO_NOT_USE = 2,
	NRF_DM_QUALITY_CRC_FAIL = 3,
} nrf_dm_quality_t;

/**
 * @brief SINR indicator for IQ measurements
 */
typedef enum {
	NRF_DM_SINR_INDICATOR_HIGH     = 0,
	NRF_DM_SINR_INDICATOR_MEDIUM   = 1,
	NRF_DM_SINR_INDICATOR_LOW      = 2,
	NRF_DM_SINR_INDICATOR_VERY_LOW = 3,
} nrf_dm_sinr_indicator_t;

/**
 * @brief Role definition
 */
typedef enum {
	NRF_DM_ROLE_NONE = 0,
	/** Act as an initiator */
	NRF_DM_ROLE_INITIATOR = 1,
	/** Act as a reflector */
	NRF_DM_ROLE_REFLECTOR = 2,
} nrf_dm_role_t;

/**
 * @brief Status of the procedure
 */
typedef enum {
	NRF_DM_STATUS_SUCCESS = 0,
	NRF_DM_STATUS_FAIL_SYNC = 1,
	NRF_DM_STATUS_EVENT_FAIL_TIMEOUT = 2,
	NRF_DM_STATUS_CONF_INVALID = 3
} nrf_dm_status_t;

/**
 * @brief Ranging modes
 */
typedef enum {
	NRF_DM_RANGING_MODE_RTT,
	NRF_DM_RANGING_MODE_MCPD,
} nrf_dm_ranging_mode_t;

/**
 * @brief Container of IQ values for local and remote measured tones
 */
typedef struct {
	/** In-phase measurements of tones on this device */
	float i_local[80];
	/** Quadrature-phase measurement of tones on this device */
	float q_local[80];
	/** In-phase measurements of tones from remote device */
	float i_remote[80];
	/** Quadrature-phase measurements of tones from remote device */
	float q_remote[80];
} nrf_dm_iq_tones_t;

typedef struct {
	/** SINR-Indicator for tones on this device */
	nrf_dm_sinr_indicator_t sinr_indicator_local[80];
	/** SINR-Indicator for tones on peer device */
	nrf_dm_sinr_indicator_t sinr_indicator_remote[80];
} nrf_dm_iq_tones_sinr_indicator_t;

/**
 * @brief Container of distance estimate results for a number of different
 * methods, in meters.
 */
typedef union {
	struct _mcpd {
		/** MCPD: Distance estimate based on IFFT of spectrum */
		float ifft;
		/** MCPD: Distance estimate based on average phase slope estimation */
		float phase_slope;
		/** RSSI: Distance estimate based on Friis path loss formula */
		float rssi_openspace;
		/** Best effort distance estimate */
		float best;
	} mcpd;

	struct _rtt {
		/** RTT: Distance estimate based on RTT measurement */
		float rtt;
	} rtt;
} nrf_dm_dist_estimates_t;

/**
 * @brief Configuration options for antenna configurations.
 * @note Only NRF_DM_ANTENNA_COMP_1_1 is supported.
 */
typedef enum {
	/** Initiator uses 1 antenna, reflector uses 1 antenna. */
	NRF_DM_ANTENNA_COMP_1_1,
	/** Initiator uses 2 antenna, reflector uses 1 antenna. */
	NRF_DM_ANTENNA_COMP_2_1,
	/** Initiator uses 3 antenna, reflector uses 1 antenna. */
	NRF_DM_ANTENNA_COMP_3_1,
	/** Initiator uses 4 antenna, reflector uses 1 antenna. */
	NRF_DM_ANTENNA_COMP_4_1,
	/** Initiator uses 1 antenna, reflector uses 2 antenna. */
	NRF_DM_ANTENNA_COMP_1_2,
	/** Initiator uses 1 antenna, reflector uses 3 antenna. */
	NRF_DM_ANTENNA_COMP_1_3,
	/** Initiator uses 1 antenna, reflector uses 4 antenna. */
	NRF_DM_ANTENNA_COMP_1_4,
	/** Initiator uses 2 antenna, reflector uses 2 antenna. */
	NRF_DM_ANTENNA_COMP_2_2,
} nrf_dm_antenna_composition_t;

/**
 * @brief Output data for distance estimation
 */
typedef struct {
	/** IQ values for local and remote measured tones.
	 * NOTE: make sure to only read data from the antennas that were configured in
	 * @ref nrf_dm_configure(). If the device uses N antennas, then the local values
	 * for N entries of @ref iq_tones are valid. If the peer uses M antennas, then
	 * the remote values for M entries of @ref iq_tones are valid.
	 */
	nrf_dm_iq_tones_t iq_tones[NRF_DM_MAX_NUM_ANT];

	/** SINR-Indicators for the tones in iq_tones[0] */
	nrf_dm_iq_tones_sinr_indicator_t tone_sinr_indicators;

	/** Mode used for ranging */
	nrf_dm_ranging_mode_t ranging_mode;
	/** Distance estimate results */
	nrf_dm_dist_estimates_t distance_estimates;

	/** Status of procedure */
	nrf_dm_status_t status;

	/** Computed link loss (TX power - RSSI) */
	uint8_t link_loss;

	/** RSSI measured on this device on packets from remote */
	uint8_t rssi_local;
	/** RSSI measured on remote devices on packets from this device */
	uint8_t rssi_remote;

	/** TX power on this device */
	int8_t txpwr_local;
	/** TX power on remote device */
	int8_t txpwr_remote;

	/** Quality indicator */
	nrf_dm_quality_t quality;

	/** Total time measured during RTT measurements */
	int32_t rtt_accumulated_ns;

	/** Number of RTT measurements taken */
	uint8_t rtt_count;
} nrf_dm_report_t;

typedef struct {
	/** GPIO pin attached to antenna switch. */
	uint8_t gpio;
	/** GPIO_TE channel used for the antenna switching. */
	uint8_t gpio_te;
} nrf_dm_antenna_gpio_t;

typedef struct {
	/** Number of used GPIO pins defined in @ref antenna_gpios */
	uint8_t antenna_gpios_count;
	/** GPIO pins for the antenna switching */
	nrf_dm_antenna_gpio_t *antenna_gpios;
	/** Number of antenna patterns defined in @ref switch_pattern */
	uint8_t switch_pattern_count;
	/** Switch pattern to be used for antenna switching. Expected to be
	 * an array of length @ref switch_pattern_count. Each entry in the
	 * array defines an antenna configuration. If bit N is set in a
	 * configuration, GPIO pin N in @ref antenna_gpios is set.
	 * NOTE: to limit the amount of PPI channels used, minimise the
	 * hamming distance between two successive patterns
	 */
	uint32_t *switch_pattern;
} nrf_dm_antenna_config_t;

#define NRF_DM_DEFAULT_SINGLE_ANTENNA_CONFIG                                               \
	((nrf_dm_antenna_config_t){                                                        \
		.antenna_gpios_count = 0,                                                  \
		.antenna_gpios = NULL,                                                     \
		.switch_pattern_count = 0,                                                 \
		.switch_pattern = NULL,                                                    \
	})

typedef struct {
	/** Number of PPI channels passed to the library */
	uint8_t ppi_chan_count;
	/** PPI channels passed to the library.*/
	uint8_t *ppi_chan;
} nrf_dm_ppi_config_t;

typedef struct {
	/** Role of the device */
	nrf_dm_role_t role;
	/** Ranging mode to use in the procedure */
	nrf_dm_ranging_mode_t ranging_mode;
	/** Seed used in pseudo random number generation. Needs to be the same for an
	 *  initiator and a reflector that will range with eachother.
	 */
	uint32_t rng_seed;
	/** Radio transmit power, given in dBm. Valid values defined in
	 * nrfXXXXX_bitfields.h, as RADIO_TXPOWER_TXPOWER_YYYYdBm
	 */
	uint32_t tx_power_dbm;
	/** Antenna composition of initiator and reflector. */
	nrf_dm_antenna_composition_t ant_comp;
} nrf_dm_config_t;

#if defined(RADIO_TXPOWER_TXPOWER_Pos8dBm)
#define NRF_DM_DEFAULT_TX_POWER RADIO_TXPOWER_TXPOWER_Pos8dBm
#elif defined(RADIO_TXPOWER_TXPOWER_Pos4dBm)
#define NRF_DM_DEFAULT_TX_POWER RADIO_TXPOWER_TXPOWER_Pos4dBm
#elif defined(RADIO_TXPOWER_TXPOWER_Pos1dBm)
#define NRF_DM_DEFAULT_TX_POWER RADIO_TXPOWER_TXPOWER_Pos1dBm
#elif defined(RADIO_TXPOWER_TXPOWER_0dBm)
#define NRF_DM_DEFAULT_TX_POWER RADIO_TXPOWER_TXPOWER_0dBm
#endif

#define NRF_DM_DEFAULT_ANTENNA_COMP (NRF_DM_ANTENNA_COMP_1_1)
/**
 * @brief Default configuration define, note that both role and rng_seed
 * needs to be configured even when using the default config.
 */
#define NRF_DM_DEFAULT_CONFIG                                                          \
	((nrf_dm_config_t){                                                            \
		.role = NRF_DM_ROLE_NONE,                                              \
		.ranging_mode = NRF_DM_RANGING_MODE_MCPD,                              \
		.rng_seed = 0,                                                         \
		.tx_power_dbm = NRF_DM_DEFAULT_TX_POWER,                               \
		.ant_comp = NRF_DM_DEFAULT_ANTENNA_COMP,                               \
	})


/**
 * @brief Initialize the distance measurement procedure
 *
 * @param[in] ppi_cfg PPI channels to be used by the library. The required amount of PPI channels
 * is defined by the maximum hamming distance between two successive patterns in @ref
 * nrf_dm_antenna_config_t.switch_pattern divided by two.
 * On the nrf53 series, only 2 DPPI channels are used.
 * A minimum of 2 PPI channels need to be supplied, even if a single antenna is used.
 * @param[in] antenna_cfg The configuration to use for antenna switching
 * @param[in] timer What timer do you want me to use?
 */
void nrf_dm_init(const nrf_dm_ppi_config_t *ppi_cfg, const nrf_dm_antenna_config_t *antenna_cfg,
		 NRF_TIMER_Type * timer);

/**
 * @brief Configures the distance measurement procedure
 *
 * @param[in] config Pointer to configuration instance
 */
nrf_dm_status_t nrf_dm_configure(const nrf_dm_config_t *config);

/**
 * @brief Get the duration in microseconds that it will take to
 * execute the distance measurement in initiator role
 * with the given configuration.
 * A reflector needs to reserve more time because it needs to be
 * waiting for the initiators initial sync packet.
 * @note A call to @ref nrf_dm_proc_execute will take more time to
 * execute for a given config, since that also includes some pre-processing
 * and post-processing. To get the full expected duration use the constant
 * defined below @ref NRF_DM_PROC_EXECUTE_DURATION_OVERHEAD_US:
 * nrf_dm_get_duration_us(&config) + NRF_DM_PROC_EXECUTE_DURATION_OVERHEAD_US
 *
 * @param[in] config Pointer to configuration instance
 * @return Duration in microseconds.
 */
uint32_t nrf_dm_get_duration_us(const nrf_dm_config_t *config);

/**
 * @brief Maximum overhead duration in microseconds for pre-
 * and post-processing in @ref nrf_dm_proc_execute when executing
 * in initiator role.The full (expected) duration of the call to
 * @ref nrf_dm_proc_execute is given by:
 * nrf_dm_get_duration_us(&config) + NRF_DM_PROC_EXECUTE_DURATION_OVERHEAD_US
 */
#define NRF_DM_PROC_EXECUTE_DURATION_OVERHEAD_US (200)


/**
 * @brief Start the distance measurement.
 * @note Reflector will wait for sync, while initiator will start immediately.
 * After completion it's possible to request distance with @ref nrf_dm_calc.
 *
 * @param [in] timeout_us If the distance measurement is not complete within this time
 *             the measurment will be aborted.
 *             For the measurement to complete the timeout must be at least
 *             the duration given by @ref nrf_dm_get_duration_us for the used
 *             configuration @ref nrf_dm_config_t plus the overhead given by the
 *             constant @ref NRF_DM_PROC_EXECUTE_DURATION_OVERHEAD_US.
 *             A reflector should typically use a larger timeout because it needs
 *             to wait for the initial sync packet sent by the initiator.
 *
 * @return A status code, NRF_DM_STATUS_SUCCESS if the measurement is successful, otherwise
 *         an appropriate failure status.
 */
nrf_dm_status_t nrf_dm_proc_execute(uint32_t timeout_us);

/**
 * @brief Partially populate the report.
 * This populates the report but does not set the distance estimates and the quality.
 * @param[out] p_report Report populated with the raw data from the last ranging.
 */
void nrf_dm_populate_report(nrf_dm_report_t *p_report);

/**
 * @brief This is the length, in number of channels, of the channel map used within the library.
 */
#define NRF_DM_CHANNEL_MAP_LEN 75

/**
 * @brief Get the channel hopping sequence used for a given configuration instance.
 * @note  The hopping sequence is given as a sequence of indexes, where the corresponding
 *        RF frequency is 2.4GHz + (index * 1MHz).
 * @param[in]  config Pointer to configuration instance
 * @param[out] hopping_sequence Array to which the hopping sequence will be written.
 *             needs to be of length @ref NRF_DM_CHANNEL_MAP_LEN
 */
void nrf_dm_get_hopping_sequence(const nrf_dm_config_t *config,
				 uint8_t hopping_sequence[NRF_DM_CHANNEL_MAP_LEN]);

/**
 * @brief  Estimate the distance
 * Fills @ref nrf_dm_report_t with distance estimates either mcpd or
 * rtt according to the raw data in the report.
 * @note The report needs to be populated using @ref nrf_dm_populate_report before @ref nrf_dm_calc
 *       can be called.
 *
 * @param[in,out] p_report Report containing the raw data where the result is written into.
 * @return A quality measure of the estimated distance.
 */
nrf_dm_quality_t nrf_dm_calc(nrf_dm_report_t *p_report);

/**
 * @brief  Estimate the distance with more computation-intensive higher precision algorithm.
 * @note   Only works with MCPD ranging mode.
 * @note   The report needs to be populated using @ref nrf_dm_populate_report before
 *         @ref nrf_dm_high_precision_calc can be called.
 * @param[in,out] p_report Report containing the raw data where the result is written into.
 * @return The estimated distance in meters.
 */
float nrf_dm_high_precision_calc(nrf_dm_report_t *p_report);

/**
 * @brief Get a null-terminated string containing the library revision.
 */
const char *nrf_dm_version_string_get(void);

#ifdef __cplusplus
}
#endif

#endif // __NRF_DM_H_

/**@} */
