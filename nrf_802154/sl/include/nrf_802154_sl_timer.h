/*
 * Copyright (c) 2020, Nordic Semiconductor ASA
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
 * The @b timer module allows scheduling future actions to happen. Object of type
 * @ref nrf_802154_sl_timer_t (and related functions) is responsible for this feature.
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

/**@brief Number of array elements required by @ref nrf_802154_sl_timer_priv_placeholder_t. */
#define NRF_802154_SL_TIMER_PRIV_SIZE 2U

/**@brief Type of result returned by timer-related functions.
 *
 * Possible values:
 * @ref NRF_802154_SL_TIMER_RET_SUCCESS
 * @ref NRF_802154_SL_TIMER_RET_TOO_LATE
 * @ref NRF_802154_SL_TIMER_RET_TOO_DISTANT
 * @ref NRF_802154_SL_TIMER_RET_NO_RESOURCES
 * @ref NRF_802154_SL_TIMER_RET_INACTIVE
 * @ref NRF_802154_SL_TIMER_RET_BAD_REQUEST
 */
typedef uint32_t nrf_802154_sl_timer_ret_t;

/**@brief Operation performed successfully. */
#define NRF_802154_SL_TIMER_RET_SUCCESS      0U

/**@brief Passed invalid parameters to a call.
 * This result may occur if the user filled incorrect values in the request
 * object or given feature is not supported yet
 */
#define NRF_802154_SL_TIMER_RET_BAD_REQUEST  1U

/**@brief Operation was requested too late to be performed in requested time. */
#define NRF_802154_SL_TIMER_RET_TOO_LATE     2U

/**@brief The trigger time for the requested operation is too distant.
 *
 * Some platforms have a limitation to how far in the future operation can be
 * scheduled. This result may happen for @ref nrf_802154_sl_timer_add with
 * @ref nrf_802154_sl_timer_t::trigger_time containing a value too far from `now`.
 */
#define NRF_802154_SL_TIMER_RET_TOO_DISTANT  3U

/**@brief There were no available resources required to schedule requested activity.
 *
 * This result may happen for @ref nrf_802154_sl_timer_add with
 * @ref nrf_802154_sl_timer_t::action_type containing
 * @ref NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE. To trigger hardware task by the timer,
 * some hardware resources like PPI/DPPI channels, EGU channels, timer compare channels
 * (implementation dependent) were needed, but they were unavailable.
 */
#define NRF_802154_SL_TIMER_RET_NO_RESOURCES 4U

/**@brief The timer was inactive at the moment of requested operation. */
#define NRF_802154_SL_TIMER_RET_INACTIVE     5U

/**@brief Type representing a timer. */
typedef struct nrf_802154_sl_timer_s nrf_802154_sl_timer_t;

/**@brief Type of callback function pointer to be called when timer triggers. */
typedef void (* nrf_802154_sl_timer_callback_t)(nrf_802154_sl_timer_t * p_timer);

/**@brief Action types to be performed when a timer triggers.
 *
 * This is a bitmask with possible masks:
 * @ref NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK
 * @ref NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE
 */
typedef uint8_t nrf_802154_sl_timer_action_type_t;

/**@brief Timer action type mask allowing a callback to be called. */
#define NRF_802154_SL_TIMER_ACTION_TYPE_CALLBACK (1U << 0)
/**@brief Timer action type mask allowing triggering a hardware task. */
#define NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE (1U << 1)

/**
 * @brief Special value that indicates that (D)PPI configuration is incomplete.
 */
#define NRF_802154_SL_TIMER_INVALID_PPI_CHANNEL  UINT32_MAX

/**@brief Structure that represents private fields of a timer required by the implementation. */
typedef struct
{
    uint64_t placeholder[NRF_802154_SL_TIMER_PRIV_SIZE];
} nrf_802154_sl_timer_priv_placeholder_t;

struct nrf_802154_sl_timer_s
{
    /**@brief Private fields required by the implementation.
     *
     * Must be at the beginning of the structure.
     */
    nrf_802154_sl_timer_priv_placeholder_t priv;

    /**@brief The timestamp in microseconds at which the timer should be triggered.
     *
     * The callback will be called no earlier than at given @p trigger_time, but may be delayed
     * due to processing time taken by other ISRs.
     */
    uint64_t trigger_time;

    /**@brief User-provided data.
     *
     * User preparing a timer can store any data within this field. Timer module does not
     * process this data.
     */
    union
    {
        void       * p_pointer;
        const void * p_cpointer;
    } user_data;

    /**@brief Action type to be performed when the timer triggers.
     *
     * Depending on value bits set in this bitmask, appropriate fields of @c action is valid.
     */
    nrf_802154_sl_timer_action_type_t action_type;

    struct
    {
        /**@brief Parameters applicable when @c action_type contains
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

        /**@brief Parameters applicable when @c action_type contains
         *        @ref NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE */
        struct
        {
            /**@brief Identifier of (D)PPI channel to be triggered at @c trigger_time.
             *
             * The API user is responsible for creating all necessary hardware bindings between
             * provided (D)PPI channel and the requested task, as well as necessary configuration
             * of these resources. Moreover, the API user must keep the hardware bindings enabled
             * and functional until the requested hardware task is triggered or until the timer
             * is successfully removed.
             *
             * A special value of @ref NRF_802154_SL_TIMER_INVALID_PPI_CHANNEL can be set. In this
             * case, the binding creation is postponed and the user is required to use the
             * @ref nrf_802154_sl_timer_update_ppi to complete the configuration. Must do so
             * before @c trigger_time expires.
             */
            uint32_t ppi_channel;
        } hardware;
    } action;
};

/**@brief Initializes the timer module. */
void nrf_802154_sl_timer_module_init(void);

/**@brief Uninitializes the timer module. */
void nrf_802154_sl_timer_module_uninit(void);

/**@brief Gets current time.
 *
 * @return Time in microseconds.
 */
uint64_t nrf_802154_sl_timer_current_time_get(void);

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

/**@brief De-initializes a timer instance.
 *
 * This function plays a role of a destructor.
 *
 * @param p_timer   Pointer to a timer instance to be de-initialized.
 */
void nrf_802154_sl_timer_deinit(nrf_802154_sl_timer_t * p_timer);

/**@brief Adds a timer to the active timers list.
 *
 * This function adds the timer pointed by @p p_timer parameter to an active timers
 * list. Before call to @ref nrf_802154_sl_timer_add the timer must fill
 * public fields of timer object instance in the following way:
 * - @ref nrf_802154_sl_timer_t::trigger_time is set up to the moment when the timer
 *   is expected to trigger
 * - Optionally @ref nrf_802154_sl_timer_t::user_data may be set, to user-dependent value.
 * - @ref nrf_802154_sl_timer_t::action_type is set according to requested action.
 * - Depending on value written to @ref nrf_802154_sl_timer_t::action_type
 *   matching fields of @ref nrf_802154_sl_timer_t::action must be filled.
 *
 * When this function succeeds the timer module takes ownership of the timer instance.
 * A user is forbidden to modify the structure until the callback is called or
 * @ref nrf_802154_sl_timer_remove is successfully called. Modification of timer owned by the
 * timer service results in an undefined behavior. In particular the user
 * is forbidden to set a new value of @ref nrf_802154_sl_timer_t::trigger_time as long as the
 * timer is owned by the timer service.
 *
 * Please note that if @ref nrf_802154_sl_timer_t::trigger_time is in the past or very near in
 * the future, the timer is scheduled and will fire as soon as possible but not earlier than
 * given by @ref nrf_802154_sl_timer_t::trigger_time.
 *
 * @param p_timer   Pointer to a timer instance to add to active timers list.
 *
 * @retval NRF_802154_SL_TIMER_RET_SUCCESS
 *      Function succeeded. The timer is sussessfully added. The requested action is
 *      scheduled to happen.
 * @retval NRF_802154_SL_TIMER_RET_NO_RESOURCES
 *      The requested hardware task can't be bound to the timer due to lack of hardware resources.
 * @retval NRF_802154_SL_TIMER_RET_BAD_REQUEST
 *      A user passed unsupported or invalid parameters in @p p_timer.
 */
nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_add(nrf_802154_sl_timer_t * p_timer);

/**@brief Removes a timer from the active timers list.
 *
 * @param p_timer   Pointer to a timer instance to be removed from the active timers list.
 *
 * @retval NRF_802154_SL_TIMER_RET_SUCCESS
 *      The timer was running and it has been successfully removed from active timers list.
 *      The requested action (either callback or hardware task trigger) will not happen.
 * @retval NRF_802154_SL_TIMER_RET_INACTIVE
 *      The timer was not running at the moment of the call and is not in the active timers list.
 *      Nothing can be implied about the completion of the timer's requested action, without
 *      checking external conditions. It is up to the caller to detect if requested action
 *      (either callback, hardware task or both) has already been executed, is currently
 *      in progress or yet remains to be executed.
 */
nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_remove(nrf_802154_sl_timer_t * p_timer);

/**@brief Updates the (D)PPI channel to be triggered.
 *
 * @param p_timer   Pointer to a timer instance to be updated.
 * @param ppi_chn   Identifier of (D)PPI channel to be triggered at @c trigger_time.
 *
 * @retval NRF_802154_SL_TIMER_RET_SUCCESS
 *      The (D)PPI channel was updated and it was done on time, i.e. before specified
 *      trigger time.
 * @retval NRF_802154_SL_TIMER_RET_TOO_LATE
 *      It has been detected that the timer has already fired and because of this the update
 *      could not complete at all or did not complete on time. Nevertheless, the (D)PPI may
 *      have been triggered, but it cannot be stated reliably.
 * @retval NRF_802154_SL_TIMER_RET_BAD_REQUEST
 *      A user passed invalid parameters in @p p_timer, e.g. @c action_type does not contain
 *      @ref NRF_802154_SL_TIMER_ACTION_TYPE_HARDWARE.
 */
nrf_802154_sl_timer_ret_t nrf_802154_sl_timer_update_ppi(nrf_802154_sl_timer_t * p_timer,
                                                         uint32_t                ppi_chn);

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_SL_TIMER_H_ */
