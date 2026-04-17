/*
 * Copyright (c) Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file mpsl_cx_protocol_api.h
 *
 * @defgroup mpsl_cx MPSL Protocol interface for radio coexistence interface.
 * @ingroup  mpsl
 *
 * This module provides the interface for the protocols to communicate with a coexistence
 * arbitrator, Packet Traffic Arbitrator, PTA.
 *
 * Concurrency model of this module:
 * Each function defined in this interface may be called from any execution context (thread,
 * including preemptive thread in operating system, or handler). However, none of the functions is
 * re-entrant. It is responsibility of API user to ensure that even if one function is preempted by
 * other execution context, the other execution context does not call preempted function.
 *
 * It is not allowed to call a function defined in this module while the same function is called
 * from another preempted context.
 * It is allowed to call any function defined in this module while any other is preempted.
 * It is allowed to call any function defined in this module from @ref mpsl_cx_cb_t
 * implementation.
 * Callback function @ref mpsl_cx_cb_t called from this module may be called from thread or
 * handler context. But it never preempts itself.
 * @{
 */

#ifndef MPSL_CX_PROTOCOL_H__
#define MPSL_CX_PROTOCOL_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_errno.h"

/**
 * @brief Radio operations which may be requested from or granted by PTA.
 */
enum mpsl_cx_op_t
{
  /** Listening for asynchronous transmissions */
  MPSL_CX_OP_IDLE_LISTEN = 0x01,

  /** Active reception
   *
   * This value may be used to indicate:
   * * RX window:
   *   * in BLE connection event
   *   * 802.15.4 CSL
   *   * ...
   * * Detected a frame transmitted asynchronously:
   *   * during BLE scanning
   *   * during 802.15.4 idle listening
   *   * ...
   */
  MPSL_CX_OP_RX          = 0x02,

  /** Active transmission */
  MPSL_CX_OP_TX          = 0x04,
};

/**
 * @brief Bitmap representing a list of radio operations.
 *
 * Logical OR of radio operations defined in @ref mpsl_cx_op_t.
 */
typedef uint8_t mpsl_cx_op_map_t;

/**
 * @brief Priority of given radio operation.
 *
 * This numerical value is translated by given PTA interface driver to appropriate signal.
 * The straightforward translation would be <= UINT8_MAX/2 is low priority, while > UINT8_MAX/2
 * is high priority. More complicated PTAs support more than 2 priority levels and valid matching
 * must be implemented for such devices.
 *
 * Priority values must be aligned between all users of this API in given system. When multiple
 * radio protocols are enabled (like Bluetooth LE and IEEE 802.15.4), all protocols must use
 * aligned values (Bluetooth low priority operations must use numerically lower value than
 * IEEE 802.15.4 high priority operations).
 */
typedef uint8_t mpsl_cx_prio_t;

/**
 * @brief Coexistence request signal triggers.
 */
typedef enum
{
    MPSL_CX_REQ_TRIG_INSTANT,  ///< Request is triggered instantly, during @ref mpsl_cx_request call
} mpsl_cx_req_trig_t;

/**
 * @brief Structure with parameters describing next coexistence request.
 */
typedef struct
{
    mpsl_cx_op_map_t   ops;     ///< List of requested radio operations
    mpsl_cx_prio_t     prio;    ///< Priority of requested radio operation
    mpsl_cx_req_trig_t trigger; ///< When request is to be triggered
} mpsl_cx_request_t;

/**
 * @brief Callback used to notify radio protocol about changes of granted radio operations.
 *
 * @param[in]  granted_ops  List of all operations currently allowed by PTA.
 */
typedef void (*mpsl_cx_cb_t)(mpsl_cx_op_map_t granted_ops);

/**
 * @brief Request radio operation from PTA
 *
 * This function is to be called before transceiver is enabled to inform PTA about planned radio
 * operation which is about to start and about the priority of this operation.
 *
 * Each call of this function overrides the last request.
 *
 * This function is usually called shortly before scheduled radio operations, or just after
 * detection of asynchronous events indicating radio operations.
 *
 * Call to this function cannot preempt a call to @ref mpsl_cx_release. It is responsibility of
 * user of this interface to prevent such preemption.
 *
 * @param[in] p_req_params  Pointer to a structure containing parameters of given request.
 *
 * @retval 0         Requested radio operation from PTA
 * @retval -EINVAL   Incorrect value of any field in @p p_req_params or @p p_req_params is NULL.
 */
int32_t mpsl_cx_request(const mpsl_cx_request_t * p_req_params);

/**
 * @brief Release last radio operation requested from PTA
 *
 * This function is to be called after transceiver is disabled to inform PTA that any previously
 * requested operations by @ref mpsl_cx_request are not requested anymore.
 *
 * This function releases requested radio operations immediately.
 *
 * Call to this function cannot preempt a call to @ref mpsl_cx_request. It is responsibility of
 * user of this interface to prevent such preemption.
 *
 * @retval 0          Last request is released.
 * @retval -EALREADY  There was no request to release.
 */
int32_t mpsl_cx_release(void);

/**
 * @brief Get list of radio operations currently granted by PTA.
 *
 * @note PTA may change granted radio operations list at any time. It is possible that value
 *       returned by this function is outdated before the function ends its execution.
 *
 * @param[out] p_granted_ops  Map of granted radio operations.
 *
 * @retval 0        List of granted radio operations is stored in the passed memory address.
 * @retval -EINVAL  @p granted_ops is NULL pointer.
 */
int32_t mpsl_cx_granted_ops_get(mpsl_cx_op_map_t * p_granted_ops);

/**
 * @brief Get time PTA needs to respond with grant signal to request signal.
 *
 * There are three ways to manage the grant signal by PTA:
 * 1. Keep grant signal in denied state by default and provide grant only after request.
 *    The maximum delay between request and grant signal is known.
 * 2. Keep grant signal in approved state by default and deny only if given request cannot be
 *    accepted. Radio is not allowed to start any operation for known time after the request signal
 *    is set. After this delay the grant signal can indicate denial anytime to prevent or to abort
 *    radio operation.
 * 3. Manage the granted signal constantly, even if there is no request from given radio.
 *
 * This function is to be used to verify how long the user of this interface should wait from
 * calling @ref mpsl_cx_request (return from function) to @ref mpsl_cx_granted_ops_get (call
 * to function) to prevent getting false results while PTA is responding to the last request.
 *
 * @retval >0 Minimal delay from setting the request signal to the time grant signal gets
 *            meaningful value. In microseconds, rounded up
 * @retval 0  Grant signal has always meaningful value. Can be checked before, during, or after
 *            setting the request signal.
 */
uint32_t mpsl_cx_req_grant_delay_get(void);

/**
 * @brief Register callback function called when radio operations granted by PTA change.
 *
 * This function is used by radio protocols that require CPU processing when list of granted
 * operations changes. One example is a protocol waiting with asynchronous transmission until TX
 * operation is granted.
 *
 * When radio protocol stops using Coexistence interface to hand it over to another protocol,
 * it shall reset callback by calling this function with NULL value passed in @p cb.
 *
 * @param[in] cb  Pointer to a callback function called when radio operations granted by PTA
 *                change. If NULL, callback function is reset and nothing is called when
 *                granted radio operations change.
 *
 * @retval 0  Successfully set or reset callback function.
 */
int32_t mpsl_cx_register_callback(mpsl_cx_cb_t cb);

#endif // MPSL_CX_PROTOCOL_H__

/**
  @}
*/
