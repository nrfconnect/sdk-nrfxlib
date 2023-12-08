/*
 * Copyright (c) 2021, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * @brief Module that defines the Low Power Timer Abstraction Layer for the nrf_802154_sl_timer service
 *
 * @details
 * Design claims:
 * 1. lptimer works in @c lpticks (Low Power Timer Ticks), it is not aware of other timer units with
 *    exception to convenience functions for calculating lpticks to other units.
 * 2. One @c lptick is at least 1 microsecond, but can be greater.
 * 3. lptimer can rarely fire spurious "timer fired" callout as well as rarely perform spurious (D)PPI
 *    triggering (if connections have been made). This phenomena should be avoided, but modules relying
 *    on lptimer must be immune to such behavior.
 * 4. lptimer counts @c lpticks as @c uint64_t type, handling (and hiding from higher layers) any
 *    underlying RTC overflow events.
 */

#ifndef NRF_802154_PLATFORM_SL_LPTIMER_H_
#define NRF_802154_PLATFORM_SL_LPTIMER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Special value to use when no (D)PPI configuration is requested. */
#define NRF_802154_SL_HW_TASK_PPI_INVALID UINT32_MAX

/**@brief Type of result returned by LPTIMER platform functions. */
typedef uint32_t nrf_802154_sl_lptimer_platform_result_t;

/**@brief Operation performed successfully. */
#define NRF_802154_SL_LPTIMER_PLATFORM_SUCCESS      0

/**@brief The timer was scheduled too early in the future. */
#define NRF_802154_SL_LPTIMER_PLATFORM_TOO_LATE     1

/**@brief The timer was scheduled too far in the future. */
#define NRF_802154_SL_LPTIMER_PLATFORM_TOO_DISTANT  2

/**@brief No free hardware resources available to perform the operation. */
#define NRF_802154_SL_LPTIMER_PLATFORM_NO_RESOURCES 3

/**@brief Operation could not be completed due to a wrong state. */
#define NRF_802154_SL_LPTIMER_PLATFORM_WRONG_STATE  4

/** @brief Initializes the Timer.
 */
void nrf_802154_platform_sl_lp_timer_init(void);

/** @brief Deinitializes the Timer.
 */
void nrf_802154_platform_sl_lp_timer_deinit(void);

/**@brief Returns current state of the lptimer in @c lpticks .
 */
uint64_t nrf_802154_platform_sl_lptimer_current_lpticks_get(void);

/**@brief Converts time in microseconds to @c lpticks
 *
 * @param[in] us         Number of microseconds to convert.
 * @param[in] round_up   @c true, to force rounding up. @c false to round down.
 *
 * @return Time corresponding to @p us in @c lpticks.
 */
uint64_t nrf_802154_platform_sl_lptimer_us_to_lpticks_convert(uint64_t us, bool round_up);

/**@brief Converts time in @c lpticks to microseconds.
 *
 * @param[in] lpticks   Number of lpticks to convert.
 *
 * @return Time corresponding to @p lpticks in microseconds.
 */
uint64_t nrf_802154_platform_sl_lptimer_lpticks_to_us_convert(uint64_t lpticks);

/**@brief Schedules a lptimer event to happen at given time.
 *
 * @param[in]   fire_lpticks    Timer state (in @c lpticks) at which timer event should fire.
 *                              If @p fire_lpticks are in the past, the lptimer event will be triggered
 *                              asap, but still from the context of an lptimer's ISR. If
 *                              @ref nrf_802154_platform_sl_lptimer_critical_section_enter is
 *                              in effect, processing will be delayed until pending interrupt
 *                              can be processed.
 */
void nrf_802154_platform_sl_lptimer_schedule_at(uint64_t fire_lpticks);

/**@brief Disables generation of any event scheduled by @ref nrf_802154_platform_sl_lptimer_schedule_at
 *
 * @note If called from ISR priority higher than the priority from which
 * @ref nrf_802154_sl_timer_handler is called, the @ref nrf_802154_sl_timer_handler function
 * may be still called after @ref nrf_802154_platform_sl_lptimer_disable. It is due to the fact
 * that ISR can be preempted in such way, that decision to call @ref nrf_802154_sl_timer_handler
 * has already been taken and the call is inevitable (or is already in process of execution)
 */
void nrf_802154_platform_sl_lptimer_disable(void);

/**@brief Enters into critical section of the lptimer module.
 * @note Critical section can be nested
 *
 * Inside critical section calls to @ref nrf_802154_sl_timer_handler do not happen. This
 * includes indirect effect of calling @ref nrf_802154_platform_sl_lptimer_schedule_at. If necessary
 * the interrupt calling the @ref nrf_802154_sl_timer_handler function it will remain pending
 * at least until a corresponding call to @ref nrf_802154_platform_sl_lptimer_critical_section_exit.
 */
void nrf_802154_platform_sl_lptimer_critical_section_enter(void);

/**@brief Exits out of critical section of the lptimer module.
 */
void nrf_802154_platform_sl_lptimer_critical_section_exit(void);

/**@brief Prepares hardware bindings for triggering hardware task.
 *
 * This function configures timer compare channel to fire at a specified time and publish
 * the signal to a specific PPI channel. The PPI channel may initially be set to
 * @ref NRF_802154_SL_HW_TASK_PPI_INVALID and later updated with
 * @ref nrf_802154_platform_sl_lptimer_hw_task_update_ppi.
 *
 * @param[in]  fire_lpticks  Timer state (in @c lpticks) at which timer event should fire.
 * @param[in]  ppi_channel   (D)PPI channel that compare event will be published to.
 *
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_SUCCESS
 *      The timer was started successfuly and (D)PPI channel was connected, if requested.
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_TOO_LATE
 *      The timer was scheduled too early in the future. The (D)PPI channel was not connected.
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_TOO_DISTANT
 *      The timer was scheduled too far in the future. The (D)PPI channel was not connected.
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_NO_RESOURCES
 *      No available compare channels to schedule a timer. The (D)PPI channel was not connected.
 */
nrf_802154_sl_lptimer_platform_result_t nrf_802154_platform_sl_lptimer_hw_task_prepare(
    uint64_t fire_lpticks,
    uint32_t ppi_channel);

/**@brief Removes hardware bindings created for hardware task and stops the timer.
 *
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_SUCCESS
 *      The cleaning was successful.
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_WRONG_STATE
 *      Cleaning was not performed because the module is in an unsuitable state.
 */
nrf_802154_sl_lptimer_platform_result_t nrf_802154_platform_sl_lptimer_hw_task_cleanup(void);

/**@brief Updates the hardware bindings for triggering hardware task.
 *
 * If the timer was started with @ref nrf_802154_platform_sl_lptimer_hw_task_prepare
 * without specifying a valid (D)PPI channel (@ref NRF_802154_SL_HW_TASK_PPI_INVALID)
 * it is possible to use this function to update the (D)PPI channel to a valid one.
 *
 * @param[in]  ppi_channel  (D)PPI channel that compare event will be published to.
 *
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_SUCCESS
 *      The (D)PPI channel was connected and it was done on time, i.e. before specified
 *      fire time.
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_TOO_LATE
 *      The (D)PPI channel was connected, but it is not sure if it was done in time:
 *      it has been detected that the timer has already fired.
 * @retval NRF_802154_SL_LPTIMER_PLATFORM_WRONG_STATE
 *      The (D)PPI channel was not connected, because the timer is not properly configured.
 */
nrf_802154_sl_lptimer_platform_result_t nrf_802154_platform_sl_lptimer_hw_task_update_ppi(
    uint32_t ppi_channel);

/**
 * @brief Starts a one-shot synchronization timer that expires at the nearest possible timepoint.
 *
 * On timer expiration, the @ref nrf_802154_sl_timestamper_synchronized function is called and the
 * event returned by @ref nrf_802154_platform_sl_lptimer_sync_event_get is triggered.
 *
 * @note @ref nrf_802154_sl_timestamper_synchronized may be called multiple times.
 */
void nrf_802154_platform_sl_lptimer_sync_schedule_now(void);

/**
 * @brief Starts a one-shot synchronization timer that expires at the specified time.
 *
 * This function starts a one-shot synchronization timer that expires at @p fire_lpticks.
 *
 * On timer expiration, @ref nrf_802154_sl_timestamper_synchronized function is called and
 * the event returned by @ref nrf_802154_platform_sl_lptimer_sync_event_get is triggered.
 *
 * @param[in]  fire_lpticks  Timer state (in @c lpticks) at which timer event should fire.
 */
void nrf_802154_platform_sl_lptimer_sync_schedule_at(uint64_t fire_lpticks);

/**
 * @brief Stops the currently running synchronization timer.
 */
void nrf_802154_platform_sl_lptimer_sync_abort(void);

/**
 * @brief Gets the event used to synchronize this timer with the HP Timer.
 *
 * @returns  Address of the peripheral register corresponding to the event
 *           to be used for the timer synchronization.
 *
 */
uint32_t nrf_802154_platform_sl_lptimer_sync_event_get(void);

/**
 * @brief Gets the timestamp of the synchronization event.
 *
 * @returns  Timestamp of the synchronization event.
 */
uint64_t nrf_802154_platform_sl_lptimer_sync_lpticks_get(void);

/**
 * @brief Gets the granularity of the timer.
 *
 * This function can be used to round up or round down the time calculations.
 *
 * @returns Timer granularity in microseconds.
 */
uint32_t nrf_802154_platform_sl_lptimer_granularity_get(void);

/**@brief Handler called from an ISR of lptimer.
 *
 * @param now_lpticks   Specifies moment in time at which the handler is triggered.
 */
extern void nrf_802154_sl_timer_handler(uint64_t now_lpticks);

/**
 * @brief Callback function executed when the synchronization timer expires.
 */
extern void nrf_802154_sl_timestamper_synchronized(void);

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_PLATFORM_SL_LPTIMER_H_
