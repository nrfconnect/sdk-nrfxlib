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

#ifndef NRF_802154_SL_RSCH_H__
#define NRF_802154_SL_RSCH_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrf_802154_sl_rsch Resource Scheduler Service
 * @{
 * @ingroup nrf_802154_sl_rsch
 * @brief The Resource Scheduler Service for the 802.15.4 Service Layer
 */

#define NRF_802154_SL_RSCH_TS_PRIORITY_LOWEST 0U   /**< Lowest possible priority of a timeslot. */
#define NRF_802154_SL_RSCH_SESSION_PRIV_SIZE  1U   /**< Number of words needed for private fields of a session. */
#define NRF_802154_SL_RSCH_DEFAULT_MARGIN_US  100U /**< Default margin in microseconds. Refer to @ref nrf_802154_sl_rsch_ts_param_t. */

/** @brief Scheduler's return codes.
 *
 * Possible values:
 * @ref NRF_802154_SL_RSCH_RET_SUCCESS
 * @ref NRF_802154_SL_RSCH_RET_ALREADY_REQUESTED
 * @ref NRF_802154_SL_RSCH_RET_INVALID_PARAM
 * @ref NRF_802154_SL_RSCH_RET_HIGHER_PRIO
 * @ref NRF_802154_SL_RSCH_RET_RSRC_DENIED
 * @ref NRF_802154_SL_RSCH_RET_TIMESLOT_INACTIVE
 * @ref NRF_802154_SL_RSCH_RET_DURATION_TIMEOUT
 * @ref NRF_802154_SL_RSCH_RET_NO_MEMORY

 */
typedef uint32_t nrf_802154_sl_rsch_ret_t;

#define NRF_802154_SL_RSCH_RET_SUCCESS           0U /**< Operation finished successfully. */
#define NRF_802154_SL_RSCH_RET_ALREADY_REQUESTED 1U /**< Operation failed because there is an already requested timeslot. */
#define NRF_802154_SL_RSCH_RET_INVALID_PARAM     2U /**< Operation failed because the provided parameters are invalid. */
#define NRF_802154_SL_RSCH_RET_HIGHER_PRIO       3U /**< Operation failed because of a higher priority request that overlaps in time. */
#define NRF_802154_SL_RSCH_RET_RSRC_DENIED       4U /**< Operation failed because of a denied resource. */
#define NRF_802154_SL_RSCH_RET_TIMESLOT_INACTIVE 5U /**< Operation failed because the timeslot's state does not allow it. */
#define NRF_802154_SL_RSCH_RET_DURATION_TIMEOUT  6U /**< Operation finished because its requested duration expired. */
#define NRF_802154_SL_RSCH_RET_NO_MEMORY         7U /**< Operation failed because there was no memory to complete it. */

/** @brief Flags indicating resources that can be accessed in a timeslot.
 *
 * This type is a mask of bits that represent access to different resources that may be necessary
 * for successful operation inside a timeslot.
 *
 * Possible masks:
 * @ref NRF_802154_SL_RSCH_RSRC_NONE
 * @ref NRF_802154_SL_RSCH_RSRC_RADIO
 * @ref NRF_802154_SL_RSCH_RSRC_RF
 */
typedef uint32_t nrf_802154_sl_rsch_resource_t;

#define NRF_802154_SL_RSCH_RSRC_NONE  0U       /**< Indicates that no resources are required. */
#define NRF_802154_SL_RSCH_RSRC_RADIO (1 << 0) /**< Indicates that exclusive access to RADIO peripheral is required. */
#define NRF_802154_SL_RSCH_RSRC_RF    (1 << 1) /**< Indicates that exclusive access to radio wave medium is required. */

/** @brief Types 802.15.4 operations.
 *
 * Possible values:
 * @ref NRF_802154_SL_RSCH_OP_NONE
 * @ref NRF_802154_SL_RSCH_OP_LISTEN
 * @ref NRF_802154_SL_RSCH_OP_RX
 * @ref NRF_802154_SL_RSCH_OP_TX
 */
typedef uint32_t nrf_802154_sl_rsch_operation_t;

#define NRF_802154_SL_RSCH_OP_NONE   0U /**< Non-802.15.4 related operation. */
#define NRF_802154_SL_RSCH_OP_LISTEN 1U /**< 802.15.4 idle listening. */
#define NRF_802154_SL_RSCH_OP_RX     2U /**< 802.15.4 active reception. */
#define NRF_802154_SL_RSCH_OP_TX     3U /**< 802.15.4 transmission. */

/** @brief Structure that represents private fields of a session required by the implementation. */
typedef struct
{
    uint32_t placeholder[NRF_802154_SL_RSCH_SESSION_PRIV_SIZE];
} nrf_802154_sl_rsch_session_priv_placeholder_t;

/** @brief Structure that represents the parameters of a session. */
typedef struct
{
    nrf_802154_sl_rsch_session_priv_placeholder_t priv;
} nrf_802154_sl_rsch_session_t;

/** @brief Structure that holds metadata passed in @ref nrf_802154_sl_rsch_ts_start_callback_t.
 */
typedef struct
{
    /** @brief Identifier of the started timeslot. */
    uint32_t                       ts_id;

    /** @brief Time in microseconds that the started timeslot is expected to last. */
    uint32_t                       duration_us;

    /** @brief Pointer to session that the started timeslot belongs to. */
    nrf_802154_sl_rsch_session_t * p_session;

} nrf_802154_sl_rsch_ts_start_callback_metadata_t;

/** @brief Structure that holds metadata passed in @ref nrf_802154_sl_rsch_ts_end_callback_t.
 */
typedef struct
{
    /** @brief Identifier of the ending timeslot. */
    uint32_t                 ts_id;

    /** @brief Return code that informs about the reason of timeslot end. */
    nrf_802154_sl_rsch_ret_t ret;

    /** @brief Pointer to resources that are to be handed over and do not require deconfiguration.
     *
     * When a timeslot is about to end, it's possible that there are other timeslots scheduled to
     * occur immediately after it such that their required resources overlap with the resources
     * requested by the ending timeslot. In order to avoid unnecessary deconfiguration
     * and reconfiguration of resources that are continuously granted and required by consecutive
     * timeslots, the scheduler fills this field with resources that need not be deconfigured
     * upon the timeslot end so that they can be handed over to the following session. All remaining
     * resources requested by the session must be deconfigured completely before
     * @ref nrf_802154_sl_rsch_ts_release is called.
     *
     * @note If this field is set to NULL, all resources requested upon request must be deconfigured
     * completely before @ref nrf_802154_sl_rsch_ts_release is called.
     */
    nrf_802154_sl_rsch_resource_t * p_rsrc;

    /** @brief Pointer to session the ending timeslot belongs to. */
    nrf_802154_sl_rsch_session_t  * p_session;

} nrf_802154_sl_rsch_ts_end_callback_metadata_t;

/** @brief Structure that holds metadata passed in @ref nrf_802154_sl_rsch_ts_cancelled_callback_t. */
typedef struct
{
    /** @brief Identifier of the cancelled timeslot. */
    uint32_t                       ts_id;

    /** @brief Return code that informs about the reason of timeslot cancellation. */
    nrf_802154_sl_rsch_ret_t       ret;

    /** @brief Pointer to session the cancelled timeslot belongs to. */
    nrf_802154_sl_rsch_session_t * p_session;

} nrf_802154_sl_rsch_ts_cancelled_callback_metadata_t;

/** @brief Structure that holds metadata passed in @ref nrf_802154_sl_rsch_ts_cleanup_callback_t. */
typedef struct
{
    /** @brief Identifier of the timeslot that should be cleaned up. */
    uint32_t                        ts_id;

    /** @brief Pointer to resources that ought to be deconfigured. */
    nrf_802154_sl_rsch_resource_t * p_rsrc;

    /** @brief Pointer to session the timeslot belongs to. */
    nrf_802154_sl_rsch_session_t  * p_session;

} nrf_802154_sl_rsch_ts_cleanup_callback_metadata_t;

/** @brief Function called by the scheduler to indicate that a requested timeslot has started.
 *
 * After this function is called, the session has exclusive access to the resources specified
 * upon request.
 *
 * @param[in]  p_context   Pointer to data provided by the user.
 * @param[in]  p_metadata  Pointer to data containing information regarding the started timeslot.
 */
typedef void (* nrf_802154_sl_rsch_ts_start_callback_t)(
    void * p_context, nrf_802154_sl_rsch_ts_start_callback_metadata_t * p_metadata);

/** @brief Function called by the scheduler to indicate that a requested and successfully started
 *         timeslot is about to end.
 *
 * The scheduler issues this call to occur when only a period of time specified upon timeslot
 * request is left until the expected end of the timeslot. The call can be a result of a number
 * of events, e.g. a preemption performed by a higher priority request, required resource denied
 * by an external arbiter or timeout of the specified duration of the timeslot. After this function
 * is called, the session is responsible for releasing exclusive access to the requested resources
 * by calling @ref nrf_802154_sl_rsch_ts_release. Failing to do so in the specified time margin
 * might result in blocking other 802.15.4 operations as well as other radio protocols.
 *
 * @param[in]  p_context   Pointer to data provided by the user.
 * @param[in]  p_metadata  Pointer to data containing information regarding the ending timeslot.
 */
typedef void (* nrf_802154_sl_rsch_ts_end_callback_t)(
    void * p_context, nrf_802154_sl_rsch_ts_end_callback_metadata_t * p_metadata);

/** @brief Function called by the scheduler to indicate that a requested and not yet started
 *         timeslot was cancelled by the scheduler.
 *
 * This function is called when a successful request from the past is cancelled by the scheduler
 * before the requested timeslot is started. The call can be a result of a higher priority timeslot
 * requested for the same period of time or a required resource certain to be unavailable
 * at the specified period of time.
 *
 * @param[in]  p_context   Pointer to data provided by the user.
 * @param[in]  p_metadata  Pointer to data containing information regarding the cancelled timeslot.
 */
typedef void (* nrf_802154_sl_rsch_ts_cancelled_callback_t)(
    void * p_context, nrf_802154_sl_rsch_ts_cancelled_callback_metadata_t * p_metadata);

/** @brief Function called by the scheduler to indicate that a requested and not yet started
 *         timeslot requires deconfiguration of resources that were handed over to it.
 *
 * It might happen that a timeslot had had its resources handed over, but they became unavailable
 * before the timeslot could start. Although the timeslot never starts, it must deconfigure
 * the resources that were handed over to it and that were denied by the scheduler. Therefore
 * the session gains exclusive access to the resources it requested, but only to deconfigure
 * those of them that are not granted anymore. After this function is called, the session
 * is responsible for releasing exclusive access to the requested resources by calling
 * @ref nrf_802154_sl_rsch_ts_release. Failing to do so in the specified time margin might result
 * in blocking other 802.15.4 operations as well as other radio protocols.
 */
typedef void (* nrf_802154_sl_rsch_ts_cleanup_callback_t)(
    void * p_context, nrf_802154_sl_rsch_ts_cleanup_callback_metadata_t * p_metadata);

/** @brief Structure that represents the parameters of a timeslot. */
typedef struct
{
    /** @brief Resources necessary for the operations inside the timeslot to be successful.
     */
    nrf_802154_sl_rsch_resource_t rsrc;

    /** @brief Type of the operation to be performed inside the timeslot.
     */
    nrf_802154_sl_rsch_operation_t op;

    /** @brief Function called by the scheduler to indicate that timeslot has started.
     */
    nrf_802154_sl_rsch_ts_start_callback_t start;

    /** @brief Function called by the scheduler to indicate that timeslot is about to end.
     */
    nrf_802154_sl_rsch_ts_end_callback_t end;

    /** @brief Function called by the scheduler to indicate that timeslot was cancelled.
     */
    nrf_802154_sl_rsch_ts_cancelled_callback_t cancelled;

    /** @brief Pointer to data provided by the caller that is unmodified by the scheduler
     *         and passed back to the caller in callbacks.
     */
    void * p_context;

    /** @brief Flag that indicates if timeslot should be started immediately.
     */
    bool immediate;

    /** @brief Absolute time in microseconds that the timeslot should start in.
     *
     * @note This field is used only if @p immediate is false.
     */
    uint32_t target_time;

    /** @brief Requested time in microseconds that the timeslot is expected to last.
     *
     * If the timeslot's priority is set to the lowest priority possible, setting this field
     * to 0 makes the timeslot last for an indefinite period of time. Such timeslot finished
     * with a @ref nrf_802154_sl_rsch_ts_release call or when its end callback is called.
     *
     * @note If this field is set to 0 when the timeslot's priority is not the lowest possible,
     * @ref nrf_802154_sl_rsch_ts_request returns @ref NRF_802154_SL_RSCH_RET_INVALID_PARAM
     * and the timeslot is not scheduled.
     */
    uint32_t duration_us;

    /** @brief Minimum time in microseconds that the timeslot can last to serve its purpose.
     *
     * If the caller wishes to allocate timeslot shorter than defined by @p duration_us
     * that can be started earlier, this field should be set to a value indicating minimum
     * duration of the timeslot that can be utilized by the caller. If the caller cannot
     * utilize shorter timeslots than requested, this field shall be set to 0.
     */
    uint32_t min_duration_us;

    /** @brief Minimum time in microseconds that a session needs to clean up after their timeslot.
     *
     * When a timeslot is about to be finished, @p end callback is issued to the session that
     * requested it. This parameter specifies the minimum time that the session might need to
     * perform all actions that need to be done before releasing the timeslot with
     * @ref nrf_802154_sl_rsch_ts_release.
     */
    uint32_t margin_us;

    /** @brief Priority of the timeslot.
     *
     * The lowest possible priority of a timeslot is equal @ref NRF_802154_SL_RSCH_TS_PRIORITY_LOWEST.
     * Any number larger than that value indicates a higher priority.
     */
    uint8_t priority;

} nrf_802154_sl_rsch_ts_param_t;

/** @brief Structure that represents timeslot parameters that can be updated. */
typedef struct
{
    /** @brief Resources necessary for the operations inside the timeslot to be successful.
     */
    nrf_802154_sl_rsch_resource_t rsrc;

    /** @brief Type of the operation to be performed inside the timeslot.
     */
    nrf_802154_sl_rsch_operation_t op;

    /** @brief Requested time in microseconds that the timeslot is expected to last.
     */
    uint32_t duration_us;

    /** @brief Priority of the timeslot.
     *
     * The lowest possible priority of a timeslot is equal @ref NRF_802154_SL_RSCH_TS_PRIORITY_LOWEST.
     * Any number larger than that value indicates a higher priority.
     */
    uint8_t priority;

} nrf_802154_sl_rsch_ts_update_param_t;

/** @brief Initializes the scheduler.
 *
 * @note This function must be called once, before any other function from this module.
 *
 * @note No timeslots are requested after initialization. In order to start radio activity,
 *       @ref nrf_802154_sl_rsch_ts_request should be called.
 */
void nrf_802154_sl_rsch_init(void);

/** @brief Deinitializes the scheduler. */
void nrf_802154_sl_rsch_deinit(void);

/** @brief Initializes a session.
 *
 * @param[out]  p_session   Pointer to a session structure.
 *
 * @retval  NRF_802154_SL_RSCH_RET_SUCCESS           Session was initialized successfully.
 * @retval  NRF_802154_SL_RSCH_RET_ALREADY_REQUESTED Initialization failed. Session was initialized already.
 * @retval  NRF_802154_SL_RSCH_NO_MEMORY             Initialization failed. The amount of available memory
 *                                                   is insufficient.
 */
nrf_802154_sl_rsch_ret_t nrf_802154_sl_rsch_session_init(nrf_802154_sl_rsch_session_t * p_session);

/** @brief Requests a timeslot.
 *
 * @note There can only be a single scheduled timeslot in a given session.
 *
 * When a timeslot is requested successfully and this function returns
 * @ref NRF_802154_SL_RSCH_RET_SUCCESS, the scheduler assigns it a unique (within the provided
 * session) identifier passed to the caller through @p p_ts_id. This identifier must be used
 * in order to update parameters of a timeslot with @ref nrf_802154_sl_rsch_ts_param_update
 * or release it by calling @ref nrf_802154_sl_rsch_ts_release. The identifier is also passed
 * as a parameter of all callbacks registered by the caller upon request.
 *
 * @param[inout] p_session   Pointer to the session that the timeslot belongs to.
 * @param[in]    p_param     Pointer to structure that holds parameters defining the timeslot.
 * @param[out]   p_ts_id     Pointer to the timeslot's identifier within a session.
 *
 * @retval  NRF_802154_SL_RSCH_RET_SUCCESS            Timeslot was requested successfully.
 * @retval  NRF_802154_SL_RSCH_RET_ALREADY_REQUESTED  The specified session already has a timeslot
 *                                                    request pending.
 * @retval  NRF_802154_SL_RSCH_RET_INVALID_PARAM      The provided parameters are invalid.
 * @retval  NRF_802154_SL_RSCH_RET_HIGHER_PRIO        There's a higher priority timeslot scheduled
 *                                                    for an overlapping period of time.
 */
nrf_802154_sl_rsch_ret_t nrf_802154_sl_rsch_ts_request(
    nrf_802154_sl_rsch_session_t        * p_session,
    const nrf_802154_sl_rsch_ts_param_t * p_param,
    uint32_t                            * p_ts_id);

/** @brief Releases a timeslot.
 *
 * Calling this function on behalf of a successfully requested but not yet started timeslot
 * cancels that request. After calling this function the timeslot is not scheduled and is not
 * going to start.
 *
 * Calling this function on behalf of an active (successfully requested and started) or ending
 * (i.e. after end callback was issued) timeslot ends that timeslot. Its resources are released
 * and can be used immediately by other sessions.
 *
 * @note The caller of this API is assumed to act cooperatively. Failing to release a timeslot
 * in the specified time margin after receiving its end callback might lead to blocking other
 * 802.15.4 operations as well as other radio protocols.
 *
 * @param[inout] p_session  Pointer to the session that the timeslot belongs to.
 * @param[in]    ts_id      Identifier of the timeslot to be released.
 *
 * @retval  NRF_802154_SL_RSCH_RET_SUCCESS            The timeslot was released successfully.
 * @retval  NRF_802154_SL_RSCH_RET_TIMESLOT_INACTIVE  The timeslot could not be released because
 *                                                    it has already been released or it was not
 *                                                    requested at all.
 */
nrf_802154_sl_rsch_ret_t nrf_802154_sl_rsch_ts_release(nrf_802154_sl_rsch_session_t * p_session,
                                                       uint32_t                       ts_id);

/** @brief Updates parameters of an active timeslot.
 *
 * This function can be called only to modify parameters of a currently active timeslot,
 * i.e. a timeslot that started and was not yet released. Only a limited subset of all timeslot
 * parameters can be modified when it is already active: its required resources, operation to be
 * performed inside of it, its duration and priority.
 *
 * If there is another active timeslot running in parallel, which is using a separate subset
 * of resources, and the resources passed in this function overlap with that separate subset,
 * the scheduler chooses timeslots based on priority. If the updated priority is higher than
 * the already running parallel timeslot, that timeslot is preempted with an end callback and its
 * resources can be granted to the higher priority updated timeslot. If the priorities are equal,
 * the already running timeslot is preferred and the update fails. If the updated priority is
 * lower than the already running timeslot, the update fails.
 *
 * @note The caller of this API is assumed to act cooperatively. Repeatably extending timeslot's
 * duration and increasing its priority might lead to blocking other 802.15.4 operations as well as
 * other radio protocols.
 *
 * @note If the update is successful, the scheduler notifies the caller about the required resources
 * being granted with a start callback.
 *
 * @note If the update fails, the original timeslot is not ended and its parameters still hold.
 *
 * @note If the update is not carried out before the timeslot ends, the session receives
 * an end callback and it shall release the timeslot immediately.
 *
 * @note Irrespectively of whether the update is successful, the timeslot's identifier is not modified.
 *
 * @param[in]  p_session    Pointer to the session that the timeslot belongs to.
 * @param[in]  p_param      Pointer to structure that holds timeslot parameters that can be updated.
 * @param[in]  ts_id        Identifier of the timeslot to be updated.
 *
 * @retval  NRF_802154_SL_RSCH_RET_SUCCESS            Timeslot's parameters were updated successfully.
 * @retval  NRF_802154_SL_RSCH_RET_HIGHER_PRIO        The update failed because the updated priority
 *                                                    was not high enough to perform it.
 * @retval  NRF_802154_SL_RSCH_RET_TIMESLOT_INACTIVE  The update failed because the timeslot is inactive.
 */
nrf_802154_sl_rsch_ret_t nrf_802154_sl_rsch_ts_param_update(
    const nrf_802154_sl_rsch_session_t         * p_session,
    const nrf_802154_sl_rsch_ts_update_param_t * p_update_param);

/** @brief Initializes timeslot request parameters to default values.
 *
 * @param[out] p_param  Pointer to structure that holds timeslot parameters to be set to default.
 */
static inline void nrf_802154_sl_rsch_ts_param_init(nrf_802154_sl_rsch_ts_param_t * p_param)
{
    p_param->rsrc            = NRF_802154_SL_RSCH_RSRC_NONE;
    p_param->op              = NRF_802154_SL_RSCH_OP_NONE;
    p_param->start           = NULL;
    p_param->end             = NULL;
    p_param->cancelled       = NULL;
    p_param->p_context       = NULL;
    p_param->immediate       = true;
    p_param->target_time     = 0;
    p_param->duration_us     = 0;
    p_param->min_duration_us = 0;
    p_param->margin_us       = NRF_802154_SL_RSCH_DEFAULT_MARGIN_US;
    p_param->priority        = NRF_802154_SL_RSCH_TS_PRIORITY_LOWEST + 1;
}

/**
 *@}
 **/

#ifdef __cplusplus
}
#endif

#endif // NRF_802154_SL_RSCH_H__
