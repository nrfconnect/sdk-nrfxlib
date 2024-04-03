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
  MPSL_PM_EVENT_STATE_IN_EVENT = 0,
  MPSL_PM_EVENT_STATE_BEFORE_EVENT,
  MPSL_PM_EVENT_STATE_NO_EVENTS_LEFT
} mpsl_pm_event_state_t;

/** @brief MPSL Power Management parameters.
 *
 *  These parameters should be used in a low-priority context out of MPSL.
 *  They are used to inform an external power management system about latency requirements and next expected event.
*/
typedef struct
{
  uint64_t              event_time_rel_us;              ///< Relative time to next event. Only valid during MPSL_PM_EVENT_STATE_BEFORE_EVENT.
  mpsl_pm_event_state_t event_state;                    ///< State of event @ref mpsl_pm_event_state_t.
  uint8_t               cnt_flag;                       ///< Used both to inform low priority that it has been preempted and to keep track of PM parameter updates from event to event.
} mpsl_pm_params_t;

 /** @brief Get latency requirements and next expected event time.
 *
 *  @param[out] p_params               Pointer to event parameters
 *
 *  @retval true if high-prio didn't update the parameters while we read them
 */
bool mpsl_pm_params_get(mpsl_pm_params_t * p_params);

#ifdef __cplusplus
}
#endif

#endif // MPSL_PM_H__

/**
  @}
*/
