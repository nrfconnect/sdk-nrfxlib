/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
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
#include <stdbool.h>
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

/** @brief MPSL waits for low frequency clock to start by default. */
#define MPSL_DEFAULT_SKIP_WAIT_LFCLK_STARTED false

/** @brief Type representing LFCLK oscillator source. */
typedef struct
{
  /** LF oscillator clock source, see @ref MPSL_CLOCK_LF_SRC. */
  uint8_t source;

  /** Only for ::MPSL_CLOCK_LF_SRC_RC.
   *  Calibration timer interval in 1/4 second units.
   *
   *  @note To avoid excessive clock drift, 0.5 degrees Celsius is
   *        the maximum temperature change allowed in one calibration timer interval.
   *        The interval should be selected to ensure this.
   *
   *  @note Must be 0 if source is not ::MPSL_CLOCK_LF_SRC_RC.
   */
  uint8_t rc_ctiv;

  /** Only for ::MPSL_CLOCK_LF_SRC_RC.
   *  How often (in number of calibration intervals) the RC oscillator shall be
   *  calibrated if the temperature hasn't changed.
   *  * 0: Always calibrate even if the temperature hasn't changed.
   *  * 1-33: Check the temperature and only calibrate if it has changed,
   *          however calibration will take place every rc_temp_ctiv intervals in any case.
   *
   *  @note Must be 0 if source is not ::MPSL_CLOCK_LF_SRC_RC.
   *
   *  @note The application must ensure calibration at least
   *        once every 8 seconds to ensure +/-500 ppm clock stability.
   *        The recommended configuration for ::MPSL_CLOCK_LF_SRC_RC
   *        is given by @ref MPSL_RECOMMENDED_RC_CTIV and
   *        @ref MPSL_RECOMMENDED_RC_TEMP_CTIV.
   *        This sets the calibration interval to 4 seconds
   *        and guarantees clock calibration every second calibration interval.
   *        That is, the clock will be calibrated every 8 seconds.
   *        If the temperature changes more than 0.5 every 4 seconds,
   *        the clock will be calibrated every 4 seconds.
   *        See the Product Specification for more information.
   */
  uint8_t rc_temp_ctiv;

  /** Accuracy of the low frequency clock in parts per million (ppm).
   *  Default value is @ref MPSL_DEFAULT_CLOCK_ACCURACY_PPM.
   */
  uint16_t accuracy_ppm;

  /** Determines whether MPSL waits for the low frequency clock to start
   *  during initialization or not.
   *  If it is set to true, MPSL will wait for the low frequency clock later,
   *  before the low frequency clock is used for the first time.
   *  Default value is @ref MPSL_DEFAULT_SKIP_WAIT_LFCLK_STARTED.
   *
   * @note If this option is set to true and the application writes to
   *       NRF_CLOCK->TASKS_LFCLKSTART before mpsl_init() is called,
   *       the application shall either:
   *       - Write to NRF_CLOCK->TASKS_LFCLKSTOP.
   *       - Keep NRF_CLOCK->EVENTS_LFCLKSTARTED untouched.
   */
  bool skip_wait_lfclk_started;
} mpsl_clock_lfclk_cfg_t;

/* @brief Available options for informing MPSL of the true high-frequency oscillator ramp-up time, in microseconds. */
typedef enum
{
  MPSL_CLOCK_HF_LATENCY_BEST = 396,
  MPSL_CLOCK_HF_LATENCY_TYPICAL = 854,
  MPSL_CLOCK_HF_LATENCY_WORST_CASE = 1525,
} mpsl_clock_hfclk_latency_config_t;

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
 * @note Don't use this API if the integration layer of MPSL provides a driver that uses this function.
 *       This is the case for applications in the nRF Connect SDK where there is a clock control driver
 *       with a corresponding on/off manager.
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
 * @note Don't use this API if the integration layer of MPSL provides a driver that uses this function.
 *       This is the case for applications in the nRF Connect SDK where there is a clock control driver
 *       with a corresponding on/off manager.
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

/** @brief Informs MPSL about the actual ramp-up time of the high-frequency crystal oscillator.
 *
 * @param[in] mpsl_clock_hfclk_latency_config Setting for the time it takes for the HFCLK to ramp up.
 *
 * @retval 0  Success
 * @retval 1  Error, passed value not in @ref mpsl_clock_hfclk_latency_config_t
 */
int32_t mpsl_clock_hfclk_latency_set(mpsl_clock_hfclk_latency_config_t mpsl_clock_hfclk_latency_config);

/** @brief Trigger a task upon start of the RTC.
 *
 * MPSL will trigger the task at the same time as the RTC is started.
 *
 * @param[in] task_address The task address to be triggered
 */
void mpsl_clock_task_trigger_on_rtc_start_set(uint32_t task_address);

#ifdef __cplusplus
}
#endif

#endif // MPSL_CLOCK_H__

/**@} */
