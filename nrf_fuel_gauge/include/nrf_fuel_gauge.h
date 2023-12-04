/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 * @brief  nRF Fuel Gauge library API.
 */

#ifndef __NRF_FUEL_GAUGE_H__
#define __NRF_FUEL_GAUGE_H__

/**
 * @defgroup nrf_fuel_gauge nRF Fuel Gauge library
 * @{
 *
 * @brief Algorithm for battery state-of-charge estimation ("fuel gauge").
 *
 * @details The algorithm determines the predicted state-of-charge (SOC) based on the following
 * data: specific model of the battery type used in the application along with
 * measurements of battery voltage, current, and temperature.
 * The SOC is a value between 0% and 100%.
 *
 * @note Current and voltage measurements do not have to be made at a certain interval.
 *       However, the two values should be measured as closely as possible in time.
 */

#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version number of nRF Fuel Gauge library in format "x.y.z".
 */
extern const char *nrf_fuel_gauge_version;

/**
 * @brief Build date of nRF Fuel Gauge library.
 */
extern const char *nrf_fuel_gauge_build_date;

/**
 * @brief Battery model parameters.
 *
 * @details Parameter details purposefully not documented.
 */
struct battery_model {
	float param_1[201];
	float temps[3];
	float param_2[122];
	float param_3[201];
	float param_4[201];
	float param_5[201];
	float param_6[201];
	float param_7[122];
	float param_8[122];
	float param_9[3];
	float param_10[6];
	float param_11[6];
	float param_12[3];
	char name[64];
};

/**
 * @brief Library state information. Useful for debugging.
 */
struct nrf_fuel_gauge_state_info {
	/** Predicted voltage. */
	float yhat;
	/** Battery resistance. */
	float r0;
	/** Truncated temperature. This state is used when the measured temperature falls outside
	 * the modeled range.
	 */
	float T_truncated;
};

/**
 * @brief Optional static configuration parameters.
 *
 * @details Parameter details purposefully not documented.
 */
struct nrf_fuel_gauge_config_parameters {
	float tau;
	float neta1;
	float neta2;
	float neta3;
};

/**
 * @brief Optional runtime configuration parameters.
 *
 * @details Parameter details purposefully not documented.
 */
struct nrf_fuel_gauge_runtime_parameters {
	float a;
	float b;
	float c;
	float d;
};

/**
 * @brief Library init parameter struct.
 */
struct nrf_fuel_gauge_init_parameters {
	/** Initial voltage in volts [V]. */
	float v0;
	/** Initial current in amperes [A]. */
	float i0;
	/** Initial temperature in centigrades [C]. */
	float t0;
	/** Battery model */
	const struct battery_model *model;
	/** Optional configuration parameters. Set to NULL to use default values. */
	const struct nrf_fuel_gauge_config_parameters *opt_params;
};

/**
 * @brief Initialize the nRF Fuel Gauge library.
 *
 * @param[in] parameters Pointer to the parameter structure.
 * @param[out] v0 Adjusted battery voltage (optional, for logging purposes).
 *
 * @retval 0 Initialization successful.
 * @retval -22 Initialization failed due to invalid parameters.
 */
int nrf_fuel_gauge_init(const struct nrf_fuel_gauge_init_parameters *parameters, float *v0);

/**
 * @brief Process battery measurement values.
 *
 * @param v Measured battery voltage [V].
 * @param i Measured battery current [A].
 * @param T Measured battery temperature [C].
 * @param t_delta Time delta since previous processed measurement [s].
 * @param state Pointer to state info struct (optional, set to null to ignore).
 *
 * @retval Predicted state-of-charge [%].
 */
float nrf_fuel_gauge_process(
		float v, float i, float T, float t_delta, struct nrf_fuel_gauge_state_info *state);

/**
 * @brief Get predicted "time-to-empty" discharge duration.
 *
 * @details Time-to-empty is calculated based on the discharge rate. It may be NAN.
 *
 * @retval Predicted time-to-empty [s].
 */
float nrf_fuel_gauge_tte_get(void);

/**
 * @brief Get predicted "time-to-full" charging duration.
 *
 * @details Time-to-full is calculated based on the charge rate. It may be NAN.
 *
 * @param cc_charging True if ongoing charging is in constant current (CC) state. Otherwise false.
 * @param i_term Termination current [A].
 *
 * @retval Predicted time-to-full [s].
 */
float nrf_fuel_gauge_ttf_get(bool cc_charging, float i_term);

/**
 * @brief Put library into the idle state.
 *
 * @details Informing the library of idle states helps improve predictions when there are periods
 *          of low activity and no new battery measurements.
 *          If this function is not called,
 *          discharge current in low power states may be over-estimated.
 *          For example, consider a system that enters a state where only the low frequency
 *          oscillator is running to generate a wakeup sometime later. To minimize the power usage
 *          during this period, no battery measurements are made. The idle current is likely
 *          to be too low to accurately measure dynamically, but it is known ahead of time.
 *          Using this function with the expected average current will help the library
 *          more accurately predict the power drain during the idle, low-power period.
 *          When activity is resumed, use @ref nrf_fuel_gauge_process as normal.
 *
 * @note Make sure this function is called from the same context as @ref nrf_fuel_gauge_process.
 *
 * @param v Measured battery voltage in volts [V].
 * @param T Measured battery temperature in centigrades [C].
 * @param i_avg Expected average current in idle state in amperes [A].
 */
void nrf_fuel_gauge_idle_set(float v, float T, float i_avg);

/**
 * @brief Update runtime parameters.
 *
 * @details Parameter details purposefully not documented.
 *
 * @param params New parameter values.
 */
void nrf_fuel_gauge_param_adjust(const struct nrf_fuel_gauge_runtime_parameters *params);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __NRF_FUEL_GAUGE_H__ */
