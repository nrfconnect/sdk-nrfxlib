/*
 * Copyright (c) 2018, Nordic Semiconductor ASA
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
 * @brief Module that defines the Radio Scheduler interface.
 *
 */

#ifndef NRF_802154_RSCH_H_
#define NRF_802154_RSCH_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_rsch Radio Scheduler
 * @{
 * @ingroup nrf_802154
 * @brief The Radio Scheduler interface.
 *
 * Radio Scheduler is responsible to schedule radio activities and preconditions in time. It is
 * expected that the Radio Scheduler module manages timings to meet the requirements requested from
 * the core module.
 *
 * Examples of the radio activity preconditions are: High-Frequency Clock running, radio arbiter (RAAL)
 * granted access to the RADIO peripheral.
 */

/**
 * @brief Maximum number of DTX delayed timeslots that can be scheduled simultaneously.
 */
#ifndef NRF_802154_RSCH_DLY_TS_OP_DTX_SLOTS
#define NRF_802154_RSCH_DLY_TS_OP_DTX_SLOTS 1
#endif

/**
 * @brief Maximum number of DRX delayed timeslots that can be scheduled simultaneously.
 */
#ifndef NRF_802154_RSCH_DLY_TS_OP_DRX_SLOTS
#define NRF_802154_RSCH_DLY_TS_OP_DRX_SLOTS 2
#endif

/**
 * @brief Maximum number of CSMA/CA delayed timeslots that can be scheduled simultaneously.
 */
#ifndef NRF_802154_RSCH_DLY_TS_OP_CSMACA_SLOTS
#define NRF_802154_RSCH_DLY_TS_OP_CSMACA_SLOTS 1
#endif

/**
 * @brief Number of available slots for all delayed timeslots.
 */
#define NRF_802154_RSCH_DLY_TS_SLOTS      \
    NRF_802154_RSCH_DLY_TS_OP_DTX_SLOTS + \
    NRF_802154_RSCH_DLY_TS_OP_DRX_SLOTS + \
    NRF_802154_RSCH_DLY_TS_OP_CSMACA_SLOTS

/**
 * @brief List of the preconditions that have to be met before any radio activity.
 */
typedef enum
{
    RSCH_PREC_HFCLK,
    RSCH_PREC_RAAL,
    RSCH_PREC_COEX,
    RSCH_PREC_CNT,
} rsch_prec_t;

/**
 * @brief Priorities of the 802.15.4 radio operations.
 */
typedef enum
{
    RSCH_PRIO_IDLE,                                    ///< Priority used in the sleep state. With this priority, RSCH releases all preconditions.
    RSCH_PRIO_IDLE_LISTENING,                          ///< Priority used during the idle listening procedure.
    RSCH_PRIO_RX,                                      ///< Priority used when a frame is being received.
    RSCH_PRIO_DETECT,                                  ///< Priority used to detect channel conditions (CCA, ED).
    RSCH_PRIO_TX,                                      ///< Priority used to transmit a frame.

    RSCH_PRIO_MIN_APPROVED = RSCH_PRIO_IDLE_LISTENING, ///< Minimal priority indicating that the given precondition is approved.
    RSCH_PRIO_MAX          = RSCH_PRIO_TX,             ///< Maximal priority available in the RSCH module.
} rsch_prio_t;

/**
 * @brief Enumeration of the delayed timeslot operation types.
 */
typedef enum
{
    RSCH_DLY_TS_OP_DTX,    ///< Timeslot for delayed TX operation.
    RSCH_DLY_TS_OP_DRX,    ///< Timeslot for delayed RX operation.
    RSCH_DLY_TS_OP_CSMACA, ///< Timeslot for CSMA/CA operation.
} rsch_dly_ts_op_t;

/**
 * @brief Identifier of a delayed timeslot.
 */
typedef uint32_t rsch_dly_ts_id_t;

/**
 * @brief Enumeration of the precondition requesting strategies.
 */
typedef enum
{
    /** The following delayed timeslot type requires preconditions to be requested immediately
     *  and released only by @ref nrf_802154_rsch_delayed_timeslot_cancel call. It is scheduled
     *  irrespectively of current approved preconditions, which might introduce additional delays.
     *  This delayed timeslot type also supports requests with target time in the past.
     */
    RSCH_DLY_TS_TYPE_RELAXED,

    /** The following delayed timeslot type requires preconditions to be requested as late
     *  as possible to ensure they are ramped up on target time, and released as soon as possible.
     *  This mode of operation does not allow for requests in the past and puts strict conditions
     *  on preconditions that must be approved when the scheduled operation starts.
     */
    RSCH_DLY_TS_TYPE_PRECISE,
} rsch_dly_ts_type_t;

/**
 * @brief Function pointer used for notifying about delayed timeslot start.
 *
 * @param[in]  dly_ts_id  Identifier of the started delayed timeslot.
 */
typedef void (* rsch_dly_ts_started_callback_t)(rsch_dly_ts_id_t dly_ts_id);

/**
 * @brief Structure that holds parameters of a delayed timeslot request.
 */
typedef struct
{
    uint64_t                       trigger_time;     ///< Trigger time of the timeslot start, in microseconds.
    bool                           ppi_trigger_en;   ///< Enable the (D)PPI triggering after the start of the timeslot.
    uint32_t                       ppi_trigger_dly;  ///< Time delta between @p trigger_time and the moment of (D)PPI triggering.
    rsch_prio_t                    prio;             ///< Priority level required for the delayed timeslot.
    rsch_dly_ts_op_t               op;               ///< Operation to be performed in the requested timeslot.
    rsch_dly_ts_type_t             type;             ///< Type of the requested timeslot.
    rsch_dly_ts_started_callback_t started_callback; ///< Callback called when delayed timeslot starts.
    rsch_dly_ts_id_t               id;               ///< Identifier of the timeslot provided by its owner. Its value must be unique, to unambigously map to an active slot.
} rsch_dly_ts_param_t;

/**
 * @brief Initializes Radio Scheduler.
 *
 * @note This function must be called once, before any other function from this module.
 *
 * @note Radio Scheduler starts in the inactive mode after the initialization. To start the radio activity,
 *       @ref nrf_802154_rsch_continuous_mode_enter should be called.
 *
 */
void nrf_802154_rsch_init(void);

/**
 * @brief Deinitializes Radio Scheduler.
 *
 */
void nrf_802154_rsch_uninit(void);

/**
 * @brief Sets the priority for the continuous radio mode.
 *
 * In the continuous mode, Radio Scheduler tries to satisfy all preconditions for as long as
 * possible to give the radio driver core as much radio time as possible while
 * disturbing the other activities to the minimum extent.
 *
 * @note The start of a timeslot is indicated by the @ref nrf_802154_rsch_prec_is_approved call.
 * @note To disable the continuous radio mode, the @ref RSCH_PRIO_IDLE should be used.
 *
 * @param[in]  prio  Priority level used in the continuous radio mode.
 *
 */
void nrf_802154_rsch_continuous_mode_priority_set(rsch_prio_t prio);

/**
 * @brief Confirms that the current part of the continuous timeslot is ended by the core.
 *
 * This confirmation is used by the core to synchronize the ending of the continuous timeslot parts with
 * the RSCH module.
 *
 */
void nrf_802154_rsch_continuous_ended(void);

/**
 * @brief Immediately requests a timeslot for radio communication.
 *
 * This function is to be called only after @ref nrf_802154_rsch_prec_is_approved indicated the
 * start of a timeslot.
 *
 * @param[in] length_us  Requested radio timeslot length in microseconds.
 *
 * @retval true   Radio driver has now exclusive access to the RADIO peripheral for the
 *                full length of the timeslot.
 * @retval false  Slot cannot be assigned due to other activities.
 *
 */
bool nrf_802154_rsch_timeslot_request(uint32_t length_us);

/**
 * @brief Requests a timeslot in the future.
 *
 * Request timeslot that is to be granted in the future. The function parameters provide data when
 * the timeslot is supposed to start and how long it is to last. When the requested timeslot starts,
 * @p p_dly_ts_param->started_callback is called.
 *
 * For a given type of operation to be performed in the delayed timeslot, @p p_dly_ts_param->op,
 * there's a limited number of delayed timeslots that can be scheduled at the same time:
 *   - for delayed transmissions it's @ref NRF_802154_RSCH_DLY_TS_OP_DTX_SLOTS;
 *   - for delayed receptions it's @ref NRF_802154_RSCH_DLY_TS_OP_DRX_SLOTS;
 *   - for CSMA/CA it's @ref NRF_802154_RSCH_DLY_TS_OP_CSMACA_SLOTS.
 *
 * If the API user requests more delayed timeslots with given @p p_dly_ts_param->op than specified
 * by these constants, the request will fail unconditionally. It also implies that there can only
 * be as many delayed timeslots scheduled simultaneously in total as the sum of these constants.
 *
 * @note Every call to this function must be paired with a call to
 *       @ref nrf_802154_rsch_delayed_timeslot_cancel so that the assigned slot becomes
 *       available for future requests.
 *
 * @note @p p_dly_ts_param->started_callback can be delayed and it is up to
 *       the called module to check the delay and decide if it causes any issues.
 *
 * @note The time parameters use the same units that are used in the SL Timer module.
 *
 * @param[in]   p_dly_ts_param  Parameters of the requested delayed timeslot.
 *
 * @retval true   Requested timeslot has been scheduled.
 * @retval false  Requested timeslot cannot be scheduled and will not be granted.
 */
bool nrf_802154_rsch_delayed_timeslot_request(const rsch_dly_ts_param_t * p_dly_ts_param);

/**
 * @brief Cancels a requested future timeslot.
 *
 * The delayed timeslot must be explicitly cancelled from the @p p_dly_ts_param->started_callback
 * handler function passed to @ref nrf_802154_rsch_delayed_timeslot_request. When timeslot is
 * released from the handler context, the @p handler flag must be set to @p true.
 * Otherwise, the timeslot can be cancelled before it starts, which may be earlier than entering the
 * @p p_dly_ts_param->started_callback handler function.
 *
 * Care must be taken when canceling a timeslot from an interrupt context. Delayed timeslots with
 * type @p RSCH_DLY_TS_TYPE_PRECISE cannot be safely canceled from an interrupt context with higher
 * priority than delayed timeslot callback context.
 *
 * @param[in] dly_ts_id  Identifier of the requested timeslot. If the provided value does not refer
 *                       to any scheduled delayed timeslot, the function returns false.
 * @param[in] handler    Indicates whether the requested timeslot is released from handler function.
 *
 * @retval true     Scheduled timeslot has been cancelled.
 * @retval false    No scheduled timeslot has been requested (nothing to cancel).
 */
bool nrf_802154_rsch_delayed_timeslot_cancel(rsch_dly_ts_id_t dly_ts_id, bool handler);

/**
 * @brief Updates the id of (D)PPI channel to be triggered in the current timeslot.
 *
 * The update must be done after the start of the timeslot but earlier than @c ppi_trigger_dly.
 * If updating does not complete on time, (D)PPI triggering may not be performed.
 *
 * @retval true     (D)PPI channel has been updated and it was done on time, i.e. before
 *                  target trigger time.
 * @retval false    Update could not complete or did not complete on time. Nevertheless,
 *                  the (D)PPI may have been triggered, but it cannot be stated reliably.
 */
bool nrf_802154_rsch_delayed_timeslot_ppi_update(uint32_t ppi_channel);

/**
 * @brief Updates priority of a requested delayed timeslot.
 *
 * @param[in] dly_ts_id    Identifier of the requested timeslot. If the provided value does not
 *                         refer to any scheduled or active delayed timeslot, the function
 *                         returns false.
 * @param[in] dly_ts_prio  Priority to be assigned to the requested timeslot.
 *
 * @retval true     Scheduled timeslot's priority has been updated.
 * @retval false    Priority of the specified timeslot could not be updated.
 */
bool nrf_802154_rsch_delayed_timeslot_priority_update(rsch_dly_ts_id_t dly_ts_id,
                                                      rsch_prio_t      dly_ts_prio);

/**
 * @brief Retrieves the remaining time until a requested timeslot begins.
 *
 * @param[in]  dly_ts_id        ID of the requested timeslot.
 * @param[out] p_time_to_start  Pointer to a 32-bit variable where time to start will be stored.
 *
 * @retval true     The given timeslot was scheduled and p_time_to_start contains a valid value.
 * @retval false    The given timeslot was not scheduled and p_time_to_start was not modified.
 */
bool nrf_802154_rsch_delayed_timeslot_time_to_start_get(rsch_dly_ts_id_t dly_ts_id,
                                                        uint64_t       * p_time_to_start);

/**
 * @brief Checks if there is a pending timeslot request.
 *
 * @note The delayed timeslot is considered requested once its preconditions are requested
 *       or granted.
 *
 * @retval true   There is a timeslot request pending.
 * @retval false  There are no pending timeslot requests.
 */
bool nrf_802154_rsch_timeslot_is_requested(void);

/**
 * @brief Checks if the RSCH precondition is satisfied.
 *
 * @param[in]  prec    RSCH precondition to be checked.
 * @param[in]  prio    Minimal required priority level of the given precondition.
 *
 * @retval true   Precondition @p prec is currently granted.
 * @retval false  Precondition @p prec is not currently granted.
 */
bool nrf_802154_rsch_prec_is_approved(rsch_prec_t prec, rsch_prio_t prio);

/**
 * @brief Gets the remaining time of the currently granted timeslot, in microseconds.
 *
 * @returns  Number of microseconds remaining in the currently granted timeslot.
 */
uint32_t nrf_802154_rsch_timeslot_us_left_get(void);

/**
 * @brief Notifies the core about changes of the approved priority level.
 *
 * If @p prio is greater than @ref RSCH_PRIO_IDLE, the radio driver has exclusive access to the
 * peripherals until this function is called with @p prio equal to @ref RSCH_PRIO_IDLE.
 *
 * @note The end of the timeslot is indicated by @p prio equal to @ref RSCH_PRIO_IDLE.
 *
 * @param[in]  prio  Currently approved priority level.
 */
extern void nrf_802154_rsch_continuous_prio_changed(rsch_prio_t prio);

#if defined(CONFIG_SOC_SERIES_BSIM_NRFXX)
/**
 * @brief Retrieves time remaining to hardware trigger of a delayed timeslot.
 *
 * @note This function exists for testing purposes only. It should not be used in products.
 */
uint32_t nrf_802154_rsch_delayed_timeslot_time_to_hw_trigger_get(void);
#endif /* defined(CONFIG_SOC_SERIES_BSIM_NRFXX) */

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif /* NRF_802154_RSCH_H_ */
