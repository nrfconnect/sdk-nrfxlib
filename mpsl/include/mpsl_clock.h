/*
 * Copyright (c) 2011 - 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file mpsl_clock.h
 *
 * @defgroup mpsl_clock MPSL Clock interface
 * @ingroup  mpsl
 *
 * The MPSL clock interface provides APIs for controlling the clocks.
 * @{
 */

#ifndef MPSL_CLOCK_H__
#define MPSL_CLOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "nrf.h"
#include "nrf_errno.h"

/** @brief Low frequency clock source. */
enum MPSL_CLOCK_LF_SRC
{
    MPSL_CLOCK_LF_SRC_RC             = 0, /**< LFCLK RC oscillator. */
    MPSL_CLOCK_LF_SRC_XTAL           = 1, /**< LFCLK crystal oscillator. */
    MPSL_CLOCK_LF_SRC_SYNTH          = 2, /**< LFCLK Synthesized from HFCLK. */
#if defined(NRF52_SERIES)
    MPSL_CLOCK_LF_SRC_EXT_LOW_SWING  = 3, /**< Apply external low swing signal to XL1, ground XL2.
                                               This option is not available on nRF53. */
    MPSL_CLOCK_LF_SRC_EXT_FULL_SWING = 4, /**< Apply external full swing signal to XL1,
                                               leave XL2 grounded or unconnected.
                                               This option is not available on nRF53.*/
#endif
};

/** @brief Recommended RC clock calibration timer interval. */
#define MPSL_RECOMMENDED_RC_CTIV        16

/** @brief Recommended RC clock calibration timer interval for temperature changes. */
#define MPSL_RECOMMENDED_RC_TEMP_CTIV   2

/** @brief Default LF clock accuracy in parts per million (ppm). */
#define MPSL_DEFAULT_CLOCK_ACCURACY_PPM  250

/** @brief Type representing LFCLK oscillator source. */
typedef struct
{
    uint8_t source;         /**< LF oscillator clock source, see @ref MPSL_CLOCK_LF_SRC. */
    uint8_t rc_ctiv;        /**< Only for ::MPSL_CLOCK_LF_SRC_RC.
                                 Calibration timer interval in 1/4 second units.
                                 @note To avoid excessive clock drift, 0.5 degrees Celsius is the
                                       maximum temperature change allowed in one calibration timer
                                       interval. The interval should be selected to ensure this.
                                 @note Must be 0 if source is not ::MPSL_CLOCK_LF_SRC_RC. */
    uint8_t rc_temp_ctiv;   /**< Only for ::MPSL_CLOCK_LF_SRC_RC: How often (in number of calibration
                                 intervals) the RC oscillator shall be calibrated if the temperature
                                 hasn't changed.
                                      0: Always calibrate even if the temperature hasn't changed.
                                      2-33: Check the temperature and only calibrate if it has changed,
                                            however calibration will take place every rc_temp_ctiv
                                            intervals in any case.

                                 @note Must be 0 if source is not ::MPSL_CLOCK_LF_SRC_RC.

                                 @note The application must ensure calibration at least once every
                                       8 seconds to ensure +/-500 ppm clock stability.
                                       The recommended configuration for ::MPSL_CLOCK_LF_SRC_RC
                                       is given by @ref MPSL_RECOMMENDED_RC_CTIV
                                       and @ref MPSL_RECOMMENDED_RC_TEMP_CTIV.
                                       This sets the calibration interval to 4 seconds and guarantees
                                       clock calibration every second calibration interval. That is,
                                       the clock will be calibrated every 8 seconds.
                                       If the temperature changes more that 0.5 every 4 seconds, the
                                       clock will be calibrated every 4 seconds. See the
                                       Product Specification for more information. */
    uint16_t accuracy_ppm;  /**< Accuracy of the low frequency clock in parts per million (ppm). Default value is @ref MPSL_DEFAULT_CLOCK_ACCURACY_PPM.*/
} mpsl_clock_lfclk_cfg_t;

/** @brief High frequency clock callback.
 *
 * This callback will be called when the high frequency clock is started.
 * It will be executed in the same execution priority as @ref mpsl_low_priority_process.
 */
typedef void (*mpsl_clock_hfclk_callback_t)(void);

/** @brief Request the high frequency crystal oscillator.
 *
 * Will start the high frequency crystal oscillator, the startup time of the crystal varies
 * and the ::mpsl_clock_hfclk_is_running function can be polled to check if it has started.
 *
 * @see mpsl_clock_hfclk_is_running
 * @see mpsl_clock_hfclk_release
 *
 * @param[in] hfclk_started_callback Function to be called when the high frequency clock is started.
 *                                   The callback will be executed in the context as
 *                                   @ref mpsl_low_priority_process.
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_request(mpsl_clock_hfclk_callback_t hfclk_started_callback);

/** @brief Releases the high frequency crystal oscillator.
 *
 * Indicates that the high frequency crystal oscillator is not needed by the application.
 * MPSL may continue to use the high frequency clock if it is requested by protocol stacks.
 * MPSL will automatically turn it off when it is no longer needed.
 *
 * @see mpsl_clock_hfclk_is_running
 * @see mpsl_clock_hfclk_request
 *
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_release(void);

/** @brief Checks if the high frequency crystal oscillator is running.
 *
 * @see mpsl_clock_hfclk_request
 * @see mpsl_clock_hfclk_release
 *
 * @param[out] p_is_running 1 if the external crystal oscillator is running, 0 if not.
 *
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_is_running(uint32_t * p_is_running);

#ifdef __cplusplus
}
#endif

#endif // MPSL_CLOCK_H__

/**@} */
