/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_pm.h
 *
 * @defgroup mpsl_pm MPSL API for configuration of a System Power Manager external to the MPSL.
 * @ingroup  mpsl
 *
 * @{
 */

#ifndef MPSL_PM_H__
#define MPSL_PM_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MPSL_PM_EVENT_TIME_INVALID UINT64_MAX

/** @brief MPSL Power Management state in regards to next event. */
typedef enum
{
  MPSL_PM_EVENT_STATE_IN_EVENT = 0, // TODO: DRGN-23673: Remove this when sdk-nrf is updated to use new low latency API.
  MPSL_PM_EVENT_STATE_BEFORE_EVENT,
  MPSL_PM_EVENT_STATE_NO_EVENTS_LEFT
} mpsl_pm_event_state_t;

/** @brief MPSL Power Management parameters.
 *
 *  These parameters should be used in a low-priority context out of MPSL.
 *  They are used to inform an external power management system about next expected event.
*/
typedef struct
{
  uint64_t              event_time_abs_us;              ///< Absolute time of next event. Only valid during MPSL_PM_EVENT_STATE_BEFORE_EVENT.
  mpsl_pm_event_state_t event_state;                    ///< State of event @ref mpsl_pm_event_state_t.
  uint8_t               cnt_flag;                       ///< Event time update request counter.
} mpsl_pm_params_t;

 /** @brief Get next expected event time.
 *
 *  @param[out] p_params               Pointer to event parameters
 *
 *  @retval true if high-prio didn't update the parameters while we read them
 */
bool mpsl_pm_params_get(mpsl_pm_params_t * p_params);

/** @brief Acknowledge event registration.
 *
 * When low-priority context has registered a new event time with the external power management system,
 * based on @ref mpsl_pm_params_get, this function shall be called to acknowledge registration.
 *
 * @param[in] req_counter Counter value corresponding to the event time registered.
 */
void mpsl_pm_event_registered_ack(uint8_t req_counter);

/** @brief MPSL Power Management low latency state.
 *
 * The different states of low latency requests. The external power management system shall use
 * @ref mpsl_pm_low_latency_state_set to tell MPSL whether or not low latency mode is active.
 * For transitions that are not instant, the corresponding
 * REQUESTING and RELEASING states shall be used.
 *
 * When low latency state is either OFF or ON, the external power management system shall call
 * @ref mpsl_pm_low_latency_requested and initiate transition to the appropriate state. MPSL
 * will only request low latency in the OFF state, and only release low latency in the ON state.
 */
typedef enum
{
  MPSL_PM_LOW_LATENCY_STATE_OFF = 0,
  MPSL_PM_LOW_LATENCY_STATE_REQUESTING,
  MPSL_PM_LOW_LATENCY_STATE_ON,
  MPSL_PM_LOW_LATENCY_STATE_RELEASING
} mpsl_pm_low_latency_state_t;

/** @brief Get low latency request status.
 *
 *  @retval true if low latency requested by MPSL, false otherwise.
 */
bool mpsl_pm_low_latency_requested(void);

/** @brief Set low latency state.
 *
 * When the external power management system initiates transitioning to and from low
 * latency state, it shall call this function to notify MPSL about the state changes.
 *
 *  @param[in] state  The current low latency state.
 */
void mpsl_pm_low_latency_state_set(mpsl_pm_low_latency_state_t state);

/** @brief Get low latency state.
 *
 *  @retval The current state of low latency mode.
 */
mpsl_pm_low_latency_state_t mpsl_pm_low_latency_state_get(void);

#ifdef __cplusplus
}
#endif

#endif // MPSL_PM_H__

/**
  @}
*/
