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
#include "nrfx.h"
#if !defined (NRF54H_SERIES) && !defined (NRF92_SERIES)
#include "hal/nrf_clock.h"
#endif /* !NRF54H20_XXAA && !NRF92H20_XXAA */

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

/** @brief Worst LF clock accuracy allowed by the BLUETOOTH CORE SPECIFICATION Version 6.0 | Vol 6, Part B
 *         4.2.2 Sleep clock accuracy in parts per million (ppm).
 */
#define MPSL_WORST_CLOCK_ACCURACY_PPM  500

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
  MPSL_CLOCK_HF_LATENCY_WORST_CASE = 1400,
} mpsl_clock_hfclk_latency_config_t;

/** @brief Supported high frequency clock sources. */
typedef enum {
  MPSL_CLOCK_HF_SRC_XO = 0,
#if defined(NRF_CLOCK_HAS_HFCLK24M) && NRF_CLOCK_HAS_HFCLK24M
  MPSL_CLOCK_HF_SRC_HFCLK24M = 1, /**< HFCLK24M. */
#endif /* NRF_CLOCK_HAS_HFCLK24M */
  MPSL_CLOCK_HF_SRC_MAX = 2,
} mpsl_clock_hfclk_src_t;

/** @brief Event types returned by the hfclk callback handler. */
typedef enum
{
  /** @brief HFCLK has been started.
   *
   * @note On nRF54L series SoCs the event is returned when the clock
   *       has started and is stable (EVENT_XOTUNED has occurred).
   */
  MPSL_CLOCK_EVT_HFCLK_STARTED = 0,
#if defined(NRF_CLOCK_HAS_HFCLK24M) && NRF_CLOCK_HAS_HFCLK24M
  /** @brief HFCLK24M has been started. */
  MPSL_CLOCK_EVT_HFCLK24M_STARTED = 1,
#endif /* NRF_CLOCK_HAS_HFCLK24M */
#if defined(NRF_CLOCK_HAS_XO_TUNE) && NRF_CLOCK_HAS_XO_TUNE
  MPSL_CLOCK_EVT_XO_TUNED = 2, /**< XO tune has been done. */
#endif /* NRF_CLOCK_HAS_XO_TUNE */
  MPSL_CLOCK_EVT_MAX = 3,
} mpsl_clock_evt_type_t;

/** @brief High frequency clock callback.
 *
 * This callback will be called when the high frequency clock is started.
 * It will be executed in the same execution priority as @ref mpsl_low_priority_process.
 */
typedef void (*mpsl_clock_hfclk_callback_t)(void);

/** @brief Request the high frequency crystal oscillator.
 * 
 * @deprecated Use ::mpsl_clock_hfclk_src_request instead. This function will be removed in a future release.
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
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @param[in] hfclk_started_callback Function to be called when the high frequency clock is started.
 *                                   On nRF54L series SoCs the callback is called when the clock
 *                                   has started and is stable (EVENT_XOTUNED has occurred).
 *                                   The callback will be executed in the context as
 *                                   @ref mpsl_low_priority_process.
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_request(mpsl_clock_hfclk_callback_t hfclk_started_callback);

/** @brief Releases the high frequency crystal oscillator.
 * 
 * @deprecated Use ::mpsl_clock_hfclk_src_release instead. This function will be removed in a future release.
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
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_release(void);

/** @brief Checks if the high frequency crystal oscillator is running.
 * 
 * @deprecated Use ::mpsl_clock_hfclk_src_is_running instead. This function will be removed in a future release.
 *
 * @see mpsl_clock_hfclk_request
 * @see mpsl_clock_hfclk_release
 *
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @param[out] p_is_running 1 if the external crystal oscillator is running, 0 if not.
 *
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_is_running(uint32_t * p_is_running);

/** @brief Informs MPSL about the actual ramp-up time of the high-frequency crystal oscillator.
 *
 * @note Using a value smaller than the actual ramp-up time needed will cause asserts.
 *
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @param[in] hfclk_rampup_time_us Ramp-up time of the high-frequency oscillator, in microseconds. See @ref mpsl_clock_hfclk_latency_config_t for recommended values.
 *
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_latency_set(uint16_t hfclk_rampup_time_us);

/** @brief Trigger a task upon start of the RTC.
 *
 * MPSL will trigger the task at the same time as the RTC is started.
 *
 * The function is supported only for nRF52 and nRF53 series.
 *
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @param[in] task_address The task address to be triggered
 */
void mpsl_clock_task_trigger_on_rtc_start_set(uint32_t task_address);

/** @brief High frequency clock callback.
 *
 * This callback will be called when the high frequency clock is started.
 * It will be executed in the same execution priority as @ref mpsl_low_priority_process.
 *
 * For list of event types that can be provided to this callback, see @ref mpsl_clock_evt_type_t.
 *
 * @param[in] evt_type The event type that occurred.
 */
typedef void (*mpsl_clock_hfclk_request_callback_t)(mpsl_clock_evt_type_t evt_type);

/** @brief Request a high frequency clock with a given source.
 *
 * Will start the high frequency clock with a given source, the startup time of the clock varies
 * and the ::mpsl_clock_hfclk_src_is_running function can be polled to check if it has started.
 *
  * @note Don't use this API if the integration layer of MPSL provides a driver that uses this function.
 *       This is the case for applications in the nRF Connect SDK where there is a clock control driver
 *       with a corresponding on/off manager.
 *
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @see mpsl_clock_hfclk_src_is_running
 * @see mpsl_clock_hfclk_src_release
 *
 * @param[in] src The high frequency clock source requested.
 * @param[in] hfclk_started_callback The callback to be called when the high frequency clock is started.
 *                                   If the function is called multiple times before the clock is started,
 *                                   the callback will be superseded by the last callback.
 * @retval 0  Success, negative value in case of failure.
 *
 * @note On nRF54L series SoCs the callback is called when the clock
 *                                   has started and is stable (EVENT_XOTUNED has occurred).
 *                                   The callback will be executed in the context as
 *                                   @ref mpsl_low_priority_process. 
 */
int32_t mpsl_clock_hfclk_src_request(mpsl_clock_hfclk_src_t src, mpsl_clock_hfclk_request_callback_t hfclk_started_callback);

/** @brief Releases a high frequency clock for a given source.
 *
 * Indicates that the high frequency clock for a given source is not needed by the application.
 * MPSL may continue to use the high frequency clock if it is a source requested by protocol stacks.
 * MPSL will automatically turn it off when it is no longer needed.
 *
 * @see mpsl_clock_hfclk_src_is_running
 * @see mpsl_clock_hfclk_src_request
 *
 * @note Don't use this API if the integration layer of MPSL provides a driver that uses this function.
 *       This is the case for applications in the nRF Connect SDK where there is a clock control driver
 *       with a corresponding on/off manager.
 *
 * @note This API is not supported when an external clock driver has been registered.

 * @param[in] src The high frequency clock source to release.
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_src_release(mpsl_clock_hfclk_src_t src);

/** @brief Checks if the high frequency clock for a given source is running.
 *
 * @see mpsl_clock_hfclk_src_request
 * @see mpsl_clock_hfclk_src_release
 *
 * @note This API is not supported when an external clock driver has been registered.
 *       See also @ref mpsl_clock_ctrl_source_register().
 *
 * @param[in] src The high frequency clock source to check.
 * @param[out] p_is_running 1 if the high frequency clock is running, 0 if not.
 *
 * @retval 0  Success
 */
int32_t mpsl_clock_hfclk_src_is_running(mpsl_clock_hfclk_src_t src, uint32_t * p_is_running);

/** @brief Type representing LFCLK clock handling external API that is expected by MPSL to be
 *         delivered on init.
 */
typedef struct
{
  /** @brief Pointer to function that waits for LFCLK to be started
   *
   * This API will be called from the same execution context as @ref mpsl_low_priority_process.
   *
   * The pointer is mandatory for all supported platforms.
   *
   * @return Non-negative value in case of success. Negative value in case of failure.
   */
  int32_t (*lfclk_wait)(void);

  /** @brief Pointer to function that triggers LFCLK callibration start
   *
   * The function may used from high priority or low priority execution context.
   *
   * The pointer is mandatory for nRF52, nRF53 series.
   */
  void (*lfclk_calibration_start)(void);

  /** @brief Pointer to function that checks if LFCLK callibration is enabled
   *
   * The function may used from high priority or low priority execution context.
   *
   * The pointer is mandatory for nRF52 series.
   *
   * @return true if calibration is enabled, false otherwise.
   */
  bool (*lfclk_calibration_is_enabled)(void);

  /** @brief Pointer to function that requests LFCLK
   *
   * This API will be called from the same execution context as @ref mpsl_init()
   *
   * The pointer is mandatory for all supported platforms.
   *
   * @return Non-negative value in case of success. Negative value in case of failure.
   */
  int32_t (*lfclk_request)(void);

  /** @brief Pointer to function that releases LFCLK
   *
   * This API will be called from the same execution context as @ref mpsl_uninit()
   *
   * The pointer is mandatory for all supported platforms.
   *
   * @return Non-negative value in case of success. Negative value in case of failure.
   */
  int32_t (*lfclk_release)(void);

  /** @brief Value of available LFCLK accuracy
   *
   * The value is mandatory for all supported platforms.
   */
  uint16_t accuracy_ppm;

  /** @brief Determines whether MPSL waits for the low frequency clock to start during initialization or not.
   *  If it is set to true, MPSL will wait for the low frequency clock later, before the low frequency
   *  clock is used for the first time.
   *
   * @note For nRF54h SoC series the MPSL always waits for LFCLK in mpsl_init(). Blocking wait at
   *       later stage may prevent Radio core to get notification from system controller about LFCLK
   *       being ready.
   */
  bool skip_wait_lfclk_started;
} mpsl_clock_lfclk_ctrl_source_t;

/** @brief Type representing HFCLK clock handling external API that is expected by MPSL to be
 *         delivered on init.
 */
typedef struct
{
  /** @brief Pointer to function that puts HFCLK request
   *
   * The function is used from high priority context.
   *
   * The pointer is mandatory for all supported platforms.
   */
  void (*hfclk_request)(void);

  /** @brief Pointer to function that releases HFCLK request
   *
   * The function is used from high priority context.
   *
   * The pointer is mandatory for all supported platforms.
   */
  void (*hfclk_release)(void);

  /** @brief Pointer to function that checks if HFCLK is running
   *
   * The function is used from high priority context.
   *
   * The pointer is mandatory for all supported platforms.
   *
   * @return true if HFCLK is running, false otherwise
   */
  bool (*hfclk_is_running)(void);

  /** @brief Value of a HFXO startup time.
   *
   * The value is mandatory for all supported platforms.
   */
  uint16_t startup_time_us;
} mpsl_clock_hfclk_ctrl_source_t;

/** @brief Register an external clock driver.
 *
 * When this API is called, MPSL will use an external clock driver instead of the
 * built-in clock driver.
 *
 * @note The API must be called before mpsl_init() to enable MPSL use external clock driver.
 *
 * The API is supported for nRF54h series. It is experimental for nRF52, nRF53 series.
 *
 * @param[in] p_lfclk_ctrl_source A pointer to struct holding the API to LFCLK control.
 * @param[in] p_hfclk_ctrl_source A pointer to struct holding the API to HFCLK control.
 *
 * @note Any of the clock control pointers can't be NULL. In such case  the function
 *        return -NRF_EINVAL.
 *
 * @retval 0               Clock control API is successfully registered.
 * @retval -NRF_EPERM      Clock control API is already initialized.
 * @retval -NRF_EINVAL     Invalid parameters supplied.
 */
int32_t mpsl_clock_ctrl_source_register(const mpsl_clock_lfclk_ctrl_source_t * p_lfclk_ctrl_source,
                                        const mpsl_clock_hfclk_ctrl_source_t * p_hfclk_ctrl_source);

/** @brief Unregisters external clock driver from MPSL.
 *
 * The function shall not be called when MPSL is initialized.
 *
 * @note The API must be called after the mpsl_uninit() to disable MPSL use external clock driver.
 *
 * The API is supported for nRF54h series. It is experimental for nRF52, nRF53 series.
 *
 * @retval 0               Clock control API is successfully unregistered.
 * @retval -NRF_EPERM      Clock control module is still initialized.
 */
int32_t mpsl_clock_ctrl_source_unregister(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_CLOCK_H__

/**@} */
