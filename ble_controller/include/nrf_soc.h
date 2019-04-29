/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */


#ifndef NRF_SOC_H__
#define NRF_SOC_H__

#include <stdint.h>
#include "nrf.h"
#include "nrf_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@addtogroup NRF_SOC_DEFINES Defines
 * @{ */

/**@brief The minimum allowed timeslot extension time. */
#define NRF_RADIO_MINIMUM_TIMESLOT_LENGTH_EXTENSION_TIME_US (200)

/**@brief The maximum processing time to handle a timeslot extension. */
#define NRF_RADIO_MAX_EXTENSION_PROCESSING_TIME_US           (17)

/**@brief The latest time before the end of a timeslot the timeslot can be extended. */
#define NRF_RADIO_MIN_EXTENSION_MARGIN_US                    (79)

#define SD_EVT_IRQn                       (SWI2_IRQn)        /**< SoftDevice Event IRQ number. Used for both protocol events and SoC events. */
#define SD_EVT_IRQHandler                 (SWI2_IRQHandler)  /**< SoftDevice Event IRQ handler. Used for both protocol events and SoC events.
                                                                       The default interrupt priority for this handler is set to 6 */
#define NRF_RADIO_LENGTH_MIN_US           (100)               /**< The shortest allowed radio timeslot, in microseconds. */
#define NRF_RADIO_LENGTH_MAX_US           (100000)            /**< The longest allowed radio timeslot, in microseconds. */

#define NRF_RADIO_DISTANCE_MAX_US         (128000000UL - 1UL) /**< The longest timeslot distance, in microseconds, allowed for the distance parameter (see @ref nrf_radio_request_normal_t) in the request. */

#define NRF_RADIO_EARLIEST_TIMEOUT_MAX_US (128000000UL - 1UL) /**< The longest timeout, in microseconds, allowed when requesting the earliest possible timeslot. */

#define NRF_RADIO_START_JITTER_US         (2)                 /**< The maximum jitter in @ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START relative to the requested start time. */

/**@} */

/**@addtogroup NRF_SOC_ENUMS Enumerations
 * @{ */

/**@brief The Radio signal callback types. */
enum NRF_RADIO_CALLBACK_SIGNAL_TYPE
{
  NRF_RADIO_CALLBACK_SIGNAL_TYPE_START,             /**< This signal indicates the start of the radio timeslot. */
  NRF_RADIO_CALLBACK_SIGNAL_TYPE_TIMER0,            /**< This signal indicates the NRF_TIMER0 interrupt. */
  NRF_RADIO_CALLBACK_SIGNAL_TYPE_RADIO,             /**< This signal indicates the NRF_RADIO interrupt. */
  NRF_RADIO_CALLBACK_SIGNAL_TYPE_EXTEND_FAILED,     /**< This signal indicates extend action failed. */
  NRF_RADIO_CALLBACK_SIGNAL_TYPE_EXTEND_SUCCEEDED   /**< This signal indicates extend action succeeded. */
};

/**@brief The actions requested by the signal callback.
 *
 *  This code gives the SOC instructions about what action to take when the signal callback has
 *  returned.
 */
enum NRF_RADIO_SIGNAL_CALLBACK_ACTION
{
  NRF_RADIO_SIGNAL_CALLBACK_ACTION_NONE,            /**< Return without action. */
  NRF_RADIO_SIGNAL_CALLBACK_ACTION_EXTEND,          /**< Request an extension of the current
                                                         timeslot. Maximum execution time for this action:
                                                         @ref NRF_RADIO_MAX_EXTENSION_PROCESSING_TIME_US.
                                                         This action must be started at least
                                                         @ref NRF_RADIO_MIN_EXTENSION_MARGIN_US before
                                                         the end of the timeslot. */
  NRF_RADIO_SIGNAL_CALLBACK_ACTION_END,             /**< End the current radio timeslot. */
  NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END  /**< Request a new radio timeslot and end the current timeslot. */
};

/**@brief Radio timeslot high frequency clock source configuration. */
enum NRF_RADIO_HFCLK_CFG
{
  NRF_RADIO_HFCLK_CFG_XTAL_GUARANTEED, /**< The SoftDevice will guarantee that the high frequency clock source is the
                                           external crystal for the whole duration of the timeslot. This should be the
                                           preferred option for events that use the radio or require high timing accuracy.
                                           @note The SoftDevice will automatically turn on and off the external crystal,
                                           at the beginning and end of the timeslot, respectively. The crystal may also
                                           intentionally be left running after the timeslot, in cases where it is needed
                                           by the SoftDevice shortly after the end of the timeslot. */
  NRF_RADIO_HFCLK_CFG_NO_GUARANTEE    /**< This configuration allows for earlier and tighter scheduling of timeslots.
                                           The RC oscillator may be the clock source in part or for the whole duration of the timeslot.
                                           The RC oscillator's accuracy must therefore be taken into consideration.
                                           @note If the application will use the radio peripheral in timeslots with this configuration,
                                           it must make sure that the crystal is running and stable before starting the radio. */
};

/**@brief Radio timeslot priorities. */
enum NRF_RADIO_PRIORITY
{
  NRF_RADIO_PRIORITY_HIGH,                          /**< High (equal priority as the normal connection priority of the SoftDevice stack(s)). */
  NRF_RADIO_PRIORITY_NORMAL,                        /**< Normal (equal priority as the priority of secondary activities of the SoftDevice stack(s)). */
};

/**@brief Radio timeslot request type. */
enum NRF_RADIO_REQUEST_TYPE
{
  NRF_RADIO_REQ_TYPE_EARLIEST,                      /**< Request radio timeslot as early as possible. This should always be used for the first request in a session. */
  NRF_RADIO_REQ_TYPE_NORMAL                         /**< Normal radio timeslot request. */
};

/**@brief SoC Events. */
enum NRF_SOC_EVTS
{
  NRF_EVT_HFCLKSTARTED,                         /**< Event indicating that the HFCLK has started. */
  NRF_EVT_POWER_FAILURE_WARNING,                /**< Event indicating that a power failure warning has occurred. */
  NRF_EVT_FLASH_OPERATION_SUCCESS,              /**< Event indicating that the ongoing flash operation has completed successfully. */
  NRF_EVT_FLASH_OPERATION_ERROR,                /**< Event indicating that the ongoing flash operation has timed out with an error. */
  NRF_EVT_RADIO_BLOCKED,                        /**< Event indicating that a radio timeslot was blocked. */
  NRF_EVT_RADIO_CANCELED,                       /**< Event indicating that a radio timeslot was canceled by SoftDevice. */
  NRF_EVT_RADIO_SIGNAL_CALLBACK_INVALID_RETURN, /**< Event indicating that a radio timeslot signal callback handler return was invalid. */
  NRF_EVT_RADIO_SESSION_IDLE,                   /**< Event indicating that a radio timeslot session is idle. */
  NRF_EVT_RADIO_SESSION_CLOSED,                 /**< Event indicating that a radio timeslot session is closed. */
  NRF_EVT_POWER_USB_POWER_READY,                /**< Event indicating that a USB 3.3 V supply is ready. */
  NRF_EVT_POWER_USB_DETECTED,                   /**< Event indicating that voltage supply is detected on VBUS. */
  NRF_EVT_POWER_USB_REMOVED,                    /**< Event indicating that voltage supply is removed from VBUS. */
  NRF_EVT_NUMBER_OF_EVTS
};

/**@} */


/**@addtogroup NRF_SOC_STRUCTURES Structures
 * @{ */

/**@brief Represents a mutex for use with the nrf_mutex functions.
 * @note Accessing the value directly is not safe, use the mutex functions!
 */
typedef volatile uint8_t nrf_mutex_t;

/**@brief Parameters for a request for a timeslot as early as possible. */
typedef struct
{
  uint8_t       hfclk;                              /**< High frequency clock source, see @ref NRF_RADIO_HFCLK_CFG. */
  uint8_t       priority;                           /**< The radio timeslot priority, see @ref NRF_RADIO_PRIORITY. */
  uint32_t      length_us;                          /**< The radio timeslot length (in the range 100 to 100,000] microseconds). */
  uint32_t      timeout_us;                         /**< Longest acceptable delay until the start of the requested timeslot (up to @ref NRF_RADIO_EARLIEST_TIMEOUT_MAX_US microseconds). */
} nrf_radio_request_earliest_t;

/**@brief Parameters for a normal radio timeslot request. */
typedef struct
{
  uint8_t       hfclk;                              /**< High frequency clock source, see @ref NRF_RADIO_HFCLK_CFG. */
  uint8_t       priority;                           /**< The radio timeslot priority, see @ref NRF_RADIO_PRIORITY. */
  uint32_t      distance_us;                        /**< Distance from the start of the previous radio timeslot (up to @ref NRF_RADIO_DISTANCE_MAX_US microseconds). */
  uint32_t      length_us;                          /**< The radio timeslot length (in the range [100..100,000] microseconds). */
} nrf_radio_request_normal_t;

/**@brief Radio timeslot request parameters. */
typedef struct
{
  uint8_t                         request_type;     /**< Type of request, see @ref NRF_RADIO_REQUEST_TYPE. */
  union
  {
    nrf_radio_request_earliest_t  earliest;         /**< Parameters for requesting a radio timeslot as early as possible. */
    nrf_radio_request_normal_t    normal;           /**< Parameters for requesting a normal radio timeslot. */
  } params;                                         /**< Parameter union. */
} nrf_radio_request_t;

/**@brief Return parameters of the radio timeslot signal callback. */
typedef struct
{
  uint8_t               callback_action;            /**< The action requested by the application when returning from the signal callback, see @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION. */
  union
  {
    struct
    {
      nrf_radio_request_t * p_next;                 /**< The request parameters for the next radio timeslot. */
    } request;                                      /**< Additional parameters for return_code @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END. */
    struct
    {
      uint32_t              length_us;              /**< Requested extension of the radio timeslot duration (microseconds) (for minimum time see @ref NRF_RADIO_MINIMUM_TIMESLOT_LENGTH_EXTENSION_TIME_US). */
    } extend;                                       /**< Additional parameters for return_code @ref NRF_RADIO_SIGNAL_CALLBACK_ACTION_EXTEND. */
  } params;                                         /**< Parameter union. */
} nrf_radio_signal_callback_return_param_t;

/**@brief The radio timeslot signal callback type.
 *
 * @note In case of invalid return parameters, the radio timeslot will automatically end
 *       immediately after returning from the signal callback and the
 *       @ref NRF_EVT_RADIO_SIGNAL_CALLBACK_INVALID_RETURN event will be sent.
 * @note The returned struct pointer must remain valid after the signal callback
 *       function returns. For instance, this means that it must not point to a stack variable.
 *
 * @param[in] signal_type Type of signal, see @ref NRF_RADIO_CALLBACK_SIGNAL_TYPE.
 *
 * @return Pointer to structure containing action requested by the application.
 */
typedef nrf_radio_signal_callback_return_param_t * (*nrf_radio_signal_callback_t) (uint8_t signal_type);

/**@brief Gets any pending events generated by the SoC API.
 *
 * The application should keep calling this function to get events, until ::NRF_ERROR_NOT_FOUND is returned.
 *
 * @param[out] p_evt_id Set to one of the values in @ref NRF_SOC_EVTS, if any events are pending.
 *
 * @retval ::NRF_SUCCESS An event was pending. The event id is written in the p_evt_id parameter.
 * @retval ::NRF_ERROR_NOT_FOUND No pending events.
 */
uint32_t sd_evt_get(uint32_t * p_evt_id);

/**@brief Opens a session for radio timeslot requests.
 *
 * @note Only one session can be open at a time.
 * @note p_radio_signal_callback(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START) will be called when the radio timeslot
 *       starts. From this point the NRF_RADIO and NRF_TIMER0 peripherals can be freely accessed
 *       by the application.
 * @note p_radio_signal_callback(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_TIMER0) is called whenever the NRF_TIMER0
 *       interrupt occurs.
 * @note p_radio_signal_callback(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_RADIO) is called whenever the NRF_RADIO
 *       interrupt occurs.
 * @note p_radio_signal_callback() will be called at ARM interrupt priority level 0. This
 *       implies that none of the sd_* API calls can be used from p_radio_signal_callback().
 *
 * @param[in] p_radio_signal_callback The signal callback.
 *
 * @retval ::NRF_ERROR_INVALID_ADDR p_radio_signal_callback is an invalid function pointer.
 * @retval ::NRF_ERROR_BUSY If session cannot be opened.
 * @retval ::NRF_ERROR_INTERNAL If a new session could not be opened due to an internal error.
 * @retval ::NRF_SUCCESS Otherwise.
 */
uint32_t sd_radio_session_open(nrf_radio_signal_callback_t p_radio_signal_callback);

/**@brief Closes a session for radio timeslot requests.
 *
 * @note Any current radio timeslot will be finished before the session is closed.
 * @note If a radio timeslot is scheduled when the session is closed, it will be canceled.
 * @note The application cannot consider the session closed until the @ref NRF_EVT_RADIO_SESSION_CLOSED
 *       event is received.
 *
 * @retval ::NRF_ERROR_FORBIDDEN If session not opened.
 * @retval ::NRF_ERROR_BUSY If session is currently being closed.
 * @retval ::NRF_SUCCESS Otherwise.
 */
uint32_t sd_radio_session_close(void);

/**@brief Requests a radio timeslot.
 *
 * @note The request type is determined by p_request->request_type, and can be one of @ref NRF_RADIO_REQ_TYPE_EARLIEST
 *       and @ref NRF_RADIO_REQ_TYPE_NORMAL. The first request in a session must always be of type @ref NRF_RADIO_REQ_TYPE_EARLIEST.
 * @note For a normal request (@ref NRF_RADIO_REQ_TYPE_NORMAL), the start time of a radio timeslot is specified by
 *       p_request->distance_us and is given relative to the start of the previous timeslot.
 * @note A too small p_request->distance_us will lead to a @ref NRF_EVT_RADIO_BLOCKED event.
 * @note Timeslots scheduled too close will lead to a @ref NRF_EVT_RADIO_BLOCKED event.
 * @note See the SoftDevice Specification for more on radio timeslot scheduling, distances and lengths.
 * @note If an opportunity for the first radio timeslot is not found before 100 ms after the call to this
 *       function, it is not scheduled, and instead a @ref NRF_EVT_RADIO_BLOCKED event is sent.
 *       The application may then try to schedule the first radio timeslot again.
 * @note Successful requests will result in nrf_radio_signal_callback_t(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START).
 *       Unsuccessful requests will result in a @ref NRF_EVT_RADIO_BLOCKED event, see @ref NRF_SOC_EVTS.
 * @note The jitter in the start time of the radio timeslots is +/- @ref NRF_RADIO_START_JITTER_US us.
 * @note The nrf_radio_signal_callback_t(@ref NRF_RADIO_CALLBACK_SIGNAL_TYPE_START) call has a latency relative to the
 *       specified radio timeslot start, but this does not affect the actual start time of the timeslot.
 * @note NRF_TIMER0 is reset at the start of the radio timeslot, and is clocked at 1MHz from the high frequency
 *       (16 MHz) clock source. If p_request->hfclk_force_xtal is true, the high frequency clock is
 *       guaranteed to be clocked from the external crystal.
 * @note The SoftDevice will neither access the NRF_RADIO peripheral nor the NRF_TIMER0 peripheral
 *       during the radio timeslot.
 *
 * @param[in] p_request Pointer to the request parameters.
 *
 * @retval ::NRF_ERROR_FORBIDDEN Either:
 *                                - The session is not open.
 *                                - The session is not IDLE.
 *                                - This is the first request and its type is not @ref NRF_RADIO_REQ_TYPE_EARLIEST.
 *                                - The request type was set to @ref NRF_RADIO_REQ_TYPE_NORMAL after a
 *                                  @ref NRF_RADIO_REQ_TYPE_EARLIEST request was blocked.
 * @retval ::NRF_ERROR_INVALID_ADDR If the p_request pointer is invalid.
 * @retval ::NRF_ERROR_INVALID_PARAM If the parameters of p_request are not valid.
 * @retval ::NRF_SUCCESS Otherwise.
 */
uint32_t sd_radio_request(nrf_radio_request_t const * p_request);

/**@} */

#ifdef __cplusplus
}
#endif
#endif // NRF_SOC_H__
