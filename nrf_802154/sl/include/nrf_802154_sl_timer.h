/*
 * Copyright (c) 2020 - 2021, Nordic Semiconductor ASA
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
 * @brief Module that provides the timer service for the 802.15.4 driver.
 *
 * There are two main features of this module.
 * - @b timer allowing scheduling further action to happen. Object of type
 *   @ref nrf_802154_sl_timer_t (and related functions) is responsible for this feature.
 *
 * - @b timestamper allowing timestamping of hardware events occuring in peripherals.
 *   Object of type @ref nrf_802154_sl_timestamper_t (and related functions) is
 *   responsible for this feature.
 *
 * There are two time resolutions available:
 * - @b coarse with resolution of 1/32768 s
 * - @b fine with resolution of 1 microsecond.
 *
 * Fine resolution time service requires a hardware fine resolution timer to be started which can
 * increase power consumption. The fine resolution timer is started on demand
 * by timestampers or software fine resolution timers. The fine resolution timer is stopped
 * when no timestmpers and no timers require fine resolution anymore.
 */

#ifndef NRF_802154_SL_TIMER_H_
#define NRF_802154_SL_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_sl_timer Timer Service
 * @{
 * @ingroup nrf_802154_sl_timer
 * @brief The Timer Service for the 802.15.4 driver.
 *
 */

/**@brief Number of words required by @ref nrf_802154_sl_timer_priv_placeholder_t. */
#define NRF_802154_SL_TIMER_PRIV_SIZE       1U

/**@brief Number of words required by @ref nrf_802154_sl_timestamper_priv_placeholder_t. */
#define NRF_802154_SL_TIMESTAMPER_PRIV_SIZE 1U

/**@brief Type of result returned by timer-related functions.
 *
 * Possible values:
 * @ref NRF_802154_SL_TIMER_RET_SUCCESS
 * @ref NRF_802154_SL_TIMER_RET_TOO_LATE
 * @ref NRF_802154_SL_TIMER_RET_ACTION_IN_PROGRESS
 * @ref NRF_802154_SL_TIMER_RET_NO_RESOURCES
 * @ref NRF_802154_SL_TIMER_RET_BAD_STATE
 */
typedef uint32_t nrf_802154_sl_timer_ret_t;

/**@brief Operation performed successfully. */
#define NRF_802154_SL_TIMER_RET_SUCCESS            0U

/**@brief Operation was requested too late to be scheduled in requested time. */
#define NRF_802154_SL_TIMER_RET_TOO_LATE           1U

/**@brief Operation was requested when there was no way to prevent previously scheduled activity. */
#define NRF_802154_SL_TIMER_RET_ACTION_IN_PROGRESS 2U

/**@brief There were no available resources required to schedule requested activity.
 *
 * This result may happen for @ref nrf_802154_sl_timer_add with
 * @ref nrf_802154_sl_timer_t::action_type equal to
 * @ref NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE_TASK. To trigger hardware task by the timer,
 * some hardware resources like PPI/DPPI channels, EGU channels, timer compare channels
 * (implementation dependent) were needed, but they were unavailable.
 */
#define NRF_802154_SL_TIMER_RET_NO_RESOURCES       3U

/**@brief The state of a timer object was inappropriate for requested operation. */
#define NRF_802154_SL_TIMER_RET_BAD_STATE          4U

/**@brief Type representing timer. */
typedef struct nrf_802154_sl_timer_s nrf_802154_sl_timer_t;

/**@brief Type of callback function pointer to be called when timer triggers. */
typedef void (* nrf_802154_sl_timer_callback_t)(nrf_802154_sl_timer_t * p_timer);

/**@brief Action types to be performed when a timer triggers.
 *
 * Possible values:
 * @ref NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK
 * @ref NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE_TASK
 */
typedef uint8_t nrf_802154_sl_timer_action_type_t;

/**@brief Timer action type causing a callback to be called. */
#define NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK      0U
/**@brief Timer action type causing a hardware task to be triggered. */
#define NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE_TASK 1U

/**@brief Structure that represents private fields of a timer required by the implementation. */
typedef struct
{
    uint32_t placeholder[NRF_802154_SL_TIMER_PRIV_SIZE];
} nrf_802154_sl_timer_priv_placeholder_t;

struct nrf_802154_sl_timer_s
{
    /**@brief Private fields required by the implementation. */
    nrf_802154_sl_timer_priv_placeholder_t priv;

    /**@brief The timestamp in microseconds at which the timer should be triggered.
     *
     * For action_type == nrf_802154_sl_timer_action_type_callback the timer
     * resolution is always nrf_802154_sl_timer_resolution_coarse. The callback will
     * be called no earlier than at given @p trigger_time, but may be delayed due
     * to processing time taken by other ISRs.
     */
    uint32_t trigger_time;

    /**@brief User-provided data.
     *
     * User preparing a timer can store any data within this field. Timer module does not
     * process this data.
     */
    void * p_user_data;

    /**@brief Action type to be performed when the timer triggers.
     *
     * Depending on value of this field, appropriate version of @c action union variant
     * is valid.
     */
    nrf_802154_sl_timer_action_type_t action_type;

    union
    {
        /**@brief Parameters applicable when @c action_type is equal to
         *        @ref NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK */
        struct
        {
            /**@brief Callback to be called when the timer triggers.
             *
             * The callback is called within an ISR. Please take into account possible
             * latency between moment of trigger and moment when the callback gets called.
             */
            nrf_802154_sl_timer_callback_t callback;
        } callback;

        /**@brief Parameters applicable when @c action_type is equal to
         *        @ref NRF_802154_SL_TIMER_ACTION_HARDWARE_TASK */
        struct
        {
            /**@brief Identifier of (D)PPI channel the task to be triggered subscribes to.
             *
             * The API user is responsible for creating all necessary hardware bindings between
             * provided (D)PPI channel and the requested task, as well as necessary configuration
             * of these resources. Moreover, the API user must keep the hardware bindings enabled
             * and functional until the requested hardware task is triggered.
             */
            uint32_t ppi_channel;
        } hardware_task;
    } action;
};

/**@brief Initializes the timer module. */
void nrf_802154_sl_timer_module_init(void);

/**@brief Uninitializes the timer module. */
void nrf_802154_sl_timer_module_uninit(void);

/**@brief Gets current time.
 *
 * @return Time in microseconds with coarse resolution.
 */
uint32_t nrf_802154_sl_timer_current_time_get(void);

/**@brief Initializes a timer instance.
 *
 * This function plays a role of a constructor. It should be called once
 * per timer instance before any other API call related to given
 * timer instance. Calling any other API related to a timer
 * instance before call to @ref nrf_802154_sl_timer_init or
 * calling @ref nrf_802154_sl_timer_init more than once is an
 * undefined behavior.
 *
 * @param p_timer   Pointer to a timer instance to be initialized.
 */
void nrf_802154_sl_timer_init(nrf_802154_sl_timer_t * p_timer);

/**@brief Adds a timer to the active timers list.
 *
 * This function adds the timer pointed by @p p_timer parameter to an active timers
 * list. Before call to @ref nrf_802154_sl_timer_add the timer must fill
 * public fields of timer object instance in the following way:
 * - @ref nrf_802154_sl_timer_t::trigger_time is set up to the moment when the timer
 *   is expected to trigger
 * - Optionally @ref nrf_802154_sl_timer_t::p_user_data may be set, to user-dependent value.
 * - @ref nrf_802154_sl_timer_t::action_type is set according to requested action.
 * - Depending on value written to @ref nrf_802154_sl_timer_t::action_type
 *   matching version on union @ref nrf_802154_sl_timer_t::action must be filled.
 *
 * When this function succeeds the timer module takes ownership of the timer instance.
 * A user is forbidden to modify structure until callback or @ref nrf_802154_sl_timer_remove
 * is called.
 *
 * @param p_timer   Pointer to a timer instance to add to active timers list.
 *
 * @retval NRF_802154_SL_TIMER_RET_SUCCESS
 *      Function succeeded. The timer is sussessfully added. The requested action is
 *      scheduled to happen.
 * @retval NRF_802154_SL_TIMER_RET_TOO_LATE
 *      The requested trigger_time is too close in the future or is already in the past.
 * @retval NRF_802154_SL_TIMER_RET_NO_RESOURCES
 *      The requested task can't be bound to the timer due to lack of hardware resources.
 * @retval NRF_802154_SL_TIMER_RET_BAD_STATE
 *      A user requested to add a timer actually owned by the timers service.
 */
nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_add(nrf_802154_sl_timer_t * p_timer);

/**@brief Removes a timer from the active timers list.
 *
 * @param p_timer   Pointer to a timer instance to be removed from the active timers list.
 *
 * @retval NRF_802154_SL_TIMER_RET_SUCCESS
 *      The timer was running and it has been successfully removed from active timers list.
 *      The requested action (either callback or hardware task trigger) will not happen.
 * @retval NRF_802154_SL_TIMER_RET_TOO_LATE
 *      The timer was already not running at the moment of call. The request action is finished.
 *      When callback was requested it means that the callback has been called and its execution
 *      finished. The timer is removed from the active timers queue.
 *      When requested action was to trigger hardware task, the task has been triggered.
 *      The timer is removed from the active timers queue and the timer module is no longer
 *      configured to trigger the requested task.
 * @retval NRF_802154_SL_TIMER_RET_ACTION_IN_PROGRESS
 *      The timer was not running but it's requested action has not finished yet.
 *      When callback was requested it means that the timer triggered callback execution,
 *      but the calback has not finished yet its execution. This may happen when the timer
 *      is removed from an ISR of higher priority than ISR performing timer callback execution.
 *      The implementation of user provided callback determines if it is safe to reuse the timer.
 *      When requested action was to trigger hardware task, this result will not be returned.
 */
nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_remove(nrf_802154_sl_timer_t * p_timer);

/**@brief Type representing a timestamper object. */
typedef struct nrf_802154_sl_timestamper_s nrf_802154_sl_timestamper_t;

/**@brief Structure that represents private fields of a timestamper required
 *        by the implementation. */
typedef struct
{
    uint32_t placeholder[NRF_802154_SL_TIMESTAMPER_PRIV_SIZE];
} nrf_802154_sl_timestamper_priv_placeholder_t;

struct nrf_802154_sl_timestamper_s
{
    /**@brief Private fields required by the implementation. */
    nrf_802154_sl_timestamper_priv_placeholder_t priv;
};

/**@brief Type of result returned by timer-related functions.
 *
 * Possible values:
 * @ref NRF_802154_SL_TIMESTAMPER_RET_SUCCESS,
 * @ref NRF_802154_SL_TIMESTAMPER_RET_NO_RESOURCES,
 * @ref NRF_802154_SL_TIMESTAMPER_RET_BAD_STATE,
 * @ref NRF_802154_SL_TIMESTAMPER_RET_NO_EVENT,
 * @ref NRF_802154_SL_TIMESTAMPER_RET_TRY_LATER.
 */
typedef uint32_t nrf_802154_sl_timestamper_ret_t;

/**@brief Operation on a timestamper succeed. */
#define NRF_802154_SL_TIMESTAMPER_RET_SUCCESS      0U

/**@brief Operation on a timestamper failed due to lack of required resources.
 *
 * Timestamper may require hardware compare channels of fine resolution timer
 * to bind given event to timer's capture task. This result indicates that these
 * resources were unavailable.
 */
#define NRF_802154_SL_TIMESTAMPER_RET_NO_RESOURCES 1U

/**@brief The state of a timestamper object was inappropriate for requested operation. */
#define NRF_802154_SL_TIMESTAMPER_RET_BAD_STATE    2U

/**@brief Event which should be timestamped has not triggered yet. */
#define NRF_802154_SL_TIMESTAMPER_RET_NO_EVENT     3U

/**@brief Calculation of timestamp is impossible due to not synchronized fine and coarse
 *        resolution timers.
 *
 * Please refer to @ref nrf_802154_sl_timestamper_timestamp_get for details.
 */
#define NRF_802154_SL_TIMESTAMPER_RET_TRY_LATER    4U

/**@brief Initializes an instance of a timestamper object.
 *
 * This function plays a role of a constructor. It should be called once
 * per timestamper instance before any other API call related to given
 * timestamper instance. Calling any other API related to a timestamper
 * instance before call to @ref nrf_802154_sl_timestamper_init or
 * calling @ref nrf_802154_sl_timestamper_init more than once is an
 * undefined behavior.
 *
 * @param p_timestamper  Pointer to a timestamper object instance to be initialized.
 */
void nrf_802154_sl_timestamper_init(nrf_802154_sl_timestamper_t * p_timestamper);

/**@brief Prepares timestamper instance to allow timestamping of a hardware event.
 *
 * Timestamper can make a single timestamp of a hardware event.
 * In case of successful execution this functions causes:
 * - creation of hardware bindings between given (D)PPI channel and fine resolution timer
 * - start of fine resolution timer if not running yet.
 *
 * When the hardware event connected to the provided (D)PPI channel occurs, timestamp is triggered.
 * The API user is responsible for creating all necessary bindings in hardware between the event
 * to be timestamped and the provided (D)PPI channel, as well as any necessary configuration of
 * these resources. Moreover, the API user must keep the hardware bindings enabled and functional
 * until the event is propagated through the provided (D)PPI channel. If the API user fails
 * to do that, the timestamp will not be taken. That condition can be satisfied easily if the user
 * keeps the hardware bindings enabled and functional until @ref nrf_802154_sl_timestamper_fetch
 * or @ref nrf_802154_sl_timestamper_get is called.
 *
 * The timer module does not provide any way of triggering software action in response to an event.
 * Providing such feature, i.e. appropriate ISR for given event, is a responsibilty
 * of a user. The user should call either @ref nrf_802154_sl_timestamper_fetch
 * or @ref nrf_802154_sl_timestamper_timestamp_get as early after event occurrence as
 * practically possible to release hardware resources used for timestamping.
 *
 * When the user is done with the timestamper @ref nrf_802154_sl_timestamper_cleanup
 * should be called. When the user wishes to re-use the timestamper instance,
 * @ref @ref nrf_802154_sl_timestamper_cleanup should be called first and then
 * @ref nrf_802154_sl_timestamper_setup should be called.
 *
 * @param p_timestamper Pointer to a timestamper object that is responsible for
 *                      keeping timestamper state.
 * @param ppi_channel   Identifier of (D)PPI channel that the hardware event to be timestamped
 *                      by @p p_timestamper publishes to. If @p ppi_channel represents
 *                      PPI channel (as opposed to DPPI channel), then the TEP register associated
 *                      with that channel must not be configured, as the timestamping mechanism
 *                      relies on it to be free.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_SUCCESS
 *      The timestamper setup was successful.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_NO_RESOURCES
 *      There was no hardware resources to perform timestamping.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_BAD_STATE
 *      The timestamper was already setup to perform timestamping and no cleanup
 *      has been called yet.
 */
nrf_802154_sl_timestamper_ret_t nrf_802154_sl_timestamper_setup(
    nrf_802154_sl_timestamper_t * p_timestamper,
    uint32_t                      ppi_channel);

/**@brief Fetches data required to calculate timestamp.
 *
 * This function fetches data allowing calculation of absolute timestamp by further call to
 * @ref nrf_802154_sl_timestamper_timestamp_get. When it succeeds, some resources required for
 * timestamping (compare channels, hardware event bindings) are released and can be
 * re-used.
 *
 * When fine resolution timer is synchronized to coarse resolution timer, all data
 * allowing successfull @ref nrf_802154_sl_timestamper_timestamp_get is stored within
 * timestamper object.
 *
 * When fine resolution timer is not synchronized yet to coarse resolution timer,
 * almost all data is stored within timestamper object and remaining data will be stored
 * in timestamper object by synchronization event.
 *
 * Call to this function does not stop fine resolution timer as this can be done by
 * @ref nrf_802154_sl_timestamper_cleanup only.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_SUCCESS
 *      The event being timestamped has triggered. The data required to calculate timestamp
 *      has been latched within the timestamper object. The hardware bindings between event and
 *      timer have been cleaned up as they have become not necessary.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_BAD_STATE
 *      The timestamper has not been setup for timestamping (@ref nrf_802154_sl_timestamper_setup
 *      has not been called before). The timestamper state remains unchanged.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_NO_EVENT
 *      The event being timestamped has not triggered yet. The timestamper state remains
 *      unchanged.
 */
nrf_802154_sl_timestamper_ret_t nrf_802154_sl_timestamper_fetch(
    nrf_802154_sl_timestamper_t * p_timestamper);

/**@brief Gets timestamp made by a timestamper.
 *
 * This function calls internally @ref nrf_802154_sl_timestamper_fetch when necessary.
 * After that when all data is stored within timestamper object it calculates
 * timestamp.
 *
 * Call to this function does not stop fine resolution timer as this can be done by
 * @ref nrf_802154_sl_timestamper_cleanup only.
 *
 * The timestamper object can be re-used by call to @ref nrf_802154_sl_timestamper_cleanup
 * and then @ref nrf_802154_sl_timestamper_setup.
 *
 * @param p_timestamper     Pointer to a timestamper instance.
 * @param p_timestamp       Timestamp in microseconds of an event will be written
 *                          at this pointer if the function returns
 *                          @ref NRF_802154_SL_TIMESTAMPER_RET_SUCCESS.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_SUCCESS
 *      The timestamp is available and written at p_timestamp pointer.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_BAD_STATE
 *      The timestamper has not been setup for timestamping, i.e.
 *      @ref nrf_802154_sl_timestamper_setup has not been called before.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_NO_EVENT
 *      The event being timestamped has not triggered yet.
 *
 * @retval NRF_802154_SL_TIMESTAMPER_RET_TRY_LATER
 *      The event has triggered and the data allowing timestamping is fetched, but
 *      currently this data doesn't allow calculation of absolute timestamp.
 *      The timestamp will be available later when coarse and fine clocks are synchronized.
 */
nrf_802154_sl_timestamper_ret_t nrf_802154_sl_timestamper_timestamp_get(
    nrf_802154_sl_timestamper_t * p_timestamper,
    uint32_t                    * p_timestamp);

/**@brief Cleans up after timestamping.
 *
 * This function definitely ends timestamping process related to given timestamper instance.
 * This function must be called complementary to @ref nrf_802154_sl_timestamper_setup
 * no matter if user managed to get the timestamp or not.
 *
 * This function is responsible for cleaning up after @ref nrf_802154_sl_timestamper_setup.
 * It removes any hardware bindings (if still existing) made
 * by @ref nrf_802154_sl_timestamper_setup, frees any resources allocated by the call and
 * releases fine resolution timer (if still requested). Please note that fine resolution timer
 * can be left running if other resources require it (other timestampers, timers etc.).
 *
 * When the user calls this function on a timestamper instance which has not been setup
 * for timestamping, i.e. @ref nrf_802154_sl_timestamper_setup has not been called,
 * the call does nothing.
 *
 * If user didn't call @ref nrf_802154_sl_timestamper_timestamp_get the timestamp
 * (if it has been ever made) will be lost, but it allows abandoning of timestamping
 * and releases used resources.
 *
 * @param p_timestamper  Pointer to a timestamper instance to be cleaned up.
 */
void nrf_802154_sl_timestamper_cleanup(nrf_802154_sl_timestamper_t * p_timestamper);

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SL_TIMER_H_ */
