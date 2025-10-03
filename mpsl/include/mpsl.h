/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl.h
 *
 * @defgroup mpsl Multiprotocol Service Layer interface
 *
 * The main APIs needed to configure, enable, and use the MPSL.
 * @{
 */

#ifndef MPSL_H__
#define MPSL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "nrf_peripherals.h"
#include "nrf_errno.h"
#include "mpsl_clock.h"

/** @brief High IRQ priority
 *
 *  High IRQ priority is used for realtime scheduling. It
 *  should not be used except when inside radio
 *  timeslot by the owner of the timeslot */
#define MPSL_HIGH_IRQ_PRIORITY 0

/** @brief Size of build revision array in bytes. */
#define MPSL_BUILD_REVISION_SIZE 20

/** @brief Bitmask of (D)PPI channels reserved for MPSL. */
#if defined(PPI_PRESENT)
#define MPSL_RESERVED_PPI_CHANNELS ((1UL << 19) | (1UL << 30) | (1UL << 31))
#elif defined(GRTC_PRESENT)
#define MPSL_RESERVED_PPI_CHANNELS (1UL << 0)
#elif defined(DPPIC_PRESENT)
#define MPSL_RESERVED_PPI_CHANNELS ((1UL << 0) | (1UL << 1) | (1UL << 2))
#else
#error Unknown NRF series.
#endif

/** @brief    Function prototype for the assert handler.
 *
 * @note      If an internal assert occurs this function is called. It is supposed to log the assert and stop execution.
 *
 * @param[in] file   The filename where the assertion occurred.
 * @param[in] line   The line number where the assertion occurred.
 */
typedef void (*mpsl_assert_handler_t)(const char * const file, const uint32_t line);

/** @brief     MPSL initialization
 *
 * @param[in]  p_clock_config    Clock configuration.
                                 If NULL the LF clock will be configured as an RC source with rc_ctiv =
                                 @ref MPSL_RECOMMENDED_RC_CTIV, .rc_temp_ctiv =
                                 @ref MPSL_RECOMMENDED_RC_TEMP_CTIV, and .accuracy_ppm = @ref MPSL_DEFAULT_CLOCK_ACCURACY_PPM.
 * @param[in]  low_prio_irq      IRQ to pend when low priority processing should be executed. The application
 *                               shall call @ref mpsl_low_priority_process after this IRQ has occurred.
 * @param[in]  p_assert_handler  Pointer to MPSL assert handler.
 *
 * @note If `CONFIG_SYSTEM_CLOCK_NO_WAIT` is set to 0,
 *       never modify the SEVONPEND flag in the SCR register,
 *       while this function is executing.
 *       Doing so might lead to a deadlock.
 *
 * @note If only Front End Module functionality is needed, @ref mpsl_fem_init can be called instead.
 *
 * @retval  0               MPSL is successfully initialized.
 * @retval  -NRF_EPERM      MPSL is already initialized.
 * @retval  -NRF_EINVAL     Invalid parameters supplied.
 */
int32_t mpsl_init(mpsl_clock_lfclk_cfg_t const * p_clock_config, IRQn_Type low_prio_irq, mpsl_assert_handler_t p_assert_handler);

/** @brief      Uninitialize MPSL. Stops clocks and scheduler. This will release all peripherals and
 *             reduce power usage.
 *
 * @note       This function assumes no protocol stacks are running, and no timeslots are requested.
 *             All initialized protocol stacks need to be stopped before calling this function.
 *             Failing to do so will lead to undefined behavior.
 */
void mpsl_uninit(void);

/** @brief      Returns true if MPSL is already initialized, false otherwise.
 *
 *  @returns    True if initialized, false if not.
 */
bool mpsl_is_initialized(void);

/** @brief Obtain build revision
 *
 * The application must provide a buffer that is at least @ref MPSL_BUILD_REVISION_SIZE
 * bytes long. MPSL will copy the build revision to the provided buffer.
 *
 *  @param[in,out] p_build_revision  Build revision.
 *
 * @retval 0              Success
 * @retval -NRF_EINVAL    Invalid argument provided
 */
int32_t mpsl_build_revision_get(uint8_t * p_build_revision);

/** @brief      RADIO interrupt handler
 *
 * @note       This handler should be placed in the interrupt vector table.
 *             The interrupt priority level should be priority 0.
 */
void MPSL_IRQ_RADIO_Handler(void);

/** @brief      RTC0 interrupt handler
 *
 * @note       This handler should be placed in the interrupt vector table.
 *             The interrupt priority level should be priority 0
 */
void MPSL_IRQ_RTC0_Handler(void);

/** @brief      TIMER0 interrupt handler.
 *
 * @note       This handler should be placed in the interrupt vector table.
 *             The interrupt priority level should be priority 0
 */
void MPSL_IRQ_TIMER0_Handler(void);

/** @brief      POWER_CLOCK interrupt handler
 *
 * @note       This handler should be placed in the interrupt vector table.
 *             The interrupt priority level should be lower than priority 0.
 */
void MPSL_IRQ_CLOCK_Handler(void);

/** @brief MPSL low priority processing handler.
 *
 * @note This handler should be called when MPSL signals low priority processing should be executed
 *       (via low_prio_irq provided to @ref mpsl_init) within reasonable time (a at least a few
 *       100 ms). The caller is responsible to ensure this function is not called concurrently with
 *       any other low priority MPSL API functions, for more information see thread safety in the
 *       MPSL documentation.
 */
void mpsl_low_priority_process(void);

/** @brief Application needs to call this when calibration shall occur.
 *
 * In the nRF Connect SDK, it is designed to be called with a period of CONFIG_CLOCK_CONTROL_NRF_CALIBRATION_PERIOD
 */
void mpsl_calibration_timer_handle(void);

/** @brief RFU
 *
 * RFU
 */
void mpsl_pan_rfu(void);
#ifdef __cplusplus
}
#endif

#endif // MPSL_H__

/**@} */
