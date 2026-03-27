/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_radio_notification.h
 *
 * @defgroup mpsl_radio_notification MPSL Radio Notification interface
 * @ingroup  mpsl
 *
 * APIs for configuring radio event notifications in MPSL.
 * @{
 */

#ifndef MPSL_RADIO_NOTIFICATION_H__
#define MPSL_RADIO_NOTIFICATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "nrf_errno.h"
#include "nrfx.h"

#define MPSL_RADIO_NOTIFICATION_DISTANCE_NONE 0
#if defined(NRF52_SERIES) || defined(NRF53_SERIES)
#define MPSL_RADIO_NOTIFICATION_DISTANCE_MIN_US 200
#else
#define MPSL_RADIO_NOTIFICATION_DISTANCE_MIN_US 50
#endif

#define MPSL_RADIO_NOTIFICATION_DISTANCE_MAX_US 5500


/** @brief Radio notification types. */
enum MPSL_RADIO_NOTIFICATION_TYPES
{
    MPSL_RADIO_NOTIFICATION_TYPE_NONE = 0,        /**< The event does not have a radio notification signal. */
    MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_ACTIVE,   /**< Using interrupt for notification when the radio will be enabled. */
    MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE, /**< Using interrupt for notification when the radio has been disabled. */
    MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_BOTH,     /**< Using interrupt for notification both when the radio will be enabled and disabled. */
};


/** @brief Radio notification callback types. */
typedef enum
{
    MPSL_RADIO_NOTIFICATION_SOURCE_ACTIVE,   /**< Indicates that the callback is given for the active notification. */
    MPSL_RADIO_NOTIFICATION_SOURCE_INACTIVE, /**< Indicates that the callback is given for the inactive notification */
} mpsl_radio_notification_source_t;

/** @brief Function prototype for the radio notification callback.
 *
 * @note This callback is called from high priority interrupt context.
 *       The callback should perform minimal processing and return quickly.
 *       Heavy processing should be deferred to lower priority context.
 */
typedef void (*mpsl_radio_notification_cb_t)(mpsl_radio_notification_source_t notification_source);

/** @brief Configures the Radio Notification signal.
 *
 * @note
 *      - The notification signal is given immediately before or after radio activity as a callback.
 *      - To ensure that the radio notification signal behaves in a consistent way, the radio
 *        notifications must be configured when there is no protocol stack enabled.
 *        It is recommended that the radio notification signal is
 *        configured directly after the MPSL is enabled.
 *
 * @param[in]  type         Type of notification signal, see @ref MPSL_RADIO_NOTIFICATION_TYPES.
 *                          @ref MPSL_RADIO_NOTIFICATION_TYPE_NONE shall be used to turn off radio
 *                          notification. Using @ref MPSL_RADIO_NOTIFICATION_DISTANCE_NONE is
 *                          recommended (but not required) to be used with
 *                          @ref MPSL_RADIO_NOTIFICATION_TYPE_NONE.
 *
 * @param[in]  distance_us  Distance between the notification signal and start of radio activity.
 *                          This parameter is ignored when @ref MPSL_RADIO_NOTIFICATION_TYPE_NONE or
 *                          @ref MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE is used.
 *                          For other notification types, the value of this parameter must be between
 *                          @ref MPSL_RADIO_NOTIFICATION_DISTANCE_MIN_US and
 *                          @ref MPSL_RADIO_NOTIFICATION_DISTANCE_MAX_US.
 *
 * @param[in]  cb           Callback for the notification.
 *                          This is called from priority level 0. The application is responsible for
 *                          spending as little time in this callback as possible.
 *                          Therefore the recommended usage for this callback is to pend interrupt(s) with a lower priority.
 *
 * @retval -NRF_EINVAL       Invalid parameters supplied.
 * @retval -NRF_EINPROGRESS  A protocol stack or other SoftDevice is running. Stop all
 *                           running activities and retry.
 * @retval 0  Success
 */
int32_t mpsl_radio_notification_cfg_set(uint8_t type, uint16_t distance_us, mpsl_radio_notification_cb_t cb);

#ifdef __cplusplus
}
#endif

#endif // MPSL_RADIO_NOTIFICATION_H__

/**@} */
