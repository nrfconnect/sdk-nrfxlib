/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_fem_config_simple_gpio.h
 *
 * @defgroup mpsl_fem_simple_gpio MPSL Simple GPIO Front End Module Configuration
 * @ingroup  mpsl_fem
 *
 * @{
 */

#ifndef MPSL_FEM_CONFIG_SIMPLE_GPIO_H__
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_H__

#include <stdint.h>
#include <stdbool.h>
#include "mpsl_fem_config_common.h"
#include "nrfx.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NRF52_SERIES)
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_PPI_CHANNELS_COUNT  2
#else
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_DPPI_CHANNELS_COUNT 3
#define MPSL_FEM_CONFIG_SIMPLE_GPIO_EGU_CHANNELS_COUNT  3
#endif

/** @brief Configuration parameters for the Front End Module Simple GPIO variant.
 *
 *  A Simple GPIO Front End Module may be used with all Front End Modules
 *  which use one wire for a Power Amplifier (PA) and one wire for a Linear
 *  Noise Amplifier (LNA).
 */
typedef struct
{
    /** Configration structure of the Simple GPIO Front End Module. */
    struct
    {
        /** Time between the activation of the PA pin and the start of the radio transmission.
         *  Should be no bigger than Radio Ramp-Up time. */
        uint32_t pa_time_gap_us;
        /** Time between the activation of the LNA pin and the start of the radio reception.
         *  Should be no bigger than Radio Ramp-Up time. */
        uint32_t lna_time_gap_us;
        /** Configurable PA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   pa_gain_db;
        /** Configurable LNA gain. Ignored if the amplifier is not supporting this feature. */
        int8_t   lna_gain_db;
        /** Effective TX/RX gain in dB when the Front End Module bypass path is selected.
         *  This parameter is usually negative to express attenuation.
         *
         *  Set to 0 to disable bypass handling. A non-zero value together with enabled
         *  @ref tx_bypass_pin_config and/or @ref rx_bypass_pin_config (on SoCs with DPPI) selects
         *  bypass TX via @ref mpsl_fem_tx_power_split and bypass RX via @ref mpsl_fem_lna_is_configured.
         */
        int8_t bypass_gain_db;
    } fem_config;

    /** Power Amplifier pin configuration. */
    mpsl_fem_gpiote_pin_config_t pa_pin_config;
    /** Low Noise Amplifier pin configuration. */
    mpsl_fem_gpiote_pin_config_t lna_pin_config;
    /**
     * TX bypass pin configuration.
     *
     * GPIO used to select the Front End Module TX bypass path (lower gain/attenuated TX) as
     * opposed to the PA path. Works together with @c fem_config.bypass_gain_db.
     *
     * Bypass is active only when @c fem_config.bypass_gain_db is not @c 0 and
     * @ref mpsl_fem_gpiote_pin_config_t.enable is @c true. On nRF52 series (PPI) TX bypass is
     * not supported.
     *
     * When MPSL chooses bypass for a transmission, PA timing also drives this pin (activate and
     * deactivate in parallel with @c pa_pin_config). When MPSL chooses the PA path, @c pa_pin_config
     * alone is driven and this pin is left out of PA activation.
     *
     * Same configuration as @c lna_pin_config:
     * On some devices the TX bypass select shares the LNA control line. Set this field equal to
     * @c lna_pin_config (same GPIO, @c active_high, @c enable, and @c gpiote_ch_id). The driver
     * detects the shared pin and routes bypass GPIOTE tasks through the LNA tasks so hardware
     * timing stays consistent.
     *
     * Dedicated bypass GPIO:
     * Use a different GPIO and GPIOTE channel than @c lna_pin_config and @c pa_pin_config. The
     * driver toggles that line together with @c pa_pin_config only for bypass TX.
     *
     * Same GPIO as @c pa_pin_config:
     * Not supported for normal Front End Module wiring (PA and bypass are separate controls).
     */
    mpsl_fem_gpiote_pin_config_t tx_bypass_pin_config;
    /**
     * RX bypass pin configuration.
     *
     * GPIO used to select the Front End Module RX bypass path as opposed to the LNA path.
     * Works together with @c fem_config.bypass_gain_db.
     *
     * RX bypass is active only when @c fem_config.bypass_gain_db is not @c 0 and
     * @ref mpsl_fem_gpiote_pin_config_t.enable is @c true. On nRF52 series (PPI) RX bypass is
     * not supported.
     *
     * When enabled, @ref mpsl_fem_lna_is_configured reports @c fem_config.bypass_gain_db instead
     * of @c fem_config.lna_gain_db, and LNA timing drives this pin in parallel with
     * @c lna_pin_config on activate and deactivate.
     *
     * Same configuration as @c pa_pin_config:
     * On some devices the RX bypass select shares the PA control line. Set this field equal to
     * @c pa_pin_config (same GPIO, @c active_high, @c enable, and @c gpiote_ch_id). The driver
     * aliases RX bypass GPIOTE tasks to the PA tasks.
     *
     * Same configuration as @c tx_bypass_pin_config:
     * When one GPIO selects bypass for both TX and RX, set this field equal to
     * @c tx_bypass_pin_config. The driver aliases RX bypass tasks to the TX bypass tasks (after
     * TX bypass/LNA aliasing is applied).
     *
     * Dedicated bypass GPIO:
     * Use a different GPIO and GPIOTE channel than @c pa_pin_config, @c tx_bypass_pin_config,
     * and @c lna_pin_config. The driver toggles that line together with @c lna_pin_config for
     * every reception.
     *
     * Same GPIO as @c lna_pin_config:
     * Not supported.
     */
    mpsl_fem_gpiote_pin_config_t rx_bypass_pin_config;

#if defined(NRF52_SERIES)
    /** Array of PPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      ppi_channels[MPSL_FEM_CONFIG_SIMPLE_GPIO_PPI_CHANNELS_COUNT];
#else
    /** Array of DPPI channels which need to be provided to Front End Module to operate. */
    uint8_t                      dppi_channels[MPSL_FEM_CONFIG_SIMPLE_GPIO_DPPI_CHANNELS_COUNT];
    /** Number of EGU instance for which @c egu_channels apply. */
    uint8_t                      egu_instance_no;
    /** Array of EGU channels (belonging to EGU instance number @c egu_instance_no) which
     *  need to be provided to Front End Module to operate. */
    uint8_t                      egu_channels[MPSL_FEM_CONFIG_SIMPLE_GPIO_EGU_CHANNELS_COUNT];
#endif

} mpsl_fem_simple_gpio_interface_config_t;

/** @brief Configures the PA and LNA device interface.
 *
 * This function sets device interface parameters for the PA/LNA module.
 * The module can then be used to control PA or LNA (or both) through the given interface and resources.
 *
 * The function also sets the PPI and GPIOTE channels to be configured for the PA/LNA interface.
 *
 * @param[in] p_config Pointer to the interface parameters for the PA/LNA device.
 *
 * @retval   0             PA/LNA control successfully configured.
 * @retval   -NRF_EPERM    PA/LNA is not available.
 *
 */
int32_t mpsl_fem_simple_gpio_interface_config_set(mpsl_fem_simple_gpio_interface_config_t const * const p_config);

/**
 * @brief Simple GPIO Front End Module Timings
 *
 * A Simple GPIO Front End Module may be used with all Front End Modules which
 * use one wire for PA and one wire for LNA.
 * The timing restrictions should be obtained from its corresponding datasheet.
 */

/** Time in microseconds when PA GPIO is activated before the radio is ready for transmission. */
#define MPSL_FEM_SIMPLE_GPIO_DEFAULT_PA_TIME_IN_ADVANCE_US  23

/** Time in microseconds when LNA GPIO is activated before the radio is ready for reception. */
#define MPSL_FEM_SIMPLE_GPIO_DEFAULT_LNA_TIME_IN_ADVANCE_US 5

/**
 * @brief Simple GPIO Front End Module Gains
 *
 * A Simple GPIO Front End Module may be used with all Front End Modules which
 * use one wire for PA and one wire for LNA.
 * The gains should be obtained from its corresponding datasheet.
 */

/** Gain of the PA in dB. */
#define MPSL_FEM_SIMPLE_GPIO_PA_DEFAULT_GAIN_DB   22

/** Gain of the LNA in dB. */
#define MPSL_FEM_SIMPLE_GPIO_LNA_DEFAULT_GAIN_DB  11

#ifdef __cplusplus
}
#endif

#endif // MPSL_FEM_CONFIG_SIMPLE_GPIO_H__

/**@} */
