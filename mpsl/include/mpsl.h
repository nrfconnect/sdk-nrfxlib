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

/* Reserved (D)PPI, PPIB and IPCT resources for the supported platforms. */
#if defined(NRF52_SERIES)
#define MPSL_RESERVED_PPI_CHANNELS ((1UL << 19) | (1UL << 30) | (1UL << 31))
/* This corresponds to the PPI channels 19, 30, and 31. */
#define MPSL_PPI_CHANNELS_USED_MASK (0xc0080000)
#elif defined(NRF53_SERIES)
#define MPSL_RESERVED_PPI_CHANNELS ((1UL << 0) | (1UL << 1) | (1UL << 2))
/* This corresponds to the DPPI channels 0, 1, and 2. */
#define MPSL_DPPIC_CHANNELS_USED_MASK (0x00000007)
#elif defined(NRF54L_SERIES) || defined(NRF71_SERIES)
#define MPSL_RESERVED_PPI_CHANNELS (1UL << 0)
#define MPSL_DPPIC10_CHANNELS_USED_MASK (0x00000001)
#define MPSL_DPPIC20_CHANNELS_USED_MASK (0x00000001)
#define MPSL_PPIB11_CHANNELS_USED_MASK  (0x00000001)
#define MPSL_PPIB21_CHANNELS_USED_MASK  (0x00000001)
#elif defined(NRF54H_SERIES) || defined(GRTC_PRESENT)
#define MPSL_RESERVED_PPI_CHANNELS (1UL << 0)
#define MPSL_DPPIC020_CHANNELS_USED_MASK (0x00000001)
#define MPSL_IPCT130_CHANNELS_USED_MASK  (0x00000001)
#else
#error Unknown NRF series.
#endif

/* Defines which timer is being used by the MPSL implementation */
#if defined(NRF52_SERIES)
	#define MPSL_TIMER0 NRF_TIMER0
#elif defined(NRF53_SERIES)
	#define MPSL_TIMER0 NRF_TIMER0_NS
#elif defined(NRF54L_SERIES) || defined(NRF71_SERIES)
	#define MPSL_TIMER0 NRF_TIMER10
#else
	#define MPSL_TIMER0 NRF_TIMER020
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
                                 The parameter is not used when external clock driver is registered @ref mpsl_clock_ctrl_source_register().
 * @param[in]  low_prio_irq      IRQ to pend when low priority processing should be executed. The application
 *                               shall call @ref mpsl_low_priority_process after this IRQ has occurred.
 * @param[in]  p_assert_handler  Pointer to MPSL assert handler.
 *
 * @note If `CONFIG_SYSTEM_CLOCK_NO_WAIT` is set to 0,
 *       never modify the SEVONPEND flag in the SCR register,
 *       while this function is executing.
 *       Doing so might lead to a deadlock.
 *
 * @note For nRF54h SoC series the function always waits for LFCLK to be ready. The LFCLK is handled by system controller
 *       so response must arrive from other domain. That shall be done in non-blocking context. To do not change
 *       requirements for other MPSL APIs delayed wait for LFCLK is not allowed for the nRF54h SoC series.
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
 * For nRF52 and nRF53 series the RTC timer is NRF_RTC0.
 * For nRF54 series devices, the RTC timer corresponds to NRF_GRTC.
 *
 * @note       This handler should be placed in the interrupt vector table.
 *             The interrupt priority level should be priority 0
 */
void MPSL_IRQ_RTC0_Handler(void);

/** @brief      TIMER0 interrupt handler.
 *
 * The timer being used is defined by @ref MPSL_TIMER0.
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

/** @brief MPSL requesting CONSTLAT to be on.
 *
 * The application needs to implement this function.
 * MPSL will call the function when it needs CONSTLAT to be on.
 * It only calls the function on nRF54L Series devices.
 */
void mpsl_constlat_request_callback(void);

/** @brief De-request CONSTLAT to be on.
 *
 * The application needs to implement this function.
 * MPSL will call the function when it no longer needs CONSTLAT to be on.
 * It only only calls the function on nRF54L Series devices.
 */
void mpsl_lowpower_request_callback(void);
#ifdef __cplusplus
}
#endif

#endif // MPSL_H__

/**@} */
