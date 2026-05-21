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
 * The algorithm can dynamically make an assessment of the battery state-of-health (SOH)
 * for secondary cell batteries.
 * SOH is a percentage value of the modelled battery capacity. A new battery is expected to have a
 * SOH value close to 100%. As the battery ages, the SOH value will decrease.
 *
 * The SOH estimation is based on observing full charge cycles, and will improve over time.
 * To facilitate the SOH estimate, the iteration rate can be increased while charging.
 * For example to a rate around 2 Hz to 5 Hz.
 * When @ref nrf_fuel_gauge_config_parameters.soh_estimate_enable is set, the library will
 * automatically estimate and apply the SOH value to the SOC estimate.
 * The SOH estimate can be retrieved using @ref nrf_fuel_gauge_soh_get,
 * and overridden using @ref nrf_fuel_gauge_config_params_adjust.
 *
 * @note As this library has two variants, make sure to use a battery model that matches the
 * configured variant: primary cell (non-rechargeable) or secondary cell (rechargeable).
 *
 * @note Current and voltage measurements do not have to be made at a certain interval.
 *       However, the two values should be measured as closely as possible in time.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Helper macro to get basic initialization parameters for primary cell variant
 * 
 * @param[in] _v0 Initial voltage in volts [V].
 * @param[in] _i0 Initial current in amperes [A].
 * @param[in] _t0 Initial temperature in centigrades [C].
 * @param[in] _battery_model Pointer to battery model struct.
 */
#define NRF_FUEL_GAUGE_DEFAULT_INIT_PARAMETERS_PRIMARY(_v0, _i0, _t0, _battery_model)\
	((struct nrf_fuel_gauge_init_parameters) {\
		.v0 = _v0,\
		.i0 = _i0,\
		.t0 = _t0,\
		.model_primary = _battery_model,\
		.opt_params = NULL,\
		.state = NULL,\
		.lock_func = NULL,\
	})

/**
 * @brief Helper macro to get basic initialization parameters for secondary cell variant
 * 
 * @param[in] _v0 Initial voltage in volts [V].
 * @param[in] _i0 Initial current in amperes [A].
 * @param[in] _t0 Initial temperature in centigrades [C].
 * @param[in] _battery_model Pointer to battery model struct.
 */
#define NRF_FUEL_GAUGE_DEFAULT_INIT_PARAMETERS_SECONDARY(_v0, _i0, _t0, _battery_model)\
	((struct nrf_fuel_gauge_init_parameters) {\
		.v0 = _v0,\
		.i0 = _i0,\
		.t0 = _t0,\
		.model = _battery_model,\
		.opt_params = NULL,\
		.state = NULL,\
		.lock_func = NULL,\
	})

enum nrf_fuel_gauge_variant_type {
	/** Primary cell (non-rechargeable) battery variant. */
	NRF_FUEL_GAUGE_VARIANT_PRIMARY,
	/** Secondary cell (rechargeable) battery variant. */
	NRF_FUEL_GAUGE_VARIANT_SECONDARY,
};

/**
 * @brief Version number of nRF Fuel Gauge library in format "x.y.z".
 */
extern const char *nrf_fuel_gauge_version;

/**
 * @brief Build date of nRF Fuel Gauge library.
 */
extern const char *nrf_fuel_gauge_build_date;

/**
 * @brief Build variant of nRF Fuel Gauge library.
 */
extern const enum nrf_fuel_gauge_variant_type nrf_fuel_gauge_variant;

/**
 * @brief Size of the internal state memory used by nRF Fuel Gauge library.
 */
extern const size_t nrf_fuel_gauge_state_size;

/**
 * @brief Secondary cell (rechargeable) battery model parameters.
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
 * @brief Primary cell (non-rechargeable) battery model parameters.
 *
 * @details Parameter details purposefully not documented.
 */
struct battery_model_primary {
	float param_4[201];
	float param_3[201];
	float param_1[201];
	float param_5[201];
	float param_6[201];
	float param_2[88];
	float param_8[88];
	float param_7[88];
	float param_13[201];
	float param_14[201];
	float param_12;
	float param_15;
	float param_16;
	signed char param_17[20];
	float param_18[2];
	float param_19;
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
	/**
	 * Truncated temperature. This state is used when the measured temperature falls outside
	 * the modeled range.
	 */
	float T_truncated;
	/** Corrected battery current. */
	float i_corrected;
	/** Raw state-of-charge value, not rounded off. */
	float soc_raw;
	/** The raw state-of-health value, not subject to the specified minimum battery capacity. */
	float soh_raw;
	/**
	 * Battery charge cycle count.
	 * Only applicable for secondary cell variant.
	 */
	uint16_t cycle_count;
	/**
	 * Battery cumulative charge in mAh since last full cycle.
	 * Only applicable for secondary cell variant.
	 */
	float cycle_count_cumulative_mah;
	/** Filter gains */
	float k1;
	float k2;
};

/**
 * @brief Configuration parameters.
 *
 * @details Use @ref nrf_fuel_gauge_config_params_default_get to initialize this struct with
 *          default values, or @ref nrf_fuel_gauge_config_params_current_get to get the currently
 *          used values.
 *
 * @details Some parameter details purposefully not documented.
 */
struct nrf_fuel_gauge_config_parameters {
	/**
	 * Primary cell: when true, do not let state-of-charge increase.
	 * Secondary cell: when true, do not let state-of-charge increase unless when charging.
	 */
	bool discard_positive_deltaz;
	/** Minimum duration of battery charge/discharge before TTE becomes available [s] */
	float tte_min_time;
	/**
	 * Reset threshold for TTE calculation. If short-term state-of-charge gradient differs
	 * from the long-term state-of-charge by this factor or more, the TTE state is reset.
	 */
	float tte_reset_threshold;
	/**
	 * Enable state-of-health estimation.
	 * Only applicable for secondary cell variant.
	 */
	bool soh_estimate_enable;
	/**
	 * Enable state-of-health estimation from partial charge.
	 * If this is set to false, state-of-health will only be estimated when
	 * @ref NRF_FUEL_GAUGE_CHARGE_STATE_COMPLETE is reported.
	 * Only applicable for secondary cell variant.
	 */
	bool soh_partial_charge_enable;
	/**
	 * Initial battery state-of-health, as a percentage of modeled battery capacity.
	 */
	float soh;
	/**
	 * State-of-charge low threshold for state-of-health calculation [%].
	 * State-of-health calculations will begin when charging starts at this state-of-charge
	 * level or lower.
	 * Only applicable for secondary cell variant.
	 */
	float soh_calc_threshold;
	/**
	 * Minimum capacity of a fresh battery in mAh.
	 * Use this option to account for typical variation for a battery type, such that a new battery
	 * always has 100% state-of-health. If this is set to NAN, a default minimum capacity of 95%
	 * of the modeled battery capacity will be used.
	 * Only applicable for secondary cell variant.
	 */
	float battery_min_capacity;
	/**
	 * Initial battery charge cycle count.
	 * Only applicable for secondary cell variant.
	 */
	uint16_t cycle_count;
	/**
	 * Initial battery cumulative charge in mAh since last full cycle.
	 * Only applicable for secondary cell variant.
	 */
	float cycle_count_cumulative_mah;
	/* The following parameters are purposefully undocumented */
	float tau1;
	float tau2;
	float neta1;
	float neta2;
	float beta1;
	float beta2;
	float gating;
	float volt_drop;
	uint8_t vk_buffer_length;
	float soh_alpha1;
	float soh_alpha2;
	float soh_raw;
	float a;
	float b;
	float c;
	float d;
	float cycle_repl_min;
	float soh_up_cap_cycle_min;
	float soh_up_max;
	float soh_rep_up_max;
	float soh_jump_thr;
	float soh_partial_min_deltaSOC;
	float soh_partial_R_penalty;
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
	/** Pointer to static battery model. The model is not copied and must be kept valid. */
	union {
		/** Secondary cell battery model, when using secondary cell variant of library. */
		const struct battery_model *model;
		/** Primary cell battery model, when using primary cell variant of library. */
		const struct battery_model_primary *model_primary;
	};
	/**
	 * Optional configuration parameters. Set to NULL to use default values, or use
	 *  @ref nrf_fuel_gauge_config_params_default_get.
	 */
	const struct nrf_fuel_gauge_config_parameters *opt_params;
	/**
	 * Optional lock function for synchronizing access to library state when using the library
	 * from multiple contexts. Set to NULL when not used.
	 *
	 * @param context Context pointer provided to the lock function.
	 * @param lock When true, the function should acquire the lock. Otherwise release it.
	 * @retval 0 if lock/unlock successful, negative error code if failed.
	 */
	int (*lock_func)(const void *context, bool lock);
	/** Optional context pointer provided to the lock function. Set to NULL when not used. */
	const void *lock_context;
	/**
	 * Optional state memory, previously retrieved using @ref nrf_fuel_gauge_state_get.
	 * When used, the library resumes from this state.
	 * @ref opt_params and initial measurements are ignored when state is provided.
	 * Set to NULL when not used or when initializing the library for the first time.
	 *
	 * The state buffer is copied into library internal state memory,
	 * and the provided buffer can be freed or reused after initialization.
	 *
	 * @note If the battery model memory address has changed since the state was stored, the
	 * @ref model or @ref model_primary pointer will always take precedence when it is not NULL.
	 *
	 * @note After a library version update, the state from the old version might not be compatible
	 * with the new version, and @ref nrf_fuel_gauge_init will return -EBADF. In this case,
	 * call the function again without the state argument.
	 * Use @ref nrf_fuel_gauge_state_compatible_check to validate compatibility.
	 *
	 * @note If there is a significant difference between the measured battery voltage and the
	 * battery voltage in the stored state, state-of-charge converge will be slower than an
	 * initialization not using a stored state.
	 */
	const void *state;
	/** Size of the state buffer. Ignored when @ref state is NULL. */
	size_t state_size;
};

/**
 * @brief Charger state enum.
 *
 * @details The library uses this (optional) information to improve predictions.
 *
 * @note Time-to-full (TTF) prediction requires this information.
 */
enum nrf_fuel_gauge_charge_state {
	/** Charger not active. */
	NRF_FUEL_GAUGE_CHARGE_STATE_IDLE,
	/** Trickle charging. */
	NRF_FUEL_GAUGE_CHARGE_STATE_TRICKLE,
	/**
	 * Constant current charging (fast charging) with charge current indicated by
	 * @ref NRF_FUEL_GAUGE_EXT_STATE_INFO_CHARGE_CURRENT_LIMIT.
	 */
	NRF_FUEL_GAUGE_CHARGE_STATE_CC,
	/**
	 * Constant current charging (fast charging), limited by low vbus voltage,
	 * USB current restriction, temperature, or other factors. Charge current will be lower than
	 * the limit indicated by @ref NRF_FUEL_GAUGE_EXT_STATE_INFO_CHARGE_CURRENT_LIMIT.
	 */
	NRF_FUEL_GAUGE_CHARGE_STATE_CC_LIMITED,
	/** Constant voltage charging. */
	NRF_FUEL_GAUGE_CHARGE_STATE_CV,
	/** Charging completed. */
	NRF_FUEL_GAUGE_CHARGE_STATE_COMPLETE,
};

/**
 * @brief State info type used for informing the library of external factors and events.
 *
 * @details The library uses this (optional) information to improve predictions.
 */
enum nrf_fuel_gauge_ext_state_info_type {
	/** VBUS/USB has been connected to supply external power */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_VBUS_CONNECTED,
	/** VBUS/USB has been disconnected */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_VBUS_DISCONNECTED,
	/** Change in charger state (as determined by charger device) */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_CHARGE_STATE_CHANGE,
	/** Multiply current by this factor when charging */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_CHARGE_CURRENT_CORRECTION,
	/** Multiply current by this factor when discharging */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_DISCHARGE_CURRENT_CORRECTION,
	/** Specify the charge current limit used by the charger */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_CHARGE_CURRENT_LIMIT,
	/** Specify the termination current used by the charger */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_TERM_CURRENT,
	/** The battery has been replaced */
	NRF_FUEL_GAUGE_EXT_STATE_INFO_BATTERY_REPLACED,
};

/**
 * @brief State info data type used for additional information.
 */
union nrf_fuel_gauge_ext_state_info_data {
	/** Change in charge state */
	enum nrf_fuel_gauge_charge_state charge_state;
	/** Factor used for current measurement correction */
	float current_correction_factor;
	/** Battery charge current limit [A] */
	float charge_current_limit;
	/** Battery charge termination current [A] */
	float charge_term_current;
};

/**
 * @brief Initialize configuration parameter structure to default values.
 *
 * @param[in] params Pointer to the parameter structure.
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_config_params_default_get(struct nrf_fuel_gauge_config_parameters *params);

/**
 * @brief Initialize configuration parameter structure to the currently used values.
 *
 * @param[in] params Pointer to the parameter structure.
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_config_params_current_get(struct nrf_fuel_gauge_config_parameters *params);

/**
 * @brief Check if the provided state is compatible with the current library version.
 *
 * @param[in] state Pointer to the state structure.
 * @param[in] state_size Size of the state structure.
 * @retval 0 when successful.
 * @retval -EINVAL Invalid argument.
 * @retval -EBADF State is not compatible with current library version.
 */
int nrf_fuel_gauge_state_compatible_check(const void *state, size_t state_size);

/**
 * @brief Initialize the nRF Fuel Gauge library.
 *
 * @details The library can be initialized based on a previously stored state.
 *          This is useful when there are periods without RAM retention.
 *          To resume from a previously stored state, the battery model must be the same
 *          as when the library was first initialized.
 *
 * @note The battery model referenced in @ref nrf_fuel_gauge_init_parameters must be kept valid
 *       as long as the library is in use.
 *
 * @param[in] parameters Pointer to the parameter structure.
 * @param[out] v0 Adjusted battery voltage (optional, for logging purposes).
 * @retval 0 Initialization successful.
 * @retval -22 Initialization failed due to invalid parameters.
 */
int nrf_fuel_gauge_init(const struct nrf_fuel_gauge_init_parameters *parameters, float *v0);

/**
 * @brief Process battery measurement values.
 *
 * @details State-of-charge for primary cell battery models may be rounded off.
 *          The raw value can always be obtained from the optional state info struct.
 *
 * @param[in] v Measured battery voltage [V].
 * @param[in] i Measured battery current [A].
 * @param[in] T Measured battery temperature [C].
 * @param[in] t_delta Time delta since previous processed measurement [s].
 * @param[out] soc Pointer to predicted state-of-charge [%].
 * @param[out] state Pointer to state info struct (optional, set to null to ignore).
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_process(float v, float i, float T, float t_delta, float *soc,
			     struct nrf_fuel_gauge_state_info *state);

/**
 * @brief Get predicted state-of-charge value.
 *
 * @details This function returns the most recently calculated state-of-charge value
 *          generated by @ref nrf_fuel_gauge_process.
 * @param[out] soc Pointer to predicted state-of-charge [%].
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_soc_get(float *soc);

/**
 * @brief Get predicted "time-to-empty" discharge duration.
 *
 * @details Time-to-empty is calculated based on the discharge rate. It may be NAN.
 *
 * @note Primary cell variant will always return NAN.
 *
 * @param[out] tte Pointer to predicted time-to-empty [s].
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_tte_get(float *tte);

/**
 * @brief Get predicted "time-to-full" charging duration.
 *
 * @details Time-to-full is calculated based on the charge rate. It may be NAN.
 *
 * @note Primary cell variant will always return NAN.
 *
 * @param[out] ttf Pointer to predicted time-to-full [s].
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_ttf_get(float *ttf);

/**
 * @brief Get predicted battery state-of-health (SOH) as a percentage of modeled battery capacity.
 *
 * @details State-of-health is calculated when a sufficient number of charge cycles has been
 *          observed. It is important to use the @ref nrf_fuel_gauge_ext_state_update function
 *          to inform the library of the charger behavior for state-of-health to get calculated.
 *
 *          This predicition will get more accurate over time.
 *
 *          To preserve the state-of-health related data, store the fuel gauge state using
 *          @ref nrf_fuel_gauge_state_get and resume from the stored state when re-initializing the
 *          library. The state can be stored before performing a reset or entry to a low-power
 *          state with no RAM retention, or when the battery is getting critically low.
 *          To reduce energy usage and wear on non-volatile storage, only store after the
 *          state-of-health estimate has changed or when the charge cycle count has increased.
 *
 * @note Only applicable for secondary cell variant.
 *
 * @param[out] soh Pointer to predicted state-of-health [%].
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_soh_get(float *soh);

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
 * @param[in] v Measured battery voltage in volts [V].
 * @param[in] T Measured battery temperature in centigrades [C].
 * @param[in] i_avg Expected average current in idle state in amperes [A].
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_idle_set(float v, float T, float i_avg);

/**
 * @brief Retrieve library state data.
 *
 * @details The state data can be stored in persistent storage during a period of no RAM
 *          retention, and later used to resume the library from the same state.
 *
 * @param[out] state Memory to copy state into.
 * @param[in] size Size of buffer. Must be at least @ref nrf_fuel_gauge_state_size.
 *
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_state_get(void *state, size_t size);

/**
 * @brief Optional function to inform library of external events and factors.
 *
 * @details Informing the library of certain external events and factors can help improve
 *          battery state predictions.
 *
 * @note @ref nrf_fuel_gauge_ttf_get relies on external charger state information.
 *
 * @param[in] type Type of external event.
 * @param[in] data Additional data for certain info types. Set to NULL when not relevant.
 *
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_ext_state_update(enum nrf_fuel_gauge_ext_state_info_type type,
	union nrf_fuel_gauge_ext_state_info_data *data);

/**
 * @brief Update configuraton parameters.
 *
 * @details Parameter details purposefully not documented. NAN float values will be ignored.
 *
 * @param[in] params New parameter values. Values set to NAN will be ignored.
 * @retval 0 when successful, otherwise negative error code.
 */
int nrf_fuel_gauge_config_params_adjust(const struct nrf_fuel_gauge_config_parameters *params);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __NRF_FUEL_GAUGE_H__ */
