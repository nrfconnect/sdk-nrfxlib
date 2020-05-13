/*
 * Copyright (c) 2013 - 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

/**
 * @file mpsl_radio_notification.h
 *
 * @defgroup mpsl_radio_notification MPSL Radio notification interface
 * @ingroup  mpsl
 *
 * The MPSL radio interface provides APIs for configuring radio notifications.
 * When radio notifications are enabled, the application is notified when an
 * MPSL timeslot starts or ends. By using radio notifications, the application is
 * informed when a protocol uses the radio.
 *
 * @{
 */

#ifndef MPSL_RADIO_NOTIFICATION_H__
#define MPSL_RADIO_NOTIFICATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "nrf_errno.h"

/** @brief Guaranteed time for application to process radio inactive notification. */
#define MPSL_RADIO_NOTIFICATION_INACTIVE_GUARANTEED_TIME_US  (62)

/** @brief Radio notification distances. */
enum MPSL_RADIO_NOTIFICATION_DISTANCES
{
  MPSL_RADIO_NOTIFICATION_DISTANCE_NONE = 0, /**< The event does not have a notification. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_200US,    /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_420US,    /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_800US,    /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_1740US,   /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_2680US,   /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_3620US,   /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_4560US,   /**< The distance from the active notification to start of radio activity. */
  MPSL_RADIO_NOTIFICATION_DISTANCE_5500US    /**< The distance from the active notification to start of radio activity. */
};

/** @brief Radio notification types. */
enum MPSL_RADIO_NOTIFICATION_TYPES
{
    MPSL_RADIO_NOTIFICATION_TYPE_NONE = 0,        /**< The event does not have a radio notification signal. */
    MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_ACTIVE,   /**< Using interrupt for notification when the radio will be enabled. */
    MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE, /**< Using interrupt for notification when the radio has been disabled. */
    MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_BOTH,     /**< Using interrupt for notification both when the radio will be enabled and disabled. */
};

/** @brief Configures the Radio Notification signal.
 *
 * @note
 *      - The notification signal latency depends on the interrupt priority settings of IRQ used
 *        for notification signal.
 *      - To ensure that the radio notification signal behaves in a consistent way, the radio
 *        notifications must be configured when there is no protocol stack enabled.
 *        It is recommended that the radio notification signal is
 *        configured directly after the MPSL is enabled.
 *
 * @param[in]  type      Type of notification signal, see @ref MPSL_RADIO_NOTIFICATION_TYPES.
 *                       @ref MPSL_RADIO_NOTIFICATION_TYPE_NONE shall be used to turn off radio
 *                       notification. Using @ref MPSL_RADIO_NOTIFICATION_DISTANCE_NONE is
 *                       recommended (but not required) to be used with
 *                       @ref MPSL_RADIO_NOTIFICATION_TYPE_NONE.
 *
 * @param[in]  distance  Distance between the notification signal and start of radio activity,
 *                       see @ref MPSL_RADIO_NOTIFICATION_DISTANCES.
 *                       This parameter is ignored when @ref MPSL_RADIO_NOTIFICATION_TYPE_NONE or
 *                       @ref MPSL_RADIO_NOTIFICATION_TYPE_INT_ON_INACTIVE is used.
 *
 * @param[in]  irq       IRQ to pend as the radio notification signal, e.g SWI1_IRQn.
 *                       If the application uses IRQs that are already in use, e.g, RADIO_IRQn,
 *                       TIMER0_IQRn or similar, the behavior is undefined.
 *                       The interrupt priority level should be lower than priority level 0, that is,
 *                       a higher numerical priority value. The application is responsible for
 *                       setting the interrupt priority.
 *
 *
 * @retval -NRF_EINVAL    Invalid parameters supplied.
 * @retval -NRF_EAGAIN    A protocol stack or other SoftDevice is running. Stop all
 *                        running activities and retry.
 * @retval 0  Success
 */
int32_t mpsl_radio_notification_cfg_set(uint8_t type, uint8_t distance, IRQn_Type irq);

#ifdef __cplusplus
}
#endif

#endif // MPSL_RADIO_NOTIFICATION_H__

/**@} */
