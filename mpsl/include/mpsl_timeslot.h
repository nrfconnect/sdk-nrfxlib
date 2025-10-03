/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_timeslot.h
 *
 * @defgroup mpsl_timeslot MPSL timeslot interface
 * @ingroup  mpsl
 *
 * The Timeslot interface allows the application to run another radio protocol concurrently with
 * Bluetooth LE activity. When a timeslot is granted, the application has exclusive access
 * to the normally blocked RADIO, TIMER0, CCM, and AAR peripherals.
 * The application can use the peripherals freely for the duration of the timeslot.
 * @{
 */

#ifndef MPSL_TIMESLOT_H__
#define MPSL_TIMESLOT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "nrf.h"
#include "nrf_peripherals.h"
#include "nrf_errno.h"

/** @brief The shortest allowed timeslot event in microseconds. */
#define MPSL_TIMESLOT_LENGTH_MIN_US                    (100UL)

/** @brief The longest allowed timeslot event in microseconds. */
#define MPSL_TIMESLOT_LENGTH_MAX_US                    (100000UL)

/** @brief The longest timeslot distance in microseconds allowed for the distance parameter
           see @ref mpsl_timeslot_request_normal_t. */
#define MPSL_TIMESLOT_DISTANCE_MAX_US                  (256000000UL - 1UL)

/**@brief The longest timeout in microseconds allowed when requesting the earliest possible timeslot. */
#define MPSL_TIMESLOT_EARLIEST_TIMEOUT_MAX_US          (256000000UL - 1UL)

/** @brief The maximum jitter in @ref MPSL_TIMESLOT_SIGNAL_START relative to the requested start time. */
#if defined(GRTC_PRESENT)
#define MPSL_TIMESLOT_START_JITTER_US                  (0UL)
#else
#define MPSL_TIMESLOT_START_JITTER_US                  (1UL)
#endif

/** @brief The minimum allowed timeslot extension time. */
#define MPSL_TIMESLOT_EXTENSION_TIME_MIN_US            (200UL)

/** @brief The maximum processing time to handle a timeslot extension. */
#define MPSL_TIMESLOT_EXTENSION_PROCESSING_TIME_MAX_US (25UL)

/** @brief The latest time before the end of a timeslot when timeslot can be extended. */
#define MPSL_TIMESLOT_EXTENSION_MARGIN_MIN_US          (87UL)

/** @brief Size of a single timeslot context. */
#define MPSL_TIMESLOT_CONTEXT_SIZE                     (48)

/** @brief Maximum number of timeslot sessions. */
#define MPSL_TIMESLOT_CONTEXT_COUNT_MAX                (8)

/** @brief The timeslot session id type */
typedef uint8_t mpsl_timeslot_session_id_t;

/** @brief The timeslot signal types. */
enum MPSL_TIMESLOT_SIGNAL
{
    MPSL_TIMESLOT_SIGNAL_START            = 0,  /**< This signal indicates the start of the timeslot.
                                                The signal will be executed in the same context as
                                                @ref MPSL_IRQ_TIMER0_Handler. */
    MPSL_TIMESLOT_SIGNAL_TIMER0           = 1,  /**< This signal indicates the TIMER0 interrupt.
                                                The signal will be executed in the same context as
                                                @ref MPSL_IRQ_TIMER0_Handler. */
    MPSL_TIMESLOT_SIGNAL_RADIO            = 2,  /**< This signal indicates the RADIO interrupt.
                                                The signal will be executed in the same context as
                                                @ref MPSL_IRQ_RADIO_Handler. */
    MPSL_TIMESLOT_SIGNAL_EXTEND_FAILED    = 3,  /**< This signal indicates extend action failed.
                                                The signal will be executed in the same context as
                                                the previous signal. */
    MPSL_TIMESLOT_SIGNAL_EXTEND_SUCCEEDED = 4,  /**< This signal indicates extend action succeeded.
                                                The signal will be executed in the same context as
                                                the previous signal. */

    MPSL_TIMESLOT_SIGNAL_BLOCKED          = 5,  /**< The previous request was blocked. The signal will
                                                be executed in the same context as
                                                @ref mpsl_low_priority_process. */
    MPSL_TIMESLOT_SIGNAL_CANCELLED        = 6,  /**< The previous request was cancelled. The signal will
                                                be executed in the same context as
                                                @ref mpsl_low_priority_process.  */
    MPSL_TIMESLOT_SIGNAL_SESSION_IDLE     = 7,  /**< The timeslot session has no more pending requests.
                                                The signal will be executed in the same context as
                                                @ref mpsl_low_priority_process. */
    MPSL_TIMESLOT_SIGNAL_INVALID_RETURN   = 8,  /**< The previous timeslot callback return value was invalid.
                                                The signal will be executed in the same context as
                                                the previous signal which had an invalid return value.
                                                The application should avoid to continuously provide
                                                invalid return values. Doing so, will lead to an
                                                infinite loop. */
    MPSL_TIMESLOT_SIGNAL_SESSION_CLOSED   = 9,  /**< The session has been closed. */
    MPSL_TIMESLOT_SIGNAL_OVERSTAYED       = 10, /**< The timeslot event was closed too late.
                                                An assert will be triggered after the processing
                                                of this signal completes. */
};

/** @brief The actions requested by the signal callback.
 *
 * This code gives instructions about what action to take when the signal callback has
 * returned.
 *
 * @note For signals executed in low priority returning any action other than
 *       MPSL_TIMESLOT_SIGNAL_ACTION_NONE will result in an assert.
 */
enum MPSL_TIMESLOT_SIGNAL_ACTION
{
    MPSL_TIMESLOT_SIGNAL_ACTION_NONE            = 0,  /**< Return without action. */
    MPSL_TIMESLOT_SIGNAL_ACTION_EXTEND          = 1,  /**< Request an extension of the current
                                                      timeslot event.
                                                      Maximum execution time for this action:
                                                      @ref MPSL_TIMESLOT_EXTENSION_PROCESSING_TIME_MAX_US.
                                                      This action must be started at least
                                                      @ref MPSL_TIMESLOT_EXTENSION_MARGIN_MIN_US before
                                                      the end of a timeslot event.
                                                      @note This signal action may only be used from
                                                      within a timeslot event. */
    MPSL_TIMESLOT_SIGNAL_ACTION_END             = 2,  /**< End the current timeslot event.
                                                      @note This signal action may only be called
                                                      from within a timeslot event. */
    MPSL_TIMESLOT_SIGNAL_ACTION_REQUEST         = 3,  /**< Request a new timeslot event.
                                                      @note If this signal action is used from within
                                                      a timeslot, the current timeslot event is closed. */
};

/** @brief Timeslot high frequency clock source configuration. */
enum MPSL_TIMESLOT_HFCLK_CFG
{
    MPSL_TIMESLOT_HFCLK_CFG_XTAL_GUARANTEED = 0, /**< The high frequency clock source is the external crystal
                                                 for the whole duration of the timeslot. This should be the
                                                 preferred option for events that use the radio or
                                                 require high timing accuracy.
                                                 @note The external crystal will automatically be
                                                 turned on and off at the beginning and end of the
                                                 timeslot. */
    MPSL_TIMESLOT_HFCLK_CFG_NO_GUARANTEE    = 1, /**< This configuration allows for earlier and tighter
                                                 scheduling of timeslots. The RC oscillator may be
                                                 the clock source in part or for the whole duration
                                                 of the timeslot. The RC oscillator's accuracy must
                                                 therefore be taken into consideration.
                                                 @note If the application will use the radio peripheral
                                                 in timeslots with this configuration, it must ensure
                                                 that the crystal is running and stable before
                                                 starting the radio. */
};

/** @brief Timeslot event priorities. */
enum MPSL_TIMESLOT_PRIORITY
{
    MPSL_TIMESLOT_PRIORITY_HIGH   = 0,           /**< High priority. */
    MPSL_TIMESLOT_PRIORITY_NORMAL = 1,           /**< Low priority. */
};

/** @brief Timeslot request type. */
enum MPSL_TIMESLOT_REQUEST_TYPE
{
    MPSL_TIMESLOT_REQ_TYPE_EARLIEST    = 0,      /**< Request timeslot as early as possible.
                                                 This should always be used for the first request
                                                 in a session.
                                                 @note It is not permitted to request an earliest
                                                 timeslot from within a timeslot. */
    MPSL_TIMESLOT_REQ_TYPE_NORMAL      = 1,      /**< Normal timeslot request. */
};

/** @brief Parameters for a request for a timeslot as early as possible. */
typedef struct
{
    uint8_t       hfclk;        /**< High frequency clock source, see @ref MPSL_TIMESLOT_HFCLK_CFG. */
    uint8_t       priority;     /**< The timeslot priority, see @ref MPSL_TIMESLOT_PRIORITY. */
    uint32_t      length_us;    /**< The timeslot length, @sa @ref MPSL_TIMESLOT_LENGTH_MIN_US,
                                     @sa @ref MPSL_TIMESLOT_LENGTH_MAX_US. */
    uint32_t      timeout_us;   /**< Longest acceptable delay until the start of the requested
                                     timeslot, up to @ref MPSL_TIMESLOT_EARLIEST_TIMEOUT_MAX_US
                                     microseconds. */
} mpsl_timeslot_request_earliest_t;

/** @brief Parameters for a normal timeslot request. */
typedef struct
{
    uint8_t       hfclk;        /**< High frequency clock source, see @ref MPSL_TIMESLOT_HFCLK_CFG. */
    uint8_t       priority;     /**< The timeslot priority, see @ref MPSL_TIMESLOT_PRIORITY. */
    uint32_t      distance_us;  /**< Distance from the start of the previous timeslot
                                     up to @ref MPSL_TIMESLOT_DISTANCE_MAX_US microseconds. */
    uint32_t      length_us;    /**< The timeslot length, @sa @ref MPSL_TIMESLOT_LENGTH_MIN_US,
                                     @sa @ref MPSL_TIMESLOT_LENGTH_MAX_US. */
} mpsl_timeslot_request_normal_t;

/**@brief Timeslot request parameters. */
typedef struct
{
    uint8_t                       request_type;  /**< Type of request, see @ref MPSL_TIMESLOT_REQUEST_TYPE. */
    union
    {
        mpsl_timeslot_request_earliest_t earliest;      /**< Parameters for requesting a timeslot as
                                                        early as possible. */
        mpsl_timeslot_request_normal_t   normal;        /**< Parameters for requesting a normal timeslot. */
    } params; /**< Union containing parameters for the request specified. */
} mpsl_timeslot_request_t;

/** @brief Return parameters of the timeslot signal callback. */
typedef struct
{
    uint8_t         callback_action;         /**< The action requested by the application when
                                                  returning from the signal callback, see
                                                  @ref MPSL_TIMESLOT_SIGNAL_ACTION. */
    union
    {
        struct
        {
            mpsl_timeslot_request_t *p_next; /**< The request parameters for the next timeslot. */
        } request;                           /**< Additional parameters for return_code
                                                  @ref MPSL_TIMESLOT_SIGNAL_ACTION_REQUEST. */
        struct
        {
            uint32_t             length_us;  /**< Requested extension of the timeslot duration.
                                                  The minimum time is
                                                  @ref MPSL_TIMESLOT_EXTENSION_TIME_MIN_US). */
        } extend;                            /**< Additional parameters for return_code
                                                  @ref MPSL_TIMESLOT_SIGNAL_ACTION_EXTEND. */
    } params;                                /**< Parameter union. */
} mpsl_timeslot_signal_return_param_t;


/** @brief The timeslot signal callback type.
 *
 * @note In case of invalid return parameters, the timeslot will automatically end
 *       immediately after returning from the signal callback and the
 *       @ref MPSL_TIMESLOT_SIGNAL_INVALID_RETURN event will be sent.
 * @note The returned struct pointer must remain valid after the signal callback
 *       function returns. For instance, this means that it must not point to a stack variable.
 *
 * @param[in] session_id Session id as returned by @ref mpsl_timeslot_session_open.
 * @param[in] signal     Type of signal, see @ref MPSL_TIMESLOT_SIGNAL.
 *
 * @return Pointer to structure containing action requested by the application.
 */
typedef mpsl_timeslot_signal_return_param_t * (*mpsl_timeslot_callback_t) (mpsl_timeslot_session_id_t session_id,
                                                                           uint32_t signal);

/** @brief Set or update the MPSL timeslot configuration.
 *
 * @note Resource configuration can only be performed when all timeslots are closed.
 *
 * @param[in] p_mem      Pointer to a memory location for timeslot sessions.
 *                       The size of the memory block needs to be at least
 *                       n_sessions * @ref MPSL_TIMESLOT_CONTEXT_SIZE bytes.
 *                       The memory needs to reside in RAM.
 *                       The pointer needs to be aligned to a 4-byte boundary.
 * @param[in] n_sessions Number of timeslot sessions.
 *                       Maximum number of supported timeslot sessions is @ref MPSL_TIMESLOT_CONTEXT_COUNT_MAX.
 *
 * @retval  0            The configuration was applied successfully.
 * @retval  -NRF_EPERM   Timeslots need to be configured when no timeslots are open.
 * @retval  -NRF_EINVAL  Invalid argument provided.
 * @retval  -NRF_EFAULT  The memory is not aligned to a 4-byte boundary.
 */
int32_t mpsl_timeslot_session_count_set(void* p_mem, uint8_t n_sessions);

/** @brief Opens a session for timeslot requests.
 *
 * @note Only one session can be open at a time.
 * @note mpsl_timeslot_signal_callback(@ref MPSL_TIMESLOT_SIGNAL_START) will be called when the timeslot
 *       starts. From this point the RADIO, TIMER0, AAR, and CCM peripherals can be freely accessed
 *       by the application.
 * @note mpsl_timeslot_signal_callback(@ref MPSL_TIMESLOT_SIGNAL_TIMER0) is called whenever
 *       the TIMER0 interrupt occurs.
 * @note mpsl_timeslot_signal_callback(@ref MPSL_TIMESLOT_SIGNAL_RADIO) is called whenever the RADIO
 *       interrupt occurs.
 * @note If the low frequency clock is not running when this function is called,
 *       the function will wait until the low frequency clock has started.
 *       See @ref mpsl_clock_lfclk_cfg_t::skip_wait_lfclk_started.
 *
 * @param[in]  mpsl_timeslot_signal_callback The signal callback.
 * @param[out] p_session_id                  Pointer to the id of the session that was opened.
 *
 * @retval  0             Request was successful.
 * @retval  -NRF_ENOMEM   All sessions are already open.
 */
int32_t mpsl_timeslot_session_open(mpsl_timeslot_callback_t mpsl_timeslot_signal_callback,
                                   mpsl_timeslot_session_id_t* p_session_id);

/** @brief Closes a session for timeslot requests.
 *
 * @note Any current timeslot will be finished before the session is closed.
 * @note If a timeslot is scheduled when the session is closed, it will be canceled.
 *
 * @param[in] session_id The session identifier as returned by @ref mpsl_timeslot_session_open.
 *
 * @retval 0              Success
 * @retval  -NRF_EAGAIN   Session already closed
 */
int32_t mpsl_timeslot_session_close(mpsl_timeslot_session_id_t session_id);

/** @brief Requests a timeslot.
 *
 * @note The first request in a session must always be of type @ref MPSL_TIMESLOT_REQ_TYPE_EARLIEST.

 * @note Successful requests will result in mpsl_timeslot_signal_callback_t(@ref MPSL_TIMESLOT_SIGNAL_START).
 *       Unsuccessful requests will result in a @ref MPSL_TIMESLOT_SIGNAL_BLOCKED event.
 * @note The jitter in the start time of the timeslots is +/- @ref MPSL_TIMESLOT_START_JITTER_US us.
 * @note The mpsl_timeslot_signal_callback_t(@ref MPSL_TIMESLOT_SIGNAL_START) call has a latency relative to the
 *       specified timeslot start, but this does not affect the actual start time of the timeslot.
 * @note TIMER0 is reset at the start of the timeslot, and is clocked at 1MHz from the high frequency
 *       (16 MHz) clock source
 * @note No stack will neither access the RADIO peripheral nor the TIMER0 peripheral
 *       during the timeslot.
 *
 * @param[in] session_id  The session identifier as returned by @ref mpsl_timeslot_session_open.
 * @param[in] p_request   Pointer to the request parameters.
 *
 * @retval 0              Success
 * @retval  -NRF_EINVAL   The parameters of p_request are not valid
 * @retval  -NRF_ENOENT   The session is not open.
 * @retval  -NRF_EAGAIN   The session is not IDLE.
 */
int32_t mpsl_timeslot_request(mpsl_timeslot_session_id_t session_id, mpsl_timeslot_request_t const * p_request);

#ifdef __cplusplus
}
#endif

#endif /* MPSL_TIMESLOT_H__ */

/**@} */
